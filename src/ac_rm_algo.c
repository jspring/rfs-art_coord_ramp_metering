/* ac_rm_algo.c - interconverts arterial signal and ramp meter controller data,
**	processes it with a linear optimizing algorithm, and writes the new
**	timing settings back to the database
*/
#include <db_include.h>
#include "msgs.h"
#include "ac_rm_algo.h"
#include "atsc.h"
#include "atsc_clt_vars.h"
#include <ab3418_lib.h>
#include "ab3418comm.h"
#include "max_green_lib.h"

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
	DB_SHORT_STATUS_VAR
};

int NUM_TRIG_VARS_ALGO = sizeof(db_trig_list_algo)/sizeof(int);


int main( int argc, char *argv[]) {

	db_clt_typ *pclt;              /* Database client pointer */
	char hostname[MAXHOSTNAMELEN];
	char *domain = DEFAULT_SERVICE;
	int xport = COMM_OS_XPORT;
	posix_timer_typ *ptmr;
	trig_info_typ trig_info;
        int ipc_message_error_ctr = 0;
	int retval = 0;
	get_long_status8_resp_mess_typ get_long_status8_resp_mess;
	get_short_status_resp_t short_status;
	unsigned char greenstat = 0;
	unsigned char greenstat_sav = 0;
	phase_timing_t phase_timing;
	struct timespec start_time;
	struct timespec end_time;

	int interval = 1000;
	int verbose = 0;

/****************************************************************************/
/******* Dongyan's code *****************************************************/
/****************************************************************************/
	char filename[128]="measurement_data.txt";
	int new_max_green = 15;
	int old_max_green = 15;
	double LT_queue=0.0;
	double ramp_queue=0.0; 
	double old_LT_queue=0.0; 
	double old_ramp_queue=0.0;      //variables for the measurement

	int flag,flag2;
	FILE *fp = fopen(filename,"r");
	if(fp==NULL)
	return -1;
/****************************************************************************/
/****************************************************************************/

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
	
	while(1) {	
		retval = clt_ipc_receive(pclt, &trig_info, sizeof(trig_info));
                if( DB_TRIG_VAR(&trig_info) == DB_TSCP_STATUS_VAR )
                        db_clt_read(pclt, DB_TSCP_STATUS_VAR, sizeof(get_long_status8_resp_mess_typ), &get_long_status8_resp_mess);

//                if( DB_TRIG_VAR(&trig_info) == DB_SHORT_STATUS_VAR ) {
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

/****************************************************************************************************************************/
/*************************************  Dongyan's code begins****************************************************************/				
/****************************************************************************************************************************/
				//when need to get new measurement
				flag = get_measurement(fp, &LT_queue, &ramp_queue);     //not finished
				if(flag) {
					fprintf(stderr, "get_measurement failed. Exiting....\n");	
					fclose(fp);
					return -1;
				}
				new_max_green = get_new_max_green_phase3(LT_queue, old_LT_queue, ramp_queue, old_ramp_queue, old_max_green);
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
/****************************************************************************************************************************/
/*************************************  Dongyan's code ends******************************************************************/				
/****************************************************************************************************************************/
//				phase_timing.max_green1 = new_max_green;
//				db_clt_write(pclt, DB_PHASE_3_TIMING_VAR , sizeof(phase_timing_t), &phase_timing);
				}
			greenstat_sav = greenstat;
//		}
	}
	return 0;
}
