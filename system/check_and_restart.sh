#!/bin/bash

ps -elf |grep -v grep | grep '/home/art_coord_ramp_metering/src/lnx/wrfiles_ac_rm'

if [[ $? -ne 0 ]]
then
	/home/art_coord_ramp_metering/system/artgo
fi

ps -elf |grep -v grep | grep 'receive_urms_data'

if [[ $? -ne 0 ]]
then
	/home/art_coord_ramp_metering/system/artgo
fi

ps -elf |grep -v grep | grep '/home/atsc/ab3418/lnx/ab3418comm'

if [[ $? -ne 0 ]]
then
	/home/art_coord_ramp_metering/system/urmsgo
fi

ps -elf |grep -v grep | grep '/home/art_coord_ramp_metering/src/lnx/clt_vars'

if [[ $? -ne 0 ]]
then
	/home/art_coord_ramp_metering/system/urmsgo
fi

ps -elf |grep -v grep | grep '/home/path/db/lnx/db_slv'

if [[ $? -ne 0 ]]
then
	/home/art_coord_ramp_metering/system/urmsgo
fi
