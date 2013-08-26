#!/bin/sh
# Run this on sobu46 connected to arterial host.
# It listens for a connection from sobu7
# (192.168.200.7) on port 4455.

while [[ 1 ]]
do
	nc -L 10.0.1.202:4444 -p 4455
	sleep 1
done
