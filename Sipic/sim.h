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
#include "hardware.h"
#include "cpu.h"
#include "sipic_cfg.h"

typedef  struct  sim {
    CORE_24F    *p_core;
    MEM_24      *p_mem_prog;
    MEM         *p_mem_data;
    PERIPHERAL  *p_periph_head;
    HW_MOD      *p_hw_head;
    CORE_DATA   *p_core_data;
} SIM;
    
struct call_stack_entry;

typedef  struct  call_stack_entry {
    struct call_stack_entry  *next;
    CPU_INT32U  addr;
    CPU_INT32U  depth;
} CALL_STACK_ENTRY;
    
CALL_STACK_ENTRY  *CallStackHead;

void                                Sim_Init();

void                                Sim_Run();

void            DLL_API             Sim_Step();

unsigned short  DLL_API C_STD_CALL  Sim_GetValueFromDataMem(unsigned short addr);

unsigned int    DLL_API C_STD_CALL  Sim_GetOPCFromProgMem  (unsigned int addr);
    
void                                Sim_LinkCall           (CPU_INT32U  PC);
    
void                                Sim_UnlinkCall         (void);
    
CPU_INT32U                          Sim_GetCallDepth       (void);
    
CPU_INT32U                          Sim_AddrForDepth       (CPU_INT32U  depth);

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
}
#endif

#endif
