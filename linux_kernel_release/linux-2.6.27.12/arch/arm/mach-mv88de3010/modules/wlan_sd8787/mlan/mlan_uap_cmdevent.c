/** @file mlan_uap_cmdevent.c
 *
 *  @brief This file contains the handling of AP mode command and event
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
#include "mlan_11n.h"

/********************************************************
    Local Functions
********************************************************/
/** 
 *  @brief This function handles the command response error
 *  
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to command buffer
 *
 *  @return             N/A
 */
void
uap_process_cmdresp_error(mlan_private * pmpriv, HostCmd_DS_COMMAND * resp,
                          mlan_ioctl_req * pioctl_buf)
{
    mlan_adapter *pmadapter = pmpriv->adapter;

    ENTER();

    PRINTM(MERROR, "CMD_RESP: cmd %#x error, result=%#x\n", resp->command,
           resp->result);
    if (pioctl_buf)
        pioctl_buf->status_code = resp->result;
    /* 
     * Handling errors here
     */
    wlan_insert_cmd_to_free_q(pmadapter, pmadapter->curr_cmd);

    wlan_request_cmd_lock(pmadapter);
    pmadapter->curr_cmd = MNULL;
    wlan_release_cmd_lock(pmadapter);

    LEAVE();
    return;
}

/** 
 * @brief This function prepares command of hs_cfg.
 *   
 * @param pmpriv       A pointer to mlan_private structure
 * @param cmd          A pointer to HostCmd_DS_COMMAND structure
 * @param cmd_action   The action: GET or SET
 * @param pdata_buf    A pointer to data buffer
 *
 * @return             MLAN_STATUS_SUCCESS
 */
static mlan_status
wlan_uap_cmd_802_11_hs_cfg(IN pmlan_private pmpriv,
                           IN HostCmd_DS_COMMAND * cmd,
                           IN t_u16 cmd_action,
                           IN HostCmd_DS_802_11_HS_CFG_ENH * pdata_buf)
{
    HostCmd_DS_802_11_HS_CFG_ENH *phs_cfg = &cmd->params.opt_hs_cfg;

    ENTER();
    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_802_11_HS_CFG_ENH);
    cmd->size =
        wlan_cpu_to_le16(S_DS_GEN + sizeof(HostCmd_DS_802_11_HS_CFG_ENH));

    if (pdata_buf == MNULL) {
        phs_cfg->action = HS_ACTIVATE;
        phs_cfg->params.hs_activate.resp_ctrl = RESP_NEEDED;
    } else {
        phs_cfg->action = HS_CONFIGURE;
        phs_cfg->params.hs_config.conditions =
            wlan_cpu_to_le32(pdata_buf->params.hs_config.conditions);
        phs_cfg->params.hs_config.gpio = pdata_buf->params.hs_config.gpio;
        phs_cfg->params.hs_config.gap = pdata_buf->params.hs_config.gap;
        PRINTM(MCMND, "HS_CFG_CMD: condition:0x%x gpio:0x%x gap:0x%x\n",
               phs_cfg->params.hs_config.conditions,
               phs_cfg->params.hs_config.gpio, phs_cfg->params.hs_config.gap);
    }
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function prepares command for config uap settings
 *  
 *  @param pmpriv		A pointer to mlan_private structure
 *  @param cmd	   		A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   the action: GET or SET
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure 
 *  @return         MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_uap_cmd_ap_config(pmlan_private pmpriv,
                       IN HostCmd_DS_COMMAND * cmd,
                       IN t_u16 cmd_action, IN pmlan_ioctl_req pioctl_buf)
{
    mlan_ds_bss *bss = MNULL;
    HostCmd_DS_SYS_CONFIG *sys_config =
        (HostCmd_DS_SYS_CONFIG *) & cmd->params.sys_config;
    t_u8 *tlv = MNULL;
    MrvlIEtypes_MacAddr_t *tlv_mac = MNULL;
    MrvlIEtypes_SsIdParamSet_t *tlv_ssid = MNULL;
    MrvlIEtypes_beacon_period_t *tlv_beacon_period = MNULL;
    MrvlIEtypes_dtim_period_t *tlv_dtim_period = MNULL;
    MrvlIEtypes_RatesParamSet_t *tlv_rates = MNULL;
    MrvlIEtypes_tx_rate_t *tlv_txrate = MNULL;
    MrvlIEtypes_mcbc_rate_t *tlv_mcbc_rate = MNULL;
    MrvlIEtypes_tx_power_t *tlv_tx_power = MNULL;
    MrvlIEtypes_bcast_ssid_t *tlv_bcast_ssid = MNULL;
    MrvlIEtypes_antenna_mode_t *tlv_antenna = MNULL;
    MrvlIEtypes_pkt_forward_t *tlv_pkt_forward = MNULL;
    MrvlIEtypes_max_sta_count_t *tlv_sta_count = MNULL;
    MrvlIEtypes_sta_ageout_t *tlv_sta_ageout = MNULL;
    MrvlIEtypes_ps_sta_ageout_t *tlv_ps_sta_ageout = MNULL;
    MrvlIEtypes_rts_threshold_t *tlv_rts_threshold = MNULL;
    MrvlIEtypes_frag_threshold_t *tlv_frag_threshold = MNULL;
    MrvlIEtypes_retry_limit_t *tlv_retry_limit = MNULL;
    MrvlIEtypes_eapol_pwk_hsk_timeout_t *tlv_pairwise_timeout = MNULL;
    MrvlIEtypes_eapol_pwk_hsk_retries_t *tlv_pairwise_retries = MNULL;
    MrvlIEtypes_eapol_gwk_hsk_timeout_t *tlv_groupwise_timeout = MNULL;
    MrvlIEtypes_eapol_gwk_hsk_retries_t *tlv_groupwise_retries = MNULL;
    MrvlIEtypes_mac_filter_t *tlv_mac_filter = MNULL;
    MrvlIEtypes_channel_band_t *tlv_chan_band = MNULL;
    MrvlIEtypes_ChanListParamSet_t *tlv_chan_list = MNULL;
    ChanScanParamSet_t *pscan_chan = MNULL;
    MrvlIEtypes_auth_type_t *tlv_auth_type = MNULL;
    MrvlIEtypes_encrypt_protocol_t *tlv_encrypt_protocol = MNULL;
    MrvlIEtypes_akmp_t *tlv_akmp = MNULL;
    MrvlIEtypes_cipher_t *tlv_cipher = MNULL;
    MrvlIEtypes_rsn_replay_prot_t *tlv_rsn_prot = MNULL;
    MrvlIEtypes_passphrase_t *tlv_passphrase = MNULL;
    MrvlIEtypes_group_rekey_time_t *tlv_rekey_time = MNULL;
    MrvlIEtypes_wep_key_t *tlv_wep_key = MNULL;
    MrvlIEtypes_radio_control_t *tlv_radio_control = MNULL;

    t_u32 cmd_size = 0;
    t_u8 zero_mac[] = { 0, 0, 0, 0, 0, 0 };
    t_u16 i;

    ENTER();
    if (pioctl_buf == MNULL) {
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }

    bss = (mlan_ds_bss *) pioctl_buf->pbuf;

    cmd->command = wlan_cpu_to_le16(HOST_CMD_APCMD_SYS_CONFIGURE);
    sys_config->action = wlan_cpu_to_le16(cmd_action);
    cmd_size = sizeof(HostCmd_DS_SYS_CONFIG) - 1 + S_DS_GEN;

    tlv = (t_u8 *) sys_config->tlv_buffer;
    if (memcmp
        (pmpriv->adapter, zero_mac, &bss->param.bss_config.mac_addr,
         MLAN_MAC_ADDR_LENGTH)) {
        tlv_mac = (MrvlIEtypes_MacAddr_t *) tlv;
        tlv_mac->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_MAC_ADDRESS);
        tlv_mac->header.len = wlan_cpu_to_le16(MLAN_MAC_ADDR_LENGTH);
        memcpy(pmpriv->adapter, tlv_mac->mac, &bss->param.bss_config.mac_addr,
               MLAN_MAC_ADDR_LENGTH);
        cmd_size += sizeof(MrvlIEtypes_MacAddr_t);
        tlv += sizeof(MrvlIEtypes_MacAddr_t);
    }

    if (bss->param.bss_config.ssid.ssid_len) {
        tlv_ssid = (MrvlIEtypes_SsIdParamSet_t *) tlv;
        tlv_ssid->header.type = wlan_cpu_to_le16(TLV_TYPE_SSID);
        tlv_ssid->header.len =
            wlan_cpu_to_le16((t_u16) bss->param.bss_config.ssid.ssid_len);
        memcpy(pmpriv->adapter, tlv_ssid->ssid, bss->param.bss_config.ssid.ssid,
               bss->param.bss_config.ssid.ssid_len);
        cmd_size +=
            sizeof(MrvlIEtypesHeader_t) + bss->param.bss_config.ssid.ssid_len;
        tlv +=
            sizeof(MrvlIEtypesHeader_t) + bss->param.bss_config.ssid.ssid_len;
    }

    if ((bss->param.bss_config.beacon_period >= MIN_BEACON_PERIOD) &&
        (bss->param.bss_config.beacon_period <= MAX_BEACON_PERIOD)) {
        tlv_beacon_period = (MrvlIEtypes_beacon_period_t *) tlv;
        tlv_beacon_period->header.type =
            wlan_cpu_to_le16(TLV_TYPE_UAP_BEACON_PERIOD);
        tlv_beacon_period->header.len = wlan_cpu_to_le16(sizeof(t_u16));
        tlv_beacon_period->beacon_period =
            wlan_cpu_to_le16(bss->param.bss_config.beacon_period);
        cmd_size += sizeof(MrvlIEtypes_beacon_period_t);
        tlv += sizeof(MrvlIEtypes_beacon_period_t);
    }

    if ((bss->param.bss_config.dtim_period >= MIN_DTIM_PERIOD) &&
        (bss->param.bss_config.dtim_period <= MAX_DTIM_PERIOD)) {
        tlv_dtim_period = (MrvlIEtypes_dtim_period_t *) tlv;
        tlv_dtim_period->header.type =
            wlan_cpu_to_le16(TLV_TYPE_UAP_DTIM_PERIOD);
        tlv_dtim_period->header.len = wlan_cpu_to_le16(sizeof(t_u8));
        tlv_dtim_period->dtim_period = bss->param.bss_config.dtim_period;
        cmd_size += sizeof(MrvlIEtypes_dtim_period_t);
        tlv += sizeof(MrvlIEtypes_dtim_period_t);
    }

    if (bss->param.bss_config.rates[0]) {
        tlv_rates = (MrvlIEtypes_RatesParamSet_t *) tlv;
        tlv_rates->header.type = wlan_cpu_to_le16(TLV_TYPE_RATES);
        for (i = 0; i < MAX_DATA_RATES && bss->param.bss_config.rates[i]; i++) {
            tlv_rates->rates[i] = bss->param.bss_config.rates[i];
        }
        tlv_rates->header.len = wlan_cpu_to_le16(i);
        cmd_size += sizeof(MrvlIEtypesHeader_t) + i;
        tlv += sizeof(MrvlIEtypesHeader_t) + i;
    }

    if (bss->param.bss_config.tx_data_rate <= DATA_RATE_54M) {
        tlv_txrate = (MrvlIEtypes_tx_rate_t *) tlv;
        tlv_txrate->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_TX_DATA_RATE);
        tlv_txrate->header.len = wlan_cpu_to_le16(sizeof(t_u16));
        tlv_txrate->tx_data_rate =
            wlan_cpu_to_le16(bss->param.bss_config.tx_data_rate);
        cmd_size += sizeof(MrvlIEtypes_tx_rate_t);
        tlv += sizeof(MrvlIEtypes_tx_rate_t);
    }

    if (bss->param.bss_config.mcbc_data_rate <= DATA_RATE_54M) {
        tlv_mcbc_rate = (MrvlIEtypes_mcbc_rate_t *) tlv;
        tlv_mcbc_rate->header.type =
            wlan_cpu_to_le16(TLV_TYPE_UAP_MCBC_DATA_RATE);
        tlv_mcbc_rate->header.len = wlan_cpu_to_le16(sizeof(t_u16));
        tlv_mcbc_rate->mcbc_data_rate =
            wlan_cpu_to_le16(bss->param.bss_config.mcbc_data_rate);
        cmd_size += sizeof(MrvlIEtypes_mcbc_rate_t);
        tlv += sizeof(MrvlIEtypes_mcbc_rate_t);
    }

    if (bss->param.bss_config.tx_power_level <= MAX_TX_POWER) {
        tlv_tx_power = (MrvlIEtypes_tx_power_t *) tlv;
        tlv_tx_power->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_TX_POWER);
        tlv_tx_power->header.len = wlan_cpu_to_le16(sizeof(t_u8));
        tlv_tx_power->tx_power = bss->param.bss_config.tx_power_level;
        cmd_size += sizeof(MrvlIEtypes_tx_power_t);
        tlv += sizeof(MrvlIEtypes_tx_power_t);
    }

    if (bss->param.bss_config.bcast_ssid_ctl <= MTRUE) {
        tlv_bcast_ssid = (MrvlIEtypes_bcast_ssid_t *) tlv;
        tlv_bcast_ssid->header.type =
            wlan_cpu_to_le16(TLV_TYPE_UAP_BCAST_SSID_CTL);
        tlv_bcast_ssid->header.len = wlan_cpu_to_le16(sizeof(t_u8));
        tlv_bcast_ssid->bcast_ssid_ctl = bss->param.bss_config.bcast_ssid_ctl;
        cmd_size += sizeof(MrvlIEtypes_bcast_ssid_t);
        tlv += sizeof(MrvlIEtypes_bcast_ssid_t);
    }

    if ((bss->param.bss_config.tx_antenna == ANTENNA_MODE_A) ||
        (bss->param.bss_config.tx_antenna == ANTENNA_MODE_B)) {
        tlv_antenna = (MrvlIEtypes_antenna_mode_t *) tlv;
        tlv_antenna->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_ANTENNA_CTL);
        tlv_antenna->header.len = wlan_cpu_to_le16(sizeof(t_u8) + sizeof(t_u8));
        tlv_antenna->which_antenna = TX_ANTENNA;
        tlv_antenna->antenna_mode = bss->param.bss_config.tx_antenna;
        cmd_size += sizeof(MrvlIEtypes_antenna_mode_t);
        tlv += sizeof(MrvlIEtypes_antenna_mode_t);
    }

    if ((bss->param.bss_config.rx_antenna == ANTENNA_MODE_A) ||
        (bss->param.bss_config.rx_antenna == ANTENNA_MODE_B)) {
        tlv_antenna = (MrvlIEtypes_antenna_mode_t *) tlv;
        tlv_antenna->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_ANTENNA_CTL);
        tlv_antenna->header.len = wlan_cpu_to_le16(sizeof(t_u8) + sizeof(t_u8));
        tlv_antenna->which_antenna = RX_ANTENNA;
        tlv_antenna->antenna_mode = bss->param.bss_config.rx_antenna;
        cmd_size += sizeof(MrvlIEtypes_antenna_mode_t);
        tlv += sizeof(MrvlIEtypes_antenna_mode_t);
    }

    if (bss->param.bss_config.pkt_forward_ctl <= MTRUE) {
        tlv_pkt_forward = (MrvlIEtypes_pkt_forward_t *) tlv;
        tlv_pkt_forward->header.type =
            wlan_cpu_to_le16(TLV_TYPE_UAP_PKT_FWD_CTL);
        tlv_pkt_forward->header.len = wlan_cpu_to_le16(sizeof(t_u8));
        tlv_pkt_forward->pkt_forward_ctl =
            bss->param.bss_config.pkt_forward_ctl;
        cmd_size += sizeof(MrvlIEtypes_pkt_forward_t);
        tlv += sizeof(MrvlIEtypes_pkt_forward_t);
    }

    if (bss->param.bss_config.max_sta_count <= MAX_STA_COUNT) {
        tlv_sta_count = (MrvlIEtypes_max_sta_count_t *) tlv;
        tlv_sta_count->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_MAX_STA_CNT);
        tlv_sta_count->header.len = wlan_cpu_to_le16(sizeof(t_u16));
        tlv_sta_count->max_sta_count =
            wlan_cpu_to_le16(bss->param.bss_config.max_sta_count);
        cmd_size += sizeof(MrvlIEtypes_max_sta_count_t);
        tlv += sizeof(MrvlIEtypes_max_sta_count_t);
    }

    if (((bss->param.bss_config.sta_ageout_timer >= MIN_STAGE_OUT_TIME) &&
         (bss->param.bss_config.sta_ageout_timer <= MAX_STAGE_OUT_TIME)) ||
        (bss->param.bss_config.sta_ageout_timer == 0)) {
        tlv_sta_ageout = (MrvlIEtypes_sta_ageout_t *) tlv;
        tlv_sta_ageout->header.type =
            wlan_cpu_to_le16(TLV_TYPE_UAP_STA_AGEOUT_TIMER);
        tlv_sta_ageout->header.len = wlan_cpu_to_le16(sizeof(t_u32));
        tlv_sta_ageout->sta_ageout_timer =
            wlan_cpu_to_le32(bss->param.bss_config.sta_ageout_timer);
        cmd_size += sizeof(MrvlIEtypes_sta_ageout_t);
        tlv += sizeof(MrvlIEtypes_sta_ageout_t);
    }

    if (((bss->param.bss_config.ps_sta_ageout_timer >= MIN_STAGE_OUT_TIME) &&
         (bss->param.bss_config.ps_sta_ageout_timer <= MAX_STAGE_OUT_TIME)) ||
        (bss->param.bss_config.ps_sta_ageout_timer == 0)) {
        tlv_ps_sta_ageout = (MrvlIEtypes_ps_sta_ageout_t *) tlv;
        tlv_ps_sta_ageout->header.type =
            wlan_cpu_to_le16(TLV_TYPE_UAP_PS_STA_AGEOUT_TIMER);
        tlv_ps_sta_ageout->header.len = wlan_cpu_to_le16(sizeof(t_u32));
        tlv_ps_sta_ageout->ps_sta_ageout_timer =
            wlan_cpu_to_le32(bss->param.bss_config.ps_sta_ageout_timer);
        cmd_size += sizeof(MrvlIEtypes_ps_sta_ageout_t);
        tlv += sizeof(MrvlIEtypes_ps_sta_ageout_t);
    }
    if (bss->param.bss_config.rts_threshold <= MAX_RTS_THRESHOLD) {
        tlv_rts_threshold = (MrvlIEtypes_rts_threshold_t *) tlv;
        tlv_rts_threshold->header.type =
            wlan_cpu_to_le16(TLV_TYPE_UAP_RTS_THRESHOLD);
        tlv_rts_threshold->header.len = wlan_cpu_to_le16(sizeof(t_u16));
        tlv_rts_threshold->rts_threshold =
            wlan_cpu_to_le16(bss->param.bss_config.rts_threshold);
        cmd_size += sizeof(MrvlIEtypes_rts_threshold_t);
        tlv += sizeof(MrvlIEtypes_rts_threshold_t);
    }

    if ((bss->param.bss_config.frag_threshold >= MIN_FRAG_THRESHOLD) &&
        (bss->param.bss_config.frag_threshold <= MAX_FRAG_THRESHOLD)) {
        tlv_frag_threshold = (MrvlIEtypes_frag_threshold_t *) tlv;
        tlv_frag_threshold->header.type =
            wlan_cpu_to_le16(TLV_TYPE_UAP_FRAG_THRESHOLD);
        tlv_frag_threshold->header.len = wlan_cpu_to_le16(sizeof(t_u16));
        tlv_frag_threshold->frag_threshold =
            wlan_cpu_to_le16(bss->param.bss_config.frag_threshold);
        cmd_size += sizeof(MrvlIEtypes_frag_threshold_t);
        tlv += sizeof(MrvlIEtypes_frag_threshold_t);
    }

    if (bss->param.bss_config.retry_limit <= MAX_RETRY_LIMIT) {
        tlv_retry_limit = (MrvlIEtypes_retry_limit_t *) tlv;
        tlv_retry_limit->header.type =
            wlan_cpu_to_le16(TLV_TYPE_UAP_RETRY_LIMIT);
        tlv_retry_limit->header.len = wlan_cpu_to_le16(sizeof(t_u8));
        tlv_retry_limit->retry_limit = (t_u8) bss->param.bss_config.retry_limit;
        cmd_size += sizeof(MrvlIEtypes_retry_limit_t);
        tlv += sizeof(MrvlIEtypes_retry_limit_t);
    }

    if (bss->param.bss_config.pairwise_update_timeout) {
        tlv_pairwise_timeout = (MrvlIEtypes_eapol_pwk_hsk_timeout_t *) tlv;
        tlv_pairwise_timeout->header.type =
            wlan_cpu_to_le16(TLV_TYPE_UAP_EAPOL_PWK_HSK_TIMEOUT);
        tlv_pairwise_timeout->header.len = wlan_cpu_to_le16(sizeof(t_u32));
        tlv_pairwise_timeout->pairwise_update_timeout =
            wlan_cpu_to_le32(bss->param.bss_config.pairwise_update_timeout);
        cmd_size += sizeof(MrvlIEtypes_eapol_pwk_hsk_timeout_t);
        tlv += sizeof(MrvlIEtypes_eapol_pwk_hsk_timeout_t);
    }

    if (bss->param.bss_config.pwk_retries) {
        tlv_pairwise_retries = (MrvlIEtypes_eapol_pwk_hsk_retries_t *) tlv;
        tlv_pairwise_retries->header.type =
            wlan_cpu_to_le16(TLV_TYPE_UAP_EAPOL_PWK_HSK_RETRIES);
        tlv_pairwise_retries->header.len = wlan_cpu_to_le16(sizeof(t_u32));
        tlv_pairwise_retries->pwk_retries =
            wlan_cpu_to_le32(bss->param.bss_config.pwk_retries);
        cmd_size += sizeof(MrvlIEtypes_eapol_pwk_hsk_retries_t);
        tlv += sizeof(MrvlIEtypes_eapol_pwk_hsk_retries_t);
    }

    if (bss->param.bss_config.groupwise_update_timeout) {
        tlv_groupwise_timeout = (MrvlIEtypes_eapol_gwk_hsk_timeout_t *) tlv;
        tlv_groupwise_timeout->header.type =
            wlan_cpu_to_le16(TLV_TYPE_UAP_EAPOL_GWK_HSK_TIMEOUT);
        tlv_groupwise_timeout->header.len = wlan_cpu_to_le16(sizeof(t_u32));
        tlv_groupwise_timeout->groupwise_update_timeout =
            wlan_cpu_to_le32(bss->param.bss_config.groupwise_update_timeout);
        cmd_size += sizeof(MrvlIEtypes_eapol_gwk_hsk_timeout_t);
        tlv += sizeof(MrvlIEtypes_eapol_gwk_hsk_timeout_t);
    }

    if (bss->param.bss_config.gwk_retries) {
        tlv_groupwise_retries = (MrvlIEtypes_eapol_gwk_hsk_retries_t *) tlv;
        tlv_groupwise_retries->header.type =
            wlan_cpu_to_le16(TLV_TYPE_UAP_EAPOL_GWK_HSK_RETRIES);
        tlv_groupwise_retries->header.len = wlan_cpu_to_le16(sizeof(t_u32));
        tlv_groupwise_retries->gwk_retries =
            wlan_cpu_to_le32(bss->param.bss_config.gwk_retries);
        cmd_size += sizeof(MrvlIEtypes_eapol_gwk_hsk_retries_t);
        tlv += sizeof(MrvlIEtypes_eapol_gwk_hsk_retries_t);
    }

    if ((bss->param.bss_config.filter.filter_mode <= MAC_FILTER_MODE_BLOCK_MAC)
        && (bss->param.bss_config.filter.mac_count <= MAX_MAC_FILTER_NUM)) {
        tlv_mac_filter = (MrvlIEtypes_mac_filter_t *) tlv;
        tlv_mac_filter->header.type =
            wlan_cpu_to_le16(TLV_TYPE_UAP_STA_MAC_ADDR_FILTER);
        tlv_mac_filter->header.len =
            wlan_cpu_to_le16(2 +
                             MLAN_MAC_ADDR_LENGTH *
                             bss->param.bss_config.filter.mac_count);
        tlv_mac_filter->count = (t_u8) bss->param.bss_config.filter.mac_count;
        tlv_mac_filter->filter_mode =
            (t_u8) bss->param.bss_config.filter.filter_mode;
        memcpy(pmpriv->adapter, tlv_mac_filter->mac_address,
               (t_u8 *) bss->param.bss_config.filter.mac_list,
               MLAN_MAC_ADDR_LENGTH * bss->param.bss_config.filter.mac_count);
        cmd_size +=
            sizeof(MrvlIEtypesHeader_t) + 2 +
            MLAN_MAC_ADDR_LENGTH * bss->param.bss_config.filter.mac_count;
        tlv +=
            sizeof(MrvlIEtypesHeader_t) + 2 +
            MLAN_MAC_ADDR_LENGTH * bss->param.bss_config.filter.mac_count;
    }

    if ((((bss->param.bss_config.band_cfg & BAND_CONFIG_ACS_MODE) ==
          BAND_CONFIG_MANUAL) && (bss->param.bss_config.channel > 0) &&
         (bss->param.bss_config.channel <= MAX_CHANNEL)) ||
        (bss->param.bss_config.band_cfg & BAND_CONFIG_ACS_MODE)) {
        tlv_chan_band = (MrvlIEtypes_channel_band_t *) tlv;
        tlv_chan_band->header.type =
            wlan_cpu_to_le16(TLV_TYPE_UAP_CHAN_BAND_CONFIG);
        tlv_chan_band->header.len =
            wlan_cpu_to_le16(sizeof(t_u8) + sizeof(t_u8));
        tlv_chan_band->band_config = bss->param.bss_config.band_cfg;
        tlv_chan_band->channel = bss->param.bss_config.channel;
        cmd_size += sizeof(MrvlIEtypes_channel_band_t);
        tlv += sizeof(MrvlIEtypes_channel_band_t);
    }

    if ((bss->param.bss_config.chan_list.num_of_chan) &&
        (bss->param.bss_config.chan_list.num_of_chan <=
         MLAN_SCAN_CHANNEL_NUM)) {
        tlv_chan_list = (MrvlIEtypes_ChanListParamSet_t *) tlv;
        tlv_chan_list->header.type = wlan_cpu_to_le16(TLV_TYPE_CHANLIST);
        tlv_chan_list->header.len =
            wlan_cpu_to_le16((t_u16)
                             (sizeof(ChanScanParamSet_t) *
                              bss->param.bss_config.chan_list.num_of_chan));

        pscan_chan = tlv_chan_list->chan_scan_param;
        for (i = 0; i < bss->param.bss_config.chan_list.num_of_chan; i++) {
            pscan_chan->chan_number =
                (t_u8) bss->param.bss_config.chan_list.chan[i];
            pscan_chan++;
        }
        cmd_size += sizeof(tlv_chan_list->header) +
            (sizeof(ChanScanParamSet_t) *
             bss->param.bss_config.chan_list.num_of_chan);
        tlv +=
            sizeof(tlv_chan_list->header) +
            (sizeof(ChanScanParamSet_t) *
             bss->param.bss_config.chan_list.num_of_chan);
    }

    if (bss->param.bss_config.auth_mode <= MLAN_AUTH_MODE_SHARED) {
        tlv_auth_type = (MrvlIEtypes_auth_type_t *) tlv;
        tlv_auth_type->header.type = wlan_cpu_to_le16(TLV_TYPE_AUTH_TYPE);
        tlv_auth_type->header.len = wlan_cpu_to_le16(sizeof(t_u8));
        tlv_auth_type->auth_type = (t_u8) bss->param.bss_config.auth_mode;
        cmd_size += sizeof(MrvlIEtypes_auth_type_t);
        tlv += sizeof(MrvlIEtypes_auth_type_t);
    }

    if (bss->param.bss_config.protocol) {
        tlv_encrypt_protocol = (MrvlIEtypes_encrypt_protocol_t *) tlv;
        tlv_encrypt_protocol->header.type =
            wlan_cpu_to_le16(TLV_TYPE_UAP_ENCRYPT_PROTOCOL);
        tlv_encrypt_protocol->header.len = wlan_cpu_to_le16(sizeof(t_u16));
        tlv_encrypt_protocol->protocol =
            wlan_cpu_to_le16(bss->param.bss_config.protocol);
        cmd_size += sizeof(MrvlIEtypes_encrypt_protocol_t);
        tlv += sizeof(MrvlIEtypes_encrypt_protocol_t);
    }
    if ((bss->param.bss_config.radio_ctl == 0) ||
        (bss->param.bss_config.radio_ctl == 1)) {
        tlv_radio_control = (MrvlIEtypes_radio_control_t *) tlv;
        tlv_radio_control->header.type =
            wlan_cpu_to_le16(TLV_TYPE_UAP_RADIO_CONTROL);
        tlv_radio_control->header.len = wlan_cpu_to_le16(sizeof(t_u8));
        tlv_radio_control->radio_ctl = bss->param.bss_config.radio_ctl;
        cmd_size += sizeof(MrvlIEtypes_radio_control_t);
        tlv += sizeof(MrvlIEtypes_radio_control_t);
    }

    if ((bss->param.bss_config.protocol == PROTOCOL_WPA) ||
        (bss->param.bss_config.protocol == PROTOCOL_WPA2) ||
        (bss->param.bss_config.protocol == PROTOCOL_WPA2_MIXED)) {
        tlv_akmp = (MrvlIEtypes_akmp_t *) tlv;
        tlv_akmp->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_AKMP);
        tlv_akmp->header.len = wlan_cpu_to_le16(sizeof(t_u16));
        tlv_akmp->key_mgmt = wlan_cpu_to_le16(KEY_MGMT_PSK);
        cmd_size += sizeof(MrvlIEtypes_akmp_t);
        tlv += sizeof(MrvlIEtypes_akmp_t);

        if ((bss->param.bss_config.wpa_cfg.
             pairwise_cipher & VALID_CIPHER_BITMAP) &&
            (bss->param.bss_config.wpa_cfg.
             group_cipher & VALID_CIPHER_BITMAP)) {
            tlv_cipher = (MrvlIEtypes_cipher_t *) tlv;
            tlv_cipher->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_CIPHER);
            tlv_cipher->header.len =
                wlan_cpu_to_le16(sizeof(t_u8) + sizeof(t_u8));
            tlv_cipher->pairwise_cipher =
                bss->param.bss_config.wpa_cfg.pairwise_cipher;
            tlv_cipher->group_cipher =
                bss->param.bss_config.wpa_cfg.group_cipher;
            cmd_size += sizeof(MrvlIEtypes_cipher_t);
            tlv += sizeof(MrvlIEtypes_cipher_t);
        }

        if (bss->param.bss_config.wpa_cfg.rsn_protection <= MTRUE) {
            tlv_rsn_prot = (MrvlIEtypes_rsn_replay_prot_t *) tlv;
            tlv_rsn_prot->header.type =
                wlan_cpu_to_le16(TLV_TYPE_UAP_RSN_REPLAY_PROTECT);
            tlv_rsn_prot->header.len = wlan_cpu_to_le16(sizeof(t_u8));
            tlv_rsn_prot->rsn_replay_prot =
                bss->param.bss_config.wpa_cfg.rsn_protection;
            cmd_size += sizeof(MrvlIEtypes_rsn_replay_prot_t);
            tlv += sizeof(MrvlIEtypes_rsn_replay_prot_t);
        }

        if (bss->param.bss_config.wpa_cfg.length) {
            tlv_passphrase = (MrvlIEtypes_passphrase_t *) tlv;
            tlv_passphrase->header.type =
                wlan_cpu_to_le16(TLV_TYPE_UAP_WPA_PASSPHRASE);
            tlv_passphrase->header.len =
                (t_u16) wlan_cpu_to_le16(bss->param.bss_config.wpa_cfg.length);
            memcpy(pmpriv->adapter, tlv_passphrase->passphrase,
                   bss->param.bss_config.wpa_cfg.passphrase,
                   bss->param.bss_config.wpa_cfg.length);
            cmd_size +=
                sizeof(MrvlIEtypesHeader_t) +
                bss->param.bss_config.wpa_cfg.length;
            tlv +=
                sizeof(MrvlIEtypesHeader_t) +
                bss->param.bss_config.wpa_cfg.length;
        }

        if (bss->param.bss_config.wpa_cfg.gk_rekey_time < MAX_GRP_TIMER) {
            tlv_rekey_time = (MrvlIEtypes_group_rekey_time_t *) tlv;
            tlv_rekey_time->header.type =
                wlan_cpu_to_le16(TLV_TYPE_UAP_GRP_REKEY_TIME);
            tlv_rekey_time->header.len = wlan_cpu_to_le16(sizeof(t_u32));
            tlv_rekey_time->gk_rekey_time =
                wlan_cpu_to_le32(bss->param.bss_config.wpa_cfg.gk_rekey_time);
            cmd_size += sizeof(MrvlIEtypes_group_rekey_time_t);
            tlv += sizeof(MrvlIEtypes_group_rekey_time_t);
        }
    } else {
        if ((bss->param.bss_config.wep_cfg.key0.length) &&
            ((bss->param.bss_config.wep_cfg.key0.length == 5) ||
             (bss->param.bss_config.wep_cfg.key0.length == 10) ||
             (bss->param.bss_config.wep_cfg.key0.length == 13) ||
             (bss->param.bss_config.wep_cfg.key0.length == 26))) {
            tlv_wep_key = (MrvlIEtypes_wep_key_t *) tlv;
            tlv_wep_key->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_WEP_KEY);
            tlv_wep_key->header.len =
                wlan_cpu_to_le16(2 + bss->param.bss_config.wep_cfg.key0.length);
            tlv_wep_key->key_index =
                bss->param.bss_config.wep_cfg.key0.key_index;
            tlv_wep_key->is_default =
                bss->param.bss_config.wep_cfg.key0.is_default;
            memcpy(pmpriv->adapter, tlv_wep_key->key,
                   bss->param.bss_config.wep_cfg.key0.key,
                   bss->param.bss_config.wep_cfg.key0.length);
            cmd_size +=
                sizeof(MrvlIEtypesHeader_t) + 2 +
                bss->param.bss_config.wep_cfg.key0.length;
            tlv +=
                sizeof(MrvlIEtypesHeader_t) + 2 +
                bss->param.bss_config.wep_cfg.key0.length;
        }

        if ((bss->param.bss_config.wep_cfg.key1.length) &&
            ((bss->param.bss_config.wep_cfg.key1.length == 5) ||
             (bss->param.bss_config.wep_cfg.key1.length == 10) ||
             (bss->param.bss_config.wep_cfg.key1.length == 13) ||
             (bss->param.bss_config.wep_cfg.key1.length == 26))) {
            tlv_wep_key = (MrvlIEtypes_wep_key_t *) tlv;
            tlv_wep_key->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_WEP_KEY);
            tlv_wep_key->header.len =
                wlan_cpu_to_le16(2 + bss->param.bss_config.wep_cfg.key1.length);
            tlv_wep_key->key_index =
                bss->param.bss_config.wep_cfg.key1.key_index;
            tlv_wep_key->is_default =
                bss->param.bss_config.wep_cfg.key1.is_default;
            memcpy(pmpriv->adapter, tlv_wep_key->key,
                   bss->param.bss_config.wep_cfg.key1.key,
                   bss->param.bss_config.wep_cfg.key1.length);
            cmd_size +=
                sizeof(MrvlIEtypesHeader_t) + 2 +
                bss->param.bss_config.wep_cfg.key1.length;
            tlv +=
                sizeof(MrvlIEtypesHeader_t) + 2 +
                bss->param.bss_config.wep_cfg.key1.length;
        }

        if ((bss->param.bss_config.wep_cfg.key2.length) &&
            ((bss->param.bss_config.wep_cfg.key2.length == 5) ||
             (bss->param.bss_config.wep_cfg.key2.length == 10) ||
             (bss->param.bss_config.wep_cfg.key2.length == 13) ||
             (bss->param.bss_config.wep_cfg.key2.length == 26))) {
            tlv_wep_key = (MrvlIEtypes_wep_key_t *) tlv;
            tlv_wep_key->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_WEP_KEY);
            tlv_wep_key->header.len =
                wlan_cpu_to_le16(2 + bss->param.bss_config.wep_cfg.key2.length);
            tlv_wep_key->key_index =
                bss->param.bss_config.wep_cfg.key2.key_index;
            tlv_wep_key->is_default =
                bss->param.bss_config.wep_cfg.key2.is_default;
            memcpy(pmpriv->adapter, tlv_wep_key->key,
                   bss->param.bss_config.wep_cfg.key2.key,
                   bss->param.bss_config.wep_cfg.key2.length);
            cmd_size +=
                sizeof(MrvlIEtypesHeader_t) + 2 +
                bss->param.bss_config.wep_cfg.key2.length;
            tlv +=
                sizeof(MrvlIEtypesHeader_t) + 2 +
                bss->param.bss_config.wep_cfg.key2.length;
        }

        if ((bss->param.bss_config.wep_cfg.key3.length) &&
            ((bss->param.bss_config.wep_cfg.key3.length == 5) ||
             (bss->param.bss_config.wep_cfg.key3.length == 10) ||
             (bss->param.bss_config.wep_cfg.key3.length == 13) ||
             (bss->param.bss_config.wep_cfg.key3.length == 26))) {
            tlv_wep_key = (MrvlIEtypes_wep_key_t *) tlv;
            tlv_wep_key->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_WEP_KEY);
            tlv_wep_key->header.len =
                wlan_cpu_to_le16(2 + bss->param.bss_config.wep_cfg.key3.length);
            tlv_wep_key->key_index =
                bss->param.bss_config.wep_cfg.key3.key_index;
            tlv_wep_key->is_default =
                bss->param.bss_config.wep_cfg.key3.is_default;
            memcpy(pmpriv->adapter, tlv_wep_key->key,
                   bss->param.bss_config.wep_cfg.key3.key,
                   bss->param.bss_config.wep_cfg.key3.length);
            cmd_size +=
                sizeof(MrvlIEtypesHeader_t) + 2 +
                bss->param.bss_config.wep_cfg.key3.length;
            tlv +=
                sizeof(MrvlIEtypesHeader_t) + 2 +
                bss->param.bss_config.wep_cfg.key3.length;
        }
    }
    cmd->size = (t_u16) wlan_cpu_to_le16(cmd_size);
    PRINTM(MCMND, "AP config: cmd_size=%d\n", cmd_size);
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function prepares command of sys_config
 *  
 *  @param pmpriv		A pointer to mlan_private structure
 *  @param cmd	   		A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   the action: GET or SET
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure 
 *  @return         MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_uap_cmd_sys_configure(pmlan_private pmpriv,
                           IN HostCmd_DS_COMMAND * cmd,
                           IN t_u16 cmd_action, IN pmlan_ioctl_req pioctl_buf)
{
    mlan_ds_bss *bss = MNULL;
    HostCmd_DS_SYS_CONFIG *sys_config =
        (HostCmd_DS_SYS_CONFIG *) & cmd->params.sys_config;
    MrvlIEtypes_MacAddr_t *mac_tlv = MNULL;
    mlan_ds_misc_cfg *misc = MNULL;
    MrvlIEtypesHeader_t *ie_header =
        (MrvlIEtypesHeader_t *) sys_config->tlv_buffer;
    t_u8 *ie = (t_u8 *) sys_config->tlv_buffer + sizeof(MrvlIEtypesHeader_t);
    t_u16 req_len = 0, travel_len = 0;
    custom_ie *cptr = MNULL;
    mlan_status ret = MLAN_STATUS_SUCCESS;

    ENTER();

    cmd->command = wlan_cpu_to_le16(HOST_CMD_APCMD_SYS_CONFIGURE);
    sys_config->action = wlan_cpu_to_le16(cmd_action);
    cmd->size = wlan_cpu_to_le16(sizeof(HostCmd_DS_SYS_CONFIG) - 1 + S_DS_GEN);
    if (pioctl_buf == MNULL) {
        mac_tlv = (MrvlIEtypes_MacAddr_t *) sys_config->tlv_buffer;
        cmd->size =
            wlan_cpu_to_le16(sizeof(HostCmd_DS_SYS_CONFIG) - 1 + S_DS_GEN +
                             sizeof(MrvlIEtypes_MacAddr_t));
        mac_tlv->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_MAC_ADDRESS);
        mac_tlv->header.len = wlan_cpu_to_le16(MLAN_MAC_ADDR_LENGTH);
        ret = MLAN_STATUS_SUCCESS;
        goto done;
    }
    if (pioctl_buf->req_id == MLAN_IOCTL_BSS) {
        bss = (mlan_ds_bss *) pioctl_buf->pbuf;
        if (bss->sub_command == MLAN_OID_BSS_MAC_ADDR) {
            mac_tlv = (MrvlIEtypes_MacAddr_t *) sys_config->tlv_buffer;
            cmd->size =
                wlan_cpu_to_le16(sizeof(HostCmd_DS_SYS_CONFIG) - 1 + S_DS_GEN +
                                 sizeof(MrvlIEtypes_MacAddr_t));
            mac_tlv->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_MAC_ADDRESS);
            mac_tlv->header.len = wlan_cpu_to_le16(MLAN_MAC_ADDR_LENGTH);
            if (cmd_action == HostCmd_ACT_GEN_SET)
                memcpy(pmpriv->adapter, mac_tlv->mac, &bss->param.mac_addr,
                       MLAN_MAC_ADDR_LENGTH);
        } else if ((bss->sub_command == MLAN_OID_UAP_BSS_CONFIG) &&
                   (cmd_action == HostCmd_ACT_GEN_SET)) {
            ret = wlan_uap_cmd_ap_config(pmpriv, cmd, cmd_action, pioctl_buf);
            goto done;
        }
    } else if (pioctl_buf->req_id == MLAN_IOCTL_MISC_CFG) {
        misc = (mlan_ds_misc_cfg *) pioctl_buf->pbuf;
        if ((misc->sub_command == MLAN_OID_MISC_CUSTOM_IE) &&
            (misc->param.cust_ie.type == TLV_TYPE_MGMT_IE)) {
            cmd->size =
                wlan_cpu_to_le16(sizeof(HostCmd_DS_SYS_CONFIG) - 1 + S_DS_GEN +
                                 sizeof(MrvlIEtypesHeader_t) +
                                 misc->param.cust_ie.len);
            ie_header->type = wlan_cpu_to_le16(TLV_TYPE_MGMT_IE);
            ie_header->len = wlan_cpu_to_le16(misc->param.cust_ie.len);

            if (ie && misc->param.cust_ie.ie_data_list) {
                req_len = misc->param.cust_ie.len;
                travel_len = 0;
                /* conversion for index, mask, len */
                if (req_len == sizeof(t_u16))
                    misc->param.cust_ie.ie_data_list[0].ie_index =
                        wlan_cpu_to_le16(misc->param.cust_ie.ie_data_list[0].
                                         ie_index);
                while (req_len > sizeof(t_u16)) {
                    cptr =
                        (custom_ie
                         *) (((t_u8 *) & misc->param.cust_ie.ie_data_list) +
                             travel_len);
                    travel_len +=
                        cptr->ie_length + sizeof(custom_ie) - MAX_IE_SIZE;
                    req_len -=
                        cptr->ie_length + sizeof(custom_ie) - MAX_IE_SIZE;
                    cptr->ie_index = wlan_cpu_to_le16(cptr->ie_index);
                    cptr->mgmt_subtype_mask =
                        wlan_cpu_to_le16(cptr->mgmt_subtype_mask);
                    cptr->ie_length = wlan_cpu_to_le16(cptr->ie_length);
                }
                memcpy(pmpriv->adapter, ie, misc->param.cust_ie.ie_data_list,
                       misc->param.cust_ie.len);
            }
        }
    }
  done:
    LEAVE();
    return ret;
}

/** 
 *  @brief This function handles command resp for get uap settings
 *  
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure 
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status
wlan_uap_ret_cmd_ap_config(IN pmlan_private pmpriv,
                           IN HostCmd_DS_COMMAND * resp,
                           IN mlan_ioctl_req * pioctl_buf)
{
    HostCmd_DS_SYS_CONFIG *sys_config =
        (HostCmd_DS_SYS_CONFIG *) & resp->params.sys_config;
    mlan_ds_bss *bss = MNULL;
    MrvlIEtypesHeader_t *tlv = MNULL;
    t_u16 tlv_buf_left = 0;
    t_u16 tlv_type = 0;
    t_u16 tlv_len = 0;
    MrvlIEtypes_MacAddr_t *tlv_mac = MNULL;
    MrvlIEtypes_SsIdParamSet_t *tlv_ssid = MNULL;
    MrvlIEtypes_beacon_period_t *tlv_beacon_period = MNULL;
    MrvlIEtypes_dtim_period_t *tlv_dtim_period = MNULL;
    MrvlIEtypes_RatesParamSet_t *tlv_rates = MNULL;
    MrvlIEtypes_tx_rate_t *tlv_txrate = MNULL;
    MrvlIEtypes_mcbc_rate_t *tlv_mcbc_rate = MNULL;
    MrvlIEtypes_tx_power_t *tlv_tx_power = MNULL;
    MrvlIEtypes_bcast_ssid_t *tlv_bcast_ssid = MNULL;
    MrvlIEtypes_antenna_mode_t *tlv_antenna = MNULL;
    MrvlIEtypes_pkt_forward_t *tlv_pkt_forward = MNULL;
    MrvlIEtypes_max_sta_count_t *tlv_sta_count = MNULL;
    MrvlIEtypes_sta_ageout_t *tlv_sta_ageout = MNULL;
    MrvlIEtypes_ps_sta_ageout_t *tlv_ps_sta_ageout = MNULL;
    MrvlIEtypes_rts_threshold_t *tlv_rts_threshold = MNULL;
    MrvlIEtypes_frag_threshold_t *tlv_frag_threshold = MNULL;
    MrvlIEtypes_retry_limit_t *tlv_retry_limit = MNULL;
    MrvlIEtypes_eapol_pwk_hsk_timeout_t *tlv_pairwise_timeout = MNULL;
    MrvlIEtypes_eapol_pwk_hsk_retries_t *tlv_pairwise_retries = MNULL;
    MrvlIEtypes_eapol_gwk_hsk_timeout_t *tlv_groupwise_timeout = MNULL;
    MrvlIEtypes_eapol_gwk_hsk_retries_t *tlv_groupwise_retries = MNULL;
    MrvlIEtypes_mac_filter_t *tlv_mac_filter = MNULL;
    MrvlIEtypes_channel_band_t *tlv_chan_band = MNULL;
    MrvlIEtypes_ChanListParamSet_t *tlv_chan_list = MNULL;
    ChanScanParamSet_t *pscan_chan = MNULL;
    MrvlIEtypes_auth_type_t *tlv_auth_type = MNULL;
    MrvlIEtypes_encrypt_protocol_t *tlv_encrypt_protocol = MNULL;
    MrvlIEtypes_akmp_t *tlv_akmp = MNULL;
    MrvlIEtypes_cipher_t *tlv_cipher = MNULL;
    MrvlIEtypes_rsn_replay_prot_t *tlv_rsn_prot = MNULL;
    MrvlIEtypes_passphrase_t *tlv_passphrase = MNULL;
    MrvlIEtypes_group_rekey_time_t *tlv_rekey_time = MNULL;
    MrvlIEtypes_wep_key_t *tlv_wep_key = MNULL;
    MrvlIEtypes_preamble_t *tlv_preamble = MNULL;
    MrvlIEtypes_radio_control_t *tlv_radio_control = MNULL;
    wep_key *pkey = MNULL;
    t_u16 i;

    ENTER();

    bss = (mlan_ds_bss *) pioctl_buf->pbuf;
    tlv = (MrvlIEtypesHeader_t *) sys_config->tlv_buffer;
    tlv_buf_left = resp->size - (sizeof(HostCmd_DS_SYS_CONFIG) - 1 + S_DS_GEN);

    while (tlv_buf_left >= sizeof(MrvlIEtypesHeader_t)) {

        tlv_type = wlan_le16_to_cpu(tlv->type);
        tlv_len = wlan_le16_to_cpu(tlv->len);

        switch (tlv_type) {
        case TLV_TYPE_UAP_MAC_ADDRESS:
            tlv_mac = (MrvlIEtypes_MacAddr_t *) tlv;
            memcpy(pmpriv->adapter, &bss->param.bss_config.mac_addr,
                   tlv_mac->mac, MLAN_MAC_ADDR_LENGTH);
            break;
        case TLV_TYPE_SSID:
            tlv_ssid = (MrvlIEtypes_SsIdParamSet_t *) tlv;
            bss->param.bss_config.ssid.ssid_len = tlv_len;
            memcpy(pmpriv->adapter, bss->param.bss_config.ssid.ssid,
                   tlv_ssid->ssid, tlv_len);
            break;
        case TLV_TYPE_UAP_BEACON_PERIOD:
            tlv_beacon_period = (MrvlIEtypes_beacon_period_t *) tlv;
            bss->param.bss_config.beacon_period =
                wlan_le16_to_cpu(tlv_beacon_period->beacon_period);
            break;
        case TLV_TYPE_UAP_DTIM_PERIOD:
            tlv_dtim_period = (MrvlIEtypes_dtim_period_t *) tlv;
            bss->param.bss_config.dtim_period = tlv_dtim_period->dtim_period;
            break;
        case TLV_TYPE_RATES:
            tlv_rates = (MrvlIEtypes_RatesParamSet_t *) tlv;
            memcpy(pmpriv->adapter, bss->param.bss_config.rates,
                   tlv_rates->rates, tlv_len);
            break;
        case TLV_TYPE_UAP_TX_DATA_RATE:
            tlv_txrate = (MrvlIEtypes_tx_rate_t *) tlv;
            bss->param.bss_config.tx_data_rate =
                wlan_le16_to_cpu(tlv_txrate->tx_data_rate);
            break;
        case TLV_TYPE_UAP_MCBC_DATA_RATE:
            tlv_mcbc_rate = (MrvlIEtypes_mcbc_rate_t *) tlv;
            bss->param.bss_config.mcbc_data_rate =
                wlan_le16_to_cpu(tlv_mcbc_rate->mcbc_data_rate);
            break;
        case TLV_TYPE_UAP_TX_POWER:
            tlv_tx_power = (MrvlIEtypes_tx_power_t *) tlv;
            bss->param.bss_config.tx_power_level = tlv_tx_power->tx_power;
            break;
        case TLV_TYPE_UAP_BCAST_SSID_CTL:
            tlv_bcast_ssid = (MrvlIEtypes_bcast_ssid_t *) tlv;
            bss->param.bss_config.bcast_ssid_ctl =
                tlv_bcast_ssid->bcast_ssid_ctl;
            break;
        case TLV_TYPE_UAP_RADIO_CONTROL:
            tlv_radio_control = (MrvlIEtypes_radio_control_t *) tlv;
            bss->param.bss_config.radio_ctl = tlv_radio_control->radio_ctl;
            break;
        case TLV_TYPE_UAP_ANTENNA_CTL:
            tlv_antenna = (MrvlIEtypes_antenna_mode_t *) tlv;
            if (tlv_antenna->which_antenna == TX_ANTENNA)
                bss->param.bss_config.tx_antenna = tlv_antenna->antenna_mode;
            else if (tlv_antenna->which_antenna == RX_ANTENNA)
                bss->param.bss_config.rx_antenna = tlv_antenna->antenna_mode;
            break;
        case TLV_TYPE_UAP_PKT_FWD_CTL:
            tlv_pkt_forward = (MrvlIEtypes_pkt_forward_t *) tlv;
            bss->param.bss_config.pkt_forward_ctl =
                tlv_pkt_forward->pkt_forward_ctl;
            break;
        case TLV_TYPE_UAP_MAX_STA_CNT:
            tlv_sta_count = (MrvlIEtypes_max_sta_count_t *) tlv;
            bss->param.bss_config.max_sta_count =
                wlan_le16_to_cpu(tlv_sta_count->max_sta_count);
            break;
        case TLV_TYPE_UAP_STA_AGEOUT_TIMER:
            tlv_sta_ageout = (MrvlIEtypes_sta_ageout_t *) tlv;
            bss->param.bss_config.sta_ageout_timer =
                wlan_le32_to_cpu(tlv_sta_ageout->sta_ageout_timer);
            break;
        case TLV_TYPE_UAP_PS_STA_AGEOUT_TIMER:
            tlv_ps_sta_ageout = (MrvlIEtypes_ps_sta_ageout_t *) tlv;
            bss->param.bss_config.ps_sta_ageout_timer =
                wlan_le32_to_cpu(tlv_ps_sta_ageout->ps_sta_ageout_timer);
            break;
        case TLV_TYPE_UAP_RTS_THRESHOLD:
            tlv_rts_threshold = (MrvlIEtypes_rts_threshold_t *) tlv;
            bss->param.bss_config.rts_threshold =
                wlan_le16_to_cpu(tlv_rts_threshold->rts_threshold);
            break;
        case TLV_TYPE_UAP_FRAG_THRESHOLD:
            tlv_frag_threshold = (MrvlIEtypes_frag_threshold_t *) tlv;
            bss->param.bss_config.frag_threshold =
                wlan_le16_to_cpu(tlv_frag_threshold->frag_threshold);
            break;
        case TLV_TYPE_UAP_RETRY_LIMIT:
            tlv_retry_limit = (MrvlIEtypes_retry_limit_t *) tlv;
            bss->param.bss_config.retry_limit = tlv_retry_limit->retry_limit;
            break;
        case TLV_TYPE_UAP_EAPOL_PWK_HSK_TIMEOUT:
            tlv_pairwise_timeout = (MrvlIEtypes_eapol_pwk_hsk_timeout_t *) tlv;
            bss->param.bss_config.pairwise_update_timeout =
                wlan_le32_to_cpu(tlv_pairwise_timeout->pairwise_update_timeout);
            break;
        case TLV_TYPE_UAP_EAPOL_PWK_HSK_RETRIES:
            tlv_pairwise_retries = (MrvlIEtypes_eapol_pwk_hsk_retries_t *) tlv;
            bss->param.bss_config.pwk_retries =
                wlan_le32_to_cpu(tlv_pairwise_retries->pwk_retries);
            break;
        case TLV_TYPE_UAP_EAPOL_GWK_HSK_TIMEOUT:
            tlv_groupwise_timeout = (MrvlIEtypes_eapol_gwk_hsk_timeout_t *) tlv;
            bss->param.bss_config.groupwise_update_timeout =
                wlan_le32_to_cpu(tlv_groupwise_timeout->
                                 groupwise_update_timeout);
            break;
        case TLV_TYPE_UAP_EAPOL_GWK_HSK_RETRIES:
            tlv_groupwise_retries = (MrvlIEtypes_eapol_gwk_hsk_retries_t *) tlv;
            bss->param.bss_config.gwk_retries =
                wlan_le32_to_cpu(tlv_groupwise_retries->gwk_retries);
            break;
        case TLV_TYPE_UAP_STA_MAC_ADDR_FILTER:
            tlv_mac_filter = (MrvlIEtypes_mac_filter_t *) tlv;
            bss->param.bss_config.filter.mac_count = tlv_mac_filter->count;
            bss->param.bss_config.filter.filter_mode =
                tlv_mac_filter->filter_mode;
            memcpy(pmpriv->adapter,
                   (t_u8 *) bss->param.bss_config.filter.mac_list,
                   tlv_mac_filter->mac_address,
                   MLAN_MAC_ADDR_LENGTH *
                   bss->param.bss_config.filter.mac_count);
            break;
        case TLV_TYPE_UAP_CHAN_BAND_CONFIG:
            tlv_chan_band = (MrvlIEtypes_channel_band_t *) tlv;
            bss->param.bss_config.band_cfg = tlv_chan_band->band_config;
            bss->param.bss_config.channel = tlv_chan_band->channel;
            break;
        case TLV_TYPE_CHANLIST:
            tlv_chan_list = (MrvlIEtypes_ChanListParamSet_t *) tlv;
            bss->param.bss_config.chan_list.num_of_chan =
                tlv_len / sizeof(ChanScanParamSet_t);
            pscan_chan = tlv_chan_list->chan_scan_param;
            for (i = 0; i < bss->param.bss_config.chan_list.num_of_chan; i++) {
                bss->param.bss_config.chan_list.chan[i] =
                    pscan_chan->chan_number;
                pscan_chan++;
            }
            break;
        case TLV_TYPE_AUTH_TYPE:
            tlv_auth_type = (MrvlIEtypes_auth_type_t *) tlv;
            bss->param.bss_config.auth_mode = tlv_auth_type->auth_type;
            break;
        case TLV_TYPE_UAP_ENCRYPT_PROTOCOL:
            tlv_encrypt_protocol = (MrvlIEtypes_encrypt_protocol_t *) tlv;
            bss->param.bss_config.protocol =
                wlan_le16_to_cpu(tlv_encrypt_protocol->protocol);
            break;
        case TLV_TYPE_UAP_AKMP:
            tlv_akmp = (MrvlIEtypes_akmp_t *) tlv;
            bss->param.bss_config.key_mgmt =
                wlan_le16_to_cpu(tlv_akmp->key_mgmt);
            break;
        case TLV_TYPE_UAP_CIPHER:
            tlv_cipher = (MrvlIEtypes_cipher_t *) tlv;
            bss->param.bss_config.wpa_cfg.pairwise_cipher =
                tlv_cipher->pairwise_cipher;
            bss->param.bss_config.wpa_cfg.group_cipher =
                tlv_cipher->group_cipher;
            break;
        case TLV_TYPE_UAP_RSN_REPLAY_PROTECT:
            tlv_rsn_prot = (MrvlIEtypes_rsn_replay_prot_t *) tlv;
            bss->param.bss_config.wpa_cfg.rsn_protection =
                tlv_rsn_prot->rsn_replay_prot;
            break;
        case TLV_TYPE_UAP_WPA_PASSPHRASE:
            tlv_passphrase = (MrvlIEtypes_passphrase_t *) tlv;
            bss->param.bss_config.wpa_cfg.length = tlv_len;
            memcpy(pmpriv->adapter, bss->param.bss_config.wpa_cfg.passphrase,
                   tlv_passphrase->passphrase,
                   bss->param.bss_config.wpa_cfg.length);
            break;
        case TLV_TYPE_UAP_GRP_REKEY_TIME:
            tlv_rekey_time = (MrvlIEtypes_group_rekey_time_t *) tlv;
            bss->param.bss_config.wpa_cfg.gk_rekey_time =
                wlan_le32_to_cpu(tlv_rekey_time->gk_rekey_time);
            break;
        case TLV_TYPE_UAP_WEP_KEY:
            tlv_wep_key = (MrvlIEtypes_wep_key_t *) tlv;
            pkey = MNULL;
            if (tlv_wep_key->key_index == 0)
                pkey = &bss->param.bss_config.wep_cfg.key0;
            else if (tlv_wep_key->key_index == 1)
                pkey = &bss->param.bss_config.wep_cfg.key1;
            else if (tlv_wep_key->key_index == 2)
                pkey = &bss->param.bss_config.wep_cfg.key2;
            else if (tlv_wep_key->key_index == 3)
                pkey = &bss->param.bss_config.wep_cfg.key3;
            if (pkey) {
                pkey->key_index = tlv_wep_key->key_index;
                pkey->is_default = tlv_wep_key->is_default;
                pkey->length = tlv_len - 2;
                memcpy(pmpriv->adapter, pkey->key, tlv_wep_key->key,
                       pkey->length);
            }
            break;
        case TLV_TYPE_UAP_PREAMBLE_CTL:
            tlv_preamble = (MrvlIEtypes_preamble_t *) tlv;
            bss->param.bss_config.preamble_type = tlv_preamble->preamble_type;
            break;
        }

        tlv_buf_left -= tlv_len + sizeof(MrvlIEtypesHeader_t);
        tlv =
            (MrvlIEtypesHeader_t *) ((t_u8 *) tlv + tlv_len +
                                     sizeof(MrvlIEtypesHeader_t));
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function handles the command response of sys_config
 *  
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure 
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status
wlan_uap_ret_sys_config(IN pmlan_private pmpriv,
                        IN HostCmd_DS_COMMAND * resp,
                        IN mlan_ioctl_req * pioctl_buf)
{
    t_u16 resp_len = 0, travel_len = 0;
    custom_ie *cptr;
    HostCmd_DS_SYS_CONFIG *sys_config =
        (HostCmd_DS_SYS_CONFIG *) & resp->params.sys_config;
    mlan_ds_bss *bss = MNULL;
    mlan_ds_misc_cfg *misc = MNULL;
    MrvlIEtypes_MacAddr_t *tlv =
        (MrvlIEtypes_MacAddr_t *) sys_config->tlv_buffer;
    mlan_ds_misc_custom_ie *cust_ie = MNULL;

    ENTER();
    if (pioctl_buf) {
        if (pioctl_buf->req_id == MLAN_IOCTL_BSS) {
            bss = (mlan_ds_bss *) pioctl_buf->pbuf;
            if (bss->sub_command == MLAN_OID_BSS_MAC_ADDR) {
                if (TLV_TYPE_UAP_MAC_ADDRESS ==
                    wlan_le16_to_cpu(tlv->header.type)) {
                    memcpy(pmpriv->adapter, &bss->param.mac_addr, tlv->mac,
                           MLAN_MAC_ADDR_LENGTH);
                }
            } else if ((bss->sub_command == MLAN_OID_UAP_BSS_CONFIG) &&
                       (pioctl_buf->action == MLAN_ACT_GET)) {
                wlan_uap_ret_cmd_ap_config(pmpriv, resp, pioctl_buf);
            }
        }
        if (pioctl_buf->req_id == MLAN_IOCTL_MISC_CFG) {
            misc = (mlan_ds_misc_cfg *) pioctl_buf->pbuf;
            cust_ie = (mlan_ds_misc_custom_ie *) sys_config->tlv_buffer;
            if ((pioctl_buf->action == MLAN_ACT_GET) &&
                (misc->sub_command == MLAN_OID_MISC_CUSTOM_IE)) {

                cust_ie->type = wlan_le16_to_cpu(cust_ie->type);
                resp_len = cust_ie->len = wlan_le16_to_cpu(cust_ie->len);
                travel_len = 0;
                /* conversion for index, mask, len */
                if (resp_len == sizeof(t_u16))
                    cust_ie->ie_data_list[0].ie_index =
                        wlan_cpu_to_le16(cust_ie->ie_data_list[0].ie_index);

                while (resp_len > sizeof(t_u16)) {
                    cptr =
                        (custom_ie *) (((t_u8 *) cust_ie->ie_data_list) +
                                       travel_len);
                    cptr->ie_index = wlan_le16_to_cpu(cptr->ie_index);
                    cptr->mgmt_subtype_mask =
                        wlan_le16_to_cpu(cptr->mgmt_subtype_mask);
                    cptr->ie_length = wlan_le16_to_cpu(cptr->ie_length);
                    travel_len +=
                        cptr->ie_length + sizeof(custom_ie) - MAX_IE_SIZE;
                    resp_len -=
                        cptr->ie_length + sizeof(custom_ie) - MAX_IE_SIZE;
                }
                memcpy(pmpriv->adapter, &misc->param.cust_ie, cust_ie,
                       (cust_ie->len + sizeof(MrvlIEtypesHeader_t)));
            }
        }
    } else {
        if (TLV_TYPE_UAP_MAC_ADDRESS == wlan_le16_to_cpu(tlv->header.type)) {
            memcpy(pmpriv->adapter, pmpriv->curr_addr, tlv->mac,
                   MLAN_MAC_ADDR_LENGTH);
        }
    }
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function handles the command response of sys_info
 *  
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure 
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status
wlan_uap_ret_sys_info(IN pmlan_private pmpriv,
                      IN HostCmd_DS_COMMAND * resp,
                      IN mlan_ioctl_req * pioctl_buf)
{
    HostCmd_DS_SYS_INFO *sys_info =
        (HostCmd_DS_SYS_INFO *) & resp->params.sys_info;
    mlan_ds_get_info *info;
    ENTER();
    if (pioctl_buf) {
        info = (mlan_ds_get_info *) pioctl_buf->pbuf;
        memcpy(pmpriv->adapter, info->param.ver_ext.version_str,
               sys_info->sys_info, sizeof(char) * 64);
    }
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function prepares command of snmp_mib
 *           
 *  @param pmpriv		A pointer to mlan_private structure
 *  @param cmd	   		A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   the action: GET or SET
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure 
 *  @return         MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_uap_cmd_snmp_mib(pmlan_private pmpriv,
                      IN HostCmd_DS_COMMAND * cmd,
                      IN t_u16 cmd_action, IN pmlan_ioctl_req pioctl_buf)
{
    HostCmd_DS_802_11_SNMP_MIB *psnmp_mib = &cmd->params.smib;
    MrvlIEtypes_snmp_oid_t *psnmp_oid = MNULL;
    t_u8 i;

    t_u8 snmp_oids[] = {
        tkip_mic_failures,
        ccmp_decrypt_errors,
        wep_undecryptable_count,
        wep_icv_error_count,
        decrypt_failure_count,
        dot11_mcast_tx_count,
        dot11_failed_count,
        dot11_retry_count,
        dot11_multi_retry_count,
        dot11_frame_dup_count,
        dot11_rts_success_count,
        dot11_rts_failure_count,
        dot11_ack_failure_count,
        dot11_rx_fragment_count,
        dot11_mcast_rx_frame_count,
        dot11_fcs_error_count,
        dot11_tx_frame_count,
        dot11_rsna_tkip_cm_invoked,
        dot11_rsna_4way_hshk_failures,
    };

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_802_11_SNMP_MIB);
    psnmp_mib->query_type = wlan_cpu_to_le16(HostCmd_ACT_GEN_GET);
    cmd->size =
        wlan_cpu_to_le16(sizeof(t_u16) + S_DS_GEN +
                         sizeof(snmp_oids) * sizeof(MrvlIEtypes_snmp_oid_t));
    psnmp_oid = (MrvlIEtypes_snmp_oid_t *) & psnmp_mib->oid;
    for (i = 0; i < sizeof(snmp_oids); i++) {
        psnmp_oid->header.type = wlan_cpu_to_le16(snmp_oids[i]);
        psnmp_oid->header.len = wlan_cpu_to_le16(sizeof(t_u32));
        psnmp_oid++;
    }
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function handles the command response of snmp_mib
 *  
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure 
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status
wlan_uap_ret_snmp_mib(IN pmlan_private pmpriv,
                      IN HostCmd_DS_COMMAND * resp,
                      IN mlan_ioctl_req * pioctl_buf)
{
    HostCmd_DS_802_11_SNMP_MIB *psnmp_mib =
        (HostCmd_DS_802_11_SNMP_MIB *) & resp->params.smib;
    mlan_ds_get_info *info;
    MrvlIEtypes_snmp_oid_t *psnmp_oid = MNULL;
    t_u16 tlv_buf_left = 0;
    t_u16 tlv_type = 0;

    ENTER();
    if (!pioctl_buf) {
        LEAVE();
        return MLAN_STATUS_SUCCESS;
    }
    info = (mlan_ds_get_info *) pioctl_buf->pbuf;
    tlv_buf_left = resp->size - (sizeof(t_u16) + S_DS_GEN);
    psnmp_oid = (MrvlIEtypes_snmp_oid_t *) & psnmp_mib->oid;
    while (tlv_buf_left >= sizeof(MrvlIEtypes_snmp_oid_t)) {
        tlv_type = wlan_le16_to_cpu(psnmp_oid->header.type);
        switch (tlv_type) {
        case tkip_mic_failures:
            info->param.ustats.tkip_mic_failures =
                wlan_le32_to_cpu(psnmp_oid->data);
            break;
        case ccmp_decrypt_errors:
            info->param.ustats.ccmp_decrypt_errors =
                wlan_le32_to_cpu(psnmp_oid->data);
            break;
        case wep_undecryptable_count:
            info->param.ustats.wep_undecryptable_count =
                wlan_le32_to_cpu(psnmp_oid->data);
            break;
        case wep_icv_error_count:
            info->param.ustats.wep_icv_error_count =
                wlan_le32_to_cpu(psnmp_oid->data);
            break;
        case decrypt_failure_count:
            info->param.ustats.decrypt_failure_count =
                wlan_le32_to_cpu(psnmp_oid->data);
            break;
        case dot11_mcast_tx_count:
            info->param.ustats.mcast_tx_count =
                wlan_le32_to_cpu(psnmp_oid->data);
            break;
        case dot11_failed_count:
            info->param.ustats.failed_count = wlan_le32_to_cpu(psnmp_oid->data);
            break;
        case dot11_retry_count:
            info->param.ustats.retry_count = wlan_le32_to_cpu(psnmp_oid->data);
            break;
        case dot11_multi_retry_count:
            info->param.ustats.multi_retry_count =
                wlan_le32_to_cpu(psnmp_oid->data);
            break;
        case dot11_frame_dup_count:
            info->param.ustats.frame_dup_count =
                wlan_le32_to_cpu(psnmp_oid->data);
            break;
        case dot11_rts_success_count:
            info->param.ustats.rts_success_count =
                wlan_le32_to_cpu(psnmp_oid->data);
            break;
        case dot11_rts_failure_count:
            info->param.ustats.rts_failure_count =
                wlan_le32_to_cpu(psnmp_oid->data);
            break;
        case dot11_ack_failure_count:
            info->param.ustats.ack_failure_count =
                wlan_le32_to_cpu(psnmp_oid->data);
            break;
        case dot11_rx_fragment_count:
            info->param.ustats.rx_fragment_count =
                wlan_le32_to_cpu(psnmp_oid->data);
            break;
        case dot11_mcast_rx_frame_count:
            info->param.ustats.mcast_rx_frame_count =
                wlan_le32_to_cpu(psnmp_oid->data);
            break;
        case dot11_fcs_error_count:
            info->param.ustats.fcs_error_count =
                wlan_le32_to_cpu(psnmp_oid->data);
            break;
        case dot11_tx_frame_count:
            info->param.ustats.tx_frame_count =
                wlan_le32_to_cpu(psnmp_oid->data);
            break;
        case dot11_rsna_tkip_cm_invoked:
            info->param.ustats.rsna_tkip_cm_invoked =
                wlan_le32_to_cpu(psnmp_oid->data);
            break;
        case dot11_rsna_4way_hshk_failures:
            info->param.ustats.rsna_4way_hshk_failures =
                wlan_le32_to_cpu(psnmp_oid->data);
            break;
        }
        tlv_buf_left -= sizeof(MrvlIEtypes_snmp_oid_t);
        psnmp_oid++;
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function prepares command of deauth station
 *           
 *  @param pmpriv		A pointer to mlan_private structure
 *  @param cmd	   		A pointer to HostCmd_DS_COMMAND structure
 *  @param pdata_buf    A pointer to data buffer
 *  @return         MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_uap_cmd_sta_deauth(pmlan_private pmpriv,
                        IN HostCmd_DS_COMMAND * cmd, IN t_void * pdata_buf)
{
    HostCmd_DS_STA_DEAUTH *pcmd_sta_deauth =
        (HostCmd_DS_STA_DEAUTH *) & cmd->params.sta_deauth;
    mlan_deauth_param *deauth = (mlan_deauth_param *) pdata_buf;

    ENTER();
    cmd->command = wlan_cpu_to_le16(HOST_CMD_APCMD_STA_DEAUTH);
    cmd->size = wlan_cpu_to_le16(S_DS_GEN + sizeof(HostCmd_DS_STA_DEAUTH));
    memcpy(pmpriv->adapter, pcmd_sta_deauth->mac, deauth->mac_addr,
           MLAN_MAC_ADDR_LENGTH);
    pcmd_sta_deauth->reason = wlan_cpu_to_le16(deauth->reason_code);
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function handles the command response of sta_list
 *  
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure 
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status
wlan_uap_ret_sta_list(IN pmlan_private pmpriv,
                      IN HostCmd_DS_COMMAND * resp,
                      IN mlan_ioctl_req * pioctl_buf)
{
    HostCmd_DS_STA_LIST *sta_list =
        (HostCmd_DS_STA_LIST *) & resp->params.sta_list;
    mlan_ds_get_info *info;
    MrvlIEtypes_sta_info_t *tlv = MNULL;
    t_u8 i = 0;

    ENTER();
    if (pioctl_buf) {
        info = (mlan_ds_get_info *) pioctl_buf->pbuf;
        info->param.sta_list.sta_count = wlan_le16_to_cpu(sta_list->sta_count);
        tlv =
            (MrvlIEtypes_sta_info_t *) ((t_u8 *) sta_list +
                                        sizeof(HostCmd_DS_STA_LIST));
        info->param.sta_list.sta_count =
            MIN(info->param.sta_list.sta_count, MAX_NUM_CLIENTS);
        for (i = 0; i < info->param.sta_list.sta_count; i++) {
            memcpy(pmpriv->adapter, info->param.sta_list.info[i].mac_address,
                   tlv->mac_address, MLAN_MAC_ADDR_LENGTH);
            info->param.sta_list.info[i].power_mfg_status =
                tlv->power_mfg_status;
            info->param.sta_list.info[i].rssi = tlv->rssi;
            tlv++;
        }
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function prepares command of get_hw_spec.
 *  
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param pcmd         A pointer to HostCmd_DS_COMMAND structure
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
static mlan_status
wlan_uap_cmd_get_hw_spec(IN pmlan_private pmpriv, IN HostCmd_DS_COMMAND * pcmd)
{
    HostCmd_DS_GET_HW_SPEC *hw_spec = &pcmd->params.hw_spec;

    ENTER();

    pcmd->command = wlan_cpu_to_le16(HostCmd_CMD_GET_HW_SPEC);
    pcmd->size = wlan_cpu_to_le16(sizeof(HostCmd_DS_GET_HW_SPEC) + S_DS_GEN);
    memcpy(pmpriv->adapter, hw_spec->permanent_addr, pmpriv->curr_addr,
           MLAN_MAC_ADDR_LENGTH);

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function handles the command response of get_hw_spec
 *  
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to command buffer
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status
wlan_uap_ret_get_hw_spec(IN pmlan_private pmpriv,
                         IN HostCmd_DS_COMMAND * resp, IN t_void * pioctl_buf)
{
    HostCmd_DS_GET_HW_SPEC *hw_spec = &resp->params.hw_spec;
    mlan_adapter *pmadapter = pmpriv->adapter;
    mlan_status ret = MLAN_STATUS_SUCCESS;
    t_u32 i;

    ENTER();

    pmadapter->fw_cap_info = wlan_le32_to_cpu(hw_spec->fw_cap_info);
    pmadapter->fw_release_number = hw_spec->fw_release_number;
    PRINTM(MINFO, "GET_HW_SPEC: fw_release_number- 0x%X\n",
           wlan_le32_to_cpu(pmadapter->fw_release_number));
    PRINTM(MINFO, "GET_HW_SPEC: Permanent addr- %2x:%2x:%2x:%2x:%2x:%2x\n",
           hw_spec->permanent_addr[0], hw_spec->permanent_addr[1],
           hw_spec->permanent_addr[2], hw_spec->permanent_addr[3],
           hw_spec->permanent_addr[4], hw_spec->permanent_addr[5]);
    PRINTM(MINFO, "GET_HW_SPEC: hw_if_version=0x%X  version=0x%X\n",
           wlan_le16_to_cpu(hw_spec->hw_if_version),
           wlan_le16_to_cpu(hw_spec->version));

    if (pmpriv->curr_addr[0] == 0xff)
        memmove(pmpriv->adapter, pmpriv->curr_addr, hw_spec->permanent_addr,
                MLAN_MAC_ADDR_LENGTH);

    pmadapter->mp_end_port = wlan_le16_to_cpu(hw_spec->mp_end_port);

    for (i = 1; i <= (unsigned) (MAX_PORT - pmadapter->mp_end_port); i++) {
        pmadapter->mp_data_port_mask &= ~(1 << (MAX_PORT - i));
    }

    LEAVE();
    return ret;
}

/**
 *  @brief This function will return the pointer to station wapi key in station list
 *  		table which matches the give mac address
 *  
 *  @param priv    A pointer to mlan_private
 *  @param mac     mac address to find in station list table
 *
 *  @return	   A pointer to structure wapi_key_tbl
 */
sta_node *
wlan_get_station_entry(mlan_private * priv, t_u8 * mac)
{
    sta_node *sta_ptr;

    ENTER();

    if (!mac) {
        LEAVE();
        return MNULL;
    }
    if (!(sta_ptr = (sta_node *) util_peek_list(priv->adapter->pmoal_handle,
                                                &priv->sta_list,
                                                priv->adapter->callbacks.
                                                moal_spin_lock,
                                                priv->adapter->callbacks.
                                                moal_spin_unlock))) {
        LEAVE();
        return MNULL;
    }
    while (sta_ptr != (sta_node *) & priv->sta_list) {
        if (!memcmp
            (priv->adapter, sta_ptr->mac_addr, mac, MLAN_MAC_ADDR_LENGTH)) {
            LEAVE();
            return sta_ptr;
        }
        sta_ptr = sta_ptr->pnext;
    }
    LEAVE();
    return MNULL;
}

/**
 *  @brief This function will return the pointer to station wapi key in station list
 *  		table which matches the give mac address
 *  
 *  @param priv    A pointer to mlan_private
 *  @param mac     mac address to find in station list table
 *
 *  @return	   A pointer to structure wapi_key_tbl
 */
sta_node *
wlan_add_station_entry(mlan_private * priv, t_u8 * mac)
{
    sta_node *sta_ptr = MNULL;

    ENTER();
    sta_ptr = wlan_get_station_entry(priv, mac);
    if (sta_ptr)
        goto done;
    if (priv->adapter->callbacks.
        moal_malloc(priv->adapter->pmoal_handle, sizeof(sta_node), MLAN_MEM_DEF,
                    (t_u8 **) & sta_ptr)) {
        PRINTM(MERROR, "Failed to allocate memory for station node\n");
        return MNULL;
    }
    memset(priv->adapter, sta_ptr, 0, sizeof(sta_node));
    memcpy(priv->adapter, sta_ptr->mac_addr, mac, MLAN_MAC_ADDR_LENGTH);
    util_enqueue_list_tail(priv->adapter->pmoal_handle, &priv->sta_list,
                           (pmlan_linked_list) sta_ptr,
                           priv->adapter->callbacks.moal_spin_lock,
                           priv->adapter->callbacks.moal_spin_unlock);
  done:
    LEAVE();
    return sta_ptr;
}

/**
 *  @brief This function will delete a station entry from station list
 *  		
 *  
 *  @param priv    A pointer to mlan_private
 *  @param mac     station's mac address 
 *
 *  @return	   N/A
 */
t_void
wlan_delete_station_entry(mlan_private * priv, t_u8 * mac)
{
    sta_node *sta_ptr = MNULL;
    if ((sta_ptr = wlan_get_station_entry(priv, mac))) {
        util_unlink_list(priv->adapter->pmoal_handle, &priv->sta_list,
                         (pmlan_linked_list) sta_ptr,
                         priv->adapter->callbacks.moal_spin_lock,
                         priv->adapter->callbacks.moal_spin_unlock);
        priv->adapter->callbacks.moal_mfree(priv->adapter->pmoal_handle,
                                            (t_u8 *) sta_ptr);
    }
    return;
}

/**
 *  @brief Clean up wapi station list
 *
 *  @param priv  Pointer to the mlan_private driver data struct
 *
 *  @return      N/A
 */
t_void
wlan_delete_station_list(pmlan_private priv)
{
    sta_node *sta_ptr;

    while ((sta_ptr =
            (sta_node *) util_dequeue_list(priv->adapter->pmoal_handle,
                                           &priv->sta_list,
                                           priv->adapter->callbacks.
                                           moal_spin_lock,
                                           priv->adapter->callbacks.
                                           moal_spin_unlock))) {
        priv->adapter->callbacks.moal_mfree(priv->adapter->pmoal_handle,
                                            (t_u8 *) sta_ptr);
    }
    return;
}

/**
 *  @brief This function will search for the specific ie
 *  		
 *  
 *  @param priv    A pointer to mlan_private
 *  @param ie_buf  A pointer to ie_buf
 *  @param ie_len  total ie length
 *  @param id      ie's id
 *
 *  @return	       ie's poiner or MNULL
 */
t_u8 *
wlan_get_specific_ie(pmlan_private priv, t_u8 * ie_buf, t_u8 ie_len,
                     IEEEtypes_ElementId_e id)
{
    t_u32 bytes_left = ie_len;
    t_u8 *pcurrent_ptr = ie_buf;
    t_u16 total_ie_len;
    t_u8 *ie_ptr = MNULL;
    IEEEtypes_ElementId_e element_id;
    t_u8 element_len;
    DBG_HEXDUMP(MCMD_D, "ie", ie_buf, ie_len);
    while (bytes_left >= 2) {
        element_id = (IEEEtypes_ElementId_e) (*((t_u8 *) pcurrent_ptr));
        element_len = *((t_u8 *) pcurrent_ptr + 1);
        total_ie_len = element_len + sizeof(IEEEtypes_Header_t);
        if (bytes_left < total_ie_len) {
            PRINTM(MERROR, "InterpretIE: Error in processing IE, "
                   "bytes left < IE length\n");
            break;
        }
        if (element_id == id) {
            PRINTM(MCMND, "Find IE: id=%d\n", id);
            DBG_HEXDUMP(MCMND, "IE", pcurrent_ptr, total_ie_len);
            ie_ptr = pcurrent_ptr;
            break;
        }
        pcurrent_ptr += element_len + 2;
        /* Need to account for IE ID and IE Len */
        bytes_left -= (element_len + 2);
    }
    return ie_ptr;
}

#define ASSOC_EVENT_FIX_SIZE    12
/**
 *  @brief This function will search for the specific ie
 *  		
 *  
 *  @param priv    A pointer to mlan_private
 *  @param pevent  A pointer to event buf
 *
 *  @return	       MTRUE/MFALSE;
 */
t_u32
wlan_check_11n_capability(pmlan_private priv, pmlan_buffer pevent)
{
    t_u32 ret = MFALSE;
    t_u16 tlv_type, tlv_len;
    t_u8 *assoc_req_ie = MNULL;
    t_u8 ie_len = 0;
    t_u8 *pht_cap = MNULL;
    int tlv_buf_left = pevent->data_len - ASSOC_EVENT_FIX_SIZE;
    MrvlIEtypesHeader_t *tlv =
        (MrvlIEtypesHeader_t *) (pevent->pbuf + pevent->data_offset +
                                 ASSOC_EVENT_FIX_SIZE);
    MrvlIETypes_MgmtFrameSet_t *mgmt_tlv = MNULL;
    while (tlv_buf_left >= (int) sizeof(MrvlIEtypesHeader_t)) {
        tlv_type = wlan_le16_to_cpu(tlv->type);
        tlv_len = wlan_le16_to_cpu(tlv->len);
        if ((sizeof(MrvlIEtypesHeader_t) + tlv_len) >
            (unsigned int) tlv_buf_left) {
            PRINTM(MERROR, "wrong tlv: tlvLen=%d, tlvBufLeft=%d\n", tlv_len,
                   tlv_buf_left);
            break;
        }
        if (tlv_type == TLV_TYPE_UAP_MGMT_FRAME) {
            mgmt_tlv = (MrvlIETypes_MgmtFrameSet_t *) tlv;
            if ((mgmt_tlv->frame_control.type == 0) &&
                (mgmt_tlv->frame_control.sub_type == SUBTYPE_ASSOC_REQUEST)) {
                ie_len =
                    tlv_len - sizeof(IEEEtypes_FrameCtl_t) -
                    sizeof(IEEEtypes_ReAssocRqst_t);
                assoc_req_ie =
                    (t_u8 *) tlv + sizeof(MrvlIETypes_MgmtFrameSet_t) +
                    sizeof(IEEEtypes_ReAssocRqst_t);
                pht_cap =
                    wlan_get_specific_ie(priv, assoc_req_ie, ie_len,
                                         HT_CAPABILITY);
                if (pht_cap) {
                    PRINTM(MCMND, "STA supports 11n\n");
                    ret = MTRUE;
                } else {
                    PRINTM(MCMND, "STA doesn't support 11n\n");
                }
                break;
            }
        }
        tlv_buf_left -= (sizeof(MrvlIEtypesHeader_t) + tlv_len);
        tlv =
            (MrvlIEtypesHeader_t *) ((t_u8 *) tlv + tlv_len +
                                     sizeof(MrvlIEtypesHeader_t));
    }
    return ret;
}

#define BSS_START_EVENT_FIX_SIZE    12
/**
 *  @brief This function will search for the specific ie
 *  		
 *  
 *  @param priv    A pointer to mlan_private
 *  @param pevent  A pointer to event buf
 *
 *  @return	       N/A
 */
void
wlan_check_uap_capability(pmlan_private priv, pmlan_buffer pevent)
{
    t_u16 tlv_type, tlv_len;
    int tlv_buf_left = pevent->data_len - BSS_START_EVENT_FIX_SIZE;
    MrvlIEtypesHeader_t *tlv =
        (MrvlIEtypesHeader_t *) (pevent->pbuf + pevent->data_offset +
                                 BSS_START_EVENT_FIX_SIZE);
    const t_u8 wmm_oui[4] = { 0x00, 0x50, 0xf2, 0x02 };
    IEEEtypes_WmmParameter_t *pWmmParamIe = MNULL;
    priv->wmm_enabled = MFALSE;
    priv->is_11n_enabled = MFALSE;
    priv->pkt_fwd = MFALSE;
    while (tlv_buf_left >= (int) sizeof(MrvlIEtypesHeader_t)) {
        tlv_type = wlan_le16_to_cpu(tlv->type);
        tlv_len = wlan_le16_to_cpu(tlv->len);
        if ((sizeof(MrvlIEtypesHeader_t) + tlv_len) >
            (unsigned int) tlv_buf_left) {
            PRINTM(MERROR, "wrong tlv: tlvLen=%d, tlvBufLeft=%d\n", tlv_len,
                   tlv_buf_left);
            break;
        }
        if (tlv_type == VENDOR_SPECIFIC_221) {
            if (!memcmp
                (priv->adapter, (t_u8 *) tlv + sizeof(MrvlIEtypesHeader_t),
                 wmm_oui, sizeof(wmm_oui))) {
                DBG_HEXDUMP(MCMD_D, "wmm ie tlv", tlv,
                            tlv_len + sizeof(MrvlIEtypesHeader_t));
                priv->wmm_enabled = MFALSE;
                wlan_wmm_setup_ac_downgrade(priv);
                priv->wmm_enabled = MTRUE;
                pWmmParamIe = (IEEEtypes_WmmParameter_t *) ((t_u8 *) tlv + 2);
                pWmmParamIe->vend_hdr.len = (t_u8) tlv_len;
                pWmmParamIe->vend_hdr.element_id = WMM_IE;
                wlan_wmm_setup_queue_priorities(priv, pWmmParamIe);
            }
        }
        if (tlv_type == HT_CAPABILITY) {
            DBG_HEXDUMP(MCMD_D, "HT_CAP tlv", tlv,
                        tlv_len + sizeof(MrvlIEtypesHeader_t));
            priv->is_11n_enabled = MTRUE;
        }
        if (tlv_type == TLV_TYPE_UAP_PKT_FWD_CTL) {
            DBG_HEXDUMP(MCMD_D, "pkt_fwd tlv", tlv,
                        tlv_len + sizeof(MrvlIEtypesHeader_t));
            /* We will disable host pkt forwarding if firmware's pkt_fwd_ctl is 
               enabled */
            priv->pkt_fwd =
                (*((t_u8 *) tlv + sizeof(MrvlIEtypesHeader_t)) == 1) ? 0 : 1;
            PRINTM(MCMND, "pkt_fwd=%d\n", priv->pkt_fwd);
        }
        tlv_buf_left -= (sizeof(MrvlIEtypesHeader_t) + tlv_len);
        tlv =
            (MrvlIEtypesHeader_t *) ((t_u8 *) tlv + tlv_len +
                                     sizeof(MrvlIEtypesHeader_t));
    }
    if (priv->wmm_enabled == MFALSE) {
        /* Since WMM is not enabled, setup the queues with the defaults */
        wlan_wmm_setup_queues(priv);
    }
}

/********************************************************
    Global Functions
********************************************************/
/** 
 *  @brief This function prepare the command before sending to firmware.
 *  
 *  @param priv       A pointer to mlan_private structure
 *  @param cmd_no       Command number
 *  @param cmd_action   Command action: GET or SET
 *  @param cmd_oid      Cmd oid: treated as sub command
 *  @param pioctl_buf   A pointer to MLAN IOCTL Request buffer
 *  @param pdata_buf    A pointer to information buffer
 *  @param pcmd_buf      A pointer to cmd buf
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
mlan_uap_prepare_cmd(IN t_void * priv,
                     IN t_u16 cmd_no,
                     IN t_u16 cmd_action,
                     IN t_u32 cmd_oid,
                     IN t_void * pioctl_buf,
                     IN t_void * pdata_buf, IN t_void * pcmd_buf)
{
    HostCmd_DS_COMMAND *cmd_ptr = (HostCmd_DS_COMMAND *) pcmd_buf;
    mlan_private *pmpriv = (mlan_private *) priv;
    mlan_status ret = MLAN_STATUS_SUCCESS;

    ENTER();

    /* Prepare command */
    switch (cmd_no) {
    case HostCmd_CMD_SOFT_RESET:
    case HOST_CMD_APCMD_BSS_STOP:
    case HOST_CMD_APCMD_BSS_START:
    case HOST_CMD_APCMD_SYS_INFO:
    case HOST_CMD_APCMD_SYS_RESET:
    case HOST_CMD_APCMD_STA_LIST:
        cmd_ptr->command = wlan_cpu_to_le16(cmd_no);
        cmd_ptr->size = wlan_cpu_to_le16(S_DS_GEN);
        break;
    case HOST_CMD_APCMD_SYS_CONFIGURE:
        ret =
            wlan_uap_cmd_sys_configure(pmpriv, cmd_ptr, cmd_action,
                                       (pmlan_ioctl_req) pioctl_buf);
        break;
    case HostCmd_CMD_802_11_PS_MODE_ENH:
        ret =
            wlan_cmd_enh_power_mode(pmpriv, cmd_ptr, cmd_action,
                                    (t_u16) cmd_oid, pdata_buf);
        break;
    case HostCmd_CMD_FUNC_INIT:
        if (pmpriv->adapter->hw_status == WlanHardwareStatusReset)
            pmpriv->adapter->hw_status = WlanHardwareStatusReady;
        cmd_ptr->command = wlan_cpu_to_le16(cmd_no);
        cmd_ptr->size = wlan_cpu_to_le16(S_DS_GEN);
        break;
    case HostCmd_CMD_FUNC_SHUTDOWN:
        pmpriv->adapter->hw_status = WlanHardwareStatusReset;
        cmd_ptr->command = wlan_cpu_to_le16(cmd_no);
        cmd_ptr->size = wlan_cpu_to_le16(S_DS_GEN);
        break;
    case HostCmd_CMD_802_11_SNMP_MIB:
        ret =
            wlan_uap_cmd_snmp_mib(pmpriv, cmd_ptr, cmd_action,
                                  (pmlan_ioctl_req) pioctl_buf);
        break;
    case HOST_CMD_APCMD_STA_DEAUTH:
        ret = wlan_uap_cmd_sta_deauth(pmpriv, cmd_ptr, pdata_buf);
        break;
    case HostCmd_CMD_GET_HW_SPEC:
        ret = wlan_uap_cmd_get_hw_spec(pmpriv, cmd_ptr);
        break;
    case HostCmd_CMD_802_11_HS_CFG_ENH:
        ret =
            wlan_uap_cmd_802_11_hs_cfg(pmpriv, cmd_ptr, cmd_action,
                                       (HostCmd_DS_802_11_HS_CFG_ENH *)
                                       pdata_buf);
        break;
    case HostCmd_CMD_RECONFIGURE_TX_BUFF:
        ret = wlan_cmd_recfg_tx_buf(pmpriv, cmd_ptr, cmd_action, pdata_buf);
        break;
    case HostCmd_CMD_AMSDU_AGGR_CTRL:
        ret = wlan_cmd_amsdu_aggr_ctrl(pmpriv, cmd_ptr, cmd_action, pdata_buf);
        break;
    case HostCmd_CMD_11N_ADDBA_REQ:
        ret = wlan_cmd_11n_addba_req(pmpriv, cmd_ptr, pdata_buf);
        break;
    case HostCmd_CMD_11N_DELBA:
        ret = wlan_cmd_11n_delba(pmpriv, cmd_ptr, pdata_buf);
        break;
    case HostCmd_CMD_11N_ADDBA_RSP:
        ret = wlan_cmd_11n_addba_rspgen(pmpriv, cmd_ptr, pdata_buf);
        break;
    default:
        PRINTM(MERROR, "PREP_CMD: unknown command- %#x\n", cmd_no);
        ret = MLAN_STATUS_FAILURE;
        break;
    }
    LEAVE();
    return ret;
}

/** 
 *  @brief This function handles the AP mode command response
 *  
 *  @param priv             A pointer to mlan_private structure
 *  @param cmdresp_no       cmd no
 *  @param pcmd_buf         cmdresp buf
 *  @param pioctl           A pointer to ioctl buf
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
mlan_process_uap_cmdresp(IN t_void * priv,
                         IN t_u16 cmdresp_no,
                         IN t_void * pcmd_buf, IN t_void * pioctl)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = (mlan_private *) priv;
    HostCmd_DS_COMMAND *resp = (HostCmd_DS_COMMAND *) pcmd_buf;
    mlan_ioctl_req *pioctl_buf = (mlan_ioctl_req *) pioctl;
    mlan_adapter *pmadapter = pmpriv->adapter;
    int ctr;

    ENTER();

    /* If the command is not successful, cleanup and return failure */
    if (resp->result != HostCmd_RESULT_OK) {
        uap_process_cmdresp_error(pmpriv, resp, pioctl_buf);
        return MLAN_STATUS_FAILURE;
    }

    /* Command successful, handle response */
    switch (cmdresp_no) {
    case HOST_CMD_APCMD_BSS_STOP:
    case HOST_CMD_APCMD_BSS_START:
    case HOST_CMD_APCMD_SYS_RESET:
        break;
    case HOST_CMD_APCMD_SYS_INFO:
        ret = wlan_uap_ret_sys_info(pmpriv, resp, pioctl_buf);
        break;
    case HOST_CMD_APCMD_SYS_CONFIGURE:
        ret = wlan_uap_ret_sys_config(pmpriv, resp, pioctl_buf);
        break;
    case HostCmd_CMD_802_11_PS_MODE_ENH:
        ret = wlan_ret_enh_power_mode(pmpriv, resp, pioctl_buf);
        break;
    case HostCmd_CMD_FUNC_INIT:
    case HostCmd_CMD_FUNC_SHUTDOWN:
        break;
    case HostCmd_CMD_802_11_SNMP_MIB:
        ret = wlan_uap_ret_snmp_mib(pmpriv, resp, pioctl_buf);
        break;
    case HOST_CMD_APCMD_STA_DEAUTH:
        break;
    case HOST_CMD_APCMD_STA_LIST:
        ret = wlan_uap_ret_sta_list(pmpriv, resp, pioctl_buf);
        break;
    case HostCmd_CMD_GET_HW_SPEC:
        ret = wlan_uap_ret_get_hw_spec(pmpriv, resp, pioctl_buf);
        break;
    case HostCmd_CMD_802_11_HS_CFG_ENH:
        ret = wlan_ret_802_11_hs_cfg(pmpriv, resp, pioctl_buf);
        break;
    case HostCmd_CMD_11N_ADDBA_REQ:
        ret = wlan_ret_11n_addba_req(pmpriv, resp);
        break;
    case HostCmd_CMD_11N_DELBA:
        ret = wlan_ret_11n_delba(pmpriv, resp);
        break;
    case HostCmd_CMD_11N_ADDBA_RSP:
        ret = wlan_ret_11n_addba_resp(pmpriv, resp);
        break;
    case HostCmd_CMD_RECONFIGURE_TX_BUFF:
        pmadapter->tx_buf_size =
            (t_u16) wlan_le16_to_cpu(resp->params.tx_buf.buff_size);
        pmadapter->curr_tx_buf_size =
            (t_u16) wlan_le16_to_cpu(resp->params.tx_buf.buff_size);
        pmadapter->mp_end_port =
            wlan_le16_to_cpu(resp->params.tx_buf.mp_end_port);
        pmadapter->mp_data_port_mask = DATA_PORT_MASK;

        for (ctr = 1; ctr <= MAX_PORT - pmadapter->mp_end_port; ctr++) {
            pmadapter->mp_data_port_mask &= ~(1 << (MAX_PORT - ctr));
        }
        pmadapter->curr_wr_port = 1;
        PRINTM(MCMND, "end port %d, data port mask %x\n",
               wlan_le16_to_cpu(resp->params.tx_buf.mp_end_port),
               pmadapter->mp_data_port_mask);
        PRINTM(MCMND, "max_tx_buf_size=%d, tx_buf_size=%d\n",
               pmadapter->max_tx_buf_size, pmadapter->tx_buf_size);
        break;
    case HostCmd_CMD_AMSDU_AGGR_CTRL:
        ret = wlan_ret_amsdu_aggr_ctrl(pmpriv, resp, pioctl_buf);
        break;
    default:
        PRINTM(MERROR, "CMD_RESP: Unknown command response %#x\n",
               resp->command);
        break;
    }
    LEAVE();
    return ret;
}

/** 
 *  @brief This function handles events generated by firmware
 *  
 *  @param priv		A pointer to mlan_private structure
 *
 *  @return		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
mlan_process_uap_event(IN t_void * priv)
{
    pmlan_private pmpriv = (pmlan_private) priv;
    pmlan_adapter pmadapter = pmpriv->adapter;
    pmlan_callbacks pcb = &pmadapter->callbacks;
    mlan_status ret = MLAN_STATUS_SUCCESS;
    t_u32 eventcause = pmadapter->event_cause;
    pmlan_buffer pmbuf = pmadapter->pmlan_buffer_event;
    t_u8 *event_buf = MNULL;
    mlan_event *pevent = MNULL;
    t_u8 sta_addr[MLAN_MAC_ADDR_LENGTH];
    sta_node *sta_ptr = MNULL;
    t_u8 i = 0;

    ENTER();

    /* Allocate memory for event buffer */
    ret =
        pcb->moal_malloc(pmadapter->pmoal_handle, MAX_EVENT_SIZE, MLAN_MEM_DEF,
                         &event_buf);
    if ((ret != MLAN_STATUS_SUCCESS) || !event_buf) {
        PRINTM(MERROR, "Could not allocate buffer for event buf\n");
        goto done;
    }
    pevent = (pmlan_event) event_buf;
    pevent->event_id = 0;

    memcpy(pmadapter, pmbuf->pbuf + pmbuf->data_offset, (t_u8 *) & eventcause,
           sizeof(eventcause));

    switch (eventcause) {
    case EVENT_MICRO_AP_BSS_START:
        memcpy(pmadapter, pmpriv->curr_addr, pmadapter->event_body + 2,
               MLAN_MAC_ADDR_LENGTH);
        pevent->event_id = MLAN_EVENT_ID_UAP_FW_BSS_START;
        wlan_check_uap_capability(pmpriv, pmbuf);
        break;
    case EVENT_MICRO_AP_BSS_ACTIVE:
        pmpriv->media_connected = MTRUE;
        pevent->event_id = MLAN_EVENT_ID_UAP_FW_BSS_ACTIVE;
        break;
    case EVENT_MICRO_AP_BSS_IDLE:
        pevent->event_id = MLAN_EVENT_ID_UAP_FW_BSS_IDLE;
        pmpriv->media_connected = MFALSE;
        wlan_clean_txrx(pmpriv);
        wlan_delete_station_list(pmpriv);
        break;
    case EVENT_PS_AWAKE:
        PRINTM(MINFO, "EVENT: AWAKE \n");
        PRINTM(MEVENT, "||");
        pmadapter->pm_wakeup_card_req = MFALSE;
        pmadapter->pm_wakeup_fw_try = MFALSE;
        pmadapter->ps_state = PS_STATE_AWAKE;
        break;
    case EVENT_PS_SLEEP:
        PRINTM(MINFO, "EVENT: SLEEP\n");
        PRINTM(MEVENT, "__");
        /* Handle unexpected PS SLEEP event */
        pmadapter->ps_state = PS_STATE_PRE_SLEEP;
        wlan_check_ps_cond(pmadapter);
        break;
    case EVENT_MICRO_AP_STA_ASSOC:
        memcpy(pmadapter, sta_addr, pmadapter->event_body + 2,
               MLAN_MAC_ADDR_LENGTH);
        sta_ptr = wlan_add_station_entry(pmpriv, sta_addr);
        if (pmpriv->is_11n_enabled) {
            sta_ptr->is_11n_enabled = wlan_check_11n_capability(pmpriv, pmbuf);
            for (i = 0; i < MAX_NUM_TID; i++) {
                if (sta_ptr->is_11n_enabled)
                    sta_ptr->ampdu_sta[i] = pmpriv->aggr_prio_tbl[i].ampdu_user;
                else
                    sta_ptr->ampdu_sta[i] = BA_STREAM_NOT_ALLOWED;
            }
        }
        pevent->event_id = MLAN_EVENT_ID_DRV_PASSTHU;
        break;
    case EVENT_MICRO_AP_STA_DEAUTH:
        memcpy(pmadapter, sta_addr, pmadapter->event_body + 2,
               MLAN_MAC_ADDR_LENGTH);
        pmadapter->callbacks.moal_spin_lock(pmadapter->pmoal_handle,
                                            pmpriv->wmm.ra_list_spinlock);
        if (pmpriv->is_11n_enabled) {
            wlan_uap_11n_cleanup_txbastream_tbl(pmpriv, sta_addr);
            wlan_uap_11n_cleanup_reorder_tbl(pmpriv, sta_addr);
        }
        pmadapter->callbacks.moal_spin_unlock(pmadapter->pmoal_handle,
                                              pmpriv->wmm.ra_list_spinlock);
        wlan_delete_station_entry(pmpriv, sta_addr);
        pevent->event_id = MLAN_EVENT_ID_DRV_PASSTHU;
        break;
    case EVENT_HS_ACT_REQ:
        PRINTM(MEVENT, "EVENT: HS_ACT_REQ\n");
        ret =
            wlan_prepare_cmd(pmpriv, HostCmd_CMD_802_11_HS_CFG_ENH, 0, 0, MNULL,
                             MNULL);
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
        pmadapter->tx_buf_size =
            MIN(pmadapter->curr_tx_buf_size,
                wlan_le16_to_cpu(*(t_u16 *) pmadapter->event_body));
        PRINTM(MEVENT, "tx_buf_size %d\n", pmadapter->tx_buf_size);
        break;
    default:
        pevent->event_id = MLAN_EVENT_ID_DRV_PASSTHU;
        break;
    }

    if (pevent->event_id) {
        pevent->bss_num = pmpriv->bss_index;
        pevent->event_len = pmbuf->data_len;
        memcpy(pmadapter, (t_u8 *) pevent->event_buf,
               pmbuf->pbuf + pmbuf->data_offset, pevent->event_len);
        wlan_recv_event(pmpriv, pevent->event_id, pevent);
    }
  done:
    if (event_buf)
        pcb->moal_mfree(pmadapter->pmoal_handle, event_buf);
    LEAVE();
    return ret;
}

/**
 *  @brief  This function issues commands to initialize firmware
 *
 *  @param priv     	A pointer to mlan_private structure
 *  @param first_sta	flag for first station
 *
 *  @return		MLAN_STATUS_SUCCESS or error code
 */
mlan_status
mlan_uap_init_cmd(IN t_void * priv, IN t_u8 first_sta)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    pmlan_private pmpriv = (pmlan_private) priv;
    t_u16 last_cmd = 0;

    ENTER();

    if (!first_sta)
        goto get_mac;
    ret = wlan_prepare_cmd(pmpriv, HostCmd_CMD_FUNC_INIT,
                           HostCmd_ACT_GEN_SET, 0, MNULL, MNULL);
    if (ret) {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }
    last_cmd = HostCmd_CMD_FUNC_INIT;
    ret = wlan_prepare_cmd(pmpriv, HostCmd_CMD_GET_HW_SPEC,
                           HostCmd_ACT_GEN_GET, 0, MNULL, MNULL);
    if (ret) {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }
    last_cmd = HostCmd_CMD_GET_HW_SPEC;
  get_mac:
    ret = wlan_prepare_cmd(pmpriv, HOST_CMD_APCMD_SYS_CONFIGURE,
                           HostCmd_ACT_GEN_GET, 0, MNULL, MNULL);
    if (ret) {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }
    last_cmd = HOST_CMD_APCMD_SYS_CONFIGURE;
    /** set last_init_cmd */
    if (last_cmd) {
        pmpriv->adapter->last_init_cmd = last_cmd;
        ret = MLAN_STATUS_PENDING;
    }
  done:
    LEAVE();
    return ret;
}
