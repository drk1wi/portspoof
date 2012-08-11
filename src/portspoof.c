/*
 *   portspoof       Service signature obfucastor 
 *   Copyright (C) 12012 Piotr Duszyński <piotr[at]duszynski.eu>
 *
 *   This program is free software; you can redistribute it and/or modify it
 *   under the terms of the GNU General Public License as published by the
 *   Free Software Foundation; either version 2 of the License, or (at your
 *   option) any later version.
 * 
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *   See the GNU General Public License for more details.
 * 
 *   You should have received a copy of the GNU General Public License along
 *   with this program; if not, see <http://www.gnu.org/licenses>.
 * 
 *   Linking portspoof statically or dynamically with other modules is making
 *   a combined work based on portspoof. Thus, the terms and conditions of
 *   the GNU General Public License cover the whole combination.
 * 
 *   In addition, as a special exception, the copyright holder of portspoof
 *   gives you permission to combine portspoof with free software programs or
 *   libraries that are released under the GNU LGPL. You may copy
 *   and distribute such a system following the terms of the GNU GPL for
 *   portspoof and the licenses of the other code concerned.
 * 
 *   Note that people who make modified versions of portspoof are not obligated
 *   to grant this special exception for their modified versions; it is their
 *   choice whether to do so. The GNU General Public License gives permission
 *   to release a modified version without this exception; this exception
 *   also makes it possible to release a modified version which carries
 *   forward this exception.
 */

//TODO  headers - to be clean up

#include "config.h"
#include <sys/types.h>
#include <pthread.h>
#include <sys/timeb.h>
#include <sys/wait.h>

#ifdef OPENBSD

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netinet/in_systm.h>
#include <netinet/in_var.h>
#include <netinet/ip.h>
#include <netinet/ip_var.h>
#include <net/pfvar.h>
#include <net/if_pfsync.h>
#include <net/if.h>
#include <net/pfvar.h>
#include <arpa/inet.h>
#include <altq/altq.h>

#endif

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "portspoof.h"
#include "revregex.h"
#include "threads.h"
#include "connection.h"
#include "log.h"

#define CONFSEPARATOR "/"

char opts=0;
char *log_file="portspoof.log";

pthread_cond_t new_connection_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t new_connection_mutex = PTHREAD_MUTEX_INITIALIZER;

extern char *__progname;

void 
usage(void)
{
	printf("Usage: portspoof [OPTION]...\n"
	"Portspoof - service signature obfuscator.\n\n"
	  "-i             bind to a user defined ip address\n"
	  "-p			  bind to a user defined port number\n"
	  "-f			  use user defined signture file\n"
	  "-l			  log port scanning alerts to a file\n"
	  "-t			  number of threads\n"
	  "-c			  length of client queue per thread\n"
	  "-v			  be verbose\n"
	  "-h			  display this help and exit\n\n"
	"Without any OPTION - use default values and continue");
	
	exit(1);
}

int choose_thread()
{
	int i=MAX_THREADS-1;
	int min = i;
	while(i >=0)
	{
		if(threads[i].client_count < threads[min].client_count)
		{
			min = i;
		}
		i--;
	}		

	if(threads[min].client_count==MAX_CLIENT_PER_THREAD)
		return -1;
	
	return min;
}

int main(int argc, char **argv)
{
	int	ch;
	char* bind_ip;
	char* signature_file="signatures";
	char* configuration_file="portspoof.conf";
	int sockd,newsockfd;
	int addrlen;
	int pid;
	struct sockaddr_in my_name, peer_name;
	int status;
	char buf_file[BUFSIZE];
	int num_lines = 0;
	int buf_size=0;
	unsigned short int port=DEFAULT_PORT;
	
	#ifdef CONFDIR
		configuration_file = CONFDIR CONFSEPARATOR "portspoof.conf";
		signature_file = CONFDIR CONFSEPARATOR "signatures";
	#endif
	
	
	
	while ((ch = getopt(argc, argv,"l:i:p:f:t:c:dh")) != -1) {
		switch (ch) {
		case 'i':
			bind_ip = optarg;
			opts |= OPT_IP;
			break;
		case 'p':
			port  = atoi(optarg);
			opts |= OPT_PORT;
			break;
		case 'f':
			signature_file  = optarg;
			opts |= OPT_SIG_FILE;
			break;
		case 'd':
			opts |= OPT_DEBUG;
			printf("-> Verbose mode on.\n");
			break;
		case 'l':
			opts |= OPT_LOG_FILE;
			log_file  = optarg;
			printf("-> Using log file %s\n",log_file);
			break;
		case 't':
			printf("-> Threading options to be implemented.\n");
			break;
		case 'c':
			printf("-> Threading options to be implemented.\n");
			break;
		case 'h':
			usage();
			break;
		default:
			printf("Try ` %s -h' for more information.\n\n", __progname);
			exit(0);
			break;
			}
	}
	

	if( !(opts&OPT_IP || opts&OPT_PORT || opts&OPT_SIG_FILE))
	{
		printf("-> No parameters - using default values.\n");
	}

	//check log file
	if(opts & OPT_LOG_FILE)
		log_create(log_file);

	// open file
	
	if(opts & OPT_SIG_FILE)
	printf("-> Using user defined signature file %s\n",signature_file);
	
	FILE *fp = fopen(signature_file, "r");
	if (fp == NULL) {
	    printf("Error opening file: %s \n",signature_file);
	    return -1;
	}
	
	// get number of lines
	while (fgets(buf_file, BUFSIZE, fp))
	    if (!(strlen(buf_file) == BUFSIZE-1 && buf_file[BUFSIZE-2] != '\n'))
			{
				num_lines++;
			}
						
	// allocate memory
 	arr_lines2 = malloc(num_lines * sizeof(struct signature*));

	int i=0;	
	for(;i<num_lines;i++)
		{
			struct signature* tmp;
			tmp=malloc(sizeof(signature));
			arr_lines2[i]=tmp; 
		}
	
	// read lines
	rewind(fp);
	
	num_lines = 0;
	int line_length,len;
	char* tmp;
	
	while (fgets(buf_file, BUFSIZE, fp))
	    if (!(strlen(buf_file) == BUFSIZE-1 && buf_file[BUFSIZE-2] != '\n'))
			{
				line_length=strlen(buf_file);
				tmp=malloc(line_length+1);
				memset(tmp,0,line_length+1);
				strncpy(tmp,buf_file,line_length-1); //without new line
				
				//DEBUG
				//printf("line nr %d\n",num_lines);
				//printf("nr of line: %d line len %d",num_lines,line_length);				
				//printf("%d",line_length);				
				
				((signature*)(arr_lines2[num_lines]))->cptr=process_signature(tmp,&len);
				((signature*)(arr_lines2[num_lines]))->len=len;
				num_lines++;
				
			}
	
	fclose(fp);
	
	i=0;	
	int tmpi=0;
	srand(time(0));
	// set port spoof mapping
	for(;i<SIGNATURES_SIZE;i++)
		{
			signatures[i]=i%num_lines; //rand()%num_lines; //TODO different port mapping schemes
		}


 	/* create thread pool */
	for(i = 0; i < MAX_THREADS; i++)
	{
		pthread_create(&threads[i].tid, NULL, process_connection, (void *) i);
		threads[i].client_count = 0;
	}
		
	/* create a socket */
	sockd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockd == -1)
	{
	perror("Socket creation error");
	exit(1);
	}

	 int n = 1;
	 setsockopt(sockd, SOL_SOCKET, SO_REUSEADDR , &n, sizeof(n));

	/* server address  - by default localhost */
	  my_name.sin_family = PF_INET;
	  if(opts & OPT_IP)
	   {
			printf("-> Binding to iface: %s\n",bind_ip);
			inet_aton(bind_ip, &my_name.sin_addr.s_addr);
		 
		}
	  else
		my_name.sin_addr.s_addr = INADDR_ANY; // inet_addr("127.0.0.1");
	  
	  if(opts & OPT_PORT)
		{
			printf("-> Binding to port: %d\n",port);
			my_name.sin_port = htons(port);
			
		}
	  else 
	  my_name.sin_port = htons(DEFAULT_PORT);

	  status = bind(sockd, (struct sockaddr*)&my_name, sizeof(my_name));
	  if (status == -1)
	  {
	    perror("Binding error");
	    exit(1);
	  }

	  // Set queue sizeof
	  status = listen(sockd, 10);
	  if (status == -1)
	  {
	    perror("Listen set error");
	    exit(1);
	  }
	  int choosen;
	  while(1)
	  { 
	    /* wait for a connection */
	    addrlen = sizeof(peer_name);
	    newsockfd = accept(sockd, (struct sockaddr*)&peer_name, &addrlen);

        if (newsockfd < 0)
	    perror("ERROR on accept");
	    else{

	    nonblock(newsockfd); 
	
			start:
            pthread_mutex_lock(&new_connection_mutex);
			choosen=choose_thread();


	    	if( choosen == -1)
				{
					pthread_mutex_unlock(&new_connection_mutex);
					sleep(1);
					goto start;
				}
						

			if(opts & OPT_DEBUG)
            fprintf(stderr," new conn - thread choosen: %d -  nr. of connections already in queue: %d\n",choosen,threads[choosen].client_count);

			for(i = 0; i < MAX_CLIENT_PER_THREAD; i++)
			{
				if(threads[choosen].clients[i] == 0)
				{
					threads[choosen].clients[i] = newsockfd;
					threads[choosen].client_count++;
					break;
				}
			}
	     pthread_mutex_unlock(&new_connection_mutex);
	
	    }
	  }

	return 0;
}
