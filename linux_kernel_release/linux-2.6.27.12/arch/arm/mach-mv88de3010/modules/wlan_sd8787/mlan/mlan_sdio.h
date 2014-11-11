/** @file mlan_sdio.h
 * 
 *  @brief This file contains definitions for SDIO interface.
 *  
 *  Copyright (C) 2008-2010, Marvell International Ltd.
 *  All Rights Reserved
 */
/****************************************************
Change log:
****************************************************/

#ifndef	_MLAN_SDIO_H
#define	_MLAN_SDIO_H

/** Block mode */
#define BLOCK_MODE	1
/** Fixed address mode */
#define FIXED_ADDRESS	0

/** Control register for BUS interface */
#define BUS_INTERFACE_CONTROL_REG 	0x07

/** Asynchronous interrupt mode */
#define ASYNC_INT_MODE			0x20

/** Port for registers */
#define REG_PORT			0
/** LSB of read bitmap */
#define RD_BITMAP_L			0x04
/** MSB of read bitmap */
#define RD_BITMAP_U			0x05
/** LSB of write bitmap */
#define WR_BITMAP_L			0x06
/** MSB of write bitmap */
#define WR_BITMAP_U			0x07
/** LSB of read length for port 0 */
#define RD_LEN_P0_L			0x08
/** MSB of read length for port 0 */
#define RD_LEN_P0_U			0x09
/** Ctrl port */
#define CTRL_PORT			0
/** Ctrl port mask */
#define CTRL_PORT_MASK			0x0001
/** Data port mask */
#define DATA_PORT_MASK			0xfffe
/** Misc. Config Register : Auto Re-enable interrupts */
#define AUTO_RE_ENABLE_INT             	MBIT(4)

/* Host Control Registers */
/** Host Control Registers : I/O port 0 */
#define IO_PORT_0_REG			0x78
/** Host Control Registers : I/O port 1 */
#define IO_PORT_1_REG			0x79
/** Host Control Registers : I/O port 2 */
#define IO_PORT_2_REG			0x7A

/** Host Control Registers : Configuration */
#define CONFIGURATION_REG		0x00
/** Host Control Registers : Host without Command 53 finish host*/
#define HOST_TO_CARD_EVENT       (0x1U << 3)
/** Host Control Registers : Host without Command 53 finish host */
#define HOST_WO_CMD53_FINISH_HOST	(0x1U << 2)
/** Host Control Registers : Host power up */
#define HOST_POWER_UP			(0x1U << 1)
/** Host Control Registers : Host power down */
#define HOST_POWER_DOWN			(0x1U << 0)

/** Host Control Registers : Host interrupt mask */
#define HOST_INT_MASK_REG		0x02
/** Host Control Registers : Upload host interrupt mask */
#define UP_LD_HOST_INT_MASK		(0x1U)
/** Host Control Registers : Download host interrupt mask */
#define DN_LD_HOST_INT_MASK		(0x2U)
/** Enable Host interrupt mask */
#define HIM_ENABLE			(UP_LD_HOST_INT_MASK | DN_LD_HOST_INT_MASK)
/** Disable Host interrupt mask */
#define	HIM_DISABLE			0xff

/** Host Control Registers : Host interrupt status */
#define HOST_INTSTATUS_REG		0x03
/** Host Control Registers : Upload host interrupt status */
#define UP_LD_HOST_INT_STATUS		(0x1U)
/** Host Control Registers : Download host interrupt status */
#define DN_LD_HOST_INT_STATUS		(0x2U)

/** Host Control Registers : Host interrupt RSR */
#define HOST_INT_RSR_REG		0x01
/** Host Control Registers : Upload host interrupt RSR */
#define UP_LD_HOST_INT_RSR		(0x1U)

/** Host Control Registers : Host interrupt status */
#define HOST_INT_STATUS_REG		0x28
/** Host Control Registers : Upload CRC error */
#define UP_LD_CRC_ERR			(0x1U << 2)
/** Host Control Registers : Upload restart */
#define UP_LD_RESTART              	(0x1U << 1)
/** Host Control Registers : Download restart */
#define DN_LD_RESTART              	(0x1U << 0)

/* Card Control Registers */
/** Card Control Registers : Read SQ base address A0 register */
#define SQ_READ_BASE_ADDRESS_A0_REG  	0x40
/** Card Control Registers : Read SQ base address A1 register */
#define SQ_READ_BASE_ADDRESS_A1_REG  	0x41
/** Card Control Registers : Read SQ base address A2 register */
#define SQ_READ_BASE_ADDRESS_A2_REG  	0x42
/** Card Control Registers : Read SQ base address A3 register */
#define SQ_READ_BASE_ADDRESS_A3_REG  	0x43
/** Card Control Registers : Read SQ base address B0 register */
#define SQ_READ_BASE_ADDRESS_B0_REG  	0x44
/** Card Control Registers : Read SQ base address B1 register */
#define SQ_READ_BASE_ADDRESS_B1_REG  	0x45
/** Card Control Registers : Read SQ base address B2 register */
#define SQ_READ_BASE_ADDRESS_B2_REG  	0x46
/** Card Control Registers : Read SQ base address B3 register */
#define SQ_READ_BASE_ADDRESS_B3_REG  	0x47

/** Card Control Registers : Card status register */
#define CARD_STATUS_REG              	0x30
/** Card Control Registers : Card I/O ready */
#define CARD_IO_READY              	(0x1U << 3)
/** Card Control Registers : CIS card ready */
#define CIS_CARD_RDY                 	(0x1U << 2)
/** Card Control Registers : Upload card ready */
#define UP_LD_CARD_RDY               	(0x1U << 1)
/** Card Control Registers : Download card ready */
#define DN_LD_CARD_RDY               	(0x1U << 0)

/** Card Control Registers : Host interrupt mask register */
#define HOST_INTERRUPT_MASK_REG      	0x34
/** Card Control Registers : Host power interrupt mask */
#define HOST_POWER_INT_MASK          	(0x1U << 3)
/** Card Control Registers : Abort card interrupt mask */
#define ABORT_CARD_INT_MASK          	(0x1U << 2)
/** Card Control Registers : Upload card interrupt mask */
#define UP_LD_CARD_INT_MASK          	(0x1U << 1)
/** Card Control Registers : Download card interrupt mask */
#define DN_LD_CARD_INT_MASK          	(0x1U << 0)

/** Card Control Registers : Card interrupt status register */
#define CARD_INTERRUPT_STATUS_REG    	0x38
/** Card Control Registers : Power up interrupt */
#define POWER_UP_INT                 	(0x1U << 4)
/** Card Control Registers : Power down interrupt */
#define POWER_DOWN_INT               	(0x1U << 3)

/** Card Control Registers : Card interrupt RSR register */
#define CARD_INTERRUPT_RSR_REG       	0x3c
/** Card Control Registers : Power up RSR */
#define POWER_UP_RSR                 	(0x1U << 4)
/** Card Control Registers : Power down RSR */
#define POWER_DOWN_RSR               	(0x1U << 3)

/** Card Control Registers : Debug 0 register */
#define DEBUG_0_REG                  	0x70
/** Card Control Registers : SD test BUS 0 */
#define SD_TESTBUS0                  	(0x1U)
/** Card Control Registers : Debug 1 register */
#define DEBUG_1_REG                  	0x71
/** Card Control Registers : SD test BUS 1 */
#define SD_TESTBUS1                  	(0x1U)
/** Card Control Registers : Debug 2 register */
#define DEBUG_2_REG                  	0x72
/** Card Control Registers : SD test BUS 2 */
#define SD_TESTBUS2                  	(0x1U)
/** Card Control Registers : Debug 3 register */
#define DEBUG_3_REG                  	0x73
/** Card Control Registers : SD test BUS 3 */
#define SD_TESTBUS3                  	(0x1U)
/** Card Control Registers : Card OCR 0 register */
#define CARD_OCR_0_REG               	0x68
/** Card Control Registers : Card OCR 1 register */
#define CARD_OCR_1_REG               	0x69
/** Card Control Registers : Card OCR 3 register */
#define CARD_OCR_3_REG               	0x6A
/** Card Control Registers : Card config register */
#define CARD_CONFIG_REG              	0x6B
/** Card Control Registers : Miscellaneous Configuration Register */
#define CARD_MISC_CFG_REG              	0x6C
/** Card Control Registers : Card revision register */
#define CARD_REVISION_REG            	0x5c
/** Card Control Registers : Command 53 finish G BUS */
#define CMD53_FINISH_GBUS            	(0x1U << 1)
/** Card Control Registers : SD negative edge */
#define SD_NEG_EDGE                  	(0x1U << 0)

/* Special registers in function 0 of the SDxx card */
/** Special register in function 0 of the SDxxx card : Scratch 0 */
#define	SCRATCH_0_REG			0x80fe
/** Special register in function 0 of the SDxxx card : Scratch 1 */
#define	SCRATCH_1_REG			0x80ff
/** Host F1 read base 0 */
#define HOST_F1_RD_BASE_0		0x0040
/** Host F1 read base 1 */
#define HOST_F1_RD_BASE_1		0x0041
/** Host F1 card ready */
#define HOST_F1_CARD_RDY		0x0020

/** Chip Id Register 0 */
#define CARD_CHIP_ID_0_REG		0x801c
/** Chip Id Register 1 */
#define CARD_CHIP_ID_1_REG		0x801d
/** Firmware status 0 register */
#define CARD_FW_STATUS0_REG		0x60
/** Firmware status 1 register */
#define CARD_FW_STATUS1_REG		0x61
/** Rx length register */
#define CARD_RX_LEN_REG			0x62
/** Rx unit register */
#define CARD_RX_UNIT_REG		0x63

/** Event header Len*/
#define MLAN_EVENT_HEADER_LEN           8

#define MAX_BYTE_MODE_SIZE             512

#ifdef SDIO_MULTI_PORT_TX_AGGR

/** SDIO Tx aggregation in progress ? */
#define MP_TX_AGGR_IN_PROGRESS(a) (a->mpa_tx.pkt_cnt>0)

/** SDIO Tx aggregation buffer room for next packet ? */
#define MP_TX_AGGR_BUF_HAS_ROOM(a,mbuf, len) ((a->mpa_tx.buf_len+len)<= a->mpa_tx.buf_size)

/** Copy current packet (SDIO Tx aggregation buffer) to SDIO buffer */
#define MP_TX_AGGR_BUF_PUT(a, mbuf, port) do{                   \
    pmadapter->callbacks.moal_memmove(a->pmoal_handle, &a->mpa_tx.buf[a->mpa_tx.buf_len],mbuf->pbuf+mbuf->data_offset,mbuf->data_len);\
    a->mpa_tx.buf_len += mbuf->data_len;                        \
    if(!a->mpa_tx.pkt_cnt){                                     \
        a->mpa_tx.start_port = port;                            \
    }                                                           \
    if(a->mpa_tx.start_port<=port){                             \
        a->mpa_tx.ports |= (1<<(a->mpa_tx.pkt_cnt)); 			\
    }else{                                                      \
          a->mpa_tx.ports |= (1<<(a->mpa_tx.pkt_cnt+1+(MAX_PORT - a->mp_end_port)));           \
    }                                                           \
    a->mpa_tx.pkt_cnt++;                                        \
}while(0);

/** SDIO Tx aggregation limit ? */
#define MP_TX_AGGR_PKT_LIMIT_REACHED(a) (a->mpa_tx.pkt_cnt==a->mpa_tx.pkt_aggr_limit)

/** SDIO Tx aggregation port limit ? */
#define MP_TX_AGGR_PORT_LIMIT_REACHED(a) ((a->curr_wr_port < \
			a->mpa_tx.start_port) && (((MAX_PORT - \
			a->mpa_tx.start_port) + a->curr_wr_port) >= \
				SDIO_MP_AGGR_DEF_PKT_LIMIT))

/** Reset SDIO Tx aggregation buffer parameters */
#define MP_TX_AGGR_BUF_RESET(a) do{         \
   a->mpa_tx.pkt_cnt = 0;                   \
   a->mpa_tx.buf_len = 0;                   \
   a->mpa_tx.ports = 0;                     \
   a->mpa_tx.start_port = 0;                \
} while(0);

#endif /* SDIO_MULTI_PORT_TX_AGGR */

#ifdef SDIO_MULTI_PORT_RX_AGGR

/** SDIO Rx aggregation limit ? */
#define MP_RX_AGGR_PKT_LIMIT_REACHED(a) (a->mpa_rx.pkt_cnt== a->mpa_rx.pkt_aggr_limit)

/** SDIO Tx aggregation port limit ? */
#define MP_RX_AGGR_PORT_LIMIT_REACHED(a) ((a->curr_rd_port < \
			a->mpa_rx.start_port) && (((MAX_PORT - \
			a->mpa_rx.start_port) + a->curr_rd_port) >= \
			SDIO_MP_AGGR_DEF_PKT_LIMIT))

/** SDIO Rx aggregation in progress ? */
#define MP_RX_AGGR_IN_PROGRESS(a) (a->mpa_rx.pkt_cnt>0)

/** SDIO Rx aggregation buffer room for next packet ? */
#define MP_RX_AGGR_BUF_HAS_ROOM(a,rx_len)   ((a->mpa_rx.buf_len+rx_len)<=a->mpa_rx.buf_size)

/** Prepare to copy current packet from card to SDIO Rx aggregation buffer */
#define MP_RX_AGGR_SETUP(a, mbuf, port, rx_len) do{    \
    a->mpa_rx.buf_len += rx_len;                       \
    if(!a->mpa_rx.pkt_cnt){                            \
        a->mpa_rx.start_port = port;                   \
    }                                                  \
    if(a->mpa_rx.start_port<=port){                    \
        a->mpa_rx.ports |= (1<<(a->mpa_rx.pkt_cnt));   \
    }else{                                             \
        a->mpa_rx.ports |= (1<<(a->mpa_rx.pkt_cnt+1)); \
    }                                                  \
    a->mpa_rx.mbuf_arr[a->mpa_rx.pkt_cnt] = mbuf;      \
    a->mpa_rx.len_arr[a->mpa_rx.pkt_cnt] = rx_len;     \
    a->mpa_rx.pkt_cnt++;                               \
}while(0);

/** Reset SDIO Rx aggregation buffer parameters */
#define MP_RX_AGGR_BUF_RESET(a) do{         \
   a->mpa_rx.pkt_cnt = 0;                   \
   a->mpa_rx.buf_len = 0;                   \
   a->mpa_rx.ports = 0;                     \
   a->mpa_rx.start_port = 0;                \
} while(0);

#endif /* SDIO_MULTI_PORT_RX_AGGR */

/** Disable host interrupt */
mlan_status wlan_disable_host_int(pmlan_adapter pmadapter);
/** Enable host interrupt */
mlan_status wlan_enable_host_int(pmlan_adapter pmadapter);
/** Probe and initialization function */
mlan_status wlan_sdio_probe(pmlan_adapter pmadapter);
/** multi interface download check */
mlan_status wlan_check_winner_status(mlan_adapter * pmadapter, t_u32 * val);
/** Firmware status check */
mlan_status wlan_check_fw_status(mlan_adapter * pmadapter, t_u32 pollnum);
/** Read interrupt status */
t_void wlan_interrupt(pmlan_adapter pmadapter);
/** Process Interrupt Status */
mlan_status wlan_process_int_status(mlan_adapter * pmadapter);
/** Transfer data to card */
#ifdef SDIO_MULTI_PORT_TX_AGGR
mlan_status wlan_host_to_card_mp_aggr(mlan_adapter * pmadapter,
                                      mlan_buffer * mbuf, t_u8 port,
                                      t_u32 next_pkt_len);
#endif
mlan_status wlan_sdio_host_to_card(mlan_adapter * pmadapter, t_u8 type,
                                   mlan_buffer * mbuf,
                                   mlan_tx_param * tx_param);
#endif /* _MLAN_SDIO_H */
