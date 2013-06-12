#include <db_include.h>

int get_measurement(FILE *fp, double *LT_queue, double *ramp_queue);
unsigned char need_reduce_max_green(double LT_queue, double old_LT_queue, double ramp_queue, double old_ramp_queue);
int get_new_max_green_phase3(double LT_queue, double old_LT_queue, double ramp_queue, double old_ramp_queue, int old_max_green);
int set_new_max_green_phase3(int new_max_green);
