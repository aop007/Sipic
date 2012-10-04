//
//  coff_parser.c
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-02.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#include <stdio.h>
#include <coff_parser.h>
#include <cpu.h>

void  CoffParser_ReadFile(const char       *p_file_name,
                          COFF_PARSER_ERR  *p_err)
{
    FILE        *p_file;
    CPU_INT16U   stream_err;
    FILE_HDR     file_hdr;
    SECTION_HDR  section_hdr_tbl[5];
    SECTION_HDR *p_sec_hdr;
    CPU_INT16U   section_iterator;
    
                                                                /* Open File. */
    p_file = fopen(p_file_name, "r");
    
    if (p_file == NULL) {
       *p_err = COFF_PARSER_ERR_FILE_OPEN;
        return;
    }
    
                                                                /* Read File Header. */
    stream_err = fread(&file_hdr,
                        1u,
                        FILE_HDR_SIZE,
                        p_file);
    
    if (stream_err != FILE_HDR_SIZE) {
       *p_err = COFF_PARSER_ERR_FILE_READ;
        stream_err = fclose(p_file);
        return;
    }
    
    p_sec_hdr = &section_hdr_tbl[0];
    
    for (section_iterator = 0 ; section_iterator < file_hdr.f_nscns ; section_iterator++) {
        stream_err = fread(p_sec_hdr,
                           1u,
                           SECTION_HDR_SIZE,
                           p_file);
        
        if (stream_err != SECTION_HDR_SIZE) {
            *p_err = COFF_PARSER_ERR_FILE_READ;
            stream_err = fclose(p_file);
            return;
        }
        
        p_sec_hdr++;
    }
    
    
    
    stream_err = fclose(p_file);
    
    *p_err = COFF_PARSER_ERR_NONE;
}