//
//  File.c
//  SipicUI
//
//  Created by Alexis Ouellet-Patenaude on 2012-11-11.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#include "peripheral.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ADC *Peri_ADC_Init(MEM       *p_mem_data,
                   PERI_ERR  *p_err)
{
    ADC       *p_adc;
    ADC_MEM   *p_mem;
    ADC_DATA  *p_data;
    MEM_ERR   mem_err;
    
    p_data = malloc(sizeof(ADC_DATA));
    
    if (p_data == NULL) {
        *p_err = PERI_ERR_ALLOC;
        return (NULL);
    }
    
    p_data->data = 0;
    
    p_mem = Mem_GetAddr(p_mem_data,
                        PERI_ADC_BASE_ADDR,
                        &mem_err);
    
    if (p_mem == NULL) {
        free(p_data);
        *p_err = PERI_ERR_INVALID_MEM;
        return (NULL);
    }
    
    memset(p_mem, 0x00, sizeof(ADC_MEM));
    
    p_adc = malloc(sizeof(ADC));
    
    if (p_adc == NULL) {
        free(p_data);
        *p_err = PERI_ERR_ALLOC;
        return (NULL);
    }
    
    p_adc->p_mem  = p_mem;
    p_adc->p_data = p_data;
    
    *p_err = PERI_ERR_NONE;
    return p_adc;
}

void Peri_ADC(MEM_24       *p_mem_prog,
              MEM          *p_mem_data,
              CORE_24F     *p_core,
              ADC          *p_adc,
              PERI_ERR     *p_err)
{
#if 0
    CPU_INT32U  PC;
    
    PC = Core_PC_Get(p_core);
    
    if (PC == 0x1C08) {
        printf("");
    }
#endif
    ADC_MEM  *p_adc_mem;
    
    
    p_adc_mem = p_adc->p_mem;
    
    if ((p_adc_mem->CON1 & ADC_ADCON1_ADON) == 0) {             /* ADC Module is off. No processing. */
        *p_err = PERI_ERR_NONE;
        return;
    }
    
    if (p_adc_mem->CON1 & ADC_ADCON1_ADSIDL) {                  /* ADC Module is stop when in idle mode. */
        *p_err = PERI_ERR_NONE;
        return;
    }
    
}