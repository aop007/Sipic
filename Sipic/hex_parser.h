//
//  hex_parser.h
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-05.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#ifndef Sipic_hex_parser_h
#define Sipic_hex_parser_h

#include "cpu.h"
#include "hex_cfg.h"
#include "mem.h"

#define  HEX_PARSER_START_OF_LINE_CHAR  ':'

/*
 ************* ERROR TYPE ***********
 */

#define  HEX_PARSER_ERR_NONE        2000u
#define  HEX_PARSER_ERR_FILE_OPEN   2001u
#define  HEX_PARSER_ERR_FILE_READ   2002u

#define  HEX_PARSER_ERR_MEM_ALLOC   2101u


/*
 ********** TYPE DEFINITION *********
 */

typedef  CPU_INT16U        HEX_PARSER_ERR;


/*
 ******** FUNCTION PROTOTYPE ********
 */

void  HexParser_ReadFile(const  char            *p_file_name,
                                MEM             *p_mem,
                                HEX_PARSER_ERR  *p_err);

#endif
