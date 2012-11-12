//
//  peripheral.h
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-25.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#ifndef Sipic_peripheral_h
#define Sipic_peripheral_h

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

#include "hardware.h"
#include "mem.h"
#include "core_24f.h"
#include "err.h"



struct peripheral;

typedef struct peripheral {
    struct peripheral  *p_next;
           void        *p_device;
           CPU_INT32U   Type;
} PERIPHERAL;

typedef  CPU_INT08U  ISR_VECT_NUM;

void Peripheral_Run(const  CORE_ERR    *p_core_err,
                           MEM_24      *p_mem_prog,
                           MEM         *p_mem_data,
                           CORE_24F    *p_core,
                           PERIPHERAL  *p_peri_list,
                           PERI_ERR    *p_err);

/* ISR */
#define  ISR_VECT_NUM_RESET0        0x00
#define  ISR_VECT_NUM_RESET1        0x02
#define  ISR_VECT_NUM_RESV0         0x04
#define  ISR_VECT_NUM_OSCI_TRAP     0x06
#define  ISR_VECT_NUM_ADDR_TRAP     0x08
#define  ISR_VECT_NUM_STAK_TRAP     0x0A
#define  ISR_VECT_NUM_MATH_TRAP     0x0C
#define  ISR_VECT_NUM_RESV1         0x0E
#define  ISR_VECT_NUM_RESV2         0x10
#define  ISR_VECT_NUM_RESV3         0x12

#define  PERI_ISR_BASE_ADDR         0x0080
#define  PERI_TYPE_ISR              CPU_MAKE_TYPE('I','S','R',' ')
    
#define  ISR_INTCON1                0x00
#define  ISR_INTCON2                0x01
    
typedef struct isr_mem
{
    volatile  CPU_INT16U  INTCON[2];
    volatile  CPU_INT16U  IFS[3];
    volatile  CPU_INT16U  Reserved0;
    volatile  CPU_INT16U  IEC[2];
    volatile  CPU_INT16U  Reserved1;
    volatile  CPU_INT16U  IPC[12];
} ISR_MEM;
    
typedef  struct  isr_data {
    CPU_INT32U  data;
} ISR_DATA;
    
typedef struct isr {
    ISR_MEM   *p_mem;
    ISR_DATA  *p_data;
} ISR;

ISR *Peri_ISR_Init(MEM       *p_mem_data,
                   PERI_ERR  *p_err);
    
void Peri_ISR(ISR_VECT_NUM   isr_vect_num,
              MEM_24        *p_mem_prog,
              MEM           *p_mem_data,
              CORE_24F      *p_core,
              PERI_ERR      *p_err);
    
/* ADC */
#define  ADC_ADCON1_ADON        DEF_BIT_15
#define  ADC_ADCON1_ADSIDL      DEF_BIT_13
#define  ADC_ADCON1_FORM       (DEF_BIT_09 | DEF_BIT_08)
#define  ADC_ADCON1_SSRC       (DEF_BIT_07 | DEF_BIT_06 | DEF_BIT_05)
#define  ADC_ADCON1_SIMSAM      DEF_BIT_03
#define  ADC_ADCON1_ASAM        DEF_BIT_02
#define  ADC_ADCON1_SAMP        DEF_BIT_01
#define  ADC_ADCON1_DONE        DEF_BIT_00
    
#define  ADC_ADCON3_SAMC       (DEF_BIT_12 | DEF_BIT_11 | DEF_BIT_10 | DEF_BIT_09 | DEF_BIT_08)
#define  ADC_ADCON3_ADRC        DEF_BIT_07
#define  ADC_ADCON3_ADCS       (DEF_BIT_05 | DEF_BIT_04 | DEF_BIT_03 | DEF_BIT_02 | DEF_BIT_01 | DEF_BIT_00)
    
#define  ADC_STATUS_NONE            DEF_BIT_NONE
#define  ADC_STATUS_SAMPLING        DEF_BIT_01
#define  ADC_STATUS_CONVERTING      DEF_BIT_02
#define  ADC_STATUS_DONE            DEF_BIT_03
    
    
#define  PERI_ADC_BASE_ADDR         0x0280
#define  PERI_TYPE_ADC              CPU_MAKE_TYPE('A','D','C',' ')
    
typedef struct adc_mem {
    volatile  CPU_INT16U  BUF[16];
    
    volatile  CPU_INT16U  CON1;
    volatile  CPU_INT16U  CON2;
    volatile  CPU_INT16U  CON3;
    
    volatile  CPU_INT16U  HS;
    volatile  CPU_INT16U  CFG;
    volatile  CPU_INT16U  SSL;
} ADC_MEM;
    
typedef  struct  adc_data {
    CPU_INT32U  SampleClk;
    CPU_INT32U  Status;
    
    CPU_INT16U  SampTime;
    CPU_INT16U  ConvTime;
    
    CPU_INT16U  ADCON1_Shadow;
} ADC_DATA;

typedef struct adc {
    ADC_MEM   *p_mem;
    ADC_DATA  *p_data;
} ADC;
    
ADC *Peri_ADC_Init(MEM       *p_mem_data,
                   PERI_ERR  *p_err);
    
void Peri_ADC(MEM_24       *p_mem_prog,
              MEM          *p_mem_data,
              CORE_24F     *p_core,
              ADC          *p_adc,
              PERI_ERR     *p_err);

/* Tmr */
#define  PERI_TYPE_TMR_A              CPU_MAKE_TYPE('T','M','R','A')

#define  TMR_1_ADDR_TBL_IX            0
#define  TMR_2_ADDR_TBL_IX            1
#define  TMR_4_ADDR_TBL_IX            2

#define  TMR_CON_TON                  DEF_BIT_15
#define  TMR_CON_TSIDL                DEF_BIT_13
#define  TMR_CON_TGATE                DEF_BIT_06
#define  TMR_CON_TCKPS               (DEF_BIT_05 | DEF_BIT_04)
#define  TMR_CON_TSYNC                DEF_BIT_02
#define  TMR_CON_TCS                  DEF_BIT_01

#define  TMR_EXT_STATE_LO               0u
#define  TMR_EXT_STATE_HI               1u

#define  TMR_EXT_HI_TO_LO_TRSH             1.60
#define  TMR_EXT_LO_TO_HI_TRSH             1.70


static const CPU_INT16U PERI_TMR_BASE_ADDR[] = {0x0100, 0x0106, 0x0114};

static const CPU_INT16U PERI_TMR_TCKPS[]     = {1, 8, 64, 256};

typedef struct tmr_data {
    HW_IF_DATA_TYPE  *p_pin;
    CPU_BOOLEAN       ext_input_state;
    CPU_INT32U        inputTick;
} TMR_DATA;

typedef struct tmr_a_mem {
    volatile  CPU_INT16U  TMR;
    volatile  CPU_INT16U  PR;
    volatile  CPU_INT16U  CON;
} TMR_A_MEM;

typedef struct tmr_a {
    TMR_A_MEM  *p_mem;
    TMR_DATA   *p_data;
} TMR_A;

TMR_A *Peri_TmrA_Init(CPU_INT32U        tmr_nbr,
                      HW_IF_DATA_TYPE  *p_pin,
                      MEM              *p_mem_data,
                      PERI_ERR         *p_err);

void Peri_TMR_A(MEM_24       *p_mem_prog,
                MEM          *p_mem_data,
                CORE_24F     *p_core,
                TMR_A        *p_tmr,
                PERI_ERR     *p_err);

/* Core Stuff */

CPU_INT32U  CallDepth;
    
void Peripheral_ISR(const  CORE_ERR    *p_core_err,
                           MEM_24      *p_mem_prog,
                           MEM         *p_mem_data,
                           CORE_24F    *p_core,
                           PERI_ERR    *p_err);
    
#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
}
#endif

#endif
