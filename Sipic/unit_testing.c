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
#define  TIME_BINARY_BASE_ADDR  0x0862
#define  TIME_BINARY_BASE_SIZE  0x0004

#define  TIME_STRUCT_BASE_ADDR  0x0928
#define  TIME_STRUCT_BASE_SIZE  0x0012

void  UT_Testting(SIM  *p_sim)
{
    CORE_24F    *p_core;
    MEM         *p_mem;
    CPU_INT32U   pc;
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
    }

    if (pc == 0x0A62) {
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