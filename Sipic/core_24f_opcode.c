//
//  core_24f_opcode.c
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-06.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core_24f_opcode.h"



void  Core_OPC_Stats (MEM  *p_mem)
{
    MEM_HDR     *p_mem_hdr;
    CPU_INT16U   opc_ctr[256];
    CPU_INT32U   mem_size;
    OPCODE       opc;
    OPCODE      *p_opc;
    CPU_INT32U   ix;
    
    
    memset(&opc_ctr[0], 0, sizeof(opc_ctr));
    
    p_mem_hdr = &p_mem->Hdr;
    
    while (p_mem_hdr->Next->Hdr.Index != 0) {
        
        mem_size = p_mem_hdr->Size;
        p_opc    = p_mem->Ptr;
        
        for (ix = 0 ; ix < p_mem_hdr->Size ; ix++) {
            opc = (*p_opc & 0x00FF0000) >> 16;
            opc_ctr[opc]++;
            p_opc++;
        }
        
        p_mem     =  p_mem_hdr->Next;
        p_mem_hdr = &p_mem->Hdr;
    }
    
    printf("\r\nOPC STATS:");
    
    for (ix = 0 ; ix < sizeof(opc_ctr) / sizeof (CPU_INT16U) ; ix++) {
        if (ix % 16 == 0) {
            printf("\r\n");
        }
#if 0
        printf("\r\n%#x = %d", ix, opc_ctr[ix]);
#else
        printf("%d\t", opc_ctr[ix]);
#endif
    }
}

void  Core_NOP_00 (MEM         *p_mem_prom,
                   MEM         *p_mem_data,
                   CORE_24F    *p_core,
                   CPU_INT32U   args,
                   CORE_ERR    *p_err)
{
    p_core->PC += 2;
    
    *p_err = CORE_ERR_NONE;
}

void  Core_CALL_02            (MEM         *p_mem_prog,
                               MEM         *p_mem_data,
                               CORE_24F    *p_core,
                               CPU_INT32U   args,
                               CORE_ERR    *p_err)
{
    OPCODE   next_word;
    MEM_ERR  mem_err;
    
    p_core->PC += 2;
    
    next_word   = Mem_Get(p_mem_prog, p_core->PC, &mem_err);
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    p_core->PC += 2;
    
    Core_Push( p_core->PC & 0x00FFFF, p_core, p_mem_data, p_err);
    
    if (*p_err != CORE_ERR_NONE) {
        return;
    }
    
    Core_Push((p_core->PC & 0xFF0000) >> 16, p_core, p_mem_data, p_err);

    if (*p_err != CORE_ERR_NONE) {
        return;
    }
    
    p_core->PC = (next_word & 0x00007F) << 16 |
                 (args      & 0x00FFFE);
    
    *p_err = CORE_ERR_NONE;
}

void  Core_GOTO_04 (MEM         *p_mem_prog,
                    MEM         *p_mem_data,
                    CORE_24F    *p_core,
                    CPU_INT32U   args,
                    CORE_ERR    *p_err)
{
    MEM_ERR     mem_err;
    OPCODE      next_word;
    CPU_INT32U  next_PC;
    
    next_word = Mem_Get(p_mem_prog, p_core->PC + 2, &mem_err);
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
            
    next_PC   = ((next_word & 0x00007F) << 16) | (args & 0x00FFFE);
    
    p_core->PC = next_PC;
    
    *p_err = CORE_ERR_NONE;
}

void Core_RETURN_060  (MEM         *p_mem_prog,
                       MEM         *p_mem_data,
                       CORE_24F    *p_core,
                       CPU_INT32U   args,
                       CORE_ERR    *p_err)
{
    CPU_INT32U  word1;
    CPU_INT32U  word2;
    
    word1 = Core_Pop(p_core, p_mem_data, p_err);
    
    if (*p_err != CORE_ERR_NONE) {
        return;
    }
    
    word2 = Core_Pop(p_core, p_mem_data, p_err);
    
    if (*p_err != CORE_ERR_NONE) {
        return;
    }
    
    p_core->PC = word1 << 16 | word2;
    
    *p_err = CORE_ERR_NONE;
}

void  Core_MOV_2 (MEM         *p_mem_prog,
                  MEM         *p_mem_data,
                  CORE_24F    *p_core,
                  CPU_INT32U   args,
                  CORE_ERR    *p_err)
{
    CPU_INT32U  literal;
    CPU_INT32U  w_reg;
    
    
    literal = (args & 0x0FFFF0) >> 4;
    w_reg   =  args & 0x00000F;
    
    p_core->W[w_reg] = literal;
    
    p_core->PC += 2;
    
    *p_err = CORE_ERR_NONE;
}

void Core_BRA_37       (MEM         *p_mem_prog,
                        MEM         *p_mem_data,
                        CORE_24F    *p_core,
                        CPU_INT32U   args,
                        CORE_ERR    *p_err)
{
    CPU_INT16U     slit;
    CPU_INT16S  *p_slit;
    CPU_INT32U   addr;
    
    slit = args;
    
    p_slit = (CPU_INT16S *)&slit;
    
    addr   = p_core->PC + 2 + 2 * (*p_slit);
    
    p_core->PC = addr;
    
    *p_err = CORE_ERR_NONE;
}

void Core_MATH_WS_WD   (MEM          *p_mem_prog,
                        MEM          *p_mem_data,
                        CORE_24F     *p_core,
                        CPU_INT32U    args,
                        CORE_MATH_OP  math_op,
                        CORE_ERR     *p_err)
{
    CPU_INT32U  base_w_addr;                                    /* wwww */
    CPU_INT32U  size_op;                                        /* B    */
    CPU_INT32U  dst_addr_mode;                                  /* qqq  */
    CPU_INT32U  dst_addr;                                       /* dddd */
    CPU_INT32U  src_addr_mode;                                  /* ppp  */
    CPU_INT32U  src_addr;                                       /* ssss */
    CPU_INT32S  operand_1;
    CPU_INT32S  operand_2;
    CPU_INT32S  result;
    CPU_INT32U  value_mask;
    CPU_INT32S  addr_offset;
    CPU_INT32S  value_temp;
    MEM_ERR     mem_err;
    
    
    base_w_addr   = (args & 0x078000) >> 15;
    size_op       = (args & 0x004000) >> 12;
    dst_addr_mode = (args & 0x003800) >> 11;
    dst_addr      = (args & 0x000780) >>  7;
    src_addr_mode = (args & 0x000070) >>  4;
    src_addr      =  args & 0x00000F;
    
    if (size_op == 0u) {
        addr_offset = 2;
        value_mask  = 0xFFFF;
    } else {
        addr_offset = 1;
        value_mask  = 0x00FF;
        *p_err = CORE_ERR_OPC_UNSUPORTED_YET;
        return;
    }
    
    operand_1 = CPU_SignExt16(p_core->W[base_w_addr]);

    switch (src_addr_mode) {
        case CORE_OPC_ADDR_MODE_DIR:
            operand_2 = p_core->W[src_addr] & value_mask;
            break;
            
        case CORE_OPC_ADDR_MODE_IND:
            operand_2  = Mem_Get(p_mem_data, (p_core->W[src_addr] & 0xFFFE), &mem_err);
            
            if (mem_err != MEM_ERR_NONE) {
                *p_err = CORE_ERR_INVALID_MEM;
                return;
            }
            
            if ((size_op != 0) &&
                ((p_core->W[src_addr] & 0x0001) == 0x0001)) {
                operand_2 >>= 8;
            }
            
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_DEC:
            operand_2 = Mem_Get(p_mem_data, (p_core->W[src_addr] & 0xFFFE), &mem_err);
            
            if (mem_err != MEM_ERR_NONE) {
                *p_err = CORE_ERR_INVALID_MEM;
                return;
            }
            
            if ((size_op != 0) &&
                ((p_core->W[src_addr] & 0x0001) == 0x0001)) {
                operand_2 >>= 8;
            }
            
            p_core->W[src_addr] -= addr_offset;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_INC:
            operand_2 = Mem_Get(p_mem_data, (p_core->W[src_addr] & 0xFFFE), &mem_err);
            
            if (mem_err != MEM_ERR_NONE) {
                *p_err = CORE_ERR_INVALID_MEM;
                return;
            }
            
            if ((size_op != 0) &&
                ((p_core->W[src_addr] & 0x0001) == 0x0001)) {
                operand_2 >>= 8;
            }
            
            p_core->W[src_addr] += addr_offset;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_DEC:
            p_core->W[src_addr] -= addr_offset;
            
            operand_2 = Mem_Get(p_mem_data, (p_core->W[src_addr] & 0xFFFE), &mem_err);
            
            if (mem_err != MEM_ERR_NONE) {
                *p_err = CORE_ERR_INVALID_MEM;
                return;
            }
            
            if ((size_op != 0) &&
                ((p_core->W[src_addr] & 0x0001) == 0x0001)) {
                operand_2 >>= 8;
            }
            
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_INC:
            p_core->W[src_addr] += addr_offset;
            
            operand_2 = Mem_Get(p_mem_data, (p_core->W[src_addr] & 0xFFFE), &mem_err);
            
            if (mem_err != MEM_ERR_NONE) {
                *p_err = CORE_ERR_INVALID_MEM;
                return;
            }
            
            if ((size_op != 0) &&
                ((p_core->W[src_addr] & 0x0001) == 0x0001)) {
                operand_2 >>= 8;
            }
            
            break;
            
        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return;
    }

    switch (math_op) {
        case CORE_MATH_OP_ADD:
            result = operand_1 + operand_2;
            break;
            
        case CORE_MATH_OP_SUB:
            result = operand_1 - operand_2;
            break;
            
        case CORE_MATH_OP_MUL:
            result = operand_1 * operand_2;
            break;
            
        case CORE_MATH_OP_DIV:
            result = operand_1 / operand_2;
            break;
            
        default:
            *p_err = CORE_ERR_INVALID_MATH_OP;
            return;
    }
    
    
    switch (dst_addr_mode) {
        case CORE_OPC_ADDR_MODE_DIR:
            p_core->W[dst_addr] &= ~(value_mask);
            p_core->W[dst_addr] |=   result;
            break;
            
        case CORE_OPC_ADDR_MODE_IND:
            if (size_op == 0) {
                Mem_Set(p_mem_data, p_core->W[dst_addr], result, &mem_err);
                
                if (mem_err != MEM_ERR_NONE) {
                    *p_err = CORE_ERR_INVALID_MEM;
                    return;
                }
                
            } else {
                value_temp = Mem_Get(p_mem_data, (p_core->W[dst_addr] & 0xFFFE), &mem_err);
                
                if (mem_err != MEM_ERR_NONE) {
                    *p_err = CORE_ERR_INVALID_MEM;
                    return;
                }
                
                if ((p_core->W[dst_addr] & 0x0001) == 0x0001) {
                    value_temp &= 0x00FF;
                    value_temp |= (result << 8);
                } else {
                    value_temp &= 0xFF00;
                    value_temp |= result;
                }
                Mem_Set(p_mem_data, (p_core->W[dst_addr] & 0xFFFE), value_temp, &mem_err);
                
                if (mem_err != MEM_ERR_NONE) {
                    *p_err = CORE_ERR_INVALID_MEM;
                    return;
                }
            }
            
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_DEC:
            if (size_op == 0) {
                Mem_Set(p_mem_data, p_core->W[dst_addr], result, &mem_err);
                
                if (mem_err != MEM_ERR_NONE) {
                    *p_err = CORE_ERR_INVALID_MEM;
                    return;
                }
                
            } else {
                value_temp = Mem_Get(p_mem_data, (p_core->W[dst_addr] & 0xFFFE), &mem_err);
                
                if (mem_err != MEM_ERR_NONE) {
                    *p_err = CORE_ERR_INVALID_MEM;
                    return;
                }
                
                if ((p_core->W[dst_addr] & 0x0001) == 0x0001) {
                    value_temp &= 0x00FF;
                    value_temp |= (result << 8);
                } else {
                    value_temp &= 0xFF00;
                    value_temp |= result;
                }
                Mem_Set(p_mem_data, (p_core->W[dst_addr] & 0xFFFE), value_temp, &mem_err);
                
                if (mem_err != MEM_ERR_NONE) {
                    *p_err = CORE_ERR_INVALID_MEM;
                    return;
                }
            }
            
            p_core->W[dst_addr] -= addr_offset;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_INC:
            if (size_op == 0) {
                Mem_Set(p_mem_data, p_core->W[dst_addr], result, &mem_err);
                
                if (mem_err != MEM_ERR_NONE) {
                    *p_err = CORE_ERR_INVALID_MEM;
                    return;
                }
                
            } else {
                value_temp = Mem_Get(p_mem_data, (p_core->W[dst_addr] & 0xFFFE), &mem_err);
                
                if (mem_err != MEM_ERR_NONE) {
                    *p_err = CORE_ERR_INVALID_MEM;
                    return;
                }
                
                if ((p_core->W[dst_addr] & 0x0001) == 0x0001) {
                    value_temp &= 0x00FF;
                    value_temp |= (result << 8);
                } else {
                    value_temp &= 0xFF00;
                    value_temp |= result;
                }
                Mem_Set(p_mem_data, (p_core->W[dst_addr] & 0xFFFE), value_temp, &mem_err);
                
                if (mem_err != MEM_ERR_NONE) {
                    *p_err = CORE_ERR_INVALID_MEM;
                    return;
                }
            }
            
            p_core->W[dst_addr] += addr_offset;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_DEC:
            p_core->W[dst_addr] -= addr_offset;
            
            if (size_op == 0) {
                Mem_Set(p_mem_data, p_core->W[dst_addr], result, &mem_err);
                
                if (mem_err != MEM_ERR_NONE) {
                    *p_err = CORE_ERR_INVALID_MEM;
                    return;
                }
                
            } else {
                value_temp = Mem_Get(p_mem_data, (p_core->W[dst_addr] & 0xFFFE), &mem_err);
                
                if (mem_err != MEM_ERR_NONE) {
                    *p_err = CORE_ERR_INVALID_MEM;
                    return;
                }
                
                if ((p_core->W[dst_addr] & 0x0001) == 0x0001) {
                    value_temp &= 0x00FF;
                    value_temp |= (result << 8);
                } else {
                    value_temp &= 0xFF00;
                    value_temp |= result;
                }
                Mem_Set(p_mem_data, (p_core->W[dst_addr] & 0xFFFE), value_temp, &mem_err);
                
                if (mem_err != MEM_ERR_NONE) {
                    *p_err = CORE_ERR_INVALID_MEM;
                    return;
                }
            }
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_INC:
            p_core->W[dst_addr] += addr_offset;
            
            if (size_op == 0) {
                Mem_Set(p_mem_data, p_core->W[dst_addr], result, &mem_err);
                
                if (mem_err != MEM_ERR_NONE) {
                    *p_err = CORE_ERR_INVALID_MEM;
                    return;
                }
                
            } else {
                value_temp = Mem_Get(p_mem_data, (p_core->W[dst_addr] & 0xFFFE), &mem_err);
                
                if (mem_err != MEM_ERR_NONE) {
                    *p_err = CORE_ERR_INVALID_MEM;
                    return;
                }
                
                if ((p_core->W[dst_addr] & 0x0001) == 0x0001) {
                    value_temp &= 0x00FF;
                    value_temp |= (result << 8);
                } else {
                    value_temp &= 0xFF00;
                    value_temp |= result;
                }
                Mem_Set(p_mem_data, (p_core->W[dst_addr] & 0xFFFE), value_temp, &mem_err);
                
                if (mem_err != MEM_ERR_NONE) {
                    *p_err = CORE_ERR_INVALID_MEM;
                    return;
                }
            }
            break;
            
        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return;
    }
    
    p_core->PC += 2;
    *p_err = CORE_ERR_NONE;    
}

void Core_SUB_50006 (MEM         *p_mem_prog,
                     MEM         *p_mem_data,
                     CORE_24F    *p_core,
                     CPU_INT32U   args,
                     CORE_ERR    *p_err)
{
    CPU_INT32U  base_w;
    CPU_INT32U  size_op;
    CPU_INT32U  addr_mode;
    CPU_INT32U  dest_w;
    CPU_INT32U  lit_op;
    CPU_INT32S  operand1;
    CPU_INT32S  result;
    
    base_w    = (args & 0x078000) >> 15;
    size_op   = (args & 0x004000) >> 14;
    addr_mode = (args & 0x003800) >> 11;
    dest_w    = (args & 0x000780) >>  7;
    lit_op    =  args & 0x00001F;
    
    if (size_op != 0) {
        *p_err = CORE_ERR_OPC_UNSUPORTED_YET;
        return;
    }
    
    operand1 = CPU_SignExt16(p_core->W[base_w]);
    
    result   = operand1 - lit_op;
    
    switch (addr_mode) {
            
    }
    
    *p_err = CORE_ERR_OPC_UNSUPORTED_YET;
}

void Core_MOV_WS_WD_78 (MEM         *p_mem_prog,
                        MEM         *p_mem_data,
                        CORE_24F    *p_core,
                        CPU_INT32U   args,
                        CORE_ERR    *p_err)
{
    CPU_INT32U  off_w;
    CPU_INT32U  size_op;
    CPU_INT32U  src_addr_mode;
    CPU_INT32U  dst_addr_mode;
    CPU_INT32U  src_w;
    CPU_INT32U  dst_w;
    CPU_INT32U  value;
    CPU_INT32U  value_mask;
    CPU_INT32U  value_temp;
    CPU_INT08S  addr_offset;
    MEM_ERR     mem_err;
    
    
    
    off_w         = (args & 0x078000) >> 15;
    size_op       =  args & 0x004000;
    dst_addr_mode = (args & 0x003800) >> 11;
    dst_w         = (args & 0x000780) >>  7;
    src_addr_mode = (args & 0x000070) >>  4;
    src_w         =  args & 0x00000F;
    
    if (size_op == 0u) {
        addr_offset = 2;
        value_mask  = 0xFFFF;
    } else {
        addr_offset = 1;
        value_mask  = 0x00FF;
        //*p_err = CORE_ERR_OPC_UNSUPORTED_YET;
        //return;
    }
    
    switch (src_addr_mode) {
        case CORE_OPC_ADDR_MODE_DIR:
            value = p_core->W[src_w] & value_mask;
            break;
            
        case CORE_OPC_ADDR_MODE_IND:
            value  = Mem_Get(p_mem_data, (p_core->W[src_w] & 0xFFFE), &mem_err);
            
            if (mem_err != MEM_ERR_NONE) {
                *p_err = CORE_ERR_INVALID_MEM;
                return;
            }
            
            if ((size_op != 0) &&
                ((p_core->W[src_w] & 0x0001) == 0x0001)) {
                value >>= 8;
            }
            
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_DEC:
            value = Mem_Get(p_mem_data, (p_core->W[src_w] & 0xFFFE), &mem_err);
            
            if (mem_err != MEM_ERR_NONE) {
                *p_err = CORE_ERR_INVALID_MEM;
                return;
            }
            
            if ((size_op != 0) &&
                ((p_core->W[src_w] & 0x0001) == 0x0001)) {
                value >>= 8;
            }
            
            p_core->W[src_w] -= addr_offset;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_INC:
            value = Mem_Get(p_mem_data, (p_core->W[src_w] & 0xFFFE), &mem_err);
            
            if (mem_err != MEM_ERR_NONE) {
                *p_err = CORE_ERR_INVALID_MEM;
                return;
            }
            
            if ((size_op != 0) &&
                ((p_core->W[src_w] & 0x0001) == 0x0001)) {
                value >>= 8;
            }
            
            p_core->W[src_w] += addr_offset;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_DEC:
            p_core->W[src_w] -= addr_offset;
            
            value = Mem_Get(p_mem_data, (p_core->W[src_w] & 0xFFFE), &mem_err);
            
            if (mem_err != MEM_ERR_NONE) {
                *p_err = CORE_ERR_INVALID_MEM;
                return;
            }
            
            if ((size_op != 0) &&
                ((p_core->W[src_w] & 0x0001) == 0x0001)) {
                value >>= 8;
            }
            
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_INC:
            p_core->W[src_w] += addr_offset;
            
            value = Mem_Get(p_mem_data, (p_core->W[src_w] & 0xFFFE), &mem_err);
            
            if (mem_err != MEM_ERR_NONE) {
                *p_err = CORE_ERR_INVALID_MEM;
                return;
            }
            
            if ((size_op != 0) &&
                ((p_core->W[src_w] & 0x0001) == 0x0001)) {
                value >>= 8;
            }
            
            break;
            
        case CORE_OPC_ADDR_MODE_OFFSET:
            value = Mem_Get(p_mem_data, ((p_core->W[src_w] + p_core->W[off_w]) & 0xFFFE), &mem_err);
            
            if (mem_err != MEM_ERR_NONE) {
                *p_err = CORE_ERR_INVALID_MEM;
                return;
            }
            
            if ((size_op != 0) &&
                ((p_core->W[src_w] + p_core->W[off_w]) == 0x0001)) {
                value >>= 8;
            }
            
            break;
            
        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return;
    }
    
    value &= value_mask;
    
    switch (dst_addr_mode) {
        case CORE_OPC_ADDR_MODE_DIR:
                p_core->W[dst_w] &= ~(value_mask);
                p_core->W[dst_w] |=   value;            
            break;
            
        case CORE_OPC_ADDR_MODE_IND:
            if (size_op == 0) {
                Mem_Set(p_mem_data, p_core->W[dst_w], value, &mem_err);
                
                if (mem_err != MEM_ERR_NONE) {
                    *p_err = CORE_ERR_INVALID_MEM;
                    return;
                }
                
            } else {
                value_temp = Mem_Get(p_mem_data, (p_core->W[dst_w] & 0xFFFE), &mem_err);
                
                if (mem_err != MEM_ERR_NONE) {
                    *p_err = CORE_ERR_INVALID_MEM;
                    return;
                }
                
                if ((p_core->W[dst_w] & 0x0001) == 0x0001) {
                    value_temp &= 0x00FF;
                    value_temp |= (value << 8);
                } else {
                    value_temp &= 0xFF00;
                    value_temp |= value;
                }
                Mem_Set(p_mem_data, (p_core->W[dst_w] & 0xFFFE), value_temp, &mem_err);
                
                if (mem_err != MEM_ERR_NONE) {
                    *p_err = CORE_ERR_INVALID_MEM;
                    return;
                }
            }
            
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_DEC:
            if (size_op == 0) {
                Mem_Set(p_mem_data, p_core->W[dst_w], value, &mem_err);
                
                if (mem_err != MEM_ERR_NONE) {
                    *p_err = CORE_ERR_INVALID_MEM;
                    return;
                }
                
            } else {
                value_temp = Mem_Get(p_mem_data, (p_core->W[dst_w] & 0xFFFE), &mem_err);
                
                if (mem_err != MEM_ERR_NONE) {
                    *p_err = CORE_ERR_INVALID_MEM;
                    return;
                }
                
                if ((p_core->W[dst_w] & 0x0001) == 0x0001) {
                    value_temp &= 0x00FF;
                    value_temp |= (value << 8);
                } else {
                    value_temp &= 0xFF00;
                    value_temp |= value;
                }
                Mem_Set(p_mem_data, (p_core->W[dst_w] & 0xFFFE), value_temp, &mem_err);
                
                if (mem_err != MEM_ERR_NONE) {
                    *p_err = CORE_ERR_INVALID_MEM;
                    return;
                }
            }
            
            p_core->W[dst_w] -= addr_offset;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_INC:
            if (size_op == 0) {
                Mem_Set(p_mem_data, p_core->W[dst_w], value, &mem_err);
                
                if (mem_err != MEM_ERR_NONE) {
                    *p_err = CORE_ERR_INVALID_MEM;
                    return;
                }
                
            } else {
                value_temp = Mem_Get(p_mem_data, (p_core->W[dst_w] & 0xFFFE), &mem_err);
                
                if (mem_err != MEM_ERR_NONE) {
                    *p_err = CORE_ERR_INVALID_MEM;
                    return;
                }
                
                if ((p_core->W[dst_w] & 0x0001) == 0x0001) {
                    value_temp &= 0x00FF;
                    value_temp |= (value << 8);
                } else {
                    value_temp &= 0xFF00;
                    value_temp |= value;
                }
                Mem_Set(p_mem_data, (p_core->W[dst_w] & 0xFFFE), value_temp, &mem_err);
                
                if (mem_err != MEM_ERR_NONE) {
                    *p_err = CORE_ERR_INVALID_MEM;
                    return;
                }
            }
            
            p_core->W[dst_w] += addr_offset;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_DEC:
            p_core->W[dst_w] -= addr_offset;
            
            if (size_op == 0) {
                Mem_Set(p_mem_data, p_core->W[dst_w], value, &mem_err);
                
                if (mem_err != MEM_ERR_NONE) {
                    *p_err = CORE_ERR_INVALID_MEM;
                    return;
                }
                
            } else {
                value_temp = Mem_Get(p_mem_data, (p_core->W[dst_w] & 0xFFFE), &mem_err);
                
                if (mem_err != MEM_ERR_NONE) {
                    *p_err = CORE_ERR_INVALID_MEM;
                    return;
                }
                
                if ((p_core->W[dst_w] & 0x0001) == 0x0001) {
                    value_temp &= 0x00FF;
                    value_temp |= (value << 8);
                } else {
                    value_temp &= 0xFF00;
                    value_temp |= value;
                }
                Mem_Set(p_mem_data, (p_core->W[dst_w] & 0xFFFE), value_temp, &mem_err);
                
                if (mem_err != MEM_ERR_NONE) {
                    *p_err = CORE_ERR_INVALID_MEM;
                    return;
                }
            }
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_INC:
            p_core->W[dst_w] += addr_offset;
            
            if (size_op == 0) {
                Mem_Set(p_mem_data, p_core->W[dst_w], value, &mem_err);
                
                if (mem_err != MEM_ERR_NONE) {
                    *p_err = CORE_ERR_INVALID_MEM;
                    return;
                }
                
            } else {
                value_temp = Mem_Get(p_mem_data, (p_core->W[dst_w] & 0xFFFE), &mem_err);
                
                if (mem_err != MEM_ERR_NONE) {
                    *p_err = CORE_ERR_INVALID_MEM;
                    return;
                }
                
                if ((p_core->W[dst_w] & 0x0001) == 0x0001) {
                    value_temp &= 0x00FF;
                    value_temp |= (value << 8);
                } else {
                    value_temp &= 0xFF00;
                    value_temp |= value;
                }
                Mem_Set(p_mem_data, (p_core->W[dst_w] & 0xFFFE), value_temp, &mem_err);
                
                if (mem_err != MEM_ERR_NONE) {
                    *p_err = CORE_ERR_INVALID_MEM;
                    return;
                }
            }
            break;
            
        case CORE_OPC_ADDR_MODE_OFFSET:
            
            if (size_op == 0) {
                Mem_Set(p_mem_data, p_core->W[dst_w] + p_core->W[off_w], value, &mem_err);
                
                if (mem_err != MEM_ERR_NONE) {
                    *p_err = CORE_ERR_INVALID_MEM;
                    return;
                }
                
            } else {
                value_temp = Mem_Get(p_mem_data, ((p_core->W[dst_w] + p_core->W[off_w]) & 0xFFFE), &mem_err);
                
                if (mem_err != MEM_ERR_NONE) {
                    *p_err = CORE_ERR_INVALID_MEM;
                    return;
                }
                
                if (((p_core->W[dst_w] + p_core->W[off_w]) & 0x0001) == 0x0001) {
                    value_temp &= 0xFF00;
                    value_temp |= (value << 8);
                } else {
                    value_temp &= 0x00FF;
                    value_temp |= value;
                }
                Mem_Set(p_mem_data, ((p_core->W[dst_w] + p_core->W[off_w]) & 0xFFFE), value_temp, &mem_err);
                
                if (mem_err != MEM_ERR_NONE) {
                    *p_err = CORE_ERR_INVALID_MEM;
                    return;
                }
            }
            break;
            
        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return;
    }
    
    p_core->PC += 2;
    
    *p_err = CORE_ERR_NONE;
}

void  Core_MOV_M_W_80 (MEM         *p_mem_prog,
                       MEM         *p_mem_data,
                       CORE_24F    *p_core,
                       CPU_INT32U   args,
                       CORE_ERR    *p_err)
{
    CPU_INT32U  addr;
    CPU_INT32U  value;
    CPU_INT32U  w_reg;
    MEM_ERR     mem_err;

    addr  = (args & 0x07FFF0) >> 3;
    w_reg = (args & 0x00000F);
    
    value = Mem_Get(p_mem_data, addr, &mem_err);
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    p_core->W[w_reg] = value;
    
    p_core->PC += 2;
    
    *p_err = CORE_ERR_NONE;
}

void  Core_MOV_W_M_88 (MEM         *p_mem_prog,
                       MEM         *p_mem_data,
                       CORE_24F    *p_core,
                       CPU_INT32U   args,
                       CORE_ERR    *p_err)
{
    CPU_INT32U  addr;
    CPU_INT32U  value;
    CPU_INT32U  w_reg;
    MEM_ERR     mem_err;
    
    addr  = (args & 0x07FFF0) >> 3;
    w_reg = (args & 0x00000F);
    
    value = p_core->W[w_reg];

    Mem_Set(p_mem_data, addr, value, &mem_err);
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    p_core->PC += 2;
    
    *p_err = CORE_ERR_NONE;
}


void  Core_BSET_W_A0 (MEM         *p_mem_prog,
                      MEM         *p_mem_data,
                      CORE_24F    *p_core,
                      CPU_INT32U   args,
                      CORE_ERR    *p_err)
{
    printf("\r\nCore_BSET_W_A0 Unimplemented");
    *p_err = CORE_ERR_OPC_UNSUPORTED_YET;
}

void  Core_BSET_M_A8 (MEM         *p_mem_prog,
                      MEM         *p_mem_data,
                      CORE_24F    *p_core,
                      CPU_INT32U   args,
                      CORE_ERR    *p_err)
{
    CPU_INT32U  addr;
    CPU_INT16U  bit;
    MEM_ERR     mem_err;
    CPU_INT32U  value;
    
    bit  = ((args & 0x00E000) >> 12) |
            (args & 0x000001);
    
    addr =  (args & 0x001FFE) >> 1;
    
    value = Mem_Get(p_mem_data, addr, &mem_err);
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    value |= (1 << bit);
    
    Mem_Set(p_mem_data, addr, value, &mem_err);
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    p_core->PC += 2;
    
    *p_err = CORE_ERR_NONE;
}

void  Core_BCLR_M_A9 (MEM         *p_mem_prog,
                      MEM         *p_mem_data,
                      CORE_24F    *p_core,
                      CPU_INT32U   args,
                      CORE_ERR    *p_err)
{
    CPU_INT32U  value;
    CPU_INT32U  addr;
    CPU_INT32U  bit;
    MEM_ERR     mem_err;
    
    addr  = (args & 0x001FFE) >>  1;
    bit   = (args & 0x00E000) >> 12 |
            (args & 0x000001);
    
    value = Mem_Get(p_mem_data, addr, &mem_err);
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }

    value &= ~(1 << bit);
    
    Mem_Set(p_mem_data, addr, value, &mem_err);
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    p_core->PC += 2;
    
    *p_err = CORE_ERR_NONE;
}

void  Core_BTSC_AF (MEM         *p_mem_prog,
                    MEM         *p_mem_data,
                    CORE_24F    *p_core,
                    CPU_INT32U   args,
                    CORE_ERR    *p_err)
{
    CPU_INT32U  value;
    CPU_INT32U  addr;
    CPU_INT32U  bit;
    CPU_INT32U  opc_words;
    CPU_INT32U  size_op;
    OPCODE      opc;
    MEM_ERR     mem_err;
    
    addr    =  args & 0x001FFE;
    size_op =  args & 0x000001;
    bit     =  1 << ((args & 0x00E000) >> 12);
    
    if (size_op == 1) {
        bit <<= 8;
    }
    
    value   = Mem_Get(p_mem_data, addr, &mem_err);
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    if ((value & bit) == 0u) {
        
        
        opc = Mem_Get(p_mem_prog, p_core->PC, &mem_err);
        
        if (mem_err != MEM_ERR_NONE) {
            *p_err = CORE_ERR_INVALID_MEM;
            return;
        }
#if 1
        opc_words = Core_OPC_Words(opc);
        
        switch (opc_words) {
            case 1:
            case 2:
                p_core->PC += 2 * opc_words;
                break;
                
            default:
                *p_err = CORE_ERR_INVALID_OPC_CYCLE;
                return;
        }
#else
        p_core->PC += 2;
        *p_err = CORE_ERR_OPC_UNSUPORTED_YET;
        return;
#endif
    }
    
    p_core->PC += 2;
    
    *p_err = CORE_ERR_NONE;
}

void Core_SETM_MOV_8BL_WN_B3C (MEM         *p_mem_prog,
                               MEM         *p_mem_data,
                               CORE_24F    *p_core,
                               CPU_INT32U   args,
                               CORE_ERR    *p_err)
{
    CPU_INT32U  value;
    CPU_INT32U  w_reg;
    
    
    value = args & 0x000FF0;
    w_reg = args & 0x00000F;
    
    p_core->W[w_reg] &= 0xFF00;
    p_core->W[w_reg] |= value;
    
    p_core->PC += 2;
    *p_err = CORE_ERR_NONE;
}

void Core_SETM_MOV_8BL_WN_B7A (MEM         *p_mem_prog,
                               MEM         *p_mem_data,
                               CORE_24F    *p_core,
                               CPU_INT32U   args,
                               CORE_ERR    *p_err)
{
    CPU_INT32U  size_op;
    CPU_INT32U  addr;
    CPU_INT32U  val;
    CPU_INT32U  reg_val;
    CPU_INT32U  mask;
    MEM_ERR     mem_err;
    
    size_op = args & 0x004000;
    addr    = args & 0x001FFF;
    
    if (size_op == 0u) {
        val = p_core->W[0] & 0xFFFF;
    } else {
        val = p_core->W[0] & 0x00FF;
    }
    
    
    reg_val = Mem_Get(p_mem_data, (addr & 0x001FFE), &mem_err);
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    if ((addr & 0x000001) == 1) {
        mask  = 0x00FF;
        val <<= 8;
        val  &= ~(mask);
    } else {
        mask  = 0xFFFF;
    }
    
    reg_val &= mask;
    reg_val |= val;
    
    Mem_Set(p_mem_data, (addr & 0x001FFE), reg_val, &mem_err);
    
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    
    p_core->PC += 2;
    *p_err = CORE_ERR_NONE;
}

void Core_ADD_B40     (MEM         *p_mem_prog,
                       MEM         *p_mem_data,
                       CORE_24F    *p_core,
                       CPU_INT32U   args,
                       CORE_ERR    *p_err)
{
    CPU_INT32U  size_op;
    CPU_INT32U  dest;
    CPU_INT32U  addr;
    CPU_INT32S  value_0;
    CPU_INT32S  value_1;
    CPU_INT32S  val_wreg;
    MEM_ERR     mem_err;
    
    
    size_op = args & 0x004000;
    dest    = args & 0x002000;
    addr    = args & 0x001FFF;
    
    if (size_op != 0) {
        *p_err = CORE_ERR_OPC_UNSUPORTED_YET;
        return;
    }
    
    value_0 = Mem_Get(p_mem_data, (addr & 0xFFFE), &mem_err);
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    value_0  = CPU_SignExt16(value_0);
    val_wreg = CPU_SignExt16(p_core->W[0]);
    
    value_1 = value_0 + val_wreg;
    
    if (dest == 0) {
        p_core->W[0] = value_1 & 0xFFFF;
    } else {
        Mem_Set(p_mem_data, (addr & 0xFFFE), (value_1 & 0xFFFF), &mem_err);
        
        if (mem_err != MEM_ERR_NONE) {
            *p_err = CORE_ERR_INVALID_MEM;
            return;
        }
    }
    
    /* Update Status Register */
    if (((value_0 & 0x00000080) &&                              /* DC */
         (value_1 & 0x00000080))) {
        p_core->SR |=   CORE_SR_DC;
    } else {
        p_core->SR &= ~(CORE_SR_DC);
    }
    
    if (value_1 < 0) {                                          /* N */
        p_core->SR |=   CORE_SR_N;
    } else {
        p_core->SR &= ~(CORE_SR_N);
    }
    
    if ((value_0 >= 0) &&                                       /* OV */
        (value_1 <  0)) {
        p_core->SR |=   CORE_SR_OV;
    } else {
        p_core->SR &= ~(CORE_SR_OV);
    }
    
    if (value_1 == 0) {                                         /* Z */
        p_core->SR |=   CORE_SR_Z;
    } else {
        p_core->SR &= ~(CORE_SR_Z);
    }
    
    if (((value_0 & 0x00008000) &&                              /* C */
         (value_1 & 0x00008000))) {
        p_core->SR |=   CORE_SR_C;
    } else {
        p_core->SR &= ~(CORE_SR_C);
    }
    
    p_core->PC += 2;
    *p_err = CORE_ERR_NONE;
}

void Core_MUL_SS_B98 (MEM         *p_mem_prog,
                      MEM         *p_mem_data,
                      CORE_24F    *p_core,
                      CPU_INT32U   args,
                      CORE_ERR    *p_err)
{
    CPU_INT32U  base_reg;
    CPU_INT32U  dest_reg;
    CPU_INT32U  addr_mode;
    CPU_INT32U  srce_reg;
    
    CPU_INT32S  operand0;
    CPU_INT32S  operand1;
    CPU_INT32S  result;
    
    MEM_ERR     mem_err;
    

    
    base_reg  = (args & 0x007800) >> 11;
    dest_reg  = (args & 0x000780) >>  7;
    addr_mode = (args & 0x000070) >>  4;
    srce_reg  =  args & 0x00000F;
    
    operand0 = CPU_SignExt16(p_core->W[base_reg]);
    
    switch (addr_mode) {
        case CORE_OPC_ADDR_MODE_DIR:
            operand1 = p_core->W[srce_reg];
            break;
            
        case CORE_OPC_ADDR_MODE_IND:
            operand1 = Mem_Get(p_mem_data, p_core->W[srce_reg], &mem_err);
            
            if (mem_err != MEM_ERR_NONE) {
                *p_err = CORE_ERR_INVALID_MEM;
            }
            break;            
            
        case CORE_OPC_ADDR_MODE_IND_POS_DEC:
            operand1 = Mem_Get(p_mem_data, p_core->W[srce_reg], &mem_err);
            
            if (mem_err != MEM_ERR_NONE) {
                *p_err = CORE_ERR_INVALID_MEM;
            }
            
            p_core->W[srce_reg] -= 2;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_INC:
            operand1 = Mem_Get(p_mem_data, p_core->W[srce_reg], &mem_err);
            
            if (mem_err != MEM_ERR_NONE) {
                *p_err = CORE_ERR_INVALID_MEM;
            }
            
            p_core->W[srce_reg] += 2;
            break;
        
        case CORE_OPC_ADDR_MODE_IND_PRE_DEC:
            p_core->W[srce_reg] -= 2;
            
            operand1 = Mem_Get(p_mem_data, p_core->W[srce_reg], &mem_err);
            
            if (mem_err != MEM_ERR_NONE) {
                *p_err = CORE_ERR_INVALID_MEM;
            }
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_INC:
            p_core->W[srce_reg] += 2;
            
            operand1 = Mem_Get(p_mem_data, p_core->W[srce_reg], &mem_err);
            
            if (mem_err != MEM_ERR_NONE) {
                *p_err = CORE_ERR_INVALID_MEM;
            }
            break;
            
        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return;
    }
    
    result = operand0 * operand1;
    
    p_core->W[dest_reg]     = (result & 0x0000FFFF);
    p_core->W[dest_reg + 1] = (result & 0xFFFF0000);
    p_core->PC += 2;
    
    *p_err = CORE_ERR_NONE;
}

void Core_MOV_BF8 (MEM         *p_mem_prog,
                   MEM         *p_mem_data,
                   CORE_24F    *p_core,
                   CPU_INT32U   args,
                   CORE_ERR    *p_err)
{
    CPU_INT32U  size_op;
    CPU_INT32U  dest;
    CPU_INT32U  addr;
    CPU_INT32S  value_0;
    CPU_INT32S  val_wreg;
    MEM_ERR     mem_err;
    
    
    size_op = args & 0x004000;
    dest    = args & 0x002000;
    addr    = args & 0x001FFF;
    
#if 0
    if (size_op != 0) {
        *p_err = CORE_ERR_OPC_UNSUPORTED_YET;
        return;
    }
#endif
    
    value_0 = Mem_Get(p_mem_data, (addr & 0xFFFE), &mem_err);
    value_0 = CPU_SignExt16(value_0);
    
    if (dest == 0) {
        if (size_op == 0) {
            p_core->W[0] = value_0;
        } else {
            val_wreg = p_core->W[0];
            
            if ((addr & 0x0001) == 0) {
                value_0 = (val_wreg & 0xFF00) | (value_0 & 0x00FF);
                value_0 = CPU_SignExt16(value_0);
                p_core->W[0] = value_0;
            } else {
                value_0 = (val_wreg & 0xFF00) | ((value_0 & 0xFF00) >> 8);
                value_0 = CPU_SignExt16(value_0);
                p_core->W[0] = value_0;
            }
        }
    } else {
        // Nothing to do, f -> f.
        // Just update N & Z flags.
    }
    
    if (value_0 < 0) {                                          /* N */
        p_core->SR |=   CORE_SR_N;
    } else {
        p_core->SR &= ~(CORE_SR_N);
    }
    
    if (value_0 == 0) {                                         /* Z */
        p_core->SR |=   CORE_SR_Z;
    } else {
        p_core->SR &= ~(CORE_SR_Z);
    }
    
    p_core->PC += 2;
    *p_err = CORE_ERR_NONE;
}

void Core_SETM_WS_EB8 (MEM         *p_mem_prog,
                       MEM         *p_mem_data,
                       CORE_24F    *p_core,
                       CPU_INT32U   args,
                       CORE_ERR    *p_err)
{
    printf("\r\nCore_SETM_WS_EB8 Unimplemented");
    *p_err = CORE_ERR_OPC_UNSUPORTED_YET;
}

void Core_INC_EC0 (MEM         *p_mem_prog,
                   MEM         *p_mem_data,
                   CORE_24F    *p_core,
                   CPU_INT32U   args,
                   CORE_ERR    *p_err)
{
    CPU_INT32U  addr;
    CPU_INT32U  size_op;
    CPU_INT32U  dest_sel;
    CPU_INT32S  value_0;
    CPU_INT32S  value_1;
    MEM_ERR     mem_err;
    
    addr     = args & 0x001FFF;
    size_op  = args & 0x004000;
    dest_sel = args & 0x002000;
    
    if (size_op != 0) {
        *p_err = CORE_ERR_OPC_UNSUPORTED_YET;
        return;
    }
    
    value_0 = Mem_Get(p_mem_data, addr, &mem_err);
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    value_0 = CPU_SignExt16(value_0);
    
    value_1 = value_0 + 1;
    
                                                                /* Update Status Register */
    if (((value_0 & 0x00000080) &&                              /* DC */
         (value_1 & 0x00000080))) {
        p_core->SR |=   CORE_SR_DC;
    } else {
        p_core->SR &= ~(CORE_SR_DC);
    }
    
    if (value_1 < 0) {                                          /* N */
        p_core->SR |=   CORE_SR_N;
    } else {
        p_core->SR &= ~(CORE_SR_N);
    }
    
    if ((value_0 >= 0) &&                                       /* OV */
        (value_1 <  0)) {
        p_core->SR |=   CORE_SR_OV;
    } else {
        p_core->SR &= ~(CORE_SR_OV);
    }

    if (value_1 == 0) {                                         /* Z */
        p_core->SR |=   CORE_SR_Z;
    } else {
        p_core->SR &= ~(CORE_SR_Z);
    }
        
    if (((value_0 & 0x00008000) &&                              /* C */
         (value_1 & 0x00008000))) {
        p_core->SR |=   CORE_SR_C;
    } else {
        p_core->SR &= ~(CORE_SR_C);
    }

    if (dest_sel == 0) {
        p_core->W[0] = value_1;
    } else {
        Mem_Set(p_mem_data, addr, value_1, &mem_err);
        
        if (mem_err != MEM_ERR_NONE) {
            *p_err = CORE_ERR_INVALID_MEM;
            return;
        }
    }
    
    p_core->PC += 2;
}


void Core_SETM_M_W0_EF8 (MEM         *p_mem_prog,
                         MEM         *p_mem_data,
                         CORE_24F    *p_core,
                         CPU_INT32U   args,
                         CORE_ERR    *p_err)
{
    CPU_INT32U  size_op;
    CPU_INT32U  dest;
    CPU_INT32U  addr;
    CPU_INT32U  val;
    CPU_INT32U  reg_val;
    MEM_ERR     mem_err;
    
    size_op = args & 0x004000;
    dest    = args & 0x002000;
    addr    = args & 0x001FFF;
    
    if (size_op == 0u) {
        val = 0xFFFF;
    } else {
        val = 0xFF;
    }
    
    if (dest == 0u) {
        p_core->W[0] |= val;
    } else {
        if ((DEF_FIELD_IS_SET(addr,    0x000001)  &&
             DEF_FIELD_IS_SET(size_op, 0x004000))) {
            
        }
        
        reg_val = Mem_Get(p_mem_data, (addr & 0x001FFE), &mem_err);
        
        if (mem_err != MEM_ERR_NONE) {
            *p_err = CORE_ERR_INVALID_MEM;
            return;
        }
        
        reg_val |= val;
        
        Mem_Set(p_mem_data, (addr & 0x001FFE), reg_val, &mem_err);

        
        if (mem_err != MEM_ERR_NONE) {
            *p_err = CORE_ERR_INVALID_MEM;
            return;
        }
    }
    
    p_core->PC += 2;
    *p_err = CORE_ERR_NONE;
}

void Core_CLR_M_W0_EF0 (MEM         *p_mem_prog,
                        MEM         *p_mem_data,
                        CORE_24F    *p_core,
                        CPU_INT32U   args,
                        CORE_ERR    *p_err)
{
    CPU_INT32U  size_op;
    CPU_INT32U  dest;
    CPU_INT32U  addr;
    CPU_INT32U  val;
    CPU_INT32U  reg_val;
    MEM_ERR     mem_err;
    
    size_op = args & 0x004000;
    dest    = args & 0x002000;
    addr    = args & 0x001FFF;
    
    if (size_op == 0u) {
        val = 0xFFFF;
    } else {
        val = 0xFF;
    }
    
    if (dest == 0u) {
        p_core->W[0] &= ~(val);
    } else {
        if ((DEF_FIELD_IS_SET(addr,    0x000001)  &&
             DEF_FIELD_IS_SET(size_op, 0x004000))) {
            
        }
        
        reg_val = Mem_Get(p_mem_data, (addr & 0x001FFE), &mem_err);
        
        if (mem_err != MEM_ERR_NONE) {
            *p_err = CORE_ERR_INVALID_MEM;
            return;
        }
        
        reg_val &= ~(val);
        
        Mem_Set(p_mem_data, (addr & 0x001FFE), reg_val, &mem_err);
        
        if (mem_err != MEM_ERR_NONE) {
            *p_err = CORE_ERR_INVALID_MEM;
            return;
        }
    }
    
    p_core->PC += 2;
    *p_err = CORE_ERR_NONE;
}

void  Core_PUSH_F8 (MEM         *p_mem_prog,
                    MEM         *p_mem_data,
                    CORE_24F    *p_core,
                    CPU_INT32U   args,
                    CORE_ERR    *p_err)
{
    CPU_INT32U  val;
    MEM_ERR     mem_err;
    
    val = Mem_Get(p_mem_data, args, &mem_err);
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    Core_Push(val, p_core, p_mem_data, p_err);
    
    if (*p_err != CORE_ERR_NONE) {
        return;
    }
    
    p_core->PC += 2;
}

void  Core_POP_F9 (MEM         *p_mem_prog,
                   MEM         *p_mem_data,
                   CORE_24F    *p_core,
                   CPU_INT32U   args,
                   CORE_ERR    *p_err)
{
    CPU_INT32U  val;
    MEM_ERR     mem_err;
    
    val = Core_Pop(p_core, p_mem_data, p_err);
    
    if (*p_err != CORE_ERR_NONE) {
        return;
    }
    
    Mem_Set(p_mem_data, args, val, &mem_err);
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    p_core->PC += 2;
}

void Core_SE_FB00       (MEM         *p_mem_prog,
                         MEM         *p_mem_data,
                         CORE_24F    *p_core,
                         CPU_INT32U   args,
                         CORE_ERR    *p_err)
{
    CPU_INT32U  w_dst;
    CPU_INT32U  w_src;
    CPU_INT32U  addr_mode;
    CPU_INT32S  value;
    MEM_ERR     mem_err;
    
    
    w_dst     = (args & 0x000780) >> 7;
    addr_mode = (args & 0x000070) >> 4;
    w_src     =  args & 0x00000F;
    
    mem_err = MEM_ERR_NONE;
    
    switch (addr_mode) {
        case CORE_OPC_ADDR_MODE_DIR:
            value = (p_core->W[w_src] & 0x00FF);
            break;
            
        case CORE_OPC_ADDR_MODE_IND:
            value = Mem_Get(p_mem_data, (p_core->W[w_src] & 0xFFFE), &mem_err);
            
            if ((p_core->W[w_src] & 0x0001) == 0) {
                value  &= 0x00FF;
            } else {
                value  &= 0xFF00;
                value >>= 8;
            }
            break;
                            
        case CORE_OPC_ADDR_MODE_IND_POS_DEC:
            value = Mem_Get(p_mem_data, (p_core->W[w_src] & 0xFFFE), &mem_err);
            
            if ((p_core->W[w_src] & 0x0001) == 0) {
                value  &= 0x00FF;
            } else {
                value  &= 0xFF00;
                value >>= 8;
            }
            
            p_core->W[w_src]--;
            
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_INC:
            value = Mem_Get(p_mem_data, (p_core->W[w_src] & 0xFFFE), &mem_err);
            
            if ((p_core->W[w_src] & 0x0001) == 0) {
                value  &= 0x00FF;
            } else {
                value  &= 0xFF00;
                value >>= 8;
            }
            
            p_core->W[w_src]++;
            
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_DEC:
            p_core->W[w_src]--;
            
            value = Mem_Get(p_mem_data, (p_core->W[w_src] & 0xFFFE), &mem_err);
            
            if ((p_core->W[w_src] & 0x0001) == 0) {
                value  &= 0x00FF;
            } else {
                value  &= 0xFF00;
                value >>= 8;
            }
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_INC:
            p_core->W[w_src]++;
            
            value = Mem_Get(p_mem_data, (p_core->W[w_src] & 0xFFFE), &mem_err);
            
            if ((p_core->W[w_src] & 0x0001) == 0) {
                value  &= 0x00FF;
            } else {
                value  &= 0xFF00;
                value >>= 8;
            }
            break;
            
        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return;
    }
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    value = CPU_SignExt08(value);
    
    p_core->W[w_dst] = value;
    
    if (value < 0) {                                            /* N & C */
        p_core->SR |=   CORE_SR_N;
        p_core->SR &= ~(CORE_SR_C);
    } else {
        p_core->SR &= ~(CORE_SR_N);
        p_core->SR |=   CORE_SR_C;
    }
    
    
    if (value == 0) {                                           /* Z */
        p_core->SR |=   CORE_SR_Z;
    } else {
        p_core->SR &= ~(CORE_SR_Z);
    }
    
    p_core->PC += 2;
    *p_err = CORE_ERR_NONE;
}





















