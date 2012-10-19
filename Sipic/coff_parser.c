//
//  coff_parser.c
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-02.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#if 0
#include <stdio.h>
#include <coff_parser.h>
#include <coff_cfg.h>
#include <cpu.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>


/*
 ******** LOCAL FUNCTION PROTOTYPE ********
 */

static  void  CoffParser_SwapSectionHdr (SECTION          *p_section);

static  void  CoffParser_LoadSection    (SECTION          *p_section,
                                         CPU_INT08U       *p_file_data,
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
#if 0
    CPU_INT08U   optional_hdr[64];
#endif
    struct stat  file_status;
    CPU_INT64U   file_size;
    CPU_INT64U   file_rem_size;
    CPU_INT64U   file_index;
    CPU_INT08U  *p_file_content;
    OPT_HDR      opt_hdr;
    
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
        stream_err = fclose(p_file);
       *p_err = COFF_PARSER_ERR_FILE_READ;
        return;
    }
    
    stat(p_file_name, &file_status);
    
    file_size      = file_status.st_size;
    file_rem_size  = file_size - FILE_HDR_SIZE;
    p_file_content = malloc(file_rem_size);

    if (p_file_content == NULL) {
       *p_err = COFF_PARSER_ERR_MEM_ALLOC;
        return;
    }
    
    fread(p_file_content, 1u, file_rem_size, p_file);
    fclose(p_file);
    
#if 0
    stream_err = fread(&optional_hdr[0],
                       1u,
                       file_hdr.f_opthdr,
                       p_file);
    
    if (stream_err != file_hdr.f_opthdr) {
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
#else
    memcpy(&opt_hdr, &p_file_content[0], OPT_HDR_SIZE);
    
#endif

    p_section  = &section[0];
    file_index = file_hdr.f_opthdr;
                                                                /* Read Section Header. */
    for (section_iterator = 0 ; section_iterator < file_hdr.f_nscns ; section_iterator++) {
        
        (void)memcpy(((       void         *)&p_section->section),
                     ((const  void         *)&p_file_content[file_index]),
                     ((       unsigned  long) SECTION_HDR_SIZE));
    
        p_section->p_data = (CPU_INT08U *)0;
        p_section++;
        file_index += SECTION_HDR_SIZE;
    }
    
    p_section = &section[0];
    
    for (section_iterator = 0 ; section_iterator < file_hdr.f_nscns ; section_iterator++) {
        CoffParser_LoadSection(p_section,
                               p_file_content,
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
                                     CPU_INT08U       *p_file_data,
                                     COFF_PARSER_ERR  *p_err)
{
    SECTION_HDR  *p_sec_hdr;
    void         *p_mem;
    CPU_INT16U   data_preview_size;
    CPU_INT32U   section_idx;
    CPU_INT32U   section_size;
    
    p_sec_hdr    = &p_section->section;
    section_size =  p_sec_hdr->s_size;
    p_mem = malloc(section_size);
    
    if (p_mem == NULL) {
       *p_err = COFF_PARSER_ERR_MEM_ALLOC;
        return;
    }
    
    p_section->p_data = p_mem;
    section_idx = p_sec_hdr->s_scnptr + 8;
    
    memcpy(p_mem, &p_file_data[section_idx], section_size);
    
    data_preview_size = section_size;
    
    if (data_preview_size > COFF_CFG_DATA_PREVIEW_SIZE) {
        data_preview_size = COFF_CFG_DATA_PREVIEW_SIZE;
    }
    
    if (p_sec_hdr->s_scnptr != 0) {
    
        memcpy(&p_section->data[0],
                p_section->p_data,
                data_preview_size);
    
    }
    
    *p_err = COFF_PARSER_ERR_NONE;
}

#endif











