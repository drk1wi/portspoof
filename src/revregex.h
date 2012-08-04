/*
 *   portspoof       Service signature obfucastor 
 *   Copyright (C) 12012 Piotr Duszyński <piotr[at]duszynski.eu>
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
 *   a combined work based on portspoof. Thus, the terms and conditions of
 *   the GNU General Public License cover the whole combination.
 * 
 *   In addition, as a special exception, the copyright holder of portspoof
 *   gives you permission to combine portspoof with free software programs or
 *   libraries that are released under the GNU LGPL. You may copy
 *   and distribute such a system following the terms of the GNU GPL for
 *   portspoof and the licenses of the other code concerned.
 * 
 *   Note that people who make modified versions of portspoof are not obligated
 *   to grant this special exception for their modified versions; it is their
 *   choice whether to do so. The GNU General Public License gives permission
 *   to release a modified version without this exception; this exception
 *   also makes it possible to release a modified version which carries
 *   forward this exception.
 */

#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define BUFSIZE 1024
#define SIGNATURES_SIZE 65535 // max port range

#ifndef REVREGEX_VARS
#define REVREGEX_VARS

typedef struct {
	int len;
	char* cptr;
} signature;


extern  int signatures[SIGNATURES_SIZE];
extern  int num_signatures;
extern  struct signature **arr_lines2;
#endif


char * revregex_bracket(char * str,int start_offset,int end_offset, char endmetachar);
char * fill_specialchars(char * str, int* param_len, int start_offset,int end_offset);
char* revregex(char * param_str,int* param_len,int start_offset,int end_offset);
int char2hex(char* ptr);
int ishex(char* ch);
char * escape_hex(char* str,int* final_len);
char * process_signature(const char* str, int* len);


