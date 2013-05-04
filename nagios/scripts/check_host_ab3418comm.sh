#!/bin/sh

PID=`/usr/bin/pgrep -f "/home/atsc/ab3418/lnx/ab3418comm" 2> /dev/null`

if [ "$?" -eq 0 ] ; then
   echo "OK - PID is $PID"
   exit 0
elif [ "$?" -eq 1 ] ; then
   echo "CRITICAL - ab3418comm does not appear to be running"
   exit 2
else
   echo "UNKNOWN - pgrep returned $?"
   exit 3
fi
