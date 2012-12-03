//
//  unit_testing.c
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-05.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#include "unit_testing.h"

#define TEST_WREG(wreg, val)    if (!((p_sim->p_core->W[wreg]) == val)) printf("\r\nW[%d] != %04X. val = %04X",wreg,val,p_sim->p_core->W[wreg])

#define TEST_DMEM(addr, val)    if (!((Mem_Get(p_sim->p_mem_data, (addr & 0xFFFE), &mem_err)) == val)) printf("\r\n[%d] != %04X. val = %04X",addr,val, Mem_Get(p_sim->p_mem_data, (addr & 0xFFFE), &mem_err))

#define TEST_SR(val)            if (p_sim->p_core->SR != val) printf("\r\nSR != %04X. SR = %04X",val,p_sim->p_core->SR)


#define TEST_SRC_W(x)             (x & 0xF) 
#define TEST_SRC_MODE(x)          (x & 0x7) <<  4
#define TEST_DST_W(x)             (x & 0xF) <<  7
#define TEST_DST_MODE(x)          (x & 0x7) << 11
#define TEST_BAS_W(x)             (x & 0xF) << 15
#define TEST_BYTE                   0x004000
#define TEST_WORD                   0x000000



tm_struct   time_buffer_before;
tm_struct   time_buffer_after;
CPU_INT32U  time_binary_before;
CPU_INT32U  time_binary_after;

//0x0862-0x0863 CurrentTimeBinary 
#define  TIME_BINARY_BASE_ADDR  0x0866
#define  TIME_BINARY_BASE_SIZE  0x0004

#define  TIME_STRUCT_BASE_ADDR  0x0928
#define  TIME_STRUCT_BASE_SIZE  0x0012

CPU_INT32U DaysToMonth[13] = {0,31,59,90,120,151,181,212,243,273,304,334,365}; 

CPU_INT32U DS1371_DateToBinary(tm_struct *datetime) { 
    CPU_INT32U iday; 
    CPU_INT32U val; 
    
    
    iday = 365 * (datetime->tm_year - 70) + DaysToMonth[datetime->tm_mon] + (datetime->tm_mday - 1); 
    iday = iday + (datetime->tm_year - 69) / 4; 
    
    if ((datetime->tm_mon > 1) && ((datetime->tm_year % 4) == 0)) { 
        iday++; 
    } 
    
    val = datetime->tm_sec + 60 * datetime->tm_min + 3600 * (datetime->tm_hour + 24 * iday); 
    return val; 
}

void  UT_CleanContext(SIM  *p_sim) {
    CORE_ERR  core_err;
    
    Core_Reset(p_sim->p_core, &core_err);

    printf("\r\n");
}

void  UT_Testting(SIM  *p_sim)
{
    UT_CleanContext(p_sim);
    UT_SUB_F_EX1(p_sim);

    UT_CleanContext(p_sim);
    UT_SUB_F_EX2(p_sim);

    UT_CleanContext(p_sim);
    UT_DIV_U(p_sim);

    UT_CleanContext(p_sim);
    UT_DIV_S(p_sim);

    UT_CleanContext(p_sim);
    UT_XOR_Wb_Ws_Wd_B(p_sim);

    UT_CleanContext(p_sim);
    UT_XOR_Wb_Ws_Wd_W(p_sim);

    UT_CleanContext(p_sim);
    UT_RLC_Ws_Wd_B(p_sim);

    UT_CleanContext(p_sim);
    UT_RLC_Ws_Wd_W(p_sim);
    
    UT_CleanContext(p_sim);
    UT_CP_B(p_sim);

    UT_CleanContext(p_sim);
    UT_CP_W(p_sim);

    /**/
    UT_CleanContext(p_sim);
    UT_DIV3232A(p_sim);

    while(1) {
    
    }

#if 0
     CORE_24F    *p_core;
    MEM         *p_mem;
    CPU_INT32U   pc;
    CPU_INT32U   time_binary;
    MEM_ERR      mem_err;
    CORE_ERR     core_err;


    p_core       = p_sim->p_core;
    p_mem        = p_sim->p_mem_data;


    pc = Core_PC_Get(p_core);

    if (pc == 0x0A5E) {
        Mem_Load((void *)&time_buffer_before, TIME_STRUCT_BASE_ADDR, TIME_STRUCT_BASE_SIZE, p_mem, &mem_err);
        Mem_Load((void *)&time_binary_before, TIME_BINARY_BASE_ADDR, TIME_BINARY_BASE_SIZE, p_mem, &mem_err);
        printf("\r\n%d/%d/%d\t%d:%d:%d\t%d",
            time_buffer_before.tm_mday,
            time_buffer_before.tm_mon,
            time_buffer_before.tm_year + 1900,
            time_buffer_before.tm_hour,
            time_buffer_before.tm_min,
            time_buffer_before.tm_sec,
            time_binary_before);

        time_binary = DS1371_DateToBinary(&time_buffer_before);
    }

    if (pc == 0x0AAA) {
        Mem_Load((void *)&time_buffer_after, TIME_STRUCT_BASE_ADDR, TIME_STRUCT_BASE_SIZE, p_mem, &mem_err);
        Mem_Load((void *)&time_binary_after, TIME_BINARY_BASE_ADDR, TIME_BINARY_BASE_SIZE, p_mem, &mem_err);
        printf("\r\n%d/%d/%d\t%d:%d:%d\t%d",
            time_buffer_after.tm_mday,
            time_buffer_after.tm_mon,
            time_buffer_after.tm_year + 1900,
            time_buffer_after.tm_hour,
            time_buffer_after.tm_min,
            time_buffer_after.tm_sec,
            time_binary_after);

        time_binary = DS1371_DateToBinary(&time_buffer_after);
    }
#endif
}

#define  UT_SUB_F_ADDR_F1  0x0A05

void UT_SUB_F_EX1(SIM * p_sim) {
    CORE_24F    *p_core;
    MEM_24      *p_mem;
    CPU_INT16U   val;
    MEM_ERR      mem_err;
    CORE_ERR     core_err;
     

    p_core       = p_sim->p_core;
    p_mem        = p_sim->p_mem_prog;

    p_core->W[0] = 0x7804;
    p_core->SR   = 0x0000;

    Mem_Set(p_sim->p_mem_data, (UT_SUB_F_ADDR_F1 & 0xFFFE), 0x9439, &mem_err);

    /* SUB.B 0x1FFF */
    Mem_Set24(p_mem, 0, (0xB56000 | UT_SUB_F_ADDR_F1), &mem_err);

    Core_PC_Set(p_core, 0);

    Core_Run(p_sim->p_core,
             p_sim->p_mem_prog,
             p_sim->p_mem_data,
            &core_err);

    val = Mem_Get(p_sim->p_mem_data, (UT_SUB_F_ADDR_F1 & 0xFFFE), &mem_err);

    if (p_core->W[0] != 0x7804) {
        printf("\r\np_core->W[0] != 0x7804");
    }

    if (val != 0x9039) {
        printf("\r\n*0x1FFE != 0x9039");
    }

    if (p_core->SR != 0x0009) {
        printf("\r\np_core->SR != 0x0009");
    }
}

#define  UT_SUB_F_ADDR_F2  0x0A04

void UT_SUB_F_EX2(SIM * p_sim) {
    CORE_24F    *p_core;
    MEM_24      *p_mem;
    CPU_INT16U   val;
    MEM_ERR      mem_err;
    CORE_ERR     core_err;
     

    p_core       = p_sim->p_core;
    p_mem        = p_sim->p_mem_prog;

    p_core->W[0] = 0x6234;
    p_core->SR   = 0x0000;

    Mem_Set(p_sim->p_mem_data, (UT_SUB_F_ADDR_F2 & 0xFFFE), 0x4523, &mem_err);

    Mem_Set24(p_mem, 0, (0xB50000 | UT_SUB_F_ADDR_F2), &mem_err);

    Core_PC_Set(p_core, 0);

    Core_Run(p_sim->p_core,
             p_sim->p_mem_prog,
             p_sim->p_mem_data,
            &core_err);

    val = Mem_Get(p_sim->p_mem_data, (UT_SUB_F_ADDR_F2 & 0xFFFE), &mem_err);

    if (p_core->W[0] != 0xE2EF) {
        printf("\r\np_core->W[0] != 0xE2EF");
    }

    if (val != 0x4523) {
        printf("\r\n*0x1FFE != 0x4523");
    }

    if (p_core->SR != 0x0008) {
        printf("\r\np_core->SR != 0x0008");
    }
}

void UT_DIV_U(SIM * p_sim) {
    CORE_24F  *p_core;
    MEM_24    *p_mem;
    MEM_ERR    mem_err;
    CORE_ERR   core_err;


    p_core         = p_sim->p_core;
    p_mem          = p_sim->p_mem_prog;

    p_core->W[0]   = 0x5555;
    p_core->W[1]   = 0x1234;
    p_core->W[7]   = 0x8000;
    p_core->W[8]   = 0x0200;
    p_core->RCOUNT = 0x0001;
    p_core->SR     = CORE_SR_RA;

    Mem_Set24(p_sim->p_mem_prog, 0, 0xD88388, &mem_err);

    Core_PC_Set(p_core, 0);

    Core_Run(p_sim->p_core,
             p_sim->p_mem_prog,
             p_sim->p_mem_data,
            &core_err);

    if (p_core->W[0] != 0x0040) {
        printf("\r\np_core->W[0] != 0x0040");
    }

    if (p_core->W[1] != 0x0000) {
        printf("\r\np_core->W[1] != 0x0000");
    }

    if (p_core->W[7] != 0x8000) {
        printf("\r\np_core->W[7] != 0x8000");
    }

    if (p_core->W[8] != 0x0200) {
        printf("\r\np_core->W[8] != 0x0200");
    }

    if ((p_core->SR & ~(CORE_SR_RA)) != 0x0002) {
        printf("\r\np_core->SR   != 0x0002");
    }


}

void UT_DIV_S(SIM * p_sim) {
    CORE_24F  *p_core;
    MEM_24    *p_mem;
    MEM_ERR    mem_err;
    CORE_ERR   core_err;


    p_core         = p_sim->p_core;
    p_mem          = p_sim->p_mem_prog;

    p_core->W[0]   = 0x5555;
    p_core->W[1]   = 0x1234;
    p_core->W[4]   = 0x3000;
    p_core->W[3]   = 0x0027;
    p_core->RCOUNT = 0x0001;
    p_core->SR     = CORE_SR_RA;

    Core_PC_Set(p_core, 0);

    Mem_Set24(p_sim->p_mem_prog, 0, 0x00d80203, &mem_err);

    Core_Run(p_sim->p_core,
             p_sim->p_mem_prog,
             p_sim->p_mem_data,
            &core_err);

    if (p_core->W[0] != 0x13B) {
        printf("\r\np_core->W[0] != 0x0040");
    }

    if (p_core->W[1] != 0x0003) {
        printf("\r\np_core->W[1] != 0x0003");
    }

    if (p_core->W[4] != 0x3000) {
        printf("\r\np_core->W[4] != 0x3000");
    }

    if (p_core->W[3] != 0x0027) {
        printf("\r\np_core->W[3] != 0x0027");
    }

    if ((p_core->SR & ~(CORE_SR_RA)) != 0x0000) {
        printf("\r\np_core->SR   != 0x0000");
    }


}

void UT_DIV3232A(SIM * p_sim)
{
    CORE_24F  *p_core;
    MEM_24    *p_mem;
    MEM_ERR    mem_err;
    CORE_ERR   core_err;


    p_core         = p_sim->p_core;
    p_mem          = p_sim->p_mem_prog;

    p_core->W[0]   = 0xB221;
    p_core->W[1]   = 0x4E84;
    p_core->W[2]   = 0x003C;
    p_core->W[3]   = 0x0000;
    p_core->SR    &= ~(CORE_SR_DC);

    Mem_Set24(p_sim->p_mem_prog, 0, 0x0204B6, &mem_err);
    Mem_Set24(p_sim->p_mem_prog, 2, 0x000000, &mem_err);

    Core_PC_Set(p_core, 0);

    while (Core_PC_Get(p_core) != 4) {

        Core_Run(p_sim->p_core,
                 p_sim->p_mem_prog,
                 p_sim->p_mem_data,
                &core_err);

    }

    printf("\r\nUT_DIV3232A Done.");

    if (p_core->W[0] != 0x02F8) {
        printf("\r\np_core->W[0] != 0x02F8: 0x%04X", p_core->W[0]);
    }

    if (p_core->W[1] != 0x014F) {
        printf("\r\np_core->W[1] != 0x014F: 0x%04X", p_core->W[1]);
    }

}

#define  XOR_MEM_1  0x2000
#define  XOR_MEM_2  0x2600

void UT_XOR_Wb_Ws_Wd_B(SIM * p_sim)
{
    CORE_24F  *p_core;
    MEM_24    *p_mem;
    MEM_ERR    mem_err;
    OPCODE     opcode;
    CORE_ERR   core_err;


    p_core         = p_sim->p_core;
    p_mem          = p_sim->p_mem_prog;

    p_core->W[1]   = 0xAAAA;
    p_core->W[5]   = XOR_MEM_1;
    p_core->W[9]   = XOR_MEM_2;
    p_core->SR     = CORE_SR_NONE;

    /* XOR.B  W1, [W5++], [W9++] p.5-262 */
    opcode  = 0x680000;
    opcode |= 0x004000;     /* Byte mode */
    opcode |= 0x008000;     /* Wb = W1   */
    opcode |= 0x001800;     /* Dst Addr Mode = 011 [++] */
    opcode |= 0x000480;     /* Dst Wreg = W9 */
    opcode |= 0x000030;     /* Src Addr Mode = 011 [++] */
    opcode |= 0x000005;     /* Src Wreg = W5 */

    Mem_Set24(p_sim->p_mem_prog, 0, opcode, &mem_err);

    Mem_Set(p_sim->p_mem_data, XOR_MEM_1, 0x115A, &mem_err);
    Mem_Set(p_sim->p_mem_data, XOR_MEM_2, 0x0000, &mem_err);

    Core_PC_Set(p_core, 0);

    Core_Run(p_sim->p_core,
                p_sim->p_mem_prog,
                p_sim->p_mem_data,
            &core_err);

    TEST_WREG(1 ,0xAAAA);
    TEST_WREG(5 ,(XOR_MEM_1+1));
    TEST_WREG(9 ,(XOR_MEM_2+1));

    TEST_DMEM(XOR_MEM_1, 0x115A);
    TEST_DMEM(XOR_MEM_2, 0x00F0);

    TEST_SR(CORE_SR_N);
}

void UT_XOR_Wb_Ws_Wd_W(SIM * p_sim)
{
    CORE_24F  *p_core;
    MEM_24    *p_mem;
    MEM_ERR    mem_err;
    OPCODE     opcode;
    CORE_ERR   core_err;


    p_core         = p_sim->p_core;
    p_mem          = p_sim->p_mem_prog;

    p_core->W[1]   = 0xFEDC;
    p_core->W[5]   = 0x1234;
    p_core->W[9]   = 0xA34D;
    p_core->SR     = CORE_SR_NONE;

    /* XOR  W1, W5, W9  p.5-263 */
    opcode  = 0x680000;
    //opcode |= 0x004000;     /* Byte mode */
    opcode |= 0x008000;     /* Wb = W1   */
    opcode |= 0x000000;     /* Dst Addr Mode = 000 */
    opcode |= 0x000480;     /* Dst Wreg = W9 */
    opcode |= 0x000000;     /* Src Addr Mode = 000 */
    opcode |= 0x000005;     /* Src Wreg = W5 */

    Mem_Set24(p_sim->p_mem_prog, 0, opcode, &mem_err);

    Core_PC_Set(p_core, 0);

    Core_Run(p_sim->p_core,
                p_sim->p_mem_prog,
                p_sim->p_mem_data,
            &core_err);

    TEST_WREG(1 ,0xFEDC);
    TEST_WREG(5 ,0x1234);
    TEST_WREG(9 ,0xECE8);

    TEST_SR(CORE_SR_N);
}


void UT_RLC_Ws_Wd_B(SIM * p_sim)
{
    CORE_24F  *p_core;
    MEM_24    *p_mem;
    MEM_ERR    mem_err;
    OPCODE     opcode;
    CORE_ERR   core_err;


    p_core         = p_sim->p_core;
    p_mem          = p_sim->p_mem_prog;

    p_core->W[0]   = 0x9976;
    p_core->W[3]   = 0x5879;
    p_core->SR     = CORE_SR_C;

    /* RLC.B  W0, W3 p.5-205 */
    opcode  = CORE_OPC_RLC_W;
    opcode |= TEST_BYTE;
    opcode |= TEST_SRC_MODE(CORE_OPC_ADDR_MODE_DIR);
    opcode |= TEST_SRC_W(0);
    opcode |= TEST_DST_MODE(CORE_OPC_ADDR_MODE_DIR);
    opcode |= TEST_DST_W(3);


    Mem_Set24(p_sim->p_mem_prog, 0, opcode, &mem_err);

    Core_PC_Set(p_core, 0);

    Core_Run(p_sim->p_core,
                p_sim->p_mem_prog,
                p_sim->p_mem_data,
            &core_err);

    TEST_WREG(0 , 0x9976);
    TEST_WREG(3 , 0x58ED);

    TEST_SR(CORE_SR_N);
}

void UT_RLC_Ws_Wd_W(SIM * p_sim)
{
    CORE_24F  *p_core;
    MEM_24    *p_mem;
    MEM_ERR    mem_err;
    OPCODE     opcode;
    CORE_ERR   core_err;


    p_core         = p_sim->p_core;
    p_mem          = p_sim->p_mem_prog;

    p_core->W[2]   = 0x2008;
    p_core->W[8]   = 0x094E;
    p_core->SR     = CORE_SR_C;

    Mem_Set(p_sim->p_mem_data, 0x094E, 0x3689, &mem_err);
    Mem_Set(p_sim->p_mem_data, 0x2008, 0xC041, &mem_err);

    /* RLC  [W2++], [W8] p.5-206 */
    opcode  = CORE_OPC_RLC_W;
    opcode |= TEST_WORD;
    opcode |= TEST_SRC_MODE(CORE_OPC_ADDR_MODE_IND_POS_INC);
    opcode |= TEST_SRC_W(2);
    opcode |= TEST_DST_MODE(CORE_OPC_ADDR_MODE_IND);
    opcode |= TEST_DST_W(8);


    Mem_Set24(p_sim->p_mem_prog, 0, opcode, &mem_err);



    Core_PC_Set(p_core, 0);

    Core_Run(p_sim->p_core,
                p_sim->p_mem_prog,
                p_sim->p_mem_data,
            &core_err);

    TEST_WREG(2 , 0x200A);
    TEST_WREG(8 , 0x094E);

    TEST_DMEM(0x094E, 0x8083);
    TEST_DMEM(0x2008, 0xC041);

    TEST_SR((CORE_SR_N|CORE_SR_C));
}

void UT_CP_B(SIM * p_sim)
{
    CORE_24F  *p_core;
    MEM_24    *p_mem;
    MEM_ERR    mem_err;
    OPCODE     opcode;
    CORE_ERR   core_err;
    
    
    p_core         = p_sim->p_core;
    p_mem          = p_sim->p_mem_prog;
    
    p_core->W[0]   = 0xABA9;
    p_core->W[1]   = 0x2000;
    p_core->SR     = CORE_SR_NONE;
    
    Mem_Set(p_sim->p_mem_data, 0x2000, 0xD004, &mem_err);
    
    /* CP.B  W0, [W1++] p.5-84 */
    opcode  = CORE_OPC_CP_WB_WS;
    opcode |= 0x000400;
    opcode |= TEST_SRC_MODE(CORE_OPC_ADDR_MODE_IND_POS_INC);
    opcode |= TEST_SRC_W(1);
    opcode |= 0 << 11;
    
    Mem_Set24(p_sim->p_mem_prog, 0, opcode, &mem_err);

    Core_PC_Set(p_core, 0);
    
    Core_Run(p_sim->p_core,
             p_sim->p_mem_prog,
             p_sim->p_mem_data,
             &core_err);
    
    TEST_WREG(0 , 0xABA9);
    TEST_WREG(1 , 0x2001);
    
    TEST_DMEM(0x2000, 0xD004);
    
    TEST_SR((CORE_SR_N));
}

void UT_CP_W(SIM * p_sim)
{
    CORE_24F  *p_core;
    MEM_24    *p_mem;
    MEM_ERR    mem_err;
    OPCODE     opcode;
    CORE_ERR   core_err;
    
    
    p_core         = p_sim->p_core;
    p_mem          = p_sim->p_mem_prog;
    
    p_core->W[5]   = 0x2334;
    p_core->W[6]   = 0x8001;
    p_core->SR     = CORE_SR_NONE;
        
    /* CP.B  W0, [W1++] p.5-84 */
    opcode  = CORE_OPC_CP_WB_WS;
    opcode |= 0 << 10;
    opcode |= TEST_SRC_MODE(CORE_OPC_ADDR_MODE_DIR);
    opcode |= TEST_SRC_W(6);
    opcode |= 5 << 11;
     
    Mem_Set24(p_sim->p_mem_prog, 0, opcode, &mem_err);
    
    Core_PC_Set(p_core, 0);
    
    Core_Run(p_sim->p_core,
             p_sim->p_mem_prog,
             p_sim->p_mem_data,
             &core_err);
    
    TEST_WREG(5 , 0x2334);
    TEST_WREG(6 , 0x8001);
    
    TEST_SR((CORE_SR_N|CORE_SR_OV));
}