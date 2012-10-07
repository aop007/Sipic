//
//  core_24f_opcode.h
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-06.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#ifndef Sipic_core_24f_opcode_h
#define Sipic_core_24f_opcode_h

#include "core_24f.h"
#include "mem.h"

#define  CORE_OPC_NOP     0x000000
#define  CORE_OPC_BRA     0x010000
#define  CORE_OPC_CALL    0x020000
//                        0x030000
#define  CORE_OPC_GOTO    0x040000
#define  CORE_OPC_RETLW   0x050000
#define  CORE_OPC_RETFIE  0x060000
#define  CORE_OPC_RCALL   0x070000
#define  CORE_OPC_DO      0x080000
#define  CORE_OPC_REPEAT  0x090000
//                        0x0A0000
//                        0x0B0000
#define  CORE_OPC_BRA_OA  0x0C0000
#define  CORE_OPC_BRA_OB  0x0D0000
#define  CORE_OPC_BRA_SA  0x0E0000
#define  CORE_OPC_BRA_SB  0x0F0000

#define  CORE_OPC_MOV_L_W 0x200000

#define  CORE_OPC_MOV_M_W 0x800000
#define  CORE_OPC_MOV_W_M 0x880000


void Core_OPC_Stats(MEM * mem);

void Core_GOTO_04(MEM         *p_mem,
                  CORE_24F    *p_core,
                  CPU_INT32U   args,
                  CORE_ERR    *p_err);

void Core_MOV_2  (CORE_24F    *p_core,
                  CPU_INT32U   args,
                  CORE_ERR    *p_err);

void Core_MOV_80 (MEM         *p_mem,
                  CORE_24F    *p_core,
                  CPU_INT32U   args,
                  CORE_ERR    *p_err);

void Core_MOV_88 (MEM         *p_mem,
                  CORE_24F    *p_core,
                  CPU_INT32U   args,
                  CORE_ERR    *p_err);

#endif
