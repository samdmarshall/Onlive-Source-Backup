/** @file mlan_sta_rx.c
 *
 *  @brief This file contains the handling of RX in MLAN
 *  module.
 * 
 *  Copyright (C) 2008-2009, Marvell International Ltd. 
 *  All Rights Reserved
 */

/********************************************************
Change log:
    10/27/2008: initial version
********************************************************/

#include "mlan.h"
#include "mlan_11d.h"
#include "mlan_join.h"
#include "mlan_scan.h"
#include "mlan_util.h"
#include "mlan_fw.h"
#include "mlan_main.h"
#include "mlan_rx.h"
#include "mlan_11n_aggr.h"
#include "mlan_11n_rxreorder.h"

/********************************************************
		Local Variables
********************************************************/

/** Ethernet II header */
typedef struct
{
    /** Ethernet II header destination address */
    t_u8 dest_addr[MLAN_MAC_ADDR_LENGTH];
    /** Ethernet II header source address */
    t_u8 src_addr[MLAN_MAC_ADDR_LENGTH];
    /** Ethernet II header length */
    t_u16 ethertype;

} EthII_Hdr_t;

/********************************************************
		Global Variables
********************************************************/

/********************************************************
		Local Functions
********************************************************/

/********************************************************
		Global functions
********************************************************/
/**
 *  @brief This function processes received packet and forwards it
 *  		to kernel/upper layer
 *  
 *  @param pmadapter A pointer to mlan_adapter
 *  @param pmbuf     A pointer to mlan_buffer which includes the received packet
 *
 *  @return 	   MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_process_rx_packet(pmlan_adapter pmadapter, pmlan_buffer pmbuf)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    pmlan_private priv = pmadapter->priv[pmbuf->bss_num];
    RxPacketHdr_t *prx_pkt;
    RxPD *prx_pd;
    int hdr_chop;
    EthII_Hdr_t *peth_hdr;
    t_u8 rfc1042_eth_hdr[MLAN_MAC_ADDR_LENGTH] =
        { 0xaa, 0xaa, 0x03, 0x00, 0x00, 0x00 };

    ENTER();
    prx_pd = (RxPD *) (pmbuf->pbuf + pmbuf->data_offset);

    prx_pkt = (RxPacketHdr_t *) ((t_u8 *) prx_pd + prx_pd->rx_pkt_offset);

    DBG_HEXDUMP(MDAT_D, "Rx", pmbuf->pbuf + pmbuf->data_offset,
                MIN(pmbuf->data_len, MAX_DATA_DUMP_LEN));

/** Rx packet type debugging */
#define RX_PKT_TYPE_DEBUG  0xEF
/** Small debug type */
#define DBG_TYPE_SMALL  2
/** Size of debugging structure */
#define SIZE_OF_DBG_STRUCT 4
    if (prx_pd->rx_pkt_type == RX_PKT_TYPE_DEBUG) {
        t_u8 dbgType;
        dbgType = *(t_u8 *) & prx_pkt->eth803_hdr;
        if (dbgType == DBG_TYPE_SMALL) {
            PRINTM(MFW_D, "\n");
            DBG_HEXDUMP(MFW_D, "FWDBG",
                        (t_s8 *) ((t_u8 *) & prx_pkt->eth803_hdr +
                                  SIZE_OF_DBG_STRUCT), prx_pd->rx_pkt_length);
            PRINTM(MFW_D, "FWDBG::\n");
        }
        goto done;
    }

    PRINTM(MINFO, "RX Data: data_len - prx_pd->rx_pkt_offset = %d - %d = %d\n",
           pmbuf->data_len, prx_pd->rx_pkt_offset,
           pmbuf->data_len - prx_pd->rx_pkt_offset);

    HEXDUMP("RX Data: Dest", prx_pkt->eth803_hdr.dest_addr,
            sizeof(prx_pkt->eth803_hdr.dest_addr));
    HEXDUMP("RX Data: Src", prx_pkt->eth803_hdr.src_addr,
            sizeof(prx_pkt->eth803_hdr.src_addr));

    if (!memcmp(&prx_pkt->rfc1042_hdr,
                rfc1042_eth_hdr, sizeof(rfc1042_eth_hdr))) {
        /* 
         *  Replace the 803 header and rfc1042 header (llc/snap) with an 
         *    EthernetII header, keep the src/dst and snap_type (ethertype).
         *  The firmware only passes up SNAP frames converting
         *    all RX Data from 802.11 to 802.2/LLC/SNAP frames.
         *  To create the Ethernet II, just move the src, dst address right
         *    before the snap_type.
         */
        peth_hdr = (EthII_Hdr_t *)
            ((t_u8 *) & prx_pkt->eth803_hdr
             + sizeof(prx_pkt->eth803_hdr) + sizeof(prx_pkt->rfc1042_hdr)
             - sizeof(prx_pkt->eth803_hdr.dest_addr)
             - sizeof(prx_pkt->eth803_hdr.src_addr)
             - sizeof(prx_pkt->rfc1042_hdr.snap_type));

        memcpy(peth_hdr->src_addr, prx_pkt->eth803_hdr.src_addr,
               sizeof(peth_hdr->src_addr));
        memcpy(peth_hdr->dest_addr, prx_pkt->eth803_hdr.dest_addr,
               sizeof(peth_hdr->dest_addr));

        /* Chop off the RxPD + the excess memory from the 802.2/llc/snap header 
           that was removed. */
        hdr_chop = (t_u8 *) peth_hdr - (t_u8 *) prx_pd;
    } else {
        HEXDUMP("RX Data: LLC/SNAP",
                (t_u8 *) & prx_pkt->rfc1042_hdr, sizeof(prx_pkt->rfc1042_hdr));

        /* Chop off the RxPD */
        hdr_chop = (t_u8 *) & prx_pkt->eth803_hdr - (t_u8 *) prx_pd;
    }

    /* Chop off the leading header bytes so the it points to the start of
       either the reconstructed EthII frame or the 802.2/llc/snap frame */
    pmbuf->data_len -= hdr_chop;
    pmbuf->data_offset += hdr_chop;
    pmbuf->pparent = MNULL;

    priv->rxpd_rate = prx_pd->rx_rate;

    priv->rxpd_htinfo = prx_pd->ht_info;

    ret = pmadapter->callbacks.moal_recv_packet(pmadapter->pmoal_handle, pmbuf);
    if (ret == MLAN_STATUS_FAILURE) {
        PRINTM(MERROR, "RX Error: moal_recv_packet" " returns failure\n");
    }

    pmadapter->callbacks.moal_get_system_time(&pmbuf->out_ts_sec,
                                              &pmbuf->out_ts_usec);
    PRINTM(MDATA, "%lu.%lu : Data => kernel\n", pmbuf->out_ts_sec,
           pmbuf->out_ts_usec);

    if (ret != MLAN_STATUS_PENDING) {
        pmadapter->callbacks.moal_recv_complete(pmadapter->pmoal_handle, pmbuf,
                                                0, ret);
    }

  done:
    LEAVE();

    return ret;
}

/**
 *   @brief This function processes the received buffer
 *     
 *   @param adapter A pointer to mlan_adapter
 *   @param pmbuf     A pointer to the received buffer
 *
 *   @return        MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
mlan_process_sta_rx_packet(IN t_void * adapter, IN pmlan_buffer pmbuf)
{
    pmlan_adapter pmadapter = (pmlan_adapter) adapter;
    mlan_status ret = MLAN_STATUS_SUCCESS;
    RxPD *prx_pd;
    RxPacketHdr_t *prx_pkt;
    pmlan_private priv = pmadapter->priv[pmbuf->bss_num];
    t_u8 ta[MLAN_MAC_ADDR_LENGTH];
    ENTER();

    prx_pd = (RxPD *) (pmbuf->pbuf + pmbuf->data_offset);
    /* Endian conversion */
    endian_convert_RxPD(prx_pd);
    prx_pkt = (RxPacketHdr_t *) ((t_u8 *) prx_pd + prx_pd->rx_pkt_offset);

    if ((prx_pd->rx_pkt_offset + prx_pd->rx_pkt_length) > pmbuf->data_len) {
        PRINTM(MERROR, "Wrong rx packet: len=%d,rx_pkt_offset=%d,"
               " rx_pkt_length=%d\n", pmbuf->data_len,
               prx_pd->rx_pkt_offset, prx_pd->rx_pkt_length);
        ret = MLAN_STATUS_FAILURE;
        pmadapter->callbacks.moal_recv_complete(pmadapter->pmoal_handle,
                                                pmbuf, 0, ret);

        goto done;
    }
    if (prx_pd->rx_pkt_type == PKT_TYPE_AMSDU) {
        pmbuf->data_len = prx_pd->rx_pkt_length;
        pmbuf->data_offset += prx_pd->rx_pkt_offset;
        wlan_11n_deaggregate_pkt(priv, pmbuf);
        goto done;
    }
    /* 
     * If the packet is not an unicast packet then send the packet
     * directly to os. Don't pass thru rx reordering
     */
    if (!IS_11N_ENABLED(priv) ||
        memcmp(priv->curr_addr, prx_pkt->eth803_hdr.dest_addr,
               MLAN_MAC_ADDR_LENGTH)) {
        wlan_process_rx_packet(pmadapter, pmbuf);
        goto done;
    }

    if (queuing_ra_based(priv)) {
        memcpy(ta, prx_pkt->eth803_hdr.src_addr, MLAN_MAC_ADDR_LENGTH);
    } else {
        memcpy(ta,
               priv->curr_bss_params.bss_descriptor.mac_address,
               MLAN_MAC_ADDR_LENGTH);
    }

    /* Reorder and send to OS */
    if ((prx_pd->rx_pkt_type == PKT_TYPE_BAR)
        || (ret = mlan_11n_rxreorder_pkt(priv, prx_pd->seq_num,
                                         prx_pd->priority, ta,
                                         prx_pd->rx_pkt_type, (void *) pmbuf))
        ) {
        if ((ret =
             pmadapter->callbacks.moal_recv_complete(pmadapter->pmoal_handle,
                                                     pmbuf, 0, ret)))
            PRINTM(MERROR, "RX Error: moal_recv_complete returns" " failure\n");
    }

  done:

    LEAVE();
    return (ret);
}
