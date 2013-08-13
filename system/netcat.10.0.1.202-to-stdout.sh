#!/bin/bash
# Run this on the arterial host, connected to sobu46

while [[ 1 ]]
do
	/bin/nc -l 10.0.1.202 4466
	sleep 1
done
