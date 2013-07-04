#!/bin/bash

# 16:41:28.578

TIME1STR=$1
TIME2STR=$2

TIMETEMP=`echo $TIME1STR | sed -e '{s/:/ /g}' -e '{s/\./ /g}'`
TIME1=`echo $TIMETEMP | awk '{print (((($1*3600)+($2*60)+($3))*1000))+$4}'`

TIMETEMP=`echo $TIME2STR | sed -e '{s/:/ /g}' -e '{s/\./ /g}'`
TIME2=`echo $TIMETEMP | awk '{print (((($1*3600)+($2*60)+($3))*1000))+$4}'`


echo TIMETEMP $TIMETEMP TIME1 $TIME1
