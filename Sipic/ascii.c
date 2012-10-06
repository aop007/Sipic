//
//  ascii.c
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-05.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#include <stdio.h>
#include "ascii.h"

CPU_INT08U  ASCII_GetNibble  (CPU_CHAR c)
{
    switch (c) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return (c - '0');

            
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            return (c - 'a' + 10);
            
            
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
            return (c - 'A' + 10);
            
            
        default:
            return (0);

    }
}

CPU_CHAR  ASCII_GetChar  (FILE  *p_file)
{
    CPU_CHAR  value;
    
    if (p_file != NULL) {
        fread(&value, 1u, sizeof(value), p_file);
    }
    
    return value;
}

CPU_INT08U  ASCII_GetByte  (FILE  *p_file)
{
    CPU_INT08U  value;
    CPU_CHAR    array[2 * sizeof(value)];
    CPU_INT16U  ix;
    CPU_INT32U  multiplier;
    
    value      = 0u;
    multiplier = 1u;
    
    if (p_file != NULL) {
        fread(&array, 1u, sizeof(array), p_file);
    }
    
    for (ix = 0 ; ix < sizeof(value) ; ix++ ) {
        value      += (multiplier * 16) * ASCII_GetNibble(array[ix * 2]);
        value      += (multiplier     ) * ASCII_GetNibble(array[ix * 2 + 1]);
        
        multiplier *= 256;
    }
    
    return (value);
}

CPU_INT16U  ASCII_GetShort (FILE  *p_file)
{
    CPU_INT16U  value;
    CPU_CHAR    array[2 * sizeof(value)];
    CPU_INT16U  ix;
    CPU_INT32U  multiplier;
    
    value      = 0u;
    multiplier = 1u;
    
    if (p_file != NULL) {
        fread(&array, 1u, sizeof(array), p_file);
    }
    
    for (ix = 0 ; ix < sizeof(value) ; ix++ ) {
        value      += (multiplier * 16) * ASCII_GetNibble(array[ix * 2]);
        value      += (multiplier     ) * ASCII_GetNibble(array[ix * 2 + 1]);
        
        multiplier *= 256;
    }
    
    return (value);
}

CPU_INT32U  ASCII_GetInt   (FILE  *p_file)
{
    CPU_INT32U  value;
    CPU_CHAR    array[2 * sizeof(value)];
    CPU_INT16U  ix;
    CPU_INT32U  multiplier;
    
    value      = 0u;
    multiplier = 1u;
    
    if (p_file != NULL) {
        fread(&array, 1u, sizeof(array), p_file);
    }
    
    for (ix = 0 ; ix < sizeof(value) ; ix++ ) {
        value      += (multiplier * 16) * ASCII_GetNibble(array[ix * 2]);
        value      += (multiplier     ) * ASCII_GetNibble(array[ix * 2 + 1]);
        
        multiplier *= 256;
    }
    
    return (value);
}
