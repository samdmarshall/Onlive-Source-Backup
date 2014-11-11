/*
 * Linux uses timer 0
 */
#ifdef CONFIG_MV88DE3010_BERLIN

/* Refer to Galois_memmap.h, pic.h, apb_ictl.h, apb_perf_base.h */
#define G_MEMMAP_PIC_REG_BASE			0xF8420000

/* programmable interrupt controller base address offset for CPUx */
#define G_RA_PIC_CPU0					0x00000
#define G_RA_PIC_CPU1					0x00020
#define G_RA_PIC_cmn					0x00090

#define G_RA_perPIC_nGIntE				0x0001C
#define G_RA_perPIC_nIntE				0x00018

#define G_RA_PIC_int_CPU0toCPU1			0x00060
#define G_RA_PIC_int_CPU1toCPU0			0x00070
#define G_RA_PIC_int_PCIEtoCPU0			0x00080
#define G_RA_PIC_int_PCIEtoCPU1			0x00088

/* interrupt status regisiter */
#define G_RA_cmn_nIntSts0				0x0003C
#define G_RA_cmn_nIntSts1				0x00040

#define G_APB_ICTL_INST0_BASE			0xF7E83000 /* 0xF7E80000+0x3000 */
#define G_APB_ICTL_INST1_BASE			0xF7E83400 /* 0xF7E80000+0x3400 */

#define G_APB_ICTL_IRQ_FINALSTATUS_L	0x30
#define G_SM_ICTL1_BASE					0xF7FCE000 /* 0xF7F80000+0x40000+0xE000 */
#define G_SM_ICTL1_IRQ_FINALSTATUS_L	0x30

/* 
 *  Interrupt numbers
 */
#define G_IRQ_dHubIntrAvio0		0
#define G_IRQ_dHubIntrAvio1		1
#define G_IRQ_dmaIntrVpro		2
#define G_IRQ_ictlInstCPUIrq	3
#define G_IRQ_pex2intr_int		4
#define G_IRQ_pex2intr_err		5
#define G_IRQ_intrGfxM0			6
#define G_IRQ_SATAintr			7
#define G_IRQ_ethIrq			8
#define G_IRQ_drmIntr			9
#define G_IRQ_drmFigoInt		10
#define G_IRQ_usb0Int			11
#define G_IRQ_usb1Int			12
#define G_IRQ_sm2socInt			13
#define G_IRQ_ptpInt			14

#ifdef CONFIG_MV88DE3010_BERLIN_B0
#define G_IRQ_sm2socHwInt0		15
#define G_IRQ_sm2socHwInt1		16
#define G_IRQ_sdio_interrupt	17
#define G_IRQ_intrPb			18
#define G_IRQ_zspInt			19
#define G_IRQ_PIC_MASK			0xFFF00000
#else
#define G_IRQ_PIC_MASK			0xFFFF8000
#endif

#ifdef CONFIG_MV88DE3010_BERLIN_CPU1
#define G_IRQ_CPU0_TO_CPU1		30
#define G_IRQ_PCIE_TO_CPU1		31
#else
#define G_IRQ_CPU1_TO_CPU0		30
#define G_IRQ_PCIE_TO_CPU0		31
#endif

#define G_IRQ_APB_LSbGpioInst0	32
#define G_IRQ_APB_LSbGpioInst1	33
#define G_IRQ_APB_LSbGpioInst2	34
#define G_IRQ_APB_LSbGpioInst3	35
#define G_IRQ_APB_LSbSsiInst1	36
#define G_IRQ_APB_LSbWdtInst0	37
#define G_IRQ_APB_LSbWdtInst1	38
#define G_IRQ_APB_LSbWdtInst2	39
#define G_IRQ_APB_LSbTimerInst1_0	40
#define G_IRQ_APB_LSbTimerInst1_1	41
#define G_IRQ_APB_LSbTimerInst1_2	42
#define G_IRQ_APB_LSbTimerInst1_3	43
#define G_IRQ_APB_LSbTimerInst1_4	44
#define G_IRQ_APB_LSbTimerInst1_5	45
#define G_IRQ_APB_LSbTimerInst1_6	46
#define G_IRQ_APB_LSbTimerInst1_7	47
#define G_IRQ_APB_LSbI2cInst0		48
#define G_IRQ_APB_LSbI2cInst1_ic_gen_call_intr	49
#define G_IRQ_APB_LSbI2cInst1_ic_rx_under_intr	50
#define G_IRQ_APB_LSbI2cInst1_ic_rx_over_intr	51
#define G_IRQ_APB_LSbI2cInst1_ic_rx_full_intr	52
#define G_IRQ_APB_LSbI2cInst1_ic_tx_over_intr	53
#define G_IRQ_APB_LSbI2cInst1_ic_tx_empty_intr	54
#define G_IRQ_APB_LSbI2cInst1_ic_rd_req_intr	55
#define G_IRQ_APB_LSbI2cInst1_ic_tx_abrt_intr	56
#define G_IRQ_APB_LSbI2cInst1_ic_rx_done_intr	57
#define G_IRQ_APB_LSbI2cInst1_ic_activity_intr	58
#define G_IRQ_APB_LSbI2cInst1_ic_stop_det_intr	59
#define G_IRQ_APB_LSbI2cInst1_ic_start_det_intr	60
#define G_IRQ_APB_ICTL_MASK		0xE0000000 /* mask out bit 31~29, only count bit 0~28 */

#define G_IRQ_SM_Watchdog0		64
#define G_IRQ_SM_Watchdog1		65
#define G_IRQ_SM_Watchdog2		66
#define G_IRQ_SM_Timers			67
#define G_IRQ_SM_GPIO1			68 /* for GPIO ports 8~15 */
#define G_IRQ_SM_SPI			69
#define G_IRQ_SM_I2C0			70
#define G_IRQ_SM_I2C1			71
#define G_IRQ_SM_UART0			72
#define G_IRQ_SM_UART1			73
#define G_IRQ_SM_UART2			74
#define G_IRQ_SM_GPIO0			75 /* for GPIO ports 0~7 */
#define G_IRQ_SM_ADC			76
#define G_IRQ_SM_SOC2SM			77 /* SM ICTL0 only, never happen */
#define G_IRQ_SM_ICTL1_MASK		0xFFFFC000 /* mask out bit 31~14, only count bit 13~0 */

#define GALOIS_IRQ_START 		0
#define GALOIS_APB_IRQ_START 	32
#define GALOIS_SM_IRQ_START		64

#ifdef CONFIG_MV88DE3010_BERLIN_CPU1
#define LINUX_TIMER 	1
#define TIMER_IRQ		G_IRQ_APB_LSbTimerInst1_1
#else
#define LINUX_TIMER 	0
#define TIMER_IRQ		G_IRQ_APB_LSbTimerInst1_0
#endif
#define NR_IRQS			96

/* For compatible with Galois */
#define GALOIS_ICTL0_VECTOR	G_IRQ_ictlInstCPUIrq
#define IRQ_SATAINTR		G_IRQ_SATAintr
#define IRQ_ETHINTR 		G_IRQ_ethIrq
#ifdef CONFIG_MV88DE3010_BERLIN_UARTBUG
#define IRQ_APB_UARTINST0 	0
#define IRQ_APB_UARTINST1 	0
#define IRQ_APB_UARTINST2 	0
#else
#define IRQ_APB_UARTINST0 	G_IRQ_SM_UART0
#define IRQ_APB_UARTINST1 	G_IRQ_SM_UART1
#define IRQ_APB_UARTINST2 	G_IRQ_SM_UART2
#endif
#define IRQ_APB_TWSIINST0	G_IRQ_APB_LSbI2cInst0
#define IRQ_APB_TWSIINST1	G_IRQ_APB_LSbI2cInst1_ic_gen_call_intr
#define IRQ_APB_TWSIINST2	G_IRQ_SM_I2C0
#define IRQ_APB_TWSIINST3	G_IRQ_SM_I2C1
#define IRQ_APB_SSIINST1	G_IRQ_APB_LSbSsiInst1
#define IRQ_USB0		G_IRQ_usb0Int
#define IRQ_USB1		G_IRQ_usb1Int
#define IRQ_APB_GPIOINST0	G_IRQ_APB_LSbGpioInst0
#define IRQ_APB_GPIOINST1	G_IRQ_APB_LSbGpioInst1
#define IRQ_APB_GPIOINST2	G_IRQ_APB_LSbGpioInst2
#define IRQ_APB_GPIOINST3	G_IRQ_APB_LSbGpioInst3

#else /* BERLIN */

/*
 * AHB PIC space
 */
#define GALOIS_PIC_REG_BASE 0xF8400000

/* Per PIC register space size is 0x20 */
#define GALOIS_PIC0_fINTE 0x08
#define GALOIS_PIC0_fGINTE 0x0C
#define GALOIS_PIC0_nINTE 0x18
#define GALOIS_PIC0_nGINTE 0x1C

#define GALOIS_PIC1_fINTE 0x28
#define GALOIS_PIC1_fGINTE 0x2C
#define GALOIS_PIC1_nINTE 0x38
#define GALOIS_PIC1_nGINTE 0x3C

#define GALOIS_PIC2_fINTE 0x48
#define GALOIS_PIC2_fGINTE 0x4C
#define GALOIS_PIC2_nINTE 0x58
#define GALOIS_PIC2_nGINTE 0x5C

/* Inter-CPU interrupts register space */
#define GALOIS_PIC_INT_CPU0_CPU1 0x60
#define GALOIS_PIC_DATA_CPU0_CPU1 0x64
#define GALOIS_PIC_INT_CPU0_PCIE 0x68
#define GALOIS_PIC_DATA_CPU0_PCIE 0x6C
#define GALOIS_PIC_INT_CPU1_CPU0 0x70
#define GALOIS_PIC_DATA_CPU1_CPU0 0x74
#define GALOIS_PIC_INT_CPU1_PCIE 0x78
#define GALOIS_PIC_DATA_CPU1_PCIE 0x7C
#define GALOIS_PIC_INT_PCIE_CPU0 0x80
#define GALOIS_PIC_DATA_PCIE_CPU0 0x84
#define GALOIS_PIC_INT_PCIE_CPU1 0x88
#define GALOIS_PIC_DATA_PCIE_CPU1 0x8C

/* Common PIC register space starts from 0x90 */ 
#define GALOIS_PIC_fSTATUS_REG 0x98
#define GALOIS_PIC_nSTATUS_REG 0xA4

/*
 * APB ICTL space
 */
#define GALOIS_APBPERIF_REG_BASE 0xF7F30000	/* just for reference */
#define GALOIS_APB_ICTL_REG_BASE 0xF7F30800

#define GALOIS_APB_ICTL_FINALSTATUS_L_REG 0x30
#define GALOIS_APB_ICTL_FINALSTATUS_H_REG 0x34

/* 
 *  Interrupt numbers
 */
#define IRQ_DMAINTR0AVIO		0
#define IRQ_DMAINTR1AVIO		1
#define IRQ_DMAINTR2AVIO		2
#define IRQ_VOPINT				3
#define IRQ_SEMINTR0VPRO		4
#define IRQ_SEMINTR1VPRO		5
#define IRQ_SEMINTR2VPRO		6
#define IRQ_DMAINTR0VPRO		7
#define IRQ_DMAINTR1VPRO		8
#define IRQ_DMAINTR2VPRO		9
#define IRQ_ICTLINST0CPU		10	/* GALOIS_ICTL0_VECTOR */
#define IRQ_ICTLINST1CPU		11	/* GALOIS_ICTL1_VECTOR */
#define IRQ_ICTLINST2CPU		12	/* GALOIS_ICTL2_VECTOR */
#define IRQ_PEX2INTR_INT		13
#define IRQ_PEX2INTR_ERR		14
#define IRQ_INTRGFXM0			15
#define IRQ_SATAINTR			16
#define IRQ_ETHINTR				17
#define IRQ_DRMINTR				18
#define IRQ_I2SINT				19

#define IRQ_APB_I2CINST2		32
#define IRQ_APB_GPIOINST0		33
#define IRQ_APB_UARTINST0		34
#define IRQ_APB_I2CINST0		35
#define IRQ_APB_UARTINST1		36
#define IRQ_APB_SSIINST1		37
#define IRQ_APB_SSIINST0		38
#define IRQ_APB_SSIINST2		39
#define IRQ_APB_WDINST0			40
#define IRQ_APB_WDINST1			41
#define IRQ_APB_WDINST2			42
/* to be added */
#define IRQ_APB_I2CINST1_xx1	43
#define IRQ_APB_I2CINST1_xx2	44
#define IRQ_APB_I2CINST1_xx3	45
#define IRQ_APB_I2CINST1_xx4	46
#define IRQ_APB_I2CINST1_xx5	47
#define IRQ_APB_I2CINST1_xx6	48
#define IRQ_APB_I2CINST1_xx7	49
#define IRQ_APB_I2CINST1_xx8	50
#define IRQ_APB_I2CINST1_xx9	51
#define IRQ_APB_I2CINST1_xxa	52
#define IRQ_APB_I2CINST1_xxb	53
#define IRQ_APB_I2CINST1_xxc	54
#define IRQ_APB_TIMERINST1_0	55
#define IRQ_APB_TIMERINST1_1	56
#define IRQ_APB_TIMERINST1_2	57
#define IRQ_APB_TIMERINST1_3	58
#define IRQ_APB_TIMERINST1_4	59
#define IRQ_APB_TIMERINST1_5	60
#define IRQ_APB_TIMERINST1_6	61
#define IRQ_APB_TIMERINST1_7	62
#define IRQ_APB_I2CINST3		63
#define IRQ_APB_GPIOINST1		64
#define IRQ_APB_GPIOINST2		65
#define IRQ_APB_GPIOINST3		66

#define GALOIS_IRQ_START		0
#define GALOIS_APB_IRQ_START	32
#define GALOIS_APB_IRQ_START_H	64
#define GALOIS_GPIO_IRQ_START	96

#define GALOIS_ICTL0_VECTOR		IRQ_ICTLINST0CPU
#define GALOIS_ICTL1_VECTOR		IRQ_ICTLINST1CPU
#define GALOIS_ICTL2_VECTOR		IRQ_ICTLINST2CPU

#define LINUX_TIMER 			0
#define TIMER_IRQ				IRQ_APB_TIMERINST1_0	/* LINUX_TIMER=0 */
#define NR_IRQS					67

#endif /* BERLIN */
