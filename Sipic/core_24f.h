//
//  core_24f.h
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-05.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#ifndef Sipic_core_24f_h
#define Sipic_core_24f_h

#include "cpu.h"
#include "mem.h"
#include "err.h"
#include "sipic_cfg.h"

#define  CORE_SR_C     0x0001
#define  CORE_SR_Z     0x0002
#define  CORE_SR_OV    0x0004
#define  CORE_SR_N     0x0008
#define  CORE_SR_RA    0x0010
#define  CORE_SR_IPL0  0x0020
#define  CORE_SR_IPL1  0x0040
#define  CORE_SR_IPL2  0x0080
#define  CORE_SR_DC    0x0100
#define  CORE_SR_DA    0x0200
#define  CORE_SR_SAB   0x0400
#define  CORE_SR_OAB   0x0800
#define  CORE_SR_SB    0x1000
#define  CORE_SR_SA    0x2000
#define  CORE_SR_OB    0x4000
#define  CORE_SR_OA    0x8000

#define  CORE_MATH_OP_ADD  0x0001
#define  CORE_MATH_OP_SUB  0x0002
#define  CORE_MATH_OP_MUL  0x0003
#define  CORE_MATH_OP_DIV  0x0004


typedef struct core_24f {
    
#if 0
    CPU_INT16U  W0;
    CPU_INT16U  W1;
    CPU_INT16U  W2;
    CPU_INT16U  W3;
    CPU_INT16U  W4;
    CPU_INT16U  W5;
    CPU_INT16U  W6;
    CPU_INT16U  W7;
    CPU_INT16U  W8;
    CPU_INT16U  W9;
    CPU_INT16U  W10;
    CPU_INT16U  W11;
    CPU_INT16U  W12;
    CPU_INT16U  W13;
    CPU_INT16U  W14;
    CPU_INT16U  W15;
#endif
    
    CPU_INT16U  W[16];
    
    CPU_INT16U  SPLIM;
    
    CPU_INT64U  ACCA;
    CPU_INT64U  ACCB;
    
    CPU_INT32U  PC;
    
    CPU_INT08U  TBLPAG;
    
    CPU_INT08U  PSVPAG;
    
    CPU_INT16U  RCOUNT;
    
    CPU_INT32U  DOSTART;
    
    CPU_INT32U  DOEND;
    
    CPU_INT16U SR;
    
#ifdef  CORE_CFG_CYCLE_CNTR
    CPU_INT64U CYCLE;
#endif
    
} CORE_24F;

typedef  CPU_INT16U  CORE_MATH_OP;

void        Core_Push(CPU_INT32U   val,
                      CORE_24F    *p_core,
                      MEM         *p_mem,
                      CORE_ERR    *p_err);

CPU_INT32U  Core_Pop (CORE_24F    *p_core,
                      MEM         *p_mem,
                      CORE_ERR    *p_err);

void        Core_Run (MEM         *p_mem_prog,
                      MEM         *p_mem_data,
                      CORE_ERR    *p_err);

CPU_INT08U  Core_GetCarry (CORE_24F  *p_core);

/* 2                    opcode      mask
 CALL EXPR           0x020000 /  0xFF0001
 DO   #lit14, Expr   0x080000 /  0xFFC000
 DO   Wn,     Expr   0x088000 /  0xFFFFF0
 GOTO Wn             0x014000 /  0xFFFFF0
 */

/* 1
 Otherwise
 */

CPU_INT32U  Core_OPC_Words (OPCODE  opc);

#endif
