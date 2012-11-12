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
    HW_MOD      *p_hw;
    HW_MOD      *p_hw_next;
    PERI_ERR     peri_err;
    HW_IF       *p_hw_if;
    HW_ERR       hw_err;
    
    
    p_sim                = &sim_struct;
    p_sim->p_core_data   = &core_data;
    p_sim->p_periph_head =  NULL;
    p_sim->p_hw_head     =  NULL;
    p_peri_next          =  NULL;
    p_hw_next            =  NULL;
    
    core_data.cycles = 0;
    core_data.fcy    = 118662200u;
    
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
    /**********************************/
    /**********************************/
    /**********************************/
    
    /************* HW IF **************/
    p_hw = malloc(sizeof(HW_IF));
    
    
    p_hw->Type     = HW_TYPE_IF;
    p_hw_if        = HW_IF_Init(&hw_err);
    p_hw->p_device = p_hw_if;
    p_hw->p_next   = p_hw_next;
    
    p_hw_next      = p_hw;
    /*********** END HW IF ************/
    
    
    /************** OSC ***************/
    p_hw = malloc(sizeof(HW_OSC));
    
    
    p_hw->Type     = WH_TYPE_OSC;
    p_hw->p_device = HW_OSC_Init(RTCC_OSC_PERIOD, RTCC_OSC_AMP, RTCC_OSC_OFFSET, &p_hw_if->RC14, &hw_err);
    p_hw->p_next   = p_hw_next;
    
    p_hw_next      = p_hw;
    /************ END OSC *************/
    
    p_sim->p_hw_head = p_hw_next;
}

void Sim_Run()
{
    SIM        *p_sim;
    CORE_DATA  *p_core_data;
    PERI_ERR    peri_err;
    CORE_ERR    core_err;
    
    core_err    = CORE_ERR_NONE;
    peri_err    = PERI_ERR_NONE;
    p_sim       = &sim_struct;
    p_core_data = p_sim->p_core_data;
    
    
    
    while (1) {
        p_core_data->cycles++;
        
        /* Process HW */
        HW_Run(p_sim->p_mem_data,
               p_sim->p_hw_head,
               &hw_static_err);
        
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
    CORE_DATA  *p_core_data;

    p_sim       = &sim_struct;
    p_core_data = p_sim->p_core_data;
    
    p_core_data->cycles++;
    
    /* Process HW */
    HW_Run(p_sim->p_mem_data,
           p_sim->p_hw_head,
          &hw_static_err);
    
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