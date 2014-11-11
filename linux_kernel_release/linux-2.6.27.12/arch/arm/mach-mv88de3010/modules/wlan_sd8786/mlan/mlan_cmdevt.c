/**
 * @file mlan_cmdevt.c
 *
 *  @brief This file contains the handling of CMD/EVENT in MLAN
 *
 *
 *  Copyright (C) 2009, Marvell International Ltd. 
 *  All Rights Reserved
 *   
 */

/*************************************************************
Change Log:
    05/12/2009: initial version
************************************************************/
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
    t_u8 *head_ptr = MNULL;
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
    head_ptr = (t_u8 *) ALIGN_ADDR(pcmd_node->cmdbuf->pbuf, HEADER_ALIGNMENT);
    pcmd_node->cmdbuf->data_offset = head_ptr - pcmd_node->cmdbuf->pbuf;
    memset(head_ptr, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

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

    if (util_peek_list(&pmadapter->cmd_free_q,
                       pmadapter->callbacks.moal_spin_lock,
                       pmadapter->callbacks.moal_spin_unlock)) {
        pcmd_node = (cmd_ctrl_node *) util_dequeue_list(&pmadapter->cmd_free_q,
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
        pmadapter->callbacks.moal_recv_complete(pmadapter->pmoal_handle,
                                                pcmd_node->respbuf, 0,
                                                MLAN_STATUS_SUCCESS);
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
        memcpy(misc->param.hostcmd.cmd, (void *) resp, size);
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
    memcpy((void *) cmd, pcmd_ptr->cmd, pcmd_ptr->len);
    PRINTM(MCMND, "Host command size = %d\n", pcmd_ptr->len);
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
    pcmd->seq_num = wlan_cpu_to_le16(pmadapter->seq_num);

    wlan_request_cmd_lock(pmadapter);
    pmadapter->curr_cmd = pcmd_node;
    wlan_release_cmd_lock(pmadapter);

    cmd_code = wlan_le16_to_cpu(pcmd->command);
    cmd_size = wlan_le16_to_cpu(pcmd->size);

    /* Set BSS_NO_BITS to HostCmd */
    cmd_code = HostCmd_SET_BSS_NO(cmd_code, pcmd_node->priv->bss_num);
    pcmd->command = wlan_cpu_to_le16(cmd_code);

    pcmd_node->cmdbuf->data_len = cmd_size;

    pmadapter->callbacks.moal_get_system_time(&sec, &usec);
    PRINTM(MCMND, "DNLD_CMD (%lu.%lu): 0x%x, act 0x%x, len %d, seqno %d\n",
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

    /* Setup the timer after transmit command */
    pcb->moal_start_timer(pmadapter->pmlan_cmd_timer, MFALSE, MRVDRV_TIMER_10S);

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
    HostCmd_DS_802_11_PS_MODE_ENH *pps_mode = MNULL;
    opt_sleep_confirm_buffer *sleep_cfm_buf =
        (opt_sleep_confirm_buffer *) (pmadapter->psleep_cfm->pbuf +
                                      pmadapter->psleep_cfm->data_offset);

    ENTER();

    if ((wlan_get_priv(pmadapter, MLAN_BSS_TYPE_ANY))->bss_type ==
        MLAN_BSS_TYPE_STA) {
        cmd_len = sizeof(HostCmd_DS_COMMAND);
        pps_mode = &sleep_cfm_buf->ps_cfm_sleep.params.psmode_enh;
        sleep_cfm_buf->ps_cfm_sleep.seq_num =
            wlan_cpu_to_le16(++pmadapter->seq_num);
        pps_mode->params.sleep_cfm.resp_ctrl = wlan_cpu_to_le16(RESP_NEEDED);
        DBG_HEXDUMP(MCMD_D, "SLEEP_CFM", &sleep_cfm_buf->ps_cfm_sleep, cmd_len);
    }

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
            MLAN_BSS_TYPE_STA) {
            if (!pps_mode->params.sleep_cfm.resp_ctrl) {
                /* Response is not needed for sleep confirm command */
                pmadapter->ps_state = PS_STATE_SLEEP;
            } else {
                pmadapter->ps_state = PS_STATE_SLEEP_CFM;
            }

            if (pps_mode->params.sleep_cfm.resp_ctrl != RESP_NEEDED
                && (pmadapter->is_hs_configured &&
                    !pmadapter->sleep_period.period)) {
                pmadapter->pm_wakeup_card_req = MTRUE;
                wlan_host_sleep_activated_event(wlan_get_priv
                                                (pmadapter, MLAN_BSS_TYPE_STA),
                                                MTRUE);
            }
        }
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

        memset(&mevent, 0, sizeof(mlan_event));
        mevent.bss_num = priv->bss_num;
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
    cmd_ctrl_node *pcmd_array;
    t_u32 buf_size;
    t_u32 i;

    ENTER();

    /* Allocate and initialize cmd_ctrl_node */
    buf_size = sizeof(cmd_ctrl_node) * MRVDRV_NUM_OF_CMD_BUFFER;
    ret = pcb->moal_malloc(buf_size, (t_u8 **) & pcmd_array);
    if (ret != MLAN_STATUS_SUCCESS || !pcmd_array) {
        PRINTM(MERROR, "ALLOC_CMD_BUF: Failed to allocate pcmd_array\n");
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    pmadapter->cmd_pool = pcmd_array;
    memset(pmadapter->cmd_pool, 0, buf_size);

    /* Allocate and initialize command buffers */
    for (i = 0; i < MRVDRV_NUM_OF_CMD_BUFFER; i++) {
        if (!
            (pcmd_array[i].pmbuf =
             wlan_alloc_mlan_buffer(pcb,
                                    (MRVDRV_SIZE_OF_CMD_BUFFER +
                                     HEADER_ALIGNMENT)))) {
            PRINTM(MERROR, "ALLOC_CMD_BUF: pcmd_buf: out of memory\n");
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
            PRINTM(MCMND, "Free all the command buffer.\n");
            wlan_free_mlan_buffer(pcb, pcmd_array[i].pmbuf);
            pcmd_array[i].pmbuf = MNULL;
        }
        if (pcmd_array[i].respbuf) {
            pmadapter->callbacks.moal_recv_complete(pmadapter->pmoal_handle,
                                                    pcmd_array[i].respbuf, 0,
                                                    MLAN_STATUS_SUCCESS);
            pcmd_array[i].respbuf = MNULL;
        }
    }
    /* Release cmd_ctrl_node */
    if (pmadapter->cmd_pool) {
        PRINTM(MCMND, "Free command pool.\n");
        pcb->moal_mfree((t_u8 *) pmadapter->cmd_pool);
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
    t_u32 bss_num = 0;
    t_u32 in_ts_sec;
    t_u32 in_ts_usec;
    ENTER();

    /* Save the last event to debug log */
    pmadapter->dbg.last_event_index =
        (pmadapter->dbg.last_event_index + 1) % DBG_CMD_NUM;
    pmadapter->dbg.last_event[pmadapter->dbg.last_event_index] = eventcause;

    /* Get BSS number and corresponding priv */
    bss_num = EVENT_GET_BSS_NUM((t_u16) eventcause);
    priv = pmadapter->priv[bss_num];
    if (!priv) {
        bss_num = 0;
        priv = wlan_get_priv(pmadapter, MLAN_BSS_TYPE_ANY);
    }
    /* Clear BSS_NO_BITS from event */
    eventcause &= EVENT_ID_MASK;
    if (pmbuf)
        pmbuf->bss_num = bss_num;

    if (eventcause != EVENT_PS_SLEEP && eventcause != EVENT_PS_AWAKE) {
        pmadapter->callbacks.moal_get_system_time(&in_ts_sec, &in_ts_usec);
        PRINTM(MEVENT, "%lu.%lu : Event: 0x%x\n", in_ts_sec, in_ts_usec,
               eventcause);
    }

    ret = priv->ops.process_event(priv);

    pmadapter->event_cause = 0;
    pmadapter->pmlan_buffer_event = MNULL;

    if (pmbuf) {
        pmadapter->callbacks.moal_recv_complete(pmadapter->pmoal_handle, pmbuf,
                                                0, MLAN_STATUS_SUCCESS);
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
    if (cmd_no == HostCmd_CMD_802_11_SCAN)
        wlan_queue_scan_cmd(pmpriv, pcmd_node);
    else
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
        if (wlan_le16_to_cpu(pm->action) == DIS_PS) {
            if (pmadapter->ps_state != PS_STATE_AWAKE)
                add_tail = MFALSE;
        }
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
    pcmd_node = (cmd_ctrl_node *) util_peek_list(&pmadapter->cmd_pending_q,
                                                 pmadapter->callbacks.
                                                 moal_spin_lock,
                                                 pmadapter->callbacks.
                                                 moal_spin_unlock);

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

        util_unlink_list(&pmadapter->cmd_pending_q,
                         (pmlan_linked_list) pcmd_node,
                         pmadapter->callbacks.moal_spin_lock,
                         pmadapter->callbacks.moal_spin_unlock);
        wlan_release_cmd_lock(pmadapter);
        ret = wlan_dnld_cmd_to_fw(priv, pcmd_node);
        priv = wlan_get_priv(pmadapter, MLAN_BSS_TYPE_STA);
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
        pcb->moal_stop_timer(pmadapter->pmlan_cmd_timer);
        /* Cancel command timeout timer */
        pmadapter->cmd_timer_is_set = MFALSE;
    }

    if (!pmadapter->curr_cmd || !pmadapter->curr_cmd->respbuf) {
        resp = (HostCmd_DS_COMMAND *) pmadapter->upld_buf;
        resp->command = wlan_le16_to_cpu(resp->command);
        PRINTM(MERROR, "CMD_RESP: MNULL curr_cmd, 0x%x\n", resp->command);
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
    if (pmadapter->curr_cmd->cmd_flag & CMD_F_HOSTCMD) {
        /* Copy original response back to response buffer */
        wlan_ret_host_cmd(pmpriv, resp, pioctl_buf);
    }
    resp->command = wlan_le16_to_cpu(resp->command);
    resp->size = wlan_le16_to_cpu(resp->size);
    resp->seq_num = wlan_le16_to_cpu(resp->seq_num);
    resp->result = wlan_le16_to_cpu(resp->result);

    orig_cmdresp_no = resp->command;
    cmdresp_result = resp->result;

    /* Save the last command response to debug log */
    pmadapter->dbg.last_cmd_resp_index =
        (pmadapter->dbg.last_cmd_resp_index + 1) % DBG_CMD_NUM;
    pmadapter->dbg.last_cmd_resp_id[pmadapter->dbg.last_cmd_resp_index] =
        orig_cmdresp_no;

    pmadapter->callbacks.moal_get_system_time(&sec, &usec);
    PRINTM(MCMND, "CMD_RESP (%lu.%lu): 0x%x, result %d, len %d, seqno %d\n",
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

    /* Get BSS number and corresponding priv */
    pmpriv = pmadapter->priv[HostCmd_GET_BSS_NO(resp->command)];
    if (!pmpriv)
        pmpriv = wlan_get_priv(pmadapter, MLAN_BSS_TYPE_ANY);
    /* Clear RET_BIT & BSS_NO_BITS from HostCmd */
    resp->command &= HostCmd_CMD_ID_MASK;
    cmdresp_no = resp->command;

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
        pmadapter->callbacks.moal_get_system_time(&sec, &usec);
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

    PRINTM(MCMND, "MOAL Cancel IOCTL: 0x%x sub_id=0x%x action=%d\n",
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
