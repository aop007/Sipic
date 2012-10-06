//
//  hex_parser.c
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-05.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "hex_parser.h"
#include "hex_cfg.h"
#include "ascii.h"
#include "mem.h"
#include "sipic_cfg.h"

void HexParser_ReadFile(const char      *p_file_name,
                         MEM             *p_mem,
                         HEX_PARSER_ERR  *p_err)
{
    FILE        *p_file;
    CPU_CHAR     sol;
    CPU_INT08U   byte_count;
    CPU_INT16U   address;
    CPU_INT08U   record_type;
    CPU_INT32U   block[4];
    CPU_INT08U   check_sum;
    CPU_INT08U   ix;
    
    
    p_mem->Ptr  = (CPU_INT32U *)malloc(SIPIC_CFG_MEM_SIZE_OCTET);
    p_mem->Size =                      SIPIC_CFG_MEM_SIZE_OCTET;
    
    p_file = fopen(p_file_name, "r");
    
    if (p_file == NULL) {
        *p_err = HEX_PARSER_ERR_FILE_OPEN;
        return;
    }
    
    while (1) {
        sol = ASCII_GetChar(p_file);
        
        if (sol == HEX_PARSER_START_OF_LINE_CHAR) {
            byte_count  = ASCII_GetByte(p_file);
            address     = ASCII_GetShort(p_file);
            address     = CPU_Swap16(address);
            record_type = ASCII_GetByte(p_file);
            
            if (byte_count >= 4) {
                block[0] = ASCII_GetInt(p_file);
            }
            
            if (byte_count >= 8) {
                block[1] = ASCII_GetInt(p_file);
            }
            
            if (byte_count >= 12) {
                block[2] = ASCII_GetInt(p_file);
            }
            
            if (byte_count >= 16) {
                block[3] = ASCII_GetInt(p_file);
            }
            
            check_sum    = ASCII_GetByte(p_file);
            
            for (ix = 0 ; ix < (byte_count / 4) ; ix++) {
                p_mem->Ptr[address + ix * 4] = block[ix];
                printf("\r\n%x", block[ix]);
            }
        }
    }
}