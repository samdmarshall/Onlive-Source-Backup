/** @file mlan_uap_txrx.c
 *
 *  @brief This file contains AP mode transmit and receive functions
 * 
 *  Copyright (C) 2009-2010, Marvell International Ltd. 
 *  All Rights Reserved
 */

/********************************************************
Change log:
    02/05/2009: initial version
********************************************************/

#include "mlan.h"
#include "mlan_util.h"
#include "mlan_fw.h"
#ifdef STA_SUPPORT
#include "mlan_join.h"
#endif
#include "mlan_main.h"
#include "mlan_uap.h"
#include "mlan_sdio.h"
#include "mlan_wmm.h"
#include "mlan_11n_aggr.h"
#include "mlan_11n_rxreorder.h"

/********************************************************
    Global Functions
********************************************************/
/** 
 *  @brief This function fill the txpd for tx packet  
 *  
 *  @param priv	   A pointer to mlan_private structure
 *  @param pmbuf   A pointer to the mlan_buffer for process
 *
 *  @return 	   headptr or MNULL
 */
t_void *
mlan_process_uap_txpd(IN t_void * priv, IN pmlan_buffer pmbuf)
{
    pmlan_private pmpriv = (pmlan_private) priv;
    UapTxPD *plocal_tx_pd;
    t_u8 *head_ptr = MNULL;
    ENTER();

    if (!pmbuf->data_len) {
        PRINTM(MERROR, "uAP Tx Error: Invalid packet length: %d\n",
               pmbuf->data_len);
        pmbuf->status_code = MLAN_ERROR_PKT_SIZE_INVALID;
        goto done;
    }
    if (pmbuf->data_offset < (sizeof(UapTxPD) + INTF_HEADER_LEN +
                              HEADER_ALIGNMENT)) {
        PRINTM(MERROR, "not enough space for UapTxPD: len=%d, offset=%d\n",
               pmbuf->data_len, pmbuf->data_offset);
        DBG_HEXDUMP(MDAT_D, "drop pkt", pmbuf->pbuf + pmbuf->data_offset,
                    pmbuf->data_len);
        pmbuf->status_code = MLAN_ERROR_PKT_SIZE_INVALID;
        goto done;
    }

    /* head_ptr should be aligned */
    head_ptr =
        pmbuf->pbuf + pmbuf->data_offset - sizeof(UapTxPD) - INTF_HEADER_LEN;
    head_ptr = (t_u8 *) ((t_u32) head_ptr & ~((t_u32) (HEADER_ALIGNMENT - 1)));

    plocal_tx_pd = (UapTxPD *) (head_ptr + INTF_HEADER_LEN);
    memset(pmpriv->adapter, plocal_tx_pd, 0, sizeof(UapTxPD));

    /* Set the BSS number to TxPD */
    plocal_tx_pd->bss_num = GET_BSS_NUM(pmpriv);
    plocal_tx_pd->bss_type = pmpriv->bss_type;

    plocal_tx_pd->tx_pkt_length = (t_u16) pmbuf->data_len;

    plocal_tx_pd->priority = (t_u8) pmbuf->priority;
    plocal_tx_pd->pkt_delay_2ms =
        wlan_wmm_compute_driver_packet_delay(pmpriv, pmbuf);

    if (plocal_tx_pd->priority < NELEMENTS(pmpriv->wmm.user_pri_pkt_tx_ctrl))
        /* 
         * Set the priority specific tx_control field, setting of 0 will
         *   cause the default value to be used later in this function
         */
        plocal_tx_pd->tx_control
            = pmpriv->wmm.user_pri_pkt_tx_ctrl[plocal_tx_pd->priority];

    /* Offset of actual data */
    plocal_tx_pd->tx_pkt_offset =
        (t_u16) ((t_u32) pmbuf->pbuf + pmbuf->data_offset -
                 (t_u32) plocal_tx_pd);

    uap_endian_convert_TxPD(plocal_tx_pd);

    /* Adjust the data offset and length to include TxPD in pmbuf */
    pmbuf->data_len += pmbuf->data_offset;
    pmbuf->data_offset = head_ptr - pmbuf->pbuf;
    pmbuf->data_len -= pmbuf->data_offset;

  done:
    LEAVE();
    return head_ptr;
}

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
wlan_upload_uap_rx_packet(pmlan_adapter pmadapter, pmlan_buffer pmbuf)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    UapRxPD *prx_pd;
    ENTER();
    prx_pd = (UapRxPD *) (pmbuf->pbuf + pmbuf->data_offset);

    /* Chop off RxPD */
    pmbuf->data_len -= prx_pd->rx_pkt_offset;
    pmbuf->data_offset += prx_pd->rx_pkt_offset;
    pmbuf->pparent = MNULL;

    pmadapter->callbacks.moal_get_system_time(pmadapter->pmoal_handle,
                                              &pmbuf->out_ts_sec,
                                              &pmbuf->out_ts_usec);
    PRINTM(MDATA, "%lu.%lu : Data => kernel\n", pmbuf->out_ts_sec,
           pmbuf->out_ts_usec);

    ret = pmadapter->callbacks.moal_recv_packet(pmadapter->pmoal_handle, pmbuf);
    if (ret == MLAN_STATUS_FAILURE) {
        PRINTM(MERROR, "uAP Rx Error: moal_recv_packet returned error\n");
    }

    if (ret != MLAN_STATUS_PENDING) {
        pmadapter->callbacks.moal_free_mlan_buffer(pmadapter->pmoal_handle,
                                                   pmbuf);
    }
    LEAVE();

    return ret;

}

/**
 *  @brief This function processes received packet and forwards it
 *  		to kernel/upper layer
 *  
 *  @param adapter   A pointer to mlan_adapter
 *  @param pmbuf     A pointer to mlan_buffer which includes the received packet
 *
 *  @return 	   MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
mlan_process_uap_rx_packet(IN t_void * adapter, IN pmlan_buffer pmbuf)
{
    pmlan_adapter pmadapter = (pmlan_adapter) adapter;
    mlan_status ret = MLAN_STATUS_SUCCESS;
    UapRxPD *prx_pd;
    RxPacketHdr_t *prx_pkt;
    pmlan_private priv = pmadapter->priv[pmbuf->bss_num];
    t_u8 ta[MLAN_MAC_ADDR_LENGTH];

    ENTER();

    prx_pd = (UapRxPD *) (pmbuf->pbuf + pmbuf->data_offset);
    /* Endian conversion */
    uap_endian_convert_RxPD(prx_pd);
    prx_pkt = (RxPacketHdr_t *) ((t_u8 *) prx_pd + prx_pd->rx_pkt_offset);

    DBG_HEXDUMP(MDAT_D, "UAP Rx", pmbuf->pbuf + pmbuf->data_offset,
                MIN(pmbuf->data_len, MAX_DATA_DUMP_LEN));

    PRINTM(MINFO, "RX Data: data_len - prx_pd->rx_pkt_offset = %d - %d = %d\n",
           pmbuf->data_len, prx_pd->rx_pkt_offset,
           pmbuf->data_len - prx_pd->rx_pkt_offset);

    if ((prx_pd->rx_pkt_offset + prx_pd->rx_pkt_length) >
        (t_u16) pmbuf->data_len) {
        PRINTM(MERROR,
               "Wrong rx packet: len=%d,rx_pkt_offset=%d,"
               " rx_pkt_length=%d\n", pmbuf->data_len, prx_pd->rx_pkt_offset,
               prx_pd->rx_pkt_length);
        ret = MLAN_STATUS_FAILURE;
        pmadapter->callbacks.moal_free_mlan_buffer(pmadapter->pmoal_handle,
                                                   pmbuf);
        goto done;
    }
    pmbuf->data_len = prx_pd->rx_pkt_offset + prx_pd->rx_pkt_length;

    pmbuf->priority = prx_pd->priority;
    if (prx_pd->rx_pkt_type == PKT_TYPE_AMSDU) {
        pmbuf->data_len = prx_pd->rx_pkt_length;
        pmbuf->data_offset += prx_pd->rx_pkt_offset;
        wlan_11n_deaggregate_pkt(priv, pmbuf);
        goto done;
    }
    memcpy(pmadapter, ta, prx_pkt->eth803_hdr.src_addr, MLAN_MAC_ADDR_LENGTH);
    /* check if UAP enable 11n */
    if (!priv->is_11n_enabled ||
        !wlan_11n_get_rxreorder_tbl((mlan_private *) priv, prx_pd->priority,
                                    ta)) {
        if (priv->pkt_fwd == MTRUE)
            wlan_process_uap_rx_packet(priv, pmbuf);
        else
            wlan_upload_uap_rx_packet(pmadapter, pmbuf);
        goto done;
    }
    /* Reorder and send to OS */
    if ((ret = mlan_11n_rxreorder_pkt(priv, prx_pd->seq_num,
                                      prx_pd->priority, ta,
                                      (t_u8) prx_pd->rx_pkt_type,
                                      (void *) pmbuf))
        || (prx_pd->rx_pkt_type == PKT_TYPE_BAR)) {
        pmadapter->callbacks.moal_free_mlan_buffer(pmadapter->pmoal_handle,
                                                   pmbuf);
    }
  done:
    LEAVE();
    return ret;
}

/**
 *  @brief This function processes received packet and forwards it
 *  		to kernel/upper layer or send back to firmware 
 *  
 *  @param priv      A pointer to mlan_private
 *  @param pmbuf     A pointer to mlan_buffer which includes the received packet
 *
 *  @return 	   MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_uap_recv_packet(IN mlan_private * priv, IN pmlan_buffer pmbuf)
{
    pmlan_adapter pmadapter = priv->adapter;
    mlan_status ret = MLAN_STATUS_SUCCESS;
    RxPacketHdr_t *prx_pkt;
    sta_node *sta_ptr = MNULL;
    pmlan_buffer newbuf = MNULL;

    ENTER();

    prx_pkt = (RxPacketHdr_t *) ((t_u8 *) pmbuf->pbuf + pmbuf->data_offset);

    DBG_HEXDUMP(MDAT_D, "uap_recv_packet", pmbuf->pbuf + pmbuf->data_offset,
                MIN(pmbuf->data_len, MAX_DATA_DUMP_LEN));

    PRINTM(MDATA, "AMSDU dest %02x:%02x:%02x:%02x:%02x:%02x\n",
           prx_pkt->eth803_hdr.dest_addr[0], prx_pkt->eth803_hdr.dest_addr[1],
           prx_pkt->eth803_hdr.dest_addr[2], prx_pkt->eth803_hdr.dest_addr[3],
           prx_pkt->eth803_hdr.dest_addr[4], prx_pkt->eth803_hdr.dest_addr[5]);

    /* don't do packet forwarding in disconnected state */
    if (priv->media_connected == MFALSE)
        goto upload;
    if (prx_pkt->eth803_hdr.dest_addr[0] & 0x01) {
        /* Multicast pkt */
        if ((newbuf =
             wlan_alloc_mlan_buffer(pmadapter, &pmadapter->callbacks,
                                    MLAN_TX_DATA_BUF_SIZE_2K))) {
            newbuf->bss_num = pmbuf->bss_num;
            newbuf->buf_type = pmbuf->buf_type;
            newbuf->priority = pmbuf->priority;
            newbuf->in_ts_sec = pmbuf->in_ts_sec;
            newbuf->in_ts_usec = pmbuf->in_ts_usec;
            newbuf->data_offset =
                (sizeof(UapTxPD) + INTF_HEADER_LEN + HEADER_ALIGNMENT);
            newbuf->flags = MLAN_BUF_FLAG_BRIDGE_PKT;

            /* copy the data */
            memcpy(pmadapter, (t_u8 *) newbuf->pbuf + newbuf->data_offset,
                   pmbuf->pbuf + pmbuf->data_offset, pmbuf->data_len);
            newbuf->data_len = pmbuf->data_len;
            wlan_wmm_add_buf_txqueue(pmadapter, newbuf);

        }
    } else {
        if ((sta_ptr =
             wlan_get_station_entry(priv, prx_pkt->eth803_hdr.dest_addr))) {
            /* Inter BSS packet */
            if ((newbuf =
                 wlan_alloc_mlan_buffer(pmadapter, &pmadapter->callbacks,
                                        MLAN_TX_DATA_BUF_SIZE_2K))) {
                newbuf->bss_num = pmbuf->bss_num;
                newbuf->buf_type = pmbuf->buf_type;
                newbuf->priority = pmbuf->priority;
                newbuf->in_ts_sec = pmbuf->in_ts_sec;
                newbuf->in_ts_usec = pmbuf->in_ts_usec;
                newbuf->data_offset =
                    (sizeof(UapTxPD) + INTF_HEADER_LEN + HEADER_ALIGNMENT);
                newbuf->flags = MLAN_BUF_FLAG_BRIDGE_PKT;

                /* copy the data */
                memcpy(pmadapter, (t_u8 *) newbuf->pbuf + newbuf->data_offset,
                       pmbuf->pbuf + pmbuf->data_offset, pmbuf->data_len);
                newbuf->data_len = pmbuf->data_len;
                wlan_wmm_add_buf_txqueue(pmadapter, newbuf);

            }
            goto done;
        }
    }
  upload:
    /** send packet to moal */
    ret = pmadapter->callbacks.moal_recv_packet(pmadapter->pmoal_handle, pmbuf);
  done:
    LEAVE();
    return ret;
}

/**
 *  @brief This function processes received packet and forwards it
 *  		to kernel/upper layer or send back to firmware 
 *  
 *  @param priv      A pointer to mlan_private
 *  @param pmbuf     A pointer to mlan_buffer which includes the received packet
 *
 *  @return 	   MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_process_uap_rx_packet(IN mlan_private * priv, IN pmlan_buffer pmbuf)
{
    pmlan_adapter pmadapter = priv->adapter;
    mlan_status ret = MLAN_STATUS_SUCCESS;
    UapRxPD *prx_pd;
    RxPacketHdr_t *prx_pkt;
    sta_node *sta_ptr = MNULL;
    pmlan_buffer newbuf = MNULL;

    ENTER();

    prx_pd = (UapRxPD *) (pmbuf->pbuf + pmbuf->data_offset);
    prx_pkt = (RxPacketHdr_t *) ((t_u8 *) prx_pd + prx_pd->rx_pkt_offset);

    DBG_HEXDUMP(MDAT_D, "uap_rx_packet", pmbuf->pbuf + pmbuf->data_offset,
                MIN(pmbuf->data_len, MAX_DATA_DUMP_LEN));

    PRINTM(MINFO, "RX Data: data_len - prx_pd->rx_pkt_offset = %d - %d = %d\n",
           pmbuf->data_len, prx_pd->rx_pkt_offset,
           pmbuf->data_len - prx_pd->rx_pkt_offset);
    PRINTM(MDATA, "Rx dest %02x:%02x:%02x:%02x:%02x:%02x\n",
           prx_pkt->eth803_hdr.dest_addr[0], prx_pkt->eth803_hdr.dest_addr[1],
           prx_pkt->eth803_hdr.dest_addr[2], prx_pkt->eth803_hdr.dest_addr[3],
           prx_pkt->eth803_hdr.dest_addr[4], prx_pkt->eth803_hdr.dest_addr[5]);

    /* don't do packet forwarding in disconnected state */
    if (priv->media_connected == MFALSE)
        goto upload;
    if (prx_pkt->eth803_hdr.dest_addr[0] & 0x01) {
        /* Multicast pkt */
        if ((newbuf =
             wlan_alloc_mlan_buffer(pmadapter, &pmadapter->callbacks,
                                    MLAN_TX_DATA_BUF_SIZE_2K))) {
            newbuf->bss_num = pmbuf->bss_num;
            newbuf->buf_type = pmbuf->buf_type;
            newbuf->priority = pmbuf->priority;
            newbuf->in_ts_sec = pmbuf->in_ts_sec;
            newbuf->in_ts_usec = pmbuf->in_ts_usec;
            newbuf->data_offset =
                (sizeof(UapTxPD) + INTF_HEADER_LEN + HEADER_ALIGNMENT);
            newbuf->flags = MLAN_BUF_FLAG_BRIDGE_PKT;

            /* copy the data, skip rxpd */
            memcpy(pmadapter, (t_u8 *) newbuf->pbuf + newbuf->data_offset,
                   pmbuf->pbuf + pmbuf->data_offset + prx_pd->rx_pkt_offset,
                   pmbuf->data_len - prx_pd->rx_pkt_offset);
            newbuf->data_len = pmbuf->data_len - prx_pd->rx_pkt_offset;
            wlan_wmm_add_buf_txqueue(pmadapter, newbuf);

        }
    } else {
        if ((sta_ptr =
             wlan_get_station_entry(priv, prx_pkt->eth803_hdr.dest_addr))) {
            /* Inter BSS packet */
            pmbuf->data_len -= prx_pd->rx_pkt_offset;
            pmbuf->data_offset += prx_pd->rx_pkt_offset;
            pmbuf->flags = MLAN_BUF_FLAG_BRIDGE_PKT;
            wlan_wmm_add_buf_txqueue(pmadapter, pmbuf);
            goto done;
        }
    }
  upload:
    /* Chop off RxPD */
    pmbuf->data_len -= prx_pd->rx_pkt_offset;
    pmbuf->data_offset += prx_pd->rx_pkt_offset;
    pmbuf->pparent = MNULL;

    pmadapter->callbacks.moal_get_system_time(pmadapter->pmoal_handle,
                                              &pmbuf->out_ts_sec,
                                              &pmbuf->out_ts_usec);
    PRINTM(MDATA, "%lu.%lu : Data => kernel\n", pmbuf->out_ts_sec,
           pmbuf->out_ts_usec);

    ret = pmadapter->callbacks.moal_recv_packet(pmadapter->pmoal_handle, pmbuf);
    if (ret == MLAN_STATUS_FAILURE) {
        PRINTM(MERROR, "uAP Rx Error: moal_recv_packet returned error\n");
    }

    if (ret != MLAN_STATUS_PENDING) {
        pmadapter->callbacks.moal_free_mlan_buffer(pmadapter->pmoal_handle,
                                                   pmbuf);
    }
  done:
    LEAVE();
    return ret;
}
