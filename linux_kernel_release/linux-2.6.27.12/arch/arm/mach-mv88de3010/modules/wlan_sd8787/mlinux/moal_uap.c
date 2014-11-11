/** @file moal_uap.c
  *
  * @brief This file contains the major functions in UAP
  * driver. 
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

#include	"moal_main.h"
#include    "moal_uap.h"
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
 *  @brief uap addba parameter handler
 *   
 *  @param dev      A pointer to net_device structure
 *  @param req      A pointer to ifreq structure
 *  @return         0 --success, otherwise fail
 */
static int
woal_uap_addba_param(struct net_device *dev, struct ifreq *req)
{
    moal_private *priv = (moal_private *) netdev_priv(dev);
    mlan_ioctl_req *ioctl_req = NULL;
    mlan_ds_11n_cfg *cfg_11n = NULL;
    addba_param param;
    int ret = 0;

    ENTER();
    memset(&param, 0, sizeof(param));

    /* Sanity check */
    if (req->ifr_data == NULL) {
        PRINTM(MERROR, "uap_addba_param() corrupt data\n");
        ret = -EFAULT;
        goto done;
    }
    if (copy_from_user(&param, req->ifr_data, sizeof(param))) {
        PRINTM(MERROR, "Copy from user failed\n");
        ret = -EFAULT;
        goto done;
    }
    PRINTM(MIOCTL,
           "addba param: action=%d, timeout=%d, txwinsize=%d, rxwinsize=%d\n",
           (int) param.action, (int) param.timeout, (int) param.txwinsize,
           (int) param.rxwinsize);

    ioctl_req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_11n_cfg));
    if (ioctl_req == NULL) {
        LEAVE();
        return -ENOMEM;
    }
    cfg_11n = (mlan_ds_11n_cfg *) ioctl_req->pbuf;
    cfg_11n->sub_command = MLAN_OID_11N_CFG_ADDBA_PARAM;
    ioctl_req->req_id = MLAN_IOCTL_11N_CFG;

    if (!param.action) {
        /* Get addba param from MLAN */
        ioctl_req->action = MLAN_ACT_GET;
    } else {
        /* Set addba param in MLAN */
        ioctl_req->action = MLAN_ACT_SET;
        cfg_11n->param.addba_param.timeout = param.timeout;
        cfg_11n->param.addba_param.txwinsize = param.txwinsize;
        cfg_11n->param.addba_param.rxwinsize = param.rxwinsize;
    }
    if (MLAN_STATUS_SUCCESS !=
        woal_request_ioctl(priv, ioctl_req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    param.timeout = cfg_11n->param.addba_param.timeout;
    param.txwinsize = cfg_11n->param.addba_param.txwinsize;
    param.rxwinsize = cfg_11n->param.addba_param.rxwinsize;

    /* Copy to user */
    if (copy_to_user(req->ifr_data, &param, sizeof(param))) {
        PRINTM(MERROR, "Copy to user failed!\n");
        ret = -EFAULT;
        goto done;
    }
  done:
    if (ioctl_req)
        kfree(ioctl_req);
    LEAVE();
    return ret;
}

/** 
 *  @brief uap aggr priority tbl 
 *   
 *  @param dev      A pointer to net_device structure
 *  @param req      A pointer to ifreq structure
 *  @return         0 --success, otherwise fail
 */
static int
woal_uap_aggr_priotbl(struct net_device *dev, struct ifreq *req)
{
    moal_private *priv = (moal_private *) netdev_priv(dev);
    mlan_ioctl_req *ioctl_req = NULL;
    mlan_ds_11n_cfg *cfg_11n = NULL;
    aggr_prio_tbl param;
    int ret = 0;
    int i = 0;

    ENTER();
    memset(&param, 0, sizeof(param));

    /* Sanity check */
    if (req->ifr_data == NULL) {
        PRINTM(MERROR, "woal_uap_aggr_priotbl() corrupt data\n");
        ret = -EFAULT;
        goto done;
    }
    if (copy_from_user(&param, req->ifr_data, sizeof(param))) {
        PRINTM(MERROR, "Copy from user failed\n");
        ret = -EFAULT;
        goto done;
    }
    DBG_HEXDUMP(MCMD_D, "aggr_prio_tbl", (t_u8 *) & param, sizeof(param));

    ioctl_req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_11n_cfg));
    if (ioctl_req == NULL) {
        LEAVE();
        return -ENOMEM;
    }
    cfg_11n = (mlan_ds_11n_cfg *) ioctl_req->pbuf;
    cfg_11n->sub_command = MLAN_OID_11N_CFG_AGGR_PRIO_TBL;
    ioctl_req->req_id = MLAN_IOCTL_11N_CFG;

    if (!param.action) {
        /* Get aggr_prio_tbl from MLAN */
        ioctl_req->action = MLAN_ACT_GET;
    } else {
        /* Set aggr_prio_tbl in MLAN */
        ioctl_req->action = MLAN_ACT_SET;
        for (i = 0; i < MAX_NUM_TID; i++) {
            cfg_11n->param.aggr_prio_tbl.ampdu[i] = param.ampdu[i];
            cfg_11n->param.aggr_prio_tbl.amsdu[i] = param.amsdu[i];
        }
    }
    if (MLAN_STATUS_SUCCESS !=
        woal_request_ioctl(priv, ioctl_req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    for (i = 0; i < MAX_NUM_TID; i++) {
        param.ampdu[i] = cfg_11n->param.aggr_prio_tbl.ampdu[i];
        param.amsdu[i] = cfg_11n->param.aggr_prio_tbl.amsdu[i];
    }
    /* Copy to user */
    if (copy_to_user(req->ifr_data, &param, sizeof(param))) {
        PRINTM(MERROR, "Copy to user failed!\n");
        ret = -EFAULT;
        goto done;
    }
  done:
    if (ioctl_req)
        kfree(ioctl_req);
    LEAVE();
    return ret;
}

/** 
 *  @brief uap addba reject tbl 
 *   
 *  @param dev      A pointer to net_device structure
 *  @param req      A pointer to ifreq structure
 *  @return         0 --success, otherwise fail
 */
static int
woal_uap_addba_reject(struct net_device *dev, struct ifreq *req)
{
    moal_private *priv = (moal_private *) netdev_priv(dev);
    mlan_ioctl_req *ioctl_req = NULL;
    mlan_ds_11n_cfg *cfg_11n = NULL;
    addba_reject_para param;
    int ret = 0;
    int i = 0;

    ENTER();
    memset(&param, 0, sizeof(param));

    /* Sanity check */
    if (req->ifr_data == NULL) {
        PRINTM(MERROR, "woal_uap_addba_reject() corrupt data\n");
        ret = -EFAULT;
        goto done;
    }
    if (copy_from_user(&param, req->ifr_data, sizeof(param))) {
        PRINTM(MERROR, "Copy from user failed\n");
        ret = -EFAULT;
        goto done;
    }
    DBG_HEXDUMP(MCMD_D, "addba_reject tbl", (t_u8 *) & param, sizeof(param));

    ioctl_req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_11n_cfg));
    if (ioctl_req == NULL) {
        LEAVE();
        return -ENOMEM;
    }
    cfg_11n = (mlan_ds_11n_cfg *) ioctl_req->pbuf;
    cfg_11n->sub_command = MLAN_OID_11N_CFG_ADDBA_REJECT;
    ioctl_req->req_id = MLAN_IOCTL_11N_CFG;

    if (!param.action) {
        /* Get addba_reject tbl from MLAN */
        ioctl_req->action = MLAN_ACT_GET;
    } else {
        /* Set addba_reject tbl in MLAN */
        ioctl_req->action = MLAN_ACT_SET;
        for (i = 0; i < MAX_NUM_TID; i++) {
            cfg_11n->param.addba_reject[i] = param.addba_reject[i];
        }
    }
    if (MLAN_STATUS_SUCCESS !=
        woal_request_ioctl(priv, ioctl_req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    for (i = 0; i < MAX_NUM_TID; i++) {
        param.addba_reject[i] = cfg_11n->param.addba_reject[i];
    }
    /* Copy to user */
    if (copy_to_user(req->ifr_data, &param, sizeof(param))) {
        PRINTM(MERROR, "Copy to user failed!\n");
        ret = -EFAULT;
        goto done;
    }
  done:
    if (ioctl_req)
        kfree(ioctl_req);
    LEAVE();
    return ret;
}

/** 
 *  @brief configure deep sleep 
 *   
 *  @param dev      A pointer to net_device structure
 *  @param req      A pointer to ifreq structure
 *  @return         0 --success, otherwise fail
 */
static int
woal_uap_deep_sleep(struct net_device *dev, struct ifreq *req)
{
    moal_private *priv = (moal_private *) netdev_priv(dev);
    mlan_ioctl_req *ioctl_req = NULL;
    mlan_ds_pm_cfg *pm = NULL;
    deep_sleep_para param;
    int ret = 0;

    ENTER();
    memset(&param, 0, sizeof(param));

    /* Sanity check */
    if (req->ifr_data == NULL) {
        PRINTM(MERROR, "woal_uap_deep_sleep() corrupt data\n");
        ret = -EFAULT;
        goto done;
    }
    if (copy_from_user(&param, req->ifr_data, sizeof(param))) {
        PRINTM(MERROR, "Copy from user failed\n");
        ret = -EFAULT;
        goto done;
    }
    DBG_HEXDUMP(MCMD_D, "deep_sleep_para", (t_u8 *) & param, sizeof(param));

    ioctl_req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_pm_cfg));
    if (ioctl_req == NULL) {
        LEAVE();
        return -ENOMEM;
    }
    pm = (mlan_ds_pm_cfg *) ioctl_req->pbuf;
    pm->sub_command = MLAN_OID_PM_CFG_DEEP_SLEEP;
    ioctl_req->req_id = MLAN_IOCTL_PM_CFG;

    if (!param.action) {
        /* Get deep_sleep status from MLAN */
        ioctl_req->action = MLAN_ACT_GET;
    } else {
        /* Set deep_sleep in MLAN */
        ioctl_req->action = MLAN_ACT_SET;
        if (param.deep_sleep == MTRUE) {
            pm->param.auto_deep_sleep.auto_ds = DEEP_SLEEP_ON;
            pm->param.auto_deep_sleep.idletime = param.idle_time;
        } else {
            pm->param.auto_deep_sleep.auto_ds = DEEP_SLEEP_OFF;
        }
    }
    if (MLAN_STATUS_SUCCESS !=
        woal_request_ioctl(priv, ioctl_req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    if (pm->param.auto_deep_sleep.auto_ds == DEEP_SLEEP_ON)
        param.deep_sleep = MTRUE;
    else
        param.deep_sleep = MFALSE;
    param.idle_time = pm->param.auto_deep_sleep.idletime;
    /* Copy to user */
    if (copy_to_user(req->ifr_data, &param, sizeof(param))) {
        PRINTM(MERROR, "Copy to user failed!\n");
        ret = -EFAULT;
        goto done;
    }
  done:
    if (ioctl_req)
        kfree(ioctl_req);
    LEAVE();
    return ret;
}

/** 
 *  @brief uap ioctl handler
 *   
 *  @param dev      A pointer to net_device structure
 *  @param req      A pointer to ifreq structure
 *  @return         0 --success, otherwise fail
 */
static int
woal_uap_ioctl(struct net_device *dev, struct ifreq *req)
{
    int ret = 0;
    t_u32 subcmd = 0;
    ENTER();
    /* Sanity check */
    if (req->ifr_data == NULL) {
        PRINTM(MERROR, "uap_bss_ctrl() corrupt data\n");
        ret = -EFAULT;
        goto done;
    }
    if (copy_from_user(&subcmd, req->ifr_data, sizeof(subcmd))) {
        PRINTM(MERROR, "Copy from user failed\n");
        ret = -EFAULT;
        goto done;
    }

    PRINTM(MIOCTL, "ioctl subcmd=%d\n", (int) subcmd);
    switch (subcmd) {
    case UAP_ADDBA_PARA:
        ret = woal_uap_addba_param(dev, req);
        break;
    case UAP_AGGR_PRIOTBL:
        ret = woal_uap_aggr_priotbl(dev, req);
        break;
    case UAP_ADDBA_REJECT:
        ret = woal_uap_addba_reject(dev, req);
        break;
    case UAP_DEEP_SLEEP:
        ret = woal_uap_deep_sleep(dev, req);
    default:
        break;
    }
  done:
    LEAVE();
    return ret;
}

/** 
 *  @brief uap station deauth ioctl handler
 *   
 *  @param dev      A pointer to net_device structure
 *  @param req      A pointer to ifreq structure
 *  @return         0 --success, otherwise fail
 */
static int
woal_uap_sta_deauth_ioctl(struct net_device *dev, struct ifreq *req)
{
    moal_private *priv = (moal_private *) netdev_priv(dev);
    mlan_ioctl_req *ioctl_req = NULL;
    mlan_ds_bss *bss = NULL;
    mlan_deauth_param deauth_param;
    int ret = 0;

    ENTER();

    memset(&deauth_param, 0, sizeof(mlan_deauth_param));
    /* Sanity check */
    if (req->ifr_data == NULL) {
        PRINTM(MERROR, "uap_sta_deauth_ioctl() corrupt data\n");
        ret = -EFAULT;
        goto done;
    }
    if (copy_from_user(&deauth_param, req->ifr_data, sizeof(mlan_deauth_param))) {
        PRINTM(MERROR, "Copy from user failed\n");
        ret = -EFAULT;
        goto done;
    }

    PRINTM(MIOCTL,
           "ioctl deauth station: %02x:%02x:%02x:%02x:%02x:%02x, reason=%d\n",
           deauth_param.mac_addr[0], deauth_param.mac_addr[1],
           deauth_param.mac_addr[2], deauth_param.mac_addr[3],
           deauth_param.mac_addr[4], deauth_param.mac_addr[5],
           deauth_param.reason_code);

    ioctl_req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_bss));
    if (ioctl_req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    bss = (mlan_ds_bss *) ioctl_req->pbuf;

    bss->sub_command = MLAN_OID_UAP_DEAUTH_STA;
    ioctl_req->req_id = MLAN_IOCTL_BSS;
    ioctl_req->action = MLAN_ACT_SET;

    memcpy(&bss->param.deauth_param, &deauth_param, sizeof(mlan_deauth_param));
    if (MLAN_STATUS_SUCCESS !=
        woal_request_ioctl(priv, ioctl_req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        if (copy_to_user(req->ifr_data, &ioctl_req->status_code, sizeof(t_u32)))
            PRINTM(MERROR, "Copy to user failed!\n");
        goto done;
    }

  done:
    if (ioctl_req)
        kfree(ioctl_req);
    LEAVE();
    return ret;
}

/** 
 *  @brief uap bss control ioctl handler
 *   
 *  @param dev      A pointer to net_device structure
 *  @param req      A pointer to ifreq structure
 *  @return         0 --success, otherwise fail
 */
static int
woal_uap_bss_ctrl(struct net_device *dev, struct ifreq *req)
{
    moal_private *priv = (moal_private *) netdev_priv(dev);
    mlan_ioctl_req *ioctl_req = NULL;
    mlan_ds_bss *bss = NULL;
    t_u32 data = 0;
    int ret = 0;

    ENTER();

    /* Sanity check */
    if (req->ifr_data == NULL) {
        PRINTM(MERROR, "uap_bss_ctrl() corrupt data\n");
        ret = -EFAULT;
        goto done;
    }
    if (copy_from_user(&data, req->ifr_data, sizeof(data))) {
        PRINTM(MERROR, "Copy from user failed\n");
        ret = -EFAULT;
        goto done;
    }

    PRINTM(MIOCTL, "ioctl bss ctrl=%d\n", (int) data);
    if ((data != UAP_BSS_START) && (data != UAP_BSS_STOP) &&
        (data != UAP_BSS_RESET)) {
        PRINTM(MERROR, "Invalid parameter: %d\n", (int) data);
        ret = -EINVAL;
        goto done;
    }

    ioctl_req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_bss));
    if (ioctl_req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    bss = (mlan_ds_bss *) ioctl_req->pbuf;
    switch (data) {
    case UAP_BSS_START:
        bss->sub_command = MLAN_OID_BSS_START;
        break;
    case UAP_BSS_STOP:
        bss->sub_command = MLAN_OID_BSS_STOP;
        break;
    case UAP_BSS_RESET:
        bss->sub_command = MLAN_OID_UAP_BSS_RESET;
        break;
    }
    ioctl_req->req_id = MLAN_IOCTL_BSS;
    ioctl_req->action = MLAN_ACT_SET;

    if (MLAN_STATUS_SUCCESS !=
        woal_request_ioctl(priv, ioctl_req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        if (copy_to_user(req->ifr_data, &ioctl_req->status_code, sizeof(t_u32)))
            PRINTM(MERROR, "Copy to user failed!\n");
        goto done;
    }
  done:
    if (ioctl_req)
        kfree(ioctl_req);

    LEAVE();
    return ret;
}

/** 
 *  @brief uap power mode ioctl handler
 *   
 *  @param dev      A pointer to net_device structure
 *  @param req      A pointer to ifreq structure
 *  @return         0 --success, otherwise fail
 */
static int
woal_uap_power_mode_ioctl(struct net_device *dev, struct ifreq *req)
{
    moal_private *priv = (moal_private *) netdev_priv(dev);
    mlan_ioctl_req *ioctl_req = NULL;
    mlan_ds_pm_cfg *pm_cfg = NULL;
    mlan_ds_ps_mgmt ps_mgmt;
    int ret = 0;

    ENTER();

    memset(&ps_mgmt, 0, sizeof(mlan_ds_ps_mgmt));

    /* Sanity check */
    if (req->ifr_data == NULL) {
        PRINTM(MERROR, "uap_power_mode_ioctl() corrupt data\n");
        ret = -EFAULT;
        goto done;
    }
    if (copy_from_user(&ps_mgmt, req->ifr_data, sizeof(mlan_ds_ps_mgmt))) {
        PRINTM(MERROR, "Copy from user failed\n");
        ret = -EFAULT;
        goto done;
    }

    PRINTM(MIOCTL,
           "ioctl power: flag=0x%x ps_mode=%d ctrl_bitmap=%d min_sleep=%d max_sleep=%d "
           "inact_to=%d min_awake=%d max_awake=%d\n", ps_mgmt.flags,
           (int) ps_mgmt.ps_mode, (int) ps_mgmt.sleep_param.ctrl_bitmap,
           (int) ps_mgmt.sleep_param.min_sleep,
           (int) ps_mgmt.sleep_param.max_sleep,
           (int) ps_mgmt.inact_param.inactivity_to,
           (int) ps_mgmt.inact_param.min_awake,
           (int) ps_mgmt.inact_param.max_awake);

    if (ps_mgmt.
        flags & ~(PS_FLAG_PS_MODE | PS_FLAG_SLEEP_PARAM |
                  PS_FLAG_INACT_SLEEP_PARAM)) {
        PRINTM(MERROR, "Invalid parameter: flags = 0x%x\n", ps_mgmt.flags);
        ret = -EINVAL;
        goto done;
    }

    if (ps_mgmt.ps_mode > PS_MODE_INACTIVITY) {
        PRINTM(MERROR, "Invalid parameter: ps_mode = %d\n",
               (int) ps_mgmt.flags);
        ret = -EINVAL;
        goto done;
    }

    ioctl_req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_pm_cfg));
    if (ioctl_req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    pm_cfg = (mlan_ds_pm_cfg *) ioctl_req->pbuf;
    pm_cfg->sub_command = MLAN_OID_PM_CFG_PS_MODE;
    ioctl_req->req_id = MLAN_IOCTL_PM_CFG;
    if (ps_mgmt.flags) {
        ioctl_req->action = MLAN_ACT_SET;
        memcpy(&pm_cfg->param.ps_mgmt, &ps_mgmt, sizeof(mlan_ds_ps_mgmt));
    } else {
        ioctl_req->action = MLAN_ACT_GET;
    }

    if (MLAN_STATUS_SUCCESS !=
        woal_request_ioctl(priv, ioctl_req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        if (copy_to_user(req->ifr_data, &ioctl_req->status_code, sizeof(t_u32)))
            PRINTM(MERROR, "Copy to user failed!\n");
        goto done;
    }
    if (!ps_mgmt.flags) {
        /* Copy to user */
        if (copy_to_user
            (req->ifr_data, &pm_cfg->param.ps_mgmt, sizeof(mlan_ds_ps_mgmt))) {
            PRINTM(MERROR, "Copy to user failed!\n");
            ret = -EFAULT;
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
 *  @brief uap BSS ioctl handler
 *   
 *  @param dev      A pointer to net_device structure
 *  @param req      A pointer to ifreq structure
 *  @return         0 --success, otherwise fail
 */
static int
woal_uap_bss_ioctl(struct net_device *dev, struct ifreq *req)
{
    moal_private *priv = (moal_private *) netdev_priv(dev);
    int ret = 0;
    mlan_ds_bss *bss = NULL;
    mlan_ioctl_req *ioctl_req = NULL;
    int offset = 0;
    t_u32 action = 0;

    ENTER();

    /* Sanity check */
    if (req->ifr_data == NULL) {
        PRINTM(MERROR, "uap_bss_ioctl() corrupt data\n");
        ret = -EFAULT;
        goto done;
    }

    /* Get action */
    if (copy_from_user(&action, req->ifr_data + offset, sizeof(action))) {
        PRINTM(MERROR, "Copy from user failed\n");
        ret = -EFAULT;
        goto done;
    }
    offset += sizeof(action);

    /* Allocate an IOCTL request buffer */
    ioctl_req =
        (mlan_ioctl_req *) woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_bss));
    if (ioctl_req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    bss = (mlan_ds_bss *) ioctl_req->pbuf;
    bss->sub_command = MLAN_OID_UAP_BSS_CONFIG;
    ioctl_req->req_id = MLAN_IOCTL_BSS;
    if (action == 1) {
        ioctl_req->action = MLAN_ACT_SET;
    } else {
        ioctl_req->action = MLAN_ACT_GET;
    }

    if (ioctl_req->action == MLAN_ACT_SET) {
        /* Get the BSS config from user */
        if (copy_from_user
            (&bss->param.bss_config, req->ifr_data + offset,
             sizeof(mlan_uap_bss_param))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
    }

    if (MLAN_STATUS_SUCCESS !=
        woal_request_ioctl(priv, ioctl_req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    if (ioctl_req->action == MLAN_ACT_GET) {
        offset = sizeof(action);

        /* Copy to user : BSS config */
        if (copy_to_user
            (req->ifr_data + offset, &bss->param.bss_config,
             sizeof(mlan_uap_bss_param))) {
            PRINTM(MERROR, "Copy to user failed!\n");
            ret = -EFAULT;
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
 *  @brief uap get station list handler
 *   
 *  @param dev      A pointer to net_device structure
 *  @param req      A pointer to ifreq structure
 *  @return         0 --success, otherwise fail
 */
static int
woal_uap_get_sta_list_ioctl(struct net_device *dev, struct ifreq *req)
{
    moal_private *priv = (moal_private *) netdev_priv(dev);
    int ret = 0;
    mlan_ds_get_info *info = NULL;
    mlan_ioctl_req *ioctl_req = NULL;

    ENTER();

    /* Sanity check */
    if (req->ifr_data == NULL) {
        PRINTM(MERROR, "uap_get_sta_list_ioctl() corrupt data\n");
        ret = -EFAULT;
        goto done;
    }

    /* Allocate an IOCTL request buffer */
    ioctl_req =
        (mlan_ioctl_req *) woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_get_info));
    if (ioctl_req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    info = (mlan_ds_get_info *) ioctl_req->pbuf;
    info->sub_command = MLAN_OID_UAP_STA_LIST;
    ioctl_req->req_id = MLAN_IOCTL_GET_INFO;
    ioctl_req->action = MLAN_ACT_GET;

    if (MLAN_STATUS_SUCCESS !=
        woal_request_ioctl(priv, ioctl_req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    if (ioctl_req->action == MLAN_ACT_GET) {
        /* Copy to user : sta_list */
        if (copy_to_user
            (req->ifr_data, &info->param.sta_list, sizeof(mlan_ds_sta_list))) {
            PRINTM(MERROR, "Copy to user failed!\n");
            ret = -EFAULT;
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
 *  @brief uap hs_cfg ioctl handler
 *   
 *  @param dev      A pointer to net_device structure
 *  @param req      A pointer to ifreq structure
 *  @return         0 --success, otherwise fail
 */
static int
woal_uap_hs_cfg(struct net_device *dev, struct ifreq *req)
{
    moal_private *priv = (moal_private *) netdev_priv(dev);
    mlan_ioctl_req *ioctl_req = NULL;
    mlan_ds_pm_cfg *pmcfg = NULL;
    mlan_ds_hs_cfg hscfg;
    ds_hs_cfg hs_cfg;
    int ret = 0;

    ENTER();

    memset(&hscfg, 0, sizeof(mlan_ds_hs_cfg));
    memset(&hs_cfg, 0, sizeof(ds_hs_cfg));

    /* Sanity check */
    if (req->ifr_data == NULL) {
        PRINTM(MERROR, "uap_power_mode_ioctl() corrupt data\n");
        ret = -EFAULT;
        goto done;
    }
    if (copy_from_user(&hs_cfg, req->ifr_data, sizeof(ds_hs_cfg))) {
        PRINTM(MERROR, "Copy from user failed\n");
        ret = -EFAULT;
        goto done;
    }

    PRINTM(MIOCTL,
           "ioctl hscfg: flags=0x%lx condition=0x%lx gpio=%d gap=0x%lx\n",
           hs_cfg.flags, hs_cfg.conditions, (int) hs_cfg.gpio, hs_cfg.gap);

    ioctl_req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_pm_cfg));
    if (ioctl_req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    pmcfg = (mlan_ds_pm_cfg *) ioctl_req->pbuf;
    pmcfg->sub_command = MLAN_OID_PM_CFG_HS_CFG;
    ioctl_req->req_id = MLAN_IOCTL_PM_CFG;
    if (hs_cfg.flags & HS_CFG_FLAG_SET) {
        ioctl_req->action = MLAN_ACT_SET;
        if (hs_cfg.flags != HS_CFG_FLAG_ALL) {
            woal_get_hs_params(priv, MOAL_IOCTL_WAIT, &hscfg);
            memcpy(&pmcfg->param.hs_cfg, &hscfg, sizeof(mlan_ds_hs_cfg));
        }
        if (hs_cfg.flags & HS_CFG_FLAG_CONDITION)
            pmcfg->param.hs_cfg.conditions = hs_cfg.conditions;
        if (hs_cfg.flags & HS_CFG_FLAG_GPIO)
            pmcfg->param.hs_cfg.gpio = hs_cfg.gpio;
        if (hs_cfg.flags & HS_CFG_FLAG_GAP)
            pmcfg->param.hs_cfg.gap = hs_cfg.gap;
    } else {
        ioctl_req->action = MLAN_ACT_GET;
    }
    pmcfg->param.hs_cfg.is_invoke_hostcmd = MTRUE;
    if (MLAN_STATUS_SUCCESS !=
        woal_request_ioctl(priv, ioctl_req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    if (!(hs_cfg.flags & HS_CFG_FLAG_SET)) {
        hs_cfg.flags =
            HS_CFG_FLAG_CONDITION | HS_CFG_FLAG_GPIO | HS_CFG_FLAG_GAP;
        hs_cfg.conditions = pmcfg->param.hs_cfg.conditions;
        hs_cfg.gpio = pmcfg->param.hs_cfg.gpio;
        hs_cfg.gap = pmcfg->param.hs_cfg.gap;
        /* Copy to user */
        if (copy_to_user(req->ifr_data, &hs_cfg, sizeof(ds_hs_cfg))) {
            PRINTM(MERROR, "Copy to user failed!\n");
            ret = -EFAULT;
            goto done;
        }
    }
  done:
    if (ioctl_req)
        kfree(ioctl_req);
    LEAVE();
    return ret;
}

/********************************************************
		Global Functions
********************************************************/
/** 
 *  @brief This function sets multicast addresses to firmware
 *  
 *  @param dev     A pointer to net_device structure
 *  @return        n/a
 */
void
woal_uap_set_multicast_list(struct net_device *dev)
{
    ENTER();

    LEAVE();
}

/**
 *  @brief ioctl function - entry point
 *
 *  @param dev		A pointer to net_device structure
 *  @param req	   	A pointer to ifreq structure
 *  @param cmd 		Command
 *
 *  @return          0 --success, otherwise fail
 */
int
woal_uap_do_ioctl(struct net_device *dev, struct ifreq *req, int cmd)
{
    int ret = 0;
    ENTER();
    PRINTM(MIOCTL, "uap_do_ioctl: ioctl cmd = 0x%x\n", cmd);
    switch (cmd) {
    case UAP_HOSTCMD:
        ret = woal_hostcmd_ioctl(dev, req);
        break;
    case UAP_IOCTL_CMD:
        ret = woal_uap_ioctl(dev, req);
        break;
    case UAP_POWER_MODE:
        ret = woal_uap_power_mode_ioctl(dev, req);
        break;
    case UAP_BSS_CTRL:
        ret = woal_uap_bss_ctrl(dev, req);
        break;
    case UAP_BSS_CONFIG:
        ret = woal_uap_bss_ioctl(dev, req);
        break;
    case UAP_STA_DEAUTH:
        ret = woal_uap_sta_deauth_ioctl(dev, req);
        break;
    case UAP_GET_STA_LIST:
        ret = woal_uap_get_sta_list_ioctl(dev, req);
        break;
    case UAP_CUSTOM_IE:
        ret = woal_custom_ie_ioctl(dev, req);
        break;
    case UAP_GET_BSS_TYPE:
        ret = woal_get_bss_type(dev, req);
        break;
    case UAP_HS_CFG:
        ret = woal_uap_hs_cfg(dev, req);
        break;
    default:
        ret = -EINVAL;
        break;
    }
    LEAVE();
    return ret;
}

/** 
 *  @brief Get version 
 *   
 *  @param priv 		A pointer to moal_private structure
 *  @param version		A pointer to version buffer
 *  @param max_len		max length of version buffer
 *
 *  @return 	   		N/A
 */
void
woal_uap_get_version(moal_private * priv, char *version, int max_len)
{
    mlan_ds_get_info *info = NULL;
    mlan_ioctl_req *req = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_get_info));
    if (req == NULL) {
        LEAVE();
        return;
    }

    info = (mlan_ds_get_info *) req->pbuf;
    info->sub_command = MLAN_OID_GET_VER_EXT;
    req->req_id = MLAN_IOCTL_GET_INFO;
    req->action = MLAN_ACT_GET;

    status = woal_request_ioctl(priv, req, MOAL_PROC_WAIT);
    if (status == MLAN_STATUS_SUCCESS) {
        PRINTM(MINFO, "MOAL UAP VERSION: %s\n",
               info->param.ver_ext.version_str);
        snprintf(version, max_len, driver_version,
                 info->param.ver_ext.version_str);
    }

    if (req && (status != MLAN_STATUS_PENDING))
        kfree(req);

    LEAVE();
    return;
}

/** 
 *  @brief Get uap statistics 
 *   
 *  @param priv 		        A pointer to moal_private structure
 *  @param wait_option          Wait option
 *  @param ustats               A pointer to mlan_ds_uap_stats structure
 *
 *  @return                     MLAN_STATUS_SUCCESS -- success, otherwise fail 
 */
mlan_status
woal_uap_get_stats(moal_private * priv, t_u8 wait_option,
                   mlan_ds_uap_stats * ustats)
{
    mlan_ds_get_info *info = NULL;
    mlan_ioctl_req *req = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_get_info));
    if (req == NULL) {
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }

    info = (mlan_ds_get_info *) req->pbuf;
    info->sub_command = MLAN_OID_GET_STATS;
    req->req_id = MLAN_IOCTL_GET_INFO;
    req->action = MLAN_ACT_GET;

    status = woal_request_ioctl(priv, req, wait_option);
    if (status == MLAN_STATUS_SUCCESS) {
        if (ustats)
            memcpy(ustats, &info->param.ustats, sizeof(mlan_ds_uap_stats));
    }

    if (req && (status != MLAN_STATUS_PENDING))
        kfree(req);

    LEAVE();
    return status;
}
