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

 
#ifndef SERVER_H
#define SERVER_H

#include <pthread.h>
#include <sys/param.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <stdio.h>
#include <pwd.h>
#include <unistd.h>
#include <stdexcept>
#include <sys/resource.h>

#include <time.h> 
#include "Threads.h" 
#include "Configuration.h"

#include "connection.h"


using namespace std;

class Server{

	private:
		int sockd,newsockfd;
		int addrlen;
		struct sockaddr_in my_name, peer_name;
		int status;
		
		Configuration* configuration;

	public:
		Server(Configuration* configuration);
		int choose_thread();
		bool run();
};

#endif

