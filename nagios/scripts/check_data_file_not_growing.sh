#!/bin/bash

INTERVAL=8

FILENAME1=`ls -tr /big/data/ac_rm_1/a[01]* | tail -1`
if [[ $? -ne 0 ]]
then
	echo "No data files in /big/data/ac_rm_1"
	exit 1
fi

FILESIZE1=`stat -c %s "$FILENAME1" 2> /dev/null`
   
sleep $INTERVAL
   
FILENAME2=`ls -tr /big/data/ac_rm_1/a[01]* | tail -1`
FILESIZE2=`stat -c %s "$FILENAME2" 2> /dev/null`
   
if [[ $FILENAME1 == $FILENAME2 ]]
then
      if [ $FILESIZE1 -ne $FILESIZE2 ] ; then
         echo "OK - Size change for $FILENAME1 in $INTERVAL seconds is $(($FILESIZE2 - $FILESIZE1)) bytes"
         exit 0
      else
         echo "CRITICAL - $FILENAME1 is $FILESIZE2 bytes"
         exit 2
      fi
else
	echo "New data file $FILENAME2 is not the same as $FILENAME1; rechecking..."
fi

FILENAME1=`ls -tr /big/data/ac_rm_1/a[01]* | tail -1`
FILESIZE1=`stat -c %s "$FILENAME1" 2> /dev/null`
   
sleep $INTERVAL
   
FILENAME2=`ls -tr /big/data/ac_rm_1/a[01]* | tail -1`
FILESIZE2=`stat -c %s "$FILENAME2" 2> /dev/null`
   
if [[ $FILENAME1 == $FILENAME2 ]]
then
      if [ $FILESIZE1 -ne $FILESIZE2 ] ; then
       echo "OK - Size change for $FILENAME1 in $INTERVAL seconds is $(($FILESIZE2 - $FILESIZE1)) bytes"
         exit 0
      else
         echo "CRITICAL - $FILENAME1 is $FILESIZE2 bytes"
         exit 2
      fi
else
	echo "New data file $FILENAME2 is STILL not the same as $FILENAME1; we have a problem"
	exit 3 
fi
