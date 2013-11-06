#!/bin/bash
# From a data file, compares the number of different sequence numbers in the
# rm2ac_ctr counter to the number of lines in the file. If the loop
# interval of urms.c is the same as that of wrfiles_ac_rm, there should
# be a 1-to-1 correspondence in the sequence number and a line of data. If
# the file writing is twice the speed as the sending of messages from the controller (the 
# current setting), we should see a 50% maximum.  Right now, it's hovering around 47%,
# so we'll give it a 7% margin

FILE=$1
THRESHOLD_PCT=40

NUM_SEQ_NUM=`cat $FILE | awk '{print $61}' | uniq -c | wc -l`
NUMLINES=`wc -l $FILE | awk '{print $1}'`
if [[ $NUMLINES -eq 0 ]]
then
	NUMLINES=1
fi
PCT=$(($NUM_SEQ_NUM*100/$NUMLINES))
echo $FILE $NUMLINES $NUM_SEQ_NUM $PCT
if [[ $PCT -lt $THRESHOLD_PCT ]]
then
	exit 1
fi
exit 0
