#Portspoof [![Build Status](https://secure.travis-ci.org/drk1wi/portspoof.png)](https://secure.travis-ci.org/drk1wi/portspoof.png)

## Effective defense against port scanners

### Short description:



####*Art of Annoyance*


The Portspoof program primary goal is to enhance OS security through a set of few techniques:

**1. All TCP ports are always open** 

Instead of informing an attacker that a particular port is CLOSED or FILTERED a system with Portspoof will return SYN+ACK for every port connection attempt.

As a result it is impractical to use stealth (SYN, ACK, etc.) port scanning against your system, since all ports are always reported as OPEN.
Therefore, it is difficult to determine if a valid software is listening on a particular port (check out the screenshots).

**2. Every open TCP port emulates a services**

Portspoof has a huge dynamic service signature database, that will be used to generate valid service signatures and feed them to your offenders scanning software.

Scanning software usually tries to determine a service version that is running on an open port. This step is mandatory if one would want to identify port numbers on which you are running your services on a system behing the Portspoof.
For this reason Portspoof will respond to every service probe with a valid service signature, that is dynamically generated based on a service signature regular expression database.
As a result an attacker will not be able to determine which port numbers your system is truly using.


By using those two techniques together:

* your attackers will have a tough time while trying to identify your real services.
* the only way to determine if a service is emulated is through a protocol probe (imagine probing protocols for 65k open ports!).
* it takes more than 8hours and 200MB of sent data in order to properly go through the reconessaince phase for your system (nmap -sV -p - equivalent).
	

####*Art of Active (Offensive) Defense*

Portspoof can be used as an 'Exploitation Framework Frontend', that turns your system into responsive and aggressive machine. In practice this usually means exploiting your attackers' tools and exploits. 
At the moment there are few example exploits in the configuration file (portspoof.conf)


Portspoof is meant to be a lightweight, fast, portable and secure addition to the any firewall system or security system.
The general goal of the program is to make the reconessaince phase slow and bothersome for your attackers as much it is only possible.
This is quite a change to the standard 5s Nmap scan, that will give a full view of your systems running services.

####*Features*

The most important features that Portspoof has:

- it will add some real pain to your attackers reconessaince phase.
- it is a userland software and does not require root privileges ! 
- it binds to just ONE tcp port per a running instance !
- it is easily customizable through your iptables rules 
- marginal CPU and memory usage (multithreaded) 
- more than 9000 dynamic service signatures to feed your attackers scanning software ! 

###Author: Piotr Duszynski (piotr[at]duszynski.eu) 
http://portspoof.org/

Follow me at @drk1wi

###License
Consult the COPYING file.

###Compile instructions
Consult the INSTALL file.

###Other important files 

* AUTHORS - File with Author contact info
* Changelog - What I have implemented
* TODO - What I am planning to implement in the upcoming versions
* FAQ - Bug reporting and frequently asked questions


