#include <stdio.h>
#include "math_function.h"
#include "max_green_lib.h"
char filename[128]="measurement_data.txt";

#define true 1

int main()
{
	int new_max_green, old_max_green;
	double LT_queue=0.0, ramp_queue=0.0, old_LT_queue=0.0, old_ramp_queue=0.0;	//variables for the measurement

	int flag,flag2;
	FILE *fp = fopen(filename,"r");
	if(fp==NULL)
		return -1;

	//get old max green
	old_max_green = 19;

	while(true)
	{
		//when need to get new measurement
		flag = get_measurement(fp, &LT_queue, &ramp_queue);	//not finished

		if(flag)
		{
			fclose(fp);
			return -1;
		}

		new_max_green = get_new_max_green_phase3(LT_queue, old_LT_queue, ramp_queue, old_ramp_queue, old_max_green);
		
		flag2 = set_new_max_green_phase3(new_max_green);	//not finished
		if(flag2)
		{
			fclose(fp);
			return -1;
		}
		old_max_green = new_max_green;
		old_LT_queue = LT_queue;
		old_ramp_queue = ramp_queue;
	}
	
	fclose(fp);
	return 0;
}
