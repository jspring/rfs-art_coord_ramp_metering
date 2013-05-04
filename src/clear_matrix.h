void clear_matrix_At(int m,int n)
{
	int i,j;
	for(i=0;i<m;i++)
		for(j=0;j<n;j++)
			A_t[i][j]=0.0;
}

void clear_matrix_H(int m,int n)
{
	int i,j;
	for(i=0;i<m;i++)
		for(j=0;j<n;j++)
			H[i][j]=0.0;
}

void clear_matrix_A1(int m,int n)
{
	int i,j;
	for(i=0;i<m;i++)
		for(j=0;j<n;j++)
			A1[i][j]=0.0;
}
void clear_matrix_A2(int m,int n)
{
	int i,j;
	for(i=0;i<m;i++)
		for(j=0;j<n;j++)
			A2[i][j]=0.0;
}
void clear_matrix_Aeq(int m,int n)
{
	int i,j;
	for(i=0;i<m;i++)
		for(j=0;j<n;j++)
			Aeq[i][j]=0.0;
}
void clear_matrix_A(int m,int n)
{
	int i,j;
	for(i=0;i<m;i++)
		for(j=0;j<n;j++)
			A[i][j]=0.0;
}
void clear_matrix_A1t(int m,int n)
{
	int i,j;
	for(i=0;i<m;i++)
		for(j=0;j<n;j++)
			A1_t[i][j]=0.0;
}
void clear_matrix_A2t(int m,int n)
{
	int i,j;
	for(i=0;i<m;i++)
		for(j=0;j<n;j++)
			A2_t[i][j]=0.0;
}
void clear_matrix_Aeqt(int m,int n)
{
	int i,j;
	for(i=0;i<m;i++)
		for(j=0;j<n;j++)
			Aeq_t[i][j]=0.0;
}
void clear_x()
{
	int i;
	for(i=0;i<SIGNAL_PHASE_NUM+SIGNAL_PHASE_NUM;i++)
		x[i]=0.0;
}

void clear_matrix_fc(int m)
{
	int i;
	for(i=0;i<m;i++)
		fc[i]=0.0;
}

void clear_matrix_xc(int m)
{
	int i;
	for(i=0;i<m;i++)
		xc[i]=0.0;
}
void clear_matrix_fd(int m)
{
	int i;
	for(i=0;i<m;i++)
		fd[i]=0.0;
}
void clear_matrix_xd(int m)
{
	int i;
	for(i=0;i<m;i++)
		xd[i]=0.0;
}
void clear_matrix_Ac(int m,int n)
{
	int i,j;
	for(i=0;i<m;i++)
		for(j=0;j<n;j++)
			Ac[i][j]=0.0;
}
void clear_matrix_A1c(int m,int n)
{
	int i,j;
	for(i=0;i<m;i++)
		for(j=0;j<n;j++)
			A1c[i][j]=0.0;
}
void clear_matrix_A2c(int m,int n)
{
	int i,j;
	for(i=0;i<m;i++)
		for(j=0;j<n;j++)
			A2c[i][j]=0.0;
}

void clear_matrix_Aeqc(int m,int n)
{
	int i,j;
	for(i=0;i<m;i++)
		for(j=0;j<n;j++)
			Aeqc[i][j]=0.0;
}

void clear_matrix_Ad(int m,int n)
{
	int i,j;
	for(i=0;i<m;i++)
		for(j=0;j<n;j++)
			Ad[i][j]=0.0;
}

void clear_matrix_A1d(int m,int n)
{
	int i,j;
	for(i=0;i<m;i++)
		for(j=0;j<n;j++)
			A1d[i][j]=0.0;
}

void clear_matrix_A2d(int m,int n)
{
	int i,j;
	for(i=0;i<m;i++)
		for(j=0;j<n;j++)
			A2d[i][j]=0.0;
}

void clear_matrix_Aeqd(int m,int n)
{
	int i,j;
	for(i=0;i<m;i++)
		for(j=0;j<n;j++)
			Aeqd[i][j]=0.0;
}
void write_matrix(float A[][9],int m,int n,char *str,FILE *fp)
{
	int i,j;
	fprintf(fp,"%s",str);
	fprintf(fp,"\n");
	for(i=0;i<m;i++)
	{
		for(j=0;j<n;j++)
			fprintf(fp,"%5.2f\t",A[i][j]);
		fprintf(fp,"\n");
	}
	fprintf(fp,"\n");
}
void write_matrix_8_2(float A[][8],int m,int n,char *str,FILE *fp)
{
	int i,j;
	fprintf(fp,"%s",str);
	fprintf(fp,"\n");
	for(i=0;i<m;i++)
	{
		for(j=0;j<n;j++)
			fprintf(fp,"%5.2f\t",A[i][j]);
		fprintf(fp,"\n");
	}
	fprintf(fp,"\n");
}
void write_matrix_8(double A[],int m,int n,char *str,FILE *fp)
{
	int i,j;
	fprintf(fp,"%s",str);
	fprintf(fp,"\n");

		for(j=0;j<n;j++)
			fprintf(fp,"%5.2lf\t",A[j]);
		fprintf(fp,"\n");

	fprintf(fp,"\n");
}

void write_matrix_8f(float A[],int m,int n,char *str,FILE *fp)
{
	int i,j;
	fprintf(fp,"%s",str);
	fprintf(fp,"\n");

		for(j=0;j<n;j++)
			fprintf(fp,"%5.2f\t",A[j]);
		fprintf(fp,"\n");

	fprintf(fp,"\n");
}
void write_matrix_17(float A[][17],int m,int n,char *str,FILE *fp)
{
	int i,j;
	fprintf(fp,"%s",str);
	fprintf(fp,"\n");
	for(i=0;i<m;i++)
	{
		for(j=0;j<n;j++)
			fprintf(fp,"%5.2f\t",A[i][j]);
		fprintf(fp,"\n");
	}
	fprintf(fp,"\n");
}
