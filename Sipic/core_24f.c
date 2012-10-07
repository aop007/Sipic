//
//  core_24f.c
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-05.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#include <stdio.h>
#include "core_24f.h"
#include "core_24f_opcode.h"
#include "mem.h"

void  Core_Run(MEM         *p_mem,
               CPU_INT32U   start_addr,
               CORE_ERR    *p_err)
{
    CORE_24F    core_24f;
    OPCODE      opcode;
    OPCODE      instruction;
    OPCODE      args;
    MEM_ERR     mem_err;
    CORE_ERR    core_err;
    
    memset(&core_24f, 0, sizeof(core_24f));
    
    core_24f.PC = start_addr;
    
    while (1) {
        opcode = Mem_Get(p_mem,
                         core_24f.PC,
                        &mem_err);
        
        instruction = opcode & 0xF00000;
        args        = opcode & 0x0FFFFF;
        
        switch (instruction) {
            case CORE_OPC_MOV_L_W:
                Core_MOV_2(&core_24f,
                           args,
                           &core_err);
                break;
                
            case CORE_OPC_MOV_W_M:
                Core_MOV_8(p_mem,
                           &core_24f,
                           args,
                           &core_err);
                break;
                
            default:
                break;
        }
        
        instruction = opcode & 0xF80000;
        args        = opcode & 0x07FFFF;
        
        switch (instruction) {
            case CORE_OPC_MOV_M_W:
                Core_MOV_80(p_mem,
                            &core_24f,
                            args,
                            &core_err);
                break;
                
                
            case CORE_OPC_MOV_W_M:
                Core_MOV_88(p_mem,
                            &core_24f,
                            args,
                            &core_err);
                break;
                
            default:
                break;
        }
        
        instruction = opcode & 0xFF0000;
        args        = opcode & 0x00FFFF;
        
        switch (instruction) {
            case CORE_OPC_NOP:
                break;
                
            case CORE_OPC_BRA:
                break;
                
            case CORE_OPC_CALL:
                break;
                
            case CORE_OPC_GOTO:
                Core_GOTO_04(p_mem,
                            &core_24f,
                             args,
                            &core_err);
                             
                printf("\r\nGOTO.");
                break;
                
            default:
                break;
        }
        
        if (core_err != CORE_ERR_NONE) {
            break;
        }
    }
    
    printf("\r\nFATAL CORE ERROR.");
}