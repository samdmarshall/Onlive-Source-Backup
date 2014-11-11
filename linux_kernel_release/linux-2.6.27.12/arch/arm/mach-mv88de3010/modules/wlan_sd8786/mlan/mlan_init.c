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
#include "mlan_sdio.h"

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
        PRINTM(MERROR, "Allocation failed for add_bsspriotbl\n");
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
    priv->curr_pkt_filter =
        HostCmd_ACT_MAC_RX_ON | HostCmd_ACT_MAC_TX_ON |
        HostCmd_ACT_MAC_ETHERNETII_ENABLE;

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

    memset(&priv->mrvl_assoc_tlv_buf, 0, sizeof(priv->mrvl_assoc_tlv_buf));
    priv->mrvl_assoc_tlv_buf_len = 0;
    memset(&priv->wps, 0, sizeof(priv->wps));
    memset(&priv->gen_ie_buf, 0, sizeof(priv->gen_ie_buf));
    priv->gen_ie_buf_len = 0;
    memset(priv->vs_ie, 0, sizeof(priv->vs_ie));

    priv->wmm_required = MTRUE;
    priv->wmm_enabled = MFALSE;
    priv->wmm_qosinfo = 0;
    priv->gen_null_pkg = MTRUE; /* Enable NULL Pkg generation */
    pmadapter->callbacks.moal_init_lock(&priv->rx_pkt_lock);

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
    t_u32 buf_size;
    BSSDescriptor_t *ptemp_scan_table;
    t_u8 *head_ptr = MNULL;

    ENTER();

    /* Allocate buffer to store the BSSID list */
    buf_size = sizeof(BSSDescriptor_t) * MRVDRV_MAX_BSSID_LIST;
    ret =
        pmadapter->callbacks.moal_malloc(buf_size,
                                         (t_u8 **) & ptemp_scan_table);
    if (ret != MLAN_STATUS_SUCCESS || !ptemp_scan_table) {
        PRINTM(MERROR, "Failed to allocate scan table\n");
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
    ret = wlan_alloc_cmd_buffer(pmadapter);
    if (ret != MLAN_STATUS_SUCCESS) {
        PRINTM(MERROR, "Failed to allocate command buffer\n");
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }

    for (i = 0; i < MLAN_MAX_BSS_NUM; ++i) {
        util_init_list_head(&pmadapter->bssprio_tbl[i].bssprio_head,
                            MTRUE, pmadapter->callbacks.moal_init_lock);

        pmadapter->bssprio_tbl[i].bssprio_cur = MNULL;
    }
    ret =
        pmadapter->callbacks.moal_malloc(MAX_MP_REGS + HEADER_ALIGNMENT,
                                         (t_u8 **) & pmadapter->mp_regs_buf);
    if (ret != MLAN_STATUS_SUCCESS || !pmadapter->mp_regs_buf) {
        PRINTM(MERROR, "Failed to allocate mp_regs_buf\n");
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }
    pmadapter->mp_regs =
        (t_u8 *) ALIGN_ADDR(pmadapter->mp_regs_buf, HEADER_ALIGNMENT);

    PRINTM(MINFO, "Try to assign mp_regs to handle\n");
    ret =
        pmadapter->callbacks.moal_get_cardregs(pmadapter->pmoal_handle,
                                         pmadapter->mp_regs);
    if (ret != MLAN_STATUS_SUCCESS) {
        PRINTM(MERROR, "Failed to assign mp_regs to handle\n");
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }

#if defined(SDIO_MULTI_PORT_TX_AGGR) || defined(SDIO_MULTI_PORT_RX_AGGR)
    ret = wlan_alloc_sdio_mpa_buffers(pmadapter, SDIO_MP_TX_AGGR_DEF_BUF_SIZE,
                                      SDIO_MP_RX_AGGR_DEF_BUF_SIZE);
    if (ret != MLAN_STATUS_SUCCESS) {
        PRINTM(MERROR, "Failed to allocate sdio mp-a buffers\n");
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }
#endif
    if ((wlan_get_priv(pmadapter, MLAN_BSS_TYPE_ANY))->bss_type ==
        MLAN_BSS_TYPE_STA)
        pmadapter->psleep_cfm =
            wlan_alloc_mlan_buffer(&pmadapter->callbacks,
                                   HEADER_ALIGNMENT +
                                   sizeof(opt_sleep_confirm_buffer));

    if (!pmadapter->psleep_cfm) {
        PRINTM(MERROR, "Failed to allocate sleep confirm buffers\n");
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }
    head_ptr =
        (t_u8 *) ALIGN_ADDR(pmadapter->psleep_cfm->pbuf +
                            pmadapter->psleep_cfm->data_offset,
                            HEADER_ALIGNMENT);
    pmadapter->psleep_cfm->data_offset +=
        head_ptr - (pmadapter->psleep_cfm->pbuf +
                    pmadapter->psleep_cfm->data_offset);

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
    int i;
    opt_sleep_confirm_buffer *sleep_cfm_buf =
        (opt_sleep_confirm_buffer *) (pmadapter->psleep_cfm->pbuf +
                                      pmadapter->psleep_cfm->data_offset);

    ENTER();

    pmadapter->cmd_sent = MFALSE;
    pmadapter->data_sent = MTRUE;
    pmadapter->mp_rd_bitmap = 0;
    pmadapter->mp_wr_bitmap = 0;
    pmadapter->curr_rd_port = 1;
    pmadapter->curr_wr_port = 1;
    for (i = 0; i < MAX_NUM_TID; i++) {
        pmadapter->tx_eligibility[i] = 1;
    }

#ifdef SDIO_MULTI_PORT_TX_AGGR
    pmadapter->mpa_tx.buf_len = 0;
    pmadapter->mpa_tx.pkt_cnt = 0;
    pmadapter->mpa_tx.start_port = 0;

    pmadapter->mpa_tx.enabled = 1;
    pmadapter->mpa_tx.pkt_aggr_limit = SDIO_MP_AGGR_DEF_PKT_LIMIT;
#endif /* SDIO_MULTI_PORT_TX_AGGR */

#ifdef SDIO_MULTI_PORT_RX_AGGR
    pmadapter->mpa_rx.buf_len = 0;
    pmadapter->mpa_rx.pkt_cnt = 0;
    pmadapter->mpa_rx.start_port = 0;

    pmadapter->mpa_rx.enabled = 1;
    pmadapter->mpa_rx.pkt_aggr_limit = SDIO_MP_AGGR_DEF_PKT_LIMIT;
#endif /* SDIO_MULTI_PORT_RX_AGGR */

    pmadapter->cmd_resp_received = MFALSE;
    pmadapter->event_received = MFALSE;
    pmadapter->data_received = MFALSE;

    pmadapter->cmd_timer_is_set = MFALSE;

    /* PnP and power profile */
    pmadapter->surprise_removed = MFALSE;

    /* Status variables */
    pmadapter->hw_status = WlanHardwareStatusInitializing;

    pmadapter->ps_mode = Wlan802_11PowerModeCAM;
    pmadapter->ps_state = PS_STATE_AWAKE;
    pmadapter->need_to_wakeup = MFALSE;

    /* Scan type */
    pmadapter->scan_type = HostCmd_SCAN_TYPE_ACTIVE;
    /* Scan mode */
    pmadapter->scan_mode = HostCmd_BSS_MODE_ANY;
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
    pmadapter->multiple_dtim = MRVDRV_DEFAULT_MULTIPLE_DTIM;

    pmadapter->local_listen_interval = 0;       /* default value in firmware
                                                   will be used */
    pmadapter->fw_wakeup_method = WAKEUP_FW_UNCHANGED;

    pmadapter->is_deep_sleep = MFALSE;

    pmadapter->delay_null_pkt = MFALSE;
    pmadapter->delay_to_ps = 100;

    pmadapter->pm_wakeup_card_req = MFALSE;

    pmadapter->pm_wakeup_fw_try = MFALSE;

    pmadapter->max_tx_buf_size = MLAN_TX_DATA_BUF_SIZE_2K;
    pmadapter->tx_buf_size = MLAN_TX_DATA_BUF_SIZE_2K;

    pmadapter->is_hs_configured = MFALSE;
    pmadapter->hs_cfg.params.hs_config.conditions = HOST_SLEEP_CFG_CANCEL;
    pmadapter->hs_cfg.params.hs_config.gpio = 0;
    pmadapter->hs_cfg.params.hs_config.gap = 0;
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

    pmadapter->psleep_cfm->buf_type = MLAN_BUF_TYPE_CMD;

    if ((wlan_get_priv(pmadapter, MLAN_BSS_TYPE_ANY))->bss_type ==
        MLAN_BSS_TYPE_STA) {
        pmadapter->psleep_cfm->data_len = sizeof(HostCmd_DS_COMMAND);
        memset(&sleep_cfm_buf->ps_cfm_sleep, 0, sizeof(HostCmd_DS_COMMAND));
        sleep_cfm_buf->ps_cfm_sleep.command =
            wlan_cpu_to_le16(HostCmd_CMD_802_11_PS_MODE_ENH);
        sleep_cfm_buf->ps_cfm_sleep.size =
            wlan_cpu_to_le16(sizeof(HostCmd_DS_COMMAND));
        sleep_cfm_buf->ps_cfm_sleep.result = 0;
        sleep_cfm_buf->ps_cfm_sleep.params.psmode_enh.action =
            wlan_cpu_to_le16(SLEEP_CONFIRM);
    }

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
    memset(&pmadapter->arp_filter, 0, sizeof(pmadapter->arp_filter));
    pmadapter->arp_filter_size = 0;

    LEAVE();
    return;
}

/********************************************************
        Global Functions
********************************************************/
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
    pmlan_private priv = MNULL;
    t_s32 i = 0;
    t_u8 first_sta = MTRUE;

    ENTER();

    /* Allocate memory for member of adapter structure */
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
    if (pmadapter->mfg_mode != MTRUE) {
#endif
        for (i = 0; i < MLAN_MAX_BSS_NUM; i++) {
            if (pmadapter->priv[i]) {
                ret =
                    pmadapter->priv[i]->ops.init_cmd(pmadapter->priv[i],
                                                     first_sta);
                if (ret == MLAN_STATUS_FAILURE)
                    goto done;
                first_sta = MFALSE;
            }
        }
#ifdef MFG_CMD_SUPPORT
    }
#endif

    if (util_peek_list(&pmadapter->cmd_pending_q,
                       pmadapter->callbacks.moal_spin_lock,
                       pmadapter->callbacks.moal_spin_unlock)) {
        /* Send the first command in queue and return */
        if (mlan_main_process(pmadapter) == MLAN_STATUS_FAILURE)
	    ret = MLAN_STATUS_FAILURE;
	else
            ret = MLAN_STATUS_PENDING;
    } else {
        pmadapter->hw_status = WlanHardwareStatusReady;
    }
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
        PRINTM(MERROR, "The adapter is MNULL.\n");
        LEAVE();
        return;
    }

    wlan_cancel_all_pending_cmd(pmadapter);
    /* Free command buffer */
    PRINTM(MINFO, "Free Command buffer\n");
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

    PRINTM(MINFO, "Free ScanTable\n");
    if (pmadapter->pscan_table) {
        pcb->moal_mfree((t_u8 *) pmadapter->pscan_table);
        pmadapter->pscan_table = MNULL;
    }

    if (pmadapter->mp_regs_buf) {
        pcb->moal_mfree((t_u8 *) pmadapter->mp_regs_buf);
        pmadapter->mp_regs_buf = MNULL;
        pmadapter->mp_regs = MNULL;
    }
#if defined(SDIO_MULTI_PORT_TX_AGGR) || defined(SDIO_MULTI_PORT_RX_AGGR)
    wlan_free_sdio_mpa_buffers(pmadapter);
#endif
    wlan_free_mlan_buffer(&pmadapter->callbacks, pmadapter->psleep_cfm);
    LEAVE();
    return;
}

/**
 *  @brief The cmdresp handler calls this function for init_fw_complete callback
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *
 *  @return		MLAN_STATUS_SUCCESS
 *                      	The firmware initialization callback succeeded.
 * 			MLAN_STATUS_FAILURE
 *                      	The firmware initialization callback failed.
 */
mlan_status
wlan_init_fw_complete(IN pmlan_adapter pmadapter)
{
    mlan_status status = MLAN_STATUS_SUCCESS;
    pmlan_callbacks pcb = &pmadapter->callbacks;

    ENTER();

    /* Check if hardware is ready */
    if (pmadapter->hw_status != WlanHardwareStatusReady)
        status = MLAN_STATUS_FAILURE;

    LEAVE();
    /* Invoke callback */
    return pcb->moal_init_fw_complete(pmadapter->pmoal_handle, status);
}

/**
 *  @brief The cmdresp handler calls this function for shutdown_fw_complete callback
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *
 *  @return		MLAN_STATUS_SUCCESS
 *                      	The firmware shutdown callback succeeded.
 *			MLAN_STATUS_FAILURE
 *                      	The firmware shutdown callback failed.
 */
mlan_status
wlan_shutdown_fw_complete(IN pmlan_adapter pmadapter)
{
    pmlan_callbacks pcb = &pmadapter->callbacks;
    mlan_status status = MLAN_STATUS_SUCCESS;

    pmadapter->hw_status = WlanHardwareStatusNotReady;
    /* Invoke callback */
    return pcb->moal_shutdown_fw_complete(pmadapter->pmoal_handle, status);
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
