#define MAX_LOG_MSG_LEN 200
#include "Configuration.h"

class Configuration;
extern Configuration* configuration;

void log_create(const char* file);
void log_write(const char* msg);
