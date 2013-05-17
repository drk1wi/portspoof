#include "log.h"
#include <syslog.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_cond_t log_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;


void log_create(char* file){
	
	FILE *fp = fopen(log_file, "a");
		if (fp == NULL) {
			FILE *fp = fopen(log_file, "w");
		}
	
	fclose(fp);
	return;
	
}
void log_write(char* msg) {

	pthread_mutex_lock(&log_mutex);

	if(opts & OPT_LOG_FILE)
	{
		FILE *fp = fopen(log_file, "a");
		if (fp == NULL) {
		    printf("Error opening file: %s \n",log_file);
			exit(1);
		}
		
		fprintf(fp,"%s",msg);
		fclose(fp);
		
	}	
	
	if(!(opts & OPT_SYSLOG_DIS))
	{
	openlog("portspoof", LOG_PID|LOG_CONS, LOG_USER);
 	syslog(LOG_INFO," %s",msg);
 	closelog();
	}
	pthread_mutex_unlock(&log_mutex);
	
	return;

}