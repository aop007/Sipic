//
//  core_24f.c
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-05.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core_24f.h"
#include "core_24f_opcode.h"
#include "mem.h"
#include "cpu.h"
#include "main.h"

CORE_24F * Core_Init(MEM         *p_mem_data,
                             CPU_INT32U   addr,
                             CORE_ERR    *p_err)
{
    CORE_24F  *p_core;
    MEM_ERR    mem_err;
#if 1
    CPU_INT64U  offset;
#endif
    
    
    p_core = (CORE_24F *)Mem_GetAddr(p_mem_data, addr, &mem_err);
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return (NULL);
    }
    
    memset(p_core, 0, sizeof(CORE_24F));
    
    p_core->W[15] = 0x0800;
#ifdef  CORE_CFG_CYCLE_CNTR
    p_core->CYCLE = 0;
#endif
    
#if 1
    offset = (CPU_INT64U)&p_core->SPLIM - (CPU_INT64U)p_core;
    offset = (CPU_INT64U)&p_core->ACCA - (CPU_INT64U)p_core;
    offset = (CPU_INT64U)&p_core->ACCB - (CPU_INT64U)p_core;
    offset = (CPU_INT64U)&p_core->PC - (CPU_INT64U)p_core;
    offset = (CPU_INT64U)&p_core->RCOUNT - (CPU_INT64U)p_core;
    offset = (CPU_INT64U)&p_core->DCOUNT - (CPU_INT64U)p_core;
    offset = (CPU_INT64U)&p_core->SR - (CPU_INT64U)p_core;
#endif
    /* Write protection */
    
    Mem_SetAccess(p_mem_data, 0x0042, 0x01EF, &mem_err);        /* Protect SR SRF */
    
    return (p_core);
}

void  Core_Run(CORE_24F  *p_core_24f,
               MEM_24    *p_mem_prog,
               MEM       *p_mem_data,
               CORE_ERR  *p_err)
{
    CPU_BOOLEAN   found_instruction;
    CPU_BOOLEAN   is_call;
    //CORE_24F     *p_core_24f;
    OPCODE        opcode;
    OPCODE        instruction;
    OPCODE        args;
    CPU_INT32U    ix;
    CPU_INT32U    uncaught_instructions;
    CPU_INT32U    pc;
    MEM_ERR       mem_err;
    //CORE_ERR      core_err;
    
    
    uncaught_instructions = 0;
    //memset(p_core_24f, 0, sizeof(CORE_24F));
    
    
#if 0
    while (1) {
#endif
        pc     = Core_PC_Get(p_core_24f);
        opcode = Mem_Get24(p_mem_prog,
                           pc,
                          &mem_err);

        if (mem_err != MEM_ERR_NONE) {
            *p_err = CORE_ERR_ADDR_ERROR_TRAP;
            return;
        }

        if (opcode == 0) {
            CORE_TRACE_DEBUG(("\r\nNULL OPC \t %d", uncaught_instructions));
            
        }

        if ((p_core_24f->SR & CORE_SR_RA) == CORE_SR_RA) {
            p_core_24f->RCOUNT--;
            if (p_core_24f->RCOUNT == 0) {
                p_core_24f->SR &= ~(CORE_SR_RA);
            } else {
                Core_PC_Slide(p_core_24f, -2);
            }
        }

#ifdef  CORE_CFG_CYCLE_CNTR
        CORE_TRACE_DEBUG(("\r\nPC = %004x\tOPC = %006x\tCYCLE = %d |", Core_PC_Get(p_core_24f), opcode, (CPU_INT32U)p_core_24f->CYCLE));
#else
        CORE_TRACE_DEBUG(("\r\n"));
        for (ix = 0 ; ix < Call_Depth ; ix++) {
            CORE_TRACE_DEBUG(("-"));
        }
        CORE_TRACE_DEBUG(("PC = %004x\tOPC = %006x\t |", Core_PC_Get(p_core_24f), opcode));
#endif
#if 1
        if ((Core_PC_Get(p_core_24f) == 0x0C90)) { // && (p_core_24f->W[0] == 0)){
            uncaught_instructions *= 1;
            CORE_TRACE_DEBUG((""));
        }
        
        if (p_core_24f->W[15] == 0) {
            CORE_TRACE_DEBUG((""));
        }
#endif
        found_instruction = DEF_NO;
        is_call           = DEF_NO;
        
        if (found_instruction == DEF_NO) {
            instruction = opcode & 0xFFFFFF;
            args        = opcode & 0x000000;
            found_instruction = DEF_YES;
            
            switch (instruction) {
                case CORE_OPC_RETURN:
                    Core_RETURN_060(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                case CORE_OPC_RETFIE:
                    Core_RETFIE_064(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                    
                default:
                    found_instruction = DEF_NO;
                    break;
            }
        }

        if (found_instruction == DEF_NO) {
            instruction = opcode & 0xFFBFF0;
            args        = opcode & 0x00400F;
            found_instruction = DEF_YES;
            
            switch (instruction) {
                case CORE_OPC_SWAP_WN:
                    Core_SWAP_FD800(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                    
                default:
                    found_instruction = DEF_NO;
                    break;
            }
        }
        
        if (found_instruction == DEF_NO) {
            instruction = opcode & 0xFFFB80;
            args        = opcode & 0x00047F;
            found_instruction = DEF_YES;
            
            switch (instruction) {
                case CORE_OPC_CP0_WN_SF:
                    Core_CP0_E0000(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                    
                default:
                    found_instruction = DEF_NO;
                    break;
            }
        }
        
        if (found_instruction == DEF_NO) {
            instruction = opcode & 0xFF807F;
            args        = opcode & 0x007F80;
            found_instruction = DEF_YES;
            
            switch (instruction) {
                case CORE_OPC_CLR_WD:
                    Core_CLR_WD_EB0000(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                    
                default:
                    found_instruction = DEF_NO;
                    break;
            }
        }
        
        if (found_instruction == DEF_NO) {
            instruction = opcode & 0xFF8060;
            args        = opcode & 0x007F9F;
            found_instruction = DEF_YES;
            
            switch (instruction) {
                case CORE_OPC_MUL_UU_LIT:
                    Core_MUL_UU_B8006(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                    
                default:
                    found_instruction = DEF_NO;
                    break;
            }
        }
        
        
        
        if (found_instruction == DEF_NO) {
            instruction = opcode & 0xF80060;
            args        = opcode & 0x07FF9F;
            found_instruction = DEF_YES;
            
            switch (instruction) {
                case CORE_OPC_LIT_WB:
                    Core_SUB_50006(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                    
                case CORE_OPC_ADDC_WB_LIT_WD:
                    Core_ADDC_48006(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                default:
                    found_instruction = DEF_NO;
                    break;
            }
        }
        
        if (found_instruction == DEF_NO) {
            instruction = opcode & 0xFF0480;
            args        = opcode & 0x00FB7F;
            found_instruction = DEF_YES;
            
            switch (instruction) {
                case CORE_OPC_BCLR_W:
                case CORE_OPC_BSET_W:
                case CORE_OPC_BTG_W:
                    Core_BIT_LOGIC_W(p_mem_prog, p_mem_data, p_core_24f, args, instruction, p_err);
                    break;
                    
                default:
                    found_instruction = DEF_NO;
                    break;
            }
        }

        if (found_instruction == DEF_NO) {
            instruction = opcode & 0xFF0F80;
            args        = opcode & 0x00F07F;
            found_instruction = DEF_YES;
            
            switch (instruction) {
                case CORE_OPC_BTSC_W:
                    Core_BTSC_W(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                case CORE_OPC_BTSS_W:
                    Core_BTSS_W(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                default:
                    found_instruction = DEF_NO;
                    break;
            }
        }
        
        if (found_instruction == DEF_NO) {
            instruction = opcode & 0xFF83E0;
            args        = opcode & 0x007C1F;
            found_instruction = DEF_YES;
            
            switch (instruction) {
                case CORE_OPC_CP_W_LIT:
                    Core_CP_E1006(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                    
                default:
                    found_instruction = DEF_NO;
                    break;
            }
        }
        
        if (found_instruction == DEF_NO) {
            instruction = opcode & 0xFF8380;
            args        = opcode & 0x007C7F;
            found_instruction = DEF_YES;
            
            switch (instruction) {
                case CORE_OPC_CP_WB_WS:
                    Core_CP_E1000(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                default:
                    found_instruction = DEF_NO;
                    break;
            }
        }
        
        if (found_instruction == DEF_NO) {
            instruction = opcode & 0xFFF800;
            args        = opcode & 0x0007FF;
            found_instruction = DEF_YES;
            
            switch (instruction) {
                case CORE_OPC_SE:
                    Core_SE_FB00(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                    
                default:
                    found_instruction = DEF_NO;
                    break;
            }
        }
        
        if (found_instruction == DEF_NO) {
            instruction = opcode & 0xFFF000;
            args        = opcode & 0x000FFF;
            found_instruction = DEF_YES;
            
            switch (instruction) {
                case CORE_OPC_MOV_8BL_WN:
                    Core_SETM_MOV_8BL_WN_B3C(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                    
                default:
                    found_instruction = DEF_NO;
                    break;
            }
        }
        
        
        
        if (found_instruction == DEF_NO) {
            instruction = opcode & 0xFFC000;
            args        = opcode & 0x003FFF;
            found_instruction = DEF_YES;
            
            switch (instruction) {
                case CORE_OPC_REPEAT:
                    Core_REPEAT_LIT14(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                case CORE_OPC_ZE:
                    Core_ZE_FB8(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                default:
                    found_instruction = DEF_NO;
                    break;
            }
        }
        
        if (found_instruction == DEF_NO) {
            instruction = opcode & 0xFF8030;
            args        = opcode & 0x007FCF;
            found_instruction = DEF_YES;
            
            switch (instruction) {
                case CORE_OPC_DIV_S:
                    Core_DIV_S_D8000(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                    
                default:
                    found_instruction = DEF_NO;
                    break;
            }
        }

        if (found_instruction == DEF_NO) {
            instruction = opcode & 0xFF8000;
            args        = opcode & 0x007FFF;
            found_instruction = DEF_YES;
            
            switch (instruction) {
                case CORE_OPC_ADD_LIT_W:
                case CORE_OPC_ADDC_LIT_W:
                case CORE_OPC_SUB_W_LIT:
                case CORE_OPC_SUBB_W_LIT:
                case CORE_OPC_AND_LIT_W:
                case CORE_OPC_IOR_LIT_W:
                case CORE_OPC_XOR_LIT_W:
                    Core_MATH_WN_LIT(p_mem_prog, p_mem_data, p_core_24f, args, instruction, p_err);
                    break;

#if 1
                /* BDf ffff ffff ffff */
                case CORE_OPC_ADD_M_W:
                case CORE_OPC_ADDC_M_W:
                case CORE_OPC_AND_M_W:
                case CORE_OPC_ASR_M_W:
                case CORE_OPC_CLR_M_W:
                case CORE_OPC_COM_M_W:
                case CORE_OPC_DEC_M_W:
                case CORE_OPC_DEC2_M_W:
                case CORE_OPC_INC_M_W:
                case CORE_OPC_INC2_M_W:
                case CORE_OPC_IOR_M_W:
                case CORE_OPC_LSR_M_W:
                case CORE_OPC_MOV2_M_W:
                case CORE_OPC_NEG_M_W:
                case CORE_OPC_RLC_M_W:
                case CORE_OPC_RLNC_M_W:
                case CORE_OPC_RRC_M_W:
                case CORE_OPC_RRNC_M_W:
                case CORE_OPC_SETM_M_W:
                case CORE_OPC_SL_M_W:
                case CORE_OPC_SUB_M_W:
                case CORE_OPC_SUBB_M_W:
                case CORE_OPC_SUBBR_M_W:
                case CORE_OPC_SUBR_M_W:
                case CORE_OPC_XOR_M_W:
                    Core_Logical_M_W(p_mem_prog, p_mem_data, p_core_24f, args, instruction, p_err);
                    break;
#else
                case CORE_OPC_ADD_B40:
                    Core_ADD_B40(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                case CORE_OPC_SETM_M_W0:
                    Core_SETM_M_W0_EF8(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                case CORE_OPC_CLR_M_W0:
                    Core_CLR_M_W0_EF0(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                case CORE_OPC_ADDC_M_W:
                    Core_ADDC_B48(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;

                case CORE_OPC_INC_M:
                    Core_INC_EC0(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                case CORE_OPC_MOV_M_W:
                    Core_MOV_BF8(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
#endif
                    

                    
                case CORE_OPC_MUL_UU:
                    Core_MUL_UU_B80(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;

                case CORE_OPC_MUL_US:
                    Core_MUL_SS_B88(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                case CORE_OPC_MUL_SS:
                    Core_MUL_SS_B98(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    

                    
                case CORE_OPC_SETM_WS:
                    Core_SETM_WS_EB8(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                
                    
                    

                    
                case CORE_OPC_INC_W:
                case CORE_OPC_DEC_W:
                case CORE_OPC_RLC_W:
                case CORE_OPC_ASR_W:
                case CORE_OPC_COM_W:
                case CORE_OPC_INC2_W:
                case CORE_OPC_DEC2_W:
                case CORE_OPC_LSR_W:
                case CORE_OPC_NEG_W:
                case CORE_OPC_RLNC_W:
                case CORE_OPC_RRC_W:
                case CORE_OPC_RRNC_W:
                case CORE_OPC_SL_W:
                case CORE_OPC_TBLRDH_W:
                case CORE_OPC_TBLRDL_W:
                case CORE_OPC_TBLWDH_W:
                case CORE_OPC_TBLWDL_W:

                    Core_Logical(p_mem_prog, p_mem_data, p_core_24f, args, instruction, p_err);
                    break;
                                        
                default:
                    found_instruction = DEF_NO;
                    break;
            }
        }
        
        if (found_instruction == DEF_NO) {
            instruction = opcode & 0xFFA000;
            args        = opcode & 0x005FFF;
            found_instruction = DEF_YES;
            
            switch (instruction) {
                case CORE_OPC_MOV_WN_M:
                    Core_SETM_MOV_8BL_WN_B7A(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                case CORE_OPC_CP0_M:
                    Core_CP0_E20(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                default:
                    found_instruction = DEF_NO;
                    break;
            }
        }
        
        if (found_instruction == DEF_NO) {
            instruction = opcode & 0xFF0000;
            args        = opcode & 0x00FFFF;
            found_instruction = DEF_YES;
            
            switch (instruction) {
                case CORE_OPC_NOP:
                    Core_NOP_00(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                case CORE_OPC_CALL:
                    Core_CALL_02(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    is_call = DEF_YES;
                    break;

                case CORE_OPC_GOTO:
                    Core_GOTO_04(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;

                case CORE_OPC_BSET_M:
                    Core_BSET_M_A8(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                    
                case CORE_OPC_BCLR_M:
                    Core_BCLR_M_A9(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                case CORE_OPC_BTSS:
                    Core_BTSS_AE(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                case CORE_OPC_BTSC:
                    Core_BTSC_AF(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                case CORE_OPC_PUSH_F8:
                    Core_PUSH_F8(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                case CORE_OPC_POP_F9:
                    Core_POP_F9(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                    
                    
                default:
                    found_instruction = DEF_NO;
                    break;
            }
        }

        
        if (found_instruction == DEF_NO) {
            instruction = opcode & 0xF80000;
            args        = opcode & 0x07FFFF;
            found_instruction = DEF_YES;
            
            switch (instruction) {
                case CORE_OPC_ADD_WB_WS_WD:
                case CORE_OPC_SUB_WB_WS_WD:
                case CORE_OPC_ADDC_WB_WS_WD:
                case CORE_OPC_AND_WB_WS_WD:
                case CORE_OPC_IOR_WB_WS_WD:
                case CORE_OPC_SUBB_WB_WS_WD:
                case CORE_OPC_SUBBR_WB_WS_WD:
                case CORE_OPC_SUBR_WB_WS_WD:
                case CORE_OPC_XOR_WB_WS_WD:
                    Core_MATH_WS_WD(p_mem_prog, p_mem_data, p_core_24f, args, instruction, p_err);
                    break;

                    
                case CORE_OPC_MOV_WS_WD:
                    Core_MOV_WS_WD_78(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                    
                case CORE_OPC_MOV_M_W:
                    Core_MOV_M_W_80(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                    
                case CORE_OPC_MOV_W_M:
                    Core_MOV_W_M_88(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
#if 0
                case CORE_OPC_ADDC_WB_WS_WD:
                    Core_ADDC_48(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
#endif
                default:
                    found_instruction = DEF_NO;
                    break;
            }
        }

        
        if (found_instruction == DEF_NO) {
            instruction = opcode & 0xF00000;
            args        = opcode & 0x0FFFFF;
            found_instruction = DEF_YES;
        
            switch (instruction) {
                case CORE_OPC_MOV_L_W:
                    Core_MOV_2(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                    
                case CORE_OPC_BRA_COND_SLIT:
                    Core_BRA_3(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                
                
                default:
                    found_instruction = DEF_NO;
                    break;
            }
        }

        
        if ((found_instruction == DEF_NO) ||
            (*p_err            != CORE_ERR_NONE)) {
            *p_err = CORE_ERR_OPC_NOTFOUND;
#ifdef  CORE_CFG_CYCLE_CNTR
            CORE_TRACE_INFO(("\r\nINSTRUCTION NOT FOUND. %X at %X \t@%ld cycles.", opcode, Core_PC_Get(p_core_24f), p_core_24f->CYCLE));
#else
            CORE_TRACE_INFO(("\r\nINSTRUCTION Error. %X at %X.", opcode, Core_PC_Get(p_core_24f)));
#endif
            return;
        }
        
        for (ix = 0; ix < 16 ; ix++) {
            CORE_TRACE_DEBUG(("\t%004x", p_core_24f->W[ix]));
        }
 
#if 0
        if (is_call) {
            CORE_TRACE_DEBUG(("\r\n"));
        }
#endif
        
        if (*p_err != CORE_ERR_NONE) {
            return;
        }

#ifdef  CORE_CFG_CYCLE_CNTR
        p_core_24f->CYCLE++;
#endif
#if 0
    }
#endif
    //CORE_TRACE_INFO(("\r\nFATAL CORE ERROR: %d", (CPU_INT32U)(*p_err)));
}

void  Core_Push(CPU_INT32U   val,
                CORE_24F    *p_core,
                MEM         *p_mem,
                CORE_ERR    *p_err)
{
    MEM_ERR  mem_err;
    
    Mem_Set(p_mem, p_core->W[15], val, &mem_err);
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    p_core->W[15] += 2;
    
    *p_err = CORE_ERR_NONE;
}

CPU_INT32U  Core_Pop (CORE_24F    *p_core,
                      MEM         *p_mem,
                      CORE_ERR    *p_err)
{
    CPU_INT32U  val;
    MEM_ERR     mem_err;
    
    p_core->W[15] -= 2;
    
    val = Mem_Get(p_mem, p_core->W[15], &mem_err);
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return (0xFFFFFFFF);
    }
    
    *p_err = CORE_ERR_NONE;
    
    return (val);
}

CPU_INT32U  Core_OPC_Words (OPCODE  opc)
{
    if (((opc & 0xFF0001) == 0x020000) |                        /* CALL EXPR           0x020000 /  0xFF0001 */
        ((opc & 0xFFC000) == 0x080000) |                        /* DO   #lit14, Expr   0x080000 /  0xFFC000 */
        ((opc & 0xFFFFF0) == 0x088000) |                        /* DO   Wn,     Expr   0x088000 /  0xFFFFF0 */
        ((opc & 0xFFFFF0) == 0x014000)) {                       /* GOTO Wn             0x014000 /  0xFFFFF0 */
        return 2;
    } else {
        return 1;
    }
}

CPU_INT08U  Core_GetC (CORE_24F  *p_core)
{
    if ((p_core->SR & CORE_SR_C) == 0) {
        return (0);
    } else {
        return (1);
    }
}

void Core_SetC (CORE_24F     *p_core,
                CPU_BOOLEAN   value)
{
    if (value) {
        p_core->SR |=  CORE_SR_C;
    } else {
        p_core->SR &= ~CORE_SR_C;
    }
}

CPU_INT08U  Core_GetZ (CORE_24F  *p_core)
{
    if ((p_core->SR & CORE_SR_Z) == 0) {
        return (0);
    } else {
        return (1);
    }
}

CPU_INT08U  Core_GetOV (CORE_24F  *p_core)
{
    if ((p_core->SR & CORE_SR_OV) == 0) {
        return (0);
    } else {
        return (1);
    }
}

CPU_INT08U  Core_GetN (CORE_24F  *p_core)
{
    if ((p_core->SR & CORE_SR_N) == 0) {
        return (0);
    } else {
        return (1);
    }
}

void  Core_PC_Slide (CORE_24F    *p_core,
                     CPU_INT32S   slide)
{
    CPU_INT32U  PC;
    
    memcpy(&PC, (const void *)&p_core->PC[0], sizeof(PC));
    
    PC += slide;
    
    memcpy((void *)&p_core->PC[0], &PC, sizeof(PC));
}

void  Core_PC_Set (CORE_24F    *p_core,
                   CPU_INT32U   value)
{
    CPU_INT32U  PC;
    
    memcpy(&PC, (const void *)&p_core->PC[0], sizeof(PC));
    
    PC = value;
    
    memcpy((void *)&p_core->PC[0], &PC, sizeof(PC));
}

CPU_INT32U  Core_PC_Get (CORE_24F    *p_core)
{
    CPU_INT32U  PC;
    
    memcpy(&PC, (const void *)&p_core->PC[0], sizeof(PC));
    
    return (PC);
}

CPU_INT32U  Core_MaskGet (CPU_INT08U  byte_mode, 
                          CPU_INT32U  addr)
{
    if (byte_mode == 0) {
        return 0xFFFF;
    } else {
        if (addr & 0x1) {
            return 0xFF00;
        } else {
            return 0xFF;
        }
    }
}

CPU_INT32U  Core_Align   (CPU_INT32U    value,
                          CPU_INT32U    mask)
{
    switch(mask) {
        case 0xFF:
        case 0xFFFF:
            return (value & mask);

        case 0xFF00:
            return ((value & mask) >> 8);

        default:
            printf("\r\n***\r\nUnsupported mask\r\n***");
            return 0;
    }
}

CPU_INT32U  Core_Merge   (CPU_INT32U    value_original,
                          CPU_INT32U    value,
                          CPU_INT32U    mask)
{
    switch(mask) {
        case 0xFF:
        case 0xFFFF:
            return (value & mask) | (value_original & ~(mask));
            
        case 0xFF00:
            return ((value << 8) & mask) | (value_original & ~(mask));
            
        default:
            printf("\r\n***\r\nUnsupported mask\r\n***");
            return 0;
    }
}







