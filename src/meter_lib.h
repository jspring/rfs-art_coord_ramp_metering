#include <db_include.h>
#include <urms.h>

int get_meter_measurement(FILE *fp, double *freeway_occ, double *freeway_flow, double *ramp_flow);
//double get_meter_rate(double occ, double q_r, double q_in);
int send_meter_rate(double meter_rate);

//Defined in meter_lib.c
extern db_clt_typ *pclt;
extern db_urms_status_t db_urms_status;

//Defined in meter_lib.c
extern float get_occ_main_lead_1(void);
extern float get_occ_main_trail_1(void);
extern float get_occ_main_lead_2(void);
extern float get_occ_main_trail_2(void);
extern float get_occ_main_lead_3(void);
extern float get_occ_main_trail_3(void);

extern int get_vol_main_lead_1(void);
extern int get_vol_main_trail_1(void);
extern int get_vol_main_lead_2(void);
extern int get_vol_main_trail_2(void);
extern int get_vol_main_lead_3(void);
extern int get_vol_main_trail_3(void);

extern float get_occ_metered_queue_1(void);
extern int get_vol_metered_queue_1(void);
extern int get_vol_metered_demand_1(void);
extern int get_vol_metered_passage_1(void);

extern float get_occ_metered_queue_2(void);
extern int get_vol_metered_queue_2(void);
extern int get_vol_metered_demand_2(void);
extern int get_vol_metered_passage_2(void);

extern float get_occ_metered_queue_3(void);
extern int get_vol_metered_queue_3(void);
extern int get_vol_metered_demand_3(void);
extern int get_vol_metered_passage_3(void);
