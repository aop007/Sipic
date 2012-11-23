//
//  core_24f.h
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-05.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#ifndef Sipic_core_24f_h
#define Sipic_core_24f_h

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

#include "cpu.h"
#include "mem.h"
#include "err.h"
#include "sipic_cfg.h"


typedef  CPU_INT16U  CORE_SR_FLAGS;

enum sr_dir {
    CORE_SR_DIR_NA   =  0u,
    CORE_SR_DIR_UP   =  1u,
    CORE_SR_DIR_DOWN =  2u
};

typedef enum sr_dir CORE_SR_DIR;

#define  CORE_SR_NONE  0x0000
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
    
#define  CORE_SR_IPL_MAKS  (CORE_SR_IPL2 | CORE_SR_IPL1 | CORE_SR_IPL0)

#define  CORE_CORECON_IPL3 0x0008

typedef struct core_24f {
    
    volatile CPU_INT16U  W[16];
    
    volatile CPU_INT16U  SPLIM;
    
    volatile CPU_INT16U  ACCA[3];
    volatile CPU_INT16U  ACCB[3];
    
    volatile CPU_INT08U  PC[4];
    
    volatile CPU_INT08U  TBLPAG;
    
    volatile CPU_INT08U  Reserved0;
    
    volatile CPU_INT08U  PSVPAG;
    
    volatile CPU_INT08U  Reserved1;
    
    volatile CPU_INT16U  RCOUNT;
    volatile CPU_INT16U  DCOUNT;
    
    volatile CPU_INT08U  DOSTART[4];
    
    volatile CPU_INT08U  DOEND[4];
    
    volatile CPU_INT16U  SR;
    
    volatile CPU_INT16U  CORCON;
    volatile CPU_INT16U  MODCON;
    
} CORE_24F;

    struct core_context;
    
typedef struct core_context {
    struct core_context  *p_next;
           CORE_24F       context;
} CORE_CONTEXT;
    
typedef struct core_data {
    CPU_INT64U     cycles;
    CPU_INT32U     fcy;
    CORE_CONTEXT  *p_context_head;
    OPCODE         ra_opc;
} CORE_DATA;

CORE_24F * Core_Init(MEM         *p_mem_data,
                     CPU_INT32U   addr,
                     CORE_ERR    *p_err);

void        Core_Push(CPU_INT32U   val,
                      CORE_24F    *p_core,
                      MEM         *p_mem,
                      CORE_ERR    *p_err);

CPU_INT32U  Core_Pop (CORE_24F    *p_core,
                      MEM         *p_mem,
                      CORE_ERR    *p_err);

void        Core_Run (CORE_24F    *p_core,
                      MEM_24      *p_mem_prog,
                      MEM         *p_mem_data,
                      CORE_ERR    *p_err);

CPU_INT08U  Core_GetC     (CORE_24F     *p_core);
CPU_INT08U  Core_GetZ     (CORE_24F     *p_core);
CPU_INT08U  Core_GetOV    (CORE_24F     *p_core);
CPU_INT08U  Core_GetN     (CORE_24F     *p_core);

void        Core_SetC     (CORE_24F     *p_core, 
                           CPU_BOOLEAN   value);

void        Core_PC_Slide (CORE_24F    *p_core,
                           CPU_INT32S   slide);

void        Core_PC_Set   (CORE_24F    *p_core,
                           CPU_INT32U   value);

CPU_INT32U  Core_PC_Get   (CORE_24F    *p_core);

CPU_INT32U  Core_MaskGet (CPU_INT08U    byte_mode, 
                          CPU_INT32U    addr);

CPU_INT32U  Core_Align   (CPU_INT32U    value,
                          CPU_INT32U    mask);
    
CPU_INT32U  Core_Merge   (CPU_INT32U    value_original,
                          CPU_INT32U    value,
                          CPU_INT32U    mask);
    
CPU_INT08U  Core_GetIPL  (CORE_24F     *p_core);

CPU_INT32U  Core_OPC_Words (OPCODE  opc);

void  Core_UpdateC(CORE_24F       *p_core,
                   CPU_INT32U      initial_val,
                   CPU_INT32U      final_val,
                   CPU_INT32U      size_op);

void  Core_UpdateZ(CORE_24F       *p_core,
                   CPU_INT32U      final_val);

void  Core_UpdateOV(CORE_24F       *p_core,
                    CPU_INT32U      initial_val,
                    CPU_INT32U      final_val,
                    CORE_SR_DIR     direction,
                    CPU_INT32U      size_op);

void  Core_UpdateN(CORE_24F       *p_core,
                   CPU_INT32U      final_val,
                   CPU_INT32U      size_op);

void  Core_UpdateDC(CORE_24F       *p_core,
                    CPU_INT32U      initial_val,
                    CPU_INT32U      final_val,
                    CPU_INT32U      size_op);

void  Core_UpdateSRFlags(CORE_24F       *p_core,
                         CORE_SR_FLAGS   flags,
                         CORE_SR_DIR     direction,
                         CPU_INT32U      initial_val,
                         CPU_INT32U      final_val,
                         CPU_INT32U      size_op);
    
#if 0
void        Core_InsertRA_OPC(CORE_DATA  *p_core_data,
                              OPCODE      opc);
#endif

#if  (CORE_INTEGRITY_CHECK == DEF_ENABLED)
void        Core_PushContext     (CORE_24F  *p_core);
void        Core_PopCheckContext (CORE_24F  *p_core);
#endif

extern  CPU_INT32S   Call_Depth;

#if    (WRITE_REPORT_EN == DEF_ENABLED)
#define WRITE_REPORT
#endif

#ifdef WRITE_REPORT
  FILE        *p_out;
  CPU_CHAR     file_buffer[512];
  CPU_INT32U   file_buffer_len;
#endif

#define  RA_METHOD_OLD   0u
#define  RA_METHOD_NEW   1u
    
    
#define  RA_METHOD      RA_METHOD_NEW
    
#endif