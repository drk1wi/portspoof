#include "Fuzzer.h"

Fuzzer::Fuzzer()
{	
	

	return;
}

Fuzzer::Fuzzer(std::string nmapfuzzsignatures_file,std::string nmapfuzzpayload_file)
{	
	
	this->nmapfuzzsignatures_file = nmapfuzzsignatures_file	;
	this->nmapfuzzpayload_file = nmapfuzzpayload_file	;
	counter=0;
	return;
}

bool Fuzzer::processSignatureFile()
{
	
	
		
					
	
	return 0;
					
}

bool Fuzzer::PrepareFuzzer()
{
	
		this->fp_payloads=fopen(this->nmapfuzzpayload_file.c_str(), "r");
		char buf_file[BUFSIZE];
	
		if ( this->fp_payloads == NULL) {
		    fprintf(stdout,"Error opening payload file: %s \n",this->nmapfuzzpayload_file.c_str());
			return 1;
		}
		
		FILE *fp = fopen(this->nmapfuzzsignatures_file.c_str(), "r");
		if (fp == NULL) {
			  fprintf(stdout,"Error opening signature file: %s \n",this->nmapfuzzsignatures_file.c_str());
			return 1;
		}

		while (fgets(buf_file, BUFSIZE, fp))
		nmapfuzzsignatures.push_back(std::string(buf_file));
		fclose(fp);
			
}


std::vector<char>  Fuzzer::GetFUZZ()
{
	
	
	char buf_file[BUFSIZE];
	std::string input_line,input_wrapped;
	std::vector<char> result_vector;

	
	fgets(buf_file, BUFSIZE, this->fp_payloads);
	
	input_line=std::string(buf_file);
	input_wrapped=Utils::wrapNMAP(this->nmapfuzzsignatures[this->counter%this->nmapfuzzsignatures.size()],input_line);

	this->counter++;

	for(int i=0; i<input_wrapped.length();i++)
		result_vector.push_back(input_wrapped[i]);
	
	return result_vector;
	
	
}


/*
std::string input_line;
std::getline(std::cin, input_line);


for(int i=0; i<input_line.length();i++)
	result_vector.push_back(input_line[i]);
return result_vector;
*/