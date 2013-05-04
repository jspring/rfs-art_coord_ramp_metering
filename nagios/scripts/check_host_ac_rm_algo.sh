#!/bin/sh

PID=`/usr/bin/pgrep -f "/home/art_coord_ramp_metering/src/lnx/ac_rm_algo" 2> /dev/null`

if [ "$?" -eq 0 ] ; then
   echo "OK - PID is $PID"
   exit 0
elif [ "$?" -eq 1 ] ; then
   echo "CRITICAL - ac_rm_algo does not appear to be running"
   exit 2
else
   echo "UNKNOWN - pgrep returned $?"
   exit 3
fi
