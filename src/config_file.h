#include <stdio.h>
#include <string.h>

#include "revregex.h" 
#include "connection.h"

int process_config_file(struct signature **arr_lines2,int* signatures,int num_lines, char* config_file);
char *get_substring_value(char* str);
int get_nr_of_payloads(char* config_file);
