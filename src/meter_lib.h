#include <db_include.h>

int send_freeway_data();
int get_meter_measurement(FILE *fp, double *freeway_occ, double *freeway_flow, double *ramp_flow);
double get_meter_rate(double occ, double q_r, double q_in);
int send_meter_rate(double meter_rate);