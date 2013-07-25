#include "max_green_lib.h"

#define MAX_GREEN_UPPERBOUND 25
#define MAX_GREEN_LOWERBOUND 12
#define LTQUEUE_THRESHOLD 20.0
#define RAMP_LIMIT 76.0
#define RAMP_QUEUE_THRESHOLD RAMP_LIMIT/2
//#define RTQUEUE_THRESHOLD 20.0
#define MAX_GREEN_CHANGE 2
#define RAMP_QUEUE_DELTA 10.0
#define RT_RELEASE_THRESHOLD 20

#define bool	unsigned char
#define true	1
#define false	0

int get_measurement(FILE *fp, double *LT_queue, double *ramp_queue, double *RT_queue)
{
	int flag;
	flag = fscanf(fp, "%lf %lf %lf", LT_queue, ramp_queue, RT_queue);
	if(flag!=3)
		return 1;
	else
		return 0;
}


unsigned char need_reduce_max_green(double LT_queue, double old_LT_queue, double ramp_queue, double old_ramp_queue)
{
	
	if((LT_queue>LTQUEUE_THRESHOLD && LT_queue>old_LT_queue) || (ramp_queue>RAMP_QUEUE_THRESHOLD && ramp_queue>old_ramp_queue-RAMP_QUEUE_DELTA))
	{
		return true;
	}
	else
	{
		return false;
	}
}



int get_new_max_green_phase3(double LT_queue, double old_LT_queue, double ramp_queue, double old_ramp_queue, int old_max_green, double RT_queue, double new_meter_rate, double old_meter_rate)
{
	int new_max_green = old_max_green;
	if(need_reduce_max_green(LT_queue, old_LT_queue, ramp_queue, old_ramp_queue))	//need to reduce time
	{
		new_max_green = old_max_green-MAX_GREEN_CHANGE;
		new_max_green = max(new_max_green,MAX_GREEN_LOWERBOUND);
	}
	else	//can increase time
	{
		new_max_green = old_max_green+MAX_GREEN_CHANGE;
		new_max_green = min(new_max_green,MAX_GREEN_UPPERBOUND);
	}
	if(RT_queue>RT_RELEASE_THRESHOLD)
	{
		new_max_green=MAX_GREEN_UPPERBOUND;
	}
	return new_max_green;
}

int set_new_max_green_phase3(int new_max_green)
{
	int flag=0;
	printf("%d\n",new_max_green);
	return flag;
}
