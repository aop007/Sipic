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

#define NO_VERBOSE 1u

static  void  Core_Init(CORE_24F  *p_core)
{
    p_core->W[15] = 0x0800;
    p_core->CYCLE = 0;
}

void  Core_Run(MEM       *p_mem_prog,
               MEM       *p_mem_data,
               CORE_ERR  *p_err)
{
    CPU_BOOLEAN  found_instruction;
    CPU_BOOLEAN  is_call;
    CORE_24F     core_24f;
    OPCODE       opcode;
    OPCODE       instruction;
    OPCODE       args;
    CPU_INT32U   ix;
    CPU_INT32U   uncaught_instructions;
    MEM_ERR      mem_err;
    CORE_ERR     core_err;
    
    
    uncaught_instructions = 0;
    memset(&core_24f, 0, sizeof(core_24f));
    
    Core_Init(&core_24f);
    
    while (1) {
        opcode = Mem_Get(p_mem_prog,
                         core_24f.PC,
                        &mem_err);

        if (opcode == 0) {
            printf("\r\nNULL OPC \t %d", uncaught_instructions);
            
        }

        if ((core_24f.SR & CORE_SR_RA) == CORE_SR_RA) {
            core_24f.RCOUNT--;
            if (core_24f.RCOUNT == 0) {
                core_24f.SR &= ~(CORE_SR_RA);
            } else {
                core_24f.PC -= 2;
            }
        }
#ifndef  NO_VERBOSE
        printf("\r\nPC = %004x\tOPC = %006x\tCYCLE = %d |", core_24f.PC, opcode, (CPU_INT32U)core_24f.CYCLE);
#endif
#if 0
        if (core_24f.PC == 0x0966) {
            printf("Here comes INVALID_MEM");
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
                    Core_RETURN_060(p_mem_prog, p_mem_data, &core_24f, args, &core_err);
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
                    Core_CP0_E0000(p_mem_prog, p_mem_data, &core_24f, args, &core_err);
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
                    Core_CLR_WD_EB0000(p_mem_prog, p_mem_data, &core_24f, args, &core_err);
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
                    Core_MUL_UU_B8006(p_mem_prog, p_mem_data, &core_24f, args, &core_err);
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
                    Core_SUB_50006(p_mem_prog, p_mem_data, &core_24f, args, &core_err);
                    break;
                    
                    
                case CORE_OPC_ADDC_WB_LIT_WD:
                    Core_ADDC_48006(p_mem_prog, p_mem_data, &core_24f, args, &core_err);
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
                    Core_BTSC_W(p_mem_prog, p_mem_data, &core_24f, args, &core_err);
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
                    Core_CP_E1006(p_mem_prog, p_mem_data, &core_24f, args, &core_err);
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
                    Core_SE_FB00(p_mem_prog, p_mem_data, &core_24f, args, &core_err);
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
                    Core_SETM_MOV_8BL_WN_B3C(p_mem_prog, p_mem_data, &core_24f, args, &core_err);
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
                    Core_REPEAT_LIT14(p_mem_prog, p_mem_data, &core_24f, args, &core_err);
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
                    Core_DIV_S_D8000(p_mem_prog, p_mem_data, &core_24f, args, &core_err);
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
                case CORE_OPC_SUB_WN_LIT:
                    Core_SUB_B10(p_mem_prog, p_mem_data, &core_24f, args, &core_err);
                    break;
                    
                case CORE_OPC_ADD_B40:
                    Core_ADD_B40(p_mem_prog, p_mem_data, &core_24f, args, &core_err);
                    break;
                    
                case CORE_OPC_MUL_US:
                    Core_MUL_SS_B88(p_mem_prog, p_mem_data, &core_24f, args, &core_err);
                    break;
                    
                case CORE_OPC_MUL_SS:
                    Core_MUL_SS_B98(p_mem_prog, p_mem_data, &core_24f, args, &core_err);
                    break;
                    
                case CORE_OPC_MOV_M_WM:
                    Core_MOV_BF8(p_mem_prog, p_mem_data, &core_24f, args, &core_err);
                    break;
                    
                case CORE_OPC_SETM_WS:
                    Core_SETM_WS_EB8(p_mem_prog, p_mem_data, &core_24f, args, &core_err);
                    break;
                
                case CORE_OPC_INC_EC0:
                    Core_INC_EC0(p_mem_prog, p_mem_data, &core_24f, args, &core_err);
                    break;
                    
                case CORE_OPC_SETM_M_W0:
                    Core_SETM_M_W0_EF8(p_mem_prog, p_mem_data, &core_24f, args, &core_err);
                    break;
                    
                    
                case CORE_OPC_CLR_M_W0:
                    Core_CLR_M_W0_EF0(p_mem_prog, p_mem_data, &core_24f, args, &core_err);
                    break;
                    
                case CORE_OPC_ADDC_M_W:
                    Core_ADDC_B48(p_mem_prog, p_mem_data, &core_24f, args, &core_err);
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
                    Core_SETM_MOV_8BL_WN_B7A(p_mem_prog, p_mem_data, &core_24f, args, &core_err);
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
                    Core_NOP_00(p_mem_prog, p_mem_data, &core_24f, args, &core_err);
                    break;
                    

                case CORE_OPC_BRA_LE_EXPR:
                    Core_BRA_34(p_mem_prog, p_mem_data, &core_24f, args, &core_err);
                    break;
                    
                case CORE_OPC_BRA_NZ_EXPR:
                    Core_BRA_3A(p_mem_prog, p_mem_data, &core_24f, args, &core_err);
                    break;

                    
                case CORE_OPC_CALL:
                    Core_CALL_02(p_mem_prog, p_mem_data, &core_24f, args, &core_err);
                    is_call = DEF_YES;
                    break;
                    
                    
                case CORE_OPC_GOTO:
                    Core_GOTO_04(p_mem_prog, p_mem_data, &core_24f, args, &core_err);
                    break;
                    
                    
                case CORE_OPC_BRA_EXPR:
                    Core_BRA_37(p_mem_prog, p_mem_data, &core_24f, args, &core_err);
                    break;
                    
                    
                case CORE_OPC_BSET_W:
                    Core_BSET_W_A0(p_mem_prog, p_mem_data, &core_24f, args, &core_err);
                    break;
                    
                    
                case CORE_OPC_BSET_M:
                    Core_BSET_M_A8(p_mem_prog, p_mem_data, &core_24f, args, &core_err);
                    break;
                    
                    
                case CORE_OPC_BCLR_M:
                    Core_BCLR_M_A9(p_mem_prog, p_mem_data, &core_24f, args, &core_err);
                    break;
                    
                    
                case CORE_OPC_BTSC:
                    Core_BTSC_AF(p_mem_prog, p_mem_data, &core_24f, args, &core_err);
                    break;
                    
                case CORE_OPC_PUSH_F8:
                    Core_PUSH_F8(p_mem_prog, p_mem_data, &core_24f, args, &core_err);
                    break;
                    
                case CORE_OPC_POP_F9:
                    Core_POP_F9(p_mem_prog, p_mem_data, &core_24f, args, &core_err);
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
                case CORE_OPC_ADD_WB_WS:
                    Core_MATH_WS_WD(p_mem_prog,
                                    p_mem_data,
                                    &core_24f,
                                    args,
                                    CORE_MATH_OP_ADD,
                                    &core_err);
                    break;
                    
                case CORE_OPC_SUB_WB_WS:
                    Core_MATH_WS_WD(p_mem_prog,
                                    p_mem_data,
                                    &core_24f,
                                    args,
                                    CORE_MATH_OP_SUB,
                                    &core_err);
                    break;
                    
                case CORE_OPC_MOV_WS_WD:
                    Core_MOV_WS_WD_78(p_mem_prog,
                                      p_mem_data,
                                      &core_24f,
                                      args,
                                      &core_err);
                    break;
                    
                    
                case CORE_OPC_MOV_M_W:
                    Core_MOV_M_W_80(p_mem_prog,
                                    p_mem_data,
                                    &core_24f,
                                    args,
                                    &core_err);
                    break;
                    
                    
                case CORE_OPC_MOV_W_M:
                    Core_MOV_W_M_88(p_mem_prog,
                                    p_mem_data,
                                    &core_24f,
                                    args,
                                    &core_err);
                    break;
                    
                case CORE_OPC_ADDC_WB_WS_WD:
                    Core_ADDC_48(p_mem_prog, p_mem_data, &core_24f, args, &core_err);
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
                    Core_MOV_2(p_mem_prog,
                               p_mem_data,
                              &core_24f,
                               args,
                              &core_err);
                    break;
                
                
                default:
                    found_instruction = DEF_NO;
                    break;
            }
        }

        
        if (found_instruction == DEF_NO) {
#if 0
            core_err = CORE_ERR_OPC_NOTFOUND;
            printf("\r\nINSTRUCTION NOT FOUND. %X at %X", opcode, core_24f.PC);
            break;
#else
            uncaught_instructions++;
            core_24f.PC += 2;
            core_err = CORE_ERR_NONE;
            printf("\r\nPC = %004x\tOPC = %006x\tCYCLE = %d |", core_24f.PC, opcode, (CPU_INT32U)core_24f.CYCLE);
#endif
            
        }
#ifndef  NO_VERBOSE
        for (ix = 0; ix < 16 ; ix++) {
            printf("\t%004x", core_24f.W[ix]);
        }
        
        if (is_call) {
            printf("\r\n");
        }
#endif
        if (core_err != CORE_ERR_NONE) {
            break;
        }

#ifdef  CORE_CFG_CYCLE_CNTR
        core_24f.CYCLE++;
#endif
    }
    
    printf("\r\nFATAL CORE ERROR: %d", core_err);
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

CPU_INT08U  Core_GetCarry (CORE_24F  *p_core)
{
    if ((p_core->SR & CORE_SR_C) == 0) {
        return (0);
    } else {
        return (1);
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










