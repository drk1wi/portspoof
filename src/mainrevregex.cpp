#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <string.h>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <syslog.h>
#include <pthread.h>
#include <unistd.h>

using namespace std;


string revregexn(string str)
{

	//defines
	char lnaw='(';
	char rnaw=')';
	char lbrak='[';
	char rbrak=']';
	char bslash='\\';
	//


	stringstream ss;	
	std::vector<char> result_vector;
	std::string::iterator lastcut=str.begin();
	std::string::iterator iter1;
	std::string::iterator iter2;
	string result_string=str;

	for(iter1=str.begin();iter1<str.end();iter1++) // remove () [regex special char.] from  the string
	{
		if(*iter1==lnaw && ( (iter1 == str.begin() ) || *(iter1-1)!=bslash))
		{
			ss<<str.substr(lastcut - str.begin(),iter1 - str.begin());

			for(iter2=iter1;iter2<str.end();iter2++)
			{
				if(*iter2==rnaw && *(iter2-1)!=bslash ){
					
					ss<<str.substr(iter1+1 - str.begin(),iter2-2-str.begin());
					lastcut=iter2;
					iter1=iter2;
					break;
				}
				//fprintf(stdout,"Regex error %s !! \n",str.c_str());
			}

		}

	}

	//process results

	if(iter1==str.end())
	{
		ss<<str.substr(lastcut +1 - str.begin(),iter1-str.begin());
		result_string=ss.str();

	}


	return result_string;
}


int main(int argc, char **argv)
{

string s ="1(234)567\n";
cout<<revregexn(s);
s ="(1234)5\n";
cout<<revregexn(s);

return 0;


}

