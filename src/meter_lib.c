#include "meter_lib.h"

#define LAMBDA_IN 2
#define LAMBDA_OUT 3
#define ALPHA 1.0
#define OCC_CRITICAL 25.0
#define K_R 70.0
#define ALINEA_LOWER_BOUND 1200.0
#define ALINEA_UPPER_BOUND 1700.0


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
int send_meter_rate(double meter_rate)
{
	printf("%5.2lf\n",meter_rate);
	return 0;
}
