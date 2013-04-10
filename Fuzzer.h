#ifndef FUZZER_H
#define FUZZER_H

#define NMAP_FUZZ_FILE_SIG "nmapfuzzsignatures"
#define NMAP_FUZZ_FILE_PAY "nmapfuzzpayloads"

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
#include <pthread.h>

#include "Utils.h"
#include "Configuration.h"

using namespace std;

typedef vector < string > Nmap_Fuzz_Vector;

class Fuzzer{
	private:
	
		std::string nmapfuzzsignatures_file;
		std::string nmapfuzzpayload_file;
		Nmap_Fuzz_Vector nmapfuzzsignatures;
		FILE *fp_payloads;
		int counter;
		
	public:
		Fuzzer();
		Fuzzer(std::string,std::string);
		bool processSignatureFile();
		bool Prepare();
		std::vector<char> GetFUZZ();		
			
};


#endif
