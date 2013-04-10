#ifndef CONFIG_H
#define CONFIG_H

#define CONFSEPARATOR "/"
#define DEFAULT_PORT 4444
#define BUFSIZE 1024

#define LOG_FILE "portspoof.log"
#define CONF_FILE "portspoof.conf"
#define SIGNATURE_FILE "signatures"
#define OPT_FUZZ 1
#define OPT_IP 2
#define OPT_PORT 3
#define OPT_DEBUG 4
#define OPT_SIG_FILE 5
#define OPT_LOG_FILE 6
#define OPT_SYSLOG_DIS 7
#define OPT_CONFIG_FILE 8
#define OPT_FUZZ_NMAP 9


#define MAX_PORTS 65535

#define NMAP_FUZZ_FILE_SIG "nmapfuzzsignatures"
#define FUZZ_FILE_PAYLOAD "nmapfuzzpayloads"

#include <string>
#include <stdio.h>
#include <ctype.h>
#include <pcap.h>
#include <map>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <algorithm> 
#include <iostream>
#include <ctime> 
#include <bitset>

#include "revregex.h" 
#include "connection.h"

#include "Utils.h"

using namespace std;


typedef map < unsigned short, std::vector<char> > Port_Signature_Map;
typedef vector < string > Raw_Signatures_Vector;
typedef vector < string > Nmap_Fuzz_Vector;


class Configuration {
	private:
		std::string configfile;
		std::string signaturefile;
		std::string logfile;
		std::string bind_ip;
		unsigned short int port;
		bitset<10> opts;
		
		unsigned short ifuzz;		
		Port_Signature_Map portsignatureemap;
		Raw_Signatures_Vector rawsignatures;
		
		//fuzzing part
		std::string nmapfuzzsignatures_file;
		std::string fuzzpayload_file;
		Nmap_Fuzz_Vector nmapfuzzsignatures;
		FILE *fp_payloads;
		int counter;
		std::string input_line;
		
	public:
		Configuration();
		
 		bool processArgs(int argc, char** argv);
		bool readConfigFile();
		std::vector<char> mapPort2Signature(unsigned short port);
		void usage(void);
		bool getConfigValue(int value);
		std::string getConfigFile();
		std::string getSignatureFile();
		std::string getLogFile();
		std::string getBindIP();
		unsigned short int getPort();
		bool processSignatureFile();
		
		//fuzzing part
		bool PrepareFuzzer();
		std::vector<char> GetFUZZ();
		
			
};


#endif
