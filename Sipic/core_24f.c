//
//  core_24f.c
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-05.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "core_24f.h"
#include "core_24f_opcode.h"
#include "mem.h"
#include "cpu.h"

static  void  Core_Init(CORE_24F  *p_core)
{
    p_core->W[15] = 0x0800;
}

void  Core_Run(MEM         *p_mem,
               CORE_ERR    *p_err)
{
    CPU_BOOLEAN  found_instruction;
    CORE_24F     core_24f;
    OPCODE       opcode;
    OPCODE       instruction;
    OPCODE       args;
    MEM_ERR      mem_err;
    CORE_ERR     core_err;
    
    memset(&core_24f, 0, sizeof(core_24f));
    
    Core_Init(&core_24f);
    
    while (1) {
        printf("\r\nPC = 0x%X", core_24f.PC);
        
        
        opcode = Mem_Get(p_mem,
                         core_24f.PC,
                        &mem_err);
        
        instruction = opcode & 0xF00000;
        args        = opcode & 0x0FFFFF;
        found_instruction = DEF_YES;
        
        switch (instruction) {
            case CORE_OPC_MOV_L_W:
                Core_MOV_2(&core_24f,
                           args,
                           &core_err);
                break;
                
                
            default:
                found_instruction = DEF_NO;
                break;
        }
        
        if (found_instruction == DEF_NO) {
            instruction = opcode & 0xF80000;
            args        = opcode & 0x07FFFF;
            found_instruction = DEF_YES;
            
            switch (instruction) {
                case CORE_OPC_MOV_WS_WD:
                    Core_MOV_WS_WD_78(p_mem,
                                      &core_24f,
                                      args,
                                      &core_err);
                    break;
                    
                    
                case CORE_OPC_MOV_M_W:
                    Core_MOV_M_W_80(p_mem,
                                   &core_24f,
                                    args,
                                   &core_err);
                    break;
                    
                    
                case CORE_OPC_MOV_W_M:
                    Core_MOV_W_M_88(p_mem,
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
            instruction = opcode & 0xFF0000;
            args        = opcode & 0x00FFFF;
            found_instruction = DEF_YES;
            
            switch (instruction) {
                case CORE_OPC_NOP:
                    Core_NOP_00(p_mem, &core_24f, args, &core_err);
                    break;
                    
#if 0
                case CORE_OPC_BRA:
                    break;
#endif

                case CORE_OPC_CALL:
                    Core_CALL_02(p_mem, &core_24f, args, &core_err);
                    break;

                    
                case CORE_OPC_GOTO:
                    Core_GOTO_04(p_mem, &core_24f, args, &core_err);
                    break;
                    

                case CORE_OPC_BRA_EXPR:
                    Core_BRA_37(p_mem, &core_24f, args, &core_err);
                    break;

                    
                case CORE_OPC_BSET_W:
                    Core_BSET_W_A0(&core_24f, args, &core_err);
                    break;
                    
                    
                case CORE_OPC_BSET_M:
                    Core_BSET_M_A8(p_mem, &core_24f, args, &core_err);
                    break;
                    
                    
                case CORE_OPC_BCLR_M:
                    Core_BCLR_M_A9(p_mem, &core_24f, args, &core_err);
                    break;
                    
                    
                case CORE_OPC_BTSC:
                    Core_BTSC_AF(p_mem, &core_24f, args, &core_err);
                    break;
                    
                case CORE_OPC_PUSH_F8:
                    Core_PUSH_F8(p_mem, &core_24f, args, &core_err);
                    break;
                    
                case CORE_OPC_POP_F9:
                    Core_POP_F9(p_mem, &core_24f, args, &core_err);
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
                case CORE_OPC_SETM_WS:
                    Core_SETM_WS_EB8(p_mem, &core_24f, args, &core_err);
                    break;
                
                    
                case CORE_OPC_SETM_M_W0:
                    Core_SETM_M_W0_EF8(p_mem, &core_24f, args, &core_err);
                    break;
                    
                    
                case CORE_OPC_CLR_M_W0:
                    Core_CLR_M_W0_EF0(p_mem, &core_24f, args, &core_err);
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
                    Core_SETM_MOV_8BL_WN_B7A(p_mem, &core_24f, args, &core_err);
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
                    Core_SETM_MOV_8BL_WN_B3C(p_mem, &core_24f, args, &core_err);
                    break;
                    
                    
                default:
                    found_instruction = DEF_NO;
                    break;
            }
        }
        
        if (found_instruction == DEF_NO) {
            instruction = opcode & 0xFFFFFF;
            args        = opcode & 0x000000;
            found_instruction = DEF_YES;
            
            switch (instruction) {
                case CORE_OPC_RETURN:
                    Core_RETURN_060(p_mem, &core_24f, args, &core_err);
                    break;
                    
                    
                default:
                    found_instruction = DEF_NO;
                    break;
            }
        }
        
        if (found_instruction == DEF_NO) {
            core_err = CORE_ERR_OPC_NOTFOUND;
            printf("\r\nINSTRUCTION NOT FOUND. %X at %X", opcode, core_24f.PC);
            break;
        }
        
        
        if (core_err != CORE_ERR_NONE) {
            break;
        }
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














