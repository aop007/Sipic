//
//  sim.h
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-25.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#ifndef Sipic_sim_h
#define Sipic_sim_h

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

#include "core_24f.h"
#include "peripheral.h"
#include "cpu.h"
#include "sipic_cfg.h"

typedef  struct  sim {
    CORE_24F    *p_core;
    MEM_24      *p_mem_prog;
    MEM         *p_mem_data;
    PERIPHERAL  *p_periph_head;
} SIM;

void                                Sim_Init();

void                                Sim_Run();

void            DLL_API             Sim_Step();

unsigned short  DLL_API C_STD_CALL  Sim_GetValueFromDataMem(unsigned short addr);

unsigned int    DLL_API C_STD_CALL  Sim_GetOPCFromProgMem(unsigned int addr);


#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
}
#endif

#endif
