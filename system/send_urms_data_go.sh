#!/bin/bash

while [[ 1 ]]
do
	if [[ $1 == "home" ]]
	then
		/home/atsc/urms/lnx/send_urms_data -r 10.0.1.5
	else
		/home/atsc/urms/lnx/send_urms_data
	fi
	sleep 5
done
