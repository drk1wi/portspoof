/*
 *   Portspoof  - Service Signature Emulator      
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




#include "Server.h"
#include "Configuration.h"
#include "threads.h"

#include "revregex.h"
#include "connection.h"
#include "log.h"

Configuration* configuration;
Server* server;


int main(int argc, char **argv)
{

	configuration = new Configuration();
		
	if(configuration->processArgs(argc,argv))
		exit(1);
		
	if(configuration->processSignatureFile())
		exit(1);
		
	if(configuration->readConfigFile())
		exit(1);
		


	//check log file
	if(configuration->getConfigValue(OPT_LOG_FILE))
		log_create(configuration->getLogFile().c_str());

	// open file	
	if(configuration->getConfigValue(OPT_SIG_FILE))
	fprintf(stdout,"-> Using user defined signature file %s\n",configuration->getSignatureFile().c_str());
	fflush(stdout);
	



	//create portspoof
	server = new Server(configuration);
	server->run();

	return 0;
}
