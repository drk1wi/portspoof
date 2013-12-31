#Portspoof [![Build Status](https://secure.travis-ci.org/drk1wi/portspoof.png)](https://secure.travis-ci.org/drk1wi/portspoof.png)

## Effective defense against port scanners

### Short description:

The Portspoof program primary goal is to enhance OS security, through:
	- simulating open ports, thus it is diffucult to determine if a valid software is listening on a particular port (check out the screenshot)
	- emulating bogus services on all open ports by sending valid service signatures to your offenders scanning software.

	As a result:
	- half-open port scans are no longer deterministic and one has to service probe all ports
	- service probes always return a valid service
	- port scanners end up with 65535 valid signatures to analyze

###General benefits of using this software are:
* Protection against well known port scanners (all scanning results are chaotic and difficult to interpret)
* Possibility to use your current firewall rules to decide for which hosts "port spoofing" applies
* Port scanning detection functionality
* Lightweight and fast
	
	
###Author: Piotr Duszynski (piotr@duszynski.eu) 
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



