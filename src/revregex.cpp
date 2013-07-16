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


#include "revregex.h"

 int signatures[SIGNATURES_SIZE];
 int num_signatures=30;
 struct signature **arr_lines2;

char * revregex_bracket(char * str,int start_offset,int end_offset, int* retlen) //index: '[' ... ']' 
{
	//TODO hex support
	char bslash='\\';
	char word='w';
	char digit='d';
	char range='-';

	//flags
	char nnot=0;
	char wordf=0;
	char digitf=0;
	char rangeword=0;
	char rangedigit=0;
	
	//character pool
	char characters[255]={0,};
	int chari=0;
	char characterstmp[255]={0,};
	long character_class=0;
	
	//skip first bracket char
	int i=start_offset+1;
	int lend_offset=end_offset;
	int tmpj;
	
	if( str[i]=='^') //not flag
	{
		i++;nnot=1;
	}
	
	// DEBUG
	//fprintf(stdout,"%d %d",i,end_offset);
	
	for(i;i<lend_offset;i++)
	{
		 		
			if( str[i]==bslash && i+1!=lend_offset ) //special chars - check if character class
			{
				if(str[i+1]=='c') 
					character_class|=1<<1;
				else if(str[i+1]=='s')
					character_class|=2<<1;
				else if(str[i+1]=='S')
					character_class|=1<<3;
				else if(str[i+1]=='d')
					character_class|=1<<4;
				else if(str[i+1]=='D')
					character_class|=1<<5;
				else if(str[i+1]=='w')
					character_class|=1<<6;
				else if(str[i+1]=='W')
					character_class|=1<<7;	
				else if(str[i+1]=='n')
					characters['\n']=1;
				else if(str[i+1]=='r')
					characters['\r']=1;
				else if(str[i+1]=='t')
					characters['\t']=1;
				else if(str[i+1]=='v')
					characters['\v']=1;
				else if(str[i+1]=='f')
					characters['\f']=1;
				else if(str[i+1]=='0')
					characters['\0']=1;
				else
					{
						//DEBUG
						//fprintf(stdout,"unknown char: %c !\n", str[i+1]); //DEBUG - ignore this char it probably was escaped
						characters[str[i+1]]=1;
					}
				i++;
				
			}
			else if( isalpha(str[i]) && (i+1)!=lend_offset && str[i+1]==range && (i+2)!=lend_offset && isalpha(str[i+2])) //check if rangeword
			{
					//DEBUG
					//fprintf(stdout,"rangew");
					
					//add chars from range to the pool
					tmpj=str[i];
					for(tmpj;tmpj<=str[i+2];tmpj++)
					{
						characters[tmpj]=1;
					}
					i+=3;
					rangeword=1;
					
			}
			else if( isdigit(str[i]) && (i+1)!=lend_offset && str[i+1]==range && (i+2)!=lend_offset && isdigit(str[i+2])) //check if rangedigit
			{
					//DEBUG
					//fprintf(stdout,"ranged");
					
					tmpj=str[i];
					for(tmpj;tmpj<=str[i+2];tmpj++)
					{
						characters[tmpj]=1;
					}
					i+=3;
					rangedigit=1;
					
			} 
			else if(str[i]=='.')
				character_class|=1<<8;
			else if(str[i]=='|') //TODO: implement this
				character_class|=1<<9;
			else
			{
				//printf ("# [ char %c ]\n",str[i]);
				characters[str[i]]=1;
			} 
				
	}

	char endmetachar=str[end_offset+1]; //TODO: should be ok unless one creates "[a-z]", which is invalid regex!
	int finsize=0;
	//srand (time(NULL) );
	
	// fill character pool
	if(character_class & 1<<1) 
	{
		
	}
	else if(character_class & 1<<2) 
	{
		
	}
	else if(character_class & 1<<3) 
	{

	}
	else if(character_class & 1<<4) 
	{
		int j='0';
		for(j;j<='9';j++)
		{
			characters[j]=1;
		}
		
	}
	else if(character_class & 1<<5) 
	{
		
		
		
	}
	else if(character_class & 1<<6) 
	{
		int j='a';
		for(j;j<='z';j++)
		{
			characters[j]=1;
		}
		
		j='A';
		for(j;j<='Z';j++)
		{
			characters[j]=1;
		}
			
	}
	else if(character_class & 1<<7) 
	{
		
	}
	else if(character_class & 1<<8) 
	{
		/*
		int j=0;
		for(j;j<255;j++)
		{
			characters[j]=1;
		}
		
		characters['\n']=0;
		*/
	}
	else if(character_class & 1<<9) 
	{

	}
	
	
	// simple support - TODO to extend
	if(endmetachar=='*')
		finsize=rand()%10;
	else if(endmetachar =='+')
		finsize=1+rand()%9;
		
	// DEBUG f
	//fprintf(stdout,"\n###\n");
		
	
	//TODO to be corrected
	i=0;
	for(i;i<255;i++)
	{
		if(nnot==0 && characters[i])
		{
			characterstmp[chari]=i;
			chari++;
			//fprintf(stdout,"%c",i);
		}
		else if(nnot && characters[i]==0)
		{
			characterstmp[chari]=i;
			chari++;
			
		}
	}
			
			
	char *finstr=(char*)malloc((finsize+1)*sizeof(char));
	memset(finstr,0,(finsize+1)*sizeof(char));
	
	if(chari)
	{
		int tmp;
		i=0;
		for(i;i<finsize;i++)
		{
			tmp=rand()%chari;
			finstr[i]=characterstmp[tmp];	
		}
	}
	
	*retlen=finsize;
	return finstr;
}

char * fill_specialchars(char * str,int* param_len, int start_offset,int end_offset)
{
	
	char bslash='\\';
	char word='w';
	char digit='d';
	char dot='.';
	char newline='n';
	char creturn='r';
	char tab='t';
	
	
	char* tmp;	// tmp string for merging
	int tmplen=end_offset-start_offset;
	int tmpi=0;
		
	
    if (!(tmp = (char*)malloc(tmplen * sizeof(char))))
        exit(1);	
	memset(tmp,0,tmplen);
	

	int i=start_offset;
	
	for(i;i<end_offset;i++)
	{
			if(str[i]==bslash && i+1!=end_offset && str[i+1]==word )
			{
					tmp[tmpi]=97+rand()%25;	
					tmpi++;
					i++;
					
					if(i+1!=end_offset && (str[i+1]=='+' ||str[i+1]=='*') )
						i++;
					
			}
			else if(str[i]==bslash && i+1!=end_offset && str[i+1]==digit )
			{
					tmp[tmpi]=48+rand()%10;
					tmpi++;		
					i++;
					
					if(i+1!=end_offset && (str[i+1]=='+' ||str[i+1]=='*') )
						i++;	
				
			}
			else if(str[i]==bslash && i+1!=end_offset && str[i+1]==newline )
			{
					tmp[tmpi]='\n';
					tmpi++;
					i++;	
			}
			else if(str[i]==bslash && i+1!=end_offset && str[i+1]==creturn )
			{
					tmp[tmpi]='\r';
					tmpi++;
					i++;	
			}
			else if(str[i]==bslash && i+1!=end_offset && str[i+1]==tab )
			{
					tmp[tmpi]='\t';
					tmpi++;
					i++;	
			}
			else if(str[i]==dot && i!=start_offset && str[i-1]!=bslash)
			{

					tmp[tmpi]=97+rand()%25;	
					tmpi++;	
			}		
			else
			{
				tmp[tmpi]=str[i];
				tmpi++;
			}		
			
	}
	
	char* fin;
	int finlen=tmpi+1;
	
    if (!(fin = (char*)malloc(finlen * sizeof(char))))
        exit(1);	

	memset(fin,0,finlen);
	memcpy(fin,tmp,finlen-1);
	*param_len=finlen-1;
	free(tmp);
		return fin;
}

char* revregex(char * param_str,int* param_len,int start_offset,int end_offset) // with brackets
{
	char lnaw='(';
	char rnaw=')';
	char lbrak='[';
	char rbrak=']';
	char bslash='\\';
	
	
	char* str; //main string
	int str_len=*param_len;
	int str_end_offset=end_offset;
	
	char* tmp;	// tmp string for merging
	int tmplen;
	
    if (!(str = (char*)malloc((str_len+1) * sizeof(char))))
        exit(1);	
	memset(str,0,str_len+1);
	memcpy(str,param_str+start_offset,str_len);
	
	// start
	int i; 
	int j;
	int retlen;
	char* retstr;
	repeat1:
	for(i=start_offset;i<=str_end_offset;i++) // remove () from string
	{
		if(str[i]==lnaw && i!=start_offset && str[i-1]!=bslash)
		{
			j=i;
			for(j;j<str_end_offset;j++)
			{
				if(str[j]==rnaw && str[j-1]!=bslash ){
					
					//fprintf(stdout,"#(%d %d)\n",i,j);
					//revregex(str,j-i,i+1,j);
					
					tmplen=str_len - 2 ;
					
					if (!(tmp = (char*)malloc( ( tmplen +	1) * sizeof(char)))) // alloc without the brackets
				        exit(1);	
					memset(tmp,0,( tmplen +	1));
					
					//get rid of ()
					memcpy(tmp,str,i); // copy up to index i
					memcpy(tmp+i,str+i+1,j-i); // copy i-j
					memcpy(tmp+j-1,str+j+1,str_len-j-1);
					
					//fprintf(stdout,"# offset change: %d\n", retlen-(j-i));
					free(str);
					str=tmp;
					str_len=str_len-2;
					str_end_offset=str_end_offset-2;
					goto repeat1;	
				}

				
			}
			
		}
		
	}
	
	//fprintf(stdout,"#%s\n",str);
	
	
	
	repeat2:
	i=start_offset;
	for(i=start_offset;i<=str_end_offset;i++)
	{
		if(str[i]==lbrak && i!=start_offset && str[i-1]!=bslash) // find left bracket
		{
			j=i;
			for(j;j<str_end_offset;j++) //find right bracket (without control char)
			{
				if(str[j]==rbrak && str[j-1]!=bslash ){
					

					//fprintf(stdout,"# [%d %c %d %c ]\n",i,str[i],j,str[j]);
					retstr=revregex_bracket(str,i,j,&retlen);					
					
					// merge it
				
					tmplen=str_len - (j-i) + retlen;
						
						if (!(tmp = (char*)malloc(tmplen))) 
					        exit(1);	
						memset(tmp,0,tmplen);
						memcpy(tmp,str,i); // copy up to index i
						memcpy(tmp+i,retstr,retlen); // copy new string
						memcpy(tmp+i+retlen,str+j+2,str_len-j-1); // copy after index j without control
						free(str);
						str=tmp;
						str_len=tmplen;
						str_end_offset=str_end_offset+retlen - (j-i);
						goto repeat2;	
						
					}

				
			}
			
		}
		
	}

	*param_len=str_len;
	return str;
}

int char2hex(char* ptr)
{
	unsigned int value = 0;
	char ch = *ptr;
	int i=2;
		   
		while(i--) {
		if (ch >= '0' && ch <= '9')
			value = (value << 4) + (ch - '0');
		 else if (ch >= 'A' && ch <= 'F')
			value = (value << 4) + (ch - 'A' + 10);
         else if (ch >= 'a' && ch <= 'f')
			value = (value << 4) + (ch - 'a' + 10);	  
		else
            return value;    
	    ch = *(++ptr);
		}
	  
	return value;
	
}

int ishex(char* ch)
{
	
	if (*ch >= '0' && *ch <= '9')
		return 1;
	 else if (*ch >= 'A' && *ch <= 'F')
		return 1;
     else if (*ch >= 'a' && *ch <= 'f')
		return 1;
	else
        return 0;

}

char * escape_hex(char* str,int* final_len)
{
	char bslash='\\';
	int i=0,i2=0;
	
	int length=strlen(str);
	char *str2 = (char*)malloc(length+1);
	memset(str2,0,length+1);
	
	while(*(str+i)!='\0'){
		if(*(str+i)==bslash){
			
		 	if(*(str+i+1)!='\0' && *(str+i+1)=='0'){
				*(str2+i2)=0;
				i2++;
				i++;
			}
			else if(*(str+i+1)!='\0' && *(str+i+1)=='x' && *(str+i+2)!='\0' && ishex(str+i+2) && *(str+i+3)!='\0' && ishex(str+i+3))
				{
					//fprintf(stdout,"\\%hhx",char2hex(str+i+2));
					*(str2+i2)=(char)char2hex(str+i+2);
					i2++;
					i+=3;

				}
			else
			{
				//fprintf(stdout,"%c",*(str+i));
			}
		}
		else{
			*(str2+i2)=*(str+i);
			i2++;
			
		}
		
	
		i++;
	}
	
	*final_len=i2;	
	char* strfin;
    if (!(strfin = (char*)malloc((i2 + 1) * sizeof(char))))
        exit(1);

	memset(strfin,0,i2+1);
	memcpy(strfin,str2,i2);
	free(str2);
	return strfin;
	
}


std::vector<char> process_signature(std::string str)
{
	//cout<<str;
	//cout.flush();
	
	size_t length = str.length();
	char *str2 =(char*) malloc(length+1);
	memset(str2,0,length+1);
	memcpy(str2,str.c_str(),length+1);		
	int final_len=length;
	char *str3=revregex(str2,&final_len,0,length-1);	
	char* str4=fill_specialchars(str3,&final_len,0,final_len);
	char* str5=escape_hex(str4,&final_len);
	free(str2);
	free(str3);
	free(str4);
	
	/*
	fprintf(stdout,"\n##hex##\n");
	int t=0;
	for(;t<final_len;t++)
	{
			if(*(str5+t)==0)
				fprintf(stdout,"\\00");
			else if(*(str5+t)=='\n')
				fprintf(stdout,"\\n");
			else if(*(str5+t)=='\r')
				fprintf(stdout,"\\r");
			else
				fprintf(stdout,"\\%x",*(str5+t));
	}
	fprintf(stdout,"\n");
	*/

	std::vector<char> result_vector;
	
	for(int i=0; i<final_len;i++)
		result_vector.push_back(str5[i]);

	free(str5);

	return result_vector;
	
}


