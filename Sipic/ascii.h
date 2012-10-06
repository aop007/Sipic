//
//  ascii.h
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-05.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#ifndef Sipic_ascii_h
#define Sipic_ascii_h

#include <stdio.h>
#include "cpu.h"

CPU_CHAR    ASCII_GetChar  (FILE  *p_file);
CPU_INT08U  ASCII_GetByte  (FILE  *p_file);
CPU_INT16U  ASCII_GetShort (FILE  *p_file);
CPU_INT32U  ASCII_GetInt   (FILE  *p_file);


#endif
