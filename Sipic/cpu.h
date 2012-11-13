//
//  cpu.h
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-02.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#ifndef Sipic_cpu_h
#define Sipic_cpu_h

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

#include "err.h"

#define  DEF_NO     0u
#define  DEF_FAIL   0u
#define  DEF_FALSE  0u

#define  DEF_YES    1u
#define  DEF_OK     1u
#define  DEF_TRUE   1u
    
#define  DEF_BIT_15     0x8000
#define  DEF_BIT_14     0x4000
#define  DEF_BIT_13     0x2000
#define  DEF_BIT_12     0x1000
#define  DEF_BIT_11     0x0800
#define  DEF_BIT_10     0x0400
#define  DEF_BIT_09     0x0200
#define  DEF_BIT_08     0x0100
#define  DEF_BIT_07     0x0080
#define  DEF_BIT_06     0x0040
#define  DEF_BIT_05     0x0020
#define  DEF_BIT_04     0x0010
#define  DEF_BIT_03     0x0008
#define  DEF_BIT_02     0x0004
#define  DEF_BIT_01     0x0002
#define  DEF_BIT_00     0x0001
#define  DEF_BIT_NONE   0x0000

#define  PI             3.1415926535897932384626433
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
    
#define  CPU_MAKE_TYPE(a,b,c,d)  (a << 24 | b << 16 | c << 8 | d)

CPU_INT32S  CPU_SignExt08 (CPU_INT32U);
CPU_INT32S  CPU_SignExt16 (CPU_INT32U);
    
CPU_INT32S  CPU_SignExt10 (CPU_INT32U);

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
}
#endif

#endif
