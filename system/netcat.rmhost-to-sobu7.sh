#!/bin/bash
# Run this on the ramp meter host, connected to sobu7

while [[ 1 ]]
do
	/bin/nc 10.0.1.7 4444
	sleep 1
done
