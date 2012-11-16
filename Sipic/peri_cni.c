#include "peripheral.h"

CNI *Peri_CNI_Init(HW_IF_DATA_TYPE **p_pin_tbl,
                   CPU_INT16U        pin_cnt,
                   ISR_VECT_NUM      isr_vect_num,
                   MEM              *p_mem_data,
                   PERI_ERR         *p_err)
{
    CNI       *p_cni;
    CNI_MEM   *p_mem;
    CNI_DATA  *p_data;
    MEM_ERR   mem_err;
    
    p_data = malloc(sizeof(CNI_DATA));
    
    if (p_data == NULL) {
        *p_err = PERI_ERR_ALLOC;
        return (NULL);
    }
    
    memset(p_data, 0x00, sizeof(CNI_DATA));
    
    p_mem = Mem_GetAddr(p_mem_data,
                        PERI_CNI_BASE_ADDR,
                        &mem_err);
    
    if (p_mem == NULL) {
        free(p_data);
        *p_err = PERI_ERR_INVALID_MEM;
        return (NULL);
    }
    
    memset(p_mem, 0x00, sizeof(CNI_MEM));
    
    p_cni = malloc(sizeof(CNI));
    
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

/*
typedef struct cni_data {
    ISR_VECT_NUM      isr_num;
    HW_IF_DATA_TYPE   previous_pin_level[CNI_PIN_CNT];
    CPU_BOOLEAN       previous_pin_state[CNI_PIN_CNT];
    HW_IF_DATA_TYPE  *pin_tbl[CNI_PIN_CNT];
} CNI_DATA;

*/

void Peri_CNI(MEM_24       *p_mem_prog,
              MEM          *p_mem_data,
              CORE_24F     *p_core,
              CNI          *p_cni,
              PERI_ERR     *p_err)
{
    CNI_DATA         *p_data;
    HW_IF_DATA_TYPE  *p_pin;
    CPU_INT32U        ix;
    
    p_data = (CNI_DATA *)p_cni->p_data;
    
    for (ix = 0 ; ix < CNI_PIN_CNT ; ix++) {
        if (previous_pin_state == 0) && (*pin_tbl[i] )    
    }
}