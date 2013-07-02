#ifndef FUZZER_H
#define FUZZER_H

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
#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Utils.h"
#include "Configuration.h"

using namespace std;


 static const char* const   fuzz_oracle[]=
{
"A",
"%n%n%n%n%n",
"%%20n",
"%n%p%s%d%x",
"%.1024d",
"%.2049d",
"-1",
"32767",
"65535",
"-2147483647",
"0xffffffff",
"a|id > /tmp/FZ|b",
"a`id > /tmp/FZ`b",
"a'id > /tmp/FZ'b",
"a;id > /tmp/FZ;b",
"a&&id > /tmp/FZ&&b"
};


class Fuzzer{
	private:
	
		std::string nmapfuzzsignatures_file;
		std::string fuzzpayload_file;
		Nmap_Fuzz_Vector nmapfuzzsignatures;
		FILE *fp_payloads;
		std::vector<char> input_line;
		Configuration* configuration;
		int counter;
		int payload_counter;
		int nmapfuzzsignatures_size;
		
	public:
		Fuzzer();
		Fuzzer(Configuration* configuration);
		bool processSignatureFile();
		std::vector<char> GetFUZZ();
		std::vector<char> GenerateFuzzPayload();		
		std::vector<char> intToBytes(int paramInt);
		std::vector<char> shortToBytes(unsigned short paramInt);
				
		bool PrepareFuzzer();
};


#endif
