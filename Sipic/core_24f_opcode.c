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
#include "main.h"

CPU_INT32S  Call_Depth = 0;

void  Core_OPC_Stats (MEM_24  *p_mem)
{
    MEM_HDR_24  *p_mem_hdr;
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
    
    CORE_TRACE_DEBUG("\r\nOPC STATS:");
    
    for (ix = 0 ; ix < sizeof(opc_ctr) / sizeof (CPU_INT16U) ; ix++) {
        if (ix % 16 == 0) {
            CORE_TRACE_DEBUG("\r\n");
        }
#if 0
        CORE_TRACE_DEBUG("\r\n%#x = %d", ix, opc_ctr[ix]);
#else
        CORE_TRACE_DEBUG(("%d\t", opc_ctr[ix]));
#endif
    }
}

void  Core_NOP_00 (MEM_24         *p_mem_prog,
                   MEM         *p_mem_data,
                   CORE_24F    *p_core,
                   CPU_INT32U   args,
                   CORE_ERR    *p_err)
{
    Core_PC_Slide(p_core, 2);
    
    *p_err = CORE_ERR_NONE;
}

void  Core_CALL_02            (MEM_24      *p_mem_prog,
                               MEM         *p_mem_data,
                               CORE_24F    *p_core,
                               CPU_INT32U   args,
                               CORE_ERR    *p_err)
{
    CPU_INT32U  pc;
    OPCODE      next_word;
    MEM_ERR     mem_err;
    
    Core_PC_Slide(p_core, 2);
    
    next_word   = Mem_Get24(p_mem_prog, Core_PC_Get(p_core), &mem_err);
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    Core_PC_Slide(p_core, 2);

    pc = Core_PC_Get(p_core);
    
    Core_Push((pc  & 0x00FFFF), p_core, p_mem_data, p_err);
    
    if (*p_err != CORE_ERR_NONE) {
        return;
    }
    
    Core_Push(((pc & 0xFF0000) >> 16), p_core, p_mem_data, p_err);

    if (*p_err != CORE_ERR_NONE) {
        return;
    }
    
    pc = (next_word & 0x00007F) << 16 | (args & 0x00FFFE);
    
    if (pc >= 0x4000) {
        printf("\r\nPC out of bounds.");
    }
    
    Core_PC_Set(p_core, pc);
    
    Call_Depth++;
    
    *p_err = CORE_ERR_NONE;
}

void  Core_GOTO_04 (MEM_24      *p_mem_prog,
                    MEM         *p_mem_data,
                    CORE_24F    *p_core,
                    CPU_INT32U   args,
                    CORE_ERR    *p_err)
{
    MEM_ERR     mem_err;
    OPCODE      next_word;
    CPU_INT32U  next_PC;
    
    next_word = Mem_Get24(p_mem_prog, Core_PC_Get(p_core) + 2, &mem_err);
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
            
    next_PC   = ((next_word & 0x00007F) << 16) | (args & 0x00FFFE);
    
    Core_PC_Set(p_core,next_PC);
    
    *p_err = CORE_ERR_NONE;
}

void Core_RETURN_060  (MEM_24      *p_mem_prog,
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
    
    Core_PC_Set(p_core, word1 << 16 | word2);
    
    Call_Depth--;
    
    *p_err = CORE_ERR_NONE;
}

void Core_RETFIE_064 (MEM_24      *p_mem_prog,
                      MEM         *p_mem_data,
                      CORE_24F    *p_core,
                      CPU_INT32U   args,
                      CORE_ERR    *p_err)
{
    CPU_INT08U  SRL;
    CPU_INT08U  IPL3;
    CPU_INT32U  word1;
    CPU_INT32U  word2;
    CPU_INT32U  PC;
    
    word1 = Core_Pop(p_core, p_mem_data, p_err);
    
    if (*p_err != CORE_ERR_NONE) {
        return;
    }
    
    word2 = Core_Pop(p_core, p_mem_data, p_err);
    
    if (*p_err != CORE_ERR_NONE) {
        return;
    }
    
    PC = (word1 & 0x00FF) << 16 | word2;
    
    if (PC > 0x4000) {
        printf("\r\nPC out of bounds.");
    }
    
    Core_PC_Set(p_core, PC);
    
    SRL  = (word1 & 0xFF00) >> 8;
    IPL3 =  word1 & 0x00FF;
    
    p_core->SR     &= 0xFF00;
    p_core->SR     |= SRL;
    
    p_core->CORCON &= ~(CORE_CORECON_IPL3);
    p_core->CORCON |=   IPL3;
    
    Call_Depth--;
    
    *p_err = CORE_ERR_NONE;
}

void Core_REPEAT_LIT14(MEM_24      *p_mem_prog,
                       MEM         *p_mem_data,
                       CORE_24F    *p_core,
                       CPU_INT32U   args,
                       CORE_ERR    *p_err)
{
    CPU_INT32U  iterations;
    OPCODE      next_opc;
    CPU_INT32U  PC;
    MEM_ERR     mem_err;
    
    
    iterations = args & 0x003FFF;
    PC         = Core_PC_Get(p_core) + 2;
    next_opc   = Mem_Get24(p_mem_prog, PC, &mem_err);

#if 0
    Core_InsertRA_OPC(&core_data, next_opc);
#endif
    
    p_core->RCOUNT  = iterations;
    Core_PC_Slide(p_core, 4);
    p_core->SR     |= CORE_SR_RA;
    
    *p_err = CORE_ERR_NONE;
}

void  Core_MOV_2 (MEM_24      *p_mem_prog,
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
    
    Core_PC_Slide(p_core, 2);
    
    *p_err = CORE_ERR_NONE;
}

void Core_BRA_3 (MEM_24      *p_mem_prog,
                 MEM         *p_mem_data,
                 CORE_24F    *p_core,
                 CPU_INT32U   args,
                 CORE_ERR    *p_err)
{
    CPU_INT16U     slit;
    CPU_INT16S  *p_slit;
    CPU_INT32U   addr;
    CPU_INT32U   cond_sel;
    CPU_BOOLEAN  result;
    
    slit     =  args & 0x00FFFF;
    cond_sel = (args & 0x0F0000) >> 16;
    
    p_slit = (CPU_INT16S *)&slit;
    
    addr   = Core_PC_Get(p_core) + 2 + 2 * (*p_slit);
    
    switch (cond_sel) {
        case CORE_OPC_BRA_OV:
            result = Core_GetOV(p_core);
            break;
            
        case CORE_OPC_BRA_C:
            result = Core_GetC(p_core);
            break;
            
        case CORE_OPC_BRA_Z:
            result = Core_GetZ(p_core);
            break;
            
        case CORE_OPC_BRA_N:
            result = Core_GetN(p_core);
            break;
            
        case CORE_OPC_BRA_LE:
            result = Core_GetZ(p_core) || (Core_GetN(p_core) && !Core_GetOV(p_core)) || (!Core_GetN(p_core) && Core_GetOV(p_core));
            break;
            
        case CORE_OPC_BRA_LT:
            *p_err = CORE_ERR_OPC_UNSUPORTED_YET;
            return;
            
        case CORE_OPC_BRA_LEU:
            result = !Core_GetC(p_core)||Core_GetZ(p_core);
            break;
            
        case CORE_OPC_BRA_UNC:
            result = DEF_YES;
            break;
            
        case CORE_OPC_BRA_NOV:
            *p_err = CORE_ERR_OPC_UNSUPORTED_YET;
            return;
            
        case CORE_OPC_BRA_NC:
            result = !Core_GetC(p_core);
            break;
            
        case CORE_OPC_BRA_NZ:
            result = !Core_GetZ(p_core);
            break;
            
        case CORE_OPC_BRA_NN:
            *p_err = CORE_ERR_OPC_UNSUPORTED_YET;
            return;
            
        case CORE_OPC_BRA_GT:
            *p_err = CORE_ERR_OPC_UNSUPORTED_YET;
            return;
            
        case CORE_OPC_BRA_GE:
            result = !Core_GetN(p_core);
            break;
            
        case CORE_OPC_BRA_GTU:
            result = Core_GetC(p_core) && !Core_GetZ(p_core);
            break;
            
        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return; 
    }

    if (result) {
        Core_PC_Set(p_core, addr);
    } else {
        Core_PC_Slide(p_core, 2);
    }
    
    *p_err = CORE_ERR_NONE;
}

void Core_MATH_WN_LIT  (MEM_24       *p_mem_prog,
                        MEM          *p_mem_data,
                        CORE_24F     *p_core,
                        CPU_INT32U    args,
                        OPCODE        math_opc,
                        CORE_ERR     *p_err)
{
    CPU_INT32U  size_op;
    CPU_INT32U  lit;
    CPU_INT32U  w_reg;
    CPU_INT32S  result;
    CPU_INT32U  initial;
    CPU_INT32U  flags;
    CPU_INT32U  mask;

    size_op = (args & 0x004000) >> 14;
    lit     = (args & 0x003FF0) >>  4;
    w_reg   =  args & 0x00000F;

    if (size_op == 0) {
        mask = 0xFFFF;
    } else {
        mask = 0xFF;
    }

    initial = p_core->W[w_reg] & mask;

    switch (math_opc) {
        case CORE_OPC_ADD_LIT_W:
            result  = lit + initial;
            flags   = CORE_SR_DC | CORE_SR_N | CORE_SR_OV | CORE_SR_Z | CORE_SR_C;
            break;

        case CORE_OPC_ADDC_LIT_W:
            result  = lit + initial + Core_GetC(p_core);
            flags   = CORE_SR_DC | CORE_SR_N | CORE_SR_OV | CORE_SR_Z | CORE_SR_C;
            break;

        case CORE_OPC_SUB_W_LIT:
            result  = initial - lit;
            flags   = CORE_SR_DC | CORE_SR_N | CORE_SR_OV | CORE_SR_Z | CORE_SR_C;
            break;

        case CORE_OPC_AND_LIT_W:
            result  = initial & lit;
            flags   = CORE_SR_N | CORE_SR_Z;
            break;

        case CORE_OPC_IOR_LIT_W:
            result  = initial | lit;
            flags   = CORE_SR_N | CORE_SR_Z;
            break;

        case CORE_OPC_XOR_LIT_W:
            result  = initial ^ lit;
            flags   = CORE_SR_N | CORE_SR_Z;
            break;

        case CORE_OPC_SUBB_W_LIT:
            result  = initial - lit;
            if (!Core_GetC(p_core)) {
                result--;
            }
            flags   = CORE_SR_DC | CORE_SR_N | CORE_SR_OV | CORE_SR_Z | CORE_SR_C;
            break;

        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return;
    }

    p_core->W[w_reg] &= ~mask;
    p_core->W[w_reg] |= (result & mask);

    /* Update Status Register */
    if (flags & CORE_SR_DC) {
        if (((initial & 0x00000080) &&                              /* DC */
             (result  & 0x00000080))) {
            p_core->SR |=   CORE_SR_DC;
        } else {
            p_core->SR &= ~(CORE_SR_DC);
        }
    }
    
    if (flags & CORE_SR_N) {
        if (result < 0) {                                          /* N */
            p_core->SR |=   CORE_SR_N;
        } else {
            p_core->SR &= ~(CORE_SR_N);
        }
    }

    if (flags & CORE_SR_OV) {
        if (result > initial) {
            p_core->SR |=   CORE_SR_OV;
        } else {
            p_core->SR &= ~(CORE_SR_OV);
        }
    }
    
    if (flags & CORE_SR_Z) {
        if (result == 0) {                                         /* Z */
            p_core->SR |=   CORE_SR_Z;
        } else {
            p_core->SR &= ~(CORE_SR_Z);
        }
    }
    
    if (flags & CORE_SR_C) {
        if (((initial & 0x00008000) &&                              /* C */
             (result  & 0x00008000))) {
            p_core->SR |=   CORE_SR_C;
        } else {
            p_core->SR &= ~(CORE_SR_C);
        }
    }
    
    Core_PC_Slide(p_core, 2);
    *p_err = CORE_ERR_NONE;
}

void Core_MOV_WS_WD_OFF (MEM_24       *p_mem_prog,
                         MEM          *p_mem_data,
                         CORE_24F     *p_core,
                         CPU_INT32U    args,
                         OPCODE        instruction,
                         CORE_ERR     *p_err)
{
    CPU_INT16U   lit;
    CPU_INT16S   slit;
    CPU_INT32U   size_op;
    CPU_INT32U   dst_w;
    CPU_INT32U   src_w;
    CPU_INT32U   value;
    CPU_INT32U   addr;
    CPU_INT32U   mask;
    CPU_INT32U   value_original;
    MEM_ERR      mem_err;
    
    lit     = ((args & 0x078000) >>  9) |
              ((args & 0x003800) >>  8) |
              ((args & 0x000070) >>  4);
    size_op =  (args & 0x004000) >> 14;
    dst_w   =  (args & 0x000780) >>  7;
    src_w   =   args & 0x00000F;
    
    slit    = CPU_SignExt10(lit);
    
    switch (instruction) {
        case CORE_OPC_MOV_WS_OFF_WD:
            addr  = p_core->W[src_w] + slit;
            mask  = Core_MaskGet(size_op, addr);
            
            value = Mem_Get(p_mem_data, (addr & 0xFFFE), &mem_err);
            
            if (mem_err != MEM_ERR_NONE) {
                *p_err = CORE_ERR_INVALID_MEM;
                return;
            }
            
            value_original   = p_core->W[dst_w];
            value            = Core_Merge(value_original, value, mask);
            
            p_core->W[dst_w] = value;
            
            break;
            
        case CORE_OPC_MOV_WS_WD_OFF:
            
            value = p_core->W[src_w];
            
            addr  = p_core->W[dst_w] + slit;
            mask  = Core_MaskGet(size_op, addr);
            
            value_original   = Mem_Get(p_mem_data, (addr & 0xFFFE), &mem_err);
            
            if (mem_err != MEM_ERR_NONE) {
                *p_err = CORE_ERR_INVALID_MEM;
                return;
            }
            
            value            = Core_Merge(value_original, value, mask);
            
            Mem_Set(p_mem_data, (addr & 0xFFFE), value, &mem_err);
            
            if (mem_err != MEM_ERR_NONE) {
                *p_err = CORE_ERR_INVALID_MEM;
                return;
            }
            break;
            
        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return;
    }
    
    Core_PC_Slide(p_core, 2);
    *p_err = CORE_ERR_NONE;
}

void Core_MATH_WS_WD   (MEM_24       *p_mem_prog,
                        MEM          *p_mem_data,
                        CORE_24F     *p_core,
                        CPU_INT32U    args,
                        OPCODE        math_opc,
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
#if 0
        *p_err = CORE_ERR_OPC_UNSUPORTED_YET;
        return;
#endif
    }
    
    operand_1 = (CPU_SignExt16(p_core->W[base_w_addr]) & value_mask);

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
            
            if ((size_op                        != 0) &&
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
            
            if ((size_op                        != 0) &&
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
            
            if ((size_op                        != 0) &&
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
            
            if ((size_op                        != 0) &&
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
            
            if ((size_op                        != 0) &&
                ((p_core->W[src_addr] & 0x0001) == 0x0001)) {
                operand_2 >>= 8;
            }
            
            break;
            
        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return;
    }

    switch (math_opc) {
        case CORE_OPC_ADD_WB_WS_WD:
            result = operand_1 + operand_2;
            break;
            
        case CORE_OPC_ADDC_WB_WS_WD:
            result = operand_1 + operand_2 + Core_GetC(p_core);
            break;
            
        case CORE_OPC_SUB_WB_WS_WD:
            result = operand_1 - operand_2;
            break;

        case CORE_OPC_XOR_WB_WS_WD:
            result = operand_1 ^ operand_2;
            break;

        case CORE_OPC_IOR_WB_WS_WD:
            result = operand_1 | operand_2;
            break;

        case CORE_OPC_AND_WB_WS_WD:
            result = operand_1 & operand_2;
            break;

        case CORE_OPC_SUBB_WB_WS_WD:
            result = operand_1 - operand_2;
            if (!Core_GetC(p_core)) {
                result--;
            }
            break;
            
        case CORE_OPC_SUBBR_WB_WS_WD:
        case CORE_OPC_SUBR_WB_WS_WD:
            *p_err = CORE_ERR_OPC_UNSUPORTED_YET;
            return;
            
        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
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
    
    Core_PC_Slide(p_core, 2);
    *p_err = CORE_ERR_NONE;    
}

void Core_ADDC_48    (MEM_24      *p_mem_prog,
                      MEM         *p_mem_data,
                      CORE_24F    *p_core,
                      CPU_INT32U   args,
                      CORE_ERR    *p_err)
{
    CPU_INT32U  base_w;
    CPU_INT32U  size_op;
    CPU_INT32U  dest_addr_mode;
    CPU_INT32U  dest_w;
    CPU_INT32U  srce_addr_mode;
    CPU_INT32U  srce_w;
    CPU_INT32U  offset;
    CPU_INT32S  result;
    CPU_INT32U  operand1;
    MEM_ERR     mem_err;
    
    base_w          = (args & 0x078000) >> 15;
    size_op         = (args & 0x004000) >> 14;
    dest_addr_mode  = (args & 0x003800) >> 11;
    dest_w          = (args & 0x000780) >>  7;
    srce_addr_mode  = (args & 0x000070) >>  4;
    srce_w          =  args & 0x00000F;
    
    if (size_op != 0) {
        offset = 1;
        *p_err = CORE_ERR_OPC_UNSUPORTED_YET;
        return;
    } else {
        offset = 2;
    }
    
    mem_err = MEM_ERR_NONE;
    
    switch (srce_addr_mode) {
        case CORE_OPC_ADDR_MODE_DIR:
            operand1 = p_core->W[srce_w];
            break;
            
        case CORE_OPC_ADDR_MODE_IND:
            operand1 = Mem_Get(p_mem_data, p_core->W[srce_w], &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_DEC:
            operand1 = Mem_Get(p_mem_data, p_core->W[srce_w], &mem_err);
            p_core->W[srce_w] -= offset;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_INC:
            operand1 = Mem_Get(p_mem_data, p_core->W[srce_w], &mem_err);
            p_core->W[srce_w] += offset;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_DEC:
            p_core->W[srce_w] -= offset;
            operand1 = Mem_Get(p_mem_data, p_core->W[srce_w], &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_INC:
            p_core->W[srce_w] += offset;
            operand1 = Mem_Get(p_mem_data, p_core->W[srce_w], &mem_err);
            break;
            
        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return;
    }
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    result  = p_core->W[base_w] + operand1 + Core_GetC(p_core);
    mem_err = MEM_ERR_NONE;
    
    switch (dest_addr_mode) {
        case CORE_OPC_ADDR_MODE_DIR:
            p_core->W[dest_w] = result;
            break;
            
        case CORE_OPC_ADDR_MODE_IND:
            Mem_Set(p_mem_data, p_core->W[dest_w], result, &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_DEC:
            Mem_Set(p_mem_data, p_core->W[dest_w], result, &mem_err);
            p_core->W[dest_w] -= offset;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_INC:
            Mem_Set(p_mem_data, p_core->W[dest_w], result, &mem_err);
            p_core->W[dest_w] += offset;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_DEC:
            p_core->W[dest_w] -= offset;
            Mem_Set(p_mem_data, p_core->W[dest_w], result, &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_INC:
            p_core->W[dest_w] += offset;
            Mem_Set(p_mem_data, p_core->W[dest_w], result, &mem_err);
            break;
            
        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return;
    }
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    if (size_op == 0) {
        result = CPU_SignExt16(result);
    } else {
        *p_err = CORE_ERR_OPC_UNSUPORTED_YET;
        return;
    }
    
    /* Update Status Register */
    if (((operand1 & 0x00000080) &&                             /* DC */
         (result   & 0x00000080))) {
        p_core->SR |=   CORE_SR_DC;
    } else {
        p_core->SR &= ~(CORE_SR_DC);
    }
    
    if (result < 0) {                                           /* N */
        p_core->SR |=   CORE_SR_N;
    } else {
        p_core->SR &= ~(CORE_SR_N);
    }
    
    if ((p_core->W[base_w] >=  0) &&                            /* OV */
        (result            <   0)) {
        p_core->SR |=   CORE_SR_OV;
    } else {
        p_core->SR &= ~(CORE_SR_OV);
    }
    
    if (result == 0) {                                          /* Z */
        p_core->SR |=   CORE_SR_Z;
    } else {
        p_core->SR &= ~(CORE_SR_Z);
    }
    
    if (((operand1 & 0x00008000) &&                             /* C */
         (result   & 0x00008000))) {
        p_core->SR |=   CORE_SR_C;
    } else {
        p_core->SR &= ~(CORE_SR_C);
    }
    
    Core_PC_Slide(p_core, 2);
    
    *p_err = CORE_ERR_NONE;
    
}


void Core_ADDC_48006 (MEM_24      *p_mem_prog,
                      MEM         *p_mem_data,
                      CORE_24F    *p_core,
                      CPU_INT32U   args,
                      CORE_ERR    *p_err)
{
    CPU_INT32U  base_w;
    CPU_INT32U  size_op;
    CPU_INT32U  addr_mode;
    CPU_INT32U  dest_w;
    CPU_INT32U  operand1;
    CPU_INT32U  offset;
    CPU_INT32S  result;
    MEM_ERR     mem_err;
    
    base_w    = (args & 0x078000) >> 15;
    size_op   = (args & 0x004000) >> 14;
    addr_mode = (args & 0x003800) >> 11;
    dest_w    = (args & 0x000780) >>  7;
    operand1  =  args & 0x00001F;
    
    if (size_op != 0) {
        offset = 1;
        *p_err = CORE_ERR_OPC_UNSUPORTED_YET;
        return;
    } else {
        offset = 2;
    }
    
    result  = p_core->W[base_w] + operand1 + Core_GetC(p_core);
    mem_err = MEM_ERR_NONE;
    
    switch (addr_mode) {
        case CORE_OPC_ADDR_MODE_DIR:
            p_core->W[dest_w] = result;
            break;
            
        case CORE_OPC_ADDR_MODE_IND:
            Mem_Set(p_mem_data, p_core->W[dest_w], result, &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_DEC:
            Mem_Set(p_mem_data, p_core->W[dest_w], result, &mem_err);
            p_core->W[dest_w] -= offset;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_INC:
            Mem_Set(p_mem_data, p_core->W[dest_w], result, &mem_err);
            p_core->W[dest_w] += offset;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_DEC:
            p_core->W[dest_w] -= offset;
            Mem_Set(p_mem_data, p_core->W[dest_w], result, &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_INC:
            p_core->W[dest_w] += offset;
            Mem_Set(p_mem_data, p_core->W[dest_w], result, &mem_err);
            break;
            
        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return;
    }
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    if (size_op == 0) {
        result = CPU_SignExt16(result);
    } else {
        *p_err = CORE_ERR_OPC_UNSUPORTED_YET;
        return;
    }
    
    /* Update Status Register */
    if (((operand1 & 0x00000080) &&                             /* DC */
         (result   & 0x00000080))) {
        p_core->SR |=   CORE_SR_DC;
    } else {
        p_core->SR &= ~(CORE_SR_DC);
    }
    
    if (result < 0) {                                           /* N */
        p_core->SR |=   CORE_SR_N;
    } else {
        p_core->SR &= ~(CORE_SR_N);
    }
    
    if ((p_core->W[base_w] >=  0) &&                            /* OV */
        (result            <   0)) {
        p_core->SR |=   CORE_SR_OV;
    } else {
        p_core->SR &= ~(CORE_SR_OV);
    }
    
    if (result == 0) {                                          /* Z */
        p_core->SR |=   CORE_SR_Z;
    } else {
        p_core->SR &= ~(CORE_SR_Z);
    }
    
    if (((operand1 & 0x00008000) &&                             /* C */
         (result   & 0x00008000))) {
        p_core->SR |=   CORE_SR_C;
    } else {
        p_core->SR &= ~(CORE_SR_C);
    }
    
    Core_PC_Slide(p_core, 2);
    
    *p_err = CORE_ERR_NONE;
    
}

void Core_SUB_50006 (MEM_24      *p_mem_prog,
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
    CPU_INT32S  offset;
    MEM_ERR     mem_err;

    
    base_w    = (args & 0x078000) >> 15;
    size_op   = (args & 0x004000) >> 14;
    addr_mode = (args & 0x003800) >> 11;
    dest_w    = (args & 0x000780) >>  7;
    lit_op    =  args & 0x00001F;
    
    if (size_op != 0) {
        offset = 1;
        *p_err = CORE_ERR_OPC_UNSUPORTED_YET;
        return;
    } else {
        offset = 2;
    }
    
    operand1 = CPU_SignExt16(p_core->W[base_w]);
    
    result   = operand1 - lit_op;
    
    switch (addr_mode) {
        case CORE_OPC_ADDR_MODE_DIR:
            p_core->W[dest_w] = result & 0xFFF;
            break;

        case CORE_OPC_ADDR_MODE_IND:
            Mem_Set(p_mem_data, (p_core->W[dest_w] & 0xFFFE), result & 0xFFFF, &mem_err);

            if (mem_err != MEM_ERR_NONE) {
                *p_err = CORE_ERR_INVALID_MEM;
                return;
            }
            break;

        case CORE_OPC_ADDR_MODE_IND_POS_DEC:
            Mem_Set(p_mem_data, (p_core->W[dest_w] & 0xFFFE), result & 0xFFFF, &mem_err);

            if (mem_err != MEM_ERR_NONE) {
                *p_err = CORE_ERR_INVALID_MEM;
                return;
            }

            p_core->W[dest_w] -= offset;
            break;

        case CORE_OPC_ADDR_MODE_IND_POS_INC:
            Mem_Set(p_mem_data, (p_core->W[dest_w] & 0xFFFE), result & 0xFFFF, &mem_err);

            if (mem_err != MEM_ERR_NONE) {
                *p_err = CORE_ERR_INVALID_MEM;
                return;
            }

            p_core->W[dest_w] += offset;
            break;

        case CORE_OPC_ADDR_MODE_IND_PRE_DEC:
            p_core->W[dest_w] -= offset;
            Mem_Set(p_mem_data, (p_core->W[dest_w] & 0xFFFE), result & 0xFFFF, &mem_err);

            if (mem_err != MEM_ERR_NONE) {
                *p_err = CORE_ERR_INVALID_MEM;
                return;
            }
            break;

        case CORE_OPC_ADDR_MODE_IND_PRE_INC:
            p_core->W[dest_w] += offset;
            Mem_Set(p_mem_data, (p_core->W[dest_w] & 0xFFFE), result & 0xFFFF, &mem_err);

            if (mem_err != MEM_ERR_NONE) {
                *p_err = CORE_ERR_INVALID_MEM;
                return;
            }
            break;
    
        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return;
    }

    /* Update Status Register */
    if (((operand1 & 0x00000080) &&                              /* DC */
         (result   & 0x00000080))) {
        p_core->SR |=   CORE_SR_DC;
    } else {
        p_core->SR &= ~(CORE_SR_DC);
    }
    
    if (result < 0) {                                          /* N */
        p_core->SR |=   CORE_SR_N;
    } else {
        p_core->SR &= ~(CORE_SR_N);
    }
    
    if ((operand1 <  0) &&                                       /* OV */
        (result   >= 0)) {
        p_core->SR |=   CORE_SR_OV;
    } else {
        p_core->SR &= ~(CORE_SR_OV);
    }
    
    if (result == 0) {                                         /* Z */
        p_core->SR |=   CORE_SR_Z;
    } else {
        p_core->SR &= ~(CORE_SR_Z);
    }
    
    if (((operand1 & 0x00008000) &&                              /* C */
         (result   & 0x00008000))) {
        p_core->SR |=   CORE_SR_C;
    } else {
        p_core->SR &= ~(CORE_SR_C);
    }

    Core_PC_Slide(p_core, 2);

    *p_err = CORE_ERR_NONE;
}

void Core_MOV_WS_WD_78 (MEM_24      *p_mem_prog,
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
    CPU_INT32U  mask;
    CPU_INT08S  addr_offset;
    CPU_INT32U  value_original;
    MEM_ERR     mem_err;
    
    
    
    off_w         = (args & 0x078000) >> 15;
    size_op       = (args & 0x004000) >> 14;
    dst_addr_mode = (args & 0x003800) >> 11;
    dst_w         = (args & 0x000780) >>  7;
    src_addr_mode = (args & 0x000070) >>  4;
    src_w         =  args & 0x00000F;
    mem_err       = MEM_ERR_NONE;
    
    if (size_op == 0u) {
        addr_offset = 2;
    } else {
        addr_offset = 1;
    }
    
    switch (src_addr_mode) {
        case CORE_OPC_ADDR_MODE_DIR:
            mask              = Core_MaskGet(size_op, 0);
            value             = p_core->W[src_w];
            break;
            
        case CORE_OPC_ADDR_MODE_IND:
            mask              = Core_MaskGet(size_op, p_core->W[src_w]);
            value             = Mem_Get(p_mem_data, (p_core->W[src_w] & 0xFFFE), &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_DEC:
            mask              = Core_MaskGet(size_op, p_core->W[src_w]);
            value             = Mem_Get(p_mem_data, (p_core->W[src_w] & 0xFFFE), &mem_err);
            p_core->W[src_w] -= addr_offset;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_INC:
            mask              = Core_MaskGet(size_op, p_core->W[src_w]);
            value             = Mem_Get(p_mem_data, (p_core->W[src_w] & 0xFFFE), &mem_err);
            p_core->W[src_w] += addr_offset;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_DEC:
            p_core->W[src_w] -= addr_offset;
            mask              = Core_MaskGet(size_op, p_core->W[src_w]);
            value             = Mem_Get(p_mem_data, (p_core->W[src_w] & 0xFFFE), &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_INC:
            p_core->W[src_w] += addr_offset;
            mask              = Core_MaskGet(size_op, p_core->W[src_w]);
            value             = Mem_Get(p_mem_data, (p_core->W[src_w] & 0xFFFE), &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_OFFSET:
            mask              = Core_MaskGet(size_op, ((p_core->W[src_w] + p_core->W[off_w]) & 0xFFFE));
            value             = Mem_Get(p_mem_data, ((p_core->W[src_w] + p_core->W[off_w]) & 0xFFFE), &mem_err);
            break;
            
        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return;
    }
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }

    value = Core_Align(value, mask);
    
    switch (dst_addr_mode) {
        case CORE_OPC_ADDR_MODE_DIR:
            mask             = Core_MaskGet(size_op, 0);
            value_original   = p_core->W[dst_w];
            p_core->W[dst_w] = Core_Merge(value_original, value, mask);
            break;
            
        case CORE_OPC_ADDR_MODE_IND:
            mask           = Core_MaskGet(size_op, p_core->W[dst_w]);
            value_original = Mem_Get(p_mem_data, (p_core->W[dst_w] & 0xFFFE), &mem_err);
            value          = Core_Merge(value_original, value, mask);
            Mem_Set(p_mem_data, (p_core->W[dst_w] & 0xFFFE), value, &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_DEC:
            mask           = Core_MaskGet(size_op, p_core->W[dst_w]);
            value_original = Mem_Get(p_mem_data, (p_core->W[dst_w] & 0xFFFE), &mem_err);
            value          = Core_Merge(value_original, value, mask);
            Mem_Set(p_mem_data, (p_core->W[dst_w] & 0xFFFE), value, &mem_err);
            p_core->W[dst_w] -= addr_offset;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_INC:
            mask           = Core_MaskGet(size_op, p_core->W[dst_w]);
            value_original = Mem_Get(p_mem_data, (p_core->W[dst_w] & 0xFFFE), &mem_err);
            value          = Core_Merge(value_original, value, mask);
            Mem_Set(p_mem_data, (p_core->W[dst_w] & 0xFFFE), value, &mem_err);
            
            p_core->W[dst_w] += addr_offset;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_DEC:
            p_core->W[dst_w] -= addr_offset;
            
            mask           = Core_MaskGet(size_op, p_core->W[dst_w]);
            value_original = Mem_Get(p_mem_data, (p_core->W[dst_w] & 0xFFFE), &mem_err);
            value          = Core_Merge(value_original, value, mask);
            Mem_Set(p_mem_data, (p_core->W[dst_w] & 0xFFFE), value, &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_INC:
            p_core->W[dst_w] += addr_offset;
            mask           = Core_MaskGet(size_op, p_core->W[dst_w]);
            value_original = Mem_Get(p_mem_data, (p_core->W[dst_w] & 0xFFFE), &mem_err);
            value          = Core_Merge(value_original, value, mask);
            Mem_Set(p_mem_data, (p_core->W[dst_w] & 0xFFFE), value, &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_OFFSET:
            mask           = Core_MaskGet(size_op, (p_core->W[dst_w] + p_core->W[off_w]));
            value_original = Mem_Get(p_mem_data, ((p_core->W[dst_w] + p_core->W[off_w]) & 0xFFFE), &mem_err);
            value          = Core_Merge(value_original, value, mask);
            Mem_Set(p_mem_data, ((p_core->W[dst_w] + p_core->W[off_w]) & 0xFFFE), value, &mem_err);
            break;
            
        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return;
    }
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    Core_PC_Slide(p_core, 2);
    
    *p_err = CORE_ERR_NONE;
}

void  Core_MOV_M_W_80 (MEM_24      *p_mem_prog,
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
    
    Core_PC_Slide(p_core, 2);
    
    *p_err = CORE_ERR_NONE;
}

void  Core_MOV_W_M_88 (MEM_24      *p_mem_prog,
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
    
    Core_PC_Slide(p_core, 2);
    
    *p_err = CORE_ERR_NONE;
}

void Core_Logical_W_L_W(MEM_24      *p_mem_prog,
                        MEM         *p_mem_data,
                        CORE_24F    *p_core,
                        CPU_INT32U   args,
                        OPCODE       instruction,
                        CORE_ERR    *p_err)
{
    CPU_INT32U  src_w;
    CPU_INT32U  size_op;
    CPU_INT32U  addr_mode;
    CPU_INT32U  dst_w;
    CPU_INT32U  lit;
    CPU_INT32U  offset;
    CPU_INT32S  operand1;
    CPU_INT32S  result;
    CPU_INT32U  value_original;
    CPU_INT32U  flags;
    CPU_INT32U  mask;
    MEM_ERR     mem_err;
    
    
    src_w     = (args & 0x078000) >> 15;
    size_op   = (args & 0x004000) >> 14;
    addr_mode = (args & 0x003800) >> 11;
    dst_w     = (args & 0x000780) >>  7;
    lit       =  args & 0x00001F;
    
    flags     = CORE_SR_NONE;
    mem_err   = MEM_ERR_NONE;
    
    if (size_op != 0) {
        offset = 1;
        mask   = 0xFF;
        operand1 = CPU_SignExt08(p_core->W[src_w] & mask);
    } else {
        offset = 2;
        mask   = 0xFFFF;
        operand1 = CPU_SignExt16(p_core->W[src_w] & mask);
    }
    
    switch (instruction) {
        case CORE_OPC_ADD_W_LIT_W:
            result = operand1 + lit;
            flags  = CORE_SR_DC | CORE_SR_N | CORE_SR_OV | CORE_SR_Z | CORE_SR_C;
            break;
            
        case CORE_OPC_ADDC_W_LIT_W:
            result = operand1 + lit + Core_GetC(p_core);
            flags  = CORE_SR_DC | CORE_SR_N | CORE_SR_OV | CORE_SR_Z | CORE_SR_C;
            break;
            
        case CORE_OPC_AND_W_LIT_W:
            result = operand1 & lit;
            flags  = CORE_SR_N | CORE_SR_Z;
            break;
            
        case CORE_OPC_SUB_W_LIT_W:
            result = operand1 - lit;
            flags  = CORE_SR_DC | CORE_SR_N | CORE_SR_OV | CORE_SR_Z | CORE_SR_C;
            break;
            
        case CORE_OPC_SUBB_W_LIT_W:
            result = operand1 - lit;
            if (!Core_GetC(p_core)) {
                result--;
            }
            flags  = CORE_SR_DC | CORE_SR_N | CORE_SR_OV | CORE_SR_Z | CORE_SR_C;
            break;
            
        case CORE_OPC_SUBR_W_LIT_W:
            result = lit - operand1;
            flags  = CORE_SR_DC | CORE_SR_N | CORE_SR_OV | CORE_SR_Z | CORE_SR_C;
            break;
            
        case CORE_OPC_SUBBR_W_LIT_W:
            result =  lit - operand1;
            if (!Core_GetC(p_core)) {
                result--;
            }
            flags  = CORE_SR_DC | CORE_SR_N | CORE_SR_OV | CORE_SR_Z | CORE_SR_C;
            break;
            
        case CORE_OPC_IOR_W_LIT_W:
            result = operand1 | lit;
            flags  = CORE_SR_N | CORE_SR_Z;
            break;
            
        case CORE_OPC_XOR_W_LIT_W:
            result = operand1 ^ lit;
            flags  = CORE_SR_N | CORE_SR_Z;
            break;
            
        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return;
    }
    
    switch (addr_mode) {
        case CORE_OPC_ADDR_MODE_DIR:
            mask = Core_MaskGet(size_op, 0);
            p_core->W[dst_w] = result;
            break;
            
        case CORE_OPC_ADDR_MODE_IND:
            mask              = Core_MaskGet(size_op, p_core->W[dst_w]);
            value_original    = Mem_Get(p_mem_data, p_core->W[dst_w], &mem_err);
            result            = Core_Merge(value_original, result, mask);
            Mem_Set(p_mem_data, p_core->W[dst_w], result, &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_DEC:
            mask              = Core_MaskGet(size_op, p_core->W[dst_w]);
            value_original    = Mem_Get(p_mem_data, p_core->W[dst_w], &mem_err);
            result            = Core_Merge(value_original, result, mask);
            Mem_Set(p_mem_data, p_core->W[dst_w], result, &mem_err);
            p_core->W[src_w] -= offset;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_INC:
            mask              = Core_MaskGet(size_op, p_core->W[dst_w]);
            value_original    = Mem_Get(p_mem_data, p_core->W[dst_w], &mem_err);
            result            = Core_Merge(value_original, result, mask);
            Mem_Set(p_mem_data, p_core->W[dst_w], result, &mem_err);
            p_core->W[src_w] += offset;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_DEC:
            p_core->W[src_w] -= offset;
            mask              = Core_MaskGet(size_op, p_core->W[dst_w]);
            value_original    = Mem_Get(p_mem_data, p_core->W[dst_w], &mem_err);
            result            = Core_Merge(value_original, result, mask);
            Mem_Set(p_mem_data, p_core->W[dst_w], result, &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_INC:
            p_core->W[src_w] += offset;
            mask              = Core_MaskGet(size_op, p_core->W[dst_w]);
            value_original    = Mem_Get(p_mem_data, p_core->W[dst_w], &mem_err);
            result            = Core_Merge(value_original, result, mask);
            Mem_Set(p_mem_data, p_core->W[dst_w], result, &mem_err);
            break;
            
        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return;
    }
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    /* Update Status Register */
    if (((operand1 & 0x00000080) &&                              /* DC */
         (result   & 0x00000080))) {
        p_core->SR |=   CORE_SR_DC;
    } else {
        p_core->SR &= ~(CORE_SR_DC);
    }
    
    if (result < 0) {                                          /* N */
        p_core->SR |=   CORE_SR_N;
    } else {
        p_core->SR &= ~(CORE_SR_N);
    }
    
    if ((operand1 <  0) &&                                       /* OV */
        (result   >= 0)) {
        p_core->SR |=   CORE_SR_OV;
    } else {
        p_core->SR &= ~(CORE_SR_OV);
    }
    
    if (result == 0) {                                         /* Z */
        p_core->SR |=   CORE_SR_Z;
    } else {
        p_core->SR &= ~(CORE_SR_Z);
    }
    
    if (((operand1 & 0x00008000) &&                              /* C */
         (result   & 0x00008000))) {
        p_core->SR |=   CORE_SR_C;
    } else {
        p_core->SR &= ~(CORE_SR_C);
    }
    
    Core_PC_Slide(p_core, 2);
    
    *p_err = CORE_ERR_NONE;
}

void Core_BTST (MEM_24      *p_mem_prog,
                MEM         *p_mem_data,
                CORE_24F    *p_core,
                CPU_INT32U   args,
                CORE_ERR    *p_err)
{
    CPU_INT32U   z_flag;
    CPU_INT32U   test_bit;
    CPU_INT32U   addr_mode;
    CPU_INT32U   src_w;
    CPU_INT32U   value;
    CPU_INT32U   mask;
    CPU_BOOLEAN  result;
    MEM_ERR      mem_err;
    
    
    z_flag    = (args & 0x008000) >> 15;
    test_bit  = (args & 0x007800) >> 11;
    addr_mode = (args & 0x000070) >>  4;
    src_w     =  args & 0x00000F;
    mem_err   = MEM_ERR_NONE;
    
    switch (addr_mode) {
        case CORE_OPC_ADDR_MODE_DIR:
            value = p_core->W[src_w];
            break;
            
        case CORE_OPC_ADDR_MODE_IND:
            value = Mem_Get(p_mem_data, p_core->W[src_w], &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_DEC:
            value = Mem_Get(p_mem_data, p_core->W[src_w], &mem_err);
            p_core->W[src_w] -= 2;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_INC:
            value = Mem_Get(p_mem_data, p_core->W[src_w], &mem_err);
            p_core->W[src_w] += 2;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_DEC:
            p_core->W[src_w] -= 2;
            value = Mem_Get(p_mem_data, p_core->W[src_w], &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_INC:
            p_core->W[src_w] += 2;
            value = Mem_Get(p_mem_data, p_core->W[src_w], &mem_err);
            break;
            
        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return;
    }
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    mask   = 1 << (p_core->W[test_bit]);
    result = ((value & mask) == mask);
    
    if (z_flag) {
        p_core->SR     &= ~CORE_SR_Z;
        if (!result) {
            p_core->SR |=  CORE_SR_Z;
        }
    } else {
        p_core->SR     &= ~CORE_SR_C;
        if (result) {
            p_core->SR |=  CORE_SR_C;
        }
    }
    
    Core_PC_Slide(p_core, 2);
    *p_err = CORE_ERR_NONE;
}

void  Core_BIT_LOGIC_W (MEM_24      *p_mem_prog,
                        MEM         *p_mem_data,
                        CORE_24F    *p_core,
                        CPU_INT32U   args,
                        OPCODE       operation,
                        CORE_ERR    *p_err)
{
    CPU_INT32U  bit_pos;
    CPU_INT32U  size_op;
    CPU_INT32U  addr_mode;
    CPU_INT32U  w_reg;
    CPU_INT32U  value;
    CPU_INT32U  mask;
    MEM_ERR     mem_err;


    bit_pos   = (args & 0x00F000) >> 12;
    size_op   = (args & 0x000400) >> 10;
    addr_mode = (args & 0x000070) >>  4;
    w_reg     =  args & 0x00000F;

    if (size_op != 0) {
        *p_err = CORE_ERR_OPC_UNSUPORTED_YET;
        return;
    }

    mask = 1 << bit_pos;

    mem_err = MEM_ERR_NONE;
    
    switch (addr_mode) {
        case CORE_OPC_ADDR_MODE_DIR:
            value = p_core->W[w_reg];
            break;
            
        case CORE_OPC_ADDR_MODE_IND:
            value = Mem_Get(p_mem_data, p_core->W[w_reg], &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_DEC:
            value = Mem_Get(p_mem_data, p_core->W[w_reg], &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_INC:
            value = Mem_Get(p_mem_data, p_core->W[w_reg], &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_DEC:
            p_core->W[w_reg] -= 2;
            value = Mem_Get(p_mem_data, p_core->W[w_reg], &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_INC:
            p_core->W[w_reg] += 2;
            value = Mem_Get(p_mem_data, p_core->W[w_reg], &mem_err);
            break;
            
        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return;
    }
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    switch (operation) {
        case CORE_OPC_BCLR_W:
            value &= ~mask;
            break;
        case CORE_OPC_BSET_W:
            value |=  mask;
            break;
        case CORE_OPC_BTG_W:
            if ((value & mask) == mask) {
                value &= ~mask;
            } else {
                value |=  mask;
            }
            break;

        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return;
    }
    
    switch (addr_mode) {
        case CORE_OPC_ADDR_MODE_DIR:
            p_core->W[w_reg] = value;
            break;
            
        case CORE_OPC_ADDR_MODE_IND:
            Mem_Set(p_mem_data, p_core->W[w_reg], value, &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_DEC:
            Mem_Set(p_mem_data, p_core->W[w_reg], value, &mem_err);
            p_core->W[w_reg] -= 2;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_INC:
            Mem_Set(p_mem_data, p_core->W[w_reg], value, &mem_err);
            p_core->W[w_reg] += 2;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_DEC:
            Mem_Set(p_mem_data, p_core->W[w_reg], value, &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_INC:
            Mem_Set(p_mem_data, p_core->W[w_reg], value, &mem_err);
            break;
            
        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return;
    }
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }

    Core_PC_Slide(p_core, 2);
    
    *p_err = CORE_ERR_NONE;
}

void  Core_BSET_M_A8 (MEM_24      *p_mem_prog,
                      MEM         *p_mem_data,
                      CORE_24F    *p_core,
                      CPU_INT32U   args,
                      CORE_ERR    *p_err)
{
    CPU_INT32U  addr;
    CPU_INT16U  bit;
    MEM_ERR     mem_err;
    CPU_INT32U  value;
    
    bit  = (args & 0x00E000) >> 13;
    addr =  args & 0x001FFF;
    
    if (addr & 0x1) {
        bit += 8;
    }
    
    value = Mem_Get(p_mem_data, (addr & 0x001FFE), &mem_err);
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    value |= (1 << bit);
    
    Mem_Set(p_mem_data, (addr & 0x001FFE), value, &mem_err);
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    Core_PC_Slide(p_core, 2);
    
    *p_err = CORE_ERR_NONE;
}

void  Core_BCLR_M_A9 (MEM_24      *p_mem_prog,
                      MEM         *p_mem_data,
                      CORE_24F    *p_core,
                      CPU_INT32U   args,
                      CORE_ERR    *p_err)
{
    CPU_INT32U  addr;
    CPU_INT16U  bit;
    MEM_ERR     mem_err;
    CPU_INT32U  value;
    
    bit  = (args & 0x00E000) >> 13;
    addr =  args & 0x001FFF;
    
    if (addr & 0x1) {
        bit += 8;
    }
    
    value = Mem_Get(p_mem_data, (addr & 0x001FFE), &mem_err);
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    value &= ~(1 << bit);
    
    Mem_Set(p_mem_data, (addr & 0x001FFE), value, &mem_err);
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    Core_PC_Slide(p_core, 2);
    
    *p_err = CORE_ERR_NONE;
}

void  Core_BTSS_W (MEM_24      *p_mem_prog,
                   MEM         *p_mem_data,
                   CORE_24F    *p_core,
                   CPU_INT32U   args,
                   CORE_ERR    *p_err)
{
    CPU_INT32U  bit;
    CPU_INT32U  addr_mode;
    CPU_INT32U  src_w;
    CPU_INT32U  value;
    OPCODE      opc;
    CPU_INT32U  opc_words;
    MEM_ERR     mem_err;
    
    
    bit       = (args & 0x00F000) >> 12;
    addr_mode = (args & 0x000070) >>  4;
    src_w     =  args & 0x00000F;
    
    
    mem_err = MEM_ERR_NONE;
    
    switch (addr_mode) {
        case CORE_OPC_ADDR_MODE_DIR:
            value = p_core->W[src_w];
            break;
            
        case CORE_OPC_ADDR_MODE_IND:
            value = Mem_Get(p_mem_data, p_core->W[src_w], &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_DEC:
            value = Mem_Get(p_mem_data, p_core->W[src_w], &mem_err);
            p_core->W[src_w] -= 2;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_INC:
            value = Mem_Get(p_mem_data, p_core->W[src_w], &mem_err);
            p_core->W[src_w] += 2;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_DEC:
            p_core->W[src_w] -= 2;
            value = Mem_Get(p_mem_data, p_core->W[src_w], &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_INC:
            p_core->W[src_w] += 2;
            value = Mem_Get(p_mem_data, p_core->W[src_w], &mem_err);
            break;
            
        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return;
    }
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    
    if ((value & bit) == bit) {
        
        
        opc = Mem_Get24(p_mem_prog, Core_PC_Get(p_core), &mem_err);
        
        if (mem_err != MEM_ERR_NONE) {
            *p_err = CORE_ERR_INVALID_MEM;
            return;
        }

        opc_words = Core_OPC_Words(opc);
        
        switch (opc_words) {
            case 1:
            case 2:
                Core_PC_Slide(p_core, 2 * opc_words);
                break;
                
            default:
                *p_err = CORE_ERR_INVALID_OPC_CYCLE;
                return;
        }

    }
    
    Core_PC_Slide(p_core, 2);
    
    *p_err = CORE_ERR_NONE;
}

void  Core_BTSC_W (MEM_24      *p_mem_prog,
                   MEM         *p_mem_data,
                   CORE_24F    *p_core,
                   CPU_INT32U   args,
                   CORE_ERR    *p_err)
{
    CPU_INT32U  bit;
    CPU_INT32U  addr_mode;
    CPU_INT32U  src_w;
    CPU_INT32U  value;
    OPCODE      opc;
    CPU_INT32U  opc_words;
    MEM_ERR     mem_err;
    
    
    bit       = (args & 0x00F000) >> 12;
    addr_mode = (args & 0x000070) >>  4;
    src_w     =  args & 0x00000F;
    
    
    mem_err = MEM_ERR_NONE;
    
    switch (addr_mode) {
        case CORE_OPC_ADDR_MODE_DIR:
            value = p_core->W[src_w];
            break;
            
        case CORE_OPC_ADDR_MODE_IND:
            value = Mem_Get(p_mem_data, p_core->W[src_w], &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_DEC:
            value = Mem_Get(p_mem_data, p_core->W[src_w], &mem_err);
            p_core->W[src_w] -= 2;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_INC:
            value = Mem_Get(p_mem_data, p_core->W[src_w], &mem_err);
            p_core->W[src_w] += 2;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_DEC:
            p_core->W[src_w] -= 2;
            value = Mem_Get(p_mem_data, p_core->W[src_w], &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_INC:
            p_core->W[src_w] += 2;
            value = Mem_Get(p_mem_data, p_core->W[src_w], &mem_err);
            break;
            
        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return;
    }
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    
    if ((value & (1 << bit)) == 0u) {
        
        
        opc = Mem_Get24(p_mem_prog, Core_PC_Get(p_core), &mem_err);
        
        if (mem_err != MEM_ERR_NONE) {
            *p_err = CORE_ERR_INVALID_MEM;
            return;
        }

        opc_words = Core_OPC_Words(opc);
        
        switch (opc_words) {
            case 1:
            case 2:
                Core_PC_Slide(p_core, 2 * opc_words);
                break;
                
            default:
                *p_err = CORE_ERR_INVALID_OPC_CYCLE;
                return;
        }

    }
    
    Core_PC_Slide(p_core, 2);
    
    *p_err = CORE_ERR_NONE;
}

void  Core_BTSC_AF (MEM_24      *p_mem_prog,
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
    
    
    if (size_op == 1) {
        bit     =  1 << (((args & 0x00E000) >> 12) | (args & 0x000001));
        bit <<= 8;
    } else {
        bit     =  1 << ((args & 0x00E000) >> 13);
    }
    
    value   = Mem_Get(p_mem_data, addr, &mem_err);
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    if ((value & bit) == 0u) {
        
        
        opc = Mem_Get24(p_mem_prog, Core_PC_Get(p_core), &mem_err);
        
        if (mem_err != MEM_ERR_NONE) {
            *p_err = CORE_ERR_INVALID_MEM;
            return;
        }
        
        opc_words = Core_OPC_Words(opc);
        
        switch (opc_words) {
            case 1:
            case 2:
                Core_PC_Slide(p_core, 2 * opc_words);
                break;
                
            default:
                *p_err = CORE_ERR_INVALID_OPC_CYCLE;
                return;
        }
    }
    
    Core_PC_Slide(p_core, 2);
    
    *p_err = CORE_ERR_NONE;
}

void  Core_BTSS_AE (MEM_24      *p_mem_prog,
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
    
    
    if (size_op == 1) {
        bit     =  1 << (((args & 0x00E000) >> 12) | (args & 0x000001));
        bit <<= 8;
    } else {
        bit     =  1 << ((args & 0x00E000) >> 13);
    }
    
    value   = Mem_Get(p_mem_data, addr, &mem_err);
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    if ((value & bit) != 0u) {
        
        
        opc = Mem_Get24(p_mem_prog, Core_PC_Get(p_core), &mem_err);
        
        if (mem_err != MEM_ERR_NONE) {
            *p_err = CORE_ERR_INVALID_MEM;
            return;
        }
        
        opc_words = Core_OPC_Words(opc);
        
        switch (opc_words) {
            case 1:
            case 2:
                Core_PC_Slide(p_core, 2 * opc_words);
                break;
                
            default:
                *p_err = CORE_ERR_INVALID_OPC_CYCLE;
                return;
        }
    }
    
    Core_PC_Slide(p_core, 2);
    
    *p_err = CORE_ERR_NONE;
}

void Core_ADDC_B08 (MEM_24      *p_mem_prog,
                    MEM         *p_mem_data,
                    CORE_24F    *p_core,
                    CPU_INT32U   args,
                    CORE_ERR    *p_err)
{
    CPU_INT32U  size_op;
    CPU_INT32U  lit;
    CPU_INT32U  w_reg;
    CPU_INT32U  result;
    CPU_INT32U  initial;

    size_op = (args & 0x004000) >> 14;
    lit     = (args & 0x003FF0) >>  4;
    w_reg   =  args & 0x00000F;

    if (size_op != 0) {
        *p_err = CORE_ERR_OPC_UNSUPORTED_YET;
        return;
    }

    initial = p_core->W[w_reg];
    result  = lit + initial + Core_GetC(p_core);

    p_core->W[w_reg] = result;

       /* Update Status Register */
    if (((initial & 0x00000080) &&                              /* DC */
         (result  & 0x00000080))) {
        p_core->SR |=   CORE_SR_DC;
    } else {
        p_core->SR &= ~(CORE_SR_DC);
    }
    
    //if (result < 0) {                                          /* N */
    //    p_core->SR |=   CORE_SR_N;
    //} else {
        p_core->SR &= ~(CORE_SR_N);
    //}
    
    if (result > initial) {
        p_core->SR |=   CORE_SR_OV;
    } else {
        p_core->SR &= ~(CORE_SR_OV);
    }
    
    if (result == 0) {                                         /* Z */
        p_core->SR |=   CORE_SR_Z;
    } else {
        p_core->SR &= ~(CORE_SR_Z);
    }
    
    if (((initial & 0x00008000) &&                              /* C */
         (result  & 0x00008000))) {
        p_core->SR |=   CORE_SR_C;
    } else {
        p_core->SR &= ~(CORE_SR_C);
    }
    
    Core_PC_Slide(p_core, 2);
    *p_err = CORE_ERR_NONE;
}

void Core_SETM_MOV_8BL_WN_B3C (MEM_24      *p_mem_prog,
                               MEM         *p_mem_data,
                               CORE_24F    *p_core,
                               CPU_INT32U   args,
                               CORE_ERR    *p_err)
{
    CPU_INT32U  value;
    CPU_INT32U  w_reg;
    
    
    value = (args & 0x000FF0) >> 4;
    w_reg =  args & 0x00000F;
    
    p_core->W[w_reg] &= 0xFF00;
    p_core->W[w_reg] |= value;
    
    Core_PC_Slide(p_core, 2);
    *p_err = CORE_ERR_NONE;
}

void Core_SETM_MOV_8BL_WN_B7A (MEM_24      *p_mem_prog,
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
    

    
    Core_PC_Slide(p_core, 2);
    *p_err = CORE_ERR_NONE;
}

void Core_SUB_B10 (MEM_24      *p_mem_prog,
                   MEM         *p_mem_data,
                   CORE_24F    *p_core,
                   CPU_INT32U   args,
                   CORE_ERR    *p_err)
{
    CPU_INT32U  size_op;
    CPU_INT32U  operand1;
    CPU_INT32U  dest_w;
    CPU_INT32U  result;
    CPU_INT32U  initial;
    
    size_op  = (args & 0x004000) >> 14;
    operand1 = (args & 0x003FF0) >>  4;
    dest_w   =  args & 0x00000F;
    
    if (size_op != 0) {
        *p_err = CORE_ERR_OPC_UNSUPORTED_YET;
        return;
    }
    
    initial = p_core->W[dest_w];
    result  = initial - operand1;
    
    p_core->W[dest_w] = (result & 0xFFFF);
    
    /* Update Status Register */
    if (((initial & 0x00000080) &&                              /* DC */
         (result  & 0x00000080))) {
        p_core->SR |=   CORE_SR_DC;
    } else {
        p_core->SR &= ~(CORE_SR_DC);
    }
    
    //if (result < 0) {                                          /* N */
    //    p_core->SR |=   CORE_SR_N;
    //} else {
        p_core->SR &= ~(CORE_SR_N);
    //}
    
    if (result > initial) {
        p_core->SR |=   CORE_SR_OV;
    } else {
        p_core->SR &= ~(CORE_SR_OV);
    }
    
    if (result == 0) {                                         /* Z */
        p_core->SR |=   CORE_SR_Z;
    } else {
        p_core->SR &= ~(CORE_SR_Z);
    }
    
    if (((initial & 0x00008000) &&                              /* C */
         (result  & 0x00008000))) {
        p_core->SR |=   CORE_SR_C;
    } else {
        p_core->SR &= ~(CORE_SR_C);
    }
    
    Core_PC_Slide(p_core, 2);
    *p_err = CORE_ERR_NONE;
}

void Core_ADD_B40     (MEM_24      *p_mem_prog,
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
    
    Core_PC_Slide(p_core, 2);
    *p_err = CORE_ERR_NONE;
}

void Core_ADDC_B48 (MEM_24      *p_mem_prog,
                    MEM         *p_mem_data,
                    CORE_24F    *p_core,
                    CPU_INT32U   args,
                    CORE_ERR    *p_err)
{
    CPU_INT32U  size_op;
    CPU_INT32U  destination;
    CPU_INT32U  addr;
    CPU_INT32U  operand;
    CPU_INT32U  result;
    MEM_ERR     mem_err;
    
    size_op         = (args & 0x004000) >> 14;
    destination     = (args & 0x002800) >> 13;
    addr            =  args & 0x001FFF;
    
    if (size_op != 0) {
        *p_err = CORE_ERR_OPC_UNSUPORTED_YET;
        return;
    }
    
    operand = Mem_Get(p_mem_data, addr, &mem_err);
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    result = operand + p_core->W[0] + Core_GetC(p_core);
    
    /* Update Status Register */
    if (((operand & 0x00000080) &&                             /* DC */
         (result  & 0x00000080))) {
        p_core->SR |=   CORE_SR_DC;
    } else {
        p_core->SR &= ~(CORE_SR_DC);
    }
    
    p_core->SR &= ~(CORE_SR_N);                                 /* N */
    
    if (operand > result) {
        p_core->SR |=   CORE_SR_OV;
    } else {
        p_core->SR &= ~(CORE_SR_OV);
    }
    
    if (result == 0) {                                          /* Z */
        //p_core->SR |=   CORE_SR_Z;
    } else {
        p_core->SR &= ~(CORE_SR_Z);
    }
    
    if (((operand & 0x00008000) &&                             /* C */
         (result  & 0x00008000))) {
        p_core->SR |=   CORE_SR_C;
    } else {
        p_core->SR &= ~(CORE_SR_C);
    }
    
    Core_PC_Slide(p_core, 2);
    
    *p_err = CORE_ERR_NONE;

}

void Core_DEC_E90 (MEM_24      *p_mem_prog,
                   MEM         *p_mem_data,
                   CORE_24F    *p_core,
                   CPU_INT32U   args,
                   CORE_ERR    *p_err)
{
    CPU_INT32U  size_op;
    CPU_INT32U  dst_addr_mode;
    CPU_INT32U  dst_w;
    CPU_INT32U  src_addr_mode;
    CPU_INT32U  src_w;
    CPU_INT32U  value;
    CPU_INT32U  initial;
    CPU_INT32U  offset;
    MEM_ERR     mem_err;
    
    
    size_op       = (args & 0x004000) >> 14;
    dst_addr_mode = (args & 0x003800) >> 11;
    dst_w         = (args & 0x000780) >>  7;
    src_addr_mode = (args & 0x000070) >>  4;
    src_w         =  args & 0x00000F;
    
    if (size_op != 0) {
        offset = 1;
        *p_err = CORE_ERR_OPC_UNSUPORTED_YET;
        return;
    } else {
        offset = 2;
    }
    
    mem_err = MEM_ERR_NONE;
    
    switch (src_addr_mode) {
        case CORE_OPC_ADDR_MODE_DIR:
            value = p_core->W[src_w];
            break;
            
        case CORE_OPC_ADDR_MODE_IND:
            value = Mem_Get(p_mem_data, p_core->W[src_w], &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_DEC:
            value = Mem_Get(p_mem_data, p_core->W[src_w], &mem_err);
            p_core->W[src_w] -= 2;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_INC:
            value = Mem_Get(p_mem_data, p_core->W[src_w], &mem_err);
            p_core->W[src_w] += 2;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_DEC:
            p_core->W[src_w] -= 2;
            value = Mem_Get(p_mem_data, p_core->W[src_w], &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_INC:
            p_core->W[src_w] += 2;
            value = Mem_Get(p_mem_data, p_core->W[src_w], &mem_err);
            break;
            
        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return;
    }
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    initial = value;
    value--;
    
    switch (dst_addr_mode) {
        case CORE_OPC_ADDR_MODE_DIR:
            p_core->W[dst_w] = value;
            break;
            
        case CORE_OPC_ADDR_MODE_IND:
            Mem_Set(p_mem_data, p_core->W[dst_w], value, &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_DEC:
            Mem_Set(p_mem_data, p_core->W[dst_w], value, &mem_err);
            p_core->W[src_w] -= 2;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_INC:
            Mem_Set(p_mem_data, p_core->W[dst_w], value, &mem_err);
            p_core->W[src_w] += 2;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_DEC:
            p_core->W[src_w] -= 2;
            Mem_Set(p_mem_data, p_core->W[dst_w], value, &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_INC:
            p_core->W[src_w] += 2;
            Mem_Set(p_mem_data, p_core->W[dst_w], value, &mem_err);
            break;
            
        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return;
    }
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    /* Update Status Register */
    if (((initial & 0x00000080) &&                              /* DC */
         (value   & 0x00000080))) {
        p_core->SR |=   CORE_SR_DC;
    } else {
        p_core->SR &= ~(CORE_SR_DC);
    }
    
    p_core->SR &= ~(CORE_SR_N);                                 /* N */

    
    if (initial < value) {
        p_core->SR |=   CORE_SR_OV;
    } else {
        p_core->SR &= ~(CORE_SR_OV);
    }
    
    if (value == 0) {                                         /* Z */
        p_core->SR |=   CORE_SR_Z;
    } else {
        p_core->SR &= ~(CORE_SR_Z);
    }
    
    if (((initial & 0x00008000) &&                              /* C */
         (value   & 0x00008000))) {
        p_core->SR |=   CORE_SR_C;
    } else {
        p_core->SR &= ~(CORE_SR_C);
    }

    Core_PC_Slide(p_core, 2);
    *p_err = CORE_ERR_NONE;
}

void Core_Logical_M_W (MEM_24      *p_mem_prog,
                       MEM         *p_mem_data,
                       CORE_24F    *p_core,
                       CPU_INT32U   args,
                       OPCODE       operation,
                       CORE_ERR    *p_err)
{
    CPU_INT32U  size_op;
    CPU_INT32U  dest;
    CPU_INT32U  addr;
    CPU_INT32S  value_0;
    CPU_INT32S  value_1;
    CPU_INT32U  value_original;
    CPU_INT32S  val_wreg;
    CPU_INT32U  flags;
    CPU_INT32U  mask;
    MEM_ERR     mem_err;
    
    
    size_op = (args & 0x004000) >> 14;
    dest    = (args & 0x002000) >> 13;
    addr    =  args & 0x001FFF;
    flags   =  CORE_SR_NONE;
    mask    =  Core_MaskGet(size_op, addr);
    
    value_0 = Mem_Get(p_mem_data, (addr & 0xFFFE), &mem_err);
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    value_0  = Core_Align(value_0, mask);
    
    value_0  = CPU_SignExt16(value_0);
    val_wreg = CPU_SignExt16(p_core->W[0]);
    
    switch (operation) {
        case CORE_OPC_ADD_M_W:
            value_1 = value_0 + val_wreg;
            flags   = CORE_SR_DC | CORE_SR_N | CORE_SR_OV | CORE_SR_Z | CORE_SR_C;
            break;
            
        case CORE_OPC_ADDC_M_W:
            value_1 = value_0 + val_wreg + Core_GetC(p_core);
            flags   = CORE_SR_DC | CORE_SR_N | CORE_SR_OV | CORE_SR_Z | CORE_SR_C;
            break;
            
        case CORE_OPC_AND_M_W:
            value_1 = value_0 & val_wreg;
            flags   = CORE_SR_N | CORE_SR_Z;
            break;
            
        case CORE_OPC_ASR_M_W:
            *p_err = CORE_ERR_OPC_UNSUPORTED_YET;
            return;
            
        case CORE_OPC_CLR_M_W:
            value_1 = 0x0000;
            break;
            
        case CORE_OPC_COM_M_W:
        case CORE_OPC_DEC_M_W:
        case CORE_OPC_DEC2_M_W:
            *p_err = CORE_ERR_OPC_UNSUPORTED_YET;
            return;
            
        case CORE_OPC_INC_M_W:
            value_1 = value_0 + 1;
            flags   = CORE_SR_DC | CORE_SR_N | CORE_SR_OV | CORE_SR_Z | CORE_SR_C;
            break;
            
        case CORE_OPC_INC2_M_W:
        case CORE_OPC_IOR_M_W:
        case CORE_OPC_LSR_M_W:
            *p_err = CORE_ERR_OPC_UNSUPORTED_YET;
            return;
            
        case CORE_OPC_MOV2_M_W:
            value_1 = value_0;
            flags   = CORE_SR_N | CORE_SR_Z;
            break;
            
        case CORE_OPC_NEG_M_W:
        case CORE_OPC_RLC_M_W:
        case CORE_OPC_RLNC_M_W:
        case CORE_OPC_RRC_M_W:
        case CORE_OPC_RRNC_M_W:
            *p_err = CORE_ERR_OPC_UNSUPORTED_YET;
            return;
            
        case CORE_OPC_SETM_M_W:
            value_1 = 0xFFFF;
            break;
            
        case CORE_OPC_SL_M_W:
        case CORE_OPC_SUB_M_W:
        case CORE_OPC_SUBB_M_W:
        case CORE_OPC_SUBBR_M_W:
        case CORE_OPC_SUBR_M_W:
        case CORE_OPC_XOR_M_W:
            *p_err = CORE_ERR_OPC_UNSUPORTED_YET;
            return;
            
        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return;
    }
    
    if (dest == 0) {
        value_original = p_core->W[0];
        mask = Core_MaskGet(size_op, 0);
        p_core->W[0] = Core_Merge(value_original, value_1, mask);
    } else {
        value_original    = Mem_Get(p_mem_data, (addr & 0xFFFE), &mem_err);
        
        Mem_Set(p_mem_data,
               (addr & 0xFFFE),
                Core_Merge(value_original, value_1, mask),
               &mem_err);
        
        if (mem_err != MEM_ERR_NONE) {
            *p_err = CORE_ERR_INVALID_MEM;
            return;
        }
    }
    
    /* Update Status Register */
    if (flags & CORE_SR_DC) {
        if (((value_0 & 0x00000080) &&                              /* DC */
             (value_1 & 0x00000080))) {
            p_core->SR |=   CORE_SR_DC;
        } else {
            p_core->SR &= ~(CORE_SR_DC);
        }
    }
    
    if (flags & CORE_SR_N) {
        if (value_1 < 0) {                                          /* N */
            p_core->SR |=   CORE_SR_N;
        } else {
            p_core->SR &= ~(CORE_SR_N);
        }
    }
    
    if (flags & CORE_SR_OV) {
        if (value_1 > value_0) {
            p_core->SR |=   CORE_SR_OV;
        } else {
            p_core->SR &= ~(CORE_SR_OV);
        }
    }
    
    if (flags & CORE_SR_Z) {
        if (value_1 == 0) {                                         /* Z */
            p_core->SR |=   CORE_SR_Z;
        } else {
            p_core->SR &= ~(CORE_SR_Z);
        }
    }
    
    if (flags & CORE_SR_C) {
        if (((value_0 & 0x00008000) &&                              /* C */
             (value_1 & 0x00008000))) {
            p_core->SR |=   CORE_SR_C;
        } else {
            p_core->SR &= ~(CORE_SR_C);
        }
    }
    
    Core_PC_Slide(p_core, 2);
    *p_err = CORE_ERR_NONE;

}

void Core_Logical (MEM_24      *p_mem_prog,
                   MEM         *p_mem_data,
                   CORE_24F    *p_core,
                   CPU_INT32U   args,
                   OPCODE       operation,
                   CORE_ERR    *p_err)
{
    CPU_INT32U  size_op;
    CPU_INT32U  dst_addr_mode;
    CPU_INT32U  dst_w;
    CPU_INT32U  src_addr_mode;
    CPU_INT32U  src_w;
    CPU_INT32U  value;
    CPU_INT32U  value_original;
    CPU_INT32U  initial;
    CPU_INT32U  offset;
    CPU_INT32U  src_mask;
    CPU_INT32U  dst_mask;
    MEM_ERR     mem_err;
    
    
    size_op       = (args & 0x004000) >> 14;
    dst_addr_mode = (args & 0x003800) >> 11;
    dst_w         = (args & 0x000780) >>  7;
    src_addr_mode = (args & 0x000070) >>  4;
    src_w         =  args & 0x00000F;
    
    if (size_op == 0) {
        offset = 2;
    } else {
        offset = 1;
    }
    
    mem_err = MEM_ERR_NONE;
    
    switch (src_addr_mode) {
        case CORE_OPC_ADDR_MODE_DIR:
            src_mask = Core_MaskGet(size_op, 0);
            value    = p_core->W[src_w];
            break;
            
        case CORE_OPC_ADDR_MODE_IND:
            src_mask = Core_MaskGet(size_op, p_core->W[src_w]);
            value    = Mem_Get(p_mem_data, p_core->W[src_w], &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_DEC:
            src_mask          = Core_MaskGet(size_op, p_core->W[src_w]);
            value             = Mem_Get(p_mem_data, p_core->W[src_w], &mem_err);
            p_core->W[src_w] -= offset;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_INC:
            src_mask          = Core_MaskGet(size_op, p_core->W[src_w]);
            value             = Mem_Get(p_mem_data, p_core->W[src_w], &mem_err);
            p_core->W[src_w] += offset;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_DEC:
            p_core->W[src_w] -= offset;
            src_mask          = Core_MaskGet(size_op, p_core->W[src_w]);
            value             = Mem_Get(p_mem_data, p_core->W[src_w], &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_INC:
            p_core->W[src_w] += offset;
            src_mask          = Core_MaskGet(size_op, p_core->W[src_w]);
            value             = Mem_Get(p_mem_data, p_core->W[src_w], &mem_err);
            break;
            
        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return;
    }
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    value   = Core_Align(value, src_mask);
    initial = value;

    switch (operation) {
        case CORE_OPC_INC_W:
            value++;
            break;
            
        case CORE_OPC_DEC_W:
            value--;
            break;
            
        case CORE_OPC_INC2_W:
            value += 2;
            break;
        case CORE_OPC_DEC2_W:
            value -= 2;
            break;

        case CORE_OPC_RLC_W:

            value <<= 1;
            value  &= 0xFFFFFFFE;
            value  |= Core_GetC(p_core);
            if (size_op == 0) {
                Core_SetC(p_core, (value & 0x00010000) >> 16);
            } else {
                Core_SetC(p_core, (value & 0x00000100) >> 8);
            }
            break;

        case CORE_OPC_RRC_W:
            if (size_op == 0) {
                value |= (Core_GetC(p_core) << 16);
            } else { 
                value |= (Core_GetC(p_core) << 8);
            }
            Core_SetC(p_core, (value & 0x1));
            value >>= 1;
            value  &= 0xFFFFFFFF;           
            break;
            
        case CORE_OPC_ASR_W:
        case CORE_OPC_COM_W:
        case CORE_OPC_LSR_W:
        case CORE_OPC_NEG_W:
        case CORE_OPC_RLNC_W:
        case CORE_OPC_RRNC_W:
        case CORE_OPC_SL_W:
        case CORE_OPC_TBLRDH_W:
        case CORE_OPC_TBLRDL_W:
        case CORE_OPC_TBLWDH_W:
        case CORE_OPC_TBLWDL_W:
            *p_err = CORE_ERR_OPC_UNSUPORTED_YET;
            return;

        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return;
    }
    
    
    switch (dst_addr_mode) {
        case CORE_OPC_ADDR_MODE_DIR:
            dst_mask = Core_MaskGet(size_op, 0); 
            p_core->W[dst_w] = value;
            break;
            
        case CORE_OPC_ADDR_MODE_IND:
            dst_mask          = Core_MaskGet(size_op, p_core->W[dst_w]);
            value_original    = Mem_Get(p_mem_data, p_core->W[dst_w], &mem_err);
            value             = Core_Merge(value_original, value, dst_mask);
            Mem_Set(p_mem_data, p_core->W[dst_w], value, &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_DEC:
            dst_mask          = Core_MaskGet(size_op, p_core->W[dst_w]);
            value_original    = Mem_Get(p_mem_data, p_core->W[dst_w], &mem_err);
            value             = Core_Merge(value_original, value, dst_mask);
            Mem_Set(p_mem_data, p_core->W[dst_w], value, &mem_err);
            p_core->W[src_w] -= offset;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_INC:
            dst_mask          = Core_MaskGet(size_op, p_core->W[dst_w]);
            value_original    = Mem_Get(p_mem_data, p_core->W[dst_w], &mem_err);
            value             = Core_Merge(value_original, value, dst_mask);
            Mem_Set(p_mem_data, p_core->W[dst_w], value, &mem_err);
            p_core->W[src_w] += offset;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_DEC:
            p_core->W[src_w] -= offset;
            dst_mask          = Core_MaskGet(size_op, p_core->W[dst_w]);
            value_original    = Mem_Get(p_mem_data, p_core->W[dst_w], &mem_err);
            value             = Core_Merge(value_original, value, dst_mask);
            Mem_Set(p_mem_data, p_core->W[dst_w], value, &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_INC:
            p_core->W[src_w] += offset;
            dst_mask          = Core_MaskGet(size_op, p_core->W[dst_w]);
            value_original    = Mem_Get(p_mem_data, p_core->W[dst_w], &mem_err);
            value             = Core_Merge(value_original, value, dst_mask);
            Mem_Set(p_mem_data, p_core->W[dst_w], value, &mem_err);
            break;
            
        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return;
    }
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    /* Update Status Register */
    if (((initial & 0x00000080) &&                              /* DC */
         (value   & 0x00000080))) {
        p_core->SR |=   CORE_SR_DC;
    } else {
        p_core->SR &= ~(CORE_SR_DC);
    }
    
    p_core->SR &= ~(CORE_SR_N);                                 /* N */

    
    if (initial < value) {
        p_core->SR |=   CORE_SR_OV;
    } else {
        p_core->SR &= ~(CORE_SR_OV);
    }
    
    if (value == 0) {                                         /* Z */
        p_core->SR |=   CORE_SR_Z;
    } else {
        p_core->SR &= ~(CORE_SR_Z);
    }
    
    if (((initial & 0x00008000) &&                              /* C */
         (value   & 0x00008000))) {
        p_core->SR |=   CORE_SR_C;
    } else {
        p_core->SR &= ~(CORE_SR_C);
    }

    Core_PC_Slide(p_core, 2);
    *p_err = CORE_ERR_NONE;
}

void Core_MUL_UU_B8006 (MEM_24      *p_mem_prog,
                        MEM         *p_mem_data,
                        CORE_24F    *p_core,
                        CPU_INT32U   args,
                        CORE_ERR    *p_err)
{
    CPU_INT32U base_w;
    CPU_INT32U dest_w;
    CPU_INT32U operand1;
    CPU_INT32U result;
    
    base_w   = (args & 0x007800) >> 11;
    dest_w   = (args & 0x000780) >>  7;
    operand1 =  args & 0x00001F;
    
    result = p_core->W[base_w] * operand1;
    
    p_core->W[dest_w]     = (result & 0x0000FFFF);
    p_core->W[dest_w + 1] = (result & 0xFFFF0000);
    
    Core_PC_Slide(p_core, 2);
    
    *p_err = CORE_ERR_NONE;
}

void Core_MUL_UU_B80 (MEM_24      *p_mem_prog,
                      MEM         *p_mem_data,
                      CORE_24F    *p_core,
                      CPU_INT32U   args,
                      CORE_ERR    *p_err)
{
    CPU_INT32U  base_reg;
    CPU_INT32U  dest_reg;
    CPU_INT32U  addr_mode;
    CPU_INT32U  srce_reg;
    
    CPU_INT32U  operand0;
    CPU_INT32U  operand1;
    CPU_INT32U  result;
    
    MEM_ERR     mem_err;
    
    
    
    base_reg  = (args & 0x007800) >> 11;
    dest_reg  = (args & 0x000780) >>  7;
    addr_mode = (args & 0x000070) >>  4;
    srce_reg  =  args & 0x00000F;
    
    operand0 = p_core->W[base_reg];
    
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
    p_core->W[dest_reg + 1] = (result & 0xFFFF0000) >> 16;
    Core_PC_Slide(p_core, 2);
    
    *p_err = CORE_ERR_NONE;
}

void Core_MUL_SS_B88 (MEM_24      *p_mem_prog,
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
    Core_PC_Slide(p_core, 2);
    
    *p_err = CORE_ERR_NONE;
}

void Core_MUL_SS_B98 (MEM_24      *p_mem_prog,
                      MEM         *p_mem_data,
                      CORE_24F    *p_core,
                      CPU_INT32U   args,
                      CORE_ERR    *p_err)
{
    CPU_INT32U  base_reg;
    CPU_INT32U  dest_reg;
    CPU_INT32U  addr_mode;
    CPU_INT32U  srce_reg;
    
    CPU_INT32U  operand0;
    CPU_INT32S  operand1;
    CPU_INT32S  result;
    
    MEM_ERR     mem_err;
    

    
    base_reg  = (args & 0x007800) >> 11;
    dest_reg  = (args & 0x000780) >>  7;
    addr_mode = (args & 0x000070) >>  4;
    srce_reg  =  args & 0x00000F;
    
    operand0 = p_core->W[base_reg];
    
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
    Core_PC_Slide(p_core, 2);
    
    *p_err = CORE_ERR_NONE;
}

void Core_MOV_BF8 (MEM_24      *p_mem_prog,
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
    
    Core_PC_Slide(p_core, 2);
    *p_err = CORE_ERR_NONE;
}

void Core_DIV_SU_D8000 (MEM_24      *p_mem_prog,
                       MEM         *p_mem_data,
                       CORE_24F    *p_core,
                       CPU_INT32U   args,
                       OPCODE       instruction,
                       CORE_ERR    *p_err)
{
    CPU_INT32U  msw;
    CPU_INT32U  lsw;
    CPU_INT32U  size_op;
    CPU_INT32U  div_w;
    CPU_INT32S  operand1;
    CPU_INT32S  operand2;
    CPU_INT32S  result;
    CPU_INT32S  modulo;
    
    
    msw     = (args & 0x007800) >> 11;
    lsw     = (args & 0x000780) >>  7;
    size_op = (args & 0x000040) >>  6;
    div_w   =  args & 0x00000F;
    
    if (size_op != 0) {
        *p_err = CORE_ERR_OPC_UNSUPORTED_YET;
        return;
    }

    if ((p_core->RCOUNT == 0) && ((p_core->SR & CORE_SR_RA) == CORE_SR_RA)) {
    
        p_core->W[0] = p_core->W[lsw];
    
        switch (instruction) {
            case CORE_OPC_DIV_S:
                if ((p_core->W[lsw] & 0x8000) == 0) {
                    p_core->W[1] = 0x0000;
                } else {
                    p_core->W[1] = 0xFFFF;
                }
                break;
     
            case CORE_OPC_DIV_U:
                p_core->W[1] = 0x0000;
                break;
        }
    
        operand1 = (p_core->W[1]   << 16) | p_core->W[0];
        operand2 =  p_core->W[div_w];

    
        if (operand2 != 0) {
            result   =  operand1 / operand2;
            modulo   =  operand1 % operand2;

            if (size_op == 0) {
                p_core->W[0] = result & 0xFFFF;
                p_core->W[1] = modulo & 0xFFFF;
            } else {
                *p_err = CORE_ERR_OPC_UNSUPORTED_YET;
                return;
            }
    
            if (result < 0) {                                           /* N */
                p_core->SR |=   CORE_SR_N;
            } else {
                p_core->SR &= ~(CORE_SR_N);
            }
        
            if (modulo == 0) {                                          /* Z */
                p_core->SR |=   CORE_SR_Z;
            } else {
                p_core->SR &= ~(CORE_SR_Z);
            }
        
            p_core->SR &=   ~CORE_SR_OV;

        } else {
            p_core->SR |=   CORE_SR_OV;                             /* OV */
        }
    
    }

    Core_PC_Slide(p_core, 2);
    
    *p_err = CORE_ERR_NONE;
}

void Core_CP0_E0000 (MEM_24      *p_mem_prog,
                     MEM         *p_mem_data,
                     CORE_24F    *p_core,
                     CPU_INT32U   args,
                     CORE_ERR    *p_err)
{
    CPU_INT32U  size_op;
    CPU_INT32U  addr_mode;
    CPU_INT32U  w_reg;
    CPU_INT32U  value;
    CPU_INT32U  offset;
    CPU_INT32U  mask;
    MEM_ERR     mem_err;
    
    
    size_op   = (args & 0x000400) >> 10;
    addr_mode = (args & 0x000070) >>  4;
    w_reg     =  args & 0x00000F;
    
    if (size_op != 0) {
        offset = 1;
    } else {
        offset = 2;
    }
    
    mem_err   = MEM_ERR_NONE;
    
    switch (addr_mode) {
        case CORE_OPC_ADDR_MODE_DIR:
            mask  = Core_MaskGet(size_op, p_core->W[w_reg]);
            value = p_core->W[w_reg] & mask;
            break;
            
        case CORE_OPC_ADDR_MODE_IND:
            mask  = Core_MaskGet(size_op, p_core->W[w_reg]);
            value = Mem_Get(p_mem_data, p_core->W[w_reg], &mem_err) & mask;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_DEC:
            mask  = Core_MaskGet(size_op, p_core->W[w_reg]);
            value = Mem_Get(p_mem_data, p_core->W[w_reg], &mem_err) & mask;
            p_core->W[w_reg] -= offset;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_INC:
            mask  = Core_MaskGet(size_op, p_core->W[w_reg]);
            value = Mem_Get(p_mem_data, p_core->W[w_reg], &mem_err) & mask;
            p_core->W[w_reg] += offset;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_DEC:
            p_core->W[w_reg] -= offset;
            mask  = Core_MaskGet(size_op, p_core->W[w_reg]);
            value = Mem_Get(p_mem_data, p_core->W[w_reg], &mem_err) & mask;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_INC:
            p_core->W[w_reg] += offset;
            mask  = Core_MaskGet(size_op, p_core->W[w_reg]);
            value = Mem_Get(p_mem_data, p_core->W[w_reg], &mem_err) & mask;
            break;
            
        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return;
    }
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    value = Core_Align(value, mask);
    
    
    /* Update Status Register */
                                                                
    p_core->SR &= ~(CORE_SR_DC);                                /* DC */

    p_core->SR |=   CORE_SR_N;                                  /* N */
                                                                
    p_core->SR &= ~(CORE_SR_OV);                                /* OV */

    
    if (value == 0) {                                           /* Z */
        p_core->SR |=   CORE_SR_Z;
    } else {
        p_core->SR &= ~(CORE_SR_Z);
    }
    
    p_core->SR &= ~(CORE_SR_C);                                 /* C */
    
    Core_PC_Slide(p_core, 2);
    *p_err = CORE_ERR_NONE;
}

void Core_CP0_E20 (MEM_24      *p_mem_prog,
                   MEM         *p_mem_data,
                   CORE_24F    *p_core,
                   CPU_INT32U   args,
                   CORE_ERR    *p_err)
{
    CPU_INT32U  size_op;
    CPU_INT32U  addr;
    CPU_INT32U  result;
    CPU_INT32U  mask;
    MEM_ERR     mem_err;
    
    size_op = (args & 0x004000) >> 14;
    addr    =  args & 0x001FFF;
    
    mask    = Core_MaskGet(size_op, addr);

    result  = Mem_Get(p_mem_data, (addr & 0xFFFE), &mem_err);

    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    result  = Core_Align(result, mask);
    
    /* Update Status Register */
                                                                /* DC */

    p_core->SR &= ~(CORE_SR_DC);
    
    /* N */
    p_core->SR &= ~(CORE_SR_N);
    
    /* OV */
    p_core->SR &= ~(CORE_SR_OV);
    
    if (result == 0) {                                          /* Z */
        p_core->SR |=   CORE_SR_Z;
    } else {
        p_core->SR &= ~(CORE_SR_Z);
    }
    
                                                                /* C */
    p_core->SR &= ~(CORE_SR_C);
    
    Core_PC_Slide(p_core, 2);
    
    *p_err = CORE_ERR_NONE;
    
}

void Core_CP_WS_WB (MEM_24      *p_mem_prog,
                    MEM         *p_mem_data,
                    CORE_24F    *p_core,
                    CPU_INT32U   args,
                    OPCODE       instruction,
                    CORE_ERR    *p_err)
{
    CPU_INT32U  src_w;
    CPU_INT32U  size_op;
    CPU_INT32U  addr_mode;
    CPU_INT32U  dst_w;
    CPU_INT32U  result;
    CPU_INT32U  operand1;
    CPU_INT32U  operand2;
    CPU_INT32U  mask;
    MEM_ERR     mem_err;
    
    
    src_w     = (args & 0x007800) >> 11;
    size_op   = (args & 0x000400) >> 10;
    addr_mode = (args & 0x000070) >> 4;
    dst_w     =  args & 0x00000F;
    
    if (size_op != 0) {
        mask = 0xFF;
    } else {
        mask = 0xFFFF;
    }
    
    operand1  = p_core->W[src_w] & mask;
    
    mem_err = MEM_ERR_NONE;
    
    switch (addr_mode) {
        case CORE_OPC_ADDR_MODE_DIR:
            mask     = Core_MaskGet(size_op, 0);
            operand2 = p_core->W[dst_w] & mask;
            break;
            
        case CORE_OPC_ADDR_MODE_IND:
            mask     = Core_MaskGet(size_op, p_core->W[dst_w]);
            operand2 = Mem_Get(p_mem_data, (p_core->W[dst_w] & 0xFFFE), &mem_err);
            operand2 = Core_Align(operand2, mask);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_DEC:
            mask     = Core_MaskGet(size_op, p_core->W[dst_w]);
            operand2 = Mem_Get(p_mem_data, (p_core->W[dst_w] & 0xFFFE), &mem_err);
            operand2 = Core_Align(operand2, mask);
            p_core->W[src_w] -= 2;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_INC:
            mask     = Core_MaskGet(size_op, p_core->W[dst_w]);
            operand2 = Mem_Get(p_mem_data, (p_core->W[dst_w] & 0xFFFE), &mem_err);
            operand2 = Core_Align(operand2, mask);
            p_core->W[src_w] += 2;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_DEC:
            p_core->W[src_w] -= 2;
            mask     = Core_MaskGet(size_op, p_core->W[dst_w]);
            operand2 = Mem_Get(p_mem_data, (p_core->W[dst_w] & 0xFFFE), &mem_err);
            operand2 = Core_Align(operand2, mask);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_INC:
            p_core->W[src_w] += 2;
            mask     = Core_MaskGet(size_op, p_core->W[dst_w]);
            operand2 = Mem_Get(p_mem_data, (p_core->W[dst_w] & 0xFFFE), &mem_err);
            operand2 = Core_Align(operand2, mask);
            break;
            
        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return;
    }
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    switch (instruction) {
        case CORE_OPC_CP_WB_WS:
            result = operand1 - operand2;
            break;
            
        case CORE_OPC_CPB_WB_WS:
            result = operand1 - operand2;
            if (!Core_GetC(p_core)) {
                result--;
            }
            break;
            
        default:
            break;
    }
    
    
    /* Update Status Register */
    if (((operand1 & 0x00000080) &&                              /* DC */
         (result  & 0x00000080))) {
        p_core->SR |=   CORE_SR_DC;
    } else {
        p_core->SR &= ~(CORE_SR_DC);
    }
    
                                                                /* N */
    p_core->SR &= ~(CORE_SR_N);
    
                                                                /* OV */
    p_core->SR &= ~(CORE_SR_OV);
    
    if (result == 0) {                                          /* Z */
        p_core->SR |=   CORE_SR_Z;
    } else {
        p_core->SR &= ~(CORE_SR_Z);
    }
    
    if (((operand1 & 0x00008000) &&                              /* C */
         (result  & 0x00008000))) {
        p_core->SR |=   CORE_SR_C;
    } else {
        p_core->SR &= ~(CORE_SR_C);
    }
    
    Core_PC_Slide(p_core, 2);
    
    *p_err = CORE_ERR_NONE;
}

void Core_CP_E1006 (MEM_24      *p_mem_prog,
                    MEM         *p_mem_data,
                    CORE_24F    *p_core,
                    CPU_INT32U   args,
                    CORE_ERR    *p_err)
{
    CPU_INT32U  base_w;
    CPU_INT32U  size_op;
    CPU_INT32U  lit;
    CPU_INT16U  operand;
    CPU_INT16U  result;
    
    
    base_w  = (args & 0x007800) >> 11;
    size_op = (args & 0x000400) >> 10;
    lit     =  args & 0x00001F;
    
    operand = p_core->W[base_w];
    result  = operand - lit;
    
    /* Update Status Register */
    if (((operand & 0x00000080) &&                              /* DC */
         (result  & 0x00000080))) {
        p_core->SR |=   CORE_SR_DC;
    } else {
        p_core->SR &= ~(CORE_SR_DC);
    }
    
    if (result < 0) {                                          /* N */
        p_core->SR |=   CORE_SR_N;
    } else {
        p_core->SR &= ~(CORE_SR_N);
    }
    
    if ((operand <  0) &&                                       /* OV */
        (result  >= 0)) {
        p_core->SR |=   CORE_SR_OV;
    } else {
        p_core->SR &= ~(CORE_SR_OV);
    }
    
    if (result == 0) {                                         /* Z */
        p_core->SR |=   CORE_SR_Z;
    } else {
        p_core->SR &= ~(CORE_SR_Z);
    }
    
    if (((operand & 0x00008000) &&                              /* C */
         (result  & 0x00008000))) {
        p_core->SR |=   CORE_SR_C;
    } else {
        p_core->SR &= ~(CORE_SR_C);
    }
    
    Core_PC_Slide(p_core, 2);
    
    *p_err = CORE_ERR_NONE;
}

void Core_SETM_WS_EB8 (MEM_24      *p_mem_prog,
                       MEM         *p_mem_data,
                       CORE_24F    *p_core,
                       CPU_INT32U   args,
                       CORE_ERR    *p_err)
{
    CPU_INT32U  size_op;
    CPU_INT32U  addr_mode;
    CPU_INT32U  dest_w;
    CPU_INT32U  offset;
    CPU_INT32U  value;
    MEM_ERR     mem_err;
    
    
    size_op   = (args & 0x004000) >> 14;
    addr_mode = (args & 0x003800) >> 11;
    dest_w    = (args & 0x000780) >>  7;
    
    if (size_op != 0) {
        offset = 1;
        value  = 0xFF;
        *p_err = CORE_ERR_OPC_UNSUPORTED_YET;
        return;
    } else {
        offset = 2;
        value  = 0xFFFF;
    }
    
    mem_err = MEM_ERR_NONE;
    
    switch (addr_mode) {
        case CORE_OPC_ADDR_MODE_DIR:
            p_core->W[dest_w] = value;
            break;
            
        case CORE_OPC_ADDR_MODE_IND:
            Mem_Set(p_mem_data, p_core->W[dest_w], value, &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_DEC:
            Mem_Set(p_mem_data, p_core->W[dest_w], value, &mem_err);
            p_core->W[dest_w] -= offset;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_INC:
            Mem_Set(p_mem_data, p_core->W[dest_w], value, &mem_err);
            p_core->W[dest_w] += offset;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_DEC:
            p_core->W[dest_w] -= offset;
            Mem_Set(p_mem_data, p_core->W[dest_w], value, &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_INC:
            p_core->W[dest_w] += offset;
            Mem_Set(p_mem_data, p_core->W[dest_w], value, &mem_err);
            break;
            
        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return;
    }
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    Core_PC_Slide(p_core, 2);
    
    *p_err = CORE_ERR_NONE;
}

void Core_INC_EC0 (MEM_24      *p_mem_prog,
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
    
    Core_PC_Slide(p_core, 2);
    *p_err = CORE_ERR_NONE;
}

void Core_Logical_Shift_W_L_W  (MEM_24      *p_mem_prog,
                          MEM         *p_mem_data,
                          CORE_24F    *p_core,
                          CPU_INT32U   args,
                          OPCODE       instruction,
                          CORE_ERR    *p_err)
{
    CPU_INT32U  src_w;
    CPU_INT32U  dst_w;
    CPU_INT32U  lit;
    CPU_INT32S  value_0S;
    CPU_INT32U  value_0U;
    CPU_INT32S  value_1;
    
    
    src_w = (args & 0x007800) >> 11;
    dst_w = (args & 0x000780) >>  7;
    lit   =  args & 0x00000F;
    
    
    
    switch (instruction) {
        case CORE_OPC_ASR_W_L_W:
            value_0S = CPU_SignExt16(p_core->W[src_w]);
            value_1  = value_0S >> lit;
            break;
            
        case CORE_OPC_LSR_W_L_W:
            value_0U = p_core->W[src_w];
            value_1  = value_0U >> lit;
            break;
            
        case CORE_OPC_SL_W_L_W:
            value_0U = p_core->W[src_w];
            value_1  = value_0U << lit;
            break;
            
        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return;
    }
    
    p_core->W[dst_w] = value_1;
    
    if ((value_1 & 0x8000) != 0) {
        p_core->SR |=   CORE_SR_N;
    } else {
        p_core->SR &= ~(CORE_SR_N);
    }
    
    if (value_1 == 0) {
        p_core->SR |=   CORE_SR_Z;
    } else {
        p_core->SR &= ~(CORE_SR_Z);
    }
    
    Core_PC_Slide(p_core, 2);
    *p_err = CORE_ERR_NONE;
}

void Core_CLR_WD_EB0000 (MEM_24      *p_mem_prog,
                         MEM         *p_mem_data,
                         CORE_24F    *p_core,
                         CPU_INT32U   args,
                         CORE_ERR    *p_err)
{
    CPU_INT32U  size_op;
    CPU_INT32U  addr_mode;
    CPU_INT32U  dest_w;
    CPU_INT32U  offset;
    MEM_ERR     mem_err;
    
    
    size_op   = (args & 0x004000) >> 15;
    addr_mode = (args & 0x003800) >> 11;
    dest_w    = (args & 0x000780) >>  7;
    
    if (size_op != 0) {
        offset = 1;
        *p_err = CORE_ERR_OPC_UNSUPORTED_YET;
        return;
    } else {
        offset = 2;
    }
    
    mem_err = MEM_ERR_NONE;
    
    switch (addr_mode) {
        case CORE_OPC_ADDR_MODE_DIR:
            p_core->W[dest_w] = 0;
            break;
            
        case CORE_OPC_ADDR_MODE_IND:
            Mem_Set(p_mem_data, p_core->W[dest_w], 0, &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_DEC:
            Mem_Set(p_mem_data, p_core->W[dest_w], 0, &mem_err);
            p_core->W[dest_w] -= offset;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_INC:
            Mem_Set(p_mem_data, p_core->W[dest_w], 0, &mem_err);
            p_core->W[dest_w] += offset;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_DEC:
            p_core->W[dest_w] -= offset;
            Mem_Set(p_mem_data, p_core->W[dest_w], 0, &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_INC:
            p_core->W[dest_w] += offset;
            Mem_Set(p_mem_data, p_core->W[dest_w], 0, &mem_err);
            break;
            
        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return;
    }
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    Core_PC_Slide(p_core, 2);
    
    *p_err = CORE_ERR_NONE;
}

void Core_SETM_M_W0_EF8 (MEM_24      *p_mem_prog,
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
    
    Core_PC_Slide(p_core, 2);
    *p_err = CORE_ERR_NONE;
}

void Core_CLR_M_W0_EF0 (MEM_24      *p_mem_prog,
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
    PERI_ERR    peri_err;
    
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
             DEF_FIELD_IS_CLR(size_op, 0x004000))) {

            Trap_Post(ISR_TRAP_NUM_ADDR, &peri_err);
            *p_err = CORE_ERR_ADDR_ERROR_TRAP;
            return;
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
    
    Core_PC_Slide(p_core, 2);
    *p_err = CORE_ERR_NONE;
}

void  Core_PUSH_F8 (MEM_24      *p_mem_prog,
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
    
    Core_PC_Slide(p_core, 2);
}

void  Core_POP_F9 (MEM_24      *p_mem_prog,
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
    
    Core_PC_Slide(p_core, 2);
}

void Core_SE_FB00       (MEM_24      *p_mem_prog,
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
    
    Core_PC_Slide(p_core, 2);
    *p_err = CORE_ERR_NONE;
}

void Core_ZE_FB8        (MEM_24      *p_mem_prog,
                         MEM         *p_mem_data,
                         CORE_24F    *p_core,
                         CPU_INT32U   args,
                         CORE_ERR    *p_err)
{
    CPU_INT32U  dst_addr_mode;
    CPU_INT32U  dst_w;
    CPU_INT32U  src_addr_mode;
    CPU_INT32U  src_w;
    CPU_INT32S  value;
    MEM_ERR     mem_err;


    dst_addr_mode = (args & 0x003800) >> 11;
    dst_w         = (args & 0x000780) >>  7;
    src_addr_mode = (args & 0x000070) >>  4;
    src_w         =  args & 0x00000F;

        mem_err = MEM_ERR_NONE;
    
    switch (src_addr_mode) {
        case CORE_OPC_ADDR_MODE_DIR:
            value = p_core->W[src_w];
            break;
            
        case CORE_OPC_ADDR_MODE_IND:
            value = Mem_Get(p_mem_data, p_core->W[src_w], &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_DEC:
            value = Mem_Get(p_mem_data, p_core->W[src_w], &mem_err);
            p_core->W[src_w] -= 2;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_INC:
            value = Mem_Get(p_mem_data, p_core->W[src_w], &mem_err);
            p_core->W[src_w] += 2;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_DEC:
            p_core->W[src_w] -= 2;
            value = Mem_Get(p_mem_data, p_core->W[src_w], &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_INC:
            p_core->W[src_w] += 2;
            value = Mem_Get(p_mem_data, p_core->W[src_w], &mem_err);
            break;
            
        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return;
    }
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
    
    value &= 0x00FF;
    
    switch (dst_addr_mode) {
        case CORE_OPC_ADDR_MODE_DIR:
            p_core->W[dst_w] = value;
            break;
            
        case CORE_OPC_ADDR_MODE_IND:
            Mem_Set(p_mem_data, p_core->W[dst_w], value, &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_DEC:
            Mem_Set(p_mem_data, p_core->W[dst_w], value, &mem_err);
            p_core->W[src_w] -= 2;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_POS_INC:
            Mem_Set(p_mem_data, p_core->W[dst_w], value, &mem_err);
            p_core->W[src_w] += 2;
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_DEC:
            p_core->W[src_w] -= 2;
            Mem_Set(p_mem_data, p_core->W[dst_w], value, &mem_err);
            break;
            
        case CORE_OPC_ADDR_MODE_IND_PRE_INC:
            p_core->W[src_w] += 2;
            Mem_Set(p_mem_data, p_core->W[dst_w], value, &mem_err);
            break;
            
        default:
            *p_err = CORE_ERR_INVALID_OPC_ARG;
            return;
    }
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }

        /* Update Status Register */
    
    if (value < 0) {                                          /* N */
        p_core->SR |=   CORE_SR_N;
    } else {
        p_core->SR &= ~(CORE_SR_N);
    }
    
    if (value == 0) {                                         /* Z */
        p_core->SR |=   CORE_SR_Z;
    } else {
        p_core->SR &= ~(CORE_SR_Z);
    }
    
    p_core->SR &= ~(CORE_SR_C);/* C */

     Core_PC_Slide(p_core, 2);
    *p_err = CORE_ERR_NONE;
}

void Core_SWAP_FD800    (MEM_24      *p_mem_prog,
                         MEM         *p_mem_data,
                         CORE_24F    *p_core,
                         CPU_INT32U   args,
                         CORE_ERR    *p_err)
{
    CPU_INT32U  size_op;
    CPU_INT32U  w_reg;
    CPU_INT32U  value;

    size_op = (args & 0x004000) >> 14;
    w_reg   =  args & 0x00000F;

    value = p_core->W[w_reg];

    if (size_op == 0) {
        value = ((value & 0xFF00) >> 8) | ((value & 0x00FF) << 8);
    } else {
        value = ((value & 0x00F0) >> 4) | ((value & 0x000F) << 4) | (value & 0xFF00);
    }

    p_core->W[w_reg] = value;

    Core_PC_Slide(p_core, 2);
    *p_err = CORE_ERR_NONE;
}



















