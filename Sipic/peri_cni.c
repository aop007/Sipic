#include "peripheral.h"

CNI *Peri_CNI_Init(HW_IF_DATA_TYPE **p_pin_tbl,
                   CPU_INT08U       *p_bit_tbl,
                   CPU_INT16U        pin_cnt,
                   ISR_VECT_NUM      isr_vect_num,
                   MEM              *p_mem_data,
                   PERI_ERR         *p_err)
{
    CNI       *p_cni;
    CNI_MEM   *p_mem;
    CNI_DATA  *p_data;
    MEM_ERR    mem_err;
    

    p_data = (CNI_DATA *)malloc(sizeof(CNI_DATA));
    
    if (p_data == NULL) {
        *p_err = PERI_ERR_ALLOC;
        return (NULL);
    }
    
    memset(p_data, 0x00, sizeof(CNI_DATA));

    memcpy(p_data->pin_tbl, p_pin_tbl, sizeof(HW_IF_DATA_TYPE *) * pin_cnt);
    memcpy(p_data->bit_tbl, p_bit_tbl, sizeof(CPU_INT08U       ) * pin_cnt);
    p_data->isr_num = isr_vect_num;
    p_data->pin_cnt = pin_cnt;
    
    p_mem = Mem_GetAddr(p_mem_data,
                        PERI_CNI_BASE_ADDR,
                        &mem_err);
    
    if (p_mem == NULL) {
        free(p_data);
        *p_err = PERI_ERR_INVALID_MEM;
        return (NULL);
    }
    
    memset(p_mem, 0x00, sizeof(CNI_MEM));
    
    p_cni = (CNI *)malloc(sizeof(CNI));
    
    if (p_cni == NULL) {
        free(p_data);
        *p_err = PERI_ERR_ALLOC;
        return (NULL);
    }
    
    p_cni->p_mem  = p_mem;
    p_cni->p_data = p_data;
    
    *p_err = PERI_ERR_NONE;
    return p_cni;
}

void Peri_CNI(MEM_24       *p_mem_prog,
              MEM          *p_mem_data,
              CORE_24F     *p_core,
              CNI          *p_cni,
              PERI_ERR     *p_err)
{
    CNI_DATA         *p_data;
    CNI_MEM          *p_mem;
    CPU_INT32U        ix;
    CPU_BOOLEAN       pin_as_changed;
    CPU_BOOLEAN       issue_isr;
    CPU_INT16U        reg;
    CPU_INT16U        bit;
    
    
    p_mem  = (CNI_MEM  *)p_cni->p_mem;
    p_data = (CNI_DATA *)p_cni->p_data;
    issue_isr = DEF_NO;
    
    
    for (ix = 0 ; ix < p_data->pin_cnt ; ix++) {
        reg = p_data->bit_tbl[ix] / 16;
        bit = p_data->bit_tbl[ix] % 16;
        pin_as_changed = DEF_NO;
        
        if ((p_data->previous_pin_state[ix] == 0) && (*p_data->pin_tbl[ix] >= HW_SCHMITT_HI_TRIG)) {
            p_data->previous_pin_state[ix] = 1;
            pin_as_changed = DEF_YES;
        } else if ((p_data->previous_pin_state[ix] == 1) && (*p_data->pin_tbl[ix] <= HW_SCHMITT_LO_TRIG)) {
            p_data->previous_pin_state[ix] = 0;
            pin_as_changed = DEF_YES;
        }
        
        if ((p_mem->CNEN[reg] & (1 << bit)) && (pin_as_changed == DEF_YES)) {
            issue_isr = DEF_YES;
        }
        
    }
    
    if (issue_isr == DEF_YES) {
        ISR_Post(p_data->isr_num, p_err);
    }
}