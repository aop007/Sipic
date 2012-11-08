//
//  core_24f_opcode.h
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-06.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#ifndef Sipic_core_24f_opcode_h
#define Sipic_core_24f_opcode_h

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

#include "core_24f.h"
#include "mem.h"

#define  CORE_OPC_NOP        0x000000
#define  CORE_OPC_BRA        0x010000
#define  CORE_OPC_CALL       0x020000

#define  CORE_OPC_GOTO       0x040000
#define  CORE_OPC_RETLW      0x050000
#define  CORE_OPC_RETURN     0x060000
#define  CORE_OPC_RETFIE     0x064000
#define  CORE_OPC_RCALL      0x070000
#define  CORE_OPC_DO         0x080000
#define  CORE_OPC_REPEAT     0x090000

#define  CORE_OPC_BRA_OA     0x0C0000
#define  CORE_OPC_BRA_OB     0x0D0000
#define  CORE_OPC_BRA_SA     0x0E0000
#define  CORE_OPC_BRA_SB     0x0F0000

#define  CORE_OPC_MOV_L_W    0x200000

#define  CORE_OPC_BRA_COND_SLIT  0x300000

#define  CORE_OPC_ADD_WB_WS_WD   0x400000
#define  CORE_OPC_ADDC_WB_WS_WD  0x480000
#define  CORE_OPC_AND_WB_WS_WD   0x600000
#define  CORE_OPC_IOR_WB_WS_WD   0x700000
#define  CORE_OPC_SUB_WB_WS_WD   0x500000
#define  CORE_OPC_SUBB_WB_WS_WD  0x510000
#define  CORE_OPC_SUBBR_WB_WS_WD 0x110000
#define  CORE_OPC_SUBR_WB_WS_WD  0x100000
#define  CORE_OPC_XOR_WB_WS_WD   0x680000
    
#define  CORE_OPC_ADDC_WB_LIT_WD 0x480060

#define  CORE_OPC_LIT_WB     0x500060



#define  CORE_OPC_MOV_WS_WD  0x780000

#define  CORE_OPC_MOV_M_W    0x800000
#define  CORE_OPC_MOV_W_M    0x880000

#define  CORE_OPC_BSET_W     0xA00000
#define  CORE_OPC_BTSS_W     0xA60000
#define  CORE_OPC_BTSC_W     0xA70000
#define  CORE_OPC_BSET_M     0xA80000
#define  CORE_OPC_BCLR_M     0xA90000
#define  CORE_OPC_BTSC       0xAF0000

#define  CORE_OPC_ADDC_LIT_W 0xB08000
#define  CORE_OPC_SUB_WN_LIT 0xB10000
#define  CORE_OPC_ADDC_M_W   0xB48000
#define  CORE_OPC_MUL_UU     0xB80000
#define  CORE_OPC_MUL_UU_LIT 0xB80060
#define  CORE_OPC_MUL_US     0xB88000
#define  CORE_OPC_MUL_SS     0xB98000
#define  CORE_OPC_MOV_8BL_WN 0xB3C000
#define  CORE_OPC_ADD_B40    0xB40000
#define  CORE_OPC_MOV_WN_M   0xB7A000
#define  CORE_OPC_MOV_M_WM   0xBF8000

#define  CORE_OPC_RLC_WS_WD  0xD28000
#define  CORE_OPC_DIV_S      0xD80000

#define  CORE_OPC_CP0_WN_SF  0xE00000
#define  CORE_OPC_CP_W_LIT   0xE10060
#define  CORE_OPC_CP_WB_WS   0xE10000
#define  CORE_OPC_DEC_WS_WD  0xE90000
#define  CORE_OPC_SETM_WS    0xEB8000
#define  CORE_OPC_INC_EC0    0xEC0000
#define  CORE_OPC_CLR_WD     0xEB0000
#define  CORE_OPC_CLR_M_W0   0xEF0000
#define  CORE_OPC_SETM_M_W0  0xEF8000

#define  CORE_OPC_PUSH_F8    0xF80000
#define  CORE_OPC_POP_F9     0xF90000

#define  CORE_OPC_SE         0xFB0000

#define  CORE_OPC_ADDR_MODE_DIR          0x00
#define  CORE_OPC_ADDR_MODE_IND          0x01
#define  CORE_OPC_ADDR_MODE_IND_POS_DEC  0x02
#define  CORE_OPC_ADDR_MODE_IND_POS_INC  0x03
#define  CORE_OPC_ADDR_MODE_IND_PRE_DEC  0x04
#define  CORE_OPC_ADDR_MODE_IND_PRE_INC  0x05
#define  CORE_OPC_ADDR_MODE_OFFSET       0x06


#define  CORE_OPC_BRA_OV        0x0
#define  CORE_OPC_BRA_C         0x1
#define  CORE_OPC_BRA_Z         0x2
#define  CORE_OPC_BRA_N         0x3
#define  CORE_OPC_BRA_LE        0x4
#define  CORE_OPC_BRA_LT        0x5
#define  CORE_OPC_BRA_LEU       0x6
#define  CORE_OPC_BRA_UNC       0x7
#define  CORE_OPC_BRA_NOV       0x8
#define  CORE_OPC_BRA_NC        0x9
#define  CORE_OPC_BRA_NZ        0xA
#define  CORE_OPC_BRA_NN        0xB
#define  CORE_OPC_BRA_GT        0xC
#define  CORE_OPC_BRA_GE        0xD
#define  CORE_OPC_BRA_GTU       0xE
    
    
void Core_OPC_Stats           (MEM_24      *p_mem);

void  Core_NOP_00             (MEM_24      *p_mem_prog,
                               MEM         *p_mem_data,
                               CORE_24F    *p_core,
                               CPU_INT32U   args,
                               CORE_ERR    *p_err);

void  Core_CALL_02            (MEM_24      *p_mem_prog,
                               MEM         *p_mem_data,
                               CORE_24F    *p_core,
                               CPU_INT32U   args,
                               CORE_ERR    *p_err);


void Core_GOTO_04      (MEM_24      *p_mem_prog,
                        MEM         *p_mem_data,
                        CORE_24F    *p_core,
                        CPU_INT32U   args,
                        CORE_ERR    *p_err);

void Core_RETURN_060  (MEM_24      *p_mem_prog,
                       MEM         *p_mem_data,
                       CORE_24F    *p_core,
                       CPU_INT32U   args,
                       CORE_ERR    *p_err);

void Core_RETFIE_064  (MEM_24      *p_mem_prog,
                       MEM         *p_mem_data,
                       CORE_24F    *p_core,
                       CPU_INT32U   args,
                       CORE_ERR    *p_err);

void Core_REPEAT_LIT14(MEM_24      *p_mem_prog,
                       MEM         *p_mem_data,
                       CORE_24F    *p_core,
                       CPU_INT32U   args,
                       CORE_ERR    *p_err);


void Core_MOV_2        (MEM_24      *p_mem_prog,
                        MEM         *p_mem_data,
                        CORE_24F    *p_core,
                        CPU_INT32U   args,
                        CORE_ERR    *p_err);
    
void Core_BRA_3 (MEM_24      *p_mem_prog,
                     MEM         *p_mem_data,
                     CORE_24F    *p_core,
                     CPU_INT32U   args,
                 CORE_ERR    *p_err);

void Core_MATH_WS_WD   (MEM_24        *p_mem_prog,
                        MEM           *p_mem_data,
                        CORE_24F      *p_core,
                        CPU_INT32U     args,
                        OPCODE         math_opc,
                        CORE_ERR      *p_err);

void Core_ADDC_48 (MEM_24      *p_mem_prog,
                   MEM         *p_mem_data,
                   CORE_24F    *p_core,
                   CPU_INT32U   args,
                   CORE_ERR    *p_err);

void Core_ADDC_48006 (MEM_24      *p_mem_prog,
                      MEM         *p_mem_data,
                      CORE_24F    *p_core,
                      CPU_INT32U   args,
                      CORE_ERR    *p_err);

void Core_SUB_50006 (MEM_24      *p_mem_prog,
                     MEM         *p_mem_data,
                     CORE_24F    *p_core,
                     CPU_INT32U   args,
                     CORE_ERR    *p_err);

void Core_MOV_WS_WD_78 (MEM_24      *p_mem_prog,
                        MEM         *p_mem_data,
                        CORE_24F    *p_core,
                        CPU_INT32U   args,
                        CORE_ERR    *p_err);

void Core_MOV_M_W_80   (MEM_24      *p_mem_prog,
                        MEM         *p_mem_data,
                        CORE_24F    *p_core,
                        CPU_INT32U   args,
                        CORE_ERR    *p_err);

void Core_MOV_W_M_88   (MEM_24      *p_mem_prog,
                        MEM         *p_mem_data,
                        CORE_24F    *p_core,
                        CPU_INT32U   args,
                        CORE_ERR    *p_err);

void Core_BSET_W_A0    (MEM_24      *p_mem_prog,
                        MEM         *p_mem_data,
                        CORE_24F    *p_core,
                        CPU_INT32U   args,
                        CORE_ERR    *p_err);

void Core_BSET_M_A8    (MEM_24      *p_mem_prog,
                        MEM         *p_mem_data,
                        CORE_24F    *p_core,
                        CPU_INT32U   args,
                        CORE_ERR    *p_err);

void Core_BCLR_M_A9    (MEM_24      *p_mem_prog,
                        MEM         *p_mem_data,
                        CORE_24F    *p_core,
                        CPU_INT32U   args,
                        CORE_ERR    *p_err);

void  Core_BTSS_W (MEM_24      *p_mem_prog,
                   MEM         *p_mem_data,
                   CORE_24F    *p_core,
                   CPU_INT32U   args,
                   CORE_ERR    *p_err);

void  Core_BTSC_W (MEM_24      *p_mem_prog,
                   MEM         *p_mem_data,
                   CORE_24F    *p_core,
                   CPU_INT32U   args,
                   CORE_ERR    *p_err);

void  Core_BTSC_AF (MEM_24      *p_mem_prog,
                    MEM         *p_mem_data,
                    CORE_24F    *p_core,
                    CPU_INT32U   args,
                    CORE_ERR    *p_err);

void Core_ADDC_B08 (MEM_24      *p_mem_prog,
                    MEM         *p_mem_data,
                    CORE_24F    *p_core,
                    CPU_INT32U   args,
                    CORE_ERR    *p_err);

void Core_SETM_MOV_8BL_WN_B3C (MEM_24      *p_mem_prog,
                               MEM         *p_mem_data,
                               CORE_24F    *p_core,
                               CPU_INT32U   args,
                               CORE_ERR    *p_err);

void Core_SETM_MOV_8BL_WN_B7A (MEM_24      *p_mem_prog,
                               MEM         *p_mem_data,
                               CORE_24F    *p_core,
                               CPU_INT32U   args,
                               CORE_ERR    *p_err);

void Core_SUB_B10 (MEM_24      *p_mem_prog,
                   MEM         *p_mem_data,
                   CORE_24F    *p_core,
                   CPU_INT32U   args,
                   CORE_ERR    *p_err);

void Core_ADD_B40     (MEM_24      *p_mem_prog,
                       MEM         *p_mem_data,
                       CORE_24F    *p_core,
                       CPU_INT32U   args,
                       CORE_ERR    *p_err);

void Core_ADDC_B48 (MEM_24      *p_mem_prog,
                    MEM         *p_mem_data,
                    CORE_24F    *p_core,
                    CPU_INT32U   args,
                    CORE_ERR    *p_err);

void Core_MUL_UU_B8006 (MEM_24      *p_mem_prog,
                        MEM         *p_mem_data,
                        CORE_24F    *p_core,
                        CPU_INT32U   args,
                        CORE_ERR    *p_err);

void Core_MUL_UU_B80 (MEM_24      *p_mem_prog,
                      MEM         *p_mem_data,
                      CORE_24F    *p_core,
                      CPU_INT32U   args,
                      CORE_ERR    *p_err);

void Core_MUL_SS_B88 (MEM_24      *p_mem_prog,
                      MEM         *p_mem_data,
                      CORE_24F    *p_core,
                      CPU_INT32U   args,
                      CORE_ERR    *p_err);

void Core_MUL_SS_B98 (MEM_24      *p_mem_prog,
                      MEM         *p_mem_data,
                      CORE_24F    *p_core,
                      CPU_INT32U   args,
                      CORE_ERR    *p_err);

void Core_MOV_BF8 (MEM_24      *p_mem_prog,
                   MEM         *p_mem_data,
                   CORE_24F    *p_core,
                   CPU_INT32U   args,
                   CORE_ERR    *p_err);

void Core_DIV_S_D8000 (MEM_24      *p_mem_prog,
                       MEM         *p_mem_data,
                       CORE_24F    *p_core,
                       CPU_INT32U   args,
                       CORE_ERR    *p_err);

void Core_DEC_E90 (MEM_24      *p_mem_prog,
                   MEM         *p_mem_data,
                   CORE_24F    *p_core,
                   CPU_INT32U   args,
                   CORE_ERR    *p_err);

void Core_CP0_E0000 (MEM_24      *p_mem_prog,
                     MEM         *p_mem_data,
                     CORE_24F    *p_core,
                     CPU_INT32U   args,
                     CORE_ERR    *p_err);

void Core_CP_E1000 (MEM_24      *p_mem_prog,
                        MEM         *p_mem_data,
                        CORE_24F    *p_core,
                        CPU_INT32U   args,
                        CORE_ERR    *p_err);
    
void Core_CP_E1006 (MEM_24      *p_mem_prog,
                    MEM         *p_mem_data,
                    CORE_24F    *p_core,
                    CPU_INT32U   args,
                    CORE_ERR    *p_err);

void Core_SETM_WS_EB8  (MEM_24      *p_mem_prog,
                        MEM         *p_mem_data,
                        CORE_24F    *p_core,
                        CPU_INT32U   args,
                        CORE_ERR    *p_err);

void Core_INC_EC0      (MEM_24      *p_mem_prog,
                        MEM         *p_mem_data,
                        CORE_24F    *p_core,
                        CPU_INT32U   args,
                        CORE_ERR    *p_err);

void Core_CLR_WD_EB0000 (MEM_24      *p_mem_prog,
                         MEM         *p_mem_data,
                         CORE_24F    *p_core,
                         CPU_INT32U   args,
                         CORE_ERR    *p_err);

void Core_SETM_M_W0_EF8 (MEM_24      *p_mem_prog,
                         MEM         *p_mem_data,
                         CORE_24F    *p_core,
                         CPU_INT32U   args,
                         CORE_ERR    *p_err);

void Core_CLR_M_W0_EF0  (MEM_24      *p_mem_prog,
                         MEM         *p_mem_data,
                         CORE_24F    *p_core,
                         CPU_INT32U   args,
                         CORE_ERR    *p_err);

void Core_CLR_M_W0_EF0  (MEM_24      *p_mem_prog,
                         MEM         *p_mem_data,
                         CORE_24F    *p_core,
                         CPU_INT32U   args,
                         CORE_ERR    *p_err);

void Core_PUSH_F8       (MEM_24      *p_mem_prog,
                         MEM         *p_mem_data,
                         CORE_24F    *p_core,
                         CPU_INT32U   args,
                         CORE_ERR    *p_err);

void Core_POP_F9        (MEM_24      *p_mem_prog,
                         MEM         *p_mem_data,
                         CORE_24F    *p_core,
                         CPU_INT32U   args,
                         CORE_ERR    *p_err);

void Core_SE_FB00       (MEM_24      *p_mem_prog,
                         MEM         *p_mem_data,
                         CORE_24F    *p_core,
                         CPU_INT32U   args,
                         CORE_ERR    *p_err);

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
}
#endif

#endif
