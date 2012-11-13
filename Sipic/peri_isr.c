//
//  Peri_Interrupt.c
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-25.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#include <stdio.h>
#include "peripheral.h"
#include "main.h"


ISR *Peri_ISR_Init(MEM       *p_mem_data,
                   PERI_ERR  *p_err)
{
    ISR       *p_isr;
    ISR_MEM   *p_mem;
    ISR_DATA  *p_data;
    MEM_ERR   mem_err;
    
    p_data = malloc(sizeof(ISR_DATA));
    
    if (p_data == NULL) {
        *p_err = PERI_ERR_ALLOC;
        return (NULL);
    }
    
    p_data->data = 0;
    
    p_mem = Mem_GetAddr(p_mem_data,
                        PERI_ISR_BASE_ADDR,
                        &mem_err);
    
    if (p_mem == NULL) {
        free(p_data);
        *p_err = PERI_ERR_INVALID_MEM;
        return (NULL);
    }
    
    memset(p_mem, 0x00, sizeof(ISR_MEM));
    
    p_mem->IPC[0]  = 0x4444;
    p_mem->IPC[1]  = 0x4444;
    p_mem->IPC[2]  = 0x4444;
    p_mem->IPC[3]  = 0x4444;
    p_mem->IPC[4]  = 0x4444;
    p_mem->IPC[5]  = 0x4444;
    
    p_mem->IPC[6]  = 0x0044;
    p_mem->IPC[7]  = 0x0000;
    p_mem->IPC[9]  = 0x4044;
    p_mem->IPC[10] = 0x4004;
    p_mem->IPC[11] = 0x0000;
    
    p_isr = malloc(sizeof(ISR));
    
    if (p_isr == NULL) {
        free(p_data);
        *p_err = PERI_ERR_ALLOC;
        return (NULL);
    }
    
    p_isr->p_mem  = p_mem;
    p_isr->p_data = p_data;
    
    p_isr_static = p_isr;
    *p_err = PERI_ERR_NONE;
    return p_isr;
}

void Peri_ISR(MEM_24       *p_mem_prog,
              MEM          *p_mem_data,
              CORE_24F     *p_core,
              PERI_ERR     *p_err)
{
    
}

void Peri_ISR_FromVect(CPU_INT32U    isr_vect_addr,
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
    
    ISR_addr = Mem_Get24(p_mem_prog, isr_vect_addr, &mem_err);
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = PERI_ERR_INVALID_MEM;
    }
    
    Core_PC_Set(p_core, ISR_addr);
    
    Call_Depth++;
    
    *p_err = PERI_ERR_NONE;
}

void ISR_Post(ISR_VECT_NUM   vect_num,
              PERI_ERR      *p_err)
{
    ISR_MEM  *p_isr_mem;
    
    
    if (p_isr_static == NULL) {
        *p_err = PERI_ERR_INVALID_PTR;
        return;
    }
    
    p_isr_mem = p_isr_static->p_mem;
    
    switch(vect_num) {
        case ISR_VECT_NUM_INT0:
            p_isr_mem->IFS[0] |= DEF_BIT_00;
            break;
            
        case ISR_VECT_NUM_IC1:
            p_isr_mem->IFS[0] |= DEF_BIT_01;
            break;
            
        case ISR_VECT_NUM_OC1:
            p_isr_mem->IFS[0] |= DEF_BIT_02;
            break;
            
        case ISR_VECT_NUM_T1:
            p_isr_mem->IFS[0] |= DEF_BIT_03;
            break;
            
        case ISR_VECT_NUM_IC2:
            p_isr_mem->IFS[0] |= DEF_BIT_04;
            break;
            
        case ISR_VECT_NUM_OC2:
            p_isr_mem->IFS[0] |= DEF_BIT_05;
            break;
            
        case ISR_VECT_NUM_T2:
            p_isr_mem->IFS[0] |= DEF_BIT_06;
            break;
            
        case ISR_VECT_NUM_T3:
            p_isr_mem->IFS[0] |= DEF_BIT_07;
            break;
            
        case ISR_VECT_NUM_SPI1:
            p_isr_mem->IFS[0] |= DEF_BIT_08;
            break;
            
        case ISR_VECT_NUM_U1RX:
            p_isr_mem->IFS[0] |= DEF_BIT_09;
            break;
            
        case ISR_VECT_NUM_U1TX:
            p_isr_mem->IFS[0] |= DEF_BIT_10;
            break;
            
        case ISR_VECT_NUM_ADC:
            p_isr_mem->IFS[0] |= DEF_BIT_11;
            break;
            
        case ISR_VECT_NUM_NVM:
            p_isr_mem->IFS[0] |= DEF_BIT_12;
            break;
            
        case ISR_VECT_NUM_SI2C:
            p_isr_mem->IFS[0] |= DEF_BIT_13;
            break;
            
        case ISR_VECT_NUM_MI2C:
            p_isr_mem->IFS[0] |= DEF_BIT_14;
            break;
            
        case ISR_VECT_NUM_IC:
            p_isr_mem->IFS[0] |= DEF_BIT_15;
            break;
            
        case ISR_VECT_NUM_INT1:
            p_isr_mem->IFS[1] |= DEF_BIT_00;
            break;
            
        case ISR_VECT_NUM_IC7:
            p_isr_mem->IFS[1] |= DEF_BIT_01;
            break;
            
        case ISR_VECT_NUM_IC8:
            p_isr_mem->IFS[1] |= DEF_BIT_02;
            break;
            
        case ISR_VECT_NUM_OC3:
            p_isr_mem->IFS[1] |= DEF_BIT_03;
            break;
            
        case ISR_VECT_NUM_OC4:
            p_isr_mem->IFS[1] |= DEF_BIT_04;
            break;
            
        case ISR_VECT_NUM_T4:
            p_isr_mem->IFS[1] |= DEF_BIT_05;
            break;
            
        case ISR_VECT_NUM_T5:
            p_isr_mem->IFS[1] |= DEF_BIT_06;
            break;
            
        case ISR_VECT_NUM_INT2:
            p_isr_mem->IFS[1] |= DEF_BIT_07;
            break;
            
        case ISR_VECT_NUM_U2RX:
            p_isr_mem->IFS[1] |= DEF_BIT_08;
            break;
            
        case ISR_VECT_NUM_U2TX:
            p_isr_mem->IFS[1] |= DEF_BIT_09;
            break;
            
        case ISR_VECT_NUM_PWM:
            p_isr_mem->IFS[2] |= DEF_BIT_07;
            break;
            
        case ISR_VECT_NUM_QEI:
            p_isr_mem->IFS[2] |= DEF_BIT_08;
            break;
            
        case ISR_VECT_NUM_FLTA:
            p_isr_mem->IFS[2] |= DEF_BIT_11;
            break;
            
        default:
            *p_err = PERI_ERR_INVALID_ISR_NUM;
            return;
    }
    
    *p_err = PERI_ERR_NONE;
}

void Trap_Post(IST_TRAP_NUM   trap_num,
               PERI_ERR      *p_err)
{
    ISR_MEM  *p_isr_mem;
    
    
    if (p_isr_static == NULL) {
        *p_err = PERI_ERR_INVALID_PTR;
        return;
    }
    
    p_isr_mem = p_isr_static->p_mem;

    p_isr_mem->INTCON[0] |= trap_num;
    
    *p_err = PERI_ERR_NONE;
}