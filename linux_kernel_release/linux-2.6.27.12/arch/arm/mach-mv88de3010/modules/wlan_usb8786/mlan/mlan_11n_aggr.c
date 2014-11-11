/** @file mlan_11n_aggr.c
 *
 *  @brief This file contains functions for 11n Aggregation.
 *
 *  Copyright (C) 2008-2009, Marvell International Ltd. 
 *  All Rights Reserved
 */

/********************************************************
Change log:
    11/10/2008: initial version
********************************************************/

#include "mlan.h"
#include "mlan_11d.h"
#include "mlan_join.h"
#include "mlan_scan.h"
#include "mlan_util.h"
#include "mlan_fw.h"
#include "mlan_main.h"
#include "mlan_tx.h"
#include "mlan_rx.h"
#include "mlan_wmm.h"
#include "mlan_11n.h"
#include "mlan_11n_aggr.h"

/********************************************************
    Local Variables
********************************************************/

/********************************************************
    Global Variables
********************************************************/

/********************************************************
    Local Functions
********************************************************/

/**
 *  @brief Aggregate individual packets into one AMSDU packet
 *
 *  @param pmadapter A pointer to mlan_adapter structure
 *  @param amsdu_buf A pointer to packet buffer
 *  @param data      A pointer to aggregated data packet being formed
 *  @param pkt_len   Length of current packet to aggregate
 *  @param pad       Pad
 *
 *  @return         Final packet size
 */
static int
wlan_11n_form_amsdu_pkt(pmlan_adapter pmadapter, t_u8 * amsdu_buf, t_u8 * data,
                        int pkt_len, int *pad)
{
    int dt_offset, amsdu_buf_offset;
    Rfc1042Hdr_t snap = {
        .llc_dsap = 0xaa,
        .llc_ssap = 0xaa,
        .llc_ctrl = 0x03,
        .snap_oui[0] = 0x00,
        .snap_oui[1] = 0x00,
        .snap_oui[2] = 0x00,
        .snap_type = 0x0000     /* 
                                 * This field will be overwritten
                                 * later with ethertype
                                 */
    };

    ENTER();

    memcpy(amsdu_buf, data, (MLAN_MAC_ADDR_LENGTH) * 2);
    dt_offset = amsdu_buf_offset = (MLAN_MAC_ADDR_LENGTH) * 2;

    snap.snap_type = *(t_u16 *) (data + dt_offset);
    dt_offset += sizeof(t_u16);
    *(t_u16 *) (amsdu_buf + amsdu_buf_offset) = mlan_htons(pkt_len +
                                                           LLC_SNAP_LEN -
                                                           ((2 *
                                                             MLAN_MAC_ADDR_LENGTH)
                                                            + sizeof(t_u16)));
    amsdu_buf_offset += sizeof(t_u16);
    memcpy(amsdu_buf + amsdu_buf_offset, &snap, LLC_SNAP_LEN);
    amsdu_buf_offset += LLC_SNAP_LEN;

    memcpy(amsdu_buf + amsdu_buf_offset, data + dt_offset, pkt_len - dt_offset);
    *pad = (((pkt_len + LLC_SNAP_LEN) & 3)) ?
        (4 - (((pkt_len + LLC_SNAP_LEN)) & 3)) : 0;

    LEAVE();
    return (pkt_len + LLC_SNAP_LEN + *pad);
}

/**
 *  @brief Add TxPD to AMSDU header
 *
 *  @param priv     A pointer to mlan_private structure
 *  @param mbuf		Pointer to buffer where the TxPD will be formed 
 *
 *  @return		N/A
 */
static void
wlan_11n_form_amsdu_txpd(mlan_private * priv, mlan_buffer * mbuf)
{
    TxPD *ptx_pd;
    mlan_adapter *pmadapter = priv->adapter;

    ENTER();

    ptx_pd = (TxPD *) mbuf->pbuf;
    memset(ptx_pd, 0, sizeof(TxPD));

    /* 
     * Original priority has been overwritten 
     */
    ptx_pd->priority = mbuf->priority;
    ptx_pd->pkt_delay_2ms = wlan_wmm_compute_driver_packet_delay(priv, mbuf);
    ptx_pd->bss_num = mbuf->bss_num;

    if (pmadapter->ps_state != PS_STATE_FULL_POWER) {
        if (MTRUE == wlan_check_last_packet_indication(priv)) {
            pmadapter->tx_lock_flag = MTRUE;
            ptx_pd->flags = MRVDRV_TxPD_POWER_MGMT_LAST_PACKET;
        }
    }

    /* Always zero as the data is followed by TxPD */
    ptx_pd->tx_pkt_offset = sizeof(TxPD);
    ptx_pd->tx_pkt_type = PKT_TYPE_AMSDU;

    if (ptx_pd->tx_control == 0)
        /* TxCtrl set by user or default */
        ptx_pd->tx_control = priv->pkt_tx_ctrl;

    endian_convert_TxPD(ptx_pd);

    LEAVE();
}

/**
 *  @brief Update the TxPktLength field in TxPD after the complete AMSDU
 *  packet is formed
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param mbuf     	TxPD buffer
 *
 *  @return		N/A
 */
static inline void
wlan_11n_update_pktlen_amsdu_txpd(pmlan_adapter pmadapter, pmlan_buffer mbuf)
{
    TxPD *ptx_pd;

    ENTER();

    ptx_pd = (TxPD *) mbuf->pbuf;
    ptx_pd->tx_pkt_length = wlan_cpu_to_le16(mbuf->data_len - sizeof(TxPD));

    LEAVE();
}

/**
 *  @brief Get number of aggregated packets
 *
 *  @param data			A pointer to packet data
 *  @param total_pkt_len	Total packet length
 *
 *  @return			Number of packets
 */
static int
wlan_11n_get_num_aggrpkts(t_u8 * data, int total_pkt_len)
{
    int pkt_count = 0, pkt_len, pad;

    while (total_pkt_len > 0) {
        /* Length will be in network format, change it to host */
        pkt_len = mlan_ntohs((*(t_u16 *) (data + (2 * MLAN_MAC_ADDR_LENGTH))));
        pad = (((pkt_len + sizeof(Eth803Hdr_t)) & 3)) ?
            (4 - ((pkt_len + sizeof(Eth803Hdr_t)) & 3)) : 0;
        data += pkt_len + pad + sizeof(Eth803Hdr_t);
        total_pkt_len -= pkt_len + pad + sizeof(Eth803Hdr_t);
        ++pkt_count;
    }

    return pkt_count;
}

/********************************************************
    Global Functions
********************************************************/

/**
 *  @brief Deaggregate the received AMSDU packet 
 *
 *  @param priv		A pointer to mlan_private structure
 *  @param pmbuf	A pointer to aggregated data packet
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
mlan_status
wlan_11n_deaggregate_pkt(mlan_private * priv, pmlan_buffer pmbuf)
{
    t_u16 pkt_len;
    int total_pkt_len;
    t_u8 *data;
    int pad;
    mlan_status ret = MLAN_STATUS_FAILURE;
    RxPacketHdr_t *prx_pkt;
    mlan_buffer *daggr_mbuf;
    mlan_adapter *pmadapter = priv->adapter;
    t_u8 rfc1042_eth_hdr[MLAN_MAC_ADDR_LENGTH] = { 0xaa, 0xaa, 0x03,
        0x00, 0x00, 0x00
    };

    ENTER();

    data = (t_u8 *) (pmbuf->pbuf + pmbuf->data_offset);
    total_pkt_len = pmbuf->data_len;

    /* Sanity test */
    if (total_pkt_len > MLAN_RX_DATA_BUF_SIZE) {
        PRINTM(ERROR, "Total packet length greater than tx buffer"
               " size %d\n", total_pkt_len);
        goto done;
    }

    pmbuf->use_count = wlan_11n_get_num_aggrpkts(data, total_pkt_len);

    while (total_pkt_len > 0) {
        prx_pkt = (RxPacketHdr_t *) data;
        /* Length will be in network format, change it to host */
        pkt_len = mlan_ntohs((*(t_u16 *) (data + (2 * MLAN_MAC_ADDR_LENGTH))));
        if (pkt_len > total_pkt_len) {
            PRINTM(ERROR, "Error in pkt_len %d %d\n", total_pkt_len, pkt_len);
            break;
        }

        pad = (((pkt_len + sizeof(Eth803Hdr_t)) & 3)) ?
            (4 - ((pkt_len + sizeof(Eth803Hdr_t)) & 3)) : 0;

        total_pkt_len -= pkt_len + pad + sizeof(Eth803Hdr_t);

        if (memcmp(&prx_pkt->rfc1042_hdr,
                   rfc1042_eth_hdr, sizeof(rfc1042_eth_hdr)) == 0) {
            memmove(data + LLC_SNAP_LEN, data, (2 * MLAN_MAC_ADDR_LENGTH));
            data += LLC_SNAP_LEN;
            pkt_len += sizeof(Eth803Hdr_t) - LLC_SNAP_LEN;
        } else {
            *(t_u16 *) (data + (2 * MLAN_MAC_ADDR_LENGTH))
                = (t_u16) 0;
            pkt_len += sizeof(Eth803Hdr_t);
        }

        if ((pmadapter->callbacks.moal_malloc(sizeof(mlan_buffer),
                                              (t_u8 **) & daggr_mbuf))) {
            PRINTM(ERROR, "Error allocating daggr mlan_buffer\n");
            return MLAN_STATUS_FAILURE;
        }

        memcpy(daggr_mbuf, pmbuf, sizeof(mlan_buffer));

        daggr_mbuf->data_offset = 0;
        daggr_mbuf->data_len = pkt_len;
        daggr_mbuf->pbuf = data;
        daggr_mbuf->pdesc = MNULL;
        daggr_mbuf->pparent = pmbuf;

        ret =
            pmadapter->callbacks.moal_recv_packet(pmadapter->pmoal_handle,
                                                  daggr_mbuf);

        switch (ret) {
        case MLAN_STATUS_PENDING:
            break;
        case MLAN_STATUS_FAILURE:
            PRINTM(ERROR, "Deaggr, send to moal failed\n");
        case MLAN_STATUS_SUCCESS:
            mlan_recv_packet_complete(pmadapter, daggr_mbuf, ret);
            break;
        default:
            break;
        }

        data += pkt_len + pad;
    }

  done:
    LEAVE();
    return ret;
}

/**
 *  @brief Aggregate multiple packets into one single AMSDU packet
 *
 *  @param priv 	A pointer to mlan_private structure
 *  @param pra_list	Pointer to the RA List table containing the pointers
 *  			    to packets.
 *  @param headroom	Any interface specific headroom that may be need. TxPD 
 *  				will be formed leaving this headroom.
 *  @param ptrindex	Pointer index
 *
 *  @return		Final packet size
 */
int
wlan_11n_aggregate_pkt(mlan_private * priv, raListTbl * pra_list,
                       int headroom, int ptrindex)
{
    int pkt_size = 0;
    pmlan_adapter pmadapter = priv->adapter;
    mlan_buffer *pmbuf_aggr, *pmbuf_src;
    t_u8 *data;
    int pad = 0;
    mlan_status ret = MLAN_STATUS_SUCCESS;
    t_u32 sec, usec;

    ENTER();

    PRINTM(DAT_D, "Handling Aggr packet\n");

    if ((pmbuf_src = (pmlan_buffer) util_peek_list(&pra_list->buf_head,
                                                   pmadapter->callbacks.
                                                   moal_spin_lock,
                                                   pmadapter->callbacks.
                                                   moal_spin_unlock))) {

        if (!
            (pmbuf_aggr =
             wlan_alloc_mlan_buffer(&pmadapter->callbacks,
                                    (pmadapter->tx_buf_size + headroom)))) {
            PRINTM(ERROR, "Error allocating mlan_buffer\n");
            return MLAN_STATUS_FAILURE;
        }

        data = pmbuf_aggr->pbuf + headroom;
        memcpy(pmbuf_aggr, pmbuf_src, sizeof(mlan_buffer));

        pmbuf_aggr->pbuf = data;
        pmbuf_aggr->pdesc = MNULL;
        pmbuf_aggr->data_offset = 0;

        /* Form AMSDU */
        wlan_11n_form_amsdu_txpd(priv, pmbuf_aggr);
        pkt_size = sizeof(TxPD);
    } else {
        goto leave;
    }

    while (pmbuf_src && ((pkt_size + (pmbuf_src->data_len + LLC_SNAP_LEN))
                         <= pmadapter->tx_buf_size)) {

        pmbuf_src = (pmlan_buffer)
            util_dequeue_list(&pra_list->buf_head,
                              pmadapter->callbacks.moal_spin_lock,
                              pmadapter->callbacks.moal_spin_unlock);
        pmadapter->callbacks.moal_spin_unlock(priv->wmm.ra_list_spinlock);
        pkt_size += wlan_11n_form_amsdu_pkt(pmadapter,
                                            (data + pkt_size),
                                            pmbuf_src->pbuf +
                                            pmbuf_src->data_offset,
                                            pmbuf_src->data_len, &pad);

        DBG_HEXDUMP(DAT_D, "pmbuf_src", pmbuf_src, sizeof(mlan_buffer));
        pmadapter->callbacks.moal_send_packet_complete(pmadapter->pmoal_handle,
                                                       pmbuf_src,
                                                       MLAN_STATUS_SUCCESS);

        pmadapter->callbacks.moal_spin_lock(priv->wmm.ra_list_spinlock);

        if (!wlan_is_ralist_valid(priv, pra_list, ptrindex)) {
            pmadapter->callbacks.moal_spin_unlock(priv->wmm.ra_list_spinlock);
            LEAVE();
            return MLAN_STATUS_FAILURE;
        }

        pmbuf_src = (pmlan_buffer) util_peek_list(&pra_list->buf_head,
                                                  pmadapter->callbacks.
                                                  moal_spin_lock,
                                                  pmadapter->callbacks.
                                                  moal_spin_unlock);
    }

    pmadapter->callbacks.moal_spin_unlock(priv->wmm.ra_list_spinlock);

    /* Last AMSDU packet does not need padding */
    pkt_size -= pad;
    pmbuf_aggr->data_len = pkt_size;
    wlan_11n_update_pktlen_amsdu_txpd(pmadapter, pmbuf_aggr);
    pmadapter->data_sent = MTRUE;
    ret = pmadapter->callbacks.moal_write_data_async(pmadapter->pmoal_handle,
                                                     pmbuf_aggr,
                                                     MLAN_USB_EP_DATA);
    switch (ret) {
    case MLAN_STATUS_RESOURCE:
        pmadapter->callbacks.moal_spin_lock(priv->wmm.ra_list_spinlock);

        if (!wlan_is_ralist_valid(priv, pra_list, ptrindex)) {
            pmadapter->callbacks.moal_spin_unlock(priv->wmm.ra_list_spinlock);
            LEAVE();
            return MLAN_STATUS_FAILURE;
        }
        util_enqueue_list_head(&pra_list->buf_head,
                               (pmlan_linked_list) pmbuf_aggr,
                               pmadapter->callbacks.moal_spin_lock,
                               pmadapter->callbacks.moal_spin_unlock);
        pmbuf_aggr->flags = MLAN_BUF_FLAG_REQUEUED_PKT;
        pmadapter->callbacks.moal_spin_unlock(priv->wmm.ra_list_spinlock);
        PRINTM(DATA, "MLAN_STATUS_RESOURCE is returned\n");
        break;
    case MLAN_STATUS_FAILURE:
        pmadapter->data_sent = MFALSE;
        PRINTM(ERROR, "Error: moal_write_data_async failed: 0x%X\n", ret);
        pmadapter->dbg.num_tx_host_to_card_failure++;
        mlan_write_data_async_complete(pmadapter, pmbuf_aggr, MLAN_USB_EP_DATA,
                                       ret);
        goto leave;
    case MLAN_STATUS_PENDING:
        pmadapter->data_sent = MFALSE;
    default:
        break;
    }

    pmadapter->callbacks.moal_get_system_time(&sec, &usec);
    PRINTM(DATA, "%lu.%lu : Data => FW\n", sec, usec);

  leave:
    LEAVE();
    return pkt_size;
}
