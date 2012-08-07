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



#include <pthread.h>
#include <sys/param.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#ifdef OPENBSD


#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/in_var.h>
#include <netinet/ip.h>
#include <netinet/ip_var.h>
#include <net/if.h>
#include <net/pfvar.h>
#include <net/if_pfsync.h>

#endif

#include "config.h"
#include "threads.h"
#include "connection.h"
#include "porspoof.h"

Thread threads[MAX_THREADS];


#ifdef OPENBSD

#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/in_var.h>
#include <netinet/ip.h>
#include <netinet/ip_var.h>
#include <net/if.h>
#include <net/pfvar.h>
#include <net/if_pfsync.h>


void
print_host(struct pf_addr *addr, u_int16_t port, sa_family_t af, u_int16_t rdom)
{
        int opts=0;
        char buf[48];

                
        if (inet_ntop(AF_INET, addr, buf, sizeof(buf)) == 0)
                printf("?");
        else
                printf("%s", buf);

        if (port) {
                if (af == AF_INET)
                        printf(":%u", ntohs(port));
                        else
                printf("[%u]", ntohs(port));
        }

}


int print_state(struct pfsync_state *s)
{
	
	
	struct pfsync_state_peer *src, *dst;
	struct pfsync_state_key *sk, *nk;
	struct protoent *p;
	int min, sec;
	int afto = (s->key[PF_SK_STACK].af != s->key[PF_SK_WIRE].af);
	int idx;
	int ret=0;

	src = &s->dst;
	dst = &s->src;
	sk = &s->key[PF_SK_WIRE];
	nk = &s->key[PF_SK_STACK];

	/*
	printf("%s ", s->ifname);
	if ((p = getprotobynumber(s->proto)) != NULL)
		printf("%s ", p->p_name);
	else
		printf("%u ", s->proto);

	print_host(&nk->addr[1], nk->port[1], nk->af, nk->rdomain);
	*/
	
	if (nk->af != sk->af || PF_ANEQ(&nk->addr[1], &sk->addr[1], nk->af) ||
	    nk->port[1] != sk->port[1] ||
	    nk->rdomain != sk->rdomain) {
		idx = afto ? 0 : 1;
		printf(" (");
		print_host(&sk->addr[idx], sk->port[idx], sk->af,
		    sk->rdomain);
		printf(")");
		ret=sk->port[idx];
	}
	
	/*
	if (s->direction == PF_OUT || (afto && s->direction == PF_IN))
		printf(" -> ");
	else
		printf(" <- ");
		
	*/
	
	print_host(&nk->addr[0], nk->port[0], nk->af, nk->rdomain);
	
	/*printf("    ");
	if (s->proto == IPPROTO_TCP) {
		if (src->state <= TCPS_TIME_WAIT &&
		    dst->state <= TCPS_TIME_WAIT)
			//printf("   %s:%s\n", tcpstates[src->state],tcpstates[dst->state]);
		}
		*/
	return ret;
}

int get_original_port(struct in_addr sip, u_int16_t sport)
{
	
	struct pfsync_state_peer *src, *dst;
	struct pfsync_state_key *sk, *nk;
	
	char *pf_device = "/dev/pf";
	int	mode = O_RDONLY;
	int dev = open(pf_device,mode);
	if (dev == -1)
		exit(1);
		
	struct pfioc_states ps;
	struct pfsync_state *p;
	char *inbuf = NULL, *newinbuf = NULL;
	unsigned int len = 0;
	int i;

	memset(&ps, 0, sizeof(ps));
	for (;;) {
		ps.ps_len = len;
		if (len) {
			newinbuf = realloc(inbuf, len);
			if (newinbuf == NULL)
				err(1, "realloc");
			ps.ps_buf = inbuf = newinbuf;
		}

		if (ioctl(dev, DIOCGETSTATES, &ps) < 0) {
			warn("DIOCGETSTATES");
			free(inbuf);
			return (-1);
		}
		if (ps.ps_len + sizeof(struct pfioc_states) < len)
			break;
		if (len == 0 && ps.ps_len == 0)
			goto done;
		if (len == 0 && ps.ps_len != 0)
			len = ps.ps_len;
		if (ps.ps_len == 0)
			goto done;
		len *= 2;
	}
	
	p = ps.ps_states;
	for (i = 0; i < ps.ps_len; i += sizeof(*p), p++) {
		if ( p->proto == IPPROTO_TCP && p->direction == PF_IN)
		{
			src = &p->dst;
			dst = &p->src;
			sk = &p->key[PF_SK_WIRE];
			nk = &p->key[PF_SK_STACK];
			u_int16_t state_port=nk->port[0];
			
			//DEBUG
				
		 	if(memcmp(&sip,&nk->addr[0],sizeof(int)) ==0 && state_port == sport){
				close(*pf_device);
				return print_state(p);
			}
		
		}
	}
done:
	close(*pf_device);
	free(inbuf);
	return (0);
}

#endif


void nonblock(int sockfd)
{
	int opts;
	opts = fcntl(sockfd, F_GETFL);
	if(opts < 0)
	{
		perror("fcntl(F_GETFL)\n");
		exit(1);
	}
	opts = (opts | O_NONBLOCK);
	if(fcntl(sockfd, F_SETFL, opts) < 0) 
	{
		perror("fcntl(F_SETFL)\n");
		exit(1);
	}
}

void process_connection(void *arg)
{
	int tid = (int) arg;
	int len,i;  
	char* str;
	char buffer;
	int original_port=DEFAULT_PORT;
	int n = 0;
	
	while(1) {

		sleep(1);
		for(i = 0; i < MAX_CLIENT_PER_THREAD; i++)
		{
				
			if(threads[tid].clients[i] != 0)
			{
		     
			 	n = recv(threads[tid].clients[i], &buffer,1, 0);			
			
				// deal with different recv buffer size
				if(n == 0){
				
				if(opts & OPT_DEBUG)
				fprintf(stderr,"client %d closed connection 0\n", threads[tid].clients[i]);
				
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
					{
						close(threads[tid].clients[i]); 			
					}
					else
					fprintf(stderr,"errno: %d\n", errno);    
					    				
					pthread_mutex_lock(&new_connection_mutex);
					threads[tid].clients[i] = 0;
					threads[tid].client_count--;
					pthread_mutex_unlock(&new_connection_mutex);
					
				}
				else
				{
					
				struct sockaddr_in peer_sockaddr;
				int peer_sockaddr_len=sizeof(struct sockaddr_in);

				#ifdef OPENBSD
				//  BSD
				getpeername(threads[tid].clients[i], (struct sockaddr *) &peer_sockaddr, &peer_sockaddr_len);
				original_port=get_original_port(peer_sockaddr.sin_addr,peer_sockaddr.sin_port);
				#else
				if ( getsockopt (threads[tid].clients[i], SOL_IP, SO_ORIGINAL_DST, (struct sockaddr*)&peer_sockaddr, &peer_sockaddr_len ))
						perror("Getsockopt failed");
				original_port = ntohs(peer_sockaddr.sin_port);
				#endif
				
			
			  	if(opts & OPT_DEBUG)
				{
					fprintf(stderr,"\n---\nThread nr.%d for port %d \n", tid,original_port);//: rcv from %s:%d\n", (int)tid,inet_ntoa(peer_sockaddr.sin_addr), ntohs(peer_sockaddr.sin_port));
				}
			
				str=((signature*)(arr_lines2[signatures[original_port]]))->cptr;
				len=((signature*)(arr_lines2[signatures[original_port]]))->len;
				
			  	if(opts & OPT_DEBUG)
				{
				fprintf(stderr,"signature sent -> ");	
				int t=0;
				for(;t<len;t++)
				{
						if(*(str+t)==0)
							printf("\\00");
						else if(*(str+t)=='\n')
							printf("\\n");
						else if(*(str+t)=='\r')
							printf("\\r");
						else
							printf("\\%x",*(str+t));
				}
				printf("\n---\n");
				
				}
				
				if(send(threads[tid].clients[i], str, len,0)==-1)
					perror("Send to socket failed");
				
				close(threads[tid].clients[i]);
				
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
}
