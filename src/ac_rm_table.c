#include <db_include.h>
#include <db_utils.h>
#include <path_gps_lib.h>
#include "data_log.h"
#include "atsc.h"
#include "atsc_clt_vars.h"
#include <ab3418_lib.h>
#include <ab3418comm.h>

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
timestamp_t timestamp;

/**
 *      This array is used to specify how to read in the DB vars.
 */
db_var_spec_t db_vars_ac_rm[] =
{
        {DB_SHORT_STATUS_VAR, sizeof(get_short_status_resp_t), &short_status},
        {DB_TSCP_STATUS_VAR, sizeof(get_long_status8_resp_mess_typ), &status}
};
int num_db_vars = (sizeof(db_vars_ac_rm)/sizeof(db_var_spec_t));

/** This array is used to specify the output format of the data file.
 */
data_log_column_spec_t file_spec[] = 
{
        {"HH:MM:SS.SSS ", &timestamp, BASE_TIMESTAMP, REPLAY_TIME},		//###1
//get_long_status8_resp_mess_typ
        {"%hhu ",   &status.master_clock, BASE_CHAR, REPLAY_USE},		//###2
        {"%hhu ",   &status.seq_number, BASE_CHAR, REPLAY_USE},			//###3
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
        {"%hhx ",   &short_status.greens, BASE_CHAR, REPLAY_USE}		//###15
//        {"%hhu ",   &status.err, BASE_CHAR, REPLAY_USE},			//###16
//        {"%hhu ",   &status.errindex, BASE_CHAR, REPLAY_USE},			//###17

        //Current acceleration received from n cars ahead
//        {"%.2f ",   &m56_m430.host_rx_xg_491, BASE_FLOAT, REPLAY_USE},		//###7
//        {"%u ",   &m56_m431.message_timeout_counter, BASE_INT, REPLAY_USE},	//###8
};

#define NUM_FILE_COLUMNS sizeof(file_spec)/sizeof(data_log_column_spec_t)
int num_file_columns = sizeof(file_spec)/sizeof(data_log_column_spec_t);
