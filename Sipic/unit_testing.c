//
//  unit_testing.c
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-05.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#include "unit_testing.h"

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

void  UT_CleanContext(SIM  *p_sim) {
    CORE_ERR  core_err;
    
    Core_Reset(p_sim->p_core, &core_err);
}

void  UT_Testting(SIM  *p_sim)
{
    UT_CleanContext(p_sim);
    UT_SUB_F_EX1(p_sim);

    UT_CleanContext(p_sim);
    UT_SUB_F_EX2(p_sim);

    UT_CleanContext(p_sim);
    UT_DIV_U(p_sim);

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


    p_core       = p_sim->p_core;
    p_mem        = p_sim->p_mem_prog;

    p_core->W[0] = 0x5555;
    p_core->W[1] = 0x1234;
    p_core->W[7] = 0x8000;
    p_core->W[8] = 0x0200;
    p_core->SR   = 0x0000;

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

    if (p_core->SR   != 0x0002) {
        printf("\r\np_core->SR   != 0x0002");
    }


}

void UT_DIV_S(SIM * p_sim) {
    CORE_24F  *p_core;
    MEM_24    *p_mem;
    MEM_ERR    mem_err;
    CORE_ERR   core_err;


    p_core       = p_sim->p_core;
    p_mem        = p_sim->p_mem_prog;

    p_core->W[0] = 0x5555;
    p_core->W[1] = 0x1234;
    p_core->W[4] = 0x3000;
    p_core->W[3] = 0x0027;
    p_core->SR   = 0x0000;

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

    if (p_core->SR   != 0x0000) {
        printf("\r\np_core->SR   != 0x0000");
    }


}