#!/bin/sh
# Run this on sobu7 connected to ramp meter host.
# It connects to port 4455 on sobu46 and forwards 
# packets to it from port 4444 (connected to the RM host
# 10.0.1.201).

while [[ 1 ]]
do
	nc -L 192.168.200.46:4455 -p 4444
	sleep 1
done
