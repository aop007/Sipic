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
#include "main.h"

void HexParser_ReadFile(const char       *p_file_name,
                         MEM_24          *p_mem,
                         HEX_PARSER_ERR  *p_err)
{
    FILE        *p_file;
#ifdef  HEX_PARSER_PRINT_MAP
    FILE        *p_file_out;
    CPU_CHAR     buffer_out[256];
    CPU_INT32U   buf_len;
    OPCODE       opcode;
#endif
    
    MEM_HDR_24  *p_mem_hdr;
    CPU_CHAR     sol;
    CPU_INT08U   byte_count;
    CPU_INT16U   address_lo;
    CPU_INT16U   address_hi;
    CPU_INT32U   address;
    CPU_INT08U   record_type;
    CPU_INT32U   block[4];
    CPU_INT08U   check_sum;
    CPU_INT32U   ix;
    CPU_BOOLEAN  eof;
    MEM_ERR      mem_err;
    
    p_mem_hdr       = &p_mem->Hdr;
    address_hi      = 0u;
    eof             = DEF_NO;
    
    p_file     = fopen(p_file_name, "r");
#ifdef  HEX_PARSER_PRINT_MAP
    p_file_out = fopen("/Users/aop007/Documents/Projets/DawnStar/Sipic/Sipic/Sipic/InputFiles/main_memory_map.txt", "wb");
#endif
    
    if (p_file == NULL) {
        *p_err = HEX_PARSER_ERR_FILE_OPEN;
        return;
    }
    
    while (1) {
        sol = ASCII_GetChar(p_file);
        
        if (sol == HEX_PARSER_START_OF_LINE_CHAR) {
            byte_count     = ASCII_GetByte(p_file);
            address_lo     = ASCII_GetShort(p_file);
            address_lo     = CPU_Swap16(address_lo);
            address        = (address_hi << 16) | address_lo;
            
            //CORE_TRACE_DEBUG("\r\nAddress = %x", address);
            
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
            
            switch (record_type) {
                case HEX_PARSER_RCRD_TYPE_DATA:
                    
                    for (ix = 0 ; ix < (byte_count / 4) ; ix++) {
                        
                        Mem_Set24(p_mem,
                                  (address + ix * 4) / 2,
                                  block[ix],
                                 &mem_err);
                        
                        if (mem_err != MEM_ERR_NONE) {
                            CORE_TRACE_DEBUG(("\r\nMEM_Set() Error at %x", address + ix * 4));
                           *p_err = mem_err;
                        }

                        //printf("\r\n%x", block[ix]);
                    }
                    
                    break;
                    
                case HEX_PARSER_RCRD_TYPE_EOF:
                    eof = DEF_YES;
                    break;
                    
                case HEX_PARSER_RCRD_TYPE_ESA:
                    break;
                    
                case HEX_PARSER_RCRD_TYPE_SSAR:
                    break;
                    
                case HEX_PARSER_RCRD_TYPE_ELA:
                    address_hi = ASCII_GetShort(p_file);
                    address_hi = CPU_Swap16(address_hi);
                    break;
                    
                case HEX_PARSER_RCRD_TYPE_SLA:
                    break;
            }

            check_sum    = ASCII_GetByte(p_file);
            
            if (eof == DEF_YES) {
                break;
            }
            
        }
    }
#ifdef  HEX_PARSER_PRINT_MAP
    for (ix = 0 ; ix < 0x4000 ; ix += 2) {
        opcode  = Mem_Get24(p_mem, ix, &mem_err);
        buf_len = sprintf(&buffer_out[0], "\r\n%004x\t%006x", ix, opcode);
        fwrite(&buffer_out, 1, buf_len, p_file_out);
    }
    fclose(p_file_out);
#endif
    fclose(p_file);
    
    
    *p_err = HEX_PARSER_ERR_NONE;
}