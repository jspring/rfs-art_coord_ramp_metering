#ifndef _CONTROL_COMPUTE_H_
#define _CONTROL_COMPUTE_H_

#include "temp_global.h"
#include "variables2.h"
#include "variables.h"

int get_detector_occ_count(int data[][MAXCOL], int first_row, int last_row, int det_col);

float get_detector_green_occ(int data[][MAXCOL], int max_row, int det_col, int sig_col);

int check_phase5_signal(int data[][MAXCOL],int max_row);

int check_phase8_detector(int data[][MAXCOL],int max_row);

int check_phase5_detector(int data[][MAXCOL],int max_row);

int check_phase8_signal(int data[][MAXCOL],int max_row);

int check_realtime_data_health(int data[][MAXCOL],int max_row);

float get_meter_rate(struct ramp_variables *pramp_data);

int get_intersection_measurement(struct signal_variables* psignal_data,struct ramp_variables *pramp_data);

bool need_reduce_max_green(struct signal_variables *psignal_data);

bool check_activation_via_queue_occ(struct ramp_variables *pramp_data);

bool check_activation_via_intersection_occ(struct signal_variables* psignal_data);

bool check_activation_via_queue_estimation(struct signal_variables *psignal_data);

float get_ramp_exit_vol(struct ramp_variables* pramp_data, float t1, float t2);

int get_stepsofgreen(int data[][MAXCOL], int current_row, int max_row, int sig_col);

float get_detector_counts(int data[][MAXCOL],int max_row,int sig_col,int det_col);

float get_ramp_queue(struct signal_variables *psignal_data,struct ramp_variables *pramp_data);

bool IS_activation(struct signal_variables* psignal_data,struct ramp_variables* pramp_data);

int get_new_max_green_phase3(struct signal_variables* psignal_data,struct ramp_variables* pramp_data);

float get_ALINEA_rate(struct ramp_variables* pramp_data);

int reset_ramp_queue(struct signal_variables* psignal_data);

#endif