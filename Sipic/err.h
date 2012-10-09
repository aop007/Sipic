//
//  err.h
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-06.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#ifndef Sipic_err_h
#define Sipic_err_h

/*
 * COFF PARSER ERRORS
 */

#define  COFF_PARSER_ERR_NONE         1000u
#define  COFF_PARSER_ERR_FILE_OPEN    1001u
#define  COFF_PARSER_ERR_FILE_READ    1002u

#define  COFF_PARSER_ERR_MEM_ALLOC    1101u

/*
 * HEX PARSER ERRORS
 */

#define  HEX_PARSER_ERR_NONE          2000u
#define  HEX_PARSER_ERR_FILE_OPEN     2001u
#define  HEX_PARSER_ERR_FILE_READ     2002u

#define  HEX_PARSER_ERR_MEM_ALLOC     2101u

/*
 * MEM ERRORS
 */

#define  MEM_ERR_NONE                 3000u
#define  MEM_ERR_ALLOC_FAIL           3001u
#define  MEM_ERR_INVALID_LOC          3002u

/*
 * CORE ERRORS
 */

#define  CORE_ERR_NONE                4000u
#define  CORE_ERR_OPC_NOTFOUND        4001u
#define  CORE_ERR_INVALID_MEM         4002u
#define  CORE_ERR_INVALID_OPC_ARG     4003u
#define  CORE_ERR_OPC_UNSUPORTED_YET  4004u

#endif
