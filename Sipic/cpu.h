//
//  cpu.h
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-02.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#ifndef Sipic_cpu_h
#define Sipic_cpu_h

/*
 ********** TYPE DEFINITION *********
 */

typedef  unsigned  char   CPU_BOOLEAN;

typedef  unsigned  char   CPU_INT08U;
typedef    signed  char   CPU_INT08S;

typedef  unsigned  short  CPU_INT16U;
typedef    signed  short  CPU_INT16S;

typedef  unsigned  int    CPU_INT32U;
typedef    signed  int    CPU_INT32S;


CPU_INT16U  CPU_Swap16 (CPU_INT16U  val);

CPU_INT32U  CPU_Swap32 (CPU_INT32U  val);

#endif
