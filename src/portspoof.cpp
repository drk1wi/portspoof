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


#include <sys/types.h>
#include <pthread.h>
#include <sys/timeb.h>
#include <sys/wait.h>
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

#include "revregex.h"
#include "threads.h"
#include "connection.h"
#include "log.h"

Configuration* configuration;

pthread_cond_t new_connection_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t new_connection_mutex = PTHREAD_MUTEX_INITIALIZER;


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

	int sockd,newsockfd;
	int addrlen;
	//int pid;
	struct sockaddr_in my_name, peer_name;
	int status;

	
	configuration = new Configuration();
		
	if(configuration->processArgs(argc,argv))
		exit(1);
		
	if(configuration->processSignatureFile())
		exit(1);
		
	if(configuration->readConfigFile())
		exit(1);
	
	/*	
	if(configuration->getConfigValue(OPT_FUZZ_NMAP))
	{
		if(configuration->PrepareFuzzer())
		{
		fprintf(stdout,"Fuzzer prepare failed!\n");
		fflush(stdout);
		exit(1);
		}
		
		fprintf(stdout,"-> Preparing fuzzer!\n");
		
	}
	*/
	

	//check log file
	if(configuration->getConfigValue(OPT_LOG_FILE))
		log_create(configuration->getLogFile().c_str());

	// open file	
	if(configuration->getConfigValue(OPT_SIG_FILE))
	fprintf(stdout,"-> Using user defined signature file %s\n",configuration->getSignatureFile().c_str());
	fflush(stdout);
	
 	/* create thread pool */
	for(int i = 0; i < MAX_THREADS; i++)
	{
		pthread_create(&threads[i].tid, NULL, &process_connection, (void *) i);
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
	  if(configuration->getConfigValue(OPT_IP))
	   {
			fprintf(stdout,"-> Binding to iface: %s\n",configuration->getBindIP().c_str());
			inet_aton(configuration->getBindIP().c_str(), &my_name.sin_addr);
		 
		}
	  else
		my_name.sin_addr.s_addr = INADDR_ANY; 
	  
	  if(configuration->getConfigValue(OPT_PORT))
		{
			fprintf(stdout,"-> Binding to port: %d\n",configuration->getPort());
			my_name.sin_port = htons(configuration->getPort());
			
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
	    newsockfd = accept(sockd, (struct sockaddr*)&peer_name,(socklen_t*) &addrlen);
		
		
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
						

			if(configuration->getConfigValue(OPT_DEBUG))
            fprintf(stdout," new conn - thread choosen: %d -  nr. of connections already in queue: %d\n",choosen,threads[choosen].client_count);
			fflush(stdout);
			
			for(int i = 0; i < MAX_CLIENT_PER_THREAD; i++)
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
