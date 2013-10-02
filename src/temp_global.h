#ifndef		_TEMP_GLOBAL_H_
#define		_TEMP_GLOBAL_H_

#ifndef _TEMP_GLOBAL_H_DECLS
#define _DECL extern
#else
#define _DECL
#endif

_DECL	float hr,min,sec;
_DECL	int green_yellow_overlap,presence1,presence2,presence3,presence4,greens,yellows,reds;
	//redudant
_DECL	char c1,c2;
_DECL	unsigned int a1,a2,a3,a4,a5,a6,a7,a8;
_DECL	int lead_stat_0,lead_vol_0,trail_stat_0,trail_vol_0;
_DECL	int lead_stat_1,lead_vol_1,trail_stat_1,trail_vol_1;
_DECL	int lead_stat_2,lead_vol_2,trail_stat_2,trail_vol_2;
_DECL	float lead_occ_0,trail_occ_0;
_DECL	float lead_occ_1,trail_occ_1;
_DECL	float lead_occ_2,trail_occ_2;

_DECL	int demand_vol_2,passage_vol_2,queue_vol_2,b1_2,b2_2,b3_2;
_DECL	float queue_occ_2;
_DECL	int demand_vol_1,passage_vol_1,queue_vol_1,b1_1,b2_1,b3_1;
_DECL	float queue_occ_1;
_DECL	int demand_vol_0,passage_vol_0,queue_vol_0,b1_0,b2_0,b3_0;
_DECL	float queue_occ_0;
_DECL	int rate_0,rate_1,rate_2;

#endif