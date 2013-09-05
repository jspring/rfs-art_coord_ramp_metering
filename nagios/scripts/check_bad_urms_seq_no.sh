#!/bin/bash
#
# Looks at the number of different values from the message counter
# from the ramp meter computer. The latter sends a message to the
# arterial computer whenever it polls the ramp meter. It increments
# its message counter every time, so this counter should be constantly
# incrementing until it rolls over at 255. If it stops, then there are
# no messages being received from the ramp meter computer, and we should
# restart the data collection (since there's no way to gracefully reset
# the tcp connection on a listener).

FILE=$1
THRESHOLD_PCT=20

NUM_SEQ_NUM=`tail -200 $FILE | awk '{print $61}' | uniq -c | wc -l`

if [[ $NUM_SEQ_NUM -lt 9 ]]
then
	exit 1
fi
exit 0
