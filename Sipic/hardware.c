//
//  hardware.c
//  SipicUI
//
//  Created by Alexis Ouellet-Patenaude on 2012-11-11.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#include <stdio.h>
#include "hardware.h"


void HW_Run(MEM         *p_mem_data,
            HW_MOD      *p_hw_list,
            HW_ERR      *p_err)
{
    HW_MOD  *p_hw_mod;
    
    
    p_hw_mod = p_hw_list;
    
    while (p_hw_mod != NULL) {
        
        switch (p_hw_mod->Type) {
            case WH_TYPE_OSC:
                HW_OSC_Run(p_hw_mod->p_device, p_err);
                break;
                
                
            default:
                *p_err = HW_ERR_INVALID_HW_TYPE;
                return;
        }
        
        p_hw_mod = p_hw_mod->p_next;
    }
}