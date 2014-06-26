/*
 *   Portspoof  - Service Signature Emulator  / Exploitation Framework Frontend   
 *   Copyright (C) 2012 Piotr Duszyński <piotr[at]duszynski.eu>
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
 *   a combined work based on Portspoof. Thus, the terms and conditions of
 *   the GNU General Public License cover the whole combination.
 * 
 *   In addition, as a special exception, the copyright holder of Portspoof
 *   gives you permission to combine Portspoof with free software programs or
 *   libraries that are released under the GNU LGPL. You may copy
 *   and distribute such a system following the terms of the GNU GPL for
 *   Portspoof and the licenses of the other code concerned.
 * 
 *   Note that people who make modified versions of Portspoof are not obligated
 *   to grant this special exception for their modified versions; it is their
 *   choice whether to do so. The GNU General Public License gives permission
 *   to release a modified version without this exception; this exception
 *   also makes it possible to release a modified version which carries
 *   forward this exception.
 */

#include <pthread.h>
#include <sys/param.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <time.h> 
#include "Threads.h"
#include "connection.h"
#include "Configuration.h"

/*
ipstr has to be of length INET_ADDRSTRLEN
or INET6_ADDRSTRLEN
*/
int get_ipstr(int fd, char *ipstr)
{
  socklen_t len;
  struct sockaddr_storage addr;

  len = sizeof(struct sockaddr_storage);
  getpeername(fd, (struct sockaddr *)&addr, &len);

  if (addr.ss_family == AF_INET)
  {
    struct sockaddr_in *s = (struct sockaddr_in *)&addr;
    inet_ntop(AF_INET, &s->sin_addr, ipstr, INET_ADDRSTRLEN);
  }
  else
  { // AF_INET6
    struct sockaddr_in6 *s = (struct sockaddr_in6 *)&addr;
    inet_ntop(AF_INET6, &s->sin6_addr, ipstr, INET6_ADDRSTRLEN);
  }
  return 1;
}

void nonblock(int sockfd)
{
	int opts;
	opts = fcntl(sockfd, F_GETFL);
	if(opts < 0)
	{
		perror("fcntl(F_GETFL)\n");
		exit(1);
	}
	opts = (opts|O_NONBLOCK );
	if(fcntl(sockfd, F_SETFL, opts) < 0) 
	{
		perror("fcntl(F_SETFL)\n");
		exit(1);
	}

}

void* process_connection(void *arg)
{
	int tid =  *((int*)(&arg));
	string str;
	char buffer[MAX_BUFFER_SIZE];
	unsigned int buffer_size=0;
	int original_port=DEFAULT_PORT;
	int n = 0;
	time_t timestamp;
	struct sockaddr_in peer_sockaddr;
	int peer_sockaddr_len=sizeof(struct sockaddr_in);
	char* msg;
    char ipstr[INET6_ADDRSTRLEN];
	memset(ipstr, '\0', INET6_ADDRSTRLEN);
    fd_set read_mask;
	struct timeval tv;
	int select_return;
	tv.tv_sec = 1;
	tv.tv_usec = 0;

	while(1) {
		
		sleep(1);
		for(int i = 0; i < MAX_CLIENT_PER_THREAD; i++)
		{
				
			if(threads[tid].clients[i] != 0)
			{
		     
				timestamp = time(NULL); 
				
				if(configuration->getConfigValue(OPT_NOT_NMAP_SCANNER))
					n = 1; // just reply...
				else
				{
					nonblock(threads[tid].clients[i]);
					FD_ZERO(&read_mask);
					FD_SET(threads[tid].clients[i], &read_mask);
  				    
  				    select_return = select(threads[tid].clients[i], &read_mask, (fd_set *)0, (fd_set *)0, &tv);

					if(select_return <= 0) /* [timeout=0, -1= ERROR] is returned */
					{
						n=1;
					}
					else
					{
						buffer_size=configuration->mapPort2Buffer(original_port);
						n = recv(threads[tid].clients[i],buffer,buffer_size, 0);	
					}
				}
			
				// deal with different recv buffer size
				if(n == 0){
				
					#ifdef OSX
							original_port = ntohs(peer_sockaddr.sin_port);
					#else
					
					if ( getsockopt (threads[tid].clients[i], SOL_IP, SO_ORIGINAL_DST, (struct sockaddr*)&peer_sockaddr,(socklen_t*) (socklen_t*) &peer_sockaddr_len )){
								perror("Getsockopt failed: Have you set up your IPTABLES rules correctly ?");
								goto close_socket;
							}
					else
            		original_port = ntohs(peer_sockaddr.sin_port);
					get_ipstr(threads[tid].clients[i], ipstr);

					#endif
				
					//LOG
					msg=(char*)malloc(MAX_LOG_MSG_LEN);
					memset(msg,0,MAX_LOG_MSG_LEN);
					snprintf(msg,MAX_LOG_MSG_LEN,"%d # Port_probe # REMOVING_SOCKET # source_ip:%s # dst_port:%d  \n",(int)timestamp,ipstr,original_port);//" port:%d src_ip%s\n", original_port,;
					Utils::log_write(configuration,msg);
					free(msg);
					//
				
					close_socket:
					if(configuration->getConfigValue(OPT_DEBUG))
					fprintf(stdout,"Thread nr. %d : client %d closed connection\n",tid, threads[tid].clients[i]);
				
					//shutdown(threads[tid].clients[i],SHUT_WR);
	        		close(threads[tid].clients[i]);
				
					pthread_mutex_lock(&new_connection_mutex);
					threads[tid].clients[i] = 0;
					threads[tid].client_count--;
					pthread_mutex_unlock(&new_connection_mutex);
				
				}
				else if(n < 0){

						
					if(errno == EAGAIN)
					{
						continue; // Nmap NULL probe (no data) -> skip && go to another socket (client)
					}
					else if(errno == 104) // Client terminted connection -> get rid of the socket now!
					{}
					else
					fprintf(stdout,"errno: %d\n", errno);    
					
					#ifdef OSX
					

						original_port = ntohs(peer_sockaddr.sin_port);
					
					#else
					
					if ( getsockopt (threads[tid].clients[i], SOL_IP, SO_ORIGINAL_DST, (struct sockaddr*)&peer_sockaddr,(socklen_t*)  &peer_sockaddr_len )){
								perror("Getsockopt failed");
								goto close_socket2;
							}
					else
            		original_port = ntohs(peer_sockaddr.sin_port);
					get_ipstr(threads[tid].clients[i], ipstr);

					#endif
				
					//LOG
					msg =(char*)malloc(MAX_LOG_MSG_LEN);
					memset(msg,0,MAX_LOG_MSG_LEN);
					snprintf(msg,MAX_LOG_MSG_LEN,"%d # Port_probe # REMOVING_SOCKET # source_ip:%s # dst_port:%d  \n",(int)timestamp,ipstr,original_port);//" port:%d src_ip%s\n", original_port,;
					Utils::log_write(configuration,msg);
					free(msg);
					//	
					
					close_socket2:
					close(threads[tid].clients[i]); 			
					
					pthread_mutex_lock(&new_connection_mutex);
					threads[tid].clients[i] = 0;
					threads[tid].client_count--;
					pthread_mutex_unlock(&new_connection_mutex);
					
				}
				else
				{
					
				#ifdef OSX
				//  BSD
				original_port = ntohs(peer_sockaddr.sin_port);
				//
				#else
				// Linux
				if ( getsockopt (threads[tid].clients[i], SOL_IP, SO_ORIGINAL_DST, (struct sockaddr*)&peer_sockaddr, (socklen_t*) &peer_sockaddr_len ))
						perror("Getsockopt failed");

		        get_ipstr(threads[tid].clients[i], ipstr);
		        original_port = ntohs(peer_sockaddr.sin_port);
				//
				#endif
							  	
				//LOG
				char* msg=(char*)malloc(MAX_LOG_MSG_LEN);
				memset(msg,0,MAX_LOG_MSG_LEN);
				snprintf(msg,MAX_LOG_MSG_LEN,"%d # Service_probe # SIGNATURE_SEND # source_ip:%s # dst_port:%d  \n",(int)timestamp,ipstr,original_port);//" port:%d src_ip%s\n", original_port,;
				Utils::log_write(configuration,msg);
				free(msg);
				//
			
			  	if(configuration->getConfigValue(OPT_DEBUG))
				{
					fprintf(stdout,"\n---\nThread nr.%d for port %d \n", tid,original_port);
				}
			
				std::vector<char> vectsignature=configuration->mapPort2Signature(original_port);
			
				int buffertosendsize=vectsignature.size();
				char* buffertosend= (char*)malloc(buffertosendsize);
				
				for(int j=0; j<buffertosendsize;j++)
					buffertosend[j]=vectsignature[j];
					
			  	if(configuration->getConfigValue(OPT_DEBUG))
				{
					
				
				fprintf(stdout,"signature sent -> ");	
				for(int t=0;t<buffertosendsize;t++)
				{
						if(*(buffertosend+t)==0)
							fprintf(stdout,"\\00");
						else if(*(buffertosend+t)=='\n')
							fprintf(stdout,"\\n");
						else if(*(buffertosend+t)=='\r')
							fprintf(stdout,"\\r");
						else
							fprintf(stdout,"\\%x",*(buffertosend+t));
				}
				fprintf(stdout,"\n---\n");
			
				}
				fflush(stdout);
	            
				
				if(send(threads[tid].clients[i], buffertosend, buffertosendsize,0)==-1)
					perror("Send to socket failed");
				
				
				close(threads[tid].clients[i]);
				
				free(buffertosend);
				
				pthread_mutex_lock(&new_connection_mutex);
				threads[tid].clients[i] = 0;
				threads[tid].client_count--;
				pthread_mutex_unlock(&new_connection_mutex);

				}


			}
			else
			pthread_mutex_unlock(&new_connection_mutex);
		    
		}
	}

	return 0;
}