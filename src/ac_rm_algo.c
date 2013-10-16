/* ac_rm_algo.c - interconverts arterial signal and ramp meter controller data,
**	processes it with a linear optimizing algorithm, and writes the new
**	timing settings back to the database
*/
#include <db_include.h>
#include "msgs.h"
#include "ac_rm_algo.h"
#include "atsc.h"
#include "urms.h"
#include "atsc_clt_vars.h"
#include <ab3418_lib.h>
#include "ab3418comm.h"
#include "max_green_lib.h"
#include "meter_lib.h"
#include "wrfiles_ac_rm.h"

#define _VARIABLES_H_DECLS

#include "variables2.h"
#include "variables.h"
#include "temp_global.h"
#include "interface.h"
#include "init.h"
#include "read_data.h"
#include "control_compute.h"

long int count = 0;

static jmp_buf exit_env;

#define NUM_PHASES	8

static void sig_hand( int code )
{
        if( code == SIGALRM )
                return;
        else
                longjmp( exit_env, code );
}

static int sig_list[] =
{
        SIGINT,
        SIGQUIT,
        SIGTERM,
        SIGALRM,
        ERROR,
};

int db_trig_list_algo[] =  {
	DB_TSCP_STATUS_VAR,
	DB_SHORT_STATUS_VAR,
	DB_URMS_STATUS_VAR
};

int NUM_TRIG_VARS_ALGO = sizeof(db_trig_list_algo)/sizeof(int);

FILE *meter_fp;
FILE *signal_fp;
int test(db_signal_data_t *db_signal_data, db_ramp_data_t *db_ramp_data, unsigned char signal_flag, unsigned char ramp_flag);
int set_globals(phase_status_t *phase_status, get_long_status8_resp_mess_typ *status, db_urms_status_t *db_urms_status, urms_datafile_t *urms_datafile);

int main( int argc, char *argv[]) {

	char hostname[MAXHOSTNAMELEN];
	char *domain = DEFAULT_SERVICE;
	int xport = COMM_OS_XPORT;
	posix_timer_typ *ptmr;
	trig_info_typ trig_info;

	db_urms_t db_urms;
	db_signal_data_t db_signal_data;
	db_ramp_data_t db_ramp_data;
	get_long_status8_resp_mess_typ get_long_status8_resp_mess;
	get_short_status_resp_t short_status;
	urms_datafile_t urms_datafile;
	phase_status_t phase_status;

        int ipc_message_error_ctr = 0;
	unsigned char greenstat = 0;
	unsigned char greenstat_sav = 0;
	phase_timing_t phase_timing;
	struct timespec start_time;
	struct timespec end_time;
	struct timespec call_test;
	struct timespec call_test_sav;
	unsigned char ramp_flag = 0;
	unsigned char ramp_flag_sav = 0;
	unsigned char signal_flag = 0;
	unsigned char signal_flag_sav = 0;
	double timediff = 0;
	float meter_rate_sav = 900.0;

	int interval = 1000;
	int verbose = 0;
	int retval = 0;

	//variables for the intersection measurement
//	int new_max_green = 15;
	int option;

	int flag2;

        /* Read and interpret any user switches. */
        while ((option = getopt(argc, argv, "v")) != EOF) {
                switch(option) {
                case 'v':
                        verbose = 1;
                        break;
                default:
                        printf("Usage: %s\n", argv[0]);
                        printf("    -d <data file> \n");
                        printf("    -v (verbose)\n");
                        exit(EXIT_FAILURE);
                }
        }


	get_local_name(hostname, MAXHOSTNAMELEN);
	if ( ((pclt = db_list_init(argv[0], hostname,
		domain, xport, NULL, 0, 
		db_trig_list_algo, NUM_TRIG_VARS_ALGO)) == NULL))
		exit(EXIT_FAILURE);
	if (setjmp(exit_env) != 0) {
		db_list_done(pclt, NULL, 0, NULL, 0);
		printf("%s: %d IPC message errors\n", argv[0], 
			ipc_message_error_ctr);
		exit(EXIT_SUCCESS);
	} else
		sig_ign(sig_list, sig_hand);

	if ((ptmr = timer_init( interval, ChannelCreate(0))) == NULL) {
		fprintf(stderr, "Unable to initialize delay timer\n");
		exit(EXIT_FAILURE);
	}

        init_signal(&signal_data);
        init_realtimedata(&signal_data);
        init_ramp_data(&ramp_data);
	memset(&db_urms, 0, sizeof(db_urms_t));
	memset(&get_long_status8_resp_mess, 0, sizeof(get_long_status8_resp_mess_typ));
	memset(&short_status, 0, sizeof(get_short_status_resp_t));
	memset(&urms_datafile, 0, sizeof(urms_datafile_t));
	memset(&phase_status, 0, sizeof(phase_status_t));

	meter_fp = fopen("meter_result.txt", "w");
	signal_fp = fopen("signal_result.txt", "w");

	db_clt_read(pclt, DB_PHASE_3_TIMING_VAR, sizeof(phase_timing_t), &phase_timing);
	printf("Phase 3 min green %d max green 1 %d maximum green %d\n",
		phase_timing.min_green,
		phase_timing.max_green1,
		phase_timing.min_green + phase_timing.max_green1
	);
	db_clt_read(pclt, DB_PHASE_7_TIMING_VAR, sizeof(phase_timing_t), &phase_timing);
	printf("Phase 7 min green %d max green 1 %d maximum green %d\n",
		phase_timing.min_green,
		phase_timing.max_green1,
		phase_timing.min_green + phase_timing.max_green1
	);
	clock_gettime(CLOCK_REALTIME, &call_test_sav);
	
	while(1) {	
		clt_ipc_receive(pclt, &trig_info, sizeof(trig_info));

                if( DB_TRIG_VAR(&trig_info) == DB_TSCP_STATUS_VAR ) {
                        db_clt_read(pclt, DB_TSCP_STATUS_VAR, sizeof(get_long_status8_resp_mess_typ), &get_long_status8_resp_mess);
			if(verbose)
				printf("Got DB_TSCP_STATUS_VAR\n");
		}
		//Read in ramp & mainline data
                if( DB_TRIG_VAR(&trig_info) == DB_URMS_STATUS_VAR ) {
                        db_clt_read(pclt, DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status);
			if(verbose)
				printf("Got DB_URMS_STATUS_VAR\n");
//			printf("db_urms_status lane 1 rate %d\n", (db_urms_status.metered_lane_stat[0].metered_lane_rate_msb << 8) + (unsigned char)db_urms_status.metered_lane_stat[0].metered_lane_rate_lsb);
//			printf("ml_lead_occ_1 %f db_urms_status mainline 1 calc occ %f\n", get_occ_main_lead_1(), (float)(((db_urms_status.mainline_stat[0].lead_occ_msb << 8) + (unsigned char)(db_urms_status.mainline_stat[0].lead_occ_lsb)) / 10.0));
		}

                        db_clt_read(pclt, DB_SHORT_STATUS_VAR, sizeof(get_short_status_resp_t), &short_status);
			db_clt_read(pclt, DB_PHASE_STATUS_VAR, sizeof(phase_status_t), &phase_status);
			db_clt_read(pclt, DB_URMS_DATAFILE_VAR, sizeof(urms_datafile_t), &urms_datafile);
                if( DB_TRIG_VAR(&trig_info) == DB_SHORT_STATUS_VAR ) {
			if(verbose)
				printf("Got DB_SHORT_STATUS_VAR\n");

			greenstat = short_status.greens;
			if( (greenstat_sav == 0x0) &&
			    ((greenstat  == 0x44 )) ) 
                        	clock_gettime(CLOCK_REALTIME, &start_time);
			if( (greenstat_sav == 0x44) &&
			    ((greenstat  == 0x40 )) ) {
				clock_gettime(CLOCK_REALTIME, &end_time);
			        printf("%s: Green time for phase 3 %f sec\n\n", argv[0],
                                (end_time.tv_sec + (end_time.tv_nsec/1.0e9)) -
                                (start_time.tv_sec +(start_time.tv_nsec/1.0e9))
                                );
			}
			if( (greenstat_sav == 0x22) &&
			    ((greenstat & greenstat_sav) == 0 )) {
				printf("\n\nPhases 2 and 6 should be yellow now\n");
				db_clt_read(pclt, DB_PHASE_3_TIMING_VAR , sizeof(phase_timing_t), &phase_timing);

//				new_max_green = signal_data.new_max_green;
#define PHASE_3			3
#define YELLOW_NO_CHANGE	0
#define ALL_RED_NO_CHANGE	0
#define MIN_GREEN_NO_CHANGE	0
//				flag2 = db_set_min_max_green(pclt, PHASE_3, MIN_GREEN_NO_CHANGE, new_max_green, YELLOW_NO_CHANGE, ALL_RED_NO_CHANGE, verbose);
//				if(flag2) {
//					fprintf(stderr, "db_set_min_max_green failed. Exiting....\n");	
//					return -1;
//				}
//				phase_timing.max_green1 = new_max_green;
//				db_clt_write(pclt, DB_PHASE_3_TIMING_VAR , sizeof(phase_timing_t), &phase_timing);

				}
			greenstat_sav = greenstat;
		}
		retval = set_globals(&phase_status, &get_long_status8_resp_mess, &db_urms_status, &urms_datafile);
		clock_gettime(CLOCK_REALTIME, &call_test);
		timediff = (call_test.tv_sec + (call_test.tv_nsec/1.0e9)) - (call_test_sav.tv_sec + (call_test_sav.tv_nsec/1.0e9));
		if(timediff >= 0.05) {
			read_real_time_data(&signal_data);
		db_signal_data.LT_occ = signal_data.LT_occ;
		db_signal_data.old_LT_occ = signal_data.old_LT_occ;
		db_signal_data.RT_occ = signal_data.RT_occ;
		db_signal_data.ramp_queue = signal_data.ramp_queue;
		db_signal_data.old_ramp_queue = signal_data.old_ramp_queue;
		db_signal_data.RT_exceed_num = signal_data.RT_exceed_num;
		db_signal_data.new_max_green = signal_data.new_max_green;
		db_signal_data.old_max_green = signal_data.old_max_green;
		db_signal_data.last_sent_max_green = signal_data.last_sent_max_green;
		db_signal_data.data_row = signal_data.data_row;
		db_signal_data.regular_remain_cycle = signal_data.regular_remain_cycle;
		db_signal_data.overwrite_remain_cycle = signal_data.overwrite_remain_cycle;
		db_signal_data.cycle_passed = signal_data.cycle_passed;
		db_signal_data.prev_cycle_terminate_time = signal_data.prev_cycle_terminate_time;
		db_signal_data.current_cycle_terminate_time = signal_data.current_cycle_terminate_time;
		db_signal_data.prev_queue_reset_time = signal_data.prev_queue_reset_time;
		db_signal_data.regular_release = signal_data.regular_release;
		db_signal_data.overwrite_release = signal_data.overwrite_release;
		db_signal_data.realtime_data[PHASE5SIGNAL] = signal_data.realtime_data[signal_data.data_row][PHASE5SIGNAL];
		db_signal_data.realtime_data[PHASE5APPROACH1] = signal_data.realtime_data[signal_data.data_row][PHASE5APPROACH1];
		db_signal_data.realtime_data[PHASE5APPROACH2] = signal_data.realtime_data[signal_data.data_row][PHASE5APPROACH2];
		db_signal_data.realtime_data[PHASE5STOPBAR] = signal_data.realtime_data[signal_data.data_row][PHASE5STOPBAR];
		db_signal_data.realtime_data[PHASE8SIGNAL] = signal_data.realtime_data[signal_data.data_row][PHASE8SIGNAL];
		db_signal_data.realtime_data[PHASE8APPROACH] = signal_data.realtime_data[signal_data.data_row][PHASE8APPROACH];
		db_signal_data.realtime_data[PHASE8STOPBAR] = signal_data.realtime_data[signal_data.data_row][PHASE8STOPBAR];
		db_signal_data.realtime_data[PHASE6SIGNAL] = signal_data.realtime_data[signal_data.data_row][PHASE6SIGNAL];
		db_signal_data.realtime_data[PHASE7SIGNAL] = signal_data.realtime_data[signal_data.data_row][PHASE7SIGNAL];
		db_signal_data.realtime_data[PHASE3SIGNAL] = signal_data.realtime_data[signal_data.data_row][PHASE3SIGNAL];
		db_clt_write(pclt, DB_SIGNAL_DATA_VAR, sizeof(db_signal_data_t), &db_signal_data);

		signal_flag = get_barrier_flag();
		if((signal_flag != 0) && (signal_flag_sav == 0)) {
			signal_flag = 1;
			signal_flag_sav = 1;
			printf("Got signal flag!!!!!!\n");
		}
		else {
			if((signal_flag != 0) && (signal_flag_sav != 0)) {
				signal_flag = 0;
			}
			else {
				signal_flag = 0;
				signal_flag_sav = 0;
			}
		}

		ramp_flag = get_ramp_flag();
		if((ramp_flag != 0) && (ramp_flag_sav == 0)) {
			ramp_flag = 1;
			ramp_flag_sav = 1;
			printf("Got ramp flag!!!!!!\n");
		}
		else {
			if((ramp_flag != 0) && (ramp_flag_sav != 0)) {
				ramp_flag = 0;
			}
			else {
				ramp_flag = 0;
				ramp_flag_sav = 0;
			}
		}
		call_test_sav.tv_sec = call_test.tv_sec;
		call_test_sav.tv_nsec = call_test.tv_nsec;
		if( (ramp_flag != 0) || (signal_flag != 0) )
			test(&db_signal_data, &db_ramp_data, signal_flag, ramp_flag);
		}
		if(ramp_flag != 0) {
//			if( ramp_data.new_meter_rate != meter_rate_sav) 
			{
				db_urms.lane_2_release_rate = ramp_data.new_meter_rate;
				db_urms.lane_3_release_rate = ramp_data.new_meter_rate;
				db_urms.lane_2_action = URMS_ACTION_FIXED_RATE;
				db_urms.lane_3_action = URMS_ACTION_FIXED_RATE;
				db_urms.lane_2_plan = 2;
				db_urms.lane_3_plan = 1;
				db_clt_write(pclt, DB_URMS_VAR, sizeof(db_urms_t), &db_urms);
				printf("ac_rm_algo: new meter rate %f\n", ramp_data.new_meter_rate);
				meter_rate_sav = ramp_data.new_meter_rate;
			}
		}
	}
	fclose(meter_fp);
	fclose(signal_fp);
	return 0;
}

int test(db_signal_data_t *db_signal_data, db_ramp_data_t *db_ramp_data, unsigned char signal_flag, unsigned char ramp_flag)
{
	struct signal_variables *psignal_data = &signal_data;
	struct ramp_variables *pramp_data = &ramp_data;
	int flag;
	typedef unsigned char u8;
	typedef unsigned short u16;

	flag = read_ramp_data(pramp_data);
	//get new meter rate
	if(flag==0)
	{
			db_ramp_data->mainline_lead_occ[0] = pramp_data->mainline_lead_occ[NUMBER_RAMP_DATA-1][0];
			db_ramp_data->mainline_lead_occ[1] = pramp_data->mainline_lead_occ[NUMBER_RAMP_DATA-1][1];
			db_ramp_data->mainline_lead_occ[2] = pramp_data->mainline_lead_occ[NUMBER_RAMP_DATA-1][2];

			fprintf(meter_fp,"%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%d\n",
				get_current_time(),
				pramp_data->mainline_lead_occ[NUMBER_RAMP_DATA-1][1], 
				db_ramp_data->mainline_lead_occ[1],
				pramp_data->mainline_lead_occ[NUMBER_RAMP_DATA-1][2],
				pramp_data->new_meter_rate,
				(int)count);
//			printf("lead_occ_0 %.1f lead_occ_1 %.1f lead_occ_2 %.1f\n", lead_occ_0, lead_occ_1, lead_occ_2); 
			fflush(NULL);
			db_ramp_data->mainline_trail_occ[0] = pramp_data->mainline_trail_occ[NUMBER_RAMP_DATA-1][0];
			db_ramp_data->mainline_trail_occ[1] = pramp_data->mainline_trail_occ[NUMBER_RAMP_DATA-1][1];
			db_ramp_data->mainline_trail_occ[2] = pramp_data->mainline_trail_occ[NUMBER_RAMP_DATA-1][2];

			db_ramp_data->queue_occ[0] = pramp_data->queue_occ[NUMBER_RAMP_DATA-1][0];
			db_ramp_data->queue_occ[1] = pramp_data->queue_occ[NUMBER_RAMP_DATA-1][1];
			db_ramp_data->queue_occ[2] = pramp_data->queue_occ[NUMBER_RAMP_DATA-1][2];

			db_ramp_data->meter_rate[0] = pramp_data->meter_rate[NUMBER_RAMP_DATA-1][0];
			db_ramp_data->meter_rate[1] = pramp_data->meter_rate[NUMBER_RAMP_DATA-1][1];
			db_ramp_data->meter_rate[2] = pramp_data->meter_rate[NUMBER_RAMP_DATA-1][2];

			db_ramp_data->data_time = pramp_data->data_time[NUMBER_RAMP_DATA-1];
			db_ramp_data->prev_update_data = pramp_data->prev_update_data;
			db_ramp_data->prev_occ_out = pramp_data->prev_occ_out;
			db_ramp_data->new_meter_rate = pramp_data->new_meter_rate;

			db_ramp_data->mainline_avg_occupancy[0] = pramp_data->mainline_avg_occupancy[0];
			db_ramp_data->mainline_avg_occupancy[1] = pramp_data->mainline_avg_occupancy[1];
			db_ramp_data->mainline_avg_occupancy[2] = pramp_data->mainline_avg_occupancy[2];

			db_ramp_data->mainline_avg_volume[0] = (u16)pramp_data->mainline_avg_volume[0];
			db_ramp_data->mainline_avg_volume[1] = (u16)pramp_data->mainline_avg_volume[1];
			db_ramp_data->mainline_avg_volume[2] = (u16)pramp_data->mainline_avg_volume[2];

			db_ramp_data->mainline_lead_status[0] = (u8)pramp_data->mainline_lead_status[NUMBER_RAMP_DATA-1][0];
			db_ramp_data->mainline_lead_status[1] = (u8)pramp_data->mainline_lead_status[NUMBER_RAMP_DATA-1][1];
			db_ramp_data->mainline_lead_status[2] = (u8)pramp_data->mainline_lead_status[NUMBER_RAMP_DATA-1][2];

			db_ramp_data->mainline_trail_status[0] = (u8)pramp_data->mainline_trail_status[NUMBER_RAMP_DATA-1][0];
			db_ramp_data->mainline_trail_status[1] = (u8)pramp_data->mainline_trail_status[NUMBER_RAMP_DATA-1][1];
			db_ramp_data->mainline_trail_status[2] = (u8)pramp_data->mainline_trail_status[NUMBER_RAMP_DATA-1][2];

			db_ramp_data->mainline_lead_vol[0] = (u8)pramp_data->mainline_lead_vol[NUMBER_RAMP_DATA-1][0];
			db_ramp_data->mainline_lead_vol[1] = (u8)pramp_data->mainline_lead_vol[NUMBER_RAMP_DATA-1][1];
			db_ramp_data->mainline_lead_vol[2] = (u8)pramp_data->mainline_lead_vol[NUMBER_RAMP_DATA-1][2];

			db_ramp_data->mainline_trail_vol[0] = (u8)pramp_data->mainline_trail_vol[NUMBER_RAMP_DATA-1][0];
			db_ramp_data->mainline_trail_vol[1] = (u8)pramp_data->mainline_trail_vol[NUMBER_RAMP_DATA-1][1];
			db_ramp_data->mainline_trail_vol[2] = (u8)pramp_data->mainline_trail_vol[NUMBER_RAMP_DATA-1][2];

			db_ramp_data->passage_vol[0] = (u8)pramp_data->passage_vol[NUMBER_RAMP_DATA-1][0];
			db_ramp_data->passage_vol[1] = (u8)pramp_data->passage_vol[NUMBER_RAMP_DATA-1][1];
			db_ramp_data->passage_vol[2] = (u8)pramp_data->passage_vol[NUMBER_RAMP_DATA-1][2];

			db_ramp_data->demand_vol[0] = (u8)pramp_data->demand_vol[NUMBER_RAMP_DATA-1][0];
			db_ramp_data->demand_vol[1] = (u8)pramp_data->demand_vol[NUMBER_RAMP_DATA-1][1];
			db_ramp_data->demand_vol[2] = (u8)pramp_data->demand_vol[NUMBER_RAMP_DATA-1][2];

			db_ramp_data->queue_vol[0] = (u8)pramp_data->queue_vol[NUMBER_RAMP_DATA-1][0];
			db_ramp_data->queue_vol[1] = (u8)pramp_data->queue_vol[NUMBER_RAMP_DATA-1][1];
			db_ramp_data->queue_vol[2] = (u8)pramp_data->queue_vol[NUMBER_RAMP_DATA-1][2];
			db_clt_write(pclt, DB_RAMP_DATA_VAR, sizeof(db_ramp_data_t), db_ramp_data);

			//Must wait until known good data before checking flag and doing rate calculation
			if( ramp_flag == 1 )
			{
				pramp_data->new_meter_rate = get_ALINEA_rate(pramp_data);
			}
	}
	if(signal_flag == 1)
	{
		//get new max green
		flag=get_intersection_measurement(psignal_data,pramp_data);
		if(flag==0)
		{
			flag=get_new_max_green_phase3(psignal_data,pramp_data);
			fprintf(signal_fp,"%f\t%f\t%f\t%f\t%f\t%d\t%d\n",get_current_time(),psignal_data->LT_occ,psignal_data->RT_occ,pramp_data->queue_occ[NUMBER_RAMP_DATA-1][1],psignal_data->ramp_queue,psignal_data->new_max_green,(int)count);
			fflush(NULL);
			if (psignal_data->new_max_green!=psignal_data->last_sent_max_green)
			{
				set_new_max_green_phase3(psignal_data->new_max_green);
				psignal_data->last_sent_max_green=psignal_data->new_max_green;
			}
		}
		psignal_data->data_row=-1;
	}

	if(QUEUE_RESET &&  get_current_time() - psignal_data->prev_queue_reset_time >=QUEUE_RESET_INTERVAL && METHOD_FOR_RAMP_QUEUE==1)
	{
		reset_ramp_queue(psignal_data);
		psignal_data->prev_queue_reset_time = get_current_time();
	}
	return 0;
}


int set_globals(phase_status_t *phase_status, get_long_status8_resp_mess_typ *status, db_urms_status_t *db_urms_status, urms_datafile_t *urms_datafile) {

	hr =  phase_status->ts.hour;
	sec =  phase_status->ts.sec;
	min =  phase_status->ts.min;
  
	greens = phase_status->greens;
	yellows = phase_status->yellows;
	reds = phase_status->reds;
	green_yellow_overlap = status->green_yellow_overlap;
	presence1 = status->presence1;
	presence2 = status->presence2;
	presence3 = status->presence3;
	presence4 = status->presence4;
	
//	printf("set_globals: greens %#hhx yellows %#hhx reds %#hhx\n", greens, yellows, reds);
//	printf("set_globals: green_yellow_overlap %#hhx presence1 %#hhx presence2 %#hhx presence3 %#hhx presence4 %#hhx\n", green_yellow_overlap, presence1, presence2, presence3, presence4);
//      printf("set_globals: urms_datafile->mainline_lead_occ[1] %.1f urms_datafile->mainline_lead_occ[2] %.1f\n", urms_datafile->mainline_lead_occ[1], urms_datafile->mainline_lead_occ[2]);
//      fflush(NULL);


	lead_stat_0 = db_urms_status->mainline_stat[0].lead_stat;
	lead_vol_0 = db_urms_status->mainline_stat[0].lead_vol;
	lead_occ_0 = urms_datafile->mainline_lead_occ[0];
	trail_stat_0 = db_urms_status->mainline_stat[0].trail_stat;
	trail_vol_0 = db_urms_status->mainline_stat[0].trail_vol;
	trail_occ_0 = urms_datafile->mainline_trail_occ[0];
	
	lead_stat_1 = db_urms_status->mainline_stat[1].lead_stat;
	lead_vol_1 = db_urms_status->mainline_stat[1].lead_vol;
	lead_occ_1 = urms_datafile->mainline_lead_occ[1];
	trail_stat_1 = db_urms_status->mainline_stat[1].trail_stat;
	trail_vol_1 = db_urms_status->mainline_stat[1].trail_vol;
	trail_occ_1 = urms_datafile->mainline_trail_occ[1];
	
	lead_stat_2 = db_urms_status->mainline_stat[2].lead_stat;
	lead_vol_2 = db_urms_status->mainline_stat[2].lead_vol;
	lead_occ_2 = urms_datafile->mainline_lead_occ[2];
	trail_stat_2 = db_urms_status->mainline_stat[2].trail_stat;
	trail_vol_2 = db_urms_status->mainline_stat[2].trail_vol;
	trail_occ_2 = urms_datafile->mainline_trail_occ[2];

	demand_vol_0 = db_urms_status->metered_lane_stat[0].demand_vol;
	passage_vol_0 = db_urms_status->metered_lane_stat[0].passage_vol;
	queue_occ_0 = urms_datafile->queue_occ[0];
	queue_vol_0 = db_urms_status->queue_stat[0].vol;
	rate_0 = urms_datafile->metering_rate[0];

	demand_vol_1 = db_urms_status->metered_lane_stat[1].demand_vol;
	passage_vol_1 = db_urms_status->metered_lane_stat[1].passage_vol;
	queue_occ_1 = urms_datafile->queue_occ[1];
	queue_vol_1 = db_urms_status->queue_stat[1].vol;
	rate_1 = urms_datafile->metering_rate[1];

	demand_vol_2 = db_urms_status->metered_lane_stat[2].demand_vol;
	passage_vol_2 = db_urms_status->metered_lane_stat[2].passage_vol;
	queue_occ_2 = urms_datafile->queue_occ[2];
	queue_vol_2 = db_urms_status->queue_stat[2].vol;
	rate_2 = urms_datafile->metering_rate[2];

	return 0;

}
