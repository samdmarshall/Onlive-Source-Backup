/**
 * @file mlan_cmdevt.c
 *
 *  @brief This file contains the handling of CMD/EVENT in MLAN
 *
 *
 *  Copyright (C) 2009-2010, Marvell International Ltd. 
 *  All Rights Reserved
 *   
 */

/*************************************************************
Change Log:
    05/12/2009: initial version
************************************************************/
#include "mlan.h"
#ifdef STA_SUPPORT
#include "mlan_join.h"
#endif
#include "mlan_util.h"
#include "mlan_fw.h"
#include "mlan_main.h"
#include "mlan_wmm.h"
#include "mlan_11n.h"
#include "mlan_11h.h"
#include "mlan_sdio.h"
/********************************************************
                Local Variables
********************************************************/

/*******************************************************
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
 *  @param pioctl_buf   A pointer to MLAN IOCTL Request buffer
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
    mlan_adapter *pmadapter = pmpriv->adapter;

    ENTER();

    if (pcmd_node == MNULL) {
        LEAVE();
        return;
    }
    pcmd_node->priv = pmpriv;
    pcmd_node->cmd_oid = cmd_oid;
    pcmd_node->pioctl_buf = pioctl_buf;
    pcmd_node->pdata_buf = pdata_buf;

    pcmd_node->cmdbuf = pcmd_node->pmbuf;

    /* Make sure head_ptr for cmd buf is Align */
    pcmd_node->cmdbuf->data_offset = 0;
    memset(pmadapter, pcmd_node->cmdbuf->pbuf, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

    /* Prepare mlan_buffer for command sending */
    pcmd_node->cmdbuf->buf_type = MLAN_BUF_TYPE_CMD;
    pcmd_node->cmdbuf->data_offset += INTF_HEADER_LEN;

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

    if (util_peek_list(pmadapter->pmoal_handle, &pmadapter->cmd_free_q,
                       pmadapter->callbacks.moal_spin_lock,
                       pmadapter->callbacks.moal_spin_unlock)) {
        pcmd_node = (cmd_ctrl_node *) util_dequeue_list(pmadapter->pmoal_handle,
                                                        &pmadapter->cmd_free_q,
                                                        pmadapter->callbacks.
                                                        moal_spin_lock,
                                                        pmadapter->callbacks.
                                                        moal_spin_unlock);
    } else {
        PRINTM(MERROR, "GET_CMD_NODE: cmd_ctrl_node is not available\n");
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

    if (pcmd_node->respbuf) {
        pmadapter->callbacks.moal_free_mlan_buffer(pmadapter->pmoal_handle,
                                                   pcmd_node->respbuf);
        pcmd_node->respbuf = MNULL;
    }

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
         (cmd_ctrl_node *) util_peek_list(pmadapter->pmoal_handle,
                                          &pmadapter->cmd_pending_q,
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
 *  @brief This function handles the command response of host_cmd
 *  
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *
 *  @return        MLAN_STATUS_SUCCESS
 */
static mlan_status
wlan_ret_host_cmd(IN pmlan_private pmpriv,
                  IN HostCmd_DS_COMMAND * resp, IN mlan_ioctl_req * pioctl_buf)
{
    mlan_ds_misc_cfg *misc;
    t_u16 size = wlan_le16_to_cpu(resp->size);

    ENTER();

    PRINTM(MINFO, "host command response size = %d\n", size);
    size = MIN(size, MRVDRV_SIZE_OF_CMD_BUFFER);
    if (pioctl_buf) {
        misc = (mlan_ds_misc_cfg *) pioctl_buf->pbuf;
        misc->param.hostcmd.len = size;
        memcpy(pmpriv->adapter, misc->param.hostcmd.cmd, (void *) resp, size);
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

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
    memcpy(pmpriv->adapter, (void *) cmd, pcmd_ptr->cmd, pcmd_ptr->len);
    PRINTM(MINFO, "Host command size = %d\n", pcmd_ptr->len);
    LEAVE();
    return MLAN_STATUS_SUCCESS;
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
    t_u32 sec, usec;

    ENTER();

    if (!pmadapter || !pcmd_node) {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    pcmd =
        (HostCmd_DS_COMMAND *) (pcmd_node->cmdbuf->pbuf +
                                pcmd_node->cmdbuf->data_offset);
    if (pcmd_node->pioctl_buf != MNULL)
        pioctl_buf = (mlan_ioctl_req *) pcmd_node->pioctl_buf;

    /* Sanity test */
    if (pcmd == MNULL || pcmd->size == 0) {
        PRINTM(MERROR, "DNLD_CMD: pcmd is null or command size is zero, "
               "Not sending\n");
        if (pioctl_buf != MNULL)
            pioctl_buf->status_code = MLAN_ERROR_CMD_DNLD_FAIL,
                wlan_insert_cmd_to_free_q(pmadapter, pcmd_node);
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    /* Set command sequence number */
    pmadapter->seq_num++;
    pcmd->seq_num =
        wlan_cpu_to_le16(HostCmd_SET_SEQ_NO_BSS_INFO
                         (pmadapter->seq_num, pcmd_node->priv->bss_num,
                          pcmd_node->priv->bss_type));

    wlan_request_cmd_lock(pmadapter);
    pmadapter->curr_cmd = pcmd_node;
    wlan_release_cmd_lock(pmadapter);

    cmd_code = wlan_le16_to_cpu(pcmd->command);
    cmd_size = wlan_le16_to_cpu(pcmd->size);

    pcmd_node->cmdbuf->data_len = cmd_size;

    pmadapter->callbacks.moal_get_system_time(pmadapter->pmoal_handle, &sec,
                                              &usec);
    PRINTM(MCMND, "DNLD_CMD (%lu.%lu): 0x%x, act 0x%x, len %d, seqno 0x%x\n",
           sec, usec, cmd_code,
           wlan_le16_to_cpu(*(t_u16 *) ((t_u8 *) pcmd + S_DS_GEN)), cmd_size,
           wlan_le16_to_cpu(pcmd->seq_num));
    DBG_HEXDUMP(MCMD_D, "DNLD_CMD", (t_u8 *) pcmd, cmd_size);

    /* Send the command to lower layer */

    pcmd_node->cmdbuf->data_offset -= INTF_HEADER_LEN;
    pcmd_node->cmdbuf->data_len += INTF_HEADER_LEN;
    /* Extra header for SDIO is added here */
    ret =
        wlan_sdio_host_to_card(pmadapter, MLAN_TYPE_CMD, pcmd_node->cmdbuf,
                               MNULL);

    if (ret == MLAN_STATUS_FAILURE) {
        PRINTM(MERROR, "DNLD_CMD: Host to Card Failed\n");
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

    /* Clear BSS_NO_BITS from HostCmd */
    cmd_code &= HostCmd_CMD_ID_MASK;
    /* soft_reset command has no command response, we should return here */
    if (cmd_code == HostCmd_CMD_SOFT_RESET) {
        PRINTM(MCMND, "DNLD_CMD: SoftReset\n");
        if (pioctl_buf) {
            wlan_request_cmd_lock(pmadapter);
            pmadapter->curr_cmd->pioctl_buf = MNULL;
            wlan_release_cmd_lock(pmadapter);
            pcb->moal_ioctl_complete(pmadapter->pmoal_handle, pioctl_buf,
                                     MLAN_STATUS_SUCCESS);
        }
        goto done;
    }

    /* Setup the timer after transmit command */
    pcb->moal_start_timer(pmadapter->pmoal_handle, pmadapter->pmlan_cmd_timer,
                          MFALSE, MRVDRV_TIMER_60S);

    pmadapter->cmd_timer_is_set = MTRUE;

    ret = MLAN_STATUS_SUCCESS;

  done:
    LEAVE();
    return ret;
}

/**
 *  @brief This function sends sleep confirm command to firmware.
 * 
 *  @param pmadapter  A pointer to mlan_adapter structure
 *
 *  @return        MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_dnld_sleep_confirm_cmd(mlan_adapter * pmadapter)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    static t_u32 i = 0;
    t_u16 cmd_len = 0;
    opt_sleep_confirm_buffer *sleep_cfm_buf =
        (opt_sleep_confirm_buffer *) (pmadapter->psleep_cfm->pbuf +
                                      pmadapter->psleep_cfm->data_offset);

    ENTER();

    cmd_len = sizeof(OPT_Confirm_Sleep);
    pmadapter->seq_num++;
    sleep_cfm_buf->ps_cfm_sleep.seq_num =
        wlan_cpu_to_le16(HostCmd_SET_SEQ_NO_BSS_INFO
                         (pmadapter->seq_num,
                          (wlan_get_priv(pmadapter, MLAN_BSS_TYPE_ANY))->
                          bss_num,
                          (wlan_get_priv(pmadapter, MLAN_BSS_TYPE_ANY))->
                          bss_type));
    DBG_HEXDUMP(MCMD_D, "SLEEP_CFM", &sleep_cfm_buf->ps_cfm_sleep, cmd_len);

    /* Send sleep confirm command to firmware */

    pmadapter->psleep_cfm->data_len = cmd_len + INTF_HEADER_LEN;
    ret = wlan_sdio_host_to_card(pmadapter, MLAN_TYPE_CMD,
                                 pmadapter->psleep_cfm, MNULL);

    if (ret == MLAN_STATUS_FAILURE) {
        PRINTM(MERROR, "SLEEP_CFM: failed\n");
        pmadapter->dbg.num_cmd_sleep_cfm_host_to_card_failure++;
        goto done;
    } else {
        if ((wlan_get_priv(pmadapter, MLAN_BSS_TYPE_ANY))->bss_type ==
            MLAN_BSS_TYPE_UAP)
            pmadapter->ps_state = PS_STATE_SLEEP_CFM;
#ifdef STA_SUPPORT
        if ((wlan_get_priv(pmadapter, MLAN_BSS_TYPE_ANY))->bss_type ==
            MLAN_BSS_TYPE_STA) {
            if (!sleep_cfm_buf->ps_cfm_sleep.sleep_cfm.resp_ctrl) {
                /* Response is not needed for sleep confirm command */
                pmadapter->ps_state = PS_STATE_SLEEP;
            } else {
                pmadapter->ps_state = PS_STATE_SLEEP_CFM;
            }

            if (!sleep_cfm_buf->ps_cfm_sleep.sleep_cfm.resp_ctrl
                && (pmadapter->is_hs_configured &&
                    !pmadapter->sleep_period.period)) {
                pmadapter->pm_wakeup_card_req = MTRUE;
                wlan_host_sleep_activated_event(wlan_get_priv
                                                (pmadapter, MLAN_BSS_TYPE_STA),
                                                MTRUE);
            }
        }
#endif /* STA_SUPPORT */

#define NUM_SC_PER_LINE         16
        if (++i % NUM_SC_PER_LINE == 0)
            PRINTM(MEVENT, "+\n");
        else
            PRINTM(MEVENT, "+");
    }

  done:
    LEAVE();
    return ret;
}

/********************************************************
                Global Functions
********************************************************/

/** 
 *  @brief Event handler
 *  
 *  @param priv		A pointer to mlan_private structure
 *  @param event_id	Event ID
 *  @param pmevent	Event buffer
 *
 *  @return		MLAN_STATUS_SUCCESS
 */
mlan_status
wlan_recv_event(pmlan_private priv, mlan_event_id event_id, t_void * pmevent)
{
    pmlan_callbacks pcb = &priv->adapter->callbacks;

    ENTER();

    if (pmevent)
        /* The caller has provided the event. */
        pcb->moal_recv_event(priv->adapter->pmoal_handle,
                             (pmlan_event) pmevent);
    else {
        mlan_event mevent;

        memset(priv->adapter, &mevent, 0, sizeof(mlan_event));
        mevent.bss_num = priv->bss_index;
        mevent.event_id = event_id;
        mevent.event_len = 0;

        pcb->moal_recv_event(priv->adapter->pmoal_handle, &mevent);
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

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
    cmd_ctrl_node *pcmd_array = MNULL;
    t_u32 buf_size;
    t_u32 i;

    ENTER();

    /* Allocate and initialize cmd_ctrl_node */
    buf_size = sizeof(cmd_ctrl_node) * MRVDRV_NUM_OF_CMD_BUFFER;
    ret =
        pcb->moal_malloc(pmadapter->pmoal_handle, buf_size,
                         MLAN_MEM_DEF | MLAN_MEM_DMA, (t_u8 **) & pcmd_array);
    if (ret != MLAN_STATUS_SUCCESS || !pcmd_array) {
        PRINTM(MERROR, "ALLOC_CMD_BUF: Failed to allocate pcmd_array\n");
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    pmadapter->cmd_pool = pcmd_array;
    memset(pmadapter, pmadapter->cmd_pool, 0, buf_size);

    /* Allocate and initialize command buffers */
    for (i = 0; i < MRVDRV_NUM_OF_CMD_BUFFER; i++) {
        if (!(pcmd_array[i].pmbuf = wlan_alloc_mlan_buffer(pmadapter,
                                                           pcb,
                                                           MRVDRV_SIZE_OF_CMD_BUFFER)))
        {
            PRINTM(MERROR,
                   "ALLOC_CMD_BUF: Failed to allocate command buffer\n");
            ret = MLAN_STATUS_FAILURE;
            goto done;
        }
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
        PRINTM(MINFO, "FREE_CMD_BUF: cmd_pool is Null\n");
        goto done;
    }

    pcmd_array = pmadapter->cmd_pool;

    /* Release shared memory buffers */
    for (i = 0; i < MRVDRV_NUM_OF_CMD_BUFFER; i++) {
        if (pcmd_array[i].pmbuf) {
            PRINTM(MINFO, "Free all the command buffer.\n");
            wlan_free_mlan_buffer(pmadapter, pcb, pcmd_array[i].pmbuf);
            pcmd_array[i].pmbuf = MNULL;
        }
        if (pcmd_array[i].respbuf) {
            pmadapter->callbacks.moal_free_mlan_buffer(pmadapter->pmoal_handle,
                                                       pcmd_array[i].respbuf);
            pcmd_array[i].respbuf = MNULL;
        }
    }
    /* Release cmd_ctrl_node */
    if (pmadapter->cmd_pool) {
        PRINTM(MINFO, "Free command pool.\n");
        pcb->moal_mfree(pmadapter->pmoal_handle, (t_u8 *) pmadapter->cmd_pool);
        pmadapter->cmd_pool = MNULL;
    }

  done:
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function handles events generated by firmware
 *  
 *  @param pmadapter		A pointer to mlan_adapter structure
 *
 *  @return		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_process_event(pmlan_adapter pmadapter)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    pmlan_private priv = wlan_get_priv(pmadapter, MLAN_BSS_TYPE_ANY);
    pmlan_buffer pmbuf = pmadapter->pmlan_buffer_event;
    t_u32 eventcause = pmadapter->event_cause;
    t_u32 in_ts_sec;
    t_u32 in_ts_usec;
    ENTER();

    /* Save the last event to debug log */
    pmadapter->dbg.last_event_index =
        (pmadapter->dbg.last_event_index + 1) % DBG_CMD_NUM;
    pmadapter->dbg.last_event[pmadapter->dbg.last_event_index] =
        (t_u16) eventcause;

    /* Get BSS number and corresponding priv */
    priv =
        wlan_get_priv_by_id(pmadapter, EVENT_GET_BSS_NUM(eventcause),
                            EVENT_GET_BSS_TYPE(eventcause));
    if (!priv)
        priv = wlan_get_priv(pmadapter, MLAN_BSS_TYPE_ANY);
    /* Clear BSS_NO_BITS from event */
    eventcause &= EVENT_ID_MASK;
    pmadapter->event_cause = eventcause;

    if (pmbuf)
        pmbuf->bss_num = priv->bss_index;

    if (eventcause != EVENT_PS_SLEEP && eventcause != EVENT_PS_AWAKE) {
        pmadapter->callbacks.moal_get_system_time(pmadapter->pmoal_handle,
                                                  &in_ts_sec, &in_ts_usec);
        PRINTM(MEVENT, "%lu.%lu : Event: 0x%x\n", in_ts_sec, in_ts_usec,
               eventcause);
    }

    ret = priv->ops.process_event(priv);

    pmadapter->event_cause = 0;
    pmadapter->pmlan_buffer_event = MNULL;

    if (pmbuf) {
        pmadapter->callbacks.moal_free_mlan_buffer(pmadapter->pmoal_handle,
                                                   pmbuf);
    }

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
    pcb->moal_spin_lock(pmadapter->pmoal_handle, pmadapter->pmlan_cmd_lock);

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
    pcb->moal_spin_unlock(pmadapter->pmoal_handle, pmadapter->pmlan_cmd_lock);

    LEAVE();
    return;
}

/** 
 *  @brief This function prepare the command before sending to firmware.
 *  
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd_no       Command number
 *  @param cmd_action   Command action: GET or SET
 *  @param cmd_oid      Cmd oid: treated as sub command
 *  @param pioctl_buf   A pointer to MLAN IOCTL Request buffer
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
    cmd_ctrl_node *pcmd_node = MNULL;
    HostCmd_DS_COMMAND *cmd_ptr = MNULL;

    ENTER();

    /* Sanity test */
    if (pmadapter == MNULL) {
        PRINTM(MERROR, "PREP_CMD: pmadapter is MNULL\n");
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    if (pmadapter->surprise_removed) {
        PRINTM(MERROR, "PREP_CMD: Card is Removed\n");
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    if (pmadapter->hw_status == WlanHardwareStatusReset) {
        if (cmd_no != HostCmd_CMD_FUNC_INIT) {
            PRINTM(MERROR, "PREP_CMD: FW is in reset state\n");
            ret = MLAN_STATUS_FAILURE;
            goto done;
        }
    }

    /* Get a new command node */
    pcmd_node = wlan_get_cmd_node(pmadapter);

    if (pcmd_node == MNULL) {
        PRINTM(MERROR, "PREP_CMD: No free cmd node\n");
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    /* Initialize the command node */
    wlan_init_cmd_node(pmpriv, pcmd_node, cmd_oid, pioctl_buf, pdata_buf);

    if (pcmd_node->cmdbuf == MNULL) {
        PRINTM(MERROR, "PREP_CMD: No free cmd buf\n");
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    cmd_ptr =
        (HostCmd_DS_COMMAND *) (pcmd_node->cmdbuf->pbuf +
                                pcmd_node->cmdbuf->data_offset);
    cmd_ptr->command = cmd_no;
    cmd_ptr->result = 0;

    /* Prepare command */
    if (cmd_no)
        ret =
            pmpriv->ops.prepare_cmd(pmpriv, cmd_no, cmd_action, cmd_oid,
                                    pioctl_buf, pdata_buf, cmd_ptr);
    else {
        ret = wlan_cmd_host_cmd(pmpriv, cmd_ptr, pdata_buf);
        pcmd_node->cmd_flag |= CMD_F_HOSTCMD;
    }

    /* Return error, since the command preparation failed */
    if (ret != MLAN_STATUS_SUCCESS) {
        PRINTM(MERROR, "PREP_CMD: Command 0x%x preparation failed\n", cmd_no);
        wlan_insert_cmd_to_free_q(pmadapter, pcmd_node);
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    /* Send command */
#ifdef STA_SUPPORT
    if (cmd_no == HostCmd_CMD_802_11_SCAN)
        wlan_queue_scan_cmd(pmpriv, pcmd_node);
    else
#endif
        wlan_insert_cmd_to_pending_q(pmadapter, pcmd_node, MTRUE);
  done:
    LEAVE();
    return ret;
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
    util_enqueue_list_tail(pmadapter->pmoal_handle, &pmadapter->cmd_free_q,
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
    HostCmd_DS_COMMAND *pcmd = MNULL;
    t_u16 command;

    ENTER();

    if (pcmd_node == MNULL) {
        PRINTM(MERROR, "QUEUE_CMD: pcmd_node is MNULL\n");
        goto done;
    }

    pcmd =
        (HostCmd_DS_COMMAND *) (pcmd_node->cmdbuf->pbuf +
                                pcmd_node->cmdbuf->data_offset);
    if (pcmd == MNULL) {
        PRINTM(MERROR, "QUEUE_CMD: pcmd is MNULL\n");
        goto done;
    }

    command = wlan_le16_to_cpu(pcmd->command);

/* Exit_PS command needs to be queued in the header always. */
    if (command == HostCmd_CMD_802_11_PS_MODE_ENH) {
        HostCmd_DS_802_11_PS_MODE_ENH *pm = &pcmd->params.psmode_enh;
        if ((wlan_le16_to_cpu(pm->action) == DIS_PS) ||
            (wlan_le16_to_cpu(pm->action) == DIS_AUTO_PS)) {
            if (pmadapter->ps_state != PS_STATE_AWAKE)
                add_tail = MFALSE;
        }
    }

    if (add_tail) {
        util_enqueue_list_tail(pmadapter->pmoal_handle,
                               &pmadapter->cmd_pending_q,
                               (pmlan_linked_list) pcmd_node,
                               pmadapter->callbacks.moal_spin_lock,
                               pmadapter->callbacks.moal_spin_unlock);
    } else {
        util_enqueue_list_head(pmadapter->pmoal_handle,
                               &pmadapter->cmd_pending_q,
                               (pmlan_linked_list) pcmd_node,
                               pmadapter->callbacks.moal_spin_lock,
                               pmadapter->callbacks.moal_spin_unlock);
    }

    PRINTM(MCMND, "QUEUE_CMD: cmd=0x%x is queued\n", command);

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
        PRINTM(MERROR, "EXEC_NEXT_CMD: pmadapter is MNULL\n");
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }
    /* Check if already in processing */
    if (pmadapter->curr_cmd) {
        PRINTM(MERROR, "EXEC_NEXT_CMD: there is command in processing!\n");
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    wlan_request_cmd_lock(pmadapter);
    /* Check if any command is pending */
    pcmd_node =
        (cmd_ctrl_node *) util_peek_list(pmadapter->pmoal_handle,
                                         &pmadapter->cmd_pending_q,
                                         pmadapter->callbacks.moal_spin_lock,
                                         pmadapter->callbacks.moal_spin_unlock);

    if (pcmd_node) {
        pcmd =
            (HostCmd_DS_COMMAND *) (pcmd_node->cmdbuf->pbuf +
                                    pcmd_node->cmdbuf->data_offset);
        priv = pcmd_node->priv;

        if (pmadapter->ps_state != PS_STATE_AWAKE) {
            PRINTM(MERROR,
                   "Cannot send command in sleep state, this should not happen\n");
            wlan_release_cmd_lock(pmadapter);
            goto done;
        }

        util_unlink_list(pmadapter->pmoal_handle,
                         &pmadapter->cmd_pending_q,
                         (pmlan_linked_list) pcmd_node,
                         pmadapter->callbacks.moal_spin_lock,
                         pmadapter->callbacks.moal_spin_unlock);
        wlan_release_cmd_lock(pmadapter);
        ret = wlan_dnld_cmd_to_fw(priv, pcmd_node);
        priv = wlan_get_priv(pmadapter, MLAN_BSS_TYPE_ANY);
        /* Any command sent to the firmware when host is in sleep mode, should
           de-configure host sleep */
        /* We should skip the host sleep configuration command itself though */
        if (priv &&
            (pcmd->command !=
             wlan_cpu_to_le16(HostCmd_CMD_802_11_HS_CFG_ENH))) {
            if (pmadapter->hs_activated == MTRUE) {
                pmadapter->is_hs_configured = MFALSE;
                wlan_host_sleep_activated_event(priv, MFALSE);
            }
        }
        goto done;
    } else {
        wlan_release_cmd_lock(pmadapter);
    }
    ret = MLAN_STATUS_SUCCESS;
  done:
    LEAVE();
    return ret;
}

/** 
 *  @brief This function handles the command response
 *  
 *  @param pmadapter    A pointer to mlan_adapter structure
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_process_cmdresp(mlan_adapter * pmadapter)
{
    HostCmd_DS_COMMAND *resp = MNULL;
    mlan_private *pmpriv = wlan_get_priv(pmadapter, MLAN_BSS_TYPE_ANY);
    mlan_status ret = MLAN_STATUS_SUCCESS;
    t_u16 orig_cmdresp_no;
    t_u16 cmdresp_no;
    t_u16 cmdresp_result;
    mlan_ioctl_req *pioctl_buf = MNULL;
    mlan_callbacks *pcb = (mlan_callbacks *) & pmadapter->callbacks;
    t_u32 sec, usec;

    ENTER();

    /* Now we got response from FW, cancel the command timer */
    if (pmadapter->cmd_timer_is_set) {
        /* Cancel command timeout timer */
        pcb->moal_stop_timer(pmadapter->pmoal_handle,
                             pmadapter->pmlan_cmd_timer);
        /* Cancel command timeout timer */
        pmadapter->cmd_timer_is_set = MFALSE;
    }

    if (!pmadapter->curr_cmd || !pmadapter->curr_cmd->respbuf) {
        resp = (HostCmd_DS_COMMAND *) pmadapter->upld_buf;
        resp->command = wlan_le16_to_cpu(resp->command);
        PRINTM(MERROR, "CMD_RESP: No curr_cmd, 0x%x\n", resp->command);
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    if (pmadapter->curr_cmd->pioctl_buf != MNULL) {
        pioctl_buf = (mlan_ioctl_req *) pmadapter->curr_cmd->pioctl_buf;
    }
    pmadapter->num_cmd_timeout = 0;

    DBG_HEXDUMP(MCMD_D, "CMD_RESP",
                pmadapter->curr_cmd->respbuf->pbuf +
                pmadapter->curr_cmd->respbuf->data_offset, pmadapter->upld_len);

    resp =
        (HostCmd_DS_COMMAND *) (pmadapter->curr_cmd->respbuf->pbuf +
                                pmadapter->curr_cmd->respbuf->data_offset);
    if (pmadapter->curr_cmd->cmd_flag & CMD_F_CANCELED) {
        PRINTM(MERROR, "CMD_RESP: 0x%x been canceled!",
               wlan_le16_to_cpu(resp->command));
        wlan_insert_cmd_to_free_q(pmadapter, pmadapter->curr_cmd);
        wlan_request_cmd_lock(pmadapter);
        pmadapter->curr_cmd = MNULL;
        wlan_release_cmd_lock(pmadapter);
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }
    if (pmadapter->curr_cmd->cmd_flag & CMD_F_HOSTCMD) {
        /* Copy original response back to response buffer */
        wlan_ret_host_cmd(pmpriv, resp, pioctl_buf);
    }
    orig_cmdresp_no = wlan_le16_to_cpu(resp->command);
    resp->size = wlan_le16_to_cpu(resp->size);
    resp->seq_num = wlan_le16_to_cpu(resp->seq_num);
    resp->result = wlan_le16_to_cpu(resp->result);

    /* Get BSS number and corresponding priv */
    pmpriv =
        wlan_get_priv_by_id(pmadapter, HostCmd_GET_BSS_NO(resp->seq_num),
                            HostCmd_GET_BSS_TYPE(resp->seq_num));
    if (!pmpriv)
        pmpriv = wlan_get_priv(pmadapter, MLAN_BSS_TYPE_ANY);
    /* Clear RET_BIT from HostCmd */
    resp->command = (orig_cmdresp_no & HostCmd_CMD_ID_MASK);
    cmdresp_no = resp->command;

    cmdresp_result = resp->result;

    /* Save the last command response to debug log */
    pmadapter->dbg.last_cmd_resp_index =
        (pmadapter->dbg.last_cmd_resp_index + 1) % DBG_CMD_NUM;
    pmadapter->dbg.last_cmd_resp_id[pmadapter->dbg.last_cmd_resp_index] =
        orig_cmdresp_no;

    pmadapter->callbacks.moal_get_system_time(pmadapter->pmoal_handle, &sec,
                                              &usec);
    PRINTM(MCMND, "CMD_RESP (%lu.%lu): 0x%x, result %d, len %d, seqno 0x%x\n",
           sec, usec, orig_cmdresp_no, cmdresp_result, resp->size,
           resp->seq_num);

    if (!(orig_cmdresp_no & HostCmd_RET_BIT)) {
        PRINTM(MERROR, "CMD_RESP: Invalid response to command!");
        if (pioctl_buf) {
            pioctl_buf->status_code = MLAN_ERROR_FW_CMDRESP;
        }
        wlan_insert_cmd_to_free_q(pmadapter, pmadapter->curr_cmd);
        wlan_request_cmd_lock(pmadapter);
        pmadapter->curr_cmd = MNULL;
        wlan_release_cmd_lock(pmadapter);
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    if (pmadapter->curr_cmd->cmd_flag & CMD_F_HOSTCMD) {
        pmadapter->curr_cmd->cmd_flag &= ~CMD_F_HOSTCMD;
        if ((cmdresp_result == HostCmd_RESULT_OK)
            && (cmdresp_no == HostCmd_CMD_802_11_HS_CFG_ENH))
            ret = wlan_ret_802_11_hs_cfg(pmpriv, resp, pioctl_buf);
    } else {
        /* handle response */
        ret = pmpriv->ops.process_cmdresp(pmpriv, cmdresp_no, resp, pioctl_buf);
    }

    if ((pmadapter->hw_status == WlanHardwareStatusInitializing) &&
        (pmadapter->last_init_cmd == cmdresp_no)) {
        if (cmdresp_result == HostCmd_RESULT_OK)
            pmadapter->hw_status = WlanHardwareStatusInitdone;
        else
            wlan_init_fw_complete(pmadapter);
    }

    if (pmadapter->curr_cmd) {
        pioctl_buf = (mlan_ioctl_req *) pmadapter->curr_cmd->pioctl_buf;
        if (pioctl_buf && (ret == MLAN_STATUS_SUCCESS))
            pioctl_buf->status_code = MLAN_ERROR_NO_ERROR;

        /* Clean up and put current command back to cmd_free_q */
        wlan_insert_cmd_to_free_q(pmadapter, pmadapter->curr_cmd);

        wlan_request_cmd_lock(pmadapter);
        pmadapter->curr_cmd = MNULL;
        wlan_release_cmd_lock(pmadapter);
    }

  done:
    LEAVE();
    return ret;
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
    mlan_ioctl_req *pioctl_buf = MNULL;
    t_u32 sec, usec;
    t_u8 i;
    ENTER();

    pmadapter->cmd_timer_is_set = MFALSE;
    pmadapter->num_cmd_timeout++;
    pmadapter->dbg.num_cmd_timeout++;
    if (!pmadapter->curr_cmd) {
        PRINTM(MWARN, "CurCmd Empty\n");
        goto exit;
    }
    pcmd_node = pmadapter->curr_cmd;
    if (pcmd_node->pioctl_buf != MNULL) {
        pioctl_buf = (mlan_ioctl_req *) pcmd_node->pioctl_buf;
        pioctl_buf->status_code = MLAN_ERROR_CMD_TIMEOUT;
    }

    if (pcmd_node) {
        pmadapter->dbg.timeout_cmd_id =
            pmadapter->dbg.last_cmd_id[pmadapter->dbg.last_cmd_index];
        pmadapter->dbg.timeout_cmd_act =
            pmadapter->dbg.last_cmd_act[pmadapter->dbg.last_cmd_index];
        pmadapter->callbacks.moal_get_system_time(pmadapter->pmoal_handle, &sec,
                                                  &usec);
        PRINTM(MERROR, "Timeout cmd id (%lu.%lu) = 0x%x, act = 0x%x \n", sec,
               usec, pmadapter->dbg.timeout_cmd_id,
               pmadapter->dbg.timeout_cmd_act);

        PRINTM(MERROR, "num_data_h2c_failure = %d\n",
               pmadapter->dbg.num_tx_host_to_card_failure);
        PRINTM(MERROR, "num_cmd_h2c_failure = %d\n",
               pmadapter->dbg.num_cmd_host_to_card_failure);

        PRINTM(MERROR, "num_cmd_timeout = %d\n",
               pmadapter->dbg.num_cmd_timeout);
        PRINTM(MERROR, "num_tx_timeout = %d\n", pmadapter->dbg.num_tx_timeout);

        PRINTM(MERROR, "last_cmd_index = %d\n", pmadapter->dbg.last_cmd_index);
        PRINTM(MERROR, "last_cmd_id = ");
        for (i = 0; i < DBG_CMD_NUM; i++) {
            PRINTM(MERROR, "0x%x ", pmadapter->dbg.last_cmd_id[i]);
        }
        PRINTM(MMSG, "\n");
        PRINTM(MERROR, "last_cmd_act = ");
        for (i = 0; i < DBG_CMD_NUM; i++) {
            PRINTM(MERROR, "0x%x ", pmadapter->dbg.last_cmd_act[i]);
        }
        PRINTM(MMSG, "\n");
        PRINTM(MERROR, "last_cmd_resp_index = %d\n",
               pmadapter->dbg.last_cmd_resp_index);
        PRINTM(MERROR, "last_cmd_resp_id = ");
        for (i = 0; i < DBG_CMD_NUM; i++) {
            PRINTM(MERROR, "0x%x ", pmadapter->dbg.last_cmd_resp_id[i]);
        }
        PRINTM(MMSG, "\n");

        PRINTM(MERROR, "last_event_index = %d\n",
               pmadapter->dbg.last_event_index);
        PRINTM(MERROR, "last_event = ");
        for (i = 0; i < DBG_CMD_NUM; i++) {
            PRINTM(MERROR, "0x%x ", pmadapter->dbg.last_event[i]);
        }
        PRINTM(MMSG, "\n");

        PRINTM(MERROR, "data_sent=%d cmd_sent=%d\n", pmadapter->data_sent,
               pmadapter->cmd_sent);

        PRINTM(MERROR, "ps_mode=%d ps_state=%d\n", pmadapter->ps_mode,
               pmadapter->ps_state);
    }
    if (pmadapter->hw_status == WlanHardwareStatusInitializing)
        wlan_init_fw_complete(pmadapter);
  exit:
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
            (cmd_ctrl_node *) util_peek_list(pmadapter->pmoal_handle,
                                             &pmadapter->cmd_pending_q,
                                             pcb->moal_spin_lock,
                                             pcb->moal_spin_unlock))) {
        util_unlink_list(pmadapter->pmoal_handle, &pmadapter->cmd_pending_q,
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
#ifdef STA_SUPPORT
    /* Cancel all pending scan command */
    while ((pcmd_node =
            (cmd_ctrl_node *) util_peek_list(pmadapter->pmoal_handle,
                                             &pmadapter->scan_pending_q,
                                             pcb->moal_spin_lock,
                                             pcb->moal_spin_unlock))) {
        util_unlink_list(pmadapter->pmoal_handle, &pmadapter->scan_pending_q,
                         (pmlan_linked_list) pcmd_node, pcb->moal_spin_lock,
                         pcb->moal_spin_unlock);
        pcmd_node->pioctl_buf = MNULL;
        wlan_insert_cmd_to_free_q(pmadapter, pcmd_node);
    }
    wlan_request_cmd_lock(pmadapter);
    pmadapter->scan_processing = MFALSE;
    wlan_release_cmd_lock(pmadapter);
#endif
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

    PRINTM(MIOCTL, "MOAL Cancel IOCTL: 0x%x sub_id=0x%x action=%d\n",
           pioctl_req->req_id, *((t_u32 *) pioctl_req->pbuf),
           (int) pioctl_req->action);

    if ((pmadapter->curr_cmd) &&
        (pmadapter->curr_cmd->pioctl_buf == pioctl_req)) {
        wlan_request_cmd_lock(pmadapter);
        pcmd_node = pmadapter->curr_cmd;
        pcmd_node->pioctl_buf = MNULL;
        pcmd_node->cmd_flag |= CMD_F_CANCELED;
        wlan_release_cmd_lock(pmadapter);
    }

    wlan_request_cmd_lock(pmadapter);
    while ((pcmd_node =
            wlan_get_pending_ioctl_cmd(pmadapter, pioctl_req)) != MNULL) {
        util_unlink_list(pmadapter->pmoal_handle, &pmadapter->cmd_pending_q,
                         (pmlan_linked_list) pcmd_node,
                         pmadapter->callbacks.moal_spin_lock,
                         pmadapter->callbacks.moal_spin_unlock);
        pcmd_node->pioctl_buf = MNULL;
        wlan_insert_cmd_to_free_q(pmadapter, pcmd_node);
    }
    wlan_release_cmd_lock(pmadapter);
#ifdef STA_SUPPORT
    if (pioctl_req->req_id == MLAN_IOCTL_SCAN) {
        /* Cancel all pending scan command */
        while ((pcmd_node =
                (cmd_ctrl_node *) util_peek_list(pmadapter->pmoal_handle,
                                                 &pmadapter->scan_pending_q,
                                                 pcb->moal_spin_lock,
                                                 pcb->moal_spin_unlock))) {
            util_unlink_list(pmadapter->pmoal_handle,
                             &pmadapter->scan_pending_q,
                             (pmlan_linked_list) pcmd_node, pcb->moal_spin_lock,
                             pcb->moal_spin_unlock);
            pcmd_node->pioctl_buf = MNULL;
            wlan_insert_cmd_to_free_q(pmadapter, pcmd_node);
        }
        wlan_request_cmd_lock(pmadapter);
        pmadapter->scan_processing = MFALSE;
        wlan_release_cmd_lock(pmadapter);
    }
#endif
    pioctl_req->status_code = MLAN_ERROR_CMD_CANCEL;
    pcb->moal_ioctl_complete(pmadapter->pmoal_handle, pioctl_req,
                             MLAN_STATUS_FAILURE);

    LEAVE();
    return;
}

/**
 *  @brief This function checks conditions and prepares to
 *              send sleep confirm command to firmware if OK.
 * 
 *  @param pmadapter    A pointer to mlan_adapter structure
 *
 *  @return             N/A
 */
t_void
wlan_check_ps_cond(mlan_adapter * pmadapter)
{
    ENTER();

    if (!pmadapter->cmd_sent &&
        !pmadapter->curr_cmd && !IS_CARD_RX_RCVD(pmadapter)) {
        wlan_dnld_sleep_confirm_cmd(pmadapter);
    } else {
        PRINTM(MCMND, "Delay Sleep Confirm (%s%s%s)\n",
               (pmadapter->cmd_sent) ? "D" : "",
               (pmadapter->curr_cmd) ? "C" : "",
               (IS_CARD_RX_RCVD(pmadapter)) ? "R" : "");
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
            PRINTM(MEVENT, "hs_activated\n");
            wlan_recv_event(priv, MLAN_EVENT_ID_DRV_HS_ACTIVATED, MNULL);
        } else
            PRINTM(MWARN, "hs_activated: HS not configured !!!\n");
    } else {
        PRINTM(MEVENT, "hs_deactived\n");
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
 *  @brief This function handles the command response of hs_cfg
 * 
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status
wlan_ret_802_11_hs_cfg(IN pmlan_private pmpriv,
                       IN HostCmd_DS_COMMAND * resp,
                       IN mlan_ioctl_req * pioctl_buf)
{
    pmlan_adapter pmadapter = pmpriv->adapter;
    HostCmd_DS_802_11_HS_CFG_ENH *phs_cfg = &resp->params.opt_hs_cfg;
    ENTER();

    if (phs_cfg->action == HS_ACTIVATE) {
        wlan_host_sleep_activated_event(pmpriv, MTRUE);
        goto done;
    } else {
        phs_cfg->params.hs_config.conditions =
            wlan_le32_to_cpu(phs_cfg->params.hs_config.conditions);
        PRINTM(MCMND,
               "CMD_RESP: HS_CFG cmd reply result=%#x,"
               " conditions=0x%x gpio=0x%x gap=0x%x\n", resp->result,
               phs_cfg->params.hs_config.conditions,
               phs_cfg->params.hs_config.gpio, phs_cfg->params.hs_config.gap);
    }
    if (phs_cfg->params.hs_config.conditions != HOST_SLEEP_CFG_CANCEL) {
        pmadapter->is_hs_configured = MTRUE;
    } else {
        pmadapter->is_hs_configured = MFALSE;
        if (pmadapter->hs_activated)
            wlan_host_sleep_activated_event(pmpriv, MFALSE);
    }
  done:
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Perform hs related activities on receiving the power up interrupt
 *  
 *  @param pmadapter  A pointer to the adapter structure
 *  @return           N/A
 */
t_void
wlan_process_hs_config(pmlan_adapter pmadapter)
{
    PRINTM(MINFO,
           "Auto canceling host sleep since there is some interrupt from the firmware\n");
    wlan_pm_wakeup_card(pmadapter);
    pmadapter->hs_activated = MFALSE;
    pmadapter->is_hs_configured = MFALSE;
    wlan_host_sleep_activated_event(wlan_get_priv(pmadapter, MLAN_BSS_TYPE_ANY),
                                    MFALSE);
    return;
}

/** 
 *  @brief Check sleep confirm command response and set the state to ASLEEP
 *  
 *  @param pmadapter  A pointer to the adapter structure
 *  @param pbuf       A pointer to the command response buffer
 *  @param upld_len   Command response buffer length
 *  @return           N/A
 */
void
wlan_process_sleep_confirm_resp(pmlan_adapter pmadapter, t_u8 * pbuf,
                                t_u32 upld_len)
{
    HostCmd_DS_COMMAND *cmd;
    pmlan_private pmpriv = wlan_get_priv(pmadapter, MLAN_BSS_TYPE_ANY);

    ENTER();

    if (!upld_len) {
        PRINTM(MERROR, "Command size is 0\n");
        LEAVE();
        return;
    }
    cmd = (HostCmd_DS_COMMAND *) pbuf;
    cmd->result = wlan_le16_to_cpu(cmd->result);
    cmd->command = wlan_le16_to_cpu(cmd->command);
    cmd->seq_num = wlan_le16_to_cpu(cmd->seq_num);

    /* Get BSS number and corresponding priv */
    pmpriv =
        wlan_get_priv_by_id(pmadapter, HostCmd_GET_BSS_NO(cmd->seq_num),
                            HostCmd_GET_BSS_TYPE(cmd->seq_num));
    if (!pmpriv)
        pmpriv = wlan_get_priv(pmadapter, MLAN_BSS_TYPE_ANY);
    /* Update sequence number */
    cmd->seq_num = HostCmd_GET_SEQ_NO(cmd->seq_num);
    /* Clear RET_BIT from HostCmd */
    cmd->command &= HostCmd_CMD_ID_MASK;

    if (cmd->command != HostCmd_CMD_802_11_PS_MODE_ENH) {
        PRINTM(MERROR,
               "Received unexpected response for command %x, result = %x\n",
               cmd->command, cmd->result);
        return;
    }
    PRINTM(MEVENT, "#\n");
    if (cmd->result != MLAN_STATUS_SUCCESS) {
        PRINTM(MERROR, "Sleep confirm command failed\n");
        LEAVE();
        return;
    }
    pmadapter->pm_wakeup_card_req = MTRUE;

    if (pmadapter->is_hs_configured) {
        wlan_host_sleep_activated_event(wlan_get_priv
                                        (pmadapter, MLAN_BSS_TYPE_ANY), MTRUE);
    }
    pmadapter->ps_state = PS_STATE_SLEEP;
    LEAVE();
}

/**
 *  @brief This function prepares command of power mode
 *  
 *  @param pmpriv		A pointer to mlan_private structure
 *  @param cmd	   		A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   the action: GET or SET
 *  @param ps_bitmap    PS bitmap
 *  @param pdata_buf    A pointer to data buffer
 *  @return         MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_cmd_enh_power_mode(pmlan_private pmpriv,
                        IN HostCmd_DS_COMMAND * cmd,
                        IN t_u16 cmd_action,
                        IN t_u16 ps_bitmap, IN t_void * pdata_buf)
{
    HostCmd_DS_802_11_PS_MODE_ENH *psmode_enh = &cmd->params.psmode_enh;
    t_u8 *tlv = MNULL;
    t_u16 cmd_size = 0;

    PRINTM(MCMND, "PS Command: action = 0x%x, bitmap = 0x%x\n", cmd_action,
           ps_bitmap);

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_802_11_PS_MODE_ENH);
    if (cmd_action == DIS_AUTO_PS) {
        psmode_enh->action = wlan_cpu_to_le16(DIS_AUTO_PS);
        psmode_enh->params.ps_bitmap = wlan_cpu_to_le16(ps_bitmap);
        cmd->size = wlan_cpu_to_le16(S_DS_GEN + AUTO_PS_FIX_SIZE);
    } else if (cmd_action == GET_PS) {
        psmode_enh->action = wlan_cpu_to_le16(GET_PS);
        psmode_enh->params.ps_bitmap = wlan_cpu_to_le16(ps_bitmap);
        cmd->size = wlan_cpu_to_le16(S_DS_GEN + AUTO_PS_FIX_SIZE);
    } else if (cmd_action == EN_AUTO_PS) {
        psmode_enh->action = wlan_cpu_to_le16(EN_AUTO_PS);
        psmode_enh->params.auto_ps.ps_bitmap = wlan_cpu_to_le16(ps_bitmap);
        cmd_size = S_DS_GEN + AUTO_PS_FIX_SIZE;
        tlv = (t_u8 *) cmd + cmd_size;
        if (ps_bitmap & BITMAP_STA_PS) {
            pmlan_adapter pmadapter = pmpriv->adapter;
            MrvlIEtypes_ps_param_t *ps_tlv = (MrvlIEtypes_ps_param_t *) tlv;
            ps_param *ps_mode = &ps_tlv->param;
            ps_tlv->header.type = wlan_cpu_to_le16(TLV_TYPE_PS_PARAM);
            ps_tlv->header.len =
                wlan_cpu_to_le16(sizeof(MrvlIEtypes_ps_param_t) -
                                 sizeof(MrvlIEtypesHeader_t));
            cmd_size += sizeof(MrvlIEtypes_ps_param_t);
            tlv += sizeof(MrvlIEtypes_ps_param_t);
            ps_mode->null_pkt_interval =
                wlan_cpu_to_le16(pmadapter->null_pkt_interval);
            ps_mode->multiple_dtims =
                wlan_cpu_to_le16(pmadapter->multiple_dtim);
            ps_mode->bcn_miss_timeout =
                wlan_cpu_to_le16(pmadapter->bcn_miss_time_out);
            ps_mode->local_listen_interval =
                wlan_cpu_to_le16(pmadapter->local_listen_interval);
            ps_mode->adhoc_wake_period =
                wlan_cpu_to_le16(pmadapter->adhoc_awake_period);
            ps_mode->delay_to_ps = wlan_cpu_to_le16(pmadapter->delay_to_ps);
            ps_mode->mode = wlan_cpu_to_le16(pmadapter->enhanced_ps_mode);
        }
        if (ps_bitmap & BITMAP_AUTO_DS) {
            MrvlIEtypes_auto_ds_param_t *auto_ps_tlv =
                (MrvlIEtypes_auto_ds_param_t *) tlv;
            auto_ds_param *auto_ds = &auto_ps_tlv->param;
            t_u16 idletime = 0;
            auto_ps_tlv->header.type = wlan_cpu_to_le16(TLV_TYPE_AUTO_DS_PARAM);
            auto_ps_tlv->header.len =
                wlan_cpu_to_le16(sizeof(MrvlIEtypes_auto_ds_param_t) -
                                 sizeof(MrvlIEtypesHeader_t));
            cmd_size += sizeof(MrvlIEtypes_auto_ds_param_t);
            tlv += sizeof(MrvlIEtypes_auto_ds_param_t);
            if (pdata_buf)
                idletime = ((mlan_ds_auto_ds *) pdata_buf)->idletime;
            auto_ds->deep_sleep_timeout = wlan_cpu_to_le16(idletime);
        }
#if defined(UAP_SUPPORT)
        if (ps_bitmap & (BITMAP_UAP_INACT_PS | BITMAP_UAP_DTIM_PS)) {
            mlan_ds_ps_mgmt *ps_mgmt = (mlan_ds_ps_mgmt *) pdata_buf;
            MrvlIEtypes_sleep_param_t *sleep_tlv = MNULL;
            MrvlIEtypes_inact_sleep_param_t *inact_tlv = MNULL;
            if (ps_mgmt->flags & PS_FLAG_SLEEP_PARAM) {
                sleep_tlv = (MrvlIEtypes_sleep_param_t *) tlv;
                sleep_tlv->header.type =
                    wlan_cpu_to_le16(TLV_TYPE_AP_SLEEP_PARAM);
                sleep_tlv->header.len =
                    wlan_cpu_to_le16(sizeof(MrvlIEtypes_sleep_param_t) -
                                     sizeof(MrvlIEtypesHeader_t));
                sleep_tlv->ctrl_bitmap =
                    wlan_cpu_to_le32(ps_mgmt->sleep_param.ctrl_bitmap);
                sleep_tlv->min_sleep =
                    wlan_cpu_to_le32(ps_mgmt->sleep_param.min_sleep);
                sleep_tlv->max_sleep =
                    wlan_cpu_to_le32(ps_mgmt->sleep_param.max_sleep);
                cmd_size += sizeof(MrvlIEtypes_sleep_param_t);
                tlv += sizeof(MrvlIEtypes_sleep_param_t);
            }
            if (ps_mgmt->flags & PS_FLAG_INACT_SLEEP_PARAM) {
                inact_tlv = (MrvlIEtypes_inact_sleep_param_t *) tlv;
                inact_tlv->header.type =
                    wlan_cpu_to_le16(TLV_TYPE_AP_INACT_SLEEP_PARAM);
                inact_tlv->header.len =
                    wlan_cpu_to_le16(sizeof(MrvlIEtypes_inact_sleep_param_t)
                                     - sizeof(MrvlIEtypesHeader_t));
                inact_tlv->inactivity_to =
                    wlan_cpu_to_le32(ps_mgmt->inact_param.inactivity_to);
                inact_tlv->min_awake =
                    wlan_cpu_to_le32(ps_mgmt->inact_param.min_awake);
                inact_tlv->max_awake =
                    wlan_cpu_to_le32(ps_mgmt->inact_param.max_awake);
                cmd_size += sizeof(MrvlIEtypes_inact_sleep_param_t);
                tlv += sizeof(MrvlIEtypes_inact_sleep_param_t);
            }
        }
#endif
        cmd->size = wlan_cpu_to_le16(cmd_size);
    }
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function handles the command response of ps_mode_enh
 * 
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status
wlan_ret_enh_power_mode(IN pmlan_private pmpriv,
                        IN HostCmd_DS_COMMAND * resp,
                        IN mlan_ioctl_req * pioctl_buf)
{
    pmlan_adapter pmadapter = pmpriv->adapter;
    HostCmd_DS_802_11_PS_MODE_ENH *ps_mode = &resp->params.psmode_enh;
    ps_mode->action = wlan_le16_to_cpu(ps_mode->action);
    PRINTM(MINFO, "CMD_RESP: PS_MODE cmd reply result=%#x action=0x%X\n",
           resp->result, ps_mode->action);
    if (ps_mode->action == EN_AUTO_PS) {
        ps_mode->params.auto_ps.ps_bitmap =
            wlan_le16_to_cpu(ps_mode->params.auto_ps.ps_bitmap);
        if (ps_mode->params.auto_ps.ps_bitmap & BITMAP_AUTO_DS) {
            PRINTM(MCMND, "Enabled auto deep sleep\n");
            pmpriv->adapter->is_deep_sleep = MTRUE;
        }
        if (ps_mode->params.auto_ps.ps_bitmap & BITMAP_STA_PS) {
            PRINTM(MCMND, "Enabled STA power save\n");
            if (pmadapter->sleep_period.period) {
                PRINTM(MCMND, "Setting uapsd/pps mode to TRUE\n");
            }
        }
#if defined(UAP_SUPPORT)
        if (ps_mode->params.auto_ps.
            ps_bitmap & (BITMAP_UAP_INACT_PS | BITMAP_UAP_DTIM_PS)) {
            pmadapter->ps_mode = Wlan802_11PowerModePSP;
            PRINTM(MCMND, "Enabled uAP power save\n");
        }
#endif
    } else if (ps_mode->action == DIS_AUTO_PS) {
        ps_mode->params.ps_bitmap = wlan_cpu_to_le16(ps_mode->params.ps_bitmap);
        if (ps_mode->params.ps_bitmap & BITMAP_AUTO_DS) {
            pmpriv->adapter->is_deep_sleep = MFALSE;
            PRINTM(MCMND, "Disabled auto deep sleep\n");
        }
        if (ps_mode->params.ps_bitmap & BITMAP_STA_PS) {
            PRINTM(MCMND, "Disabled STA power save\n");
            if (pmadapter->sleep_period.period) {
                pmadapter->delay_null_pkt = MFALSE;
                pmadapter->tx_lock_flag = MFALSE;
                pmadapter->pps_uapsd_mode = MFALSE;
            }
        }
#if defined(UAP_SUPPORT)
        if (ps_mode->params.
            ps_bitmap & (BITMAP_UAP_INACT_PS | BITMAP_UAP_DTIM_PS)) {
            pmadapter->ps_mode = Wlan802_11PowerModeCAM;
            PRINTM(MCMND, "Disabled uAP power save\n");
        }
#endif
    }
#if defined(UAP_SUPPORT)
    else if ((ps_mode->action == GET_PS) && pioctl_buf) {
        mlan_ds_pm_cfg *pm_cfg = (mlan_ds_pm_cfg *) pioctl_buf->pbuf;
        MrvlIEtypes_sleep_param_t *sleep_tlv = MNULL;
        MrvlIEtypes_inact_sleep_param_t *inact_tlv = MNULL;
        MrvlIEtypesHeader_t *tlv = MNULL;
        t_u16 tlv_type = 0;
        t_u16 tlv_len = 0;
        t_u16 tlv_buf_left = 0;
        if (pm_cfg->sub_command == MLAN_OID_PM_CFG_PS_MODE) {
            ps_mode->params.ps_bitmap =
                wlan_le16_to_cpu(ps_mode->params.ps_bitmap);
            pm_cfg->param.ps_mgmt.flags = PS_FLAG_PS_MODE;
            if (ps_mode->params.ps_bitmap & BITMAP_UAP_INACT_PS)
                pm_cfg->param.ps_mgmt.ps_mode = PS_MODE_INACTIVITY;
            else if (ps_mode->params.ps_bitmap & BITMAP_UAP_DTIM_PS)
                pm_cfg->param.ps_mgmt.ps_mode = PS_MODE_PERIODIC_DTIM;
            else
                pm_cfg->param.ps_mgmt.ps_mode = PS_MODE_DISABLE;
            tlv_buf_left = resp->size - (S_DS_GEN + AUTO_PS_FIX_SIZE);
            tlv = (MrvlIEtypesHeader_t *) ((t_u8 *) ps_mode + AUTO_PS_FIX_SIZE);
            while (tlv_buf_left >= sizeof(MrvlIEtypesHeader_t)) {
                tlv_type = wlan_le16_to_cpu(tlv->type);
                tlv_len = wlan_le16_to_cpu(tlv->len);
                switch (tlv_type) {
                case TLV_TYPE_AP_SLEEP_PARAM:
                    sleep_tlv = (MrvlIEtypes_sleep_param_t *) tlv;
                    pm_cfg->param.ps_mgmt.flags |= PS_FLAG_SLEEP_PARAM;
                    pm_cfg->param.ps_mgmt.sleep_param.ctrl_bitmap =
                        wlan_le32_to_cpu(sleep_tlv->ctrl_bitmap);
                    pm_cfg->param.ps_mgmt.sleep_param.min_sleep =
                        wlan_le32_to_cpu(sleep_tlv->min_sleep);
                    pm_cfg->param.ps_mgmt.sleep_param.max_sleep =
                        wlan_le32_to_cpu(sleep_tlv->max_sleep);
                    break;
                case TLV_TYPE_AP_INACT_SLEEP_PARAM:
                    inact_tlv = (MrvlIEtypes_inact_sleep_param_t *) tlv;
                    pm_cfg->param.ps_mgmt.flags |= PS_FLAG_INACT_SLEEP_PARAM;
                    pm_cfg->param.ps_mgmt.inact_param.inactivity_to =
                        wlan_le32_to_cpu(inact_tlv->inactivity_to);
                    pm_cfg->param.ps_mgmt.inact_param.min_awake =
                        wlan_le32_to_cpu(inact_tlv->min_awake);
                    pm_cfg->param.ps_mgmt.inact_param.max_awake =
                        wlan_le32_to_cpu(inact_tlv->max_awake);
                    break;
                }
                tlv_buf_left -= tlv_len + sizeof(MrvlIEtypesHeader_t);
                tlv =
                    (MrvlIEtypesHeader_t *) ((t_u8 *) tlv + tlv_len +
                                             sizeof(MrvlIEtypesHeader_t));
            }
        }
    }
#endif
    return MLAN_STATUS_SUCCESS;
}
