float alpha=1.3;
int get_g5_g8()
{
	fc[1]=(float)(saturation_flow[4]/3600.0*Beta[4]/RA);
	fc[2]=(float)(saturation_flow[7]/3600.0*Beta[7]/RA);
	fc[3]=-1.0;
	m3c=0;
	m2c=2;
	m1c=6;
	//g5>gmin5
	A2c[0][0]=Gmin[4];
	A2c[0][1]=-1.0;
	//g8>gmin8
	A2c[1][0]=Gmin[7];
	A2c[1][2]=-1.0;
	//g5<gmax5
	A1c[0][0]=Gmax[4];
	A1c[0][1]=-1.0;
	//g8<gmax8
	A1c[1][0]=Gmax[7];
	A1c[1][2]=-1.0;
	//sum(f*beta)<RA
	A1c[2][0]=RA;
	A1c[2][1]=(float)(-saturation_flow[4]/3600.0*Beta[4]);
	A1c[2][2]=(float)(-saturation_flow[7]/3600.0*Beta[7]);
	//g5+g8<cycle
	A1c[3][0]=cycle;
	A1c[3][1]=-1.0;
	A1c[3][2]=-1.0;
	//epsilon
	A1c[4][0]=0.0;
	A1c[4][1]=(float)(-1.0/desired_green[4]);
	A1c[4][2]=alpha/desired_green[7];
	A1c[4][3]=1.0;
	A1c[5][0]=0.0;
	A1c[5][1]=(float)(1.0/desired_green[4]);
	A1c[5][2]=-alpha/desired_green[7];
	A1c[5][3]=1.0;
	//combine matrix
	combine_Ac(fc,A1c,A2c,Aeqc,m1c,m2c,m3c);
	//linprog get g5,g8
	Nc=SIGNAL_PHASE_NUM2;
	float **aa;
	int icase;
	int *iposv;
	int *izrov;
	izrov=ivector(1,SIGNAL_PHASE_NUM2);
	iposv=ivector(1,m1c+m2c+m3c);
	aa=convert_matrix(&Ac[0][0],1,MPc,1,NPc);
	simplx(aa, Mc, Nc, m1c, m2c, m3c, &icase,izrov,iposv);
	if(icase==0)	//get solution
	{
//		clear_x();	//not added, clear matrices
		int i,j;
		for(j=1;j<=m1c+m2c+m3c;j++)
		{
			if(iposv[j]<=Nc)
				xc[iposv[j]-1]=aa[j+1][1];
		}
		g_opt[4]=xc[0];
		g_opt[7]=xc[1];

	}	
	return 0;
}

int get_remain_g()
{
	m1d=10;m2d=9;m3d=2;
	//get g_opt after g5,g8 are got
	fd[6]=-1.0;
	fd[7]=-1.0;
	fd[8]=-10.0;
	fd[9]=-10.0;
	//g>=Gmin
	//g1
	A2d[0][0]=Gmin[0];
	A2d[0][1]=-1.0;
	//g2
	A2d[1][0]=Gmin[1];
	A2d[1][2]=-1.0;
	//g3
	A2d[2][0]=Gmin[2];
	A2d[2][3]=-1.0;
	//g6
	A2d[3][0]=Gmin[5];
	A2d[3][4]=-1.0;
	//g7
	A2d[4][0]=Gmin[6];
	A2d[4][5]=-1.0;
	//epsilon
	//g1
	A2d[5][0]=desired_green[0];
	A2d[5][1]=-1.0;
	A2d[5][6]=-1.0;
	//g3
	A2d[6][0]=desired_green[2];
	A2d[6][3]=-1.0;
	A2d[6][7]=-1.0;
	//g6
	A2d[7][0]=desired_green[5];
	A2d[7][4]=-1.0;
	A2d[7][8]=-1.0;
	//g7
	A2d[8][0]=desired_green[6];
	A2d[8][5]=-1.0;
	A2d[8][9]=-1.0;

	//g1+g2+g3<=cycle
	A1d[0][0]=cycle;
	A1d[0][1]=-1.0;
	A1d[0][2]=-1.0;
	A1d[0][3]=-1.0;
	//g<=gmax
	//g1
	A1d[1][0]=Gmax[0];
	A1d[1][1]=-1.0;
	//g2
	A1d[2][0]=Gmax[1];
	A1d[2][2]=-1.0;
	//g3
	A1d[3][0]=Gmax[2];
	A1d[3][3]=-1.0;
	//g6
	A1d[4][0]=Gmax[5];
	A1d[4][4]=-1.0;
	//g7
	A1d[5][0]=Gmax[6];
	A1d[5][5]=-1.0;
	//epsilon
	//g1
	A1d[6][0]=desired_green[0];
	A1d[6][1]=-1.0;
	A1d[6][6]=1.0;
	//g3
	A1d[7][0]=desired_green[2];
	A1d[7][3]=-1.0;
	A1d[7][7]=1.0;
	//g6
	A1d[8][0]=desired_green[5];
	A1d[8][4]=-1.0;
	A1d[8][8]=1.0;
	//g7
	A1d[9][0]=desired_green[6];
	A1d[9][5]=-1.0;
	A1d[9][9]=1.0;

	//g6+g7=cycle-g5
	Aeqd[0][0]=cycle-g_opt[4];
	Aeqd[0][4]=-1.0;
	Aeqd[0][5]=-1.0;
	//g1+g2-g6=g5
	Aeqd[1][0]=g_opt[4];
	Aeqd[1][1]=-1.0;
	Aeqd[1][2]=-1.0;
	Aeqd[1][4]=1.0;
	//combine matrix
	combine_Ad(fd,A1d,A2d,Aeqd,m1d,m2d,m3d);
	//linprog get g5,g8
	Nd=SIGNAL_PHASE_NUM3;
	float **aa;
	int icase;
	int *iposv;
	int *izrov;
	izrov=ivector(1,SIGNAL_PHASE_NUM3);
	iposv=ivector(1,m1d+m2d+m3d);
	aa=convert_matrix(&Ad[0][0],1,MPd,1,NPd);
	simplx(aa, Md, Nd, m1d, m2d, m3d, &icase,izrov,iposv);
	if(icase==0)	//get solution
	{
//		clear_x();	//not added, clear matrices
		int i,j;
		for(j=1;j<=m1d+m2d+m3d;j++)
		{
			if(iposv[j]<=Nd)
				xd[iposv[j]-1]=aa[j+1][1];
		}
		g_opt[0]=xd[0];
		g_opt[1]=xd[1];
		g_opt[2]=xd[2];
		g_opt[5]=xd[3];
		g_opt[6]=xd[4];
		g_opt[3]=g_opt[6]-g_opt[2];

	}	
	return 0;
}

int get_opt_green_via_method_2()
{
	clear_g_opt();
	clear_all_matrix2();
	get_g5_g8();
	get_remain_g();
	return 0;
}