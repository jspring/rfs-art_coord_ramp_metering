#!/bin/bash

while [[ 1 ]]
do
	/usr/bin/ssh -R 5556:localhost:80 jspring@128.32.234.154 -o ConnectTimeout=15
	echo `date` Retrying ssh connection to PEMS server....>>/var/log/ssh2pems.log
	sleep 1
done
