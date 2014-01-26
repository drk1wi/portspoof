#!/bin/bash
# Starts and stops Portspoof daemon and sets up iptables
# Script will pick the interface with the most traffic to set up iptable on if not specified. 
# Credit: Matthew Bricker https://twitter.com/MatthewBricker https://github.com/MatthewBricker

function pickInterface {
	interfaces=`grep -v Inter /proc/net/dev|sed -e "1d"|grep -v lo|sort -n|sed -n 1p`
		if [ `echo $interfaces|awk '{print $2}'` -ne 0 ] ; then
			interface=`echo $interfaces|awk '{print $1}'|sed s/\://`
			setUpIptables
		else
			echo "WARNING: All interfaces appear to have no traffic."
			echo "         Please specify an interface to start Portspoof on."
			exit 1
		fi
}

function setUpIptables {
	runningPorts=(`netstat -an |grep -E "tcp\ ".*LISTEN|grep -v 127.0.0.1|awk '{print $4}'|sed s/.*://|sort -n`)
	if [ ${runningPorts[0]} -ne 1 ] && [ ${runningPorts[0]} -ne 2 ]; then
		underPort=$((${runningPorts[0]}-1))
		counter=0
		iptables -t nat -A PREROUTING -i $interface -p tcp -m tcp --dport 1:$underPort -j REDIRECT --to-ports 4444
		for i in "${runningPorts[@]}" ; do
			counter=$(($counter+1))
			upperPort=$(($i+1))
			underPort=$((${runningPorts[$counter]}-1))
			if [ $underPort -eq -1 ]; then
				iptables -t nat -A PREROUTING -i $interface -p tcp -m tcp --dport $upperPort:65535 -j REDIRECT --to-ports 4444
			else
				iptables -t nat -A PREROUTING -i $interface -p tcp -m tcp --dport $upperPort:$underPort -j REDIRECT --to-ports 4444
			fi
		done
	fi
}

function cleanUpIptables {
	IFS=$'\n'
	ipTableEntries=(`iptables -t nat -L PREROUTING  -n --line-numbe| grep "redir ports 4444"`)
	for i in "${ipTableEntries[@]}" ; do
		iptables -t nat -D PREROUTING 1
	done
}
case "$1" in
start)
	
             if !  pidof portspoof >/dev/null; then
					if [ -z "$2" ]; then
						echo "Setting the interface with the most traffic to run Portspoof"
						pickInterface
					elif [ $2 == "lo" ] ; then
						echo "WARNING: Setting the loopback interface up for PortSpoof."
					else 
						existingInterfaces=(`grep -v Inter /proc/net/dev|sed -e "1d"|awk '{print $1}'|sed s/\://|grep -v lo`)
						for i in "${existingInterfaces[@]}" ; do 
							if [ $2 == $i ] ; then
								if [ `grep $2 /proc/net/dev|awk '{print $2}'` -eq 0 ] ; then
									echo "WARNING: Specified interface is down or not flowing traffic."
								fi
							interface=$2
							echo "Setting up iptables to run Portspoof on $interface"
							setUpIptables
							break
							fi
						done
						if [ -z "$interface" ] ; then
							echo "Failed to find Specifed interface '$2'."
							echo "List of avalible interfaces:"
							for i in "${existingInterfaces[@]}" ; do 
								echo $i
							done
							exit 1 
						fi
					fi
                   echo "Starting Portspoof..."
                   /usr/local/bin/portspoof -D -c /usr/local/etc/portspoof.conf -s /usr/local/etc/portspoof_signatures
             else
                    echo "Portspoof already running..."
             fi
		
;;

stop)
	
				if pidof portspoof >/dev/null; then
					killall portspoof  >/dev/null
					echo "Portspoof stopped.."
				else
					echo "Portspoof not running.."
				fi
				echo "Cleaning up iptables rules."
				cleanUpIptables
;;

restart)
	$0 stop
	$0 start
;;

*)
	echo "Usage: $0 {start|stop|restart} interface"
	exit 1
esac
