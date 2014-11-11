/** @file mlan_shim.c
 *  
 *  @brief This file contains APIs to MOAL module.
 * 
 *  Copyright (C) 2008-2009, Marvell International Ltd. 
 *  All Rights Reserved
 */

/**
 *  @mainpage MLAN Driver
 *
 *  @section overview_sec Overview
 *
 *  The MLAN is an OS independent WLAN driver for Marvell 802.11
 *  embedded chipset.
 * 
 *  @section copyright_sec Copyright
 *
 *  Copyright (C) 2008-2009, Marvell International Ltd. 
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
#include "mlan_sdio.h"

/********************************************************
                Local Variables
********************************************************/
/** mlan function table */
static mlan_operations ops[] = {
    {
     /* init cmd handler */
     mlan_sta_init_cmd,
     /* ioctl handler */
     mlan_sta_ioctl,
     /* cmd handler */
     mlan_sta_prepare_cmd,
     /* cmdresp handler */
     mlan_process_sta_cmdresp,
     /* rx handler */
     mlan_process_sta_rx_packet,
     /* Event handler */
     mlan_process_sta_event,
     /* txpd handler */
     mlan_process_sta_txpd,
     /* bss type */
     MLAN_BSS_TYPE_STA,
     },
};

/********************************************************
        Global Variables
********************************************************/

/**
 *  MLAN Adapter pointer
 */
mlan_adapter *g_pmadapter = MNULL;

/********************************************************
        Local Functions
*******************************************************/

/********************************************************
        Global Functions
********************************************************/

/**
 *  @brief This function registers MOAL to MLAN module.
 *  
 *  @param pmdevice        A pointer to a mlan_device structure
 *                         allocated in MOAL
 *  @param ppmlan_adapter  A pointer to a t_void pointer to store
 *                         mlan_adapter structure pointer as the context
 *
 *  @return                MLAN_STATUS_SUCCESS
 *                             The registration succeeded.
 *                         MLAN_STATUS_FAILURE
 *                             The registration failed.
 *
 * mlan_status mlan_register (
 *   IN pmlan_device     pmdevice,
 *   OUT t_void          **ppmlan_adapter
 * );
 *
 * Comments
 *   MOAL constructs mlan_device data structure to pass moal_handle and
 *   mlan_callback table to MLAN. MLAN returns mlan_adapter pointer to
 *   the ppmlan_adapter buffer provided by MOAL.
 * Headers:
 *   declared in mlan_decl.h
 * See Also
 *   mlan_unregister
 */
mlan_status
mlan_register(IN pmlan_device pmdevice, OUT t_void ** ppmlan_adapter)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    pmlan_adapter pmadapter = MNULL;
    pmlan_callbacks pcb = MNULL;
    t_u32 i = 0;
    t_u32 j = 0;

    ASSERT(pmdevice);
    ASSERT(ppmlan_adapter);
    ASSERT(pmdevice->callbacks.moal_malloc);
    ASSERT(pmdevice->callbacks.moal_memset);
    ASSERT(pmdevice->callbacks.moal_memmove);

    /* Allocate memory for adapter structure */
    if ((pmdevice->callbacks.moal_malloc(sizeof(mlan_adapter),
                                         (t_u8 **) & pmadapter) !=
         MLAN_STATUS_SUCCESS)
        || !pmadapter) {
        ret = MLAN_STATUS_FAILURE;
        goto exit_register;
    }
    g_pmadapter = pmadapter;

    pmdevice->callbacks.moal_memset(pmadapter, 0, sizeof(mlan_adapter));

    pcb = &pmadapter->callbacks;

    /* Save callback functions */
    pmdevice->callbacks.moal_memmove(pcb,
                                     &pmdevice->callbacks,
                                     sizeof(mlan_callbacks));

    /* Assertion for all callback functions */
    ASSERT(pcb->moal_init_fw_complete);
    ASSERT(pcb->moal_shutdown_fw_complete);
    ASSERT(pcb->moal_send_packet_complete);
    ASSERT(pcb->moal_recv_complete);
    ASSERT(pcb->moal_recv_packet);
    ASSERT(pcb->moal_recv_event);
    ASSERT(pcb->moal_ioctl_complete);
    ASSERT(pcb->moal_write_reg);
    ASSERT(pcb->moal_read_reg);
    ASSERT(pcb->moal_alloc_mlan_buffer);
    ASSERT(pcb->moal_free_mlan_buffer);
    ASSERT(pcb->moal_write_data_sync);
    ASSERT(pcb->moal_read_data_sync);
    ASSERT(pcb->moal_mfree);
    ASSERT(pcb->moal_memcpy);
    ASSERT(pcb->moal_memcmp);
    ASSERT(pcb->moal_get_system_time);
    ASSERT(pcb->moal_init_timer);
    ASSERT(pcb->moal_free_timer);
    ASSERT(pcb->moal_start_timer);
    ASSERT(pcb->moal_stop_timer);
    ASSERT(pcb->moal_init_lock);
    ASSERT(pcb->moal_free_lock);
    ASSERT(pcb->moal_spin_lock);
    ASSERT(pcb->moal_spin_unlock);
    ASSERT(pcb->moal_print);
    ASSERT(pcb->moal_get_cardregs);

    ENTER();

    /* Save pmoal_handle */
    pmadapter->pmoal_handle = pmdevice->pmoal_handle;
    pmadapter->int_mode = pmdevice->int_mode;
    pmadapter->gpio_pin = pmdevice->gpio_pin;
    if ((pmadapter->int_mode == INTMODE_GPIO) && (pmadapter->gpio_pin == 0)) {
        PRINTM(MERROR, "SDIO_GPIO_INT_CONFIG: Invalid GPIO Pin\n");
        ret = MLAN_STATUS_FAILURE;
        goto error;
    }

    //atomic_set(pmdevice->int_flag, 1);
   // PRINTM(MINFO, "device->int_flag = %d\n", atomic_read(pmdevice->int_flag));
   // pmadapter->int_flag = pmdevice->int_flag;
   // PRINTM(MINFO, "pmadapter->int_flag = %d\n", atomic_read(pmadapter->int_flag));
    /* card specific probing has been deferred until now .. */
    if (MLAN_STATUS_SUCCESS != (ret = wlan_sdio_probe(pmadapter))) {
        ret = MLAN_STATUS_FAILURE;
        goto error;
    }
#ifdef MFG_CMD_SUPPORT
    pmadapter->mfg_mode = pmdevice->mfg_mode;
#endif

    for (i = 0; i < MLAN_MAX_BSS_NUM; i++) {
        pmadapter->priv[i] = MNULL;
        if (pmdevice->bss_attr[i].active == MTRUE) {
            /* For valid bss_attr, allocate memory for private structure */
            if ((pcb->moal_malloc(sizeof(mlan_private),
                                  (t_u8 **) & pmadapter->priv[i]) !=
                 MLAN_STATUS_SUCCESS)
                || !pmadapter->priv[i]) {
                ret = MLAN_STATUS_FAILURE;
                goto error;
            }

            memset(pmadapter->priv[i], 0, sizeof(mlan_private));

            pmadapter->priv[i]->adapter = pmadapter;

            /* Save bss_type, frame_type & bss_priority */
            pmadapter->priv[i]->bss_type = pmdevice->bss_attr[i].bss_type;
            pmadapter->priv[i]->frame_type = pmdevice->bss_attr[i].frame_type;
            pmadapter->priv[i]->bss_priority =
                pmdevice->bss_attr[i].bss_priority;

            /* Save bss_num */
            pmadapter->priv[i]->bss_num = i;

            /* init function table */
            for (j = 0; j < (sizeof(ops) / sizeof(ops[0])); j++) {
                if (ops[j].bss_type == pmadapter->priv[i]->bss_type) {
                    memcpy(&pmadapter->priv[i]->ops, &ops[j],
                           sizeof(mlan_operations));
                }
            }
        }
    }

    /* Initialize locks */
    if (pcb->moal_init_lock(&pmadapter->pmlan_lock)
        != MLAN_STATUS_SUCCESS) {
        ret = MLAN_STATUS_FAILURE;
        goto error;
    }

    if (pcb->moal_init_lock(&pmadapter->pmain_proc_lock)
        != MLAN_STATUS_SUCCESS) {
        ret = MLAN_STATUS_FAILURE;
        goto error;
    }

    if (pcb->moal_init_lock(&pmadapter->pmlan_cmd_lock)
        != MLAN_STATUS_SUCCESS) {
        ret = MLAN_STATUS_FAILURE;
        goto error;
    }
    if (pcb->
        moal_init_timer(&pmadapter->pmlan_cmd_timer, wlan_cmd_timeout_func,
                        pmadapter)
        != MLAN_STATUS_SUCCESS) {
        ret = MLAN_STATUS_FAILURE;
        goto error;
    }
    /* Return pointer of mlan_adapter to MOAL */
    *ppmlan_adapter = pmadapter;

    LEAVE();
    goto exit_register;

  error:
    PRINTM(MINFO, "Leave mlan_register with error\n");
    LEAVE();
    /* Free resources */
    if (pmadapter->pmlan_cmd_timer)
        pcb->moal_free_timer(pmadapter->pmlan_cmd_timer);
    if (pmadapter->pmlan_cmd_lock)
        pcb->moal_free_lock(pmadapter->pmlan_cmd_lock);
    if (pmadapter->pmain_proc_lock)
        pcb->moal_free_lock(pmadapter->pmain_proc_lock);
    if (pmadapter->pmlan_lock)
        pcb->moal_free_lock(pmadapter->pmlan_lock);
    for (i = 0; i < MLAN_MAX_BSS_NUM; i++) {
        if (pmadapter->priv[i])
            pcb->moal_mfree((t_u8 *) pmadapter->priv[i]);
    }
    pcb->moal_mfree((t_u8 *) pmadapter);

  exit_register:
    return ret;
}

/**
 *  @brief This function unregisters MOAL from MLAN module.
 *  
 *  @param pmlan_adapter   A pointer to a mlan_device structure
 *                         allocated in MOAL
 *
 *  @return                MLAN_STATUS_SUCCESS
 *                             The deregistration succeeded.
 *                         MLAN_STATUS_FAILURE
 *                             The deregistration failed.
 */
mlan_status
mlan_unregister(IN t_void * pmlan_adapter)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_adapter *pmadapter = (mlan_adapter *) pmlan_adapter;
    pmlan_callbacks pcb;
    t_s32 i = 0;

    ASSERT(pmlan_adapter);

    ENTER();

    pcb = &pmadapter->callbacks;

    /* Free timers */
    if (pmadapter->pmlan_cmd_timer)
        pcb->moal_free_timer(pmadapter->pmlan_cmd_timer);

    /* Free locks */
    pcb->moal_free_lock(pmadapter->pmlan_cmd_lock);
    pcb->moal_free_lock(pmadapter->pmain_proc_lock);
    pcb->moal_free_lock(pmadapter->pmlan_lock);

    /* Free private structures */
    for (i = 0; i < MLAN_MAX_BSS_NUM; i++) {
        if (pmadapter->priv[i]) {
            pcb->moal_mfree((t_u8 *) pmadapter->priv[i]);
        }
    }

    LEAVE();
    /* Free mlan_adapter */
    pcb->moal_mfree((t_u8 *) pmadapter);

    return ret;
}

/**
 *  @brief This function downloads the firmware
 *  
 *  @param pmlan_adapter   A pointer to a t_void pointer to store
 *                         mlan_adapter structure pointer
 *  @param pmfw            A pointer to firmware image
 *
 *  @return                MLAN_STATUS_SUCCESS
 *                             The firmware download succeeded.
 *                         MLAN_STATUS_FAILURE
 *                             The firmware download failed.
 */
mlan_status
mlan_dnld_fw(IN t_void * pmlan_adapter, IN pmlan_fw_image pmfw)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_adapter *pmadapter = (mlan_adapter *) pmlan_adapter;
    t_u32 poll_num = 1;

    ENTER();
    ASSERT(pmlan_adapter);

    /* Check if firmware is already running */
    ret = wlan_check_fw_status(pmadapter, poll_num);
    if (ret == MLAN_STATUS_SUCCESS) {
        PRINTM(MMSG, "WLAN FW already running! Skip FW download\n");
        goto done;
    }
    poll_num = MAX_FIRMWARE_POLL_TRIES;

    if (pmfw) {
        /* Download helper/firmware */
        ret = wlan_dnld_fw(pmadapter, pmfw);
        if (ret != MLAN_STATUS_SUCCESS) {
            PRINTM(MERROR, "wlan_dnld_fw fail ret=0x%x\n", ret);
            LEAVE();
            return ret;
        }
    }

    /* Check if the firmware is downloaded successfully or not */
    ret = wlan_check_fw_status(pmadapter, poll_num);
    if (ret != MLAN_STATUS_SUCCESS) {
        PRINTM(MFATAL, "FW failed to be active in time!\n");
        ret = MLAN_STATUS_FAILURE;
        LEAVE();
        return ret;
    }
  done:

    /* re-enable host interrupt for mlan after fw dnld is successful */
    wlan_enable_host_int(pmadapter);

    LEAVE();
    return ret;
}

/**
 *  @brief This function initializes the firmware
 *  
 *  @param pmlan_adapter   A pointer to a t_void pointer to store
 *                         mlan_adapter structure pointer
 *
 *  @return                MLAN_STATUS_SUCCESS
 *                             The firmware initialization succeeded.
 *                         MLAN_STATUS_PENDING
 *                             The firmware initialization is pending.
 *                         MLAN_STATUS_FAILURE
 *                             The firmware initialization failed.
 */
mlan_status
mlan_init_fw(IN t_void * pmlan_adapter)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_adapter *pmadapter = (mlan_adapter *) pmlan_adapter;

    ENTER();
    ASSERT(pmlan_adapter);

    pmadapter->hw_status = WlanHardwareStatusInitializing;

    /* Initialize firmware, may return PENDING */
    ret = wlan_init_fw(pmadapter);
    PRINTM(MINFO, "wlan_init_fw returned ret=0x%x\n", ret);

    LEAVE();
    return ret;
}

/**
 *  @brief Shutdown firmware
 *
 *  @param pmlan_adapter	A pointer to mlan_adapter structure
 *
 *  @return			MLAN_STATUS_SUCCESS
 *                      		The firmware shutdown call succeeded.
 *				MLAN_STATUS_PENDING
 *      	                	The firmware shutdown call is pending.
 *				MLAN_STATUS_FAILURE
 *      	                	The firmware shutdown call failed.
 */
mlan_status
mlan_shutdown_fw(IN t_void * pmlan_adapter)
{
    mlan_status ret = MLAN_STATUS_PENDING;
    mlan_adapter *pmadapter = (mlan_adapter *) pmlan_adapter;
    mlan_private *priv = MNULL;
    pmlan_callbacks pcb;
    t_s32 i = 0;

    ENTER();
    ASSERT(pmlan_adapter);
    /* mlan already shutdown */
    if (pmadapter->hw_status == WlanHardwareStatusNotReady)
        return MLAN_STATUS_SUCCESS;

    pmadapter->hw_status = WlanHardwareStatusClosing;
    /* wait for mlan_process to complete */
    if (pmadapter->mlan_processing) {
        PRINTM(MWARN, "mlan main processing is still running\n");
        return ret;
    }

    /* shut down mlan */
    PRINTM(MINFO, "Shutdown mlan...\n");
    pcb = &pmadapter->callbacks;
    /* Clean up Tx/Rx queues and delete BSS priority table */
    for (i = 0; i < MLAN_MAX_BSS_NUM; i++) {
        if (pmadapter->priv[i]) {
            priv = pmadapter->priv[i];

            wlan_clean_txrx(priv);
            wlan_wmm_cleanup_node(priv);
            pcb->moal_free_lock(priv->rx_pkt_lock);
            wlan_delete_bsspriotbl(priv);
            pcb->moal_free_lock(priv->wmm.ra_list_spinlock);
        }
    }

    for (i = 0; i < MLAN_MAX_BSS_NUM; i++)
        util_free_list_head(&pmadapter->bssprio_tbl[i].bssprio_head,
                            pcb->moal_free_lock);

    if (pcb->moal_spin_lock(pmadapter->pmlan_lock)
        != MLAN_STATUS_SUCCESS) {
        ret = MLAN_STATUS_FAILURE;
        goto exit_shutdown_fw;
    }

    /* Free adapter structure */
    wlan_free_adapter(pmadapter);

    if (pcb->moal_spin_unlock(pmadapter->pmlan_lock)
        != MLAN_STATUS_SUCCESS) {
        ret = MLAN_STATUS_FAILURE;
        goto exit_shutdown_fw;
    }

    /* Notify completion */
    ret = wlan_shutdown_fw_complete(pmadapter);

  exit_shutdown_fw:
    LEAVE();
    return ret;
}

/**
 *  @brief The main process
 *
 *  @param pmlan_adapter	A pointer to mlan_adapter structure
 *
 *  @return			MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
mlan_main_process(IN t_void * pmlan_adapter)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_adapter *pmadapter = (mlan_adapter *) pmlan_adapter;
    pmlan_callbacks pcb;
    t_u32 retried = 0;

    ENTER();

    ASSERT(pmlan_adapter);

    pcb = &pmadapter->callbacks;

//retry:
    pcb->moal_spin_lock(pmadapter->pmain_proc_lock);

    /* Check if already processing */
    if (pmadapter->mlan_processing) {
        pcb->moal_spin_unlock(pmadapter->pmain_proc_lock);
	PRINTM(MMSG, "%s: already in IRQ processing\n", __func__);
    	goto exit_main_proc;
    }   else {
    //    pmadapter->mlan_processing = MTRUE;
        pcb->moal_spin_unlock(pmadapter->pmain_proc_lock);
    } 
  process_start:
    do {
#if 0
    	if (atomic_read(pmadapter->int_flag) > 0)
	{ 
	     PRINTM(MMSG, "%s: In loop? int_flag = %d\n", __func__, atomic_read(pmadapter->int_flag) );

             atomic_set(pmadapter->int_flag, 0);
	}
#endif
        /* Is MLAN shutting down or not ready? */
        if ((pmadapter->hw_status == WlanHardwareStatusClosing) ||
            (pmadapter->hw_status == WlanHardwareStatusNotReady))
            break;
        /* Handle pending SDIO interrupts if any */
        if (pmadapter->sdio_ireg) {
            if (pmadapter->hs_activated == MTRUE)
                wlan_process_hs_config(pmadapter);
	    PRINTM(MMSG, "%s: sdio_ireg NULL, He Wu\n", __func__);
	    break;
#if 0
            wlan_process_int_status(pmadapter);
#endif
        }
        /* Need to wake up the card ? */
        if ((pmadapter->ps_state == PS_STATE_SLEEP) &&
            (pmadapter->pm_wakeup_card_req &&
             !pmadapter->pm_wakeup_fw_try) &&
            (util_peek_list
             (&pmadapter->cmd_pending_q, pcb->moal_spin_lock,
              pcb->moal_spin_unlock)
             || !wlan_wmm_lists_empty(pmadapter))) {
            pmadapter->pm_wakeup_fw_try = MTRUE;
            wlan_pm_wakeup_card(pmadapter);
            continue;
        }
        if (IS_CARD_RX_RCVD(pmadapter)) {
            pmadapter->pm_wakeup_fw_try = MFALSE;
            if (pmadapter->ps_state == PS_STATE_SLEEP)
                pmadapter->ps_state = PS_STATE_AWAKE;
        } else {
            /* We have tried to wakeup the card already */
            if (pmadapter->pm_wakeup_fw_try)
                break;
            if (pmadapter->ps_state != PS_STATE_AWAKE ||
                (pmadapter->tx_lock_flag == MTRUE))
                break;

            if (pmadapter->scan_processing || pmadapter->data_sent
                || wlan_wmm_lists_empty(pmadapter)
                ) {
                if (pmadapter->cmd_sent || pmadapter->curr_cmd ||
                    (!util_peek_list(&pmadapter->cmd_pending_q,
                                     pcb->moal_spin_lock,
                                     pcb->moal_spin_unlock))) {
                    break;
                }
            }
        }

        /* Check for Cmd Resp */
        if (pmadapter->cmd_resp_received) {
            pmadapter->cmd_resp_received = MFALSE;
            wlan_process_cmdresp(pmadapter);

            /* call moal back when init_fw is done */
            if (pmadapter->hw_status == WlanHardwareStatusInitdone) {
                pmadapter->hw_status = WlanHardwareStatusReady;
                wlan_init_fw_complete(pmadapter);
            }
        }

        /* Check for event */
        if (pmadapter->event_received) {
            pmadapter->event_received = MFALSE;
            wlan_process_event(pmadapter);
        }

        /* Check if we need to confirm Sleep Request received previously */
        if (pmadapter->ps_state == PS_STATE_PRE_SLEEP) {

            if (!pmadapter->cmd_sent && !pmadapter->curr_cmd) {
                wlan_check_ps_cond(pmadapter);
            }
        }

        /* 
         * The ps_state may have been changed during processing of
         * Sleep Request event.
         */
        if ((pmadapter->ps_state == PS_STATE_SLEEP)
            || (pmadapter->ps_state == PS_STATE_PRE_SLEEP)
            || (pmadapter->ps_state == PS_STATE_SLEEP_CFM)
            || (pmadapter->tx_lock_flag == MTRUE)
            )
            continue;

        if (!pmadapter->cmd_sent && !pmadapter->curr_cmd) {
            if (wlan_exec_next_cmd(pmadapter) == MLAN_STATUS_FAILURE) {
                ret = MLAN_STATUS_FAILURE;
                break;
            }
        }

        /* TODO: need to check tid eligibility for transfer to avoid dead loop */
        if (!pmadapter->scan_processing && !pmadapter->data_sent &&
            !wlan_wmm_lists_empty(pmadapter)) {
            wlan_wmm_process_tx(pmadapter);
            if (pmadapter->hs_activated == MTRUE) {
                pmadapter->is_hs_configured = MFALSE;
                wlan_host_sleep_activated_event(wlan_get_priv
                                                (pmadapter, MLAN_BSS_TYPE_STA),
                                                MFALSE);
            }
        }

        if (pmadapter->delay_null_pkt && !pmadapter->cmd_sent &&
            !pmadapter->curr_cmd && !IS_COMMAND_PENDING(pmadapter) &&
            wlan_wmm_lists_empty(pmadapter)) {
            if (wlan_send_null_packet
                (wlan_get_priv(pmadapter, MLAN_BSS_TYPE_STA),
                 MRVDRV_TxPD_POWER_MGMT_NULL_PACKET |
                 MRVDRV_TxPD_POWER_MGMT_LAST_PACKET)
                == MLAN_STATUS_SUCCESS) {
                pmadapter->delay_null_pkt = MFALSE;
            }
            break;
        }
    } while (MTRUE);
//    } while (atomic_read(pmadapter->int_flag));

    pcb->moal_spin_lock(pmadapter->pmain_proc_lock);
//    if ((pmadapter->sdio_ireg) || IS_CARD_RX_RCVD(pmadapter)) {
    if (IS_CARD_RX_RCVD(pmadapter)) {
        pcb->moal_spin_unlock(pmadapter->pmain_proc_lock);
        goto process_start;
    }

    pmadapter->mlan_processing = MFALSE;
    pcb->moal_spin_unlock(pmadapter->pmain_proc_lock);

  exit_main_proc:
    if (pmadapter->hw_status == WlanHardwareStatusClosing)
        mlan_shutdown_fw(pmadapter);
    LEAVE();
    return ret;
}

/**
 *  @brief Function to send packet
 *
 *  @param pmlan_adapter	A pointer to mlan_adapter structure
 *  @param pmbuf		A pointer to mlan_buffer structure
 *
 *  @return			MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
mlan_send_packet(IN t_void * pmlan_adapter, IN pmlan_buffer pmbuf)
{
    mlan_status ret = MLAN_STATUS_PENDING;
    mlan_adapter *pmadapter = (mlan_adapter *) pmlan_adapter;

    ENTER();
    ASSERT(pmlan_adapter && pmbuf);

    ASSERT(pmbuf->bss_num < MLAN_MAX_BSS_NUM);

    /* Transmit the packet */
    wlan_wmm_add_buf_txqueue(pmadapter, pmbuf);

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
    mlan_status ret = MLAN_STATUS_SUCCESS;
    pmlan_adapter pmadapter = (pmlan_adapter) adapter;
    pmlan_private pmpriv = MNULL;

    ENTER();

    if (pioctl_req == MNULL) {
        PRINTM(MERROR, "MLAN IOCTL information buffer is NULL\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }
    if (pioctl_req->action == MLAN_ACT_CANCEL) {
        wlan_cancel_pending_ioctl(pmadapter, pioctl_req);
        ret = MLAN_STATUS_SUCCESS;
        goto exit;
    }
    pmpriv = pmadapter->priv[pioctl_req->bss_num];
    ret = pmpriv->ops.ioctl(adapter, pioctl_req);
  exit:
    LEAVE();
    return ret;
}

