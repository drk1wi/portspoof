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



#include <iostream>

using namespace std;

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
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <net/if.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/if_ether.h>
#include <Windows.h>
#include <winsock.h>



#include "Packets.h"
#include "PacketsManager.h"
#include "Configuration.h"

#define MAXBYTES2CAPTURE 1000

PacketsManager* packetsmanager;
Configuration* configuration;


void packet_callback(u_char *useless,const struct pcap_pkthdr* pkthdr,const u_char* packet)
{
	 struct ip *ip;
	 struct tcphdr *tcp;
	 int ip_len;
	 unsigned int ip_addr;
	
	 ip = (struct ip*)(packet + sizeof(struct ether_header));
  	 ip_len = (ip->ip_hl & 0x0f) * 4;
  	 tcp = (struct tcphdr*)(packet + sizeof(struct ether_header) + ip_len);

	 ip_addr=(unsigned int) ip->ip_src.s_addr;
	
	 packetsmanager->addKnock(ip_addr,ntohs(tcp->th_dport),configuration);

	

}

int main(int argc, char** argv){

 pcap_t *descr = NULL; 
 char errbuf[PCAP_ERRBUF_SIZE];
 memset(errbuf,0,PCAP_ERRBUF_SIZE); 
 struct bpf_program fp;		/* The compiled filter */
 bpf_u_int32 mask;		/* Our netmask */
 bpf_u_int32 net;		/* Our IP */
 
		configuration = new Configuration();
		
		if(configuration->readConfigFile())
			exit(1);
			
		if(configuration->processArgs(argc,argv))
			exit(1);
			
		packetsmanager = new PacketsManager();
		cout<<"Device: "<<configuration->getDevice().c_str()<<endl;
		cout<<"Filter: "<<configuration->getFilter().c_str()<<endl;
		

		/* Find the properties for the device */
		if (pcap_lookupnet(configuration->getDevice().c_str(), &net, &mask, errbuf) == -1) {
			printf("Couldn't get netmask for device %s: %s\n", configuration->getDevice().c_str(), errbuf);
			net = 0;
			mask = 0;
		}
		
		//promisc mode.
		if ( (descr = pcap_open_live(configuration->getDevice().c_str(), BUFSIZ, 1,  512, errbuf)) == NULL){
	    	printf("ERROR: %s\n", errbuf);
	    	exit(1);
	 	}
	
		cout<<"Promisc mode set"<<endl;
	
		if (pcap_compile(descr, &fp, configuration->getFilter().c_str(), 0, net) == -1) { 
			 printf("Couldn't parse filter %s: %s\n", configuration->getFilter().c_str(), pcap_geterr(descr));
	    	 exit(1);
		 }
		 if (pcap_setfilter(descr, &fp) == -1) {
			 printf("Couldn't install filter %s: %s\n", configuration->getFilter().c_str(), pcap_geterr(descr));
	    	 exit(1);
		 }
	 	
		cout<<"Listening..."<<endl;
		pcap_loop(descr,-1,packet_callback,NULL);
	    return 0;
}