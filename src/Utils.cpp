#include "Utils.h"
void Utils::hexdump(void *mem, unsigned int len)
{
    unsigned int i, j;
    
    for(i = 0; i < len + ((len % HEXDUMP_COLS) ? (HEXDUMP_COLS - len % HEXDUMP_COLS) : 0); i++)
    {
        /* print offset */
        if(i % HEXDUMP_COLS == 0)
        {
            fprintf(stdout,"0x%06x: ", i);
        }
        
        /* print hex data */
        if(i < len)
        {
            fprintf(stdout,"%02x ", 0xFF & ((char*)mem)[i]);
        }
        else /* end of block, just aligning for ASCII dump */
        {
            fprintf(stdout,"   ");
        }
        
        /* print ASCII dump */
        if(i % HEXDUMP_COLS == (HEXDUMP_COLS - 1))
        {
            for(j = i - (HEXDUMP_COLS - 1); j <= i; j++)
            {
                if(j >= len) /* end of block, not really printing */
                {
                    putchar(' ');
                }
                else if(isprint(((char*)mem)[j])) /* printable char */
                {
                    putchar(0xFF & ((char*)mem)[j]);
                }
                else /* other char */
                {
                    putchar('.');
                }
            }
            putchar('\n');
        }
    }
}

std::vector<char> Utils::wrapNMAP(string wrapper,std::vector<char> payload)
{
	stringstream ss;	
	string str;
	std::vector<char> result_vector;
	
	ss<<wrapper.substr(0,wrapper.find("__FUZZ__"));
	str=ss.str();
	
	for(int i=0; i<str.length();i++)
		result_vector.push_back(str[i]);	
	
	result_vector.insert(result_vector.end(),payload.begin(),payload.end());
	
	ss.str("");
	ss<<wrapper.substr(wrapper.find("__FUZZ__")+strlen("__FUZZ__"),wrapper.size());
	
	str=ss.str();
	
	for(int i=0; i<str.length();i++)
		result_vector.push_back(str[i]);		
	
	return result_vector;
}

std::vector<char> Utils::str2vector( std::string& s)
{
	std::vector<char> result_vector;
	
	for(int i=0; i<s.length();i++)
		result_vector.push_back(s[i]);
		
	return result_vector;
	
	
}
int Utils::isNumeric (const char * s)
{
    if (s == NULL || *s == '\0' || isspace(*s))
     return 0;
    char * p;
    strtod(s, &p);
    return *p == '\0';
}


std::vector<char> Utils::unescape(std::vector<char> & s)
{
  std::vector<char> res;
  vector<char>::const_iterator it = s.begin();
  while (it != s.end())
  {
    char c = *it++;
    if (c == '\\' && it != s.end())
    {
      switch (*it++) {
      case 'n': c = '\n'; break;
      case 'r': c = '\r'; break;
      case 't': c = '\t'; break;

      // all other escapes
      default: 
        // invalid escape sequence - skip it. alternatively you can copy it as is, throw an exception...
        continue;
      }
    }
    res.push_back(c);
  }

  return res;
}


char * Utils::get_substring_value(char* str)
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
			fprintf(stdout,"Error in configuration file1");
			exit(1);
			}
		}
	}
	
	if(soffset==-1 || eoffset==-1)
	{
			fprintf(stdout,"Error in configuration file2");
			exit(1);
	}
				
	char *substr=(char*)malloc(eoffset-soffset);
	memset(substr,0,eoffset-soffset);
	memcpy(substr,str+soffset+1,eoffset-soffset-1);
	return substr;	
}