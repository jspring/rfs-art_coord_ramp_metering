#include <db_include.h>
#include <msgs.h>

//Arterial signal
#define MAX_PHASE 8
#define	SIGNAL_PHASE_NUM MAX_PHASE

int read_input(get_long_status8_resp_mess_typ *pget_long_status8_resp_mess);
char file_input[128]="matlab_in.txt";
char file_output[128]="matlab_out.txt";


int SIGNAL_PHASE = 8;
float cycle = 110;
float demand[MAX_PHASE] = {9.0, 32.0, 9.0, 36.0, 64.0, 31.0, 42.0, 44.0};
float queue[MAX_PHASE]= {4.0, 6.0, 1.0, 2.0, 14.0, 9.0, 2.0, 6.0};
float saturation_flow[MAX_PHASE] = {3600.0, 3600.0, 3600.0, 3600.0, 3600.0, 3600.0, 1800.0, 1800.0};
float Gmin[MAX_PHASE] = {10, 15, 10, 10, 15, 15, 15, 15};
float Beta[MAX_PHASE] = {0, 0, 0, 0, 1, 0, 0, 1};
float weight[MAX_PHASE] = {1, 2, 3, 4.1, 5, 6, 7, 8.1};
float RA = 73.0;
float Gmax[MAX_PHASE]={20.0,80.0,30.0,80.0,80.0,80.0,50.0,80.0};
float vehicle_for_phase[MAX_PHASE];
float DEMAND_INTERVAL =5*60.0;
float lost_time[MAX_PHASE]={5.0,5.0,5.0,5.0,5.0,5.0,5.0,5.0};
float desired_green[MAX_PHASE];
unsigned char arterial_greens[MAX_PHASE];
unsigned char arterial_yellows[MAX_PHASE];
unsigned char arterial_veh_calls[MAX_PHASE];
unsigned char arterial_ped_calls[MAX_PHASE];

//Ramp meter
#define NUM_METERED_LANES	4
#define NUM_MAINLINE_LANES	4
unsigned char ramp_passage_det_status[NUM_METERED_LANES];
unsigned char ramp_demand_det_status[NUM_METERED_LANES];
unsigned char ramp_queue_det_status[NUM_METERED_LANES];
unsigned char ramp_greens[NUM_METERED_LANES];
unsigned char ramp_yellows[NUM_METERED_LANES];
unsigned char release_rate[NUM_METERED_LANES]; 
unsigned int mainline_occupancy[NUM_MAINLINE_LANES];

#include "linear_prog.h"
#include "method2.h"

int get_vehicle_for_phase()
{
	int i;
	for(i=0;i<SIGNAL_PHASE;i++)
		vehicle_for_phase[i] = ((demand[i] * cycle) / DEMAND_INTERVAL) + queue[i];
	return 0;
}
int get_desired_green()
{
	int i;
	for(i=0;i<SIGNAL_PHASE;i++)
		desired_green[i]=vehicle_for_phase[i]/saturation_flow[i]*3600+lost_time[i];
	return 0;
}
char check_ramp_inequality()
{
//	int true = 1;
//	char false = 0;
	if(desired_green[4]*saturation_flow[4]/3600.0*Beta[4]+desired_green[7]*saturation_flow[7]/3600.0*Beta[7]<=RA)
//	if(false)
//		return true;
		return 1;
	else
//		return false;
		return 0;
}

int get_opt_green_via_method_1()
{
	int flag_opt=choose_constraints();	//choose the contraints that should be included in the optimization. Contraints must give a feasible set
	if (flag_opt!=0)
	{
		switch (flag_opt)
		{
			case 1:printf("test 1 fail");break;	//if see test 1 fail, change the Gmin
			case 2:printf("test 2 fail");break;
			case 3:printf("test 3 fail");break;
		}
		return flag_opt;
	}

	//below for linprog
	int flag=g_opt_linprog();
	if(flag==0)
	{
		printf("got new green\n");
	}
	else
	{
		char str[120];
		sprintf(str,"opt signal fail, flag=%d",flag);
		printf("%s\n", str);
	}
	return 0;
}



int get_opt_green()
{
	get_vehicle_for_phase();
	get_desired_green();
	if(check_ramp_inequality())
		get_opt_green_via_method_1();
	else
		get_opt_green_via_method_2();
	return 0;
}

int save_g_opt()
{
	FILE *fp=fopen(file_output,"w+");
	if(fp==NULL)
	{
		printf("fail to open output file");
		return 1;
	}
	int i;
	for(i=0;i<SIGNAL_PHASE_NUM;i++)
		fprintf(fp,"%f\t",g_opt[i]);
	fclose(fp);
	return 0;
}
