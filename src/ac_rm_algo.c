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
static jmp_buf exit_env;

#define NUM_PHASES	8
extern float cycle;
extern float demand[NUM_PHASES];
extern float queue[NUM_PHASES];
extern float saturation_flow[NUM_PHASES];
extern float Gmin[NUM_PHASES];
extern float Beta[NUM_PHASES];
extern float weight[NUM_PHASES];
extern float RA;

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


int main( int argc, char *argv[]) {

	db_clt_typ *pclt;              /* Database client pointer */
	char hostname[MAXHOSTNAMELEN];
	char *domain = DEFAULT_SERVICE;
	int xport = COMM_OS_XPORT;
	posix_timer_typ *ptmr;
	trig_info_typ trig_info;

	db_urms_t db_urms;
	get_long_status8_resp_mess_typ get_long_status8_resp_mess;
	get_short_status_resp_t short_status;
	db_urms_status_t db_urms_status;

        int ipc_message_error_ctr = 0;
	int retval = 0;
	unsigned char greenstat = 0;
	unsigned char greenstat_sav = 0;
	phase_timing_t phase_timing;
	struct timespec start_time;
	struct timespec end_time;

	int interval = 1000;
	int verbose = 0;
/******* Dongyan's code *****************************************************/
/****************************************************************************/
	char filename[128]="measurement_data.txt";
	char meter_datafile[128]="meter_data.txt";
	//variables for the intersection measurement
	int new_max_green = 15;
	int old_max_green = 15;
	double LT_queue=0.0;
	double ramp_queue=0.0; 
	double old_LT_queue=0.0; 
	double old_ramp_queue=0.0; 
	double RT_queue=0.0;	
	//variables for the onramp data
	double freeway_occ, freeway_flow, ramp_flow;
	double new_meter_rate, old_meter_rate;

	int flag,flag2;
	FILE *fp = fopen(filename,"r");
	if(fp==NULL)
	return -1;

	int flag_m, flag_m2;
	FILE *fp_m = fopen(meter_datafile,"r");
	if(fp_m==NULL)
		return -1;	
/*******************************************************************************************************************/
/*******************************************************************************************************************/

	get_local_name(hostname, MAXHOSTNAMELEN);
	if ( ((pclt = db_list_init(argv[0], hostname,
		domain, xport, NULL, 0, 
		db_trig_list_algo, NUM_TRIG_VARS_ALGO)) == NULL))
		exit(EXIT_FAILURE);
printf("ac_rm_algo: Got to 1\n");
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
	
	while(1) {	
		retval = clt_ipc_receive(pclt, &trig_info, sizeof(trig_info));
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
		}

                if( DB_TRIG_VAR(&trig_info) == DB_SHORT_STATUS_VAR ) {
			if(verbose)
				printf("Got DB_SHORT_STATUS_VAR\n");
                        db_clt_read(pclt, DB_SHORT_STATUS_VAR, sizeof(get_short_status_resp_t), &short_status);
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
//				db_clt_read(pclt, DB_PHASE_3_TIMING_VAR , sizeof(phase_timing_t), &phase_timing);
//				old_max_green = phase_timing.max_green1;

/****************************************************************************/
/******  Dongyan's code begins***********************************************/				
/****************************************************************************/

				//the functions below should be at the master computer
				old_meter_rate = ramp_flow;
				new_meter_rate = get_meter_rate(freeway_occ, ramp_flow, freeway_flow);
				db_urms.lane_1_release_rate = new_meter_rate;
				db_urms.lane_2_release_rate = new_meter_rate;
				db_urms.lane_1_action = URMS_ACTION_FIXED_RATE;
				db_urms.lane_2_action = URMS_ACTION_FIXED_RATE;
				db_urms.lane_1_plan = 1;
				db_urms.lane_2_plan = 2;
				db_clt_write(pclt, DB_URMS_VAR, sizeof(db_urms_t), &db_urms);
printf("ac_rm_algo: new meter rate %f\n", new_meter_rate);
				flag_m2 = send_meter_rate(new_meter_rate);	//send meter rate to onramp laptop, then the onramp laptop should set the new meter rate to controller
				if(flag_m2)
				{
					fclose(fp_m);
					return -1;
				}

				flag = get_measurement(fp, &LT_queue, &ramp_queue, &RT_queue);
				if(flag) {
					fprintf(stderr, "get_measurement failed. Exiting....\n");	
					fclose(fp);
					return -1;
				}
				new_max_green = get_new_max_green_phase3(LT_queue, old_LT_queue, ramp_queue, old_ramp_queue, old_max_green, RT_queue, new_meter_rate, old_meter_rate);
#define PHASE_3			3
#define YELLOW_NO_CHANGE	0
#define ALL_RED_NO_CHANGE	0
#define MIN_GREEN_NO_CHANGE	0
				flag2 = db_set_min_max_green(pclt, PHASE_3, MIN_GREEN_NO_CHANGE, new_max_green, YELLOW_NO_CHANGE, ALL_RED_NO_CHANGE, verbose);
				if(flag2) {
					fprintf(stderr, "db_set_min_max_green failed. Exiting....\n");	
					fclose(fp);
					return -1;
				}
				old_max_green = new_max_green;
				old_LT_queue = LT_queue;
				old_ramp_queue = ramp_queue;
/****************************************************************************/
/******  Dongyan's code ends***********************************************/				
/****************************************************************************/
				phase_timing.max_green1 = new_max_green;
				db_clt_write(pclt, DB_PHASE_3_TIMING_VAR , sizeof(phase_timing_t), &phase_timing);

				}
			greenstat_sav = greenstat;
		}
	}
	fclose(fp);
	fclose(fp_m);
	return 0;
}
