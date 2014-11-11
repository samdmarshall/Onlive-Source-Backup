/** @file mlan_tx.c
 *
 *  @brief This file contains the handling of data packet
 *  transmission in MLAN module.
 * 
 *  Copyright (C) 2008-2009, Marvell International Ltd. 
 *  All Rights Reserved
 */

/********************************************************
Change log:
    10/21/2008: initial version
********************************************************/

#include "mlan.h"
#include "mlan_11d.h"
#include "mlan_join.h"
#include "mlan_scan.h"
#include "mlan_util.h"
#include "mlan_fw.h"
#include "mlan_main.h"
#include "mlan_tx.h"

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
		Global functions
********************************************************/

/** 
 *  @brief This function checks the conditions and sends packet to device 
 *  
 *  @param priv	   A pointer to mlan_private structure
 *  @param pmbuf   A pointer to the mlan_buffer for process
 *
 *  @return 	   mlan_status
 */
mlan_status
wlan_process_tx(pmlan_private priv, pmlan_buffer pmbuf)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    pmlan_adapter pmadapter = priv->adapter;
    TxPD *plocal_tx_pd;
    t_u8 *head_ptr;
    t_u32 sec, usec;

    ENTER();

    if (!pmbuf->data_len) {
        PRINTM(ERROR, "Tx Error: bad packet length: %d\n", pmbuf->data_len);
        pmbuf->status_code = MLAN_ERROR_PKT_SIZE_INVALID;
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }
    if (pmbuf->data_offset < (sizeof(TxPD) + INTF_HEADER_LEN +
                              HEADER_ALIGNMENT)) {
        PRINTM(ERROR, "not enough space for TxPD: %d\n", pmbuf->data_len);

        pmbuf->status_code = MLAN_ERROR_PKT_SIZE_INVALID;
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    /* head_ptr should be aligned */
    head_ptr =
        pmbuf->pbuf + pmbuf->data_offset - sizeof(TxPD) - INTF_HEADER_LEN;
    head_ptr = (t_u8 *) ((t_u32) head_ptr & ~((t_u32) (HEADER_ALIGNMENT - 1)));

    plocal_tx_pd = (TxPD *) (head_ptr + INTF_HEADER_LEN);
    memset(plocal_tx_pd, 0, sizeof(TxPD));
    /* Set the BSS number to TxPD */
    plocal_tx_pd->bss_num = priv->bss_num;

    plocal_tx_pd->tx_pkt_length = pmbuf->data_len;

    plocal_tx_pd->priority = (t_u8) pmbuf->priority;
    plocal_tx_pd->pkt_delay_2ms =
        wlan_wmm_compute_driver_packet_delay(priv, pmbuf);

    if (plocal_tx_pd->priority < NELEMENTS(priv->wmm.user_pri_pkt_tx_ctrl))
        /* 
         * Set the priority specific tx_control field, setting of 0 will
         *   cause the default value to be used later in this function
         */
        plocal_tx_pd->tx_control
            = priv->wmm.user_pri_pkt_tx_ctrl[plocal_tx_pd->priority];

    if (pmadapter->ps_state != PS_STATE_FULL_POWER) {
        if (MTRUE == wlan_check_last_packet_indication(priv)) {
            pmadapter->tx_lock_flag = MTRUE;
            plocal_tx_pd->flags = MRVDRV_TxPD_POWER_MGMT_LAST_PACKET;
        }
    }

    /* Offset of actual data */
    plocal_tx_pd->tx_pkt_offset = (t_u32) pmbuf->pbuf + pmbuf->data_offset -
        (t_u32) plocal_tx_pd;

    if (!plocal_tx_pd->tx_control) {
        /* TxCtrl set by user or default */
        plocal_tx_pd->tx_control = priv->pkt_tx_ctrl;
    }

    endian_convert_TxPD(plocal_tx_pd);

    /* Adjust the data offset and length to include TxPD in pmbuf */
    pmbuf->data_len += pmbuf->data_offset;
    pmbuf->data_offset = head_ptr - pmbuf->pbuf;
    pmbuf->data_len -= pmbuf->data_offset;

    pmadapter->data_sent = MTRUE;
    ret =
        pmadapter->callbacks.moal_write_data_async(pmadapter->pmoal_handle,
                                                   pmbuf, MLAN_USB_EP_DATA);
    switch (ret) {
    case MLAN_STATUS_RESOURCE:
        PRINTM(DATA, "MLAN_STATUS_RESOURCE is returned\n");
        goto done;
    case MLAN_STATUS_FAILURE:
        pmadapter->data_sent = MFALSE;
        PRINTM(ERROR, "Error: moal_write_data_async failed: 0x%X\n", ret);
        pmadapter->dbg.num_tx_host_to_card_failure++;
        mlan_write_data_async_complete(pmadapter, pmbuf, MLAN_USB_EP_DATA, ret);
        goto done;
    case MLAN_STATUS_PENDING:
        pmadapter->data_sent = MFALSE;
    default:
        break;
    }

    pmadapter->callbacks.moal_get_system_time(&sec, &usec);
    PRINTM(DATA, "%lu.%lu : Data => FW\n", sec, usec);

    DBG_HEXDUMP(DAT_D, "Tx", head_ptr + INTF_HEADER_LEN,
                MIN(pmbuf->data_len + sizeof(TxPD), MAX_DATA_DUMP_LEN));

  done:
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
 *  @brief This function tells firmware to send a NULL data packet.
 *  
 *  @param priv     A pointer to mlan_private structure
 *  @param flags    Trasmit Pkt Flags
 *
 *  @return 	    N/A
 */
mlan_status
wlan_send_null_packet(pmlan_private priv, t_u8 flags)
{
    pmlan_adapter pmadapter = priv->adapter;
    TxPD *ptx_pd;
/* sizeof(TxPD) + Interface specific header */
#define NULL_PACKET_HDR 64
    t_u32 data_len = NULL_PACKET_HDR + HEADER_ALIGNMENT;
    pmlan_buffer pmbuf;
    t_u8 *ptr;
    mlan_status ret = MLAN_STATUS_SUCCESS;
    t_u32 sec, usec;

    ENTER();

    if (pmadapter->surprise_removed == MTRUE) {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    if (priv->media_connected == MFALSE) {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    pmbuf = wlan_alloc_mlan_buffer(&pmadapter->callbacks, data_len);
    if (!pmbuf) {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }
    memset(pmbuf->pbuf, 0, data_len);
    pmbuf->bss_num = priv->bss_num;
    pmbuf->buf_type = MLAN_BUF_TYPE_DATA;
    ptr = (t_u8 *) ALIGN_ADDR(pmbuf->pbuf + pmbuf->data_offset,
                              HEADER_ALIGNMENT);
    pmbuf->data_offset = ptr - pmbuf->pbuf;
    pmbuf->data_len = data_len - pmbuf->data_offset;
    ptx_pd = (TxPD *) (ptr + INTF_HEADER_LEN);

    ptx_pd->tx_control = priv->pkt_tx_ctrl;
    ptx_pd->flags = flags;
    ptx_pd->priority = WMM_HIGHEST_PRIORITY;
    ptx_pd->tx_pkt_offset = sizeof(TxPD);

    endian_convert_TxPD(ptx_pd);

    ret =
        pmadapter->callbacks.moal_write_data_async(pmadapter->pmoal_handle,
                                                   pmbuf, MLAN_USB_EP_DATA);
    switch (ret) {
    case MLAN_STATUS_RESOURCE:
        pmadapter->data_sent = MTRUE;
        /* Fall through FAILURE handling */
    case MLAN_STATUS_FAILURE:
        wlan_free_mlan_buffer(&pmadapter->callbacks, pmbuf);
        PRINTM(ERROR, "TX Error: wlan_send_null_packet failed! ret=%d\n", ret);
        pmadapter->dbg.num_tx_host_to_card_failure++;
        goto done;
    case MLAN_STATUS_SUCCESS:
        wlan_free_mlan_buffer(&pmadapter->callbacks, pmbuf);
        PRINTM(DATA, "TX: wlan_send_null_packet succeeded!\n");
        break;
    case MLAN_STATUS_PENDING:
        break;
    default:
        break;
    }

    pmadapter->callbacks.moal_get_system_time(&sec, &usec);
    PRINTM(DATA, "%lu.%lu : Null data => FW\n", sec, usec);
    DBG_HEXDUMP(DAT_D, "Null data", ptr, sizeof(TxPD) + INTF_HEADER_LEN);
  done:
    LEAVE();
    return ret;
}

/** 
 *  @brief This function checks if we need to send last packet indication.
 *  
 *  @param priv    A pointer to mlan_private structure
 *
 *  @return 	   MTRUE or MFALSE
 */
t_u8
wlan_check_last_packet_indication(pmlan_private priv)
{
    pmlan_adapter pmadapter = priv->adapter;
    t_u8 ret = MFALSE;
    t_u8 prop_ps = MTRUE;

    ENTER();

    if (!pmadapter->sleep_period.period || priv->gen_null_pkg == MFALSE) {
        LEAVE();
        return ret;
    }

    if (wlan_wmm_lists_empty(pmadapter)) {
        if (((priv->curr_bss_params.wmm_uapsd_enabled == MTRUE)
             && priv->wmm_qosinfo) || prop_ps)

            ret = MTRUE;
    }

    LEAVE();
    return ret;
}
