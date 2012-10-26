//
//  Peri_Interrupt.c
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-25.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#include <stdio.h>
#include "peripheral.h"

void Peri_ISR(ISR_VECT_NUM  isr_prio_num,
              MEM_24       *p_mem_prog,
              MEM          *p_mem_data,
              CORE_24F     *p_core,
              PERI_ERR     *p_err)
{
    //
}