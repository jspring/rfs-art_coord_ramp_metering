/* ac_rm_algo.c - interconverts arterial signal and ramp meter controller data,
**	processes it with a linear optimizing algorithm, and writes the new
**	timing settings back to the database
*/
#include <db_include.h>
#include "msgs.h"
#include "ac_rm_algo.h"
#include "ab3418comm.h"

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

unsigned int db_trig_list[] =  {
	DB_TSCP_STATUS_VAR
};

int NUM_TRIG_VARS = sizeof(db_trig_list)/sizeof(int);


int main( int argc, char *argv[]) {

	db_clt_typ *pclt;              /* Database client pointer */
	char hostname[MAXHOSTNAMELEN];
	char *domain = DEFAULT_SERVICE;
	int xport = COMM_OS_XPORT;
	posix_timer_typ *ptmr;
	trig_info_typ trig_info;
        int ipc_message_error_ctr = 0;
	int retval;
	get_long_status8_resp_mess_typ get_long_status8_resp_mess;
	algo_input_t algo_input[NUM_PHASES];

	int i;
	int opt;
	int interval = 1000;

	for(i=0; i<NUM_PHASES; i++) {
		demand[i] = 20;
	}

	get_local_name(hostname, MAXHOSTNAMELEN);
	if ( ((pclt = db_list_init(argv[0], hostname,
		domain, xport, NULL, 0, 
		db_trig_list, NUM_TRIG_VARS)) == NULL))
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
//                        db_clt_read(pclt, DB_TSCP_STATUS_VAR, sizeof(get_long_status8_resp_mess_typ), &get_long_status8_resp_mess);
//		read_input();
		get_opt_green();
		save_g_opt();
	}
	return 0;
}
