//
//  coff_parser.c
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-02.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#include <stdio.h>
#include <coff_parser.h>
#include <coff_cfg.h>
#include <cpu.h>
#include <stdlib.h>


/*
 ******** LOCAL FUNCTION PROTOTYPE ********
 */

static  void  CoffParser_SwapSectionHdr (SECTION          *p_section);

static  void  CoffParser_LoadSection    (SECTION          *p_section,
                                         FILE             *p_file,
                                         COFF_PARSER_ERR  *p_err);

/*
 ******** CoffParser_ReadFile() ********
 */

void  CoffParser_ReadFile(const char       *p_file_name,
                          COFF_PARSER_ERR  *p_err)
{
    FILE        *p_file;
    CPU_INT16U   stream_err;
    FILE_HDR     file_hdr;
    SECTION      section[SECTION_COUNT];
    SECTION     *p_section;
    CPU_INT16U   section_iterator;
    CPU_INT08U   optional_hdr[64];
    
    
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
    
                                                                /* Read optional header. */
    if (file_hdr.f_opthdr > 0) {
        stream_err = fread(&optional_hdr[0],
                           1u,
                           file_hdr.f_opthdr,
                           p_file);
        
        if (stream_err != file_hdr.f_opthdr) {
            *p_err = COFF_PARSER_ERR_FILE_READ;
            stream_err = fclose(p_file);
            return;
        }
    }
    
    //p_sec_hdr = &section_hdr_tbl[0];
    p_section = &section[0];
                                                                /* Read Section Header. */
    for (section_iterator = 0 ; section_iterator < file_hdr.f_nscns ; section_iterator++) {
        stream_err = fread(&p_section->section,
                            1u,
                            SECTION_HDR_SIZE,
                            p_file);
        
        if (stream_err != SECTION_HDR_SIZE) {
            *p_err = COFF_PARSER_ERR_FILE_READ;
            stream_err = fclose(p_file);
            return;
        }
        
        CoffParser_SwapSectionHdr(p_section);
        
        p_section->p_data = (CPU_INT08U *)0;
        p_section++;
    }
    
    p_section = &section[0];
    
    for (section_iterator = 0 ; section_iterator < file_hdr.f_nscns ; section_iterator++) {
        CoffParser_LoadSection(p_section,
                               p_file,
                              &stream_err);
        
        switch (stream_err) {
            case COFF_PARSER_ERR_MEM_ALLOC:
            case COFF_PARSER_ERR_FILE_READ:
                stream_err = fclose(p_file);
                return;
                
                
            default:
                break;
        }
        
        p_section++;
    }
    
    stream_err = fclose(p_file);
    
    *p_err = COFF_PARSER_ERR_NONE;
}

/*
 ******** CoffParser_SwapSectionHdr() ********
 */

static  void  CoffParser_SwapSectionHdr(SECTION  *p_section)
{
    SECTION_HDR  *p_sec_hdr;
    
    
    p_sec_hdr = &p_section->section;
    
    p_sec_hdr->s_paddr   = CPU_Swap32(p_sec_hdr->s_paddr);
    p_sec_hdr->s_vaddr   = CPU_Swap32(p_sec_hdr->s_vaddr);
    p_sec_hdr->s_size    = CPU_Swap32(p_sec_hdr->s_size);
    p_sec_hdr->s_scnptr  = CPU_Swap32(p_sec_hdr->s_scnptr);
    p_sec_hdr->s_relptr  = CPU_Swap32(p_sec_hdr->s_relptr);
    p_sec_hdr->s_lnnoptr = CPU_Swap32(p_sec_hdr->s_lnnoptr);
    p_sec_hdr->s_nreloc  = CPU_Swap16(p_sec_hdr->s_nreloc);
    p_sec_hdr->s_nlnno   = CPU_Swap16(p_sec_hdr->s_nlnno);
    p_sec_hdr->s_flags   = CPU_Swap32(p_sec_hdr->s_flags);
}

static  void  CoffParser_LoadSection(SECTION          *p_section,
                                     FILE             *p_file,
                                     COFF_PARSER_ERR  *p_err)
{
    SECTION_HDR  *p_sec_hdr;
    void         *p_mem;
    CPU_INT32U   stream_err;
    
    
    p_sec_hdr = &p_section->section;
    p_mem = malloc(p_sec_hdr->s_size);
    
    if (p_mem == NULL) {
       *p_err = COFF_PARSER_ERR_MEM_ALLOC;
        return;
    }
    
    p_section->p_data = p_mem;
    
    stream_err = fread(p_mem,
                       1u,
                       p_sec_hdr->s_size,
                       p_file);
    
    if (stream_err != p_sec_hdr->s_size) {
        *p_err = COFF_PARSER_ERR_FILE_READ;
        return;
    }
    
    
    *p_err = COFF_PARSER_ERR_NONE;
}













