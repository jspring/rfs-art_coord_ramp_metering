#!/bin/bash
# From a data file, compares the number of different sequence numbers in the
# GetLongStatus8 message to the number of lines in the file. If the loop
# interval of ab3418comm is the same as that of wrfiles_ac_rm, there should
# be a 1-to-1 correspondence in the sequence number and a line of data. If
# the file writing is twice the speed as the polling of the controller (the 
# current setting), we should see a 50% maximum.  Right now, it's hovering 
# around 30%, but there are two polls sent for each poll loop 
# (GetLongStatus8 and GetShortStatus), so maybe the counter increments for
# both? (Although we should see 25%, not 30%.)

FILE=$1
THRESHOLD_PCT=20

NUM_SEQ_NUM=`cat $FILE | awk '{print $3}' | uniq -c | wc -l`
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
