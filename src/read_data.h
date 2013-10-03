#ifndef _READ_DATA_H_
#define _READ_DATA_H_

#include "temp_global.h"
#include "variables2.h"
#include "variables.h"
#include "interface.h"

extern int old_phase6_signal_status;
extern int ramp_flag;
extern int signal_flag;

int get_new_data(char str[],struct signal_variables *psignal_data,struct ramp_variables *pramp_data);

bool check_mainline_occ_health_lead(struct ramp_variables* pramp_data,int lane_id);

bool check_mainline_occ_health_trail(struct ramp_variables* pramp_data,int lane_id);

bool check_mainline_vol_health_lead(struct ramp_variables* pramp_data,int lane_id);

bool check_mainline_vol_health_trail(struct ramp_variables* pramp_data,int lane_id);

float get_mainline_average_occupancy(struct ramp_variables* pramp_data);

float get_mainline_average_volume(struct ramp_variables* pramp_data);

int move_ramp_data_forward(struct ramp_variables* pramp_data);

int get_new_ramp_data(struct ramp_variables* pramp_data);

int check_new_ramp_data(struct ramp_variables* pramp_data);

int replace_ramp_data(struct ramp_variables* pramp_data);

int is_value_negative();

int read_ramp_data(struct ramp_variables* pramp_data);

int read_real_time_data(struct signal_variables* psignal_data);

#endif
