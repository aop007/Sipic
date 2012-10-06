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

typedef  struct  mem {
    CPU_INT32U * Ptr;
    CPU_INT32U   Size;
} MEM;

#define  MEM_SIZE  sizeof(MEM);

#endif
