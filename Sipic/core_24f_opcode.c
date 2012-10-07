//
//  core_24f_opcode.c
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-06.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "core_24f_opcode.h"

#if 0
void Core_Decode(OPCODE opc)
{
    OPCODE  High_Nib;
    
    High_Nib = opc & 0x00FF000000;
    
    switch (High_Nib) {
        
    }
}
#endif

void Core_OPC_Stats(MEM * p_mem)
{
#if 1
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
#endif
}

void Core_GOTO_04(MEM         *p_mem,
                  CORE_24F    *p_core,
                  CPU_INT32U   args,
                  CORE_ERR    *p_err)
{
    MEM_ERR     mem_err;
    OPCODE      next_word;
    CPU_INT32U  next_PC;
    
    next_word = Mem_Get(p_mem, p_core->PC + 2, &mem_err);
    
    if (mem_err != MEM_ERR_NONE) {
        *p_err = CORE_ERR_INVALID_MEM;
        return;
    }
            
    next_PC   = ((next_word & 0x00007F) << 16) | (args & 0x00FFFE);
    
    p_core->PC = next_PC;
    
    *p_err = CORE_ERR_NONE;
}

void Core_MOV_2(CORE_24F    *p_core,
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

void Core_MOV_M_W_80 (MEM         *p_mem,
                      CORE_24F    *p_core,
                      CPU_INT32U   args,
                      CORE_ERR    *p_err)
{
#error "Implement me!"
}

void Core_MOV_W_M_88 (MEM         *p_mem,
                      CORE_24F    *p_core,
                      CPU_INT32U   args,
                      CORE_ERR    *p_err)
{
#error "Implement me!"
}



