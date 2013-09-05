#!/bin/bash

ps -elf |grep -v grep | grep 'wrfiles_ac_rm'

if [[ $? -ne 0 ]]
then
	/home/art_coord_ramp_metering/system/urmsgo
fi

ps -elf |grep -v grep | grep 'send_urms_data'

if [[ $? -ne 0 ]]
then
	/home/art_coord_ramp_metering/system/urmsgo
fi

ps -elf |grep -v grep | grep '/home/atsc/urms/lnx/urms'

if [[ $? -ne 0 ]]
then
	/home/art_coord_ramp_metering/system/urmsgo
fi

ps -elf |grep -v grep | grep '/home/path/db/lnx/db_slv'

if [[ $? -ne 0 ]]
then
	/home/art_coord_ramp_metering/system/urmsgo
fi
