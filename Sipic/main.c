//
//  main.c
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-02.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#include <stdio.h>
#include <coff_parser.h>
#include <coff_cfg.h>

int main(int argc, const char * argv[])
{
    COFF_PARSER_ERR  coff_err;

    CoffParser_ReadFile(COFF_CFG_FILE_PATH,
                       &coff_err);
    
    // insert code here...
    printf("Hello, World!\n");
    return 0;
}

