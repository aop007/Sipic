#ifndef  __MAIN_H__
#define  __MAIN_H__

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

#include <stdio.h>
#include "sipic_cfg.h"
#include "mem.h"
#include "core_24f.h"
#include "core_24f_opcode.h"
#include "mem_cfg_24f3011.h"
#include "unit_testing.h"
#include "sim.h"

#if    (SIPIC_CFG_PARSE_METHOD == SIPIC_PARSE_METHOD_COFF)
#include <coff_parser.h>
#include <coff_cfg.h>
#elif  (SIPIC_CFG_PARSE_METHOD == SIPIC_PARSE_METHOD_HEX)
#include "hex_parser.h"
#include "hex_cfg.h"
#endif

#define CORE_TRACE_LEVEL 1

#if (CORE_TRACE_LEVEL > 0)
#define CORE_TRACE_INFO(x) printf x
#else
#define CORE_TRACE_INFO(x) 
#endif

#if (CORE_TRACE_LEVEL > 1)
#define CORE_TRACE_DEBUG(x) printf x
#else
#define CORE_TRACE_DEBUG(x) 
#endif
    
#define  CORE_INTEGRITY_CHECK   DEF_DISABLED

SIM         sim_struct;
CORE_DATA   core_data;
ISR        *p_isr_static;
HW_ERR      hw_static_err;
PERI_ERR    peri_static_err;
CORE_ERR    core_static_err;
    
CPU_BOOLEAN    EnableDebugPrintf;

#if 0
int  main (int argc, const char * argv[]);
#endif
    
#define  SIPIC_UNIT_TESTING    DEF_DISABLED
#define  WRITE_REPORT_EN       DEF_DISABLED

//void DLL_API C_STD_CALL init (void);
void DLL_API C_STD_CALL init_sipic(void);
void DLL_API C_STD_CALL run  (void);

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
}
#endif

#endif