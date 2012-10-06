//
//  main.c
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-02.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#include <stdio.h>
#include "sipic_cfg.h"
#include "mem.h"

#if    (SIPIC_CFG_PARSE_METHOD == SIPIC_PARSE_METHOD_COFF)
#include <coff_parser.h>
#include <coff_cfg.h>
#elif  (SIPIC_CFG_PARSE_METHOD == SIPIC_PARSE_METHOD_HEX)
#include <hex_parser.h>
#include <hex_cfg.h>
#endif


int main(int argc, const char * argv[])
{
    MEM  mem;
#if  (SIPIC_CFG_PARSE_METHOD == SIPIC_PARSE_METHOD_COFF)
    COFF_PARSER_ERR  coff_err;

    CoffParser_ReadFile(COFF_CFG_FILE_PATH,
                       &coff_err);
#elif (SIPIC_CFG_PARSE_METHOD == SIPIC_PARSE_METHOD_HEX)
    HEX_PARSER_ERR  err;
    
    HexParser_ReadFile(HEX_CFG_FILE_PATH,
                       &mem,
                       &err);
#endif
    
    // insert code here...
    printf("Hello, World!\n");
    return 0;
}

