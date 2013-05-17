#ifndef CONFIG_H
#define CONFIG_H

#define CONFSEPARATOR "/"
#define DEFAULT_PORT 4444
#define BUFSIZE 1024

#define LOG_FILE "portspoof.log"
#define CONF_FILE "portspoof.conf"
#define SIGNATURE_FILE "signatures"
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
class Fuzzer;

#include "Fuzzer.h"


class Configuration {
	private:
		std::string configfile;
		std::string signaturefile;
		std::string logfile;
		std::string bind_ip;
		unsigned short int port;
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
			
};


#endif
