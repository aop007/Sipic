	//
//  peripheral.c
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-25.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#include <stdio.h>
#include "peripheral.h"


void Peripheral_Run(const  CORE_ERR    *p_core_err,
                           MEM_24      *p_mem_prog,
                           MEM         *p_mem_data,
                           CORE_24F    *p_core,
                           PERIPHERAL  *p_peri_list,
                           PERI_ERR    *p_err)
{
#if 1
    PERIPHERAL  *p_peri;
    
    
    p_peri = p_peri_list;
    
    while (p_peri != NULL) {
    
        switch (p_peri->Type) {
            case PERI_TYPE_ISR:
                Peripheral_ISR(p_core_err,
                               p_mem_prog,
                               p_mem_data,
                               p_core,
                               p_err);
                break;
                
            case PERI_TYPE_ADC:
                Peri_ADC(       p_mem_prog,
                                p_mem_data,
                                p_core,
                         (ADC *)p_peri->p_device,
                                p_err);
                break;
                
            default:
                *p_err = PERI_ERR_INVALID_PERI_TYPE;
                return;
        }
        
        p_peri = p_peri->p_next;
    }
    
#else
    // Process Interrupt Engine
    
#endif
}

void Peripheral_ISR(const  CORE_ERR    *p_core_err,
                           MEM_24      *p_mem_prog,
                           MEM         *p_mem_data,
                           CORE_24F    *p_core,
                           PERI_ERR    *p_err)
{
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