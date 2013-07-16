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


std::vector<char> str2vector( std::string s)
{
	std::vector<char> result_vector;
	
	for(int i=0; i<s.length();i++)
		result_vector.push_back(s[i]);
		
	return result_vector;
	
	
}

std::vector<char> cutvector(std::vector<char> str,int start_offset, int end_offset)
{

	std::vector<char> result_vector;


	for(int i=start_offset;i<=end_offset;i++)
			result_vector.push_back(str[i]);

	
		return result_vector;

}


std::vector<char> mergevector(std::vector<char> str,std::vector<char> str2)
{

	std::vector<char> result_vector;

	for(int i=0;i<str.size();i++)
		result_vector.push_back(str[i]);
	
	for(int i=0;i<str2.size();i++)
		result_vector.push_back(str2[i]);

	return result_vector;

}




std::vector<char> revregexn(std::vector<char> str)
{

	//defines
	char lnaw='(';
	char rnaw=')';
	char lbrak='[';
	char rbrak=']';
	char bslash='\\';

	std::vector<char> result_vector=str;
	std::vector<char> tmp;

	repeat_remove:
	tmp.clear();
	
	for(int i=0;i<result_vector.size();i++) // remove parenthises
	{
		if(result_vector[i]==lnaw && ( (i == 0 ) || result_vector[i-1]!=bslash))
		{

			for(int j=i;j<result_vector.size();)
			{
				if(result_vector[j]==rnaw && result_vector[j-1]!=bslash ){
					
					tmp=mergevector(tmp,cutvector(result_vector,0,i-1));
					tmp=mergevector(tmp,cutvector(result_vector,i+1,j-1));
					tmp=mergevector(tmp,cutvector(result_vector,j+1,result_vector.size()-1));
					result_vector=tmp;
					goto repeat_remove;
					}

				j++;

				if(j==result_vector.size())
				{

				fprintf(stdout,"Regex error : !!");
				
				for(int k=0;k<result_vector.size();k++)
					cout<<result_vector[k];
				cout<<endl;
				}
			}

		}

	}

	repeat_remove2:
	tmp.clear();

	for(int i=0;i<result_vector.size();i++) // 
	{
		if(result_vector[i]==lbrak && ( (i == 0 ) || result_vector[i-1]!=bslash))
		{

			for(int j=i;j<result_vector.size();)
			{
				if(result_vector[j]==rbrak && result_vector[j-1]!=bslash ){
					
					tmp=mergevector(tmp,cutvector(result_vector,0,i-1));
					tmp=mergevector(tmp,cutvector(result_vector,i+1,j-1));
					tmp=mergevector(tmp,cutvector(result_vector,j+1,result_vector.size()-1));
					result_vector=tmp;
					goto repeat_remove2;
					}

				j++;

				if(j==result_vector.size())
				{

				fprintf(stdout,"Regex error : !!");
				
				for(int k=0;k<result_vector.size();k++)
					cout<<result_vector[k];
				cout<<endl;
				}
			}

		}

	}
	return result_vector;
}


int main(int argc, char **argv)
{

std::vector<char> result_vector1;
std::vector<char> result_vector;


string s ="123[xxx]sfssf\n";

result_vector=revregexn(str2vector(s));

for(int i=0;i<result_vector.size();i++)
	cout<<result_vector[i];



s ="(1xx)\n";

result_vector=revregexn(str2vector(s));

for(int i=0;i<result_vector.size();i++)
	cout<<result_vector[i];

/*
s ="(1234)5\n";
cout<<revregexn(s);
*/

return 0;


}

