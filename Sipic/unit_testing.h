//
//  unit_testing.c
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-05.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#ifndef  _UNIT_TESTING_
#define  _UNIT_TESTING_

#include "sim.h"
#include "core_24f_opcode.h"

typedef struct {
    CPU_INT16U tm_sec;		/* seconds after the minute - [0,59]   */
    CPU_INT16U tm_min;		/* minutes after the hour   - [0,59]   */
    CPU_INT16U tm_hour;	/* hours since midnight     - [0,23]   */
    CPU_INT16U tm_mday;	/* day of the month         - [1,31]   */
    CPU_INT16U tm_mon;		/* months since January     - [0,11]   */
    CPU_INT16U tm_year;	/* years since 1900					   */
    CPU_INT16U tm_wday;	/* days since Sunday         - [0,6]   */
    CPU_INT16U tm_yday;	/* days since January 1      - [0,365] */
    CPU_INT16U tm_isdst;	/* daylight savings time flag NOT USED */
} tm_struct;

typedef struct internal_binary_datetime {
    CPU_INT32U hour; 
	CPU_INT32U day; 
	CPU_INT32U minute; 
	CPU_INT32U second; 
	CPU_INT32U month; 
	CPU_INT32U year; 
	CPU_INT32U whole_minutes; 
	CPU_INT32U whole_hours; 
	CPU_INT32U whole_days; 
	CPU_INT32U whole_days_since_1968; 
	CPU_INT32U leap_year_periods; 
	CPU_INT32U days_since_current_lyear; 
	CPU_INT32U whole_years; 
	CPU_INT32U days_since_first_of_year; 
	CPU_INT32U days_to_month; 
	CPU_INT32U day_of_week;
} INTERNAL_BINARY_DATETIME;

void  UT_Testting(SIM  *p_sim);

void  UT_TestTimeConversion (MEM  *p_mem_data);

void  UT_DIV_U          (SIM  *p_sim);
void  UT_DIV_S          (SIM  *p_sim);
void  UT_SUB_F_EX1      (SIM  *p_sim);
void  UT_SUB_F_EX2      (SIM  *p_sim);
void  UT_DIV3232A       (SIM  *p_sim);
void  UT_XOR_Wb_Ws_Wd_B (SIM  *p_sim);
void  UT_XOR_Wb_Ws_Wd_W (SIM  *p_sim);
void  UT_RLC_Ws_Wd_B    (SIM  *p_sim);
void  UT_RLC_Ws_Wd_W    (SIM  *p_sim);
void  UT_CP_B           (SIM  *p_sim);
void  UT_CP_W           (SIM  *p_sim);
#endif