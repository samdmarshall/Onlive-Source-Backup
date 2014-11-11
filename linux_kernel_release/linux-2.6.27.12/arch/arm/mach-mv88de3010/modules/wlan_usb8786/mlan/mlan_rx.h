/** @file mlan_rx.h
 *
 *  @brief This file contains data packet reception defines.
 *
 *  Copyright (C) 2008-2009, Marvell International Ltd. 
 *  All Rights Reserved
 */

/******************************************************
Change log:
    10/27/2008: initial version
******************************************************/

#ifndef _MLAN_RX_H_
#define _MLAN_RX_H_

/** Ethernet header */
typedef struct
{
    /** Ethernet header destination address */
    t_u8 dest_addr[MLAN_MAC_ADDR_LENGTH];
    /** Ethernet header source address */
    t_u8 src_addr[MLAN_MAC_ADDR_LENGTH];
    /** Ethernet header length */
    t_u16 h803_len;

} Eth803Hdr_t;

/** RFC 1042 header */
typedef struct
{
    /** LLC DSAP */
    t_u8 llc_dsap;
    /** LLC SSAP */
    t_u8 llc_ssap;
    /** LLC CTRL */
    t_u8 llc_ctrl;
    /** SNAP OUI */
    t_u8 snap_oui[3];
    /** SNAP type */
    t_u16 snap_type;

} Rfc1042Hdr_t;

/** Rx packet header */
typedef struct
{
    /** Etherner header */
    Eth803Hdr_t eth803_hdr;
    /** RFC 1042 header */
    Rfc1042Hdr_t rfc1042_hdr;

} RxPacketHdr_t;

/** RxPD Descriptor */
typedef struct _RxPD
{
    /** BSS type */
    t_u8 bss_type;
    /** BSS number */
    t_u8 bss_num;
    /** Rx Packet Length */
    t_u16 rx_pkt_length;
    /** Rx Pkt offset */
    t_u16 rx_pkt_offset;
    /** Rx packet type */
    t_u16 rx_pkt_type;
    /** Sequence number */
    t_u16 seq_num;
    /** Packet Priority */
    t_u8 priority;
    /** Rx Packet Rate */
    t_u8 rx_rate;
    /** SNR */
    t_s8 snr;
    /** Noise Floor */
    t_s8 nf;
    /** Ht Info [Bit 0] RxRate format: LG=0, HT=1
     * [Bit 1]  HT Bandwidth: BW20 = 0, BW40 = 1
     * [Bit 2]  HT Guard Interval: LGI = 0, SGI = 1 */
    t_u8 ht_info;
} RxPD, *PRxPD;

#endif /* !_MLAN_RX_H_ */
