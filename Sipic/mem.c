//
//  mem.c
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-05.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mem.h"

static  CPU_INT32U  Mem_GetSegSize(const MEM_CFG  *p_cfg);



MEM * Mem_Init(const  MEM_CFG     *p_cfg,
                      CPU_INT32U       cfg_size,
                      MEM_ERR       *p_err)
{
    CPU_INT16U  mem_seg_cnt;
    CPU_INT16U  ix;
    MEM         *p_mem_head;
    MEM         *p_mem;
    MEM         *p_mem_prev;
    MEM_HDR     *p_mem_hdr;
    CPU_INT32U   seg_size;
    
    
    mem_seg_cnt = cfg_size / sizeof(MEM_CFG);
    p_mem_prev  = (MEM *)0;
    
    
    for (ix = 0 ; ix < mem_seg_cnt ; ix++) {
        
        p_mem = malloc(sizeof(MEM));
        
        if (p_mem == NULL) {
            *p_err = MEM_ERR_ALLOC_FAIL;
            return (MEM *)0;
        }
        
        p_mem_hdr = &p_mem->Hdr;
        
        seg_size   = Mem_GetSegSize(p_cfg);
        p_mem->Ptr = malloc(seg_size * 4);
        
        if (p_mem == NULL) {
            *p_err = MEM_ERR_ALLOC_FAIL;
             return (MEM *)0;
        }
        
        p_mem_hdr->Start = p_cfg->Start;
        p_mem_hdr->End   = p_cfg->End;
        p_mem_hdr->Size  = seg_size;
        p_mem_hdr->Index = ix;
        p_mem_hdr->Prev  = p_mem_prev;
        strcpy(p_mem_hdr->Name, p_cfg->Name);
        
        if (p_mem_prev != NULL) {
            p_mem_prev->Hdr.Next = p_mem;
        }
        
        if (ix == 0) {
            p_mem_head = p_mem;
        }
        
        p_mem_prev   = p_mem;
        p_cfg++;
    }
    
    p_mem->Hdr.Next      = p_mem_head;
    p_mem_head->Hdr.Prev = p_mem;
    
    return p_mem_head;
}

void  Mem_Set (MEM         *p_mem,
               CPU_INT32U   addr,
               CPU_INT32U   val,
               MEM_ERR     *p_err)
{
    MEM         *p_mem_current;
    MEM_HDR     *p_mem_hdr;
    CPU_INT16U   seg_cnt;
    CPU_INT16U   ix;
    CPU_BOOLEAN  mem_loc_found;
    
    
    mem_loc_found =  DEF_NO;
    p_mem_hdr     = &p_mem->Hdr.Prev->Hdr;
    seg_cnt       =  p_mem_hdr->Index + 1;
    p_mem_current =  p_mem;
    
    for (ix = 0 ; ix < seg_cnt ; ix++) {
        p_mem_hdr = &p_mem_current->Hdr;
        
        if ((addr >= p_mem_hdr->Start) &&
            (addr <= p_mem_hdr->End)) {

#if 0
            if (p_mem_current->Ptr[(addr - p_mem_hdr->Start) / 2] == 0u) {
#endif
            p_mem_current->Ptr[(addr - p_mem_hdr->Start) / 2] = val;
#if 0
            } else {
                printf("\r\nCorrupting Memory!!!");
            }
#endif
#ifdef  NO_VERBOSE
            printf("\r\nMemSet @%004x <= %004x\r\n",addr, val);
#endif
            mem_loc_found = DEF_YES;
            break;
            
        } else {
            p_mem_current = p_mem_hdr->Next;
        }
    }
    
    if (mem_loc_found == DEF_NO) {
        *p_err = MEM_ERR_INVALID_LOC;
    } else {
        *p_err = MEM_ERR_NONE;
    }
}

CPU_INT32U  Mem_Get(MEM         *p_mem,
                    CPU_INT32U   addr,
                    MEM_ERR     *p_err)
{
    MEM         *p_mem_current;
    MEM_HDR     *p_mem_hdr;
    CPU_INT16U   seg_cnt;
    CPU_INT16U   ix;
    CPU_BOOLEAN  mem_loc_found;
    
    
    mem_loc_found =  DEF_NO;
    p_mem_hdr     = &p_mem->Hdr.Prev->Hdr;
    seg_cnt       =  p_mem_hdr->Index + 1;
    p_mem_current =  p_mem;
    
    for (ix = 0 ; ix < seg_cnt ; ix++) {
        p_mem_hdr = &p_mem_current->Hdr;
        
        if ((addr >= p_mem_hdr->Start) &&
            (addr <= p_mem_hdr->End)) {
#if 0
            if (p_mem_current->Ptr[(addr - p_mem_hdr->Start) / 2] == 0u) {
                p_mem_current->Ptr[(addr - p_mem_hdr->Start) / 2] = val;
            } else {
                printf("\r\nCorrupting Memory!!!");
            }
#else
            *p_err = MEM_ERR_NONE;
            return (p_mem_current->Ptr[(addr - p_mem_hdr->Start) / 2]);
#endif
            mem_loc_found = DEF_YES;
            break;
            
        } else {
            p_mem_current = p_mem_hdr->Next;
        }
    }
    
    if (mem_loc_found == DEF_NO) {
        *p_err = MEM_ERR_INVALID_LOC;
    } else {
        *p_err = MEM_ERR_NONE;
    }
    
    return (0u);
}

CPU_INT32U  Mem_GetSegSize(const MEM_CFG *p_cfg)
{
    return ((p_cfg->End - p_cfg->Start + 2) / 2);
}