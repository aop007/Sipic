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

#ifndef AOP_DEBUG
CPU_INT32U  RCOUNT_1C06 = 0xFFFFFFFF;
#endif

CORE_24F  *Core_Init(MEM         *p_mem_data,
                     CPU_INT32U   addr,
                     CORE_ERR    *p_err)
{
    CORE_24F  *p_core;
    MEM_ERR    mem_err;
    
    
    p_core = (CORE_24F *)Mem_GetAddr(p_mem_data, addr, &mem_err);
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return (NULL);
    }
    
    Core_Reset(p_core, p_err);

    if (*p_err != CORE_ERR_NONE) {
        return;
    }

    /* Write protection */
    
    Mem_SetAccess(p_mem_data, 0x0042, 0x01EF, &mem_err);        /* Protect SR SRF */
#ifdef WRITE_REPORT
#if 0
    p_out = fopen("/Users/aop007/Documents/Projets/DawnStar/Sipic/Sipic/Sipic/InputFiles/old_RA.txt", "wb");
#else
    p_out = fopen("new2_RA.txt", "wb");
#endif
#endif
    return (p_core);
}

void  Core_Reset(CORE_24F  *p_core,
                 CORE_ERR  *p_err)
{
    memset(p_core, 0, sizeof(CORE_24F));
    
    p_core->W[15] = 0x0800;
    
    *p_err = CORE_ERR_NONE;
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
    
    //EnableDebugPrintf = 1;
    
#if 0
    while (1) {
#endif
        
#if (RA_METHOD == RA_METHOD_OLD)
        pc     = Core_PC_Get(p_core_24f);
        opcode = Mem_Get24(p_mem_prog,
                           pc,
                          &mem_err);

        if (mem_err != MEM_ERR_NONE) {
             
            *p_err = CORE_ERR_ADDR_ERROR_TRAP;
            return;
        }
        
        if (((p_core_24f->SR & CORE_SR_RA) == CORE_SR_RA) &&
            (p_core_24f->RCOUNT > 0)) {
            p_core_24f->RCOUNT--;
            if (p_core_24f->RCOUNT == 0) {
                p_core_24f->SR &= ~(CORE_SR_RA);
            } else {
                Core_PC_Slide(p_core_24f, -2);
            }
        }
#elif (RA_METHOD == RA_METHOD_NEW)
        if ((p_core_24f->SR & CORE_SR_RA) == CORE_SR_RA) {
            
            if (p_core_24f->RCOUNT == 0) {
                p_core_24f->SR &= ~CORE_SR_RA;
            } else {
                p_core_24f->RCOUNT -= 1;
            }
        }
        
        pc = Core_PC_Get(p_core_24f);
        
        opcode = Mem_Get24(p_mem_prog,
                           pc,
                           &mem_err);
#else
#error RA_METHOD not specified
#endif
        
#if 0
        if (core_data.cycles == (0x0135653d - 100)) {
#ifdef WRITE_REPORT
            fflush(p_out);
#endif
            EnableDebugPrintf = 1;
        }
#endif
#if 0
        if ((Core_PC_Get(p_core_24f) == 0x0538)) {
            if (RCOUNT_1C06 < p_core_24f->RCOUNT) {
                uncaught_instructions *= 1;
                printf("Caught that motofoka.");
            }
            RCOUNT_1C06 = p_core_24f->RCOUNT;
        }
#endif

#if 0
        if ((Core_PC_Get(p_core_24f) == 0) && (core_data.cycles != 1)) {
            printf("Reset motofoka.");
#ifdef WRITE_REPORT
            fflush(p_out);
#endif
        }
#endif

#if 1
        if ((Core_PC_Get(p_core_24f) == 0x0A28)) { // && (p_core_24f->W[0] == 0)){
            //EnableDebugPrintf = 1;
            printf("");
            uncaught_instructions *= 1;
#ifdef WRITE_REPORT
            fflush(p_out);
#endif
        }
#endif

#if 1
        if ((Core_PC_Get(p_core_24f) == 0x0A3A)) { // && (p_core_24f->W[0] == 0)){
            //EnableDebugPrintf = 1;
            printf("");
            uncaught_instructions *= 1;
#ifdef WRITE_REPORT
            fflush(p_out);
#endif
        }
#endif


#if 1
        if ((Core_PC_Get(p_core_24f) == 0x30AA)) { // && (p_core_24f->W[0] == 0)){
            printf("\r\nMainLoop");
            uncaught_instructions *= 1;
#ifdef WRITE_REPORT
            fflush(p_out);
#endif
            CORE_TRACE_DEBUG((""));
        }
#endif
#if 0  
        if (core_data.cycles == 177) { // && (p_core_24f->W[0] == 0)){
            printf("\r\n");
            uncaught_instructions *= 1;
            CORE_TRACE_DEBUG((""));
        }
#endif
        
#if 0
        if (opcode == 0x781FB0) {
            uncaught_instructions *= 10;
            CORE_TRACE_DEBUG(("\r\nNULL OPC \t %d", uncaught_instructions));
        }
#endif
            
#if 0
            printf("\t");
            
            for (ix = 0; ix < 16 ; ix++) {
                printf("%004x ", p_core_24f->W[ix]);
            }
#endif
        
#if 0
        CORE_TRACE_DEBUG(("\r\n"));
        for (ix = 0 ; ix < Call_Depth ; ix++) {
            CORE_TRACE_DEBUG(("-"));
        }
        CORE_TRACE_DEBUG(("PC = %004x\tOPC = %006x\t |", Core_PC_Get(p_core_24f), opcode));
#endif

#if 0
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
#if  (CORE_INTEGRITY_CHECK == DEF_ENABLED)
                    Core_PopCheckContext(p_core_24f);
#endif
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
            instruction = opcode & 0xFFF880;
            args        = opcode & 0x00077F;
            found_instruction = DEF_YES;
            
            switch (instruction) {
                case CORE_OPC_MOVD_BE0:
                    Core_MOVD_BE0(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
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
            instruction = opcode & 0xFF8070;
            args        = opcode & 0x007F8F;
            found_instruction = DEF_YES;
            
            switch (instruction) {
                case CORE_OPC_ASR_W_L_W:
                case CORE_OPC_LSR_W_L_W:
                case CORE_OPC_SL_W_L_W:
                    Core_Logical_Shift_W_L_W(p_mem_prog, p_mem_data, p_core_24f, args, instruction, p_err);
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
                case CORE_OPC_ADD_W_LIT_W:
                case CORE_OPC_ADDC_W_LIT_W:
                case CORE_OPC_AND_W_LIT_W:
                case CORE_OPC_SUB_W_LIT_W:
                case CORE_OPC_SUBB_W_LIT_W:
                case CORE_OPC_SUBR_W_LIT_W:
                case CORE_OPC_SUBBR_W_LIT_W:
                case CORE_OPC_IOR_W_LIT_W:
                case CORE_OPC_XOR_W_LIT_W:
                    
                    Core_Logical_W_L_W(p_mem_prog, p_mem_data, p_core_24f, args, instruction, p_err);
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
            instruction = opcode & 0xFF0780;
            args        = opcode & 0x00F87F;
            found_instruction = DEF_YES;
            
            switch (instruction) {
                case CORE_OPC_BTST_WS_WB:
                    Core_BTST(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
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
                case CORE_OPC_CPB_WB_WS:
                    Core_CP_WS_WB(p_mem_prog, p_mem_data, p_core_24f, args, instruction, p_err);
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
                case CORE_OPC_DIV_U:
                    Core_DIV_SU_D8000(p_mem_prog, p_mem_data, p_core_24f, args, instruction, p_err);
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
                case CORE_OPC_BCLR_M:
                case CORE_OPC_BTG_M:
#if 1
                    Core_BIT_Manip_M(p_mem_prog, p_mem_data, p_core_24f, args, instruction, p_err);
                    break;
#else
                    Core_BSET_M_A8(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                    
                case CORE_OPC_BCLR_M:
                    Core_BCLR_M_A9(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
#endif               
                case CORE_OPC_BTSS:
                    Core_BTSS_AE(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                case CORE_OPC_BTSC:
                    Core_BTSC_AF(p_mem_prog, p_mem_data, p_core_24f, args, p_err);
                    break;
                    
                    
                default:
                    found_instruction = DEF_NO;
                    break;
            }
        }
        
        if (found_instruction == DEF_NO) {
            instruction = opcode & 0xFF0001;
            args        = opcode & 0x00FFFE;
            found_instruction = DEF_YES;
            
            switch (instruction) {
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

                case CORE_OPC_MOV_WS_OFF_WD:
                case CORE_OPC_MOV_WS_WD_OFF:
                    Core_MOV_WS_WD_OFF(p_mem_prog, p_mem_data, p_core_24f, args, instruction, p_err);
                    break;
                    
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
    
#if 1
    if (EnableDebugPrintf == 1) {
        printf("\r\nPC = %004x\tOPC = %006x", pc, opcode);
        printf("\tRA = %d", p_core_24f->RCOUNT);
        printf("\tCY = %lu", core_data.cycles);
        printf("\tIPL = %d", Core_GetIPL(p_core_24f));
        printf("\tCD = %d", Call_Depth);
#ifdef WRITE_REPORT
        file_buffer_len = sprintf(file_buffer, "\r\nPC = %004x\tOPC = %006x", Core_PC_Get(p_core_24f), opcode);
        fwrite(file_buffer, 1, file_buffer_len, p_out);
        file_buffer_len = sprintf(file_buffer, "\tRA = %d", p_core_24f->RCOUNT);
        fwrite(file_buffer, 1, file_buffer_len, p_out);
        file_buffer_len = sprintf(file_buffer, "\tCY = %lu", core_data.cycles);
        fwrite(file_buffer, 1, file_buffer_len, p_out);
        file_buffer_len = sprintf(file_buffer, "\tIPL = %d", Core_GetIPL(p_core_24f));
        fwrite(file_buffer, 1, file_buffer_len, p_out);
        file_buffer_len = sprintf(file_buffer, "\tCD = %d", Call_Depth);
        fwrite(file_buffer, 1, file_buffer_len, p_out);
#endif
    }
#endif
        
#if 0
#if (RA_METHOD == RA_METHOD_NEW)
        if ((p_core_24f->SR & CORE_SR_RA) == CORE_SR_RA) {
            Core_PC_Slide(p_core_24f, -2);
        }
#endif
#endif

#if 1
        if (EnableDebugPrintf == 1) {
            printf("\t");
#ifdef WRITE_REPORT
            file_buffer_len = sprintf(file_buffer, "\t");
            fwrite(file_buffer, 1, file_buffer_len, p_out);
#endif
            for (ix = 0; ix < 16 ; ix++) {
                printf("%004x ", p_core_24f->W[ix]);
#ifdef WRITE_REPORT
                file_buffer_len = sprintf(file_buffer, "%004x ", p_core_24f->W[ix]);
                fwrite(file_buffer, 1, file_buffer_len, p_out);
#endif
            }
            
        }
#endif
        
        if ((found_instruction == DEF_NO) ||
            (*p_err            != CORE_ERR_NONE)) {
#ifdef  CORE_CFG_CYCLE_CNTR
            CORE_TRACE_INFO(("\r\nINSTRUCTION NOT FOUND. %X at %X \t@%ld cycles.", opcode, pc, p_core_24f->CYCLE));
#else
            CORE_TRACE_INFO(("\r\nINSTRUCTION Error %d. %X at %X.",*p_err, opcode, pc));
#endif
            return;
        }
        
        for (ix = 0; ix < 16 ; ix++) {
            CORE_TRACE_DEBUG(("\t%004x", p_core_24f->W[ix]));
        }
 
#if 0
        if (is_call) {
            CPU_INT32U  PC = Core_PC_Get(p_core_24f);
            printf("\r\n0x%x", PC);
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
 
#if 1
    if ((p_core->SR & CORE_SR_RA) == 0) {
#endif
        memcpy(&PC, (const void *)&p_core->PC[0], sizeof(PC));
    
        PC += slide;
    
        memcpy((void *)&p_core->PC[0], &PC, sizeof(PC));
#if 1
    }
#endif
}

void  Core_PC_Set (CORE_24F    *p_core,
                   CPU_INT32U   value)
{
    CPU_INT32U  PC;
    
    memcpy(&PC, (const void *)&p_core->PC[0], sizeof(PC));
    
    //PC = value;
    
    memcpy((void *)&p_core->PC[0], &value, sizeof(PC));
    
    if (PC >= 0x4000) {
        printf("\r\nPC out of bound!");
    }
#ifndef DEBUG_FOR_PIZZA
    memcpy(&PC, (const void *)&p_core->PC[0], sizeof(PC));
#endif
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

CPU_INT08U  Core_GetIPL(CORE_24F  *p_core)
{
    CPU_INT32U  ipl;

    ipl = ((p_core->SR     & CORE_SR_IPL_MAKS) >> 5) |
           (p_core->CORCON & CORE_CORECON_IPL3);
    
    return (ipl);
}

void  Core_UpdateSRFlags(CORE_24F       *p_core,
                         CORE_SR_FLAGS   flags,
                         CORE_SR_DIR     direction,
                         CPU_INT32U      initial_val,
                         CPU_INT32U      final_val,
                         CPU_INT32U      size_op)
{
    if (flags & CORE_SR_C)  { Core_UpdateC (p_core, initial_val, final_val, direction, size_op); }
    if (flags & CORE_SR_Z)  { Core_UpdateZ (p_core,              final_val);                     }
    if (flags & CORE_SR_OV) { Core_UpdateOV(p_core, initial_val, final_val, direction, size_op); }
    if (flags & CORE_SR_N)  { Core_UpdateN (p_core,              final_val,            size_op); }
    if (flags & CORE_SR_DC) { Core_UpdateDC(p_core, initial_val, final_val, direction, size_op); }
}

void  Core_UpdateDC(CORE_24F       *p_core,
                    CPU_INT32U      initial_val,
                    CPU_INT32U      final_val,
                    CORE_SR_DIR     direction,
                    CPU_INT32U      size_op)
{
    /* Exemple:
    DS70157C-page 4-26
    DS70157C-page 5-124 
    DS70157C-page 5-125 
    DS70157C-page 5-126 
    ... 
    */
    CPU_BOOLEAN  condition1 = ((initial_val & 0x00000100) == 0) && ((final_val & 0x00000100) != 0);

    if (condition1) {
        if (direction == CORE_SR_DIR_UP) {
            p_core->SR |=   CORE_SR_DC;
        } else {
            p_core->SR &= ~(CORE_SR_DC);
        }
    }
}

void  Core_UpdateN(CORE_24F       *p_core,
                   CPU_INT32U      final_val,
                   CPU_INT32U      size_op)
{

    if (((final_val & 0x00008000) && (size_op == 0)) ||
        ((final_val & 0x00000080) && (size_op == 1))) {                
        p_core->SR |=   CORE_SR_N;
    } else {
        p_core->SR &= ~(CORE_SR_N);
    }
}

void  Core_UpdateOV(CORE_24F       *p_core,
                    CPU_INT32U      initial_val,
                    CPU_INT32U      final_val,
                    CORE_SR_DIR     direction,
                    CPU_INT32U      size_op)
{
    CPU_INT16S  initial_s;
    CPU_INT16S  final_s;


    initial_s = (CPU_INT16S)initial_val;
    final_s   = (CPU_INT16S)final_val;

    if (((final_s > initial_s) && (direction == CORE_SR_DIR_DOWN)) ||
        ((final_s < initial_s) && (direction == CORE_SR_DIR_UP))) {                
        p_core->SR |=   CORE_SR_OV;
    } else {
        p_core->SR &= ~(CORE_SR_OV);
    }
}

void  Core_UpdateC(CORE_24F       *p_core,
                   CPU_INT32U      initial_val,
                   CPU_INT32U      final_val,
                   CORE_SR_DIR     direction,
                   CPU_INT32U      size_op)
{
    CPU_BOOLEAN  condition1 = ((initial_val & 0x00010000) == 0) && ((final_val & 0x00010000) != 0) && (size_op == 0);
    CPU_BOOLEAN  condition2 = ((initial_val & 0x00000100) == 0) && ((final_val & 0x00000100) != 0) && (size_op == 1);

    if (condition1 || condition2) {
        if (direction == CORE_SR_DIR_UP) {
            p_core->SR |=   CORE_SR_C;
        } else {
            p_core->SR &= ~(CORE_SR_C);
        }
    } else {
        if (direction == CORE_SR_DIR_DOWN) {
            p_core->SR |=   CORE_SR_C;
        } else {
            p_core->SR &= ~(CORE_SR_C);
        }
    }
}

void  Core_UpdateZ(CORE_24F       *p_core,
                   CPU_INT32U      final_val)
{
    if (final_val == 0) {                
        p_core->SR |=   CORE_SR_Z;
    } else {
        p_core->SR &= ~(CORE_SR_Z);
    }
}

#if 0
void Core_InsertRA_OPC(CORE_DATA  *p_core_data,
                       OPCODE      opc)
{
    p_core_data->ra_opc = opc;
}
#endif

#if  (CORE_INTEGRITY_CHECK == DEF_ENABLED)
void Core_PushContext(CORE_24F  *p_core)
{
    CORE_CONTEXT  *p_core_to_push;
    
    
    p_core_to_push = malloc(sizeof(CORE_CONTEXT));
    
    if (p_core_to_push == NULL) {
        return;
    }
    
    memccpy(&p_core_to_push->context, p_core, 1, sizeof(CORE_24F));
    
    p_core_to_push->p_next   = core_data.p_context_head;
    core_data.p_context_head = p_core_to_push;
}
#endif

#if  (CORE_INTEGRITY_CHECK == DEF_ENABLED)
void Core_PopCheckContext(CORE_24F  *p_core)
{
    CORE_CONTEXT  *p_core_to_pop;
    CORE_24F      *p_core_pop;
    CPU_INT32S     ix;
    CPU_BOOLEAN    corruption;
    
    
    corruption               = DEF_NO;
    p_core_to_pop            = core_data.p_context_head;
    core_data.p_context_head = core_data.p_context_head->p_next;
    
    if (p_core_to_pop == NULL) {
        return;
    }
    
    p_core_pop = &p_core_to_pop->context;
    
    for (ix = 0 ; ix < 16 ; ix++) {
        if (p_core_pop->W[ix] != p_core->W[ix]) {
            //printf("W%d corrupted.", ix);
            corruption = DEF_YES;
        }
    }
    
    if (corruption == DEF_YES) {
        printf("\r\n");
        
        for (ix = 0; ix < 16 ; ix++) {
            printf("%004x ", p_core->W[ix]);
        }
        
        printf("\r\n");
        
        for (ix = 0; ix < 16 ; ix++) {
            printf("%004x ", p_core_pop->W[ix]);
        }
    }
    
    free(p_core_to_pop);
}
#endif

#if  (DIVISION_BYPASS == DEF_ENABLED)
/*
Inputs:
    W0 = Numerator   LSW
    W1 = Numerator   MSW
    W2 = Denominator LSW
    W3 = Denominator NSW

Outputs:
    W0 = Quotient    LSW
    W1 = Quotient    MSW
*/

void Core_DIV3232A (MEM_24      *p_mem_prog,
                    MEM         *p_mem_data,
                    CORE_24F    *p_core,
                    CORE_ERR    *p_err)
{
    CPU_INT32U  numerator;
    CPU_INT32U  denominator;
    CPU_INT32U  quotient;

    numerator    = (p_core->W[1] << 16) | p_core->W[0];
    denominator  = (p_core->W[3] << 16) | p_core->W[2];

    quotient     = numerator / denominator;

    p_core->W[1] = quotient >> 16;
    p_core->W[0] = quotient  & 0xFFFF;

    *p_err = CORE_ERR_NONE;
}
#endif