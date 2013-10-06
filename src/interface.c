#include <db_include.h>

#include "interface.h"

float get_data_time(void)
{
	return (float) hr*3600+min*60+sec;
}

float get_update_time_ramp(struct ramp_variables *pramp_data)
{
	return (float) hr*3600+min*60+sec;
}
/*
int get_phase5_approach1_status()
{
	return (presence3%2);
}
int get_phase5_approach2_status()
{
	return (presence3>>2)%2;
}
int get_phase5_stopbar_status()
{
	return (presence2>>4)%2;
}

// This is actually the advance detector for phase 6 RT
// (i.e. overlap B)
int get_phase8_approach_status()
{
	//return rand() / (double)RAND_MAX<0.4;
	return (presence1>>1)%2;
}

// This is actually the stop bar detector for phase 6 RT
// (i.e. overlap B)
int get_phase8_stopbar_status()
{
	return (presence3>>5)%2;
}
*/

float get_current_time()
{
	timestamp_t ts;

	get_current_timestamp(&ts);
	return (TS_TO_MS(&ts)/1000.0);
//	return (float) hr*3600+min*60+sec;
}

int set_new_max_green_phase3(int x)
{
	return 0;
}

