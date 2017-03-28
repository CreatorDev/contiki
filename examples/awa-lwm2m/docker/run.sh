#!/bin/bash

ip route del default
ip route add default via 172.30.0.1 dev eth0
ip -6 route del default
ip -6 route add default via $IPV6_GATEWAY dev eth1

# run sshd
sed -i 's,^PermitRootLogin.*$,PermitRootLogin yes,g' /etc/ssh/sshd_config
mkdir /var/run/sshd
/usr/sbin/sshd
if [ ! -f /.pwgen ]; then
  PASS=$(pwgen 10 1)
  echo PASS $PASS
  echo -e "$PASS\n$PASS" | passwd
  touch /.pwgen
fi

exec "$@"