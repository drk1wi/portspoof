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

 

#ifndef CONFIG_H
#define CONFIG_H

#define CONFSEPARATOR "/"
#define DEFAULT_PORT 4444

#define LOG_FILE "portspoof.log"
#define CONF_FILE "portspoof.conf"
#define SIGNATURE_FILE "portspoof_signatures"

#define DAEMON_USER "daemon"
#define DAEMON_GROUP "daemon"

#define OPT_FUZZ_WORDLIST 1
#define OPT_IP 2
#define OPT_PORT 3
#define OPT_DEBUG 4
#define OPT_SIG_FILE 5
#define OPT_LOG_FILE 6
#define OPT_SYSLOG_DIS 7
#define OPT_CONFIG_FILE 8
#define OPT_FUZZ_NMAP 9
#define OPT_FUZZ_INTERNAL 10
#define OPT_NOT_NMAP_SCANNER 11
#define OPT_FUZZ_RANDOM 12
#define OPT_RUN_AS_D 13


#define MAX_PORTS 65535

#include <string>
#include <stdio.h>
#include <ctype.h>
#include <map>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <algorithm> 
#include <iostream>
#include <ctime> 
#include <bitset>
#include <sys/types.h>
#include <stdio.h>
#include <pwd.h> 
#include <grp.h>
#include <unistd.h>


#include "Utils.h"
#include "Threads.h"


#include "revregex.h" 
#include "connection.h"

using namespace std;


typedef map < unsigned short, std::vector<char> > Port_Signature_Map;
typedef vector < string > Raw_Signatures_Vector;
typedef vector < string > Nmap_Fuzz_Vector;
class Fuzzer;

#include "Fuzzer.h"


class Configuration {
	private:
		std::string configfile;
		std::string signaturefile;
		std::string logfile;
		std::string bind_ip;
		std::string username;
		std::string group;

		unsigned short int port;
		int thread_number;
		bool fuzzing_mode;
		bitset<20> opts;
		Fuzzer* fuzzer;
		std::string nmapfuzzsignatures_file;
		std::string fuzzpayload_file;
		Port_Signature_Map portsignatureemap;
		Raw_Signatures_Vector rawsignatures;
		
		
	public:
		Configuration();
		void usage(void);
 		bool processArgs(int argc, char** argv);
		bool readConfigFile();
		std::vector<char> mapPort2Signature(unsigned short port);
		bool processSignatureFile();
		
		//getters
		std::string getConfigFile();
		std::string getSignatureFile();
		std::string getLogFile();
		std::string getBindIP();
		std::string getNmapfuzzSignaturesFile();
		std::string getFuzzPayloadFile();
		bool getConfigValue(int value);
		unsigned short int getPort();
		int getThreadNr();
		int getGroupid();
		int getUserid();
};


#endif
