/**\file
 * wrfiles_ac_rm.c    File to save the data from the arterial coordination
 *		and ramp metering 2070s
 *
 *              Default time for saving data is
 *              every 75 msec.  
 *
 *              Every so often (according to a switch received in
 *              the startup line) the files will be closed and a new
 *              set of files opened.
 *
 *		This code has been extensivley rewritten (Sue, May 2009)
 *		to make	it easier to add and delete DB VARs and columns,
 *		and to allow data to be saved in memory between
 *		file writes if desired, instead of writing a line
 *		at a time. Specification for files that show data
 *		from many different DB VARs are now in cacc3_tables.c		
 *
 *
 * Copyright (c) 2000-2009   Regents of the University of California
 *
 *
 */
#include <db_include.h>
#include <path_gps_lib.h>
#include <atsc.h>
#include <atsc_clt_vars.h>
#include <msgs.h>
#include <ab3418_lib.h>
#include <ab3418comm.h>
#include "data_log.h"
#include "clt_vars.h"

static int sig_list[] = 
{
	SIGINT,
	SIGQUIT,
	SIGTERM,
	SIGALRM,
	ERROR,
};

static jmp_buf exit_env;

static void sig_hand(int code)
{
	if (code == SIGALRM)
		return;
	else
		longjmp(exit_env, code);
}

/**
 *	File pointers used for each file saved.
 */
FILE *f_ac_rm = NULL;

buff_typ *pfirst_buff;
buff_typ *pbuff_ac_rm;

extern timestamp_t timestamp;                 // used when reading back in
extern double utc_seconds_since_midnight;    // UTC seconds since midnight
extern double seconds_since_start;       // seconds since start of run


extern data_log_column_spec_t file_spec[];
extern int num_file_columns;

extern db_var_spec_t db_vars_ac_rm[];
extern int num_ac_rm_vars;

path_gps_point_t cabinet_gps; 		// GPS in cabinet

int main(int argc, char *argv[])
{
	int option;
	int exitsig;
	double start_time;
	db_clt_typ *pclt;
	char hostname[MAXHOSTNAMELEN+1];
	int old_fileday = 99;	/// originally set to bad value to force search
	int serial_num;		/// set in open_data_log	
	char id_string[80];
	char monthday[80];
	char serialnum[80];

	int file_time = 15;     /// Number of minutes to record to a file 
	int interval = 50;	/// Number of milliseconds between saves 
	posix_timer_typ *ptimer;       /* Timing proxy */
	char *domain = DEFAULT_SERVICE; // usually no need to change this 
	int xport = COMM_OS_XPORT;	// set correct for OS in sys_os.h 
	FILE *first_file = NULL;
	char *first_file_str;
	char tripdir[80];
	char tripstr[80];
	char ac_rm_pre[80];
	int i;
	int use_stdout = 0;

	memset(id_string, 0, sizeof(id_string));
	memset(monthday, 0, sizeof(monthday));
	memset(serialnum, 0, sizeof(serialnum));
	memset(tripdir, 0, sizeof(tripdir));
	memset(tripstr, 0, sizeof(tripstr));
	memset(ac_rm_pre, 0, sizeof(ac_rm_pre));

	/* Read and interpret any user switches. */
	while ((option = getopt(argc, argv, "d:m:io")) != EOF) {
		switch(option) {
		case 'd':
			strcpy(tripdir, optarg);
			break;
	        case 'm':
			file_time = atoi(optarg);
			if(file_time == 0)
			    file_time = 10000;
			break;
	        case 'i':
			interval = atoi(optarg); 
			break;
	        case 'o':
			use_stdout = 1; 
			break;
	        default:
			printf("Usage: %s\n", argv[0]); 
			printf("    -d <trip directory> \n");
			printf("    -m <file time in minutes (def. 15 min)> \n");
			printf("    -i <loop time in ms (def. 50 ms)> \n");
			printf("    -o (use stdout instead of named file as data output)\n");
			exit(EXIT_FAILURE);
	        }
	}

	if(use_stdout == 0) {
		strcpy(tripstr, tripdir+17);
		printf("tripdir %s\n", tripdir);
		printf("tripstr as string %s as decimal %d\n", tripstr, atoi(tripstr));
		printf(" file columns %d\n", num_file_columns);
	}

	/* Log in to the database (shared global memory).  Default to the
	 * the current host. Assumes other processes that create variables
	 * are already running, since list is NULL.
	 */
	get_local_name(hostname, MAXHOSTNAMELEN);
	if ((pclt = db_list_init(argv[0], hostname, domain, xport, 
			NULL, 0, NULL, 0)) == NULL) {
		printf("Database initialization error in %s.\n", argv[0]);
		exit(EXIT_FAILURE);
	}
        
	/* Setup a timer for every 'interval' msec. */
	if ((ptimer = timer_init(interval, DB_CHANNEL(pclt) )) == NULL) {
		printf("Unable to initialize wrfiles timer\n");
		exit(EXIT_FAILURE);
	}
       /* Open a set of files for recording data. Set up value of "firstfile"
	 * that will be used to handle timestamp and name set-up handling on
	 * reopens
	 */
	if(use_stdout == 0) {
		strcpy(ac_rm_pre, tripdir);
		strcat(ac_rm_pre, "/");
		strcat(ac_rm_pre, "a");
                if (first_file == NULL) {
                        open_data_log_infix(&f_ac_rm, ac_rm_pre, ".dat",
                         &start_time, &old_fileday, &serial_num, monthday, serialnum, tripstr);
                        first_file = f_ac_rm;
                        first_file_str = ac_rm_pre;
			pfirst_buff = pbuff_ac_rm;
			sprintf(id_string, "%s%s%s", monthday, tripstr, serialnum);
                } else {
                        open_another_file(&f_ac_rm, ac_rm_pre,
                                  id_string, ".dat");
                }
	}

	if(( exitsig = setjmp(exit_env)) != 0) {
		if (f_ac_rm)
			fclose(f_ac_rm);
		db_list_done(pclt, NULL, 0, NULL, 0);
	    exit(EXIT_SUCCESS);
	} else
		sig_ign(sig_list, sig_hand);

	fflush(stdout);

	for(;;) {
	/* Read the time of day.  This will be used as the "official
	 * timestamp" in all of the files. */
		get_current_timestamp(&timestamp);

                /* Read in all DB vars, whether or not the associated files
                 * are being written
                 */
                for (i = 0; i < num_ac_rm_vars; i++){
                        db_clt_read(pclt,
                                db_vars_ac_rm[i].db_id_num,
                                db_vars_ac_rm[i].size,
                                db_vars_ac_rm[i].var_pointer);
		}

		/** Check if time to close and reopen data logs */
		if(use_stdout == 0) {
               		if (reopen_data_log_infix(&first_file, file_time, first_file_str,
                                ".dat", &start_time, &old_fileday, &serial_num,
                                monthday, serialnum, tripstr, pfirst_buff))  {
                        // done for first file in reopen_data_log

			sprintf(id_string, "%s%s%s", monthday, tripstr, serialnum);
			reopen_another_file(&f_ac_rm, ac_rm_pre,
                                        id_string, ".dat", pbuff_ac_rm);
			}
		}

		if(use_stdout) {
			save_to_spec(stdout, timestamp, 0,
                                pbuff_ac_rm, num_file_columns,
                                &file_spec[0]);
		}
		else {
			save_to_spec(f_ac_rm, timestamp, 0,
                                pbuff_ac_rm, num_file_columns,
                                &file_spec[0]);
		}
		fflush(NULL);

		TIMER_WAIT(ptimer);
	}
}
