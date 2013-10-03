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

struct signal_variables signal_data;
struct ramp_variables ramp_data;
int old_phase6_signal_status;

FILE *meter_fp;
FILE *signal_fp;
int test(char *filename);

int main( int argc, char *argv[]) {

	char hostname[MAXHOSTNAMELEN];
	char *domain = DEFAULT_SERVICE;
	int xport = COMM_OS_XPORT;
	posix_timer_typ *ptmr;
	trig_info_typ trig_info;

	db_urms_t db_urms;
	get_long_status8_resp_mess_typ get_long_status8_resp_mess;
	get_short_status_resp_t short_status;

        int ipc_message_error_ctr = 0;
	unsigned char greenstat = 0;
	unsigned char greenstat_sav = 0;
	phase_timing_t phase_timing;
	struct timespec start_time;
	struct timespec end_time;

	int interval = 1000;
	int verbose = 0;

	//variables for the intersection measurement
	int new_max_green = 15;
	//variables for the onramp data
	double new_meter_rate;
	int option;

	char *datafile = NULL;
	int flag2;

        /* Read and interpret any user switches. */
        while ((option = getopt(argc, argv, "f:v")) != EOF) {
                switch(option) {
                case 'f':
                        datafile = strdup(optarg);
                        break;
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
        meter_fp=fopen("meter_result.txt","w+");
        signal_fp=fopen("signal_result.txt","w+");
	
	while(1) {	
		ramp_flag = 1;
		signal_flag = 1;
		test(datafile);
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
//				new_meter_rate = get_meter_rate(freeway_occ, ramp_flow, freeway_flow);
				db_urms.lane_1_release_rate = new_meter_rate;
				db_urms.lane_2_release_rate = new_meter_rate;
				db_urms.lane_1_action = URMS_ACTION_FIXED_RATE;
				db_urms.lane_2_action = URMS_ACTION_FIXED_RATE;
				db_urms.lane_1_plan = 1;
				db_urms.lane_2_plan = 2;
				db_clt_write(pclt, DB_URMS_VAR, sizeof(db_urms_t), &db_urms);
printf("ac_rm_algo: new meter rate %f\n", new_meter_rate);

//				new_max_green = get_new_max_green_phase3(LT_queue, old_LT_queue, ramp_queue, old_ramp_queue, old_max_green, RT_queue, new_meter_rate, old_meter_rate);
#define PHASE_3			3
#define YELLOW_NO_CHANGE	0
#define ALL_RED_NO_CHANGE	0
#define MIN_GREEN_NO_CHANGE	0
				flag2 = db_set_min_max_green(pclt, PHASE_3, MIN_GREEN_NO_CHANGE, new_max_green, YELLOW_NO_CHANGE, ALL_RED_NO_CHANGE, verbose);
				if(flag2) {
					fprintf(stderr, "db_set_min_max_green failed. Exiting....\n");	
					return -1;
				}
/****************************************************************************/
/******  Dongyan's code ends***********************************************/				
/****************************************************************************/
				phase_timing.max_green1 = new_max_green;
				db_clt_write(pclt, DB_PHASE_3_TIMING_VAR , sizeof(phase_timing_t), &phase_timing);

				}
			greenstat_sav = greenstat;
		}
	}
        fclose(meter_fp);
        fclose(signal_fp);

	return 0;
}

int test(char filename[])
{
	FILE *fp = fopen(filename,"r");
	char str[1000];
	struct signal_variables *psignal_data = &signal_data;
	struct ramp_variables *pramp_data = &ramp_data;
	int flag;

	if(fp!=NULL)
	{
		while(fgets(str,sizeof(str),fp)!=NULL)
		{count++;
			get_new_data(str,psignal_data,pramp_data);
		}
		fclose(fp);
	}
			if(ramp_flag == 1)
			{
				flag = read_ramp_data(pramp_data);
				//get new meter rate
				if(flag==0)
				{
					pramp_data->new_meter_rate = get_ALINEA_rate(pramp_data);
					fprintf(meter_fp,"%f\t%f\t%f\t%f\t%d\n",get_current_time(),pramp_data->mainline_lead_occ[NUMBER_RAMP_DATA-1][1],pramp_data->mainline_lead_occ[NUMBER_RAMP_DATA-1][2],pramp_data->new_meter_rate,count);
				}
				ramp_flag = 0;
			}
			read_real_time_data(psignal_data);
			if(signal_flag == 1)
			{
				old_phase6_signal_status = 0;
				//get new max green
				flag=get_intersection_measurement(psignal_data,pramp_data);

				if(flag==0)
				{
					flag=get_new_max_green_phase3(psignal_data,pramp_data);
					fprintf(signal_fp,"%f\t%f\t%f\t%f\t%f\t%d\t%d\n",get_current_time(),psignal_data->LT_occ,psignal_data->RT_occ,pramp_data->queue_occ[NUMBER_RAMP_DATA-1][1],psignal_data->ramp_queue,psignal_data->new_max_green,count);
					if (psignal_data->new_max_green!=psignal_data->last_sent_max_green)
					{
						set_new_max_green_phase3(psignal_data->new_max_green);
						psignal_data->last_sent_max_green=psignal_data->new_max_green;
					}
				}
				psignal_data->data_row=-1;
				signal_flag = 0;

			}
			if(QUEUE_RESET &&  get_current_time() - psignal_data->prev_queue_reset_time >=QUEUE_RESET_INTERVAL && METHOD_FOR_RAMP_QUEUE==1)
			{
				reset_ramp_queue(psignal_data);
				psignal_data->prev_queue_reset_time = get_current_time();
			}
return 0;
}

