#include <db_include.h>
#include <path_gps_lib.h>
#include "data_log.h"
#include "atsc.h"
#include "atsc_clt_vars.h"
#include <ab3418_lib.h>
#include <ab3418comm.h>
#include <urms.h>
#include "wrfiles_ac_rm.h"

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

/**
 *      This array is used to specify how to read in the DB vars.
 */
db_var_spec_t db_vars_ac_rm[] =
{
        {DB_SHORT_STATUS_VAR, sizeof(get_short_status_resp_t), &short_status},
        {DB_TSCP_STATUS_VAR, sizeof(get_long_status8_resp_mess_typ), &status},
        {DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status},
        {DB_URMS_DATAFILE_VAR, sizeof(urms_datafile_t), &urms_datafile},
        {DB_PHASE_STATUS_VAR, sizeof(phase_status_t), &phase_status}
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

        {"%hhu ",   &db_urms_status.mainline_stat[0].lead_vol, BASE_CHAR, REPLAY_USE},	//###18
        {"%.1f ",   &urms_datafile.mainline_lead_occ[0], BASE_FLOAT, REPLAY_USE},	//###19
        {"%hhu ",   &db_urms_status.mainline_stat[0].trail_vol, BASE_CHAR, REPLAY_USE},	//###20
        {"%.1f ",   &urms_datafile.mainline_trail_occ[0], BASE_FLOAT, REPLAY_USE},	//###21

        {"%hhu ",   &db_urms_status.mainline_stat[1].lead_vol, BASE_CHAR, REPLAY_USE},	//###22
        {"%.1f ",   &urms_datafile.mainline_lead_occ[1], BASE_FLOAT, REPLAY_USE},	//###23
        {"%hhu ",   &db_urms_status.mainline_stat[1].trail_vol, BASE_CHAR, REPLAY_USE},	//###24
        {"%.1f ",   &urms_datafile.mainline_trail_occ[1], BASE_FLOAT, REPLAY_USE},	//###25

        {"%hhu ",   &db_urms_status.mainline_stat[2].lead_vol, BASE_CHAR, REPLAY_USE},	//###26
        {"%.1f ",   &urms_datafile.mainline_lead_occ[2], BASE_FLOAT, REPLAY_USE},	//###27
        {"%hhu ",   &db_urms_status.mainline_stat[2].trail_vol, BASE_CHAR, REPLAY_USE},	//###28
        {"%.1f ",   &urms_datafile.mainline_trail_occ[2], BASE_FLOAT, REPLAY_USE},	//###29

        {"%hhu ",   &db_urms_status.metered_lane_stat[0].demand_vol, BASE_CHAR, REPLAY_USE},	//###30
        {"%hhu ",   &db_urms_status.metered_lane_stat[0].passage_vol, BASE_CHAR, REPLAY_USE},	//###31
        {"%.1f ",   &urms_datafile.queue_occ[0], BASE_FLOAT, REPLAY_USE},	//###32
        {"%hhu ",   &db_urms_status.queue_stat[0].vol, BASE_CHAR, REPLAY_USE},	//###33
        {"%hu ",   &urms_datafile.metering_rate[0], BASE_SHORT, REPLAY_USE},	//###34

        {"%hhu ",   &db_urms_status.metered_lane_stat[1].demand_vol, BASE_CHAR, REPLAY_USE},	//###35
        {"%hhu ",   &db_urms_status.metered_lane_stat[1].passage_vol, BASE_CHAR, REPLAY_USE},	//###36
        {"%.1f ",   &urms_datafile.queue_occ[1], BASE_FLOAT, REPLAY_USE},	//###37
        {"%hhu ",   &db_urms_status.queue_stat[1].vol, BASE_CHAR, REPLAY_USE},	//###38
        {"%hu ",   &urms_datafile.metering_rate[1], BASE_SHORT, REPLAY_USE},	//###39

        {"%hhu ",   &db_urms_status.metered_lane_stat[2].demand_vol, BASE_CHAR, REPLAY_USE},	//###40
        {"%hhu ",   &db_urms_status.metered_lane_stat[2].passage_vol, BASE_CHAR, REPLAY_USE},	//###41
        {"%.1f ",   &urms_datafile.queue_occ[2], BASE_FLOAT, REPLAY_USE},	//###42
        {"%hhu ",   &db_urms_status.queue_stat[2].vol, BASE_CHAR, REPLAY_USE},	//###43
        {"%hu ",   &urms_datafile.metering_rate[2], BASE_SHORT, REPLAY_USE},	//###44


//        {"%hhu ",   &status.err, BASE_CHAR, REPLAY_USE},			//###48
//        {"%hhu ",   &status.errindex, BASE_CHAR, REPLAY_USE},			//###49

        //Current acceleration received from n cars ahead
//        {"%.2f ",   &m56_m430.host_rx_xg_491, BASE_FLOAT, REPLAY_USE},	//###7
//        {"%u ",   &m56_m431.message_timeout_counter, BASE_INT, REPLAY_USE},	//###8
};

#define NUM_FILE_COLUMNS sizeof(file_spec)/sizeof(data_log_column_spec_t)
int num_file_columns = sizeof(file_spec)/sizeof(data_log_column_spec_t);
