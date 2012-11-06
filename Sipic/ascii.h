//
//  ascii.h
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-05.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#ifndef Sipic_ascii_h
#define Sipic_ascii_h

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

#include <stdio.h>
#include "cpu.h"
#include "err.h"

CPU_CHAR    ASCII_GetChar  (FILE  *p_file);
CPU_INT08U  ASCII_GetByte  (FILE  *p_file);
CPU_INT16U  ASCII_GetShort (FILE  *p_file);
CPU_INT32U  ASCII_GetInt   (FILE  *p_file);

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
}
#endif

#endif
