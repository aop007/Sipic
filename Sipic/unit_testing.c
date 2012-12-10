//
//  unit_testing.c
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-05.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#include "unit_testing.h"

#define TEST_WREG(wreg, val)    if (!((p_sim->p_core->W[wreg]) == val)) printf("\r\nW[%d] != %04X. val = %04X",wreg,val,p_sim->p_core->W[wreg])

#define TEST_DMEM(addr, val)    if (!((Mem_Get(p_sim->p_mem_data, (addr & 0xFFFE), &mem_err)) == val)) printf("\r\n[%d] != %04X. val = %04X",addr,val, Mem_Get(p_sim->p_mem_data, (addr & 0xFFFE), &mem_err))

#define TEST_SR(val)            if (p_sim->p_core->SR != val) printf("\r\nSR != %04X. SR = %04X",val,p_sim->p_core->SR)


#define TEST_SRC_W(x)             (x & 0xF) 
#define TEST_SRC_MODE(x)          (x & 0x7) <<  4
#define TEST_DST_W(x)             (x & 0xF) <<  7
#define TEST_DST_MODE(x)          (x & 0x7) << 11
#define TEST_BAS_W(x)             (x & 0xF) << 15
#define TEST_BYTE                   0x004000
#define TEST_WORD                   0x000000



tm_struct   time_buffer_before;
tm_struct   time_buffer_after;
CPU_INT32U  time_binary_before;
CPU_INT32U  time_binary_after;

//0x0862-0x0863 CurrentTimeBinary 
#define  TIME_BINARY_BASE_ADDR  0x0866
#define  TIME_BINARY_BASE_SIZE  0x0004

#define  TIME_STRUCT_BASE_ADDR  0x0928
#define  TIME_STRUCT_BASE_SIZE  0x0012

CPU_INT32U DaysToMonth[13] = {0,31,59,90,120,151,181,212,243,273,304,334,365}; 

CPU_INT32U DS1371_DateToBinary(tm_struct *datetime) { 
    CPU_INT32U iday; 
    CPU_INT32U val; 
    
    
    iday = 365 * (datetime->tm_year - 70) + DaysToMonth[datetime->tm_mon] + (datetime->tm_mday - 1); 
    iday = iday + (datetime->tm_year - 69) / 4; 
    
    if ((datetime->tm_mon > 1) && ((datetime->tm_year % 4) == 0)) { 
        iday++; 
    } 
    
    val = datetime->tm_sec + 60 * datetime->tm_min + 3600 * (datetime->tm_hour + 24 * iday); 
    return val; 
}

void DS1371_BinaryToDate(unsigned long binary, tm_struct *datetime, INTERNAL_BINARY_DATETIME *ibd) 
{ 
    /*
	unsigned long hour; 
	unsigned long day; 
	unsigned long minute; 
	unsigned long second; 
	unsigned long month; 
	unsigned long year; 
	unsigned long whole_minutes; 
	unsigned long whole_hours; 
	unsigned long whole_days; 
	unsigned long whole_days_since_1968; 
	unsigned long leap_year_periods; 
	unsigned long days_since_current_lyear; 
	unsigned long whole_years; 
	unsigned long days_since_first_of_year; 
	unsigned long days_to_month; 
	unsigned long day_of_week; 
    */
	 
	ibd->whole_minutes = binary / 60; 

	ibd->second = binary - (60 * ibd->whole_minutes);			// leftover seconds 

	ibd->whole_hours = ibd->whole_minutes / 60; 

	ibd->minute = ibd->whole_minutes - (60 * ibd->whole_hours);	// leftover minutes 

	ibd->whole_days	= ibd->whole_hours / 24; 

	ibd->hour	= ibd->whole_hours - (24 * ibd->whole_days);		// leftover hours 

	 
	   
	ibd->whole_days_since_1968 = ibd->whole_days + 365 + 366;  

	ibd->leap_year_periods = ibd->whole_days_since_1968 / ((4 * 365) + 1);	 

	ibd->days_since_current_lyear = ibd->whole_days_since_1968 % ((4 * 365) + 1); 

	 
	// if days are after a current leap year then add a leap year period 
	if ((ibd->days_since_current_lyear >= (31 + 29))) 

	{ 
		ibd->leap_year_periods++; 

	}  
	 
	ibd->whole_years = (ibd->whole_days_since_1968 - ibd->leap_year_periods) / 365; 

	ibd->days_since_first_of_year = ibd->whole_days_since_1968 - (ibd->whole_years * 365) - ibd->leap_year_periods; 

 
	if ((ibd->days_since_current_lyear <= 365) && (ibd->days_since_current_lyear >= 60)) 

	{ 
		ibd->days_since_first_of_year++; 

	} 
	 
	ibd->year = ibd->whole_years + 68; 

 
	// setup for a search for what month it is based on how many days have past 
	//	within the current year 
	ibd->month = 13; 

	ibd->days_to_month = 366; 

	 
	while (ibd->days_since_first_of_year < ibd->days_to_month) 
	{ 

		ibd->month--; 

		ibd->days_to_month = DaysToMonth[ibd->month-1]; 

		if ((ibd->month > 2) && ((ibd->year % 4) == 0)) 

		{ 
			ibd->days_to_month++; 

		} 
		 
		ibd->day = ibd->days_since_first_of_year - ibd->days_to_month + 1; 

		ibd->day_of_week = (ibd->whole_days + 4) % 7; 

	} 

	 
	datetime->tm_yday = ibd->days_since_first_of_year;	/* days since January 1 - [0,365]	*/ 

	datetime->tm_sec = ibd->second;						/* seconds after the minute - [0,59]	*/ 

	datetime->tm_min = ibd->minute;						/* minutes after the hour - [0,59]	*/ 

	datetime->tm_hour = ibd->hour;						/* hours since midnight - [0,23]	*/ 

	datetime->tm_mday = ibd->day;						/* day of the month - [1,31]	*/ 

	datetime->tm_wday = ibd->day_of_week;				/* days since Sunday - [0,6]	*/ 

	datetime->tm_mon = ibd->month;						/* months since January - [0,11]	*/ 

	datetime->tm_year = ibd->year;						/* years since 1900	*/ 

} 


void  UT_CleanContext(SIM  *p_sim) {
    CORE_ERR  core_err;
    
    Core_Reset(p_sim->p_core, &core_err);

    printf("\r\n");
}

void  UT_Testting(SIM  *p_sim)
{
    //tm_struct   time_buffer;
    //CPU_INT32U  binary_time;


    /* Test Time Conversion Functions. */
#if 0
    binary_time = 1317319200;
    DS1371_BinaryToDate(binary_time, &time_buffer);
    printf("\r\n%d/%d/%d\t%d:%d:%d\t%d",time_buffer.tm_mday,time_buffer.tm_mon,time_buffer.tm_year + 1900, time_buffer.tm_hour,time_buffer.tm_min,time_buffer.tm_sec, binary_time);

    binary_time++;
    DS1371_BinaryToDate(binary_time, &time_buffer);
    printf("\r\n%d/%d/%d\t%d:%d:%d\t%d",time_buffer.tm_mday,time_buffer.tm_mon,time_buffer.tm_year + 1900, time_buffer.tm_hour,time_buffer.tm_min,time_buffer.tm_sec, binary_time);
#endif

    UT_CleanContext(p_sim);
    UT_SUB_F_EX1(p_sim);

    UT_CleanContext(p_sim);
    UT_SUB_F_EX2(p_sim);

    UT_CleanContext(p_sim);
    UT_DIV_U(p_sim);

    UT_CleanContext(p_sim);
    UT_DIV_S(p_sim);

    UT_CleanContext(p_sim);
    UT_XOR_Wb_Ws_Wd_B(p_sim);

    UT_CleanContext(p_sim);
    UT_XOR_Wb_Ws_Wd_W(p_sim);

    UT_CleanContext(p_sim);
    UT_RLC_Ws_Wd_B(p_sim);

    UT_CleanContext(p_sim);
    UT_RLC_Ws_Wd_W(p_sim);
    
    UT_CleanContext(p_sim);
    UT_CP_B(p_sim);

    UT_CleanContext(p_sim);
    UT_CP_W(p_sim);

    /**/
    UT_CleanContext(p_sim);
    UT_DIV3232A(p_sim);

    while(1) {
    
    }

#if 0
     CORE_24F    *p_core;
    MEM         *p_mem;
    CPU_INT32U   pc;
    CPU_INT32U   time_binary;
    MEM_ERR      mem_err;
    CORE_ERR     core_err;


    p_core       = p_sim->p_core;
    p_mem        = p_sim->p_mem_data;


    pc = Core_PC_Get(p_core);

    if (pc == 0x0A5E) {
        Mem_Load((void *)&time_buffer_before, TIME_STRUCT_BASE_ADDR, TIME_STRUCT_BASE_SIZE, p_mem, &mem_err);
        Mem_Load((void *)&time_binary_before, TIME_BINARY_BASE_ADDR, TIME_BINARY_BASE_SIZE, p_mem, &mem_err);
        printf("\r\n%d/%d/%d\t%d:%d:%d\t%d",
            time_buffer_before.tm_mday,
            time_buffer_before.tm_mon,
            time_buffer_before.tm_year + 1900,
            time_buffer_before.tm_hour,
            time_buffer_before.tm_min,
            time_buffer_before.tm_sec,
            time_binary_before);

        time_binary = DS1371_DateToBinary(&time_buffer_before);
    }

    if (pc == 0x0AAA) {
        Mem_Load((void *)&time_buffer_after, TIME_STRUCT_BASE_ADDR, TIME_STRUCT_BASE_SIZE, p_mem, &mem_err);
        Mem_Load((void *)&time_binary_after, TIME_BINARY_BASE_ADDR, TIME_BINARY_BASE_SIZE, p_mem, &mem_err);
        printf("\r\n%d/%d/%d\t%d:%d:%d\t%d",
            time_buffer_after.tm_mday,
            time_buffer_after.tm_mon,
            time_buffer_after.tm_year + 1900,
            time_buffer_after.tm_hour,
            time_buffer_after.tm_min,
            time_buffer_after.tm_sec,
            time_binary_after);

        time_binary = DS1371_DateToBinary(&time_buffer_after);
    }
#endif
}

#define  UT_SUB_F_ADDR_F1  0x0A05

void UT_SUB_F_EX1(SIM * p_sim) {
    CORE_24F    *p_core;
    MEM_24      *p_mem;
    CPU_INT16U   val;
    MEM_ERR      mem_err;
    CORE_ERR     core_err;
     

    p_core       = p_sim->p_core;
    p_mem        = p_sim->p_mem_prog;

    p_core->W[0] = 0x7804;
    p_core->SR   = 0x0000;

    Mem_Set(p_sim->p_mem_data, (UT_SUB_F_ADDR_F1 & 0xFFFE), 0x9439, &mem_err);

    /* SUB.B 0x1FFF */
    Mem_Set24(p_mem, 0, (0xB56000 | UT_SUB_F_ADDR_F1), &mem_err);

    Core_PC_Set(p_core, 0);

    Core_Run(p_sim->p_core,
             p_sim->p_mem_prog,
             p_sim->p_mem_data,
            &core_err);

    val = Mem_Get(p_sim->p_mem_data, (UT_SUB_F_ADDR_F1 & 0xFFFE), &mem_err);

    if (p_core->W[0] != 0x7804) {
        printf("\r\np_core->W[0] != 0x7804");
    }

    if (val != 0x9039) {
        printf("\r\n*0x1FFE != 0x9039");
    }

    if (p_core->SR != 0x0009) {
        printf("\r\np_core->SR != 0x0009");
    }
}

#define  UT_SUB_F_ADDR_F2  0x0A04

void UT_SUB_F_EX2(SIM * p_sim) {
    CORE_24F    *p_core;
    MEM_24      *p_mem;
    CPU_INT16U   val;
    MEM_ERR      mem_err;
    CORE_ERR     core_err;
     

    p_core       = p_sim->p_core;
    p_mem        = p_sim->p_mem_prog;

    p_core->W[0] = 0x6234;
    p_core->SR   = 0x0000;

    Mem_Set(p_sim->p_mem_data, (UT_SUB_F_ADDR_F2 & 0xFFFE), 0x4523, &mem_err);

    Mem_Set24(p_mem, 0, (0xB50000 | UT_SUB_F_ADDR_F2), &mem_err);

    Core_PC_Set(p_core, 0);

    Core_Run(p_sim->p_core,
             p_sim->p_mem_prog,
             p_sim->p_mem_data,
            &core_err);

    val = Mem_Get(p_sim->p_mem_data, (UT_SUB_F_ADDR_F2 & 0xFFFE), &mem_err);

    if (p_core->W[0] != 0xE2EF) {
        printf("\r\np_core->W[0] != 0xE2EF");
    }

    if (val != 0x4523) {
        printf("\r\n*0x1FFE != 0x4523");
    }

    if (p_core->SR != 0x0008) {
        printf("\r\np_core->SR != 0x0008");
    }
}

void UT_DIV_U(SIM * p_sim) {
    CORE_24F  *p_core;
    MEM_24    *p_mem;
    MEM_ERR    mem_err;
    CORE_ERR   core_err;


    p_core         = p_sim->p_core;
    p_mem          = p_sim->p_mem_prog;

    p_core->W[0]   = 0x5555;
    p_core->W[1]   = 0x1234;
    p_core->W[7]   = 0x8000;
    p_core->W[8]   = 0x0200;
    p_core->RCOUNT = 0x0001;
    p_core->SR     = CORE_SR_RA;

    Mem_Set24(p_sim->p_mem_prog, 0, 0xD88388, &mem_err);

    Core_PC_Set(p_core, 0);

    Core_Run(p_sim->p_core,
             p_sim->p_mem_prog,
             p_sim->p_mem_data,
            &core_err);

    if (p_core->W[0] != 0x0040) {
        printf("\r\np_core->W[0] != 0x0040");
    }

    if (p_core->W[1] != 0x0000) {
        printf("\r\np_core->W[1] != 0x0000");
    }

    if (p_core->W[7] != 0x8000) {
        printf("\r\np_core->W[7] != 0x8000");
    }

    if (p_core->W[8] != 0x0200) {
        printf("\r\np_core->W[8] != 0x0200");
    }

    if ((p_core->SR & ~(CORE_SR_RA)) != 0x0002) {
        printf("\r\np_core->SR   != 0x0002");
    }


}

void UT_DIV_S(SIM * p_sim) {
    CORE_24F  *p_core;
    MEM_24    *p_mem;
    MEM_ERR    mem_err;
    CORE_ERR   core_err;


    p_core         = p_sim->p_core;
    p_mem          = p_sim->p_mem_prog;

    p_core->W[0]   = 0x5555;
    p_core->W[1]   = 0x1234;
    p_core->W[4]   = 0x3000;
    p_core->W[3]   = 0x0027;
    p_core->RCOUNT = 0x0001;
    p_core->SR     = CORE_SR_RA;

    Core_PC_Set(p_core, 0);

    Mem_Set24(p_sim->p_mem_prog, 0, 0x00d80203, &mem_err);

    Core_Run(p_sim->p_core,
             p_sim->p_mem_prog,
             p_sim->p_mem_data,
            &core_err);

    if (p_core->W[0] != 0x13B) {
        printf("\r\np_core->W[0] != 0x0040");
    }

    if (p_core->W[1] != 0x0003) {
        printf("\r\np_core->W[1] != 0x0003");
    }

    if (p_core->W[4] != 0x3000) {
        printf("\r\np_core->W[4] != 0x3000");
    }

    if (p_core->W[3] != 0x0027) {
        printf("\r\np_core->W[3] != 0x0027");
    }

    if ((p_core->SR & ~(CORE_SR_RA)) != 0x0000) {
        printf("\r\np_core->SR   != 0x0000");
    }


}

void UT_DIV3232A(SIM * p_sim)
{
    CORE_24F  *p_core;
    MEM_24    *p_mem;
    MEM_ERR    mem_err;
    CORE_ERR   core_err;


    p_core         = p_sim->p_core;
    p_mem          = p_sim->p_mem_prog;

    p_core->W[0]   = 0xB221;
    p_core->W[1]   = 0x4E84;
    p_core->W[2]   = 0x003C;
    p_core->W[3]   = 0x0000;
    p_core->SR    &= ~(CORE_SR_DC);

    Mem_Set24(p_sim->p_mem_prog, 0, 0x0204B6, &mem_err);
    Mem_Set24(p_sim->p_mem_prog, 2, 0x000000, &mem_err);

    Core_PC_Set(p_core, 0);

    while (Core_PC_Get(p_core) != 4) {

        Core_Run(p_sim->p_core,
                 p_sim->p_mem_prog,
                 p_sim->p_mem_data,
                &core_err);

    }

    printf("\r\nUT_DIV3232A Done.");

    if (p_core->W[0] != 0x02F8) {
        printf("\r\np_core->W[0] != 0x02F8: 0x%04X", p_core->W[0]);
    }

    if (p_core->W[1] != 0x014F) {
        printf("\r\np_core->W[1] != 0x014F: 0x%04X", p_core->W[1]);
    }

}

#define  XOR_MEM_1  0x2000
#define  XOR_MEM_2  0x2600

void UT_XOR_Wb_Ws_Wd_B(SIM * p_sim)
{
    CORE_24F  *p_core;
    MEM_24    *p_mem;
    MEM_ERR    mem_err;
    OPCODE     opcode;
    CORE_ERR   core_err;


    p_core         = p_sim->p_core;
    p_mem          = p_sim->p_mem_prog;

    p_core->W[1]   = 0xAAAA;
    p_core->W[5]   = XOR_MEM_1;
    p_core->W[9]   = XOR_MEM_2;
    p_core->SR     = CORE_SR_NONE;

    /* XOR.B  W1, [W5++], [W9++] p.5-262 */
    opcode  = 0x680000;
    opcode |= 0x004000;     /* Byte mode */
    opcode |= 0x008000;     /* Wb = W1   */
    opcode |= 0x001800;     /* Dst Addr Mode = 011 [++] */
    opcode |= 0x000480;     /* Dst Wreg = W9 */
    opcode |= 0x000030;     /* Src Addr Mode = 011 [++] */
    opcode |= 0x000005;     /* Src Wreg = W5 */

    Mem_Set24(p_sim->p_mem_prog, 0, opcode, &mem_err);

    Mem_Set(p_sim->p_mem_data, XOR_MEM_1, 0x115A, &mem_err);
    Mem_Set(p_sim->p_mem_data, XOR_MEM_2, 0x0000, &mem_err);

    Core_PC_Set(p_core, 0);

    Core_Run(p_sim->p_core,
                p_sim->p_mem_prog,
                p_sim->p_mem_data,
            &core_err);

    TEST_WREG(1 ,0xAAAA);
    TEST_WREG(5 ,(XOR_MEM_1+1));
    TEST_WREG(9 ,(XOR_MEM_2+1));

    TEST_DMEM(XOR_MEM_1, 0x115A);
    TEST_DMEM(XOR_MEM_2, 0x00F0);

    TEST_SR(CORE_SR_N);
}

void UT_XOR_Wb_Ws_Wd_W(SIM * p_sim)
{
    CORE_24F  *p_core;
    MEM_24    *p_mem;
    MEM_ERR    mem_err;
    OPCODE     opcode;
    CORE_ERR   core_err;


    p_core         = p_sim->p_core;
    p_mem          = p_sim->p_mem_prog;

    p_core->W[1]   = 0xFEDC;
    p_core->W[5]   = 0x1234;
    p_core->W[9]   = 0xA34D;
    p_core->SR     = CORE_SR_NONE;

    /* XOR  W1, W5, W9  p.5-263 */
    opcode  = 0x680000;
    //opcode |= 0x004000;     /* Byte mode */
    opcode |= 0x008000;     /* Wb = W1   */
    opcode |= 0x000000;     /* Dst Addr Mode = 000 */
    opcode |= 0x000480;     /* Dst Wreg = W9 */
    opcode |= 0x000000;     /* Src Addr Mode = 000 */
    opcode |= 0x000005;     /* Src Wreg = W5 */

    Mem_Set24(p_sim->p_mem_prog, 0, opcode, &mem_err);

    Core_PC_Set(p_core, 0);

    Core_Run(p_sim->p_core,
                p_sim->p_mem_prog,
                p_sim->p_mem_data,
            &core_err);

    TEST_WREG(1 ,0xFEDC);
    TEST_WREG(5 ,0x1234);
    TEST_WREG(9 ,0xECE8);

    TEST_SR(CORE_SR_N);
}


void UT_RLC_Ws_Wd_B(SIM * p_sim)
{
    CORE_24F  *p_core;
    MEM_24    *p_mem;
    MEM_ERR    mem_err;
    OPCODE     opcode;
    CORE_ERR   core_err;


    p_core         = p_sim->p_core;
    p_mem          = p_sim->p_mem_prog;

    p_core->W[0]   = 0x9976;
    p_core->W[3]   = 0x5879;
    p_core->SR     = CORE_SR_C;

    /* RLC.B  W0, W3 p.5-205 */
    opcode  = CORE_OPC_RLC_W;
    opcode |= TEST_BYTE;
    opcode |= TEST_SRC_MODE(CORE_OPC_ADDR_MODE_DIR);
    opcode |= TEST_SRC_W(0);
    opcode |= TEST_DST_MODE(CORE_OPC_ADDR_MODE_DIR);
    opcode |= TEST_DST_W(3);


    Mem_Set24(p_sim->p_mem_prog, 0, opcode, &mem_err);

    Core_PC_Set(p_core, 0);

    Core_Run(p_sim->p_core,
                p_sim->p_mem_prog,
                p_sim->p_mem_data,
            &core_err);

    TEST_WREG(0 , 0x9976);
    TEST_WREG(3 , 0x58ED);

    TEST_SR(CORE_SR_N);
}

void UT_RLC_Ws_Wd_W(SIM * p_sim)
{
    CORE_24F  *p_core;
    MEM_24    *p_mem;
    MEM_ERR    mem_err;
    OPCODE     opcode;
    CORE_ERR   core_err;


    p_core         = p_sim->p_core;
    p_mem          = p_sim->p_mem_prog;

    p_core->W[2]   = 0x2008;
    p_core->W[8]   = 0x094E;
    p_core->SR     = CORE_SR_C;

    Mem_Set(p_sim->p_mem_data, 0x094E, 0x3689, &mem_err);
    Mem_Set(p_sim->p_mem_data, 0x2008, 0xC041, &mem_err);

    /* RLC  [W2++], [W8] p.5-206 */
    opcode  = CORE_OPC_RLC_W;
    opcode |= TEST_WORD;
    opcode |= TEST_SRC_MODE(CORE_OPC_ADDR_MODE_IND_POS_INC);
    opcode |= TEST_SRC_W(2);
    opcode |= TEST_DST_MODE(CORE_OPC_ADDR_MODE_IND);
    opcode |= TEST_DST_W(8);


    Mem_Set24(p_sim->p_mem_prog, 0, opcode, &mem_err);



    Core_PC_Set(p_core, 0);

    Core_Run(p_sim->p_core,
                p_sim->p_mem_prog,
                p_sim->p_mem_data,
            &core_err);

    TEST_WREG(2 , 0x200A);
    TEST_WREG(8 , 0x094E);

    TEST_DMEM(0x094E, 0x8083);
    TEST_DMEM(0x2008, 0xC041);

    TEST_SR((CORE_SR_N|CORE_SR_C));
}

void UT_CP_B(SIM * p_sim)
{
    CORE_24F  *p_core;
    MEM_24    *p_mem;
    MEM_ERR    mem_err;
    OPCODE     opcode;
    CORE_ERR   core_err;
    
    
    p_core         = p_sim->p_core;
    p_mem          = p_sim->p_mem_prog;
    
    p_core->W[0]   = 0xABA9;
    p_core->W[1]   = 0x2000;
    p_core->SR     = CORE_SR_NONE;
    
    Mem_Set(p_sim->p_mem_data, 0x2000, 0xD004, &mem_err);
    
    /* CP.B  W0, [W1++] p.5-84 */
    opcode  = CORE_OPC_CP_WB_WS;
    opcode |= 0x000400;
    opcode |= TEST_SRC_MODE(CORE_OPC_ADDR_MODE_IND_POS_INC);
    opcode |= TEST_SRC_W(1);
    opcode |= 0 << 11;
    
    Mem_Set24(p_sim->p_mem_prog, 0, opcode, &mem_err);

    Core_PC_Set(p_core, 0);
    
    Core_Run(p_sim->p_core,
             p_sim->p_mem_prog,
             p_sim->p_mem_data,
             &core_err);
    
    TEST_WREG(0 , 0xABA9);
    TEST_WREG(1 , 0x2001);
    
    TEST_DMEM(0x2000, 0xD004);
    
    TEST_SR((CORE_SR_N));
}

void UT_CP_W(SIM * p_sim)
{
    CORE_24F  *p_core;
    MEM_24    *p_mem;
    MEM_ERR    mem_err;
    OPCODE     opcode;
    CORE_ERR   core_err;
    
    
    p_core         = p_sim->p_core;
    p_mem          = p_sim->p_mem_prog;
    
    p_core->W[5]   = 0x2334;
    p_core->W[6]   = 0x8001;
    p_core->SR     = CORE_SR_NONE;
        
    /* CP.B  W0, [W1++] p.5-84 */
    opcode  = CORE_OPC_CP_WB_WS;
    opcode |= 0 << 10;
    opcode |= TEST_SRC_MODE(CORE_OPC_ADDR_MODE_DIR);
    opcode |= TEST_SRC_W(6);
    opcode |= 5 << 11;
     
    Mem_Set24(p_sim->p_mem_prog, 0, opcode, &mem_err);
    
    Core_PC_Set(p_core, 0);
    
    Core_Run(p_sim->p_core,
             p_sim->p_mem_prog,
             p_sim->p_mem_data,
             &core_err);
    
    TEST_WREG(5 , 0x2334);
    TEST_WREG(6 , 0x8001);
    
    TEST_SR((CORE_SR_N|CORE_SR_OV));
}

void UT_TestTimeConversion(MEM  *p_mem_data)
{
    tm_struct                 time_buffer_pic;
    tm_struct                 time_buffer_pc;
    CPU_INT32U                binary_time;
    INTERNAL_BINARY_DATETIME  ibd_pc;
    INTERNAL_BINARY_DATETIME  ibd_pic;
    MEM_ERR                   mem_err;


    Mem_Load((void *)&time_buffer_pic, 0x0928, 18, p_mem_data, &mem_err);
    Mem_Load((void *)&binary_time,     0x0866,  4, p_mem_data, &mem_err);
    
        
    printf("\r\n%02d/%02d/%04d\t%02d:%02d:%02d\t%d",
                                        time_buffer_pic.tm_mday,
                                        time_buffer_pic.tm_mon,
                                        time_buffer_pic.tm_year + 1900,
                                        time_buffer_pic.tm_hour,
                                        time_buffer_pic.tm_min,
                                        time_buffer_pic.tm_sec,
                                        binary_time);

    DS1371_BinaryToDate(binary_time, &time_buffer_pc, &ibd_pc);

    printf("\r\n%02d/%02d/%04d\t%02d:%02d:%02d\t%d",
                                        time_buffer_pc.tm_mday,
                                        time_buffer_pc.tm_mon,
                                        time_buffer_pc.tm_year + 1900,
                                        time_buffer_pc.tm_hour,
                                        time_buffer_pc.tm_min,
                                        time_buffer_pc.tm_sec,
                                        binary_time);

    Mem_Load((void *)&ibd_pic, 0x0A56, sizeof(ibd_pic), p_mem_data, &mem_err);

    if (ibd_pic.whole_minutes            != ibd_pc.whole_minutes) { printf("\r\n*whole_minutes");}
    if (ibd_pic.second                   != ibd_pc.second) { printf("\r\n*second");}
    if (ibd_pic.whole_hours              != ibd_pc.whole_hours) { printf("\r\n*whole_hours");}
    if (ibd_pic.minute                   != ibd_pc.minute) { printf("\r\n*minute");}
    if (ibd_pic.whole_days               != ibd_pc.whole_days) { printf("\r\n*whole_days");}
    if (ibd_pic.hour                     != ibd_pc.hour) { printf("\r\n*hour");}
    if (ibd_pic.whole_days_since_1968    != ibd_pc.whole_days_since_1968) { printf("\r\n*whole_days_since_1968");}
    if (ibd_pic.leap_year_periods        != ibd_pc.leap_year_periods) { printf("\r\n*leap_year_periods");}
    if (ibd_pic.days_since_current_lyear != ibd_pc.days_since_current_lyear) { printf("\r\n*days_since_current_lyear");}
    if (ibd_pic.whole_years              != ibd_pc.whole_years) { printf("\r\n*whole_years");}
    if (ibd_pic.days_since_first_of_year != ibd_pc.days_since_first_of_year) { printf("\r\n*days_since_first_of_year");}
    if (ibd_pic.year                     != ibd_pc.year) { printf("\r\n*year");}
    if (ibd_pic.month                    != ibd_pc.month) { printf("\r\n*month");}
    if (ibd_pic.days_to_month            != ibd_pc.days_to_month) { printf("\r\n*days_to_month");}
    if (ibd_pic.day                      != ibd_pc.day) { printf("\r\n*day");}
    if (ibd_pic.day_of_week              != ibd_pc.day_of_week) { printf("\r\n*day_of_week");}
}