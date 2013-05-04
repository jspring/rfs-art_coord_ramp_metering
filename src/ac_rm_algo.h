/* ac_rm_algo.h - structs for inputting controller status data to the
	control algorithm, and outputting changed green times, via the
	database
*/

#define DB_ALGO_INPUT_TYPE	2001
#define	DB_ALGO_INPUT_VAR	DB_ALGO_INPUT_TYPE
#define DB_ALGO_OUTPUT_TYPE	2002
#define	DB_ALGO_OUTPUT_VAR	DB_ALGO_OUTPUT_TYPE

typedef struct {
	int num_phases; // Number of phases used in calculation
	float cycle_time;
	float demand;
	float queue;
	float saturation_flow;
	float gmin;
	float beta;
	float weight;
	float ra;
} algo_input_t;

typedef struct {
	float g_opt;
} algo_output_t;
