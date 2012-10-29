//
//  sipic_cfg.h
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-05.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#ifndef Sipic_sipic_cfg_h
#define Sipic_sipic_cfg_h

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

#define  SIPIC_PARSE_METHOD_COFF  1u
#define  SIPIC_PARSE_METHOD_HEX   2u

#define  SIPIC_CFG_PARSE_METHOD   SIPIC_PARSE_METHOD_HEX

#define  SIPIC_CFG_MEM_SIZE_OCTET  (64 * 1024)

#define  SIPIC_CFG_PC_START        0u

#define  CORE_CFG_CYCLE_CNTR

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
}
#endif

#endif
