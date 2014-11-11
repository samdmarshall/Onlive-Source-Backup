/** @file moal_ioctl.c
  *
  * @brief This file contains ioctl function to MLAN
  * 
  * Copyright (C) 2008-2010, Marvell International Ltd. 
  *
  * This software file (the "File") is distributed by Marvell International 
  * Ltd. under the terms of the GNU General Public License Version 2, June 1991 
  * (the "License").  You may use, redistribute and/or modify this File in 
  * accordance with the terms and conditions of the License, a copy of which 
  * is available by writing to the Free Software Foundation, Inc.,
  * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA or on the
  * worldwide web at http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt.
  *
  * THE FILE IS DISTRIBUTED AS-IS, WITHOUT WARRANTY OF ANY KIND, AND THE 
  * IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE 
  * ARE EXPRESSLY DISCLAIMED.  The License provides additional details about 
  * this warranty disclaimer.
  *
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
 *  @brief Copy multicast table
 *   
 *  @param mlist    A pointer to mlan_multicast_list structure
 *  @param dev      A pointer to net_device structure                 
 *
 *  @return         Number of mulitcast address
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
 *  @param priv         A pointer to moal_private structure
 *  @param wait         A pointer to wait_queue structure                 
 *  @param wait_option  Wait option
 *
 *  @return             None
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
#ifdef STA_SUPPORT
    case MOAL_WSTATS_WAIT:
        priv->w_stats_wait_q_woken = MFALSE;
        wait->wait = &priv->w_stats_wait_q;
        wait->condition = &priv->w_stats_wait_q_woken;
        break;
#endif
    }
    LEAVE();
    return;
}

/** 
 *  @brief Wait mlan ioctl complete
 *   
 *  @param priv         A pointer to moal_private structure
 *  @param req          A pointer to mlan_ioctl_req structure   
 *  @param wait_option  Wait option
 *
 *  @return             None
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
#ifdef STA_SUPPORT
    case MOAL_WSTATS_WAIT:
        wait_event_interruptible(priv->w_stats_wait_q,
                                 priv->w_stats_wait_q_woken);
        if (priv->w_stats_wait_q_woken == MFALSE)
            cancel_flag = MTRUE;
        break;
#endif
    }
    if (cancel_flag == MTRUE) {
        req->action = MLAN_ACT_CANCEL;
        mlan_ioctl(priv->phandle->pmlan_adapter, req);
        PRINTM(MIOCTL,
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
 *  @param priv   A pointer to moal_private structure
 *
 *  @return       MLAN_STATUS_SUCCESS
 */
mlan_status
woal_request_open(moal_private * priv)
{
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Send close request to MLAN
 *   
 *  @param priv   A pointer to moal_private structure
 *
 *  @return       MLAN_STATUS_SUCCESS
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
        PRINTM(MERROR,
               "IOCTL is not allowed while the device is not present\n");
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }
#if defined(SDIO_SUSPEND_RESUME)
    if (priv->phandle->is_suspended == MTRUE) {
        PRINTM(MERROR, "IOCTL is not allowed while suspended\n");
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }
#endif

    wait = (wait_queue *) req->reserved_1;
    req->bss_num = priv->bss_index;
    if (wait_option)
        woal_fill_wait_queue(priv, wait, wait_option);
    else
        req->reserved_1 = 0;

    /* Call MLAN ioctl handle */
    status = mlan_ioctl(priv->phandle->pmlan_adapter, req);
    switch (status) {
    case MLAN_STATUS_PENDING:
        PRINTM(MIOCTL,
               "IOCTL pending: %p id=0x%lx, sub_id=0x%lx wait_option=%d, action=%d\n",
               req, req->req_id, (*(t_u32 *) req->pbuf), wait_option,
               (int) req->action);
        atomic_inc(&priv->phandle->ioctl_pending);
        /* Status pending, wake up main process */
        queue_work(priv->phandle->workqueue, &priv->phandle->main_work);

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
 *  @param priv   A pointer to moal_private structure
 *
 *  @return       MLAN_STATUS_SUCCESS -- success, otherwise fail
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
        PRINTM(MERROR, "set mac address failed! status=%d, error_code=0x%lx\n",
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
 *  @param priv   A pointer to moal_private structure
 *  @param dev    A pointer to net_device structure                 
 *
 *  @return       None
 */
void
woal_request_set_multicast_list(moal_private * priv, struct net_device *dev)
{
    mlan_ioctl_req *req = NULL;
    mlan_ds_bss *bss = NULL;
    mlan_status status;
    ENTER();

    /* Allocate an IOCTL request buffer */
    req = (mlan_ioctl_req *) woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_bss));
    if (req == NULL) {
        PRINTM(MERROR, "%s:Fail to allocate ioctl req buffer\n", __FUNCTION__);
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
 *  @brief Send deauth command to MLAN
 *   
 *  @param priv          A pointer to moal_private structure
 *  @param wait_option          Wait option
 *  @param mac           MAC address to deauthenticate
 *
 *  @return              MLAN_STATUS_SUCCESS -- success, otherwise fail
 */
mlan_status
woal_disconnect(moal_private * priv, t_u8 wait_option, t_u8 * mac)
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
    status = woal_request_ioctl(priv, req, wait_option);

  done:
    if (req)
        kfree(req);
#ifdef REASSOCIATION
    priv->reassoc_required = MFALSE;
#endif /* REASSOCIATION */
    LEAVE();
    return status;
}

/** 
 *  @brief Send bss_start command to MLAN
 *   
 *  @param priv          A pointer to moal_private structure
 *  @param wait_option          Wait option  
 *  @param ssid_bssid    A point to mlan_ssid_bssid structure
 *
 *  @return              MLAN_STATUS_SUCCESS -- success, otherwise fail
 */
mlan_status
woal_bss_start(moal_private * priv, t_u8 wait_option,
               mlan_ssid_bssid * ssid_bssid)
{
    mlan_ioctl_req *req = NULL;
    mlan_ds_bss *bss = NULL;
    mlan_status status;

    ENTER();

    /* Stop the O.S. TX queue if needed */
    if (!netif_queue_stopped(priv->netdev))
        netif_stop_queue(priv->netdev);

    /* Allocate an IOCTL request buffer */
    req = (mlan_ioctl_req *) woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_bss));
    if (req == NULL) {
        status = MLAN_STATUS_FAILURE;
        goto done;
    }

    /* Fill request buffer */
    bss = (mlan_ds_bss *) req->pbuf;
    bss->sub_command = MLAN_OID_BSS_START;
    if (ssid_bssid)
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

/** 
 *  @brief Send get FW info request to MLAN
 *   
 *  @param priv             A pointer to moal_private structure
 *  @param wait_option      Wait option  
 *
 *  @return      None
 */
void
woal_request_get_fw_info(moal_private * priv, t_u8 wait_option)
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
    status = woal_request_ioctl(priv, req, wait_option);
    if (status == MLAN_STATUS_SUCCESS) {
        priv->phandle->fw_release_number = info->param.fw_info.fw_ver;
        if (priv->current_addr[0] == 0xff)
            memcpy(priv->current_addr, &info->param.fw_info.mac_addr,
                   sizeof(mlan_802_11_mac_addr));
        memcpy(priv->netdev->dev_addr, priv->current_addr, ETH_ALEN);
        DBG_HEXDUMP(MIF_D, "mac", priv->current_addr, 6);
    } else
        PRINTM(MERROR, "get fw info failed! status=%d, error_code=0x%lx\n",
               status, req->status_code);
  done:
    if (req)
        kfree(req);
    LEAVE();
    return;
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

#if defined(UAP_SUPPORT)
/** 
 *  @brief host command ioctl function
 *   
 *  @param dev      A pointer to net_device structure
 *  @param req      A pointer to ifreq structure
 *  @return         0 --success, otherwise fail
 */
/*********  format of ifr_data *************/
/*    buf_len + Hostcmd_body 		   */
/*    buf_len: 4 bytes                     */
/*             the length of the buf which */
/*             can be used to return data  */
/*             to application		   */
/*    Hostcmd_body       	           */
/*******************************************/
int
woal_hostcmd_ioctl(struct net_device *dev, struct ifreq *req)
{
    moal_private *priv = (moal_private *) netdev_priv(dev);
    t_u32 buf_len = 0;
    HostCmd_Header cmd_header;
    int ret = 0;
    mlan_ds_misc_cfg *misc = NULL;
    mlan_ioctl_req *ioctl_req = NULL;

    ENTER();

    /* Sanity check */
    if (req->ifr_data == NULL) {
        PRINTM(MERROR, "uap_hostcmd_ioctl() corrupt data\n");
        ret = -EFAULT;
        goto done;
    }

    if (copy_from_user(&buf_len, req->ifr_data, sizeof(buf_len))) {
        PRINTM(MERROR, "Copy from user failed\n");
        ret = -EFAULT;
        goto done;
    }

    memset(&cmd_header, 0, sizeof(cmd_header));

    /* get command header */
    if (copy_from_user
        (&cmd_header, req->ifr_data + sizeof(buf_len),
         sizeof(HostCmd_Header))) {
        PRINTM(MERROR, "Copy from user failed\n");
        ret = -EFAULT;
        goto done;
    }

    PRINTM(MINFO, "Host command len = %d\n", woal_le16_to_cpu(cmd_header.size));

    if (woal_le16_to_cpu(cmd_header.size) > MLAN_SIZE_OF_CMD_BUFFER) {
        ret = -EINVAL;
        goto done;
    }

    ioctl_req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_misc_cfg));
    if (ioctl_req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    misc = (mlan_ds_misc_cfg *) ioctl_req->pbuf;

    misc->param.hostcmd.len = woal_le16_to_cpu(cmd_header.size);

    /* get the whole command from user */
    if (copy_from_user
        (misc->param.hostcmd.cmd, req->ifr_data + sizeof(buf_len),
         misc->param.hostcmd.len)) {
        PRINTM(MERROR, "copy from user failed\n");
        ret = -EFAULT;
        goto done;
    }
    misc->sub_command = MLAN_OID_MISC_HOST_CMD;
    ioctl_req->req_id = MLAN_IOCTL_MISC_CFG;

    if (MLAN_STATUS_SUCCESS !=
        woal_request_ioctl(priv, ioctl_req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    if (misc->param.hostcmd.len > buf_len) {
        PRINTM(MERROR, "buf_len is too small, resp_len=%d, buf_len=%d\n",
               (int) misc->param.hostcmd.len, (int) buf_len);
        ret = -EFAULT;
        goto done;
    }
    if (copy_to_user
        (req->ifr_data + sizeof(buf_len), (t_u8 *) misc->param.hostcmd.cmd,
         misc->param.hostcmd.len)) {
        ret = -EFAULT;
        goto done;
    }
  done:
    if (ioctl_req)
        kfree(ioctl_req);
    LEAVE();
    return ret;
}
#endif

/** 
 *  @brief CUSTOM_IE ioctl handler
 *   
 *  @param dev      A pointer to net_device structure
 *  @param req      A pointer to ifreq structure
 *  @return         0 --success, otherwise fail
 */
int
woal_custom_ie_ioctl(struct net_device *dev, struct ifreq *req)
{
    moal_private *priv = (moal_private *) netdev_priv(dev);
    mlan_ioctl_req *ioctl_req = NULL;
    mlan_ds_misc_cfg *misc = NULL;
    mlan_ds_misc_custom_ie *custom_ie = NULL;
    int ret = 0;

    ENTER();

    /* Sanity check */
    if (req->ifr_data == NULL) {
        PRINTM(MERROR, "woal_custom_ie_ioctl() corrupt data\n");
        ret = -EFAULT;
        goto done;
    }

    if (!(custom_ie = kmalloc(sizeof(mlan_ds_misc_custom_ie), GFP_KERNEL))) {
        ret = -ENOMEM;
        goto done;
    }
    memset(custom_ie, 0, sizeof(mlan_ds_misc_custom_ie));

    if (copy_from_user
        (custom_ie, req->ifr_data, sizeof(mlan_ds_misc_custom_ie))) {
        PRINTM(MERROR, "Copy from user failed\n");
        ret = -EFAULT;
        goto done;
    }

    ioctl_req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_misc_cfg));
    if (ioctl_req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    misc = (mlan_ds_misc_cfg *) ioctl_req->pbuf;
    misc->sub_command = MLAN_OID_MISC_CUSTOM_IE;
    ioctl_req->req_id = MLAN_IOCTL_MISC_CFG;
    if ((custom_ie->len == 0) ||
        (custom_ie->len == sizeof(custom_ie->ie_data_list[0].ie_index)))
        ioctl_req->action = MLAN_ACT_GET;
    else
        ioctl_req->action = MLAN_ACT_SET;

    memcpy(&misc->param.cust_ie, custom_ie, sizeof(mlan_ds_misc_custom_ie));

    if (MLAN_STATUS_SUCCESS !=
        woal_request_ioctl(priv, ioctl_req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    if (ioctl_req->action == MLAN_ACT_GET) {
        if (copy_to_user
            (req->ifr_data, &misc->param.cust_ie,
             sizeof(mlan_ds_misc_custom_ie))) {
            PRINTM(MERROR, "Copy to user failed!\n");
            ret = -EFAULT;
            goto done;
        }
    }

  done:
    if (ioctl_req)
        kfree(ioctl_req);
    if (custom_ie)
        kfree(custom_ie);
    LEAVE();
    return ret;
}

/** 
 *  @brief ioctl function get BSS type
 *   
 *  @param dev      A pointer to net_device structure
 *  @param req      A pointer to ifreq structure
 *  @return         MLAN_STATUS_SUCCESS -- success, otherwise fail
 */
mlan_status
woal_get_bss_type(struct net_device * dev, struct ifreq * req)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    moal_private *priv = (moal_private *) netdev_priv(dev);
    int bss_type;

    ENTER();

    bss_type = (int) priv->bss_type;
    if (copy_to_user(req->ifr_data, &bss_type, sizeof(int))) {
        PRINTM(MINFO, "Copy to user failed!\n");
        ret = -EFAULT;
    }

    LEAVE();
    return ret;
}

/** 
 *  @brief Get Host Sleep parameters
 *
 *  @param priv         A pointer to moal_private structure
 *  @param wait_option  Wait option (MOAL_WAIT or MOAL_NO_WAIT)   
 *  @param hscfg        A pointer to mlan_ds_hs_cfg structure
 *
 *  @return             MLAN_STATUS_SUCCESS -- success, otherwise fail          
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
 *  @param priv             A pointer to moal_private structure
 *  @param wait_option      wait option
 *
 *  @return      MLAN_STATUS_SUCCESS, MLAN_STATUS_PENDING,
 *                      or MLAN_STATUS_FAILURE          
 */
mlan_status
woal_hs_cfg_cancel(moal_private * priv, t_u8 wait_option)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_ioctl_req *req = NULL;
    mlan_ds_pm_cfg *pmcfg = NULL;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_pm_cfg));
    if (req == NULL) {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    pmcfg = (mlan_ds_pm_cfg *) req->pbuf;
    pmcfg->sub_command = MLAN_OID_PM_CFG_HS_CFG;
    req->req_id = MLAN_IOCTL_PM_CFG;
    req->action = MLAN_ACT_SET;

    /* Get current Host Sleep configuration */
    woal_get_hs_params(priv, wait_option, &pmcfg->param.hs_cfg);

    pmcfg->param.hs_cfg.conditions = HOST_SLEEP_CFG_CANCEL;
    pmcfg->param.hs_cfg.is_invoke_hostcmd = MTRUE;

    ret = woal_request_ioctl(priv, req, wait_option);

    if (ret != MLAN_STATUS_PENDING)
        kfree(req);

  done:
    LEAVE();
    return ret;
}

#if defined(SDIO_SUSPEND_RESUME)
/**  @brief This function enables the host sleep
 *  
 *  @param priv   A Pointer to the moal_private structure
 *  @return 	  MTRUE or MFALSE
 */
int
woal_enable_hs(moal_private * priv)
{
    mlan_ioctl_req *req = NULL;
    mlan_ds_pm_cfg *pmcfg = NULL;
    moal_handle *handle = NULL;
    int hs_actived = MFALSE;

    ENTER();

    if (priv == NULL) {
        PRINTM(MERROR, "Invalid priv\n");
        goto done;
    }
    handle = priv->phandle;
    if (handle->hs_activated == MTRUE) {
        PRINTM(MIOCTL, "HS Already actived\n");
        hs_actived = MTRUE;
        goto done;
    }

    /* Enable Host Sleep */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_pm_cfg));
    if (req == NULL) {
        PRINTM(MERROR, "Falied to allocate buffer for IOCTL\n");
        goto done;
    }
    pmcfg = (mlan_ds_pm_cfg *) req->pbuf;
    pmcfg->sub_command = MLAN_OID_PM_CFG_HS_CFG;
    req->req_id = MLAN_IOCTL_PM_CFG;
    req->action = MLAN_ACT_SET;

    if (woal_get_hs_params(priv, MOAL_IOCTL_WAIT, &(pmcfg->param.hs_cfg)) ||
        (pmcfg->param.hs_cfg.conditions == HOST_SLEEP_CFG_CANCEL))
        goto done;

    handle->hs_activate_wait_q_woken = MFALSE;
    pmcfg->param.hs_cfg.is_invoke_hostcmd = MTRUE;
    if (woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT) != MLAN_STATUS_SUCCESS) {
        PRINTM(MERROR, "IOCTL request failed\n");
        goto done;
    }

    if (pmcfg->param.hs_cfg.conditions != HOST_SLEEP_CFG_CANCEL) {
        wait_event_interruptible(handle->hs_activate_wait_q,
                                 handle->hs_activate_wait_q_woken);
        hs_actived = MTRUE;
    }

  done:
    if (req)
        kfree(req);

    LEAVE();
    return hs_actived;
}
#endif

/** 
 *  @brief This function send soft_reset command to firmware 
 *  
 *  @param handle    A pointer to moal_handle structure
 *  @return 	   UAP_STATUS_SUCCESS on success, otherwise failure code
 */
mlan_status
woal_request_soft_reset(moal_handle * handle)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_ioctl_req *req = NULL;
    mlan_ds_misc_cfg *misc = NULL;
    mlan_bss_info bss_info;
    moal_private *priv = NULL;
    int i;

    ENTER();
    /* Stop data */
    for (i = 0; i < handle->priv_num; i++) {
        priv = handle->priv[i];
        if (!netif_queue_stopped(priv->netdev))
            netif_stop_queue(priv->netdev);
        if (netif_carrier_ok(priv->netdev))
            netif_carrier_off(priv->netdev);
        netif_device_detach(priv->netdev);
        woal_disconnect(priv, MOAL_PROC_WAIT, NULL);
        priv->media_connected = MFALSE;
    }
    priv = woal_get_priv(handle, MLAN_BSS_TYPE_ANY);
    memset(&bss_info, 0, sizeof(bss_info));
    woal_get_bss_info(priv, MOAL_PROC_WAIT, &bss_info);
#ifdef STA_SUPPORT
    if (bss_info.is_deep_sleep) {
        woal_set_deep_sleep(priv, MOAL_PROC_WAIT, MFALSE, 0);
    }
#endif /* STA_SUPPORT */
    if (bss_info.is_hs_configured)
        woal_hs_cfg_cancel(priv, MOAL_PROC_WAIT);
#ifdef REASSOCIATION
    if (handle->is_reassoc_timer_set) {
        woal_cancel_timer(&handle->reassoc_timer);
        handle->is_reassoc_timer_set = MFALSE;
    }
#endif /* REASSOCIATION */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_misc_cfg));
    if (req) {
        misc = (mlan_ds_misc_cfg *) req->pbuf;
        misc->sub_command = MLAN_OID_MISC_SOFT_RESET;
        req->req_id = MLAN_IOCTL_MISC_CFG;
        req->action = MLAN_ACT_SET;
        ret =
            woal_request_ioctl(woal_get_priv(handle, MLAN_BSS_TYPE_ANY), req,
                               MOAL_PROC_WAIT);
    }

    handle->surprise_removed = MTRUE;
    woal_sched_timeout(5);
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/** 
 *  @brief Set wapi enable
 *
 *  @param priv                 A pointer to moal_private structure
 *  @param wait_option          Wait option
 *  @param enable               MTRUE or MFALSE
 *
 *  @return                     MLAN_STATUS_SUCCESS -- success, otherwise fail          
 */
mlan_status
woal_set_wapi_enable(moal_private * priv, t_u8 wait_option, t_u32 enable)
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
    sec->sub_command = MLAN_OID_SEC_CFG_WAPI_ENABLED;
    req->req_id = MLAN_IOCTL_SEC_CFG;
    req->action = MLAN_ACT_SET;
    sec->param.wapi_enabled = enable;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);
  done:
    if (req && (status != MLAN_STATUS_PENDING))
        kfree(req);
    LEAVE();
    return status;
}
