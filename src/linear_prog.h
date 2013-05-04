#include "simplx.h"
#define MP m1+m2+m3+2
#define M m1+m2+m3
#define NP N+1
float A[60][SIGNAL_PHASE_NUM+1]={0.0};
float f[SIGNAL_PHASE_NUM+1]={0.0};
float A2[20][SIGNAL_PHASE_NUM+1]={0.0};
float Aeq[20][SIGNAL_PHASE_NUM+1]={0.0};
float A1[20][SIGNAL_PHASE_NUM+1]={0.0};
float H[SIGNAL_PHASE_NUM][SIGNAL_PHASE_NUM]={0.0};
float fqp[SIGNAL_PHASE_NUM]={0.0};
int m1,m2,m3,N;
float g_opt[SIGNAL_PHASE_NUM]={0.0};
float increase_upper_bound_ratio=(float) 1.5;

//for transformed linear program
float f_t[SIGNAL_PHASE_NUM+1+SIGNAL_PHASE_NUM]={0.0};
float A_t[120][SIGNAL_PHASE_NUM+1+SIGNAL_PHASE_NUM]={0.0};
float x[SIGNAL_PHASE_NUM+SIGNAL_PHASE_NUM]={0.0};
float A1_t[40][SIGNAL_PHASE_NUM+1+SIGNAL_PHASE_NUM]={0.0};
float A2_t[40][SIGNAL_PHASE_NUM+1+SIGNAL_PHASE_NUM]={0.0};
float Aeq_t[40][SIGNAL_PHASE_NUM+1+SIGNAL_PHASE_NUM]={0.0};
int m1_t,m2_t,m3_t;
//for method 2 part 1
#define SIGNAL_PHASE_NUM2 3
float Ac[60][SIGNAL_PHASE_NUM2+1]={0.0};
float fc[SIGNAL_PHASE_NUM2+1]={0.0};
float A2c[20][SIGNAL_PHASE_NUM2+1]={0.0};
float Aeqc[20][SIGNAL_PHASE_NUM2+1]={0.0};
float A1c[20][SIGNAL_PHASE_NUM2+1]={0.0};
float xc[SIGNAL_PHASE_NUM2]={0.0};
int m1c,m2c,m3c,Nc;
#define MPc m1c+m2c+m3c+2
#define Mc m1c+m2c+m3c
#define NPc Nc+1
//for method 2 part 2
#define SIGNAL_PHASE_NUM3 9
float Ad[60][SIGNAL_PHASE_NUM3+1]={0.0};
float fd[SIGNAL_PHASE_NUM3+1]={0.0};
float A2d[20][SIGNAL_PHASE_NUM3+1]={0.0};
float Aeqd[20][SIGNAL_PHASE_NUM3+1]={0.0};
float A1d[20][SIGNAL_PHASE_NUM3+1]={0.0};
float xd[SIGNAL_PHASE_NUM3]={0.0};
int m1d,m2d,m3d,Nd;
#define MPd m1d+m2d+m3d+2
#define Md m1d+m2d+m3d
#define NPd Nd+1
#include "clear_matrix.h"

void clear_all_matrix()
{
	clear_matrix_A(60,9);
	clear_matrix_A1(20,9);
	clear_matrix_A2(20,9);
	clear_matrix_Aeq(20,9);
}

void clear_all_matrix2()
{
	clear_matrix_Ac(60,SIGNAL_PHASE_NUM2+1);
	clear_matrix_A1c(20,SIGNAL_PHASE_NUM2+1);
	clear_matrix_A2c(20,SIGNAL_PHASE_NUM2+1);
	clear_matrix_Aeqc(20,SIGNAL_PHASE_NUM2+1);
	clear_matrix_fc(SIGNAL_PHASE_NUM2+1);
	clear_matrix_xc(SIGNAL_PHASE_NUM2+1);
	clear_matrix_Ad(60,SIGNAL_PHASE_NUM3+1);
	clear_matrix_A1d(20,SIGNAL_PHASE_NUM3+1);
	clear_matrix_A2d(20,SIGNAL_PHASE_NUM3+1);
	clear_matrix_Aeqd(20,SIGNAL_PHASE_NUM3+1);
	clear_matrix_fd(SIGNAL_PHASE_NUM3+1);
	clear_matrix_xd(SIGNAL_PHASE_NUM3+1);
}


void clear_g_opt()
{
	int i;
	for(i=0;i<8;i++)
		g_opt[i]=0.0;
}
void combine_A(float f[],float A1[][9], float A2[][9], float Aeq[][9], int m1, int m2, int m3)
{
	int i,j;
	for(i=0;i<9;i++)
		A[0][i]=f[i];
	for(i=0;i<m1;i++)
		for(j=0;j<9;j++)
			A[i+1][j]=A1[i][j];
	for(i=0;i<m2;i++)
		for(j=0;j<9;j++)
			A[i+1+m1][j]=A2[i][j];
	for(i=0;i<m3;i++)
		for(j=0;j<9;j++)
			A[i+1+m1+m2][j]=Aeq[i][j];
}
void combine_Ac(float fc[],float A1c[][SIGNAL_PHASE_NUM2+1], float A2c[][SIGNAL_PHASE_NUM2+1], float Aeqc[][SIGNAL_PHASE_NUM2+1], int m1c, int m2c, int m3c)
{
	int i,j;
	for(i=0;i<SIGNAL_PHASE_NUM2+1;i++)
		Ac[0][i]=fc[i];
	for(i=0;i<m1c;i++)
		for(j=0;j<SIGNAL_PHASE_NUM2+1;j++)
			Ac[i+1][j]=A1c[i][j];
	for(i=0;i<m2c;i++)
		for(j=0;j<SIGNAL_PHASE_NUM2+1;j++)
			Ac[i+1+m1c][j]=A2c[i][j];
	for(i=0;i<m3c;i++)
		for(j=0;j<SIGNAL_PHASE_NUM2+1;j++)
			Ac[i+1+m1c+m2c][j]=Aeqc[i][j];
}
void combine_Ad(float fd[],float A1d[][SIGNAL_PHASE_NUM3+1], float A2d[][SIGNAL_PHASE_NUM3+1], float Aeqd[][SIGNAL_PHASE_NUM3+1], int m1d, int m2d, int m3d)
{
	int i,j;
	for(i=0;i<SIGNAL_PHASE_NUM3+1;i++)
		Ad[0][i]=fd[i];
	for(i=0;i<m1d;i++)
		for(j=0;j<SIGNAL_PHASE_NUM3+1;j++)
			Ad[i+1][j]=A1d[i][j];
	for(i=0;i<m2d;i++)
		for(j=0;j<SIGNAL_PHASE_NUM3+1;j++)
			Ad[i+1+m1d][j]=A2d[i][j];
	for(i=0;i<m3d;i++)
		for(j=0;j<SIGNAL_PHASE_NUM3+1;j++)
			Ad[i+1+m1d+m2d][j]=Aeqd[i][j];
}

int test1()
{	//include contraints (1),(3),(5), test whether there is a feasible set, if return 0, means there is such a set, if return 1, must check the min green time/cycle contraints are correctly set
	int i;
	m1=0;m2=0;m3=0;
	for(i=0;i<SIGNAL_PHASE_NUM;i++)
		f[i+1]=-1.0;
	//g>=Gmin
	for(i=0;i<SIGNAL_PHASE_NUM;i++)
	{
		A2[m2][i+1]=-1.0;
		A2[m2][0]=(float) Gmin[i];	
		m2=m2+1;
	}

	A2[m2][0]=0.0;A2[m2][3]=1.0;A2[m2][7]=-1.0;
//	A2[m2][0]=0.0;A2[m2][3]=-1.0;A2[m2][4]=-1.0;A2[m2][5]=1.0;A2[m2][7]=1.0;
	m2=m2+1;
//	A2[m2][0]=0.0;A2[m2][4]=-1.0;A2[m2][5]=1.0;
//	m2=m2+1;
//	A2[m2][0]=0.0;A2[m2][6]=-1.0;A2[m2][7]=-1.0;A2[m2][8]=1.0;
//	m2=m2+1;

/*	Aeq[0][0]=cycle;Aeq[0][1]=-1.0;Aeq[0][2]=-1.0;Aeq[0][3]=-1.0;Aeq[0][4]=-1.0;
	Aeq[1][0]=cycle;Aeq[1][5]=-1.0;Aeq[1][6]=-1.0;Aeq[1][7]=-1.0;Aeq[1][8]=-1.0;
	Aeq[2][0]=0.0;Aeq[2][1]=-1.0;Aeq[2][2]=-1.0;Aeq[2][5]=1.0;Aeq[2][6]=1.0;
	Aeq[3][0]=0.0;Aeq[3][3]=-1.0;Aeq[3][4]=-1.0;Aeq[3][7]=1.0;Aeq[3][8]=1.0;*/

	Aeq[m3][0]=(float) cycle;Aeq[m3][1]=-1.0;Aeq[m3][2]=-1.0;Aeq[m3][7]=-1.0;
	m3=m3+1;
	Aeq[m3][0]=0.0;Aeq[m3][1]=-1.0;Aeq[m3][2]=-1.0;Aeq[m3][5]=1.0;Aeq[m3][6]=1.0;
	m3=m3+1;
	Aeq[m3][0]=0.0;Aeq[m3][6]=-1.0;Aeq[m3][7]=-1.0;Aeq[m3][8]=1.0;
	m3=m3+1;

//	m2=11;
//	m3=2;
//	m1=0;
	N=SIGNAL_PHASE_NUM;
	combine_A(f,A1,A2,Aeq,m1,m2,m3);
	float **aa;
	int icase;
	int *iposv;
	int *izrov;
	izrov=ivector(1,SIGNAL_PHASE_NUM);
	iposv=ivector(1,m1+m2+m3);
	aa=convert_matrix(&A[0][0],1,MP,1,NP);
	simplx(aa, M, N, m1, m2, m3, &icase,izrov,iposv);
	if(icase==0 || icase ==1)
	{
		free_convert_matrix(aa,1,MP,1,NP);
		free_ivector(iposv,1,M);
		free_ivector(izrov,1,N);
		return 0;
	}
	else
	{
		free_convert_matrix(aa,1,MP,1,NP);
		free_ivector(iposv,1,M);
		free_ivector(izrov,1,N);
		return 1;
	}
}

int test2()
{
	//sum(fsat*beta)<=RA
	A1[m1][0]=(float) RA;
	int i;
	for(i=0;i<8;i++)
	{
		A1[0][i+1]=(float) (-saturation_flow[i]*Beta[i]/3600.0);
	}
	m1=m1+1;

	N=SIGNAL_PHASE_NUM;
	combine_A(f,A1,A2,Aeq,m1,m2,m3);
	float **aa;
	int icase;
	int *iposv;
	int *izrov;
	izrov=ivector(1,SIGNAL_PHASE_NUM);
	iposv=ivector(1,m1+m2+m3);
	aa=convert_matrix(&A[0][0],1,MP,1,NP);
	simplx(aa, M, N, m1, m2, m3, &icase,izrov,iposv);
	if(icase==0 || icase ==1)
	{
		free_convert_matrix(aa,1,MP,1,NP);
		free_ivector(iposv,1,M);
		free_ivector(izrov,1,N);
		return 0;
	}
	else
	{
		free_convert_matrix(aa,1,MP,1,NP);
		free_ivector(iposv,1,M);
		free_ivector(izrov,1,N);
		return 1;
	}
}

int test3()
{
	combine_A(f,A1,A2,Aeq,m1,m2,m3);
	float **aa;
	int icase;
	int *iposv;
	int *izrov;
	izrov=ivector(1,SIGNAL_PHASE_NUM);
	iposv=ivector(1,m1+m2+m3);
	N=SIGNAL_PHASE_NUM;
	aa=convert_matrix(&A[0][0],1,MP,1,NP);
	simplx(aa, M, N, m1, m2, m3, &icase,izrov,iposv);
	if(icase==0 || icase ==1)
	{
		free_convert_matrix(aa,1,MP,1,NP);
		free_ivector(iposv,1,M);
		free_ivector(izrov,1,N);
		return 0;
	}
	else
	{
		free_convert_matrix(aa,1,MP,1,NP);
		free_ivector(iposv,1,M);
		free_ivector(izrov,1,N);
		return 1;
	}
}
void add_constraint_4()
{
	int i;
	for(i=0;i<SIGNAL_PHASE_NUM;i++)
	{
//		A1[m1+i][0]=(float) ((queue[i]+demand[i]*cycle/3600.0)/saturation_flow[i]*3600.0);
		A1[m1][0]=desired_green[i];
		A1[m1][i+1]=-1;
		m1=m1+1;
	}
	return;
}

float min_float(float a,float b)
{
	if(a<=b)
		return a;
	else
		return b;
}
void increase_upper_bound_in_constraint_4()
{
	int i;
//	for(i=m1-1;i>=m1-8;i--)
//	for(i=m1-SIGNAL_PHASE_NUM;i<m1;i++)
//		A1[i][0]=A1[i][0]*increase_upper_bound_ratio;
	int num_m1_old=m1-SIGNAL_PHASE_NUM;
	for(i=0;i<SIGNAL_PHASE_NUM;i++)
		A1[num_m1_old+i][0]=min_float(A1[num_m1_old+i][0]+2,Gmax[i]);
}
int choose_constraints()
{
	clear_all_matrix();
	int flag=test1();
	if(flag==1)	//test 1 fail
		return 1;
	flag=test2();
	if(flag==1)	//test 2 fail, some g_i=G_min,i
	{
		int i;
		for(i=0;i<SIGNAL_PHASE_NUM;i++)
		{
			if(Beta[i]>1e-3)
			{
				g_opt[i]=(float) Gmin[i];
				Aeq[m3][i+1]=-1;
				Aeq[m3][0]=(float) Gmin[i];
				m3=m3+1;
			}
		}
		clear_matrix_A1(m1,9);
		m1=0;
	}
	flag=test3();
	if(flag==1)
		return 2;//when set some g_i to G_min, still no feasible set
	add_constraint_4();
	flag=test3();
	int count=0;
	while(flag==1 && count<100)
	{
		count++;
		increase_upper_bound_in_constraint_4();
		flag=test3();
	}
	if(flag==1)
		return 3;//test 3 fail, the upper bound not feasible
	return 0;
}


int choose_constraints_test()
{
	clear_all_matrix();
	int flag=test1();
	if(flag==1)	//test 1 fail
		return 1;
	flag=test2();
	if(flag==1)	//test 2 fail, some g_i=G_min,i
	{
		int i;
		for(i=0;i<SIGNAL_PHASE_NUM;i++)
		{
			if(Beta[i]>1e-3)
			{
				g_opt[i]=(float) Gmin[i];
				Aeq[m3][i+1]=-1;
				Aeq[m3][0]=(float) Gmin[i];
				m3=m3+1;
			}
		}
		
		clear_matrix_A1(m1,9);
		m1=0;
		
	}

	add_constraint_4();
	flag=test3();
	int count=0;
	while(flag==1&&count<5)
	{
		increase_upper_bound_in_constraint_4();
		flag=test3();
		count++;
	}
	return 0;
}

void set_lin_ft()
{
	int i;
	for(i=0;i<SIGNAL_PHASE_NUM+1;i++)
		f_t[i]=0.0;
	for(i=SIGNAL_PHASE_NUM+1;i<SIGNAL_PHASE_NUM+1+SIGNAL_PHASE_NUM;i++)
		f_t[i]=-1.0*weight[i-SIGNAL_PHASE_NUM-1];
	return;
}



void combine_At(int m1_t, int m2_t, int m3_t)
{
	int i,j;
	for(i=0;i<SIGNAL_PHASE_NUM+1+SIGNAL_PHASE_NUM;i++)
		A_t[0][i]=f_t[i];
	for(i=0;i<m1_t;i++)
		for(j=0;j<SIGNAL_PHASE_NUM+1+SIGNAL_PHASE_NUM;j++)
			A_t[i+1][j]=A1_t[i][j];
	for(i=0;i<m2_t;i++)
		for(j=0;j<SIGNAL_PHASE_NUM+1+SIGNAL_PHASE_NUM;j++)
			A_t[i+1+m1_t][j]=A2_t[i][j];
	for(i=0;i<m3_t;i++)
		for(j=0;j<SIGNAL_PHASE_NUM+1+SIGNAL_PHASE_NUM;j++)
			A_t[i+1+m1_t+m2_t][j]=Aeq_t[i][j];
}
int transform_enlarge_matrix()
{
	int i,j;
	clear_matrix_A1t(40,SIGNAL_PHASE_NUM+1+SIGNAL_PHASE_NUM);
	m1_t=m1+SIGNAL_PHASE_NUM;
	for(i=0;i<m1;i++)
		for(j=0;j<SIGNAL_PHASE_NUM+1;j++)
			A1_t[i][j]=A1[i][j];
//	m1_t=m1;
	for(i=0;i<SIGNAL_PHASE_NUM;i++)
	{
		A1_t[m1+i][0]=desired_green[i];
		A1_t[m1+i][i+1]=-1.0;
		A1_t[m1+i][i+1+SIGNAL_PHASE_NUM]=1.0;
	}
	clear_matrix_A2t(40,SIGNAL_PHASE_NUM+1+SIGNAL_PHASE_NUM);
	m2_t=m2+SIGNAL_PHASE_NUM;
//	m2_t=m2;
	for(i=0;i<m2;i++)
		for(j=0;j<SIGNAL_PHASE_NUM+1;j++)
			A2_t[i][j]=A2[i][j];
	for(i=0;i<SIGNAL_PHASE_NUM;i++)
	{
		A2_t[m2+i][0]=desired_green[i];
		A2_t[m2+i][i+1]=-1.0;
		A2_t[m2+i][i+1+SIGNAL_PHASE_NUM]=-1.0;
	}
	clear_matrix_Aeqt(40,SIGNAL_PHASE_NUM+1+SIGNAL_PHASE_NUM);
	m3_t=m3;
	for(i=0;i<m3;i++)
		for(j=0;j<SIGNAL_PHASE_NUM+1;j++)
			Aeq_t[i][j]=Aeq[i][j];
	return 0;
}
int g_opt_linprog()
{
	clear_matrix_At(120,SIGNAL_PHASE_NUM+1+SIGNAL_PHASE_NUM);
	set_lin_ft();
	transform_enlarge_matrix();
	combine_At(m1_t,m2_t,m3_t);
	float **aa;
	int icase;
	int *iposv;
	int *izrov;
	izrov=ivector(1,SIGNAL_PHASE_NUM+SIGNAL_PHASE_NUM);
	iposv=ivector(1,m1_t+m2_t+m3_t);
	N=SIGNAL_PHASE_NUM+SIGNAL_PHASE_NUM;
	aa=convert_matrix(&A_t[0][0],1,m1_t+m2_t+m3_t+2,1,N+1);
	simplx(aa, m1_t+m2_t+m3_t, N, m1_t, m2_t, m3_t, &icase,izrov,iposv);

	if(icase==0)	//get solution
	{
		clear_x();
		int i,j;
		for(j=1;j<=m1_t+m2_t+m3_t;j++)
		{
			if(iposv[j]<=N)
				x[iposv[j]-1]=aa[j+1][1];
		}
		for(i=0;i<SIGNAL_PHASE_NUM;i++)
			g_opt[i]=(float) x[i];
	}	
	g_opt[3]=g_opt[6]-g_opt[2];
	return icase;

}


	

