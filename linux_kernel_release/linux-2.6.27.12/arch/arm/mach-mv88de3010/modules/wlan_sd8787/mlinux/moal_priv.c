/** @file  moal_priv.c
  *
  * @brief This file contains standard ioctl functions
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

/************************************************************************
Change log:
    10/30/2008: initial version
************************************************************************/

#include	"moal_main.h"
#include	"moal_sdio.h"

/********************************************************
                Local Variables
********************************************************/
/** Bands supported in Infra mode */
static t_u8 SupportedInfraBand[] = {
    BAND_B, BAND_B | BAND_G, BAND_G,
    BAND_GN, BAND_B | BAND_G | BAND_GN, BAND_G | BAND_GN,
    BAND_A, BAND_B | BAND_A, BAND_B | BAND_G | BAND_A, BAND_G | BAND_A,
    BAND_A | BAND_B | BAND_G | BAND_AN | BAND_GN,
        BAND_A | BAND_G | BAND_AN | BAND_GN, BAND_A | BAND_AN,
};

/** Bands supported in Ad-Hoc mode */
static t_u8 SupportedAdhocBand[] = {
    BAND_B, BAND_B | BAND_G, BAND_G,
    BAND_GN, BAND_B | BAND_G | BAND_GN, BAND_G | BAND_GN,
    BAND_A,
    BAND_AN, BAND_A | BAND_AN,
};

/********************************************************
		Global Variables
********************************************************/

/** Macro for maximum size of scan response buffer */
#define MAX_SCAN_RSP_BUF (16 * 1024)

/********************************************************
		Local Functions
********************************************************/
/** 
 *  @brief Handle get info resp 
 *   
 *  @param priv 	Pointer to moal_private structure
 *  @param info 	Pointer to mlan_ds_get_info structure
 *
 *  @return    		N/A
 */
void
woal_ioctl_get_info_resp(moal_private * priv, mlan_ds_get_info * info)
{
    ENTER();
    switch (info->sub_command) {
    case MLAN_OID_GET_STATS:
        priv->w_stats.discard.fragment = info->param.stats.fcs_error;
        priv->w_stats.discard.retries = info->param.stats.retry;
        priv->w_stats.discard.misc = info->param.stats.ack_failure;
        break;
    case MLAN_OID_GET_SIGNAL:
        if (info->param.signal.selector & BCN_RSSI_AVG_MASK)
            priv->w_stats.qual.level = info->param.signal.bcn_rssi_avg;
        if (info->param.signal.selector & BCN_NF_AVG_MASK)
            priv->w_stats.qual.noise = info->param.signal.bcn_nf_avg;
        break;
    default:
        break;
    }
    LEAVE();
}

/** 
 *  @brief Handle get BSS resp 
 *   
 *  @param priv 	Pointer to moal_private structure
 *  @param bss 		Pointer to mlan_ds_bss structure
 *
 *  @return    		N/A
 */
void
woal_ioctl_get_bss_resp(moal_private * priv, mlan_ds_bss * bss)
{
    t_u32 mode = 0;

    ENTER();

    switch (bss->sub_command) {
    case MLAN_OID_BSS_MODE:
        if (bss->param.bss_mode == MLAN_BSS_MODE_INFRA)
            mode = IW_MODE_INFRA;
        else if (bss->param.bss_mode == MLAN_BSS_MODE_IBSS)
            mode = IW_MODE_ADHOC;
        else
            mode = IW_MODE_AUTO;
        priv->w_stats.status = mode;
        break;
    default:
        break;
    }

    LEAVE();
    return;
}

/** 
 *  @brief Copy Rates
 *   
 *  @param dest    A pointer to destination buffer
 *  @param pos     The position for copy
 *  @param src     A pointer to source buffer
 *  @param len     Length of the source buffer
 *
 *  @return        Number of rates copied 
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
 *  @brief Get Driver Version
 *
 *  @param priv         A pointer to moal_private structure
 *  @param req          A pointer to ifreq structure
 *
 *  @return             0 --success, otherwise fail
 */
static int
woal_get_driver_version(moal_private * priv, struct ifreq *req)
{
    struct iwreq *wrq = (struct iwreq *) req;
    int len;
    char buf[MLAN_MAX_VER_STR_LEN];
    ENTER();

    woal_get_version(priv->phandle, buf, sizeof(buf) - 1);

    len = strlen(buf);
    if (wrq->u.data.pointer) {
        if (copy_to_user(wrq->u.data.pointer, buf, len)) {
            PRINTM(MERROR, "Copy to user failed\n");
            LEAVE();
            return -EFAULT;
        }
        wrq->u.data.length = len;
    }
    PRINTM(MINFO, "MOAL VERSION: %s\n", buf);
    LEAVE();
    return 0;
}

/**
 *  @brief Get extended driver version
 *
 *  @param priv         A pointer to moal_private structure
 *  @param ireq         A pointer to ifreq structure
 *
 *  @return             0 --success, otherwise fail  
 */
static int
woal_get_driver_verext(moal_private * priv, struct ifreq *ireq)
{
    struct iwreq *wrq = (struct iwreq *) ireq;
    mlan_ds_get_info *info = NULL;
    mlan_ioctl_req *req = NULL;
    int ret = 0;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_get_info));
    if (req == NULL) {
        LEAVE();
        return -ENOMEM;
    }

    info = (mlan_ds_get_info *) req->pbuf;
    info->sub_command = MLAN_OID_GET_VER_EXT;
    req->req_id = MLAN_IOCTL_GET_INFO;
    req->action = MLAN_ACT_GET;

    if (!wrq->u.data.flags) {
        info->param.ver_ext.version_str_sel =
            *((int *) (wrq->u.name + SUBCMD_OFFSET));
    } else {
        if (copy_from_user
            (&info->param.ver_ext.version_str_sel, wrq->u.data.pointer,
             sizeof(info->param.ver_ext.version_str_sel))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        } else {
            if (info->param.ver_ext.version_str_sel < 0) {
                PRINTM(MERROR, "Invalid arguments!\n");
                ret = -EINVAL;
                goto done;
            }
        }
    }

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    if (wrq->u.data.pointer) {
        if (copy_to_user(wrq->u.data.pointer, info->param.ver_ext.version_str,
                         strlen(info->param.ver_ext.version_str))) {
            PRINTM(MERROR, "Copy to user failed\n");
            ret = -EFAULT;
            goto done;
        }
        wrq->u.data.length = strlen(info->param.ver_ext.version_str);
    }

    PRINTM(MINFO, "MOAL EXTENDED VERSION: %s\n",
           info->param.ver_ext.version_str);
  done:
    if (req)
        kfree(req);

    LEAVE();
    return ret;
}

/**
 *  @brief Set Deep Sleep
 *
 *  @param priv         Pointer to the moal_private driver data struct
 *  @param wait_option  wait option
 *  @param bdeep_sleep  TRUE--enalbe deepsleep, FALSE--disable deepsleep
 *  @param idletime     Idle time for optimized PS API
 *
 *  @return             0 --success, otherwise fail
 */
int
woal_set_deep_sleep(moal_private * priv, t_u8 wait_option, BOOLEAN bdeep_sleep,
                    t_u16 idletime)
{
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_pm_cfg *pm = NULL;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_pm_cfg));
    if (req == NULL) {
        LEAVE();
        return -ENOMEM;
    }
    pm = (mlan_ds_pm_cfg *) req->pbuf;
    pm->sub_command = MLAN_OID_PM_CFG_DEEP_SLEEP;
    req->req_id = MLAN_IOCTL_PM_CFG;

    req->action = MLAN_ACT_SET;
    if (bdeep_sleep == MTRUE) {
        PRINTM(MIOCTL, "Deep Sleep: sleep\n");
        pm->param.auto_deep_sleep.auto_ds = DEEP_SLEEP_ON;
        if (idletime) {
            pm->param.auto_deep_sleep.idletime = idletime;
        }
        if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, wait_option)) {
            ret = -EFAULT;
            goto done;
        }
    } else {
        PRINTM(MIOCTL, "%lu : Deep Sleep: wakeup\n", jiffies);
        pm->param.auto_deep_sleep.auto_ds = DEEP_SLEEP_OFF;

        if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, wait_option)) {
            ret = -EFAULT;
            goto done;
        }
    }

  done:
    if (req)
        kfree(req);

    LEAVE();
    return ret;
}

/**
 *  @brief Performs warm reset
 *
 *  @param priv         A pointer to moal_private structure
 *
 *  @return             0 --success, otherwise fail  
 */
static int
woal_warm_reset(moal_private * priv)
{
    int ret = 0;
    int intf_num;
    moal_handle *handle = priv->phandle;
    mlan_ioctl_req *req = NULL;
    mlan_ds_misc_cfg *misc = NULL;
    mlan_bss_info bss_info;

    ENTER();

    /* Disable interfaces */
    for (intf_num = 0; intf_num < handle->priv_num; intf_num++) {
        netif_stop_queue(handle->priv[intf_num]->netdev);
        netif_device_detach(handle->priv[intf_num]->netdev);
    }

    /* Exit deep sleep */
    woal_set_deep_sleep(priv, MOAL_IOCTL_WAIT, MFALSE, 0);

    memset(&bss_info, 0, sizeof(bss_info));
    woal_get_bss_info(priv, MOAL_IOCTL_WAIT, &bss_info);
    if (bss_info.is_hs_configured) {
        if (MLAN_STATUS_SUCCESS != woal_hs_cfg_cancel(priv, MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            goto done;
        }
    }

    /* Disconnect from network */
    for (intf_num = 0; intf_num < handle->priv_num; intf_num++) {
        if (handle->priv[intf_num]->media_connected == MTRUE) {
            woal_disconnect(handle->priv[intf_num], MOAL_IOCTL_WAIT, NULL);
        }
    }

    /* Initialize private structures */
    for (intf_num = 0; intf_num < handle->priv_num; intf_num++)
        woal_init_priv(handle->priv[intf_num], MOAL_IOCTL_WAIT);

#ifdef REASSOCIATION
    /* Reset the reassoc timer and status */
    handle->reassoc_on = MFALSE;
    if (handle->is_reassoc_timer_set == MTRUE) {
        woal_cancel_timer(&handle->reassoc_timer);
        handle->is_reassoc_timer_set = MFALSE;
    }
#endif

    /* Restart the firmware */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_misc_cfg));
    if (req) {
        misc = (mlan_ds_misc_cfg *) req->pbuf;
        misc->sub_command = MLAN_OID_MISC_WARM_RESET;
        req->req_id = MLAN_IOCTL_MISC_CFG;
        req->action = MLAN_ACT_SET;
        if (MLAN_STATUS_SUCCESS !=
            woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            kfree(req);
            goto done;
        }
        kfree(req);
    }

    /* Enable interfaces */
    for (intf_num = 0; intf_num < handle->priv_num; intf_num++) {
        netif_device_attach(handle->priv[intf_num]->netdev);
        netif_start_queue(handle->priv[intf_num]->netdev);
    }

  done:
    LEAVE();
    return ret;
}

/**
 *  @brief Get signal
 *
 *  @param priv         A pointer to moal_private structure
 *  @param wrq	        A pointer to iwreq structure
 *
 *  @return 	  	0 --success, otherwise fail
 */
static int
woal_get_signal(moal_private * priv, struct iwreq *wrq)
{
/** Input data size */
#define IN_DATA_SIZE	2
/** Output data size */
#define OUT_DATA_SIZE	12
    int ret = 0;
    int in_data[IN_DATA_SIZE];
    int out_data[OUT_DATA_SIZE];
    mlan_ds_get_signal signal;
    int data_length = 0;

    ENTER();

    memset(in_data, 0, sizeof(in_data));
    memset(out_data, 0, sizeof(out_data));

    if (priv->media_connected == MFALSE) {
        PRINTM(MERROR, "Can not get RSSI in disconnected state\n");
        ret = -ENOTSUPP;
        goto done;
    }

    if (wrq->u.data.length) {
        if (copy_from_user
            (in_data, wrq->u.data.pointer,
             sizeof(int) * MIN(wrq->u.data.length, sizeof(in_data)))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
    }

    switch (wrq->u.data.length) {
    case 0:                    /* No checking, get everything */
        break;
    case 2:                    /* Check subtype range */
        if (in_data[1] < 1 || in_data[1] > 4) {
            ret = -EINVAL;
            goto done;
        }
        /* Fall through */
    case 1:                    /* Check type range */
        if (in_data[0] < 1 || in_data[0] > 3) {
            ret = -EINVAL;
            goto done;
        }
        break;
    default:
        ret = -EINVAL;
        goto done;
    }

    memset(&signal, 0, sizeof(mlan_ds_get_signal));
    if (MLAN_STATUS_SUCCESS !=
        woal_get_signal_info(priv, MOAL_IOCTL_WAIT, &signal)) {
        ret = -EFAULT;
        goto done;
    }
    PRINTM(MINFO, "RSSI Beacon Last   : %d\n", (int) signal.bcn_rssi_last);
    PRINTM(MINFO, "RSSI Beacon Average: %d\n", (int) signal.bcn_rssi_avg);
    PRINTM(MINFO, "RSSI Data Last     : %d\n", (int) signal.data_rssi_last);
    PRINTM(MINFO, "RSSI Data Average  : %d\n", (int) signal.data_rssi_avg);
    PRINTM(MINFO, "SNR Beacon Last    : %d\n", (int) signal.bcn_snr_last);
    PRINTM(MINFO, "SNR Beacon Average : %d\n", (int) signal.bcn_snr_avg);
    PRINTM(MINFO, "SNR Data Last      : %d\n", (int) signal.data_snr_last);
    PRINTM(MINFO, "SNR Data Average   : %d\n", (int) signal.data_snr_avg);
    PRINTM(MINFO, "NF Beacon Last     : %d\n", (int) signal.bcn_nf_last);
    PRINTM(MINFO, "NF Beacon Average  : %d\n", (int) signal.bcn_nf_avg);
    PRINTM(MINFO, "NF Data Last       : %d\n", (int) signal.data_nf_last);
    PRINTM(MINFO, "NF Data Average    : %d\n", (int) signal.data_nf_avg);

    /* Check type */
    switch (in_data[0]) {
    case 0:                    /* Send everything */
        out_data[data_length++] = signal.bcn_rssi_last;
        out_data[data_length++] = signal.bcn_rssi_avg;
        out_data[data_length++] = signal.data_rssi_last;
        out_data[data_length++] = signal.data_rssi_avg;
        out_data[data_length++] = signal.bcn_snr_last;
        out_data[data_length++] = signal.bcn_snr_avg;
        out_data[data_length++] = signal.data_snr_last;
        out_data[data_length++] = signal.data_snr_avg;
        out_data[data_length++] = signal.bcn_nf_last;
        out_data[data_length++] = signal.bcn_nf_avg;
        out_data[data_length++] = signal.data_nf_last;
        out_data[data_length++] = signal.data_nf_avg;
        break;
    case 1:                    /* RSSI */
        /* Check subtype */
        switch (in_data[1]) {
        case 0:                /* Everything */
            out_data[data_length++] = signal.bcn_rssi_last;
            out_data[data_length++] = signal.bcn_rssi_avg;
            out_data[data_length++] = signal.data_rssi_last;
            out_data[data_length++] = signal.data_rssi_avg;
            break;
        case 1:                /* bcn last */
            out_data[data_length++] = signal.bcn_rssi_last;
            break;
        case 2:                /* bcn avg */
            out_data[data_length++] = signal.bcn_rssi_avg;
            break;
        case 3:                /* data last */
            out_data[data_length++] = signal.data_rssi_last;
            break;
        case 4:                /* data avg */
            out_data[data_length++] = signal.data_rssi_avg;
            break;
        default:
            break;
        }
        break;
    case 2:                    /* SNR */
        /* Check subtype */
        switch (in_data[1]) {
        case 0:                /* Everything */
            out_data[data_length++] = signal.bcn_snr_last;
            out_data[data_length++] = signal.bcn_snr_avg;
            out_data[data_length++] = signal.data_snr_last;
            out_data[data_length++] = signal.data_snr_avg;
            break;
        case 1:                /* bcn last */
            out_data[data_length++] = signal.bcn_snr_last;
            break;
        case 2:                /* bcn avg */
            out_data[data_length++] = signal.bcn_snr_avg;
            break;
        case 3:                /* data last */
            out_data[data_length++] = signal.data_snr_last;
            break;
        case 4:                /* data avg */
            out_data[data_length++] = signal.data_snr_avg;
            break;
        default:
            break;
        }
        break;
    case 3:                    /* NF */
        /* Check subtype */
        switch (in_data[1]) {
        case 0:                /* Everything */
            out_data[data_length++] = signal.bcn_nf_last;
            out_data[data_length++] = signal.bcn_nf_avg;
            out_data[data_length++] = signal.data_nf_last;
            out_data[data_length++] = signal.data_nf_avg;
            break;
        case 1:                /* bcn last */
            out_data[data_length++] = signal.bcn_nf_last;
            break;
        case 2:                /* bcn avg */
            out_data[data_length++] = signal.bcn_nf_avg;
            break;
        case 3:                /* data last */
            out_data[data_length++] = signal.data_nf_last;
            break;
        case 4:                /* data avg */
            out_data[data_length++] = signal.data_nf_avg;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    wrq->u.data.length = data_length;
    if (copy_to_user(wrq->u.data.pointer, out_data,
                     wrq->u.data.length * sizeof(out_data[0]))) {
        PRINTM(MERROR, "Copy to user failed\n");
        ret = -EFAULT;
        goto done;
    }
  done:
    LEAVE();
    return ret;
}

/**
 *  @brief Get Deep Sleep
 *
 *  @param priv         Pointer to the moal_private driver data struct
 *  @param deep_sleep   Pointer to return deep_sleep setting
 *
 *  @return          0 --success, otherwise fail
 */
static int
woal_get_deep_sleep(moal_private * priv, t_u32 * deep_sleep)
{
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_pm_cfg *pm = NULL;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_pm_cfg));
    if (req == NULL) {
        LEAVE();
        return -ENOMEM;
    }
    pm = (mlan_ds_pm_cfg *) req->pbuf;
    pm->sub_command = MLAN_OID_PM_CFG_DEEP_SLEEP;
    req->req_id = MLAN_IOCTL_PM_CFG;

    req->action = MLAN_ACT_GET;
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    *deep_sleep = pm->param.auto_deep_sleep.auto_ds;

  done:
    if (req)
        kfree(req);

    LEAVE();
    return ret;
}

/**
 *  @brief Get/Set DeepSleep mode
 *
 *  @param priv     Pointer to the moal_private driver data struct
 *  @param wreq	    A pointer to iwreq structure
 *
 *  @return          0 --success, otherwise fail
 */
static int
woal_deep_sleep_ioctl(moal_private * priv, struct iwreq *wrq)
{
    t_u32 deep_sleep = DEEP_SLEEP_OFF;
    static t_u16 idletime = DEEP_SLEEP_IDLE_TIME;
    t_u32 data[2];

    ENTER();

    if (wrq->u.data.length == 1 || wrq->u.data.length == 2) {
        if (copy_from_user
            (&data, wrq->u.data.pointer, wrq->u.data.length * sizeof(int))) {
            PRINTM(MERROR, "Copy from user failed\n");
            LEAVE();
            return -EFAULT;
        }
        deep_sleep = data[0];
        if (deep_sleep == DEEP_SLEEP_OFF) {
            PRINTM(MINFO, "Exit Deep Sleep Mode\n");
            woal_set_deep_sleep(priv, MOAL_IOCTL_WAIT, MFALSE, 0);
        } else if (deep_sleep == DEEP_SLEEP_ON) {
            PRINTM(MINFO, "Enter Deep Sleep Mode\n");
            if (wrq->u.data.length == 2)
                idletime = data[1];
            woal_set_deep_sleep(priv, MOAL_IOCTL_WAIT, MTRUE, idletime);
        } else {
            PRINTM(MERROR, "Unknown option = %lu\n", deep_sleep);
            LEAVE();
            return -EINVAL;
        }
    } else if (wrq->u.data.length > 2) {
        PRINTM(MERROR, "Invalid number of arguments %d\n", wrq->u.data.length);
        LEAVE();
        return -EINVAL;
    } else {                    /* Display Deep Sleep settings */
        PRINTM(MINFO, "Get Deep Sleep Mode\n");
        if (MLAN_STATUS_SUCCESS != woal_get_deep_sleep(priv, &deep_sleep)) {
            LEAVE();
            return -EFAULT;
        }
    }

    /* Copy the Deep Sleep setting to user */
    if (copy_to_user(wrq->u.data.pointer, &deep_sleep, sizeof(int))) {
        PRINTM(MERROR, "Copy to user failed\n");
        LEAVE();
        return -EINVAL;
    }
    wrq->u.data.length = 1;

    LEAVE();
    return 0;
}

/**
 *  @brief Set/Get Usr 11n configuration request
 *
 *  @param priv     Pointer to the moal_private driver data struct
 *  @param wrq	    A pointer to iwreq structure
 *
 *  @return          0 --success, otherwise fail
 */
static int
woal_11n_htcap_cfg(moal_private * priv, struct iwreq *wrq)
{
    int data;
    mlan_ioctl_req *req = NULL;
    mlan_ds_11n_cfg *cfg_11n = NULL;
    int ret = 0;

    ENTER();

    if (wrq->u.data.length > 1) {
        PRINTM(MERROR, "Invalid number of arguments\n");
        ret = -EINVAL;
        goto done;
    }

    if (((req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_11n_cfg))) == NULL)) {
        ret = -ENOMEM;
        goto done;
    }

    cfg_11n = (mlan_ds_11n_cfg *) req->pbuf;
    cfg_11n->sub_command = MLAN_OID_11N_HTCAP_CFG;
    req->req_id = MLAN_IOCTL_11N_CFG;

    if (wrq->u.data.length == 0) {
        /* Get 11n tx parameters from MLAN */
        req->action = MLAN_ACT_GET;
    } else if (wrq->u.data.length == 1) {
        if (copy_from_user(&data, wrq->u.data.pointer, sizeof(int))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }

        cfg_11n->param.htcap_cfg = data;
        PRINTM(MINFO, "SET: htusrcap:0x%x\n", data);
        /* Update 11n tx parameters in MLAN */
        req->action = MLAN_ACT_SET;
    }

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    data = cfg_11n->param.htcap_cfg;
    PRINTM(MINFO, "GET: httxcap:0x%x\n", data);

    if (copy_to_user(wrq->u.data.pointer, &data, sizeof(data))) {
        PRINTM(MERROR, "Copy to user failed\n");
        ret = -EFAULT;
        goto done;
    }

    wrq->u.data.length = 1;

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Enable/Disable amsdu_aggr_ctrl
 *
 *  @param priv     Pointer to the moal_private driver data struct
 *  @param wrq	    A pointer to iwreq structure
 *
 *  @return          0 --success, otherwise fail
 */
static int
woal_11n_amsdu_aggr_ctrl(moal_private * priv, struct iwreq *wrq)
{
    int data[2];
    mlan_ioctl_req *req = NULL;
    mlan_ds_11n_cfg *cfg_11n = NULL;
    int ret = 0;

    ENTER();

    if ((wrq->u.data.length != 0) && (wrq->u.data.length != 1)) {
        PRINTM(MERROR, "Invalid number of arguments\n");
        ret = -EINVAL;
        goto done;
    }
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_11n_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    cfg_11n = (mlan_ds_11n_cfg *) req->pbuf;
    cfg_11n->sub_command = MLAN_OID_11N_CFG_AMSDU_AGGR_CTRL;
    req->req_id = MLAN_IOCTL_11N_CFG;

    if (wrq->u.data.length == 0) {
        /* Get 11n tx parameters from MLAN */
        req->action = MLAN_ACT_GET;
    } else if (wrq->u.data.length == 1) {
        if (copy_from_user(data, wrq->u.data.pointer,
                           wrq->u.data.length * sizeof(int))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        cfg_11n->param.amsdu_aggr_ctrl.enable = data[0];
        /* Update 11n tx parameters in MLAN */
        req->action = MLAN_ACT_SET;
    }
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    data[0] = cfg_11n->param.amsdu_aggr_ctrl.enable;
    data[1] = cfg_11n->param.amsdu_aggr_ctrl.curr_buf_size;

    if (copy_to_user(wrq->u.data.pointer, data, sizeof(data))) {
        PRINTM(MERROR, "Copy to user failed\n");
        ret = -EFAULT;
        goto done;
    }
    wrq->u.data.length = 2;
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Set/Get 11n configuration request
 *
 *  @param priv     Pointer to the moal_private driver data struct
 *  @param wrq	    A pointer to iwreq structure
 *
 *  @return          0 --success, otherwise fail
 */
static int
woal_11n_tx_cfg(moal_private * priv, struct iwreq *wrq)
{
    int data;
    mlan_ioctl_req *req = NULL;
    mlan_ds_11n_cfg *cfg_11n = NULL;
    int ret = 0;

    ENTER();

    if ((wrq->u.data.length != 0) && (wrq->u.data.length != 1)) {
        PRINTM(MERROR, "Invalid number of arguments\n");
        ret = -EINVAL;
        goto done;
    }
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_11n_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    cfg_11n = (mlan_ds_11n_cfg *) req->pbuf;
    cfg_11n->sub_command = MLAN_OID_11N_CFG_TX;
    req->req_id = MLAN_IOCTL_11N_CFG;

    if (wrq->u.data.length == 0) {
        /* Get 11n tx parameters from MLAN */
        req->action = MLAN_ACT_GET;
    } else if (wrq->u.data.length == 1) {
        if (copy_from_user(&data, wrq->u.data.pointer, sizeof(data))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        cfg_11n->param.tx_cfg.httxcap = data;
        PRINTM(MINFO, "SET: httxcap:%d\n", data);
        /* Update 11n tx parameters in MLAN */
        req->action = MLAN_ACT_SET;
    }
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    data = cfg_11n->param.tx_cfg.httxcap;
    PRINTM(MINFO, "GET: httxcap:%d\n", data);
    if (copy_to_user(wrq->u.data.pointer, &data, sizeof(data))) {
        PRINTM(MERROR, "Copy to user failed\n");
        ret = -EFAULT;
        goto done;
    }
    wrq->u.data.length = 1;
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Enable/Disable TX Aggregation
 *
 *  @param priv     Pointer to the moal_private driver data struct
 *  @param wrq	    A pointer to iwreq structure
 *
 *  @return          0 --success, otherwise fail
 */
static int
woal_11n_prio_tbl(moal_private * priv, struct iwreq *wrq)
{
    int data[MAX_NUM_TID * 2], i, j;
    mlan_ioctl_req *req = NULL;
    mlan_ds_11n_cfg *cfg_11n = NULL;
    int ret = 0;

    ENTER();

    if ((wrq->u.data.pointer == NULL)) {
        LEAVE();
        return -EINVAL;
    }

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_11n_cfg));
    if (req == NULL) {
        LEAVE();
        return -ENOMEM;
    }
    cfg_11n = (mlan_ds_11n_cfg *) req->pbuf;
    cfg_11n->sub_command = MLAN_OID_11N_CFG_AGGR_PRIO_TBL;
    req->req_id = MLAN_IOCTL_11N_CFG;

    if (wrq->u.data.length == 0) {
        /* Get aggr priority table from MLAN */
        req->action = MLAN_ACT_GET;
        if (MLAN_STATUS_SUCCESS !=
            woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            goto error;
        }
        wrq->u.data.length = MAX_NUM_TID * 2;
        for (i = 0, j = 0; i < (wrq->u.data.length); i = i + 2, ++j) {
            data[i] = cfg_11n->param.aggr_prio_tbl.ampdu[j];
            data[i + 1] = cfg_11n->param.aggr_prio_tbl.amsdu[j];
        }

        if (copy_to_user(wrq->u.data.pointer, data,
                         sizeof(int) * wrq->u.data.length)) {
            PRINTM(MERROR, "Copy to user failed\n");
            ret = -EFAULT;
            goto error;
        }
    } else if (wrq->u.data.length == 16) {
        if (copy_from_user(data, wrq->u.data.pointer,
                           sizeof(int) * wrq->u.data.length)) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto error;
        }
        for (i = 0, j = 0; i < (wrq->u.data.length); i = i + 2, ++j) {
            if ((data[i] > 7 && data[i] != 0xff) ||
                (data[i + 1] > 7 && data[i + 1] != 0xff)) {
                PRINTM(MERROR, "Invalid priority, valid value 0-7 or 0xff.\n");
                ret = -EFAULT;
                goto error;
            }
            cfg_11n->param.aggr_prio_tbl.ampdu[j] = data[i];
            cfg_11n->param.aggr_prio_tbl.amsdu[j] = data[i + 1];
        }

        /* Update aggr priority table in MLAN */
        req->action = MLAN_ACT_SET;
        if (MLAN_STATUS_SUCCESS !=
            woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            goto error;
        }
    } else {
        PRINTM(MERROR, "Invalid number of arguments\n");
        ret = -EINVAL;
        goto error;
    }

  error:
    if (req)
        kfree(req);

    LEAVE();
    return ret;
}

/**
 *  @brief Set/Get add BA Reject parameters
 *
 *  @param priv     Pointer to the moal_private driver data struct
 *  @param wrq	    A pointer to iwreq structure
 *
 *  @return          0 --success, otherwise fail
 */
static int
woal_addba_reject(moal_private * priv, struct iwreq *wrq)
{
    int data[MAX_NUM_TID], ret = 0, i;
    mlan_ioctl_req *req = NULL;
    mlan_ds_11n_cfg *cfg_11n = NULL;

    ENTER();

    PRINTM(MERROR, "%s\n", __FUNCTION__);
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_11n_cfg));
    if (req == NULL) {
        LEAVE();
        return -ENOMEM;
    }
    cfg_11n = (mlan_ds_11n_cfg *) req->pbuf;
    cfg_11n->sub_command = MLAN_OID_11N_CFG_ADDBA_REJECT;
    req->req_id = MLAN_IOCTL_11N_CFG;

    if (wrq->u.data.length == 0) {
        PRINTM(MERROR, "Addba reject moal\n");
        /* Get aggr priority table from MLAN */
        req->action = MLAN_ACT_GET;
        if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req,
                                                      MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            goto error;
        }

        wrq->u.data.length = MAX_NUM_TID;
        for (i = 0; i < (wrq->u.data.length); ++i) {
            data[i] = cfg_11n->param.addba_reject[i];
        }

        if (copy_to_user(wrq->u.data.pointer, data,
                         sizeof(int) * wrq->u.data.length)) {
            PRINTM(MERROR, "Copy to user failed\n");
            ret = -EFAULT;
            goto error;
        }
    } else if (wrq->u.data.length == 8) {
        if (copy_from_user(data, wrq->u.data.pointer,
                           sizeof(int) * wrq->u.data.length)) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto error;
        }
        for (i = 0; i < (wrq->u.data.length); ++i) {
            if (data[i] != 0 && data[i] != 1) {
                PRINTM(MERROR, "addba reject only takes argument as 0 or 1\n");
                ret = -EFAULT;
                goto error;
            }
            cfg_11n->param.addba_reject[i] = data[i];
        }

        /* Update aggr priority table in MLAN */
        req->action = MLAN_ACT_SET;
        if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req,
                                                      MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            goto error;
        }
    } else {
        PRINTM(MERROR, "Invalid number of arguments\n");
        ret = -EINVAL;
        goto error;
    }
  error:
    if (req)
        kfree(req);

    LEAVE();
    return ret;
}

/**
 *  @brief Set/Get add BA parameters
 *
 *  @param priv     Pointer to the moal_private driver data struct
 *  @param wrq	    A pointer to iwreq structure
 *
 *  @return          0 --success, otherwise fail
 */
static int
woal_addba_para_updt(moal_private * priv, struct iwreq *wrq)
{
    int data[3], ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_11n_cfg *cfg_11n = NULL;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_11n_cfg));
    if (req == NULL) {
        LEAVE();
        return -ENOMEM;
    }
    cfg_11n = (mlan_ds_11n_cfg *) req->pbuf;
    cfg_11n->sub_command = MLAN_OID_11N_CFG_ADDBA_PARAM;
    req->req_id = MLAN_IOCTL_11N_CFG;

    if (wrq->u.data.length == 0) {
        /* Get Add BA parameters from MLAN */
        req->action = MLAN_ACT_GET;
        if (MLAN_STATUS_SUCCESS !=
            woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            goto error;
        }
        data[0] = cfg_11n->param.addba_param.timeout;
        data[1] = cfg_11n->param.addba_param.txwinsize;
        data[2] = cfg_11n->param.addba_param.rxwinsize;
        PRINTM(MINFO, "GET: timeout:%d txwinsize:%d rxwinsize:%d\n", data[0],
               data[1], data[2]);

        wrq->u.data.length = 3;
        if (copy_to_user(wrq->u.data.pointer, data,
                         wrq->u.data.length * sizeof(int))) {
            PRINTM(MERROR, "Copy to user failed\n");
            ret = -EFAULT;
            goto error;
        }
    } else if (wrq->u.data.length == 3) {
        if (copy_from_user
            (data, wrq->u.data.pointer, wrq->u.data.length * sizeof(int))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto error;
        }
        if (data[0] < 0 || data[0] > MLAN_DEFAULT_BLOCK_ACK_TIMEOUT) {
            PRINTM(MERROR, "Incorrect addba timeout value.\n");
            ret = -EFAULT;
            goto error;
        }
        if (data[1] > MLAN_AMPDU_MAX_TXWINSIZE ||
            data[2] > MLAN_AMPDU_MAX_RXWINSIZE) {
            PRINTM(MERROR, "Incorrect Tx/Rx window size.\n");
            ret = -EFAULT;
            goto error;
        }
        cfg_11n->param.addba_param.timeout = data[0];
        cfg_11n->param.addba_param.txwinsize = data[1];
        cfg_11n->param.addba_param.rxwinsize = data[2];
        PRINTM(MINFO, "SET: timeout:%d txwinsize:%d rxwinsize:%d\n", data[0],
               data[1], data[2]);

        /* Update Add BA parameters in MLAN */
        req->action = MLAN_ACT_SET;
        if (MLAN_STATUS_SUCCESS !=
            woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
            ret = MLAN_STATUS_FAILURE;
            goto error;
        }
    } else {
        PRINTM(MERROR, "Invalid number of arguments\n");
        ret = -EINVAL;
        goto error;
    }

  error:
    if (req)
        kfree(req);

    LEAVE();
    return ret;
}

/**
 *  @brief Set/Get Transmit buffer size
 *
 *  @param priv     Pointer to the moal_private driver data struct
 *  @param wrq	    A pointer to iwreq structure
 *
 *  @return          0 --success, otherwise fail
 */
static int
woal_txbuf_cfg(moal_private * priv, struct iwreq *wrq)
{
    int buf_size;
    mlan_ioctl_req *req = NULL;
    mlan_ds_11n_cfg *cfg_11n = NULL;
    int ret = 0;

    ENTER();
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_11n_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    cfg_11n = (mlan_ds_11n_cfg *) req->pbuf;
    cfg_11n->sub_command = MLAN_OID_11N_CFG_MAX_TX_BUF_SIZE;
    req->req_id = MLAN_IOCTL_11N_CFG;

    if (wrq->u.data.length == 0) {
        /* Get Tx buffer size from MLAN */
        req->action = MLAN_ACT_GET;
    } else {
        if (copy_from_user(&buf_size, wrq->u.data.pointer, sizeof(buf_size))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        PRINTM(MINFO, "SET: Tx buffer size %d\n", buf_size);
        /* Update Tx buffer size in MLAN */
        req->action = MLAN_ACT_SET;
        cfg_11n->param.tx_buf_size = buf_size;
    }
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    buf_size = cfg_11n->param.tx_buf_size;
    if (copy_to_user(wrq->u.data.pointer, &buf_size, sizeof(buf_size))) {
        PRINTM(MERROR, "Copy to user failed\n");
        ret = -EFAULT;
        goto done;
    }
    wrq->u.data.length = 1;
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Set/Get Host Sleep configuration
 *
 *  @param priv             A pointer to moal_private structure
 *  @param wrq	            A pointer to iwreq structure
 *  @param invoke_hostcmd	MTRUE --invoke HostCmd, otherwise MFALSE
 *
 *  @return             0 --success, otherwise fail
 */
static int
woal_hs_cfg(moal_private * priv, struct iwreq *wrq, BOOLEAN invoke_hostcmd)
{
    int data[3];
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_pm_cfg *pmcfg = NULL;
    mlan_ds_hs_cfg hscfg;
    mlan_bss_info bss_info;
    int data_length = wrq->u.data.length;

    ENTER();

    memset(data, 0, sizeof(data));
    memset(&hscfg, 0, sizeof(mlan_ds_hs_cfg));

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_pm_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    pmcfg = (mlan_ds_pm_cfg *) req->pbuf;
    pmcfg->sub_command = MLAN_OID_PM_CFG_HS_CFG;
    req->req_id = MLAN_IOCTL_PM_CFG;

    if (data_length == 0) {
        req->action = MLAN_ACT_GET;
    } else {
        req->action = MLAN_ACT_SET;
        if (data_length >= 1 && data_length <= 3) {
            if (copy_from_user
                (data, wrq->u.data.pointer, sizeof(int) * data_length)) {
                PRINTM(MERROR, "Copy from user failed\n");
                ret = -EFAULT;
                goto done;
            }
        } else {
            PRINTM(MERROR, "Invalid arguments\n");
            ret = -EINVAL;
            goto done;
        }
    }

    /* HS config is blocked if HS is already activated */
    if (data_length &&
        (data[0] != HOST_SLEEP_CFG_CANCEL || invoke_hostcmd == MFALSE)) {
        memset(&bss_info, 0, sizeof(bss_info));
        woal_get_bss_info(priv, MOAL_IOCTL_WAIT, &bss_info);
        if (bss_info.is_hs_configured) {
            PRINTM(MERROR, "HS already configured\n");
            ret = -EFAULT;
            goto done;
        }
    }

    /* Do a GET first if some arguments are not provided */
    if (data_length >= 1 && data_length < 3) {
        woal_get_hs_params(priv, MOAL_IOCTL_WAIT, &hscfg);
    }

    switch (data_length) {
    case 3:                    /* Conditions, GPIO, GAP provided */
        pmcfg->param.hs_cfg.conditions = data[0];
        pmcfg->param.hs_cfg.gpio = data[1];
        pmcfg->param.hs_cfg.gap = data[2];
        break;
    case 2:                    /* Conditions, GPIO provided */
        pmcfg->param.hs_cfg.conditions = data[0];
        pmcfg->param.hs_cfg.gpio = data[1];
        pmcfg->param.hs_cfg.gap = hscfg.gap;
        break;
    case 1:                    /* Conditions provided */
        pmcfg->param.hs_cfg.conditions = data[0];
        pmcfg->param.hs_cfg.gpio = hscfg.gpio;
        pmcfg->param.hs_cfg.gap = hscfg.gap;
        break;
    case 0:
        break;
    default:
        break;
    }

    pmcfg->param.hs_cfg.is_invoke_hostcmd = invoke_hostcmd;
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    if (req->action == MLAN_ACT_GET) {
        data[0] = pmcfg->param.hs_cfg.conditions;
        data[1] = pmcfg->param.hs_cfg.gpio;
        data[2] = pmcfg->param.hs_cfg.gap;
        wrq->u.data.length = 3;
        if (copy_to_user
            (wrq->u.data.pointer, data, sizeof(int) * wrq->u.data.length)) {
            PRINTM(MERROR, "Copy to user failed\n");
            ret = -EFAULT;
            goto done;
        }
    }
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Set Host Sleep parameters
 *
 *  @param priv         A pointer to moal_private structure
 *  @param wrq	        A pointer to iwreq structure
 *
 *  @return             0 --success, otherwise fail
 */
static int
woal_hs_setpara(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    int data_length = wrq->u.data.length;

    ENTER();

    if (data_length >= 1 && data_length <= 3) {
        ret = woal_hs_cfg(priv, wrq, MFALSE);
        goto done;
    } else {
        PRINTM(MERROR, "Invalid arguments\n");
        ret = -EINVAL;
        goto done;
    }
  done:
    LEAVE();
    return ret;
}

/**
 *  @brief Get/Set inactivity timeout extend
 *
 *  @param priv         A pointer to moal_private structure
 *  @param wrq	        A pointer to iwreq structure
 *
 *  @return             0 --success, otherwise fail
 */
static int
woal_inactivity_timeout_ext(moal_private * priv, struct iwreq *wrq)
{
    int data[4];
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_pm_cfg *pmcfg = NULL;
    pmlan_ds_inactivity_to inac_to = NULL;
    int data_length = wrq->u.data.length;

    ENTER();

    memset(data, 0, sizeof(data));
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_pm_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    pmcfg = (mlan_ds_pm_cfg *) req->pbuf;
    inac_to = &pmcfg->param.inactivity_to;
    pmcfg->sub_command = MLAN_OID_PM_CFG_INACTIVITY_TO;
    req->req_id = MLAN_IOCTL_PM_CFG;

    if ((data_length != 0) && (data_length != 3) && (data_length != 4)) {
        ret = -EINVAL;
        goto done;
    }

    req->action = MLAN_ACT_GET;
    if (data_length) {
        if (copy_from_user
            (data, wrq->u.data.pointer, sizeof(int) * data_length)) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        req->action = MLAN_ACT_SET;
        inac_to->timeout_unit = data[0];
        inac_to->unicast_timeout = data[1];
        inac_to->mcast_timeout = data[2];
        inac_to->ps_entry_timeout = data[3];
    }

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    /* Copy back current values regardless of GET/SET */
    data[0] = inac_to->timeout_unit;
    data[1] = inac_to->unicast_timeout;
    data[2] = inac_to->mcast_timeout;
    data[3] = inac_to->ps_entry_timeout;

    if (req->action == MLAN_ACT_GET) {
        wrq->u.data.length = 4;
        if (copy_to_user
            (wrq->u.data.pointer, data, wrq->u.data.length * sizeof(int))) {
            PRINTM(MERROR, "Copy to user failed\n");
            ret = -EFAULT;
            goto done;
        }
    }

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Set/Get system clock
 *
 *  @param priv         A pointer to moal_private structure
 *  @param wrq	        A pointer to iwreq structure
 *
 *  @return             0 --success, otherwise fail
 */
static int
woal_ecl_sys_clock(moal_private * priv, struct iwreq *wrq)
{
    int data[64];
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_misc_cfg *cfg = NULL;
    int data_length = wrq->u.data.length;
    int i = 0;

    ENTER();

    memset(data, 0, sizeof(data));

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_misc_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    cfg = (mlan_ds_misc_cfg *) req->pbuf;
    cfg->sub_command = MLAN_OID_MISC_SYS_CLOCK;
    req->req_id = MLAN_IOCTL_MISC_CFG;

    if (!data_length)
        req->action = MLAN_ACT_GET;
    else if (data_length <= MLAN_MAX_CLK_NUM) {
        req->action = MLAN_ACT_SET;
        if (copy_from_user
            (data, wrq->u.data.pointer, sizeof(int) * data_length)) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
    } else {
        PRINTM(MERROR, "Invalid arguments\n");
        ret = -EINVAL;
        goto done;
    }

    if (req->action == MLAN_ACT_GET) {
        /* Get configurable clocks */
        cfg->param.sys_clock.sys_clk_type = MLAN_CLK_CONFIGURABLE;
        if (MLAN_STATUS_SUCCESS !=
            woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            goto done;
        }

        /* Current system clock */
        data[0] = (int) cfg->param.sys_clock.cur_sys_clk;
        wrq->u.data.length = 1;

        data_length = MIN(cfg->param.sys_clock.sys_clk_num, MLAN_MAX_CLK_NUM);

        /* Configurable clocks */
        for (i = 0; i < data_length; i++) {
            data[i + wrq->u.data.length] =
                (int) cfg->param.sys_clock.sys_clk[i];
        }
        wrq->u.data.length += data_length;

        /* Get supported clocks */
        cfg->param.sys_clock.sys_clk_type = MLAN_CLK_SUPPORTED;
        if (MLAN_STATUS_SUCCESS !=
            woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            goto done;
        }

        data_length = MIN(cfg->param.sys_clock.sys_clk_num, MLAN_MAX_CLK_NUM);

        /* Supported clocks */
        for (i = 0; i < data_length; i++) {
            data[i + wrq->u.data.length] =
                (int) cfg->param.sys_clock.sys_clk[i];
        }

        wrq->u.data.length += data_length;

        if (copy_to_user
            (wrq->u.data.pointer, data, sizeof(int) * wrq->u.data.length)) {
            PRINTM(MERROR, "Copy to user failed\n");
            ret = -EFAULT;
            goto done;
        }
    } else {
        /* Set configurable clocks */
        cfg->param.sys_clock.sys_clk_type = MLAN_CLK_CONFIGURABLE;
        cfg->param.sys_clock.sys_clk_num = MIN(MLAN_MAX_CLK_NUM, data_length);
        for (i = 0; i < cfg->param.sys_clock.sys_clk_num; i++) {
            cfg->param.sys_clock.sys_clk[i] = (t_u16) data[i];
        }

        if (MLAN_STATUS_SUCCESS !=
            woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            goto done;
        }
    }
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Set/Get Band and Adhoc-band setting
 *
 *  @param priv         A pointer to moal_private structure
 *  @param wrq	        A pointer to iwreq structure
 *
 *  @return             0 --success, otherwise fail
 */
static int
woal_band_cfg(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    unsigned int i;
    int data[4];
    int user_data_len = wrq->u.data.length;
    t_u32 infra_band = 0;
    t_u32 adhoc_band = 0;
    t_u32 adhoc_channel = 0;
    t_u32 sec_chan_offset = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_radio_cfg *radio_cfg = NULL;

    ENTER();
    if (user_data_len > 4) {
        LEAVE();
        return -EINVAL;
    }

    if (user_data_len > 0) {
        if (priv->media_connected == MTRUE) {
            LEAVE();
            return -EOPNOTSUPP;
        }
    }

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_radio_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto error;
    }
    radio_cfg = (mlan_ds_radio_cfg *) req->pbuf;
    radio_cfg->sub_command = MLAN_OID_BAND_CFG;
    req->req_id = MLAN_IOCTL_RADIO_CFG;

    if (wrq->u.data.length == 0) {
        /* Get config_bands, adhoc_start_band and adhoc_channnel values from
           MLAN */
        req->action = MLAN_ACT_GET;
        if (MLAN_STATUS_SUCCESS !=
            woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            goto error;
        }
        data[0] = radio_cfg->param.band_cfg.config_bands;       /* Infra Band */
        data[1] = radio_cfg->param.band_cfg.adhoc_start_band;   /* Adhoc Band */
        data[2] = radio_cfg->param.band_cfg.adhoc_channel;      /* Adhoc
                                                                   Channel */
        wrq->u.data.length = 3;
        if (radio_cfg->param.band_cfg.adhoc_start_band & BAND_GN
            || radio_cfg->param.band_cfg.adhoc_start_band & BAND_AN) {
            data[3] = radio_cfg->param.band_cfg.sec_chan_offset;        /* secondary 
                                                                           offset 
                                                                         */
            wrq->u.data.length = 4;
        }

        if (copy_to_user
            (wrq->u.data.pointer, data, sizeof(int) * wrq->u.data.length)) {
            ret = -EFAULT;
            goto error;
        }
    } else {
        if (copy_from_user
            (data, wrq->u.data.pointer, sizeof(int) * wrq->u.data.length)) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto error;
        }

        /* To support only <b/bg/bgn/n> */
        infra_band = data[0];
        for (i = 0; i < sizeof(SupportedInfraBand); i++)
            if (infra_band == SupportedInfraBand[i])
                break;
        if (i == sizeof(SupportedInfraBand)) {
            ret = -EINVAL;
            goto error;
        }

        /* Set Adhoc band */
        if (user_data_len >= 2) {
            adhoc_band = data[1];
            for (i = 0; i < sizeof(SupportedAdhocBand); i++)
                if (adhoc_band == SupportedAdhocBand[i])
                    break;
            if (i == sizeof(SupportedAdhocBand)) {
                ret = -EINVAL;
                goto error;
            }
        }

        /* Set Adhoc channel */
        if (user_data_len >= 3) {
            adhoc_channel = data[2];
            if (adhoc_channel == 0) {
                /* Check if specified adhoc channel is non-zero */
                ret = -EINVAL;
                goto error;
            }
        }
        if (user_data_len == 4) {
            if (!(adhoc_band & (BAND_GN | BAND_AN))) {
                PRINTM(MERROR,
                       "11n is not enabled for adhoc, can not set secondary channel offset\n");
                ret = -EINVAL;
                goto error;
            }
            sec_chan_offset = data[3];
            if ((sec_chan_offset != NO_SEC_CHANNEL) &&
                (sec_chan_offset != SEC_CHANNEL_ABOVE) &&
                (sec_chan_offset != SEC_CHANNEL_BELOW)) {
                PRINTM(MERROR,
                       "Invalid secondary channel offset, only allowed 0, 1 or 3\n");
                ret = -EINVAL;
                goto error;
            }
        }
        /* Set config_bands and adhoc_start_band values to MLAN */
        req->action = MLAN_ACT_SET;
        radio_cfg->param.band_cfg.config_bands = infra_band;
        radio_cfg->param.band_cfg.adhoc_start_band = adhoc_band;
        radio_cfg->param.band_cfg.adhoc_channel = adhoc_channel;
        radio_cfg->param.band_cfg.sec_chan_offset = sec_chan_offset;
        if (MLAN_STATUS_SUCCESS !=
            woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            goto error;
        }
    }

  error:
    if (req)
        kfree(req);

    LEAVE();
    return ret;
}

/** 
 *  @brief Set/Get BCA timeshare
 *  
 *  @param priv         A pointer to moal_private structure
 *  @param wrq          A pointer to iwreq structure
 *
 *  @return             0 --success, otherwise fail
 */
static int
woal_bca_time_share(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    int data[3];
    int user_data_len = wrq->u.data.length;
    mlan_ioctl_req *req = NULL;
    mlan_ds_bca_cfg *bca_cfg = NULL;

    ENTER();

    if (user_data_len != 1 && user_data_len != 3) {
        PRINTM(MERROR, "Invalid number of parameters\n");
        LEAVE();
        return -EINVAL;
    }

    if (copy_from_user(data, wrq->u.data.pointer, sizeof(int) * user_data_len)) {
        PRINTM(MERROR, "Copy from user failed\n");
        LEAVE();
        return -EFAULT;
    }

    if ((data[0] > TRAFFIC_TYPE_MEDIUMHIGH) && (data[0] != TRAFFIC_TYPE_RESET)) {
        PRINTM(MERROR, "Invalid traffic_type\n");
        LEAVE();
        return -EINVAL;
    }

    if (user_data_len == 3) {
        /* Valid Range for timeshare_interval: < 20 ... 60_000 > ms */
        if (data[1] < MIN_TIMESHARE_INTERVAL ||
            data[1] > MAX_TIMESHARE_INTERVAL) {
            PRINTM(MERROR,
                   "Invalid timeshare_interval Range: < 20 ... 60000 > ms\n");
            LEAVE();
            return -EINVAL;
        }
        data[1] -= (data[1] % 10);      /* If value is not multiple of 10 then
                                           take the floor value */

        /* If value is not multiple of 10 then take the floor value */
        data[2] -= (data[2] % 10);
        /* Valid Range for timeshare_interval: < 0 ... bt_time > */
        if (data[2] < 0 || data[2] > data[1]) {
            PRINTM(MERROR,
                   "Invalid bt_time  Range: < 0 ... timeshare_interval > ms\n");
            LEAVE();
            return -EINVAL;
        }
    }

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_bca_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto error;
    }
    bca_cfg = (mlan_ds_bca_cfg *) req->pbuf;
    bca_cfg->sub_command = MLAN_OID_BCA_TS;
    req->req_id = MLAN_IOCTL_BCA_CFG;
    req->action = MLAN_ACT_GET;

    if (user_data_len > 0)
        bca_cfg->param.bca_ts.traffic_type = data[0];
    if (user_data_len == 3) {
        req->action = MLAN_ACT_SET;
        bca_cfg->param.bca_ts.timeshare_interval = data[1];
        bca_cfg->param.bca_ts.bt_time = data[2];
    }

    PRINTM(MINFO, "traffic_type=0x%x", data[0]);
    if (user_data_len == 3)
        PRINTM(MINFO, " timeshare_interval=0x%x bt_time=0x%x\n", data[1],
               data[2]);

    /* Set/Get BCA time share values from MLAN */
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto error;
    }

    if (user_data_len < 3) {
        data[0] = bca_cfg->param.bca_ts.traffic_type;
        data[1] = bca_cfg->param.bca_ts.timeshare_interval;
        data[2] = bca_cfg->param.bca_ts.bt_time;

        wrq->u.data.length = 3;
        if (copy_to_user(wrq->u.data.pointer, data, sizeof(data))) {
            PRINTM(MERROR, "Copy to user failed\n");
            ret = -EFAULT;
            goto error;
        }
    }

  error:
    if (req)
        kfree(req);

    LEAVE();
    return ret;
}

/**
 *  @brief Read/Write adapter registers value 
 *
 *  @param priv         A pointer to moal_private structure
 *  @param wrq	        A pointer to iwreq structure
 *
 *  @return             0 --success, otherwise fail
 */
static int
woal_reg_read_write(moal_private * priv, struct iwreq *wrq)
{
    int data[3];
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_reg_mem *reg = NULL;
    int data_length = wrq->u.data.length;

    ENTER();

    memset(data, 0, sizeof(data));

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_reg_mem));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    reg = (mlan_ds_reg_mem *) req->pbuf;
    reg->sub_command = MLAN_OID_REG_RW;
    req->req_id = MLAN_IOCTL_REG_MEM;

    if (data_length == 2) {
        req->action = MLAN_ACT_GET;
    } else if (data_length == 3) {
        req->action = MLAN_ACT_SET;
    } else {
        ret = -EINVAL;
        goto done;
    }
    if (copy_from_user(data, wrq->u.data.pointer, sizeof(int) * data_length)) {
        PRINTM(MERROR, "Copy from user failed\n");
        ret = -EFAULT;
        goto done;
    }
    reg->param.reg_rw.type = (t_u32) data[0];
    reg->param.reg_rw.offset = (t_u32) data[1];
    if (data_length == 3)
        reg->param.reg_rw.value = (t_u32) data[2];

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    if (req->action == MLAN_ACT_GET) {
        if (copy_to_user
            (wrq->u.data.pointer, &reg->param.reg_rw.value, sizeof(int))) {
            PRINTM(MERROR, "Copy to user failed\n");
            ret = -EFAULT;
            goto done;
        }
        wrq->u.data.length = 1;
    }

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Read the EEPROM contents of the card 
 *
 *  @param priv         A pointer to moal_private structure
 *  @param wrq	        A pointer to iwreq structure
 *
 *  @return             0 --success, otherwise fail
 */
static int
woal_read_eeprom(moal_private * priv, struct iwreq *wrq)
{
    int data[2];
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_reg_mem *reg = NULL;
    int data_length = wrq->u.data.length;

    ENTER();

    memset(data, 0, sizeof(data));

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_reg_mem));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    reg = (mlan_ds_reg_mem *) req->pbuf;
    reg->sub_command = MLAN_OID_EEPROM_RD;
    req->req_id = MLAN_IOCTL_REG_MEM;

    if (data_length == 2) {
        req->action = MLAN_ACT_GET;
    } else {
        ret = -EINVAL;
        goto done;
    }
    if (copy_from_user(data, wrq->u.data.pointer, sizeof(int) * data_length)) {
        PRINTM(MERROR, "Copy from user failed\n");
        ret = -EFAULT;
        goto done;
    }

    reg->param.rd_eeprom.offset = (t_u16) data[0];
    reg->param.rd_eeprom.byte_count = (t_u16) data[1];

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    if (req->action == MLAN_ACT_GET) {
        wrq->u.data.length = reg->param.rd_eeprom.byte_count;
        if (copy_to_user
            (wrq->u.data.pointer, reg->param.rd_eeprom.value,
             wrq->u.data.length)) {
            PRINTM(MERROR, "Copy to user failed\n");
            ret = -EFAULT;
            goto done;
        }
    }

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Read/Write device memory value 
 *
 *  @param priv         A pointer to moal_private structure
 *  @param wrq	        A pointer to iwreq structure
 *
 *  @return             0 --success, otherwise fail
 */
static int
woal_mem_read_write(moal_private * priv, struct iwreq *wrq)
{
    t_u32 data[2];
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_reg_mem *reg_mem = NULL;
    int data_length = wrq->u.data.length;

    ENTER();

    memset(data, 0, sizeof(data));

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_reg_mem));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    reg_mem = (mlan_ds_reg_mem *) req->pbuf;
    reg_mem->sub_command = MLAN_OID_MEM_RW;
    req->req_id = MLAN_IOCTL_REG_MEM;

    if (data_length == 1) {
        PRINTM(MINFO, "MEM_RW: GET\n");
        req->action = MLAN_ACT_GET;
    } else if (data_length == 2) {
        PRINTM(MINFO, "MEM_RW: SET\n");
        req->action = MLAN_ACT_SET;
    } else {
        ret = -EINVAL;
        goto done;
    }
    if (copy_from_user(data, wrq->u.data.pointer, sizeof(int) * data_length)) {
        PRINTM(MERROR, "Copy from user failed\n");
        ret = -EFAULT;
        goto done;
    }

    reg_mem->param.mem_rw.addr = (t_u32) data[0];
    if (data_length == 2)
        reg_mem->param.mem_rw.value = (t_u32) data[1];

    PRINTM(MINFO, "MEM_RW: Addr=0x%x, Value=0x%x\n",
           (int) reg_mem->param.mem_rw.addr, (int) reg_mem->param.mem_rw.value);

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    if (req->action == MLAN_ACT_GET) {
        if (copy_to_user
            (wrq->u.data.pointer, &reg_mem->param.mem_rw.value, sizeof(int))) {
            PRINTM(MERROR, "Copy to user failed\n");
            ret = -EFAULT;
            goto done;
        }
        wrq->u.data.length = 1;
    }

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Get LOG
 *
 *  @param priv                 A pointer to moal_private structure
 *  @param wrq			A pointer to iwreq structure
 *
 *  @return 	   	 	0 --success, otherwise fail
 */
static int
woal_get_log(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ds_get_stats stats;
    char *buf = NULL;

    ENTER();

    PRINTM(MINFO, " GET STATS\n");
    if (!(buf = kmalloc(GETLOG_BUFSIZE, GFP_ATOMIC))) {
        PRINTM(MERROR, "kmalloc failed!\n");
        ret = -ENOMEM;
        goto done;
    }

    memset(&stats, 0, sizeof(mlan_ds_get_stats));
    if (MLAN_STATUS_SUCCESS !=
        woal_get_stats_info(priv, MOAL_IOCTL_WAIT, &stats)) {
        ret = -EFAULT;
        goto done;
    }

    if (wrq->u.data.pointer) {
        sprintf(buf, "\n"
                "mcasttxframe     %lu\n"
                "failed           %lu\n"
                "retry            %lu\n"
                "multiretry       %lu\n"
                "framedup         %lu\n"
                "rtssuccess       %lu\n"
                "rtsfailure       %lu\n"
                "ackfailure       %lu\n"
                "rxfrag           %lu\n"
                "mcastrxframe     %lu\n"
                "fcserror         %lu\n"
                "txframe          %lu\n"
                "wepicverrcnt-1   %lu\n"
                "wepicverrcnt-2   %lu\n"
                "wepicverrcnt-3   %lu\n"
                "wepicverrcnt-4   %lu\n",
                stats.mcast_tx_frame,
                stats.failed,
                stats.retry,
                stats.multi_retry,
                stats.frame_dup,
                stats.rts_success,
                stats.rts_failure,
                stats.ack_failure,
                stats.rx_frag,
                stats.mcast_rx_frame,
                stats.fcs_error,
                stats.tx_frame,
                stats.wep_icv_error[0],
                stats.wep_icv_error[1],
                stats.wep_icv_error[2], stats.wep_icv_error[3]);
        wrq->u.data.length = strlen(buf) + 1;
        if (copy_to_user(wrq->u.data.pointer, buf, wrq->u.data.length)) {
            PRINTM(MERROR, "Copy to user failed\n");
            ret = -EFAULT;
        }
    }
  done:
    if (buf)
        kfree(buf);
    LEAVE();
    return ret;
}

/**
 *  @brief Deauthenticate
 *
 *  @param priv                 A pointer to moal_private structure
 *  @param wrq			A pointer to iwreq structure
 *
 *  @return 	   	 	0 --success, otherwise fail
 */
static int
woal_deauth(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    struct sockaddr saddr;

    ENTER();
    if (wrq->u.data.length) {
        /* Deauth mentioned BSSID */
        if (copy_from_user(&saddr, wrq->u.data.pointer, sizeof(saddr))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        if (MLAN_STATUS_SUCCESS !=
            woal_disconnect(priv, MOAL_IOCTL_WAIT, (t_u8 *) saddr.sa_data))
            ret = -EFAULT;
    } else {
        if (MLAN_STATUS_SUCCESS != woal_disconnect(priv, MOAL_IOCTL_WAIT, NULL))
            ret = -EFAULT;
    }
  done:
    LEAVE();
    return ret;
}

/**
 *  @brief Set/Get TX power configurations
 *  
 *  @param priv     A pointer to moal_private structure
 *  @param wrq      A pointer to iwreq structure
 *  
 *  @return         0 --success, otherwise fail
 */
static int
woal_tx_power_cfg(moal_private * priv, struct iwreq *wrq)
{
    int data[5], user_data_len;
    int ret = 0;
    mlan_bss_info bss_info;
    mlan_ds_power_cfg *pcfg = NULL;
    mlan_ioctl_req *req = NULL;
    ENTER();

    memset(&bss_info, 0, sizeof(bss_info));
    woal_get_bss_info(priv, MOAL_IOCTL_WAIT, &bss_info);

    memset(data, 0, sizeof(data));
    user_data_len = wrq->u.data.length;

    if (user_data_len) {
        if (copy_from_user
            (data, wrq->u.data.pointer, sizeof(int) * user_data_len)) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        switch (user_data_len) {
        case 1:
            if (data[0] != 0xFF)
                ret = -EINVAL;
            break;
        case 2:
        case 4:
            if (data[0] == 0xFF) {
                ret = -EINVAL;
                break;
            }
            if ((unsigned int) data[1] < bss_info.min_power_level) {
                PRINTM(MERROR,
                       "The set powercfg rate value %d dBm is out of range (%d dBm-%d dBm)!\n",
                       data[1], (int) bss_info.min_power_level,
                       (int) bss_info.max_power_level);
                ret = -EINVAL;
                break;
            }
            if (user_data_len == 4) {
                if (data[1] > data[2]) {
                    PRINTM(MERROR, "Min power should be less than maximum!\n");
                    ret = -EINVAL;
                    break;
                }
                if (data[3] < 0) {
                    PRINTM(MERROR, "Step should not less than 0!\n");
                    ret = -EINVAL;
                    break;
                }
                if ((unsigned int) data[2] > bss_info.max_power_level) {
                    PRINTM(MERROR,
                           "The set powercfg rate value %d dBm is out of range (%d dBm-%d dBm)!\n",
                           data[2], (int) bss_info.min_power_level,
                           (int) bss_info.max_power_level);
                    ret = -EINVAL;
                    break;
                }
                if (data[3] > data[2] - data[1]) {
                    PRINTM(MERROR,
                           "Step should not greater than power difference!\n");
                    ret = -EINVAL;
                    break;
                }
            }
            break;
        default:
            ret = -EINVAL;
            break;
        }
        if (ret)
            goto done;
    }

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_power_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    pcfg = (mlan_ds_power_cfg *) req->pbuf;
    pcfg->sub_command = MLAN_OID_POWER_CFG_EXT;
    req->req_id = MLAN_IOCTL_POWER_CFG;
    if (!user_data_len)
        req->action = MLAN_ACT_GET;
    else {
        req->action = MLAN_ACT_SET;
        pcfg->param.power_ext.len = user_data_len;
        memcpy((t_u8 *) & pcfg->param.power_ext.power_data, (t_u8 *) data,
               sizeof(data));
    }
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    if (!user_data_len) {
        if (copy_to_user
            (wrq->u.data.pointer, (t_u8 *) & pcfg->param.power_ext.power_data,
             sizeof(int) * pcfg->param.power_ext.len)) {
            ret = -EFAULT;
            goto done;
        }
        wrq->u.data.length = pcfg->param.power_ext.len;
    }
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Get Tx/Rx data rates
 *  
 *  @param priv     A pointer to moal_private structure
 *  @param wrq      A pointer to iwreq structure
 *  
 *  @return         0 --success, otherwise fail
 */
static int
woal_get_txrx_rate(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ds_rate *rate = NULL;
    mlan_ioctl_req *req = NULL;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_rate));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    rate = (mlan_ds_rate *) req->pbuf;
    rate->sub_command = MLAN_OID_GET_DATA_RATE;
    req->req_id = MLAN_IOCTL_RATE;
    req->action = MLAN_ACT_GET;

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    if (copy_to_user
        (wrq->u.data.pointer, (t_u8 *) & rate->param.data_rate,
         sizeof(int) * 2)) {
        ret = -EFAULT;
        goto done;
    }
    wrq->u.data.length = 2;
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Turn on/off the sdio clock
 *  
 *  @param priv     A pointer to moal_private structure
 *  @param wrq      A pointer to iwreq structure
 *  
 *  @return         0 --success, otherwise fail
 */
static int
woal_sdio_clock_ioctl(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    int data = 2;
    /* Initialize the clock state as on */
    static int clock_state = 1;

    ENTER();

    if (wrq->u.data.length) {
        if (copy_from_user(&data, wrq->u.data.pointer, sizeof(int))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
    } else {
        wrq->u.data.length = sizeof(clock_state) / sizeof(int);
        if (copy_to_user(wrq->u.data.pointer, &clock_state, sizeof(int))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }

        goto done;
    }
    switch (data) {
    case CMD_DISABLED:
        PRINTM(MINFO, "SDIO clock is turned off\n");
        ret = woal_sdio_set_bus_clock(priv->phandle, MFALSE);
        clock_state = data;
        break;
    case CMD_ENABLED:
        PRINTM(MINFO, "SDIO clock is turned on\n");
        ret = woal_sdio_set_bus_clock(priv->phandle, MTRUE);
        clock_state = data;
        break;
    default:
        ret = -EINVAL;
        PRINTM(MINFO, "sdioclock: wrong parameter\n");
        break;
    }
  done:
    return ret;
}

/**
 *  @brief Set/Get beacon interval
 *  
 *  @param priv     A pointer to moal_private structure
 *  @param wrq      A pointer to iwreq structure
 *  
 *  @return         0 --success, otherwise fail
 */
static int
woal_beacon_interval(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ds_bss *bss = NULL;
    mlan_ioctl_req *req = NULL;
    int bcn = 0;

    ENTER();

    if (wrq->u.data.length) {
        if (copy_from_user(&bcn, wrq->u.data.pointer, sizeof(int))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        if ((bcn < MLAN_MIN_BEACON_INTERVAL) ||
            (bcn > MLAN_MAX_BEACON_INTERVAL)) {
            ret = -EINVAL;
            goto done;
        }
    }

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_bss));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    bss = (mlan_ds_bss *) req->pbuf;
    bss->sub_command = MLAN_OID_IBSS_BCN_INTERVAL;
    req->req_id = MLAN_IOCTL_BSS;
    if (!wrq->u.data.length)
        req->action = MLAN_ACT_GET;
    else {
        req->action = MLAN_ACT_SET;
        bss->param.bcn_interval = bcn;
    }

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    if (copy_to_user
        (wrq->u.data.pointer, (t_u8 *) & bss->param.bcn_interval,
         sizeof(int))) {
        ret = -EFAULT;
        goto done;
    }
    wrq->u.data.length = 1;
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Set/Get ATIM window
 *  
 *  @param priv     A pointer to moal_private structure
 *  @param wrq      A pointer to iwreq structure
 *  
 *  @return         0 --success, otherwise fail
 */
static int
woal_atim_window(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ds_bss *bss = NULL;
    mlan_ioctl_req *req = NULL;
    int atim = 0;

    ENTER();

    if (wrq->u.data.length) {
        if (copy_from_user(&atim, wrq->u.data.pointer, sizeof(int))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        if ((atim < 0) || (atim > MLAN_MAX_ATIM_WINDOW)) {
            ret = -EINVAL;
            goto done;
        }
    }

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_bss));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    bss = (mlan_ds_bss *) req->pbuf;
    bss->sub_command = MLAN_OID_IBSS_ATIM_WINDOW;
    req->req_id = MLAN_IOCTL_BSS;
    if (!wrq->u.data.length)
        req->action = MLAN_ACT_GET;
    else {
        req->action = MLAN_ACT_SET;
        bss->param.atim_window = atim;
    }

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    if (copy_to_user
        (wrq->u.data.pointer, (t_u8 *) & bss->param.atim_window, sizeof(int))) {
        ret = -EFAULT;
        goto done;
    }
    wrq->u.data.length = 1;
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 * @brief Set/Get TX data rate
 * 
 * @param priv     A pointer to moal_private structure
 * @param wrq      A pointer to iwreq structure
 * 
 * @return           0 --success, otherwise fail
 */
static int
woal_set_get_txrate(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ds_rate *rate = NULL;
    mlan_ioctl_req *req = NULL;
    int rateindex = 0;

    ENTER();
    if (wrq->u.data.length) {
        if (copy_from_user(&rateindex, wrq->u.data.pointer, sizeof(int))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
    }
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_rate));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    rate = (mlan_ds_rate *) req->pbuf;
    rate->param.rate_cfg.rate_type = MLAN_RATE_INDEX;
    rate->sub_command = MLAN_OID_RATE_CFG;
    req->req_id = MLAN_IOCTL_RATE;
    if (!wrq->u.data.length)
        req->action = MLAN_ACT_GET;
    else {
        req->action = MLAN_ACT_SET;
        if (rateindex == AUTO_RATE)
            rate->param.rate_cfg.is_rate_auto = 1;
        else {
            if ((rateindex != MLAN_RATE_INDEX_MCS32) &&
                ((rateindex < 0) || (rateindex > MLAN_RATE_INDEX_MCS7))) {
                ret = -EINVAL;
                goto done;
            }
        }
        rate->param.rate_cfg.rate = rateindex;
    }

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    } else {
        if (wrq->u.data.length)
            priv->rate_index = rateindex;
    }
    if (!wrq->u.data.length) {
        if (rate->param.rate_cfg.is_rate_auto)
            rateindex = AUTO_RATE;
        else
            rateindex = rate->param.rate_cfg.rate;
        wrq->u.data.length = 1;
        if (copy_to_user(wrq->u.data.pointer, &rateindex, sizeof(int))) {
            ret = -EFAULT;
        }
    }
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 * @brief Set/Get region code
 * 
 * @param priv     A pointer to moal_private structure
 * @param wrq      A pointer to iwreq structure
 * 
 * @return           0 --success, otherwise fail
 */
static int
woal_set_get_regioncode(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ds_misc_cfg *cfg = NULL;
    mlan_ioctl_req *req = NULL;
    int region = 0;

    ENTER();

    if (wrq->u.data.length) {
        if (copy_from_user(&region, wrq->u.data.pointer, sizeof(int))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
    }

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_misc_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    cfg = (mlan_ds_misc_cfg *) req->pbuf;
    cfg->sub_command = MLAN_OID_MISC_REGION;
    req->req_id = MLAN_IOCTL_MISC_CFG;
    if (!wrq->u.data.length)
        req->action = MLAN_ACT_GET;
    else {
        req->action = MLAN_ACT_SET;
        cfg->param.region_code = region;
    }

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    if (!wrq->u.data.length) {
        wrq->u.data.length = 1;
        if (copy_to_user
            (wrq->u.data.pointer, &cfg->param.region_code, sizeof(int)))
            ret = -EFAULT;
    }
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 * @brief Set/Get radio
 * 
 * @param priv     A pointer to moal_private structure
 * @param wrq      A pointer to iwreq structure
 * 
 * @return           0 --success, otherwise fail
 */
static int
woal_set_get_radio(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_bss_info bss_info;
    int option = 0;

    ENTER();

    memset(&bss_info, 0, sizeof(bss_info));

    if (wrq->u.data.length) {
        /* Set radio */
        if (copy_from_user(&option, wrq->u.data.pointer, sizeof(int))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        if (MLAN_STATUS_SUCCESS != woal_set_radio(priv, (t_u8) option))
            ret = -EFAULT;
    } else {
        /* Get radio status */
        woal_get_bss_info(priv, MOAL_IOCTL_WAIT, &bss_info);
        wrq->u.data.length = 1;
        if (copy_to_user
            (wrq->u.data.pointer, &bss_info.radio_on,
             sizeof(bss_info.radio_on))) {
            PRINTM(MERROR, "Copy to user failed\n");
            ret = -EFAULT;
        }
    }
  done:
    LEAVE();
    return ret;
}

#ifdef DEBUG_LEVEL1
/** 
 *  @brief Get/Set the bit mask of driver debug message control
 *
 *  @param priv			A pointer to moal_private structure
 *  @param wrq			A pointer to wrq structure
 *
 *  @return             0 --success, otherwise fail
 */
static int
woal_drv_dbg(moal_private * priv, struct iwreq *wrq)
{
    int data[4];
    int ret = 0;

    ENTER();

    if (!wrq->u.data.length) {
        data[0] = drvdbg;
        data[1] = ifdbg;
        /* Return the current driver debug bit masks */
        if (copy_to_user(wrq->u.data.pointer, data, sizeof(int) * 2)) {
            PRINTM(MERROR, "Copy to user failed\n");
            ret = -EFAULT;
            goto drvdbgexit;
        }
        wrq->u.data.length = 2;
    } else if (wrq->u.data.length < 3) {
        /* Get the driver debug bit masks */
        if (copy_from_user
            (data, wrq->u.data.pointer, sizeof(int) * wrq->u.data.length)) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto drvdbgexit;
        }
        drvdbg = data[0];
        if (wrq->u.data.length == 2)
            ifdbg = data[1];
    } else {
        PRINTM(MERROR, "Invalid parameter number\n");
        goto drvdbgexit;
    }

    printk(KERN_ALERT "drvdbg = 0x%08lx\n", drvdbg);
#ifdef DEBUG_LEVEL2
    printk(KERN_ALERT "MINFO  (%08lx) %s\n", MINFO,
           (drvdbg & MINFO) ? "X" : "");
    printk(KERN_ALERT "MWARN  (%08lx) %s\n", MWARN,
           (drvdbg & MWARN) ? "X" : "");
    printk(KERN_ALERT "MENTRY (%08lx) %s\n", MENTRY,
           (drvdbg & MENTRY) ? "X" : "");
#endif
    printk(KERN_ALERT "MFW_D  (%08lx) %s\n", MFW_D,
           (drvdbg & MFW_D) ? "X" : "");
    printk(KERN_ALERT "MCMD_D (%08lx) %s\n", MCMD_D,
           (drvdbg & MCMD_D) ? "X" : "");
    printk(KERN_ALERT "MDAT_D (%08lx) %s\n", MDAT_D,
           (drvdbg & MDAT_D) ? "X" : "");
    printk(KERN_ALERT "MIOCTL (%08lx) %s\n", MIOCTL,
           (drvdbg & MIOCTL) ? "X" : "");
    printk(KERN_ALERT "MINTR  (%08lx) %s\n", MINTR,
           (drvdbg & MINTR) ? "X" : "");
    printk(KERN_ALERT "MEVENT (%08lx) %s\n", MEVENT,
           (drvdbg & MEVENT) ? "X" : "");
    printk(KERN_ALERT "MCMND  (%08lx) %s\n", MCMND,
           (drvdbg & MCMND) ? "X" : "");
    printk(KERN_ALERT "MDATA  (%08lx) %s\n", MDATA,
           (drvdbg & MDATA) ? "X" : "");
    printk(KERN_ALERT "MERROR (%08lx) %s\n", MERROR,
           (drvdbg & MERROR) ? "X" : "");
    printk(KERN_ALERT "MFATAL (%08lx) %s\n", MFATAL,
           (drvdbg & MFATAL) ? "X" : "");
    printk(KERN_ALERT "MMSG   (%08lx) %s\n", MMSG, (drvdbg & MMSG) ? "X" : "");
    printk(KERN_ALERT "ifdbg = 0x%08lx\n", ifdbg);
    printk(KERN_ALERT "MIF_D  (%08lx) %s\n", MIF_D, (ifdbg & MIF_D) ? "X" : "");

  drvdbgexit:
    LEAVE();
    return ret;
}
#endif /* DEBUG_LEVEL1 */

/**
 * @brief Set/Get Tx/Rx antenna
 * 
 * @param priv     A pointer to moal_private structure
 * @param wrq      A pointer to iwreq structure
 * 
 * @return         0 --success, otherwise fail
 */
static int
woal_set_get_tx_rx_ant(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ds_radio_cfg *radio = NULL;
    mlan_ioctl_req *req = NULL;
    int data = 0;

    ENTER();
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_radio_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    radio = (mlan_ds_radio_cfg *) req->pbuf;
    radio->sub_command = MLAN_OID_ANT_CFG;
    req->req_id = MLAN_IOCTL_RADIO_CFG;
    if (wrq->u.data.length) {
        if (copy_from_user(&data, wrq->u.data.pointer, sizeof(int))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        req->action = MLAN_ACT_SET;
        radio->param.antenna = data;
    } else
        req->action = MLAN_ACT_GET;
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    if (!wrq->u.data.length) {
        data = radio->param.antenna;
        if (copy_to_user(wrq->u.data.pointer, &data, sizeof(int))) {
            ret = -EFAULT;
            goto done;
        }
        wrq->u.data.length = 1;
    }
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 * @brief Set/Get QoS configuration
 * 
 * @param priv     A pointer to moal_private structure
 * @param wrq      A pointer to iwreq structure
 * 
 * @return         0 --success, otherwise fail
 */
static int
woal_set_get_qos_cfg(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ds_wmm_cfg *cfg = NULL;
    mlan_ioctl_req *req = NULL;
    int data = 0;

    ENTER();
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_wmm_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    cfg = (mlan_ds_wmm_cfg *) req->pbuf;
    cfg->sub_command = MLAN_OID_WMM_CFG_QOS;
    req->req_id = MLAN_IOCTL_WMM_CFG;
    if (wrq->u.data.length) {
        if (copy_from_user(&data, wrq->u.data.pointer, sizeof(int))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        req->action = MLAN_ACT_SET;
        cfg->param.qos_cfg = (t_u8) data;
    } else
        req->action = MLAN_ACT_GET;
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    if (!wrq->u.data.length) {
        data = (int) cfg->param.qos_cfg;
        if (copy_to_user(wrq->u.data.pointer, &data, sizeof(int))) {
            ret = -EFAULT;
            goto done;
        }
        wrq->u.data.length = 1;
    }
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 * @brief Set/Get LDO configuration
 * 
 * @param priv     A pointer to moal_private structure
 * @param wrq      A pointer to iwreq structure
 * 
 * @return         0 --success, otherwise fail
 */
static int
woal_set_get_ldo_cfg(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ds_misc_cfg *cfg = NULL;
    mlan_ioctl_req *req = NULL;
    int data = 0;

    ENTER();
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_misc_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    cfg = (mlan_ds_misc_cfg *) req->pbuf;
    cfg->sub_command = MLAN_OID_MISC_LDO;
    req->req_id = MLAN_IOCTL_MISC_CFG;
    if (wrq->u.data.length) {
        if (copy_from_user(&data, wrq->u.data.pointer, sizeof(int))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        if (data != LDO_INTERNAL && data != LDO_EXTERNAL) {
            PRINTM(MERROR, "Invalid parameter, LDO config not changed\n");
            ret = -EINVAL;
            goto done;
        }
        req->action = MLAN_ACT_SET;
        cfg->param.ldo_cfg = (t_u16) data;
    } else
        req->action = MLAN_ACT_GET;
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    if (!wrq->u.data.length) {
        data = (int) cfg->param.ldo_cfg;
        if (copy_to_user(wrq->u.data.pointer, &data, sizeof(int))) {
            ret = -EFAULT;
            goto done;
        }
        wrq->u.data.length = 1;
    }
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 * @brief Set/Get WWS mode
 * 
 * @param priv     A pointer to moal_private structure
 * @param wrq      A pointer to iwreq structure
 * 
 * @return         0 --success, otherwise fail
 */
static int
woal_wws_cfg(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ds_misc_cfg *wws = NULL;
    mlan_ioctl_req *req = NULL;
    int data = 0;

    ENTER();
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_misc_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    wws = (mlan_ds_misc_cfg *) req->pbuf;
    wws->sub_command = MLAN_OID_MISC_WWS;
    req->req_id = MLAN_IOCTL_MISC_CFG;
    if (wrq->u.data.length) {
        if (copy_from_user(&data, wrq->u.data.pointer, sizeof(int))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        if (data != CMD_DISABLED && data != CMD_ENABLED) {
            ret = -EINVAL;
            goto done;
        }
        req->action = MLAN_ACT_SET;
        wws->param.wws_cfg = data;
    } else
        req->action = MLAN_ACT_GET;
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    if (!wrq->u.data.length) {
        data = wws->param.wws_cfg;
        if (copy_to_user(wrq->u.data.pointer, &data, sizeof(int))) {
            ret = -EFAULT;
            goto done;
        }
        wrq->u.data.length = 1;
    }
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 * @brief Set/Get sleep period
 * 
 * @param priv     A pointer to moal_private structure
 * @param wrq      A pointer to iwreq structure
 * 
 * @return         0 --success, otherwise fail
 */
static int
woal_sleep_pd(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ds_pm_cfg *pm_cfg = NULL;
    mlan_ioctl_req *req = NULL;
    int data = 0;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_pm_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    pm_cfg = (mlan_ds_pm_cfg *) req->pbuf;
    pm_cfg->sub_command = MLAN_OID_PM_CFG_SLEEP_PD;
    req->req_id = MLAN_IOCTL_PM_CFG;
    if (wrq->u.data.length) {
        if (copy_from_user(&data, wrq->u.data.pointer, sizeof(int))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        if ((data <= MAX_SLEEP_PERIOD && data >= MIN_SLEEP_PERIOD) ||
            (data == 0)
            || (data == SLEEP_PERIOD_RESERVED_FF)
            ) {
            req->action = MLAN_ACT_SET;
            pm_cfg->param.sleep_period = data;
        } else {
            ret = -EINVAL;
            goto done;
        }
    } else
        req->action = MLAN_ACT_GET;

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    if (!wrq->u.data.length) {
        data = pm_cfg->param.sleep_period;
        if (copy_to_user(wrq->u.data.pointer, &data, sizeof(int))) {
            ret = -EFAULT;
            goto done;
        }
        wrq->u.data.length = 1;
    }

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 * @brief Configure sleep parameters
 *
 * @param priv         A pointer to moal_private structure
 * @param req          A pointer to ifreq structure
 *
 * @return             0 --success, otherwise fail  
 */
static int
woal_sleep_params_ioctl(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_pm_cfg *pm = NULL;
    mlan_ds_sleep_params *psleep_params = NULL;
    int data[6] = { 0 }, i;
#ifdef DEBUG_LEVEL1
    char err_str[][35] = { {"sleep clock error in ppm"},
    {"wakeup offset in usec"},
    {"clock stabilization time in usec"},
    {"value of reserved for debug"}
    };
#endif

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_pm_cfg));
    if (req == NULL) {
        LEAVE();
        return -ENOMEM;
    }

    pm = (mlan_ds_pm_cfg *) req->pbuf;
    pm->sub_command = MLAN_OID_PM_CFG_SLEEP_PARAMS;
    req->req_id = MLAN_IOCTL_PM_CFG;
    psleep_params = (pmlan_ds_sleep_params) & pm->param.sleep_params;

    if (wrq->u.data.length == 0) {
        req->action = MLAN_ACT_GET;
    } else if (wrq->u.data.length == 6) {
        if (copy_from_user(data, wrq->u.data.pointer, sizeof(int) *
                           wrq->u.data.length)) {
            /* copy_from_user failed */
            PRINTM(MERROR, "S_PARAMS: copy from user failed\n");
            return -EINVAL;
        }
#define MIN_VAL 0x0000
#define MAX_VAL 0xFFFF
        for (i = 0; i < 6; i++) {
            if ((i == 3) || (i == 4)) {
                /* These two cases are handled below the loop */
                continue;
            }
            if (data[i] < MIN_VAL || data[i] > MAX_VAL) {
                PRINTM(MERROR, "Invalid %s (0-65535)!\n", err_str[i]);
                ret = -EINVAL;
                goto done;
            }
        }
        if (data[3] < 0 || data[3] > 2) {
            PRINTM(MERROR, "Invalid control periodic calibration (0-2)!\n");
            ret = -EINVAL;
            goto done;
        }
        if (data[4] < 0 || data[4] > 2) {
            PRINTM(MERROR, "Invalid control of external sleep clock (0-2)!\n");
            ret = -EINVAL;
            goto done;
        }
        req->action = MLAN_ACT_SET;
        psleep_params->error = data[0];
        psleep_params->offset = data[1];
        psleep_params->stable_time = data[2];
        psleep_params->cal_control = data[3];
        psleep_params->ext_sleep_clk = data[4];
        psleep_params->reserved = data[5];
    } else {
        ret = -EINVAL;
        goto done;
    }

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    data[0] = psleep_params->error;
    data[1] = psleep_params->offset;
    data[2] = psleep_params->stable_time;
    data[3] = psleep_params->cal_control;
    data[4] = psleep_params->ext_sleep_clk;
    data[5] = psleep_params->reserved;
    wrq->u.data.length = 6;

    if (copy_to_user(wrq->u.data.pointer, data, sizeof(int) *
                     wrq->u.data.length)) {
        PRINTM(MERROR, "QCONFIG: copy to user failed\n");
        ret = -EFAULT;
        goto done;
    }

  done:
    if (req)
        kfree(req);

    LEAVE();
    return ret;
}

#ifdef REASSOCIATION
/**
 * @brief Set/Get reassociation settings
 * 
 * @param priv     A pointer to moal_private structure
 * @param wrq      A pointer to iwreq structure
 * 
 * @return         0 --success, otherwise fail
 */
static int
woal_set_get_reassoc(moal_private * priv, struct iwreq *wrq)
{
    moal_handle *handle = priv->phandle;
    int ret = 0;
    int data = 0;

    ENTER();

    if (wrq->u.data.length) {
        if (copy_from_user(&data, wrq->u.data.pointer, sizeof(int))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        if (data == 0) {
            handle->reassoc_on = MFALSE;
            if (handle->is_reassoc_timer_set == MTRUE) {
                woal_cancel_timer(&handle->reassoc_timer);
                handle->is_reassoc_timer_set = MFALSE;
            }
        } else
            handle->reassoc_on = MTRUE;
    } else {
        data = (int) handle->reassoc_on;
        if (copy_to_user(wrq->u.data.pointer, &data, sizeof(int))) {
            ret = -EFAULT;
            goto done;
        }
        wrq->u.data.length = 1;
    }

  done:
    LEAVE();
    return ret;
}
#endif /* REASSOCIATION */

/**
 *  @brief implement WMM enable command
 *
 *  @param priv     Pointer to the moal_private driver data struct
 *  @param wrq      Pointer to user data
 *
 *  @return          0 --success, otherwise fail
 */
static int
woal_wmm_enable_ioctl(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ds_wmm_cfg *wmm = NULL;
    mlan_ioctl_req *req = NULL;
    int data = 0;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_wmm_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    wmm = (mlan_ds_wmm_cfg *) req->pbuf;
    req->req_id = MLAN_IOCTL_WMM_CFG;
    wmm->sub_command = MLAN_OID_WMM_CFG_ENABLE;

    if (wrq->u.data.length) {
        /* Set WMM configuration */
        if (copy_from_user(&data, wrq->u.data.pointer, sizeof(int))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        if ((data < CMD_DISABLED) || (data > CMD_ENABLED)) {
            ret = -EINVAL;
            goto done;
        }
        req->action = MLAN_ACT_SET;
        if (data == CMD_DISABLED)
            wmm->param.wmm_enable = MFALSE;
        else
            wmm->param.wmm_enable = MTRUE;
    } else {
        /* Get WMM status */
        req->action = MLAN_ACT_GET;
    }

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    if (wrq->u.data.pointer) {
        if (copy_to_user
            (wrq->u.data.pointer, &wmm->param.wmm_enable,
             sizeof(wmm->param.wmm_enable))) {
            PRINTM(MERROR, "Copy to user failed\n");
            ret = -EFAULT;
            goto done;
        }
        wrq->u.data.length = 1;
    }
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Implement 802.11D enable command
 *
 *  @param priv     Pointer to the moal_private driver data struct
 *  @param wrq      Pointer to user data
 *
 *  @return         0 --success, otherwise fail
 */
static int
woal_11d_enable_ioctl(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ds_11d_cfg *pcfg_11d = NULL;
    mlan_ioctl_req *req = NULL;
    int data = 0;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_11d_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    pcfg_11d = (mlan_ds_11d_cfg *) req->pbuf;
    req->req_id = MLAN_IOCTL_11D_CFG;
    pcfg_11d->sub_command = MLAN_OID_11D_CFG_ENABLE;
    if (wrq->u.data.length) {
        /* Set 11D configuration */
        if (copy_from_user(&data, wrq->u.data.pointer, sizeof(int))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        if ((data < CMD_DISABLED) || (data > CMD_ENABLED)) {
            ret = -EINVAL;
            goto done;
        }
        if (data == CMD_DISABLED)
            pcfg_11d->param.enable_11d = MFALSE;
        else
            pcfg_11d->param.enable_11d = MTRUE;
        req->action = MLAN_ACT_SET;
    } else {
        req->action = MLAN_ACT_GET;
    }

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    if (wrq->u.data.pointer) {
        if (copy_to_user
            (wrq->u.data.pointer, &pcfg_11d->param.enable_11d,
             sizeof(pcfg_11d->param.enable_11d))) {
            PRINTM(MERROR, "Copy to user failed\n");
            ret = -EFAULT;
            goto done;
        }
        wrq->u.data.length = 1;
    }
  done:
    if (req)
        kfree(req);

    LEAVE();
    return ret;
}

/**
 *  @brief Control WPS Session Enable/Disable
 *
 *  @param priv     Pointer to the moal_private driver data struct
 *  @param wrq      Pointer to user data
 *
 *  @return          0 --success, otherwise fail
 */
static int
woal_wps_cfg_ioctl(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ds_wps_cfg *pwps = NULL;
    mlan_ioctl_req *req = NULL;
    char buf[8];
    struct iwreq *wreq = (struct iwreq *) wrq;

    ENTER();

    PRINTM(MINFO, "WOAL_WPS_SESSION\n");

    memset(buf, 0, sizeof(buf));
    if (copy_from_user(buf, wreq->u.data.pointer,
                       MIN(sizeof(buf) - 1, wreq->u.data.length))) {
        PRINTM(MERROR, "Copy from user failed\n");
        ret = -EFAULT;
        goto done;
    }

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_wps_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    pwps = (mlan_ds_wps_cfg *) req->pbuf;
    req->req_id = MLAN_IOCTL_WPS_CFG;
    req->action = MLAN_ACT_SET;
    pwps->sub_command = MLAN_OID_WPS_CFG_SESSION;
    if (buf[0] == 1)
        pwps->param.wps_session = MLAN_WPS_CFG_SESSION_START;
    else
        pwps->param.wps_session = MLAN_WPS_CFG_SESSION_END;

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Set WPA passphrase and SSID
 *
 *  @param priv	    A pointer to moal_private structure
 *  @param wrq	    A pointer to user data
 *
 *  @return          0 --success, otherwise fail
 */
static int
woal_passphrase(moal_private * priv, struct iwreq *wrq)
{
    t_u16 len = 0;
    static char buf[256];
    char *begin, *end, *opt;
    int ret = 0, action = -1, i;
    mlan_ds_sec_cfg *sec = NULL;
    mlan_ioctl_req *req = NULL;
    t_u8 zero_mac[] = { 0, 0, 0, 0, 0, 0 };
    t_u8 *mac = NULL;

    ENTER();

    if (!wrq->u.data.length) {
        PRINTM(MERROR, "Argument missing for setpassphrase\n");
        ret = -EINVAL;
        goto done;
    }

    if (copy_from_user(buf, wrq->u.data.pointer, wrq->u.data.length)) {
        PRINTM(MERROR, "Copy from user failed\n");
        ret = -EFAULT;
        goto done;
    }
    buf[wrq->u.data.length] = '\0';

    if (wrq->u.data.length <= 1) {
        PRINTM(MERROR, "No valid arguments\n");
        ret = -EINVAL;
        goto done;
    }
    /* Parse the buf to get the cmd_action */
    begin = buf;
    end = woal_strsep(&begin, ';', '/');
    if (end)
        action = woal_atox(end);
    if (action < 0 || action > 2 || end[1] != '\0') {
        PRINTM(MERROR, "Invalid action argument %s\n", end);
        ret = -EINVAL;
        goto done;
    }
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_sec_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    sec = (mlan_ds_sec_cfg *) req->pbuf;
    sec->sub_command = MLAN_OID_SEC_CFG_PASSPHRASE;
    req->req_id = MLAN_IOCTL_SEC_CFG;
    if (action == 0)
        req->action = MLAN_ACT_GET;
    else
        req->action = MLAN_ACT_SET;
    while (begin) {
        end = woal_strsep(&begin, ';', '/');
        opt = woal_strsep(&end, '=', '/');
        if (!opt || !end || !end[0]) {
            PRINTM(MERROR, "Invalid option\n");
            ret = -EINVAL;
            break;
        } else if (!strnicmp(opt, "ssid", strlen(opt))) {
            if (strlen(end) > MLAN_MAX_SSID_LENGTH) {
                PRINTM(MERROR, "SSID length exceeds max length\n");
                ret = -EFAULT;
                break;
            }
            sec->param.passphrase.ssid.ssid_len = strlen(end);
            strcpy((char *) sec->param.passphrase.ssid.ssid, end);
            PRINTM(MINFO, "ssid=%s, len=%d\n", sec->param.passphrase.ssid.ssid,
                   (int) sec->param.passphrase.ssid.ssid_len);
        } else if (!strnicmp(opt, "bssid", strlen(opt))) {
            woal_mac2u8((t_u8 *) & sec->param.passphrase.bssid, end);
        } else if (!strnicmp(opt, "psk", strlen(opt)) &&
                   req->action == MLAN_ACT_SET) {
            if (strlen(end) != (MLAN_MAX_PMK_LENGTH * 2)) {
                PRINTM(MERROR, "Invalid PMK length\n");
                ret = -EINVAL;
                break;
            }
            woal_ascii2hex(sec->param.passphrase.psk.pmk.pmk, end,
                           MLAN_MAX_PMK_LENGTH * 2);
            sec->param.passphrase.psk_type = MLAN_PSK_PMK;
        } else if (!strnicmp(opt, "passphrase", strlen(opt)) &&
                   req->action == MLAN_ACT_SET) {
            if (strlen(end) < MLAN_MIN_PASSPHRASE_LENGTH ||
                strlen(end) > MLAN_MAX_PASSPHRASE_LENGTH) {
                PRINTM(MERROR, "Invalid length for passphrase\n");
                ret = -EINVAL;
                break;
            }
            sec->param.passphrase.psk_type = MLAN_PSK_PASSPHRASE;
            strcpy(sec->param.passphrase.psk.passphrase.passphrase, end);
            sec->param.passphrase.psk.passphrase.passphrase_len = strlen(end);
            PRINTM(MINFO, "passphrase=%s, len=%d\n",
                   sec->param.passphrase.psk.passphrase.passphrase,
                   (int) sec->param.passphrase.psk.passphrase.passphrase_len);
        } else {
            PRINTM(MERROR, "Invalid option %s\n", opt);
            ret = -EINVAL;
            break;
        }
    }
    if (ret)
        goto done;

    if (action == 2)
        sec->param.passphrase.psk_type = MLAN_PSK_CLEAR;
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    if (action == 0) {
        memset(buf, 0, sizeof(buf));
        if (sec->param.passphrase.ssid.ssid_len) {
            len += sprintf(buf + len, "ssid:");
            memcpy(buf + len, sec->param.passphrase.ssid.ssid,
                   sec->param.passphrase.ssid.ssid_len);
            len += sec->param.passphrase.ssid.ssid_len;
            len += sprintf(buf + len, " ");
        }
        if (memcmp(&sec->param.passphrase.bssid, zero_mac, sizeof(zero_mac))) {
            mac = (t_u8 *) & sec->param.passphrase.bssid;
            len += sprintf(buf + len, "bssid:");
            for (i = 0; i < ETH_ALEN - 1; ++i)
                len += sprintf(buf + len, "%02x:", mac[i]);
            len += sprintf(buf + len, "%02x ", mac[i]);
        }
        if (sec->param.passphrase.psk_type == MLAN_PSK_PMK) {
            len += sprintf(buf + len, "psk:");
            for (i = 0; i < MLAN_MAX_PMK_LENGTH; ++i)
                len +=
                    sprintf(buf + len, "%02x",
                            sec->param.passphrase.psk.pmk.pmk[i]);
            len += sprintf(buf + len, "\n");
        }
        if (sec->param.passphrase.psk_type == MLAN_PSK_PASSPHRASE) {
            len +=
                sprintf(buf + len, "passphrase:%s \n",
                        sec->param.passphrase.psk.passphrase.passphrase);
        }
        if (wrq->u.data.pointer) {
            if (copy_to_user(wrq->u.data.pointer, buf, len)) {
                PRINTM(MERROR, "Copy to user failed, len %d\n", len);
                ret = -EFAULT;
                goto done;
            }
            wrq->u.data.length = len;
        }

    }
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Get esupp mode
 *  
 *  @param priv     A pointer to moal_private structure
 *  @param wrq      A pointer to iwreq structure
 *  
 *  @return         0 --success, otherwise fail
 */
static int
woal_get_esupp_mode(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ds_sec_cfg *sec = NULL;
    mlan_ioctl_req *req = NULL;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_sec_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    sec = (mlan_ds_sec_cfg *) req->pbuf;
    sec->sub_command = MLAN_OID_SEC_CFG_ESUPP_MODE;
    req->req_id = MLAN_IOCTL_SEC_CFG;
    req->action = MLAN_ACT_GET;

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    if (copy_to_user
        (wrq->u.data.pointer, (t_u8 *) & sec->param.esupp_mode,
         sizeof(int) * 3)) {
        ret = -EFAULT;
        goto done;
    }
    wrq->u.data.length = 3;
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/** AES key length */
#define AES_KEY_LEN 16
/**
 *  @brief Adhoc AES control 
 *
 *  @param priv	    A pointer to moal_private structure
 *  @param wrq	    A pointer to user data
 *
 *  @return 	    0 --success, otherwise fail
 */
static int
woal_adhoc_aes_ioctl(moal_private * priv, struct iwreq *wrq)
{
    static char buf[256];
    int ret = 0, action = -1;
    unsigned int i;
    t_u8 key_ascii[32];
    t_u8 key_hex[16];
    t_u8 *tmp;
    mlan_bss_info bss_info;
    mlan_ds_sec_cfg *sec = NULL;
    mlan_ioctl_req *req = NULL;

    ENTER();

    memset(key_ascii, 0x00, sizeof(key_ascii));
    memset(key_hex, 0x00, sizeof(key_hex));

    /* Get current BSS information */
    memset(&bss_info, 0, sizeof(bss_info));
    woal_get_bss_info(priv, MOAL_IOCTL_WAIT, &bss_info);
    if (bss_info.bss_mode != MLAN_BSS_MODE_IBSS ||
        bss_info.media_connected == MTRUE) {
        PRINTM(MERROR, "STA is connected or not in IBSS mode.\n");
        ret = -EOPNOTSUPP;
        goto done;
    }

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_sec_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    if (wrq->u.data.length) {
        if (copy_from_user(buf, wrq->u.data.pointer, wrq->u.data.length)) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        buf[wrq->u.data.length] = '\0';

        if (wrq->u.data.length == 1) {
            /* Get Adhoc AES Key */
            req->req_id = MLAN_IOCTL_SEC_CFG;
            req->action = MLAN_ACT_GET;
            sec = (mlan_ds_sec_cfg *) req->pbuf;
            sec->sub_command = MLAN_OID_SEC_CFG_ENCRYPT_KEY;
            sec->param.encrypt_key.key_len = AES_KEY_LEN;
            sec->param.encrypt_key.key_index = MLAN_KEY_INDEX_UNICAST;
            if (MLAN_STATUS_SUCCESS !=
                woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
                ret = -EFAULT;
                goto done;
            }

            memcpy(key_hex, sec->param.encrypt_key.key_material,
                   sizeof(key_hex));
            HEXDUMP("Adhoc AES Key (HEX)", key_hex, sizeof(key_hex));

            wrq->u.data.length = sizeof(key_ascii) + 1;

            tmp = key_ascii;
            for (i = 0; i < sizeof(key_hex); i++)
                tmp += sprintf((char *) tmp, "%02x", key_hex[i]);
        } else if (wrq->u.data.length >= 2) {
            /* Parse the buf to get the cmd_action */
            action = woal_atox(&buf[0]);
            if (action < 1 || action > 2) {
                PRINTM(MERROR, "Invalid action argument %d\n", action);
                ret = -EINVAL;
                goto done;
            }

            req->req_id = MLAN_IOCTL_SEC_CFG;
            req->action = MLAN_ACT_SET;
            sec = (mlan_ds_sec_cfg *) req->pbuf;
            sec->sub_command = MLAN_OID_SEC_CFG_ENCRYPT_KEY;

            if (action == 1) {
                /* Set Adhoc AES Key */
                memcpy(key_ascii, &buf[2], sizeof(key_ascii));
                woal_ascii2hex(key_hex, (char *) key_ascii, sizeof(key_hex));
                HEXDUMP("Adhoc AES Key (HEX)", key_hex, sizeof(key_hex));

                sec->param.encrypt_key.key_len = AES_KEY_LEN;
                sec->param.encrypt_key.key_index = MLAN_KEY_INDEX_UNICAST;
                memcpy(sec->param.encrypt_key.key_material,
                       key_hex, sec->param.encrypt_key.key_len);

                if (MLAN_STATUS_SUCCESS !=
                    woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
                    ret = -EFAULT;
                    goto done;
                }
            } else if (action == 2) {
                /* Clear Adhoc AES Key */
                sec->param.encrypt_key.key_len = AES_KEY_LEN;
                sec->param.encrypt_key.key_index = MLAN_KEY_INDEX_UNICAST;
                memset(sec->param.encrypt_key.key_material, 0,
                       sizeof(sec->param.encrypt_key.key_material));

                if (MLAN_STATUS_SUCCESS !=
                    woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
                    ret = -EFAULT;
                    goto done;
                }
            } else {
                PRINTM(MERROR, "Invalid argument\n");
                ret = -EINVAL;
                goto done;
            }
        }

        HEXDUMP("Adhoc AES Key (ASCII)", key_ascii, sizeof(key_ascii));
        wrq->u.data.length = sizeof(key_ascii);
        if (wrq->u.data.pointer) {
            if (copy_to_user(wrq->u.data.pointer, &key_ascii,
                             sizeof(key_ascii))) {
                PRINTM(MERROR, "copy_to_user failed\n");
                ret = -EFAULT;
                goto done;
            }
        }
    }

  done:
    if (req)
        kfree(req);

    LEAVE();
    return ret;
}

#ifdef MFG_CMD_SUPPORT
/** 
 *  @brief Manufacturing command ioctl function
 *   
 *  @param priv		A pointer to moal_private structure
 *  @param wrq 		A pointer to iwreq structure
 *  @return    		0 --success, otherwise fail
 */
static int
woal_mfg_command(moal_private * priv, struct iwreq *wrq)
{
    HostCmd_Header cmd_header;
    int ret = 0;
    mlan_ds_misc_cfg *misc = NULL;
    mlan_ioctl_req *req = NULL;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_misc_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    misc = (mlan_ds_misc_cfg *) req->pbuf;
    memset(&cmd_header, 0, sizeof(cmd_header));

    /* get MFG command header */
    if (copy_from_user
        (&cmd_header, wrq->u.data.pointer, sizeof(HostCmd_Header))) {
        PRINTM(MERROR, "copy from user failed: MFG command header\n");
        ret = -EFAULT;
        goto done;
    }
    misc->param.mfgcmd.len = cmd_header.size;

    PRINTM(MINFO, "MFG command len = %lu\n", misc->param.mfgcmd.len);

    if (cmd_header.size > MLAN_SIZE_OF_CMD_BUFFER) {
        ret = -EINVAL;
        goto done;
    }

    /* get the whole command from user */
    if (copy_from_user
        (misc->param.mfgcmd.cmd, wrq->u.data.pointer, cmd_header.size)) {
        PRINTM(MERROR, "copy from user failed: MFG command\n");
        ret = -EFAULT;
        goto done;
    }
    misc->sub_command = MLAN_OID_MISC_MFG_CMD;
    req->req_id = MLAN_IOCTL_MISC_CFG;
    req->action = MLAN_ACT_SET;

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    if (copy_to_user
        (wrq->u.data.pointer, (t_u8 *) misc->param.mfgcmd.cmd,
         MIN(cmd_header.size, misc->param.mfgcmd.len))) {
        ret = -EFAULT;
        goto done;
    }
    wrq->u.data.length = MIN(cmd_header.size, misc->param.mfgcmd.len);
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}
#endif /* MFG_CMD_SUPPORT */

/** 
 *  @brief host command ioctl function
 *   
 *  @param priv		A pointer to moal_private structure
 *  @param wrq 		A pointer to iwreq structure
 *  @return    		0 --success, otherwise fail
 */
static int
woal_host_command(moal_private * priv, struct iwreq *wrq)
{
    HostCmd_Header cmd_header;
    int ret = 0;
    mlan_ds_misc_cfg *misc = NULL;
    mlan_ioctl_req *req = NULL;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_misc_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    misc = (mlan_ds_misc_cfg *) req->pbuf;
    memset(&cmd_header, 0, sizeof(cmd_header));

    /* get command header */
    if (copy_from_user
        (&cmd_header, wrq->u.data.pointer, sizeof(HostCmd_Header))) {
        PRINTM(MERROR, "copy from user failed: Host command header\n");
        ret = -EFAULT;
        goto done;
    }
    misc->param.hostcmd.len = woal_le16_to_cpu(cmd_header.size);

    PRINTM(MINFO, "Host command len = %lu\n", misc->param.hostcmd.len);

    if (woal_le16_to_cpu(cmd_header.size) > MLAN_SIZE_OF_CMD_BUFFER) {
        ret = -EINVAL;
        goto done;
    }

    /* get the whole command from user */
    if (copy_from_user
        (misc->param.hostcmd.cmd, wrq->u.data.pointer,
         woal_le16_to_cpu(cmd_header.size))) {
        PRINTM(MERROR, "copy from user failed\n");
        ret = -EFAULT;
        goto done;
    }
    misc->sub_command = MLAN_OID_MISC_HOST_CMD;
    req->req_id = MLAN_IOCTL_MISC_CFG;

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    if (copy_to_user
        (wrq->u.data.pointer, (t_u8 *) misc->param.hostcmd.cmd,
         misc->param.hostcmd.len)) {
        ret = -EFAULT;
        goto done;
    }
    wrq->u.data.length = misc->param.hostcmd.len;
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/** 
 *  @brief arpfilter ioctl function
 *   
 *  @param priv		A pointer to moal_private structure
 *  @param wrq 		A pointer to iwreq structure
 *  @return    		0 --success, otherwise fail
 */
static int
woal_arp_filter(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ds_misc_cfg *misc = NULL;
    mlan_ioctl_req *req = NULL;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_misc_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    misc = (mlan_ds_misc_cfg *) req->pbuf;
    misc->sub_command = MLAN_OID_MISC_GEN_IE;
    req->req_id = MLAN_IOCTL_MISC_CFG;
    req->action = MLAN_ACT_SET;
    misc->param.gen_ie.type = MLAN_IE_TYPE_ARP_FILTER;
    misc->param.gen_ie.len = wrq->u.data.length;

    /* get the whole command from user */
    if (copy_from_user
        (misc->param.gen_ie.ie_data, wrq->u.data.pointer, wrq->u.data.length)) {
        PRINTM(MERROR, "copy from user failed\n");
        ret = -EFAULT;
        goto done;
    }

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Create a brief scan resp to relay basic BSS info to the app layer
 *
 *  When the beacon/probe response has not been buffered, use the saved BSS
 *    information available to provide a minimum response for the application
 *    ioctl retrieval routines.  Include:
 *        - Timestamp
 *        - Beacon Period
 *        - Capabilities (including WMM Element if available)
 *        - SSID
 *
 *  @param ppbuffer  Output parameter: Buffer used to create basic scan rsp
 *  @param pbss_desc Pointer to a BSS entry in the scan table to create
 *                   scan response from for delivery to the application layer
 *
 *  @return          void
 */
static void
wlan_scan_create_brief_table_entry(t_u8 ** ppbuffer,
                                   BSSDescriptor_t * pbss_desc)
{
    t_u8 *ptmp_buf = *ppbuffer;
    t_u8 tmp_ssid_hdr[2];
    t_u8 ie_len = 0;

    if (copy_to_user(ptmp_buf, pbss_desc->time_stamp,
                     sizeof(pbss_desc->time_stamp))) {
        PRINTM(MINFO, "Copy to user failed\n");
        return;
    }
    ptmp_buf += sizeof(pbss_desc->time_stamp);

    if (copy_to_user(ptmp_buf, &pbss_desc->beacon_period,
                     sizeof(pbss_desc->beacon_period))) {
        PRINTM(MINFO, "Copy to user failed\n");
        return;
    }
    ptmp_buf += sizeof(pbss_desc->beacon_period);

    if (copy_to_user
        (ptmp_buf, &pbss_desc->cap_info, sizeof(pbss_desc->cap_info))) {
        PRINTM(MINFO, "Copy to user failed\n");
        return;
    }
    ptmp_buf += sizeof(pbss_desc->cap_info);

    tmp_ssid_hdr[0] = 0;        /* Element ID for SSID is zero */
    tmp_ssid_hdr[1] = pbss_desc->ssid.ssid_len;
    if (copy_to_user(ptmp_buf, tmp_ssid_hdr, sizeof(tmp_ssid_hdr))) {
        PRINTM(MINFO, "Copy to user failed\n");
        return;
    }
    ptmp_buf += sizeof(tmp_ssid_hdr);

    if (copy_to_user(ptmp_buf, pbss_desc->ssid.ssid, pbss_desc->ssid.ssid_len)) {
        PRINTM(MINFO, "Copy to user failed\n");
        return;
    }
    ptmp_buf += pbss_desc->ssid.ssid_len;

    if (pbss_desc->wmm_ie.vend_hdr.element_id == WMM_IE) {
        ie_len = sizeof(IEEEtypes_Header_t) + pbss_desc->wmm_ie.vend_hdr.len;
        if (copy_to_user(ptmp_buf, &pbss_desc->wmm_ie, ie_len)) {
            PRINTM(MINFO, "Copy to user failed\n");
            return;
        }

        ptmp_buf += ie_len;
    }

    if (pbss_desc->pwpa_ie) {
        if ((*(pbss_desc->pwpa_ie)).vend_hdr.element_id == WPA_IE) {
            ie_len =
                sizeof(IEEEtypes_Header_t) +
                (*(pbss_desc->pwpa_ie)).vend_hdr.len;
            if (copy_to_user(ptmp_buf, pbss_desc->pwpa_ie, ie_len)) {
                PRINTM(MINFO, "Copy to user failed\n");
                return;
            }
        }

        ptmp_buf += ie_len;
    }

    if (pbss_desc->prsn_ie) {
        if ((*(pbss_desc->prsn_ie)).ieee_hdr.element_id == RSN_IE) {
            ie_len =
                sizeof(IEEEtypes_Header_t) +
                (*(pbss_desc->prsn_ie)).ieee_hdr.len;
            if (copy_to_user(ptmp_buf, pbss_desc->prsn_ie, ie_len)) {
                PRINTM(MINFO, "Copy to user failed\n");
                return;
            }
        }

        ptmp_buf += ie_len;
    }

    *ppbuffer = ptmp_buf;
}

/** 
 *  @brief Create a wlan_ioctl_get_scan_table_entry for a given BSS 
 *         Descriptor for inclusion in the ioctl response to the user space
 *         application.
 *
 *
 *  @param pbss_desc   Pointer to a BSS entry in the scan table to form
 *                     scan response from for delivery to the application layer
 *  @param ppbuffer    Output parameter: Buffer used to output scan return struct
 *  @param pspace_left Output parameter: Number of bytes available in the 
 *                     response buffer.
 *
 *  @return MLAN_STATUS_SUCCESS, or < 0 with IOCTL error code
 */
static int
wlan_get_scan_table_ret_entry(BSSDescriptor_t * pbss_desc,
                              t_u8 ** ppbuffer, int *pspace_left)
{
    wlan_ioctl_get_scan_table_entry *prsp_entry;
    wlan_ioctl_get_scan_table_entry tmp_rsp_entry;
    int space_needed;
    t_u8 *pcurrent;
    int variable_size;

    const int fixed_size = sizeof(wlan_ioctl_get_scan_table_entry);

    ENTER();

    pcurrent = *ppbuffer;

    /* The variable size returned is the stored beacon size */
    variable_size = pbss_desc->beacon_buf_size;

    /* If we stored a beacon and its size was zero, set the variable size
       return value to the size of the brief scan response
       wlan_scan_create_brief_table_entry creates.  Also used if we are not
       configured to store beacons in the first place */
    if (!variable_size) {
        variable_size = pbss_desc->ssid.ssid_len + 2;
        variable_size += (sizeof(pbss_desc->beacon_period)
                          + sizeof(pbss_desc->time_stamp)
                          + sizeof(pbss_desc->cap_info));
        if (pbss_desc->wmm_ie.vend_hdr.element_id == WMM_IE) {
            variable_size += (sizeof(IEEEtypes_Header_t)
                              + pbss_desc->wmm_ie.vend_hdr.len);
        }

        if (pbss_desc->pwpa_ie) {
            if ((*(pbss_desc->pwpa_ie)).vend_hdr.element_id == WPA_IE) {
                variable_size += (sizeof(IEEEtypes_Header_t)
                                  + (*(pbss_desc->pwpa_ie)).vend_hdr.len);
            }
        }

        if (pbss_desc->prsn_ie) {
            if ((*(pbss_desc->prsn_ie)).ieee_hdr.element_id == RSN_IE) {
                variable_size += (sizeof(IEEEtypes_Header_t)
                                  + (*(pbss_desc->prsn_ie)).ieee_hdr.len);
            }
        }
    }

    space_needed = fixed_size + variable_size;

    PRINTM(MINFO, "GetScanTable: need(%d), left(%d)\n",
           space_needed, *pspace_left);

    if (space_needed >= *pspace_left) {
        *pspace_left = 0;
        LEAVE();
        return -E2BIG;
    }

    *pspace_left -= space_needed;

    tmp_rsp_entry.fixed_field_length = (sizeof(tmp_rsp_entry)
                                        -
                                        sizeof(tmp_rsp_entry.fixed_field_length)
                                        -
                                        sizeof(tmp_rsp_entry.bss_info_length));

    memcpy(tmp_rsp_entry.fixed_fields.bssid,
           pbss_desc->mac_address, sizeof(prsp_entry->fixed_fields.bssid));

    tmp_rsp_entry.fixed_fields.rssi = pbss_desc->rssi;
    tmp_rsp_entry.fixed_fields.channel = pbss_desc->channel;
    tmp_rsp_entry.fixed_fields.network_tsf = pbss_desc->network_tsf;
    tmp_rsp_entry.bss_info_length = variable_size;

    /* 
     *  Copy fixed fields to user space
     */
    if (copy_to_user(pcurrent, &tmp_rsp_entry, fixed_size)) {
        PRINTM(MINFO, "Copy to user failed\n");
        LEAVE();
        return -EFAULT;
    }

    pcurrent += fixed_size;

    if (pbss_desc->pbeacon_buf) {
        /* 
         *  Copy variable length elements to user space
         */
        if (copy_to_user(pcurrent, pbss_desc->pbeacon_buf,
                         pbss_desc->beacon_buf_size)) {
            PRINTM(MINFO, "Copy to user failed\n");
            LEAVE();
            return -EFAULT;
        }

        pcurrent += pbss_desc->beacon_buf_size;
    } else {
        wlan_scan_create_brief_table_entry(&pcurrent, pbss_desc);
    }

    *ppbuffer = pcurrent;

    LEAVE();

    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief Retrieve the scan response/beacon table
 *
 *  @param wrq          A pointer to iwreq structure
 *  @param scan_resp    A pointer to mlan_scan_resp structure
 *  @param scan_start   argument
 *
 *  @return             MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static int
moal_ret_get_scan_table_ioctl(struct iwreq *wrq,
                              mlan_scan_resp * scan_resp, t_u32 scan_start)
{
    pBSSDescriptor_t pbss_desc, scan_table;
    mlan_scan_resp *prsp_info;
    int ret_code;
    int ret_len;
    int space_left;
    t_u8 *pcurrent;
    t_u8 *pbuffer_end;
    t_u32 num_scans_done;

    ENTER();

    num_scans_done = 0;
    ret_code = MLAN_STATUS_SUCCESS;

    prsp_info = (mlan_scan_resp *) wrq->u.data.pointer;
    prsp_info->pscan_table =
        (t_u8 *) prsp_info + sizeof(prsp_info->num_in_scan_table);
    pcurrent = prsp_info->pscan_table;

    pbuffer_end = wrq->u.data.pointer + wrq->u.data.length - 1;
    space_left = pbuffer_end - pcurrent;
    scan_table = (BSSDescriptor_t *) (scan_resp->pscan_table);

    PRINTM(MINFO, "GetScanTable: scan_start req = %ld\n", scan_start);
    PRINTM(MINFO, "GetScanTable: length avail = %d\n", wrq->u.data.length);

    if (!scan_start) {
        PRINTM(MINFO, "GetScanTable: get current BSS Descriptor\n");

        /* Use to get current association saved descriptor */
        pbss_desc = scan_table;

        ret_code = wlan_get_scan_table_ret_entry(pbss_desc,
                                                 &pcurrent, &space_left);

        if (ret_code == MLAN_STATUS_SUCCESS) {
            num_scans_done = 1;
        }
    } else {
        scan_start--;

        while (space_left
               && (scan_start + num_scans_done < scan_resp->num_in_scan_table)
               && (ret_code == MLAN_STATUS_SUCCESS)) {

            pbss_desc = (scan_table + (scan_start + num_scans_done));

            PRINTM(MINFO, "GetScanTable: get current BSS Descriptor [%ld]\n",
                   scan_start + num_scans_done);

            ret_code = wlan_get_scan_table_ret_entry(pbss_desc,
                                                     &pcurrent, &space_left);

            if (ret_code == MLAN_STATUS_SUCCESS) {
                num_scans_done++;
            }
        }
    }

    prsp_info->num_in_scan_table = num_scans_done;
    ret_len = pcurrent - (t_u8 *) wrq->u.data.pointer;

    wrq->u.data.length = ret_len;

    /* Return ret_code (EFAULT or E2BIG) in the case where no scan results were 
       successfully encoded. */
    LEAVE();
    return (num_scans_done ? MLAN_STATUS_SUCCESS : ret_code);
}

/** 
 *  @brief Get scan table ioctl
 *
 *  @param priv     A pointer to moal_private structure
 *  @param wrq 		A pointer to iwreq structure
 *
 *  @return         MLAN_STATUS_SUCCESS -- success, otherwise fail          
 */
static mlan_status
woal_get_scan_table_ioctl(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_scan *scan = NULL;
    t_u32 scan_start;
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
    req->req_id = MLAN_IOCTL_SCAN;
    req->action = MLAN_ACT_GET;

    /* get the whole command from user */
    if (copy_from_user(&scan_start, wrq->u.data.pointer, sizeof(scan_start))) {
        PRINTM(MERROR, "copy from user failed\n");
        ret = -EFAULT;
        goto done;
    }
    if (scan_start) {
        scan->sub_command = MLAN_OID_SCAN_NORMAL;
    } else {
        scan->sub_command = MLAN_OID_SCAN_SPECIFIC_SSID;
    }
    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT);
    if (status == MLAN_STATUS_SUCCESS) {
        status = moal_ret_get_scan_table_ioctl(wrq,
                                               &scan->param.scan_resp,
                                               scan_start);
    }
  done:
    if (req && (status != MLAN_STATUS_PENDING))
        kfree(req);
    LEAVE();
    return status;
}

/** 
 *  @brief Set user scan
 *
 *  @param priv     A pointer to moal_private structure
 *  @param wrq 		A pointer to iwreq structure
 *
 *  @return         MLAN_STATUS_SUCCESS -- success, otherwise fail          
 */
static mlan_status
woal_set_user_scan_ioctl(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    mlan_ioctl_req *req = NULL;
    mlan_ds_scan *scan = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;
    union iwreq_data wrqu;

    ENTER();

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_scan) + wrq->u.data.length);
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    scan = (mlan_ds_scan *) req->pbuf;
    scan->sub_command = MLAN_OID_SCAN_USER_CONFIG;
    req->req_id = MLAN_IOCTL_SCAN;
    req->action = MLAN_ACT_SET;

    if (copy_from_user(scan->param.user_scan.scan_cfg_buf,
                       wrq->u.data.pointer, wrq->u.data.length)) {
        PRINTM(MINFO, "Copy from user failed\n");
        LEAVE();
        return -EFAULT;
    }

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT);
    if (status == MLAN_STATUS_SUCCESS) {
        memset(&wrqu, 0, sizeof(union iwreq_data));
        wireless_send_event(priv->netdev, SIOCGIWSCAN, &wrqu, NULL);
    }

  done:
    if (req && (status != MLAN_STATUS_PENDING))
        kfree(req);
    LEAVE();
    return status;
}

/** 
 *  @brief Cmd52 read/write register
 *   
 *  @param priv         A pointer to moal_private structure
 *  @param wrq          A pointer to iwreq structure
 *  @return             MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static int
woal_cmd52rdwr_ioctl(moal_private * priv, struct iwreq *wrq)
{
    t_u8 rw, func, data = 0;
    int buf[3], reg, ret = MLAN_STATUS_SUCCESS;
    int data_length = wrq->u.data.length;

    ENTER();

    if (data_length < 2 || data_length > 3) {
        PRINTM(MERROR, "Invalid number of arguments\n");
        ret = -EINVAL;
        goto done;
    }

    if (copy_from_user(buf, wrq->u.data.pointer, sizeof(int) * data_length)) {
        PRINTM(MERROR, "Copy from user failed\n");
        ret = -EFAULT;
        goto done;
    }

    func = (t_u8) buf[0];
    if (func > 7) {
        PRINTM(MERROR, "Invalid function number!\n");
        ret = -EINVAL;
        goto done;
    }
    reg = (t_u32) buf[1];
    if (data_length == 2) {
        rw = 0;                 /* CMD52 read */
        PRINTM(MINFO, "Cmd52 read, func=%d, reg=0x%08X\n", func, reg);
    }
    if (data_length == 3) {
        rw = 1;                 /* CMD52 write */
        data = (t_u8) buf[2];
        PRINTM(MINFO, "Cmd52 write, func=%d, reg=0x%08X, data=0x%02X\n", func,
               reg, data);
    }

    if (!rw) {
        sdio_claim_host(((struct sdio_mmc_card *) priv->phandle->card)->func);
        if (func)
            data =
                sdio_readb(((struct sdio_mmc_card *) priv->phandle->card)->func,
                           reg, &ret);
        else
            data =
                sdio_f0_readb(((struct sdio_mmc_card *) priv->phandle->card)->
                              func, reg, &ret);
        sdio_release_host(((struct sdio_mmc_card *) priv->phandle->card)->func);
        if (ret) {
            PRINTM(MERROR, "sdio_readb: reading register 0x%X failed\n", reg);
            goto done;
        }
    } else {
        sdio_claim_host(((struct sdio_mmc_card *) priv->phandle->card)->func);
        if (func)
            sdio_writeb(((struct sdio_mmc_card *) priv->phandle->card)->func,
                        data, reg, &ret);
        else
            sdio_f0_writeb(((struct sdio_mmc_card *) priv->phandle->card)->func,
                           data, reg, &ret);
        sdio_release_host(((struct sdio_mmc_card *) priv->phandle->card)->func);
        if (ret) {
            PRINTM(MERROR, "sdio_writeb: writing register 0x%X failed\n", reg);
            goto done;
        }
    }

    buf[0] = data;
    wrq->u.data.length = 1;
    if (copy_to_user(wrq->u.data.pointer, buf, sizeof(int))) {
        PRINTM(MERROR, "Copy to user failed\n");
        ret = -EFAULT;
        goto done;
    }

  done:
    LEAVE();
    return ret;
}

/** 
 *  @brief Cmd53 read/write register
 *     
 *  @param priv         A pointer to moal_private structure
 *  @param wrq          A pointer to iwreq structure
 *  @return             MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static int
woal_cmd53rdwr_ioctl(moal_private * priv, struct iwreq *wrq)
{
    t_u8 *buf = NULL;
    t_u8 rw, func, mode;
    t_u16 blklen = 0, blknum = 0;
    int reg = 0, pattern_len = 0, total_len = 0, pos = 0, ret =
        MLAN_STATUS_SUCCESS;
    t_u8 *data = NULL;

    ENTER();

    if (!(buf = (t_u8 *) kmalloc(WOAL_2K_BYTES, GFP_ATOMIC))) {
        PRINTM(MERROR, "Cannot allocate buffer for command!\n");
        ret = -EFAULT;
        goto done;
    }
    if (!(data = (t_u8 *) kmalloc(WOAL_2K_BYTES, GFP_ATOMIC))) {
        PRINTM(MERROR, "Cannot allocate buffer for command!\n");
        ret = -EFAULT;
        goto done;
    }
    if (wrq->u.data.length > WOAL_2K_BYTES) {
        PRINTM(MERROR, "Data lengh is too large!\n");
        ret = -EINVAL;
        goto done;
    }
    if (copy_from_user(buf, wrq->u.data.pointer, wrq->u.data.length)) {
        PRINTM(MINFO, "Copy from user failed\n");
        ret = -EFAULT;
        goto done;
    }

    rw = buf[0];                /* read/write (0/1) */
    func = buf[1];              /* func (0/1/2) */
    reg = buf[5];               /* address */
    reg = (reg << 8) + buf[4];
    reg = (reg << 8) + buf[3];
    reg = (reg << 8) + buf[2];
    mode = buf[6];              /* byte mode/block mode (0/1) */
    blklen = buf[8];            /* block size */
    blklen = (blklen << 8) + buf[7];
    blknum = buf[10];           /* block number or byte number */
    blknum = (blknum << 8) + buf[9];

    if (mode != BYTE_MODE)
        mode = BLOCK_MODE;
    total_len = (mode == BLOCK_MODE) ? blknum * blklen : blknum;
    if (total_len > WOAL_2K_BYTES) {
        PRINTM(MERROR, "Total data length is too large!\n");
        ret = -EINVAL;
        goto done;
    }
    PRINTM(MINFO, "CMD53 read/write, func = %d, addr = %#x, mode = %d, "
           "block size = %d, block(byte) number = %d\n",
           func, reg, mode, blklen, blknum);

    if (!rw) {
        sdio_claim_host(((struct sdio_mmc_card *) priv->phandle->card)->func);
        if (sdio_readsb
            (((struct sdio_mmc_card *) priv->phandle->card)->func, data, reg,
             total_len))
            PRINTM(MERROR, "sdio_readsb: reading memory 0x%x failed\n", reg);
        sdio_release_host(((struct sdio_mmc_card *) priv->phandle->card)->func);

        if (copy_to_user(wrq->u.data.pointer, data, sizeof(data))) {
            PRINTM(MINFO, "Copy to user failed\n");
            ret = -EFAULT;
            goto done;
        }
    } else {
        pattern_len = wrq->u.data.length - 11;
        if (pattern_len > total_len)
            pattern_len = total_len;
        memset(data, 0, sizeof(data));

        /* Copy/duplicate the pattern to data buffer */
        for (pos = 0; pos < total_len; pos++)
            data[pos] = buf[11 + (pos % pattern_len)];

        sdio_claim_host(((struct sdio_mmc_card *) priv->phandle->card)->func);
        if (sdio_writesb
            (((struct sdio_mmc_card *) priv->phandle->card)->func, reg, data,
             total_len))
            PRINTM(MERROR, "sdio_writesb: writing memory 0x%x failed\n", reg);
        sdio_release_host(((struct sdio_mmc_card *) priv->phandle->card)->func);
    }

  done:
    if (buf)
        kfree(buf);
    if (data)
        kfree(data);
    LEAVE();
    return ret;
}

#if defined(SDIO_MULTI_PORT_TX_AGGR) || defined(SDIO_MULTI_PORT_RX_AGGR)
/**
 * @brief Set SDIO Multi-point aggregation control parameters
 * 
 * @param priv     A pointer to moal_private structure
 * @param wrq      A pointer to iwreq structure
 * 
 * @return         0 --success, otherwise fail
 */
static int
woal_do_sdio_mpa_ctrl(moal_private * priv, struct iwreq *wrq)
{
    int data[6];
    int ret = 0;
    mlan_ds_misc_cfg *misc = NULL;
    mlan_ioctl_req *req = NULL;

    ENTER();

    if (wrq->u.data.length > 6) {
        PRINTM(MERROR, "Invalid number of arguments\n");
        ret = -EINVAL;
        goto done;
    }

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_misc_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    misc = (mlan_ds_misc_cfg *) req->pbuf;
    memset(misc, 0, sizeof(mlan_ds_misc_cfg));

    misc->sub_command = MLAN_OID_MISC_SDIO_MPA_CTRL;
    req->req_id = MLAN_IOCTL_MISC_CFG;

    /* Get the values first, then modify these values if user had modified them 
     */

    req->action = MLAN_ACT_GET;
    if ((ret = woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) !=
        MLAN_STATUS_SUCCESS) {
        PRINTM(MERROR, "woal_request_ioctl returned %d\n", ret);
        ret = -EFAULT;
        goto done;
    }

    if (wrq->u.data.length == 0) {
        data[0] = misc->param.mpa_ctrl.tx_enable;
        data[1] = misc->param.mpa_ctrl.rx_enable;
        data[2] = misc->param.mpa_ctrl.tx_buf_size;
        data[3] = misc->param.mpa_ctrl.rx_buf_size;
        data[4] = misc->param.mpa_ctrl.tx_max_ports;
        data[5] = misc->param.mpa_ctrl.rx_max_ports;

        PRINTM(MINFO, "Get Param: %d %d %d %d %d %d\n", data[0], data[1],
               data[2], data[3], data[4], data[5]);

        if (copy_to_user(wrq->u.data.pointer, data, sizeof(data))) {
            ret = -EFAULT;
            goto done;
        }
        wrq->u.data.length = sizeof(data) / sizeof(int);
        goto done;
    }

    if (copy_from_user(data, wrq->u.data.pointer,
                       sizeof(int) * wrq->u.data.length)) {
        PRINTM(MINFO, "Copy from user failed\n");
        ret = -EFAULT;
        goto done;
    }

    switch (wrq->u.data.length) {
    case 6:
        misc->param.mpa_ctrl.rx_max_ports = data[5];
    case 5:
        misc->param.mpa_ctrl.tx_max_ports = data[4];
    case 4:
        misc->param.mpa_ctrl.rx_buf_size = data[3];
    case 3:
        misc->param.mpa_ctrl.tx_buf_size = data[2];
    case 2:
        misc->param.mpa_ctrl.rx_enable = data[1];
    case 1:
        /* Set cmd */
        req->action = MLAN_ACT_SET;

        PRINTM(MINFO, "Set Param: %d %d %d %d %d %d\n", data[0], data[1],
               data[2], data[3], data[4], data[5]);

        misc->param.mpa_ctrl.tx_enable = data[0];
        break;
    default:
        PRINTM(MERROR, "Default case error\n");
        ret = -EINVAL;
        goto done;
    }

    if ((ret = woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT) !=
         MLAN_STATUS_SUCCESS)) {
        PRINTM(MERROR, "woal_request_ioctl returned %d\n", ret);
        ret = -EFAULT;
        goto done;
    }

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}
#endif /* SDIO_MULTI_PORT_TX_AGGR || SDIO_MULTI_PORT_RX_AGGR */

/** Maximum number of probes to send on each channel */
#define MAX_PROBES  4
/**
 * @brief Set/Get scan configuration parameters
 * 
 * @param priv     A pointer to moal_private structure
 * @param wrq      A pointer to iwreq structure
 * 
 * @return         0 --success, otherwise fail
 */
static int
woal_set_get_scan_cfg(moal_private * priv, struct iwreq *wrq)
{
    int data[6], ret = 0;
    mlan_ds_scan *scan = NULL;
    mlan_ioctl_req *req = NULL;

    ENTER();
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_scan));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    if (wrq->u.data.length > 6) {
        ret = -EINVAL;
        goto done;
    }
    scan = (mlan_ds_scan *) req->pbuf;
    scan->sub_command = MLAN_OID_SCAN_CONFIG;
    req->req_id = MLAN_IOCTL_SCAN;
    memset(data, 0, sizeof(data));
    if (wrq->u.data.length) {
        if (copy_from_user
            (data, wrq->u.data.pointer, (wrq->u.data.length * sizeof(int)))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        if ((data[0] < 0) || (data[0] > MLAN_SCAN_TYPE_PASSIVE)) {
            PRINTM(MERROR, "Invalid argument for scan type\n");
            ret = -EINVAL;
            goto done;
        }
        if ((data[1] < 0) || (data[1] > MLAN_SCAN_MODE_ANY)) {
            PRINTM(MERROR, "Invalid argument for scan mode\n");
            ret = -EINVAL;
            goto done;
        }
        if ((data[2] < 0) || (data[2] > MAX_PROBES)) {
            PRINTM(MERROR, "Invalid argument for scan probes\n");
            ret = -EINVAL;
            goto done;
        }
        if (((data[3] < 0) || (data[3] > MRVDRV_MAX_ACTIVE_SCAN_CHAN_TIME)) ||
            ((data[4] < 0) || (data[4] > MRVDRV_MAX_ACTIVE_SCAN_CHAN_TIME)) ||
            ((data[5] < 0) || (data[5] > MRVDRV_MAX_PASSIVE_SCAN_CHAN_TIME))) {
            PRINTM(MERROR, "Invalid argument for scan time\n");
            ret = -EINVAL;
            goto done;
        }
        req->action = MLAN_ACT_SET;
        memcpy(&scan->param.scan_cfg, data, sizeof(data));
    } else
        req->action = MLAN_ACT_GET;
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    if (!wrq->u.data.length) {
        memcpy(data, &scan->param.scan_cfg, sizeof(data));
        if (copy_to_user(wrq->u.data.pointer, data, sizeof(data))) {
            ret = -EFAULT;
            goto done;
        }
        wrq->u.data.length = sizeof(data) / sizeof(int);
    }
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/** VSIE configuration buffer length */
#define VSIE_MAX_CFG_LEN    (MLAN_MAX_VSIE_LEN - 2 + 3)
/** VSIE mask to remove the IE */
#define VSIE_MASK_DISABLE   0x00
/** VSIE Action : Get */
#define VSIE_ACTION_GET     0
/** VSIE Action : Add */
#define VSIE_ACTION_ADD     1
/** VSIE Action : Delete */
#define VSIE_ACTION_DELETE  2
/** 
 *  @brief Get/Add/Remove vendor specific IE
 *   
 *  @param priv         A pointer to moal_private structure
 *  @param wrq          A pointer to iwreq structure
 *  @return             MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static int
woal_vsie_cfg_ioctl(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0, user_data_len = 0, ie_len = 0;
    mlan_ds_misc_cfg *misc = NULL;
    mlan_ioctl_req *req = NULL;
    t_u8 *buf = NULL;

    ENTER();

    user_data_len = wrq->u.data.length;
    if (user_data_len < 2 || user_data_len == 3 ||
        user_data_len > VSIE_MAX_CFG_LEN) {
        PRINTM(MERROR, "Invalid argument number!\n");
        LEAVE();
        return -EINVAL;
    }
    if (!(buf = (t_u8 *) kmalloc(VSIE_MAX_CFG_LEN, GFP_ATOMIC))) {
        PRINTM(MERROR, "Cannot allocate buffer for command!\n");
        LEAVE();
        return -ENOMEM;
    }
    memset(buf, 0, VSIE_MAX_CFG_LEN);
    if (copy_from_user(buf, wrq->u.data.pointer, user_data_len)) {
        PRINTM(MINFO, "Copy from user failed\n");
        ret = -EFAULT;
        goto done;
    }

    if ((buf[0] > VSIE_ACTION_DELETE) ||
        (buf[1] > MLAN_MAX_VSIE_NUM - 1) ||
        ((buf[0] == VSIE_ACTION_ADD) &&
         !(buf[2] &&
           buf[2] <=
           (MLAN_VSIE_MASK_SCAN | MLAN_VSIE_MASK_ASSOC |
            MLAN_VSIE_MASK_ADHOC)))) {
        PRINTM(MERROR, "Invalid argument!\n");
        ret = -EINVAL;
        goto done;
    }

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_misc_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    req->req_id = MLAN_IOCTL_MISC_CFG;
    misc = (mlan_ds_misc_cfg *) req->pbuf;
    misc->sub_command = MLAN_OID_MISC_VS_IE;

    misc->param.vsie.id = buf[1];
    misc->param.vsie.mask = VSIE_MASK_DISABLE;
    switch (buf[0]) {
    case VSIE_ACTION_GET:
        req->action = MLAN_ACT_GET;
        break;
    case VSIE_ACTION_ADD:
        ie_len = user_data_len - 3;
        misc->param.vsie.mask = buf[2];
        misc->param.vsie.ie[1] = ie_len;
        memcpy(&misc->param.vsie.ie[2], &buf[3], ie_len);
    case VSIE_ACTION_DELETE:
        /* Set with mask 0 is remove */
        req->action = MLAN_ACT_SET;
        break;
    default:
        break;
    }

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }

    wrq->u.data.length = misc->param.vsie.ie[1];
    if (wrq->u.data.length) {
        if (copy_to_user
            (wrq->u.data.pointer, &misc->param.vsie.ie[2],
             wrq->u.data.length)) {
            PRINTM(MINFO, "Copy to user failed\n");
            ret = -EFAULT;
            goto done;
        }
    }

  done:
    if (buf)
        kfree(buf);
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 * @brief Set/Get PS configuration parameters
 * 
 * @param priv     A pointer to moal_private structure
 * @param wrq      A pointer to iwreq structure
 * 
 * @return         0 --success, otherwise fail
 */
static int
woal_set_get_ps_cfg(moal_private * priv, struct iwreq *wrq)
{
    int data[7], ret = 0;
    mlan_ds_pm_cfg *pm_cfg = NULL;
    mlan_ioctl_req *req = NULL;
    int allowed = 3;
    int i = 3;

    ENTER();

    allowed++;                  /* For ad-hoc awake period parameter */
    allowed++;                  /* For beacon missing timeout parameter */
    allowed += 2;               /* For delay to PS and PS mode parameters */

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_pm_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    if (wrq->u.data.length > allowed) {
        ret = -EINVAL;
        goto done;
    }
    pm_cfg = (mlan_ds_pm_cfg *) req->pbuf;
    pm_cfg->sub_command = MLAN_OID_PM_CFG_PS_CFG;
    req->req_id = MLAN_IOCTL_PM_CFG;
    memset(data, 0, sizeof(data));
    if (wrq->u.data.length) {
        if (copy_from_user
            (data, wrq->u.data.pointer, (wrq->u.data.length * sizeof(int)))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        if ((data[0] < PS_NULL_DISABLE)) {
            PRINTM(MERROR, "Invalid argument for PS null interval\n");
            ret = -EINVAL;
            goto done;
        }
        if ((data[1] != IGNORE_MULTIPLE_DTIM) &&
            ((data[1] < MIN_MULTIPLE_DTIM) || (data[1] > MAX_MULTIPLE_DTIM))) {
            PRINTM(MERROR, "Invalid argument for multiple DTIM\n");
            ret = -EINVAL;
            goto done;
        }
        if (data[2] < MIN_LISTEN_INTERVAL) {
            PRINTM(MERROR, "Invalid argument for listen interval\n");
            ret = -EINVAL;
            goto done;
        }
        if ((data[i] != SPECIAL_ADHOC_AWAKE_PD) &&
            ((data[i] < MIN_ADHOC_AWAKE_PD) ||
             (data[i] > MAX_ADHOC_AWAKE_PD))) {
            PRINTM(MERROR, "Invalid argument for adhoc awake period\n");
            ret = -EINVAL;
            goto done;
        }
        i++;
        if ((data[i] != DISABLE_BCN_MISS_TO) &&
            ((data[i] < MIN_BCN_MISS_TO) || (data[i] > MAX_BCN_MISS_TO))) {
            PRINTM(MERROR, "Invalid argument for beacon miss timeout\n");
            ret = -EINVAL;
            goto done;
        }
        i++;
        if (wrq->u.data.length < allowed - 1)
            data[i] = DELAY_TO_PS_UNCHANGED;
        else if ((data[i] < MIN_DELAY_TO_PS) || (data[i] > MAX_DELAY_TO_PS)) {
            PRINTM(MERROR, "Invalid argument for delay to PS\n");
            ret = -EINVAL;
            goto done;
        }
        i++;
        if ((data[i] != PS_MODE_UNCHANGED) && (data[i] != PS_MODE_AUTO) &&
            (data[i] != PS_MODE_POLL) && (data[i] != PS_MODE_NULL)) {
            PRINTM(MERROR, "Invalid argument for PS mode\n");
            ret = -EINVAL;
            goto done;
        }
        i++;
        req->action = MLAN_ACT_SET;
        memcpy(&pm_cfg->param.ps_cfg, data, sizeof(pm_cfg->param.ps_cfg));
    } else
        req->action = MLAN_ACT_GET;

    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT)) {
        ret = -EFAULT;
        goto done;
    }
    memcpy(data, &pm_cfg->param.ps_cfg, sizeof(pm_cfg->param.ps_cfg));
    if (copy_to_user(wrq->u.data.pointer, data, sizeof(int) * allowed)) {
        ret = -EFAULT;
        goto done;
    }
    wrq->u.data.length = allowed;
  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Private IOCTL entry to send an ADDTS TSPEC
 *
 *  Receive a ADDTS command from the application.  The command structure
 *    contains a TSPEC and timeout in milliseconds.  The timeout is performed
 *    in the firmware after the ADDTS command frame is sent.
 *
 *  The TSPEC is received in the API as an opaque block. The firmware will
 *    send the entire data block, including the bytes after the TSPEC.  This
 *    is done to allow extra IEs to be packaged with the TSPEC in the ADDTS
 *    action frame.
 *
 *  The IOCTL structure contains two return fields:
 *    - The firmware command result, which indicates failure and timeouts
 *    - The IEEE Status code which contains the corresponding value from
 *      any ADDTS response frame received.
 *
 *  In addition, the opaque TSPEC data block passed in is replaced with the
 *    TSPEC received in the ADDTS response frame.  In case of failure, the
 *    AP may modify the TSPEC on return and in the case of success, the
 *    medium time is returned as calculated by the AP.  Along with the TSPEC,
 *    any IEs that are sent in the ADDTS response are also returned and can be
 *    parsed using the IOCTL length as an indicator of extra elements.
 *
 *  The return value to the application layer indicates a driver execution
 *    success or failure.  A successful return could still indicate a firmware
 *    failure or AP negotiation failure via the commandResult field copied
 *    back to the application.
 *
 *  @param priv    Pointer to the mlan_private driver data struct
 *  @param wrq     A pointer to iwreq structure containing the
 *                 wlan_ioctl_wmm_addts_req_t struct for this ADDTS request
 *
 *  @return        0 if successful; IOCTL error code otherwise
 */
static int
woal_wmm_addts_req_ioctl(moal_private * priv, struct iwreq *wrq)
{
    mlan_ioctl_req *req = NULL;
    mlan_ds_wmm_cfg *cfg = NULL;
    wlan_ioctl_wmm_addts_req_t addts_ioctl;
    int ret = 0;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_wmm_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    req->req_id = MLAN_IOCTL_WMM_CFG;
    cfg = (mlan_ds_wmm_cfg *) req->pbuf;
    cfg->sub_command = MLAN_OID_WMM_CFG_ADDTS;

    memset(&addts_ioctl, 0x00, sizeof(addts_ioctl));

    if (wrq->u.data.length) {
        if (copy_from_user(&addts_ioctl, wrq->u.data.pointer,
                           MIN(wrq->u.data.length, sizeof(addts_ioctl)))) {
            PRINTM(MERROR, "TSPEC: ADDTS copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }

        cfg->param.addts.timeout = addts_ioctl.timeout_ms;
        cfg->param.addts.ie_data_len = addts_ioctl.ie_data_len;

        memcpy(cfg->param.addts.ie_data,
               addts_ioctl.ie_data, addts_ioctl.ie_data_len);

        cfg->param.addts.ie_data_len = addts_ioctl.ie_data_len;

        if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req,
                                                      MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            goto done;
        }
        addts_ioctl.cmd_result = cfg->param.addts.result;
        addts_ioctl.ieee_status_code = (t_u8) cfg->param.addts.status_code;
        addts_ioctl.ie_data_len = cfg->param.addts.ie_data_len;

        memcpy(addts_ioctl.ie_data,
               cfg->param.addts.ie_data, cfg->param.addts.ie_data_len);

        wrq->u.data.length = (sizeof(addts_ioctl)
                              - sizeof(addts_ioctl.ie_data)
                              + cfg->param.addts.ie_data_len);

        if (copy_to_user(wrq->u.data.pointer, &addts_ioctl, wrq->u.data.length)) {
            PRINTM(MERROR, "TSPEC: ADDTS copy to user failed\n");
            ret = -EFAULT;
            goto done;
        }
    }

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Private IOCTL entry to send a DELTS TSPEC
 *
 *  Receive a DELTS command from the application.  The command structure
 *    contains a TSPEC and reason code along with space for a command result
 *    to be returned.  The information is packaged is sent to the wlan_cmd.c
 *    firmware command prep and send routines for execution in the firmware.
 *
 *  The reason code is not used for WMM implementations but is indicated in
 *    the 802.11e specification.
 *
 *  The return value to the application layer indicates a driver execution
 *    success or failure.  A successful return could still indicate a firmware
 *    failure via the cmd_result field copied back to the application.
 *
 *  @param priv    Pointer to the mlan_private driver data struct
 *  @param wrq     A pointer to iwreq structure containing the
 *                 wlan_ioctl_wmm_delts_req_t struct for this DELTS request
 *
 *  @return        0 if successful; IOCTL error code otherwise
 */
static int
woal_wmm_delts_req_ioctl(moal_private * priv, struct iwreq *wrq)
{
    mlan_ioctl_req *req = NULL;
    mlan_ds_wmm_cfg *cfg = NULL;
    wlan_ioctl_wmm_delts_req_t delts_ioctl;
    int ret = 0;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_wmm_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    req->req_id = MLAN_IOCTL_WMM_CFG;
    cfg = (mlan_ds_wmm_cfg *) req->pbuf;
    cfg->sub_command = MLAN_OID_WMM_CFG_DELTS;

    memset(&delts_ioctl, 0x00, sizeof(delts_ioctl));

    if (wrq->u.data.length) {
        if (copy_from_user(&delts_ioctl, wrq->u.data.pointer,
                           MIN(wrq->u.data.length, sizeof(delts_ioctl)))) {
            PRINTM(MERROR, "TSPEC: DELTS copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }

        cfg->param.delts.status_code = (t_u32) delts_ioctl.ieee_reason_code;
        cfg->param.delts.ie_data_len = delts_ioctl.ie_data_len;

        memcpy(cfg->param.delts.ie_data,
               delts_ioctl.ie_data, delts_ioctl.ie_data_len);

        if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req,
                                                      MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            goto done;
        }

        /* Return the firmware command result back to the application layer */
        delts_ioctl.cmd_result = cfg->param.delts.result;
        wrq->u.data.length = sizeof(delts_ioctl);

        if (copy_to_user(wrq->u.data.pointer, &delts_ioctl, wrq->u.data.length)) {
            PRINTM(MERROR, "TSPEC: DELTS copy to user failed\n");
            ret = -EFAULT;
            goto done;
        }
    }

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Private IOCTL entry to get/set a specified AC Queue's parameters
 *
 *  Receive a AC Queue configuration command which is used to get, set, or
 *    default the parameters associated with a specific WMM AC Queue.
 *
 *  @param priv    Pointer to the mlan_private driver data struct
 *  @param wrq     A pointer to iwreq structure containing the
 *                 wlan_ioctl_wmm_queue_config_t struct
 *
 *  @return        0 if successful; IOCTL error code otherwise
 */
static int
woal_wmm_queue_config_ioctl(moal_private * priv, struct iwreq *wrq)
{
    mlan_ioctl_req *req = NULL;
    mlan_ds_wmm_cfg *pwmm = NULL;
    mlan_ds_wmm_queue_config *pqcfg = NULL;
    wlan_ioctl_wmm_queue_config_t qcfg_ioctl;
    int ret = 0;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_wmm_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    req->req_id = MLAN_IOCTL_WMM_CFG;
    pwmm = (mlan_ds_wmm_cfg *) req->pbuf;
    pwmm->sub_command = MLAN_OID_WMM_CFG_QUEUE_CONFIG;

    memset(&qcfg_ioctl, 0x00, sizeof(qcfg_ioctl));
    pqcfg = (mlan_ds_wmm_queue_config *) & pwmm->param.q_cfg;

    if (wrq->u.data.length) {
        if (copy_from_user(&qcfg_ioctl, wrq->u.data.pointer,
                           MIN(wrq->u.data.length, sizeof(qcfg_ioctl)))) {
            PRINTM(MERROR, "QCONFIG: copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }

        pqcfg->action = qcfg_ioctl.action;
        pqcfg->access_category = qcfg_ioctl.access_category;
        pqcfg->msdu_lifetime_expiry = qcfg_ioctl.msdu_lifetime_expiry;

        if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req,
                                                      MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            goto done;
        }
        memset(&qcfg_ioctl, 0x00, sizeof(qcfg_ioctl));
        qcfg_ioctl.action = pqcfg->action;
        qcfg_ioctl.access_category = pqcfg->access_category;
        qcfg_ioctl.msdu_lifetime_expiry = pqcfg->msdu_lifetime_expiry;
        wrq->u.data.length = sizeof(qcfg_ioctl);

        if (copy_to_user(wrq->u.data.pointer, &qcfg_ioctl, wrq->u.data.length)) {
            PRINTM(MERROR, "QCONFIG: copy to user failed\n");
            ret = -EFAULT;
            goto done;
        }
    }

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Private IOCTL entry to get and start/stop queue stats on a WMM AC
 *
 *  Receive a AC Queue statistics command from the application for a specific
 *    WMM AC.  The command can:
 *         - Turn stats on
 *         - Turn stats off
 *         - Collect and clear the stats
 *
 *  @param priv    Pointer to the moal_private driver data struct
 *  @param wrq     A pointer to iwreq structure containing the
 *                 wlan_ioctl_wmm_queue_stats_t struct
 *
 *  @return        0 if successful; IOCTL error code otherwise
 */
static int
woal_wmm_queue_stats_ioctl(moal_private * priv, struct iwreq *wrq)
{
    mlan_ioctl_req *req = NULL;
    mlan_ds_wmm_cfg *pwmm = NULL;
    mlan_ds_wmm_queue_stats *pqstats = NULL;
    wlan_ioctl_wmm_queue_stats_t qstats_ioctl;
    int ret = 0;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_wmm_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    req->req_id = MLAN_IOCTL_WMM_CFG;
    pwmm = (mlan_ds_wmm_cfg *) req->pbuf;
    pwmm->sub_command = MLAN_OID_WMM_CFG_QUEUE_STATS;

    memset(&qstats_ioctl, 0x00, sizeof(qstats_ioctl));
    pqstats = (mlan_ds_wmm_queue_stats *) & pwmm->param.q_stats;

    if (wrq->u.data.length) {
        if (copy_from_user(&qstats_ioctl, wrq->u.data.pointer,
                           MIN(wrq->u.data.length, sizeof(qstats_ioctl)))) {
            PRINTM(MERROR, "QSTATS: copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }

        memcpy((void *) pqstats, (void *) &qstats_ioctl, sizeof(qstats_ioctl));
        PRINTM(MINFO, "QSTATS: IOCTL [%d,%d]\n", qstats_ioctl.action,
               qstats_ioctl.access_category);

        if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req,
                                                      MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            goto done;
        }

        memset(&qstats_ioctl, 0x00, sizeof(qstats_ioctl));
        memcpy((void *) &qstats_ioctl, (void *) pqstats, sizeof(qstats_ioctl));
        wrq->u.data.length = sizeof(qstats_ioctl);

        if (copy_to_user
            (wrq->u.data.pointer, &qstats_ioctl, wrq->u.data.length)) {
            PRINTM(MERROR, "QSTATS: copy to user failed\n");
            ret = -EFAULT;
            goto done;
        }
    }

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Private IOCTL entry to get the status of the WMM queues
 *
 *  Return the following information for each WMM AC:
 *        - WMM IE Acm Required
 *        - Firmware Flow Required
 *        - Firmware Flow Established
 *        - Firmware Queue Enabled
 *        - Firmware Delivery Enabled
 *        - Firmware Trigger Enabled
 *
 *  @param priv    Pointer to the moal_private driver data struct
 *  @param wrq     A pointer to iwreq structure containing the
 *                 wlan_ioctl_wmm_queue_status_t struct for request
 *
 *  @return        0 if successful; IOCTL error code otherwise
 */
static int
woal_wmm_queue_status_ioctl(moal_private * priv, struct iwreq *wrq)
{
    mlan_ioctl_req *req = NULL;
    mlan_ds_wmm_cfg *pwmm = NULL;
    wlan_ioctl_wmm_queue_status_t qstatus_ioctl;
    int ret = 0;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_wmm_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    req->req_id = MLAN_IOCTL_WMM_CFG;
    pwmm = (mlan_ds_wmm_cfg *) req->pbuf;
    pwmm->sub_command = MLAN_OID_WMM_CFG_QUEUE_STATUS;

    if (wrq->u.data.length == sizeof(qstatus_ioctl)) {
        if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_NO_WAIT)) {
            ret = -EFAULT;
            goto done;
        }

        memset(&qstatus_ioctl, 0x00, sizeof(qstatus_ioctl));
        memcpy((void *) &qstatus_ioctl, (void *) &pwmm->param.q_status,
               sizeof(qstatus_ioctl));
        wrq->u.data.length = sizeof(qstatus_ioctl);
    } else {
        wrq->u.data.length = 0;
    }

    if (copy_to_user(wrq->u.data.pointer, &qstatus_ioctl, wrq->u.data.length)) {
        PRINTM(MERROR, "QSTATUS: copy to user failed\n");
        ret = -EFAULT;
        goto done;
    }

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Private IOCTL entry to get the status of the WMM Traffic Streams
 *
 *  @param priv    Pointer to the moal_private driver data struct
 *  @param wrq     A pointer to iwreq structure containing the
 *                 wlan_ioctl_wmm_ts_status_t struct for request
 *
 *  @return        0 if successful; IOCTL error code otherwise
 */
static int
woal_wmm_ts_status_ioctl(moal_private * priv, struct iwreq *wrq)
{
    mlan_ioctl_req *req = NULL;
    mlan_ds_wmm_cfg *pwmm = NULL;
    wlan_ioctl_wmm_ts_status_t ts_status_ioctl;
    int ret = 0;

    ENTER();

    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_wmm_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    req->req_id = MLAN_IOCTL_WMM_CFG;
    pwmm = (mlan_ds_wmm_cfg *) req->pbuf;
    pwmm->sub_command = MLAN_OID_WMM_CFG_TS_STATUS;

    memset(&ts_status_ioctl, 0x00, sizeof(ts_status_ioctl));

    if (wrq->u.data.length == sizeof(ts_status_ioctl)) {
        if (copy_from_user(&ts_status_ioctl, wrq->u.data.pointer,
                           MIN(wrq->u.data.length, sizeof(ts_status_ioctl)))) {
            PRINTM(MERROR, "TS_STATUS: copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }

        memset(&pwmm->param.ts_status, 0x00, sizeof(ts_status_ioctl));
        pwmm->param.ts_status.tid = ts_status_ioctl.tid;

        if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req,
                                                      MOAL_IOCTL_WAIT)) {
            ret = -EFAULT;
            goto done;
        }

        memset(&ts_status_ioctl, 0x00, sizeof(ts_status_ioctl));
        memcpy((void *) &ts_status_ioctl, (void *) &pwmm->param.ts_status,
               sizeof(ts_status_ioctl));
        wrq->u.data.length = sizeof(ts_status_ioctl);
    } else {
        wrq->u.data.length = 0;
    }

    if (copy_to_user(wrq->u.data.pointer, &ts_status_ioctl, wrq->u.data.length)) {
        PRINTM(MERROR, "TS_STATUS: copy to user failed\n");
        ret = -EFAULT;
        goto done;
    }

  done:
    if (req)
        kfree(req);
    LEAVE();
    return ret;
}

/**
 *  @brief Private IOCTL entry to get the By-passed TX packet from upper layer
 *
 *  @param priv    Pointer to the moal_private driver data struct
 *  @param wrq     A pointer to iwreq structure containing the packet
 *
 *  @return        0 if successful; IOCTL error code otherwise
 */
static int
woal_bypassed_packet_ioctl(moal_private * priv, struct iwreq *wrq)
{
    int ret = 0;
    struct sk_buff *skb = NULL;
    struct ethhdr *eth;
    t_u16 moreLen = 0, copyLen = 0;
    ENTER();

#define MLAN_BYPASS_PKT_EXTRA_OFFSET        (4)

    copyLen = wrq->u.data.length;
    moreLen = MLAN_MIN_DATA_HEADER_LEN + MLAN_BYPASS_PKT_EXTRA_OFFSET
        + sizeof(mlan_buffer);

    skb = alloc_skb(copyLen + moreLen, GFP_KERNEL);
    if (skb == NULL) {
        PRINTM(MERROR, "kmalloc no memory !!\n");
        LEAVE();
        return -ENOMEM;
    }

    skb_reserve(skb, moreLen);

    if (copy_from_user(skb_put(skb, copyLen), wrq->u.data.pointer, copyLen)) {
        PRINTM(MERROR, "PortBlock: copy from user failed\n");
        ret = -EFAULT;
        goto done;
    }

    eth = (struct ethhdr *) skb->data;
    eth->h_proto = __constant_htons(eth->h_proto);
    skb->dev = priv->netdev;

    HEXDUMP("Bypass TX Data", skb->data, MIN(skb->len, 100));

    woal_hard_start_xmit(skb, priv->netdev);
  done:
    LEAVE();
    return ret;
}

/**
 *  @brief Set/Get auth type
 *
 *  @param priv     Pointer to the moal_private driver data struct
 *  @param wrq	    A pointer to iwreq structure
 *
 *  @return          0 --success, otherwise fail
 */
static int
woal_auth_type(moal_private * priv, struct iwreq *wrq)
{
    int auth_type;
    t_u32 auth_mode;
    int ret = 0;

    ENTER();
    if (wrq->u.data.length == 0) {
        if (MLAN_STATUS_SUCCESS !=
            woal_get_auth_mode(priv, MOAL_IOCTL_WAIT, &auth_mode)) {
            ret = -EFAULT;
            goto done;
        }
        auth_type = auth_mode;
        if (copy_to_user(wrq->u.data.pointer, &auth_type, sizeof(auth_type))) {
            PRINTM(MERROR, "Copy to user failed\n");
            ret = -EFAULT;
            goto done;
        }
        wrq->u.data.length = 1;
    } else {
        if (copy_from_user(&auth_type, wrq->u.data.pointer, sizeof(auth_type))) {
            PRINTM(MERROR, "Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        PRINTM(MINFO, "SET: auth_type %d\n", auth_type);
        if (((auth_type < MLAN_AUTH_MODE_OPEN) ||
             (auth_type > MLAN_AUTH_MODE_SHARED))
            && (auth_type != MLAN_AUTH_MODE_AUTO)) {
            ret = -EINVAL;
            goto done;
        }
        auth_mode = auth_type;
        if (MLAN_STATUS_SUCCESS !=
            woal_set_auth_mode(priv, MOAL_IOCTL_WAIT, auth_mode)) {
            ret = -EFAULT;
            goto done;
        }
    }
  done:
    LEAVE();
    return ret;
}

/********************************************************
		Global Functions
********************************************************/

/** 
 *  @brief Get version 
 *   
 *  @param handle 		A pointer to moal_handle structure
 *  @param version		A pointer to version buffer
 *  @param max_len		max length of version buffer
 *
 *  @return 	   		N/A
 */
void
woal_get_version(moal_handle * handle, char *version, int max_len)
{
    union
    {
        t_u32 l;
        t_u8 c[4];
    } ver;
    char fw_ver[32];

    ENTER();

    ver.l = handle->fw_release_number;
    sprintf(fw_ver, "%u.%u.%u.p%u", ver.c[2], ver.c[1], ver.c[0], ver.c[3]);

    snprintf(version, max_len, driver_version, fw_ver);

    LEAVE();
}

/**
 *  @brief Set/Get Port Control mode
 *
 *  @param priv     Pointer to the moal_private driver data struct
 *  @param wrq	    A pointer to iwreq structure
 *
 *  @return          0 --success, otherwise fail
 */
static int
woal_port_ctrl(moal_private * priv, struct iwreq *wrq)
{
    mlan_ioctl_req *req = NULL;
    mlan_ds_sec_cfg *sec = NULL;
    int ret = 0;
    ENTER();

    /* Allocate an IOCTL request buffer */
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_sec_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }

    /* Fill request buffer */
    sec = (mlan_ds_sec_cfg *) req->pbuf;
    sec->sub_command = MLAN_OID_SEC_CFG_PORT_CTRL_ENABLED;
    req->req_id = MLAN_IOCTL_SEC_CFG;

    if (wrq->u.data.length) {
        if (copy_from_user(&sec->param.port_ctrl_enabled,
                           wrq->u.data.pointer, sizeof(int)) != 0) {
            PRINTM(MERROR, "port_ctrl:Copy from user failed\n");
            ret = -EFAULT;
            goto done;
        }
        req->action = MLAN_ACT_SET;
    } else {
        req->action = MLAN_ACT_GET;
    }

    /* Send IOCTL request to MLAN */
    if (woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT) != MLAN_STATUS_SUCCESS) {
        ret = -EFAULT;
        goto done;
    }

    if (!wrq->u.data.length) {
        if (copy_to_user(wrq->u.data.pointer, &sec->param.port_ctrl_enabled,
                         sizeof(int))) {
            PRINTM(MERROR, "port_ctrl:Copy to user failed\n");
            ret = -EFAULT;
            goto done;
        }
        wrq->u.data.length = 1;
    }

  done:
    if (req)
        kfree(req);

    LEAVE();
    return ret;
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
woal_do_ioctl(struct net_device *dev, struct ifreq *req, int cmd)
{
    moal_private *priv = (moal_private *) netdev_priv(dev);
    struct iwreq *wrq = (struct iwreq *) req;
    int ret = 0;

    ENTER();

    PRINTM(MINFO, "woal_do_ioctl: ioctl cmd = 0x%x\n", cmd);
    switch (cmd) {
    case WOAL_SETONEINT_GETWORDCHAR:
        switch (wrq->u.data.flags) {
        case WOAL_VERSION:     /* Get driver version */
            ret = woal_get_driver_version(priv, req);
            break;
        case WOAL_VEREXT:      /* Get extended driver version */
            ret = woal_get_driver_verext(priv, req);
            break;
        default:
            ret = -EOPNOTSUPP;
            break;
        }
        break;
    case WOAL_SETNONE_GETNONE:
        switch (wrq->u.data.flags) {
        case WOAL_WARMRESET:
            ret = woal_warm_reset(priv);
            break;
        default:
            ret = -EOPNOTSUPP;
            break;
        }
        break;
    case WOAL_SETONEINT_GETONEINT:
        switch (wrq->u.data.flags) {
        case WOAL_SET_GET_TXRATE:
            ret = woal_set_get_txrate(priv, wrq);
            break;
        case WOAL_SET_GET_REGIONCODE:
            ret = woal_set_get_regioncode(priv, wrq);
            break;
        case WOAL_SET_RADIO:
            ret = woal_set_get_radio(priv, wrq);
            break;
        case WOAL_WMM_ENABLE:
            ret = woal_wmm_enable_ioctl(priv, wrq);
            break;
        case WOAL_11D_ENABLE:
            ret = woal_11d_enable_ioctl(priv, wrq);
            break;
        case WOAL_SET_GET_TX_RX_ANT:
            ret = woal_set_get_tx_rx_ant(priv, wrq);
            break;
        case WOAL_SET_GET_QOS_CFG:
            ret = woal_set_get_qos_cfg(priv, wrq);
            break;
        case WOAL_SET_GET_LDO_CFG:
            ret = woal_set_get_ldo_cfg(priv, wrq);
            break;
#ifdef REASSOCIATION
        case WOAL_SET_GET_REASSOC:
            ret = woal_set_get_reassoc(priv, wrq);
            break;
#endif /* REASSOCIATION */
        case WOAL_TXBUF_CFG:
            ret = woal_txbuf_cfg(priv, wrq);
            break;
        case WOAL_SET_GET_WWS_CFG:
            ret = woal_wws_cfg(priv, wrq);
            break;
        case WOAL_SLEEP_PD:
            ret = woal_sleep_pd(priv, wrq);
            break;
        case WOAL_AUTH_TYPE:
            ret = woal_auth_type(priv, wrq);
            break;
        case WOAL_PORT_CTRL:
            ret = woal_port_ctrl(priv, wrq);
            break;

        default:
            ret = -EOPNOTSUPP;
            break;
        }
        break;

    case WOAL_SET_GET_SIXTEEN_INT:
        switch ((int) wrq->u.data.flags) {
        case WOAL_TX_POWERCFG:
            ret = woal_tx_power_cfg(priv, wrq);
            break;
#ifdef DEBUG_LEVEL1
        case WOAL_DRV_DBG:
            ret = woal_drv_dbg(priv, wrq);
            break;
#endif
        case WOAL_BEACON_INTERVAL:
            ret = woal_beacon_interval(priv, wrq);
            break;
        case WOAL_ATIM_WINDOW:
            ret = woal_atim_window(priv, wrq);
            break;
        case WOAL_SIGNAL:
            ret = woal_get_signal(priv, wrq);
            break;
        case WOAL_DEEP_SLEEP:
            ret = woal_deep_sleep_ioctl(priv, wrq);
            break;
        case WOAL_11N_TX_CFG:
            ret = woal_11n_tx_cfg(priv, wrq);
            break;
        case WOAL_11N_AMSDU_AGGR_CTRL:
            ret = woal_11n_amsdu_aggr_ctrl(priv, wrq);
            break;
        case WOAL_11N_HTCAP_CFG:
            ret = woal_11n_htcap_cfg(priv, wrq);
            break;
        case WOAL_PRIO_TBL:
            ret = woal_11n_prio_tbl(priv, wrq);
            break;
        case WOAL_ADDBA_UPDT:
            ret = woal_addba_para_updt(priv, wrq);
            break;
        case WOAL_ADDBA_REJECT:
            ret = woal_addba_reject(priv, wrq);
            break;
        case WOAL_HS_CFG:
            ret = woal_hs_cfg(priv, wrq, MTRUE);
            break;
        case WOAL_HS_SETPARA:
            ret = woal_hs_setpara(priv, wrq);
            break;
        case WOAL_REG_READ_WRITE:
            ret = woal_reg_read_write(priv, wrq);
            break;
        case WOAL_INACTIVITY_TIMEOUT_EXT:
            ret = woal_inactivity_timeout_ext(priv, wrq);
            break;
        case WOAL_SDIO_CLOCK:
            ret = woal_sdio_clock_ioctl(priv, wrq);
            break;
        case WOAL_CMD_52RDWR:
            ret = woal_cmd52rdwr_ioctl(priv, wrq);
            break;
        case WOAL_BAND_CFG:
            ret = woal_band_cfg(priv, wrq);
            break;
        case WOAL_BCA_TIME_SHARE:
            ret = woal_bca_time_share(priv, wrq);
            break;
        case WOAL_SCAN_CFG:
            ret = woal_set_get_scan_cfg(priv, wrq);
            break;
        case WOAL_PS_CFG:
            ret = woal_set_get_ps_cfg(priv, wrq);
            break;
        case WOAL_MEM_READ_WRITE:
            ret = woal_mem_read_write(priv, wrq);
            break;
#if defined(SDIO_MULTI_PORT_TX_AGGR) || defined(SDIO_MULTI_PORT_RX_AGGR)
        case WOAL_SDIO_MPA_CTRL:
            ret = woal_do_sdio_mpa_ctrl(priv, wrq);
            break;
#endif
        case WOAL_SLEEP_PARAMS:
            ret = woal_sleep_params_ioctl(priv, wrq);
            break;
        default:
            ret = -EINVAL;
            break;
        }
        break;

    case WOALGETLOG:
        ret = woal_get_log(priv, wrq);
        break;
    case WOAL_SET_GET_256_CHAR:
        switch (wrq->u.data.flags) {
        case WOAL_PASSPHRASE:
            ret = woal_passphrase(priv, wrq);
            break;
        case WOAL_ADHOC_AES:
            ret = woal_adhoc_aes_ioctl(priv, wrq);
            break;
        case WOAL_WMM_QUEUE_STATUS:
            ret = woal_wmm_queue_status_ioctl(priv, wrq);
            break;
        case WOAL_WMM_TS_STATUS:
            ret = woal_wmm_ts_status_ioctl(priv, wrq);
            break;
        default:
            ret = -EINVAL;
            break;
        }
        break;

    case WOAL_SETADDR_GETNONE:
        switch ((int) wrq->u.data.flags) {
        case WOAL_DEAUTH:
            ret = woal_deauth(priv, wrq);
            break;
        default:
            ret = -EINVAL;
            break;
        }
        break;

    case WOAL_SETNONE_GETTWELVE_CHAR:
        /* 
         * We've not used IW_PRIV_TYPE_FIXED so sub-ioctl number is
         * in flags of iwreq structure, otherwise it will be in
         * mode member of iwreq structure.
         */
        switch ((int) wrq->u.data.flags) {
        case WOAL_WPS_SESSION:
            ret = woal_wps_cfg_ioctl(priv, wrq);
            break;
        default:
            ret = -EINVAL;
            break;
        }
        break;
    case WOAL_SETNONE_GET_FOUR_INT:
        switch ((int) wrq->u.data.flags) {
        case WOAL_DATA_RATE:
            ret = woal_get_txrx_rate(priv, wrq);
            break;
        case WOAL_ESUPP_MODE:
            ret = woal_get_esupp_mode(priv, wrq);
            break;
        default:
            ret = -EINVAL;
            break;
        }
        break;

#ifdef MFG_CMD_SUPPORT
    case WOAL_MFG_CMD:
        PRINTM(MINFO, "Entering the Manufacturing ioctl SIOCCFMFG\n");
        ret = woal_mfg_command(priv, wrq);
        PRINTM(MINFO, "Manufacturing Ioctl %s\n", (ret) ? "failed" : "success");
        break;
#endif
    case WOAL_SET_GET_64_INT:
        switch ((int) wrq->u.data.flags) {
        case WOAL_ECL_SYS_CLOCK:
            ret = woal_ecl_sys_clock(priv, wrq);
            break;
        }
        break;

    case WOAL_HOST_CMD:
        ret = woal_host_command(priv, wrq);
        break;
    case WOAL_ARP_FILTER:
        ret = woal_arp_filter(priv, wrq);
        break;
    case WOAL_SET_INTS_GET_CHARS:
        switch ((int) wrq->u.data.flags) {
        case WOAL_READ_EEPROM:
            ret = woal_read_eeprom(priv, wrq);
            break;
        }
        break;
    case WOAL_SET_GET_2K_BYTES:
        switch ((int) wrq->u.data.flags) {
        case WOAL_CMD_53RDWR:
            ret = woal_cmd53rdwr_ioctl(priv, wrq);
            break;
        case WOAL_SET_USER_SCAN:
            ret = woal_set_user_scan_ioctl(priv, wrq);
            break;
        case WOAL_GET_SCAN_TABLE:
            ret = woal_get_scan_table_ioctl(priv, wrq);
            break;
        case WOAL_VSIE_CFG:
            ret = woal_vsie_cfg_ioctl(priv, wrq);
            break;
        case WOAL_WMM_ADDTS:
            ret = woal_wmm_addts_req_ioctl(priv, wrq);
            break;
        case WOAL_WMM_DELTS:
            ret = woal_wmm_delts_req_ioctl(priv, wrq);
            break;
        case WOAL_WMM_QUEUE_CONFIG:
            ret = woal_wmm_queue_config_ioctl(priv, wrq);
            break;
        case WOAL_WMM_QUEUE_STATS:
            ret = woal_wmm_queue_stats_ioctl(priv, wrq);
            break;
        case WOAL_BYPASSED_PACKET:
            ret = woal_bypassed_packet_ioctl(priv, wrq);
            break;
        default:
            ret = -EINVAL;
            break;
        }
        break;

    case WOAL_CUSTOM_IE_CFG:
        ret = woal_custom_ie_ioctl(dev, req);
        break;
    case WOAL_GET_BSS_TYPE:
        ret = woal_get_bss_type(dev, req);
        break;
    default:
        ret = -EINVAL;
        break;
    }

    LEAVE();
    return ret;
}

/** 
 *  @brief Get mode
 *
 *  @param priv          A pointer to moal_private structure
 *  @param wait_option   Wait option (MOAL_WAIT or MOAL_NO_WAIT)   
 *
 *  @return              Wireless mode
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
 *  @param priv         A pointer to moal_private structure
 *  @param wait_option  Wait option
 *  @param signal       A pointer tp mlan_ds_get_signal structure
 *
 *  @return             MLAN_STATUS_SUCCESS -- success, otherwise fail          
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
 *  @brief Get statistics information
 *
 *  @param priv         A pointer to moal_private structure
 *  @param wait_option  Wait option
 *  @param stats        A pointer to mlan_ds_get_stats structure
 *
 *  @return             MLAN_STATUS_SUCCESS -- success, otherwise fail          
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
 *  @param priv          A pointer to moal_private structure
 *  @param wait_option   Wait option
 *  @param m_rates       A pointer to moal_802_11_rates structure
 *
 *  @return              MLAN_STATUS_SUCCESS -- success, otherwise fail          
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
 *  @param priv            A pointer to moal_private structure
 *  @param wait_option     Wait option
 *  @param chan_list       A pointer to mlan_chan_list structure
 *
 *  @return                MLAN_STATUS_SUCCESS -- success, otherwise fail          
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
 *  @brief Get scan table
 *
 *  @param priv         A pointer to moal_private structure
 *  @param wait_option  Wait option
 *  @param scan_resp    A pointer to mlan_scan_resp structure
 *
 *  @return             MLAN_STATUS_SUCCESS -- success, otherwise fail
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
    memcpy((void *) &scan->param.scan_resp, (void *) scan_resp,
           sizeof(mlan_scan_resp));

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
 *  @param encrypt_mode         Encryption mode
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
        PRINTM(MINFO,
               "Find network: ssid=%s, %02x:%02x:%02x:%02x:%02x:%02x, idx=%d\n",
               ssid_bssid->ssid.ssid, mac[0], mac[1], mac[2], mac[3], mac[4],
               mac[5], (int) ssid_bssid->idx);
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
    mlan_status status = MLAN_STATUS_SUCCESS;
    ENTER();

    if (MOAL_ACQ_SEMAPHORE_BLOCK(&priv->async_sem)) {
        PRINTM(MERROR, "Acquire semaphore error, request_scan\n");
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }
    priv->scan_pending_on_block = MTRUE;

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
    } else {
        /* Normal scan */
        ioctl_req->req_id = MLAN_IOCTL_SCAN;
        ioctl_req->action = MLAN_ACT_SET;

        scan->sub_command = MLAN_OID_SCAN_NORMAL;
    }
    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, ioctl_req, wait_option);
    if (status == MLAN_STATUS_FAILURE) {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }
  done:
    if ((ioctl_req) && (status != MLAN_STATUS_PENDING))
        kfree(ioctl_req);

    if (ret == MLAN_STATUS_FAILURE) {
        priv->scan_pending_on_block = MFALSE;
        MOAL_REL_SEMAPHORE(&priv->async_sem);
    }
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
    if (bss->param.bss_chan.channel == (unsigned int) channel) {
        ret = MLAN_STATUS_SUCCESS;
        goto done;
    }
    PRINTM(MINFO, "Updating Channel from %d to %d\n",
           (int) bss->param.bss_chan.channel, channel);

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

#if WIRELESS_EXT > 14

/** 
 *  @brief This function sends customized event to application.
 *  
 *  @param priv    A pointer to moal_private structure
 *  @param str	   A pointer to event string
 *
 *  @return 	   N/A
 */
void
woal_send_iwevcustom_event(moal_private * priv, t_s8 * str)
{
    union iwreq_data iwrq;
    char buf[50];

    ENTER();

    memset(&iwrq, 0, sizeof(union iwreq_data));
    memset(buf, 0, sizeof(buf));

    snprintf(buf, sizeof(buf) - 1, "%s", str);

    iwrq.data.pointer = buf;
    iwrq.data.length = strlen(buf) + 1 + IW_EV_LCP_LEN;

    /* Send Event to upper layer */
    wireless_send_event(priv->netdev, IWEVCUSTOM, &iwrq, buf);
    PRINTM(MINFO, "Wireless event %s is sent to application\n", str);

    LEAVE();
    return;
}
#endif

#if WIRELESS_EXT >= 18
/** 
 *  @brief This function sends mic error event to application.
 *  
 *  @param priv    A pointer to moal_private structure
 *  @param event   MIC MERROR EVENT. 
 *
 *  @return 	   N/A
 */
void
woal_send_mic_error_event(moal_private * priv, t_u32 event)
{
    union iwreq_data iwrq;
    struct iw_michaelmicfailure mic;

    ENTER();

    memset(&iwrq, 0, sizeof(iwrq));
    memset(&mic, 0, sizeof(mic));
    if (event == MLAN_EVENT_ID_FW_MIC_ERR_UNI)
        mic.flags = IW_MICFAILURE_PAIRWISE;
    else
        mic.flags = IW_MICFAILURE_GROUP;
    iwrq.data.pointer = &mic;
    iwrq.data.length = sizeof(mic);

    wireless_send_event(priv->netdev, IWEVMICHAELMICFAILURE, &iwrq,
                        (char *) &mic);

    LEAVE();
    return;
}
#endif

/** 
 *  @brief Handle ioctl resp 
 *   
 *  @param priv 	Pointer to moal_private structure
 *  @param req		Pointer to mlan_ioctl_req structure
 *
 *  @return    		N/A
 */
void
woal_process_ioctl_resp(moal_private * priv, mlan_ioctl_req * req)
{
    ENTER();

    if (priv == NULL)
        return;
    switch (req->req_id) {
    case MLAN_IOCTL_GET_INFO:
        woal_ioctl_get_info_resp(priv, (mlan_ds_get_info *) req->pbuf);
        break;
    case MLAN_IOCTL_BSS:
        woal_ioctl_get_bss_resp(priv, (mlan_ds_bss *) req->pbuf);
    default:
        break;
    }

    LEAVE();
    return;
}
