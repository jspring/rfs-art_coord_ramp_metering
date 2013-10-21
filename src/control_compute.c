#include <db_include.h>
#include "control_compute.h"

extern float get_current_time(void);

int get_detector_occ_count(int data[][MAXCOL], int first_row, int last_row, int det_col)
{
	int count=0;
	int i;
	for(i=first_row;i<=last_row;i++)
		count=count+data[i][det_col];
	return count;
}

float get_detector_green_occ(int data[][MAXCOL], int max_row, int det_col, int sig_col)
{
	int occ_count=0;
	int green_count=0;
	int i=0;
	int green_start,green_terminate;
	while(i<=max_row)
	{
		if(data[i][sig_col]==0)
		{
			i++;
		}
		else
		{	//it's green
			green_start=i;
			while(i<=max_row)
			{
				if(data[i][sig_col]==0)
				{
					green_terminate=i;
					green_count=green_count+green_terminate-green_start;
					occ_count=occ_count+get_detector_occ_count(data, green_start, green_terminate-1, det_col);
					break;
				}
				else
					i++;
			}
			if(i==max_row+1 && (data[max_row][sig_col]==1 || data[max_row][sig_col]==2) )
			{
				green_count=green_count+max_row-green_start+1;
				occ_count=occ_count+get_detector_occ_count(data, green_start, max_row, det_col);
			}
		}
	}
	return (float)(occ_count)/max((float)(green_count),1.0);
}



int check_phase5_signal(int data[][MAXCOL],int max_row)
{
	int i;
	for(i=0;i<=max_row;i++)
		if(data[i][PHASE5SIGNAL]==1)
			return 0;
	return 1;
}
int check_phase8_detector(int data[][MAXCOL],int max_row)
{
	int phase8stopbar_count=get_detector_occ_count(data,0,max_row,PHASE8STOPBAR);
	int phase8approach_count=get_detector_occ_count(data,0,max_row,PHASE8APPROACH);
	if( (phase8stopbar_count==0 && phase8approach_count!=0) || (phase8stopbar_count!=0 && phase8approach_count==0) )
		return 1;
	else
		return 0;
}

int check_phase5_detector(int data[][MAXCOL],int max_row)
{
	int phase5stopbar_count=get_detector_occ_count(data,0,max_row,PHASE5STOPBAR);
	int phase5approach_count1=get_detector_occ_count(data,0,max_row,PHASE5APPROACH1);
	int phase5approach_count2=get_detector_occ_count(data,0,max_row,PHASE5APPROACH2);
	if( (phase5stopbar_count==0 && phase5approach_count1+phase5approach_count2!=0) || (phase5stopbar_count!=0 && phase5approach_count1+phase5approach_count2==0) )
		return 1;
	else
		return 0;
}
int check_phase8_signal(int data[][MAXCOL],int max_row)
{
	int i;
	for(i=0;i<=max_row;i++)
		if(data[i][PHASE8SIGNAL]==1)
			return 0;
	return 1;
}
int check_realtime_data_health(int data[][MAXCOL],int max_row)
{
	if(check_phase8_detector(data,max_row))
		return 1;
	if(check_phase5_detector(data,max_row))
		return 1;
	if(check_phase8_signal(data,max_row))
		return 1;
	if(check_phase5_signal(data,max_row))
		return 1;
	return 0;
}

float get_meter_rate(struct ramp_variables *pramp_data)
{
	return pramp_data->meter_rate[NUMBER_RAMP_DATA][1];
}
int get_intersection_measurement(struct signal_variables* psignal_data,struct ramp_variables *pramp_data)
{
	int flag=check_realtime_data_health(psignal_data->realtime_data, psignal_data->data_row);
	if(flag!=0)
		return 1;

	psignal_data->old_ramp_queue=psignal_data->ramp_queue;
	psignal_data->old_LT_occ=psignal_data->LT_occ;
	psignal_data->LT_occ=(get_detector_green_occ(psignal_data->realtime_data, psignal_data->data_row, PHASE5APPROACH1, PHASE5SIGNAL)+get_detector_green_occ(psignal_data->realtime_data, psignal_data->data_row, PHASE5APPROACH2, PHASE5SIGNAL))/2;
	psignal_data->RT_occ=get_detector_green_occ(psignal_data->realtime_data, psignal_data->data_row, PHASE8APPROACH, PHASE8SIGNAL);
	//not finish ramp queue
	psignal_data->prev_cycle_terminate_time = psignal_data->current_cycle_terminate_time;
	psignal_data->current_cycle_terminate_time = get_current_time();

	return 0;
}



bool need_reduce_max_green(struct signal_variables *psignal_data)
{
	
	if(psignal_data->LT_occ>LT_OCC_THRESHOLD)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool check_activation_via_queue_occ(struct ramp_variables *pramp_data)
{
	if (pramp_data->queue_occ[NUMBER_RAMP_DATA][1]>QUEUE_OCC_THRESHOLD_TO_ACTIVATE_INTERSECTION)
		return true;
	else
		return false;
}

bool check_activation_via_intersection_occ(struct signal_variables* psignal_data)
{
	if( psignal_data->LT_occ>LEFT_OCC_THRESHOLD_TO_ACTIVATE_INTERSECTION || psignal_data->RT_occ>RIGHT_OCC_THRESHOLD_TO_ACTIVATE_INTERSECTION )
		return true;
	else
		return false;
}

bool check_activation_via_queue_estimation(struct signal_variables *psignal_data)
{
	if(psignal_data->ramp_queue>RAMP_QUEUE_THRESHOLD){
		return true;
	}
	else {
		return false;
	}
}
float get_ramp_exit_vol(struct ramp_variables* pramp_data, float t1, float t2)
{
	int i;
	float exit_count = 0;
	for(i=NUMBER_RAMP_DATA -1;i>=0;i--)
	{
		if( pramp_data->data_time[i]<= t2+EPS && pramp_data->data_time[i]>= t1-EPS)
		{
			if(i==NUMBER_RAMP_DATA -1)
			{
				exit_count = exit_count + max(0.0,t2-pramp_data->data_time[i])/RAMP_DATA_INTERVAL * (pramp_data->passage_vol[i][0] + pramp_data->passage_vol[i][1] + pramp_data->passage_vol[i][2]);
			}
			if(i!=0)
			{
				if (pramp_data->data_time[i-1]>= t1-EPS)
					exit_count = exit_count + max(0.0,pramp_data->passage_vol[i][0] + pramp_data->passage_vol[i][1] + pramp_data->passage_vol[i][2]);
				else
					exit_count = exit_count + max(0.0,pramp_data->data_time[i]-t1)/RAMP_DATA_INTERVAL * (pramp_data->passage_vol[i][0] + pramp_data->passage_vol[i][1] + pramp_data->passage_vol[i][2]);
			}
			else
			{
				exit_count = exit_count + max(0.0,pramp_data->data_time[i]-t1)/RAMP_DATA_INTERVAL * (pramp_data->passage_vol[i][0] + pramp_data->passage_vol[i][1] + pramp_data->passage_vol[i][2]);
			}
		}
		else
		{
			break;
		}
	}
	return exit_count * RAMP_EXIT_FLOW_ADJUST_FACTOR;
}

int get_stepsofgreen(int data[][MAXCOL], int current_row, int max_row, int sig_col)
{
	int count=0;
	int i;
	for(i=current_row;i<=max_row;i++)
		if(data[i][sig_col]==1)
			count++;
	return count;
}

float get_detector_counts(int data[][MAXCOL],int max_row,int sig_col,int det_col)
{
	float counts=0.0;
	int current_row=0;
	int i;
	int flag;
	int zero_ind;
	while(current_row<=max_row)
	{
		flag=0;	
		for(i=current_row;i<=max_row;i++)
			if(data[i][det_col]==1)
			{
				flag=1;
				break;
			}
		if(flag==0)
			break;
		else
		{
			current_row=i;
			flag=0;
			for(i=current_row;i<=max_row;i++)
				if(data[i][det_col]==0)
				{
					flag=1;
					break;
				}
			if(flag==1)
			{	//0 exists
				zero_ind=i;
				if(zero_ind-current_row<MAX_STEPS)
					counts++;
				else
					if(get_stepsofgreen(data,current_row,zero_ind-1,sig_col)>0.5*(zero_ind-current_row) && data[current_row][sig_col]==1)
						counts=counts+(int) round( (float)(zero_ind-current_row)/(float)(MAX_STEPS) );
					else
						counts++;
				current_row=zero_ind;
			}
			else
			{	//only 1
				if(max_row-current_row+1<MAX_STEPS)
					counts++;
				else
					if(get_stepsofgreen(data,current_row,max_row,sig_col)>0.5*(max_row-current_row+1))
						counts=counts+round( (float)(max_row-current_row+1)/(float)(MAX_STEPS) );
					else
						counts++;
				break;
			}

		}
	}
	return counts;
}



float get_ramp_queue(struct signal_variables *psignal_data,struct ramp_variables *pramp_data)
{
	float LT_vol=get_detector_counts(psignal_data->realtime_data,psignal_data->data_row,PHASE5SIGNAL,PHASE5APPROACH1)+get_detector_counts(psignal_data->realtime_data,psignal_data->data_row,PHASE5SIGNAL,PHASE5APPROACH2);
	float RT_vol=get_detector_counts(psignal_data->realtime_data,psignal_data->data_row,PHASE8SIGNAL,PHASE8APPROACH);
	float ramp_exit_vol=get_ramp_exit_vol(pramp_data,psignal_data->prev_cycle_terminate_time,psignal_data->current_cycle_terminate_time);

	return min(100,max(psignal_data->old_ramp_queue+LT_vol+RT_vol-ramp_exit_vol,0));
}

bool IS_activation(struct signal_variables* psignal_data,struct ramp_variables* pramp_data)
{
	switch (METHOD_FOR_RAMP_QUEUE)
	{
	case 1:
		{
			psignal_data->ramp_queue = get_ramp_queue(psignal_data,pramp_data);
			return check_activation_via_queue_estimation(psignal_data);
		}
	case 2:
		return check_activation_via_queue_occ(pramp_data);
	case 3:
		return check_activation_via_intersection_occ(psignal_data);
	default:
		return false;	//if no method defined, means don't change max green
	}
}

int get_new_max_green_phase3(struct signal_variables* psignal_data,struct ramp_variables* pramp_data)
{
	if(!IS_activation(psignal_data,pramp_data))
	{
		psignal_data->new_max_green = MAX_GREEN_UPPERBOUND;
		psignal_data->old_max_green = psignal_data->new_max_green;
		psignal_data->regular_release = false;
		psignal_data->overwrite_release = false;
		psignal_data->overwrite_remain_cycle = 0;
		psignal_data->regular_remain_cycle = 0;
		psignal_data->cycle_passed = 0;
		return 0;
	}
	else if(psignal_data->overwrite_release)
	{
		psignal_data->new_max_green = MAX_GREEN_UPPERBOUND;
		psignal_data->overwrite_remain_cycle--;
		if(psignal_data->overwrite_remain_cycle==0)
		{
			psignal_data->overwrite_release = false;
		}
		return 0;
	}
	else if(psignal_data->regular_release)
	{
		psignal_data->new_max_green = MAX_GREEN_UPPERBOUND;
		psignal_data->regular_remain_cycle--;
		if(psignal_data->RT_occ>RT_RELEASE_OCC_THRESHOLD)
			psignal_data->RT_exceed_num++;
		if(psignal_data->regular_remain_cycle==0)
		{
			psignal_data->regular_release = false;
			psignal_data->cycle_passed = 0;
			if(psignal_data->RT_exceed_num>=CYCLE_TO_ACTIVATE_OVERWRITE)
			{
				psignal_data->overwrite_release = true;
				psignal_data->overwrite_remain_cycle = OVERWRITE_CYCLE;
			}
			psignal_data->RT_exceed_num = 0;
		}
		return 0;
	}
	else
	{
		if(need_reduce_max_green(psignal_data))
		{
			psignal_data->new_max_green = psignal_data->old_max_green-MAX_GREEN_CHANGE;
			psignal_data->new_max_green = max(psignal_data->new_max_green,MAX_GREEN_LOWERBOUND);
		}
		else
		{
			psignal_data->new_max_green = psignal_data->old_max_green+MAX_GREEN_CHANGE;
			psignal_data->new_max_green = min(psignal_data->new_max_green,MAX_GREEN_UPPERBOUND);
		}
		if(psignal_data->RT_occ>RT_RELEASE_OCC_THRESHOLD)
			psignal_data->RT_exceed_num++;

		psignal_data->cycle_passed++;

		if(psignal_data->cycle_passed == CYCLE_TO_RELEASE)
		{
			psignal_data->regular_release = true;
			psignal_data->regular_remain_cycle = REGULAR_CYCLE;
		}
		psignal_data->old_max_green = psignal_data->new_max_green;
	}
	return 0;
}

float get_ALINEA_rate(struct ramp_variables* pramp_data)
{
        float temp,temp2;
        float q_r;
        float q_in;
        float occ_out,occ_out_prime;
        float current_rate;
        float new_rate;
        float occ_in = (pramp_data->mainline_avg_occupancy[1] + pramp_data->mainline_avg_occupancy[2])/2.0;
        q_in = ( pramp_data->mainline_avg_volume[1] + pramp_data->mainline_avg_volume[2] ) * (3600.0/RAMP_DATA_INTERVAL);       //HOV lane excluded
        q_r = ( pramp_data->passage_vol[NUMBER_RAMP_DATA-1][1] +pramp_data->passage_vol[NUMBER_RAMP_DATA-1][2] ) * (3600.0/RAMP_DATA_INTERVAL); //HOV lane excluded

        if( occ_in < OCC_CRITICAL )
        {
                temp=(1.0f+q_r/max(q_in,1e-3));
                temp=temp*IN_LANES;
                temp=temp/OUT_LANES;
                temp=occ_in*temp;
                temp=ALPHA*temp;
//              occ_out=ALPHA*occ_in*(1+q_r/max(q_in,1e-3))*IN_LANES/OUT_LANES;
                occ_out=temp;
        }
        else
        {
                temp = occ_in * IN_LANES;
                temp = temp/OUT_LANES;
                temp2 = VEHICLE_EFFECTIVE_LENGTH;
                temp2 = temp2* q_r;
                temp2 = temp2/ SHOCH_WAVE_SPEED;
                temp2 = temp2/ OUT_LANES;
                occ_out_prime = temp+temp2;
//              occ_out_prime = occ_in * IN_LANES/OUT_LANES + 100 * VEHICLE_EFFECTIVE_LENGTH * q_r / SHOCH_WAVE_SPEED / OUT_LANES ;
                occ_out = ALINEA_GAMMA * occ_out_prime + (1-ALINEA_GAMMA) * pramp_data->prev_occ_out;
printf("get_ALINEA: occ_in %f q_in %f q_r %f temp %f temp2 %f\n", occ_in, q_in, q_r, temp,temp2);
        }
        current_rate = ( pramp_data->meter_rate[NUMBER_RAMP_DATA-1][1] + pramp_data->meter_rate[NUMBER_RAMP_DATA-1][2] ) / 2.0;
        new_rate = current_rate+ALINEA_KR*(OCC_CRITICAL-occ_out);
/*
        if( new_rate > current_rate *(1.0+RAMP_METER_CHANGE_THRESHOLD) )
                new_rate = current_rate *(1.0+RAMP_METER_CHANGE_THRESHOLD);
        if( new_rate < current_rate *(1.0-RAMP_METER_CHANGE_THRESHOLD) )
                new_rate = current_rate *(1.0-RAMP_METER_CHANGE_THRESHOLD);
*/
        if(new_rate<ALINEA_LOWER_BOUND)
                new_rate=ALINEA_LOWER_BOUND;
        if(new_rate>ALINEA_UPPER_BOUND)
                new_rate=ALINEA_UPPER_BOUND;

        pramp_data->prev_occ_out = occ_out;

        return new_rate;
}

int reset_ramp_queue(struct signal_variables* psignal_data)
{
	psignal_data->ramp_queue = 0.0;
	return 0;
}
