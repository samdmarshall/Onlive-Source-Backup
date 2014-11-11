#ifndef __DEV_INIT_DEF_H__
#define __DEV_INIT_DEF_H__
#define	DRV_GRP_PIC			0x01
#define	DRV_GRP_ICTL		0x02
#define	DRV_GRP_DMA			0x03
#define	DRV_GRP_VOP			0x04
#define	DRV_GRP_GFX			0x05
#define	DRV_GRP_SATA		0x06
#define	DRV_GRP_ETH			0x07
#define	DRV_GRP_DRM			0x08
#define	DRV_GRP_I2S			0x09
#define	DRV_GRP_PCIE		0x0A
#define	DRV_GRP_INTER_HOST_L	0x0B
#define	DRV_GRP_INTER_HOST_H	0x0C
#define LSb_DRV_GRP			16

#define DRV_TOTAL_ID_1		0x01
#define DRV_TOTAL_ID_2		0x02
#define DRV_TOTAL_ID_3		0x03
#define LSb_DRV_TOTAL_ID	8

#define DRV_CUR_ID0			0x0
#define DRV_CUR_ID1			0x1
#define DRV_CUR_ID2			0x2
#define LSb_DRV_CUR_ID		0


#define G_DEV_ID0   0
#define G_DEV_ID1   1
#define G_DEV_ID2   2
#define G_DEV_ID3   3
#define G_DEV_ID4   4
#define G_DEV_ID5   5
#define G_DEV_ID6   6
#define G_DEV_ID7   7

#define MASK_DRV_BYTES		0xFF



#define GRP_PIC_CPU0		((DRV_GRP_PIC << LSb_DRV_GRP) | (DRV_TOTAL_ID_3 << LSb_DRV_TOTAL_ID) | (DRV_CUR_ID0 << LSb_DRV_CUR_ID))
#define GRP_PIC_CPU1		((DRV_GRP_PIC << LSb_DRV_GRP) | (DRV_TOTAL_ID_3 << LSb_DRV_TOTAL_ID) | (DRV_CUR_ID1 << LSb_DRV_CUR_ID))
#define GRP_PIC_CPU2		((DRV_GRP_PIC << LSb_DRV_GRP) | (DRV_TOTAL_ID_3 << LSb_DRV_TOTAL_ID) | (DRV_CUR_ID2 << LSb_DRV_CUR_ID))

#define GRP_ICTL_DEV0		((DRV_GRP_ICTL << LSb_DRV_GRP) | (DRV_TOTAL_ID_3 << LSb_DRV_TOTAL_ID) | (DRV_CUR_ID0 << LSb_DRV_CUR_ID))
#define GRP_ICTL_DEV1		((DRV_GRP_ICTL << LSb_DRV_GRP) | (DRV_TOTAL_ID_3 << LSb_DRV_TOTAL_ID) | (DRV_CUR_ID1 << LSb_DRV_CUR_ID))
#define GRP_ICTL_DEV2		((DRV_GRP_ICTL << LSb_DRV_GRP) | (DRV_TOTAL_ID_3 << LSb_DRV_TOTAL_ID) | (DRV_CUR_ID2 << LSb_DRV_CUR_ID))


#define GRP_DEV_IRQ_ENABLE_ONLY	3	// only irq is enabled, device is not enabled. (not likely to happen)
#define GRP_DEV_VEC_ENABLE_ONLY	2	// only deviced is enabled, irq is not enabled.
#define GRP_DEV_VEC_ENABLE		1	// both device and irq are enabled.
#define GRP_DEV_VEC_DISABLE		0	// both device and irq are disabled.

#if defined(BERLIN)
#define DEV_TYPE_DHUB_AVIO0   	0x00
#define DEV_TYPE_DHUB_AVIO1   	(DEV_TYPE_DHUB_AVIO0 + 1)
#define DEV_TYPE_DMA_VPRO		(DEV_TYPE_DHUB_AVIO1 + 1)
#define DEV_TYPE_ICTL			(DEV_TYPE_DMA_VPRO + 1)
#define DEV_TYPE_PEX_INT		(DEV_TYPE_ICTL + 1)
#define DEV_TYPE_PEX_ERR		(DEV_TYPE_PEX_INT + 1)
#define DEV_TYPE_GFX			(DEV_TYPE_PEX_ERR + 1)
#define DEV_TYPE_SATA			(DEV_TYPE_GFX + 1)
#define DEV_TYPE_ETH			(DEV_TYPE_SATA + 1)
#define DEV_TYPE_DRM			(DEV_TYPE_ETH + 1)
#define	DEV_TYPE_DRM_FIGO		(DEV_TYPE_DRM + 1)
#define DEV_TYPE_USB0			(DEV_TYPE_DRM_FIGO + 1)
#define DEV_TYPE_USB1			(DEV_TYPE_USB0 + 1)
#define	DEV_TYPE_SM2SOC			(DEV_TYPE_USB1 + 1)
#define DEV_TYPE_PTP			(DEV_TYPE_SM2SOC + 1)
#define DEV_TYPE_INTER_HOST_L	(DEV_TYPE_PTP + 1)
#define DEV_TYPE_INTER_HOST_H	(DEV_TYPE_INTER_HOST_L + 1)

#define DEV_TYPE_I2C			(DEV_TYPE_INTER_HOST_H + 1)
#define DEV_TYPE_GPIO			(DEV_TYPE_I2C + 1)
#define DEV_TYPE_UART			(DEV_TYPE_GPIO + 1)
#define DEV_TYPE_SSI			(DEV_TYPE_UART + 1)
#define DEV_TYPE_WDT			(DEV_TYPE_SSI + 1)
#define DEV_TYPE_TIMER			(DEV_TYPE_WDT + 1)
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)
#define DEV_TYPE_ADSP			(DEV_TYPE_TIMER +1)
#endif

/* Topology info of device connection */
#define DEV_TYPE_PIC_FIELD  DEV_TYPE_INTER_HOST_H
#define DEV_TYPE_ICTL_FIELD DEV_TYPE_TIMER
#define DEV_TYPE_SM_FIELD   DEV_TYPE_TIMER
#else
#define DEV_TYPE_DMA_AVIO   	0x00
#define DEV_TYPE_VOP			(DEV_TYPE_DMA_AVIO + 1)
#define DEV_TYPE_SEM_VPRO		(DEV_TYPE_VOP + 1)
#define DEV_TYPE_DMA_VPRO		(DEV_TYPE_SEM_VPRO + 1)
#define DEV_TYPE_ICTL			(DEV_TYPE_DMA_VPRO + 1)
#define DEV_TYPE_PEX_INT		(DEV_TYPE_ICTL + 1)
#define DEV_TYPE_PEX_ERR		(DEV_TYPE_PEX_INT + 1)
#define DEV_TYPE_GFX			(DEV_TYPE_PEX_ERR + 1)
#define DEV_TYPE_SATA			(DEV_TYPE_GFX + 1)
#define DEV_TYPE_ETH			(DEV_TYPE_SATA + 1)
#define DEV_TYPE_DRM			(DEV_TYPE_ETH + 1)
#define DEV_TYPE_I2S			(DEV_TYPE_DRM + 1)
#define DEV_TYPE_INTER_HOST_L	(DEV_TYPE_I2S + 1)
#define DEV_TYPE_INTER_HOST_H	(DEV_TYPE_INTER_HOST_L + 1)

#define DEV_TYPE_I2C			(DEV_TYPE_INTER_HOST_H + 1)
#define DEV_TYPE_GPIO			(DEV_TYPE_I2C + 1)
#define DEV_TYPE_UART			(DEV_TYPE_GPIO + 1)
#define DEV_TYPE_SSI			(DEV_TYPE_UART + 1)
#define DEV_TYPE_WDT			(DEV_TYPE_SSI + 1)
#define DEV_TYPE_TIMER			(DEV_TYPE_WDT + 1)

/* Topology info of device connection */
#define DEV_TYPE_PIC_FIELD  DEV_TYPE_INTER_HOST_H
#define DEV_TYPE_ICTL_FIELD DEV_TYPE_TIMER
#endif


typedef struct {
	UNSG32 group;		// group id and coresponding CPUx, for example PIC, ICTL, etc.
	UNSG32 inst_id;		// instance id of the current device. For example  I2c may have up to 4 instance.
	UNSG32 vector_id;	// vecotr id
	UNSG32 vector_dev_type; // dev type of the current vector.
	UNSG32 enable;		// if the vecotr is enabled to send the interrupt to CPUx or not
	UNSG32 data;		// parameters, if any. NULL for no data
} DEV_INIT_T;
#endif //__DEV_INIT_DEF_H__
