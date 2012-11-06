//
//  main.c
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-02.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#include "main.h"

#ifndef OBJ_C
int main(int argc, const char * argv[])
{
    init();

    Sim_Run(&sim_struct);
}
#endif

void run ()
{
    Sim_Run(&sim_struct);
}

void init()
{
    SIM      *p_sim;
    //SIM       sim_struct;
    CORE_24F *p_core;
    MEM_24   *p_mem_prog;
    MEM      *p_mem_data;
    MEM_ERR   mem_err;
    CORE_ERR  core_err;
    HEX_PARSER_ERR  hex_err;    
    
    p_sim = &sim_struct;
    core_static_err = CORE_ERR_NONE;
    peri_static_err = PERI_ERR_NONE;
    
    p_mem_prog = Mem_Init24(&mem_cfg_prog_dsPIC30F[0],
                      sizeof(mem_cfg_prog_dsPIC30F),
                            &mem_err);
    
    p_mem_data = Mem_Init(&mem_cfg_data_dsPIC30F[0],
                    sizeof(mem_cfg_data_dsPIC30F),
                          &mem_err);
    
#if  (SIPIC_CFG_PARSE_METHOD == SIPIC_PARSE_METHOD_COFF)
    COFF_PARSER_ERR  coff_err;

    CoffParser_ReadFile(COFF_CFG_FILE_PATH,
                       &coff_err);
#elif (SIPIC_CFG_PARSE_METHOD == SIPIC_PARSE_METHOD_HEX)
    
    
    HexParser_ReadFile(HEX_CFG_FILE_PATH,
                       p_mem_prog,
                      &hex_err);
    
    if (hex_err != HEX_PARSER_ERR_NONE) {
        CORE_TRACE_DEBUG("\r\nError in HexParser_ReadFile().");
        return;
    }
#ifdef  CORE_STAT_EN
    Core_OPC_Stats(p_mem);
#endif
    
    p_core = Core_Init(p_mem_data, 0x0000, &core_err);
    
    p_sim->p_core     = p_core;
    p_sim->p_mem_data = p_mem_data;
    p_sim->p_mem_prog = p_mem_prog;
    
    //Sim_Run(&sim_struct);

#endif
    return;
}

