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
#include "main.h"
#include "peripheral.h"

void Sim_Init()
{
    SIM         *p_sim;
    PERIPHERAL  *p_peri;
    PERIPHERAL  *p_peri_next;
    PERI_ERR     peri_err;
    
    
    p_sim                = &sim_struct;
    p_sim->p_periph_head = NULL;
    p_peri_next          = NULL;
    
    /************** ADC ***************/
    p_peri = malloc(sizeof(PERIPHERAL));
    
    
    p_peri->Type     = PERI_TYPE_ADC;
    p_peri->p_device = Peri_ADC_Init(p_sim->p_mem_data, &peri_err);
    p_peri->p_next   = p_peri_next;
    
    p_peri_next      = p_peri;
    /************ END ADC *************/
    
    /************** ISR ***************/
    p_peri = malloc(sizeof(PERIPHERAL));
    
    
    p_peri->Type     = PERI_TYPE_ISR;
    p_peri->p_device = Peri_ISR_Init(p_sim->p_mem_data, &peri_err);
    p_peri->p_next   = p_peri_next;
    
    p_peri_next      = p_peri;
    /************ END ADC *************/
    
    
    p_sim->p_periph_head = p_peri_next;
}

void Sim_Run()
{
    SIM        *p_sim;
    PERI_ERR    peri_err;
    CORE_ERR    core_err;
    
    core_err = CORE_ERR_NONE;
    peri_err = PERI_ERR_NONE;
    p_sim    = &sim_struct;
    
    while (1) {
        
        /* Process HW */
        
        /* Process Peripherals */
        Peripheral_Run(&core_err,
                       p_sim->p_mem_prog,
                       p_sim->p_mem_data,
                       p_sim->p_core,
                       p_sim->p_periph_head,
                       &peri_err);
        
        /* Process Core */
        Core_Run(p_sim->p_core,
                 p_sim->p_mem_prog,
                 p_sim->p_mem_data,
                &core_err);
        
        //printf("\r\n%004x", Core_PC_Get(p_sim->p_core));
        
        if (core_err != CORE_ERR_NONE) {
            printf("\r\nCore Error no %d",core_err);
            printf(" ");
        }
        
    }
}

void Sim_Step()
{
    SIM        *p_sim;

    p_sim = &sim_struct;
    
        /* Process HW */
        
        /* Process Peripherals */
        Peripheral_Run(&core_static_err,
                       p_sim->p_mem_prog,
                       p_sim->p_mem_data,
                       p_sim->p_core,
                       p_sim->p_periph_head,
                       &peri_static_err);
        
        /* Process Core */
        Core_Run(p_sim->p_core,
                 p_sim->p_mem_prog,
                 p_sim->p_mem_data,
                &core_static_err);
}

unsigned short DLL_API C_STD_CALL Sim_GetValueFromDataMem(unsigned short addr)
{
    MEM_ERR  mem_err;

    return (Mem_Get(sim_struct.p_mem_data, addr, &mem_err));
}

unsigned int  DLL_API C_STD_CALL Sim_GetOPCFromProgMem(unsigned int addr)
{
    MEM_ERR  mem_err;

    return (Mem_Get24(sim_struct.p_mem_prog, addr, &mem_err));
}