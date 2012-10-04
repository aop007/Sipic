//
//  cpu.c
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-03.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#include <stdio.h>
#include <cpu.h>

CPU_INT16U  CPU_Swap16 (CPU_INT16U  val)
{
    return (((0xFF00 & val) >> 8) | ((0x00FF & val) << 8));
}

CPU_INT32U  CPU_Swap32 (CPU_INT32U  val)
{
    return (((0xFF000000 & val) >> 24) |
            ((0x00FF0000 & val) >>  8) |
            ((0x0000FF00 & val) <<  8) |
            ((0x000000FF & val) <<  24));
}