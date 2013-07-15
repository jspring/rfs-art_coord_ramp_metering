#!/bin/bash

#Get current time
currtimestr=`date +"%H %M %S"`
currtime=`echo $currtimestr | awk '{print ($1*3600) + ($2*60) + $3}'`

#Get latest timestamp from latest data file
file=`ls -tr /big/data/ac_rm_1/a[01]* | tail -1`
x=`tail -2 $file | head -1 | awk '{print $1}' | sed -e 's/:/ /g' -e 's/\./ /g'`
ts_from_file=`echo $x | awk '{print ($1*3600) + ($2*60) + $3}'`

#Deal with midnight
if [[ $ts_from_file -gt 86397 ]]
then
	ts_from_file=$((86400-$ts_from_file))
fi
if [[ $currtime -gt 86397 ]]
then
	currtime=$((86400-$currtime))
fi

timediff=$(($currtime-$ts_from_file)) 
if [[ $timediff -gt 2 ]]
then
	echo "Latest data timestamp is $timediff seconds ago"
	exit 1
fi

echo $file
exit 0
