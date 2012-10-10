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

enum coff_parser_err {
    COFF_PARSER_ERR_NONE      = 1000u,
    COFF_PARSER_ERR_FILE_OPEN = 1001u,
    COFF_PARSER_ERR_FILE_READ = 1002u,
    
    COFF_PARSER_ERR_MEM_ALLOC = 1101u
};

typedef enum coff_parser_err COFF_PARSER_ERR;

/*
 * HEX PARSER ERRORS
 */
enum hex_parser_err {
    HEX_PARSER_ERR_NONE      = 2000u,
    HEX_PARSER_ERR_FILE_OPEN = 2001u,
    HEX_PARSER_ERR_FILE_READ = 2002u,
    
    HEX_PARSER_ERR_MEM_ALLOC = 2101u
};

typedef enum hex_parser_err HEX_PARSER_ERR;

/*
 * MEM ERRORS
 */

enum mem_err {
    MEM_ERR_NONE        = 3000u,
    MEM_ERR_ALLOC_FAIL  = 3001u,
    MEM_ERR_INVALID_LOC = 3002u,
};

typedef enum mem_err MEM_ERR;

/*
 * CORE ERRORS
 */

enum core_err {
    CORE_ERR_NONE               = 4000u,
    CORE_ERR_OPC_NOTFOUND       = 4001u,
    CORE_ERR_INVALID_MEM        = 4002u,
    CORE_ERR_INVALID_OPC_ARG    = 4003u,
    CORE_ERR_OPC_UNSUPORTED_YET = 4004u,
    CORE_ERR_INVALID_OPC_CYCLE  = 4005u
};

typedef enum core_err CORE_ERR;

#endif
