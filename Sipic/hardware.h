//
//  hardware.h
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-25.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#ifndef Sipic_hardware_h
#define Sipic_hardware_h

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

#include "cpu.h"
#include "mem.h"
    
#define  MAIN_PLL_FREQ  118662200
#define  FCY            (MAIN_PLL_FREQ / 4)
#define  HW_VDD_VOLT               3.3
#define  HW_VSS_VOLT               0.0

#define  HW_SCHMITT_LO_TRIG       (0.2 * HW_VDD_VOLT)
#define  HW_SCHMITT_HI_TRIG       (0.8 * HW_VDD_VOLT)

struct hw_mod;
    
typedef  struct  hw_mod {
    struct hw_mod  *p_next;
    void           *p_device;
    CPU_INT32U      Type;
} HW_MOD;

    /* HW_IF */
#define  HW_TYPE_IF   CPU_MAKE_TYPE('I','F',' ',' ')
    
    typedef  float  HW_IF_DATA_TYPE;
    
    typedef  struct  hw_if {
        HW_IF_DATA_TYPE RF2;
        HW_IF_DATA_TYPE RF5;
        HW_IF_DATA_TYPE RF4;
        HW_IF_DATA_TYPE RF1;
        HW_IF_DATA_TYPE RF0;
        
        HW_IF_DATA_TYPE VSS0;
        HW_IF_DATA_TYPE VDD0;
        
        HW_IF_DATA_TYPE RE5;
        HW_IF_DATA_TYPE RE4;
        HW_IF_DATA_TYPE RE3;
        HW_IF_DATA_TYPE RE2;
        HW_IF_DATA_TYPE RE1;
        HW_IF_DATA_TYPE RE0;
        
        HW_IF_DATA_TYPE AVSS;
        HW_IF_DATA_TYPE AVDD;
        
        HW_IF_DATA_TYPE MCLR;
        
        HW_IF_DATA_TYPE RB0;
        HW_IF_DATA_TYPE RB1;
        HW_IF_DATA_TYPE RB2;
        HW_IF_DATA_TYPE RB3;
        HW_IF_DATA_TYPE RB4;
        HW_IF_DATA_TYPE RB5;
        HW_IF_DATA_TYPE RB6;
        HW_IF_DATA_TYPE RB7;
        HW_IF_DATA_TYPE RB8;
        
        HW_IF_DATA_TYPE VDD1;
        HW_IF_DATA_TYPE VSS1;
        
        HW_IF_DATA_TYPE CLKI;
        
        HW_IF_DATA_TYPE RC15;
        HW_IF_DATA_TYPE RC13;
        
        HW_IF_DATA_TYPE RC14;
        HW_IF_DATA_TYPE RE8;
        HW_IF_DATA_TYPE RD1;
        HW_IF_DATA_TYPE RD3;
        
        HW_IF_DATA_TYPE VSS2;
        HW_IF_DATA_TYPE VDD2;
        
        HW_IF_DATA_TYPE RD2;
        HW_IF_DATA_TYPE RD0;
        HW_IF_DATA_TYPE RF6;
        HW_IF_DATA_TYPE RF3;
    } HW_IF;
    
    HW_IF *HW_IF_Init(HW_ERR  *p_err);

    
/* OSC */
    
#define  WH_TYPE_OSC    CPU_MAKE_TYPE('O','S','C',' ')

#define  RTCC_OSC_PERIOD  (FCY / 32768)
#define  RTCC_OSC_AMP     (3.3 / 2)
#define  RTCC_OSC_OFFSET  RTCC_OSC_AMP

#define  LINE_120V_OSC_PERIOD  (FCY / 60)
#define  LINE_120V_OSC_AMP     (120)
#define  LINE_120V_OSC_OFFSET   0
    
typedef struct osc_data {
    CPU_INT32U        period;
    CPU_INT32U        phase;
    float             amplitude;
    float             offset;
    HW_IF_DATA_TYPE  *p_pin;
} OSC_DATA;
    
    
typedef struct hw_osc {
    OSC_DATA  *p_data;
} HW_OSC;

HW_OSC *HW_OSC_Init(CPU_INT32U        period,
                    float             amp,
                    float             offset,
                    HW_IF_DATA_TYPE  *p_pin,
                    HW_ERR           *p_hw_err);

void HW_OSC_Run(HW_OSC    *p_hw_osc,
                HW_ERR      *p_err);
    


/* HW CORE */
void HW_Run(MEM         *p_mem_data,
            HW_MOD      *p_hw_list,
            HW_ERR      *p_err);
    
#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
}
#endif

#endif
