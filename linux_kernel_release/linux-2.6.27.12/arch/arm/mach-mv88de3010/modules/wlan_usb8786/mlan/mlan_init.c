/** @file mlan_init.c
 *  
 *  @brief This file contains the initialization for FW
 *  and HW. 
 * 
 *  Copyright (C) 2008-2009, Marvell International Ltd. 
 *  All Rights Reserved
 */

/********************************************************
Change log:
    10/13/2008: initial version
********************************************************/

#include "mlan.h"
#include "mlan_11d.h"
#include "mlan_join.h"
#include "mlan_scan.h"
#include "mlan_util.h"
#include "mlan_fw.h"
#include "mlan_main.h"
#include "mlan_init.h"
#include "mlan_wmm.h"
#include "mlan_11n.h"

/********************************************************
        Global Variables
********************************************************/

/********************************************************
        Local Functions
********************************************************/

/**
 *  @brief This function adds a BSS priority table
 *  
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param bss_num  	BSS number
 *
 *  @return		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_add_bsspriotbl(pmlan_adapter pmadapter, t_u8 bss_num)
{
    pmlan_private priv = pmadapter->priv[bss_num];
    mlan_bssprio_node *pbssprio;
    mlan_status status = MLAN_STATUS_SUCCESS;

    ENTER();

    if ((status = pmadapter->callbacks.moal_malloc(sizeof(mlan_bssprio_node),
                                                   (t_u8 **) & pbssprio))) {
        PRINTM(ERROR, "Allocation failed for add_bsspriotbl\n");
        return status;
    }

    pbssprio->priv = priv;

    util_init_list((pmlan_linked_list) pbssprio);

    if (!pmadapter->bssprio_tbl[priv->bss_priority].bssprio_cur)
        pmadapter->bssprio_tbl[priv->bss_priority].bssprio_cur = pbssprio;

    util_enqueue_list_tail(&pmadapter->bssprio_tbl[priv->bss_priority].
                           bssprio_head, (pmlan_linked_list) pbssprio,
                           pmadapter->callbacks.moal_spin_lock,
                           pmadapter->callbacks.moal_spin_unlock);

    LEAVE();
    return status;
}

/**
 *  @brief This function deletes the BSS priority table
 *  
 *  @param priv		A pointer to mlan_private structure
 *
 *  @return		N/A
 */
t_void
wlan_delete_bsspriotbl(pmlan_private priv)
{
    int i;
    pmlan_adapter pmadapter = priv->adapter;
    mlan_bssprio_node *pbssprio_node, *ptmp_node;
    pmlan_list_head phead;

    ENTER();

    for (i = 0; i < MLAN_MAX_BSS_NUM; ++i) {
        phead = &pmadapter->bssprio_tbl[i].bssprio_head;
        if (phead) {
            pbssprio_node = (mlan_bssprio_node *) util_peek_list(phead,
                                                                 pmadapter->
                                                                 callbacks.
                                                                 moal_spin_lock,
                                                                 pmadapter->
                                                                 callbacks.
                                                                 moal_spin_unlock);
            while (pbssprio_node && ((pmlan_list_head) pbssprio_node != phead)) {
                ptmp_node = pbssprio_node->pnext;
                if (pbssprio_node->priv == priv) {
                    util_unlink_list(phead,
                                     (pmlan_linked_list) pbssprio_node,
                                     pmadapter->callbacks.moal_spin_lock,
                                     pmadapter->callbacks.moal_spin_unlock);

                    pmadapter->callbacks.moal_mfree((t_u8 *) pbssprio_node);
                }
                pbssprio_node = ptmp_node;
            }
        }
    }

    LEAVE();
}

/**
 *  @brief This function initializes the private structure
 *  		and sets default values to the members of mlan_private.
 *  
 *  @param priv    A pointer to mlan_private structure
 *
 *  @return        N/A
 */
t_void
wlan_init_priv(pmlan_private priv)
{
    int i;
    pmlan_adapter pmadapter = priv->adapter;

    ENTER();

    priv->media_connected = MFALSE;
    memset(priv->curr_addr, 0xff, MLAN_MAC_ADDR_LENGTH);

    priv->pkt_tx_ctrl = 0;
    priv->bss_mode = MLAN_BSS_MODE_INFRA;
    priv->data_rate = 0;        /* Initially indicate the rate as auto */
    priv->is_data_rate_auto = MTRUE;
    priv->bcn_avg_factor = DEFAULT_BCN_AVG_FACTOR;
    priv->data_avg_factor = DEFAULT_DATA_AVG_FACTOR;

    priv->sec_info.wep_status = Wlan802_11WEPDisabled;
    priv->sec_info.authentication_mode = MLAN_AUTH_MODE_OPEN;
    priv->sec_info.encryption_mode = MLAN_ENCRYPTION_MODE_NONE;
    for (i = 0; i < sizeof(priv->wep_key) / sizeof(priv->wep_key[0]); i++)
        memset(&priv->wep_key[i], 0, sizeof(mrvl_wep_key_t));
    priv->wep_key_curr_index = 0;
    priv->adhoc_aes_enabled = MFALSE;
    priv->gen_null_pkg = MTRUE; /* Enable NULL Pkg generation */
    priv->curr_pkt_filter = HostCmd_ACT_MAC_RX_ON |
        HostCmd_ACT_MAC_TX_ON | HostCmd_ACT_MAC_ETHERNETII_ENABLE;

    priv->beacon_period = MLAN_BEACON_INTERVAL;
    priv->pattempted_bss_desc = MNULL;
    memset(&priv->curr_bss_params, 0, sizeof(priv->curr_bss_params));
    priv->listen_interval = MLAN_DEFAULT_LISTEN_INTERVAL;

    memset(&priv->prev_ssid, 0, sizeof(priv->prev_ssid));
    memset(&priv->prev_bssid, 0, sizeof(priv->prev_bssid));
    memset(&priv->assoc_rsp_buf, 0, sizeof(priv->assoc_rsp_buf));
    priv->assoc_rsp_size = 0;
    priv->adhoc_channel = DEFAULT_AD_HOC_CHANNEL;
    priv->atim_window = 0;
    priv->adhoc_state = ADHOC_IDLE;
    priv->tx_power_level = 0;
    priv->max_tx_power_level = 0;
    priv->min_tx_power_level = 0;
    priv->tx_rate = 0;
    priv->rxpd_htinfo = 0;
    priv->rxpd_rate = 0;
    priv->rate_bitmap = 0;
    priv->data_rssi_last = 0;
    priv->data_rssi_avg = 0;
    priv->data_nf_avg = 0;
    priv->data_nf_last = 0;
    priv->bcn_rssi_last = 0;
    priv->bcn_rssi_avg = 0;
    priv->bcn_nf_avg = 0;
    priv->bcn_nf_last = 0;
    memset(&priv->wpa_ie, 0, sizeof(priv->wpa_ie));
    memset(&priv->aes_key, 0, sizeof(priv->aes_key));
    priv->wpa_ie_len = 0;
    priv->wpa_is_gtk_set = MFALSE;
    priv->wmm_required = MTRUE;
    priv->wmm_enabled = MFALSE;
    priv->wmm_qosinfo = 0;
    priv->gen_null_pkg = MTRUE; /* Enable NULL Pkg generation */
    memset(&priv->mrvl_assoc_tlv_buf, 0, sizeof(priv->mrvl_assoc_tlv_buf));
    priv->mrvl_assoc_tlv_buf_len = 0;
    memset(&priv->wps, 0, sizeof(priv->wps));
    memset(&priv->gen_ie_buf, 0, sizeof(priv->gen_ie_buf));
    priv->gen_ie_buf_len = 0;
    memset(priv->vs_ie, 0, sizeof(priv->vs_ie));

    pmadapter->callbacks.moal_init_lock(&priv->rx_pkt_lock);
    priv->pcurr_bcn_buf = MNULL;
    priv->curr_bcn_size = 0;
    pmadapter->callbacks.moal_init_lock(&priv->curr_bcn_buf_lock);

    for (i = 0; i < MAX_NUM_TID; i++)
        priv->addba_reject[i] = ADDBA_RSP_STATUS_ACCEPT;

    util_init_list_head(&priv->tx_ba_stream_tbl_ptr, MTRUE,
                        pmadapter->callbacks.moal_init_lock);
    util_init_list_head(&priv->rx_reorder_tbl_ptr, MTRUE,
                        pmadapter->callbacks.moal_init_lock);

    LEAVE();
    return;
}

/** 
 *  @brief This function allocates buffer for the members of adapter
 *  		structure like command buffer and BSSID list.
 *  
 *  @param pmadapter A pointer to mlan_adapter structure
 *
 *  @return        MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status
wlan_allocate_adapter(pmlan_adapter pmadapter)
{
    int i;
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_callbacks *pcb = (mlan_callbacks *) & pmadapter->callbacks;
    t_u32 buf_size;
    BSSDescriptor_t *ptemp_scan_table;

    ENTER();

    /* Allocate buffer to store the BSSID list */
    buf_size = sizeof(BSSDescriptor_t) * MRVDRV_MAX_BSSID_LIST;
    ret = pcb->moal_malloc(buf_size, (t_u8 **) & ptemp_scan_table);
    if (ret != MLAN_STATUS_SUCCESS || !ptemp_scan_table) {
        PRINTM(ERROR, "Failed to allocate scan table\n");
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }
    pmadapter->pscan_table = ptemp_scan_table;

    /* Initialize cmd_free_q */
    util_init_list_head(&pmadapter->cmd_free_q, MTRUE,
                        pmadapter->callbacks.moal_init_lock);
    /* Initialize cmd_pending_q */
    util_init_list_head(&pmadapter->cmd_pending_q, MTRUE,
                        pmadapter->callbacks.moal_init_lock);
    /* Initialize scan_pending_q */
    util_init_list_head(&pmadapter->scan_pending_q, MTRUE,
                        pmadapter->callbacks.moal_init_lock);

    /* Allocate command buffer */
    if (wlan_alloc_cmd_buffer(pmadapter) != MLAN_STATUS_SUCCESS) {
        PRINTM(ERROR, "Failed to allocate command buffer\n");
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }

    memset(&pmadapter->sleep_cfm_buf.ps_cfm_sleep, 0,
           sizeof(PS_CMD_ConfirmSleep));
    pmadapter->sleep_cfm_buf.ps_cfm_sleep.command =
        wlan_cpu_to_le16(HostCmd_CMD_802_11_PS_MODE);
    pmadapter->sleep_cfm_buf.ps_cfm_sleep.size =
        wlan_cpu_to_le16(sizeof(PS_CMD_ConfirmSleep));
    pmadapter->sleep_cfm_buf.ps_cfm_sleep.result = 0;
    pmadapter->sleep_cfm_buf.ps_cfm_sleep.action =
        wlan_cpu_to_le16(HostCmd_SubCmd_Sleep_Confirmed);

    for (i = 0; i < MLAN_MAX_BSS_NUM; ++i) {
        util_init_list_head(&pmadapter->bssprio_tbl[i].bssprio_head,
                            MTRUE, pmadapter->callbacks.moal_init_lock);

        pmadapter->bssprio_tbl[i].bssprio_cur = MNULL;
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function initializes the adapter structure
 *  		and sets default values to the members of adapter.
 *  
 *  @param pmadapter	A pointer to mlan_adapter structure
 *
 *  @return		N/A
 */
t_void
wlan_init_adapter(pmlan_adapter pmadapter)
{
    ENTER();

    pmadapter->cmd_sent = MFALSE;
    pmadapter->data_sent = MFALSE;
    pmadapter->cmd_resp_received = MFALSE;
    pmadapter->event_received = MFALSE;
    pmadapter->data_received = MFALSE;

    pmadapter->cmd_timer_is_set = MFALSE;

    /* PnP and power profile */
    pmadapter->surprise_removed = MFALSE;

    /* Status variables */
    pmadapter->hw_status = WlanHardwareStatusInitializing;

    /* Scan type */
    pmadapter->scan_type = HostCmd_SCAN_TYPE_ACTIVE;
    /* Scan mode */
    pmadapter->scan_mode = HostCmd_BSS_TYPE_ANY;
    /* Scan time */
    pmadapter->specific_scan_time = MRVDRV_SPECIFIC_SCAN_CHAN_TIME;
    pmadapter->active_scan_time = MRVDRV_ACTIVE_SCAN_CHAN_TIME;
    pmadapter->passive_scan_time = MRVDRV_PASSIVE_SCAN_CHAN_TIME;

    pmadapter->num_in_scan_table = 0;
    memset(pmadapter->pscan_table, 0,
           (sizeof(BSSDescriptor_t) * MRVDRV_MAX_BSSID_LIST));
    pmadapter->scan_probes = 0;

    memset(pmadapter->bcn_buf, 0, sizeof(pmadapter->bcn_buf));
    pmadapter->pbcn_buf_end = pmadapter->bcn_buf;

    pmadapter->radio_on = RADIO_ON;

    pmadapter->ps_mode = Wlan802_11PowerModeCAM;
    pmadapter->multiple_dtim = MRVDRV_DEFAULT_MULTIPLE_DTIM;

    pmadapter->ps_state = PS_STATE_FULL_POWER;
    pmadapter->need_to_wakeup = MFALSE;
    pmadapter->local_listen_interval = 0;       /* default value in firmware
                                                   will be used */

    pmadapter->pm_wakeup_card_req = MFALSE;
    pmadapter->pm_wakeup_fw_try = MFALSE;

    pmadapter->max_tx_buf_size = MLAN_TX_DATA_BUF_SIZE_2K;
    pmadapter->tx_buf_size = MLAN_TX_DATA_BUF_SIZE_2K;

    pmadapter->is_hs_configured = MFALSE;
    pmadapter->hs_cfg.conditions = HOST_SLEEP_CFG_CANCEL;
    pmadapter->hs_cfg.gpio = 0;
    pmadapter->hs_cfg.gap = 0;
    pmadapter->hs_activated = MFALSE;

    memset(pmadapter->event_body, 0, sizeof(pmadapter->event_body));
    pmadapter->hw_dot_11n_dev_cap = 0;
    pmadapter->hw_dev_mcs_support = 0;
    pmadapter->usr_dot_11n_dev_cap = 0;
    pmadapter->usr_dev_mcs_support = 0;
    pmadapter->chan_offset = 0;

    /* Initialize 802.11d */
    wlan_11d_init(pmadapter);

    wlan_wmm_init(pmadapter);
    util_init_list_head(&pmadapter->rx_data_queue, MTRUE,
                        pmadapter->callbacks.moal_init_lock);

    memset(&pmadapter->sleep_cfm_buf.ps_cfm_sleep, 0,
           sizeof(PS_CMD_ConfirmSleep));
    pmadapter->sleep_cfm_buf.ps_cfm_sleep.command =
        wlan_cpu_to_le16(HostCmd_CMD_802_11_PS_MODE);
    pmadapter->sleep_cfm_buf.ps_cfm_sleep.size =
        wlan_cpu_to_le16(sizeof(PS_CMD_ConfirmSleep));
    pmadapter->sleep_cfm_buf.ps_cfm_sleep.result = 0;
    pmadapter->sleep_cfm_buf.ps_cfm_sleep.action =
        wlan_cpu_to_le16(HostCmd_SubCmd_Sleep_Confirmed);

    memset(&pmadapter->sleep_params, 0, sizeof(pmadapter->sleep_params));
    memset(&pmadapter->sleep_period, 0, sizeof(pmadapter->sleep_period));
    pmadapter->tx_lock_flag = MFALSE;
    pmadapter->null_pkt_interval = 0;
    pmadapter->fw_bands = 0;
    pmadapter->config_bands = 0;
    pmadapter->adhoc_start_band = 0;
    pmadapter->pscan_channels = MNULL;
    pmadapter->fw_release_number = 0;
    pmadapter->fw_cap_info = 0;
    memset(&pmadapter->upld_buf, 0, sizeof(pmadapter->upld_buf));
    pmadapter->upld_len = 0;
    pmadapter->event_cause = 0;
    memset(&pmadapter->region_channel, 0, sizeof(pmadapter->region_channel));
    pmadapter->region_code = 0;
    pmadapter->bcn_miss_time_out = DEFAULT_BCN_MISS_TIMEOUT;
    pmadapter->adhoc_awake_period = 0;
    memset(&pmadapter->arp_filter, 0, sizeof(pmadapter->arp_filter));
    pmadapter->arp_filter_size = 0;

    LEAVE();
    return;
}

/**
 *  @brief  This function downloads FW blocks to device
 *
 *  @param pmadapter	A pointer to mlan_adapter
 *  @param pmfw			A pointer to firmware image
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status
wlan_prog_fw_w_helper(IN pmlan_adapter pmadapter, IN pmlan_fw_image pmfw)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    pmlan_callbacks pcb = &pmadapter->callbacks;
    t_u8 *firmware = pmfw->pfw_buf, *RecvBuff;
    t_u32 retries = MAX_FW_RETRY, DataLength;
    t_u32 FWSeqNum = 0, TotalBytes = 0, DnldCmd;
    FWData *fwdata = MNULL;
    FWSyncHeader SyncFWHeader;

    ENTER();

    /* Allocate memory for transmit */
    ret = pcb->moal_malloc(FW_DNLD_TX_BUF_SIZE, (t_u8 **) & fwdata);
    if ((ret != MLAN_STATUS_SUCCESS) || !fwdata) {
        PRINTM(ERROR, "Could not allocate buffer for FW download\n");
        goto fw_exit;
    }

    /* Allocate memory for receive */
    ret = pcb->moal_malloc(FW_DNLD_RX_BUF_SIZE, &RecvBuff);
    if ((ret != MLAN_STATUS_SUCCESS) || !RecvBuff) {
        PRINTM(ERROR, "Could not allocate buffer for FW download response\n");
        goto cleanup;
    }

    do {
        /* Copy the header of the firmware data to get the length */
        memcpy(&fwdata->fw_header, &firmware[TotalBytes], sizeof(FWHeader));

        DataLength = wlan_le32_to_cpu(fwdata->fw_header.data_length);
        DnldCmd = wlan_le32_to_cpu(fwdata->fw_header.dnld_cmd);
        TotalBytes += sizeof(FWHeader);

        /* Copy the firmware data */
        memcpy(fwdata->data, &firmware[TotalBytes], DataLength);
        fwdata->seq_num = wlan_cpu_to_le32(FWSeqNum);
        TotalBytes += DataLength;

        /* If the send/receive fails or CRC occurs then retry */
        while (retries) {
            mlan_buffer mbuf;
            int length = FW_DATA_XMIT_SIZE;
            retries--;

            memset(&mbuf, 0, sizeof(mlan_buffer));
            mbuf.pbuf = (t_u8 *) fwdata;
            mbuf.data_len = length;

            /* Send the firmware block */
            if ((ret = pcb->moal_write_data_sync(pmadapter->pmoal_handle,
                                                 &mbuf, MLAN_USB_EP_CMD_EVENT,
                                                 MLAN_USB_BULK_MSG_TIMEOUT)) !=
                MLAN_STATUS_SUCCESS) {
                PRINTM(ERROR, "fw_dnld: write_data failed, ret %d\n", ret);
                continue;
            }

            memset(&mbuf, 0, sizeof(mlan_buffer));
            mbuf.pbuf = RecvBuff;
            mbuf.data_len = FW_DNLD_RX_BUF_SIZE;

            /* Receive the firmware block response */
            if ((ret = pcb->moal_read_data_sync(pmadapter->pmoal_handle,
                                                &mbuf, MLAN_USB_EP_CMD_EVENT,
                                                MLAN_USB_BULK_MSG_TIMEOUT)) !=
                MLAN_STATUS_SUCCESS) {
                PRINTM(ERROR, "fw_dnld: read_data failed, ret %d\n", ret);
                continue;
            }

            memcpy(&SyncFWHeader, RecvBuff, sizeof(FWSyncHeader));
            endian_convert_syncfwheader(&SyncFWHeader);

            /* Check the firmware block response for CRC errors */
            if (SyncFWHeader.cmd) {
                PRINTM(ERROR, "FW received Blk with CRC error 0x%x\n",
                       SyncFWHeader.cmd);
                ret = MLAN_STATUS_FAILURE;
                continue;
            }

            retries = MAX_FW_RETRY;
            break;
        }

        FWSeqNum++;
        PRINTM(INFO, ".\n");

    } while ((DnldCmd != FW_HAS_LAST_BLOCK) && retries);

  cleanup:
    PRINTM(INFO, "fw_dnld: %d bytes downloaded\n", TotalBytes);

    if (RecvBuff)
        pcb->moal_mfree(RecvBuff);
    if (fwdata)
        pcb->moal_mfree((t_u8 *) fwdata);
    if (retries) {
        ret = MLAN_STATUS_SUCCESS;
    }

  fw_exit:
    LEAVE();
    return ret;
}

/**
 *  @brief  This function receives and checks FW ready event
 *
 *  @param pmadapter		A pointer to mlan_adapter
 *
 *  @return		MLAN_STATUS_SUCCESS or error code
 */
static mlan_status
wlan_check_fw_ready_event(pmlan_adapter pmadapter)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    pmlan_callbacks pcb = &pmadapter->callbacks;
    t_u8 *event_buf = MNULL;
    t_u32 event_sig, event_id;
    mlan_buffer mbuf;

    ENTER();

    /* Allocate memory for receive */
    ret = pcb->moal_malloc(MAX_EVENT_SIZE, &event_buf);
    if ((ret != MLAN_STATUS_SUCCESS) || !event_buf) {
        PRINTM(ERROR, "Could not allocate buffer for event buf\n");
        goto done;
    }

    memset(&mbuf, 0, sizeof(mlan_buffer));
    mbuf.pbuf = (t_u8 *) event_buf;
    mbuf.data_len = MAX_EVENT_SIZE;
    mbuf.buf_type = MLAN_BUF_TYPE_EVENT;

    /* Wait for FW ready event, timeout 2-second */
    if ((ret = pcb->moal_read_data_sync(pmadapter->pmoal_handle,
                                        &mbuf, MLAN_USB_EP_CMD_EVENT,
                                        MLAN_USB_BULK_MSG_TIMEOUT * 20)) !=
        MLAN_STATUS_SUCCESS) {
        PRINTM(ERROR, "Reading FW_READY event failed: %d\n", ret);
    } else {
        event_sig = wlan_le32_to_cpu(*((t_u32 *) event_buf));
        event_id = wlan_le32_to_cpu(*((t_u32 *) event_buf + 1));
        if (event_sig == MLAN_USB_TYPE_EVENT && event_id == EVENT_INIT_DONE) {
            PRINTM(INFO, "FW_READY event is received: 0x%x - 0x%x\n",
                   event_sig, event_id);
        } else {
            PRINTM(ERROR, "FW_READY event is not received: 0x%x - 0x%x\n",
                   event_sig, event_id);
            ret = MLAN_STATUS_FAILURE;
        }
    }
  done:
    if (event_buf)
        pcb->moal_mfree(event_buf);
    LEAVE();
    return ret;
}

/********************************************************
        Global Functions
********************************************************/

/**
 *  @brief  This function downloads firmware to card
 *
 *  @param pmadapter	A pointer to mlan_adapter
 *  @param pmfw			A pointer to firmware image
 *
 *  @return		MLAN_STATUS_SUCCESS or error code
 */
mlan_status
wlan_dnld_fw(IN pmlan_adapter pmadapter, IN pmlan_fw_image pmfw)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;

    ENTER();

    ret = wlan_prog_fw_w_helper(pmadapter, pmfw);
    if (ret != MLAN_STATUS_SUCCESS) {
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }

    ret = wlan_check_fw_ready_event(pmadapter);

    LEAVE();
    return ret;
}

/**
 *  @brief  This function issues commands to initialize firmware
 *
 *  @param pmadapter		A pointer to mlan_adapter
 *
 *  @return		MLAN_STATUS_SUCCESS or error code
 */
mlan_status
wlan_init_cmd(IN pmlan_adapter pmadapter)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    pmlan_private priv = MNULL;
    t_s32 i = 0;
    t_u16 enable = MTRUE;
    mlan_ds_11n_amsdu_aggr_ctrl amsdu_aggr_ctrl;

    ENTER();

    if (pmadapter->priv[0]) {
        priv = pmadapter->priv[0];

        /* 
         * Read MAC address from HW
         */
        ret = wlan_prepare_cmd(priv,
                               HostCmd_CMD_GET_HW_SPEC,
                               HostCmd_ACT_GEN_GET, 0, MNULL, MNULL);
        if (ret) {
            ret = MLAN_STATUS_FAILURE;
            goto done;
        }

        /* Reconfgiure tx buf size */
        ret = wlan_prepare_cmd(priv,
                               HostCmd_CMD_RECONFIGURE_TX_BUFF,
                               HostCmd_ACT_GEN_SET,
                               0, MNULL, &pmadapter->tx_buf_size);
        if (ret) {
            ret = MLAN_STATUS_FAILURE;
            goto done;
        }
    }

    for (i = 0; i < MLAN_MAX_BSS_NUM; i++) {
        if (pmadapter->priv[i]) {
            priv = pmadapter->priv[i];

            /* get tx rate */
            ret = wlan_prepare_cmd(priv,
                                   HostCmd_CMD_TX_RATE_CFG,
                                   HostCmd_ACT_GEN_GET, 0, MNULL, MNULL);
            if (ret) {
                ret = MLAN_STATUS_FAILURE;
                goto done;
            }
            priv->data_rate = 0;

            /* get tx power */
            ret = wlan_prepare_cmd(priv,
                                   HostCmd_CMD_TXPWR_CFG,
                                   HostCmd_ACT_GEN_GET, 0, MNULL, MNULL);
            if (ret) {
                ret = MLAN_STATUS_FAILURE;
                goto done;
            }
            /* set ibss coalescing_status */
            ret = wlan_prepare_cmd(priv,
                                   HostCmd_CMD_802_11_IBSS_COALESCING_STATUS,
                                   HostCmd_ACT_GEN_SET, 0, MNULL, &enable);
            if (ret) {
                ret = MLAN_STATUS_FAILURE;
                goto done;
            }

            memset(&amsdu_aggr_ctrl, 0, sizeof(amsdu_aggr_ctrl));
            amsdu_aggr_ctrl.enable = MLAN_ACT_ENABLE;
            /* Send request to firmware */
            ret = wlan_prepare_cmd(priv,
                                   HostCmd_CMD_AMSDU_AGGR_CTRL,
                                   HostCmd_ACT_GEN_SET, 0, MNULL,
                                   (t_void *) & amsdu_aggr_ctrl);
            if (ret) {
                ret = MLAN_STATUS_FAILURE;
                goto done;
            }
            /* MAC Control must be the last command in init_fw */
            /* set MAC Control */
            ret = wlan_prepare_cmd(priv,
                                   HostCmd_CMD_MAC_CONTROL,
                                   HostCmd_ACT_GEN_SET,
                                   0, MNULL, &priv->curr_pkt_filter);
            if (ret) {
                ret = MLAN_STATUS_FAILURE;
                goto done;
            }
        }
    }

    ret = MLAN_STATUS_PENDING;
  done:
    LEAVE();
    return ret;
}

/**
 *  @brief  This function initializes firmware
 *
 *  @param pmadapter		A pointer to mlan_adapter
 *
 *  @return		MLAN_STATUS_SUCCESS or error code
 */
mlan_status
wlan_init_fw(IN pmlan_adapter pmadapter)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    pmlan_private priv;
    t_s32 i = 0;

    ENTER();

    /* Allocate adapter structure */
    ret = wlan_allocate_adapter(pmadapter);
    if (ret) {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    /* Initialize adapter structure */
    wlan_init_adapter(pmadapter);

    for (i = 0; i < MLAN_MAX_BSS_NUM; i++) {
        if (pmadapter->priv[i]) {
            priv = pmadapter->priv[i];

            /* Initialize private structure */
            wlan_init_priv(priv);

            if ((ret = wlan_add_bsspriotbl(pmadapter, i))) {
                ret = MLAN_STATUS_FAILURE;
                goto done;
            }
        }
    }
#ifdef MFG_CMD_SUPPORT
    if (pmadapter->mfgmode != MTRUE)
#endif
        ret = wlan_init_cmd(pmadapter);
#ifdef MFG_CMD_SUPPORT
    if (pmadapter->mfgmode == MTRUE) {
        pmadapter->hw_status = WlanHardwareStatusReady;
        goto done;
    }
#endif

    /* Send the first command in queue and return */
    if (mlan_main_process(pmadapter) == MLAN_STATUS_FAILURE)
        ret = MLAN_STATUS_FAILURE;
    else
        ret = MLAN_STATUS_PENDING;

  done:
    LEAVE();
    return ret;
}

/** 
 *  @brief This function frees the structure of adapter
 *    
 *  @param pmadapter      A pointer to mlan_adapter structure
 *
 *  @return             N/A
 */
t_void
wlan_free_adapter(pmlan_adapter pmadapter)
{
    mlan_callbacks *pcb = (mlan_callbacks *) & pmadapter->callbacks;

    ENTER();

    if (!pmadapter) {
        PRINTM(ERROR, "The adapter is MNULL.\n");
        LEAVE();
        return;
    }

    wlan_cancel_all_pending_cmd(pmadapter);
    /* Free command buffer */
    PRINTM(INFO, "Free Command buffer\n");
    wlan_free_cmd_buffer(pmadapter);

    util_free_list_head(&pmadapter->cmd_free_q,
                        pmadapter->callbacks.moal_free_lock);

    util_free_list_head(&pmadapter->cmd_pending_q,
                        pmadapter->callbacks.moal_free_lock);

    util_free_list_head(&pmadapter->scan_pending_q,
                        pmadapter->callbacks.moal_free_lock);

    if (pmadapter->cmd_timer_is_set) {
        /* Cancel command timeout timer */
        pcb->moal_stop_timer(pmadapter->pmlan_cmd_timer);
        pmadapter->cmd_timer_is_set = MFALSE;
    }
    if (pmadapter->hb_host_timer_is_set) {
        pcb->moal_stop_timer(pmadapter->hb_host_timer);
        pmadapter->hb_host_timer_is_set = MFALSE;
    }
    if (pmadapter->hb_dev_timer_is_set) {
        pcb->moal_stop_timer(pmadapter->hb_device_timer);
        pmadapter->hb_dev_timer_is_set = MFALSE;
    }

    PRINTM(INFO, "Free ScanTable\n");
    if (pmadapter->pscan_table) {
        pcb->moal_mfree((t_u8 *) pmadapter->pscan_table);
        pmadapter->pscan_table = MNULL;
    }

    LEAVE();
    return;
}
