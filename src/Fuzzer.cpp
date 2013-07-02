#include "Fuzzer.h"

Fuzzer::Fuzzer()
{	
	

	return;
}

Fuzzer::Fuzzer(Configuration* configuration)
{	
	
	this->configuration = configuration;
	this->nmapfuzzsignatures_file = configuration->getNmapfuzzSignaturesFile();
	this->fuzzpayload_file = configuration->getFuzzPayloadFile();
	this->counter=0;
	this->payload_counter=0;
	this->PrepareFuzzer();
	
	return;
}


bool Fuzzer::PrepareFuzzer()
{
	
	if(this->configuration->getConfigValue(OPT_FUZZ_WORDLIST))
	{
		
	this->fp_payloads=fopen(this->fuzzpayload_file.c_str(), "r");
	if ( this->fp_payloads == NULL) {
	    fprintf(stdout,"Error opening payload file: %s \n",this->fuzzpayload_file.c_str());
		return 1;
	}
	
	}
	/////
	
	if(this->configuration->getConfigValue(OPT_FUZZ_NMAP))
	{
		
	FILE *fp = fopen(this->nmapfuzzsignatures_file.c_str(), "r");
	if (fp == NULL) {
		  fprintf(stdout,"Error opening nmap signature file: %s \n",this->nmapfuzzsignatures_file.c_str());
		return 1;
	}

	char buf_file[BUFSIZE];
	while (fgets(buf_file, BUFSIZE, fp))
	nmapfuzzsignatures.push_back(std::string(buf_file));
	fclose(fp);
	fprintf(stdout,"-> Nmap signatures read: %d \n",this->nmapfuzzsignatures.size());
    
	}
	
	return 0;
			
}

std::vector<char> Fuzzer::intToBytes(int paramInt)
{
     vector<char> arrayOfByte(4);
     for (int i = 0; i < 4; i++)
         arrayOfByte[3 - i] = (paramInt >> (i * 8));
     return arrayOfByte;
}


std::vector<char> Fuzzer::shortToBytes(unsigned short paramInt)
{
     vector<char> arrayOfByte(2);
     for (int i = 0; i < 2; i++)
         arrayOfByte[1 - i] = (paramInt >> (i * 4));
     return arrayOfByte;
}

std::vector<char> Fuzzer::GenerateFuzzPayload()
{
	std::vector<char> result_vector;
	std::string str;
	
	/*
	for(int i=0;i<255;i++)
		result_vector.push_back(i);
	
	return result_vector;
	
	
		
	if(this->configuration->getConfigValue(OPT_FUZZ_RANDOM))
		{
			this->payload_counter++;
			return this->shortToBytes((unsigned short)this->payload_counter);
			
		}
		*/

	if(this->payload_counter<10)
	{
		for(int i=0;i<4000*(this->payload_counter+1);i++)
			result_vector.push_back(*(fuzz_oracle[0]));
	}
	else
	{
		
		str=std::string(fuzz_oracle[this->payload_counter-9]);
		result_vector=Utils::str2vector(str);		
	}

	this->payload_counter++;
	
	if(this->payload_counter>=25)
		this->payload_counter=0;
		
	return result_vector;
	
}

std::vector<char>  Fuzzer::GetFUZZ()
{
	std::vector<char> result_vector;
	
	if(this->configuration->getConfigValue(OPT_FUZZ_WORDLIST))
	{
		
	
		if(this->counter%this->nmapfuzzsignatures.size()==0)
		{
	
			char buf_file[BUFSIZE];
			std::string str;
	
			if(fgets(buf_file, BUFSIZE, this->fp_payloads)==NULL)
			{
					fprintf(stdout,"EOF of payload file\n");
					fflush(stdout);
			}
			
			str=std::string(buf_file);
			str.erase(str.size() - 1);//remove \n
			this->input_line=Utils::str2vector(str);
	
		}
	
		this->counter++;
	
	}
	else if(this->configuration->getConfigValue(OPT_FUZZ_INTERNAL))
	{	
		result_vector=this->GenerateFuzzPayload();
	}
	else
		fprintf(stdout,"Fuzz - shouldn't be here...\n");
			
	
	if(this->configuration->getConfigValue(OPT_FUZZ_NMAP))
	{
		
		if(this->configuration->getConfigValue(OPT_FUZZ_WORDLIST))
			result_vector=Utils::wrapNMAP(this->nmapfuzzsignatures[this->counter%this->nmapfuzzsignatures.size()],this->input_line);
		else if(this->configuration->getConfigValue(OPT_FUZZ_INTERNAL))
			result_vector=Utils::wrapNMAP(this->nmapfuzzsignatures[this->counter%this->nmapfuzzsignatures.size()],result_vector);
		
		result_vector=Utils::unescape(result_vector);	
		
	}

	return result_vector;
	
	
}


/*
std::string input_line;
std::getline(std::cin, input_line);


for(int i=0; i<input_line.length();i++)
	result_vector.push_back(input_line[i]);
return result_vector;
*/