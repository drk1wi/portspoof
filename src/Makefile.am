## Process this file with automake to produce Makefile.in

# File lists
headers = config_file.h  config.h  Configuration.h  connection.h  Fuzzer.h  log.h  portspoof.h  revregex.h  threads.h  Utils.h
sources = Configuration.cpp  connection.cpp  Fuzzer.cpp  log.cpp  portspoof.cpp  revregex.cpp  Utils.cpp

# Unix executables
bin_PROGRAMS = portspoof
portspoof_SOURCES = $(headers) $(sources)

# Threads
portspoof_LDFLAGS = @LDFLAGS@ -pthread

#Preprocessor detinitions
portspoof_CPPFLAGS = -DCONFDIR='"$(sysconfdir)"' 


