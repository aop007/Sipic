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

#define  CORE_OPC_SUBR_W_LIT_W   0x100060
#define  CORE_OPC_SUBBR_W_LIT_W  0x180060

#define  CORE_OPC_MOV_L_W    0x200000

#define  CORE_OPC_BRA_COND_SLIT  0x300000

#define  CORE_OPC_ADD_WB_WS_WD   0x400000
#define  CORE_OPC_ADD_W_LIT_W    0x400060
#define  CORE_OPC_ADDC_W_LIT_W   0x480060
#define  CORE_OPC_ADDC_WB_WS_WD  0x480000
#define  CORE_OPC_SUB_W_LIT_W    0x500060
#define  CORE_OPC_SUBB_W_LIT_W   0x580060
#define  CORE_OPC_AND_WB_WS_WD   0x600000
#define  CORE_OPC_AND_W_LIT_W    0x600060
#define  CORE_OPC_IOR_WB_WS_WD   0x700000
#define  CORE_OPC_IOR_W_LIT_W    0x700060
#define  CORE_OPC_SUB_WB_WS_WD   0x500000
#define  CORE_OPC_SUBB_WB_WS_WD  0x580000
#define  CORE_OPC_SUBBR_WB_WS_WD 0x180000
#define  CORE_OPC_SUBR_WB_WS_WD  0x100000
#define  CORE_OPC_XOR_WB_WS_WD   0x680000
#define  CORE_OPC_XOR_W_LIT_W    0x680060
    






#define  CORE_OPC_MOV_WS_WD  0x780000

#define  CORE_OPC_MOV_M_W    0x800000
#define  CORE_OPC_MOV_W_M    0x880000

#define  CORE_OPC_MOV_WS_OFF_WD  0x900000
#define  CORE_OPC_MOV_WS_WD_OFF  0x980000
    
#define  CORE_OPC_BSET_W     0xA00000
#define  CORE_OPC_BCLR_W     0xA10000
#define  CORE_OPC_BTG_W      0xA20000
#define  CORE_OPC_BTSS_W     0xA60000
#define  CORE_OPC_BTSC_W     0xA70000
#define  CORE_OPC_BSET_M     0xA80000
#define  CORE_OPC_BCLR_M     0xA90000
#define  CORE_OPC_BTSS       0xAE0000
#define  CORE_OPC_BTSC       0xAF0000

#define  CORE_OPC_ADD_LIT_W  0xB00000
#define  CORE_OPC_ADDC_LIT_W 0xB08000
#define  CORE_OPC_SUB_W_LIT  0xB10000
#define  CORE_OPC_SUBB_W_LIT 0xB18000
#define  CORE_OPC_AND_LIT_W  0xB20000
#define  CORE_OPC_IOR_LIT_W  0xB30000
#define  CORE_OPC_XOR_LIT_W  0xB28000
#define  CORE_OPC_ADD_M_W    0xB40000
#define  CORE_OPC_ADDC_M_W   0xB48000
#define  CORE_OPC_SUB_M_W    0xB50000
#define  CORE_OPC_SUBB_M_W   0xB58000
#define  CORE_OPC_AND_M_W    0xB60000
#define  CORE_OPC_XOR_M_W    0xB68000
#define  CORE_OPC_MUL_UU     0xB80000
#define  CORE_OPC_MUL_UU_LIT 0xB80060
#define  CORE_OPC_MUL_US     0xB88000
#define  CORE_OPC_MUL_SS     0xB98000
#define  CORE_OPC_MOV_8BL_WN 0xB3C000
#define  CORE_OPC_IOR_M_W    0xB70000
#define  CORE_OPC_MOV_WN_M   0xB7A000
#define  CORE_OPC_SUBR_M_W   0xBD0000
#define  CORE_OPC_SUBBR_M_W  0xBD8000    
#define  CORE_OPC_MOV2_M_W   0xBF8000

#define  CORE_OPC_RLC_WS_WD  0xD28000
#define  CORE_OPC_SL_M_W     0xD40000
#define  CORE_OPC_LSR_M_W    0xD50000
#define  CORE_OPC_ASR_M_W    0xD58000
#define  CORE_OPC_RLNC_M_W   0xD60000
#define  CORE_OPC_RLC_M_W    0xD68000
#define  CORE_OPC_RRNC_M_W   0xD70000
#define  CORE_OPC_RRC_M_W    0xD78000
#define  CORE_OPC_DIV_S      0xD80000
#define  CORE_OPC_DIV_U      0xD88000
#define  CORE_OPC_ASR_W_L_W  0xDE8040
#define  CORE_OPC_LSR_W_L_W  0xDE0040
#define  CORE_OPC_SL_W_L_W   0xDD0040

#define  CORE_OPC_CP0_WN_SF  0xE00000
#define  CORE_OPC_CP_W_LIT   0xE10060
#define  CORE_OPC_CP_WB_WS   0xE10000
#define  CORE_OPC_CPB_WB_WS  0xE18000
#define  CORE_OPC_CP0_M      0xE20000
#define  CORE_OPC_SETM_WS    0xEB8000
#define  CORE_OPC_INC_M_W    0xEC0000
#define  CORE_OPC_INC2_M_W   0xEC8000
#define  CORE_OPC_CLR_WD     0xEB0000
#define  CORE_OPC_DEC_M_W    0xED0000
#define  CORE_OPC_DEC2_M_W   0xED8000
#define  CORE_OPC_NEG_M_W    0xEE0000
#define  CORE_OPC_COM_M_W    0xEE8000
#define  CORE_OPC_CLR_M_W    0xEF0000
#define  CORE_OPC_SETM_M_W   0xEF8000

#define  CORE_OPC_PUSH_F8    0xF80000
#define  CORE_OPC_POP_F9     0xF90000

#define  CORE_OPC_SE         0xFB0000
#define  CORE_OPC_ZE         0xFB8000
#define  CORE_OPC_SWAP_WN    0xFD8000
    
#define  CORE_OPC_ASR_W        0xD18000
#define  CORE_OPC_COM_W        0xEA8000
#define  CORE_OPC_DEC_W        0xE90000
#define  CORE_OPC_DEC2_W       0xE98000
#define  CORE_OPC_INC_W        0xE80000
#define  CORE_OPC_INC2_W       0xE88000
#define  CORE_OPC_LSR_W        0xD10000
#define  CORE_OPC_NEG_W        0xEA0000
#define  CORE_OPC_RLC_W        0xD28000
#define  CORE_OPC_RLNC_W       0xD20000
#define  CORE_OPC_RRC_W        0xD38000
#define  CORE_OPC_RRNC_W       0xD30000
#define  CORE_OPC_SL_W         0xD00000
#define  CORE_OPC_TBLRDH_W     0xBA8000
#define  CORE_OPC_TBLRDL_W     0xBA0000
#define  CORE_OPC_TBLWDH_W     0xBB8000
#define  CORE_OPC_TBLWDL_W     0xBB0000
    

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

void Core_MATH_WN_LIT  (MEM_24       *p_mem_prog,
                        MEM          *p_mem_data,
                        CORE_24F     *p_core,
                        CPU_INT32U    args,
                        OPCODE        math_opc,
                        CORE_ERR     *p_err);

    void Core_MOV_WS_WD_OFF (MEM_24       *p_mem_prog,
                             MEM          *p_mem_data,
                             CORE_24F     *p_core,
                             CPU_INT32U    args,
                             OPCODE        instruction,
                             CORE_ERR     *p_err);
    
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

void Core_Logical_W_L_W(MEM_24      *p_mem_prog,
                            MEM         *p_mem_data,
                            CORE_24F    *p_core,
                            CPU_INT32U   args,
                            OPCODE       instruction,
                            CORE_ERR    *p_err);
    
void Core_BIT_LOGIC_W  (MEM_24      *p_mem_prog,
                        MEM         *p_mem_data,
                        CORE_24F    *p_core,
                        CPU_INT32U   args,
                        OPCODE       instruction,
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

void  Core_BTSS_AE (MEM_24      *p_mem_prog,
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

void Core_Logical_M_W (MEM_24      *p_mem_prog,
                                MEM         *p_mem_data,
                                CORE_24F    *p_core,
                                CPU_INT32U   args,
                                OPCODE       operation,
                       CORE_ERR    *p_err);
    
void Core_Logical     (MEM_24      *p_mem_prog,
                       MEM         *p_mem_data,
                       CORE_24F    *p_core,
                       CPU_INT32U   args,
                       OPCODE       operation,
                       CORE_ERR    *p_err);
    
void Core_Logical_W_L_W  (MEM_24      *p_mem_prog,
                          MEM         *p_mem_data,
                          CORE_24F    *p_core,
                          CPU_INT32U   args,
                          OPCODE       instruction,
                          CORE_ERR    *p_err);
    
void Core_Logical_Shift_W_L_W  (MEM_24      *p_mem_prog,
                              MEM         *p_mem_data,
                              CORE_24F    *p_core,
                              CPU_INT32U   args,
                              OPCODE       instruction,
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

void Core_DIV_SU_D8000 (MEM_24      *p_mem_prog,
                        MEM         *p_mem_data,
                        CORE_24F    *p_core,
                        CPU_INT32U   args,
                        OPCODE       instruction,
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

void Core_CP_WS_WB (MEM_24      *p_mem_prog,
                        MEM         *p_mem_data,
                        CORE_24F    *p_core,
                        CPU_INT32U   args,
                        OPCODE       instruction,
                        CORE_ERR    *p_err);
    
void Core_CP0_E20 (MEM_24      *p_mem_prog,
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

void Core_ZE_FB8        (MEM_24      *p_mem_prog,
                         MEM         *p_mem_data,
                         CORE_24F    *p_core,
                         CPU_INT32U   args,
                         CORE_ERR    *p_err);

void Core_SWAP_FD800    (MEM_24      *p_mem_prog,
                         MEM         *p_mem_data,
                         CORE_24F    *p_core,
                         CPU_INT32U   args,
                         CORE_ERR    *p_err);

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
}
#endif

#endif
