# Portspoof

![License](https://img.shields.io/badge/license-GPLv2-blue.svg)
![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20POSIX-lightgrey.svg)
![Build Status](https://github.com/drk1wi/portspoof/actions/workflows/cmake.yml/badge.svg)

**Portspoof** is a security tool designed to enhance OS security by confusing and slowing down attackers during the reconnaissance phase. It emulates a multitude of open ports and service signatures, making it nearly impossible for scanners to identify the true attack surface of a system.

## Table of Contents
- [Overview](#overview)
- [Key Features](#key-features)
- [Portspoof Pro](#portspoof-pro-enterprise)
- [How It Works](#how-it-works)
- [Installation](#installation)
- [Configuration](#configuration)
- [Usage](#usage)
- [Authors & License](#authors--license)

## Overview

The primary goal of the Portspoof program is to enhance OS security through a set of techniques that make reconnaissance slow and bothersome for attackers. This is a significant shift from a standard 5-second Nmap scan that typically provides a full view of your system's running services.

### Key Features

*   **All 65535 TCP Ports Are Always Open:** Instead of informing an attacker that a port is CLOSED or FILTERED, Portspoof returns `SYN+ACK` for every connection attempt.
*   **Service Emulation:** Every open TCP port emulates a legitimate service using a huge database of dynamic signatures.
*   **Defeats Stealth Scans:** Stealth scans (SYN, ACK, etc.) become impractical because all ports are reported as `OPEN`.
*   **Confuses Version Detection:** Responds to service probes with valid, dynamically generated signatures to fool version detection tools (like `nmap -sV`).
*   **Slows Reconnaissance:** Increases scan times significantly (e.g., from seconds to hours) by forcing scanners to process bogus data for every port.
*   **Offensive Defense:** Can be used as an 'Exploitation Framework Frontend' to exploit vulnerabilities in the attacker's own scanning tools.
*   **Lightweight & Secure:** Runs in userland (no root privileges required!), binds to just **ONE** TCP port per running instance, and has marginal CPU/memory usage.
*   **Massive Signature Database:** More than 9000 dynamic service signatures to feed your attackers' scanning software!

### Portspoof Pro (Enterprise)

For enterprise environments, **Portspoof Pro** is an advanced **Active Deception Platform** built to turn dark IP space into a high-fidelity detection grid. While the open-source version handles single-host obfuscation, Pro focuses on network-wide active defense and structured threat intelligence.

**Key Advantages:**
*   **Active Deception Grid:** Turn unused subnets and dark IP blocks into an active defense layer. A single sensor emulates thousands of IPs (e.g., entire /16 networks) with millions of open ports.
*   **Stateful Protocol Emulation:** Takes deception to the next level by engaging attackers with fully interactive, multi-step protocol handshakes rather than just static banners.
*   **Production-Tested Architecture:** Built with **Rust** for high-concurrency async I/O, ensuring stateless, battle-tested reliability in production environments.
*   **Central Management:** Centralized Web UI for scalable fleet management.
*   **Active Countermeasures:** Beyond just open ports, Pro actively tarpits and exhausts attacker resources (e.g., socket locking, slow byte responses), slowing reconnaissance on a single host from minutes to **12+ hours**.
*   **Enterprise Telemetry:** Delivers structured, **SIEM-ready JSON** intelligence with built-in connectors and MITRE ATT&CK mapping.
*   **Compliance:** Designed to meet **NIS2**, **DORA**, **ISO 27001**, and **NIST CSF** requirements for anomaly detection and network monitoring.

[Learn more about Portspoof Pro](https://portspoof.io)

---

## How It Works

### 1. Defeating Port Scanners
Stealth scans (SYN, ACK, etc.) become impractical because every port is reported as `OPEN`.

**Example Nmap Scan:**
```bash
$ nmap -p 1-20 127.0.0.1
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
```

### 2. Confusing Version Detection
Portspoof responds to service probes with valid, dynamically generated signatures based on a massive regular expression database. As a result, an attacker will not be able to determine which port numbers your system is truly using.

**Example Version Scan:**
```bash
$ nmap -F -sV 127.0.0.1
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
```

### The Result
By using these techniques together:
*   Attackers will have a tough time identifying your real services.
*   The only way to determine if a service is emulated is through a protocol probe (imagine probing protocols for 65k open ports!).
*   It takes more than 8 hours and 200MB of sent data to properly go through the reconnaissance phase for your system (equivalent to `nmap -sV -p -`).

---

## Installation

### Prerequisites
Ensure you have a C++ compiler and CMake (3.10+) installed.

### Build from Source
```bash
mkdir build && cd build
cmake -DCMAKE_INSTALL_SYSCONFDIR=/etc .. 
make
sudo make install
```

---

## Configuration

Portspoof runs in userland but relies on system firewall rules to intercept traffic destined for other ports.

### 1. Configure Firewall (iptables)
Redirect all incoming TCP traffic (ports 1-65535) to the Portspoof port (default 4444).

**Linux (iptables):**
```bash
# Redirect traffic from eth0 to Portspoof
sudo iptables -t nat -A PREROUTING -i eth0 -p tcp -m tcp --dport 1:65535 -j REDIRECT --to-ports 4444
```
*Note: Replace `eth0` with your actual network interface name.*

To make this persistent, you can save your iptables rules or use the `iptables-config` provided in the `system_files` directory.

### 2. System Startup
You can add Portspoof to your startup scripts using the examples in `system_files/init.d/`.

---

## Usage

### Service Emulation Mode (Recommended)
This mode generates and feeds port scanners with bogus service signatures.
```bash
portspoof -c /etc/portspoof.conf -s /etc/portspoof_signatures -D
```

### Open Port Mode
This mode simply returns an `OPEN` state for every connection attempt without sending service banners.
```bash
portspoof -D
```

### Fuzzing Mode
Portspoof can be used to fuzz scanning tools by sending random or wordlist-based payloads.

**Fuzz with internal generator:**
```bash
# Generates random payloads of random size
portspoof -1 -v
```

**Fuzz with a wordlist:**
```bash
portspoof -f payloads.txt -v
```

---

## Authors & License

**Author:** Piotr Duszyński ([@drk1wi](https://twitter.com/drk1wi))

**License:** GNU General Public License v2.0 (GPLv2). See the `LICENSE` file for details.

For commercial, legitimate applications, please contact the author for the appropriate licensing arrangements.

---

## Reporting Issues

If you encounter any bugs or have feature requests, please report them on the [GitHub Issue Tracker](https://github.com/drk1wi/portspoof/issues) or contact the author via email at `piotr [at] duszynski.eu`.
