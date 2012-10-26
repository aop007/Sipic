//
//  sim.c
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-25.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#include <stdio.h>
#include "sim.h"
#include "core_24f.h"
#include "core_24f_opcode.h"

void Sim_Run(SIM  *p_sim)
{
    PERI_ERR    peri_err;
    CORE_ERR    core_err;
    
    core_err = CORE_ERR_NONE;
    peri_err = PERI_ERR_NONE;
    
    while (1) {
        
        /* Process HW */
        
        /* Process Peripherals */
        Peripheral_Run(&core_err,
                       p_sim->p_mem_prog,
                       p_sim->p_mem_data,
                       p_sim->p_core,
                       &peri_err);
        
        /* Process Core */
        Core_Run(p_sim->p_core,
                 p_sim->p_mem_prog,
                 p_sim->p_mem_data,
                &core_err);
        
    }
}