#include <db_include.h>
#include <path_gps_lib.h>
#include "data_log.h"
#include "atsc.h"
#include "atsc_clt_vars.h"
#include <ab3418_lib.h>
#include <ab3418comm.h>
#include <urms.h>
#include "wrfiles_ac_rm.h"
#include "variables2.h"

char *interval_str[] = {
"Walk", 
"Don't walk       ", 
"Min green        ",
"Unused           ",
"Added initial    ", 
"Passage - resting",
"Max gap          ", 
"Min gap          ", 
"Red rest         ",
"Preemption       ",
"Stop time        ",
"Red revert       ",
"Max termination  ",
"Gap termination  ",
"Force off        ",
"Red clearance    "
};
 
get_long_status8_resp_mess_typ status;
get_short_status_resp_t short_status;
phase_status_t	phase_status;
db_urms_status_t db_urms_status;
urms_datafile_t urms_datafile;
timestamp_t timestamp;
db_ramp_data_t db_ramp_data;
db_signal_data_t db_signal_data;

/**
 *      This array is used to specify how to read in the DB vars.
 */
db_var_spec_t db_vars_ac_rm[] =
{
        {DB_SHORT_STATUS_VAR, sizeof(get_short_status_resp_t), &short_status},
        {DB_TSCP_STATUS_VAR, sizeof(get_long_status8_resp_mess_typ), &status},
        {DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status},
        {DB_URMS_DATAFILE_VAR, sizeof(urms_datafile_t), &urms_datafile},
        {DB_PHASE_STATUS_VAR, sizeof(phase_status_t), &phase_status},
        {DB_SIGNAL_DATA_VAR, sizeof(db_signal_data_t), &db_signal_data},
        {DB_RAMP_DATA_VAR, sizeof(db_ramp_data_t), &db_ramp_data}
};
int num_ac_rm_vars = (sizeof(db_vars_ac_rm)/sizeof(db_var_spec_t));

/** This array is used to specify the output format of the data file.
 */
data_log_column_spec_t file_spec[] = 
{
        {"HH:MM:SS.SSS ", &timestamp, BASE_TIMESTAMP, REPLAY_TIME},		//###1
//get_long_status8_resp_mess_typ
        {"%03.3hhu ",   &status.master_clock, BASE_CHAR, REPLAY_USE},		//###2
        {"%03.3hhu ",   &status.seq_number, BASE_CHAR, REPLAY_USE},		//###3
        {"%hhx ",   &status.status, BASE_CHAR, REPLAY_USE},			//###4
        {"%hhx ",   &status.pattern, BASE_CHAR, REPLAY_USE},			//###5
        {"%hhx ",   &status.active_phase, BASE_CHAR, REPLAY_USE},		//###6
        {"%hhx ",   &status.phase_call, BASE_CHAR, REPLAY_USE},			//###7
        {"%hhx ",   &status.ped_call, BASE_CHAR, REPLAY_USE},			//###8
        {"%hhx ",   &status.interval, BASE_CHAR, REPLAY_USE},			//###9
        {"%hhx ",   &status.green_yellow_overlap, BASE_CHAR, REPLAY_USE},	//###10
        {"%hhx ",   &status.presence1, BASE_CHAR, REPLAY_USE},			//###11
        {"%hhx ",   &status.presence2, BASE_CHAR, REPLAY_USE},			//###12
        {"%hhx ",   &status.presence3, BASE_CHAR, REPLAY_USE},			//###13
        {"%hhx ",   &status.presence4, BASE_CHAR, REPLAY_USE},			//###14
        {"%hhx ",   &phase_status.greens, BASE_CHAR, REPLAY_USE},		//###15
        {"%hhx ",   &phase_status.yellows, BASE_CHAR, REPLAY_USE},		//###16
        {"%hhx ",   &phase_status.reds, BASE_CHAR, REPLAY_USE},			//###17

        {"%hhu ",   &db_urms_status.mainline_stat[0].lead_stat, BASE_CHAR, REPLAY_USE},	//###18
        {"%hhu ",   &db_urms_status.mainline_stat[0].lead_vol, BASE_CHAR, REPLAY_USE},	//###19
        {"%.1f ",   &urms_datafile.mainline_lead_occ[0], BASE_FLOAT, REPLAY_USE},	//###20
        {"%hhu ",   &db_urms_status.mainline_stat[0].trail_stat, BASE_CHAR, REPLAY_USE},//###21
        {"%hhu ",   &db_urms_status.mainline_stat[0].trail_vol, BASE_CHAR, REPLAY_USE},	//###22
        {"%.1f ",   &urms_datafile.mainline_trail_occ[0], BASE_FLOAT, REPLAY_USE},	//###23

        {"%hhu ",   &db_urms_status.mainline_stat[1].lead_stat, BASE_CHAR, REPLAY_USE},	//###24
        {"%hhu ",   &db_urms_status.mainline_stat[1].lead_vol, BASE_CHAR, REPLAY_USE},	//###25
        {"%.1f ",   &urms_datafile.mainline_lead_occ[1], BASE_FLOAT, REPLAY_USE},	//###26
        {"%hhu ",   &db_urms_status.mainline_stat[1].trail_stat, BASE_CHAR, REPLAY_USE},//###27
        {"%hhu ",   &db_urms_status.mainline_stat[1].trail_vol, BASE_CHAR, REPLAY_USE},	//###28
        {"%.1f ",   &urms_datafile.mainline_trail_occ[1], BASE_FLOAT, REPLAY_USE},	//###29

        {"%hhu ",   &db_urms_status.mainline_stat[2].lead_stat, BASE_CHAR, REPLAY_USE},	//###30
        {"%hhu ",   &db_urms_status.mainline_stat[2].lead_vol, BASE_CHAR, REPLAY_USE},	//###31
        {"%hhu ",   &db_urms_status.mainline_stat[2].trail_stat, BASE_CHAR, REPLAY_USE},//###32
        {"%.1f ",   &urms_datafile.mainline_lead_occ[2], BASE_FLOAT, REPLAY_USE},	//###33
        {"%hhu ",   &db_urms_status.mainline_stat[2].trail_vol, BASE_CHAR, REPLAY_USE},	//###34
        {"%.1f ",   &urms_datafile.mainline_trail_occ[2], BASE_FLOAT, REPLAY_USE},	//###35

        {"%hhu ",   &db_urms_status.metered_lane_stat[0].demand_vol, BASE_CHAR, REPLAY_USE},	//###36
        {"%hhu ",   &db_urms_status.metered_lane_stat[0].passage_vol, BASE_CHAR, REPLAY_USE},	//###37
        {"%.1f ",   &urms_datafile.queue_occ[0], BASE_FLOAT, REPLAY_USE},	//###38
        {"%hhu ",   &db_urms_status.queue_stat[0].vol, BASE_CHAR, REPLAY_USE},	//###39
        {"%hu ",   &urms_datafile.metering_rate[0], BASE_SHORT, REPLAY_USE},	//###40
        {"%hhu ",   &db_urms_status.cmd_src[0], BASE_CHAR, REPLAY_USE},		//###41
        {"%hhu ",   &db_urms_status.action[0], BASE_CHAR, REPLAY_USE},		//###42
        {"%hhu ",   &db_urms_status.plan[0], BASE_CHAR, REPLAY_USE},		//###43

        {"%hhu ",   &db_urms_status.metered_lane_stat[1].demand_vol, BASE_CHAR, REPLAY_USE},	//###44
        {"%hhu ",   &db_urms_status.metered_lane_stat[1].passage_vol, BASE_CHAR, REPLAY_USE},	//###45
        {"%.1f ",   &urms_datafile.queue_occ[1], BASE_FLOAT, REPLAY_USE},	//###46
        {"%hhu ",   &db_urms_status.queue_stat[1].vol, BASE_CHAR, REPLAY_USE},	//###47
        {"%hu ",   &urms_datafile.metering_rate[1], BASE_SHORT, REPLAY_USE},	//###48
        {"%hhu ",   &db_urms_status.cmd_src[1], BASE_CHAR, REPLAY_USE},		//###49
        {"%hhu ",   &db_urms_status.action[1], BASE_CHAR, REPLAY_USE},		//###50
        {"%hhu ",   &db_urms_status.plan[1], BASE_CHAR, REPLAY_USE},		//###51

        {"%hhu ",   &db_urms_status.metered_lane_stat[2].demand_vol, BASE_CHAR, REPLAY_USE},	//###52
        {"%hhu ",   &db_urms_status.metered_lane_stat[2].passage_vol, BASE_CHAR, REPLAY_USE},	//###53
        {"%.1f ",   &urms_datafile.queue_occ[2], BASE_FLOAT, REPLAY_USE},	//###54
        {"%hhu ",   &db_urms_status.queue_stat[2].vol, BASE_CHAR, REPLAY_USE},	//###55
        {"%hu ",    &urms_datafile.metering_rate[2], BASE_SHORT, REPLAY_USE},	//###56
        {"%hhu ",   &db_urms_status.cmd_src[2], BASE_CHAR, REPLAY_USE},		//###57
        {"%hhu ",   &db_urms_status.action[2], BASE_CHAR, REPLAY_USE},		//###58
        {"%hhu ",   &db_urms_status.plan[2], BASE_CHAR, REPLAY_USE},		//###59
        {"%hhu ",   &db_urms_status.rm2rmc_ctr, BASE_CHAR, REPLAY_USE},		//###60
        {"%hhu ",   &db_urms_status.rmc2ac_ctr, BASE_CHAR, REPLAY_USE},		//###61
	{"%hhu ",   &db_urms_status.is_metering, BASE_CHAR, REPLAY_USE},	//###62
	{"%hhu ",   &phase_status.barrier_flag, BASE_CHAR, REPLAY_USE},		//###63
	{"%hhu ",   &db_urms_status.computation_finished, BASE_CHAR, REPLAY_USE}, //###64
	{"%hx ",    &db_urms_status.checksum, BASE_CHAR, REPLAY_USE}, 		//###65
        {"%hu ",    &urms_datafile.metering_rate[2], BASE_SHORT, REPLAY_USE},	//###66
	{"%.1f ",   &db_ramp_data.new_meter_rate, BASE_FLOAT, REPLAY_USE}, 	//###67
	{"%d ",   &db_signal_data.old_max_green, BASE_INT, REPLAY_USE}, 	//###68
	{"%d ",   &db_signal_data.new_max_green, BASE_INT, REPLAY_USE}, 	//###69
	{"%.1f ",   &db_signal_data.LT_occ, BASE_FLOAT, REPLAY_USE}, 		//###70
	{"%.1f ",   &db_signal_data.old_LT_occ, BASE_FLOAT, REPLAY_USE}, 	//###71
	{"%.1f ",   &db_signal_data.RT_occ, BASE_FLOAT, REPLAY_USE}, 		//###72
	{"%.1f ",   &db_signal_data.ramp_queue, BASE_FLOAT, REPLAY_USE}, 	//###73
	{"%.1f ",   &db_signal_data.old_ramp_queue, BASE_FLOAT, REPLAY_USE}, 	//###74
	{"%d ",   &db_signal_data.RT_exceed_num, BASE_INT, REPLAY_USE}, 	//###75
	{"%d ",   &db_signal_data.last_sent_max_green, BASE_INT, REPLAY_USE}, 	//###76
	{"%d ",   &db_signal_data.data_row, BASE_INT, REPLAY_USE}, 		//###77
	{"%d ",   &db_signal_data.regular_remain_cycle, BASE_INT, REPLAY_USE}, 	//###78
	{"%d ",   &db_signal_data.overwrite_remain_cycle, BASE_INT, REPLAY_USE}, //###79
	{"%d ",   &db_signal_data.cycle_passed, BASE_INT, REPLAY_USE}, 		//###80
	{"%.1f ",   &db_signal_data.prev_cycle_terminate_time, BASE_FLOAT, REPLAY_USE}, //###81
	{"%.1f ",   &db_signal_data.current_cycle_terminate_time, BASE_FLOAT, REPLAY_USE}, //###82
	{"%.1f ",   &db_signal_data.prev_queue_reset_time, BASE_FLOAT, REPLAY_USE}, //###83
	{"%u ",   &db_signal_data.regular_release, BASE_INT, REPLAY_USE}, 	//###84
	{"%u ",   &db_signal_data.overwrite_release, BASE_INT, REPLAY_USE}, 	//###85
	{"%d ",   &db_signal_data.realtime_data, BASE_INT, REPLAY_USE}, 	//###86
	{"%.1f ",   &db_ramp_data.data_time, BASE_FLOAT, REPLAY_USE}, 		//###87
	{"%.1f ",   &db_ramp_data.prev_update_data, BASE_FLOAT, REPLAY_USE}, 	//###88
	{"%.1f ",   &db_ramp_data.prev_occ_out, BASE_FLOAT, REPLAY_USE}, 	//###89
	{"%hhu ",   &db_ramp_data.passage_vol[2], BASE_CHAR, REPLAY_USE}, 	//###90
};

#define NUM_FILE_COLUMNS sizeof(file_spec)/sizeof(data_log_column_spec_t)
int num_file_columns = sizeof(file_spec)/sizeof(data_log_column_spec_t);
