/** @file mlan_cmd.c
 *  
 *  @brief This file contains the handling of command.
 *  it prepares command and sends it to firmware when
 *  it is ready.
 * 
 *  Copyright (C) 2008-2009, Marvell International Ltd. 
 *  All Rights Reserved
 * 
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

/** List of commands allowed in Power Save mode */
static t_u16 Commands_Allowed_In_PS[] = {
    HostCmd_CMD_RSSI_INFO,
    HostCmd_CMD_802_11_HOST_SLEEP_CFG,
    HostCmd_CMD_802_11_WAKEUP_CONFIRM,
    HostCmd_CMD_MAC_CONTROL,
    HostCmd_CMD_MAC_MULTICAST_ADR,
    HostCmd_CMD_802_11_MAC_ADDRESS,
};

/********************************************************
                Global Variables
********************************************************/

/********************************************************
                Local Functions
********************************************************/

/** 
 *  @brief This function initializes the command node.
 *  
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param pcmd_node    A pointer to cmd_ctrl_node structure
 *  @param cmd_oid      Cmd oid: treated as sub command
 *  @param pioctl_buf   A pointer to MLAN IOCTl Request buffer
 *  @param pdata_buf    A pointer to information buffer
 *
 *  @return             N/A
 */
void
wlan_init_cmd_node(IN pmlan_private pmpriv,
                   IN cmd_ctrl_node * pcmd_node,
                   IN t_u32 cmd_oid,
                   IN t_void * pioctl_buf, IN t_void * pdata_buf)
{
    ENTER();

    if (pcmd_node == MNULL) {
        LEAVE();
        return;
    }

    pcmd_node->priv = pmpriv;
    pcmd_node->cmd_oid = cmd_oid;
    pcmd_node->pioctl_buf = pioctl_buf;
    pcmd_node->pdata_buf = pdata_buf;

    /* Prepare mlan_buffer for command sending */
    pcmd_node->mbuf.buf_type = MLAN_BUF_TYPE_CMD;
    pcmd_node->mbuf.data_offset = MLAN_USB_TYPE_LEN;
    pcmd_node->mbuf.pbuf = (t_u8 *) pcmd_node->pcmd_buf;

    LEAVE();
}

/** 
 *  @brief This function gets a free command node if available in
 *              command free queue.
 *  
 *  @param pmadapter        A pointer to mlan_adapter structure
 *
 *  @return cmd_ctrl_node   A pointer to cmd_ctrl_node structure or MNULL
 */
cmd_ctrl_node *
wlan_get_cmd_node(mlan_adapter * pmadapter)
{
    cmd_ctrl_node *pcmd_node;

    ENTER();

    if (pmadapter == MNULL) {
        LEAVE();
        return MNULL;
    }

    if (util_peek_list(&pmadapter->cmd_free_q,
                       pmadapter->callbacks.moal_spin_lock,
                       pmadapter->callbacks.moal_spin_unlock)) {
        pcmd_node = (cmd_ctrl_node *) util_dequeue_list(&pmadapter->cmd_free_q,
                                                        pmadapter->callbacks.
                                                        moal_spin_lock,
                                                        pmadapter->callbacks.
                                                        moal_spin_unlock);
    } else {
        PRINTM(ERROR, "GET_CMD_NODE: cmd_ctrl_node is not available\n");
        pcmd_node = MNULL;
    }

    LEAVE();
    return pcmd_node;
}

/** 
 *  @brief This function cleans command node.
 *  
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param pcmd_node    A pointer to cmd_ctrl_node structure
 *
 *  @return             N/A
 */
t_void
wlan_clean_cmd_node(pmlan_adapter pmadapter, cmd_ctrl_node * pcmd_node)
{
    ENTER();

    if (pcmd_node == MNULL) {
        LEAVE();
        return;
    }
    pcmd_node->cmd_oid = 0;
    pcmd_node->cmd_flag = 0;
    pcmd_node->pioctl_buf = MNULL;
    pcmd_node->pdata_buf = MNULL;

    if (pcmd_node->pcmd_buf)
        memset(pcmd_node->pcmd_buf, 0, MRVDRV_SIZE_OF_CMD_BUFFER);
    memset(&pcmd_node->mbuf, 0, sizeof(mlan_buffer));

    LEAVE();
    return;
}

/** 
 *  @brief This function checks if a command is allowed
 *              in PS mode or not.
 *  
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param command              The command ID
 *
 *  @return             MTRUE or MFALSE
 */
static t_u8
wlan_is_cmd_allowed_in_ps(pmlan_adapter pmadapter, t_u16 command)
{
    t_u32 count = sizeof(Commands_Allowed_In_PS)
        / sizeof(Commands_Allowed_In_PS[0]);
    t_u32 i;

    ENTER();

    for (i = 0; i < count; i++)
        if (command == wlan_cpu_to_le16(Commands_Allowed_In_PS[i])) {
            LEAVE();
            return MTRUE;
        }

    LEAVE();
    return MFALSE;
}

/**
 *  @brief This function sends sleep confirm command to firmware.
 * 
 *  @param priv    A pointer to mlan_private structure
 *
 *  @return        MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_dnld_sleep_confirm_cmd(pmlan_private priv)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    pmlan_adapter pmadapter = priv->adapter;
    mlan_buffer sleep_cfm;
    static t_u32 i = 0;

    ENTER();

    memset(&pmadapter->sleep_cfm_buf, 0, sizeof(sleep_confirm_buffer));
    pmadapter->sleep_cfm_buf.hdr = wlan_cpu_to_le32(MLAN_USB_TYPE_CMD);
    pmadapter->sleep_cfm_buf.ps_cfm_sleep.seq_num =
        wlan_cpu_to_le16(++pmadapter->seq_num);
    pmadapter->sleep_cfm_buf.ps_cfm_sleep.command =
        wlan_cpu_to_le16(HostCmd_CMD_802_11_PS_MODE);
    pmadapter->sleep_cfm_buf.ps_cfm_sleep.size =
        wlan_cpu_to_le16(sizeof(PS_CMD_ConfirmSleep));
    pmadapter->sleep_cfm_buf.ps_cfm_sleep.result = 0;
    pmadapter->sleep_cfm_buf.ps_cfm_sleep.action =
        wlan_cpu_to_le16(HostCmd_SubCmd_Sleep_Confirmed);

    HEXDUMP("SLEEP_CFM", &pmadapter->sleep_cfm_buf,
            sizeof(sleep_confirm_buffer));

    sleep_cfm.pbuf = (t_u8 *) & pmadapter->sleep_cfm_buf;
    sleep_cfm.data_offset = 0;
    sleep_cfm.data_len = sizeof(sleep_confirm_buffer);
    sleep_cfm.buf_type = MLAN_BUF_TYPE_CMD;

    /* Send SLEEP_CONFIRM command to lower layer */
    ret = pmadapter->callbacks.moal_write_data_async(pmadapter->pmoal_handle,
                                                     &sleep_cfm,
                                                     MLAN_USB_EP_CMD_EVENT);

    if (ret == MLAN_STATUS_FAILURE) {
        PRINTM(ERROR, "SLEEP_CFM: failed\n");
        pmadapter->dbg.num_cmd_sleep_cfm_host_to_card_failure++;
        goto done;
    } else {
        pmadapter->ps_state = PS_STATE_SLEEP;
        if (!pmadapter->sleep_period.period)
            pmadapter->pm_wakeup_card_req = MTRUE;

        if (pmadapter->is_hs_configured && !pmadapter->sleep_period.period) {
            pmadapter->pm_wakeup_card_req = MTRUE;
            wlan_host_sleep_activated_event(priv, MTRUE);
        }
#define NUM_SC_PER_LINE         16
        if (++i % NUM_SC_PER_LINE == 0)
            PRINTM(EVENT, "+\n");
        else
            PRINTM(EVENT, "+");
    }

  done:
    LEAVE();
    return ret;
}

/**
 *  @brief This function sends Enter_PS command to firmware.
 *
 *  @param priv         A pointer to mlan_private structure
 *
 *  @return             N/A
 */
t_void
wlan_enter_ps(pmlan_private priv)
{
    ENTER();

    wlan_prepare_cmd(priv, HostCmd_CMD_802_11_PS_MODE,
                     HostCmd_SubCmd_Enter_PS, 0, MNULL, MNULL);

    LEAVE();
    return;
}

/**
 *  @brief This function sends Exit_PS command to firmware.
 *
 *  @param priv         A pointer to mlan_private structure
 *
 *  @return             N/A
 */
t_void
wlan_exit_ps(pmlan_private priv)
{
    ENTER();

    wlan_prepare_cmd(priv, HostCmd_CMD_802_11_PS_MODE,
                     HostCmd_SubCmd_Exit_PS, 0, MNULL, MNULL);

    LEAVE();
    return;
}

/**
 *  @brief This function checks conditions and prepares to
 *              send sleep confirm command to firmware if OK.
 * 
 *  @param priv         A pointer to mlan_private structure
 *
 *  @return             N/A
 */
t_void
wlan_check_ps_cond(pmlan_private priv)
{
    pmlan_adapter pmadapter = priv->adapter;

    ENTER();

    if (!pmadapter->cmd_sent &&
        !pmadapter->curr_cmd && !IS_CARD_RX_RCVD(pmadapter)) {
        wlan_dnld_sleep_confirm_cmd(priv);
    } else {
        PRINTM(CMND, "Delay Sleep Confirm (%s%s%s)\n",
               (pmadapter->cmd_sent) ? "D" : "",
               (pmadapter->curr_cmd) ? "C" : "",
               (IS_CARD_RX_RCVD(pmadapter)) ? "R" : "");
    }

    LEAVE();
}

/** 
 *  @brief This function downloads a command to firmware.
 *  
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param pcmd_node    A pointer to cmd_ctrl_node structure
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status
wlan_dnld_cmd_to_fw(IN mlan_private * pmpriv, IN cmd_ctrl_node * pcmd_node)
{

    mlan_adapter *pmadapter = pmpriv->adapter;
    mlan_callbacks *pcb = (mlan_callbacks *) & pmadapter->callbacks;
    mlan_status ret = MLAN_STATUS_SUCCESS;
    HostCmd_DS_COMMAND *pcmd;
    mlan_ioctl_req *pioctl_buf = MNULL;
    t_u16 cmd_code;
    t_u16 cmd_size;
    t_u32 tmp;
    t_u32 sec, usec;

    ENTER();

    if (!pmadapter || !pcmd_node) {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    pcmd =
        (HostCmd_DS_COMMAND *) (pcmd_node->mbuf.pbuf +
                                pcmd_node->mbuf.data_offset);
    if (pcmd_node->pioctl_buf != MNULL)
        pioctl_buf = (mlan_ioctl_req *) pcmd_node->pioctl_buf;

    /* Sanity test */
    if (pcmd == MNULL || pcmd->size == 0) {
        PRINTM(ERROR, "DNLD_CMD: pcmd is null or command size is zero, "
               "Not sending\n");
        if (pioctl_buf != MNULL)
            pioctl_buf->status_code = MLAN_ERROR_CMD_DNLD_FAIL,
                wlan_insert_cmd_to_free_q(pmadapter, pcmd_node);
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    /* Set command sequence number */
    pmadapter->seq_num++;
    pcmd->seq_num = wlan_cpu_to_le16(pmadapter->seq_num);

    wlan_request_cmd_lock(pmadapter);
    pmadapter->curr_cmd = pcmd_node;
    wlan_release_cmd_lock(pmadapter);

    cmd_code = wlan_le16_to_cpu(pcmd->command);
    cmd_size = wlan_le16_to_cpu(pcmd->size);

    pcmd_node->mbuf.data_len = cmd_size;
    /* Add extra header for USB */
    if (pcmd_node->mbuf.data_offset < MLAN_USB_TYPE_LEN) {
        PRINTM(ERROR, "DNLD_CMD: data_offset is too small=%d",
               pcmd_node->mbuf.data_offset);
        if (pioctl_buf != MNULL)
            pioctl_buf->status_code = MLAN_ERROR_CMD_DNLD_FAIL,
                wlan_insert_cmd_to_free_q(pmadapter, pcmd_node);
        wlan_request_cmd_lock(pmadapter);
        pmadapter->curr_cmd = MNULL;
        wlan_release_cmd_lock(pmadapter);
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }
    tmp = wlan_cpu_to_le32(MLAN_USB_TYPE_CMD);
    memcpy((t_u8 *) pcmd - MLAN_USB_TYPE_LEN, (t_u8 *) & tmp,
           MLAN_USB_TYPE_LEN);
    pcmd_node->mbuf.data_offset -= MLAN_USB_TYPE_LEN;
    pcmd_node->mbuf.data_len += MLAN_USB_TYPE_LEN;

    pmadapter->callbacks.moal_get_system_time(&sec, &usec);
    PRINTM(CMND, "DNLD_CMD (%lu.%lu): 0x%x, act 0x%x, len %d, seqno %d\n",
           sec, usec, cmd_code,
           wlan_le16_to_cpu(*(t_u16 *) ((t_u8 *) pcmd + S_DS_GEN)), cmd_size,
           wlan_le16_to_cpu(pcmd->seq_num));
    DBG_HEXDUMP(CMD_D, "DNLD_CMD", (t_u8 *) pcmd, cmd_size);

    /* Send the command to lower layer */
    pmadapter->cmd_sent = MTRUE;
    ret = pcb->moal_write_data_async(pmadapter->pmoal_handle,
                                     &pcmd_node->mbuf, MLAN_USB_EP_CMD_EVENT);

    if (ret == MLAN_STATUS_FAILURE) {
        PRINTM(ERROR, "DNLD_CMD: Host to Card Failed\n");
        pmadapter->cmd_sent = MFALSE;
        if (pioctl_buf != MNULL)
            pioctl_buf->status_code = MLAN_ERROR_CMD_DNLD_FAIL,
                wlan_insert_cmd_to_free_q(pmadapter, pmadapter->curr_cmd);

        wlan_request_cmd_lock(pmadapter);
        pmadapter->curr_cmd = MNULL;
        wlan_release_cmd_lock(pmadapter);

        pmadapter->dbg.num_cmd_host_to_card_failure++;
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    /* Save the last command id and action to debug log */
    pmadapter->dbg.last_cmd_index =
        (pmadapter->dbg.last_cmd_index + 1) % DBG_CMD_NUM;
    pmadapter->dbg.last_cmd_id[pmadapter->dbg.last_cmd_index] = cmd_code;
    pmadapter->dbg.last_cmd_act[pmadapter->dbg.last_cmd_index] =
        wlan_le16_to_cpu(*(t_u16 *) ((t_u8 *) pcmd + S_DS_GEN));

    /* Setup the timer after transmit command */
    if (cmd_code == HostCmd_CMD_802_11_SCAN ||
        cmd_code == HostCmd_CMD_802_11_DEAUTHENTICATE ||
        cmd_code == HostCmd_CMD_802_11_ASSOCIATE)
        pcb->moal_start_timer(pmadapter->pmlan_cmd_timer,
                              MFALSE, MRVDRV_TIMER_10S);
    else
        pcb->moal_start_timer(pmadapter->pmlan_cmd_timer,
                              MFALSE, MRVDRV_TIMER_5S);

    pmadapter->cmd_timer_is_set = MTRUE;

    ret = MLAN_STATUS_SUCCESS;

  done:
    LEAVE();
    return ret;
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
wlan_cmd_get_hw_spec(IN pmlan_private pmpriv, IN HostCmd_DS_COMMAND * pcmd)
{
    HostCmd_DS_GET_HW_SPEC *hw_spec = &pcmd->params.hw_spec;

    ENTER();

    pcmd->command = wlan_cpu_to_le16(HostCmd_CMD_GET_HW_SPEC);
    pcmd->size = wlan_cpu_to_le16(sizeof(HostCmd_DS_GET_HW_SPEC) + S_DS_GEN);
    memcpy(hw_spec->permanent_addr, pmpriv->curr_addr, MLAN_MAC_ADDR_LENGTH);

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function prepares command of RSSI info.
 *  
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param pcmd         A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   Command action
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
static mlan_status
wlan_cmd_802_11_rssi_info(IN pmlan_private pmpriv,
                          IN HostCmd_DS_COMMAND * pcmd, IN t_u16 cmd_action)
{
    ENTER();

    pcmd->command = wlan_cpu_to_le16(HostCmd_CMD_RSSI_INFO);
    pcmd->size =
        wlan_cpu_to_le16(sizeof(HostCmd_DS_802_11_RSSI_INFO) + S_DS_GEN);
    pcmd->params.rssi_info.action = wlan_cpu_to_le16(cmd_action);
    pcmd->params.rssi_info.ndata = wlan_cpu_to_le16(pmpriv->data_avg_factor);
    pcmd->params.rssi_info.nbcn = wlan_cpu_to_le16(pmpriv->bcn_avg_factor);

    /* Reset SNR/NF/RSSI values in private structure */
    pmpriv->data_rssi_last = 0;
    pmpriv->data_nf_last = 0;
    pmpriv->data_rssi_avg = 0;
    pmpriv->data_nf_avg = 0;
    pmpriv->bcn_rssi_last = 0;
    pmpriv->bcn_nf_last = 0;
    pmpriv->bcn_rssi_avg = 0;
    pmpriv->bcn_nf_avg = 0;

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function prepares command of mac_control.
 *  
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param pcmd         A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   Command action
 *  @param pdata_buf    A pointer to command information buffer
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
static mlan_status
wlan_cmd_mac_control(IN pmlan_private pmpriv,
                     IN HostCmd_DS_COMMAND * pcmd,
                     IN t_u16 cmd_action, IN t_void * pdata_buf)
{
    HostCmd_DS_MAC_CONTROL *pmac = &pcmd->params.mac_ctrl;
    t_u16 action = *((t_u16 *) pdata_buf);

    ENTER();

    if (cmd_action != HostCmd_ACT_GEN_SET) {
        PRINTM(ERROR, "wlan_cmd_mac_control(): support SET only.\n");
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }

    pcmd->command = wlan_cpu_to_le16(HostCmd_CMD_MAC_CONTROL);
    pcmd->size = wlan_cpu_to_le16(sizeof(HostCmd_DS_MAC_CONTROL) + S_DS_GEN);
    pmac->action = wlan_cpu_to_le16(action);

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function prepares command of snmp_mib.
 *  
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   The action: GET or SET
 *  @param cmd_oid      OID: ENABLE or DISABLE
 *  @param pdata_buf    A pointer to command information buffer
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
static mlan_status
wlan_cmd_802_11_snmp_mib(IN pmlan_private pmpriv,
                         IN HostCmd_DS_COMMAND * cmd,
                         IN t_u16 cmd_action,
                         IN t_u16 cmd_oid, IN t_void * pdata_buf)
{
    HostCmd_DS_802_11_SNMP_MIB *psnmp_mib = &cmd->params.smib;
    t_u32 ul_temp;

    ENTER();
    PRINTM(CMND, "SNMP_CMD: cmd_oid = 0x%x\n", cmd_oid);
    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_802_11_SNMP_MIB);
    cmd->size = wlan_cpu_to_le16(sizeof(HostCmd_DS_802_11_SNMP_MIB) + S_DS_GEN);
    switch (cmd_oid) {
    case FragThresh_i:
        psnmp_mib->oid = wlan_cpu_to_le16((t_u16) FragThresh_i);
        if (cmd_action == HostCmd_ACT_GEN_GET) {
            psnmp_mib->query_type = wlan_cpu_to_le16(HostCmd_ACT_GEN_GET);
        } else if (cmd_action == HostCmd_ACT_GEN_SET) {
            psnmp_mib->query_type = wlan_cpu_to_le16(HostCmd_ACT_GEN_SET);
            psnmp_mib->buf_size = wlan_cpu_to_le16(sizeof(t_u16));
            ul_temp = *((t_u32 *) pdata_buf);
            *((t_u16 *) (psnmp_mib->value)) = wlan_cpu_to_le16((t_u16) ul_temp);
        }
        break;
    case RtsThresh_i:
        psnmp_mib->oid = wlan_cpu_to_le16((t_u16) RtsThresh_i);
        if (cmd_action == HostCmd_ACT_GEN_GET) {
            psnmp_mib->query_type = wlan_cpu_to_le16(HostCmd_ACT_GEN_GET);
        } else if (cmd_action == HostCmd_ACT_GEN_SET) {
            psnmp_mib->query_type = wlan_cpu_to_le16(HostCmd_ACT_GEN_SET);
            psnmp_mib->buf_size = wlan_cpu_to_le16(sizeof(t_u16));
            ul_temp = *((t_u32 *) pdata_buf);
            *(t_u16 *) (psnmp_mib->value) = wlan_cpu_to_le16((t_u16) ul_temp);
        }
        break;

    case ShortRetryLim_i:
        psnmp_mib->oid = wlan_cpu_to_le16((t_u16) ShortRetryLim_i);
        if (cmd_action == HostCmd_ACT_GEN_GET) {
            psnmp_mib->query_type = wlan_cpu_to_le16(HostCmd_ACT_GEN_GET);
        } else if (cmd_action == HostCmd_ACT_GEN_SET) {
            psnmp_mib->query_type = wlan_cpu_to_le16(HostCmd_ACT_GEN_SET);
            psnmp_mib->buf_size = wlan_cpu_to_le16(sizeof(t_u16));
            ul_temp = (*(t_u32 *) pdata_buf);
            *((t_u16 *) (psnmp_mib->value)) = wlan_cpu_to_le16((t_u16) ul_temp);
        }
        break;
    case Dot11D_i:
        psnmp_mib->oid = wlan_cpu_to_le16((t_u16) Dot11D_i);
        if (cmd_action == HostCmd_ACT_GEN_SET) {
            psnmp_mib->query_type = wlan_cpu_to_le16(HostCmd_ACT_GEN_SET);
            psnmp_mib->buf_size = wlan_cpu_to_le16(sizeof(t_u16));
            ul_temp = *(t_u32 *) pdata_buf;
            *((t_u16 *) (psnmp_mib->value)) = wlan_cpu_to_le16((t_u16) ul_temp);
        }
        break;
    case WwsMode_i:
        psnmp_mib->oid = wlan_cpu_to_le16((t_u16) WwsMode_i);
        if (cmd_action == HostCmd_ACT_GEN_GET) {
            psnmp_mib->query_type = wlan_cpu_to_le16(HostCmd_ACT_GEN_GET);
        } else if (cmd_action == HostCmd_ACT_GEN_SET) {
            psnmp_mib->query_type = wlan_cpu_to_le16(HostCmd_ACT_GEN_SET);
            psnmp_mib->buf_size = wlan_cpu_to_le16(sizeof(t_u16));
            ul_temp = *((t_u32 *) pdata_buf);
            *((t_u16 *) (psnmp_mib->value)) = wlan_cpu_to_le16((t_u16) ul_temp);
        }
        break;

    default:
        break;
    }
    PRINTM(CMND, "SNMP_CMD: Action=0x%x, OID=0x%x, OIDSize=0x%x, Value=0x%x\n",
           cmd_action, cmd_oid, wlan_le16_to_cpu(psnmp_mib->buf_size),
           wlan_le16_to_cpu(*(t_u16 *) psnmp_mib->value));
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function prepares command of get_log.
 *  
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
static mlan_status
wlan_cmd_802_11_get_log(IN pmlan_private pmpriv, IN HostCmd_DS_COMMAND * cmd)
{
    ENTER();
    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_802_11_GET_LOG);
    cmd->size = wlan_cpu_to_le16(sizeof(HostCmd_DS_802_11_GET_LOG) + S_DS_GEN);
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function prepares command of radio_control.
 *  
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   The action: GET or SET
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
static mlan_status
wlan_cmd_802_11_radio_control(IN pmlan_private pmpriv,
                              IN HostCmd_DS_COMMAND * cmd, IN t_u16 cmd_action)
{
    mlan_adapter *pmadapter = pmpriv->adapter;
    HostCmd_DS_802_11_RADIO_CONTROL *pradio_control = &cmd->params.radio;

    ENTER();
    cmd->size = wlan_cpu_to_le16((sizeof(HostCmd_DS_802_11_RADIO_CONTROL))
                                 + S_DS_GEN);
    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_802_11_RADIO_CONTROL);
    pradio_control->action = wlan_cpu_to_le16(cmd_action);
    pradio_control->control = wlan_cpu_to_le16(pmadapter->radio_on);
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function prepares command of tx_rate_cfg.
 *  
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   The action: GET or SET
 *  @param pdata_buf    A pointer to data buffer
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
static mlan_status
wlan_cmd_tx_rate_cfg(IN pmlan_private pmpriv,
                     IN HostCmd_DS_COMMAND * cmd,
                     IN t_u16 cmd_action, IN t_void * pdata_buf)
{
    HostCmd_DS_TX_RATE_CFG *rate_cfg = &cmd->params.tx_rate_cfg;
    MrvlRateScope_t *rate_scope;
    MrvlRateDropPattern_t *rate_drop;
    t_u16 *pbitmap_rates = (t_u16 *) pdata_buf;

    int i;

    ENTER();

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_TX_RATE_CFG);

    rate_cfg->action = wlan_cpu_to_le16(cmd_action);
    rate_cfg->cfg_index = 0;

    rate_scope = (MrvlRateScope_t *) ((t_u8 *) rate_cfg +
                                      sizeof(HostCmd_DS_TX_RATE_CFG));
    rate_scope->type = wlan_cpu_to_le16(TLV_TYPE_RATE_SCOPE);
    rate_scope->length = wlan_cpu_to_le16(sizeof(MrvlRateScope_t) -
                                          sizeof(MrvlIEtypesHeader_t));
    if (pbitmap_rates != MNULL) {
        rate_scope->hr_dsss_rate_bitmap = wlan_cpu_to_le16(pbitmap_rates[0]);
        rate_scope->ofdm_rate_bitmap = wlan_cpu_to_le16(pbitmap_rates[1]);
        for (i = 0; i < sizeof(rate_scope->ht_mcs_rate_bitmap) / sizeof(t_u16);
             i++)
            rate_scope->ht_mcs_rate_bitmap[i] =
                wlan_cpu_to_le16(pbitmap_rates[2 + i]);
    } else {
        rate_scope->hr_dsss_rate_bitmap =
            wlan_cpu_to_le16(pmpriv->bitmap_rates[0]);
        rate_scope->ofdm_rate_bitmap =
            wlan_cpu_to_le16(pmpriv->bitmap_rates[1]);
        for (i = 0; i < sizeof(rate_scope->ht_mcs_rate_bitmap) / sizeof(t_u16);
             i++)
            rate_scope->ht_mcs_rate_bitmap[i] =
                wlan_cpu_to_le16(pmpriv->bitmap_rates[2 + i]);
    }

    rate_drop = (MrvlRateDropPattern_t *) ((t_u8 *) rate_scope +
                                           sizeof(MrvlRateScope_t));
    rate_drop->type = wlan_cpu_to_le16(TLV_TYPE_RATE_DROP_CONTROL);
    rate_drop->length = wlan_cpu_to_le16(sizeof(rate_drop->rate_drop_mode));
    rate_drop->rate_drop_mode = 0;

    cmd->size = wlan_cpu_to_le16(S_DS_GEN + sizeof(HostCmd_DS_TX_RATE_CFG) +
                                 sizeof(MrvlRateScope_t) +
                                 sizeof(MrvlRateDropPattern_t));

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function prepares command of tx_power_cfg.
 *   
 *  @param pmpriv      A pointer to mlan_private structure
 *  @param cmd         A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action  The action: GET or SET
 *  @param pdata_buf   A pointer to data buffer
 *
 *  @return            MLAN_STATUS_SUCCESS
 */
static mlan_status
wlan_cmd_tx_power_cfg(IN pmlan_private pmpriv,
                      IN HostCmd_DS_COMMAND * cmd,
                      IN t_u16 cmd_action, IN t_void * pdata_buf)
{
    MrvlTypes_Power_Group_t *ppg_tlv = MNULL;
    HostCmd_DS_TXPWR_CFG *ptxp = MNULL;
    HostCmd_DS_TXPWR_CFG *ptxp_cfg = &cmd->params.txp_cfg;

    ENTER();

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_TXPWR_CFG);
    cmd->size = wlan_cpu_to_le16(S_DS_GEN + sizeof(HostCmd_DS_TXPWR_CFG));
    switch (cmd_action) {
    case HostCmd_ACT_GEN_SET:
        ptxp = (HostCmd_DS_TXPWR_CFG *) pdata_buf;
        if (ptxp->mode) {
            ppg_tlv =
                (MrvlTypes_Power_Group_t *) (pdata_buf +
                                             sizeof(HostCmd_DS_TXPWR_CFG));
            memmove(ptxp_cfg, pdata_buf,
                    sizeof(HostCmd_DS_TXPWR_CFG) +
                    sizeof(MrvlTypes_Power_Group_t) + ppg_tlv->length);

            ppg_tlv = (MrvlTypes_Power_Group_t *) ((t_u8 *) ptxp_cfg +
                                                   sizeof
                                                   (HostCmd_DS_TXPWR_CFG));
            cmd->size +=
                wlan_cpu_to_le16(sizeof(MrvlTypes_Power_Group_t) +
                                 ppg_tlv->length);
            ppg_tlv->type = wlan_cpu_to_le16(ppg_tlv->type);
            ppg_tlv->length = wlan_cpu_to_le16(ppg_tlv->length);
        } else {
            memmove(ptxp_cfg, pdata_buf, sizeof(HostCmd_DS_TXPWR_CFG));
        }
        ptxp_cfg->action = wlan_cpu_to_le16(cmd_action);
        ptxp_cfg->cfg_index = wlan_cpu_to_le16(ptxp_cfg->cfg_index);
        ptxp_cfg->mode = wlan_cpu_to_le32(ptxp_cfg->mode);
        break;
    case HostCmd_ACT_GEN_GET:
        ptxp_cfg->action = wlan_cpu_to_le16(cmd_action);
        break;
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function prepares command of Heart Beat.
 *  
 *  @param pmpriv      A pointer to mlan_private structure
 *  @param cmd         A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action  The action: GET or SET
 *  @param pdata_buf   A pointer to data buffer
 *
 *  @return            MLAN_STATUS_SUCCESS
 */
static mlan_status
wlan_cmd_heart_beat(IN pmlan_private pmpriv,
                    IN HostCmd_DS_COMMAND * cmd,
                    IN t_u16 cmd_action, IN t_void * pdata_buf)
{
    mlan_ds_misc_heart_beat *pdata_heart_beat =
        (mlan_ds_misc_heart_beat *) pdata_buf;
    HostCmd_DS_HEART_BEAT *cmd_heart_beat =
        (HostCmd_DS_HEART_BEAT *) & cmd->params.heart_beat;

    ENTER();

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_HEART_BEAT);
    cmd->size = wlan_cpu_to_le16(sizeof(HostCmd_DS_HEART_BEAT) + S_DS_GEN);
    cmd_heart_beat->action = wlan_cpu_to_le16(cmd_action);
    if (cmd_action == HostCmd_ACT_GEN_SET && pdata_heart_beat) {
        cmd_heart_beat->h2d_timer =
            wlan_cpu_to_le16(pdata_heart_beat->h2d_timer);
        cmd_heart_beat->d2h_timer =
            wlan_cpu_to_le16(pdata_heart_beat->d2h_timer);
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 * @brief This function prepares command of ps_mode.
 *   
 * @param pmpriv       A pointer to mlan_private structure
 * @param cmd          A pointer to HostCmd_DS_COMMAND structure
 * @param cmd_action   The action: GET or SET
 * @param pdata_buf    A pointer to data buffer
 *
 * @return             MLAN_STATUS_SUCCESS
 */
static mlan_status
wlan_cmd_802_11_ps_mode(IN pmlan_private pmpriv,
                        IN HostCmd_DS_COMMAND * cmd,
                        IN t_u16 cmd_action, IN t_void * pdata_buf)
{
    pmlan_adapter pmadapter = pmpriv->adapter;
    HostCmd_DS_802_11_PS_MODE *pps_mode = &cmd->params.ps_mode;

    ENTER();

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_802_11_PS_MODE);
    cmd->size = wlan_cpu_to_le16(S_DS_GEN + sizeof(HostCmd_DS_802_11_PS_MODE));
    pps_mode->action = wlan_cpu_to_le16(cmd_action);
    pps_mode->multiple_dtim = 0;

    switch (cmd_action) {
    case HostCmd_SubCmd_Enter_PS:
        PRINTM(CMND, "PS Command:" "SubCode- Enter PS\n");

        pps_mode->local_listen_interval =
            wlan_cpu_to_le16(pmadapter->local_listen_interval);
        pps_mode->null_pkt_interval =
            wlan_cpu_to_le16(pmadapter->null_pkt_interval);
        pps_mode->multiple_dtim = wlan_cpu_to_le16(pmadapter->multiple_dtim);
        pps_mode->bcn_miss_time_out =
            wlan_cpu_to_le16(pmadapter->bcn_miss_time_out);
        if (pmpriv->bss_mode == MLAN_BSS_MODE_IBSS)
            pps_mode->adhoc_awake_period =
                wlan_cpu_to_le16(pmadapter->adhoc_awake_period);
        break;
    case HostCmd_SubCmd_Exit_PS:
        PRINTM(CMND, "PS Command:" "SubCode- Exit PS\n");
        break;
    case HostCmd_SubCmd_Sleep_Confirmed:
        PRINTM(CMND, "PS Command: SubCode- sleep confirm\n");
        break;
    default:
        break;
    }

    PRINTM(CMND, "PS_MODE_CMD: Size:%d Cmd:0x%x Act:%d\n",
           cmd->size, cmd->command, pps_mode->action);
    LEAVE();
    return MLAN_STATUS_SUCCESS;
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
wlan_cmd_802_11_hs_cfg(IN pmlan_private pmpriv,
                       IN HostCmd_DS_COMMAND * cmd,
                       IN t_u16 cmd_action,
                       IN HostCmd_DS_802_11_HOST_SLEEP_CFG * pdata_buf)
{
    pmlan_adapter pmadapter = pmpriv->adapter;
    HostCmd_DS_802_11_HOST_SLEEP_CFG *phs_cfg = &cmd->params.hs_cfg;

    ENTER();

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_802_11_HOST_SLEEP_CFG);
    if ((pdata_buf->conditions != HOST_SLEEP_CFG_CANCEL)
        && ((pmadapter->arp_filter_size > 0)
            && (pmadapter->arp_filter_size <= ARP_FILTER_MAX_BUF_SIZE))) {
        PRINTM(CMND, "Attach %d bytes ArpFilter to HSCfg cmd\n",
               pmadapter->arp_filter_size);
        memcpy(((t_u8 *) phs_cfg) + sizeof(HostCmd_DS_802_11_HOST_SLEEP_CFG),
               pmadapter->arp_filter, pmadapter->arp_filter_size);
        cmd->size =
            wlan_cpu_to_le16(pmadapter->arp_filter_size +
                             sizeof(HostCmd_DS_802_11_HOST_SLEEP_CFG) +
                             S_DS_GEN);
    } else
        cmd->size =
            wlan_cpu_to_le16(S_DS_GEN +
                             sizeof(HostCmd_DS_802_11_HOST_SLEEP_CFG));
    phs_cfg->conditions = wlan_cpu_to_le32(pdata_buf->conditions);
    phs_cfg->gpio = pdata_buf->gpio;
    phs_cfg->gap = pdata_buf->gap;

    PRINTM(CMND, "HS_CFG_CMD: condition:0x%x gpio:0x%x gap:0x%x\n",
           pdata_buf->conditions, pdata_buf->gpio, pdata_buf->gap);

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function prepares command of mac_address.
 *  
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   The action: GET or SET
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
static mlan_status
wlan_cmd_802_11_mac_address(IN pmlan_private pmpriv,
                            IN HostCmd_DS_COMMAND * cmd, IN t_u16 cmd_action)
{
    ENTER();
    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_802_11_MAC_ADDRESS);
    cmd->size = wlan_cpu_to_le16(sizeof(HostCmd_DS_802_11_MAC_ADDRESS) +
                                 S_DS_GEN);
    cmd->result = 0;

    cmd->params.mac_addr.action = wlan_cpu_to_le16(cmd_action);

    if (cmd_action == HostCmd_ACT_GEN_SET) {
        memcpy(cmd->params.mac_addr.mac_addr, pmpriv->curr_addr,
               MLAN_MAC_ADDR_LENGTH);
        // HEXDUMP("SET_CMD: MAC ADDRESS-", priv->CurrentAddr, 6);
    }
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function prepares command of mac_multicast_adr.
 *  
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   The action: GET or SET
 *  @param pdata_buf    A pointer to data buffer
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
static mlan_status
wlan_cmd_mac_multicast_adr(IN pmlan_private pmpriv,
                           IN HostCmd_DS_COMMAND * cmd,
                           IN t_u16 cmd_action, IN t_void * pdata_buf)
{
    mlan_multicast_list *pmcast_list = (mlan_multicast_list *) pdata_buf;
    HostCmd_DS_MAC_MULTICAST_ADR *pmc_addr = &cmd->params.mc_addr;

    ENTER();
    cmd->size =
        wlan_cpu_to_le16(sizeof(HostCmd_DS_MAC_MULTICAST_ADR) + S_DS_GEN);
    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_MAC_MULTICAST_ADR);

    pmc_addr->action = wlan_cpu_to_le16(cmd_action);
    pmc_addr->num_of_adrs =
        wlan_cpu_to_le16((t_u16) pmcast_list->num_multicast_addr);
    memcpy(pmc_addr->mac_list, pmcast_list->mac_list,
           pmcast_list->num_multicast_addr * MLAN_MAC_ADDR_LENGTH);

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function prepares command of deauthenticate.
 *
 * @param pmpriv       A pointer to mlan_private structure
 * @param cmd          A pointer to HostCmd_DS_COMMAND structure
 * @param pdata_buf    A pointer to data buffer
 *
 * @return             MLAN_STATUS_SUCCESS
 */
mlan_status
wlan_cmd_802_11_deauthenticate(IN pmlan_private pmpriv,
                               IN HostCmd_DS_COMMAND * cmd,
                               IN t_void * pdata_buf)
{
    HostCmd_DS_802_11_DEAUTHENTICATE *pdeauth = &cmd->params.deauth;

    ENTER();

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_802_11_DEAUTHENTICATE);
    cmd->size =
        wlan_cpu_to_le16(sizeof(HostCmd_DS_802_11_DEAUTHENTICATE) + S_DS_GEN);

    /* Set AP MAC address */
    memcpy(pdeauth->mac_addr, (t_u8 *) pdata_buf, MLAN_MAC_ADDR_LENGTH);

    PRINTM(CMND, "Deauth: %02x:%02x:%02x:%02x:%02x:%02x\n",
           pdeauth->mac_addr[0], pdeauth->mac_addr[1], pdeauth->mac_addr[2],
           pdeauth->mac_addr[3], pdeauth->mac_addr[4], pdeauth->mac_addr[5]);

/** Reason code 3 = Station is leaving */
#define REASON_CODE_STA_LEAVING 3
    pdeauth->reason_code = wlan_cpu_to_le16(REASON_CODE_STA_LEAVING);

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function prepares command of ad_hoc_stop.
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status
wlan_cmd_802_11_ad_hoc_stop(IN pmlan_private pmpriv,
                            IN HostCmd_DS_COMMAND * cmd)
{
    ENTER();

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_802_11_AD_HOC_STOP);
    cmd->size = wlan_cpu_to_le16(sizeof(HostCmd_DS_802_11_AD_HOC_STOP)
                                 + S_DS_GEN);

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** Length of WEP 40 bit key */
#define WEP_40_BIT_LEN  5
/** Length of WEP 104 bit key */
#define WEP_104_BIT_LEN 13

/** 
 *  @brief This function sets WEP key(s) to key_param_set TLV(s).
 *  
 *  @param priv           	A pointer to mlan_private structure
 *  @param key_param_set    A pointer to MrvlIEtype_KeyParamSet_t structure
 *  @param key_param_len    A pointer to the length variable
 *
 *  @return                 MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status
wlan_set_keyparamset_wep(mlan_private * priv,
                         MrvlIEtype_KeyParamSet_t * key_param_set,
                         t_u16 * key_param_len)
{
    int i, cur_key_param_len = 0;
    mlan_status ret = MLAN_STATUS_SUCCESS;

    ENTER();

    /* Multi-key_param_set TLV is supported */
    for (i = 0; i < MRVL_NUM_WEP_KEY; i++) {
        if ((priv->wep_key[i].key_length == WEP_40_BIT_LEN) ||
            (priv->wep_key[i].key_length == WEP_104_BIT_LEN)) {
            key_param_set->type = wlan_cpu_to_le16(TLV_TYPE_KEY_MATERIAL);
/** Key_param_set WEP fixed length */
#define KEYPARAMSET_WEP_FIXED_LEN 8
            key_param_set->length =
                wlan_cpu_to_le16(priv->wep_key[i].key_length +
                                 KEYPARAMSET_WEP_FIXED_LEN);
            key_param_set->key_type_id = wlan_cpu_to_le16(KEY_TYPE_ID_WEP);
            key_param_set->key_info = wlan_cpu_to_le16
                (KEY_INFO_WEP_ENABLED | KEY_INFO_WEP_UNICAST |
                 KEY_INFO_WEP_MCAST);
            key_param_set->key_len =
                wlan_cpu_to_le16(priv->wep_key[i].key_length);
            /* Set WEP key index */
            key_param_set->key[0] = i;
            /* Set default Tx key flag */
            if (i == (priv->wep_key_curr_index & HostCmd_WEP_KEY_INDEX_MASK))
                key_param_set->key[1] = 1;
            else
                key_param_set->key[1] = 0;
            memmove(&key_param_set->key[2], priv->wep_key[i].key_material,
                    priv->wep_key[i].key_length);

            cur_key_param_len = priv->wep_key[i].key_length +
                KEYPARAMSET_WEP_FIXED_LEN + sizeof(MrvlIEtypesHeader_t);
            *key_param_len += cur_key_param_len;
            key_param_set =
                (MrvlIEtype_KeyParamSet_t *) ((t_u8 *) key_param_set +
                                              cur_key_param_len);
        } else if (!priv->wep_key[i].key_length) {
            continue;
        } else {
            PRINTM(ERROR, "key%d Length = %d is incorrect\n", (i + 1),
                   priv->wep_key[i].key_length);
            ret = MLAN_STATUS_FAILURE;
            goto done;
        }
    }

  done:
    LEAVE();
    return ret;
}

/** 
 *  @brief This function prepares command of key_material.
 *  
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   The action: GET or SET
 *  @param cmd_oid      OID: ENABLE or DISABLE
 *  @param pdata_buf    A pointer to data buffer
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status
wlan_cmd_802_11_key_material(IN pmlan_private pmpriv,
                             IN HostCmd_DS_COMMAND * cmd,
                             IN t_u16 cmd_action,
                             IN t_u16 cmd_oid, IN t_void * pdata_buf)
{
    HostCmd_DS_802_11_KEY_MATERIAL *pkey_material = &cmd->params.key_material;
    mlan_ds_encrypt_key *pkey = (mlan_ds_encrypt_key *) pdata_buf;
    t_u16 key_param_len = 0;
    mlan_status ret = MLAN_STATUS_SUCCESS;

    ENTER();

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_802_11_KEY_MATERIAL);
    pkey_material->action = wlan_cpu_to_le16(cmd_action);

    if (cmd_action == HostCmd_ACT_GEN_GET) {
        cmd->size = wlan_cpu_to_le16(sizeof(pkey_material->action) + S_DS_GEN);
        goto done;
    }
    if (!pkey) {
        memset(&pkey_material->key_param_set, 0,
               (MRVL_NUM_WEP_KEY * sizeof(MrvlIEtype_KeyParamSet_t)));
        ret =
            wlan_set_keyparamset_wep(pmpriv, &pkey_material->key_param_set,
                                     &key_param_len);
        cmd->size =
            wlan_cpu_to_le16(key_param_len + sizeof(pkey_material->action) +
                             S_DS_GEN);
        goto done;
    } else
        memset(&pkey_material->key_param_set, 0,
               sizeof(MrvlIEtype_KeyParamSet_t));
    if (pkey->key_len == WPA_AES_KEY_LEN) {
        PRINTM(CMND, "WPA_AES\n");
        pkey_material->key_param_set.key_type_id =
            wlan_cpu_to_le16(KEY_TYPE_ID_AES);
        if (cmd_oid == KEY_INFO_ENABLED)
            pkey_material->key_param_set.key_info =
                wlan_cpu_to_le16(KEY_INFO_AES_ENABLED);
        else
            pkey_material->key_param_set.key_info =
                !(wlan_cpu_to_le16(KEY_INFO_AES_ENABLED));

        if (pkey->key_index & 0x40000000)       /* AES pairwise key: unicast */
            pkey_material->key_param_set.key_info |=
                wlan_cpu_to_le16(KEY_INFO_AES_UNICAST);
        else                    /* AES group key: multicast */
            pkey_material->key_param_set.key_info |=
                wlan_cpu_to_le16(KEY_INFO_AES_MCAST);
    } else if (pkey->key_len == WPA_TKIP_KEY_LEN) {
        PRINTM(CMND, "WPA_TKIP\n");
        pkey_material->key_param_set.key_type_id =
            wlan_cpu_to_le16(KEY_TYPE_ID_TKIP);
        pkey_material->key_param_set.key_info =
            wlan_cpu_to_le16(KEY_INFO_TKIP_ENABLED);

        if (pkey->key_index & 0x40000000)       /* TKIP pairwise key: unicast */
            pkey_material->key_param_set.key_info |=
                wlan_cpu_to_le16(KEY_INFO_TKIP_UNICAST);
        else                    /* TKIP group key: multicast */
            pkey_material->key_param_set.key_info |=
                wlan_cpu_to_le16(KEY_INFO_TKIP_MCAST);
    }

    if (pkey_material->key_param_set.key_type_id) {
        pkey_material->key_param_set.type =
            wlan_cpu_to_le16(TLV_TYPE_KEY_MATERIAL);
        pkey_material->key_param_set.key_len = wlan_cpu_to_le16(pkey->key_len);
        memcpy(pkey_material->key_param_set.key, pkey->key_material,
               pkey->key_len);
/** Key_param_set fixed length */
#define KEYPARAMSET_FIXED_LEN 6
        pkey_material->key_param_set.length =
            wlan_cpu_to_le16((t_u16) pkey->key_len + KEYPARAMSET_FIXED_LEN);

        key_param_len =
            (pkey->key_len + KEYPARAMSET_FIXED_LEN) +
            sizeof(MrvlIEtypesHeader_t);
        cmd->size =
            wlan_cpu_to_le16(key_param_len + sizeof(pkey_material->action) +
                             S_DS_GEN);
    }
  done:
    LEAVE();
    return ret;
}

/** 
 *  @brief This function prepares command of supplicant pmk
 *  
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   The action: GET or SET
 *  @param pdata_buf    A pointer to data buffer
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
static mlan_status
wlan_cmd_802_11_supplicant_pmk(IN pmlan_private pmpriv,
                               IN HostCmd_DS_COMMAND * cmd,
                               IN t_u16 cmd_action, IN t_void * pdata_buf)
{
    MrvlIEtypes_PMK_t *ppmk_tlv = MNULL;
    MrvlIEtypes_Passphrase_t *ppassphrase_tlv = MNULL;
    MrvlIEtypes_SsIdParamSet_t *pssid_tlv = MNULL;
    MrvlIEtypes_Bssid_t *pbssid_tlv = MNULL;
    HostCmd_DS_802_11_SUPPLICANT_PMK *pesupplicant_psk =
        &cmd->params.esupplicant_psk;
    t_u8 *ptlv_buffer = (t_u8 *) pesupplicant_psk->tlv_buffer;
    mlan_ds_passphrase *psk = (mlan_ds_passphrase *) pdata_buf;
    t_u8 zero_mac[] = { 0, 0, 0, 0, 0, 0 };

    ENTER();
    /* 
     * Parse the rest of the buf here
     *      1)  <ssid="valid ssid"> - This will get the passphrase, AKMP 
     *          for specified ssid, if none specified then it will get all.
     *          Eg: iwpriv <mlanX> passphrase 0:ssid=marvell
     *      2)  <psk="psk">:<passphrase="passphare">:<bssid="00:50:43:ef:23:f3">
     *          <ssid="valid ssid"> - passphrase and psk cannot be provided to  
     *          the same SSID, Takes one SSID at a time, If ssid= is present 
     *          the it should contain a passphrase or psk. If no arguments are
     *          provided then AKMP=802.1x, and passphrase should be provided
     *          after association.
     *          End of each parameter should be followed by a ':'(except for the
     *          last parameter) as the delimiter. If ':' has to be used in 
     *          an SSID then a '/' should be preceded to ':' as a escape.
     *          Eg:iwpriv <mlanX> passphrase 
     *                    "1:ssid=mrvl AP:psk=abcdefgh:bssid=00:50:43:ef:23:f3"
     *          iwpriv <mlanX> passphrase 
     *                 "1:ssid=mrvl/: AP:psk=abcdefgd:bssid=00:50:43:ef:23:f3"
     *          iwpriv <mlanX> passphrase "1:ssid=mrvlAP:psk=abcdefgd"
     *      3)  <ssid="valid ssid"> - This will clear the passphrase 
     *          for specified ssid, if none specified then it will clear all.
     *          Eg: iwpriv <mlanX> passphrase 2:ssid=marvell
     */

    /* -1 is for t_u8 TlvBuffer[1] as this should not be included */
    cmd->size = sizeof(HostCmd_DS_802_11_SUPPLICANT_PMK) + S_DS_GEN - 1;
    if (psk->ssid.ssid_len) {
        pssid_tlv = (MrvlIEtypes_SsIdParamSet_t *) ptlv_buffer;
        pssid_tlv->header.type = wlan_cpu_to_le16(TLV_TYPE_SSID);
        pssid_tlv->header.len = psk->ssid.ssid_len;
        memcpy((char *) pssid_tlv->ssid, psk->ssid.ssid, psk->ssid.ssid_len);
        ptlv_buffer += (pssid_tlv->header.len + sizeof(MrvlIEtypesHeader_t));
        cmd->size += (pssid_tlv->header.len + sizeof(MrvlIEtypesHeader_t));
        pssid_tlv->header.len = wlan_cpu_to_le16(pssid_tlv->header.len);
    }
    if (memcmp((t_u8 *) & psk->bssid, zero_mac, sizeof(zero_mac))) {
        pbssid_tlv = (MrvlIEtypes_Bssid_t *) ptlv_buffer;
        pbssid_tlv->header.type = wlan_cpu_to_le16(TLV_TYPE_BSSID);
        pbssid_tlv->header.len = MLAN_MAC_ADDR_LENGTH;
        memcpy(pbssid_tlv->bssid, (t_u8 *) & psk->bssid, MLAN_MAC_ADDR_LENGTH);
        ptlv_buffer += (pbssid_tlv->header.len + sizeof(MrvlIEtypesHeader_t));
        cmd->size += (pbssid_tlv->header.len + sizeof(MrvlIEtypesHeader_t));
        pbssid_tlv->header.len = wlan_cpu_to_le16(pbssid_tlv->header.len);
    }
    if (psk->psk_type == MLAN_PSK_PASSPHRASE) {
        ppassphrase_tlv = (MrvlIEtypes_Passphrase_t *) ptlv_buffer;
        ppassphrase_tlv->header.type = wlan_cpu_to_le16(TLV_TYPE_PASSPHRASE);
        ppassphrase_tlv->header.len = psk->psk.passphrase.passphrase_len;
        memcpy(ppassphrase_tlv->passphrase, psk->psk.passphrase.passphrase,
               psk->psk.passphrase.passphrase_len);
        ptlv_buffer +=
            (ppassphrase_tlv->header.len + sizeof(MrvlIEtypesHeader_t));
        cmd->size +=
            (ppassphrase_tlv->header.len + sizeof(MrvlIEtypesHeader_t));
        ppassphrase_tlv->header.len =
            wlan_cpu_to_le16(ppassphrase_tlv->header.len);
    }
    if (psk->psk_type == MLAN_PSK_PMK) {
        ppmk_tlv = (MrvlIEtypes_PMK_t *) ptlv_buffer;
        ppmk_tlv->header.type = wlan_cpu_to_le16(TLV_TYPE_PMK);
        ppmk_tlv->header.len = MLAN_MAX_PMK_LENGTH;
        memcpy(ppmk_tlv->pmk, psk->psk.pmk.pmk, MLAN_MAX_PMK_LENGTH);
        ptlv_buffer += (ppmk_tlv->header.len + sizeof(MrvlIEtypesHeader_t));
        cmd->size += (ppmk_tlv->header.len + sizeof(MrvlIEtypesHeader_t));
        ppmk_tlv->header.len = wlan_cpu_to_le16(ppmk_tlv->header.len);
    }
    if ((cmd_action == HostCmd_ACT_GEN_SET) &&
        ((pssid_tlv || pbssid_tlv) && (!ppmk_tlv && !ppassphrase_tlv))) {
        PRINTM(ERROR,
               "Invalid case,ssid/bssid present without pmk or passphrase\n");
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }
    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_SUPPLICANT_PMK);
    pesupplicant_psk->action = wlan_cpu_to_le16(cmd_action);
    pesupplicant_psk->cache_result = 0;
    cmd->size = wlan_cpu_to_le16(cmd->size);
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief Handle the supplicant profile command
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   The action: GET or SET
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status
wlan_cmd_802_11_supplicant_profile(IN pmlan_private pmpriv,
                                   IN HostCmd_DS_COMMAND * cmd,
                                   IN t_u16 cmd_action)
{
    HostCmd_DS_802_11_SUPPLICANT_PROFILE *sup_profile =
        &cmd->params.esupplicant_profile;

    ENTER();

    cmd->size =
        wlan_cpu_to_le16(sizeof(HostCmd_DS_802_11_SUPPLICANT_PROFILE) +
                         S_DS_GEN - 1);
    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_SUPPLICANT_PROFILE);
    sup_profile->action = wlan_cpu_to_le16(cmd_action);
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function prepares command of rf_channel.
 *  
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   The action: GET or SET
 *  @param pdata_buf    A pointer to data buffer
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
static mlan_status
wlan_cmd_802_11_rf_channel(IN pmlan_private pmpriv,
                           IN HostCmd_DS_COMMAND * cmd,
                           IN t_u16 cmd_action, IN t_void * pdata_buf)
{
    HostCmd_DS_802_11_RF_CHANNEL *prf_chan = &cmd->params.rf_channel;

    ENTER();

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_802_11_RF_CHANNEL);
    cmd->size = wlan_cpu_to_le16(sizeof(HostCmd_DS_802_11_RF_CHANNEL)
                                 + S_DS_GEN);

    if (cmd_action == HostCmd_ACT_GEN_SET) {
        SET_SECONDARYCHAN(prf_chan->rf_type, pmpriv->adapter->chan_offset);
        prf_chan->rf_type = wlan_cpu_to_le16(prf_chan->rf_type);
        prf_chan->current_channel = wlan_cpu_to_le16(*((t_u16 *) pdata_buf));
    }
    prf_chan->action = wlan_cpu_to_le16(cmd_action);
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function prepares command of rf_antenna.
 *  
 *  @param pmpriv   A pointer to mlan_private structure
 *  @param cmd      A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   The action: GET or SET
 *  @param pdata_buf    A pointer to data buffer
 *
 *  @return         MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status
wlan_cmd_802_11_rf_antenna(IN pmlan_private pmpriv,
                           IN HostCmd_DS_COMMAND * cmd,
                           IN t_u16 cmd_action, IN t_void * pdata_buf)
{
    HostCmd_DS_802_11_RF_ANTENNA *pantenna = &cmd->params.antenna;

    ENTER();
    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_802_11_RF_ANTENNA);
    cmd->size =
        wlan_cpu_to_le16(sizeof(HostCmd_DS_802_11_RF_ANTENNA) + S_DS_GEN);

    if (cmd_action == HostCmd_ACT_GEN_SET) {
        pantenna->action = wlan_cpu_to_le16(HostCmd_ACT_SET_BOTH);
        pantenna->antenna_mode = wlan_cpu_to_le16(*(t_u16 *) pdata_buf);
    } else
        pantenna->action = wlan_cpu_to_le16(HostCmd_ACT_GET_BOTH);
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function prepares command of ibss_coalescing_status.
 *  
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   The action: GET or SET
 *  @param pdata_buf    A pointer to data buffer or MNULL
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status
wlan_cmd_ibss_coalescing_status(IN pmlan_private pmpriv,
                                IN HostCmd_DS_COMMAND * cmd,
                                IN t_u16 cmd_action, IN t_void * pdata_buf)
{
    HostCmd_DS_802_11_IBSS_STATUS *pibss_coal = &(cmd->params.ibss_coalescing);
    t_u16 enable = 0;

    ENTER();

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_802_11_IBSS_COALESCING_STATUS);
    cmd->size =
        wlan_cpu_to_le16(sizeof(HostCmd_DS_802_11_IBSS_STATUS) + S_DS_GEN);
    cmd->result = 0;
    pibss_coal->action = wlan_cpu_to_le16(cmd_action);

    switch (cmd_action) {
    case HostCmd_ACT_GEN_SET:
        if (pdata_buf != MNULL)
            enable = *(t_u16 *) pdata_buf;
        pibss_coal->enable = wlan_cpu_to_le16(enable);
        break;

        /* In other case.. Nothing to do */
    case HostCmd_ACT_GEN_GET:
    default:
        break;
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

#ifdef MFG_CMD_SUPPORT
/** 
 *  @brief This function sends general command to firmware.
 *  
 *  @param pmpriv     	A pointer to mlan_private structure
 *  @param cmd      	A pointer to HostCmd_DS_COMMAND structure
 *  @param pdata_buf	A pointer to data buffer
 *  @return         	MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status
wlan_cmd_mfg_cmd(IN pmlan_private pmpriv,
                 IN HostCmd_DS_COMMAND * cmd, IN t_void * pdata_buf)
{
    mlan_ds_misc_cmd *pcmd_ptr = (mlan_ds_misc_cmd *) pdata_buf;

    ENTER();

    /* Copy the MFG command to command buffer */
    memcpy((void *) cmd, pcmd_ptr->cmd, pcmd_ptr->len);
    PRINTM(CMND, "MFG command size = %d\n", pcmd_ptr->len);
    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_MFG_COMMAND);
    cmd->size = wlan_cpu_to_le16(cmd->size);
    cmd->result = 0;

    LEAVE();

    return MLAN_STATUS_SUCCESS;
}
#endif

/** 
 *  @brief This function sends host command to firmware.
 *  
 *  @param pmpriv     	A pointer to mlan_private structure
 *  @param cmd      	A pointer to HostCmd_DS_COMMAND structure
 *  @param pdata_buf	A pointer to data buffer
 *  @return         	MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status
wlan_cmd_host_cmd(IN pmlan_private pmpriv,
                  IN HostCmd_DS_COMMAND * cmd, IN t_void * pdata_buf)
{
    mlan_ds_misc_cmd *pcmd_ptr = (mlan_ds_misc_cmd *) pdata_buf;

    ENTER();

    /* Copy the HOST command to command buffer */
    memcpy((void *) cmd, pcmd_ptr->cmd, pcmd_ptr->len);
    PRINTM(CMND, "Host command size = %d\n", pcmd_ptr->len);
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function prepares system clock cfg command
 *  
 *  @param pmpriv    	A pointer to mlan_private structure
 *  @param cmd	   	A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action 	The action: GET or SET
 *  @param pdata_buf 	A pointer to data buffer
 *  @return 	   	MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status
wlan_cmd_sysclock_cfg(IN pmlan_private pmpriv,
                      IN HostCmd_DS_COMMAND * cmd,
                      IN t_u16 cmd_action, IN t_void * pdata_buf)
{
    HostCmd_DS_ECL_SYSTEM_CLOCK_CONFIG *cfg = &cmd->params.sys_clock_cfg;
    mlan_ds_misc_sys_clock *clk_cfg = (mlan_ds_misc_sys_clock *) pdata_buf;
    int i = 0;

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_ECL_SYSTEM_CLOCK_CONFIG);
    cmd->size =
        wlan_cpu_to_le16(sizeof(HostCmd_DS_ECL_SYSTEM_CLOCK_CONFIG) + S_DS_GEN);

    cfg->action = wlan_cpu_to_le16(cmd_action);
    cfg->cur_sys_clk = wlan_cpu_to_le16(clk_cfg->cur_sys_clk);
    cfg->sys_clk_type = wlan_cpu_to_le16(clk_cfg->sys_clk_type);
    cfg->sys_clk_len = wlan_cpu_to_le16(clk_cfg->sys_clk_num) * sizeof(t_u16);
    for (i = 0; i < clk_cfg->sys_clk_num; i++)
        cfg->sys_clk[i] = wlan_cpu_to_le16(clk_cfg->sys_clk[i]);

    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function prepares command of reconfigure tx buf
 *  
 *  @param priv         A pointer to mlan_private structure
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   The action: GET or SET
 *  @param pdata_buf    A pointer to data buffer
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
static mlan_status
wlan_cmd_recfg_tx_buf(mlan_private * priv,
                      HostCmd_DS_COMMAND * cmd, int cmd_action, void *pdata_buf)
{
    HostCmd_DS_TXBUF_CFG *ptx_buf = &cmd->params.tx_buf;
    t_u16 action = (t_u16) cmd_action;
    t_u16 buf_size = *((t_u16 *) pdata_buf);

    ENTER();
    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_RECONFIGURE_TX_BUFF);
    cmd->size = wlan_cpu_to_le16(sizeof(HostCmd_DS_TXBUF_CFG)
                                 + S_DS_GEN);
    ptx_buf->action = wlan_cpu_to_le16(action);
    switch (action) {
    case HostCmd_ACT_GEN_SET:
        PRINTM(CMND, "set tx_buf=%d\n", buf_size);
        ptx_buf->buff_size = wlan_cpu_to_le16(buf_size);
        break;
    case HostCmd_ACT_GEN_GET:
    default:
        ptx_buf->buff_size = 0;
        break;
    }
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function prepares command of amsdu aggr control
 *  
 *  @param priv         A pointer to mlan_private structure
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   The action: GET or SET
 *  @param pdata_buf    A pointer to data buffer
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
static mlan_status
wlan_cmd_amsdu_aggr_ctrl(mlan_private * priv,
                         HostCmd_DS_COMMAND * cmd,
                         int cmd_action, void *pdata_buf)
{
    HostCmd_DS_AMSDU_AGGR_CTRL *pamsdu_ctrl = &cmd->params.amsdu_aggr_ctrl;
    t_u16 action = (t_u16) cmd_action;
    mlan_ds_11n_amsdu_aggr_ctrl *aa_ctrl = (mlan_ds_11n_amsdu_aggr_ctrl *)
        pdata_buf;

    ENTER();

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_AMSDU_AGGR_CTRL);
    cmd->size = wlan_cpu_to_le16(sizeof(HostCmd_DS_AMSDU_AGGR_CTRL)
                                 + S_DS_GEN);
    pamsdu_ctrl->action = wlan_cpu_to_le16(action);
    switch (action) {
    case HostCmd_ACT_GEN_SET:
        pamsdu_ctrl->enable = wlan_cpu_to_le16(aa_ctrl->enable);
        pamsdu_ctrl->curr_buf_size = 0;
        break;
    case HostCmd_ACT_GEN_GET:
    default:
        pamsdu_ctrl->curr_buf_size = 0;
        break;
    }
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function prepares command of reg_access.
 *  
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   the action: GET or SET
 *  @param pdata_buf    A pointer to data buffer
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status
wlan_cmd_reg_access(IN HostCmd_DS_COMMAND * cmd,
                    IN t_u16 cmd_action, IN t_void * pdata_buf)
{
    mlan_ds_reg_rw *reg_rw;

    ENTER();

    reg_rw = (mlan_ds_reg_rw *) pdata_buf;
    switch (cmd->command) {
    case HostCmd_CMD_MAC_REG_ACCESS:
        {
            HostCmd_DS_MAC_REG_ACCESS *mac_reg;
            cmd->size =
                wlan_cpu_to_le16(sizeof(HostCmd_DS_MAC_REG_ACCESS) + S_DS_GEN);
            mac_reg = (HostCmd_DS_MAC_REG_ACCESS *) & cmd->params.mac_reg;
            mac_reg->action = wlan_cpu_to_le16(cmd_action);
            mac_reg->offset = wlan_cpu_to_le16((t_u16) reg_rw->offset);
            mac_reg->value = wlan_cpu_to_le32(reg_rw->value);
            break;
        }
    case HostCmd_CMD_BBP_REG_ACCESS:
        {
            HostCmd_DS_BBP_REG_ACCESS *bbp_reg;
            cmd->size =
                wlan_cpu_to_le16(sizeof(HostCmd_DS_BBP_REG_ACCESS) + S_DS_GEN);
            bbp_reg = (HostCmd_DS_BBP_REG_ACCESS *) & cmd->params.bbp_reg;
            bbp_reg->action = wlan_cpu_to_le16(cmd_action);
            bbp_reg->offset = wlan_cpu_to_le16((t_u16) reg_rw->offset);
            bbp_reg->value = (t_u8) reg_rw->value;
            break;
        }
    case HostCmd_CMD_RF_REG_ACCESS:
        {
            HostCmd_DS_RF_REG_ACCESS *rf_reg;
            cmd->size =
                wlan_cpu_to_le16(sizeof(HostCmd_DS_RF_REG_ACCESS) + S_DS_GEN);
            rf_reg = (HostCmd_DS_RF_REG_ACCESS *) & cmd->params.rf_reg;
            rf_reg->action = wlan_cpu_to_le16(cmd_action);
            rf_reg->offset = wlan_cpu_to_le16((t_u16) reg_rw->offset);
            rf_reg->value = (t_u8) reg_rw->value;
            break;
        }
    case HostCmd_CMD_PMIC_REG_ACCESS:
        {
            HostCmd_DS_PMIC_REG_ACCESS *pmic_reg;
            cmd->size =
                wlan_cpu_to_le16(sizeof(HostCmd_DS_PMIC_REG_ACCESS) + S_DS_GEN);
            pmic_reg = (HostCmd_DS_PMIC_REG_ACCESS *) & cmd->params.pmic_reg;
            pmic_reg->action = wlan_cpu_to_le16(cmd_action);
            pmic_reg->offset = wlan_cpu_to_le16((t_u16) reg_rw->offset);
            pmic_reg->value = (t_u8) reg_rw->value;
            break;
        }
    case HostCmd_CMD_802_11_EEPROM_ACCESS:
        {
            mlan_ds_read_eeprom *rd_eeprom = (mlan_ds_read_eeprom *) pdata_buf;
            HostCmd_DS_802_11_EEPROM_ACCESS *cmd_eeprom =
                (HostCmd_DS_802_11_EEPROM_ACCESS *) & cmd->params.eeprom;
            cmd->size =
                wlan_cpu_to_le16(sizeof(HostCmd_DS_802_11_EEPROM_ACCESS) +
                                 S_DS_GEN);
            cmd_eeprom->action = wlan_cpu_to_le16(cmd_action);
            cmd_eeprom->offset = wlan_cpu_to_le16(rd_eeprom->offset);
            cmd_eeprom->byte_count = wlan_cpu_to_le16(rd_eeprom->byte_count);
            cmd_eeprom->value = 0;
            break;
        }
    default:
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }
    cmd->command = wlan_cpu_to_le16(cmd->command);

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function prepares inactivity timeout command
 *  
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   the action: GET or SET
 *  @param pdata_buf    A pointer to data buffer
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status
wlan_cmd_inactivity_timeout(IN HostCmd_DS_COMMAND * cmd,
                            IN t_u16 cmd_action, IN t_void * pdata_buf)
{
    pmlan_ds_inactivity_to inac_to;
    HostCmd_DS_INACTIVITY_TIMEOUT_EXT *cmd_inac_to = &cmd->params.inactivity_to;

    ENTER();

    inac_to = (mlan_ds_inactivity_to *) pdata_buf;

    cmd->size =
        wlan_cpu_to_le16(sizeof(HostCmd_DS_INACTIVITY_TIMEOUT_EXT) + S_DS_GEN);
    cmd->command = wlan_cpu_to_le16(cmd->command);
    cmd_inac_to->action = wlan_cpu_to_le16(cmd_action);
    if (cmd_action == HostCmd_ACT_GEN_SET) {
        cmd_inac_to->timeout_unit = wlan_cpu_to_le16(inac_to->timeout_unit);
        cmd_inac_to->unicast_timeout =
            wlan_cpu_to_le16(inac_to->unicast_timeout);
        cmd_inac_to->mcast_timeout = wlan_cpu_to_le16(inac_to->mcast_timeout);
        cmd_inac_to->ps_entry_timeout =
            wlan_cpu_to_le16(inac_to->ps_entry_timeout);
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/********************************************************
                Global Functions
********************************************************/

/** 
 *  @brief This function allocates the command buffer and links
 *              it to command free queue.
 *  
 *  @param pmadapter    A pointer to mlan_adapter structure
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_alloc_cmd_buffer(IN mlan_adapter * pmadapter)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_callbacks *pcb = (mlan_callbacks *) & pmadapter->callbacks;
    cmd_ctrl_node *pcmd_array;
    t_u32 buf_size;
    t_u32 i;
    t_u8 *pcmd_buf = MNULL;

    ENTER();

    /* Allocate and initialize cmd_ctrl_node */
    buf_size = sizeof(cmd_ctrl_node) * MRVDRV_NUM_OF_CMD_BUFFER;
    ret = pcb->moal_malloc(buf_size, (t_u8 **) & pcmd_array);
    if (ret != MLAN_STATUS_SUCCESS || !pcmd_array) {
        PRINTM(ERROR, "ALLOC_CMD_BUF: Failed to allocate pcmd_array\n");
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    pmadapter->cmd_pool = pcmd_array;
    memset(pmadapter->cmd_pool, 0, buf_size);

    /* Allocate and initialize command buffers */
    buf_size = MRVDRV_SIZE_OF_CMD_BUFFER;
    for (i = 0; i < MRVDRV_NUM_OF_CMD_BUFFER; i++) {

        ret = pcb->moal_malloc(buf_size, &pcmd_buf);
        if (ret != MLAN_STATUS_SUCCESS || pcmd_buf == MNULL) {
            PRINTM(ERROR, "ALLOC_CMD_BUF: pcmd_buf: out of memory\n");
            ret = MLAN_STATUS_FAILURE;
            goto done;
        }

        memset(pcmd_buf, 0, buf_size);

        /* Update command buffer virtual */
        pcmd_array[i].pcmd_buf = pcmd_buf;
    }

    for (i = 0; i < MRVDRV_NUM_OF_CMD_BUFFER; i++) {
        wlan_insert_cmd_to_free_q(pmadapter, &pcmd_array[i]);
    }

    ret = MLAN_STATUS_SUCCESS;
  done:
    LEAVE();
    return ret;
}

/**
 *  @brief This function frees the command buffer.
 *  
 *  @param pmadapter    A pointer to mlan_adapter structure
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status
wlan_free_cmd_buffer(IN mlan_adapter * pmadapter)
{
    mlan_callbacks *pcb = (mlan_callbacks *) & pmadapter->callbacks;
    cmd_ctrl_node *pcmd_array;
    t_u32 i;

    ENTER();

    /* Need to check if cmd pool is allocated or not */
    if (pmadapter->cmd_pool == MNULL) {
        PRINTM(INFO, "FREE_CMD_BUF: cmd_pool is Null\n");
        goto done;
    }

    pcmd_array = pmadapter->cmd_pool;

    /* Release shared memory buffers */
    for (i = 0; i < MRVDRV_NUM_OF_CMD_BUFFER; i++) {
        if (pcmd_array[i].pcmd_buf) {
            PRINTM(CMND, "Free all the command buffer.\n");
            pcb->moal_mfree(pcmd_array[i].pcmd_buf);
            pcmd_array[i].pcmd_buf = MNULL;
        }
    }

    /* Release cmd_ctrl_node */
    if (pmadapter->cmd_pool) {
        PRINTM(CMND, "Free command pool.\n");
        pcb->moal_mfree((t_u8 *) pmadapter->cmd_pool);
        pmadapter->cmd_pool = MNULL;
    }

  done:
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function inserts command node to cmd_free_q
 *              after cleaning it.
 *  
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param pcmd_node    A pointer to cmd_ctrl_node structure
 *
 *  @return             N/A
 */
t_void
wlan_insert_cmd_to_free_q(IN mlan_adapter * pmadapter,
                          IN cmd_ctrl_node * pcmd_node)
{
    mlan_callbacks *pcb = (mlan_callbacks *) & pmadapter->callbacks;
    mlan_ioctl_req *pioctl_req = MNULL;
    ENTER();

    if (pcmd_node == MNULL)
        goto done;
    if (pcmd_node->pioctl_buf) {
        pioctl_req = (mlan_ioctl_req *) pcmd_node->pioctl_buf;
        if (pioctl_req->status_code != MLAN_ERROR_NO_ERROR)
            pcb->moal_ioctl_complete(pmadapter->pmoal_handle,
                                     pioctl_req, MLAN_STATUS_FAILURE);
        else
            pcb->moal_ioctl_complete(pmadapter->pmoal_handle,
                                     pioctl_req, MLAN_STATUS_SUCCESS);
    }
    /* Clean the node */
    wlan_clean_cmd_node(pmadapter, pcmd_node);

    /* Insert node into cmd_free_q */
    util_enqueue_list_tail(&pmadapter->cmd_free_q,
                           (pmlan_linked_list) pcmd_node,
                           pmadapter->callbacks.moal_spin_lock,
                           pmadapter->callbacks.moal_spin_unlock);
  done:
    LEAVE();
}

/** 
 *  @brief This function queues the command to cmd list.
 *  
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param pcmd_node    A pointer to cmd_ctrl_node structure
 *  @param add_tail      Specify if the cmd needs to be queued in the header or tail
 *
 *  @return             N/A
 */
t_void
wlan_insert_cmd_to_pending_q(IN mlan_adapter * pmadapter,
                             IN cmd_ctrl_node * pcmd_node, IN t_u32 add_tail)
{
    HostCmd_DS_COMMAND *pcmd;
    t_u16 command;

    ENTER();

    if (pcmd_node == MNULL) {
        PRINTM(ERROR, "QUEUE_CMD: pcmd_node is MNULL\n");
        goto done;
    }

    pcmd =
        (HostCmd_DS_COMMAND *) (pcmd_node->mbuf.pbuf +
                                pcmd_node->mbuf.data_offset);
    if (pcmd == MNULL) {
        PRINTM(ERROR, "QUEUE_CMD: pcmd is MNULL\n");
        goto done;
    }

    command = wlan_le16_to_cpu(pcmd->command);

    /* Exit_PS command needs to be queued in the header always. */
    if (command == HostCmd_CMD_802_11_PS_MODE) {
        HostCmd_DS_802_11_PS_MODE *pm = &pcmd->params.ps_mode;
        if (wlan_le16_to_cpu(pm->action) == HostCmd_SubCmd_Exit_PS) {
            if (pmadapter->ps_state != PS_STATE_FULL_POWER)
                add_tail = MFALSE;
        }
    }

    if ((command == HostCmd_CMD_802_11_WAKEUP_CONFIRM) ||
        (command == HostCmd_CMD_802_11_HOST_SLEEP_ACTIVATE) ||
        (command == HostCmd_CMD_802_11_HOST_SLEEP_CFG)) {
        add_tail = MFALSE;
    }
    if (add_tail) {
        util_enqueue_list_tail(&pmadapter->cmd_pending_q,
                               (pmlan_linked_list) pcmd_node,
                               pmadapter->callbacks.moal_spin_lock,
                               pmadapter->callbacks.moal_spin_unlock);
    } else {
        util_enqueue_list_head(&pmadapter->cmd_pending_q,
                               (pmlan_linked_list) pcmd_node,
                               pmadapter->callbacks.moal_spin_lock,
                               pmadapter->callbacks.moal_spin_unlock);
    }

    PRINTM(CMND, "QUEUE_CMD: cmd=0x%x is queued\n", command);

  done:
    LEAVE();
    return;
}

/** 
 *  @brief This function executes next command in command
 *      pending queue. It will put firmware back to PS mode
 *      if applicable.
 * 
 *  @param pmadapter     A pointer to mlan_adapter structure
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE 
 */
mlan_status
wlan_exec_next_cmd(mlan_adapter * pmadapter)
{
    mlan_private *priv = MNULL;
    cmd_ctrl_node *pcmd_node = MNULL;
    mlan_status ret = MLAN_STATUS_SUCCESS;
    HostCmd_DS_COMMAND *pcmd;

    ENTER();

    /* Sanity test */
    if (pmadapter == MNULL) {
        PRINTM(ERROR, "EXEC_NEXT_CMD: pmadapter is MNULL\n");
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }
    /* Check if already in processing */
    if (pmadapter->curr_cmd) {
        PRINTM(ERROR, "EXEC_NEXT_CMD: there is command in processing!\n");
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    wlan_request_cmd_lock(pmadapter);
    /* Check if any command is pending */
    pcmd_node = (cmd_ctrl_node *) util_peek_list(&pmadapter->cmd_pending_q,
                                                 pmadapter->callbacks.
                                                 moal_spin_lock,
                                                 pmadapter->callbacks.
                                                 moal_spin_unlock);

    if (pcmd_node) {
        pcmd =
            (HostCmd_DS_COMMAND *) (pcmd_node->mbuf.pbuf +
                                    pcmd_node->mbuf.data_offset);
        priv = pcmd_node->priv;

        if (wlan_is_cmd_allowed_in_ps(pmadapter, pcmd->command)) {
            if ((pmadapter->ps_state == PS_STATE_SLEEP) ||
                (pmadapter->ps_state == PS_STATE_PRE_SLEEP)) {
                PRINTM(ERROR,
                       "EXEC_NEXT_CMD: Cannot send cmd 0x%x in ps_state %d\n",
                       pcmd->command, pmadapter->ps_state);
                ret = MLAN_STATUS_FAILURE;
                wlan_release_cmd_lock(pmadapter);
                goto done;
            }
            PRINTM(CMND, "EXEC_NEXT_CMD: OK to send command "
                   "0x%x in ps_state %d\n", pcmd->command, pmadapter->ps_state);
        } else if (pmadapter->ps_state != PS_STATE_FULL_POWER) {
            /* 
             * 1. Non-PS command: 
             * Queue it. set need_to_wakeup to TRUE if current state 
             * is SLEEP, otherwise call wlan_exit_ps to send Exit_PS.
             * 2. PS command but not Exit_PS:
             * Ignore it.
             * 3. PS command Exit_PS:
             * Set need_to_wakeup to TRUE if current state is SLEEP, 
             * otherwise send this command down to firmware
             * immediately.
             */
            if (pcmd->command != wlan_cpu_to_le16(HostCmd_CMD_802_11_PS_MODE)) {
                wlan_release_cmd_lock(pmadapter);
                /* 
                 * Prepare to send Exit PS,
                 * this non PS command will be sent later
                 */
                if ((pmadapter->ps_state == PS_STATE_SLEEP) ||
                    (pmadapter->ps_state == PS_STATE_PRE_SLEEP)) {
                    /* 
                     * w/ new scheme, it will not reach here.
                     * since it is blocked in main_thread.
                     */
                    pmadapter->need_to_wakeup = MTRUE;
                } else
                    wlan_exit_ps(priv);

                ret = MLAN_STATUS_SUCCESS;
                goto done;

            } else {
                /* 
                 * PS command. Ignore it if it is not Exit_PS. 
                 * otherwise send it down immediately.
                 */
                HostCmd_DS_802_11_PS_MODE *pm = &pcmd->params.ps_mode;

                PRINTM(CMND, "EXEC_NEXT_CMD: PS cmd- Action=0x%x\n",
                       pm->action);
                if (pm->action != wlan_cpu_to_le16(HostCmd_SubCmd_Exit_PS)) {
                    PRINTM(CMND, "EXEC_NEXT_CMD: Ignore Enter PS cmd\n");
                    util_unlink_list(&pmadapter->cmd_pending_q,
                                     (pmlan_linked_list) pcmd_node,
                                     pmadapter->callbacks.moal_spin_lock,
                                     pmadapter->callbacks.moal_spin_unlock);
                    wlan_insert_cmd_to_free_q(pmadapter, pcmd_node);

                    ret = MLAN_STATUS_SUCCESS;
                    wlan_release_cmd_lock(pmadapter);
                    goto done;
                }

                if ((pmadapter->ps_state == PS_STATE_SLEEP) ||
                    (pmadapter->ps_state == PS_STATE_PRE_SLEEP)) {
                    PRINTM(CMND, "EXEC_NEXT_CMD: Ignore ExitPS cmd in sleep\n");
                    util_unlink_list(&pmadapter->cmd_pending_q,
                                     (pmlan_linked_list) pcmd_node,
                                     pmadapter->callbacks.moal_spin_lock,
                                     pmadapter->callbacks.moal_spin_unlock);
                    wlan_insert_cmd_to_free_q(pmadapter, pcmd_node);
                    pmadapter->need_to_wakeup = MTRUE;

                    ret = MLAN_STATUS_SUCCESS;
                    wlan_release_cmd_lock(pmadapter);
                    goto done;
                }

                PRINTM(CMND, "EXEC_NEXT_CMD: Sending Exit_PS down...\n");
            }
        }
        util_unlink_list(&pmadapter->cmd_pending_q,
                         (pmlan_linked_list) pcmd_node,
                         pmadapter->callbacks.moal_spin_lock,
                         pmadapter->callbacks.moal_spin_unlock);
        wlan_release_cmd_lock(pmadapter);
        ret = wlan_dnld_cmd_to_fw(priv, pcmd_node);
        goto done;
    } else {
        wlan_release_cmd_lock(pmadapter);
        /* Nothing in command pending queue now */
        priv = pmadapter->priv[0];
        if (priv->media_connected == MTRUE) {
            /* 
             * Check if in power save mode, if yes, put the device back
             * to PS mode
             */
            if ((pmadapter->ps_mode != Wlan802_11PowerModeCAM) &&
                (pmadapter->ps_state == PS_STATE_FULL_POWER)) {
                if (priv->sec_info.wpa_enabled || priv->sec_info.wpa2_enabled) {
                    if (priv->wpa_is_gtk_set) {
                        PRINTM(CMND, "EXEC_NEXT_CMD: WPA enabled and GTK_SET"
                               " go back to PS_SLEEP\n");
                        wlan_enter_ps(priv);
                    }
                } else {
                    if ((priv->bss_mode != MLAN_BSS_MODE_IBSS) ||
                        priv->curr_bss_params.bss_descriptor.atim_window) {
                        PRINTM(CMND, "EXEC_NEXT_CMD: Command PendQ is empty,"
                               " go back to PS_SLEEP\n");
                        wlan_enter_ps(priv);
                    }
                }
            }
        }
        /* 
         * The hs_activate command is sent when Host Sleep is configured
         * and de-activated in full power mode.
         */
        if (pmadapter->is_hs_configured && !pmadapter->hs_activated
            && (((pmadapter->ps_mode == Wlan802_11PowerModeCAM) &&
                 (pmadapter->ps_state == PS_STATE_FULL_POWER))
                || ((priv->bss_mode == MLAN_BSS_MODE_IBSS) &&
                    !priv->curr_bss_params.bss_descriptor.atim_window)
            )
            ) {
            ret = wlan_prepare_cmd(priv,
                                   HostCmd_CMD_802_11_HOST_SLEEP_ACTIVATE,
                                   0, 0, MNULL, MNULL);
        }
    }
    ret = MLAN_STATUS_SUCCESS;
  done:
    LEAVE();
    return ret;
}

/** 
 *  @brief This function prepare the command before sending to firmware.
 *  
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd_no       Command number
 *  @param cmd_action   Command action: GET or SET
 *  @param cmd_oid      Cmd oid: treated as sub command
 *  @param pioctl_buf   A pointer to MLAN IOCTl Request buffer
 *  @param pdata_buf    A pointer to information buffer
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_prepare_cmd(IN mlan_private * pmpriv,
                 IN t_u16 cmd_no,
                 IN t_u16 cmd_action,
                 IN t_u32 cmd_oid,
                 IN t_void * pioctl_buf, IN t_void * pdata_buf)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_adapter *pmadapter = pmpriv->adapter;
    cmd_ctrl_node *pcmd_node;
    HostCmd_DS_COMMAND *cmd_ptr;

    ENTER();

    /* Sanity test */
    if (pmadapter == MNULL) {
        PRINTM(ERROR, "PREP_CMD: pmadapter is MNULL\n");
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    if (pmadapter->surprise_removed) {
        PRINTM(ERROR, "PREP_CMD: Card is Removed\n");
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    /* Get a new command node */
    pcmd_node = wlan_get_cmd_node(pmadapter);

    if (pcmd_node == MNULL) {
        PRINTM(ERROR, "PREP_CMD: No free cmd node\n");
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    /* Initialize the command node */
    wlan_init_cmd_node(pmpriv, pcmd_node, cmd_oid, pioctl_buf, pdata_buf);

    cmd_ptr =
        (HostCmd_DS_COMMAND *) (pcmd_node->mbuf.pbuf +
                                pcmd_node->mbuf.data_offset);

    if (cmd_ptr == MNULL) {
        PRINTM(ERROR, "PREP_CMD: cmd_buf of cmd node is MNULL\n");
        wlan_insert_cmd_to_free_q(pmadapter, pcmd_node);
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    cmd_ptr->command = cmd_no;
    cmd_ptr->result = 0;

    /* Prepare command */
    switch (cmd_no) {
    case HostCmd_CMD_GET_HW_SPEC:
        ret = wlan_cmd_get_hw_spec(pmpriv, cmd_ptr);
        break;
    case HostCmd_CMD_MAC_CONTROL:
        ret = wlan_cmd_mac_control(pmpriv, cmd_ptr, cmd_action, pdata_buf);
        break;
    case HostCmd_CMD_802_11_MAC_ADDRESS:
        ret = wlan_cmd_802_11_mac_address(pmpriv, cmd_ptr, cmd_action);
        break;
    case HostCmd_CMD_MAC_MULTICAST_ADR:
        ret =
            wlan_cmd_mac_multicast_adr(pmpriv, cmd_ptr, cmd_action, pdata_buf);
        break;
    case HostCmd_CMD_TX_RATE_CFG:
        ret = wlan_cmd_tx_rate_cfg(pmpriv, cmd_ptr, cmd_action, pdata_buf);
        break;
    case HostCmd_CMD_802_11_RF_ANTENNA:
        ret =
            wlan_cmd_802_11_rf_antenna(pmpriv, cmd_ptr, cmd_action, pdata_buf);
        break;
    case HostCmd_CMD_TXPWR_CFG:
        ret = wlan_cmd_tx_power_cfg(pmpriv, cmd_ptr, cmd_action, pdata_buf);
        break;
    case HostCmd_CMD_802_11_PS_MODE:
        ret = wlan_cmd_802_11_ps_mode(pmpriv, cmd_ptr, cmd_action, pdata_buf);
        break;
    case HostCmd_CMD_802_11_HOST_SLEEP_CFG:
        ret = wlan_cmd_802_11_hs_cfg(pmpriv, cmd_ptr, cmd_action, pdata_buf);
        break;
    case HostCmd_CMD_802_11_WAKEUP_CONFIRM:
    case HostCmd_CMD_802_11_HOST_SLEEP_ACTIVATE:
        cmd_ptr->command = wlan_cpu_to_le16(cmd_no);
        cmd_ptr->size = wlan_cpu_to_le16(S_DS_GEN);
        break;
    case HostCmd_CMD_802_11_SCAN:
        ret = wlan_cmd_802_11_scan(pmpriv, cmd_ptr, pdata_buf);
        break;
    case HostCmd_CMD_802_11_ASSOCIATE:
        ret = wlan_cmd_802_11_associate(pmpriv, cmd_ptr, pdata_buf);
        break;
    case HostCmd_CMD_802_11_DEAUTHENTICATE:
        ret = wlan_cmd_802_11_deauthenticate(pmpriv, cmd_ptr, pdata_buf);
        break;
    case HostCmd_CMD_802_11_AD_HOC_START:
        ret = wlan_cmd_802_11_ad_hoc_start(pmpriv, cmd_ptr, pdata_buf);
        break;
    case HostCmd_CMD_802_11_AD_HOC_JOIN:
        ret = wlan_cmd_802_11_ad_hoc_join(pmpriv, cmd_ptr, pdata_buf);
        break;
    case HostCmd_CMD_802_11_AD_HOC_STOP:
        ret = wlan_cmd_802_11_ad_hoc_stop(pmpriv, cmd_ptr);
        break;
    case HostCmd_CMD_802_11_GET_LOG:
        ret = wlan_cmd_802_11_get_log(pmpriv, cmd_ptr);
        break;
    case HostCmd_CMD_RSSI_INFO:
        ret = wlan_cmd_802_11_rssi_info(pmpriv, cmd_ptr, cmd_action);
        break;
    case HostCmd_CMD_802_11_SNMP_MIB:
        ret =
            wlan_cmd_802_11_snmp_mib(pmpriv, cmd_ptr, cmd_action, cmd_oid,
                                     pdata_buf);
        break;
    case HostCmd_CMD_802_11_RADIO_CONTROL:
        ret = wlan_cmd_802_11_radio_control(pmpriv, cmd_ptr, cmd_action);
        break;
    case HostCmd_CMD_802_11_TX_RATE_QUERY:
        cmd_ptr->command = wlan_cpu_to_le16(HostCmd_CMD_802_11_TX_RATE_QUERY);
        cmd_ptr->size =
            wlan_cpu_to_le16(sizeof(HostCmd_TX_RATE_QUERY) + S_DS_GEN);
        pmpriv->tx_rate = 0;
        ret = MLAN_STATUS_SUCCESS;
        break;
    case HostCmd_CMD_VERSION_EXT:
        cmd_ptr->command = wlan_cpu_to_le16(cmd_no);
        cmd_ptr->params.verext.version_str_sel =
            (t_u8) (*((t_u32 *) pdata_buf));
        memcpy(&cmd_ptr->params, pdata_buf, sizeof(HostCmd_DS_VERSION_EXT));
        cmd_ptr->size =
            wlan_cpu_to_le16(sizeof(HostCmd_DS_VERSION_EXT) + S_DS_GEN);
        ret = MLAN_STATUS_SUCCESS;
        break;
    case HostCmd_CMD_802_11_RF_CHANNEL:
        ret =
            wlan_cmd_802_11_rf_channel(pmpriv, cmd_ptr, cmd_action, pdata_buf);
        break;
    case HostCmd_CMD_FUNC_INIT:
    case HostCmd_CMD_FUNC_SHUTDOWN:
        cmd_ptr->command = wlan_cpu_to_le16(cmd_no);
        cmd_ptr->size = wlan_cpu_to_le16(S_DS_GEN);
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
    case HostCmd_CMD_802_11_KEY_MATERIAL:
        ret =
            wlan_cmd_802_11_key_material(pmpriv, cmd_ptr, cmd_action, cmd_oid,
                                         pdata_buf);
        break;
    case HostCmd_CMD_SUPPLICANT_PMK:
        ret =
            wlan_cmd_802_11_supplicant_pmk(pmpriv, cmd_ptr, cmd_action,
                                           pdata_buf);
        break;
    case HostCmd_CMD_SUPPLICANT_PROFILE:
        ret = wlan_cmd_802_11_supplicant_profile(pmpriv, cmd_ptr, cmd_action);
        break;
    case HostCmd_CMD_802_11D_DOMAIN_INFO:
        ret = wlan_cmd_802_11d_domain_info(pmpriv, cmd_ptr, cmd_action);
        break;
    case HostCmd_CMD_RECONFIGURE_TX_BUFF:
        ret = wlan_cmd_recfg_tx_buf(pmpriv, cmd_ptr, cmd_action, pdata_buf);
        break;
    case HostCmd_CMD_AMSDU_AGGR_CTRL:
        ret = wlan_cmd_amsdu_aggr_ctrl(pmpriv, cmd_ptr, cmd_action, pdata_buf);
        break;
    case HostCmd_CMD_11N_CFG:
        ret = wlan_cmd_11n_cfg(pmpriv, cmd_ptr, cmd_action, pdata_buf);
        break;
    case HostCmd_CMD_WMM_GET_STATUS:
        PRINTM(CMND, "WMM: WMM_GET_STATUS cmd sent\n");
        cmd_ptr->command = wlan_cpu_to_le16(HostCmd_CMD_WMM_GET_STATUS);
        cmd_ptr->size =
            wlan_cpu_to_le16(sizeof(HostCmd_DS_WMM_GET_STATUS) + S_DS_GEN);
        ret = MLAN_STATUS_SUCCESS;
        break;
    case HostCmd_CMD_802_11_IBSS_COALESCING_STATUS:
        ret =
            wlan_cmd_ibss_coalescing_status(pmpriv, cmd_ptr, cmd_action,
                                            pdata_buf);
        break;
#ifdef MFG_CMD_SUPPORT
    case HostCmd_CMD_MFG_COMMAND:
        ret = wlan_cmd_mfg_cmd(pmpriv, cmd_ptr, pdata_buf);
        break;
#endif
    case HostCmd_CMD_HOST_COMMAND:
        ret = wlan_cmd_host_cmd(pmpriv, cmd_ptr, pdata_buf);
        pcmd_node->cmd_flag |= CMD_F_HOSTCMD;
        break;
    case HostCmd_CMD_ECL_SYSTEM_CLOCK_CONFIG:
        ret = wlan_cmd_sysclock_cfg(pmpriv, cmd_ptr, cmd_action, pdata_buf);
        break;
    case HostCmd_CMD_HEART_BEAT:
        ret = wlan_cmd_heart_beat(pmpriv, cmd_ptr, cmd_action, pdata_buf);
        break;
    case HostCmd_CMD_MAC_REG_ACCESS:
    case HostCmd_CMD_BBP_REG_ACCESS:
    case HostCmd_CMD_RF_REG_ACCESS:
    case HostCmd_CMD_PMIC_REG_ACCESS:
    case HostCmd_CMD_802_11_EEPROM_ACCESS:
        ret = wlan_cmd_reg_access(cmd_ptr, cmd_action, pdata_buf);
        break;
    case HostCmd_CMD_INACTIVITY_TIMEOUT_EXT:
        ret = wlan_cmd_inactivity_timeout(cmd_ptr, cmd_action, pdata_buf);
        break;
    default:
        PRINTM(ERROR, "PREP_CMD: unknown command- %#x\n", cmd_no);
        ret = MLAN_STATUS_FAILURE;
        break;
    }

    /* Return error, since the command preparation failed */
    if (ret != MLAN_STATUS_SUCCESS) {
        PRINTM(ERROR, "PREP_CMD: Command 0x%x preparation failed\n", cmd_no);
        wlan_insert_cmd_to_free_q(pmadapter, pcmd_node);
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    /* Send command */
    if (cmd_no == HostCmd_CMD_802_11_SCAN) {
        wlan_queue_scan_cmd(pmpriv, pcmd_node);
    } else {
        wlan_insert_cmd_to_pending_q(pmadapter, pcmd_node, MTRUE);
    }

  done:
    LEAVE();
    return ret;
}

/** 
 *  @brief This function requests a lock on command queue.
 *  
 *  @param pmadapter    A pointer to mlan_adapter structure
 *
 *  @return             N/A
 */
t_void
wlan_request_cmd_lock(IN mlan_adapter * pmadapter)
{
    mlan_callbacks *pcb = (mlan_callbacks *) & pmadapter->callbacks;

    ENTER();

    /* Call MOAL spin lock callback function */
    pcb->moal_spin_lock(pmadapter->pmlan_cmd_lock);

    LEAVE();
    return;
}

/** 
 *  @brief This function releases a lock on command queue.
 *  
 *  @param pmadapter    A pointer to mlan_adapter structure
 *
 *  @return             N/A
 */
t_void
wlan_release_cmd_lock(IN mlan_adapter * pmadapter)
{
    mlan_callbacks *pcb = (mlan_callbacks *) & pmadapter->callbacks;

    ENTER();

    /* Call MOAL spin unlock callback function */
    pcb->moal_spin_unlock(pmadapter->pmlan_cmd_lock);

    LEAVE();
    return;
}

/** 
 *  @brief This function handles the timeout of command sending.
 *  It will re-send the same command again.
 *  
 *  @param function_context   A pointer to function_context
 *  @return 	   n/a
 */
t_void
wlan_cmd_timeout_func(t_void * function_context)
{
    mlan_adapter *pmadapter = (mlan_adapter *) function_context;
    cmd_ctrl_node *pcmd_node = MNULL;
    HostCmd_DS_COMMAND *pcmd;
    mlan_ioctl_req *pioctl_buf = MNULL;
    t_u32 sec, usec;
    t_u8 i;
    ENTER();

    pmadapter->cmd_timer_is_set = MFALSE;
    pmadapter->num_cmd_timeout++;
    pmadapter->dbg.num_cmd_timeout++;
    if (!pmadapter->curr_cmd) {
        PRINTM(WARN, "CurCmd Empty\n");
        goto exit;
    }
    pcmd_node = pmadapter->curr_cmd;
    if (pcmd_node->pioctl_buf != MNULL) {
        pioctl_buf = (mlan_ioctl_req *) pcmd_node->pioctl_buf;
        pioctl_buf->status_code = MLAN_ERROR_CMD_TIMEOUT;
    }
    pcmd =
        (HostCmd_DS_COMMAND *) (pcmd_node->mbuf.pbuf +
                                pcmd_node->mbuf.data_offset +
                                MLAN_USB_TYPE_LEN);
    if (pcmd->size) {
        pmadapter->dbg.timeout_cmd_id = wlan_cpu_to_le16(pcmd->command);
        pmadapter->dbg.timeout_cmd_act =
            wlan_cpu_to_le16(*(t_u16 *) ((t_u8 *) pcmd + S_DS_GEN));
        pmadapter->callbacks.moal_get_system_time(&sec, &usec);
        PRINTM(ERROR, "Timeout cmd id (%lu.%lu) = 0x%x, act = 0x%x \n", sec,
               usec, pmadapter->dbg.timeout_cmd_id,
               pmadapter->dbg.timeout_cmd_act);

        PRINTM(ERROR, "num_data_h2c_failure = %d\n",
               pmadapter->dbg.num_tx_host_to_card_failure);
        PRINTM(ERROR, "num_cmd_h2c_failure = %d\n",
               pmadapter->dbg.num_cmd_host_to_card_failure);

        PRINTM(ERROR, "num_cmd_timeout = %d\n", pmadapter->dbg.num_cmd_timeout);
        PRINTM(ERROR, "num_tx_timeout = %d\n", pmadapter->dbg.num_tx_timeout);

        PRINTM(ERROR, "last_cmd_index = %d\n", pmadapter->dbg.last_cmd_index);
        PRINTM(ERROR, "last_cmd_id = ");
        for (i = 0; i < DBG_CMD_NUM; i++) {
            PRINTM(ERROR, "0x%x ", pmadapter->dbg.last_cmd_id[i]);
        }
        PRINTM(MSG, "\n");
        PRINTM(ERROR, "last_cmd_act = ");
        for (i = 0; i < DBG_CMD_NUM; i++) {
            PRINTM(ERROR, "0x%x ", pmadapter->dbg.last_cmd_act[i]);
        }
        PRINTM(MSG, "\n");
        PRINTM(ERROR, "last_cmd_resp_index = %d\n",
               pmadapter->dbg.last_cmd_resp_index);
        PRINTM(ERROR, "last_cmd_resp_id = ");
        for (i = 0; i < DBG_CMD_NUM; i++) {
            PRINTM(ERROR, "0x%x ", pmadapter->dbg.last_cmd_resp_id[i]);
        }
        PRINTM(MSG, "\n");

        PRINTM(ERROR, "last_event_index = %d\n",
               pmadapter->dbg.last_event_index);
        PRINTM(ERROR, "last_event = ");
        for (i = 0; i < DBG_CMD_NUM; i++) {
            PRINTM(ERROR, "0x%x ", pmadapter->dbg.last_event[i]);
        }
        PRINTM(MSG, "\n");

        PRINTM(ERROR, "data_sent=%d cmd_sent=%d\n", pmadapter->data_sent,
               pmadapter->cmd_sent);

        PRINTM(ERROR, "ps_mode=%d ps_state=%d\n", pmadapter->ps_mode,
               pmadapter->ps_state);

    }
    if (pmadapter->hw_status == WlanHardwareStatusInitializing)
        wlan_init_fw_complete(pmadapter);
  exit:
    LEAVE();
    return;
}

/**
 *  @brief This function will return the pointer to the first entry in 
 *  		pending cmd which matches the given pioctl_req
 *  
 *  @param pmadapter    A pointer to mlan_adapter
 *  @param pioctl_req   A pointer to mlan_ioctl_req buf
 *
 *  @return 	   A pointer to first entry match pioctl_req
 */
cmd_ctrl_node *
wlan_get_pending_ioctl_cmd(pmlan_adapter pmadapter, pmlan_ioctl_req pioctl_req)
{
    cmd_ctrl_node *pcmd_node = MNULL;

    ENTER();

    if (!
        (pcmd_node =
         (cmd_ctrl_node *) util_peek_list(&pmadapter->cmd_pending_q,
                                          pmadapter->callbacks.moal_spin_lock,
                                          pmadapter->callbacks.
                                          moal_spin_unlock))) {
        LEAVE();
        return MNULL;
    }
    while (pcmd_node != (cmd_ctrl_node *) & pmadapter->cmd_pending_q) {
        if (pcmd_node->pioctl_buf == pioctl_req) {
            LEAVE();
            return pcmd_node;
        }
        pcmd_node = pcmd_node->pnext;
    }
    LEAVE();
    return MNULL;
}

/**
 *  @brief Cancel pending ioctl cmd.
 *  
 *  @param pmadapter	A pointer to mlan_adapter
 *  @param pioctl_req	A pointer to mlan_ioctl_req buf
 *
 *  @return		N/A
 */
void
wlan_cancel_pending_ioctl(pmlan_adapter pmadapter, pmlan_ioctl_req pioctl_req)
{
    pmlan_callbacks pcb = &pmadapter->callbacks;
    cmd_ctrl_node *pcmd_node = MNULL;

    ENTER();

    PRINTM(CMND, "MOAL Cancel IOCTL: 0x%x sub_id=0x%x action=%d\n",
           pioctl_req->req_id, *((t_u32 *) pioctl_req->pbuf),
           (int) pioctl_req->action);

    if ((pmadapter->curr_cmd) &&
        (pmadapter->curr_cmd->pioctl_buf == pioctl_req)) {
        wlan_request_cmd_lock(pmadapter);
        pcmd_node = pmadapter->curr_cmd;
        pcmd_node->pioctl_buf = MNULL;
        wlan_release_cmd_lock(pmadapter);
    }

    wlan_request_cmd_lock(pmadapter);
    while ((pcmd_node =
            wlan_get_pending_ioctl_cmd(pmadapter, pioctl_req)) != MNULL) {
        util_unlink_list(&pmadapter->cmd_pending_q,
                         (pmlan_linked_list) pcmd_node,
                         pmadapter->callbacks.moal_spin_lock,
                         pmadapter->callbacks.moal_spin_unlock);
        pcmd_node->pioctl_buf = MNULL;
        wlan_insert_cmd_to_free_q(pmadapter, pcmd_node);
    }
    wlan_release_cmd_lock(pmadapter);
    if (pioctl_req->req_id == MLAN_IOCTL_SCAN) {
        /* Cancel all pending scan command */
        while ((pcmd_node =
                (cmd_ctrl_node *) util_peek_list(&pmadapter->scan_pending_q,
                                                 pcb->moal_spin_lock,
                                                 pcb->moal_spin_unlock))) {
            util_unlink_list(&pmadapter->scan_pending_q,
                             (pmlan_linked_list) pcmd_node, pcb->moal_spin_lock,
                             pcb->moal_spin_unlock);
            pcmd_node->pioctl_buf = MNULL;
            wlan_insert_cmd_to_free_q(pmadapter, pcmd_node);
        }
        wlan_request_cmd_lock(pmadapter);
        pmadapter->scan_processing = MFALSE;
        wlan_release_cmd_lock(pmadapter);
    }
    pioctl_req->status_code = MLAN_ERROR_CMD_CANCEL;
    pcb->moal_ioctl_complete(pmadapter->pmoal_handle, pioctl_req,
                             MLAN_STATUS_FAILURE);

    LEAVE();
    return;
}

/**
 *  @brief Cancel all pending cmd.
 *  
 *  @param pmadapter	A pointer to mlan_adapter
 *
 *  @return		N/A
 */
void
wlan_cancel_all_pending_cmd(pmlan_adapter pmadapter)
{
    cmd_ctrl_node *pcmd_node = MNULL;
    pmlan_callbacks pcb = &pmadapter->callbacks;
    mlan_ioctl_req *pioctl_buf = MNULL;

    /* Cancel current cmd */
    if ((pmadapter->curr_cmd) && (pmadapter->curr_cmd->pioctl_buf)) {
        pioctl_buf = (mlan_ioctl_req *) pmadapter->curr_cmd->pioctl_buf;
        wlan_request_cmd_lock(pmadapter);
        pmadapter->curr_cmd->pioctl_buf = MNULL;
        wlan_release_cmd_lock(pmadapter);
        pioctl_buf->status_code = MLAN_ERROR_CMD_CANCEL;
        pcb->moal_ioctl_complete(pmadapter->pmoal_handle, pioctl_buf,
                                 MLAN_STATUS_FAILURE);
    }
    /* Cancel all pending command */
    while ((pcmd_node =
            (cmd_ctrl_node *) util_peek_list(&pmadapter->cmd_pending_q,
                                             pcb->moal_spin_lock,
                                             pcb->moal_spin_unlock))) {
        util_unlink_list(&pmadapter->cmd_pending_q,
                         (pmlan_linked_list) pcmd_node, pcb->moal_spin_lock,
                         pcb->moal_spin_unlock);
        if (pcmd_node->pioctl_buf) {
            pioctl_buf = (mlan_ioctl_req *) pcmd_node->pioctl_buf;
            pioctl_buf->status_code = MLAN_ERROR_CMD_CANCEL;
            pcb->moal_ioctl_complete(pmadapter->pmoal_handle, pioctl_buf,
                                     MLAN_STATUS_FAILURE);
            pcmd_node->pioctl_buf = MNULL;
        }
        wlan_insert_cmd_to_free_q(pmadapter, pcmd_node);
    }
    /* Cancel all pending scan command */
    while ((pcmd_node =
            (cmd_ctrl_node *) util_peek_list(&pmadapter->scan_pending_q,
                                             pcb->moal_spin_lock,
                                             pcb->moal_spin_unlock))) {
        util_unlink_list(&pmadapter->scan_pending_q,
                         (pmlan_linked_list) pcmd_node, pcb->moal_spin_lock,
                         pcb->moal_spin_unlock);
        pcmd_node->pioctl_buf = MNULL;
        wlan_insert_cmd_to_free_q(pmadapter, pcmd_node);
    }
    wlan_request_cmd_lock(pmadapter);
    pmadapter->scan_processing = MFALSE;
    wlan_release_cmd_lock(pmadapter);
}
