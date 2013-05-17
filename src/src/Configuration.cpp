#include "Configuration.h"

Configuration::Configuration()
{	
	configfile = std::string(CONF_FILE);
	signaturefile = std::string(SIGNATURE_FILE);
	logfile = std::string(LOG_FILE);
	bind_ip=std::string();
	port=DEFAULT_PORT;	
	opts=0;
	nmapfuzzsignatures_file = std::string(NMAP_FUZZ_FILE_SIG);	
	fuzzpayload_file = std::string(FUZZ_FILE_PAYLOAD);	
		
	return;
}

bool Configuration::getConfigValue(int value)
{
	return this->opts[value];
}

void  Configuration::usage(void)
{
	fprintf(stdout,"Usage: portspoof [OPTION]...\n"
	"Portspoof - service signature obfuscator.\n\n"
	  "-i             bind to a user defined IP address\n"
	  "-p			  bind to a user defined PORT number\n"
	  "-s			  custom signture file\n"
	  "-c			  configuration file\n"
	  "-l			  log port scanning alerts to a file\n"
	  "-d			  disable syslog\n"
	  "-v			  be verbose\n"
	  "-f			  read fuzz payload list\n"
	  "-1			  generate fuzzing payloads\n"
	  "-n			  nmap wrap fuzz signatures\n"
	  "-h			  display this help and exit\n\n"
	"Without any OPTION - use default values and continue\n");
	
	exit(1);
}

bool Configuration::processArgs(int argc, char** argv)
{
	int	ch;
	extern char *__progname;
	
	while ((ch = getopt(argc, argv,"l:i:p:s:c:f:n:dvh123")) != -1) {
		switch (ch) {
		case 'i':
			this->bind_ip = std::string(optarg);
			this->opts[OPT_IP]=1;
			break;
		case 'p':
			this->port  = atoi(optarg);
			this->opts[OPT_PORT]=1;
			break;
		case 's':
			this->signaturefile  = std::string(optarg);
			this->opts[OPT_SIG_FILE]=1;
			
			break;
		case 'c':
			this->configfile  = std::string(optarg);
			this->opts[OPT_CONFIG_FILE]=1;
			
			break;
		case 'v':
		this->opts[OPT_DEBUG]=1;
			fprintf(stdout,"-> Verbose mode on.\n");
			break;
		case 'd':
		this->opts[OPT_SYSLOG_DIS]=1;
			fprintf(stdout,"-> Syslog logging disabled.\n");
			break;
		case 'l':
		this->opts[OPT_LOG_FILE]=1;
			this->logfile  = std::string(optarg);
			fprintf(stdout,"-> Using log file %s\n",this->logfile.c_str());
			break;	
		case 'f':
		this->opts[OPT_FUZZ_WORDLIST]=1;
			this->fuzzpayload_file=std::string(optarg);
			fprintf(stdout,"-> Reading fuzzing payloads from a file!\n");
			break;
		case 'n':
			this->opts[OPT_FUZZ_NMAP]=1;
			this->nmapfuzzsignatures_file=std::string(optarg);
			fprintf(stdout,"-> NMAP wrapper mode!\n");
			break;
		case '1':
			this->opts[OPT_FUZZ_INTERNAL]=1;
			fprintf(stdout,"-> Generate fuzzing payloads!\n");
			break;
		case '2':
			this->opts[OPT_NOT_NMAP_SCANNER]=1;
			fprintf(stdout,"-> Switching to simple reply mode (anything apart from Nmap)!\n");
			break;
		case '3':
			this->opts[OPT_FUZZ_RANDOM]=1;
			fprintf(stdout,"-> Random int fuzzing!\n");
			break;			
		case 'h':
			this->usage();
			break;
		default:
			fprintf(stdout,"Try ` %s -h' for more information.\n\n", __progname);
			exit(0);
			break;
			}
	}
	

	if(this->opts==0)
	{
		fprintf(stdout,"-> No parameters - using default values.\n");
	}
			
	if(this->getConfigValue(OPT_FUZZ_NMAP) ||this->getConfigValue(OPT_FUZZ_WORDLIST) || this->getConfigValue(OPT_FUZZ_INTERNAL))
		this->fuzzer=new Fuzzer(this);
	
	
	return 0;
}

std::string Configuration::getConfigFile()
{
	return this->configfile;
}
std::string Configuration::getSignatureFile()
{
	return this->signaturefile;
	
}
std::string Configuration::getNmapfuzzSignaturesFile()
{
	return this->nmapfuzzsignatures_file;
}
std::string Configuration::getFuzzPayloadFile()
{
	return this->fuzzpayload_file;
	
}
std::string Configuration::getLogFile()
{
	return this->logfile;
}

std::string Configuration::getBindIP()
{
	return this->bind_ip;
}

unsigned short int Configuration::getPort()
{
	return this->port;
}

std::vector<char> Configuration::mapPort2Signature(unsigned short port)
{	
	/*
	if(this->opts&OPT_FUZZ_WORDLIST
	{
		
		std::string input_line;
		std::getline(std::cin, input_line);
		std::vector<char> result_vector;

		for(int i=0; i<input_line.length();i++)
			result_vector.push_back(input_line[i]);
		return result_vector;
		
	}
	*/
	if(this->opts[OPT_FUZZ_NMAP] || this->opts[OPT_FUZZ_INTERNAL] || this->opts[OPT_FUZZ_WORDLIST])
	{
		std::vector<char> result_vector;		
		result_vector=this->fuzzer->GetFUZZ();
		return result_vector;
	}
	else
	return this->portsignatureemap[port];
}

bool Configuration::processSignatureFile()
{
	
	char buf_file[BUFSIZE];

	FILE *fp = fopen(this->signaturefile.c_str(), "r");
	if (fp == NULL) {
	    fprintf(stdout,"Error opening signature file: %s \n",this->signaturefile.c_str());
		return 1;
	}
	
	while (fgets(buf_file, BUFSIZE, fp))
				rawsignatures.push_back(std::string(buf_file));
	
	fclose(fp);
	
				  // set  random mapping
				  //srand((unsigned)time(0)); 	
				  for(int i=0;i<MAX_PORTS;i++)
					{
					//portsignatureemap.insert(make_pair(i,process_signature(rawsignatures[rand()%rawsignatures.size()])));
					portsignatureemap.insert(make_pair(i,process_signature(rawsignatures[i%rawsignatures.size()])));
			
					}
					

	return 0;
					
}

bool Configuration::readConfigFile()
{
	char tmp[BUFSIZE], str1[BUFSIZE], str2[BUFSIZE];
	int lp,hp;
	std::stringstream ss;

	
	FILE *fp = fopen(this->configfile.c_str(), "r");
	if (fp == NULL) {
	    fprintf(stdout,"Error opening file: %s \n",this->configfile.c_str());
		return 1;
	}
	
	while (fgets(tmp, BUFSIZE, fp))
	    if (strlen(tmp) >1 && tmp[0]!='#')
			{
				
				if(sscanf(tmp, "%s %s",str1,str2)==EOF){
					fprintf(stdout,"Error in configuration file");
					exit(1);
				}
				
				if(str1==NULL || str2==NULL)
				{
					fprintf(stdout,"Error in configuration file");
					exit(1);
				}
				
				if(Utils::isNumeric(str1)) //single port
				{
					sscanf(str1,"%d",&lp);
					//DEBUG
					//fprintf(stdout,"port %d value: %s\n",lp,Utils::get_substring_value(tmp));
				
					portsignatureemap[lp]=process_signature(std::string(Utils::get_substring_value(tmp)));	
					continue;
				}
				else
				{
						if(sscanf(str1, "%d-%d",&lp,&hp)==EOF){
							fprintf(stdout,"Error in configuration file\n");
							exit(1);
						}
						
						if(lp==0 || hp==0)
						{
							fprintf(stdout,"Error in configuration file");
							exit(1);
						}
						
						//DEBUG
						//fprintf(stdout,"range port %d-%d value: %s\n",lp,hp,Utils::get_substring_value(tmp));
						
						for(int i=lp;i<=hp;i++)
						portsignatureemap[i]=process_signature(std::string(Utils::get_substring_value(tmp)));
						
						continue;
						
				}
				
			}
	
	fclose(fp);

		
	return 0;
	
}

