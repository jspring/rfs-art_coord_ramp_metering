#include <stdio.h>

char file_input[128]="C:\\Taylor\\MATLAB\\matlab_in.txt";
char file_output[128]="C:\\Taylor\\MATLAB\\matlab_out.txt";
#include "signal_opt.h"


int main()
{
	read_input();
	get_opt_green();
	save_g_opt();
	return 0;
}