/**
 * @file mlan_txrx.c
 *
 *  @brief This file contains the handling of TX/RX in MLAN
 *
 *
 *  Copyright (C) 2009, Marvell International Ltd. 
 *  All Rights Reserved
 *   
 */

/*************************************************************
Change Log:
    05/11/2009: initial version
************************************************************/

#include "mlan.h"
#include "mlan_11d.h"
#include "mlan_join.h"
#include "mlan_scan.h"
#include "mlan_util.h"
#include "mlan_fw.h"
#include "mlan_main.h"
#include "mlan_rx.h"
#include "mlan_tx.h"
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
    t_u32 bss_num = 0;
    RxPD *prx_pd;

    ENTER();

    prx_pd = (RxPD *) (pmbuf->pbuf + pmbuf->data_offset);
    /* Get the BSS number from RxPD, get corresponding priv */
    bss_num = prx_pd->bss_num & (MLAN_MAX_BSS_NUM - 1);
    priv = pmadapter->priv[bss_num];
    if (!priv) {
        bss_num = 0;
        priv = wlan_get_priv(pmadapter, MLAN_BSS_TYPE_ANY);
    }
    pmbuf->bss_num = bss_num;
    ret = priv->ops.process_rx_packet(pmadapter, pmbuf);

    LEAVE();
    return ret;
}

/** 
 *  @brief This function queues the packet received from
 *  		kernel/upper layer and wake up the main thread to handle it.
 *  
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pmbuf	A pointer to mlan_buffer includes TX packet
 *
 *  @return		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_tx_packet(pmlan_adapter pmadapter, pmlan_buffer pmbuf)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;

    ENTER();

    HEXDUMP("TX Data", pmbuf->pbuf + pmbuf->data_offset,
            MIN(pmbuf->data_len, 100));

    wlan_wmm_add_buf_txqueue(pmadapter, pmbuf);

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

    ENTER();

    head_ptr = (t_u8 *) priv->ops.process_txpd(priv, pmbuf);
    if (!head_ptr) {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }
    ret = wlan_sdio_host_to_card(pmadapter, MLAN_TYPE_DATA, pmbuf, tx_param);
  done:
    switch (ret) {
    case MLAN_STATUS_RESOURCE:
        PRINTM(MDATA, "MLAN_STATUS_RESOURCE is returned\n");
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
        pmadapter->callbacks.moal_get_system_time(&sec, &usec);
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
 *  @return			MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_write_data_complete(IN pmlan_adapter pmadapter,
                         IN pmlan_buffer pmbuf, IN mlan_status status)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    pmlan_callbacks pcb;

    ENTER();

    ASSERT(pmadapter && pmbuf);

    pcb = &pmadapter->callbacks;
    if (pmbuf->buf_type == MLAN_BUF_TYPE_DATA) {
        PRINTM(MDATA, "wlan_write_data_complete: DATA\n");

        if (!pmbuf->pdesc) {
            /* pmbuf was allocated by MLAN */
            wlan_free_mlan_buffer(&pmadapter->callbacks, pmbuf);
        } else {
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
    ASSERT(pmlan_adapter && pmbuf);

    pcb = &pmadapter->callbacks;

    ASSERT(pmbuf->bss_num < MLAN_MAX_BSS_NUM);

    pmp = pmadapter->priv[pmbuf->bss_num];

    if (pmbuf->pparent) {
        pmbuf_parent = pmbuf->pparent;

        pmadapter->callbacks.moal_spin_lock(pmp->rx_pkt_lock);
        --pmbuf_parent->use_count;
        if (!pmbuf_parent->use_count) {
            pmadapter->callbacks.moal_spin_unlock(pmp->rx_pkt_lock);
            pcb->moal_recv_complete(pmadapter->pmoal_handle,
                                    pmbuf_parent, pmadapter->ioport, status);
        } else {
            pmadapter->callbacks.moal_spin_unlock(pmp->rx_pkt_lock);
        }

        pcb->moal_mfree((t_u8 *) pmbuf);
    } else {
        pcb->moal_recv_complete(pmadapter->pmoal_handle, pmbuf,
                                pmadapter->ioport, status);
    }

    LEAVE();
    return ret;
}
