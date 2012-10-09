//
//  core_24f.h
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-05.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#ifndef Sipic_core_24f_h
#define Sipic_core_24f_h

#include "cpu.h"
#include "mem.h"
#include "err.h"

typedef struct core_24f {
    
#if 0
    CPU_INT16U  W0;
    CPU_INT16U  W1;
    CPU_INT16U  W2;
    CPU_INT16U  W3;
    CPU_INT16U  W4;
    CPU_INT16U  W5;
    CPU_INT16U  W6;
    CPU_INT16U  W7;
    CPU_INT16U  W8;
    CPU_INT16U  W9;
    CPU_INT16U  W10;
    CPU_INT16U  W11;
    CPU_INT16U  W12;
    CPU_INT16U  W13;
    CPU_INT16U  W14;
    CPU_INT16U  W15;
#endif
    
    CPU_INT16U  W[16];
    
    CPU_INT16U  SPLIM;
    
    CPU_INT64U  ACCA;
    CPU_INT64U  ACCB;
    
    CPU_INT32U  PC;
    
    CPU_INT08U  TBLPAG;
    
    CPU_INT08U  PSVPAG;
    
    CPU_INT16U  RCOUNT;
    
    CPU_INT32U  DOSTART;
    
    CPU_INT32U  DOEND;
    
    union {
        CPU_INT16U SR;
        struct
        {
            CPU_INT08U SRH;
            CPU_INT08U SRL;
        }_s_s; }_s;
    
} CORE_24F;

extern  CORE_24F  Core;

typedef  CPU_INT16U  CORE_ERR;

void        Core_Push(CPU_INT32U   val,
                      CORE_24F    *p_core,
                      MEM         *p_mem,
                      CORE_ERR    *p_err);

CPU_INT32U  Core_Pop (CORE_24F    *p_core,
                      MEM         *p_mem,
                      CORE_ERR    *p_err);

void        Core_Run (MEM         *p_mem,
                      CORE_ERR    *p_err);

#endif
