#ifndef __GALOIS_INT_REGISTER_H__
#define __GALOIS_INT_REGISTER_H__

#if !defined(BERLIN_SINGLE_CPU)

#include "galois_int.h"
#include "dev_init_def.h"

#define MAX_PIC_VECTORS 32
typedef struct {
	void (*irq_init_func)(cyg_uint32 cpu_id); /* 0 means we don't support this function */
	void (*irq_enable_func)(cyg_uint32 cpu_id, cyg_uint32 vec); /* 0 means we don't support this function */
	void (*irq_disable_func)(cyg_uint32 cpu_id, cyg_uint32 vec); /* 0 means we don't support this function */
	void (*irq_ack_func)(cyg_uint32 cpu_id, cyg_uint32 vec); /* 0 means we don't support this function */
	void (*irq_isr_func)(cyg_uint32 vec, cyg_addrword_t data); /* 0 means we don't support this function */
	void (*irq_dsr_func)(cyg_uint32 vec, cyg_addrword_t data); /* 0 means we don't support this function */
	cyg_addrword_t data;
	cyg_ucount32	dsr_count;
} IRQ_FUNC_T; 
extern IRQ_FUNC_T galois_interrupt_vectors[MAX_PIC_VECTORS];
//extern DEV_INIT_T  dev_init_tbl[];
extern UNSG32 dev_init_tbl_size;

cyg_uint32 Galois_dev_check(cyg_uint32 cpu_id, cyg_uint32 dev_type, cyg_uint32 *vec);

void Galois_interrupt_vector_registration(int vec,
	void (*irq_init_func)(cyg_uint32 cpu_id),
	void (*irq_enable_func)(cyg_uint32 cpu_id, cyg_uint32 vec),
	void (*irq_disable_func)(cyg_uint32 cpu_id, cyg_uint32 vec),
	void (*irq_ack_func)(cyg_uint32 cpu_id, cyg_uint32 vec),
	void (*irq_isr_func)(cyg_uint32 vec, cyg_addrword_t data),
	void (*irq_dsr_func)(cyg_uint32 vec, cyg_addrword_t data),
	cyg_addrword_t data,
	cyg_ucount32 dsr_count);

void Galois_interrupt_register(void);
void galois_resume_sync(int value);
void Galois_initialization(cyg_uint32 cpu_id);

#else
void Galois_initialization(unsigned int cpu_id);
#endif

#endif //__GALOIS_INT_REGISTER_H__
