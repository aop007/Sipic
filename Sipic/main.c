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
#include "core_24f.h"
#include "core_24f_opcode.h"

#if    (SIPIC_CFG_PARSE_METHOD == SIPIC_PARSE_METHOD_COFF)
#include <coff_parser.h>
#include <coff_cfg.h>
#elif  (SIPIC_CFG_PARSE_METHOD == SIPIC_PARSE_METHOD_HEX)
#include <hex_parser.h>
#include <hex_cfg.h>
#endif


int main(int argc, const char * argv[])
{
    MEM      *p_mem;
    MEM_ERR   mem_err;
    CORE_ERR  core_err;
    
    p_mem = Mem_Init(&mem_err);
    
#if  (SIPIC_CFG_PARSE_METHOD == SIPIC_PARSE_METHOD_COFF)
    COFF_PARSER_ERR  coff_err;

    CoffParser_ReadFile(COFF_CFG_FILE_PATH,
                       &coff_err);
#elif (SIPIC_CFG_PARSE_METHOD == SIPIC_PARSE_METHOD_HEX)
    HEX_PARSER_ERR  hex_err;
    
    HexParser_ReadFile(HEX_CFG_FILE_PATH,
                        p_mem,
                       &hex_err);
    
    if (hex_err != HEX_PARSER_ERR_NONE) {
        printf("\r\nError in HexParser_ReadFile().");
        return -1;
    }
#ifdef  CORE_STAT_EN
    Core_OPC_Stats(p_mem);
#endif
    
    Core_Run(p_mem,
            &core_err);
#endif
    return 0;
}

