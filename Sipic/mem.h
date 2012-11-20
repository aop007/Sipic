//
//  mem.h
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-05.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#ifndef Sipic_mem_h
#define Sipic_mem_h

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"
#include "mem_cfg.h"
#include "err.h"

typedef  CPU_INT32U  OPCODE;

//typedef  struct  mem;

struct mem;
struct mem_24;

typedef  struct  MEM_HDR {
    CPU_CHAR     Name[MEM_CFG_NAME_SIZE];
    struct  mem         *Next;
    struct  mem         *Prev;
    CPU_INT32U   Start;
    CPU_INT32U   End;
    CPU_INT32U   Size;                      /* Address locations. 23bit insturctions | 16 bit data. */
    CPU_INT16U   Index;
} MEM_HDR;

typedef  struct  MEM_HDR_24 {
    CPU_CHAR     Name[MEM_CFG_NAME_SIZE];
    struct  mem_24      *Next;
    struct  mem_24      *Prev;
    CPU_INT32U   Start;
    CPU_INT32U   End;
    CPU_INT32U   Size;                      /* Address locations. 23bit insturctions | 16 bit data. */
    CPU_INT16U   Index;
} MEM_HDR_24;

typedef  struct  mem {
    MEM_HDR      Hdr;
    CPU_INT16U * Ptr;
    CPU_INT16U * WrPtr;
} MEM;

typedef  struct  mem_24 {
    MEM_HDR_24   Hdr;
    CPU_INT32U * Ptr;
} MEM_24;

typedef  struct  mem_cfg {
    CPU_CHAR    *Name;
    CPU_INT32U   Start;
    CPU_INT32U   End;
} MEM_CFG;


#define  MEM_SIZE  sizeof(MEM);

MEM        *Mem_Init (const  MEM_CFG     *p_cfg,
                      CPU_INT32U    cfg_size,
                      MEM_ERR      *p_err);

MEM_24     *Mem_Init24 (const  MEM_CFG     *p_cfg,
                               CPU_INT32U    cfg_size,
                               MEM_ERR      *p_err);

void        Mem_Set  (MEM         *p_mem,
                      CPU_INT32U   addr,
                      CPU_INT16U   val,
                      MEM_ERR     *p_err);

void  Mem_Set24 (MEM_24      *p_mem,
                 CPU_INT32U   addr,
                 CPU_INT32U   val,
                 MEM_ERR     *p_err);

CPU_INT16U  Mem_Get  (MEM         *p_mem,
                      CPU_INT32U   addr,
                      MEM_ERR     *p_err);

CPU_INT32U  Mem_Get24(MEM_24      *p_mem,
                      CPU_INT32U   addr,
                      MEM_ERR     *p_err);

void       *Mem_GetAddr(MEM         *p_mem_data,
                        CPU_INT32U   addr,
                        MEM_ERR     *p_err);

void        Mem_SetAccess(MEM         *p_mem_data,
                          CPU_INT32U   addr,
                          CPU_INT16U   write_mask,
                          MEM_ERR     *p_err);
    
    void Mem_Load(void       *buffer,
                  CPU_INT32U  addr,
                  CPU_INT32U  len,
                  MEM         *p_mem_data,
                  MEM_ERR     *p_err);

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
}
#endif

#endif
