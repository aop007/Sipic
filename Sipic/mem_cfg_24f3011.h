//
//  mem_cfg_24f3011.h
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-09.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#ifndef Sipic_mem_cfg_24f3011_h
#define Sipic_mem_cfg_24f3011_h

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

#include "mem.h"

static const MEM_CFG mem_cfg_prog_dsPIC30F[7] = {
    /* See http://ww1.microchip.com/downloads/en/DeviceDoc/70157C.pdf Figure 6-3 */
    
    {"Main Vector Table",  0x000000, 0x00007E},
    {"Alt. Vector Table",  0x000080, 0x0000FE},
    {"User Flash Program", 0x000100, 0x017FFE},
    {"Data Flash",         0x7FF000, 0x7FFFFE},
    
    {"UNITID",             0x8005C0, 0x8005FE},
    {"Fuse Cfg Registers", 0xF80000, 0xF8000E},
    {"DEVID",              0xFF0000, 0xFFFFFE}
};

static const MEM_CFG mem_cfg_data_dsPIC30F[3] = {
    {"SFR Space",               0x0000,   0x07FE},
    {"X Data RAM",              0x0800,   0x09FE},
    {"Y Data RAM",              0x0A00,   0x0BFE}
};

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
}
#endif

#endif
