## Process this file with automake to produce Makefile.in

# File lists
headers = connection.h portspoof.h revregex.h threads.h log.h config_file.h
sources = connection.c portspoof.c revregex.c log.c config_file.c

# Unix executables
bin_PROGRAMS = portspoof
portspoof_SOURCES = $(headers) $(sources)

# Threads
portspoof_LDFLAGS = @LDFLAGS@ -pthread

#Preprocessor detinitions
portspoof_CPPFLAGS = -DCONFDIR='"$(sysconfdir)"' 


