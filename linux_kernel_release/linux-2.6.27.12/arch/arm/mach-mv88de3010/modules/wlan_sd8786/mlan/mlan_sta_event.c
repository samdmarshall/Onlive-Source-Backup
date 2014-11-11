/** @file mlan_sta_event.c
 *  
 *  @brief This file contains MLAN event handling.
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
#include "mlan_tx.h"
#include "mlan_wmm.h"
#include "mlan_11n.h"

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
 *  @brief This function handles disconnect event, reports disconnect
 *  		to upper layer, cleans tx/rx packets,
 *  		resets link state etc.
 *  
 *  @param priv    A pointer to mlan_private structure
 *
 *  @return        N/A
 */
t_void
wlan_reset_connect_state(pmlan_private priv)
{
    mlan_adapter *pmadapter = priv->adapter;
    mlan_status ret = MLAN_STATUS_SUCCESS;
    state_11d_t enable;

    ENTER();

    if (priv->media_connected != MTRUE) {
        LEAVE();
        return;
    }

    PRINTM(MINFO, "Handles disconnect event.\n");

    priv->media_connected = MFALSE;

    /* Free Tx and Rx packets, report disconnect to upper layer */
    wlan_clean_txrx(priv);

    /* Reset SNR/NF/RSSI values */
    priv->data_rssi_last = 0;
    priv->data_nf_last = 0;
    priv->data_rssi_avg = 0;
    priv->data_nf_avg = 0;
    priv->bcn_rssi_last = 0;
    priv->bcn_nf_last = 0;
    priv->bcn_rssi_avg = 0;
    priv->bcn_nf_avg = 0;
    priv->rxpd_rate = 0;
    priv->rxpd_htinfo = 0;

    PRINTM(MINFO, "Current SSID=%s, SSID Length=%u\n",
           priv->curr_bss_params.bss_descriptor.ssid.ssid,
           priv->curr_bss_params.bss_descriptor.ssid.ssid_len);
    PRINTM(MINFO, "Previous SSID=%s, SSID Length=%u\n",
           priv->prev_ssid.ssid, priv->prev_ssid.ssid_len);
    priv->sec_info.ewpa_enabled = MFALSE;
    priv->sec_info.wpa_enabled = MFALSE;
    priv->sec_info.wpa2_enabled = MFALSE;
    priv->wpa_ie_len = 0;

    priv->sec_info.encryption_mode = MLAN_ENCRYPTION_MODE_NONE;

    /* Enable auto data rate */
    priv->is_data_rate_auto = MTRUE;
    priv->data_rate = 0;

    if (priv->bss_mode == MLAN_BSS_MODE_IBSS) {
        priv->adhoc_state = ADHOC_IDLE;
        priv->adhoc_is_link_sensed = MFALSE;
    }

    /* 
     * Memorize the previous SSID and BSSID so
     * it could be used for re-assoc
     */
    memcpy(&priv->prev_ssid,
           &priv->curr_bss_params.bss_descriptor.ssid,
           sizeof(mlan_802_11_ssid));
    memcpy(priv->prev_bssid,
           priv->curr_bss_params.bss_descriptor.mac_address,
           MLAN_MAC_ADDR_LENGTH);

    /* Need to erase the current SSID and BSSID info */
    memset(&priv->curr_bss_params, 0x00, sizeof(priv->curr_bss_params));

    pmadapter->tx_lock_flag = MFALSE;
    pmadapter->ps_state = PS_STATE_AWAKE;
    pmadapter->pm_wakeup_card_req = MFALSE;

    if ((wlan_11d_get_state(priv) == ENABLE_11D) &&
        (pmadapter->state_11d.user_enable_11d == DISABLE_11D)) {

        pmadapter->state_11d.enable_11d = DISABLE_11D;
        enable = DISABLE_11D;

        /* Send cmd to FW to enable/disable 11D function */
        ret = wlan_prepare_cmd(priv,
                               HostCmd_CMD_802_11_SNMP_MIB,
                               HostCmd_ACT_GEN_SET, Dot11D_i, MNULL, &enable);
        if (ret)
            PRINTM(MERROR, "11D: Failed to enable 11D\n");
    }
    if (pmadapter->num_cmd_timeout && pmadapter->curr_cmd &&
        (pmadapter->cmd_timer_is_set == MFALSE)) {
        LEAVE();
        return;
    }
    wlan_recv_event(priv, MLAN_EVENT_ID_FW_DISCONNECTED, MNULL);

    LEAVE();
}

/** 
 *  @brief This function handles link lost, deauth and
 *  		disassoc events.
 *  
 *  @param priv    A pointer to mlan_private structure
 *  @return        N/A
 */
static void
wlan_handle_disconnect_event(pmlan_private priv)
{
    ENTER();
    if (priv->media_connected == MTRUE) {
        wlan_reset_connect_state(priv);
    }
    LEAVE();
}

/** 
 *  @brief This function sends the HS_ACTIVATED event to the application
 *   
 *  @param priv         A pointer to mlan_private structure
 *  @param activated    MTRUE if activated, MFALSE if de-activated
 *
 *  @return             None
 */
t_void
wlan_host_sleep_activated_event(pmlan_private priv, t_u8 activated)
{
    ENTER();

    if (activated) {
        if (priv->adapter->is_hs_configured) {
            priv->adapter->hs_activated = MTRUE;
            wlan_recv_event(priv, MLAN_EVENT_ID_DRV_HS_ACTIVATED, MNULL);
        } else
            PRINTM(MWARN, "hs_activated: HS not configured !!!\n");
    } else {
        priv->adapter->hs_activated = MFALSE;
        wlan_recv_event(priv, MLAN_EVENT_ID_DRV_HS_DEACTIVATED, MNULL);
    }

    LEAVE();
}

/** 
 *  @brief This function sends the HS_WAKEUP event to the application
 *   
 *  @param priv         A pointer to mlan_private structure
 *
 *  @return             None
 */
t_void
wlan_host_sleep_wakeup_event(pmlan_private priv)
{
    ENTER();

    if (priv->adapter->is_hs_configured) {
        wlan_recv_event(priv, MLAN_EVENT_ID_FW_HS_WAKEUP, MNULL);
    } else {
        PRINTM(MWARN, "hs_wakeup: Host Sleep not configured !!!\n");
    }

    LEAVE();
}

/** 
 *  @brief Perform hs related activities on receving the power up interrupt
 *  
 *  @param pmadapter  A pointer to the adapter structure
 *  @return           N/A
 */
t_void
wlan_process_hs_config(pmlan_adapter pmadapter)
{
    PRINTM(MINFO,
           "Auto Cancelling host sleep since there is some interrupt from the firmware\n");
    wlan_pm_wakeup_card(pmadapter);
    pmadapter->hs_activated = MFALSE;
    pmadapter->is_hs_configured = MFALSE;
    wlan_host_sleep_activated_event(wlan_get_priv(pmadapter, MLAN_BSS_TYPE_STA),
                                    MFALSE);
    return;
}

/** 
 *  @brief This function sends the OBSS scan parameters to the application
 *  
 *  @param pmpriv     A pointer to mlan_private structure
 *
 *  @return           N/A
 */
t_void
wlan_2040_coex_event(pmlan_private pmpriv)
{
    t_u8 event_buf[100];
    mlan_event *pevent = (mlan_event *) event_buf;
    t_u8 ele_len;

    ENTER();

    if (pmpriv->curr_bss_params.bss_descriptor.poverlap_bss_scan_param &&
        pmpriv->curr_bss_params.bss_descriptor.poverlap_bss_scan_param->
        ieee_hdr.element_id == OVERLAPBSSSCANPARAM) {
        ele_len =
            pmpriv->curr_bss_params.bss_descriptor.poverlap_bss_scan_param->
            ieee_hdr.len;
        pevent->bss_num = pmpriv->bss_num;
        pevent->event_id = MLAN_EVENT_ID_DRV_OBSS_SCAN_PARAM;
        /* Copy OBSS scan parameters */
        memcpy((t_u8 *) pevent->event_buf,
               (t_u8 *) & pmpriv->curr_bss_params.bss_descriptor.
               poverlap_bss_scan_param->obss_scan_param, ele_len);
        pevent->event_len = ele_len;
        wlan_recv_event(pmpriv, MLAN_EVENT_ID_DRV_OBSS_SCAN_PARAM, pevent);
    }

    LEAVE();
}

/** 
 *  @brief This function handles events generated by firmware
 *  
 *  @param priv	A pointer to mlan_private structure
 *
 *  @return		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
mlan_process_sta_event(IN t_void * priv)
{
    pmlan_private pmpriv = (pmlan_private) priv;
    pmlan_adapter pmadapter = pmpriv->adapter;
    mlan_status ret = MLAN_STATUS_SUCCESS;
    t_u32 eventcause = pmadapter->event_cause;
    t_u8 event_buf[100];
    mlan_event *pevent = (mlan_event *) event_buf;

    ENTER();

    /* Clear BSS_NO_BITS from event */
    eventcause &= EVENT_ID_MASK;

    switch (eventcause) {
    case EVENT_DUMMY_HOST_WAKEUP_SIGNAL:
        PRINTM(MERROR,
               "Invalid EVENT: DUMMY_HOST_WAKEUP_SIGNAL, ignoring it\n");
        break;
    case EVENT_LINK_SENSED:
        PRINTM(MEVENT, "EVENT: LINK_SENSED\n");
        pmpriv->adhoc_is_link_sensed = MTRUE;
        wlan_recv_event(pmpriv, MLAN_EVENT_ID_FW_ADHOC_LINK_SENSED, MNULL);
        break;

    case EVENT_DEAUTHENTICATED:
        PRINTM(MEVENT, "EVENT: Deauthenticated\n");
        pmadapter->dbg.num_event_deauth++;
        wlan_handle_disconnect_event(pmpriv);
        break;

    case EVENT_DISASSOCIATED:
        PRINTM(MEVENT, "EVENT: Disassociated\n");
        pmadapter->dbg.num_event_disassoc++;
        wlan_handle_disconnect_event(pmpriv);
        break;

    case EVENT_LINK_LOST:
        PRINTM(MEVENT, "EVENT: Link lost\n");
        pmadapter->dbg.num_event_link_lost++;
        wlan_handle_disconnect_event(pmpriv);
        break;

    case EVENT_PS_SLEEP:
        PRINTM(MINFO, "EVENT: SLEEP\n");
        PRINTM(MEVENT, "_");

        pmadapter->ps_state = PS_STATE_PRE_SLEEP;

        wlan_check_ps_cond(pmadapter);
        break;

    case EVENT_PS_AWAKE:
        PRINTM(MINFO, "EVENT: AWAKE \n");
        PRINTM(MEVENT, "|");
        pmadapter->tx_lock_flag = MFALSE;
        if (pmadapter->sleep_period.period) {
            if (MTRUE == wlan_check_last_packet_indication(pmpriv)) {
                if (!pmadapter->data_sent && pmpriv->gen_null_pkg) {
                    wlan_send_null_packet(pmpriv,
                                          MRVDRV_TxPD_POWER_MGMT_NULL_PACKET |
                                          MRVDRV_TxPD_POWER_MGMT_LAST_PACKET);
                    pmadapter->ps_state = PS_STATE_SLEEP;
                    return MLAN_STATUS_SUCCESS;
                }
            }
        }
        pmadapter->ps_state = PS_STATE_AWAKE;
        pmadapter->pm_wakeup_card_req = MFALSE;
        pmadapter->pm_wakeup_fw_try = MFALSE;

        break;

    case EVENT_DEEP_SLEEP_AWAKE:
        wlan_pm_reset_card(pmadapter);
        PRINTM(MEVENT, "EVENT: DS_AWAKE\n");
        if (pmadapter->is_deep_sleep == MTRUE) {
            pmadapter->is_deep_sleep = MFALSE;
        }
        wlan_recv_event(pmpriv, MLAN_EVENT_ID_FW_DS_AWAKE, MNULL);
        break;

    case EVENT_HS_ACT_REQ:
        PRINTM(MEVENT, "EVENT: HS_ACT_REQ\n");
        ret = wlan_prepare_cmd(priv,
                               HostCmd_CMD_802_11_HS_CFG_ENH,
                               0, 0, MNULL, MNULL);
        break;

    case EVENT_MIC_ERR_UNICAST:
        PRINTM(MEVENT, "EVENT: UNICAST MIC ERROR\n");
        wlan_recv_event(pmpriv, MLAN_EVENT_ID_FW_MIC_ERR_UNI, MNULL);
        break;

    case EVENT_MIC_ERR_MULTICAST:
        PRINTM(MEVENT, "EVENT: MULTICAST MIC ERROR\n");
        wlan_recv_event(pmpriv, MLAN_EVENT_ID_FW_MIC_ERR_MUL, MNULL);
        break;
    case EVENT_MIB_CHANGED:
    case EVENT_INIT_DONE:
        break;

    case EVENT_ADHOC_BCN_LOST:
        PRINTM(MEVENT, "EVENT: ADHOC_BCN_LOST\n");
        pmpriv->adhoc_is_link_sensed = MFALSE;
        wlan_clean_txrx(pmpriv);
        wlan_recv_event(pmpriv, MLAN_EVENT_ID_FW_ADHOC_LINK_LOST, MNULL);
        break;
    case EVENT_BG_SCAN_REPORT:
        PRINTM(MEVENT, "EVENT: BGS_REPORT\n");
        /* Clear the previous scan result */
        memset(pmadapter->pscan_table, 0x00,
               sizeof(BSSDescriptor_t) * MRVDRV_MAX_BSSID_LIST);
        pmadapter->num_in_scan_table = 0;
        pmadapter->pbcn_buf_end = pmadapter->bcn_buf;
        ret = wlan_prepare_cmd(pmpriv,
                               HostCmd_CMD_802_11_BG_SCAN_QUERY,
                               HostCmd_ACT_GEN_GET, 0, MNULL, MNULL);
        break;
    case EVENT_WMM_STATUS_CHANGE:
        PRINTM(MEVENT, "EVENT: WMM status changed\n");
        ret = (mlan_status) wlan_cmd_wmm_status_change(pmpriv);
        break;

    case EVENT_RSSI_LOW:
        PRINTM(MEVENT, "EVENT: Beacon RSSI_LOW\n");
        wlan_recv_event(pmpriv, MLAN_EVENT_ID_FW_BCN_RSSI_LOW, MNULL);
        break;
    case EVENT_SNR_LOW:
        PRINTM(MEVENT, "EVENT: Beacon SNR_LOW\n");
        wlan_recv_event(pmpriv, MLAN_EVENT_ID_FW_BCN_SNR_LOW, MNULL);
        break;
    case EVENT_MAX_FAIL:
        PRINTM(MEVENT, "EVENT: MAX_FAIL\n");
        wlan_recv_event(pmpriv, MLAN_EVENT_ID_FW_MAX_FAIL, MNULL);
        break;
    case EVENT_RSSI_HIGH:
        PRINTM(MEVENT, "EVENT: Beacon RSSI_HIGH\n");
        wlan_recv_event(pmpriv, MLAN_EVENT_ID_FW_BCN_RSSI_HIGH, MNULL);
        break;
    case EVENT_SNR_HIGH:
        PRINTM(MEVENT, "EVENT: Beacon SNR_HIGH\n");
        wlan_recv_event(pmpriv, MLAN_EVENT_ID_FW_BCN_SNR_HIGH, MNULL);
        break;
    case EVENT_DATA_RSSI_LOW:
        PRINTM(MEVENT, "EVENT: Data RSSI_LOW\n");
        wlan_recv_event(pmpriv, MLAN_EVENT_ID_FW_DATA_RSSI_LOW, MNULL);
        break;
    case EVENT_DATA_SNR_LOW:
        PRINTM(MEVENT, "EVENT: Data SNR_LOW\n");
        wlan_recv_event(pmpriv, MLAN_EVENT_ID_FW_DATA_SNR_LOW, MNULL);
        break;
    case EVENT_DATA_RSSI_HIGH:
        PRINTM(MEVENT, "EVENT: Data RSSI_HIGH\n");
        wlan_recv_event(pmpriv, MLAN_EVENT_ID_FW_DATA_RSSI_HIGH, MNULL);
        break;
    case EVENT_DATA_SNR_HIGH:
        PRINTM(MEVENT, "EVENT: Data SNR_HIGH\n");
        wlan_recv_event(pmpriv, MLAN_EVENT_ID_FW_DATA_SNR_HIGH, MNULL);
        break;
    case EVENT_LINK_QUALITY:
        PRINTM(MEVENT, "EVENT: Link Quality\n");
        wlan_recv_event(pmpriv, MLAN_EVENT_ID_FW_LINK_QUALITY, MNULL);
        break;
    case EVENT_PRE_BEACON_LOST:
        PRINTM(MEVENT, "EVENT: Pre-Beacon Lost\n");
        wlan_recv_event(pmpriv, MLAN_EVENT_ID_FW_PRE_BCN_LOST, MNULL);
        break;
    case EVENT_IBSS_COALESCED:
        PRINTM(MEVENT, "EVENT: IBSS_COALESCED\n");
        ret = wlan_prepare_cmd(pmpriv,
                               HostCmd_CMD_802_11_IBSS_COALESCING_STATUS,
                               HostCmd_ACT_GEN_GET, 0, MNULL, MNULL);
        break;
    case EVENT_PORT_RELEASE:
        PRINTM(MEVENT, "EVENT: PORT RELEASE\n");
        wlan_recv_event(pmpriv, MLAN_EVENT_ID_FW_PORT_RELEASE, MNULL);
        pevent->bss_num = pmpriv->bss_num;
        pevent->event_id = MLAN_EVENT_ID_DRV_CONNECTED;
        pevent->event_len = MLAN_MAC_ADDR_LENGTH;
        memcpy((t_u8 *) pevent->event_buf,
               (t_u8 *) pmpriv->curr_bss_params.bss_descriptor.mac_address,
               MLAN_MAC_ADDR_LENGTH);
        wlan_recv_event(pmpriv, MLAN_EVENT_ID_DRV_CONNECTED, pevent);
        /* Send OBSS scan param to the application */
        wlan_2040_coex_event(pmpriv);
        break;
    case EVENT_ADDBA:
        PRINTM(MEVENT, "EVENT: ADDBA Request\n");
        wlan_prepare_cmd(pmpriv, HostCmd_CMD_11N_ADDBA_RSP,
                         HostCmd_ACT_GEN_SET, 0, MNULL, pmadapter->event_body);
        break;
    case EVENT_DELBA:
        PRINTM(MEVENT, "EVENT: DELBA Request\n");
        wlan_11n_delete_bastream(pmpriv, pmadapter->event_body);
        break;
    case EVENT_BA_STREAM_TIEMOUT:
        PRINTM(MEVENT, "EVENT:  BA Stream timeout\n");
        wlan_11n_ba_stream_timeout(pmpriv,
                                   (HostCmd_DS_11N_BATIMEOUT *) pmadapter->
                                   event_body);
        break;
    case EVENT_AMSDU_AGGR_CTRL:
        PRINTM(MEVENT, "EVENT:  AMSDU_AGGR_CTRL %d\n",
               *(t_u16 *) pmadapter->event_body);
        pmpriv->adapter->tx_buf_size =
            MIN(pmpriv->adapter->max_tx_buf_size,
                wlan_le16_to_cpu(*(t_u16 *) pmadapter->event_body));
        PRINTM(MEVENT, "tx_buf_size %d\n", pmpriv->adapter->tx_buf_size);
        break;

    case EVENT_WEP_ICV_ERR:
        PRINTM(MEVENT, "EVENT: WEP ICV error\n");
        pevent->bss_num = pmpriv->bss_num;
        pevent->event_id = MLAN_EVENT_ID_FW_WEP_ICV_ERR;
        pevent->event_len = sizeof(Event_WEP_ICV_ERR);
        memcpy((t_u8 *) pevent->event_buf, pmadapter->event_body,
               pevent->event_len);
        wlan_recv_event(pmpriv, MLAN_EVENT_ID_FW_WEP_ICV_ERR, pevent);
        break;

    case EVENT_BW_CHANGE:
        PRINTM(MEVENT, "EVENT: BW Change\n");
        pevent->bss_num = pmpriv->bss_num;
        pevent->event_id = MLAN_EVENT_ID_FW_BW_CHANGED;
        pevent->event_len = sizeof(t_u8);
        /* Copy event body from the event buffer */
        memcpy((t_u8 *) pevent->event_buf, pmadapter->event_body,
               pevent->event_len);
        wlan_recv_event(pmpriv, MLAN_EVENT_ID_FW_BW_CHANGED, pevent);
        break;

    default:
        PRINTM(MEVENT, "EVENT: unknown event id: %#x\n", eventcause);
        wlan_recv_event(pmpriv, MLAN_EVENT_ID_FW_UNKNOWN, MNULL);
        break;
    }

    LEAVE();
    return ret;
}
