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

#define  CORE_OPC_NOP        0x000000
#define  CORE_OPC_BRA        0x010000
#define  CORE_OPC_CALL       0x020000
//                           0x030000
#define  CORE_OPC_GOTO       0x040000
#define  CORE_OPC_RETLW      0x050000
#define  CORE_OPC_RETURN     0x060000
#define  CORE_OPC_RCALL      0x070000
#define  CORE_OPC_DO         0x080000
#define  CORE_OPC_REPEAT     0x090000
//                           0x0A0000
//                           0x0B0000
#define  CORE_OPC_BRA_OA     0x0C0000
#define  CORE_OPC_BRA_OB     0x0D0000
#define  CORE_OPC_BRA_SA     0x0E0000
#define  CORE_OPC_BRA_SB     0x0F0000

#define  CORE_OPC_BRA_EXPR   0x370000

#define  CORE_OPC_BSET_W     0xA00000
#define  CORE_OPC_BSET_M     0xA80000
#define  CORE_OPC_BCLR_M     0xA90000

#define  CORE_OPC_BTSC       0xAF0000

#define  CORE_OPC_MOV_8BL_WN 0xB3C000
#define  CORE_OPC_MOV_WN_M   0xB7A000

#define  CORE_OPC_MOV_L_W    0x200000

#define  CORE_OPC_MOV_WS_WD  0x780000
#define  CORE_OPC_MOV_M_W    0x800000
#define  CORE_OPC_MOV_W_M    0x880000


#define  CORE_OPC_SETM_WS    0xEB8000
#define  CORE_OPC_CLR_M_W0   0xEF0000
#define  CORE_OPC_SETM_M_W0  0xEF8000

#define  CORE_OPC_PUSH_F8    0xF80000
#define  CORE_OPC_POP_F9     0xF90000

#define  CORE_OPC_ADDR_MODE_DIR          0x00
#define  CORE_OPC_ADDR_MODE_IND          0x01
#define  CORE_OPC_ADDR_MODE_IND_POS_DEC  0x02
#define  CORE_OPC_ADDR_MODE_IND_POS_INC  0x03
#define  CORE_OPC_ADDR_MODE_IND_PRE_DEC  0x04
#define  CORE_OPC_ADDR_MODE_IND_PRE_INC  0x05
#define  CORE_OPC_ADDR_MODE_OFFSET       0x06


void Core_OPC_Stats           (MEM         *p_mem);

void  Core_NOP_00             (MEM         *p_mem,
                               CORE_24F    *p_core,
                               CPU_INT32U   args,
                               CORE_ERR    *p_err);

void  Core_CALL_02            (MEM         *p_mem,
                               CORE_24F    *p_core,
                               CPU_INT32U   args,
                               CORE_ERR    *p_err);


void Core_GOTO_04      (MEM         *p_mem,
                        CORE_24F    *p_core,
                        CPU_INT32U   args,
                        CORE_ERR    *p_err);

void Core_RETURN_060  (MEM         *p_mem,
                       CORE_24F    *p_core,
                       CPU_INT32U   args,
                       CORE_ERR    *p_err);

void Core_MOV_2        (CORE_24F    *p_core,
                        CPU_INT32U   args,
                        CORE_ERR    *p_err);

void Core_BRA_37       (MEM         *p_mem,
                        CORE_24F    *p_core,
                        CPU_INT32U   args,
                        CORE_ERR    *p_err);

void Core_MOV_WS_WD_78 (MEM         *p_mem,
                        CORE_24F    *p_core,
                        CPU_INT32U   args,
                        CORE_ERR    *p_err);

void Core_MOV_M_W_80   (MEM         *p_mem,
                        CORE_24F    *p_core,
                        CPU_INT32U   args,
                        CORE_ERR    *p_err);

void Core_MOV_W_M_88   (MEM         *p_mem,
                        CORE_24F    *p_core,
                        CPU_INT32U   args,
                        CORE_ERR    *p_err);

void Core_BSET_W_A0    (CORE_24F    *p_core,
                        CPU_INT32U   args,
                        CORE_ERR    *p_err);

void Core_BSET_M_A8    (MEM         *p_mem,
                        CORE_24F    *p_core,
                        CPU_INT32U   args,
                        CORE_ERR    *p_err);

void Core_BCLR_M_A9    (MEM         *p_mem,
                        CORE_24F    *p_core,
                        CPU_INT32U   args,
                        CORE_ERR    *p_err);

void  Core_BTSC_AF (MEM         *p_mem,
                    CORE_24F    *p_core,
                    CPU_INT32U   args,
                    CORE_ERR    *p_err);

void Core_SETM_MOV_8BL_WN_B3C (MEM         *p_mem,
                               CORE_24F    *p_core,
                               CPU_INT32U   args,
                               CORE_ERR    *p_err);

void Core_SETM_MOV_8BL_WN_B7A (MEM         *p_mem,
                               CORE_24F    *p_core,
                               CPU_INT32U   args,
                               CORE_ERR    *p_err);

void Core_SETM_WS_EB8  (MEM         *p_mem,
                        CORE_24F    *p_core,
                        CPU_INT32U   args,
                        CORE_ERR    *p_err);

void Core_SETM_M_W0_EF8 (MEM         *p_mem,
                         CORE_24F    *p_core,
                         CPU_INT32U   args,
                         CORE_ERR    *p_err);

void Core_CLR_M_W0_EF0  (MEM         *p_mem,
                         CORE_24F    *p_core,
                         CPU_INT32U   args,
                         CORE_ERR    *p_err);

void Core_CLR_M_W0_EF0  (MEM         *p_mem,
                         CORE_24F    *p_core,
                         CPU_INT32U   args,
                         CORE_ERR    *p_err);

void Core_PUSH_F8       (MEM         *p_mem,
                         CORE_24F    *p_core,
                         CPU_INT32U   args,
                         CORE_ERR    *p_err);

void Core_POP_F9        (MEM         *p_mem,
                         CORE_24F    *p_core,
                         CPU_INT32U   args,
                         CORE_ERR    *p_err);

#endif
