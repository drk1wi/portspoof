/*
 *   Portspoof  - Service Signature Emulator  / Exploitation Framework Frontend   
 *   Copyright (C) 2012 Piotr Duszyński <piotr[at]duszynski.eu>
 *
 *   This program is free software; you can redistribute it and/or modify it
 *   under the terms of the GNU General Public License as published by the
 *   Free Software Foundation; either version 2 of the License, or (at your
 *   option) any later version.
 * 
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *   See the GNU General Public License for more details.
 * 
 *   You should have received a copy of the GNU General Public License along
 *   with this program; if not, see <http://www.gnu.org/licenses>.
 * 
 *   Linking portspoof statically or dynamically with other modules is making
 *   a combined work based on Portspoof. Thus, the terms and conditions of
 *   the GNU General Public License cover the whole combination.
 * 
 *   In addition, as a special exception, the copyright holder of Portspoof
 *   gives you permission to combine Portspoof with free software programs or
 *   libraries that are released under the GNU LGPL. You may copy
 *   and distribute such a system following the terms of the GNU GPL for
 *   Portspoof and the licenses of the other code concerned.
 * 
 *   Note that people who make modified versions of Portspoof are not obligated
 *   to grant this special exception for their modified versions; it is their
 *   choice whether to do so. The GNU General Public License gives permission
 *   to release a modified version without this exception; this exception
 *   also makes it possible to release a modified version which carries
 *   forward this exception.
 */

 

#include "Configuration.h"

Configuration::Configuration()
{	
	configfile = std::string(CONF_FILE);
	signaturefile = std::string(SIGNATURE_FILE);
	logfile = std::string(LOG_FILE);
	bind_ip=std::string();
	username=std::string(DAEMON_USER);
	group=std::string(DAEMON_USER);

	port=DEFAULT_PORT;	
	opts=0;
	nmapfuzzsignatures_file = std::string(NMAP_FUZZ_FILE_SIG);	
	fuzzpayload_file = std::string(FUZZ_FILE_PAYLOAD);	
	thread_number=MAX_THREADS;
	fuzzing_mode=0;
	return;
}

bool Configuration::getConfigValue(int value)
{
	return this->opts[value];
}

void  Configuration::usage(void)
{
	fprintf(stdout,"Usage: portspoof [OPTION]...\n"
	"Portspoof - service emulator / frontend exploitation framework.\n\n"
	  "-i			  ip : Bind to a particular  IP address\n"
	  "-p			  port : Bind to a particular PORT number\n"
	  "-s			  file_path : Portspoof service signature regex. file\n"
	  "-c			  file_path : Portspoof configuration file\n"
	  "-l			  file_path : Log port scanning alerts to a file\n"
	  "-f			  file_path : FUZZER_MODE - fuzzing payload file list \n"
	  "-n			  file_path : FUZZER_MODE - wrapping signatures file list\n"
	  "-1			  FUZZER_MODE - generate fuzzing payloads internally\n"
	  "-2			  switch to simple reply mode (doesn't work for Nmap)!\n"
	  "-D			  run as daemon process\n"
	  "-d			  disable syslog\n"
	  "-v			  be verbose\n"
	  "-h			  display this help and exit\n");
	
	exit(1);
}

bool Configuration::processArgs(int argc, char** argv)
{
	int	ch;
	extern char *__progname;
	
	while ((ch = getopt(argc, argv,"l:i:p:s:c:f:n:dvh12D")) != -1) {
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
			fprintf(stdout,"-> Using user defined signature file %s\n",this->signaturefile.c_str());
			this->opts[OPT_SIG_FILE]=1;
			
			break;
		case 'c':
			this->configfile  = std::string(optarg);
			this->opts[OPT_CONFIG_FILE]=1;
			fprintf(stdout,"-> Using user defined configuration file %s\n",this->configfile.c_str());
			break;
		case 'v':
		this->opts[OPT_DEBUG]=1;
			fprintf(stdout,"-> Verbose mode on.\n");
			break;
		case 'd':
		this->opts[OPT_SYSLOG_DIS]=1;
			fprintf(stdout,"-> Syslog logging disabled.\n");
			break;
		case 'D':
		this->opts[OPT_RUN_AS_D]=1;
			break;
		case 'l':
		this->opts[OPT_LOG_FILE]=1;
			this->logfile  = std::string(optarg);
			fprintf(stdout,"-> Using log file %s\n",this->logfile.c_str());
			//check log file
			Utils::log_create(configuration->getLogFile().c_str());
			break;	
		case 'f':
		this->opts[OPT_FUZZ_WORDLIST]=1;
			this->fuzzpayload_file=std::string(optarg);
			if(this->opts[OPT_FUZZ_INTERNAL])
			{
				fprintf(stdout,"Error: -1 flag cannot be used with -f \n\n", __progname);
				exit(0);
			}
			fprintf(stdout,"-> Reading fuzzing payloads from a file %s!\n",this->fuzzpayload_file.c_str());
			break;
		case 'n':
			this->opts[OPT_FUZZ_NMAP]=1;
			this->nmapfuzzsignatures_file=std::string(optarg);
			fprintf(stdout,"-> Payload wrapping mode!\n");
			break;
		case '1':
			this->opts[OPT_FUZZ_INTERNAL]=1;
			if(this->opts[OPT_FUZZ_WORDLIST])
			{
				fprintf(stdout,"Error: -f flag cannot be used with -1 \n\n", __progname);
				exit(0);
			}
			fprintf(stdout,"-> Generating fuzzing payloads internally!\n");

			break;
		case '2':
			this->opts[OPT_NOT_NMAP_SCANNER]=1;
			fprintf(stdout,"-> Switching to simple reply mode (anything apart from Nmap)!\n");
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
		fprintf(stdout,"-> No parameters - switching to simple 'open port' mode.\n");
	}
			
	if(this->getConfigValue(OPT_FUZZ_NMAP) ||this->getConfigValue(OPT_FUZZ_WORDLIST) || this->getConfigValue(OPT_FUZZ_INTERNAL))
		{
		this->fuzzer=new Fuzzer(this);
		this->thread_number=1; //set thread count to 1 due to race conditions 
		this->fuzzing_mode=1;
		}

	if(this->fuzzing_mode == 0)
	{

	if(this->opts[OPT_SIG_FILE] && this->processSignatureFile())
		exit(1);
		
	if(this->opts[OPT_CONFIG_FILE] && this->readConfigFile())
		exit(1);

	if(this->generateBufferSize())
		exit(1);

	}

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


int Configuration::getThreadNr()
{
	return this->thread_number;
}


int Configuration::getUserid()
{
        struct passwd *pwd = getpwnam(this->username.c_str()); 
        if(pwd) return pwd->pw_uid;
        
        return -1;
}


int Configuration::getGroupid()
{
        struct group *grp = getgrnam(this->group.c_str()); 
        if(grp) return grp->gr_gid;

        return -1;

}




std::vector<char> Configuration::mapPort2Signature(unsigned short port)
{	
	
	if(this->opts[OPT_FUZZ_NMAP] || this->opts[OPT_FUZZ_INTERNAL] || this->opts[OPT_FUZZ_WORDLIST])
	{
		std::vector<char> result_vector;		
		result_vector=this->fuzzer->GetFUZZ();
		return result_vector;
	}
	else
	return this->portsignatureemap[port];
}


unsigned int Configuration::mapPort2Buffer(unsigned short port)
{	
	
	if(this->opts[OPT_FUZZ_NMAP] || this->opts[OPT_FUZZ_INTERNAL] || this->opts[OPT_FUZZ_WORDLIST])
		return MAX_BUFFER_SIZE;
	else
		return this->portbuffermap[port];
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
				  srand((unsigned)time(0)); 	
				  for(int i=0;i<=MAX_PORTS;i++)
					{
					portsignatureemap.insert(make_pair(i,process_signature(rawsignatures[rand()%rawsignatures.size()])));
					//portsignatureemap.insert(make_pair(i,process_signature(rawsignatures[i%rawsignatures.size()])));
			
					}
					

	return 0;
					
}


bool Configuration::generateBufferSize()
{
	srand((unsigned)time(0)); 	
	for(int i=0;i<=MAX_PORTS;i++)
	portbuffermap.insert(make_pair(i,rand()%MAX_BUFFER_SIZE));
	
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

