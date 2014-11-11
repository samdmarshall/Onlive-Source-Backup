/**
 * @file mlan_txrx.c
 *
 *  @brief This file contains the handling of TX/RX in MLAN
 *
 *
 *  Copyright (C) 2009-2010, Marvell International Ltd. 
 *  All Rights Reserved
 *   
 */

/*************************************************************
Change Log:
    05/11/2009: initial version
************************************************************/

#include "mlan.h"
#ifdef STA_SUPPORT
#include "mlan_join.h"
#endif
#include "mlan_util.h"
#include "mlan_fw.h"
#include "mlan_main.h"
#include "mlan_wmm.h"
#include "mlan_sdio.h"

/********************************************************
                Local Variables
********************************************************/

/********************************************************
                Global Variables
********************************************************/

/********************************************************
                Local Functions
********************************************************/

/********************************************************
                Global Functions
********************************************************/
/**
 *   @brief This function processes the received buffer
 *     
 *   @param pmadapter A pointer to mlan_adapter
 *   @param pmbuf     A pointer to the received buffer
 *
 *   @return        MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_handle_rx_packet(pmlan_adapter pmadapter, pmlan_buffer pmbuf)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    pmlan_private priv = wlan_get_priv(pmadapter, MLAN_BSS_TYPE_ANY);
    RxPD *prx_pd;

    ENTER();

    prx_pd = (RxPD *) (pmbuf->pbuf + pmbuf->data_offset);
    /* Get the BSS number from RxPD, get corresponding priv */
    priv =
        wlan_get_priv_by_id(pmadapter, prx_pd->bss_num & (MLAN_MAX_BSS_NUM - 1),
                            prx_pd->bss_type);
    if (!priv)
        priv = wlan_get_priv(pmadapter, MLAN_BSS_TYPE_ANY);
    pmbuf->bss_num = priv->bss_index;
    ret = priv->ops.process_rx_packet(pmadapter, pmbuf);

    LEAVE();
    return ret;
}

/** 
 *  @brief This function checks the conditions and sends packet to device 
 *  
 *  @param priv	   A pointer to mlan_private structure
 *  @param pmbuf   A pointer to the mlan_buffer for process
 *  @param tx_param A pointer to mlan_tx_param structure
 *
 *  @return 	   mlan_status
 */
mlan_status
wlan_process_tx(pmlan_private priv, pmlan_buffer pmbuf,
                mlan_tx_param * tx_param)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    pmlan_adapter pmadapter = priv->adapter;
    t_u8 *head_ptr = MNULL;
    t_u32 sec, usec;
#ifdef STA_SUPPORT
    TxPD *plocal_tx_pd = MNULL;
#endif

    ENTER();

    head_ptr = (t_u8 *) priv->ops.process_txpd(priv, pmbuf);
    if (!head_ptr) {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }
#ifdef STA_SUPPORT
    if (priv->bss_type == MLAN_BSS_TYPE_STA)
        plocal_tx_pd = (TxPD *) (head_ptr + INTF_HEADER_LEN);
#endif
    ret = wlan_sdio_host_to_card(pmadapter, MLAN_TYPE_DATA, pmbuf, tx_param);
  done:
    switch (ret) {
    case MLAN_STATUS_RESOURCE:
#ifdef STA_SUPPORT
        if ((priv->bss_type == MLAN_BSS_TYPE_STA) &&
            pmadapter->pps_uapsd_mode && (pmadapter->tx_lock_flag == MTRUE)) {
            pmadapter->tx_lock_flag = MFALSE;
            plocal_tx_pd->flags = 0;
        }
#endif
        PRINTM(MINFO, "MLAN_STATUS_RESOURCE is returned\n");
        break;
    case MLAN_STATUS_FAILURE:
        pmadapter->data_sent = MFALSE;
        PRINTM(MERROR, "Error: moal_write_data_async failed: 0x%X\n", ret);
        pmadapter->dbg.num_tx_host_to_card_failure++;
        wlan_write_data_complete(pmadapter, pmbuf, ret);
        break;
    case MLAN_STATUS_PENDING:
        pmadapter->data_sent = MFALSE;
        break;
    case MLAN_STATUS_SUCCESS:
        wlan_write_data_complete(pmadapter, pmbuf, ret);
        break;
    default:
        break;
    }

    if ((ret == MLAN_STATUS_SUCCESS) || (ret == MLAN_STATUS_PENDING)) {
        pmadapter->callbacks.moal_get_system_time(pmadapter->pmoal_handle, &sec,
                                                  &usec);
        PRINTM(MDATA, "%lu.%lu : Data => FW\n", sec, usec);

        DBG_HEXDUMP(MDAT_D, "Tx", head_ptr + INTF_HEADER_LEN,
                    MIN(pmbuf->data_len + sizeof(TxPD), MAX_DATA_DUMP_LEN));
    }
    LEAVE();
    return ret;
}

/**
 *  @brief Packet send completion handling
 *
 *  @param pmadapter		A pointer to mlan_adapter structure
 *  @param pmbuf		A pointer to mlan_buffer structure
 *  @param status		Callback status
 *
 *  @return			MLAN_STATUS_SUCCESS
 */
mlan_status
wlan_write_data_complete(IN pmlan_adapter pmadapter,
                         IN pmlan_buffer pmbuf, IN mlan_status status)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    pmlan_callbacks pcb;

    ENTER();

    MASSERT(pmadapter && pmbuf);

    pcb = &pmadapter->callbacks;
    if (pmbuf->buf_type == MLAN_BUF_TYPE_DATA) {
        PRINTM(MDATA, "wlan_write_data_complete: DATA\n");

        if (!pmbuf->pdesc) {
            /* pmbuf was allocated by MLAN */
            wlan_free_mlan_buffer(pmadapter, &pmadapter->callbacks, pmbuf);
        } else {
            if (pmbuf->flags & MLAN_BUF_FLAG_BRIDGE_PKT)
                pcb->moal_free_mlan_buffer(pmadapter->pmoal_handle, pmbuf);
            else
                /* pmbuf was allocated by MOAL */
                pcb->moal_send_packet_complete(pmadapter->pmoal_handle,
                                               pmbuf, status);
        }
    }

    LEAVE();
    return ret;
}

/**
 *  @brief Packet receive completion callback handler
 *
 *  @param pmlan_adapter	A pointer to mlan_adapter structure
 *  @param pmbuf		A pointer to mlan_buffer structure
 *  @param status		Callback status
 *
 *  @return			MLAN_STATUS_SUCCESS
 */
mlan_status
wlan_recv_packet_complete(IN t_void * pmlan_adapter,
                          IN pmlan_buffer pmbuf, IN mlan_status status)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_adapter *pmadapter = (mlan_adapter *) pmlan_adapter;
    pmlan_private pmp;
    pmlan_callbacks pcb;
    pmlan_buffer pmbuf_parent;

    ENTER();
    MASSERT(pmlan_adapter && pmbuf);

    pcb = &pmadapter->callbacks;

    MASSERT(pmbuf->bss_num < MLAN_MAX_BSS_NUM);

    pmp = pmadapter->priv[pmbuf->bss_num];

    if (pmbuf->pparent) {
        pmbuf_parent = pmbuf->pparent;

        pmadapter->callbacks.moal_spin_lock(pmadapter->pmoal_handle,
                                            pmp->rx_pkt_lock);
        --pmbuf_parent->use_count;
        if (!pmbuf_parent->use_count) {
            pmadapter->callbacks.moal_spin_unlock(pmadapter->pmoal_handle,
                                                  pmp->rx_pkt_lock);
            pcb->moal_free_mlan_buffer(pmadapter->pmoal_handle, pmbuf_parent);
        } else {
            pmadapter->callbacks.moal_spin_unlock(pmadapter->pmoal_handle,
                                                  pmp->rx_pkt_lock);
        }

        pcb->moal_mfree(pmadapter->pmoal_handle, (t_u8 *) pmbuf);
    } else {
        pcb->moal_free_mlan_buffer(pmadapter->pmoal_handle, pmbuf);
    }

    LEAVE();
    return ret;
}

/**
 *  @brief Add packet to Bypass TX queue
 *
 *  @param pmadapter  Pointer to the mlan_adapter driver data struct
 *  @param pmbuf      Pointer to the mlan_buffer data struct
 *
 *  @return         N/A
 */
t_void
wlan_add_buf_bypass_txqueue(mlan_adapter * pmadapter, pmlan_buffer pmbuf)
{
    ENTER();

    pmbuf->buf_type = MLAN_BUF_TYPE_DATA;
    pmbuf->flags = 0;
    util_enqueue_list_tail(pmadapter->pmoal_handle, &pmadapter->bypass_txq,
                           (pmlan_linked_list) pmbuf,
                           pmadapter->callbacks.moal_spin_lock,
                           pmadapter->callbacks.moal_spin_unlock);
    LEAVE();
}

/**
 *  @brief Check if packets are available in Bypass TX queue
 *
 *  @param pmadapter  Pointer to the mlan_adapter driver data struct
 *
 *  @return         MFALSE if not empty; MTRUE if empty
 */
INLINE t_u8
wlan_bypass_tx_list_empty(mlan_adapter * pmadapter)
{
    t_u8 q_empty = MTRUE;
    pmlan_callbacks pcb = &pmadapter->callbacks;
    pmlan_private pmpriv = MNULL;
    ENTER();

    if (!(pmpriv = wlan_get_priv(pmadapter, MLAN_BSS_TYPE_STA))) {
        goto end;
    }

    if (pmpriv->port_ctrl_mode == MTRUE) {
        q_empty =
            (util_peek_list
             (pmadapter->pmoal_handle, &pmadapter->bypass_txq,
              pcb->moal_spin_lock, pcb->moal_spin_unlock)) ? MFALSE : MTRUE;
    }

  end:
    LEAVE();
    return q_empty;
}

/**
 *  @brief Clean up the By-pass TX queue
 *
 *  @param pmadapter Pointer to the mlan_adapter driver data struct
 *
 *  @return      N/A
 */
t_void
wlan_cleanup_bypass_txq(mlan_adapter * pmadapter)
{
    pmlan_buffer pmbuf;
    ENTER();

    pmadapter->callbacks.moal_spin_lock(pmadapter->pmoal_handle,
                                        pmadapter->bypass_txq.plock);

    while ((pmbuf =
            (pmlan_buffer) util_peek_list(pmadapter->pmoal_handle,
                                          &pmadapter->bypass_txq, MNULL,
                                          MNULL))) {
        util_unlink_list(pmadapter->pmoal_handle, &pmadapter->bypass_txq,
                         (pmlan_linked_list) pmbuf, MNULL, MNULL);
        wlan_write_data_complete(pmadapter, pmbuf, MLAN_STATUS_FAILURE);
    }

    pmadapter->callbacks.moal_spin_unlock(pmadapter->pmoal_handle,
                                          pmadapter->bypass_txq.plock);

    LEAVE();
}

/**
 *  @brief Transmit the By-passed packet awaiting in by-pass queue
 *
 *  @param pmadapter Pointer to the mlan_adapter driver data struct
 *
 *  @return        N/A
 */
t_void
wlan_process_bypass_tx(pmlan_adapter pmadapter)
{
    pmlan_buffer pmbuf;
    mlan_tx_param tx_param;
    mlan_status status = MLAN_STATUS_SUCCESS;

    ENTER();

    if ((pmbuf = (pmlan_buffer) util_dequeue_list(pmadapter->pmoal_handle,
                                                  &pmadapter->bypass_txq,
                                                  pmadapter->callbacks.
                                                  moal_spin_lock,
                                                  pmadapter->callbacks.
                                                  moal_spin_unlock))) {
        PRINTM(MINFO, "Dequeuing bypassed packet %p\n", pmbuf);
        /* XXX: nex_pkt_len ??? */
        tx_param.next_pkt_len = 0;
        status =
            wlan_process_tx(pmadapter->priv[pmbuf->bss_num], pmbuf, &tx_param);

        if (status == MLAN_STATUS_RESOURCE) {
            /* Queue the packet again so that it will be TX'ed later */
            util_enqueue_list_head(pmadapter->pmoal_handle,
                                   &pmadapter->bypass_txq,
                                   (pmlan_linked_list) pmbuf,
                                   pmadapter->callbacks.moal_spin_lock,
                                   pmadapter->callbacks.moal_spin_unlock);
        }
    } else {
        PRINTM(MINFO, "Nothing to send\n");
    }

    LEAVE();
}
