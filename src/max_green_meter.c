#include <stdio.h>
#include "math_function.h"
#include "max_green_lib.h"
#include "meter_lib.h"

char filename[128]="C:\\Taylor\\data\\measurement_data.txt";
char meter_datafile[1280]="C:\\Taylor\\data\\meter_data.txt";


int main()
{
	//variable used for intersection
	int new_max_green, old_max_green;
	double LT_queue=0.0, ramp_queue=0.0, old_LT_queue=0.0, old_ramp_queue=0.0, RT_queue;	//variables for the measurement
	//variable used for meter
	double freeway_occ, freeway_flow, ramp_flow;
	double new_meter_rate, old_meter_rate;

	int flag,flag2;
	FILE *fp = fopen(filename,"r");
	if(fp==NULL)
		return -1;

	int flag_m, flag_m2;
	FILE *fp_m = fopen(meter_datafile,"r");
	if(fp_m==NULL)
		return -1;

	//get old max green
	old_max_green = 19;

	while(true)
	{
		//the functions below should be at the onramp laptop

		flag_m = get_meter_measurement(fp_m, &freeway_occ, &freeway_flow, &ramp_flow);		//get freeway and onramp data, this function should be at the onramp laptop
		if(flag_m)
		{
			fclose(fp_m);
			return -1;
		}

		send_freeway_data();	//send freeway data to master computer to compute new meter rate and intersection max green

		//the functions below should be at the master computer
		old_meter_rate = ramp_flow;
		new_meter_rate = get_meter_rate(freeway_occ, ramp_flow, freeway_flow);
		flag_m2 = send_meter_rate(new_meter_rate);	//send meter rate to onramp laptop, then the onramp laptop should set the new meter rate to controller
		if(flag_m2)
		{
			fclose(fp_m);
			return -1;
		}

		flag = get_measurement(fp, &LT_queue, &ramp_queue, &RT_queue);	

		if(flag)
		{
			fclose(fp);
			return -1;
		}

		new_max_green = get_new_max_green_phase3(LT_queue, old_LT_queue, ramp_queue, old_ramp_queue, old_max_green, RT_queue, new_meter_rate, old_meter_rate);
		
		flag2 = set_new_max_green_phase3(new_max_green);	
		if(flag2)
		{
			fclose(fp);
			return -1;
		}
		old_max_green = new_max_green;
		old_LT_queue = LT_queue;
		old_ramp_queue = ramp_queue;
	}
	fclose(fp_m);
	fclose(fp);
	return 0;
}