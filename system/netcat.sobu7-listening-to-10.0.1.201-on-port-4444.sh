#!/bin/sh
# Run this on sobu7 connected to ramp metering host.
# It listens for a connection from the ramp metering host
# (10.0.1.201) on port 4444.

while [[ 1 ]]
do
	/usr/bin/nc -l 10.0.1.201 -p 4444
	sleep 1
done
