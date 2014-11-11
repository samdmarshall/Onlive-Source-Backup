/** @file mlan_tx.h
 *
 *  @brief This file contains data packet transmission defines.
 *
 *  Copyright (C) 2008-2009, Marvell International Ltd. 
 *  All Rights Reserved
 */

/******************************************************
Change log:
    10/24/2008: initial version
******************************************************/

#ifndef _MLAN_TX_H_
#define _MLAN_TX_H_

/** Interface header length */
#define INTF_HEADER_LEN      0

/** Bit mask for TxPD status field for null packet */
#define MRVDRV_TxPD_POWER_MGMT_NULL_PACKET 0x01
/** Bit mask for TxPD status field for last packet */
#define MRVDRV_TxPD_POWER_MGMT_LAST_PACKET 0x08

/** TxPD descriptor */
typedef struct _TxPD
{
    /** BSS type */
    t_u8 bss_type;
    /** BSS number */
    t_u8 bss_num;
    /** Tx packet length */
    t_u16 tx_pkt_length;
    /** Tx packet offset */
    t_u16 tx_pkt_offset;
    /** Tx packet type */
    t_u16 tx_pkt_type;
    /** Tx Control */
    t_u32 tx_control;
    /** Pkt Priority */
    t_u8 priority;
    /** Transmit Pkt Flags*/
    t_u8 flags;
    /** Amount of time the packet has been queued in the driver (units = 2ms)*/
    t_u8 pkt_delay_2ms;
    /** Reserved */
    t_u8 reserved1;
} TxPD, *PTxPD;

/** Transmit a data packet */
mlan_status wlan_tx_packet(pmlan_adapter pmadapter, pmlan_buffer pmbuf);
/** Transmit a null data packet */
mlan_status wlan_send_null_packet(pmlan_private priv, t_u8 flags);

#endif /* !_MLAN_TX_H_ */
