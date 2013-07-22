#!/bin/sh
# Starts and stops Portspoof daemon
#

case "$1" in
start)
	
             if !  pidof portspoof >/dev/null; then
                   /usr/local/bin/portspoof -D -c /usr/local/etc/portspoof.conf -s /usr/local/etc/portspoof_signatures 
                   echo "Starting Portspoof..."
             else
                    echo "Portspoof already running.."
             fi
		
;;

stop)
	
               if pidof portspoof >/dev/null; then
                    killall -9 portspoof  >/dev/null
                    echo "Portspoof stopped.."
               else
                    echo "Portspoof not running.."
               fi
;;

restart)
  	$0 stop
  	$0 start
;;

*)
        echo "Usage: $0 {start|stop|restart}"
        exit 1
esac