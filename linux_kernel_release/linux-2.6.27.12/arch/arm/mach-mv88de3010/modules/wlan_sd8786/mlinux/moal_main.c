/** @file moal_main.c
  *
  * @brief This file contains the major functions in WLAN
  * driver. 
  *
  * Copyright (C) 2008-2009, Marvell International Ltd. 
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
#include 	"moal_sdio.h"

/********************************************************
		Local Variables
********************************************************/

/** Driver version */
const char driver_version[] =
    "SD8786-%s-" MLAN_RELEASE_VERSION "-(" "FP" FPNUM ")"
#ifdef	DEBUG_LEVEL2
    "-dbg"
#endif
    " ";

/** Firmware name */
char *fw_name = NULL;

#ifdef MFG_CMD_SUPPORT
/** Mfg mode */
int mfg_mode = 0;
#endif

/** SDIO interrupt mode */
//extern int intmode;
/** GPIO interrupt pin */
extern int gpiopin;

/** woal_callbacks */
static mlan_callbacks woal_callbacks = {
    .moal_init_fw_complete = moal_init_fw_complete,
    .moal_shutdown_fw_complete = moal_shutdown_fw_complete,
    .moal_send_packet_complete = moal_send_packet_complete,
    .moal_recv_complete = moal_recv_complete,
    .moal_recv_packet = moal_recv_packet,
    .moal_recv_event = moal_recv_event,
    .moal_ioctl_complete = moal_ioctl_complete,
    .moal_alloc_mlan_buffer = moal_alloc_mlan_buffer,
    .moal_free_mlan_buffer = moal_free_mlan_buffer,
    .moal_write_reg = moal_write_reg,
    .moal_read_reg = moal_read_reg,
    .moal_write_data_sync = moal_write_data_sync,
    .moal_read_data_sync = moal_read_data_sync,
    .moal_malloc = moal_malloc,
    .moal_mfree = moal_mfree,
    .moal_memset = moal_memset,
    .moal_memcpy = moal_memcpy,
    .moal_memmove = moal_memmove,
    .moal_memcmp = moal_memcmp,
    .moal_get_system_time = moal_get_system_time,
    .moal_init_timer = moal_init_timer,
    .moal_free_timer = moal_free_timer,
    .moal_start_timer = moal_start_timer,
    .moal_stop_timer = moal_stop_timer,
    .moal_init_lock = moal_init_lock,
    .moal_free_lock = moal_free_lock,
    .moal_spin_lock = moal_spin_lock,
    .moal_spin_unlock = moal_spin_unlock,
    .moal_print = moal_print,
    .moal_get_cardregs = moal_get_cardregs,
};

/** BSS attributes */
static mlan_bss_attr woal_bss_sta[] = {
    {MLAN_BSS_TYPE_STA, MLAN_DATA_FRAME_TYPE_ETH_II, MTRUE, 0},
};

int drv_mode = DRV_MODE_STA;

/** Supported drv_mode table */
static moal_drv_mode drv_mode_tbl[] = {
    {
     /* drv_mode */
     .drv_mode = DRV_MODE_STA,
     /* intf number */
     .intf_num = sizeof(woal_bss_sta) / sizeof(woal_bss_sta[0]),
     /* bss_attr */
     .bss_attr = woal_bss_sta,
     /* fw name */
     .fw_name = DEFAULT_FW_NAME,
     }
    ,
};

/********************************************************
		Global Variables
********************************************************/

/** Semaphore for add/remove card */
struct semaphore AddRemoveCardSem;
/**
 * The global variable of a pointer to moal_handle
 * structure variable
 */
moal_handle *m_handle = NULL;

#ifdef DEBUG_LEVEL1
#ifdef DEBUG_LEVEL2
#define	DEFAULT_DEBUG_MASK	(0xffffffff & ~MEVENT)
#else
#define DEFAULT_DEBUG_MASK	(MMSG | MFATAL | MERROR)
#endif /* DEBUG_LEVEL2 */
t_u32 drvdbg = DEFAULT_DEBUG_MASK;
t_u32 ifdbg = 0;
#endif /* DEBUG_LEVEL1 */

/********************************************************
		Local Functions
********************************************************/
/** 
 *  @brief This function initializes software
 *  
 *  @param handle A pointer to moal_handle structure
 *
 *  @return 	   MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
woal_init_sw(moal_handle * handle)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    int i;
    mlan_device device;
    t_void *pmlan;

    ENTER();

    /* find moal_drv_mode entry from drv_mode_tbl */
    handle->drv_mode = NULL;
    for (i = 0; i < (sizeof(drv_mode_tbl) / sizeof(drv_mode_tbl[0])); i++) {
        if (drv_mode_tbl[i].drv_mode == drv_mode) {
            handle->drv_mode = &drv_mode_tbl[i];
            break;
        }
    }

    if (!handle->drv_mode) {
        PRINTM(MERROR, "Invalid drv_mode=%d\n", drv_mode);
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }

    /* Initialize moal_handle structure */
    handle->hardware_status = HardwareStatusInitializing;
    /* PnP and power profile */
    handle->surprise_removed = MFALSE;
    init_waitqueue_head(&handle->init_wait_q);

#ifdef REASSOCIATION
    MOAL_INIT_SEMAPHORE(&handle->reassoc_sem);

#if (WIRELESS_EXT >= 18)
    handle->reassoc_on = MFALSE;
#else
    handle->reassoc_on = MTRUE;
#endif

    /* Initialize the timer for the reassociation */
    woal_initialize_timer(&handle->reassoc_timer,
                          woal_reassoc_timer_func, handle);
    handle->is_reassoc_timer_set = MFALSE;
#endif /* REASSOCIATION */
    /* Register to MLAN */
    memset(&device, 0, sizeof(mlan_device));
    device.pmoal_handle = handle;

    //device.int_mode = (t_u32) intmode;
    //device.gpio_pin = (t_u32) gpiopin;

#ifdef MFG_CMD_SUPPORT
    device.mfg_mode = (t_u32) mfg_mode;
#endif
    for (i = 0; i < handle->drv_mode->intf_num; i++) {
        device.bss_attr[i].bss_type = handle->drv_mode->bss_attr[i].bss_type;
        device.bss_attr[i].frame_type =
            handle->drv_mode->bss_attr[i].frame_type;
        device.bss_attr[i].active = handle->drv_mode->bss_attr[i].active;
        device.bss_attr[i].bss_priority =
            handle->drv_mode->bss_attr[i].bss_priority;
    }
    memcpy(&device.callbacks, &woal_callbacks, sizeof(mlan_callbacks));
    if (MLAN_STATUS_SUCCESS == mlan_register(&device, &pmlan))
        handle->pmlan_adapter = pmlan;
    else
        ret = MLAN_STATUS_FAILURE;

    LEAVE();
    return ret;
}

/** 
 *  @brief This function frees the structure of moal_handle
 *    
 *  @param handle   A pointer to moal_handle structure
 *
 *  @return 	    N/A
 */
void
woal_free_moal_handle(moal_handle * handle)
{
    ENTER();
    if (!handle) {
        PRINTM(MERROR, "The handle is NULL.\n");
        LEAVE();
        return;
    }

    if ((handle->nl_sk) && ((handle->nl_sk)->sk_socket)) {
        sock_release((handle->nl_sk)->sk_socket);
        handle->nl_sk = NULL;
    }

    if (handle->pmlan_adapter)
        mlan_unregister(handle->pmlan_adapter);
    PRINTM(MINFO, "Free Adapter\n");
    if (handle->malloc_count || handle->lock_count) {
        PRINTM(MERROR,
               "mlan has memory leak: malloc_count=%lu lock_count=%lu\n",
               handle->malloc_count, handle->lock_count);
    }
    /* Free the moal handle itself */
    kfree(handle);
    m_handle = NULL;
    LEAVE();
}

/** 
 *  @brief This function initializes firmware
 *  
 *  @param handle  A pointer to moal_handle structure
 *
 *  @return 	   MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
woal_init_fw(moal_handle * handle)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    int err;
    mlan_fw_image fw;

    ENTER();

    memset(&fw, 0, sizeof(mlan_fw_image));

    if ((err =
         request_firmware(&handle->firmware, handle->drv_mode->fw_name,
                          handle->hotplug_device)) < 0) {
        PRINTM(MFATAL, "request_firmware() failed, error code = %#x\n", err);
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }
    fw.pfw_buf = (t_u8 *) handle->firmware->data;
    fw.fw_len = handle->firmware->size;

    ret = mlan_dnld_fw(handle->pmlan_adapter, &fw);
    if (ret == MLAN_STATUS_FAILURE)
        goto done;
    PRINTM(MMSG, "WLAN FW is active\n");
#if 0
    ret = woal_sdio_enable_host_int(handle);
    if (ret != MLAN_STATUS_SUCCESS)
    {
	 PRINTM(MMSG, "Enable interrupt failed\n");
         goto done;
    }
#endif
    handle->hardware_status = HardwareStatusFwReady;
    /* enable host interrupt from SDIO after fw dnld is successful */

    handle->init_wait_q_woken = MFALSE;
    ret = mlan_init_fw(handle->pmlan_adapter);
    if (ret == MLAN_STATUS_FAILURE) {
        goto done;
    } else if (ret == MLAN_STATUS_SUCCESS) {
        handle->hardware_status = HardwareStatusReady;
        goto done;
    }
#ifdef CONFIG_MV88DE3010_BERLIN
    woal_sdio_release_host(handle);
#endif
    /* Wait for mlan_init to complete */
    wait_event_interruptible(handle->init_wait_q, handle->init_wait_q_woken);
    if (handle->hardware_status != HardwareStatusReady) {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }
    ret = MLAN_STATUS_SUCCESS;

  done:
    if (handle->firmware)
        release_firmware(handle->firmware);

    if (ret != MLAN_STATUS_SUCCESS) {
        ret = MLAN_STATUS_FAILURE;
        /* Disable interrupts from the card */
        woal_sdio_disable_host_int(handle);
    }

    LEAVE();
    return ret;
}

/** 
 *  @brief This function will fill in the mlan_buffer
 *  
 *  @param pmbuf   A pointer to mlan_buffer
 *  @param skb     A pointer to struct sk_buff 
 *
 *  @return        N/A
 */
static void
woal_fill_mlan_buffer(mlan_buffer * pmbuf, struct sk_buff *skb)
{
    struct ethhdr *eth = NULL;
    struct timeval tstamp;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,22)
    struct iphdr *iph;
#endif
    t_u8 tid = 0;

    ENTER();

    eth = (struct ethhdr *) skb->data;
    switch (eth->h_proto) {
    case __constant_htons(ETH_P_IP):
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,22)
        iph = ip_hdr(skb);
        tid = IPTOS_PREC(iph->tos);
#else
        tid = IPTOS_PREC(skb->nh.iph->tos);
#endif
        PRINTM(MDATA, "packet type ETH_P_IP: %04x, tid=%#x prio=%#x\n",
               eth->h_proto, tid, skb->priority);
        break;
    case __constant_htons(ETH_P_ARP):
        PRINTM(MDATA, "ARP packet %04x\n", eth->h_proto);
    default:
        break;
    }
/** Offset for TOS field in the IP header */
#define IPTOS_OFFSET 5
    skb->priority = tid = (tid >> IPTOS_OFFSET);
    /* Record the current time the packet was queued; used to determine the
       amount of time the packet was queued in the driver before it was sent to 
       the firmware.  The delay is then sent along with the packet to the
       firmware for aggregate delay calculation for stats and MSDU lifetime
       expiry. */
    do_gettimeofday(&tstamp);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,22)
    skb->tstamp = timeval_to_ktime(tstamp);
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,14)
    skb_set_timestamp(skb, &tstamp);
#else
    memcpy(&skb->stamp, &tstamp, sizeof(skb->stamp));
#endif
    pmbuf->pdesc = skb;
    pmbuf->pbuf = skb->head + sizeof(mlan_buffer);
    pmbuf->data_offset = skb->data - (skb->head + sizeof(mlan_buffer));
    pmbuf->data_len = skb->len;
    pmbuf->priority = skb->priority;
    pmbuf->in_ts_sec = (t_u32) tstamp.tv_sec;
    pmbuf->in_ts_usec = (t_u32) tstamp.tv_usec;

    LEAVE();
    return;
}

/** 
 *  @brief This function opens the network device
 *  
 *  @param dev     A pointer to net_device structure
 *
 *  @return        0 --success, otherwise fail
 */
static int
woal_open(struct net_device *dev)
{
    moal_private *priv = (moal_private *) netdev_priv(dev);
    t_u8 carrier_on = MFALSE;

    ENTER();

    /* On some systems the device open handler will be called before HW ready.
       Use the following flag check and wait function to work around the issue. */
    {
        int i = 0;

        while ((priv->phandle->hardware_status != HardwareStatusReady) &&
               (i < MAX_WAIT_DEVICE_READY_COUNT)) {
            i++;
            woal_sched_timeout(100);
        }
        if (i >= MAX_WAIT_DEVICE_READY_COUNT) {
            PRINTM(MFATAL, "HW not ready, wlan_open() return failure\n");
            LEAVE();
            return -EFAULT;
        }
    }
    if (!MODULE_GET) {
        LEAVE();
        return -EFAULT;
    }
    if ((priv->bss_type == MLAN_BSS_TYPE_STA) &&
        (priv->media_connected || priv->is_adhoc_link_sensed))
        carrier_on = MTRUE;
    if (carrier_on == MTRUE) {
        netif_carrier_on(priv->netdev);
        if (netif_queue_stopped(priv->netdev))
            netif_wake_queue(priv->netdev);
    } else
        netif_carrier_off(priv->netdev);
    woal_request_open(priv);

    LEAVE();
    return 0;
}

/** 
 *  @brief This function closes the network device
 *  
 *  @param dev     A pointer to net_device structure
 *
 *  @return        0
 */
static int
woal_close(struct net_device *dev)
{
    moal_private *priv = (moal_private *) netdev_priv(dev);

    ENTER();

    woal_request_close(priv);
    MODULE_PUT;

    LEAVE();
    return 0;
}

/** 
 *  @brief This function handles packet transmission
 *  
 *  @param skb     A pointer to sk_buff structure
 *  @param dev     A pointer to net_device structure
 *
 *  @return        0 --success, otherwise fail
 */
static int
woal_hard_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
    moal_private *priv = (moal_private *) netdev_priv(dev);
    mlan_buffer *pmbuf = NULL;
    mlan_status status;
    struct sk_buff *new_skb = NULL;

    ENTER();

    PRINTM(MDATA, "%lu BSS(%d): Data <= kernel\n", jiffies, priv->bss_num);

    if (priv->phandle->surprise_removed == MTRUE) {
        kfree(skb);
        priv->stats.tx_dropped++;
        goto done;
    }
    if (!skb->len || (skb->len > ETH_FRAME_LEN)) {
        PRINTM(MERROR, "Tx Error: Bad skb length %d : %d\n",
               skb->len, ETH_FRAME_LEN);
        kfree(skb);
        priv->stats.tx_dropped++;
        goto done;
    }
    if (skb_headroom(skb) < (MLAN_MIN_DATA_HEADER_LEN + sizeof(mlan_buffer))) {
        PRINTM(MWARN, "Tx: Insufficient skb headroom %d\n", skb_headroom(skb));
        /* Insufficient skb headroom - allocate a new skb */
        new_skb =
            skb_realloc_headroom(skb,
                                 MLAN_MIN_DATA_HEADER_LEN +
                                 sizeof(mlan_buffer));
        if (unlikely(!new_skb)) {
            PRINTM(MERROR, "Tx: Cannot allocate skb\n");
            kfree(skb);
            priv->stats.tx_dropped++;
            goto done;
        }
        kfree_skb(skb);
        skb = new_skb;
        PRINTM(MINFO, "new skb headroom %d\n", skb_headroom(skb));
    }
    pmbuf = (mlan_buffer *) skb->head;
    pmbuf->bss_num = priv->bss_num;
    woal_fill_mlan_buffer(pmbuf, skb);
    status = mlan_send_packet(priv->phandle->pmlan_adapter, pmbuf);
    switch (status) {
    case MLAN_STATUS_PENDING:
        atomic_inc(&priv->phandle->tx_pending);
        if (atomic_read(&priv->phandle->tx_pending) >= MAX_TX_PENDING) {
            netif_stop_queue(priv->netdev);
            dev->trans_start = jiffies;
        }
        queue_work(priv->phandle->workqueue, &priv->phandle->main_work);
        break;
    case MLAN_STATUS_SUCCESS:
        priv->stats.tx_packets++;
        priv->stats.tx_bytes += skb->len;
        woal_free_mlan_buffer(pmbuf);
        break;
    case MLAN_STATUS_FAILURE:
    default:
        priv->stats.tx_dropped++;
        woal_free_mlan_buffer(pmbuf);
        break;
    }
  done:
    LEAVE();
    return 0;
}

/** 
 *  @brief This function sets the MAC address to firmware.
 *  
 *  @param dev     A pointer to mlan_private structure
 *  @param addr    MAC address to set
 *
 *  @return        0 --success, otherwise fail
 */
static int
woal_set_mac_address(struct net_device *dev, void *addr)
{
    int ret = 0;
    moal_private *priv = (moal_private *) netdev_priv(dev);
    struct sockaddr *phw_addr = (struct sockaddr *) addr;

    ENTER();

    memset(priv->current_addr, 0, ETH_ALEN);
    /* dev->dev_addr is 6 bytes */
    HEXDUMP("dev->dev_addr:", dev->dev_addr, ETH_ALEN);

    HEXDUMP("addr:", (t_u8 *) phw_addr->sa_data, ETH_ALEN);
    memcpy(priv->current_addr, phw_addr->sa_data, ETH_ALEN);
    if (MLAN_STATUS_SUCCESS != woal_request_set_mac_address(priv)) {
        PRINTM(MERROR, "Set MAC address failed\n");
        ret = -EFAULT;
        goto done;
    }
    HEXDUMP("priv->MacAddr:", priv->current_addr, ETH_ALEN);
    memcpy(dev->dev_addr, priv->current_addr, ETH_ALEN);
  done:
    LEAVE();
    return ret;
}

/** 
 *  @brief This function sets multicast addresses to firmware
 *  
 *  @param dev     A pointer to net_device structure
 *
 *  @return        N/A
 */
static void
woal_set_multicast_list(struct net_device *dev)
{
    moal_private *priv = (moal_private *) netdev_priv(dev);
    ENTER();
    woal_request_set_multicast_list(priv, dev);
    LEAVE();
}

/** 
 *  @brief This function handles the timeout of packet
 *  		transmission
 *  
 *  @param dev     A pointer to net_device structure
 *
 *  @return        N/A
 */
static void
woal_tx_timeout(struct net_device *dev)
{
    moal_private *priv = (moal_private *) netdev_priv(dev);
    ENTER();
    PRINTM(MERROR, "%lu : Tx timeout, bss_num=%d\n", jiffies, priv->bss_num);
    dev->trans_start = jiffies;
    priv->num_tx_timeout++;
    LEAVE();
}

/** 
 *  @brief This function returns the network statistics
 *  
 *  @param dev     A pointer to net_device structure
 *
 *  @return        A pointer to net_device_stats structure
 */
static struct net_device_stats *
woal_get_stats(struct net_device *dev)
{
    moal_private *priv = (moal_private *) netdev_priv(dev);
    return &priv->stats;
}

/**
 *  @brief This function initializes the private structure 
 *  		and dev structure for station mode
 *  
 *  @param dev     A pointer to net_device structure
 *  @param priv    A pointer to moal_private structure
 *
 *  @return 	   MLAN_STATUS_SUCCESS 
 */
mlan_status
woal_init_sta_dev(struct net_device * dev, moal_private * priv)
{
    ENTER();
    /* Setup the OS Interface to our functions */
    dev->open = woal_open;
    dev->hard_start_xmit = woal_hard_start_xmit;
    dev->stop = woal_close;
    dev->do_ioctl = woal_do_ioctl;
    dev->set_mac_address = woal_set_mac_address;
    dev->tx_timeout = woal_tx_timeout;
    dev->get_stats = woal_get_stats;
    dev->watchdog_timeo = MRVDRV_DEFAULT_WATCHDOG_TIMEOUT;
    dev->hard_header_len += MLAN_MIN_DATA_HEADER_LEN + sizeof(mlan_buffer);
#ifdef  WIRELESS_EXT
#if WIRELESS_EXT < 21
    dev->get_wireless_stats = woal_get_wireless_stats;
#endif
    dev->wireless_handlers = (struct iw_handler_def *) &woal_handler_def;
#endif
/** Netif dynamic alloc */
#define NETIF_F_DYNALLOC 16
    dev->features |= NETIF_F_DYNALLOC;
    dev->flags |= IFF_BROADCAST | IFF_MULTICAST;
    dev->set_multicast_list = woal_set_multicast_list;

    /* Initialize private structure */
    init_waitqueue_head(&priv->ioctl_wait_q);
    init_waitqueue_head(&priv->cmd_wait_q);
    init_waitqueue_head(&priv->proc_wait_q);
    init_waitqueue_head(&priv->w_stats_wait_q);
    priv->current_key_index = 0;
    priv->rate_index = AUTO_RATE;
    priv->media_connected = MFALSE;
    priv->is_adhoc_link_sensed = MFALSE;
    memset(&priv->current_addr, 0, sizeof(priv->current_addr));
    memset(&priv->nick_name, 0, sizeof(priv->nick_name));
    priv->num_tx_timeout = 0;
    woal_request_get_fw_info(priv);
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 * @brief This function adds a new interface. It will
 * 		allocate, initialize and register the device.
 *  
 *  @param handle    A pointer to moal_handle structure
 *  @param bss_num   BSS number (0-7)
 *  @param bss_type  BSS type
 *
 *  @return          A pointer to the new priv structure
 */
moal_private *
woal_add_interface(moal_handle * handle, t_u8 bss_num, t_u8 bss_type)
{
    struct net_device *dev = NULL;
    moal_private *priv = NULL;

    ENTER();

    /* Allocate an Ethernet device */
    if (!(dev = alloc_etherdev(sizeof(moal_private)))) {
        PRINTM(MFATAL, "Init virtual ethernet device failed!\n");
        goto error;
    }
    /* Allocate device name */
    if ((bss_type == MLAN_BSS_TYPE_STA) && (dev_alloc_name(dev, "mlan%d") < 0)) {
        PRINTM(MERROR, "Could not allocate device name!\n");
        goto error;
    }
    priv = (moal_private *) netdev_priv(dev);
    /* Save the priv to handle */
    bss_num &= BSS_NUM_MASK;
    handle->priv[bss_num] = priv;

    /* Use the same handle structure */
    priv->phandle = handle;
    priv->netdev = dev;
    priv->bss_num = bss_num;
    priv->bss_type = bss_type;

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24)
    SET_MODULE_OWNER(dev);
#endif
    if (bss_type == MLAN_BSS_TYPE_STA)
        woal_init_sta_dev(dev, priv);

    /* Register network device */
    if (register_netdev(dev)) {
        PRINTM(MERROR, "Cannot register virtual network device!\n");
        goto error;
    }

    PRINTM(MINFO, "%s: Marvell 802.11 Adapter\n", dev->name);
#ifdef CONFIG_PROC_FS
    woal_create_proc_entry(priv);
#ifdef PROC_DEBUG
    woal_debug_entry(priv);
#endif /* PROC_DEBUG */
#endif /* CONFIG_PROC_FS */
    LEAVE();
    return priv;
  error:
    if (dev)
        free_netdev(dev);
    LEAVE();
    return NULL;
}

/** 
 *  @brief This function removes an interface.
 *  
 *  @param handle   A pointer to the moal_handle structure
 *  @param bss_num  BSS number
 *
 *  @return        N/A
 */
void
woal_remove_interface(moal_handle * handle, t_u8 bss_num)
{
    struct net_device *dev = NULL;
    moal_private *priv = handle->priv[bss_num];
    union iwreq_data wrqu;

    ENTER();
    if (!priv)
        goto error;
    dev = priv->netdev;

    if (priv->media_connected == MTRUE) {
        priv->media_connected = MFALSE;
        if (priv->bss_type == MLAN_BSS_TYPE_STA) {
            memset(wrqu.ap_addr.sa_data, 0xaa, ETH_ALEN);
            wrqu.ap_addr.sa_family = ARPHRD_ETHER;
            wireless_send_event(priv->netdev, SIOCGIWAP, &wrqu, NULL);
        }
    }
#ifdef CONFIG_PROC_FS
#ifdef PROC_DEBUG
    /* Remove proc debug */
    woal_debug_remove(priv);
#endif /* PROC_DEBUG */
    woal_proc_remove(priv);
#endif /* CONFIG_PROC_FS */
    /* Last reference is our one */
    PRINTM(MINFO, "refcnt = %d\n", atomic_read(&dev->refcnt));

    PRINTM(MINFO, "netdev_finish_unregister: %s%s\n", dev->name,
           (dev->features & NETIF_F_DYNALLOC) ? "" : ", old style");

    if (dev->reg_state == NETREG_REGISTERED)
        unregister_netdev(dev);

    /* Clear the priv in handle */
    priv->phandle->priv[priv->bss_num] = NULL;
    priv->phandle = NULL;
    priv->netdev = NULL;
    free_netdev(dev);
  error:
    LEAVE();
    return;
}

/** 
 *  @brief Send FW shutdown command to MLAN
 *   
 *  @param priv          A pointer to moal_private structure
 *  @param wait_option   Wait option
 *
 *  @return              MLAN_STATUS_SUCCESS -- success, otherwise fail
 */
mlan_status
woal_shutdown_fw(moal_private * priv, t_u8 wait_option)
{
    mlan_ioctl_req *req = NULL;
    mlan_ds_misc_cfg *misc = NULL;
    mlan_status status;

    ENTER();

    /* Allocate an IOCTL request buffer */
    req =
        (mlan_ioctl_req *) woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_misc_cfg));
    if (req == NULL) {
        status = MLAN_STATUS_FAILURE;
        goto done;
    }

    /* Fill request buffer */
    misc = (mlan_ds_misc_cfg *) req->pbuf;
    misc->sub_command = MLAN_OID_MISC_INIT_SHUTDOWN;
    misc->param.func_init_shutdown = MLAN_FUNC_SHUTDOWN;
    req->req_id = MLAN_IOCTL_MISC_CFG;
    req->action = MLAN_ACT_SET;

    /* Send IOCTL request to MLAN */
    status = woal_request_ioctl(priv, req, wait_option);

  done:
    if (req)
        kfree(req);
    LEAVE();
    return status;
}

/********************************************************
		Global Functions
********************************************************/
/**
 *  @brief This function initializes the private structure
 *  		and set default value to the member of moal_private.
 *  
 *  @param priv    A pointer to moal_private structure
 *
 *  @return 	   N/A
 */
void
woal_init_priv(moal_private * priv)
{
    ENTER();
    if (priv->bss_type == MLAN_BSS_TYPE_STA) {
        priv->current_key_index = 0;
        priv->rate_index = AUTO_RATE;
        priv->media_connected = MFALSE;
        priv->is_adhoc_link_sensed = MFALSE;
        memset(&priv->current_addr, 0, sizeof(priv->current_addr));
        memset(&priv->nick_name, 0, sizeof(priv->nick_name));
        priv->num_tx_timeout = 0;
        woal_request_get_fw_info(priv);
    }
    LEAVE();
    return;
}

/** 
 *  @brief This function return the point to structure moal_private 
 *  
 *  @param handle   Pointer to structure moal_handle
 *  @param bss_num  BSS number
 *
 *  @return         moal_private pointer or NULL
 */
moal_private *
woal_bss_num_to_priv(moal_handle * handle, t_u8 bss_num)
{
    ENTER();
    if (!handle || (bss_num >= MLAN_MAX_BSS_NUM)) {
        LEAVE();
        return NULL;
    }
    LEAVE();
    return handle->priv[bss_num];
}

/** 
 *  @brief This function alloc mlan_buffer.
 *
 *  @param size	   buffer size to allocate
 *
 *  @return        mlan_buffer pointer or NULL
 */
pmlan_buffer
woal_alloc_mlan_buffer(int size)
{
    mlan_buffer *pmbuf = NULL;
    struct sk_buff *skb;

    ENTER();
    if (!(pmbuf = kzalloc(sizeof(mlan_buffer), GFP_ATOMIC))) {
        PRINTM(MERROR, "%s: Fail to alloc mlan buffer", __FUNCTION__);
        return NULL;
    }
    if (!(skb = dev_alloc_skb(size))) {
        kfree(pmbuf);
        return NULL;
    }
    pmbuf->pdesc = (t_void *) skb;
    pmbuf->pbuf = (t_u8 *) skb->tail;
    LEAVE();
    return pmbuf;
}

/** 
 *  @brief This function alloc mlan_ioctl_req.
 *
 *  @param size	   buffer size to allocate
 *
 *  @return        mlan_ioctl_req pointer or NULL
 */
pmlan_ioctl_req
woal_alloc_mlan_ioctl_req(int size)
{
    mlan_ioctl_req *req = NULL;

    ENTER();

    if (!
        (req =
         (mlan_ioctl_req *)
         kzalloc((sizeof(mlan_ioctl_req) + size + sizeof(int) +
                  sizeof(wait_queue)), GFP_ATOMIC))) {
        PRINTM(MERROR, "%s: Fail to alloc ioctl buffer", __FUNCTION__);
        LEAVE();
        return NULL;
    }
    req->pbuf = (t_u8 *) req + sizeof(mlan_ioctl_req);
    req->buf_len = (t_u32) size;
    req->reserved_1 =
        ALIGN_ADDR((t_u8 *) req + sizeof(mlan_ioctl_req) + size, sizeof(int));

    LEAVE();
    return req;
}

/** 
 *  @brief This function frees mlan_buffer.
 *
 *  @param pmbuf   Pointer to mlan_buffer
 *
 *  @return        N/A
 */
void
woal_free_mlan_buffer(pmlan_buffer pmbuf)
{
    ENTER();
    if (!pmbuf)
        return;
    if (pmbuf->pdesc)
        dev_kfree_skb_any((struct sk_buff *) pmbuf->pdesc);
    kfree(pmbuf);
    LEAVE();
    return;
}

/** 
 *  @brief This function handles events generated by firmware
 *  
 *  @param priv    A pointer to moal_private structure
 *  @param payload A pointer to payload buffer
 *  @param len	   Length of the payload
 *  @return 	   MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
woal_broadcast_event(moal_private * priv, t_u8 * payload, t_u32 len)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    struct sk_buff *skb = NULL;
    struct nlmsghdr *nlh = NULL;
    moal_handle *handle = priv->phandle;
    struct sock *sk = handle->nl_sk;

    ENTER();
    if (len > NL_MAX_PAYLOAD) {
        PRINTM(MERROR, "event size is too big!!! len=%d\n", (int) len);
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }
    if (sk) {
        /* Allocate skb */
        if (!(skb = alloc_skb(NLMSG_SPACE(NL_MAX_PAYLOAD), GFP_ATOMIC))) {
            PRINTM(MERROR, "Could not allocate skb for netlink.\n");
            ret = MLAN_STATUS_FAILURE;
            goto done;
        }
        nlh = (struct nlmsghdr *) skb->data;
        nlh->nlmsg_len = NLMSG_SPACE(len);

        /* From kernel */
        nlh->nlmsg_pid = 0;
        nlh->nlmsg_flags = 0;

        /* Data */
        skb_put(skb, nlh->nlmsg_len);
        memcpy(NLMSG_DATA(nlh), payload, len);

        /* From Kernel */
        NETLINK_CB(skb).pid = 0;

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20)
        /* Multicast message */
        NETLINK_CB(skb).dst_pid = 0;
#endif

        /* Multicast group number */
        NETLINK_CB(skb).dst_group = NL_MULTICAST_GROUP;

        /* Send message */
        netlink_broadcast(sk, skb, 0, NL_MULTICAST_GROUP, GFP_KERNEL);

        ret = MLAN_STATUS_SUCCESS;
    } else {
        PRINTM(MERROR, "Could not send event through NETLINK. Link down.\n");
        ret = MLAN_STATUS_FAILURE;
    }
  done:
    LEAVE();
    return ret;
}

#ifdef REASSOCIATION
/**
 *  @brief This function handles re-association. it is triggered
 *  by re-assoc timer.
 *
 *  @param data    A pointer to wlan_thread structure
 *  @return        MLAN_STATUS_SUCCESS
 */
int
woal_reassociation_thread(void *data)
{
    moal_thread *pmoal_thread = data;
    moal_private *priv = NULL;
    moal_handle *handle = (moal_handle *) pmoal_thread->handle;
    wait_queue_t wait;
    int i;
    BOOLEAN reassoc_timer_req;
    mlan_802_11_ssid req_ssid;
    mlan_ssid_bssid ssid_bssid;
    mlan_status status;
    mlan_bss_info bss_info;

    ENTER();

    woal_activate_thread(pmoal_thread);
    init_waitqueue_entry(&wait, current);

    current->flags |= PF_NOFREEZE;

    for (;;) {
        add_wait_queue(&pmoal_thread->wait_q, &wait);
        set_current_state(TASK_INTERRUPTIBLE);

        schedule();

        set_current_state(TASK_RUNNING);
        remove_wait_queue(&pmoal_thread->wait_q, &wait);

        /* Cancel re-association timer */
        if (handle->is_reassoc_timer_set == MTRUE) {
            woal_cancel_timer(&handle->reassoc_timer);
            handle->is_reassoc_timer_set = MFALSE;
        }

        if (handle->surprise_removed)
            break;
        if (kthread_should_stop())
            break;

        if (handle->hardware_status != HardwareStatusReady) {
            PRINTM(MINFO, "Reassoc: Hardware status is not correct\n");
            continue;
        }
        PRINTM(MINFO, "Reassoc: Thread waking up...\n");
        reassoc_timer_req = MFALSE;

        for (i = 0; i < handle->priv_num && (priv = handle->priv[i]); i++) {
            if (priv->reassoc_required == MFALSE)
                continue;
            memset(&bss_info, 0, sizeof(bss_info));
            if (MLAN_STATUS_SUCCESS !=
                woal_get_bss_info(priv, MOAL_CMD_WAIT, &bss_info)) {
                PRINTM(MINFO, "Ressoc: Fail to get bss info\n");
                priv->reassoc_required = MFALSE;
                continue;
            }
            if (bss_info.bss_mode != MLAN_BSS_MODE_INFRA ||
                priv->media_connected != MFALSE) {
                PRINTM(MINFO, "Reassoc: ad-hoc mode or media connected\n");
                priv->reassoc_required = MFALSE;
                continue;
            }

            /* The semaphore is used to avoid reassociation thread and
               wlan_set_scan/wlan_set_essid interrupting each other.
               Reassociation should be disabled completely by application if
               wlan_set_user_scan_ioctl/wlan_set_wap is used. */
            if (MOAL_ACQ_SEMAPHORE_BLOCK(&handle->reassoc_sem)) {
                PRINTM(MERROR,
                       "Acquire semaphore error, reassociation thread\n");
                reassoc_timer_req = MTRUE;
                break;
            }
            PRINTM(MINFO, "Reassoc: Required ESSID: %s\n",
                   priv->prev_ssid_bssid.ssid.ssid);
            PRINTM(MINFO, "Reassoc: Performing Active Scan\n");

            memset(&req_ssid, 0, sizeof(mlan_802_11_ssid));
            memcpy(&req_ssid, &priv->prev_ssid_bssid.ssid,
                   sizeof(mlan_802_11_ssid));

            /* Do specific SSID scanning */
            if (MLAN_STATUS_SUCCESS !=
                woal_request_scan(priv, MOAL_CMD_WAIT, &req_ssid)) {
                PRINTM(MERROR, "Reassoc: Fail to do specific scan\n");
                reassoc_timer_req = MTRUE;
                MOAL_REL_SEMAPHORE(&handle->reassoc_sem);
                break;
            }

            if (handle->surprise_removed) {
                MOAL_REL_SEMAPHORE(&handle->reassoc_sem);
                break;
            }
            status =
                woal_set_ewpa_mode(priv, MOAL_CMD_WAIT, &priv->prev_ssid_bssid);
            /* Search AP by BSSID first */
            PRINTM(MINFO, "Reassoc: Search AP by BSSID first\n");
            memset(&ssid_bssid, 0, sizeof(mlan_ssid_bssid));
            memcpy(&ssid_bssid.bssid, &priv->prev_ssid_bssid.bssid,
                   MLAN_MAC_ADDR_LENGTH);
            status = woal_find_best_network(priv, MOAL_CMD_WAIT, &ssid_bssid);
            if (MLAN_STATUS_SUCCESS != status) {
                PRINTM(MINFO, "Reassoc: AP not found in scan list\n");
                PRINTM(MINFO, "Reassoc: Search AP by SSID\n");
                /* Search AP by SSID */
                memset(&ssid_bssid, 0, sizeof(mlan_ssid_bssid));
                memcpy(&ssid_bssid.ssid, &priv->prev_ssid_bssid.ssid,
                       sizeof(mlan_802_11_ssid));
                status =
                    woal_find_best_network(priv, MOAL_CMD_WAIT, &ssid_bssid);
            }
            if (status == MLAN_STATUS_SUCCESS) {
                /* set the wep key */
                if (bss_info.wep_status)
                    woal_enable_wep_key(priv, MOAL_IOCTL_WAIT);
                /* Zero SSID implies use BSSID to connect */
                memset(&ssid_bssid.ssid, 0, sizeof(mlan_802_11_ssid));
                status = woal_bss_start(priv, MOAL_CMD_WAIT, &ssid_bssid);
            }
            if (priv->media_connected == MFALSE)
                reassoc_timer_req = MTRUE;
            else {
                mlan_ds_rate *rate = NULL;
                mlan_ioctl_req *req = NULL;

                reassoc_timer_req = MFALSE;

                if (priv->rate_index != AUTO_RATE) {
                    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_rate));

                    if (req == NULL) {
                        LEAVE();
                        return MLAN_STATUS_FAILURE;
                    }

                    rate = (mlan_ds_rate *) req->pbuf;
                    rate->param.rate_cfg.rate_type = MLAN_RATE_INDEX;
                    rate->sub_command = MLAN_OID_RATE_CFG;
                    req->req_id = MLAN_IOCTL_RATE;

                    req->action = MLAN_ACT_SET;

                    rate->param.rate_cfg.rate = priv->rate_index;

                    if (MLAN_STATUS_SUCCESS !=
                        woal_request_ioctl(priv, req, MOAL_CMD_WAIT)) {
                        kfree(req);
                        LEAVE();
                        return MLAN_STATUS_FAILURE;
                    }
                    if (req)
                        kfree(req);
                }
            }
            MOAL_REL_SEMAPHORE(&handle->reassoc_sem);
        }
        if (handle->surprise_removed)
            break;
        if (reassoc_timer_req == MTRUE) {
            PRINTM(MINFO,
                   "Reassoc: No AP found or assoc failed. Restarting re-assoc Timer.\n");
            handle->is_reassoc_timer_set = MTRUE;
            woal_mod_timer(&handle->reassoc_timer, MOAL_TIMER_10S);
        }
    }
    woal_deactivate_thread(pmoal_thread);

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function triggers re-association by waking up
 *  re-assoc thread.
 *  
 *  @param context	A pointer to context
 *  @return		n/a
 */
void
woal_reassoc_timer_func(void *context)
{
    moal_handle *handle = (moal_handle *) context;

    ENTER();

    PRINTM(MINFO, "reassoc_timer fired.\n");
    handle->is_reassoc_timer_set = MFALSE;

    PRINTM(MINFO, "Waking Up the Reassoc Thread\n");
    wake_up_interruptible(&handle->reassoc_thread.wait_q);

    LEAVE();
    return;
}
#endif /* REASSOCIATION */

/**
 *  @brief This workqueue function handles main_process
 *  
 *  @param work    A pointer to work_struct
 *
 *  @return        N/A
 */
t_void
woal_main_work_queue(struct work_struct * work)
{
    moal_handle *handle = container_of(work, moal_handle, main_work);
#ifdef CONFIG_MV88DE3010_BERLIN
    struct sdio_mmc_card * card;
#endif
    ENTER();

    if (handle->surprise_removed == MTRUE) {
        LEAVE();
        return;
    }

    card = handle->card;
#ifndef CONFIG_MV88DE3010_BERLIN
    if (atomic_read(&handle->int_flag)) {
        atomic_set(&handle->int_flag, 0);
            mlan_interrupt(handle->pmlan_adapter);
        /* unmask all interrupts */
        woal_sdio_unmask(handle);
    }
#endif
    /* Call MLAN main process */
#ifdef CONFIG_MV88DE3010_BERLIN
    sdio_claim_host(card->func);    
#endif
    mlan_main_process(handle->pmlan_adapter);
#ifdef CONFIG_MV88DE3010_BERLIN
    sdio_release_host(card->func);
#endif

    LEAVE();
}

/**
 *  @brief This function cancel all works in the queue
 *  and destroy the main workqueue.
 *  
 *  @param handle    A pointer to moal_handle
 *
 *  @return        N/A
 */
static void
woal_terminate_workqueue(moal_handle * handle)
{
    ENTER();

    flush_workqueue(handle->workqueue);
    destroy_workqueue(handle->workqueue);
    handle->workqueue = NULL;

    LEAVE();
}

void
woal_interrupt(moal_handle * handle)
{
    ENTER();

#ifdef CONFIG_MV88DE3010_BERLIN
    struct sdio_mmc_card *card;
    card = ( struct sdio_mmc_card *)handle->card;
#endif 
    PRINTM(MINTR, "*\n");
    if (handle->surprise_removed == MTRUE) {
        LEAVE();
        return;
    }
#ifndef CONFIG_MV88DE3010_BERLIN
    atomic_inc(&handle->int_flag);
    queue_work(handle->workqueue, &handle->main_work);
#else
    if(handle->mp_regs[HOST_INTSTATUS_REG])
    {
        wlan_process_int_status(handle->pmlan_adapter);
	queue_work(handle->workqueue, &handle->main_work);
    }
#endif
    LEAVE();
}

/**
 * @brief This function adds the card. it will probe the
 * 		card, allocate the mlan_private and initialize the device. 
 *  
 *  @param card    A pointer to card
 *
 *  @return        A pointer to moal_handle structure
 */
moal_handle *
woal_add_card(void *card)
{
    moal_handle *handle = NULL;
    mlan_status status = MLAN_STATUS_SUCCESS;
    int i;

    ENTER();

    if (down_interruptible(&AddRemoveCardSem))
        goto exit_sem_err;

    /* Allocate buffer for moal_handle */
    if (!(handle = kmalloc(sizeof(moal_handle), GFP_ATOMIC))) {
        PRINTM(MERROR, "Allocate buffer for moal_handle failed!\n");
        goto err_handle;
    }

    /* Init moal_handle */
    memset(handle, 0, sizeof(moal_handle));
    handle->card = card;
    m_handle = handle;

    /* Init SW */
    if (MLAN_STATUS_SUCCESS != woal_init_sw(handle)) {
        PRINTM(MFATAL, "Software Init Failed\n");
        goto err_kmalloc;
    }
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,22)
    handle->nl_sk =
        netlink_kernel_create(NETLINK_MARVELL, NL_MULTICAST_GROUP, NULL,
                              THIS_MODULE);
#else
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24)
    handle->nl_sk =
        netlink_kernel_create(NETLINK_MARVELL, NL_MULTICAST_GROUP, NULL, NULL,
                              THIS_MODULE);
#else
    handle->nl_sk =
        netlink_kernel_create(&init_net, NETLINK_MARVELL, NL_MULTICAST_GROUP,
                              NULL, NULL, THIS_MODULE);
#endif
#endif
    if (handle->nl_sk == NULL) {
        PRINTM(MERROR,
               "Could not initialize netlink event passing mechanism!\n");
        goto err_kmalloc;
    }

    /** Create workqueue */
    handle->workqueue = create_workqueue("MOAL_WORK_QUEUE");
    if (!handle->workqueue)
        goto err_kmalloc;

    MLAN_INIT_WORK(&handle->main_work, woal_main_work_queue);

#ifdef REASSOCIATION
    PRINTM(MINFO, "Starting re-association thread...\n");
    handle->reassoc_thread.handle = handle;
    woal_create_thread(woal_reassociation_thread, &handle->reassoc_thread,
                       "woal_reassoc_service");

    while (!handle->reassoc_thread.pid) {
        woal_sched_timeout(2);
    }
#endif /* REASSOCIATION */

    /* Register the device. Fill up the private data structure with relevant
       information from the card and request for the required IRQ. */
    if (woal_register_dev(handle) != MLAN_STATUS_SUCCESS) {
        PRINTM(MFATAL, "Failed to register wlan device!\n");
        goto err_registerdev;
    }

    /* Init FW and HW */
    if (MLAN_STATUS_SUCCESS != woal_init_fw(handle)) {
        PRINTM(MFATAL, "Firmware Init Failed\n");
        goto err_init_fw;
    }

#ifdef CONFIG_PROC_FS
    /* Initialize proc fs */
    woal_proc_init(handle);
#endif /* CONFIG_PROC_FS */
    /* Add interfaces */
    for (i = 0; i < handle->drv_mode->intf_num; i++) {
        if (!woal_add_interface
            (handle, handle->priv_num,
             handle->drv_mode->bss_attr[i].bss_type)) {
            status = MLAN_STATUS_FAILURE;
            break;
        }
        handle->priv_num++;
    }
    if (status != MLAN_STATUS_SUCCESS)
        goto err_add_intf;
    up(&AddRemoveCardSem);
    LEAVE();
    return handle;
  err_add_intf:
    for (i = 0; i < handle->priv_num; i++)
        woal_remove_interface(handle, i);
#ifdef CONFIG_PROC_FS
    woal_proc_exit(handle);
#endif
  err_init_fw:
    /* Unregister device */
    PRINTM(MINFO, "unregister device\n");
    woal_unregister_dev(handle);
  err_registerdev:
    handle->surprise_removed = MTRUE;
    woal_terminate_workqueue(handle);
#ifdef REASSOCIATION
    if (handle->reassoc_thread.pid) {
        wake_up_interruptible(&handle->reassoc_thread.wait_q);
    }
    /* waiting for main thread quit */
    while (handle->reassoc_thread.pid) {
        woal_sched_timeout(2);
    }
#endif /* REASSOCIATION */
  err_kmalloc:
    if ((handle->hardware_status == HardwareStatusFwReady) ||
        (handle->hardware_status == HardwareStatusReady)) {
        PRINTM(MINFO, "shutdown mlan\n");
        handle->init_wait_q_woken = MFALSE;
        status = mlan_shutdown_fw(handle->pmlan_adapter);
        if (status == MLAN_STATUS_PENDING)
            wait_event_interruptible(handle->init_wait_q,
                                     handle->init_wait_q_woken);
    }
    woal_free_moal_handle(handle);
    m_handle = NULL;
  err_handle:
    up(&AddRemoveCardSem);
  exit_sem_err:
    LEAVE();
    return NULL;
}

/** 
 *  @brief This function removes the card.
 *  
 *  @param card    A pointer to card
 *
 *  @return        MLAN_STATUS_SUCCESS
 */
mlan_status
woal_remove_card(void *card)
{
    moal_handle *handle = NULL;
    moal_private *priv = NULL;
    mlan_status status;
    int i;

    ENTER();
    if (down_interruptible(&AddRemoveCardSem))
        goto exit_sem_err;

    handle = m_handle;
    if (!handle)
        goto exit_remove;
    handle->surprise_removed = MTRUE;

    /* Stop data */
    for (i = 0; i < handle->priv_num; i++) {
        if ((priv = handle->priv[i])) {
            if (!netif_queue_stopped(priv->netdev))
                netif_stop_queue(priv->netdev);
            if (netif_carrier_ok(priv->netdev))
                netif_carrier_off(priv->netdev);
        }
    }

    /* Shutdown firmware */
    PRINTM(MCMND, "mlan_shutdown_fw.....\n");
    handle->init_wait_q_woken = MFALSE;
    status = mlan_shutdown_fw(handle->pmlan_adapter);
    if (status == MLAN_STATUS_PENDING)
        wait_event_interruptible(handle->init_wait_q,
                                 handle->init_wait_q_woken);
    PRINTM(MCMND, "mlan_shutdown_fw done!\n");
    if (atomic_read(&handle->rx_pending) || atomic_read(&handle->tx_pending) ||
        atomic_read(&handle->ioctl_pending)) {
        PRINTM(MERROR, "ERR: rx_pending=%d,tx_pending=%d,ioctl_pending=%d\n",
               atomic_read(&handle->rx_pending),
               atomic_read(&handle->tx_pending),
               atomic_read(&handle->ioctl_pending));
    }

    /* Remove interface */
    for (i = 0; i < handle->priv_num; i++)
        woal_remove_interface(handle, i);

    woal_terminate_workqueue(handle);

#ifdef REASSOCIATION
    PRINTM(MINFO, "Free reassoc_timer\n");
    if (handle->is_reassoc_timer_set) {
        woal_cancel_timer(&handle->reassoc_timer);
        handle->is_reassoc_timer_set = MFALSE;
    }
    if (handle->reassoc_thread.pid)
        wake_up_interruptible(&handle->reassoc_thread.wait_q);

    /* waiting for main thread quit */
    while (handle->reassoc_thread.pid) {
        woal_sched_timeout(2);
    }
#endif /* REASSOCIATION */

#ifdef CONFIG_PROC_FS
    woal_proc_exit(handle);
#endif
    /* Unregister device */
    PRINTM(MINFO, "unregister device\n");
    woal_unregister_dev(handle);
    /* Free adapter structure */
    PRINTM(MINFO, "Free Adapter\n");
    woal_free_moal_handle(handle);

    m_handle = NULL;
  exit_remove:
    up(&AddRemoveCardSem);
  exit_sem_err:
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function initializes module.
 *  
 *  @return        MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static int
woal_init_module(void)
{
    int ret = (int) MLAN_STATUS_SUCCESS;
    int i = 0;

    ENTER();

    /* Replace default fw image name for specific drv_mode */
    if (fw_name) {
        for (i = 0; i < (sizeof(drv_mode_tbl) / sizeof(drv_mode_tbl[0])); i++) {
            if (drv_mode_tbl[i].drv_mode == drv_mode) {
                drv_mode_tbl[i].fw_name = fw_name;
                break;
            }
        }
    }

    /* Init mutex */
    init_MUTEX(&AddRemoveCardSem);

    /* Register with bus */
    ret = (int) woal_bus_register();

    LEAVE();
    return ret;
}

/** 
 *  @brief This function cleans module
 *  
 *  @return        N/A
 */
static void
woal_cleanup_module(void)
{
    moal_handle *handle = m_handle;
    int i;
    ENTER();
    if (down_interruptible(&AddRemoveCardSem))
        goto exit_sem_err;
    if (!handle)
        goto exit;

    for (i = 0; i < handle->priv_num; i++) {
        if ((handle->priv[i]->bss_type == MLAN_BSS_TYPE_STA) &&
            (handle->priv[i]->media_connected == MTRUE)) {
            woal_disconnect(handle->priv[i], MOAL_CMD_WAIT, NULL);
        }
    }

    for (i = 0; i < handle->priv_num; i++) {
        if (handle->priv[i]) {
            woal_shutdown_fw(handle->priv[i], MOAL_CMD_WAIT);
            break;
        }
    }
    /* Disable interrupts from the card */
    woal_sdio_disable_host_int(handle);
  exit:
    up(&AddRemoveCardSem);
  exit_sem_err:
    /* Unregister from bus */
    woal_bus_unregister();
    LEAVE();
}

module_init(woal_init_module);
module_exit(woal_cleanup_module);

module_param(fw_name, charp, 0);
MODULE_PARM_DESC(fw_name, "Firmware name");
MODULE_DESCRIPTION("M-WLAN Driver");
MODULE_AUTHOR("Marvell International Ltd.");
MODULE_VERSION(MLAN_RELEASE_VERSION);
MODULE_LICENSE("GPL");
#ifdef MFG_CMD_SUPPORT
module_param(mfg_mode, int, 0);
MODULE_PARM_DESC(mfg_mode,
                 "0: Download normal firmware; 1: Download MFG firmware");
#endif /* MFG_CMD_SUPPORT */
