#include "max_green_lib.h"

#define MAX_GREEN_UPPERBOUND 25
#define MAX_GREEN_LOWERBOUND 10
#define LTQUEUE_THRESHOLD 20.0
#define RAMP_LIMIT 76.0
#define RAMP_QUEUE_THRESHOLD RAMP_LIMIT/2
#define RTQUEUE_THRESHOLD 20.0
#define MAX_GREEN_CHANGE 2
#define RAMP_QUEUE_DELTA 10.0

#define bool	unsigned char
#define true	1
#define false	0

int get_measurement(FILE *fp, double *LT_queue, double *ramp_queue)
{
	int flag;
	flag = fscanf(fp, "%lf %lf", LT_queue, ramp_queue);
	if(flag!=2)
		return 1;
	else
		return 0;
}


bool need_reduce_max_green(double LT_queue, double old_LT_queue, double ramp_queue, double old_ramp_queue)
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



int get_new_max_green_phase3(double LT_queue, double old_LT_queue, double ramp_queue, double old_ramp_queue, int old_max_green)
{
	int new_max_green = old_max_green;
	if(need_reduce_max_green(LT_queue, old_LT_queue, ramp_queue, old_ramp_queue))	//need to reduce time
	{
		printf("get_new_max_green_phase3: reducing time; old max green %d ", new_max_green); 
		new_max_green = old_max_green-MAX_GREEN_CHANGE;
		new_max_green = max(new_max_green,MAX_GREEN_LOWERBOUND);
		printf("new max green %d\n", new_max_green); 
	}
	else	//can increase time
	{
		printf("get_new_max_green_phase3: increasing time; old max green %d ", new_max_green); 
		new_max_green = old_max_green+MAX_GREEN_CHANGE;
		new_max_green = min(new_max_green,MAX_GREEN_UPPERBOUND);
		printf("new_max_green %d\n", new_max_green); 
	}

	return new_max_green;
}

int set_new_max_green_phase3(int new_max_green)
{
	int flag;
	printf("%d\n",new_max_green);
//	scanf("%d",&flag);
	return flag;
}
