#include <db_include.h>
#include <db_utils.h>
#include <path_gps_lib.h>
#include "data_log.h"
#include "atsc.h"
#include "atsc_clt_vars.h"

atsc_typ atsc;
timestamp_t timestamp;

/**
 *      This array is used to specify how to read in the DB vars.
 */
db_var_spec_t db_vars_ac_rm[] =
{
        {DB_ATSC_VAR, sizeof(atsc_typ), &atsc}
};
int num_db_vars = (sizeof(db_vars_ac_rm)/sizeof(db_var_spec_t));

/** This array is used to specify the output format of the data file.
 */
data_log_column_spec_t file_spec[] = 
{
        {"HH:MM:SS.SSS ", &timestamp, BASE_TIMESTAMP, REPLAY_TIME},                            //###1
//atsc_typ
        {"%hhu ",   &atsc.phase_status_greens[0], BASE_CHAR, REPLAY_USE},	//###2
        {"%hhu ",   &atsc.phase_status_greens[1], BASE_CHAR, REPLAY_USE},	//###3
        {"%hhu ",   &atsc.phase_status_greens[2], BASE_CHAR, REPLAY_USE},	//###4
        {"%hhu ",   &atsc.phase_status_greens[3], BASE_CHAR, REPLAY_USE},	//###5
        {"%hhu ",   &atsc.phase_status_greens[4], BASE_CHAR, REPLAY_USE},	//###6
        {"%hhu ",   &atsc.phase_status_greens[5], BASE_CHAR, REPLAY_USE},	//###7
        {"%hhu ",   &atsc.phase_status_greens[6], BASE_CHAR, REPLAY_USE},	//###8
        {"%hhu ",   &atsc.phase_status_greens[7], BASE_CHAR, REPLAY_USE},	//###9
        {"%hhu ",   &atsc.phase_status_yellows[0], BASE_CHAR, REPLAY_USE},	//###10
        {"%hhu ",   &atsc.phase_status_yellows[1], BASE_CHAR, REPLAY_USE},	//###11
        {"%hhu ",   &atsc.phase_status_yellows[2], BASE_CHAR, REPLAY_USE},	//###12
        {"%hhu ",   &atsc.phase_status_yellows[3], BASE_CHAR, REPLAY_USE},	//###13
        {"%hhu ",   &atsc.phase_status_yellows[4], BASE_CHAR, REPLAY_USE},	//###14
        {"%hhu ",   &atsc.phase_status_yellows[5], BASE_CHAR, REPLAY_USE},	//###15
        {"%hhu ",   &atsc.phase_status_yellows[6], BASE_CHAR, REPLAY_USE},	//###16
        {"%hhu ",   &atsc.phase_status_yellows[7], BASE_CHAR, REPLAY_USE},	//###17
        {"%hhu ",   &atsc.phase_status_reds[0], BASE_CHAR, REPLAY_USE},		//###18
        {"%hhu ",   &atsc.phase_status_reds[1], BASE_CHAR, REPLAY_USE},		//###19
        {"%hhu ",   &atsc.phase_status_reds[2], BASE_CHAR, REPLAY_USE},		//###20
        {"%hhu ",   &atsc.phase_status_reds[3], BASE_CHAR, REPLAY_USE},		//###21
        {"%hhu ",   &atsc.phase_status_reds[4], BASE_CHAR, REPLAY_USE},		//###22
        {"%hhu ",   &atsc.phase_status_reds[5], BASE_CHAR, REPLAY_USE},		//###23
        {"%hhu ",   &atsc.phase_status_reds[6], BASE_CHAR, REPLAY_USE},		//###24
        {"%hhu ",   &atsc.phase_status_reds[7], BASE_CHAR, REPLAY_USE},		//###25
        {"%hhu ",   &atsc.phase_call[0], BASE_CHAR, REPLAY_USE},		//###26
        {"%hhu ",   &atsc.phase_call[1], BASE_CHAR, REPLAY_USE},		//###27
        {"%hhu ",   &atsc.phase_call[2], BASE_CHAR, REPLAY_USE},		//###28
        {"%hhu ",   &atsc.phase_call[3], BASE_CHAR, REPLAY_USE},		//###29
        {"%hhu ",   &atsc.phase_call[4], BASE_CHAR, REPLAY_USE},		//###30
        {"%hhu ",   &atsc.phase_call[5], BASE_CHAR, REPLAY_USE},		//###31
        {"%hhu ",   &atsc.phase_call[6], BASE_CHAR, REPLAY_USE},		//###32
        {"%hhu ",   &atsc.phase_call[7], BASE_CHAR, REPLAY_USE},		//###33
        {"%hhu ",   &atsc.ped_call[0], BASE_CHAR, REPLAY_USE},			//###34
        {"%hhu ",   &atsc.ped_call[1], BASE_CHAR, REPLAY_USE},			//###35
        {"%hhu ",   &atsc.ped_call[2], BASE_CHAR, REPLAY_USE},			//###36
        {"%hhu ",   &atsc.ped_call[3], BASE_CHAR, REPLAY_USE},			//###37
        {"%hhu ",   &atsc.ped_call[4], BASE_CHAR, REPLAY_USE},			//###38
        {"%hhu ",   &atsc.ped_call[5], BASE_CHAR, REPLAY_USE},			//###39
        {"%hhu ",   &atsc.ped_call[6], BASE_CHAR, REPLAY_USE},			//###40
        {"%hhu ",   &atsc.ped_call[7], BASE_CHAR, REPLAY_USE},	//###41



        //Current acceleration received from n cars ahead
//        {"%.2f ",   &m56_m430.host_rx_xg_491, BASE_FLOAT, REPLAY_USE},		//###7
//        {"%u ",   &m56_m431.message_timeout_counter, BASE_INT, REPLAY_USE},	//###8
};

#define NUM_FILE_COLUMNS sizeof(file_spec)/sizeof(data_log_column_spec_t)
int num_file_columns = sizeof(file_spec)/sizeof(data_log_column_spec_t);
