//
//  hex_parser_cfg.h
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-05.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#ifndef Sipic_hex_parser_cfg_h
#define Sipic_hex_parser_cfg_h

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

#ifdef  WIN32
#define  HEX_CFG_FILE_PATH  "C:\\Users\\alexis01.micrium01\\Projects\\Sipic\\Sipic\\InputFiles\\main.hex"
#else
#define  HEX_CFG_FILE_PATH  "/Users/aop007/Documents/Projets/DawnStar/Sipic/Sipic/Sipic/InputFiles/main.hex"
#endif

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
}
#endif

#endif
