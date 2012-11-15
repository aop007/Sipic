//
//  hw_osc.c
//  SipicUI
//
//  Created by Alexis Ouellet-Patenaude on 2012-11-11.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#include "hardware.h"
#include "mem.h"
#include "err.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

HW_OSC *HW_OSC_Init(CPU_INT32U        period,
                    float             amp,
                    float             offset,
                    HW_IF_DATA_TYPE  *p_pin,
                    HW_ERR           *p_err)
{
    HW_OSC    *p_hw_osc;
    OSC_DATA  *p_osc_data;
    
    
    p_osc_data = malloc(sizeof(OSC_DATA));
    
    if (p_osc_data == NULL) {
        *p_err = HW_ERR_ALLOC;
        return (NULL);
    }
    
    memset(p_osc_data, 0x00, sizeof(OSC_DATA));
    
    p_osc_data->phase      = 0;
    p_osc_data->period     = period;
    p_osc_data->amplitude  = amp;
    p_osc_data->offset     = offset;
    p_osc_data->p_pin      = p_pin;
    
    
    p_hw_osc = malloc(sizeof(HW_OSC));
    
    if (p_hw_osc == NULL) {
        free(p_osc_data);
        *p_err = HW_ERR_ALLOC;
        return (NULL);
    }
    
    p_hw_osc->p_data = p_osc_data;
    
    *p_err = HW_ERR_NONE;
    return p_hw_osc;
}

void HW_OSC_Run(HW_OSC  *p_hw_osc,
                HW_ERR  *p_err)
{
    OSC_DATA         *p_osc_data;
    HW_IF_DATA_TYPE  *p_pin;
    
    p_osc_data = p_hw_osc->p_data;
    p_pin      = p_osc_data->p_pin;
    
    p_osc_data->phase++;
    
    *p_pin = (HW_IF_DATA_TYPE)sin(2 * PI * p_osc_data->phase / p_osc_data->period) * p_osc_data->amplitude + p_osc_data->offset;
    
    if (p_osc_data->phase > p_osc_data->period) {
        p_osc_data->phase = 0;
    }
    
    //printf("\r\n%f", *p_pin);
    
    *p_err = HW_ERR_NONE;
}
