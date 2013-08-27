#!/bin/bash

while [[ 1 ]]
do
	/home/atsc/ab3418/lnx/receive_urms_data
	if [[ $? == 0 ]]
	then
		exit 0
	fi
done
