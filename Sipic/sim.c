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
    CPU_INT08U        CNI_Bits_Tbl[CNI_PIN_CNT];

    HW_IF_DATA_TYPE  *IO_Pins_Tbl[PERI_IO_CNT];
    IO_PORT_BIT       IO_Bits_Tbl[PERI_IO_CNT];

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
    CNI_Pins_Tbl[1] = &p_hw_if->RC13;   //CN1
    CNI_Pins_Tbl[2] = &p_hw_if->RB0;    //CN2
    CNI_Pins_Tbl[3] = &p_hw_if->RB1;    //CN3
    CNI_Pins_Tbl[4] = &p_hw_if->RB2;    //CN4
    CNI_Pins_Tbl[5] = &p_hw_if->RB3;    //CN5
    CNI_Pins_Tbl[6] = &p_hw_if->RB4;    //CN6
    CNI_Pins_Tbl[7] = &p_hw_if->RB5;    //CN7
    CNI_Pins_Tbl[8] = &p_hw_if->RF4;    //CN17
    CNI_Pins_Tbl[9] = &p_hw_if->RF5;    //CN18

    CNI_Bits_Tbl[0] =  0u;
    CNI_Bits_Tbl[1] =  1u;
    CNI_Bits_Tbl[2] =  2u;
    CNI_Bits_Tbl[3] =  3u;
    CNI_Bits_Tbl[4] =  4u;
    CNI_Bits_Tbl[5] =  5u;
    CNI_Bits_Tbl[6] =  6u;
    CNI_Bits_Tbl[7] =  7u;

    CNI_Bits_Tbl[8] = 17u;
    CNI_Bits_Tbl[9] = 18u;
    
    p_peri->Type     = PERI_TYPE_CNI;
    p_peri->p_device = Peri_CNI_Init( CNI_Pins_Tbl,
                                     &CNI_Bits_Tbl[0],
                                      CNI_PIN_CNT,
                                      ISR_VECT_NUM_CNIF,
                                      p_sim->p_mem_data,
                                     &peri_err);
    p_peri->p_next   = p_peri_next;
    
    p_peri_next      = p_peri;
    /************ END CNI *************/

    /*************** IO ***************/
    p_peri = (PERIPHERAL *)malloc(sizeof(PERIPHERAL));
    
    IO_Pins_Tbl[0x00] = &p_hw_if->RB0;  IO_Bits_Tbl[0x00].bit  =  0;  IO_Bits_Tbl[0x00].port = PERI_IO_PORTB;
    IO_Pins_Tbl[0x01] = &p_hw_if->RB1;  IO_Bits_Tbl[0x01].bit  =  1;  IO_Bits_Tbl[0x01].port = PERI_IO_PORTB;
    IO_Pins_Tbl[0x02] = &p_hw_if->RB2;  IO_Bits_Tbl[0x02].bit  =  2;  IO_Bits_Tbl[0x02].port = PERI_IO_PORTB;
    IO_Pins_Tbl[0x03] = &p_hw_if->RB3;  IO_Bits_Tbl[0x03].bit  =  3;  IO_Bits_Tbl[0x03].port = PERI_IO_PORTB;
    IO_Pins_Tbl[0x04] = &p_hw_if->RB4;  IO_Bits_Tbl[0x04].bit  =  4;  IO_Bits_Tbl[0x04].port = PERI_IO_PORTB;
    IO_Pins_Tbl[0x05] = &p_hw_if->RB5;  IO_Bits_Tbl[0x05].bit  =  5;  IO_Bits_Tbl[0x05].port = PERI_IO_PORTB;
    IO_Pins_Tbl[0x06] = &p_hw_if->RB6;  IO_Bits_Tbl[0x06].bit  =  6;  IO_Bits_Tbl[0x06].port = PERI_IO_PORTB;
    IO_Pins_Tbl[0x07] = &p_hw_if->RB7;  IO_Bits_Tbl[0x07].bit  =  7;  IO_Bits_Tbl[0x07].port = PERI_IO_PORTB;
    IO_Pins_Tbl[0x08] = &p_hw_if->RB8;  IO_Bits_Tbl[0x08].bit  =  8;  IO_Bits_Tbl[0x08].port = PERI_IO_PORTB;

    IO_Pins_Tbl[0x09] = &p_hw_if->RC13; IO_Bits_Tbl[0x09].bit  = 13;  IO_Bits_Tbl[0x09].port = PERI_IO_PORTC;
    IO_Pins_Tbl[0x0A] = &p_hw_if->RC14; IO_Bits_Tbl[0x0A].bit  = 13;  IO_Bits_Tbl[0x0A].port = PERI_IO_PORTC;
    IO_Pins_Tbl[0x0B] = &p_hw_if->RC15; IO_Bits_Tbl[0x0B].bit  = 13;  IO_Bits_Tbl[0x0B].port = PERI_IO_PORTC;

    IO_Pins_Tbl[0x0C] = &p_hw_if->RD0;  IO_Bits_Tbl[0x0C].bit  =  0;  IO_Bits_Tbl[0x0C].port = PERI_IO_PORTD;
    IO_Pins_Tbl[0x0D] = &p_hw_if->RD1;  IO_Bits_Tbl[0x0D].bit  =  1;  IO_Bits_Tbl[0x0D].port = PERI_IO_PORTD;
    IO_Pins_Tbl[0x0E] = &p_hw_if->RD2;  IO_Bits_Tbl[0x0E].bit  =  2;  IO_Bits_Tbl[0x0E].port = PERI_IO_PORTD;
    IO_Pins_Tbl[0x0F] = &p_hw_if->RD3;  IO_Bits_Tbl[0x0F].bit  =  3;  IO_Bits_Tbl[0x0F].port = PERI_IO_PORTD;

    IO_Pins_Tbl[0x10] = &p_hw_if->RE0;  IO_Bits_Tbl[0x10].bit  =  0;  IO_Bits_Tbl[0x10].port = PERI_IO_PORTE;
    IO_Pins_Tbl[0x11] = &p_hw_if->RE1;  IO_Bits_Tbl[0x11].bit  =  1;  IO_Bits_Tbl[0x11].port = PERI_IO_PORTE;
    IO_Pins_Tbl[0x12] = &p_hw_if->RE2;  IO_Bits_Tbl[0x12].bit  =  2;  IO_Bits_Tbl[0x12].port = PERI_IO_PORTE;
    IO_Pins_Tbl[0x13] = &p_hw_if->RE3;  IO_Bits_Tbl[0x13].bit  =  3;  IO_Bits_Tbl[0x13].port = PERI_IO_PORTE;
    IO_Pins_Tbl[0x14] = &p_hw_if->RE4;  IO_Bits_Tbl[0x14].bit  =  4;  IO_Bits_Tbl[0x14].port = PERI_IO_PORTE;
    IO_Pins_Tbl[0x15] = &p_hw_if->RE5;  IO_Bits_Tbl[0x15].bit  =  5;  IO_Bits_Tbl[0x15].port = PERI_IO_PORTE;
    IO_Pins_Tbl[0x16] = &p_hw_if->RE8;  IO_Bits_Tbl[0x16].bit  =  8;  IO_Bits_Tbl[0x16].port = PERI_IO_PORTE;

    IO_Pins_Tbl[0x17] = &p_hw_if->RF0;  IO_Bits_Tbl[0x17].bit  =  0;  IO_Bits_Tbl[0x17].port = PERI_IO_PORTF;
    IO_Pins_Tbl[0x18] = &p_hw_if->RF1;  IO_Bits_Tbl[0x18].bit  =  1;  IO_Bits_Tbl[0x18].port = PERI_IO_PORTF;
    IO_Pins_Tbl[0x19] = &p_hw_if->RF2;  IO_Bits_Tbl[0x19].bit  =  2;  IO_Bits_Tbl[0x19].port = PERI_IO_PORTF;
    IO_Pins_Tbl[0x1A] = &p_hw_if->RF3;  IO_Bits_Tbl[0x1A].bit  =  3;  IO_Bits_Tbl[0x1A].port = PERI_IO_PORTF;
    IO_Pins_Tbl[0x1B] = &p_hw_if->RF4;  IO_Bits_Tbl[0x1B].bit  =  4;  IO_Bits_Tbl[0x1B].port = PERI_IO_PORTF;
    IO_Pins_Tbl[0x1C] = &p_hw_if->RF5;  IO_Bits_Tbl[0x1C].bit  =  5;  IO_Bits_Tbl[0x1C].port = PERI_IO_PORTF;
    IO_Pins_Tbl[0x1D] = &p_hw_if->RF6;  IO_Bits_Tbl[0x1D].bit  =  6;  IO_Bits_Tbl[0x1D].port = PERI_IO_PORTF;

    
    
    p_peri->Type     = PERI_TYPE_IO;
    p_peri->p_device = Peri_IO_Init( IO_Pins_Tbl,
                                    &IO_Bits_Tbl[0],
                                     PERI_IO_CNT,
                                     p_sim->p_mem_data,
                                    &peri_err);
    p_peri->p_next   = p_peri_next;
    
    p_peri_next      = p_peri;
    /************* END IO *************/
    
    /************** ISR ***************/
    p_peri = (PERIPHERAL *)malloc(sizeof(PERIPHERAL));
    
    
    p_peri->Type     = PERI_TYPE_ISR;
    p_peri->p_device = Peri_ISR_Init(p_sim->p_mem_data, &peri_err);
    p_peri->p_next   = p_peri_next;
    
    p_peri_next      = p_peri;
    /************ END ADC *************/
    
    
    p_sim->p_periph_head = p_peri_next;
    
    
    /**********************************/
    
    CallStackHead = (CALL_STACK_ENTRY *)malloc(sizeof(CALL_STACK_ENTRY));

    if (CallStackHead == NULL) {
        return;
    }
    
    CallStackHead->addr  = 0x000000;
    CallStackHead->depth = 0;
    CallStackHead->next  = NULL;
}

void Sim_Run()
{
    while (1) {
        Sim_Step();
    }
}

void Sim_Step()
{
    SIM        *p_sim;
    CORE_DATA  *p_core_data;

    p_sim       = &sim_struct;
    p_core_data = p_sim->p_core_data;
    
    p_core_data->cycles++;
    
    UT_Testting(p_sim);
    
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
    
    if (core_static_err != CORE_ERR_NONE) {
        printf("\r\nCore Error no %d",core_static_err);
        printf(" ");
    }
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

void Sim_LinkCall(CPU_INT32U  PC)
{
    CALL_STACK_ENTRY  *p_new_entry;
    

    p_new_entry = (CALL_STACK_ENTRY *)malloc(sizeof(CALL_STACK_ENTRY));
    
    if (p_new_entry == NULL) {
        return;
    }
    
    p_new_entry->addr  = PC;
    p_new_entry->depth = CallStackHead->depth + 1;
    p_new_entry->next  = CallStackHead;
    
    CallStackHead = p_new_entry;
}

void Sim_UnlinkCall()
{
    CALL_STACK_ENTRY  *p_old_entry;
    
    
    p_old_entry   = CallStackHead;
    CallStackHead = CallStackHead->next;
    
    free(p_old_entry);
}

CPU_INT32U Sim_GetCallDepth (void)
{
    return Call_Depth + 1;
}

CPU_INT32U Sim_AddrForDepth(CPU_INT32U  depth)
{
    CPU_INT32U         ix;
    CPU_INT32U         levels;
    CALL_STACK_ENTRY  *p_stack_entry;
    CORE_24F          *p_core;
    
    p_core = sim_struct.p_core;
    
    if (depth == Call_Depth) {
        return Core_PC_Get(p_core);
    } else {
        p_stack_entry = CallStackHead;
        levels        = Call_Depth - depth - 1;
        
        for (ix = 0 ; ix < levels ; ix++) {
            p_stack_entry = p_stack_entry->next;
        }
        
        if (p_stack_entry->depth != (depth +1)) {
            printf("Error Depth mismatch");
        }
        
        return (p_stack_entry->addr);
    }
}









