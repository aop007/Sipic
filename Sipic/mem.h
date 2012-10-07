//
//  mem.h
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-05.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#ifndef Sipic_mem_h
#define Sipic_mem_h

#include "cpu.h"
#include "mem_cfg.h"
#include "err.h"

typedef  CPU_INT32U  OPCODE;
typedef  CPU_INT16U  MEM_ERR;

//typedef  struct  mem;

struct mem;

typedef  struct  MEM_HDR {
            CPU_CHAR     Name[MEM_CFG_NAME_SIZE];
    struct  mem         *Next;
    struct  mem         *Prev;
            CPU_INT32U   Start;
            CPU_INT32U   End;
            CPU_INT32U   Size;                      /* Address locations. 23bit insturctions | 16 bit data. */
            CPU_INT16U   Index;
} MEM_HDR;

typedef  struct  mem {
    MEM_HDR      Hdr;
    CPU_INT32U * Ptr;
} MEM;

typedef  struct  mem_cfg {
    CPU_CHAR    *Name;
    CPU_INT32U   Start;
    CPU_INT32U   End;
} MEM_CFG;


#define  MEM_SIZE  sizeof(MEM);

MEM   *Mem_Init(MEM_ERR     *p_err);

void        Mem_Set (MEM         *p_mem,
                     CPU_INT32U   addr,
                     CPU_INT32U   val,
                     MEM_ERR     *p_err);

CPU_INT32U  Mem_Get (MEM         *p_mem,
                     CPU_INT32U   addr,
                     MEM_ERR     *p_err);

#endif
