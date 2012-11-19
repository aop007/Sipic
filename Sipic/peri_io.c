//
//  Peri_io.c
//  Sipic
//
//  Created by Alexis Ouellet-Patenaude on 2012-10-25.
//  Copyright (c) 2012 Alexis Ouellet-Patenaude. All rights reserved.
//

#include <stdio.h>
#include "peripheral.h"
#include "main.h"


IO *Peri_IO_Init(HW_IF_DATA_TYPE **p_pin_tbl,
                 IO_PORT_BIT      *p_bit_tbl,
                 CPU_INT16U        pin_cnt,
                 MEM              *p_mem_data,
                 PERI_ERR         *p_err)
{
    IO       *p_io;
    IO_MEM   *p_mem;
    IO_DATA  *p_data;
    MEM_ERR   mem_err;
    
    p_data = (IO_DATA *)malloc(sizeof(IO_DATA));
    
    if (p_data == NULL) {
        *p_err = PERI_ERR_ALLOC;
        return (NULL);
    }
    
    memset(p_data, 0x00, sizeof(IO_DATA));

    p_data->pin_cnt = pin_cnt;
    memcpy(p_data->pin_tbl, p_pin_tbl, sizeof(HW_IF_DATA_TYPE *) * pin_cnt);
    memcpy(p_data->bit_tbl, p_bit_tbl, sizeof(IO_PORT_BIT      ) * pin_cnt);
    
    p_mem = (IO_MEM *)Mem_GetAddr(p_mem_data,
                                  PERI_IO_BASE_ADDR,
                                 &mem_err);
    
    if (p_mem == NULL) {
        free(p_data);
        *p_err = PERI_ERR_INVALID_MEM;
        return (NULL);
    }
    
    memset(p_mem, 0x00, sizeof(ISR_MEM));
    p_mem->PortB.TRIS   = PERI_IO_TRISB_INIT;
    p_mem->PortC.TRIS   = PERI_IO_TRISC_INIT;
    p_mem->PortD.TRIS   = PERI_IO_TRISD_INIT;
    p_mem->PortE.TRIS   = PERI_IO_TRISE_INIT;
    p_mem->PortF.TRIS   = PERI_IO_TRISF_INIT;
     
    p_io = (IO *)malloc(sizeof(IO));
    
    if (p_io == NULL) {
        free(p_data);
        *p_err = PERI_ERR_ALLOC;
        return (NULL);
    }
    
    p_io->p_mem  = p_mem;
    p_io->p_data = p_data;


    *p_err = PERI_ERR_NONE;
    return p_io;
}

void Peri_IO(MEM_24       *p_mem_prog,
              MEM         *p_mem_data,
              CORE_24F    *p_core,
              IO          *p_io,
              PERI_ERR    *p_err)
{
    CPU_INT08U   ix;
    IO_DATA      *p_data;
    IO_MEM       *p_mem;
    PORT_IO_MEM  *p_port_io_mem;


    p_data = p_io->p_data;
    p_mem  = p_io->p_mem;

    for (ix = 0 ; ix < p_data->pin_cnt ; ix++) {
        switch(p_data->bit_tbl[ix].port) {
            case 0:
                p_port_io_mem = &p_mem->PortB;
                break;

            case 1:
                p_port_io_mem = &p_mem->PortC;
                break;

            case 2:
                p_port_io_mem = &p_mem->PortD;
                break;

            case 3:
                p_port_io_mem = &p_mem->PortE;
                break;

            case 4:
                p_port_io_mem = &p_mem->PortF;
                break;

            default:
                *p_err = PERI_ERR_INVALID_MEM;
                return;
        }
        
        if (p_port_io_mem->TRIS & (1 << p_data->bit_tbl[ix].bit)) {             /* Pin is input */
            if ((p_data->pin_state[ix] == 0) && (*p_data->pin_tbl[ix] >= HW_SCHMITT_HI_TRIG)) {
                p_data->pin_state[ix]  = 1;
                p_port_io_mem->PORT   |= (1 << p_data->bit_tbl[ix].bit);
                p_port_io_mem->LAT    |= (1 << p_data->bit_tbl[ix].bit);
            } else if ((p_data->pin_state[ix] == 1) && (*p_data->pin_tbl[ix] <= HW_SCHMITT_LO_TRIG)) {
                p_data->pin_state[ix]  = 0;
                p_port_io_mem->PORT   &= ~(1 << p_data->bit_tbl[ix].bit);
                p_port_io_mem->LAT    &= ~(1 << p_data->bit_tbl[ix].bit);
            }
        } else {                                                                /* Pin is output */
            if (p_port_io_mem->PORT & (1 << p_data->bit_tbl[ix].bit)) {
                *p_data->pin_tbl[ix] = HW_VDD_VOLT;                             /* HI */
            } else {
                *p_data->pin_tbl[ix] = HW_VSS_VOLT;                             /* LO */
            }
        }
    }

    *p_err = PERI_ERR_NONE;
}