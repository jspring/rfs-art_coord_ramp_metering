
#ifndef WRFILES_AC_RM_H 
#define WRFILES_AC_RM_H

#include <data_log.h>

#define DB_URMS_DATAFILE_TYPE	4323
#define DB_URMS_DATAFILE_VAR	DB_URMS_DATAFILE_TYPE

extern timestamp_t timestamp;                 // used when reading back in
extern double utc_seconds_since_midnight;    // UTC seconds since midnight
extern double seconds_since_start;       // seconds since start of run
extern int num_ac_rm_vars;


extern data_log_column_spec_t file_spec[];
extern int num_file_columns;

extern db_var_spec_t db_vars_ac_rm[];
extern int num_ac_rm_vars;

typedef struct {
	float mainline_lead_occ[3];
	float mainline_trail_occ[3];
	float queue_occ[3];
	short metering_rate[3];
} IS_PACKED urms_datafile_t;

#endif
