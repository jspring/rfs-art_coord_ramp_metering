#include "meter_lib.h"
#include <urms.h>

#define LAMBDA_IN 2
#define LAMBDA_OUT 3
#define ALPHA 1.0
#define OCC_CRITICAL 25.0
#define K_R 70.0
#define ALINEA_LOWER_BOUND 1200.0
#define ALINEA_UPPER_BOUND 1700.0

db_clt_typ *pclt;
db_urms_status_t db_urms_status;

int send_freeway_data()
{
	return 0;
}

int get_meter_measurement(FILE *fp, double *freeway_occ, double *freeway_flow, double *ramp_flow)
{
	int flag;
	flag = fscanf(fp, "%lf %lf %lf", freeway_occ, freeway_flow, ramp_flow);
	if(flag!=3)
		return 1;
	else
		return 0;
}

/*
double get_meter_rate(double occ, double q_r, double q_in)
{
	//occ is mainline upstream occ, q_r is ramp flow(current meter rate), q_in is mainline upstream measured flow
	double occ_out;
	double new_rate;

	occ_out=ALPHA*occ*(1+q_r/q_in)*LAMBDA_IN/LAMBDA_OUT;

	new_rate=q_r+K_R*(OCC_CRITICAL-occ_out);
	if(new_rate<ALINEA_LOWER_BOUND)
		new_rate=ALINEA_LOWER_BOUND;
	if(new_rate>ALINEA_UPPER_BOUND)
		new_rate=ALINEA_UPPER_BOUND;
	new_rate=(double)(int)new_rate;
	return new_rate;
}
*/
int send_meter_rate(double meter_rate)
{
	printf("%5.2lf\n",meter_rate);
	return 0;
}

float get_occ_main_lead_1(void) {
	
	float ml_lead_occ_1 = -1;

	db_clt_read(pclt, DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status);
	ml_lead_occ_1 = ((db_urms_status.mainline_stat[0].lead_occ_msb << 8) + (unsigned char)(db_urms_status.mainline_stat[0].lead_occ_lsb)) / 10.0;
//printf("get_occ_main_lead_1: db_urms_status mainline 1 calc occ %f\n", (float)(((db_urms_status.mainline_stat[0].lead_occ_msb << 8) + (unsigned char)(db_urms_status.mainline_stat[0].lead_occ_lsb)) / 10.0));

	if( (ml_lead_occ_1 < 0) || (ml_lead_occ_1 > 100))
		return -1;
	return ml_lead_occ_1;
}

float get_occ_main_trail_1(void) {
	
	float ml_trail_occ_1 = -1;

	db_clt_read(pclt, DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status);
	ml_trail_occ_1 = ((db_urms_status.mainline_stat[0].trail_occ_msb << 8) + (unsigned char)(db_urms_status.mainline_stat[0].trail_occ_lsb)) / 10.0;
	if( (ml_trail_occ_1 < 0) || (ml_trail_occ_1 > 100))
		return -1;
	return ml_trail_occ_1;
}

float get_occ_main_lead_2(void) {
	
	float ml_lead_occ_2 = -1;

	db_clt_read(pclt, DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status);
	ml_lead_occ_2 = ((db_urms_status.mainline_stat[1].lead_occ_msb << 8) + (unsigned char)(db_urms_status.mainline_stat[1].lead_occ_lsb)) / 10.0;
	if( (ml_lead_occ_2 < 0) || (ml_lead_occ_2 > 100))
		return -1;
	return ml_lead_occ_2;
}

float get_occ_main_trail_2(void) {
	
	float ml_trail_occ_2 = -1;

	db_clt_read(pclt, DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status);
	ml_trail_occ_2 = ((db_urms_status.mainline_stat[1].trail_occ_msb << 8) + (unsigned char)(db_urms_status.mainline_stat[1].trail_occ_lsb)) / 10.0;
	if( (ml_trail_occ_2 < 0) || (ml_trail_occ_2 > 100))
		return -1;
	return ml_trail_occ_2;
}

float get_occ_main_lead_3(void) {
	
	float ml_lead_occ_3 = -1;

	db_clt_read(pclt, DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status);
	ml_lead_occ_3 = ((db_urms_status.mainline_stat[2].lead_occ_msb << 8) + (unsigned char)(db_urms_status.mainline_stat[2].lead_occ_lsb)) / 10.0;
	if( (ml_lead_occ_3 < 0) || (ml_lead_occ_3 > 100))
		return -1;
	return ml_lead_occ_3;
}

float get_occ_main_trail_3(void) {
	
	float ml_trail_occ_3 = -1;

	db_clt_read(pclt, DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status);
	ml_trail_occ_3 = ((db_urms_status.mainline_stat[2].trail_occ_msb << 8) + (unsigned char)(db_urms_status.mainline_stat[2].trail_occ_lsb)) / 10.0;
	if( (ml_trail_occ_3 < 0) || (ml_trail_occ_3 > 100))
		return -1;
	return ml_trail_occ_3;
}

int get_vol_main_lead_1(void) {
	
	int ml_lead_vol_1 = -1;

	db_clt_read(pclt, DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status);
	ml_lead_vol_1 = db_urms_status.mainline_stat[0].lead_vol;
	if(ml_lead_vol_1 < 0)
		return -1;
	return ml_lead_vol_1;
}

int get_vol_main_trail_1(void) {
	
	int ml_trail_vol_1 = -1;

	db_clt_read(pclt, DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status);
	ml_trail_vol_1 = db_urms_status.mainline_stat[0].trail_vol;
	if(ml_trail_vol_1 < 0)
		return -1;
	return ml_trail_vol_1;
}

int get_vol_main_lead_2(void) {
	
	int ml_lead_vol_2 = -1;

	db_clt_read(pclt, DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status);
	ml_lead_vol_2 = db_urms_status.mainline_stat[1].lead_vol;
	if(ml_lead_vol_2 < 0)
		return -1;
	return ml_lead_vol_2;
}

int get_vol_main_trail_2(void) {
	
	int ml_trail_vol_2 = -1;

	db_clt_read(pclt, DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status);
	ml_trail_vol_2 = db_urms_status.mainline_stat[1].trail_vol;
	if(ml_trail_vol_2 < 0)
		return -1;
	return ml_trail_vol_2;
}

int get_vol_main_lead_3(void) {
	
	int ml_lead_vol_3 = -1;

	db_clt_read(pclt, DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status);
	ml_lead_vol_3 = db_urms_status.mainline_stat[2].lead_vol;
	if(ml_lead_vol_3 < 0)
		return -1;
	return ml_lead_vol_3;
}

int get_vol_main_trail_3(void) {
	
	int ml_trail_vol_3 = -1;

	db_clt_read(pclt, DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status);
	ml_trail_vol_3 = db_urms_status.mainline_stat[2].trail_vol;
	if(ml_trail_vol_3 < 0)
		return -1;
	return ml_trail_vol_3;
}

float get_occ_metered_queue_1(void) {
	
	float metered_queue_occ_1 = -1;

	db_clt_read(pclt, DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status);
	metered_queue_occ_1 = ((db_urms_status.queue_stat[0].occ_msb << 8) + (unsigned char)(db_urms_status.queue_stat[0].occ_lsb)) / 10.0;
	if( (metered_queue_occ_1 < 0) || (metered_queue_occ_1 > 100))
		return -1;
	return metered_queue_occ_1 ;
}

int get_vol_metered_queue_1(void) {
	
	int metered_queue_vol_1 = -1;

	db_clt_read(pclt, DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status);
	metered_queue_vol_1 = db_urms_status.queue_stat[0].vol;
	if(metered_queue_vol_1 < 0)
		return -1;
	return metered_queue_vol_1;
}

int get_vol_metered_demand_1(void) {
	
	int metered_demand_vol_1 = -1;

	db_clt_read(pclt, DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status);
	metered_demand_vol_1 = db_urms_status.metered_lane_stat[0].demand_vol;
	if(metered_demand_vol_1 < 0)
		return -1;
	return metered_demand_vol_1;
}

int get_vol_metered_passage_1(void) {
	
	int metered_passage_vol_1 = -1;

	db_clt_read(pclt, DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status);
	metered_passage_vol_1 = db_urms_status.metered_lane_stat[0].passage_vol;
	if(metered_passage_vol_1 < 0)
		return -1;
	return metered_passage_vol_1;
}

float get_occ_metered_queue_2(void) {
	
	float metered_queue_occ_2 = -1;

	db_clt_read(pclt, DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status);
	metered_queue_occ_2 = ((db_urms_status.queue_stat[1].occ_msb << 8) + (unsigned char)(db_urms_status.queue_stat[1].occ_lsb)) / 10.0;
	if( (metered_queue_occ_2 < 0) || (metered_queue_occ_2 > 100))
		return -1;
	return metered_queue_occ_2 ;
}

int get_vol_metered_queue_2(void) {
	
	int metered_queue_vol_2 = -1;

	db_clt_read(pclt, DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status);
	metered_queue_vol_2 = db_urms_status.queue_stat[1].vol;
	if(metered_queue_vol_2 < 0)
		return -1;
	return metered_queue_vol_2;
}

int get_vol_metered_demand_2(void) {
	
	int metered_demand_vol_2 = -1;

	db_clt_read(pclt, DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status);
	metered_demand_vol_2 = db_urms_status.metered_lane_stat[1].demand_vol;
	if(metered_demand_vol_2 < 0)
		return -1;
	return metered_demand_vol_2;
}

int get_vol_metered_passage_2(void) {
	
	int metered_passage_vol_2 = -1;

	db_clt_read(pclt, DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status);
	metered_passage_vol_2 = db_urms_status.metered_lane_stat[1].passage_vol;
	if(metered_passage_vol_2 < 0)
		return -1;
	return metered_passage_vol_2;
}

float get_occ_metered_queue_3(void) {
	
	float metered_queue_occ_3 = -1;

	db_clt_read(pclt, DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status);
	metered_queue_occ_3 = ((db_urms_status.queue_stat[2].occ_msb << 8) + (unsigned char)(db_urms_status.queue_stat[2].occ_lsb)) / 10.0;
	if( (metered_queue_occ_3 < 0) || (metered_queue_occ_3 > 100))
		return -1;
	return metered_queue_occ_3 ;
}

int get_vol_metered_queue_3(void) {
	
	int metered_queue_vol_3 = -1;

	db_clt_read(pclt, DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status);
	metered_queue_vol_3 = db_urms_status.queue_stat[2].vol;
	if(metered_queue_vol_3 < 0)
		return -1;
	return metered_queue_vol_3;
}

int get_vol_metered_demand_3(void) {
	
	int metered_demand_vol_3 = -1;

	db_clt_read(pclt, DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status);
	metered_demand_vol_3 = db_urms_status.metered_lane_stat[2].demand_vol;
	if(metered_demand_vol_3 < 0)
		return -1;
	return metered_demand_vol_3;
}

int get_vol_metered_passage_3(void) {
	
	int metered_passage_vol_3 = -1;

	db_clt_read(pclt, DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status);
	metered_passage_vol_3 = db_urms_status.metered_lane_stat[2].passage_vol;
	if(metered_passage_vol_3 < 0)
		return -1;
	return metered_passage_vol_3;
}
