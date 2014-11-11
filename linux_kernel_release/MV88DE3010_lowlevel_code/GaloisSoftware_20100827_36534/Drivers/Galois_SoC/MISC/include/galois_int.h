#include <cyg/infra/cyg_type.h>         // base types
#include <cyg/hal/drv_api.h>

#define DSR_COUNT_ZERO	0
void galois_init_dsr_count(void);
int galois_dsr_get_count(int vec);

cyg_uint32 GALOIS_ISR(cyg_vector_t vector, cyg_addrword_t data);
void GALOIS_DSR(cyg_vector_t vector, cyg_ucount32 count, cyg_addrword_t data);
#if ((BERLIN_CHIP_VERSION >= BERLIN_B_0) && defined(AUDIO_HW_APP_INTR))
int is_HW_APP_intr(void);
cyg_uint32 HW_APP_ISR(cyg_vector_t vector, cyg_addrword_t data);
void HW_APP_DSR(cyg_vector_t vector, cyg_ucount32 count, cyg_addrword_t data);
#endif
void galois_interrupt_unmask(cyg_uint32 cpu_id, cyg_uint32 vec);
void galois_interrupt_mask(cyg_uint32 cpu_id, cyg_uint32 vec);

void GALOIS_DMA_AVIO_ISR(cyg_uint32 vec, cyg_addrword_t data);
void GALOIS_VOP_ISR(cyg_uint32 vec, cyg_addrword_t data);
void GALOIS_SEMA_VPRO_ISR(cyg_uint32 vec, cyg_addrword_t data);
void GALOIS_DMA_VPRO_ISR(cyg_uint32 vec, cyg_addrword_t data);
void GALOIS_GFX_ISR(cyg_uint32 vec, cyg_addrword_t data);
void GALOIS_DRM_ISR(cyg_uint32 vec, cyg_addrword_t data);
void GALOIS_I2S_ISR(cyg_uint32 vec, cyg_addrword_t data);
void GALOIS_ICTL_ISR(cyg_uint32 vec, cyg_addrword_t data);
void GALOIS_RTC_interrupt_acknowledge(void);
void GALOIS_DMA_AVIO_interrupt_acknowledge(void);
void GALOIS_VOP_interrupt_acknowledge(void);
void GALOIS_SEMA_VPRO_interrupt_acknowledge(void);
void GALOIS_DMA_VPRO_interrupt_acknowledge(void);
void GALOIS_GFX_interrupt_acknowledge(void);
void GALOIS_SATA_interrupt_acknowledge(void);
void GALOIS_DRM_interrupt_acknowledge(void);
void GALOIS_I2S_interrupt_acknowledge(void);
void GALOIS_INTER_HOST_H_interrupt_acknowledge(void);
void GALOIS_INTER_HOST_L_interrupt_acknowledge(void);

void system_timer_isr(cyg_uint32 timer_N);
void TIMERx_isr(cyg_uint32 timer_N);
void WATCHDOG_isr(cyg_uint32 wid);
void SPI_isr(cyg_uint32 spi_id);
void I2C_isr(cyg_uint32 i2c_id);
void GPIO_isr(cyg_uint32 gpio_id);
void RTC_isr(cyg_uint32 PIC_vec);
void GALOIS_ICTL_DSR(cyg_uint32 vec, cyg_addrword_t data);
void RTC_dsr(cyg_uint32 PIC_vec, cyg_addrword_t data);
