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
                Peripheral_ISR(       p_mem_prog,
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

void Peripheral_ISR(MEM_24      *p_mem_prog,
                    MEM         *p_mem_data,
                    CORE_24F    *p_core,
                    ISR         *p_isr,
                    PERI_ERR    *p_err)
{
    CPU_INT16U   int_prio;
    
    CPU_INT16U  ifs_ix;
    CPU_INT16U  iec_ix;
    CPU_INT16U  ipc_ix;
    
    CPU_INT16U  ifs_bit;
    CPU_INT16U  iec_bit;
    CPU_INT16U  ipc_mask;
    CPU_INT16U  ipc;
    
    CPU_INT16U  *p_ifs;
    CPU_INT16U  *p_iec;
    CPU_INT16U  *p_ipc;
    
    
    CPU_INT16S   prio_scanner;
    CPU_INT16U   highest_prio;
    CPU_INT16S   highest_prio_level;
    
    CPU_INT32U   isr_vector;
    CPU_INT32U   current_ipl;
    
    ISR_MEM     *p_isr_mem;
    
    
    p_isr_mem = p_isr->p_mem;
    
    /* Check for trap first */
    if (p_isr_mem->INTCON[0] & ISR_TRAP_MASK) {
        if (p_isr_mem->INTCON[0] & ISR_TRAP_NUM_OSCI) {
            Peri_ISR_FromVect(ISR_VECT_NUM_OSCI_TRAP, p_mem_prog, p_mem_data, p_core, p_err);
            return;
        }
        
        if (p_isr_mem->INTCON[0] & ISR_TRAP_NUM_ADDR) {
            Peri_ISR_FromVect(ISR_VECT_NUM_ADDR_TRAP, p_mem_prog, p_mem_data, p_core, p_err);
            return;
        }
        
        if (p_isr_mem->INTCON[0] & ISR_TRAP_NUM_STAK) {
            Peri_ISR_FromVect(ISR_VECT_NUM_STAK_TRAP, p_mem_prog, p_mem_data, p_core, p_err);
            return;
        }
        
        if (p_isr_mem->INTCON[0] & ISR_TRAP_NUM_MATH) {
            Peri_ISR_FromVect(ISR_VECT_NUM_MATH_TRAP, p_mem_prog, p_mem_data, p_core, p_err);
            return;
        }
    }
    
    if (p_isr_mem->INTCON[1] & IST_INTCON2_DISI) {
        *p_err = PERI_ERR_NONE;
        return;
    }
    
    
    /* Check for masked interrupts */
    if ((p_isr_mem->IFS[0] & p_isr_mem->IEC[0]) ||
        (p_isr_mem->IFS[1] & p_isr_mem->IEC[1]) ||
        (p_isr_mem->IFS[2] & p_isr_mem->IEC[2])) {
    
        prio_scanner       = sizeof(ISR_VECT_TBL) / sizeof(CPU_INT08U);
        p_ifs              = (CPU_INT16U *)&p_isr_mem->IFS[0];
        p_iec              = (CPU_INT16U *)&p_isr_mem->IEC[0];
        p_ipc              = (CPU_INT16U *)&p_isr_mem->IPC[0];
        highest_prio_level = -1;
        highest_prio       =  0;
        
        while (--prio_scanner >= 0) {
            int_prio = ISR_VECT_TBL[prio_scanner];
            ifs_ix = int_prio / 16;
            iec_ix = int_prio / 16;
            ipc_ix = int_prio /  4;

            ifs_bit  =           int_prio % 16;
            iec_bit  =           int_prio % 16;
            ipc_mask = 0x07 << ((int_prio %  4) * 4);
            
            ipc      = (p_ipc[ipc_ix] & ipc_mask) >> ((int_prio % 4) * 4);
            
            
            if ((p_ifs[ifs_ix]  & (1 << ifs_bit)) &&
                (p_iec[iec_ix]  & (1 << iec_bit)) &&
                (ipc           >= highest_prio_level)) {
                highest_prio_level = ipc;
                highest_prio       = int_prio;
            }
            
        }
        
        isr_vector  = highest_prio * 2 + ISR_VECT_BASE;
        
        current_ipl = (p_core->SR     & CORE_SR_IPL_MAKS >> 5) |
                      (p_core->CORCON & CORE_CORECON_IPL3);
        
        if ((current_ipl > 0) && (p_isr_mem->INTCON[0] & ISR_INTCON1_NSTDIS)) {     /* Interrupt prevented if nested disabled and interrupt currently occuring. */
            *p_err = PERI_ERR_NONE;
            return;
        }
        
        if (highest_prio_level > current_ipl) {
            Peri_ISR_FromVect(isr_vector, p_mem_prog, p_mem_data, p_core, p_err);
        }
        return;
    }
    
    *p_err = PERI_ERR_NONE;
}