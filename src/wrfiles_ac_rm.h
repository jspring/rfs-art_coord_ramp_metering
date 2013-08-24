#define DB_URMS_DATAFILE_TYPE	4323
#define DB_URMS_DATAFILE_VAR	DB_URMS_DATAFILE_TYPE

typedef struct {
	float mainline_lead_occ[3];
	float mainline_trail_occ[3];
	float queue_occ[3];
	short metering_rate[3];
} IS_PACKED urms_datafile_t;

