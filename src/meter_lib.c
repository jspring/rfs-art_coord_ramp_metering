#include "meter_lib.h"
#include <urms.h>
#include <ab3418_lib.h>

#define LAMBDA_IN 2
#define LAMBDA_OUT 3
#define ALPHA 1.0
#define OCC_CRITICAL 25.0
#define K_R 70.0
#define ALINEA_LOWER_BOUND 1200.0
#define ALINEA_UPPER_BOUND 1700.0

db_clt_typ *pclt;
db_urms_status_t db_urms_status;
phase_status_t phase_status;
phase_timing_t phase_timing;
get_long_status8_resp_mess_typ tscp_status;

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

float get_rate_metered_1(void) {

	float metered_1_rate = -1;

	db_clt_read(pclt, DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status);
	metered_1_rate = ((db_urms_status.metered_lane_stat[0].metered_lane_rate_msb << 8) + (unsigned char)(db_urms_status.metered_lane_stat[0].metered_lane_rate_lsb));
	if(metered_1_rate < 0)
		return -1;
	return metered_1_rate;
}

float get_rate_metered_2(void) {

	float metered_2_rate = -1;

	db_clt_read(pclt, DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status);
	metered_2_rate = ((db_urms_status.metered_lane_stat[1].metered_lane_rate_msb << 8) + (unsigned char)(db_urms_status.metered_lane_stat[1].metered_lane_rate_lsb));
	if(metered_2_rate < 0)
		return -1;
	return metered_2_rate;
}

float get_rate_metered_3(void) {

	float metered_3_rate = -1;

	db_clt_read(pclt, DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status);
	metered_3_rate = ((db_urms_status.metered_lane_stat[2].metered_lane_rate_msb << 8) + (unsigned char)(db_urms_status.metered_lane_stat[2].metered_lane_rate_lsb));
	if(metered_3_rate < 0)
		return -1;
	return metered_3_rate;
}

unsigned char get_ramp_flag(void) {

	unsigned char ramp_flag = 0;

	db_clt_read(pclt, DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status);
	ramp_flag = db_urms_status.computation_finished;
	if(ramp_flag)
		printf("get_ramp_flag: got ramp flag; statistics display on ramp meter controller should have just changed\n");
	return ramp_flag;
}

unsigned char get_barrier_flag(void) {

	unsigned char barrier_flag = 0;

	db_clt_read(pclt, DB_PHASE_STATUS_VAR, sizeof(phase_status_t), &phase_status);
	barrier_flag = phase_status.barrier_flag;
	if(barrier_flag)
		printf("get_barrier_flag: got arterial barrier flag %hhu; phases 6 and 2 should be yellow now\n", barrier_flag);
	return barrier_flag;
}

int get_status_main_lead_1(void)
{
	int mainline_1_lead_status = -1;

	db_clt_read(pclt, DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status);
	mainline_1_lead_status = db_urms_status.mainline_stat[0].lead_stat;
	if(mainline_1_lead_status < 0)
		return -1;
	return mainline_1_lead_status;
}

int get_status_main_trail_1(void)
{
	int mainline_1_trail_status = -1;

	db_clt_read(pclt, DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status);
	mainline_1_trail_status = db_urms_status.mainline_stat[0].trail_stat;
	if(mainline_1_trail_status < 0)
		return -1;
	return mainline_1_trail_status;
}

int get_status_main_lead_2(void)
{
	int mainline_2_lead_status = -1;

	db_clt_read(pclt, DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status);
	mainline_2_lead_status = db_urms_status.mainline_stat[1].lead_stat;
	if(mainline_2_lead_status < 0)
		return -1;
	return mainline_2_lead_status;
}

int get_status_main_trail_2(void)
{
	int mainline_2_trail_status = -1;

	db_clt_read(pclt, DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status);
	mainline_2_trail_status = db_urms_status.mainline_stat[1].trail_stat;
	if(mainline_2_trail_status < 0)
		return -1;
	return mainline_2_trail_status;
}

int get_status_main_lead_3(void)
{
	int mainline_3_lead_status = -1;

	db_clt_read(pclt, DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status);
	mainline_3_lead_status = db_urms_status.mainline_stat[2].lead_stat;
	if(mainline_3_lead_status < 0)
		return -1;
	return mainline_3_lead_status;
}

int get_status_main_trail_3(void)
{
	int mainline_3_trail_status = -1;

	db_clt_read(pclt, DB_URMS_STATUS_VAR, sizeof(db_urms_status_t), &db_urms_status);
	mainline_3_trail_status = db_urms_status.mainline_stat[2].trail_stat;
	if(mainline_3_trail_status < 0)
		return -1;
	return mainline_3_trail_status;
}

int get_current_max_green(void)
{
	int max_green = -1;

	db_clt_read(pclt, DB_PHASE_3_TIMING_VAR, sizeof(phase_timing_t), &phase_timing);

	max_green = phase_timing.min_green + phase_timing.max_green1;
	if(max_green< 0)
		return -1;
	return max_green;
}

int get_phase3_signal_status()
{
	int greens;
	int yellows;

	db_clt_read(pclt, DB_PHASE_STATUS_VAR, sizeof(phase_status_t), &phase_status);
	greens = phase_status.greens;
	yellows = phase_status.yellows;

        if ((greens>>2)%2)
                return 1;
        else if ((yellows>>2)%2)
                return 2;
        else
        return 0;
}

int get_phase5_signal_status()
{
	int greens;
	int yellows;

	db_clt_read(pclt, DB_PHASE_STATUS_VAR, sizeof(phase_status_t), &phase_status);
	greens = phase_status.greens;
	yellows = phase_status.yellows;

        if ((greens>>4)%2)
                return 1;
        else if ((yellows>>4)%2)
                return 2;
        else
        return 0;
}

int get_phase6_signal_status()
{
	int greens;
	int yellows;

	db_clt_read(pclt, DB_PHASE_STATUS_VAR, sizeof(phase_status_t), &phase_status);
	greens = phase_status.greens;
	yellows = phase_status.yellows;

        if ((greens>>5)%2)
                return 1;
        else if ((yellows>>5)%2)
                return 2;
        else
        return 0;
}

int get_phase7_signal_status()
{
	int greens;
	int yellows;

	db_clt_read(pclt, DB_PHASE_STATUS_VAR, sizeof(phase_status_t), &phase_status);
	greens = phase_status.greens;
	yellows = phase_status.yellows;

        if ((greens>>6)%2)
                return 1;
        else if ((yellows>>6)%2)
                return 2;
        else
        return 0;
}

// This is actually overlap B status
int get_phase8_signal_status()
{
	int green_yellow_overlap;

	db_clt_read(pclt, DB_TSCP_STATUS_VAR, sizeof(get_long_status8_resp_mess_typ), &tscp_status);
	green_yellow_overlap = tscp_status.green_yellow_overlap;
        if ((green_yellow_overlap>>1)%2)
                return 1;
        else
                return 0;
}

int get_phase5_approach1_status()
{
	db_clt_read(pclt, DB_TSCP_STATUS_VAR, sizeof(get_long_status8_resp_mess_typ), &tscp_status);
        return (tscp_status.presence3%2);
}
int get_phase5_approach2_status()
{
	db_clt_read(pclt, DB_TSCP_STATUS_VAR, sizeof(get_long_status8_resp_mess_typ), &tscp_status);
        return (tscp_status.presence3>>2)%2;
}
int get_phase5_stopbar_status()
{
	db_clt_read(pclt, DB_TSCP_STATUS_VAR, sizeof(get_long_status8_resp_mess_typ), &tscp_status);
        return (tscp_status.presence2>>4)%2;
}

// This is actually the advance detector for phase 6 RT
// (i.e. overlap B)
int get_phase8_approach_status()
{
	db_clt_read(pclt, DB_TSCP_STATUS_VAR, sizeof(get_long_status8_resp_mess_typ), &tscp_status);
        //return rand() / (double)RAND_MAX<0.4;
        return (tscp_status.presence1>>1)%2;
}

// This is actually the stop bar detector for phase 6 RT
// (i.e. overlap B)
int get_phase8_stopbar_status()
{
	db_clt_read(pclt, DB_TSCP_STATUS_VAR, sizeof(get_long_status8_resp_mess_typ), &tscp_status);
        return (tscp_status.presence3>>5)%2;
}

