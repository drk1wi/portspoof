## Process this file with automake to produce Makefile.in

# File lists
headers =   config.h  Configuration.h  connection.h  Fuzzer.h  Server.h  Revregex.h  Threads.h  Utils.h
sources = Configuration.cpp  connection.cpp  Fuzzer.cpp  Portspoof.cpp  Server.cpp Revregex.cpp  Utils.cpp

# Unix executables
bin_PROGRAMS = portspoof
portspoof_SOURCES = $(headers) $(sources)

# Threads
portspoof_LDFLAGS = @LDFLAGS@ -pthread

#Preprocessor detinitions
portspoof_CPPFLAGS = -DCONFDIR='"$(sysconfdir)"' 


