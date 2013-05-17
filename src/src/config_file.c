


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

