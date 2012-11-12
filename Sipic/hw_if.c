//
//  hw_if.c
//  SipicUI
//
//  Created by Alexis Ouellet-Patenaude on 2012-11-12.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#include <stdio.h>
#include "hardware.h"

HW_IF *HW_IF_Init(HW_ERR  *p_err)
{
    HW_IF            *p_hw_osc;
    HW_IF_DATA_TYPE  *p_pin;
    CPU_INT32U        pin_count;
    
    
    p_hw_osc = malloc(sizeof(HW_IF));
    
    if (p_hw_osc == NULL) {
        *p_err = HW_ERR_ALLOC;
        return (NULL);
    }
    
    p_pin     = (HW_IF_DATA_TYPE *)p_hw_osc;
    pin_count = sizeof(HW_IF) / sizeof(HW_IF_DATA_TYPE);
    
    while (pin_count > 0) {
        *p_pin = 0;
        p_pin++;
        pin_count--;
    }

    return (p_hw_osc);
}