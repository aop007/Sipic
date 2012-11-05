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
#include "sim.h"

#if    (SIPIC_CFG_PARSE_METHOD == SIPIC_PARSE_METHOD_COFF)
#include <coff_parser.h>
#include <coff_cfg.h>
#elif  (SIPIC_CFG_PARSE_METHOD == SIPIC_PARSE_METHOD_HEX)
#include <hex_parser.h>
#include <hex_cfg.h>
#endif
    
#ifdef   __DLL__
#define  DLL_API __declspec(dllexport)
#else
#define  DLL_API    
#endif
    
SIM  sim_struct;
PERI_ERR    peri_static_err;
CORE_ERR    core_static_err;

int  main (int argc, const char * argv[]);
void DLL_API init (void);
void DLL_API run  (void);

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
}
#endif

#endif