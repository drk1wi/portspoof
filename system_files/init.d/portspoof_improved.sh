#!/bin/sh
# Startup script for the Portspoof daemon using iptables
#
### BEGIN INIT INFO
# Provides:          portspoof
# Required-Start:    $local_fs
# Required-Stop:     $local_fs
# Should-Start:      $network $syslog iptables
# Should-Stop:       $network $syslog iptables
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Start/stop Portspoof daemon
# Description:       Start/stop Portspoof daemon
### END INIT INFO

setup_iptables() {
  # Allowed ports
  unfilteredPorts="53 80 443 49152:65534"
  interfaces="eth0"

  iptables -t nat -N PREPORTSPOOF 2> /dev/null
  iptables -t nat -F PREPORTSPOOF

  iptables -t nat -N PORTSPOOF 2> /dev/null
  iptables -t nat -F PORTSPOOF

  iptables -t nat -A PORTSPOOF -p tcp -j LOG --log-prefix 'PORTSPOOF ' --log-level 4
  iptables -t nat -A PORTSPOOF -p tcp -j REDIRECT --to-ports 4444
  iptables -t nat -A PORTSPOOF -p udp -j LOG --log-prefix 'PORTSPOOF ' --log-level 4
  iptables -t nat -A PORTSPOOF -p udp -j REDIRECT --to-ports 4444

  # Disable LAN spoofing
  #iptables -t nat -A PREPORTSPOOF -s 192.168.0.0/24 -j RETURN

  rules=$(iptables -t nat -vnL PREROUTING --line-numbers|grep "/\* PORTSPOOF-REDIRECT \*/"|awk '{ print $1 }'|tac)
  for rule in ${rules}; do
    iptables -t nat -D PREROUTING ${rule}
  done
  for int in ${interfaces}; do
    for port in ${unfilteredPorts}; do
      iptables -t nat -A PREPORTSPOOF -i ${int} -p tcp -m tcp --dport ${port} -j RETURN
      iptables -t nat -A PREPORTSPOOF -i ${int} -p udp -m udp --dport ${port} -j RETURN
    done

    iptables -t nat -A PREPORTSPOOF -p tcp -m tcp -i ${int} -j PORTSPOOF
    iptables -t nat -A PREPORTSPOOF -p udp -m udp -i ${int} -j PORTSPOOF

    iptables -t nat -A PREROUTING -i ${int} -j PREPORTSPOOF -m comment --comment "PORTSPOOF-REDIRECT"
  done
}

case "$1" in
start)
  if ! pidof portspoof > /dev/null; then
    count=$(iptables -t nat -N PREPORTSPOOF 2>&1|wc -l)
    if [ "$count" -eq 0 ]; then
      setup_iptables
    elif [ "$count" -eq 1 ]; then
      echo "iptables rules already loaded, skipping."
    else
      echo "Failed loading iptables rules."
    fi
    echo "Starting Portspoof..."
    /usr/local/bin/portspoof -D -c /usr/local/etc/portspoof.conf -s /usr/local/etc/portspoof_signatures
  else
    echo "Portspoof already running."
  fi
;;

stop)
  if pidof portspoof > /dev/null; then
    killall portspoof > /dev/null
    echo "Portspoof stopped."
  else
    echo "Portspoof not running."
  fi
;;

reload)
  setup_iptables
;;

restart)
  $0 stop
  $0 start
;;

*)
  echo "Usage: $0 {start|stop|reload|restart}"
  exit 1
esac
