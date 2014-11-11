/** @file moal_ioctl.c
  *
  * @brief This file contains ioctl function to MLAN
  * 
  * Copyright (C) 2008-2009, Marvell International Ltd. 
  * All Rights Reserved
  */

/********************************************************
Change log:
    10/21/2008: initial version
********************************************************/

#include        "moal_main.h"

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
 *  @brief Copy Rates
 *   
 *  @param dest                 A pointer to Dest Buf
 *  @param pos                  The position for copy
 *  @param src                  A pointer to Src Buf
 *  @param len                  The len of Src Buf
 *
 *  @return                     Number of rates copied 
 */
static inline int
woal_copy_rates(t_u8 * dest, int pos, t_u8 * src, int len)
{
    int i;

    for (i = 0; i < len && src[i]; i++, pos++) {
        if (pos >= MLAN_SUPPORTED_RATES)
            break;
        dest[pos] = src[i];
    }
    return pos;
}

/** 
 *  @brief Copy multicast table
 *   
 *  @param mlist              A pointer to mlan_multicast_list structure
 *  @param dev                A pointer to net_device structure                 
 *
 *  @return                   Number of mulitcast address
 */
static inline int
woal_copy_mcast_addr(mlan_multicast_list * mlist, struct net_device *dev)
{
    int i = 0;
    struct dev_mc_list *mcptr = dev->mc_list;

    for (i = 0; i < dev->mc_count; i++) {
        memcpy(&mlist->mac_list[i], mcptr->dmi_addr, ETH_ALEN);
        mcptr = mcptr->next;
    }
    return i;
}

/** 
 *  @brief Fill in wait queue 
 *   
 *  @param priv               A pointer to moal_private structure
 *  @param wait               A pointer to wait_queue structure                 
 *  @param wait_option        Wait option
 *
 *  @return                   N/A
 */
static inline void
woal_fill_wait_queue(moal_private * priv, wait_queue * wait, t_u8 wait_option)
{
    ENTER();
    wait->start_time = jiffies;
    switch (wait_option) {
    case MOAL_NO_WAIT:
        break;
    case MOAL_IOCTL_WAIT:
        priv->ioctl_wait_q_woken = MFALSE;
        wait->wait = &priv->ioctl_wait_q;
        wait->condition = &priv->ioctl_wait_q_woken;
        break;
    case MOAL_CMD_WAIT:
        priv->cmd_wait_q_woken = MFALSE;
        wait->wait = &priv->cmd_wait_q;
        wait->condition = &priv->cmd_wait_q_woken;
        break;
    case MOAL_PROC_WAIT:
        priv->proc_wait_q_woken = MFALSE;
        wait->wait = &priv->proc_wait_q;
        wait->condition = &priv->proc_wait_q_woken;
        break;
    case MOAL_WSTATS_WAIT:
        priv->w_stats_wait_q_woken = MFALSE;
        wait->wait = &priv->w_stats_wait_q;
        wait->condition = &priv->w_stats_wait_q_woken;
        break;
    }
    LEAVE();
    return;
}

/** 
 *  @brief Wait mlan ioctl complete
 *   
 *  @param priv               A pointer to moal_private structure
 *  @param req                A pointer to mlan_ioctl_req structure   
 *  @param wait_option        Wait option
 *
 *  @return                   N/A
 */
static inline void
woal_wait_ioctl_complete(moal_private * priv, mlan_ioctl_req * req,
                         t_u8 wait_option)
{
    BOOLEAN cancel_flag = MFALSE;
    ENTER();

    switch (wait_option) {
    case MOAL_NO_WAIT:
        break;
    case MOAL_IOCTL_WAIT:
        wait_event_interruptible(priv->ioctl_wait_q, priv->ioctl_wait_q_woken);
        if (priv->ioctl_wait_q_woken == MFALSE)
            cancel_flag = MTRUE;
        break;
    case MOAL_CMD_WAIT:
        wait_event_interruptible(priv->cmd_wait_q, priv->cmd_wait_q_woken);
        if (priv->cmd_wait_q_woken == MFALSE)
            cancel_flag = MTRUE;
        break;
    case MOAL_PROC_WAIT:
        wait_event_interruptible(priv->proc_wait_q, priv->proc_wait_q_woken);
        if (priv->proc_wait_q_woken == MFALSE)
            cancel_flag = MTRUE;
        break;
    case MOAL_WSTATS_WAIT:
        wait_event_interruptible(priv->w_stats_wait_q,
                                 priv->w_stats_wait_q_woken);
        if (priv->w_stats_wait_q_woken == MFALSE)
            cancel_flag = MTRUE;
        break;
    }
    if (cancel_flag == MTRUE) {
        req->action = MLAN_ACT_CANCEL;
        mlan_ioctl(priv->phandle->pmlan_adapter, req);
        PRINTM(CMND,
               "IOCTL cancel: id=0x%lx, sub_id=0x%lx wait_option=%d, action=%d\n",
               req->req_id, (*(t_u32 *) req->pbuf), wait_option,
               (int) req->action);
    }
    LEAVE();
    return;
}

/********************************************************
                Global Functions
********************************************************/
/** 
 *  @brief Send open request to MLAN
 *   
 *  @param priv                 A pointer to moal_private structure
 *
 *  @return                     MLAN_STATUS_SUCCESS
 */
mlan_status
woal_request_open(moal_private * priv)
{
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Send close request to MLAN
 *   
 *  @param priv                 A pointer to moal_private structure
 *
 *  @return                     MLAN_STATUS_SUCCESS
 */
mlan_status
woal_request_close(moal_private * priv)
{
    if (!netif_queue_stopped(priv->netdev))
        netif_stop_queue(priv->netdev);
    if (netif_carrier_ok(priv->netdev))
        netif_carrier_off(priv->netdev);
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Send ioctl request to MLAN
 *   
 *  @param priv          A pointer to moal_private structure
 *  @param req           A pointer to mlan_ioctl_req buffer
 *  @param wait_option   Wait option (MOAL_WAIT or MOAL_NO_WAIT)
 *
 *  @return              MLAN_STATUS_SUCCESS -- success, otherwise fail
 */
mlan_status
woal_request_ioctl(moal_private * priv, mlan_ioctl_req * req, t_u8 wait_option)
{
    wait_queue *wait;
    mlan_status status;

    ENTER();
    if (priv->phandle->surprise_removed == MTRUE) {
        PRINTM(ERROR, "IOCTL is not allowed while surprise_removed = TRUE\n");
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }

    wait = (wait_queue *) req->reserved_1;
    req->bss_num = priv->bss_num;
    if (wait_option)
        woal_fill_wait_queue(priv, wait, wait_option);
    else
        req->reserved_1 = 0;

    /* Call MLAN ioctl handle */
    status = mlan_ioctl(priv->phandle->pmlan_adapter, req);
    switch (status) {
    case MLAN_STATUS_PENDING:
        PRINTM(CMND,
               "IOCTL pending: id=0x%lx, sub_id=0x%lx wait_option=%d, action=%d\n",
               req->req_id, (*(t_u32 *) req->pbuf), wait_option,
               (int) req->action);
        priv->ioctl_pending++;
        /* Status pending, wake up main process */
        mlan_main_process(priv->phandle->pmlan_adapter);

        /* Wait for completion */
        if (wait_option) {
            woal_wait_ioctl_complete(priv, req, wait_option);
            status = wait->status;
        }
        break;
    case MLAN_STATUS_SUCCESS:
    case MLAN_STATUS_FAILURE:
    case MLAN_STATUS_RESOURCE:
    default:
        break;
    }
    LEAVE();
    return status;
}

/** 
 *  @brief Send set MAC address request to MLAN
 *   
 *  @param priv          A pointer to moal_private structure
 *
 *  @return              MLAN_STATUS_SUCCESS -- success, otherwise fail
 */
mlan_status
woal_request_set_mac_address(moal_private * priv)
{
    mlan_ioctl_req *req = NULL;
    mlan_ds_bss *bss = NULL;
    mlan_status status;
    ENTER();

    /* Allocate an IOCTL request buffer */
    req = (mlan_ioctl_req *) woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_bss));
    if (req == NULL) {
        status = MLAN_STATUS_FAILURE;
        goto done;
    }

    /* Fill request buffer */
    bss = (mlan_ds_bss *) req->pbuf;
    bss->sub_command = MLAN_OID_BSS_MAC_ADDR;
    memcpy(&bss->param.mac_addr, priv->current_addr,
           sizeof(mlan_802_11_mac_addr));
    req->req_id = MLAN_IOCTL_BSS;
    req->action = MLAN_ACT_SET;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, MOAL_CMD_WAIT);
    if (status == MLAN_STATUS_SUCCESS) {
        memcpy(priv->netdev->dev_addr, priv->current_addr, ETH_ALEN);
        HEXDUMP("priv->MacAddr:", priv->current_addr, ETH_ALEN);
    } else {
        PRINTM(ERROR, "set mac address failed! status=%d, error_code=0x%lx\n",
               status, req->status_code);
    }
  done:
    if (req)
        kfree(req);
    LEAVE();
    return status;
}

/** 
 *  @brief Send multicast list request to MLAN
 *   
 *  @param priv                 A pointer to moal_private structure
 *  @param dev                  A pointer to net_device structure                 
 *
 *  @return                     N/A
 */
void
woal_request_set_multicast_list(moal_private * priv, struct net_device *dev)
{
    mlan_ioctl_req *req = NULL;
    mlan_ds_bss *bss = NULL;
    mlan_status status;
    ENTER();

    /* Allocate an IOCTL request buffer */
    req =
        (mlan_ioctl_req *)
        woal_alloc_mlan_ioctl_req_no_wait(sizeof(mlan_ds_bss));
    if (req == NULL) {
        PRINTM(ERROR, "%s:Fail to alloc ioctl req buffer\n", __FUNCTION__);
        goto done;
    }

    /* Fill request buffer */
    bss = (mlan_ds_bss *) req->pbuf;
    bss->sub_command = MLAN_OID_BSS_MULTICAST_LIST;
    req->req_id = MLAN_IOCTL_BSS;
    req->action = MLAN_ACT_SET;
    if (dev->flags & IFF_PROMISC) {
        bss->param.multicast_list.mode = MLAN_PROMISC_MODE;
    } else if (dev->flags & IFF_ALLMULTI ||
               dev->mc_count > MLAN_MAX_MULTICAST_LIST_SIZE) {
        bss->param.multicast_list.mode = MLAN_ALL_MULTI_MODE;
    } else {
        bss->param.multicast_list.mode = MLAN_MULTICAST_MODE;
        if (dev->mc_count)
            bss->param.multicast_list.num_multicast_addr =
                woal_copy_mcast_addr(&bss->param.multicast_list, dev);
    }

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, MOAL_NO_WAIT);
    if (status != MLAN_STATUS_PENDING)
        kfree(req);
  done:
    LEAVE();
    return;
}

/** 
 *  @brief Send get fw info request to MLAN
 *   
 *  @param priv                 A pointer to moal_private structure
 *
 *  @return                     N/A
 */
void
woal_request_get_fw_info(moal_private * priv)
{
    mlan_ioctl_req *req = NULL;
    mlan_ds_get_info *info;
    mlan_status status;
    ENTER();
    memset(priv->current_addr, 0xff, ETH_ALEN);

    /* Allocate an IOCTL request buffer */
    req = (mlan_ioctl_req *) woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_bss));
    if (req == NULL) {
        status = MLAN_STATUS_FAILURE;
        goto done;
    }

    /* Fill request buffer */
    info = (mlan_ds_get_info *) req->pbuf;
    req->req_id = MLAN_IOCTL_GET_INFO;
    req->action = MLAN_ACT_GET;
    info->sub_command = MLAN_OID_GET_FW_INFO;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, MOAL_CMD_WAIT);
    if (status == MLAN_STATUS_SUCCESS) {
        priv->phandle->fw_release_number = info->param.fw_info.fw_ver;
        if (priv->current_addr[0] == 0xff)
            memcpy(priv->current_addr, &info->param.fw_info.mac_addr,
                   sizeof(mlan_802_11_mac_addr));
        memcpy(priv->netdev->dev_addr, priv->current_addr, ETH_ALEN);
        DBG_HEXDUMP(IF_D, "mac", priv->current_addr, 6);
    } else
        PRINTM(ERROR, "get fw info failed! status=%d, error_code=0x%lx\n",
               status, req->status_code);
  done:
    if (req)
        kfree(req);
    LEAVE();
    return;
}

/** 
 *  @brief Get Host Sleep parameters
 *
 *  @param priv                 A pointer to moal_private structure
 *  @param wait_option          Wait option (MOAL_WAIT or MOAL_NO_WAIT)   
 *  @param hscfg                A pointer to mlan_ds_hs_cfg structure
 *
 *  @return                     MLAN_STATUS_SUCCESS -- success, otherwise fail          
 */
mlan_status
woal_get_hs_params(moal_private * priv, t_u8 wait_option,
                   mlan_ds_hs_cfg * hscfg)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_ds_pm_cfg *pmcfg = NULL;
    mlan_ioctl_req *req = NULL;

    ENTER();

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_pm_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    pmcfg = (mlan_ds_pm_cfg *) req->pbuf;
    pmcfg->sub_command = MLAN_OID_PM_CFG_HS_CFG;
    req->req_id = MLAN_IOCTL_PM_CFG;
    req->action = MLAN_ACT_GET;

    /* Send IOCTL request to MLAN */
    ret = woal_request_ioctl(priv, req, wait_option);
    if (ret == MLAN_STATUS_SUCCESS) {
        if (hscfg) {
            memcpy(hscfg, &pmcfg->param.hs_cfg, sizeof(mlan_ds_hs_cfg));
        }
    }
  done:
    if (req && (ret != MLAN_STATUS_PENDING))
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Cancel Host Sleep configuration
 *
 *  @param priv         A pointer to moal_private structure
 *
 *  @return             MLAN_STATUS_SUCCESS, MLAN_STATUS_PENDING,
 *                      or MLAN_STATUS_FAILURE          
 */
mlan_status
woal_hs_cfg_cancel(moal_private * priv)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_ioctl_req *req = NULL;
    mlan_ds_pm_cfg *pmcfg = NULL;

    ENTER();

    req = woal_alloc_mlan_ioctl_req_no_wait(sizeof(mlan_ds_pm_cfg));
    if (req == NULL) {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    pmcfg = (mlan_ds_pm_cfg *) req->pbuf;
    pmcfg->sub_command = MLAN_OID_PM_CFG_HS_CFG;
    req->req_id = MLAN_IOCTL_PM_CFG;
    req->action = MLAN_ACT_SET;

    /* Get current Host Sleep configuration */
    woal_get_hs_params(priv, MOAL_IOCTL_WAIT, &pmcfg->param.hs_cfg);

    pmcfg->param.hs_cfg.conditions = HOST_SLEEP_CFG_CANCEL;
    pmcfg->param.hs_cfg.is_invoke_hostcmd = MTRUE;

    ret = woal_request_ioctl(priv, req, MOAL_NO_WAIT);

    if (ret != MLAN_STATUS_PENDING)
        kfree(req);

  done:
    LEAVE();
    return ret;
}

/** 
 *  @brief Get mode
 *
 *  @param priv                 A pointer to moal_private structure
 *  @param wait_option          Wait option (MOAL_WAIT or MOAL_NO_WAIT)   
 *
 *  @return                     wireless mode
 */
t_u32
woal_get_mode(moal_private * priv, t_u8 wait_option)
{
    int ret = 0;
    mlan_ds_bss *bss = NULL;
    mlan_ioctl_req *req = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;
    t_u32 mode = priv->w_stats.status;
    ENTER();

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_bss));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    bss = (mlan_ds_bss *) req->pbuf;
    bss->sub_command = MLAN_OID_BSS_MODE;
    req->req_id = MLAN_IOCTL_BSS;
    req->action = MLAN_ACT_GET;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
    if (status == MLAN_STATUS_SUCCESS) {
        switch (bss->param.bss_mode) {
        case MLAN_BSS_MODE_INFRA:
            mode = IW_MODE_INFRA;
            break;
        case MLAN_BSS_MODE_IBSS:
            mode = IW_MODE_ADHOC;
            break;
        default:
            mode = IW_MODE_AUTO;
            break;
        }
    }
  done:
    if (req && (status != MLAN_STATUS_PENDING))
        kfree(req);
    LEAVE();
    return mode;
}

/** 
 *  @brief Get RSSI info
 *
 *  @param priv                 A pointer to moal_private structure
 *  @param wait_option          Wait option
 *  @param signal               A pointer tp mlan_ds_get_signal structure
 *
 *  @return                     MLAN_STATUS_SUCCESS -- success, otherwise fail          
 */
mlan_status
woal_get_signal_info(moal_private * priv, t_u8 wait_option,
                     mlan_ds_get_signal * signal)
{
    int ret = 0;
    mlan_ds_get_info *info = NULL;
    mlan_ioctl_req *req = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;
    ENTER();

    /* Allocate an IOCTL request buffer */
    if (wait_option == MOAL_NO_WAIT)
        req = woal_alloc_mlan_ioctl_req_no_wait(sizeof(mlan_ds_get_info));
    else
        req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_get_info));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    info = (mlan_ds_get_info *) req->pbuf;
    info->sub_command = MLAN_OID_GET_SIGNAL;
    info->param.signal.selector = ALL_RSSI_INFO_MASK;
    req->req_id = MLAN_IOCTL_GET_INFO;
    req->action = MLAN_ACT_GET;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
    if (status == MLAN_STATUS_SUCCESS) {
        if (signal)
            memcpy(signal, &info->param.signal, sizeof(mlan_ds_get_signal));
        if (info->param.signal.selector & BCN_RSSI_AVG_MASK)
            priv->w_stats.qual.level = info->param.signal.bcn_rssi_avg;
        if (info->param.signal.selector & BCN_NF_AVG_MASK)
            priv->w_stats.qual.noise = info->param.signal.bcn_nf_avg;
    }
  done:
    if (req && (status != MLAN_STATUS_PENDING))
        kfree(req);
    LEAVE();
    return status;
}

/** 
 *  @brief Get stats
 *
 *  @param priv                 A pointer to moal_private structure
 *  @param wait_option          Wait option
 *  @param stats                A pointer to mlan_ds_get_stats structure
 *
 *  @return                     MLAN_STATUS_SUCCESS -- success, otherwise fail          
 */
mlan_status
woal_get_stats_info(moal_private * priv, t_u8 wait_option,
                    mlan_ds_get_stats * stats)
{
    int ret = 0;
    mlan_ds_get_info *info = NULL;
    mlan_ioctl_req *req = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;
    ENTER();

    /* Allocate an IOCTL request buffer */
    if (wait_option == MOAL_NO_WAIT)
        req = woal_alloc_mlan_ioctl_req_no_wait(sizeof(mlan_ds_get_info));
    else
        req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_get_info));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    info = (mlan_ds_get_info *) req->pbuf;
    info->sub_command = MLAN_OID_GET_STATS;
    req->req_id = MLAN_IOCTL_GET_INFO;
    req->action = MLAN_ACT_GET;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
    if (status == MLAN_STATUS_SUCCESS) {
        if (stats)
            memcpy(stats, &info->param.stats, sizeof(mlan_ds_get_stats));
        priv->w_stats.discard.fragment = info->param.stats.fcs_error;
        priv->w_stats.discard.retries = info->param.stats.retry;
        priv->w_stats.discard.misc = info->param.stats.ack_failure;
    }
  done:
    if (req && (status != MLAN_STATUS_PENDING))
        kfree(req);
    LEAVE();
    return status;
}

/** 
 *  @brief Get data rates
 *
 *  @param priv                 A pointer to moal_private structure
 *  @param wait_option          Wait option
 *  @param m_rates              A pointer to moal_802_11_rates structure
 *
 *  @return                     MLAN_STATUS_SUCCESS -- success, otherwise fail          
 */
mlan_status
woal_get_data_rates(moal_private * priv, t_u8 wait_option,
                    moal_802_11_rates * m_rates)
{
    int ret = 0;
    mlan_ds_rate *rate = NULL;
    mlan_ioctl_req *req = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;
    ENTER();

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_rate));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    rate = (mlan_ds_rate *) req->pbuf;
    rate->sub_command = MLAN_OID_SUPPORTED_RATES;
    req->req_id = MLAN_IOCTL_RATE;
    req->action = MLAN_ACT_GET;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
    if (status == MLAN_STATUS_SUCCESS) {
        if (m_rates)
            m_rates->num_of_rates =
                woal_copy_rates(m_rates->rates, m_rates->num_of_rates,
                                rate->param.rates, MLAN_SUPPORTED_RATES);
    }
  done:
    if (req && (status != MLAN_STATUS_PENDING))
        kfree(req);
    LEAVE();
    return status;
}

/** 
 *  @brief Get channel list
 *
 *  @param priv                 A pointer to moal_private structure
 *  @param wait_option          Wait option
 *  @param chan_list            A pointer to mlan_chan_list structure
 *
 *  @return                     MLAN_STATUS_SUCCESS -- success, otherwise fail          
 */
mlan_status
woal_get_channel_list(moal_private * priv, t_u8 wait_option,
                      mlan_chan_list * chan_list)
{
    int ret = 0;
    mlan_ds_bss *bss = NULL;
    mlan_ioctl_req *req = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;
    ENTER();

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_bss));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    bss = (mlan_ds_bss *) req->pbuf;
    bss->sub_command = MLAN_OID_BSS_CHANNEL_LIST;
    req->req_id = MLAN_IOCTL_BSS;
    req->action = MLAN_ACT_GET;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
    if (status == MLAN_STATUS_SUCCESS) {
        if (chan_list) {
            memcpy(chan_list, &bss->param.chanlist, sizeof(mlan_chan_list));
        }
    }
  done:
    if (req && (status != MLAN_STATUS_PENDING))
        kfree(req);
    LEAVE();
    return status;
}

/** 
 *  @brief Get BSS info
 *
 *  @param priv                 A pointer to moal_private structure
 *  @param wait_option          Wait option
 *  @param bss_info             A pointer to mlan_bss_info structure
 *
 *  @return                     MLAN_STATUS_SUCCESS -- success, otherwise fail          
 */
mlan_status
woal_get_bss_info(moal_private * priv, t_u8 wait_option,
                  mlan_bss_info * bss_info)
{
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_get_info *info = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;
    ENTER();

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_get_info));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    info = (mlan_ds_get_info *) req->pbuf;
    info->sub_command = MLAN_OID_GET_BSS_INFO;
    req->req_id = MLAN_IOCTL_GET_INFO;
    req->action = MLAN_ACT_GET;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
    if (status == MLAN_STATUS_SUCCESS) {
        if (bss_info) {
            memcpy(bss_info, &info->param.bss_info, sizeof(mlan_bss_info));
        }
    }
  done:
    if (req && (status != MLAN_STATUS_PENDING))
        kfree(req);
    LEAVE();
    return status;
}

#ifdef PROC_DEBUG
/** 
 *  @brief Get debug info
 *
 *  @param priv                 A pointer to moal_private structure
 *  @param wait_option          Wait option
 *  @param debug_info           A pointer to mlan_debug_info structure
 *
 *  @return                     MLAN_STATUS_SUCCESS -- success, otherwise fail          
 */
mlan_status
woal_get_debug_info(moal_private * priv, t_u8 wait_option,
                    mlan_debug_info * debug_info)
{
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_get_info *info = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;
    ENTER();

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_get_info));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    info = (mlan_ds_get_info *) req->pbuf;
    info->sub_command = MLAN_OID_GET_DEBUG_INFO;
    req->req_id = MLAN_IOCTL_GET_INFO;
    req->action = MLAN_ACT_GET;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
    if (status == MLAN_STATUS_SUCCESS) {
        if (debug_info) {
            memcpy(debug_info, &info->param.debug_info,
                   sizeof(mlan_debug_info));
        }
    }
  done:
    if (req && (status != MLAN_STATUS_PENDING))
        kfree(req);
    LEAVE();
    return status;
}

/** 
 *  @brief Set debug info
 *
 *  @param priv                 A pointer to moal_private structure
 *  @param wait_option          Wait option
 *  @param debug_info           A pointer to mlan_debug_info structure
 *
 *  @return                     MLAN_STATUS_SUCCESS -- success, otherwise fail          
 */
mlan_status
woal_set_debug_info(moal_private * priv, t_u8 wait_option,
                    mlan_debug_info * debug_info)
{
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_get_info *info = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;

    ENTER();

    if (!debug_info) {
        ret = -EINVAL;
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_get_info));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    info = (mlan_ds_get_info *) req->pbuf;
    info->sub_command = MLAN_OID_GET_DEBUG_INFO;
    memcpy(&info->param.debug_info, debug_info, sizeof(mlan_debug_info));
    req->req_id = MLAN_IOCTL_GET_INFO;
    req->action = MLAN_ACT_SET;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
  done:
    if (req && (status != MLAN_STATUS_PENDING))
        kfree(req);
    LEAVE();
    return status;
}
#endif /* PROC_DEBUG */

/** 
 *  @brief Get scan table
 *
 *  @param priv                 A pointer to moal_private structure
 *  @param wait_option          Wait option
 *  @param scan_resp            A pointer to mlan_scan_resp structure
 *
 *  @return                     MLAN_STATUS_SUCCESS -- success, otherwise fail          
 */
mlan_status
woal_get_scan_table(moal_private * priv, t_u8 wait_option,
                    mlan_scan_resp * scan_resp)
{
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_scan *scan = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;
    ENTER();

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_scan));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    scan = (mlan_ds_scan *) req->pbuf;
    scan->sub_command = MLAN_OID_SCAN_NORMAL;
    req->req_id = MLAN_IOCTL_SCAN;
    req->action = MLAN_ACT_GET;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
    if (status == MLAN_STATUS_SUCCESS) {
        if (scan_resp) {
            memcpy(scan_resp, &scan->param.scan_resp, sizeof(mlan_scan_resp));
        }
    }
  done:
    if (req && (status != MLAN_STATUS_PENDING))
        kfree(req);
    LEAVE();
    return status;
}

/** 
 *  @brief Set authentication mode
 *
 *  @param priv                 A pointer to moal_private structure
 *  @param wait_option          Wait option
 *  @param auth_mode            Authentication mode
 *
 *  @return                     MLAN_STATUS_SUCCESS -- success, otherwise fail          
 */
mlan_status
woal_set_auth_mode(moal_private * priv, t_u8 wait_option, t_u32 auth_mode)
{
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_sec_cfg *sec = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;
    ENTER();

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_sec_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    sec = (mlan_ds_sec_cfg *) req->pbuf;
    sec->sub_command = MLAN_OID_SEC_CFG_AUTH_MODE;
    req->req_id = MLAN_IOCTL_SEC_CFG;
    req->action = MLAN_ACT_SET;
    sec->param.auth_mode = auth_mode;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
  done:
    if (req && (status != MLAN_STATUS_PENDING))
        kfree(req);
    LEAVE();
    return status;
}

/** 
 *  @brief Get authentication mode
 *
 *  @param priv                 A pointer to moal_private structure
 *  @param wait_option          Wait option
 *  @param auth_mode            A pointer to authentication mode
 *
 *  @return                     MLAN_STATUS_SUCCESS -- success, otherwise fail          
 */
mlan_status
woal_get_auth_mode(moal_private * priv, t_u8 wait_option, t_u32 * auth_mode)
{
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_sec_cfg *sec = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;
    ENTER();

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_sec_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    sec = (mlan_ds_sec_cfg *) req->pbuf;
    sec->sub_command = MLAN_OID_SEC_CFG_AUTH_MODE;
    req->req_id = MLAN_IOCTL_SEC_CFG;
    req->action = MLAN_ACT_GET;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
    if (status == MLAN_STATUS_SUCCESS && auth_mode) {
        *auth_mode = sec->param.auth_mode;
    }
  done:
    if (req && (status != MLAN_STATUS_PENDING))
        kfree(req);
    LEAVE();
    return status;
}

/** 
 *  @brief Set encrypt mode
 *
 *  @param priv                 A pointer to moal_private structure
 *  @param wait_option          Wait option
 *  @param encrypt_mode         Encrypt mode
 *
 *  @return                     MLAN_STATUS_SUCCESS -- success, otherwise fail          
 */
mlan_status
woal_set_encrypt_mode(moal_private * priv, t_u8 wait_option, t_u32 encrypt_mode)
{
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_sec_cfg *sec = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;
    ENTER();

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_sec_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    sec = (mlan_ds_sec_cfg *) req->pbuf;
    sec->sub_command = MLAN_OID_SEC_CFG_ENCRYPT_MODE;
    req->req_id = MLAN_IOCTL_SEC_CFG;
    req->action = MLAN_ACT_SET;
    sec->param.encrypt_mode = encrypt_mode;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
  done:
    if (req && (status != MLAN_STATUS_PENDING))
        kfree(req);
    LEAVE();
    return status;
}

/** 
 *  @brief Get encrypt mode
 *
 *  @param priv                 A pointer to moal_private structure
 *  @param wait_option          Wait option
 *  @param encrypt_mode         A pointer to encrypt mode
 *
 *  @return                     MLAN_STATUS_SUCCESS -- success, otherwise fail          
 */
mlan_status
woal_get_encrypt_mode(moal_private * priv, t_u8 wait_option,
                      t_u32 * encrypt_mode)
{
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_sec_cfg *sec = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;

    ENTER();

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_sec_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    sec = (mlan_ds_sec_cfg *) req->pbuf;
    sec->sub_command = MLAN_OID_SEC_CFG_ENCRYPT_MODE;
    req->req_id = MLAN_IOCTL_SEC_CFG;
    req->action = MLAN_ACT_GET;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
    if (status == MLAN_STATUS_SUCCESS && encrypt_mode) {
        *encrypt_mode = sec->param.encrypt_mode;
    }
  done:
    if (req && (status != MLAN_STATUS_PENDING))
        kfree(req);
    LEAVE();
    return status;
}

/** 
 *  @brief Set wpa enable
 *
 *  @param priv                 A pointer to moal_private structure
 *  @param wait_option          Wait option
 *  @param enable               MTRUE or MFALSE
 *
 *  @return                     MLAN_STATUS_SUCCESS -- success, otherwise fail          
 */
mlan_status
woal_set_wpa_enable(moal_private * priv, t_u8 wait_option, t_u32 enable)
{
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_sec_cfg *sec = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;
    ENTER();

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_sec_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    sec = (mlan_ds_sec_cfg *) req->pbuf;
    sec->sub_command = MLAN_OID_SEC_CFG_WPA_ENABLED;
    req->req_id = MLAN_IOCTL_SEC_CFG;
    req->action = MLAN_ACT_SET;
    sec->param.wpa_enabled = enable;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
  done:
    if (req && (status != MLAN_STATUS_PENDING))
        kfree(req);
    LEAVE();
    return status;
}

/** 
 *  @brief Get WPA enable
 *
 *  @param priv                 A pointer to moal_private structure
 *  @param wait_option          Wait option
 *  @param enable               A pointer to wpa enable status
 *
 *  @return                     MLAN_STATUS_SUCCESS -- success, otherwise fail          
 */
mlan_status
woal_get_wpa_enable(moal_private * priv, t_u8 wait_option, t_u32 * enable)
{
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_sec_cfg *sec = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;

    ENTER();

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_sec_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    sec = (mlan_ds_sec_cfg *) req->pbuf;
    sec->sub_command = MLAN_OID_SEC_CFG_WPA_ENABLED;
    req->req_id = MLAN_IOCTL_SEC_CFG;
    req->action = MLAN_ACT_GET;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
    if (status == MLAN_STATUS_SUCCESS && enable) {
        *enable = sec->param.wpa_enabled;
    }
  done:
    if (req && (status != MLAN_STATUS_PENDING))
        kfree(req);
    LEAVE();
    return status;
}

/** 
 *  @brief Find the best network to associate
 *
 *  @param priv                 A pointer to moal_private structure
 *  @param wait_option          Wait option  
 *  @param ssid_bssid           A pointer to mlan_ssid_bssid structure
 *
 *  @return                     MLAN_STATUS_SUCCESS -- success, otherwise fail          
 */
mlan_status
woal_find_best_network(moal_private * priv, t_u8 wait_option,
                       mlan_ssid_bssid * ssid_bssid)
{
    mlan_ioctl_req *req = NULL;
    mlan_ds_bss *bss = NULL;
    mlan_status ret = MLAN_STATUS_SUCCESS;
    t_u8 *mac = 0;

    ENTER();

    if (!ssid_bssid) {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_bss));
    if (req == NULL) {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    /* Fill request buffer */
    bss = (mlan_ds_bss *) req->pbuf;
    req->req_id = MLAN_IOCTL_BSS;
    req->action = MLAN_ACT_GET;
    bss->sub_command = MLAN_OID_BSS_FIND_BSS;

    memcpy(&bss->param.ssid_bssid, ssid_bssid, sizeof(mlan_ssid_bssid));

    /* Send IOCTL request to MLAN */
    ret = woal_request_ioctl(priv, req, wait_option);
    if (ret == MLAN_STATUS_SUCCESS) {
        memcpy(ssid_bssid, &bss->param.ssid_bssid, sizeof(mlan_ssid_bssid));
        mac = (t_u8 *) & ssid_bssid->bssid;
        PRINTM(CMND, "Find network: ssid=%s, %02x:%02x:%02x:%02x:%02x:%02x\n",
               ssid_bssid->ssid.ssid, mac[0], mac[1], mac[2], mac[3], mac[4],
               mac[5]);
    }

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/** 
 *  @brief Request a scan
 *
 *  @param priv                 A pointer to moal_private structure
 *  @param wait_option          Wait option
 *  @param req_ssid             A pointer to mlan_802_11_ssid structure
 *  
 *  @return                     MLAN_STATUS_SUCCESS -- success, otherwise fail          
 */
mlan_status
woal_request_scan(moal_private * priv,
                  t_u8 wait_option, mlan_802_11_ssid * req_ssid)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_ioctl_req *ioctl_req = NULL;
    mlan_ds_scan *scan = NULL;

    ENTER();

    /* Allocate an IOCTL request buffer */
    ioctl_req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_scan));
    if (ioctl_req == NULL) {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    scan = (mlan_ds_scan *) ioctl_req->pbuf;

    if (req_ssid && req_ssid->ssid_len != 0) {
        /* Specific SSID scan */
        ioctl_req->req_id = MLAN_IOCTL_SCAN;
        ioctl_req->action = MLAN_ACT_SET;

        scan->sub_command = MLAN_OID_SCAN_SPECIFIC_SSID;

        memcpy(scan->param.scan_req.scan_ssid.ssid,
               req_ssid->ssid, req_ssid->ssid_len);
        scan->param.scan_req.scan_ssid.ssid_len = req_ssid->ssid_len;

        /* Send IOCTL request to MLAN */
        if (MLAN_STATUS_SUCCESS !=
            woal_request_ioctl(priv, ioctl_req, wait_option)) {
            ret = MLAN_STATUS_FAILURE;
            goto done;
        }
    } else {
        /* Normal scan */
        ioctl_req->req_id = MLAN_IOCTL_SCAN;
        ioctl_req->action = MLAN_ACT_SET;

        scan->sub_command = MLAN_OID_SCAN_NORMAL;

        /* Send IOCTL request to MLAN */
        if (MLAN_STATUS_SUCCESS !=
            woal_request_ioctl(priv, ioctl_req, wait_option)) {
            ret = MLAN_STATUS_FAILURE;
            goto done;
        }
    }

  done:
    if (ioctl_req)
        kfree(ioctl_req);

    LEAVE();
    return ret;
}

/** 
 *  @brief Set ewpa mode
 *
 *  @param priv                 A pointer to moal_private structure
 *  @param wait_option          Wait option
 *  @param ssid_bssid           A pointer to mlan_ssid_bssid structure
 *
 *  @return                     MLAN_STATUS_SUCCESS -- success, otherwise fail          
 */
mlan_status
woal_set_ewpa_mode(moal_private * priv, t_u8 wait_option,
                   mlan_ssid_bssid * ssid_bssid)
{
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_sec_cfg *sec = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;

    ENTER();

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_sec_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto error;
    }
    /* Fill request buffer */
    sec = (mlan_ds_sec_cfg *) req->pbuf;
    sec->sub_command = MLAN_OID_SEC_CFG_PASSPHRASE;
    req->req_id = MLAN_IOCTL_SEC_CFG;
    req->action = MLAN_ACT_GET;

    /* Try Get All */
    memset(&sec->param.passphrase, 0, sizeof(mlan_ds_passphrase));
    memcpy(&sec->param.passphrase.ssid, &ssid_bssid->ssid,
           sizeof(sec->param.passphrase.ssid));
    memcpy(&sec->param.passphrase.bssid, &ssid_bssid->bssid,
           MLAN_MAC_ADDR_LENGTH);
    sec->param.passphrase.psk_type = MLAN_PSK_QUERY;

    /* Send IOCTL request to MLAN */
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, wait_option))
        goto error;
    sec->param.ewpa_enabled = MFALSE;
    if (sec->param.passphrase.psk_type == MLAN_PSK_PASSPHRASE) {
        if (sec->param.passphrase.psk.passphrase.passphrase_len > 0) {
            sec->param.ewpa_enabled = MTRUE;
        }
    } else if (sec->param.passphrase.psk_type == MLAN_PSK_PMK)
        sec->param.ewpa_enabled = MTRUE;

    sec->sub_command = MLAN_OID_SEC_CFG_EWPA_ENABLED;
    req->action = MLAN_ACT_SET;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);

  error:
    if (req && (status != MLAN_STATUS_PENDING))
        kfree(req);
    LEAVE();
    return status;
}

/** 
 *  @brief Change Adhoc Channel
 *   
 *  @param priv 		A pointer to moal_private structure
 *  @param channel		The channel to be set. 
 *
 *  @return 	   		MLAN_STATUS_SUCCESS--success, MLAN_STATUS_FAILURE--fail
 */
mlan_status
woal_change_adhoc_chan(moal_private * priv, int channel)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_bss_info bss_info;
    mlan_ds_bss *bss = NULL;
    mlan_ioctl_req *req = NULL;

    ENTER();

    memset(&bss_info, 0, sizeof(bss_info));

    /* Get BSS information */
    if (MLAN_STATUS_SUCCESS !=
        woal_get_bss_info(priv, MOAL_IOCTL_WAIT, &bss_info)) {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }
    if (bss_info.bss_mode == MLAN_BSS_MODE_INFRA) {
        ret = MLAN_STATUS_SUCCESS;
        goto done;
    }

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_bss));
    if (req == NULL) {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    /* Get current channel */
    bss = (mlan_ds_bss *) req->pbuf;
    bss->sub_command = MLAN_OID_IBSS_CHANNEL;
    req->req_id = MLAN_IOCTL_BSS;
    req->action = MLAN_ACT_GET;

    /* Send IOCTL request to MLAN */
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }
    if (bss->param.bss_chan.channel == channel) {
        ret = MLAN_STATUS_SUCCESS;
        goto done;
    }
    PRINTM(CMND, "Updating Channel from %d to %d\n",
           (int) bss->param.bss_chan.channel, channel);

    /* Set channel */
    bss->param.bss_chan.channel = channel;
    req->action = MLAN_ACT_SET;

    /* Send IOCTL request to MLAN */
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }
    if (bss_info.media_connected != MTRUE) {
        ret = MLAN_STATUS_SUCCESS;
        goto done;
    }

    /* Do disonnect */
    bss->sub_command = MLAN_OID_BSS_STOP;
    memset((t_u8 *) & bss->param.bssid, 0, ETH_ALEN);

    /* Send IOCTL request to MLAN */
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    /* Do specific SSID scanning */
    if (MLAN_STATUS_SUCCESS !=
        woal_request_scan(priv, MOAL_IOCTL_WAIT, &bss_info.ssid)) {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }
    /* Start/Join Adhoc network */
    bss->sub_command = MLAN_OID_BSS_START;
    memset(&bss->param.ssid_bssid, 0, sizeof(mlan_ssid_bssid));
    memcpy(&bss->param.ssid_bssid.ssid, &bss_info.ssid,
           sizeof(mlan_802_11_ssid));

    /* Send IOCTL request to MLAN */
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = MLAN_STATUS_FAILURE;
    }

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/** 
 *  @brief Send deauth command to MLAN
 *   
 *  @param priv          A pointer to moal_private structure
 *  @param mac           MAC address to deauthenticate
 *
 *  @return              MLAN_STATUS_SUCCESS -- success, otherwise fail
 */
mlan_status
woal_disconnect(moal_private * priv, t_u8 * mac)
{
    mlan_ioctl_req *req = NULL;
    mlan_ds_bss *bss = NULL;
    mlan_status status;

    ENTER();

    /* Allocate an IOCTL request buffer */
    req = (mlan_ioctl_req *) woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_bss));
    if (req == NULL) {
        status = MLAN_STATUS_FAILURE;
        goto done;
    }

    /* Fill request buffer */
    bss = (mlan_ds_bss *) req->pbuf;
    bss->sub_command = MLAN_OID_BSS_STOP;
    if (mac)
        memcpy((t_u8 *) & bss->param.bssid, mac, sizeof(mlan_802_11_mac_addr));
    req->req_id = MLAN_IOCTL_BSS;
    req->action = MLAN_ACT_SET;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT);

  done:
    if (req)
        kfree(req);
#ifdef REASSOCIATION
    priv->reassoc_required = MFALSE;
#endif
    LEAVE();
    return status;
}

/** 
 *  @brief Send associate command to MLAN
 *   
 *  @param priv          A pointer to moal_private structure
 *  @param wait_option          Wait option  
 *  @param ssid_bssid    A point to mlan_ssid_bssid structure
 *
 *  @return              MLAN_STATUS_SUCCESS -- success, otherwise fail
 */
mlan_status
woal_associate(moal_private * priv, t_u8 wait_option,
               mlan_ssid_bssid * ssid_bssid)
{
    mlan_ioctl_req *req = NULL;
    mlan_ds_bss *bss = NULL;
    mlan_status status;

    ENTER();

    /* Allocate an IOCTL request buffer */
    req = (mlan_ioctl_req *) woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_bss));
    if (req == NULL) {
        status = MLAN_STATUS_FAILURE;
        goto done;
    }

    /* Fill request buffer */
    bss = (mlan_ds_bss *) req->pbuf;
    bss->sub_command = MLAN_OID_BSS_START;
    memcpy(&bss->param.ssid_bssid, ssid_bssid, sizeof(mlan_ssid_bssid));
    req->req_id = MLAN_IOCTL_BSS;
    req->action = MLAN_ACT_SET;
    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
  done:
    if (req)
        kfree(req);
    LEAVE();
    return status;
}

/** 
 *  @brief enable wep key 
 *  
 *  @param priv                 A pointer to moal_private structure
 *  @param wait_option          Wait option
 *
 *  @return                     MLAN_STATUS_SUCCESS -- success, otherwise fail          
 */
mlan_status
woal_enable_wep_key(moal_private * priv, t_u8 wait_option)
{
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_sec_cfg *sec = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;
    ENTER();

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_sec_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    sec = (mlan_ds_sec_cfg *) req->pbuf;
    sec->sub_command = MLAN_OID_SEC_CFG_ENCRYPT_KEY;
    req->req_id = MLAN_IOCTL_SEC_CFG;
    req->action = MLAN_ACT_SET;
    sec->param.encrypt_key.key_disable = MFALSE;
    sec->param.encrypt_key.key_len = 0;
    sec->param.encrypt_key.is_current_wep_key = MTRUE;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
  done:
    if (req && (status != MLAN_STATUS_PENDING))
        kfree(req);
    LEAVE();
    return status;
}
