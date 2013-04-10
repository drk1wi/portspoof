#ifndef UTILS_H
#define UTILS_H


#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <string.h>
#include <string>
#include <sstream>
#include <iostream>


#ifndef HEXDUMP_COLS
#define HEXDUMP_COLS 16
#endif


using namespace std;

class Utils {
	
	public:

		static void hexdump(void *mem, unsigned int len);
		static int isNumeric (const char * s);
		static char* get_substring_value(char* str);
		static string wrapNMAP(string wrapper,string payload);
		static std::string unescape( string& s);
		
};


#endif

