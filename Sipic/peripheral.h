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
typedef  CPU_INT08U  IST_TRAP_NUM;

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
    
#define  ISR_VECT_BASE              0x14

#define  ISR_TRAP_NUM_OSCI          DEF_BIT_01
#define  ISR_TRAP_NUM_STAK          DEF_BIT_02
#define  ISR_TRAP_NUM_ADDR          DEF_BIT_03
#define  ISR_TRAP_NUM_MATH          DEF_BIT_04
    
#define  ISR_INTCON1_NSTDIS         DEF_BIT_15
#define  ISR_INTCON1_OVATE          DEF_BIT_10
#define  ISR_INTCON1_OVBTE          DEF_BIT_09
#define  ISR_INTCON1_COVTE          DEF_BIT_08
#define  ISR_INTCON1_MATHERR        ISR_TRAP_NUM_MATH
#define  ISR_INTCON1_ADDRERR        ISR_TRAP_NUM_ADDR
#define  ISR_INTCON1_STKERR         ISR_TRAP_NUM_STAK
#define  ISR_INTCON1_OSCFAIL        ISR_TRAP_NUM_OSCI
    
#define  ISR_INTCON2_ALTIVT         DEF_BIT_15
#define  ISR_INTCON2_DISI           DEF_BIT_14
#define  ISR_INTCON2_INT2EP         DEF_BIT_02
#define  ISR_INTCON2_INT1EP         DEF_BIT_01
#define  ISR_INTCON2_INT0EP         DEF_BIT_00

#define  ISR_TRAP_IPL               DEF_BIT_03
    
#define  ISR_TRAP_MASK             (ISR_TRAP_NUM_MATH | ISR_TRAP_NUM_STAK | ISR_TRAP_NUM_ADDR | ISR_TRAP_NUM_OSCI)
    
#define  ISR_VECT_NUM_INT0              0u
#define  ISR_VECT_NUM_IC1               1u
#define  ISR_VECT_NUM_OC1               2u
#define  ISR_VECT_NUM_T1                3u
#define  ISR_VECT_NUM_IC2               4u
#define  ISR_VECT_NUM_OC2               5u
#define  ISR_VECT_NUM_T2                6u
#define  ISR_VECT_NUM_T3                7u
#define  ISR_VECT_NUM_SPI1              8u
#define  ISR_VECT_NUM_U1RX              9u
#define  ISR_VECT_NUM_U1TX             10u
#define  ISR_VECT_NUM_ADC              11u
#define  ISR_VECT_NUM_NVM              12u
#define  ISR_VECT_NUM_SI2C             13u
#define  ISR_VECT_NUM_MI2C             14u
#define  ISR_VECT_NUM_CNIF             15u
#define  ISR_VECT_NUM_INT1             16u
#define  ISR_VECT_NUM_IC7              17u
#define  ISR_VECT_NUM_IC8              18u
#define  ISR_VECT_NUM_OC3              19u
#define  ISR_VECT_NUM_OC4              20u
#define  ISR_VECT_NUM_T4               21u
#define  ISR_VECT_NUM_T5               22u
#define  ISR_VECT_NUM_INT2             23u
#define  ISR_VECT_NUM_U2RX             24u
#define  ISR_VECT_NUM_U2TX             25u
    
#define  ISR_VECT_NUM_PWM              39u
#define  ISR_VECT_NUM_QEI              40u

#define  ISR_VECT_NUM_FLTA             43u
    
#define  ISR_VECT_SIZE                 54u

static const CPU_INT08U ISR_VECT_TBL[] = {
    ISR_VECT_NUM_INT0,
    ISR_VECT_NUM_IC1,
    ISR_VECT_NUM_OC1,
    ISR_VECT_NUM_T1,
    ISR_VECT_NUM_IC2,
    ISR_VECT_NUM_OC2,
    ISR_VECT_NUM_T2,
    ISR_VECT_NUM_T3,
    ISR_VECT_NUM_SPI1,
    ISR_VECT_NUM_U1RX,
    ISR_VECT_NUM_U1TX,
    ISR_VECT_NUM_ADC,
    ISR_VECT_NUM_NVM,
    ISR_VECT_NUM_SI2C,
    ISR_VECT_NUM_MI2C,
    ISR_VECT_NUM_IC,
    ISR_VECT_NUM_INT1,
    ISR_VECT_NUM_IC7,
    ISR_VECT_NUM_IC8,
    ISR_VECT_NUM_OC3,
    ISR_VECT_NUM_OC4,
    ISR_VECT_NUM_T4,
    ISR_VECT_NUM_T5,
    ISR_VECT_NUM_INT2,
    ISR_VECT_NUM_U2RX,
    ISR_VECT_NUM_U2TX,
    ISR_VECT_NUM_PWM,
    ISR_VECT_NUM_QEI,
    ISR_VECT_NUM_FLTA};
    
#define  PERI_ISR_BASE_ADDR         0x0080
#define  PERI_TYPE_ISR              CPU_MAKE_TYPE('I','S','R',' ')
    
#define  ISR_INTCON1                0x00
#define  ISR_INTCON2                0x01
    
typedef struct isr_mem
{
    CPU_INT16U  INTCON[2];
    CPU_INT16U  IFS[3];
    CPU_INT16U  Reserved0;
    CPU_INT16U  IEC[3];
    CPU_INT16U  Reserved1;
    CPU_INT16U  IPC[12];
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
    
    void Peri_ISR(MEM_24      *p_mem_prog,
                  MEM         *p_mem_data,
                  CORE_24F    *p_core,
                  ISR         *p_isr,
                  PERI_ERR    *p_err);
    
void Peri_ISR_FromVect(CPU_INT32U    isr_vect_addr,
                       CPU_INT08U    ipl,
                       CPU_INT08U    ipl_old,
                       MEM_24       *p_mem_prog,
                       MEM          *p_mem_data,
                       CORE_24F     *p_core,
                       PERI_ERR     *p_err);

void ISR_Post(ISR_VECT_NUM   vect_num,
                  PERI_ERR      *p_err);
    
void Trap_Post(IST_TRAP_NUM   trap_num,
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
    CPU_INT16U  BUF[16];
    
    CPU_INT16U  CON1;
    CPU_INT16U  CON2;
    CPU_INT16U  CON3;
    
    CPU_INT16U  HS;
    CPU_INT16U  CFG;
    CPU_INT16U  SSL;
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
#define  PERI_TYPE_TMR_BC             CPU_MAKE_TYPE('T','M','B','C')

#define  TMR_1_ADDR_TBL_IX            0
#define  TMR_2_ADDR_TBL_IX            1
#define  TMR_4_ADDR_TBL_IX            2

#define  TMR_CON_TON                  DEF_BIT_15
#define  TMR_CON_TSIDL                DEF_BIT_13
#define  TMR_CON_TGATE                DEF_BIT_06
#define  TMR_CON_TCKPS               (DEF_BIT_05 | DEF_BIT_04)
#define  TMR_CON_T32                  DEF_BIT_03
#define  TMR_CON_TSYNC                DEF_BIT_02
#define  TMR_CON_TCS                  DEF_BIT_01

#define  TMR_EXT_STATE_LO               0u
#define  TMR_EXT_STATE_HI               1u

#define  TMR_EXT_HI_TO_LO_TRSH             HW_SCHMITT_LO_TRIG
#define  TMR_EXT_LO_TO_HI_TRSH             HW_SCHMITT_HI_TRIG


static const CPU_INT16U PERI_TMR_BASE_ADDR[] = {0x0100, 0x0106, 0x0114};

static const CPU_INT16U PERI_TMR_TCKPS[]     = {1, 8, 64, 256};

typedef struct tmr_data {
    ISR_VECT_NUM      isr_vect_num;
    ISR_VECT_NUM      isr_vect_num_sec;
    HW_IF_DATA_TYPE  *p_pin;
    CPU_BOOLEAN       ext_input_state;
    CPU_INT32U        inputTick;
    CPU_INT32U        inputTick_sec;
} TMR_DATA;

typedef struct tmr_a_mem {
    CPU_INT16U  TMR;
    CPU_INT16U  PR;
    CPU_INT16U  CON;
} TMR_A_MEM;

typedef struct tmr_a {
    TMR_A_MEM  *p_mem;
    TMR_DATA   *p_data;
} TMR_A;


TMR_A *Peri_TmrA_Init(CPU_INT32U        tmr_nbr,
                          HW_IF_DATA_TYPE  *p_pin,
                          ISR_VECT_NUM      isr_vect_num,
                          MEM              *p_mem_data,
                          PERI_ERR         *p_err);

void Peri_TMR_A(MEM_24       *p_mem_prog,
                MEM          *p_mem_data,
                CORE_24F     *p_core,
                TMR_A        *p_tmr,
                PERI_ERR     *p_err);

typedef struct tmr_bc_mem {
    CPU_INT16U  TMR2;
    CPU_INT16U  TMR3HLD;
    CPU_INT16U  TMR3;
    CPU_INT16U  PR2;
    CPU_INT16U  PR3;
    CPU_INT16U  CON2;
    CPU_INT16U  CON3;
} TMR_BC_MEM;

typedef struct tmr_b_mem {
    CPU_INT16U  TMR;
    CPU_INT16U  Reserved0[2];
    CPU_INT16U  PR;
    CPU_INT16U  Reserved1;
    CPU_INT16U  CON;
    CPU_INT16U  Reserved2;
} TMR_B_MEM;

typedef struct tmr_c_mem {
    CPU_INT16U  TMRHLD;
    CPU_INT16U  TMR;
    CPU_INT16U  Reserved0;
    CPU_INT16U  PR;
    CPU_INT16U  Reserved1;
    CPU_INT16U  CON;
} TMR_C_MEM;

typedef struct tmr_bc {
    TMR_BC_MEM  *p_mem;
    TMR_DATA    *p_data;
} TMR_BC;

typedef struct tmr_b {
    TMR_B_MEM   *p_mem;
    TMR_DATA    *p_data;
} TMR_B;

typedef struct tmr_c {
    TMR_C_MEM   *p_mem;
    TMR_DATA    *p_data;
} TMR_C;

TMR_BC *Peri_TmrBC_Init(CPU_INT32U        tmr_nbr,
                        HW_IF_DATA_TYPE  *p_pin,
                        ISR_VECT_NUM      isr_vect_num_b,
                        ISR_VECT_NUM      isr_vect_num_c,
                        MEM              *p_mem_data,
                        PERI_ERR         *p_err);

void Peri_TMR_BC(MEM_24       *p_mem_prog,
                     MEM          *p_mem_data,
                     CORE_24F     *p_core,
                     TMR_BC       *p_tmr,
                     PERI_ERR     *p_err);
    
void Peri_TMR_B(MEM_24       *p_mem_prog,
                MEM          *p_mem_data,
                CORE_24F     *p_core,
                TMR_B        *p_tmr,
                PERI_ERR     *p_err);

void Peri_TMR_C(MEM_24       *p_mem_prog,
                MEM          *p_mem_data,
                CORE_24F     *p_core,
                TMR_C        *p_tmr,
                PERI_ERR     *p_err);
                

/* Change Notification Peripheral */

#define  PERI_CNI_BASE_ADDR  0x00C0
#define  CNI_PIN_CNT             10u
#define  PERI_TYPE_CNI       CPU_MAKE_TYPE('C','N','I',' ')

typedef struct cni_mem {
    CPU_INT16U  CNEN[2];
    CPU_INT16U  CNPU[2];
} CNI_MEM;

typedef struct cni_data {
    ISR_VECT_NUM      isr_num;
    CPU_INT32U        prevous state;
    HW_IF_DATA_TYPE  *pin_tbl[32];
} CNI_DATA;

typedef struct cni {
    CNI_MEM     *p_mem;
    CNI_DATA    *p_data;
} CNI;

CNI *Peri_CNI_Init(HW_IF_DATA_TYPE **p_pin_tbl,
                   CPU_INT16U        pin_cnt,
                   ISR_VECT_NUM      isr_vect_num,
                   MEM              *p_mem_data,
                   PERI_ERR         *p_err);

/* Core Stuff */

//CPU_INT32U  CallDepth;
    
#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
}
#endif

#endif
