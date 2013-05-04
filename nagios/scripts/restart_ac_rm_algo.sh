#!/bin/sh

PID=`/usr/bin/pgrep -f "/home/art_coord_ramp_metering/src/lnx/ac_rm_algo" 2> /dev/null`

if [ $? -eq 1 ] && [ $1 == "CRITICAL" ] && [ $2 == "HARD" ] ; then
   /usr/bin/sudo /home/art_coord_ramp_metering/src/lnx/ac_rm_algo  2>&1 | /usr/bin/sudo /usr/sbin/cronolog /home/capath/paws/logs/ac_rm_algo-%y%m%d.log > /dev/null 2>&1 &
fi

exit 0
