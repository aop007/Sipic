//
//  err.h
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-06.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#ifndef Sipic_err_h
#define Sipic_err_h

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

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
    CORE_ERR_NONE                = 4000u,
    CORE_ERR_OPC_NOTFOUND        = 4001u,
    CORE_ERR_INVALID_MEM         = 4002u,
    CORE_ERR_INVALID_OPC_ARG     = 4003u,
    CORE_ERR_OPC_UNSUPORTED_YET  = 4004u,
    CORE_ERR_INVALID_OPC_CYCLE   = 4005u,
    CORE_ERR_INVALID_MATH_OP     = 4006u,
    
    CORE_ERR_OSCI_ERROR_TRAP     = 4100u,
    CORE_ERR_ADDR_ERROR_TRAP     = 4200u,
    CORE_ERR_MATH_ERROR_TRAP     = 4300u,
    CORE_ERR_STAK_ERROR_TRAP     = 4400u
    
};

typedef enum core_err CORE_ERR;

/*
 * PERI_ERR ERRORS
 */

enum peri_err {
    PERI_ERR_NONE               = 5000u,
    PERI_ERR_INVALID_MEM        = 5001u,
    PERI_ERR_INVALID_CORE_OP    = 5002u,
    PERI_ERR_ALLOC              = 5003u,
    PERI_ERR_INVALID_PERI_TYPE  = 5004u
};

typedef enum peri_err PERI_ERR;

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
}
#endif

#endif
