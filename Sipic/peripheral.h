//
//  peripheral.h
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-25.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#ifndef Sipic_peripheral_h
#define Sipic_peripheral_h
#include "mem.h"
#include "core_24f.h"
#include "err.h"

#define  ISR_VECT_NUM_RESET0         0x00
#define  ISR_VECT_NUM_RESET1         0x02
#define  ISR_VECT_NUM_RESV0          0x04
#define  ISR_VECT_NUM_OSCI_TRAP      0x06
#define  ISR_VECT_NUM_ADDR_TRAP      0x08
#define  ISR_VECT_NUM_STAK_TRAP      0x0A
#define  ISR_VECT_NUM_MATH_TRAP      0x0C
#define  ISR_VECT_NUM_RESV1          0x0E
#define  ISR_VECT_NUM_RESV2          0x10
#define  ISR_VECT_NUM_RESV3          0x12


struct peripheral;

typedef struct peripheral {
    struct peripheral *Next;
    
} PERIPHERAL;

typedef  CPU_INT08U  ISR_VECT_NUM;

void Peripheral_Run(const  CORE_ERR  *p_core_err,
                           MEM_24    *p_mem_prog,
                           MEM       *p_mem_data,
                           CORE_24F  *p_core,
                           PERI_ERR  *p_err);

void Peri_ISR(ISR_VECT_NUM   isr_prio_num,
              MEM_24        *p_mem_prog,
              MEM           *p_mem_data,
              CORE_24F      *p_core,
              PERI_ERR      *p_err);

#endif
