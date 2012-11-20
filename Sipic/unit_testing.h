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

void  UT_Testting(SIM  *p_sim);

void  UT_DIV_U   (SIM  *p_sim);
void  UT_DIV_S   (SIM  *p_sim);

#endif