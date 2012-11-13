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
    PERIPHERAL  *p_peri;
    
    
    p_peri = p_peri_list;
    
    while (p_peri != NULL) {
    
        switch (p_peri->Type) {
            case PERI_TYPE_ISR:
                Peri_ISR(       p_mem_prog,
                                p_mem_data,
                                p_core,
                         (ISR *)p_peri->p_device,
                                p_err);
                break;
                
            case PERI_TYPE_ADC:
                Peri_ADC(       p_mem_prog,
                                p_mem_data,
                                p_core,
                         (ADC *)p_peri->p_device,
                                p_err);
                break;

            case PERI_TYPE_TMR_A:
                Peri_TMR_A(         p_mem_prog,
                                    p_mem_data,
                                    p_core,
                           (TMR_A *)p_peri->p_device,
                                    p_err);
                break;
                
            default:
                *p_err = PERI_ERR_INVALID_PERI_TYPE;
                return;
        }
        
        p_peri = p_peri->p_next;
    }
}

