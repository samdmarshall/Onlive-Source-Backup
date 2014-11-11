#ifndef __APB_ICTL_H__
#define __APB_ICTL_H__
#define APB_ICTL_IRQ_INTEN_L		0x00
#define APB_ICTL_IRQ_INTEN_H		0x04
#define APB_ICTL_IRQ_INTMASK_L		0x08 
#define APB_ICTL_IRQ_INTMASK_H		0x0C 
#define APB_ICTL_IRQ_INTFORCE_L		0x10 
#define APB_ICTL_IRQ_INTFORCE_H		0x14
#define APB_ICTL_IRQ_RAWSTATUS_L	0x18 
#define APB_ICTL_IRQ_RAWSTATUS_H	0x1C 
#define APB_ICTL_IRQ_STATUS_L		0x20 
#define APB_ICTL_IRQ_STATUS_H		0x24
#define APB_ICTL_IRQ_MASKSTATUS_L	0x28
#define APB_ICTL_IRQ_MASKSTATUS_H	0x2C 
#define APB_ICTL_IRQ_FINALSTATUS_L	0x30
#define APB_ICTL_IRQ_FINALSTATUS_H	0x34 
#define APB_ICTL_IRQ_VECTOR			0x38 
#define APB_ICTL_IRQ_VECTOR_0		0x40 
#define APB_ICTL_IRQ_VECTOR_1		0x48 
#define APB_ICTL_IRQ_VECTOR_2		0x50
#define APB_ICTL_IRQ_VECTOR_3		0x58
#define APB_ICTL_IRQ_VECTOR_4		0x60 
#define APB_ICTL_IRQ_VECTOR_5		0x68
#define APB_ICTL_IRQ_VECTOR_6		0x70 
#define APB_ICTL_IRQ_VECTOR_7		0x78 
#define APB_ICTL_IRQ_VECTOR_8		0x80 
#define APB_ICTL_IRQ_VECTOR_9		0x88 
#define APB_ICTL_IRQ_VECTOR_10		0x90 
#define APB_ICTL_IRQ_VECTOR_11		0x98 
#define APB_ICTL_IRQ_VECTOR_12		0xA0 
#define APB_ICTL_IRQ_VECTOR_13		0xA8 
#define APB_ICTL_IRQ_VECTOR_14		0xB0 
#define APB_ICTL_IRQ_VECTOR_15		0xB8 
#define APB_ICTL_FIQ_INTEN			0xC0 
#define APB_ICTL_FIQ_INTMASK		0xC4
#define APB_ICTL_FIQ_INTFORCE		0xC8 
#define APB_ICTL_FIQ_RAWSTATUS		0xCC 
#define APB_ICTL_FIQ_STATUS			0xD0 
#define APB_ICTL_FIQ_FINALSTATUS	0xD4 
#define APB_ICTL_IRQ_PLEVEL			0xD8 
#define APB_ICTL_COMP_VERSION		0xE0
#define APB_ICTL_IRQ_PN_OFFSET		0xE8 /* +N*4, for N=0 to ICTL_IRQ_NUM-1 */

#if !defined(BERLIN)
#define LSbI2cInst2						0x00
#define LSbGpioInst0					0x01
#define LSbUartInst0					0x02
#define LSbI2cInst0						0x03
#define LSbUartInst1					0x04
#define LSbSsiInst1						0x05
#define LSbSsiInst0						0x06
#define LSbSsiInst2						0x07
#define LSbWdtInst0						0x08
#define LSbWdtInst1						0x09
#define LSbWdtInst2						0x0A
#define LSbI2cInst1_ic_gen_call_intr	0x0B
#define LSbI2cInst1_ic_rx_under_intr	0x0C
#define LSbI2cInst1_ic_rx_over_intr		0x0D
#define LSbI2cInst1_ic_rx_full_intr		0x0E
#define LSbI2cInst1_ic_tx_over_intr		0x0F
#define LSbI2cInst1_ic_tx_empty_intr	0x10
#define LSbI2cInst1_ic_rd_req_intr		0x11
#define LSbI2cInst1_ic_tx_abrt_intr		0x12
#define LSbI2cInst1_ic_rx_done_intr		0x13
#define LSbI2cInst1_ic_activity_intr	0x14
#define LSbI2cInst1_ic_stop_det_intr	0x15
#define LSbI2cInst1_ic_start_det_intr	0x16
#define LSbTimerInst1_0					0x17
#define LSbTimerInst1_1					0x18
#define LSbTimerInst1_2					0x19
#define LSbTimerInst1_3					0x1A
#define LSbTimerInst1_4					0x1B
#define LSbTimerInst1_5					0x1C
#define LSbTimerInst1_6					0x1D
#define LSbTimerInst1_7					0x1E
#define LSbI2cInst3						0x1F
#define LSbGpioInst1					0x20
#define LSbGpioInst2					0x21
#define LSbGpioInst3					0x22
#else  // BERLIN ( Email from Tzun-wei 12/26/07)
#define LSbGpioInst0					0x00	// IRQ 0       gpioInst0
#define LSbGpioInst1					0x01	// IRQ 1       gpioInst1
#define LSbGpioInst2					0x02	// IRQ 2       gpioInst2
#define LSbGpioInst3					0x03	// IRQ 3       gpioInst3
#define LSbSsiInst1						0x04	// IRQ 4       ssiInst1
#define LSbWdtInst0						0x05	// IRQ 5       wdtInst0
#define LSbWdtInst1						0x06	// IRQ 6       wdtInst1
#define LSbWdtInst2						0x07	// IRQ 7       wdtInst2
#define LSbTimerInst1_0					0x08	// IRQ 8       timerInst/intr0
#define LSbTimerInst1_1					0x09	// IRQ 9       timerInst/intr1
#define LSbTimerInst1_2					0x0A	// IRQ 10      timerInst/intr2
#define LSbTimerInst1_3					0x0B	// IRQ 11      timerInst/intr3
#define LSbTimerInst1_4					0x0C	// IRQ 12      timerInst/intr4
#define LSbTimerInst1_5					0x0D	// IRQ 13      timerInst/intr5
#define LSbTimerInst1_6					0x0E	// IRQ 14      timerInst/intr6
#define LSbTimerInst1_7					0x0F	// IRQ 15      timerInst/intr7
#define LSbI2cInst0						0x10	// IRQ 16      i2cInst0
#define LSbI2cInst1_ic_gen_call_intr	0x11	// IRQ 17      i2cInst1/ic_gen_call_intr
#define LSbI2cInst1_ic_rx_under_intr	0x12	// IRQ 18      i2cInst1/ic_rx_under_intr
#define LSbI2cInst1_ic_rx_over_intr		0x13	// IRQ 19      i2cInst1/ic_rx_over_intr
#define LSbI2cInst1_ic_rx_full_intr		0x14	// IRQ 20      i2cInst1/ic_rx_full_intr
#define LSbI2cInst1_ic_tx_over_intr		0x15	// IRQ 21      i2cInst1/ic_tx_over_intr
#define LSbI2cInst1_ic_tx_empty_intr	0x16	// IRQ 22      i2cInst1/ic_tx_empty_intr
#define LSbI2cInst1_ic_rd_req_intr		0x17	// IRQ 23      i2cInst1/ic_rd_req_intr
#define LSbI2cInst1_ic_tx_abrt_intr		0x18	// IRQ 24      i2cInst1/ic_tx_abrt_intr
#define LSbI2cInst1_ic_rx_done_intr		0x19	// IRQ 25      i2cInst1/ic_rx_done_intr
#define LSbI2cInst1_ic_activity_intr	0x1A	// IRQ 26      i2cInst1/ic_activity_intr
#define LSbI2cInst1_ic_stop_det_intr	0x1B	// IRQ 27      i2cInst1/ic_stop_det_intr
#define LSbI2cInst1_ic_start_det_intr	0x1C	// IRQ 28      i2cInst1/ic_start_det_intr
#endif
#endif // __APB_ICTL_H__

