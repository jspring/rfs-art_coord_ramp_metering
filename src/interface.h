#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#include "temp_global.h"
#include "variables2.h"
#include "variables.h"

extern float get_current_time();
/*
float get_occ_main_lead_1(void);

float get_occ_main_trail_1(void);

float get_occ_main_lead_2(void);

float get_occ_main_trail_2(void);

float get_occ_main_lead_3(void);

float get_occ_main_trail_3(void);

int get_vol_main_lead_1(void);

int get_vol_main_trail_1(void);

int get_vol_main_lead_2(void);

int get_vol_main_trail_2(void);

int get_vol_main_lead_3(void);

int get_vol_main_trail_3(void);
*/

int get_status_main_lead_1(void);

int get_status_main_lead_2(void);

int get_status_main_lead_3(void);

int get_status_main_trail_1(void);

int get_status_main_trail_2(void);

int get_status_main_trail_3(void);
/*
float get_occ_metered_queue_1(void);

int get_vol_metered_queue_1(void);

float get_occ_metered_queue_2(void);

int get_vol_metered_queue_2(void);

float get_occ_metered_queue_3(void);

int get_vol_metered_queue_3(void);

int get_vol_metered_passage_1(void);

int get_vol_metered_passage_2(void);

int get_vol_metered_passage_3(void);

int get_vol_metered_demand_1(void);

int get_vol_metered_demand_2(void);

int get_vol_metered_demand_3(void);

float get_rate_metered_1(void);

float get_rate_metered_2(void);

float get_rate_metered_3(void);
*/

float get_data_time(void);

float get_update_time_ramp(struct ramp_variables *pramp_data);

//int get_phase5_signal_status();

//int get_phase8_signal_status();

//int get_phase5_approach1_status();

//int get_phase5_approach2_status();

//int get_phase5_stopbar_status();

//int get_phase8_approach_status();

//int get_phase8_stopbar_status();

//int get_phase6_signal_status();

//int get_phase7_signal_status();

//int get_phase3_signal_status();


int set_new_max_green_phase3(int x);
#endif
