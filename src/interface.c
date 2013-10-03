#include <db_include.h>

#include "interface.h"

/*
float get_occ_main_lead_1(void)
{
	return lead_occ_0;
}
float get_occ_main_trail_1(void)
{
	return trail_occ_0;
}
float get_occ_main_lead_2(void)
{
	return lead_occ_1;
}
float get_occ_main_trail_2(void)
{
	return trail_occ_1;
}
float get_occ_main_lead_3(void)
{
	return lead_occ_2;
}
float get_occ_main_trail_3(void)
{
	return trail_occ_2;
}

int get_vol_main_lead_1(void)
{
	return lead_vol_0;
}
int get_vol_main_trail_1(void)
{
	return trail_vol_0;
}
int get_vol_main_lead_2(void)
{
	return lead_vol_1;
}
int get_vol_main_trail_2(void)
{
	return trail_vol_1;
}
int get_vol_main_lead_3(void)
{
	return lead_vol_2;
}
int get_vol_main_trail_3(void)
{
	return trail_vol_2;
}
*/



int get_status_main_lead_1(void)
{
	return lead_stat_0;
}
int get_status_main_lead_2(void)
{
	return lead_stat_1;
}
int get_status_main_lead_3(void)
{
	return lead_stat_2;
}
int get_status_main_trail_1(void)
{
	return trail_stat_0;
}
int get_status_main_trail_2(void)
{
	return trail_stat_1;
}
int get_status_main_trail_3(void)
{
	return trail_stat_2;
}




/*
float get_occ_metered_queue_1(void)
{
	return queue_occ_0;
}

int get_vol_metered_queue_1(void)
{
	return queue_vol_0;
}
float get_occ_metered_queue_2(void)
{
	return queue_occ_1;
}
int get_vol_metered_queue_2(void)
{
	return queue_vol_1;
}
float get_occ_metered_queue_3(void)
{
	return (float) queue_occ_2;
}
int get_vol_metered_queue_3(void)
{
	return queue_vol_2;
}

int get_vol_metered_passage_1(void)
{
	return passage_vol_0;
}

int get_vol_metered_passage_2(void)
{
	return passage_vol_1;
}

int get_vol_metered_passage_3(void)
{
	return passage_vol_2;
}
int get_vol_metered_demand_1(void)
{
	return demand_vol_0;
}

int get_vol_metered_demand_2(void)
{
	return demand_vol_1;
}

int get_vol_metered_demand_3(void)
{
	return demand_vol_2;
}
*/



float get_rate_metered_1(void)
{
	return rate_0;
}
float get_rate_metered_2(void)
{
	return rate_1;
}
float get_rate_metered_3(void)
{
	return rate_2;
}

float get_data_time(void)
{
	return (float) hr*3600+min*60+sec;
}

float get_update_time_ramp(struct ramp_variables *pramp_data)
{
	return (float) hr*3600+min*60+sec;
}

int get_phase5_signal_status()
{
	if ((greens>>4)%2)
		return 1;
	else if ((yellows>>4)%2)
		return 2;
	else
	return 0;
}

int get_phase8_signal_status()
{
	if ((green_yellow_overlap>>1)%2)
		return 1;
	else
		return 0;
}
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
int get_phase8_approach_status()
{
	//return rand() / (double)RAND_MAX<0.4;
	return (presence1>>1)%2;
}
int get_phase8_stopbar_status()
{
	return (presence3>>5)%2;
}
int get_phase6_signal_status()
{
	if ((greens>>5)%2)
		return 1;
	else if ((yellows>>5)%2)
		return 2;
	else
		return 0;
}
int get_phase7_signal_status()
{
	if ((greens>>6)%2)
		return 1;
	else if ((yellows>>6)%2)
		return 2;
	else
	return 0;
}
int get_phase3_signal_status()
{
	if ((greens>>2)%2)
		return 1;
	else if ((yellows>>2)%2)
		return 2;
	else
	return 0;
}

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

