//
//  Peri_Interrupt.c
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-25.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#include <stdio.h>
#include "peripheral.h"


void Peri_ISR(ISR_VECT_NUM  isr_vect_num,
              MEM_24       *p_mem_prog,
              MEM          *p_mem_data,
              CORE_24F     *p_core,
              PERI_ERR     *p_err)
{
    CPU_INT32U  PC;
    CPU_INT32U  SR;
    CPU_INT32U  CORCON;
    CPU_INT32U  ISR_addr;
    CORE_ERR    core_err;
    MEM_ERR     mem_err;
    
    
    PC      = Core_PC_Get(p_core) + 2;
    SR      = (p_core->SR & 0x00FF);
    CORCON  = (p_core->CORCON & CORE_CORECON_IPL3);
    
    PC     |= ((SR << 24) | (CORCON << 16));
    
    Core_Push((PC & 0x0000FFFF),       p_core, p_mem_data, &core_err);
    
    if (core_err != CORE_ERR_NONE) {
        *p_err = PERI_ERR_INVALID_CORE_OP;
    }
    
    Core_Push((PC & 0xFFFF0000) >> 16, p_core, p_mem_data, &core_err);
    
    if (core_err != CORE_ERR_NONE) {
        *p_err = PERI_ERR_INVALID_CORE_OP;
    }
    
    ISR_addr = Mem_Get24(p_mem_prog, isr_vect_num, &mem_err);
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = PERI_ERR_INVALID_MEM;
    }
    
    Core_PC_Set(p_core, ISR_addr);
    
    Call_Depth++;
    
    *p_err = PERI_ERR_NONE;
}