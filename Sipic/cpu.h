//
//  cpu.h
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-02.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#ifndef Sipic_cpu_h
#define Sipic_cpu_h

#include "err.h"

#define  DEF_NO     0u
#define  DEF_FAIL   0u
#define  DEF_FALSE  0u

#define  DEF_YES    1u
#define  DEF_OK     1u
#define  DEF_TRUE   1u


/*
 ********** TYPE DEFINITION *********
 */

typedef  unsigned  char   CPU_BOOLEAN;

typedef  unsigned  char   CPU_INT08U;
typedef            char   CPU_CHAR;
typedef    signed  char   CPU_INT08S;

typedef  unsigned  short  CPU_INT16U;
typedef    signed  short  CPU_INT16S;

typedef  unsigned  int    CPU_INT32U;
typedef    signed  int    CPU_INT32S;

typedef  unsigned  long   CPU_INT64U;
typedef    signed  long   CPU_INT64S;


CPU_INT16U  CPU_Swap16 (CPU_INT16U  val);

CPU_INT32U  CPU_Swap32 (CPU_INT32U  val);

#define  DEF_FIELD_IS_SET(x, y) ((x & y) == y)
#define  DEF_FIELD_IS_CLR(x, y) ((x & y) == 0)

CPU_INT32S  CPU_SignExt08 (CPU_INT32U);
CPU_INT32S  CPU_SignExt16 (CPU_INT32U);

#endif
