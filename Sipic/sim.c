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
    SIM              *p_sim;
    PERIPHERAL       *p_peri;
    PERIPHERAL       *p_peri_next;
    HW_MOD           *p_hw;
    HW_MOD           *p_hw_next;
    PERI_ERR          peri_err;
    HW_IF            *p_hw_if;
    HW_IF_DATA_TYPE  *CNI_Pins_Tbl[CNI_PIN_CNT];
    HW_ERR            hw_err;
    
    
    p_sim                = &sim_struct;
    p_sim->p_core_data   = &core_data;
    p_sim->p_periph_head =  NULL;
    p_sim->p_hw_head     =  NULL;
    p_peri_next          =  NULL;
    p_hw_next            =  NULL;
    
    core_data.cycles         = 0;
    core_data.fcy            = FCY;
    core_data.p_context_head = NULL;
    
    /************* HW IF **************/
    p_hw = (HW_MOD *)malloc(sizeof(HW_MOD));
    
    p_hw->Type     = HW_TYPE_IF;
    p_hw_if        = HW_IF_Init(&hw_err);
    p_hw->p_device = p_hw_if;
    p_hw->p_next   = p_hw_next;
    
    p_hw_next      = p_hw;
    /*********** END HW IF ************/
    
    
    /************** OSC ***************/
    p_hw = (HW_MOD *)malloc(sizeof(HW_MOD));
    
    p_hw->Type     = WH_TYPE_OSC;
    p_hw->p_device = HW_OSC_Init(LINE_120V_OSC_PERIOD, LINE_120V_OSC_AMP, LINE_120V_OSC_OFFSET, &p_hw_if->RB3, &hw_err);
    p_hw->p_next   = p_hw_next;
    
    p_hw_next      = p_hw;
    /************ END OSC *************/

    /*********** 120V SYNC ************/
    p_hw = (HW_MOD *)malloc(sizeof(HW_MOD));
    
    p_hw->Type     = WH_TYPE_OSC;
    p_hw->p_device = HW_OSC_Init(RTCC_OSC_PERIOD, RTCC_OSC_AMP, RTCC_OSC_OFFSET, &p_hw_if->RC14, &hw_err);
    p_hw->p_next   = p_hw_next;
    
    p_hw_next      = p_hw;
    /********* END 120V SYNC **********/
    
    p_sim->p_hw_head = p_hw_next;

    /**********************************/
    /**********************************/
    /**********************************/

    /************** ADC ***************/
    p_peri = (PERIPHERAL *)malloc(sizeof(PERIPHERAL));
    
    
    p_peri->Type     = PERI_TYPE_ADC;
    p_peri->p_device = Peri_ADC_Init(p_sim->p_mem_data, &peri_err);
    p_peri->p_next   = p_peri_next;
    
    p_peri_next      = p_peri;
    /************ END ADC *************/

    

    /************* TMR 1 **************/
    p_peri = (PERIPHERAL *)malloc(sizeof(PERIPHERAL));
    
    
    p_peri->Type     = PERI_TYPE_TMR_A;
    p_peri->p_device = Peri_TmrA_Init(TMR_1_ADDR_TBL_IX,
                                     &p_hw_if->RC14,
                                      ISR_VECT_NUM_T1,
                                      p_sim->p_mem_data,
                                     &peri_err);
    p_peri->p_next   = p_peri_next;
    
    p_peri_next      = p_peri;
    /*********** END TMR 1 ************/

    /************ TMR 2/3 *************/
    p_peri = (PERIPHERAL *)malloc(sizeof(PERIPHERAL));
    
    
    p_peri->Type     = PERI_TYPE_TMR_BC;
    p_peri->p_device = Peri_TmrBC_Init(TMR_2_ADDR_TBL_IX,
                                      &p_hw_if->RC14,
                                       ISR_VECT_NUM_T2,
                                       ISR_VECT_NUM_T3,
                                       p_sim->p_mem_data,
                                      &peri_err);
    p_peri->p_next   = p_peri_next;
    
    p_peri_next      = p_peri;
    /********** END TMR 2/3 ***********/

    /************ TMR 4/5 *************/
    p_peri = (PERIPHERAL *)malloc(sizeof(PERIPHERAL));
    
    
    p_peri->Type     = PERI_TYPE_TMR_BC;
    p_peri->p_device = Peri_TmrBC_Init(TMR_4_ADDR_TBL_IX,
                                      &p_hw_if->RC14,
                                       ISR_VECT_NUM_T4,
                                       ISR_VECT_NUM_T5,
                                       p_sim->p_mem_data,
                                      &peri_err);
    p_peri->p_next   = p_peri_next;
    
    p_peri_next      = p_peri;
    /********** END TMR 4/5 ***********/
    
    /************** CNI ***************/
    p_peri = (PERIPHERAL *)malloc(sizeof(PERIPHERAL));
    
    
    
    CNI_Pins_Tbl[0] = &p_hw_if->RC14;   //CN0
    CNI_Pins_Tbl[1] = &p_hw_if->RC13    //CN1
    CNI_Pins_Tbl[2] = &p_hw_if->RB0;    //CN2
    CNI_Pins_Tbl[3] = &p_hw_if->RB1;    //CN3
    CNI_Pins_Tbl[4] = &p_hw_if->RB2;    //CN4
    CNI_Pins_Tbl[5] = &p_hw_if->RB3;    //CN5
    CNI_Pins_Tbl[6] = &p_hw_if->RB4;    //CN6
    CNI_Pins_Tbl[7] = &p_hw_if->RB5;    //CN7
    CNI_Pins_Tbl[8] = &p_hw_if->RF4;    //CN17
    CNI_Pins_Tbl[9] = &p_hw_if->RF5;    //CN18
    
    p_peri->Type     = PERI_TYPE_CNI;
    p_peri->p_device = Peri_CNI_Init(&CNI_Pins_Tbl,
                                      CNI_PIN_CNT,
                                      ISR_VECT_NUM_CNIF,
                                      p_sim->p_mem_data,
                                      &peri_err);
    p_peri->p_next   = p_peri_next;
    
    p_peri_next      = p_peri;
    /************ END CNI *************/
    
    /************** ISR ***************/
    p_peri = (PERIPHERAL *)malloc(sizeof(PERIPHERAL));
    
    
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