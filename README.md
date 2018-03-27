Portspoof software overview (http://drk1wi.github.io/portspoof/)

**Short description:**

The Portspoof program primary goal is to enhance OS security through a set of following techniques:

 - All 65535 TCP ports are always open

	Instead of informing an attacker that a particular port is in a CLOSED or FILTERED state Portspoof will return SYN+ACK for every port connection attempt/
	
 	As a result it is impractical to use stealth (SYN, ACK, etc.) port scanning against your system, since all ports are always reported as OPEN:

		**`nmap -p 1-20 127.0.0.1`**
        Starting Nmap 6.47 ( http://nmap.org )
        Nmap scan report for 127.0.0.1
        Host is up (0.0018s latency).
        PORT   STATE SERVICE
        1/tcp  open  tcpmux
        2/tcp  open  compressnet
        3/tcp  open  compressnet
        4/tcp  open  unknown
        5/tcp  open  unknown
        6/tcp  open  unknown
        7/tcp  open  echo
        8/tcp  open  unknown
        9/tcp  open  discard
        10/tcp open  unknown
        11/tcp open  systat
        12/tcp open  unknown
        13/tcp open  daytime
        14/tcp open  unknown
        15/tcp open  netstat
        16/tcp open  unknown
        17/tcp open  qotd
        18/tcp open  unknown
        19/tcp open  chargen
        20/tcp open  ftp-data


 - Every open TCP port emulates a service

	Portspoof has a huge database of dynamic service signatures, that will be used to generate fake banners and fool scanners.

	Scanning software usually tries to determine a service version that is running on an open port. Portspoof will respond to every service probe with a valid service signature, that is dynamically generated based on a service signature regular expression database.
	
	As a result an attacker will not be able to determine which port numbers your system is truly using: 	

	 	**`nmap -F -sV 127.0.0.1`**
        Starting Nmap 6.47 ( http://nmap.org )
        Stats: 0:00:30 elapsed; 0 hosts completed (1 up), 1 undergoing Service Scan
        Nmap scan report for 127.0.0.1
        Host is up (0.21s latency).
        PORT      STATE SERVICE          VERSION
        7/tcp     open  http             Milestone XProtect video surveillance http interface (tu-ka)
        9/tcp     open  ntop-http        Ntop web interface 1ey (Q)
        13/tcp    open  ftp              VxWorks ftpd 6.a
        21/tcp    open  http             Grandstream VoIP phone http config 6193206
        22/tcp    open  http             Cherokee httpd X
        23/tcp    open  ftp              MacOS X Server ftpd (MacOS X Server 790751705)
        25/tcp    open  smtp?
        26/tcp    open  http             ZNC IRC bouncer http config 0.097 or later
        37/tcp    open  finger           NetBSD fingerd
        53/tcp    open  ftp              Rumpus ftpd
        79/tcp    open  http             Web e (Netscreen administrative web server)
        80/tcp    open  http             BitTornado tracker dgpX
        81/tcp    open  hosts2-ns?
        88/tcp    open  http             3Com OfficeConnect Firewall http config
        106/tcp   open  pop3pw?
        110/tcp   open  ipp              Virata-EmWeb nbF (HP Laserjet 4200 TN http config)
        111/tcp   open  imap             Dovecot imapd
        113/tcp   open  smtp             Xserve smtpd
        119/tcp   open  nntp?
        135/tcp   open  http             netTALK Duo http config
        139/tcp   open  http             Oversee Turing httpd kC (domain parking)
        143/tcp   open  crestron-control TiVo DVR Crestron control server
        144/tcp   open  http             Ares Galaxy P2P httpd 7942927
        179/tcp   open  http             WMI ViH (3Com 5500G-EI switch http config)
        199/tcp   open  smux?
        389/tcp   open  http-proxy       ziproxy http proxy
        427/tcp   open  vnc              (protocol 3)
        443/tcp   open  https?
        444/tcp   open  snpp?
        445/tcp   open  http             Pogoplug HBHTTP QpwKdZQ
        465/tcp   open  http             Gordian httpd 322410 (IQinVision IQeye3 webcam rtspd)
        513/tcp   open  login?
        514/tcp   open  finger           ffingerd
        515/tcp   open  pop3             Eudora Internet Mail Server X pop3d 4918451
        543/tcp   open  ftp              Dell Laser Printer z printer ftpd k
        544/tcp   open  ftp              Solaris ftpd
        548/tcp   open  http             Medusa httpd Elhmq (Sophos Anti-Virus Home http config)
        554/tcp   open  rtsp?
        587/tcp   open  http-proxy       Pound http proxy
        631/tcp   open  efi-webtools     EFI Fiery WebTools communication
        646/tcp   open  ldp?
        873/tcp   open  rsync?
        990/tcp   open  http             OpenWrt uHTTPd
        993/tcp   open  ftp              Konica Minolta bizhub printer ftpd
        995/tcp   open  pop3s?
        1025/tcp  open  sip-proxy        Comdasys SIP Server D
        1026/tcp  open  LSA-or-nterm?
        1027/tcp  open  IIS?
        1028/tcp  open  rfidquery        Mercury3 RFID Query protocol
        1029/tcp  open  smtp-proxy       ESET NOD32 anti-virus smtp proxy
        1110/tcp  open  http             qhttpd
        1433/tcp  open  http             ControlByWeb WebRelay-Quad http admin
        1720/tcp  open  H.323/Q.931?
        1723/tcp  open  pptp?
        1755/tcp  open  http             Siemens Simatic HMI MiniWeb httpd
        1900/tcp  open  tunnelvision     Tunnel Vision VPN info 69853
        2000/tcp  open  telnet           Patton SmartNode 4638 VoIP adapter telnetd
        2001/tcp  open  dc?
        2049/tcp  open  nfs?
        2121/tcp  open  http             Bosch Divar Security Systems http config
        2717/tcp  open  rtsp             Darwin Streaming Server 104621400
        3000/tcp  open  pop3             Solid pop3d
        3128/tcp  open  irc-proxy        muh irc proxy
        3306/tcp  open  ident            KVIrc fake identd
        3389/tcp  open  ms-wbt-server?
        3986/tcp  open  mapper-ws_ethd?
        4899/tcp  open  printer          QMC DeskLaser printer (Status o)
        5000/tcp  open  http             D-Link DSL-eTjM http config
        5009/tcp  open  airport-admin?
        5051/tcp  open  ssh              (protocol 325257)
        5060/tcp  open  http             apt-cache/apt-proxy httpd
        5101/tcp  open  ftp              OKI BVdqeC-ykAA VoIP adapter ftpd kHttKI
        5190/tcp  open  http             Conexant-EmWeb JqlM (Intertex IX68 WAP http config; SIPGT TyXT)
        5357/tcp  open  wsdapi?
        5432/tcp  open  postgresql?
        5631/tcp  open  irc              ircu ircd
        5666/tcp  open  litecoin-jsonrpc Litecoin JSON-RPC f_
        5800/tcp  open  smtp             Lotus Domino smtpd rT Beta y
        5900/tcp  open  ftp
        6000/tcp  open  http             httpd.js (Songbird WebRemote)
        6001/tcp  open  daap             mt-daapd DAAP TGeiZA
        6646/tcp  open  unknown
        7070/tcp  open  athinfod         Athena athinfod
        8000/tcp  open  amanda           Amanda backup system index server (broken: libsunmath.so.1 not found)
        8008/tcp  open  http?
        8009/tcp  open  ajp13?
        8080/tcp  open  http             D-Link DGL-4300 WAP http config
        8081/tcp  open  http             fec ysp (Funkwerk bintec R232B router; .h.K...z)
        8443/tcp  open  smtp
        8888/tcp  open  smtp             OpenVMS smtpd uwcDNI (OpenVMS RVqcGIr; Alpha)
        9100/tcp  open  jetdirect?
        9999/tcp  open  http             Embedded HTTPD 3BOzejtHW (Netgear MRd WAP http config; j)
        10000/tcp open  http             MikroTik router http config (RouterOS 0982808)
        32768/tcp open  filenet-tms?
        49152/tcp open  unknown
        49153/tcp open  http             ASSP Anti-Spam Proxy httpd XLgR(?)?
        49154/tcp open  http             Samsung AllShare httpd
        49155/tcp open  ftp              Synology DiskStation NAS ftpd
        49156/tcp open  aspi             ASPI server 837305
        49157/tcp open  sip              AVM FRITZ!Box |


 	By using those two techniques together:
 	
 	- your attackers will have a tough time while trying to identify your real services.
 	- the only way to determine if a service is emulated is through a protocol probe (imagine probing protocols for 65k open ports!).
 	- it takes more than 8hours and 200MB of sent data in order to properly go through the reconessaince phase for your system ( nmap -sV -p - equivalent).
	
	***Art of Active (Offensive) Defense***

	Portspoof can be used as an 'Exploitation Framework Frontend', that turns your system into responsive and aggressive machine. In practice this usually means exploiting your attackers' tools and exploits...
	*At the moment there are few example exploits in the configuration file (portspoof.conf)*

	Portspoof is meant to be a lightweight, fast, portable and secure addition to any firewall system or security system.
	
	The general goal of the program is to make the reconessaince phase slow and bothersome for your attackers as much it is only possible.
	This is quite a change to the standard 5s Nmap scan, that will give a full view of your systems running services.

	The most important features that this software has:

      - it will add some real pain to your attackers reconessaince phase.
      - it is a userland software and does not require root privileges ! 
      - it binds to just ONE tcp port per a running instance !
      - it is easily customizable through your iptables rules 
      - marginal CPU and memory usage (multithreaded) 
      - more than 9000 dynamic service signatures to feed your attackers scanning software ! 
	
	
Author: Piotr Duszynski (piotr [at] duszynski.eu) #

License

	Consult the COPYING file.

Compile and runtime instructions
	
	Consult the DOC file (https://github.com/drk1wi/portspoof/blob/master/DOC)

Other important files 

	AUTHORS		File with Author contact info
	Changelog   What I have implemented
	FAQ		    Bug reporting and frequently asked questions
	DOC 		Documentation file
