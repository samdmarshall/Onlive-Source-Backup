/** @file mlan_ioctl.c
 *  
 *  @brief This file contains the API handling of MLAN ioctl.
 * 
 *  Copyright (C) 2008-2009, Marvell International Ltd. 
 *  All Rights Reserved
 */

/******************************************************
Change log:
    10/21/2008: initial version
******************************************************/

#include "mlan.h"
#include "mlan_11d.h"
#include "mlan_join.h"
#include "mlan_scan.h"
#include "mlan_util.h"
#include "mlan_fw.h"
#include "mlan_main.h"
#include "mlan_wmm.h"
#include "mlan_11n.h"

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
 *  @brief enable adhoc aes key 
 *
 *  @param pmpriv	A pointer to mlan_private structure
 *
 *  @return		N/A
 */
void
wlan_enable_aes_key(pmlan_private pmpriv)
{
    mlan_ds_encrypt_key encrypt_key;
    if (pmpriv->aes_key.key_param_set.key_len != WPA_AES_KEY_LEN)
        return;
    memset(&encrypt_key, 0, sizeof(mlan_ds_encrypt_key));
    encrypt_key.key_len = WPA_AES_KEY_LEN;
    encrypt_key.key_index = 0x40000000;
    memcpy(encrypt_key.key_material, pmpriv->aes_key.key_param_set.key,
           encrypt_key.key_len);
    wlan_prepare_cmd(pmpriv, HostCmd_CMD_802_11_KEY_MATERIAL,
                     HostCmd_ACT_GEN_SET, KEY_INFO_ENABLED, MNULL,
                     &encrypt_key);
    encrypt_key.key_index &= ~0x40000000;
    wlan_prepare_cmd(pmpriv, HostCmd_CMD_802_11_KEY_MATERIAL,
                     HostCmd_ACT_GEN_SET,
                     KEY_INFO_ENABLED, MNULL, &encrypt_key);
    return;
}

/** 
 *  @brief Get signal information
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_get_info_signal(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    pmlan_private pmpriv = MNULL;
    mlan_status ret = MLAN_STATUS_SUCCESS;

    ENTER();

    if (pioctl_req != MNULL) {
        pmpriv = pmadapter->priv[pioctl_req->bss_num];
    } else {
        PRINTM(ERROR, "MLAN IOCTL information is not present\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    if (!wlan_is_cmd_allowed(pmpriv)) {
        PRINTM(MSG, "RSSI_INFO: not allowed\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    /* Check information buffer length of MLAN IOCTL */
    if (pioctl_req->buf_len < sizeof(mlan_ds_get_signal)) {
        PRINTM(WARN, "MLAN IOCTL information buffer length is too short.\n");
        pioctl_req->data_read_written = 0;
        pioctl_req->buf_len_needed = sizeof(mlan_ds_get_signal);
        ret = MLAN_STATUS_RESOURCE;
        goto exit;
    }

    /* Signal info can be obtained only if connected */
    if (pmpriv->media_connected == MFALSE) {
        PRINTM(INFO, "Can not get signal in disconnected state\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv,
                           HostCmd_CMD_RSSI_INFO,
                           HostCmd_ACT_GEN_GET,
                           0, (t_void *) pioctl_req, MNULL);

    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;

  exit:
    LEAVE();
    return ret;
}

/** 
 *  @brief Get statistics information
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_get_info_stats(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    pmlan_private pmpriv = MNULL;
    mlan_status ret = MLAN_STATUS_SUCCESS;

    ENTER();

    if (pioctl_req != MNULL) {
        pmpriv = pmadapter->priv[pioctl_req->bss_num];
    } else {
        PRINTM(ERROR, "MLAN IOCTL information is not present\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    if (!wlan_is_cmd_allowed(pmpriv)) {
        PRINTM(MSG, "GET_LOG: not allowed\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    /* Check information buffer length of MLAN IOCTL */
    if (pioctl_req->buf_len < sizeof(mlan_ds_get_stats)) {
        PRINTM(WARN, "MLAN IOCTL information buffer length is too short.\n");
        pioctl_req->data_read_written = 0;
        pioctl_req->buf_len_needed = sizeof(mlan_ds_get_stats);
        ret = MLAN_STATUS_RESOURCE;
        goto exit;
    }

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv,
                           HostCmd_CMD_802_11_GET_LOG,
                           HostCmd_ACT_GEN_GET,
                           0, (t_void *) pioctl_req, MNULL);

    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;

  exit:
    LEAVE();
    return ret;
}

/** 
 *  @brief Get BSS information
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_get_info_bss_info(IN pmlan_adapter pmadapter,
                       IN pmlan_ioctl_req pioctl_req)
{
    pmlan_private pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_ds_get_info *info;
    BSSDescriptor_t *pbss_desc;
    t_s32 tbl_idx = 0;

    ENTER();

    /* Get current BSS info */
    pbss_desc = &pmpriv->curr_bss_params.bss_descriptor;
    info = (mlan_ds_get_info *) pioctl_req->pbuf;

    /* BSS mode */
    info->param.bss_info.bss_mode = pmpriv->bss_mode;

    /* SSID */
    memcpy(&info->param.bss_info.ssid, &pbss_desc->ssid,
           sizeof(mlan_802_11_ssid));

    /* BSSID */
    memcpy(&info->param.bss_info.bssid, &pbss_desc->mac_address,
           MLAN_MAC_ADDR_LENGTH);

    /* Channel */
    info->param.bss_info.bss_chan = pbss_desc->channel;

    /* Region code */
    info->param.bss_info.region_code = pmadapter->region_code;

    /* Scan table index if connected */
    info->param.bss_info.scan_table_idx = 0;
    if (pmpriv->media_connected == MTRUE) {
        tbl_idx =
            wlan_find_ssid_in_list(pmpriv, &pbss_desc->ssid,
                                   pbss_desc->mac_address, pmpriv->bss_mode);
        if (tbl_idx >= 0)
            info->param.bss_info.scan_table_idx = tbl_idx;
    }

    /* Connection status */
    info->param.bss_info.media_connected = pmpriv->media_connected;

    /* Radio status */
    info->param.bss_info.radio_on = pmadapter->radio_on;

    /* Tx power information */
    info->param.bss_info.max_power_level = pmpriv->max_tx_power_level;
    info->param.bss_info.min_power_level = pmpriv->min_tx_power_level;

    /* AdHoc state */
    info->param.bss_info.adhoc_state = pmpriv->adhoc_state;

    /* Last beacon NF */
    info->param.bss_info.bcn_nf_last = pmpriv->bcn_nf_last;

    /** wep status */
    if (pmpriv->sec_info.wep_status == Wlan802_11WEPEnabled)
        info->param.bss_info.wep_status = MTRUE;
    else
        info->param.bss_info.wep_status = MFALSE;

    pioctl_req->data_read_written =
        sizeof(mlan_bss_info) + MLAN_SUB_COMMAND_SIZE;

    LEAVE();
    return ret;
}

/** 
 *  @brief Get debug information
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_get_info_debug_info(IN pmlan_adapter pmadapter,
                         IN pmlan_ioctl_req pioctl_req)
{
    pmlan_private pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_ds_get_info *info;

    ENTER();

    info = (mlan_ds_get_info *) pioctl_req->pbuf;

    if (pioctl_req->action == MLAN_ACT_SET) {
        memcpy(pmpriv->wmm.packets_out, info->param.debug_info.packets_out,
               sizeof(pmpriv->wmm.packets_out));
        pmadapter->max_tx_buf_size = info->param.debug_info.max_tx_buf_size;
        pmadapter->tx_buf_size = info->param.debug_info.tx_buf_size;
        pmadapter->ps_mode = info->param.debug_info.ps_mode;
        pmadapter->ps_state = info->param.debug_info.ps_state;
        pmadapter->pm_wakeup_card_req =
            info->param.debug_info.pm_wakeup_card_req;
        pmadapter->pm_wakeup_fw_try = info->param.debug_info.pm_wakeup_fw_try;
        pmadapter->is_hs_configured = info->param.debug_info.is_hs_configured;
        pmadapter->hs_activated = info->param.debug_info.hs_activated;

        pmadapter->dbg.num_cmd_host_to_card_failure =
            info->param.debug_info.num_cmd_host_to_card_failure;
        pmadapter->dbg.num_cmd_sleep_cfm_host_to_card_failure =
            info->param.debug_info.num_cmd_sleep_cfm_host_to_card_failure;
        pmadapter->dbg.num_tx_host_to_card_failure =
            info->param.debug_info.num_tx_host_to_card_failure;
        pmadapter->dbg.num_event_deauth =
            info->param.debug_info.num_event_deauth;
        pmadapter->dbg.num_event_disassoc =
            info->param.debug_info.num_event_disassoc;
        pmadapter->dbg.num_event_link_lost =
            info->param.debug_info.num_event_link_lost;
        pmadapter->dbg.num_cmd_deauth = info->param.debug_info.num_cmd_deauth;
        pmadapter->dbg.num_cmd_assoc_success =
            info->param.debug_info.num_cmd_assoc_success;
        pmadapter->dbg.num_cmd_assoc_failure =
            info->param.debug_info.num_cmd_assoc_failure;
        pmadapter->dbg.num_tx_timeout = info->param.debug_info.num_tx_timeout;
        pmadapter->dbg.num_cmd_timeout = info->param.debug_info.num_cmd_timeout;
        pmadapter->dbg.timeout_cmd_id = info->param.debug_info.timeout_cmd_id;
        pmadapter->dbg.timeout_cmd_act = info->param.debug_info.timeout_cmd_act;
        memcpy(pmadapter->dbg.last_cmd_id, info->param.debug_info.last_cmd_id,
               sizeof(pmadapter->dbg.last_cmd_id));
        memcpy(pmadapter->dbg.last_cmd_act, info->param.debug_info.last_cmd_act,
               sizeof(pmadapter->dbg.last_cmd_act));
        pmadapter->dbg.last_cmd_index = info->param.debug_info.last_cmd_index;
        memcpy(pmadapter->dbg.last_cmd_resp_id,
               info->param.debug_info.last_cmd_resp_id,
               sizeof(pmadapter->dbg.last_cmd_resp_id));
        pmadapter->dbg.last_cmd_resp_index =
            info->param.debug_info.last_cmd_resp_index;
        memcpy(pmadapter->dbg.last_event, info->param.debug_info.last_event,
               sizeof(pmadapter->dbg.last_event));
        pmadapter->dbg.last_event_index =
            info->param.debug_info.last_event_index;

        pmadapter->data_sent = info->param.debug_info.data_sent;
        pmadapter->cmd_sent = info->param.debug_info.cmd_sent;
        pmadapter->cmd_resp_received = info->param.debug_info.cmd_resp_received;
    } else {                    /* MLAN_ACT_GET */
        memcpy(info->param.debug_info.packets_out, pmpriv->wmm.packets_out,
               sizeof(pmpriv->wmm.packets_out));
        info->param.debug_info.max_tx_buf_size = pmadapter->max_tx_buf_size;
        info->param.debug_info.tx_buf_size = pmadapter->tx_buf_size;
        info->param.debug_info.rx_tbl_num =
            wlan_get_rxreorder_tbl(pmpriv, info->param.debug_info.rx_tbl);
        info->param.debug_info.tx_tbl_num =
            wlan_get_txbastream_tbl(pmpriv, info->param.debug_info.tx_tbl);
        info->param.debug_info.ps_mode = pmadapter->ps_mode;
        info->param.debug_info.ps_state = pmadapter->ps_state;
        info->param.debug_info.pm_wakeup_card_req =
            pmadapter->pm_wakeup_card_req;
        info->param.debug_info.pm_wakeup_fw_try = pmadapter->pm_wakeup_fw_try;
        info->param.debug_info.is_hs_configured = pmadapter->is_hs_configured;
        info->param.debug_info.hs_activated = pmadapter->hs_activated;

        info->param.debug_info.num_cmd_host_to_card_failure
            = pmadapter->dbg.num_cmd_host_to_card_failure;
        info->param.debug_info.num_cmd_sleep_cfm_host_to_card_failure
            = pmadapter->dbg.num_cmd_sleep_cfm_host_to_card_failure;
        info->param.debug_info.num_tx_host_to_card_failure
            = pmadapter->dbg.num_tx_host_to_card_failure;
        info->param.debug_info.num_event_deauth =
            pmadapter->dbg.num_event_deauth;
        info->param.debug_info.num_event_disassoc =
            pmadapter->dbg.num_event_disassoc;
        info->param.debug_info.num_event_link_lost =
            pmadapter->dbg.num_event_link_lost;
        info->param.debug_info.num_cmd_deauth = pmadapter->dbg.num_cmd_deauth;
        info->param.debug_info.num_cmd_assoc_success =
            pmadapter->dbg.num_cmd_assoc_success;
        info->param.debug_info.num_cmd_assoc_failure =
            pmadapter->dbg.num_cmd_assoc_failure;
        info->param.debug_info.num_tx_timeout = pmadapter->dbg.num_tx_timeout;
        info->param.debug_info.num_cmd_timeout = pmadapter->dbg.num_cmd_timeout;
        info->param.debug_info.timeout_cmd_id = pmadapter->dbg.timeout_cmd_id;
        info->param.debug_info.timeout_cmd_act = pmadapter->dbg.timeout_cmd_act;
        memcpy(info->param.debug_info.last_cmd_id, pmadapter->dbg.last_cmd_id,
               sizeof(pmadapter->dbg.last_cmd_id));
        memcpy(info->param.debug_info.last_cmd_act, pmadapter->dbg.last_cmd_act,
               sizeof(pmadapter->dbg.last_cmd_act));
        info->param.debug_info.last_cmd_index = pmadapter->dbg.last_cmd_index;
        memcpy(info->param.debug_info.last_cmd_resp_id,
               pmadapter->dbg.last_cmd_resp_id,
               sizeof(pmadapter->dbg.last_cmd_resp_id));
        info->param.debug_info.last_cmd_resp_index =
            pmadapter->dbg.last_cmd_resp_index;
        memcpy(info->param.debug_info.last_event, pmadapter->dbg.last_event,
               sizeof(pmadapter->dbg.last_event));
        info->param.debug_info.last_event_index =
            pmadapter->dbg.last_event_index;

        info->param.debug_info.data_sent = pmadapter->data_sent;
        info->param.debug_info.cmd_sent = pmadapter->cmd_sent;
        info->param.debug_info.cmd_resp_received = pmadapter->cmd_resp_received;
    }

    pioctl_req->data_read_written =
        sizeof(mlan_debug_info) + MLAN_SUB_COMMAND_SIZE;

    LEAVE();
    return ret;
}

/** 
 *  @brief Get extended version information
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_get_info_ver_ext(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    pmlan_private pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_ds_get_info *info;

    ENTER();

    if (!wlan_is_cmd_allowed(pmpriv)) {
        PRINTM(MSG, "VERSION_EXT: not allowed\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    info = (mlan_ds_get_info *) pioctl_req->pbuf;

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv,
                           HostCmd_CMD_VERSION_EXT,
                           HostCmd_ACT_GEN_GET,
                           0,
                           (t_void *) pioctl_req,
                           &info->param.ver_ext.version_str_sel);

    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;

  exit:
    LEAVE();
    return ret;
}

/** 
 *  @brief Get information handler
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_get_info_ioctl(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status status = MLAN_STATUS_SUCCESS;
    mlan_ds_get_info *pget_info = MNULL;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];

    ENTER();

    pget_info = (mlan_ds_get_info *) pioctl_req->pbuf;

    switch (pget_info->sub_command) {
    case MLAN_OID_GET_STATS:
        status = wlan_get_info_stats(pmadapter, pioctl_req);
        break;
    case MLAN_OID_GET_SIGNAL:
        status = wlan_get_info_signal(pmadapter, pioctl_req);
        break;
    case MLAN_OID_GET_FW_INFO:
        pioctl_req->data_read_written =
            sizeof(mlan_fw_info) + MLAN_SUB_COMMAND_SIZE;
        pget_info->param.fw_info.fw_ver = pmadapter->fw_release_number;
        memcpy(&pget_info->param.fw_info.mac_addr, pmpriv->curr_addr,
               MLAN_MAC_ADDR_LENGTH);
        break;
    case MLAN_OID_GET_BSS_INFO:
        status = wlan_get_info_bss_info(pmadapter, pioctl_req);
        break;
    case MLAN_OID_GET_DEBUG_INFO:
        status = wlan_get_info_debug_info(pmadapter, pioctl_req);
        break;
    case MLAN_OID_GET_VER_EXT:
        status = wlan_get_info_ver_ext(pmadapter, pioctl_req);
        break;
    default:
        status = MLAN_STATUS_FAILURE;
        break;
    }

    LEAVE();
    return status;
}

/** 
 *  @brief Set/Get SNMP MIB handler
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_snmp_mib_ioctl(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    pmlan_private pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_status ret = MLAN_STATUS_SUCCESS;
    t_u16 cmd_action = 0;
    t_u16 cmd_oid = 0;
    mlan_ds_snmp_mib *mib = MNULL;
    t_u32 value = 0;

    ENTER();

    if (!wlan_is_cmd_allowed(pmpriv)) {
        PRINTM(MSG, "SNMP_MIB: not allowed\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    if (pioctl_req->buf_len < sizeof(mlan_ds_snmp_mib)) {
        PRINTM(WARN, "MLAN IOCTL information buffer length is too short.\n");
        pioctl_req->data_read_written = 0;
        pioctl_req->buf_len_needed = sizeof(mlan_ds_snmp_mib);
        ret = MLAN_STATUS_RESOURCE;
        goto exit;
    }

    mib = (mlan_ds_snmp_mib *) pioctl_req->pbuf;
    if (pioctl_req->action == MLAN_ACT_SET)
        cmd_action = HostCmd_ACT_GEN_SET;
    else
        cmd_action = HostCmd_ACT_GEN_GET;

    switch (mib->sub_command) {
    case MLAN_OID_SNMP_MIB_RTS_THRESHOLD:
        value = mib->param.rts_threshold;
        cmd_oid = RtsThresh_i;
        break;
    case MLAN_OID_SNMP_MIB_FRAG_THRESHOLD:
        value = mib->param.frag_threshold;
        cmd_oid = FragThresh_i;
        break;
    case MLAN_OID_SNMP_MIB_RETRY_COUNT:
        value = mib->param.retry_count;
        cmd_oid = ShortRetryLim_i;
        break;
    }

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv,
                           HostCmd_CMD_802_11_SNMP_MIB,
                           cmd_action, cmd_oid, (t_void *) pioctl_req, &value);

    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;

  exit:
    LEAVE();
    return ret;
}

/** 
 *  @brief Set/Get radio status
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_radio_ioctl_radio_ctl(IN pmlan_adapter pmadapter,
                           IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_ds_radio_cfg *radio_cfg = MNULL;
    t_u16 cmd_action = 0;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];

    ENTER();

    if (!wlan_is_cmd_allowed(pmpriv)) {
        PRINTM(MSG, "RADIO_CONTROL: not allowed\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    radio_cfg = (mlan_ds_radio_cfg *) pioctl_req->pbuf;
    if (pioctl_req->action == MLAN_ACT_SET) {
        if (pmadapter->radio_on == radio_cfg->param.radio_on_off) {
            ret = MLAN_STATUS_SUCCESS;
            goto exit;
        } else {
            if (pmpriv->media_connected == MTRUE) {
                ret = MLAN_STATUS_FAILURE;
                goto exit;
            }
            cmd_action = HostCmd_ACT_GEN_SET;
            pmadapter->radio_on = radio_cfg->param.radio_on_off;
        }
    } else
        cmd_action = HostCmd_ACT_GEN_GET;

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv,
                           HostCmd_CMD_802_11_RADIO_CONTROL,
                           cmd_action, 0, (t_void *) pioctl_req, MNULL);

    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;

  exit:
    LEAVE();
    return ret;
}

/** 
 *  @brief Set/Get Infra/Ad-hoc band configuration
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_radio_ioctl_band_cfg(IN pmlan_adapter pmadapter,
                          IN pmlan_ioctl_req pioctl_req)
{
    t_u32 infra_band = 0;
    t_u32 adhoc_band = 0;
    t_u32 adhoc_channel = 0;
    mlan_ds_radio_cfg *radio_cfg = MNULL;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];

    ENTER();

    radio_cfg = (mlan_ds_radio_cfg *) pioctl_req->pbuf;
    if (pioctl_req->action == MLAN_ACT_SET) {
        infra_band = radio_cfg->param.band_cfg.config_bands;
        adhoc_band = radio_cfg->param.band_cfg.adhoc_start_band;
        adhoc_channel = radio_cfg->param.band_cfg.adhoc_channel;

        /* SET Infra band */
        if ((infra_band | pmadapter->fw_bands) & ~pmadapter->fw_bands) {
            LEAVE();
            return MLAN_STATUS_FAILURE;
        }

        if (wlan_set_regiontable(pmpriv, pmadapter->region_code, infra_band)) {
            LEAVE();
            return MLAN_STATUS_FAILURE;
        }

        if (wlan_set_universal_table(pmpriv, infra_band)) {
            LEAVE();
            return MLAN_STATUS_FAILURE;
        }
        pmadapter->config_bands = infra_band;

        /* SET Ad-hoc Band */
        if ((adhoc_band | pmadapter->fw_bands) & ~pmadapter->fw_bands) {
            LEAVE();
            return MLAN_STATUS_FAILURE;
        }

        if (adhoc_band)
            pmadapter->adhoc_start_band = adhoc_band;
        /* 
         * If no adhoc_channel is supplied verify if the existing adhoc channel
         * compiles with new adhoc_band
         */
        if (!adhoc_channel) {
            if (!wlan_find_cfp_by_band_and_channel
                (pmadapter, adhoc_band, pmpriv->adhoc_channel)) {
                /* Pass back the default channel */
                radio_cfg->param.band_cfg.adhoc_channel =
                    DEFAULT_AD_HOC_CHANNEL;
            }
        } else {                /* Retrurn error if adhoc_band and
                                   adhoc_channel combination is invalid */
            if (!wlan_find_cfp_by_band_and_channel
                (pmadapter, adhoc_band, adhoc_channel)) {
                LEAVE();
                return MLAN_STATUS_FAILURE;
            }
            pmpriv->adhoc_channel = adhoc_channel;
        }
    } else {
        radio_cfg->param.band_cfg.config_bands = pmadapter->config_bands;       /* Infra 
                                                                                   Bands 
                                                                                 */
        radio_cfg->param.band_cfg.adhoc_start_band = pmadapter->adhoc_start_band;       /* Adhoc 
                                                                                           Band 
                                                                                         */
        radio_cfg->param.band_cfg.adhoc_channel = pmpriv->adhoc_channel;        /* Adhoc 
                                                                                   Channel 
                                                                                 */
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Set/Get antenna configuration
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_PENDING --success, otherwise fail
 */
static mlan_status
wlan_radio_ioctl_ant_cfg(IN pmlan_adapter pmadapter,
                         IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_ds_radio_cfg *radio_cfg = MNULL;
    t_u16 cmd_action = 0;
    t_u16 antenna_mode;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];

    ENTER();

    if (!wlan_is_cmd_allowed(pmpriv)) {
        PRINTM(MSG, "RF_ANTENNA: not allowed\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    radio_cfg = (mlan_ds_radio_cfg *) pioctl_req->pbuf;
    if (pioctl_req->action == MLAN_ACT_SET) {
        cmd_action = HostCmd_ACT_GEN_SET;
    } else
        cmd_action = HostCmd_ACT_GEN_GET;

    /* Cast it to t_u16, antenna mode for command HostCmd_CMD_802_11_RF_ANTENNA 
       requires 2 bytes */
    antenna_mode = (t_u16) radio_cfg->param.antenna;

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv,
                           HostCmd_CMD_802_11_RF_ANTENNA,
                           cmd_action,
                           0, (t_void *) pioctl_req, (t_void *) & antenna_mode);

    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;

  exit:
    LEAVE();
    return ret;
}

/** 
 *  @brief Radio command handler
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_radio_ioctl(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status status = MLAN_STATUS_SUCCESS;
    mlan_ds_radio_cfg *radio_cfg = MNULL;

    ENTER();

    if (pioctl_req->buf_len < sizeof(mlan_ds_radio_cfg)) {
        PRINTM(WARN, "MLAN IOCTL information buffer length is too short.\n");
        pioctl_req->data_read_written = 0;
        pioctl_req->buf_len_needed = sizeof(mlan_ds_radio_cfg);
        LEAVE();
        return MLAN_STATUS_RESOURCE;
    }
    radio_cfg = (mlan_ds_radio_cfg *) pioctl_req->pbuf;
    switch (radio_cfg->sub_command) {
    case MLAN_OID_RADIO_CTRL:
        status = wlan_radio_ioctl_radio_ctl(pmadapter, pioctl_req);
        break;
    case MLAN_OID_BAND_CFG:
        status = wlan_radio_ioctl_band_cfg(pmadapter, pioctl_req);
        break;
    case MLAN_OID_ANT_CFG:
        status = wlan_radio_ioctl_ant_cfg(pmadapter, pioctl_req);
        break;
    default:
        status = MLAN_STATUS_FAILURE;
        break;
    }

    LEAVE();
    return status;
}

/** 
 *  @brief Set/Get MAC address
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_bss_ioctl_mac_address(IN pmlan_adapter pmadapter,
                           IN pmlan_ioctl_req pioctl_req)
{
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_ds_bss *bss = MNULL;
    mlan_status ret = MLAN_STATUS_SUCCESS;

    ENTER();

    bss = (mlan_ds_bss *) pioctl_req->pbuf;
    if (pioctl_req->action == MLAN_ACT_GET) {
        pioctl_req->data_read_written =
            MLAN_MAC_ADDR_LENGTH + MLAN_SUB_COMMAND_SIZE;
        memcpy(&bss->param.mac_addr, pmpriv->curr_addr, MLAN_MAC_ADDR_LENGTH);
        ret = MLAN_STATUS_SUCCESS;
        goto exit;
    }

    if (!wlan_is_cmd_allowed(pmpriv)) {
        PRINTM(MSG, "MAC_ADDRESS: not allowed\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    memcpy(pmpriv->curr_addr, &bss->param.mac_addr, MLAN_MAC_ADDR_LENGTH);

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv,
                           HostCmd_CMD_802_11_MAC_ADDRESS,
                           HostCmd_ACT_GEN_SET,
                           0, (t_void *) pioctl_req, MNULL);
    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;

  exit:
    LEAVE();
    return ret;
}

/** 
 *  @brief Set multicast list
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_bss_ioctl_set_multicast_list(IN pmlan_adapter pmadapter,
                                  IN pmlan_ioctl_req pioctl_req)
{
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_ds_bss *bss = MNULL;
    mlan_status ret = MLAN_STATUS_SUCCESS;
    t_u16 old_pkt_filter;

    ENTER();

    if (!wlan_is_cmd_allowed(pmpriv)) {
        PRINTM(MSG, "MULTICAST_ADDR: not allowed\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    old_pkt_filter = pmpriv->curr_pkt_filter;
    bss = (mlan_ds_bss *) pioctl_req->pbuf;
    if (pioctl_req->action == MLAN_ACT_GET) {
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }
    pioctl_req->data_read_written =
        sizeof(mlan_multicast_list) + MLAN_SUB_COMMAND_SIZE;
    if (bss->param.multicast_list.mode == MLAN_PROMISC_MODE) {
        PRINTM(INFO, "Enable Promiscuous mode\n");
        pmpriv->curr_pkt_filter |= HostCmd_ACT_MAC_PROMISCUOUS_ENABLE;
        pmpriv->curr_pkt_filter &= ~HostCmd_ACT_MAC_ALL_MULTICAST_ENABLE;
    } else {
        /* Multicast */
        pmpriv->curr_pkt_filter &= ~HostCmd_ACT_MAC_PROMISCUOUS_ENABLE;
        if (bss->param.multicast_list.mode == MLAN_MULTICAST_MODE) {
            PRINTM(INFO, "Enabling All Multicast!\n");
            pmpriv->curr_pkt_filter |= HostCmd_ACT_MAC_ALL_MULTICAST_ENABLE;
        } else {
            pmpriv->curr_pkt_filter &= ~HostCmd_ACT_MAC_ALL_MULTICAST_ENABLE;
            if (bss->param.multicast_list.num_multicast_addr) {
                PRINTM(INFO, "Set multicast list=%d\n",
                       bss->param.multicast_list.num_multicast_addr);
                /* Set multicast addresses to firmware */
                if (old_pkt_filter == pmpriv->curr_pkt_filter) {
                    /* Send request to firmware */
                    ret =
                        wlan_prepare_cmd(pmpriv, HostCmd_CMD_MAC_MULTICAST_ADR,
                                         HostCmd_ACT_GEN_SET, 0,
                                         (t_void *) pioctl_req,
                                         &bss->param.multicast_list);
                    if (ret == MLAN_STATUS_SUCCESS)
                        ret = MLAN_STATUS_PENDING;
                } else {
                    /* Send request to firmware */
                    ret =
                        wlan_prepare_cmd(pmpriv, HostCmd_CMD_MAC_MULTICAST_ADR,
                                         HostCmd_ACT_GEN_SET, 0, MNULL,
                                         &bss->param.multicast_list);
                }
            }
        }
    }
    PRINTM(INFO, "old_pkt_filter=0x%lx, curr_pkt_filter=0x%lx\n",
           old_pkt_filter, pmpriv->curr_pkt_filter);
    if (old_pkt_filter != pmpriv->curr_pkt_filter) {
        ret = wlan_prepare_cmd(pmpriv,
                               HostCmd_CMD_MAC_CONTROL,
                               HostCmd_ACT_GEN_SET,
                               0,
                               (t_void *) pioctl_req, &pmpriv->curr_pkt_filter);
        if (ret == MLAN_STATUS_SUCCESS)
            ret = MLAN_STATUS_PENDING;
    }

  exit:
    LEAVE();
    return ret;
}

/** 
 *  @brief Get channel list
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_bss_ioctl_get_channel_list(IN pmlan_adapter pmadapter,
                                IN pmlan_ioctl_req pioctl_req)
{
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_ds_bss *bss = MNULL;
    mlan_status ret = MLAN_STATUS_SUCCESS;
    chan_freq_power_t *cfp;
    int i, j;

    ENTER();

    bss = (mlan_ds_bss *) pioctl_req->pbuf;
    if (pioctl_req->action != MLAN_ACT_GET) {
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }
    if ((wlan_11d_get_state(pmpriv) == ENABLE_11D &&
         pmpriv->media_connected == MTRUE) &&
        ((pmpriv->bss_mode == MLAN_BSS_MODE_INFRA) ||
         (pmpriv->bss_mode == MLAN_BSS_MODE_IBSS &&
          pmpriv->adhoc_state != ADHOC_STARTED))
        ) {
        t_u8 chan_no;
        t_u8 band;

        parsed_region_chan_11d_t *parsed_region_chan = MNULL;
        parsed_region_chan_11d_t region_chan;

        BSSDescriptor_t *pbss_desc = &pmpriv->curr_bss_params.bss_descriptor;

        /* If country IE is present in the associated AP then return the
           channel list from country IE else return it from the learning table */

        if (wlan_11d_parse_domain_info(pmadapter, &pbss_desc->country_info,
                                       pbss_desc->bss_band,
                                       &region_chan) == MLAN_STATUS_SUCCESS) {

            parsed_region_chan = &region_chan;
        } else {
            parsed_region_chan = &pmadapter->parsed_region_chan;
        }

        band = parsed_region_chan->band;
        PRINTM(INFO, "band=%d no_of_chan=%d\n", band,
               parsed_region_chan->no_of_chan);

        for (i = 0; (bss->param.chanlist.num_of_chan < MLAN_MAX_CHANNEL_NUM)
             && (i < parsed_region_chan->no_of_chan); i++) {
            chan_no = parsed_region_chan->chan_pwr[i].chan;
            PRINTM(INFO, "chan_no=%d\n", chan_no);
            bss->param.chanlist.cf[bss->param.chanlist.num_of_chan].channel =
                (t_u32) chan_no;
            bss->param.chanlist.cf[bss->param.chanlist.num_of_chan].freq =
                (t_u32) wlan_11d_chan_2_freq(pmadapter, chan_no, band);
            bss->param.chanlist.num_of_chan++;
        }
    } else {
        for (j = 0; (bss->param.chanlist.num_of_chan < MLAN_MAX_CHANNEL_NUM)
             && (j <
                 sizeof(pmadapter->region_channel) /
                 sizeof(pmadapter->region_channel[0])); j++) {
            cfp = pmadapter->region_channel[j].pcfp;
            for (i = 0; (bss->param.chanlist.num_of_chan < MLAN_MAX_CHANNEL_NUM)
                 && pmadapter->region_channel[j].valid
                 && cfp && (i < pmadapter->region_channel[j].num_cfp); i++) {
                bss->param.chanlist.cf[bss->param.chanlist.num_of_chan].
                    channel = (t_u32) cfp->channel;
                bss->param.chanlist.cf[bss->param.chanlist.num_of_chan].freq =
                    (t_u32) cfp->freq;
                bss->param.chanlist.num_of_chan++;
                cfp++;
            }
        }
    }

    PRINTM(INFO, "num of channel=%d\n", bss->param.chanlist.num_of_chan);

    LEAVE();
    return ret;
}

/** 
 *  @brief Set/Get BSS channel
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_bss_ioctl_channel(IN pmlan_adapter pmadapter,
                       IN pmlan_ioctl_req pioctl_req)
{
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_ds_bss *bss = MNULL;
    mlan_status ret = MLAN_STATUS_SUCCESS;
    chan_freq_power_t *cfp;
    ENTER();
    bss = (mlan_ds_bss *) pioctl_req->pbuf;
    if (pioctl_req->action == MLAN_ACT_GET) {
        cfp =
            wlan_find_cfp_by_band_and_channel(pmadapter,
                                              pmpriv->curr_bss_params.band,
                                              (t_u16) pmpriv->curr_bss_params.
                                              bss_descriptor.channel);
        if (cfp) {
            bss->param.bss_chan.channel = cfp->channel;
            bss->param.bss_chan.freq = cfp->freq;
        } else
            ret = MLAN_STATUS_FAILURE;
        pioctl_req->data_read_written =
            sizeof(chan_freq) + MLAN_SUB_COMMAND_SIZE;
        LEAVE();
        return ret;
    }
    if (!bss->param.bss_chan.channel && !bss->param.bss_chan.freq) {
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }
    if (bss->param.bss_chan.channel) {
        cfp =
            wlan_find_cfp_by_band_and_channel(pmadapter, 0,
                                              (t_u16) bss->param.bss_chan.
                                              channel);
    } else {
        cfp =
            wlan_find_cfp_by_band_and_freq(pmadapter, 0,
                                           bss->param.bss_chan.freq);
    }
    if (!cfp || !cfp->channel) {
        PRINTM(ERROR, "Invalid channel/freq\n");
        LEAVE();
        return MLAN_STATUS_FAILURE;

    }
    if (pmpriv->bss_mode != MLAN_BSS_MODE_IBSS) {
        pmpriv->adhoc_channel = cfp->channel;
        LEAVE();
        return ret;
    }
    bss->param.bss_chan.channel = cfp->channel;
    bss->param.bss_chan.freq = cfp->freq;
    LEAVE();
    return ret;
}

/** 
 *  @brief Set/Get BSS mode
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_bss_ioctl_mode(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_ds_bss *bss = MNULL;
    mlan_status ret = MLAN_STATUS_SUCCESS;
    bss = (mlan_ds_bss *) pioctl_req->pbuf;

    ENTER();

    if (pioctl_req->action == MLAN_ACT_GET) {
        bss->param.bss_mode = pmpriv->bss_mode;
        pioctl_req->data_read_written = sizeof(t_u32) + MLAN_SUB_COMMAND_SIZE;
        goto exit;
    }

    if ((pmpriv->bss_mode == bss->param.bss_mode) ||
        (bss->param.bss_mode == MLAN_BSS_MODE_AUTO)) {
        PRINTM(INFO, "Already set to required mode! No change!\n");
        pmpriv->bss_mode = bss->param.bss_mode;
        goto exit;
    }

    if (!wlan_is_cmd_allowed(pmpriv)) {
        PRINTM(MSG, "BSS_MODE: not allowed\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    if (pmpriv->bss_mode == MLAN_BSS_MODE_INFRA) {
        if (pmadapter->ps_state != PS_STATE_FULL_POWER) {
            wlan_exit_ps(pmpriv);
        }
        pmadapter->ps_mode = Wlan802_11PowerModeCAM;
    }

    ret = wlan_disconnect(pmpriv, pioctl_req, MNULL);

    pmpriv->sec_info.authentication_mode = MLAN_AUTH_MODE_OPEN;
    pmpriv->bss_mode = bss->param.bss_mode;

  exit:
    LEAVE();
    return ret;
}

/** 
 *  @brief Start BSS
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_bss_ioctl_start(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_ds_bss *bss = (mlan_ds_bss *) pioctl_req->pbuf;
    t_s32 i = -1;

    ENTER();

    if (!wlan_is_cmd_allowed(pmpriv)) {
        PRINTM(MSG, "BSS_START: not allowed\n");
        ret = MLAN_STATUS_FAILURE;
        goto start_ssid_done;
    }

    if (pmpriv->bss_mode == MLAN_BSS_MODE_INFRA) {
        /* Infra mode */

        /* Search for the requested SSID in the scan table */
        if (bss->param.ssid_bssid.ssid.ssid_len) {
            i = wlan_find_ssid_in_list(pmpriv,
                                       &bss->param.ssid_bssid.ssid,
                                       MNULL, MLAN_BSS_MODE_INFRA);
        } else {
            i = wlan_find_bssid_in_list(pmpriv,
                                        (t_u8 *) & bss->param.ssid_bssid.bssid,
                                        MLAN_BSS_MODE_INFRA);
        }

        if (i >= 0) {
            PRINTM(INFO, "SSID found in scan list ... associating...\n");

            /* Clear any past association response stored for application
               retrieval */
            pmpriv->assoc_rsp_size = 0;
            ret =
                wlan_associate(pmpriv, pioctl_req, &pmadapter->pscan_table[i]);

            if (ret)
                goto start_ssid_done;
        } else {                /* i >= 0 */
            ret = MLAN_STATUS_FAILURE;
            goto start_ssid_done;
        }
    } else {
        /* Adhoc mode */

        /* If the requested SSID matches current SSID, return */
        if (bss->param.ssid_bssid.ssid.ssid_len &&
            (!wlan_ssid_cmp
             (pmadapter, &pmpriv->curr_bss_params.bss_descriptor.ssid,
              &bss->param.ssid_bssid.ssid))) {
            ret = MLAN_STATUS_SUCCESS;
            goto start_ssid_done;
        }

        /* Exit Adhoc mode first */
        PRINTM(INFO, "Sending Adhoc Stop\n");
        ret = wlan_disconnect(pmpriv, MNULL, MNULL);
        if (ret)
            goto start_ssid_done;

        pmpriv->adhoc_is_link_sensed = MFALSE;

        /* Search for the requested network in the scan table */
        if (bss->param.ssid_bssid.ssid.ssid_len) {
            i = wlan_find_ssid_in_list(pmpriv,
                                       &bss->param.ssid_bssid.ssid,
                                       MNULL, MLAN_BSS_MODE_IBSS);
        } else {
            i = wlan_find_bssid_in_list(pmpriv,
                                        (t_u8 *) & bss->param.ssid_bssid.bssid,
                                        MLAN_BSS_MODE_IBSS);
        }

        if (i >= 0) {
            PRINTM(INFO, "Network found in scan list ... joining ...\n");
            if (pmpriv->adhoc_aes_enabled)
                wlan_enable_aes_key(pmpriv);
            ret =
                wlan_adhoc_join(pmpriv, pioctl_req, &pmadapter->pscan_table[i]);
            if (ret)
                goto start_ssid_done;
        } else {                /* i >= 0 */
            PRINTM(INFO, "Network not found in the list, "
                   "creating adhoc with ssid = %s\n",
                   bss->param.ssid_bssid.ssid.ssid);
            if (pmpriv->adhoc_aes_enabled)
                wlan_enable_aes_key(pmpriv);
            ret =
                wlan_adhoc_start(pmpriv, pioctl_req,
                                 &bss->param.ssid_bssid.ssid);
            if (ret)
                goto start_ssid_done;
        }
    }

    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;

  start_ssid_done:
    LEAVE();
    return ret;
}

/** 
 *  @brief Stop BSS
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_bss_ioctl_stop(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    pmlan_private pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_ds_bss *bss = (mlan_ds_bss *) pioctl_req->pbuf;

    ENTER();

    if (!wlan_is_cmd_allowed(pmpriv)) {
        PRINTM(MSG, "BSS_STOP: not allowed\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    ret = wlan_disconnect(pmpriv, pioctl_req, &bss->param.bssid);

  exit:
    LEAVE();
    return ret;
}

/** 
 *  @brief Set/Get IBSS channel
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_bss_ioctl_ibss_channel(IN pmlan_adapter pmadapter,
                            IN pmlan_ioctl_req pioctl_req)
{
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_ds_bss *bss = MNULL;
    mlan_status ret = MLAN_STATUS_SUCCESS;
    t_u16 cmd_action;

    ENTER();

    if (!wlan_is_cmd_allowed(pmpriv)) {
        PRINTM(MSG, "RF_CHANNEL: not allowed\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    bss = (mlan_ds_bss *) pioctl_req->pbuf;
    if (pioctl_req->action == MLAN_ACT_GET) {
        if (pmpriv->media_connected == MFALSE) {
            bss->param.bss_chan.channel = pmpriv->adhoc_channel;
            goto exit;
        }
        cmd_action = HostCmd_ACT_GEN_GET;
    } else {
        cmd_action = HostCmd_ACT_GEN_SET;
        pmpriv->adhoc_channel = bss->param.bss_chan.channel;
    }

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv,
                           HostCmd_CMD_802_11_RF_CHANNEL,
                           cmd_action,
                           0,
                           (t_void *) pioctl_req, &bss->param.bss_chan.channel);
    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;

  exit:
    LEAVE();
    return ret;
}

/** 
 *  @brief Set/Get beacon interval
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_bss_ioctl_beacon_interval(IN pmlan_adapter pmadapter,
                               IN pmlan_ioctl_req pioctl_req)
{
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_ds_bss *bss = MNULL;
    mlan_status ret = MLAN_STATUS_SUCCESS;
    ENTER();
    bss = (mlan_ds_bss *) pioctl_req->pbuf;
    if (pioctl_req->action == MLAN_ACT_GET) {
        bss->param.bcn_interval = pmpriv->beacon_period;
        if (pmpriv->media_connected == MTRUE)
            bss->param.bcn_interval =
                pmpriv->curr_bss_params.bss_descriptor.beacon_period;
    } else
        pmpriv->beacon_period = bss->param.bcn_interval;
    pioctl_req->data_read_written = sizeof(t_u32) + MLAN_SUB_COMMAND_SIZE;
    LEAVE();
    return ret;
}

/** 
 *  @brief Set/Get ATIM window
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_bss_ioctl_atim_window(IN pmlan_adapter pmadapter,
                           IN pmlan_ioctl_req pioctl_req)
{
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_ds_bss *bss = MNULL;
    mlan_status ret = MLAN_STATUS_SUCCESS;
    ENTER();
    bss = (mlan_ds_bss *) pioctl_req->pbuf;
    if (pioctl_req->action == MLAN_ACT_GET) {
        bss->param.atim_window = pmpriv->atim_window;
        if (pmpriv->media_connected == MTRUE)
            bss->param.atim_window =
                pmpriv->curr_bss_params.bss_descriptor.atim_window;
    } else
        pmpriv->atim_window = bss->param.atim_window;

    pioctl_req->data_read_written = sizeof(t_u32) + MLAN_SUB_COMMAND_SIZE;
    LEAVE();
    return ret;
}

/** 
 *  @brief Search for a BSS
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_bss_ioctl_find_bss(IN pmlan_adapter pmadapter,
                        IN pmlan_ioctl_req pioctl_req)
{
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_ds_bss *bss = MNULL;
    mlan_status ret = MLAN_STATUS_SUCCESS;
    t_u8 zero_mac[] = { 0, 0, 0, 0, 0, 0 };
    t_u8 mac[MLAN_MAC_ADDR_LENGTH];
    int i = 0;
    BSSDescriptor_t *pbss_desc;

    ENTER();

    bss = (mlan_ds_bss *) pioctl_req->pbuf;
    if (memcmp(&bss->param.ssid_bssid.bssid, zero_mac, sizeof(zero_mac))) {
        i = wlan_find_bssid_in_list(pmpriv,
                                    (t_u8 *) & bss->param.ssid_bssid.bssid,
                                    pmpriv->bss_mode);
        if (i < 0) {
            memcpy(mac, &bss->param.ssid_bssid.bssid, sizeof(mac));
            PRINTM(ERROR, "Can not find bssid %02x:%02x:%02x:%02x:%02x:%02x\n",
                   mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
            LEAVE();
            return MLAN_STATUS_FAILURE;
        }
        pbss_desc = &pmadapter->pscan_table[i];
        memcpy(&bss->param.ssid_bssid.ssid, &pbss_desc->ssid,
               sizeof(mlan_802_11_ssid));
    } else if (bss->param.ssid_bssid.ssid.ssid_len) {
        i = wlan_find_ssid_in_list(pmpriv, &bss->param.ssid_bssid.ssid, MNULL,
                                   pmpriv->bss_mode);
        if (i < 0) {
            PRINTM(ERROR, "Can not find ssid %s\n",
                   bss->param.ssid_bssid.ssid.ssid);
            LEAVE();
            return MLAN_STATUS_FAILURE;
        }
        pbss_desc = &pmadapter->pscan_table[i];
        memcpy((t_u8 *) & bss->param.ssid_bssid.bssid,
               (t_u8 *) & pbss_desc->mac_address, MLAN_MAC_ADDR_LENGTH);
    } else {
        ret = wlan_find_best_network(pmpriv, &bss->param.ssid_bssid);
    }

    LEAVE();
    return ret;
}

/** 
 *  @brief BSS command handler
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_bss_ioctl(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status status = MLAN_STATUS_SUCCESS;
    mlan_ds_bss *bss = MNULL;

    ENTER();

    if (pioctl_req->buf_len < sizeof(mlan_ds_bss)) {
        PRINTM(WARN, "MLAN bss IOCTL length is too short.\n");
        pioctl_req->data_read_written = 0;
        pioctl_req->buf_len_needed = sizeof(mlan_ds_bss);
        LEAVE();
        return MLAN_STATUS_RESOURCE;
    }

    bss = (mlan_ds_bss *) pioctl_req->pbuf;

    switch (bss->sub_command) {
    case MLAN_OID_BSS_START:
        status = wlan_bss_ioctl_start(pmadapter, pioctl_req);
        break;
    case MLAN_OID_BSS_STOP:
        status = wlan_bss_ioctl_stop(pmadapter, pioctl_req);
        break;
    case MLAN_OID_BSS_MODE:
        status = wlan_bss_ioctl_mode(pmadapter, pioctl_req);
        break;
    case MLAN_OID_BSS_CHANNEL:
        status = wlan_bss_ioctl_channel(pmadapter, pioctl_req);
        break;
    case MLAN_OID_BSS_CHANNEL_LIST:
        status = wlan_bss_ioctl_get_channel_list(pmadapter, pioctl_req);
        break;
    case MLAN_OID_BSS_MAC_ADDR:
        status = wlan_bss_ioctl_mac_address(pmadapter, pioctl_req);
        break;
    case MLAN_OID_BSS_MULTICAST_LIST:
        status = wlan_bss_ioctl_set_multicast_list(pmadapter, pioctl_req);
        break;
    case MLAN_OID_BSS_FIND_BSS:
        status = wlan_bss_ioctl_find_bss(pmadapter, pioctl_req);
        break;
    case MLAN_OID_IBSS_BCN_INTERVAL:
        status = wlan_bss_ioctl_beacon_interval(pmadapter, pioctl_req);
        break;
    case MLAN_OID_IBSS_ATIM_WINDOW:
        status = wlan_bss_ioctl_atim_window(pmadapter, pioctl_req);
        break;
    case MLAN_OID_IBSS_CHANNEL:
        status = wlan_bss_ioctl_ibss_channel(pmadapter, pioctl_req);
        break;
    default:
        status = MLAN_STATUS_FAILURE;
        break;
    }

    LEAVE();
    return status;
}

/** 
 *  @brief Get supported rates
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_rate_ioctl_get_supported_rate(IN pmlan_adapter pmadapter,
                                   IN pmlan_ioctl_req pioctl_req)
{
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_ds_rate *rate = MNULL;
    mlan_status ret = MLAN_STATUS_SUCCESS;
    ENTER();
    if (pioctl_req->action != MLAN_ACT_GET) {
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }
    rate = (mlan_ds_rate *) pioctl_req->pbuf;
    wlan_get_active_data_rates(pmpriv, rate->param.rates);
    pioctl_req->data_read_written =
        MLAN_SUPPORTED_RATES + MLAN_SUB_COMMAND_SIZE;
    LEAVE();
    return ret;
}

/** 
 *  @brief Get rate value
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_rate_ioctl_get_rate_value(IN pmlan_adapter pmadapter,
                               IN pmlan_ioctl_req pioctl_req)
{
    mlan_ds_rate *rate = MNULL;
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];

    ENTER();

    if (!wlan_is_cmd_allowed(pmpriv)) {
        PRINTM(MSG, "TX_RATE_QUERY: not allowed\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    rate = (mlan_ds_rate *) pioctl_req->pbuf;
    rate->param.rate_cfg.is_rate_auto = pmpriv->is_data_rate_auto;
    pioctl_req->data_read_written =
        sizeof(mlan_rate_cfg_t) + MLAN_SUB_COMMAND_SIZE;

    if (pmpriv->media_connected != MTRUE) {
        switch (pmadapter->config_bands) {
        case BAND_B:
            /* Return the lowest supported rate for B band */
            rate->param.rate_cfg.rate = SupportedRates_B[0] & 0x7f;
            break;
        case BAND_G:
        case BAND_G | BAND_GN:
            /* Return the lowest supported rate for G band */
            rate->param.rate_cfg.rate = SupportedRates_G[0] & 0x7f;
            break;
        case BAND_B | BAND_G:
        case BAND_B | BAND_G | BAND_GN:
            /* Return the lowest supported rate for BG band */
            rate->param.rate_cfg.rate = SupportedRates_BG[0] & 0x7f;
            break;
        case BAND_GN:
            /* Return the lowest supported rate for N band */
            rate->param.rate_cfg.rate = SupportedRates_N[0] & 0x7f;
            break;
        default:
            PRINTM(MSG, "Invalid Band 0x%x\n", pmadapter->config_bands);
            break;
        }
    } else {
        /* Send request to firmware */
        ret = wlan_prepare_cmd(pmpriv,
                               HostCmd_CMD_802_11_TX_RATE_QUERY,
                               HostCmd_ACT_GEN_GET,
                               0, (t_void *) pioctl_req, MNULL);
        if (ret == MLAN_STATUS_SUCCESS)
            ret = MLAN_STATUS_PENDING;
    }

  exit:
    LEAVE();
    return ret;
}

/** 
 *  @brief Set rate value
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_rate_ioctl_set_rate_value(IN pmlan_adapter pmadapter,
                               IN pmlan_ioctl_req pioctl_req)
{
    mlan_ds_rate *ds_rate = MNULL;
    WLAN_802_11_RATES rates;
    t_u8 *rate = MNULL;
    int rateIndex = 0;
    int i = 0;
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    t_u16 bitmap_rates[MAX_BITMAP_RATES_SIZE];

    ENTER();

    if (!wlan_is_cmd_allowed(pmpriv)) {
        PRINTM(MSG, "TX_RATE_CFG: not allowed\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    ds_rate = (mlan_ds_rate *) pioctl_req->pbuf;

    if (ds_rate->param.rate_cfg.is_rate_auto) {
        memset(bitmap_rates, 0, sizeof(bitmap_rates));
        /* Support all HR/DSSS rates */
        bitmap_rates[0] = 0x000F;
        /* Support all OFDM rates */
        bitmap_rates[1] = 0x00FF;
        /* Support all HT-MCSs rate */
        for (i = 0; i < NELEMENTS(pmpriv->bitmap_rates) - 3; i++)
            bitmap_rates[i + 2] = 0xFFFF;
        bitmap_rates[9] = 0x3FFF;
    } else {
        memset(rates, 0, sizeof(rates));
        wlan_get_active_data_rates(pmpriv, rates);
        rate = rates;
        for (i = 0; (rate[i] && i < WLAN_SUPPORTED_RATES); i++) {
            PRINTM(INFO, "Rate=0x%X  Wanted=0x%X\n", rate[i],
                   ds_rate->param.rate_cfg.rate);
            if ((rate[i] & 0x7f) == (ds_rate->param.rate_cfg.rate & 0x7f))
                break;
        }
        if (!rate[i] || (i == WLAN_SUPPORTED_RATES)) {
            PRINTM(ERROR, "The fixed data rate 0x%X is out "
                   "of range\n", ds_rate->param.rate_cfg.rate);
            ret = MLAN_STATUS_FAILURE;
            goto exit;
        }
        memset(bitmap_rates, 0, sizeof(bitmap_rates));

        rateIndex =
            wlan_data_rate_to_index(pmadapter, ds_rate->param.rate_cfg.rate);

        /* Only allow b/g rates to be set */
        if (rateIndex >= 0 && rateIndex <= 3)
            bitmap_rates[0] = 1 << rateIndex;
        else {
            rateIndex -= 1;     /* There is a 0x00 in the table */
            if (rateIndex >= 4 && rateIndex <= 11)
                bitmap_rates[1] = 1 << (rateIndex - 4);
        }
    }

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv,
                           HostCmd_CMD_TX_RATE_CFG,
                           HostCmd_ACT_GEN_SET,
                           0, (t_void *) pioctl_req, bitmap_rates);
    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;

  exit:
    LEAVE();
    return ret;
}

/** 
 *  @brief Get rate index
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_rate_ioctl_get_rate_index(IN pmlan_adapter pmadapter,
                               IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];

    ENTER();

    if (!wlan_is_cmd_allowed(pmpriv)) {
        PRINTM(MSG, "TX_RATE_CFG: not allowed\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv,
                           HostCmd_CMD_TX_RATE_CFG,
                           HostCmd_ACT_GEN_GET,
                           0, (t_void *) pioctl_req, MNULL);
    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;

  exit:
    LEAVE();
    return ret;
}

/** 
 *  @brief Set rate index
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_rate_ioctl_set_rate_index(IN pmlan_adapter pmadapter,
                               IN pmlan_ioctl_req pioctl_req)
{
    int rate_index;
    int i;
    mlan_ds_rate *ds_rate = MNULL;
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    t_u16 bitmap_rates[MAX_BITMAP_RATES_SIZE];

    ENTER();

    if (!wlan_is_cmd_allowed(pmpriv)) {
        PRINTM(MSG, "TX_RATE_CFG: not allowed\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    ds_rate = (mlan_ds_rate *) pioctl_req->pbuf;
    rate_index = ds_rate->param.rate_cfg.rate;
    if (ds_rate->param.rate_cfg.is_rate_auto) {
        memset(bitmap_rates, 0, sizeof(bitmap_rates));
        /* Support all HR/DSSS rates */
        bitmap_rates[0] = 0x000F;
        /* Support all OFDM rates */
        bitmap_rates[1] = 0x00FF;
        /* Support all HT-MCSs rate */
        for (i = 0; i < NELEMENTS(bitmap_rates) - 3; i++)
            bitmap_rates[i + 2] = 0xFFFF;
        bitmap_rates[9] = 0x3FFF;
    } else {
        PRINTM(INFO, "Rate index is %d\n", rate_index);
        memset(bitmap_rates, 0, sizeof(bitmap_rates));
        /* Bitmap of HR/DSSS rates */
        if (rate_index >= 0 && rate_index <= 3)
            bitmap_rates[0] = 1 << rate_index;
        /* Bitmap of OFDM rates */
        if (rate_index >= 4 && rate_index <= 11)
            bitmap_rates[1] = 1 << (rate_index - 4);
        /* Bitmap of HT-MCSs allowed for initial rate */
        if (rate_index >= MLAN_RATE_INDEX_MCS0 &&
            rate_index <= MLAN_RATE_INDEX_MCS32)
            bitmap_rates[((rate_index - 12) / 16) + 2] =
                1 << ((rate_index - 12) % 16);
    }

    PRINTM(INFO, "RateBitmap=%04x%04x%04x%04x%04x%04x%04x%04x%04x%04x, "
           "IsRateAuto=%d, DataRate=%d\n",
           bitmap_rates[9], bitmap_rates[8],
           bitmap_rates[7], bitmap_rates[6],
           bitmap_rates[5], bitmap_rates[4],
           bitmap_rates[3], bitmap_rates[2],
           bitmap_rates[1], bitmap_rates[0],
           pmpriv->is_data_rate_auto, pmpriv->data_rate);

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv,
                           HostCmd_CMD_TX_RATE_CFG,
                           HostCmd_ACT_GEN_SET,
                           0, (t_void *) pioctl_req, (t_void *) bitmap_rates);
    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;

  exit:
    LEAVE();
    return ret;
}

/** 
 *  @brief Rate configuration command handler
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_rate_ioctl_cfg(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_ds_rate *rate = MNULL;
    mlan_status status = MLAN_STATUS_SUCCESS;

    ENTER();

    rate = (mlan_ds_rate *) pioctl_req->pbuf;
    if (rate->param.rate_cfg.rate_type == MLAN_RATE_VALUE) {
        if (pioctl_req->action == MLAN_ACT_GET)
            status = wlan_rate_ioctl_get_rate_value(pmadapter, pioctl_req);
        else
            status = wlan_rate_ioctl_set_rate_value(pmadapter, pioctl_req);
    } else {
        if (pioctl_req->action == MLAN_ACT_GET)
            status = wlan_rate_ioctl_get_rate_index(pmadapter, pioctl_req);
        else
            status = wlan_rate_ioctl_set_rate_index(pmadapter, pioctl_req);
    }

    LEAVE();
    return status;
}

/** 
 *  @brief Get data rates
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_rate_ioctl_get_data_rate(IN pmlan_adapter pmadapter,
                              IN pmlan_ioctl_req pioctl_req)
{
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_status ret = MLAN_STATUS_SUCCESS;

    ENTER();

    if (pioctl_req->action != MLAN_ACT_GET) {
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    if (!wlan_is_cmd_allowed(pmpriv)) {
        PRINTM(MSG, "TX_RATE_QUERY: not allowed\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv,
                           HostCmd_CMD_802_11_TX_RATE_QUERY,
                           HostCmd_ACT_GEN_GET,
                           0, (t_void *) pioctl_req, MNULL);
    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;

  exit:
    LEAVE();
    return ret;
}

/** 
 *  @brief Rate command handler
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_rate_ioctl(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status status = MLAN_STATUS_SUCCESS;
    mlan_ds_rate *rate = MNULL;

    ENTER();

    if (pioctl_req->buf_len < sizeof(mlan_ds_rate)) {
        PRINTM(WARN, "MLAN bss IOCTL length is too short.\n");
        pioctl_req->data_read_written = 0;
        pioctl_req->buf_len_needed = sizeof(mlan_ds_rate);
        LEAVE();
        return MLAN_STATUS_RESOURCE;
    }
    rate = (mlan_ds_rate *) pioctl_req->pbuf;
    switch (rate->sub_command) {
    case MLAN_OID_RATE_CFG:
        status = wlan_rate_ioctl_cfg(pmadapter, pioctl_req);
        break;
    case MLAN_OID_GET_DATA_RATE:
        status = wlan_rate_ioctl_get_data_rate(pmadapter, pioctl_req);
        break;
    case MLAN_OID_SUPPORTED_RATES:
        status = wlan_rate_ioctl_get_supported_rate(pmadapter, pioctl_req);
        break;
    default:
        status = MLAN_STATUS_FAILURE;
        break;
    }
    LEAVE();
    return status;
}

/** 
 *  @brief Get Tx power configuration
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_power_ioctl_get_power(IN pmlan_adapter pmadapter,
                           IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];

    ENTER();

    if (!wlan_is_cmd_allowed(pmpriv)) {
        PRINTM(MSG, "TXPWR_CFG: not allowed\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv,
                           HostCmd_CMD_TXPWR_CFG,
                           HostCmd_ACT_GEN_GET,
                           0, (t_void *) pioctl_req, MNULL);
    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;

  exit:
    LEAVE();
    return ret;
}

/** 
 *  @brief Set Tx power configuration
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_power_ioctl_set_power(IN pmlan_adapter pmadapter,
                           IN pmlan_ioctl_req pioctl_req)
{
    mlan_ds_power_cfg *power = MNULL;
    mlan_status ret = MLAN_STATUS_SUCCESS;
    HostCmd_DS_TXPWR_CFG *txp_cfg = MNULL;
    MrvlTypes_Power_Group_t *pg_tlv = MNULL;
    Power_Group_t *pg = MNULL;
    pmlan_callbacks pcb = &pmadapter->callbacks;
    t_u8 *buf = MNULL;
    t_u16 dbm = 0;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];

    ENTER();

    power = (mlan_ds_power_cfg *) pioctl_req->pbuf;
    if (!power->param.power_cfg.is_power_auto) {
        dbm = power->param.power_cfg.power_level;
        if ((dbm < pmpriv->min_tx_power_level) ||
            (dbm > pmpriv->max_tx_power_level)) {
            PRINTM(ERROR,
                   "The set txpower value %d dBm is out of range (%d dBm-%d dBm)!\n",
                   dbm, pmpriv->min_tx_power_level, pmpriv->max_tx_power_level);
            ret = MLAN_STATUS_FAILURE;
            goto exit;
        }
    }

    if (!wlan_is_cmd_allowed(pmpriv)) {
        PRINTM(MSG, "TXPWR_CFG: not allowed\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    ret = pcb->moal_malloc(MRVDRV_SIZE_OF_CMD_BUFFER, &buf);
    if (ret != MLAN_STATUS_SUCCESS || buf == MNULL) {
        PRINTM(MSG, "ALLOC_CMD_BUF: pcmd_buf: out of memory\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }
    memset(buf, 0, MRVDRV_SIZE_OF_CMD_BUFFER);
    txp_cfg = (HostCmd_DS_TXPWR_CFG *) buf;
    txp_cfg->action = HostCmd_ACT_GEN_SET;
    if (!power->param.power_cfg.is_power_auto) {
        txp_cfg->mode = 1;
        pg_tlv =
            (MrvlTypes_Power_Group_t *) (buf + sizeof(HostCmd_DS_TXPWR_CFG));
        pg_tlv->type = TLV_TYPE_POWER_GROUP;
        pg_tlv->length = 4 * sizeof(Power_Group_t);
        pg = (Power_Group_t *) (buf + sizeof(HostCmd_DS_TXPWR_CFG) +
                                sizeof(MrvlTypes_Power_Group_t));
        /* Power group for modulation class HR/DSSS */
        pg->first_rate_code = 0x00;
        pg->last_rate_code = 0x03;
        pg->modulation_class = MOD_CLASS_HR_DSSS;
        pg->power_step = 0;
        pg->power_min = dbm;
        pg->power_max = dbm;
        pg++;
        /* Power group for modulation class OFDM */
        pg->first_rate_code = 0x00;
        pg->last_rate_code = 0x07;
        pg->modulation_class = MOD_CLASS_OFDM;
        pg->power_step = 0;
        pg->power_min = dbm;
        pg->power_max = dbm;
        pg++;
        /* Power group for modulation class HTBW20 */
        pg->first_rate_code = 0x00;
        pg->last_rate_code = 0x20;
        pg->modulation_class = MOD_CLASS_HT;
        pg->power_step = 0;
        pg->power_min = dbm;
        pg->power_max = dbm;
        pg->ht_bandwidth = HT_BW_20;
        pg++;
        /* Power group for modulation class HTBW40 */
        pg->first_rate_code = 0x00;
        pg->last_rate_code = 0x20;
        pg->modulation_class = MOD_CLASS_HT;
        pg->power_step = 0;
        pg->power_min = dbm;
        pg->power_max = dbm;
        pg->ht_bandwidth = HT_BW_40;
    }

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv,
                           HostCmd_CMD_TXPWR_CFG,
                           HostCmd_ACT_GEN_SET, 0, (t_void *) pioctl_req, buf);
    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;
    if (buf)
        pcb->moal_mfree(buf);

  exit:
    LEAVE();
    return ret;
}

/**
 *  @brief Get modulation class from rate index
 *  
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param rate_index	Rate index
 *  
 *  @return		0 fail, otherwise return modulation class
 */
static int
wlan_get_modulation_class(pmlan_adapter pmadapter, int rate_index)
{
    ENTER();
    if (rate_index >= MOD_HR_DSSS_START && rate_index <= MOD_HR_DSSS_END) {
        return MOD_CLASS_HR_DSSS;
    } else if (rate_index >= MOD_OFDM_START && rate_index <= MOD_OFDM_END) {
        return MOD_CLASS_OFDM;
    } else if (rate_index >= MOD_HT_START && rate_index <= MOD_HT_END) {
        return MOD_CLASS_HT;
    }
    PRINTM(ERROR, "Invalid rate index = %d supplied!\n", rate_index);

    LEAVE();
    return 0;
}

/** 
 *  @brief Set extended power configuration
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_power_ioctl_set_power_ext(IN pmlan_adapter pmadapter,
                               IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_ds_power_cfg *power = MNULL;
    pmlan_callbacks pcb = &pmadapter->callbacks;
    t_u8 *buf = MNULL;
    HostCmd_DS_TXPWR_CFG *txp_cfg = MNULL;
    MrvlTypes_Power_Group_t *pg_tlv = MNULL;
    Power_Group_t *pg = MNULL;
    int class;
    t_u32 data[4];
    t_u8 ht_bw;

    ENTER();

    if (!wlan_is_cmd_allowed(pmpriv)) {
        PRINTM(MSG, "TXPWR_CFG: not allowed\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    power = (mlan_ds_power_cfg *) pioctl_req->pbuf;
    ret = pcb->moal_malloc(MRVDRV_SIZE_OF_CMD_BUFFER, &buf);
    if (ret != MLAN_STATUS_SUCCESS || buf == MNULL) {
        PRINTM(ERROR, "ALLOC_CMD_BUF: pcmd_buf: out of memory\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }
    memset(buf, 0, MRVDRV_SIZE_OF_CMD_BUFFER);
    txp_cfg = (HostCmd_DS_TXPWR_CFG *) buf;
    txp_cfg->action = HostCmd_ACT_GEN_SET;
    memcpy((t_u8 *) & data, (t_u8 *) power->param.power_ext.power_data,
           sizeof(data));
    switch (power->param.power_ext.len) {
    case 1:
        if (data[0] == 0xFF)
            txp_cfg->mode = 0;
        else
            ret = MLAN_STATUS_FAILURE;
        break;
    case 2:
    case 4:
        ht_bw = (data[0] & TX_RATE_HT_BW40_BIT) ? HT_BW_40 : HT_BW_20;
        data[0] &= ~TX_RATE_HT_BW40_BIT;
        if (!(class = wlan_get_modulation_class(pmadapter, data[0]))) {
            ret = MLAN_STATUS_FAILURE;
            break;
        }
        if (ht_bw && class != MOD_CLASS_HT) {
            ret = MLAN_STATUS_FAILURE;
            break;
        }
        txp_cfg->mode = 1;
        pg_tlv =
            (MrvlTypes_Power_Group_t *) (buf + sizeof(HostCmd_DS_TXPWR_CFG));
        pg_tlv->type = TLV_TYPE_POWER_GROUP;
        pg_tlv->length = sizeof(Power_Group_t);
        pg = (Power_Group_t *) (buf + sizeof(HostCmd_DS_TXPWR_CFG) +
                                sizeof(MrvlTypes_Power_Group_t));
        pg->modulation_class = class;
        pg->first_rate_code = data[0];
        pg->last_rate_code = data[0];
        if (class == MOD_CLASS_OFDM) {
            pg->first_rate_code = data[0] - MOD_OFDM_START;
            pg->last_rate_code = data[0] - MOD_OFDM_START;
        } else if (class == MOD_CLASS_HT) {
            pg->first_rate_code = data[0] - MOD_HT_START;
            pg->last_rate_code = data[0] - MOD_HT_START;
            pg->ht_bandwidth = ht_bw;
        }
        pg->power_min = data[1];
        pg->power_max = data[1];
        if (power->param.power_ext.len == 4) {
            pg->power_max = data[2];
            pg->power_step = data[3];
        }
        break;
    default:
        ret = MLAN_STATUS_FAILURE;
        break;
    }
    if (ret == MLAN_STATUS_FAILURE) {
        if (buf)
            pcb->moal_mfree(buf);
        goto exit;
    }

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv,
                           HostCmd_CMD_TXPWR_CFG,
                           HostCmd_ACT_GEN_SET, 0, (t_void *) pioctl_req, buf);
    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;
    if (buf)
        pcb->moal_mfree(buf);

  exit:
    LEAVE();
    return ret;
}

/** 
 *  @brief Power configuration command handler
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_power_ioctl(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status status = MLAN_STATUS_SUCCESS;
    mlan_ds_power_cfg *power = MNULL;

    ENTER();

    if (pioctl_req->buf_len < sizeof(mlan_ds_power_cfg)) {
        PRINTM(WARN, "MLAN bss IOCTL length is too short.\n");
        pioctl_req->data_read_written = 0;
        pioctl_req->buf_len_needed = sizeof(mlan_ds_power_cfg);
        LEAVE();
        return MLAN_STATUS_RESOURCE;
    }
    power = (mlan_ds_power_cfg *) pioctl_req->pbuf;
    switch (power->sub_command) {
    case MLAN_OID_POWER_CFG:
        if (pioctl_req->action == MLAN_ACT_GET)
            status = wlan_power_ioctl_get_power(pmadapter, pioctl_req);
        else
            status = wlan_power_ioctl_set_power(pmadapter, pioctl_req);
        break;
    case MLAN_OID_POWER_CFG_EXT:
        if (pioctl_req->action == MLAN_ACT_GET)
            status = wlan_power_ioctl_get_power(pmadapter, pioctl_req);
        else
            status = wlan_power_ioctl_set_power_ext(pmadapter, pioctl_req);
        break;
    default:
        status = MLAN_STATUS_FAILURE;
        break;
    }
    LEAVE();
    return status;
}

/** 
 *  @brief Set power save configurations
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *  @param ps_mode	Power save mode
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_pm_ioctl_ps_mode(IN pmlan_adapter pmadapter,
                      IN pmlan_ioctl_req pioctl_req, IN t_u16 ps_mode)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    t_u16 sub_cmd = (pmadapter->ps_mode == Wlan802_11PowerModePSP) ?
        HostCmd_SubCmd_Enter_PS : HostCmd_SubCmd_Exit_PS;

    ENTER();

    ret = wlan_prepare_cmd(pmpriv,
                           HostCmd_CMD_802_11_PS_MODE, sub_cmd,
                           0, (t_void *) pioctl_req, MNULL);
    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;

    LEAVE();
    return ret;
}

/** 
 *  @brief Set Host Sleep configurations
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *  @param phs_cfg	A pointer to HOST_SLEEP_CFG structure
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_pm_ioctl_hs_cfg(IN pmlan_adapter pmadapter,
                     IN pmlan_ioctl_req pioctl_req,
                     IN HostCmd_DS_802_11_HOST_SLEEP_CFG * phs_cfg)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];

    ENTER();

    ret = wlan_prepare_cmd(pmpriv,
                           HostCmd_CMD_802_11_HOST_SLEEP_CFG,
                           HostCmd_ACT_GEN_SET,
                           0, (t_void *) pioctl_req, phs_cfg);
    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;

    LEAVE();
    return ret;
}

/** 
 *  @brief Set/Get Inactivity timeout extend
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_pm_ioctl_inactivity_timeout(IN pmlan_adapter pmadapter,
                                 IN pmlan_ioctl_req pioctl_req)
{
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_ds_pm_cfg *pmcfg = MNULL;
    mlan_status ret = MLAN_STATUS_SUCCESS;
    t_u16 cmd_action = 0;

    ENTER();

    if (!wlan_is_cmd_allowed(pmpriv)) {
        PRINTM(MSG, "INACTIVITYTO: not allowed\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    pmcfg = (mlan_ds_pm_cfg *) pioctl_req->pbuf;
    cmd_action = HostCmd_ACT_GEN_GET;
    if (pioctl_req->action == MLAN_ACT_SET) {
        cmd_action = HostCmd_ACT_GEN_SET;
    }

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv,
                           HostCmd_CMD_INACTIVITY_TIMEOUT_EXT,
                           cmd_action, 0, (t_void *) pioctl_req,
                           (t_void *) & pmcfg->param.inactivity_to);

    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;

  exit:
    LEAVE();
    return ret;
}

/** 
 *  @brief Power save command handler
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_pm_ioctl(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status status = MLAN_STATUS_SUCCESS;
    pmlan_private pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_ds_pm_cfg *pm = MNULL;

    ENTER();

    if (pioctl_req->buf_len < sizeof(mlan_ds_pm_cfg)) {
        PRINTM(WARN, "MLAN bss IOCTL length is too short.\n");
        pioctl_req->data_read_written = 0;
        pioctl_req->buf_len_needed = sizeof(mlan_ds_pm_cfg);
        LEAVE();
        return MLAN_STATUS_RESOURCE;
    }
    pm = (mlan_ds_pm_cfg *) pioctl_req->pbuf;
    switch (pm->sub_command) {
    case MLAN_OID_PM_CFG_IEEE_PS:
        switch (pioctl_req->action) {
        case MLAN_ACT_SET:
            if (!wlan_is_cmd_allowed(pmpriv)) {
                PRINTM(MSG, "PS_MODE: not allowed\n");
                status = MLAN_STATUS_FAILURE;
                break;
            }
            if (pm->param.ps_mode)
                pmadapter->ps_mode = Wlan802_11PowerModePSP;
            else
                pmadapter->ps_mode = Wlan802_11PowerModeCAM;
            if (pmpriv->media_connected == MTRUE) {
                status =
                    wlan_pm_ioctl_ps_mode(pmadapter, pioctl_req,
                                          pmadapter->ps_mode);
            }
            break;
        case MLAN_ACT_GET:
            if (pmadapter->ps_mode == Wlan802_11PowerModePSP)
                pm->param.ps_mode = 1;
            else
                pm->param.ps_mode = 0;
            break;
        default:
            status = MLAN_STATUS_FAILURE;
            break;
        }
        break;
    case MLAN_OID_PM_CFG_HS_CFG:
        switch (pioctl_req->action) {
        case MLAN_ACT_SET:
            if (pm->param.hs_cfg.conditions != HOST_SLEEP_CFG_CANCEL) {
                if (!wlan_is_cmd_allowed(pmpriv)) {
                    PRINTM(MSG, "HS_CFG: not allowed\n");
                    status = MLAN_STATUS_FAILURE;
                    break;
                }
            }
            pmadapter->hs_cfg.conditions = pm->param.hs_cfg.conditions;
            pmadapter->hs_cfg.gpio = pm->param.hs_cfg.gpio;
            pmadapter->hs_cfg.gap = pm->param.hs_cfg.gap;
            if (pm->param.hs_cfg.is_invoke_hostcmd == MTRUE) {
                status =
                    wlan_pm_ioctl_hs_cfg(pmadapter, pioctl_req,
                                         &pmadapter->hs_cfg);
            }
            break;
        case MLAN_ACT_GET:
            pm->param.hs_cfg.conditions = pmadapter->hs_cfg.conditions;
            pm->param.hs_cfg.gpio = pmadapter->hs_cfg.gpio;
            pm->param.hs_cfg.gap = pmadapter->hs_cfg.gap;
            break;
        default:
            status = MLAN_STATUS_FAILURE;
            break;
        }
        break;
    case MLAN_OID_PM_CFG_INACTIVITY_TO:
        status = wlan_pm_ioctl_inactivity_timeout(pmadapter, pioctl_req);
        break;
    default:
        status = MLAN_STATUS_FAILURE;
        break;
    }
    LEAVE();
    return status;
}

/** 
 *  @brief Set/Get WMM status
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_wmm_ioctl_enable(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_ds_wmm_cfg *wmm = MNULL;
    ENTER();
    wmm = (mlan_ds_wmm_cfg *) pioctl_req->pbuf;
    if (pioctl_req->action == MLAN_ACT_GET)
        wmm->param.wmm_enable = pmpriv->wmm_required;
    else
        pmpriv->wmm_required = wmm->param.wmm_enable;
    pioctl_req->data_read_written = sizeof(t_u32) + MLAN_SUB_COMMAND_SIZE;
    LEAVE();
    return ret;
}

/** 
 *  @brief Set/Get WMM QoS configuration
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_wmm_ioctl_qos(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_ds_wmm_cfg *wmm = MNULL;

    ENTER();

    wmm = (mlan_ds_wmm_cfg *) pioctl_req->pbuf;

    if (pioctl_req->action == MLAN_ACT_GET)
        wmm->param.qos_cfg = pmpriv->wmm_qosinfo;
    else
        pmpriv->wmm_qosinfo = wmm->param.qos_cfg;

    pioctl_req->data_read_written = sizeof(t_u8) + MLAN_SUB_COMMAND_SIZE;

    LEAVE();
    return ret;
}

/** 
 *  @brief WMM configuration handler
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_wmm_cfg_ioctl(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status status = MLAN_STATUS_SUCCESS;
    mlan_ds_wmm_cfg *wmm = MNULL;

    ENTER();

    if (pioctl_req->buf_len < sizeof(mlan_ds_wmm_cfg)) {
        PRINTM(WARN, "MLAN bss IOCTL length is too short.\n");
        pioctl_req->data_read_written = 0;
        pioctl_req->buf_len_needed = sizeof(mlan_ds_wmm_cfg);
        LEAVE();
        return MLAN_STATUS_RESOURCE;
    }
    wmm = (mlan_ds_wmm_cfg *) pioctl_req->pbuf;
    switch (wmm->sub_command) {
    case MLAN_OID_WMM_CFG_ENABLE:
        status = wlan_wmm_ioctl_enable(pmadapter, pioctl_req);
        break;
    case MLAN_OID_WMM_CFG_QOS:
        status = wlan_wmm_ioctl_qos(pmadapter, pioctl_req);
        break;
    default:
        status = MLAN_STATUS_FAILURE;
        break;
    }
    LEAVE();
    return status;
}

/** 
 *  @brief Set/Get WPA IE   
 *
 *  @param priv         A pointer to mlan_private structure
 *  @param ie_data_ptr  A pointer to IE
 *  @param ie_len       Length of the IE
 *
 *  @return             MLAN_STATUS_SUCCESS --success, otherwise fail
 */
mlan_status
wlan_set_wpa_ie_helper(mlan_private * priv, t_u8 * ie_data_ptr, t_u16 ie_len)
{
    ENTER();

    if (ie_len) {
        if (ie_len > sizeof(priv->wpa_ie)) {
            PRINTM(ERROR, "failed to copy WPA IE, too big \n");
            LEAVE();
            return MLAN_STATUS_FAILURE;
        }
        memcpy(priv->wpa_ie, ie_data_ptr, ie_len);
        priv->wpa_ie_len = ie_len;
        PRINTM(CMND, "Set Wpa_ie_len=%d IE=%#x\n", priv->wpa_ie_len,
               priv->wpa_ie[0]);
        DBG_HEXDUMP(CMD_D, "Wpa_ie", priv->wpa_ie, priv->wpa_ie_len);
        if (priv->wpa_ie[0] == WPA_IE) {
            priv->sec_info.wpa_enabled = MTRUE;
        } else if (priv->wpa_ie[0] == RSN_IE) {
            priv->sec_info.wpa2_enabled = MTRUE;
        } else {
            priv->sec_info.wpa_enabled = MFALSE;
            priv->sec_info.wpa2_enabled = MFALSE;
        }
    } else {
        memset(priv->wpa_ie, 0, sizeof(priv->wpa_ie));
        priv->wpa_ie_len = 0;
        PRINTM(INFO, "Reset Wpa_ie_len=%d IE=%#x\n", priv->wpa_ie_len,
               priv->wpa_ie[0]);
        priv->sec_info.wpa_enabled = MFALSE;
        priv->sec_info.wpa2_enabled = MFALSE;
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Set/Get authentication mode
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_sec_ioctl_auth_mode(IN pmlan_adapter pmadapter,
                         IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_ds_sec_cfg *sec = MNULL;
    ENTER();
    sec = (mlan_ds_sec_cfg *) pioctl_req->pbuf;
    if (pioctl_req->action == MLAN_ACT_GET)
        sec->param.auth_mode = pmpriv->sec_info.authentication_mode;
    else {
        pmpriv->sec_info.authentication_mode = sec->param.auth_mode;
        if (pmpriv->sec_info.authentication_mode == MLAN_AUTH_MODE_NETWORKEAP)
            wlan_set_wpa_ie_helper(pmpriv, MNULL, 0);
    }
    pioctl_req->data_read_written = sizeof(t_u32) + MLAN_SUB_COMMAND_SIZE;
    LEAVE();
    return ret;
}

/** 
 *  @brief Set/Get encryption mode
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_sec_ioctl_encrypt_mode(IN pmlan_adapter pmadapter,
                            IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_ds_sec_cfg *sec = MNULL;
    ENTER();
    sec = (mlan_ds_sec_cfg *) pioctl_req->pbuf;
    if (pioctl_req->action == MLAN_ACT_GET)
        sec->param.encrypt_mode = pmpriv->sec_info.encryption_mode;
    else {
        pmpriv->sec_info.encryption_mode = sec->param.encrypt_mode;
    }
    pioctl_req->data_read_written = sizeof(t_u32) + MLAN_SUB_COMMAND_SIZE;
    LEAVE();
    return ret;
}

/** 
 *  @brief Set/Get WPA status
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_sec_ioctl_wpa_enable(IN pmlan_adapter pmadapter,
                          IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_ds_sec_cfg *sec = MNULL;
    ENTER();
    sec = (mlan_ds_sec_cfg *) pioctl_req->pbuf;
    if (pioctl_req->action == MLAN_ACT_GET) {
        if (pmpriv->wpa_ie_len)
            sec->param.wpa_enabled = MTRUE;
        else
            sec->param.wpa_enabled = MFALSE;
    } else {
        if (sec->param.wpa_enabled == MFALSE)
            wlan_set_wpa_ie_helper(pmpriv, MNULL, 0);
    }
    pioctl_req->data_read_written = sizeof(t_u32) + MLAN_SUB_COMMAND_SIZE;
    LEAVE();
    return ret;
}

/** 
 *  @brief Set WEP keys
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_sec_ioctl_set_wep_key(IN pmlan_adapter pmadapter,
                           IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_ds_sec_cfg *sec = MNULL;
    mrvl_wep_key_t *pwep_key = MNULL;
    int index;

    ENTER();

    if (!wlan_is_cmd_allowed(pmpriv)) {
        PRINTM(MSG, "KEY_MATERIAL: not allowed\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    if (pmpriv->wep_key_curr_index >= MRVL_NUM_WEP_KEY)
        pmpriv->wep_key_curr_index = 0;
    pwep_key = &pmpriv->wep_key[pmpriv->wep_key_curr_index];
    sec = (mlan_ds_sec_cfg *) pioctl_req->pbuf;
    if (sec->param.encrypt_key.is_current_wep_key == MTRUE)
        index = pmpriv->wep_key_curr_index;
    else
        index = sec->param.encrypt_key.key_index;
    if (sec->param.encrypt_key.key_disable == MTRUE) {
        if (pmpriv->sec_info.wep_status == Wlan802_11WEPEnabled &&
            pmpriv->media_connected == MTRUE &&
            pmpriv->bss_mode == MLAN_BSS_MODE_IBSS) {
            PRINTM(ERROR, "Can not disable key in IBSS connected state\n");
            ret = MLAN_STATUS_FAILURE;
            goto exit;
        }
        pmpriv->sec_info.wep_status = Wlan802_11WEPDisabled;
    } else if (!sec->param.encrypt_key.key_len) {
        if (pmpriv->sec_info.wep_status != Wlan802_11WEPEnabled &&
            pmpriv->media_connected == MTRUE &&
            pmpriv->bss_mode == MLAN_BSS_MODE_IBSS) {
            PRINTM(ERROR, "Can not enable key in IBSS connected state\n");
            ret = MLAN_STATUS_FAILURE;
            goto exit;
        }
        /* Copy the required key as the current key */
        pwep_key = &pmpriv->wep_key[index];
        if (!pwep_key->key_length) {
            PRINTM(ERROR, "Key not set,so cannot enable it\n");
            ret = MLAN_STATUS_FAILURE;
            goto exit;
        }
        pmpriv->wep_key_curr_index = index;
        pmpriv->sec_info.wep_status = Wlan802_11WEPEnabled;
    } else {
        pwep_key = &pmpriv->wep_key[index];
        /* Cleanup */
        memset(pwep_key, 0, sizeof(mrvl_wep_key_t));
        /* Copy the key in the driver */
        memcpy(pwep_key->key_material, sec->param.encrypt_key.key_material,
               sec->param.encrypt_key.key_len);
        pwep_key->key_index = index;
        pwep_key->key_length = sec->param.encrypt_key.key_len;
        if (pmpriv->sec_info.wep_status != Wlan802_11WEPEnabled) {
            /* 
             * The status is set as Key Absent 
             * so as to make sure we display the 
             * keys when iwlist mlanX key is used
             */
            pmpriv->sec_info.wep_status = Wlan802_11WEPKeyAbsent;
        }
        pmpriv->wep_key_curr_index = index;
    }
    if (pwep_key->key_length) {
        /* Send request to firmware */
        ret = wlan_prepare_cmd(pmpriv,
                               HostCmd_CMD_802_11_KEY_MATERIAL,
                               HostCmd_ACT_GEN_SET, 0, MNULL, MNULL);
        if (ret)
            goto exit;
    }
    if (pmpriv->sec_info.wep_status == Wlan802_11WEPEnabled)
        pmpriv->curr_pkt_filter |= HostCmd_ACT_MAC_WEP_ENABLE;
    else
        pmpriv->curr_pkt_filter &= ~HostCmd_ACT_MAC_WEP_ENABLE;

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv,
                           HostCmd_CMD_MAC_CONTROL,
                           HostCmd_ACT_GEN_SET,
                           0, (t_void *) pioctl_req, &pmpriv->curr_pkt_filter);
    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;

  exit:
    LEAVE();
    return ret;
}

/** 
 *  @brief Set WPA key
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_sec_ioctl_set_wpa_key(IN pmlan_adapter pmadapter,
                           IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_ds_sec_cfg *sec = MNULL;
    t_u8 remove_key = MFALSE;

    ENTER();

    sec = (mlan_ds_sec_cfg *) pioctl_req->pbuf;
    /* Current driver only supports key length of up to 32 bytes */
    if (sec->param.encrypt_key.key_len > MLAN_MAX_KEY_LENGTH) {
        PRINTM(ERROR, " Error in key length \n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    if (!wlan_is_cmd_allowed(pmpriv)) {
        PRINTM(MSG, "KEY_MATERIAL: not allowed\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    if (pmpriv->bss_mode == MLAN_BSS_MODE_IBSS) {
        /* 
         * IBSS/WPA-None uses only one key (Group) for both receiving and
         *  sending unicast and multicast packets.
         */
        /* Send the key as PTK to firmware */
        sec->param.encrypt_key.key_index = 0x40000000;
        ret = wlan_prepare_cmd(pmpriv,
                               HostCmd_CMD_802_11_KEY_MATERIAL,
                               HostCmd_ACT_GEN_SET,
                               KEY_INFO_ENABLED,
                               MNULL, &sec->param.encrypt_key);
        if (ret)
            goto exit;

        /* Send the key as GTK to firmware */
        sec->param.encrypt_key.key_index = ~0x40000000;
    }
    if (pmpriv->bss_mode == MLAN_BSS_MODE_IBSS &&
        sec->param.encrypt_key.key_len == WPA_AES_KEY_LEN) {
        t_u8 zero_key_material[WPA_AES_KEY_LEN];

        memset(zero_key_material, 0, sizeof(zero_key_material));
        if (memcmp(sec->param.encrypt_key.key_material, zero_key_material,
                   WPA_AES_KEY_LEN)) {
            PRINTM(INFO, "Adhoc AES Enabled.\n");
            pmpriv->adhoc_aes_enabled = MTRUE;
            remove_key = MFALSE;
        } else {
            PRINTM(INFO, "Adhoc AES Disabled.\n");
            pmpriv->adhoc_aes_enabled = MFALSE;
            remove_key = MTRUE;
        }
    }

    if (!sec->param.encrypt_key.key_index)
        sec->param.encrypt_key.key_index = 0x40000000;

    if (remove_key == MTRUE) {
        /* Send request to firmware */
        ret = wlan_prepare_cmd(pmpriv,
                               HostCmd_CMD_802_11_KEY_MATERIAL,
                               HostCmd_ACT_GEN_SET,
                               !(KEY_INFO_ENABLED),
                               (t_void *) pioctl_req, &sec->param.encrypt_key);
    } else {
        /* Send request to firmware */
        ret = wlan_prepare_cmd(pmpriv,
                               HostCmd_CMD_802_11_KEY_MATERIAL,
                               HostCmd_ACT_GEN_SET,
                               KEY_INFO_ENABLED,
                               (t_void *) pioctl_req, &sec->param.encrypt_key);
    }

    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;

  exit:
    LEAVE();
    return ret;
}

/** 
 *  @brief Get security keys
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_sec_ioctl_get_key(IN pmlan_adapter pmadapter,
                       IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_ds_sec_cfg *sec = MNULL;
    int index;
    ENTER();

    sec = (mlan_ds_sec_cfg *) pioctl_req->pbuf;

    if (pmpriv->adhoc_aes_enabled == MTRUE &&
        (pmpriv->aes_key.key_param_set.key_len == WPA_AES_KEY_LEN)) {
        HEXDUMP("Get WPA Key", pmpriv->aes_key.key_param_set.key,
                WPA_AES_KEY_LEN);
        memcpy(sec->param.encrypt_key.key_material,
               pmpriv->aes_key.key_param_set.key, WPA_AES_KEY_LEN);
        LEAVE();
        return ret;

    }

    if (pmpriv->wep_key_curr_index >= MRVL_NUM_WEP_KEY)
        pmpriv->wep_key_curr_index = 0;

    if ((pmpriv->sec_info.wep_status == Wlan802_11WEPEnabled)
        || (pmpriv->sec_info.wep_status == Wlan802_11WEPKeyAbsent)
        || pmpriv->sec_info.ewpa_enabled
        || pmpriv->sec_info.wpa_enabled || pmpriv->sec_info.wpa2_enabled) {
        sec->param.encrypt_key.key_disable = MFALSE;
    } else {
        sec->param.encrypt_key.key_disable = MTRUE;
    }

    if (sec->param.encrypt_key.is_current_wep_key == MTRUE) {
        if ((pmpriv->wep_key[pmpriv->wep_key_curr_index].key_length) &&
            (pmpriv->sec_info.wep_status == Wlan802_11WEPEnabled)) {
            index = pmpriv->wep_key_curr_index;
            sec->param.encrypt_key.key_index = pmpriv->wep_key[index].key_index;
            memcpy(sec->param.encrypt_key.key_material,
                   pmpriv->wep_key[index].key_material,
                   pmpriv->wep_key[index].key_length);
            sec->param.encrypt_key.key_len = pmpriv->wep_key[index].key_length;
        } else if ((pmpriv->sec_info.wpa_enabled)
                   || (pmpriv->sec_info.ewpa_enabled)
                   || (pmpriv->sec_info.wpa2_enabled)
            ) {
            /* Return WPA enabled */
            sec->param.encrypt_key.key_disable = MFALSE;
        } else {
            sec->param.encrypt_key.key_disable = MTRUE;
        }
    } else {
        index = sec->param.encrypt_key.key_index;
        if (pmpriv->wep_key[index].key_length) {
            sec->param.encrypt_key.key_index = pmpriv->wep_key[index].key_index;
            memcpy(sec->param.encrypt_key.key_material,
                   pmpriv->wep_key[index].key_material,
                   pmpriv->wep_key[index].key_length);
            sec->param.encrypt_key.key_len = pmpriv->wep_key[index].key_length;
        } else if ((pmpriv->sec_info.wpa_enabled)
                   || (pmpriv->sec_info.ewpa_enabled)
                   || (pmpriv->sec_info.wpa2_enabled)
            ) {
            /* Return WPA enabled */
            sec->param.encrypt_key.key_disable = MFALSE;
        } else {
            sec->param.encrypt_key.key_disable = MTRUE;
        }
    }

    LEAVE();
    return ret;
}

/** 
 *  @brief Set security key(s)
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_sec_ioctl_encrypt_key(IN pmlan_adapter pmadapter,
                           IN pmlan_ioctl_req pioctl_req)
{
    mlan_status status = MLAN_STATUS_SUCCESS;
    mlan_ds_sec_cfg *sec = MNULL;
    ENTER();
    sec = (mlan_ds_sec_cfg *) pioctl_req->pbuf;
    if (pioctl_req->action == MLAN_ACT_SET) {
        if (sec->param.encrypt_key.key_len > MAX_WEP_KEY_SIZE)
            status = wlan_sec_ioctl_set_wpa_key(pmadapter, pioctl_req);
        else
            status = wlan_sec_ioctl_set_wep_key(pmadapter, pioctl_req);
    } else {
        status = wlan_sec_ioctl_get_key(pmadapter, pioctl_req);
    }
    LEAVE();
    return status;
}

/** 
 *  @brief Set/Get WPA passphrase for esupplicant
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_sec_ioctl_passphrase(IN pmlan_adapter pmadapter,
                          IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_ds_sec_cfg *sec = MNULL;
    t_u16 cmd_action = 0;
    BSSDescriptor_t *pbss_desc;
    int i = 0;

    ENTER();

    if (!wlan_is_cmd_allowed(pmpriv)) {
        PRINTM(MSG, "SUPPLICANT_PMK: not allowed\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    sec = (mlan_ds_sec_cfg *) pioctl_req->pbuf;
    if (pioctl_req->action == MLAN_ACT_SET) {
        if (sec->param.passphrase.psk_type == MLAN_PSK_CLEAR)
            cmd_action = HostCmd_ACT_GEN_REMOVE;
        else
            cmd_action = HostCmd_ACT_GEN_SET;
    } else {
        if (sec->param.passphrase.psk_type == MLAN_PSK_QUERY) {
            if (sec->param.passphrase.ssid.ssid_len == 0) {
                i = wlan_find_bssid_in_list(pmpriv,
                                            (t_u8 *) & sec->param.passphrase.
                                            bssid, MLAN_BSS_MODE_AUTO);
                if (i > 0) {
                    pbss_desc = &pmadapter->pscan_table[i];
                    memcpy(&sec->param.passphrase.ssid, &pbss_desc->ssid,
                           sizeof(mlan_802_11_ssid));
                    memset(&sec->param.passphrase.bssid, 0,
                           MLAN_MAC_ADDR_LENGTH);
                    PRINTM(CMND, "PSK_QUERY: found ssid=%s\n",
                           sec->param.passphrase.ssid.ssid);
                }
            } else
                memset(&sec->param.passphrase.bssid, 0, MLAN_MAC_ADDR_LENGTH);
        }
        cmd_action = HostCmd_ACT_GEN_GET;
    }

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv,
                           HostCmd_CMD_SUPPLICANT_PMK,
                           cmd_action,
                           0, (t_void *) pioctl_req, &sec->param.passphrase);
    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;
  exit:
    LEAVE();
    return ret;
}

/** 
 *  @brief Set/Get esupplicant status
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_sec_ioctl_ewpa_enable(IN pmlan_adapter pmadapter,
                           IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_ds_sec_cfg *sec = MNULL;
    ENTER();
    sec = (mlan_ds_sec_cfg *) pioctl_req->pbuf;
    if (pioctl_req->action == MLAN_ACT_GET) {
        sec->param.ewpa_enabled = pmpriv->sec_info.ewpa_enabled;
    } else {
        pmpriv->sec_info.ewpa_enabled = sec->param.ewpa_enabled;
        PRINTM(CMND, "Set: ewpa_enabled = %d\n",
               (int) pmpriv->sec_info.ewpa_enabled);
    }
    pioctl_req->data_read_written = sizeof(t_u32) + MLAN_SUB_COMMAND_SIZE;
    LEAVE();
    return ret;
}

/** 
 *  @brief Get esupplicant mode
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_sec_ioctl_esupp_mode(IN pmlan_adapter pmadapter,
                          IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];

    ENTER();

    if (pmpriv->media_connected != MTRUE) {
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    if (!wlan_is_cmd_allowed(pmpriv)) {
        PRINTM(MSG, "SUPPLICANT_PROFILE: not allowed\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv,
                           HostCmd_CMD_SUPPLICANT_PROFILE,
                           HostCmd_ACT_GEN_GET_CURRENT,
                           0, (t_void *) pioctl_req, MNULL);
    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;

  exit:
    LEAVE();
    return ret;
}

/** 
 *  @brief Security configuration handler
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_sec_cfg_ioctl(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status status = MLAN_STATUS_SUCCESS;
    mlan_ds_sec_cfg *sec = MNULL;

    ENTER();

    if (pioctl_req->buf_len < sizeof(mlan_ds_sec_cfg)) {
        PRINTM(WARN, "MLAN bss IOCTL length is too short.\n");
        pioctl_req->data_read_written = 0;
        pioctl_req->buf_len_needed = sizeof(mlan_ds_sec_cfg);
        LEAVE();
        return MLAN_STATUS_RESOURCE;
    }
    sec = (mlan_ds_sec_cfg *) pioctl_req->pbuf;
    switch (sec->sub_command) {
    case MLAN_OID_SEC_CFG_AUTH_MODE:
        status = wlan_sec_ioctl_auth_mode(pmadapter, pioctl_req);
        break;
    case MLAN_OID_SEC_CFG_ENCRYPT_MODE:
        status = wlan_sec_ioctl_encrypt_mode(pmadapter, pioctl_req);
        break;
    case MLAN_OID_SEC_CFG_WPA_ENABLED:
        status = wlan_sec_ioctl_wpa_enable(pmadapter, pioctl_req);
        break;
    case MLAN_OID_SEC_CFG_ENCRYPT_KEY:
        status = wlan_sec_ioctl_encrypt_key(pmadapter, pioctl_req);
        break;
    case MLAN_OID_SEC_CFG_PASSPHRASE:
        status = wlan_sec_ioctl_passphrase(pmadapter, pioctl_req);
        break;
    case MLAN_OID_SEC_CFG_EWPA_ENABLED:
        status = wlan_sec_ioctl_ewpa_enable(pmadapter, pioctl_req);
        break;
    case MLAN_OID_SEC_CFG_ESUPP_MODE:
        status = wlan_sec_ioctl_esupp_mode(pmadapter, pioctl_req);
        break;
    default:
        status = MLAN_STATUS_FAILURE;
        break;
    }
    LEAVE();
    return status;
}

/** 
 *  @brief  Append/Reset IE buffer. 
 *   
 *  Pass an opaque block of data, expected to be IEEE IEs, to the driver 
 *    for eventual passthrough to the firmware in an associate/join 
 *    (and potentially start) command.  This function is the main body
 *    for both wlan_set_gen_ie_ioctl and wlan_set_gen_ie
 *
 *  Data is appended to an existing buffer and then wrapped in a passthrough
 *    TLV in the command API to the firmware.  The firmware treats the data
 *    as a transparent passthrough to the transmitted management frame.
 *
 *  @param priv         A pointer to mlan_private structure
 *  @param ie_data_ptr  A pointer to iwreq structure
 *  @param ie_len       Length of the IE or IE block passed in ie_data_ptr
 *
 *  @return             MLAN_STATUS_SUCCESS --success, otherwise fail
 */
int
wlan_set_gen_ie_helper(mlan_private * priv, t_u8 * ie_data_ptr, t_u16 ie_len)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    IEEEtypes_VendorHeader_t *pvendor_ie;
    const t_u8 wpa_oui[] = { 0x00, 0x50, 0xf2, 0x01 };
    const t_u8 wps_oui[] = { 0x00, 0x50, 0xf2, 0x04 };

    ENTER();

    /* If the passed length is zero, reset the buffer */
    if (!ie_len) {
        priv->gen_ie_buf_len = 0;
        priv->wps.session_enable = MFALSE;
    } else if (!ie_data_ptr) {
        /* MNULL check */
        ret = MLAN_STATUS_FAILURE;
    } else {

        pvendor_ie = (IEEEtypes_VendorHeader_t *) ie_data_ptr;
        /* Test to see if it is a WPA IE, if not, then it is a gen IE */
        if (((pvendor_ie->element_id == WPA_IE)
             && (!memcmp(pvendor_ie->oui, wpa_oui, sizeof(wpa_oui))))
            || (pvendor_ie->element_id == RSN_IE)
            ) {

            /* IE is a WPA/WPA2 IE so call set_wpa function */
            ret = wlan_set_wpa_ie_helper(priv, ie_data_ptr, ie_len);
            priv->wps.session_enable = MFALSE;
        } else {
            /* 
             * Verify that the passed length is not larger than the available 
             * space remaining in the buffer
             */
            if (ie_len < (sizeof(priv->gen_ie_buf) - priv->gen_ie_buf_len)) {

                /* Test to see if it is a WPS IE, if so, enable wps session
                   flag */
                pvendor_ie = (IEEEtypes_VendorHeader_t *) ie_data_ptr;
                if ((pvendor_ie->element_id == WPS_IE)
                    && (!memcmp(pvendor_ie->oui, wps_oui, sizeof(wps_oui)))) {
                    priv->wps.session_enable = MTRUE;
                    PRINTM(INFO, "WPS Session Enabled.\n");
                }

                /* Append the passed data to the end of the genIeBuffer */
                memcpy(priv->gen_ie_buf + priv->gen_ie_buf_len, ie_data_ptr,
                       ie_len);
                /* Increment the stored buffer length by the size passed */
                priv->gen_ie_buf_len += ie_len;
            } else {
                /* Passed data does not fit in the remaining buffer space */
                ret = MLAN_STATUS_FAILURE;
            }
        }
    }

    /* Return MLAN_STATUS_SUCCESS, or MLAN_STATUS_FAILURE for error case */
    LEAVE();
    return ret;
}

/** 
 *  @brief  Get IE buffer from driver 
 *   
 *  Used to pass an opaque block of data, expected to be IEEE IEs,
 *    back to the application.  Currently the data block passed
 *    back to the application is the saved association response retrieved 
 *    from the firmware.
 *
 *  @param priv         A pointer to mlan_private structure
 *  @param ie_data_ptr  A pointer to the IE or IE block
 *  @param ie_len_ptr   In/Out parameter pointer for the buffer length passed 
 *                      in ie_data_ptr and the resulting data length copied
 *
 *  @return             MLAN_STATUS_SUCCESS --success, otherwise fail
 */
int
wlan_get_gen_ie_helper(mlan_private * priv, t_u8 * ie_data_ptr,
                       t_u16 * ie_len_ptr)
{
    IEEEtypes_AssocRsp_t *passoc_rsp;
    int copy_size;

    ENTER();

    if ((priv->media_connected == MFALSE) || (priv->assoc_rsp_size == 0)) {
        *ie_len_ptr = 0;
        LEAVE();
        return MLAN_STATUS_SUCCESS;
    }

    passoc_rsp = (IEEEtypes_AssocRsp_t *) priv->assoc_rsp_buf;

    /* 
     * Set the amount to copy back to the application as the minimum of the 
     *   available IE data or the buffer provided by the application
     */
    copy_size = (priv->assoc_rsp_size - sizeof(passoc_rsp->capability) -
                 -sizeof(passoc_rsp->status_code) - sizeof(passoc_rsp->a_id));
    copy_size = MIN(copy_size, *ie_len_ptr);

    /* Copy the IEEE TLVs in the assoc response back to the application */
    memcpy(ie_data_ptr, (t_u8 *) passoc_rsp->ie_buffer, copy_size);
    /* Returned copy length */
    *ie_len_ptr = copy_size;
    /* No error on return */
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Set/Get WWS mode
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_misc_ioctl_wws_cfg(IN pmlan_adapter pmadapter,
                        IN pmlan_ioctl_req pioctl_req)
{
    pmlan_private pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_ds_misc_cfg *misc_cfg = MNULL;
    t_u16 cmd_action = 0;
    t_u32 enable = 0;

    ENTER();

    if (!wlan_is_cmd_allowed(pmpriv)) {
        PRINTM(MSG, "WWS_CFG: not allowed\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    if (pioctl_req->buf_len < sizeof(mlan_ds_misc_cfg)) {
        PRINTM(WARN, "MLAN IOCTL information buffer length is too short.\n");
        pioctl_req->data_read_written = 0;
        pioctl_req->buf_len_needed = sizeof(mlan_ds_misc_cfg);
        ret = MLAN_STATUS_RESOURCE;
        goto exit;
    }

    misc_cfg = (mlan_ds_misc_cfg *) pioctl_req->pbuf;
    if (pioctl_req->action == MLAN_ACT_SET)
        cmd_action = HostCmd_ACT_GEN_SET;
    else
        cmd_action = HostCmd_ACT_GEN_GET;

    enable = misc_cfg->param.wws_cfg;

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv,
                           HostCmd_CMD_802_11_SNMP_MIB,
                           cmd_action,
                           WwsMode_i, (t_void *) pioctl_req, &enable);

    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;

  exit:
    LEAVE();
    return ret;

}

/** 
 *  @brief Set/Get 11D status
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_11d_cfg_ioctl_enable(IN pmlan_adapter pmadapter,
                          IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_ds_11d_cfg *pcfg_11d = MNULL;

    ENTER();

    pcfg_11d = (mlan_ds_11d_cfg *) pioctl_req->pbuf;

    if (pioctl_req->action == MLAN_ACT_SET) {
        if (!wlan_is_cmd_allowed(pmpriv)) {
            PRINTM(MSG, "11D_ENABLE: not allowed\n");
            ret = MLAN_STATUS_FAILURE;
            goto exit;
        }
        PRINTM(INFO, "11D: 11dcfg SET=%d\n", pcfg_11d->param.enable_11d);

        /* Compare with current settings */
        if (wlan_11d_get_state(pmpriv) != pcfg_11d->param.enable_11d) {
            ret =
                wlan_11d_enable(pmpriv, pioctl_req,
                                (state_11d_t) pcfg_11d->param.enable_11d);
            if (ret == MLAN_STATUS_SUCCESS)
                ret = MLAN_STATUS_PENDING;
        } else {
            PRINTM(INFO, "11D: same as current setting, do nothing\n");
        }
    } else {
        pcfg_11d->param.enable_11d = (t_u32) wlan_11d_get_state(pmpriv);
        pioctl_req->data_read_written = sizeof(t_u32) + MLAN_SUB_COMMAND_SIZE;
        PRINTM(INFO, "11D: 11dcfg GET=%d\n", pcfg_11d->param.enable_11d);
    }

  exit:
    LEAVE();
    return ret;
}

/** 
 *  @brief 11D configuration handler
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_11d_cfg_ioctl(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status status = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_ds_11d_cfg *pcfg_11d = MNULL;

    ENTER();

    if (pioctl_req->buf_len < sizeof(mlan_ds_11d_cfg)) {
        PRINTM(WARN, "MLAN bss IOCTL length is too short.\n");
        pioctl_req->data_read_written = 0;
        pioctl_req->buf_len_needed = sizeof(mlan_ds_11d_cfg);
        status = MLAN_STATUS_RESOURCE;
        goto exit;
    }

    if (!wlan_is_cmd_allowed(pmpriv)) {
        PRINTM(MSG, "SUPPLICANT_PROFILE: not allowed\n");
        status = MLAN_STATUS_FAILURE;
        goto exit;
    }

    pcfg_11d = (mlan_ds_11d_cfg *) pioctl_req->pbuf;
    switch (pcfg_11d->sub_command) {
    case MLAN_OID_11D_CFG_ENABLE:
        status = wlan_11d_cfg_ioctl_enable(pmadapter, pioctl_req);
        break;
    default:
        status = MLAN_STATUS_FAILURE;
        break;
    }

  exit:
    LEAVE();
    return status;
}

/** 
 *  @brief WPS configuration handler
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_wps_cfg_ioctl(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status status = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_ds_wps_cfg *pwps = MNULL;

    ENTER();

    if (pioctl_req->buf_len < sizeof(mlan_ds_wps_cfg)) {
        PRINTM(WARN, "MLAN bss IOCTL length is too short.\n");
        pioctl_req->data_read_written = 0;
        pioctl_req->buf_len_needed = sizeof(mlan_ds_wps_cfg);
        LEAVE();
        return MLAN_STATUS_RESOURCE;
    }

    pwps = (mlan_ds_wps_cfg *) pioctl_req->pbuf;
    switch (pwps->sub_command) {
    case MLAN_OID_WPS_CFG_SESSION:
        if (pioctl_req->action == MLAN_ACT_SET) {
            if (pwps->param.wps_session == MLAN_WPS_CFG_SESSION_START ||
                pwps->param.wps_session == MLAN_WPS_CFG_SESSION_END)
                pmpriv->wps.session_enable = pwps->param.wps_session;
            else
                pmpriv->wps.session_enable = MLAN_WPS_CFG_SESSION_END;
        } else {
            pwps->param.wps_session = (t_u32) pmpriv->wps.session_enable;
            pioctl_req->data_read_written = sizeof(t_u32);
            PRINTM(INFO, "wpscfg GET=%d\n", pwps->param.wps_session);
        }
        break;
    default:
        status = MLAN_STATUS_FAILURE;
        break;
    }

    LEAVE();
    return status;
}

/** 
 *  @brief Read/write adapter register
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_reg_mem_ioctl_reg_rw(IN pmlan_adapter pmadapter,
                          IN pmlan_ioctl_req pioctl_req)
{
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_ds_reg_mem *reg_mem = MNULL;
    mlan_status ret = MLAN_STATUS_SUCCESS;
    t_u16 cmd_action = 0, cmd_no;

    ENTER();

    if (!wlan_is_cmd_allowed(pmpriv)) {
        PRINTM(MSG, "REG_RW: not allowed\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    reg_mem = (mlan_ds_reg_mem *) pioctl_req->pbuf;
    if (pioctl_req->action == MLAN_ACT_GET)
        cmd_action = HostCmd_ACT_GEN_GET;
    else
        cmd_action = HostCmd_ACT_GEN_SET;

    switch (reg_mem->param.reg_rw.type) {
    case MLAN_REG_MAC:
        cmd_no = HostCmd_CMD_MAC_REG_ACCESS;
        break;
    case MLAN_REG_BBP:
        cmd_no = HostCmd_CMD_BBP_REG_ACCESS;
        break;
    case MLAN_REG_RF:
        cmd_no = HostCmd_CMD_RF_REG_ACCESS;
        break;
    case MLAN_REG_PMIC:
        cmd_no = HostCmd_CMD_PMIC_REG_ACCESS;
        break;
    default:
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv, cmd_no, cmd_action,
                           0, (t_void *) pioctl_req,
                           (t_void *) & reg_mem->param.reg_rw);

    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;

  exit:
    LEAVE();
    return ret;
}

/** 
 *  @brief Read the EEPROM contents of the card 
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_reg_mem_ioctl_read_eeprom(IN pmlan_adapter pmadapter,
                               IN pmlan_ioctl_req pioctl_req)
{
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_ds_reg_mem *reg_mem = MNULL;
    mlan_status ret = MLAN_STATUS_SUCCESS;
    t_u16 cmd_action = 0;

    ENTER();

    if (!wlan_is_cmd_allowed(pmpriv)) {
        PRINTM(MSG, "RD_EEPROM: not allowed\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }
    reg_mem = (mlan_ds_reg_mem *) pioctl_req->pbuf;
    if (pioctl_req->action == MLAN_ACT_GET)
        cmd_action = HostCmd_ACT_GEN_GET;

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv,
                           HostCmd_CMD_802_11_EEPROM_ACCESS,
                           cmd_action, 0, (t_void *) pioctl_req,
                           (t_void *) & reg_mem->param.rd_eeprom);

    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;

  exit:
    LEAVE();
    return ret;
}

/** 
 *  @brief register memory access handler
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_reg_mem_ioctl(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status status = MLAN_STATUS_SUCCESS;
    mlan_ds_reg_mem *reg_mem = MNULL;

    ENTER();

    if (pioctl_req->buf_len < sizeof(mlan_ds_reg_mem)) {
        PRINTM(WARN, "MLAN REG_MEM IOCTL length is too short\n");
        pioctl_req->data_read_written = 0;
        pioctl_req->buf_len_needed = sizeof(mlan_ds_reg_mem);
        LEAVE();
        return MLAN_STATUS_RESOURCE;
    }
    reg_mem = (mlan_ds_reg_mem *) pioctl_req->pbuf;
    switch (reg_mem->sub_command) {
    case MLAN_OID_REG_RW:
        status = wlan_reg_mem_ioctl_reg_rw(pmadapter, pioctl_req);
        break;
    case MLAN_OID_EEPROM_RD:
        status = wlan_reg_mem_ioctl_read_eeprom(pmadapter, pioctl_req);
        break;
    default:
        status = MLAN_STATUS_FAILURE;
        break;
    }
    LEAVE();
    return status;
}

/** 
 *  @brief Set/Get generic IE
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_misc_ioctl_gen_ie(IN pmlan_adapter pmadapter,
                       IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_ds_misc_cfg *misc = MNULL;

    ENTER();

    misc = (mlan_ds_misc_cfg *) pioctl_req->pbuf;
    switch (misc->param.gen_ie.type) {
    case MLAN_IE_TYPE_GEN_IE:
        if (pioctl_req->action == MLAN_ACT_GET) {
            misc->param.gen_ie.len = pmpriv->wpa_ie_len;
            memcpy(misc->param.gen_ie.ie_data, pmpriv->wpa_ie,
                   misc->param.gen_ie.len);
        } else {
            wlan_set_gen_ie_helper(pmpriv, misc->param.gen_ie.ie_data,
                                   misc->param.gen_ie.len);
        }
        break;
    case MLAN_IE_TYPE_ARP_FILTER:
        memset(pmadapter->arp_filter, 0, sizeof(pmadapter->arp_filter));
        if (misc->param.gen_ie.len > ARP_FILTER_MAX_BUF_SIZE) {
            pmadapter->arp_filter_size = 0;
            PRINTM(ERROR, "Invalid ARP Filter Size\n");
            ret = MLAN_STATUS_FAILURE;
        } else {
            memcpy(pmadapter->arp_filter, misc->param.gen_ie.ie_data,
                   misc->param.gen_ie.len);
            pmadapter->arp_filter_size = misc->param.gen_ie.len;
            HEXDUMP("ArpFilter", pmadapter->arp_filter,
                    pmadapter->arp_filter_size);
        }
        break;
    default:
        PRINTM(ERROR, "Invalid IE type\n");
        ret = MLAN_STATUS_FAILURE;
    }
    pioctl_req->data_read_written =
        sizeof(mlan_ds_misc_gen_ie) + MLAN_SUB_COMMAND_SIZE;
    LEAVE();
    return ret;
}

/** 
 *  @brief Set/Get region code
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_misc_ioctl_region(IN pmlan_adapter pmadapter,
                       IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_ds_misc_cfg *misc = MNULL;
    int i;

    ENTER();

    misc = (mlan_ds_misc_cfg *) pioctl_req->pbuf;
    if (pioctl_req->action == MLAN_ACT_GET) {
        misc->param.region_code = pmadapter->region_code;
    } else {
        for (i = 0; i < MRVDRV_MAX_REGION_CODE; i++) {
            /* Use the region code to search for the index */
            if (misc->param.region_code == region_code_index[i]) {
                pmadapter->region_code = misc->param.region_code;
                break;
            }
        }
        /* It's unidentified region code */
        if (i >= MRVDRV_MAX_REGION_CODE) {
            PRINTM(ERROR, "Region Code not identified\n");
            LEAVE();
            return MLAN_STATUS_FAILURE;
        }
        if (wlan_set_regiontable
            (pmpriv, pmadapter->region_code, pmadapter->config_bands)) {
            ret = MLAN_STATUS_FAILURE;
        }

    }
    pioctl_req->data_read_written = sizeof(t_u32) + MLAN_SUB_COMMAND_SIZE;

    LEAVE();
    return ret;
}

/** 
 *  @brief Perform warm reset
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_misc_ioctl_warm_reset(IN pmlan_adapter pmadapter,
                           IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret = MLAN_STATUS_PENDING;
    pmlan_private pmpriv = pmadapter->priv[pioctl_req->bss_num];
    pmlan_callbacks pcb = &pmadapter->callbacks;
    pmlan_buffer pmbuf;
    t_s32 i = 0;

    ENTER();

    if (!wlan_is_cmd_allowed(pmpriv)) {
        PRINTM(MSG, "WARM_RESET: not allowed\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    /** Init all the head nodes and free all the locks here */
    for (i = 0; i < MLAN_MAX_BSS_NUM; i++) {
        pmpriv = pmadapter->priv[i];
        if (pmadapter->priv[i]) {
            wlan_wmm_cleanup_node(pmpriv);
            pcb->moal_free_lock(pmpriv->rx_pkt_lock);
            pcb->moal_free_lock(pmpriv->wmm.ra_list_spinlock);
        }
    }

    while ((pmbuf =
            (pmlan_buffer) util_dequeue_list(&pmadapter->rx_data_queue,
                                             pcb->moal_spin_lock,
                                             pcb->moal_spin_unlock))) {
        pcb->moal_recv_complete(pmadapter->pmoal_handle, pmbuf,
                                MLAN_USB_EP_DATA, MLAN_STATUS_FAILURE);
    }
    util_free_list_head(&pmadapter->rx_data_queue, pcb->moal_free_lock);

    /* Initialize adapter structure */
    wlan_init_adapter(pmadapter);

    /* Initialize private structures */
    for (i = 0; i < MLAN_MAX_BSS_NUM; i++) {
        pmpriv = pmadapter->priv[i];
        if (pmadapter->priv[i])
            wlan_init_priv(pmpriv);
    }

    pmpriv = pmadapter->priv[pioctl_req->bss_num];
    /* Restart the firmware */
    wlan_prepare_cmd(pmpriv, HostCmd_CMD_FUNC_SHUTDOWN,
                     HostCmd_ACT_GEN_SET, 0, MNULL, MNULL);
    wlan_prepare_cmd(pmpriv, HostCmd_CMD_FUNC_INIT,
                     HostCmd_ACT_GEN_SET, 0, pioctl_req, MNULL);

    /* Issue commands to initialize firmware */
    ret = wlan_init_cmd(pmadapter);

  exit:
    LEAVE();
    return ret;
}

/** 
 *  @brief Send Heart Beat command
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_misc_ioctl_heart_beat(IN pmlan_adapter pmadapter,
                           IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    pmlan_private pmpriv = pmadapter->priv[pioctl_req->bss_num];
    pmlan_callbacks pcb = (pmlan_callbacks) & pmadapter->callbacks;
    mlan_ds_misc_cfg *misc = (mlan_ds_misc_cfg *) pioctl_req->pbuf;
    t_u16 cmd_action = 0;

    ENTER();

    if (!wlan_is_cmd_allowed(pmpriv)) {
        PRINTM(MSG, "HEART_BEAT: not allowed\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    cmd_action = HostCmd_ACT_GEN_SET;
    if (pioctl_req->action == MLAN_ACT_GET)
        cmd_action = HostCmd_ACT_GEN_GET;

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv,
                           HostCmd_CMD_HEART_BEAT,
                           cmd_action,
                           0,
                           (t_void *) pioctl_req,
                           (t_void *) & misc->param.heart_beat);
    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;

    if (pioctl_req->action == MLAN_ACT_SET) {
        /* Setting up host timer */
        if (misc->param.heart_beat.debug_for_host == MTRUE)
            misc->param.heart_beat.h2d_timer = 0;
        if (!(misc->param.heart_beat.h2d_timer)) {
            /**cancel host timer */
            if (pmadapter->hb_host_timer_is_set) {
                pcb->moal_stop_timer(pmadapter->hb_host_timer);
                pmadapter->hb_host_timer_is_set = MFALSE;
            }
        } else if (misc->param.heart_beat.h2d_timer != HB_CFG_NO_CHANGE) {
            if (pmadapter->hb_host_timer_is_set) {
                pcb->moal_stop_timer(pmadapter->hb_host_timer);
            }
            pmadapter->hb_host_time = misc->param.heart_beat.h2d_timer * 100;
            pcb->moal_start_timer(pmadapter->hb_host_timer,
                                  MFALSE, pmadapter->hb_host_time);
            pmadapter->hb_host_timer_is_set = MTRUE;
            pmadapter->host_to_dev_traffic = 0;
        }

        /* Setting up device timer */
        if (misc->param.heart_beat.debug_for_device == MTRUE)
            misc->param.heart_beat.d2h_timer = HB_CFG_NO_CHANGE;
        if (!(misc->param.heart_beat.d2h_timer)) {
            if (pmadapter->hb_dev_timer_is_set) {
                pcb->moal_stop_timer(pmadapter->hb_device_timer);
                pmadapter->hb_dev_timer_is_set = MFALSE;
            }
        } else if (misc->param.heart_beat.d2h_timer != HB_CFG_NO_CHANGE) {
            if (pmadapter->hb_dev_timer_is_set)
                pcb->moal_stop_timer(pmadapter->hb_device_timer);
            pmadapter->hb_dev_time = misc->param.heart_beat.d2h_timer * 100;
            pcb->moal_start_timer(pmadapter->hb_device_timer,
                                  MFALSE, pmadapter->hb_dev_time);
            pmadapter->hb_dev_timer_is_set = MTRUE;
            pmadapter->hb_dev_counter = 0;
        }
    }

  exit:
    LEAVE();
    return ret;
}

#ifdef MFG_CMD_SUPPORT
/** 
 *  @brief send mfg cmd
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_misc_ioctl_mfg_cmd(IN pmlan_adapter pmadapter,
                        IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_ds_misc_cfg *misc = MNULL;

    ENTER();

    if (!wlan_is_cmd_allowed(pmpriv)) {
        PRINTM(MSG, "MFG_COMMAND: not allowed\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    misc = (mlan_ds_misc_cfg *) pioctl_req->pbuf;

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv,
                           HostCmd_CMD_MFG_COMMAND,
                           0,
                           0,
                           (t_void *) pioctl_req,
                           (t_void *) & misc->param.mfgcmd);
    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;

  exit:
    LEAVE();
    return ret;
}
#endif

/** 
 *  @brief send host cmd
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_misc_ioctl_host_cmd(IN pmlan_adapter pmadapter,
                         IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_ds_misc_cfg *misc = MNULL;

    ENTER();

    if (!wlan_is_cmd_allowed(pmpriv)) {
        PRINTM(MSG, "HOST_COMMAND: not allowed\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    misc = (mlan_ds_misc_cfg *) pioctl_req->pbuf;

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv,
                           HostCmd_CMD_HOST_COMMAND,
                           0,
                           0,
                           (t_void *) pioctl_req,
                           (t_void *) & misc->param.hostcmd);
    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;

  exit:
    LEAVE();
    return ret;
}

/** 
 *  @brief Set/Get system clock configuration
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_misc_ioctl_sysclock(IN pmlan_adapter pmadapter,
                         IN pmlan_ioctl_req pioctl_req)
{
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_ds_misc_cfg *misc = MNULL;
    mlan_status ret = MLAN_STATUS_SUCCESS;
    t_u16 cmd_action = 0;

    ENTER();

    if (!wlan_is_cmd_allowed(pmpriv)) {
        PRINTM(MSG, "ECL_SYS_CLK_CFG: not allowed\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    misc = (mlan_ds_misc_cfg *) pioctl_req->pbuf;
    if (pioctl_req->action == MLAN_ACT_GET)
        cmd_action = HostCmd_ACT_GEN_GET;
    else
        cmd_action = HostCmd_ACT_GEN_SET;

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv,
                           HostCmd_CMD_ECL_SYSTEM_CLOCK_CONFIG,
                           cmd_action,
                           0,
                           (t_void *) pioctl_req,
                           (t_void *) & misc->param.sys_clock);

    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;

  exit:
    LEAVE();
    return ret;
}

/** Flag for VSIE */
#define VSIE_FLAG   1

/** Max total length of vendor specific IEs for scan/assoc/ad-hoc */
#define MAX_TOTAL_VSIE_LEN	512

/**
 *  @brief Get/Add/Remove the vendor specific IE
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_misc_ioctl_vendor_spec_ie(IN pmlan_adapter pmadapter,
                               IN pmlan_ioctl_req pioctl_req)
{
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_ds_misc_cfg *misc = MNULL;
    mlan_status ret = MLAN_STATUS_SUCCESS;
    t_u32 id, index, max_len;
    t_u16 mask_bak;
    t_u8 bit, len_bak;

    ENTER();

    if (!wlan_is_cmd_allowed(pmpriv)) {
        PRINTM(MSG, "VSIE_CFG: not allowed\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    misc = (mlan_ds_misc_cfg *) pioctl_req->pbuf;
    id = misc->param.vsie.id;
    switch (pioctl_req->action) {
    case MLAN_ACT_GET:
        misc->param.vsie.mask = (t_u32) pmpriv->vs_ie[id].mask;
        memcpy(misc->param.vsie.ie, pmpriv->vs_ie[id].ie,
               sizeof(misc->param.vsie.ie));
        break;
    case MLAN_ACT_SET:
        mask_bak = pmpriv->vs_ie[id].mask;
        pmpriv->vs_ie[id].mask = (t_u16) misc->param.vsie.mask;
        if (!pmpriv->vs_ie[id].mask) {
            memset(&pmpriv->vs_ie[id], 0, sizeof(vendor_spec_cfg_ie));
        } else {
            /* Check if the total length of VS IEs is over limitation */
            len_bak = pmpriv->vs_ie[id].ie[1];
            pmpriv->vs_ie[id].ie[1] = misc->param.vsie.ie[1];
            for (bit = MLAN_VSIE_MASK_SCAN; bit <= MLAN_VSIE_MASK_ADHOC;
                 bit <<= 1) {
                max_len = 0;
                for (index = 0; index < MLAN_MAX_VSIE_NUM; index++) {
                    if (pmpriv->vs_ie[index].mask & bit)
                        max_len += (pmpriv->vs_ie[index].ie[1] + 2);
                }
                if (max_len > MAX_TOTAL_VSIE_LEN) {
                    PRINTM(ERROR,
                           "Total length of VS IEs for %s (%d) is over limitation (%d)\n",
                           (bit == MLAN_VSIE_MASK_SCAN) ? "scan" : (bit ==
                                                                    MLAN_VSIE_MASK_ASSOC)
                           ? "assoc" : "ad-hoc", max_len, MAX_TOTAL_VSIE_LEN);
                    pmpriv->vs_ie[id].mask = mask_bak;
                    pmpriv->vs_ie[id].ie[1] = len_bak;
                    ret = MLAN_STATUS_FAILURE;
                    break;
                }
            }

            if (!ret) {
                pmpriv->vs_ie[id].flag = VSIE_FLAG;
                memcpy(pmpriv->vs_ie[id].ie, misc->param.vsie.ie,
                       sizeof(misc->param.vsie.ie));
                pmpriv->vs_ie[id].ie[0] = VS_IE;
            }
        }
        break;
    default:
        break;
    }

  exit:
    LEAVE();
    return ret;
}

/** 
 *  @brief Miscellaneous configuration handler
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_misc_cfg_ioctl(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status status = MLAN_STATUS_SUCCESS;
    mlan_ds_misc_cfg *misc = MNULL;

    ENTER();

    if (pioctl_req->buf_len < sizeof(mlan_ds_misc_cfg)) {
        PRINTM(WARN, "MLAN bss IOCTL length is too short.\n");
        pioctl_req->data_read_written = 0;
        pioctl_req->buf_len_needed = sizeof(mlan_ds_misc_cfg);
        LEAVE();
        return MLAN_STATUS_RESOURCE;
    }
    misc = (mlan_ds_misc_cfg *) pioctl_req->pbuf;
    switch (misc->sub_command) {
    case MLAN_OID_MISC_GEN_IE:
        status = wlan_misc_ioctl_gen_ie(pmadapter, pioctl_req);
        break;
    case MLAN_OID_MISC_REGION:
        status = wlan_misc_ioctl_region(pmadapter, pioctl_req);
        break;
    case MLAN_OID_MISC_WARM_RESET:
        status = wlan_misc_ioctl_warm_reset(pmadapter, pioctl_req);
        break;
    case MLAN_OID_MISC_HEART_BEAT:
        status = wlan_misc_ioctl_heart_beat(pmadapter, pioctl_req);
        break;
#ifdef MFG_CMD_SUPPORT
    case MLAN_OID_MISC_MFG_CMD:
        status = wlan_misc_ioctl_mfg_cmd(pmadapter, pioctl_req);
        break;
#endif
    case MLAN_OID_MISC_HOST_CMD:
        status = wlan_misc_ioctl_host_cmd(pmadapter, pioctl_req);
        break;
    case MLAN_OID_MISC_SYS_CLOCK:
        status = wlan_misc_ioctl_sysclock(pmadapter, pioctl_req);
        break;
    case MLAN_OID_MISC_WWS:
        status = wlan_misc_ioctl_wws_cfg(pmadapter, pioctl_req);
        break;
    case MLAN_OID_MISC_VS_IE:
        status = wlan_misc_ioctl_vendor_spec_ie(pmadapter, pioctl_req);
        break;
    default:
        status = MLAN_STATUS_FAILURE;
        break;
    }
    LEAVE();
    return status;
}

/** 
 *  @brief Set/Get scan configuration parameter
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *  @param action	    Set/Get
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_set_get_scan_cfg(IN pmlan_adapter pmadapter,
                      IN pmlan_ioctl_req pioctl_req, IN t_u32 action)
{
    mlan_ds_scan *scan = MNULL;

    ENTER();

    scan = (mlan_ds_scan *) pioctl_req->pbuf;
    if (action == MLAN_ACT_GET) {
        scan->param.scan_cfg.scan_type = (t_u32) pmadapter->scan_type + 1;
        scan->param.scan_cfg.scan_mode = pmadapter->scan_mode;
        scan->param.scan_cfg.scan_probe = (t_u32) pmadapter->scan_probes;
        scan->param.scan_cfg.scan_time.specific_scan_time =
            (t_u32) pmadapter->specific_scan_time;
        scan->param.scan_cfg.scan_time.active_scan_time =
            (t_u32) pmadapter->active_scan_time;
        scan->param.scan_cfg.scan_time.passive_scan_time =
            (t_u32) pmadapter->passive_scan_time;
    } else {
        if (scan->param.scan_cfg.scan_type)
            pmadapter->scan_type = (t_u8) scan->param.scan_cfg.scan_type - 1;
        if (scan->param.scan_cfg.scan_mode)
            pmadapter->scan_mode = scan->param.scan_cfg.scan_mode;
        if (scan->param.scan_cfg.scan_probe)
            pmadapter->scan_probes = (t_u16) scan->param.scan_cfg.scan_probe;
        if (scan->param.scan_cfg.scan_time.specific_scan_time)
            pmadapter->specific_scan_time =
                (t_u16) scan->param.scan_cfg.scan_time.specific_scan_time;
        if (scan->param.scan_cfg.scan_time.active_scan_time)
            pmadapter->active_scan_time =
                (t_u16) scan->param.scan_cfg.scan_time.active_scan_time;
        if (scan->param.scan_cfg.scan_time.passive_scan_time)
            pmadapter->passive_scan_time =
                (t_u16) scan->param.scan_cfg.scan_time.passive_scan_time;
    }
    pioctl_req->data_read_written = sizeof(t_u32) + MLAN_SUB_COMMAND_SIZE;

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Set/Get scan
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status
wlan_scan_ioctl(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    pmlan_private pmpriv = pmadapter->priv[pioctl_req->bss_num];
    mlan_status status = MLAN_STATUS_SUCCESS;
    mlan_ds_scan *pscan;

    ENTER();

    if (!wlan_is_cmd_allowed(pmpriv)) {
        PRINTM(MSG, "%s: not allowed\n", __FUNCTION__);
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }

    pscan = (mlan_ds_scan *) pioctl_req->pbuf;

    /* Set scan */
    if (pioctl_req->action == MLAN_ACT_SET) {
        switch (pscan->sub_command) {
        case MLAN_OID_SCAN_NORMAL:
            status = wlan_scan_networks(pmpriv, pioctl_req, MNULL);
            break;
        case MLAN_OID_SCAN_SPECIFIC_SSID:
            status = wlan_scan_specific_ssid(pmpriv, pioctl_req,
                                             &pscan->param.scan_req.scan_ssid);
            break;
        case MLAN_OID_SCAN_USER_CONFIG:
            status =
                wlan_scan_networks(pmpriv, pioctl_req,
                                   (wlan_user_scan_cfg *) pscan->param.
                                   user_scan.scan_cfg_buf);
            break;
        case MLAN_OID_SCAN_CONFIG:
            status = wlan_set_get_scan_cfg(pmadapter, pioctl_req, MLAN_ACT_SET);
            break;
        default:
            status = MLAN_STATUS_FAILURE;
            break;
        }

        if ((status == MLAN_STATUS_SUCCESS) &&
            (pscan->sub_command != MLAN_OID_SCAN_CONFIG)) {
            PRINTM(INFO, "wlan_scan_ioctl: return MLAN_STATUS_PENDING\n");
            status = MLAN_STATUS_PENDING;
        }
    }
    /* Get scan */
    else {
        if (pscan->sub_command == MLAN_OID_SCAN_CONFIG) {
            status = wlan_set_get_scan_cfg(pmadapter, pioctl_req, MLAN_ACT_GET);
        } else if (pscan->sub_command == MLAN_OID_SCAN_SPECIFIC_SSID) {
            pscan->param.scan_resp.num_in_scan_table =
                pmadapter->num_in_scan_table;
            pscan->param.scan_resp.pscan_table =
                (t_u8 *) & pmpriv->curr_bss_params.bss_descriptor;
            pioctl_req->data_read_written =
                sizeof(mlan_scan_resp) + MLAN_SUB_COMMAND_SIZE;
        } else {
            pscan->param.scan_resp.num_in_scan_table =
                pmadapter->num_in_scan_table;
            pscan->param.scan_resp.pscan_table =
                (t_u8 *) pmadapter->pscan_table;
            pioctl_req->data_read_written =
                sizeof(mlan_scan_resp) + MLAN_SUB_COMMAND_SIZE;
        }
    }

    LEAVE();
    return status;
}

/********************************************************
                Global Functions
********************************************************/

/** 
 *  @brief This function checks if the command is allowed.
 * 
 *  @param pmpriv       A pointer to mlan_private structure
 *
 *  @return             MTRUE or MFALSE
 */
t_u8
wlan_is_cmd_allowed(IN mlan_private * pmpriv)
{
    t_u8 ret = MTRUE;

    ENTER();

    if (pmpriv->adapter->is_hs_configured) {
        PRINTM(INFO, "IOCTLS called when WLAN access is blocked\n");
        ret = MFALSE;
    }

    LEAVE();
    return ret;
}

/** 
 *  @brief MLAN ioctl handler
 *
 *  @param adapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
mlan_status
mlan_ioctl(IN t_void * adapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status status = MLAN_STATUS_SUCCESS;
    pmlan_adapter pmadapter = (pmlan_adapter) adapter;

    ENTER();

    if (pioctl_req == MNULL) {
        PRINTM(ERROR, "MLAN IOCTL information buffer is MNULL.\n");
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }
    if (pioctl_req->action == MLAN_ACT_CANCEL) {
        wlan_cancel_pending_ioctl(pmadapter, pioctl_req);
        LEAVE();
        return MLAN_STATUS_SUCCESS;
    }

    switch (pioctl_req->req_id) {
    case MLAN_IOCTL_SCAN:
        status = wlan_scan_ioctl(pmadapter, pioctl_req);
        break;
    case MLAN_IOCTL_BSS:
        status = wlan_bss_ioctl(pmadapter, pioctl_req);
        break;
    case MLAN_IOCTL_RADIO_CFG:
        status = wlan_radio_ioctl(pmadapter, pioctl_req);
        break;
    case MLAN_IOCTL_SNMP_MIB:
        status = wlan_snmp_mib_ioctl(pmadapter, pioctl_req);
        break;
    case MLAN_IOCTL_GET_INFO:
        status = wlan_get_info_ioctl(pmadapter, pioctl_req);
        break;
    case MLAN_IOCTL_SEC_CFG:
        status = wlan_sec_cfg_ioctl(pmadapter, pioctl_req);
        break;
    case MLAN_IOCTL_RATE:
        status = wlan_rate_ioctl(pmadapter, pioctl_req);
        break;
    case MLAN_IOCTL_POWER_CFG:
        status = wlan_power_ioctl(pmadapter, pioctl_req);
        break;
    case MLAN_IOCTL_PM_CFG:
        status = wlan_pm_ioctl(pmadapter, pioctl_req);
        break;
    case MLAN_IOCTL_WMM_CFG:
        status = wlan_wmm_cfg_ioctl(pmadapter, pioctl_req);
        break;
    case MLAN_IOCTL_WPS_CFG:
        status = wlan_wps_cfg_ioctl(pmadapter, pioctl_req);
        break;
    case MLAN_IOCTL_11N_CFG:
        status = wlan_11n_cfg_ioctl(pmadapter, pioctl_req);
        break;
    case MLAN_IOCTL_11D_CFG:
        status = wlan_11d_cfg_ioctl(pmadapter, pioctl_req);
        break;
    case MLAN_IOCTL_REG_MEM:
        status = wlan_reg_mem_ioctl(pmadapter, pioctl_req);
        break;
    case MLAN_IOCTL_MISC_CFG:
        status = wlan_misc_cfg_ioctl(pmadapter, pioctl_req);
        break;
    default:
        status = MLAN_STATUS_FAILURE;
        break;
    }

    LEAVE();
    return status;
}
