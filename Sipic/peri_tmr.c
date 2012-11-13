//
//  peri_tmr.c
//  SipicUI
//
//  Created by Alexis Ouellet-Patenaude on 2012-11-11.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#include "peripheral.h"
#include "hardware.h"
#include "mem.h"
#include "cpu.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TMR_A *Peri_TmrA_Init(CPU_INT32U        tmr_nbr,
                      HW_IF_DATA_TYPE  *p_pin,
                      ISR_VECT_NUM      isr_vect_num,
                      MEM              *p_mem_data,
                      PERI_ERR         *p_err)
{
    TMR_A       *p_tmr;
    TMR_A_MEM   *p_mem;
    TMR_DATA    *p_data;
    MEM_ERR      mem_err;
    
    p_data = (TMR_DATA *)malloc(sizeof(TMR_DATA));
    
    if (p_data == NULL) {
        *p_err = PERI_ERR_ALLOC;
        return (NULL);
    }
    
    memset(p_data, 0x00, sizeof(TMR_DATA));

    p_data->p_pin        = p_pin;
    p_data->isr_vect_num = isr_vect_num;
    
    p_mem = (TMR_A_MEM *)Mem_GetAddr(p_mem_data,
                                     PERI_TMR_BASE_ADDR[tmr_nbr],
                                    &mem_err);
    
    if (p_mem == NULL) {
        free(p_data);
        *p_err = PERI_ERR_INVALID_MEM;
        return (NULL);
    }
    
    memset(p_mem, 0x00, sizeof(ADC_MEM));
    
    p_tmr = (TMR_A *)malloc(sizeof(TMR_A));
    
    if (p_tmr == NULL) {
        free(p_data);
        *p_err = PERI_ERR_ALLOC;
        return (NULL);
    }
    
    p_tmr->p_mem  = p_mem;
    p_tmr->p_data = p_data;
    
    *p_err = PERI_ERR_NONE;
    return p_tmr;
}

void Peri_TMR_A(MEM_24       *p_mem_prog,
                MEM          *p_mem_data,
                CORE_24F     *p_core,
                TMR_A        *p_tmr,
                PERI_ERR     *p_err)
{
    TMR_A_MEM   *p_tmr_mem;
    TMR_DATA    *p_tmr_data;
    CPU_INT16U   tckps;
    
    
    p_tmr_mem  = p_tmr->p_mem;
    p_tmr_data = p_tmr->p_data;

    if ((p_tmr_mem->CON & TMR_CON_TON) != TMR_CON_TON) {
        *p_err = PERI_ERR_NONE;
        return;
    }

    if ((p_tmr_mem->CON & TMR_CON_TCS) == TMR_CON_TCS) {
        if (p_tmr_data->ext_input_state == TMR_EXT_STATE_LO) {
            if (*p_tmr_data->p_pin >= TMR_EXT_LO_TO_HI_TRSH) {
                p_tmr_data->ext_input_state = TMR_EXT_STATE_HI;
                p_tmr_data->inputTick++;
            }
        } else {
            if (*p_tmr_data->p_pin <= TMR_EXT_HI_TO_LO_TRSH) {
                p_tmr_data->ext_input_state = TMR_EXT_STATE_LO;
            }
        }
    } else {
        p_tmr_data->inputTick++;
    }

    /* Pre Scaling */
    tckps = (p_tmr_mem->CON & TMR_CON_TCKPS) >> 4;

    if (p_tmr_data->inputTick >= PERI_TMR_TCKPS[tckps]) {
        p_tmr_data->inputTick = 0;
        p_tmr_mem->TMR++;
    }

    /* Register Compare */
    if (p_tmr_mem->TMR >= p_tmr_mem->PR) {
        p_tmr_mem->TMR = 0;

        /* Interrupt posting */
        ISR_Post(p_tmr_data->isr_vect_num, p_err);
        
        printf("\r\n%lu", core_data.cycles);
        
        if (*p_err != PERI_ERR_NONE) {
            return;
        }
    }

    *p_err = PERI_ERR_NONE;
}