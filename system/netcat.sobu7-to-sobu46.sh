#!/bin/sh
# Run this on sobu7 connected to ramp meter host

while [[ 1 ]]
do
	/usr/bin/nc -L 192.168.200.46:4455 -p 4444
	sleep 1
done
