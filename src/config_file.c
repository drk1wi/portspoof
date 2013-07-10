#include "config_file.h"

int isNumeric (const char * s)
{
    if (s == NULL || *s == '\0' || isspace(*s))
     return 0;
    char * p;
    strtod(s, &p);
    return *p == '\0';
}

char * get_substring_value(char* str)
{
	int i=0;
	int soffset=-1,eoffset=-1;
	for(i;i<strlen(str);i++)
	{
		if(str[i]=='"')
		{
		
		if(soffset==-1)
			soffset=i;
		else if(eoffset==-1)
			{
				eoffset=i;
				break;
			}
		else
			{
			printf("Error in configuration file1");
			exit(1);
			}
		}
	}
	
	if(soffset==-1 || eoffset==-1)
	{
			printf("Error in configuration file2");
			exit(1);
	}
				
	char *substr=malloc(eoffset-soffset);
	memset(substr,0,eoffset-soffset);
	memcpy(substr,str+soffset+1,eoffset-soffset-1);
	return substr;	
}


int get_nr_of_payloads(char* config_file)
{
	
	FILE *fp = fopen(config_file, "r");
	if (fp == NULL) {
	    printf("Error opening file: %s \n",config_file);
	    exit(1);
	}
	
	char tmp[BUFSIZE],str1[BUFSIZE], str2[BUFSIZE];
	int nr_of_payloads=0;
	
	while (fgets(tmp, BUFSIZE, fp))
	    if (strlen(tmp) >1 && tmp[0]!='#')
			{
				
				if(sscanf(tmp, "%s %s",str1,str2)==EOF){
					printf("Error in configuration file");
					exit(1);
				}
				
				if(str1==NULL || str2==NULL)
				{
					printf("Error in configuration file");
					exit(1);
				}
				nr_of_payloads++;
			}
	
	fclose(fp);
	
	return nr_of_payloads;
}

int process_config_file(struct signature **arr_lines2,int* signatures,int num_lines, char* config_file)
{
	
	FILE *fp = fopen(config_file, "r");
	if (fp == NULL) {
	    printf("Error opening file: %s \n",config_file);
	    exit(1);
	}
	
	char tmp[BUFSIZE], str1[BUFSIZE], str2[BUFSIZE];
	int i=0,lp,hp;
	char* substr;
	int len;
	
	while (fgets(tmp, BUFSIZE, fp))
	    if (strlen(tmp) >1 && tmp[0]!='#')
			{
				
				if(sscanf(tmp, "%s %s",(char *)str1,str2)==EOF){
					printf("Error in configuration file");
					exit(1);
				}
				
				if(str1==NULL || str2==NULL)
				{
					printf("Error in configuration file");
					exit(1);
				}
				
				if(isNumeric(str1)) //single port
				{
					sscanf(str1,"%d",&lp);
					substr=get_substring_value(tmp);
					//DEBUG
					//printf("port %d value: %s\n",lp,substr);
					len=strlen(substr);
					((signature*)(arr_lines2[num_lines+i-1]))->cptr=process_signature(substr,&len);;
					((signature*)(arr_lines2[num_lines+i-1]))->len=len;
	
					
					signatures[lp]=num_lines+i-1;
					i++;
					continue;
				}
				else
				{
						if(sscanf(str1, "%d-%d",&lp,&hp)==EOF){
							printf("Error in configuration file\n");
							exit(1);
						}
						
						if(lp==0 || hp==0)
						{
							printf("Error in configuration file");
							exit(1);
						}
						
						substr=get_substring_value(tmp);
						//DEBUG
						//printf("range port %d-%d value: %s\n",lp,hp,substr);
												
						len=strlen(substr);
						((signature*)(arr_lines2[num_lines+i-1]))->cptr=process_signature(substr,&len);;
						((signature*)(arr_lines2[num_lines+i-1]))->len=len;
						
						int port=lp;
							for(port;port<=hp;port++)
							{
								signatures[port]=num_lines+i-1;
							}
						
						i++;
						continue;
						
				}
				
			}
	
	fclose(fp);
	return 0;
}