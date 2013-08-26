#!/bin/sh
# Run this on sobu46 connected to arterial host.
# It listens for a connection from the arterial host
# (10.0.1.202) on port 4444.

while [[ 1 ]]
do
	/usr/bin/nc -l 10.0.1.202 -p 4444
	sleep 1
done
