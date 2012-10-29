//
//  hex_parser.h
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-05.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#ifndef Sipic_hex_parser_h
#define Sipic_hex_parser_h

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

#include "cpu.h"
#include "hex_cfg.h"
#include "mem.h"
#include "err.h"

#define  HEX_PARSER_START_OF_LINE_CHAR  ':'

#define  HEX_PARSER_RCRD_TYPE_DATA  0u
#define  HEX_PARSER_RCRD_TYPE_EOF   1u
#define  HEX_PARSER_RCRD_TYPE_ESA   2u
#define  HEX_PARSER_RCRD_TYPE_SSAR  3u
#define  HEX_PARSER_RCRD_TYPE_ELA   4u
#define  HEX_PARSER_RCRD_TYPE_SLA   5u


/*
 ********** TYPE DEFINITION *********
 */


/*
 ******** FUNCTION PROTOTYPE ********
 */

void  HexParser_ReadFile(const  char            *p_file_name,
                                MEM_24          *p_mem,
                                HEX_PARSER_ERR  *p_err);

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
}
#endif

#endif
