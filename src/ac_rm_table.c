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
 
atsc_typ atsc;
get_long_status8_resp_mess_typ status;
timestamp_t timestamp;

/**
 *      This array is used to specify how to read in the DB vars.
 */
db_var_spec_t db_vars_ac_rm[] =
{
        {DB_ATSC_VAR, sizeof(atsc_typ), &atsc},
        {DB_TSCP_STATUS_VAR, sizeof(get_long_status8_resp_mess_typ), &status}
};
int num_db_vars = (sizeof(db_vars_ac_rm)/sizeof(db_var_spec_t));

/** This array is used to specify the output format of the data file.
 */
data_log_column_spec_t file_spec[] = 
{
        {"HH:MM:SS.SSS ", &timestamp, BASE_TIMESTAMP, REPLAY_TIME},		//###1
//get_long_status8_resp_mess_typ
        {"MastrClck %hhu ",   &status.master_clock, BASE_CHAR, REPLAY_USE},		//###2
        {"ActPh %hhx ",   &status.active_phase, BASE_CHAR, REPLAY_USE},		//###3
        {"PhCll %hhx ",   &status.phase_call, BASE_CHAR, REPLAY_USE},			//###4
        {"PdCll %hhx ",   &status.ped_call, BASE_CHAR, REPLAY_USE},			//###5
        {"INTRVL %hhx ",   &status.interval, BASE_CHAR, REPLAY_USE},			//###6
        {"GYOV %hhx ",   &status.green_yellow_overlap, BASE_CHAR, REPLAY_USE},		//###6
        {"Det1-8 %hhx ",   &status.presence1, BASE_CHAR, REPLAY_USE},			//###7
        {"Det9-16 %hhx ",   &status.presence2, BASE_CHAR, REPLAY_USE},			//###8
        {"Det17-24 %hhx ",   &status.presence3, BASE_CHAR, REPLAY_USE},			//###9
        {"Det24-32 %hhx ",   &status.presence4, BASE_CHAR, REPLAY_USE},			//###10
        {"%hhu ",   &status.seq_number, BASE_CHAR, REPLAY_USE},			//###11
//atsc_typ
        {"%hhx ",   &atsc.phase_status_greens[0], BASE_CHAR, REPLAY_USE},	//###12
        {"%hhx ",   &atsc.phase_status_greens[1], BASE_CHAR, REPLAY_USE},	//###13
        {"%hhx ",   &atsc.phase_status_greens[2], BASE_CHAR, REPLAY_USE},	//###14
        {"%hhx ",   &atsc.phase_status_greens[3], BASE_CHAR, REPLAY_USE},	//###15
        {"%hhx ",   &atsc.phase_status_greens[4], BASE_CHAR, REPLAY_USE},	//###16
        {"%hhx ",   &atsc.phase_status_greens[5], BASE_CHAR, REPLAY_USE},	//###17
        {"%hhx ",   &atsc.phase_status_greens[6], BASE_CHAR, REPLAY_USE},	//###18
        {"%hhx ",   &atsc.phase_status_greens[7], BASE_CHAR, REPLAY_USE},	//###19
        {"%hhx ",   &atsc.phase_status_yellows[0], BASE_CHAR, REPLAY_USE},	//###20
        {"%hhx ",   &atsc.phase_status_yellows[1], BASE_CHAR, REPLAY_USE},	//###21
        {"%hhx ",   &atsc.phase_status_yellows[2], BASE_CHAR, REPLAY_USE},	//###22
        {"%hhx ",   &atsc.phase_status_yellows[3], BASE_CHAR, REPLAY_USE},	//###23
        {"%hhx ",   &atsc.phase_status_yellows[4], BASE_CHAR, REPLAY_USE},	//###24
        {"%hhx ",   &atsc.phase_status_yellows[5], BASE_CHAR, REPLAY_USE},	//###25
        {"%hhx ",   &atsc.phase_status_yellows[6], BASE_CHAR, REPLAY_USE},	//###26
        {"%hhx ",   &atsc.phase_status_yellows[7], BASE_CHAR, REPLAY_USE},	//###27
        {"%hhx ",   &atsc.phase_status_reds[0], BASE_CHAR, REPLAY_USE},		//###28
        {"%hhx ",   &atsc.phase_status_reds[1], BASE_CHAR, REPLAY_USE},		//###29
        {"%hhx ",   &atsc.phase_status_reds[2], BASE_CHAR, REPLAY_USE},		//###30
        {"%hhx ",   &atsc.phase_status_reds[3], BASE_CHAR, REPLAY_USE},		//###31
        {"%hhx ",   &atsc.phase_status_reds[4], BASE_CHAR, REPLAY_USE},		//###32
        {"%hhx ",   &atsc.phase_status_reds[5], BASE_CHAR, REPLAY_USE},		//###33
        {"%hhx ",   &atsc.phase_status_reds[6], BASE_CHAR, REPLAY_USE},		//###34
        {"%hhx ",   &atsc.phase_status_reds[7], BASE_CHAR, REPLAY_USE},		//###35
        {"%hhx ",   &atsc.phase_call[0], BASE_CHAR, REPLAY_USE},		//###36
        {"%hhx ",   &atsc.phase_call[1], BASE_CHAR, REPLAY_USE},		//###37
        {"%hhx ",   &atsc.phase_call[2], BASE_CHAR, REPLAY_USE},		//###38
        {"%hhx ",   &atsc.phase_call[3], BASE_CHAR, REPLAY_USE},		//###39
        {"%hhx ",   &atsc.phase_call[4], BASE_CHAR, REPLAY_USE},		//###40
        {"%hhx ",   &atsc.phase_call[5], BASE_CHAR, REPLAY_USE},		//###41
        {"%hhx ",   &atsc.phase_call[6], BASE_CHAR, REPLAY_USE},		//###42
        {"%hhx ",   &atsc.phase_call[7], BASE_CHAR, REPLAY_USE},		//###43
        {"%hhx ",   &atsc.ped_call[0], BASE_CHAR, REPLAY_USE},			//###44
        {"%hhx ",   &atsc.ped_call[1], BASE_CHAR, REPLAY_USE},			//###45
        {"%hhx ",   &atsc.ped_call[2], BASE_CHAR, REPLAY_USE},			//###46
        {"%hhx ",   &atsc.ped_call[3], BASE_CHAR, REPLAY_USE},			//###47
        {"%hhx ",   &atsc.ped_call[4], BASE_CHAR, REPLAY_USE},			//###48
        {"%hhx ",   &atsc.ped_call[5], BASE_CHAR, REPLAY_USE},			//###49
        {"%hhx ",   &atsc.ped_call[6], BASE_CHAR, REPLAY_USE},			//###50
        {"%hhx ",   &atsc.ped_call[7], BASE_CHAR, REPLAY_USE},			//###51



        //Current acceleration received from n cars ahead
//        {"%.2f ",   &m56_m430.host_rx_xg_491, BASE_FLOAT, REPLAY_USE},		//###7
//        {"%u ",   &m56_m431.message_timeout_counter, BASE_INT, REPLAY_USE},	//###8
};

#define NUM_FILE_COLUMNS sizeof(file_spec)/sizeof(data_log_column_spec_t)
int num_file_columns = sizeof(file_spec)/sizeof(data_log_column_spec_t);
