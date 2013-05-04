#!/bin/sh

PID=`/usr/bin/pgrep -f "/home/atsc/ab3418/lnx/ab3418comm" 2> /dev/null`

if [ $? -eq 1 ] && [ $1 == "CRITICAL" ] && [ $2 == "HARD" ] ; then
   /usr/bin/sudo /home/atsc/ab3418/lnx/ab3418comm  2>&1 | /usr/bin/sudo /usr/sbin/cronolog /home/capath/paws/logs/ab3418comm-%y%m%d.log > /dev/null 2>&1 &
fi

exit 0
