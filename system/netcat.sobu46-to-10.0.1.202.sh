#!/bin/sh
# Run this on sobu46 connected to arterial host

while [[ 1 ]]
do
	/usr/bin/nc -L 10.0.1.202:4466 -p 4455
	sleep 1
done
