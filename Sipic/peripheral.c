//
//  peripheral.c
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-25.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#include <stdio.h>
#include "peripheral.h"


void Peripheral_Run(const  CORE_ERR  *p_core_err,
                           MEM_24    *p_mem_prog,
                           MEM       *p_mem_data,
                           CORE_24F  *p_core,
                           PERI_ERR  *p_err)
{
    // Process Interrupt Engine
    switch (*p_core_err) {
        case CORE_ERR_ADDR_ERROR_TRAP:
            Peri_ISR(ISR_VECT_NUM_ADDR_TRAP,
                     p_mem_prog,
                     p_mem_data,
                     p_core,
                     p_err);
            break;
        
        case CORE_ERR_NONE:
        default:
            break;
    }
}