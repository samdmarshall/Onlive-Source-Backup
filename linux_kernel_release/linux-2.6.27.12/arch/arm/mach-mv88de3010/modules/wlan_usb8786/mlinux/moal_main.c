/** @file moal_main.c
  *
  * @brief This file contains the major functions in WLAN
  * driver. 
  * 
  * Copyright (C) 2008-2009, Marvell International Ltd. 
  * All Rights Reserved
  */

/********************************************************
Change log:
    10/21/2008: initial version
********************************************************/

#include	"moal_main.h"
#include 	"moal_usb.h"

/********************************************************
		Local Variables
********************************************************/
/** Driver version */
const char driver_version[] =
    "USB8786-%s-" MLAN_RELEASE_VERSION "-(" "FP" FPNUM ")"
#ifdef	DEBUG_LEVEL2
    "-dbg"
#endif
    " ";

/** Firmware name */
char *fw_name = NULL;

#ifdef MFG_CMD_SUPPORT
/** Mfg mode */
int mfgmode = 0;
#endif

/** woal_callbacks */
static mlan_callbacks woal_callbacks = {
    .moal_init_fw_complete = moal_init_fw_complete,
    .moal_shutdown_fw_complete = moal_shutdown_fw_complete,
    .moal_send_packet_complete = moal_send_packet_complete,
    .moal_recv_complete = moal_recv_complete,
    .moal_recv_packet = moal_recv_packet,
    .moal_recv_event = moal_recv_event,
    .moal_ioctl_complete = moal_ioctl_complete,
    .moal_write_data_async = moal_write_data_async,
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
};

/** BSS attributes */
static mlan_bss_attr woal_bss[] = {
    {MLAN_BSS_TYPE_STA, MLAN_DATA_FRAME_TYPE_ETH_II},
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
#define	DEFAULT_DEBUG_MASK	(0xffffffff & ~DBG_EVENT)
#else
#define DEFAULT_DEBUG_MASK	(DBG_MSG | DBG_FATAL | DBG_ERROR)
#endif
t_u32 drvdbg = DEFAULT_DEBUG_MASK;
t_u32 ifdbg = 0;
#endif
/********************************************************
		Local Functions
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
    priv->ioctl_pending = 0;
    priv->num_tx_timeout = 0;
    LEAVE();
    return;
}

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
    int bss_num = sizeof(woal_bss) / sizeof(woal_bss[0]);

    ENTER();

    /* Initialize moal_handle structure */
    handle->hardware_status = HardwareStatusInitializing;
    /* PnP and power profile */
    handle->surprise_removed = MFALSE;
    init_waitqueue_head(&handle->init_wait_q);

#ifdef REASSOCIATION
    MOAL_INIT_SEMAPHORE(&handle->reassoc_sem);

#if (WIRELESS_EXT >= 18)

#ifndef CONFIG_MV88DE3010_BERLIN
    handle->reassoc_on = MFALSE;
#else
    handle->reassoc_on = MTRUE;
#endif

#else
    handle->reassoc_on = MTRUE;
#endif

    /* Initialize the timer for the reassociation */
    woal_initialize_timer(&handle->reassoc_timer,
                          woal_reassoc_timer_func, handle);
    handle->is_reassoc_timer_set = MFALSE;
#endif /* REASSOCIATION */

    /* Initialize private structure */
    woal_init_priv(handle->priv[0]);

    /* Register to MLAN */
    memset(&device, 0, sizeof(mlan_device));
    device.pmoal_handle = handle;
#ifdef MFG_CMD_SUPPORT
    device.mfgmode = (t_u32) mfgmode;
#endif
    for (i = 0; i < bss_num; i++) {
        device.bss_attr[i].bss_type = woal_bss[i].bss_type;
        device.bss_attr[i].frame_type = woal_bss[i].frame_type;
        device.bss_attr[i].active = MTRUE;
        device.bss_attr[i].bss_priority = 0;
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
        PRINTM(ERROR, "The handle is NULL.\n");
        LEAVE();
        return;
    }
    if (handle->pmlan_adapter)
        mlan_unregister(handle->pmlan_adapter);
    PRINTM(INFO, "Free Adapter\n");
    if (handle->malloc_count || handle->lock_count) {
        PRINTM(ERROR, "mlan has memory leak: malloc_count=%lu lock_count=%lu\n",
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
         request_firmware(&handle->firmware, fw_name,
                          handle->hotplug_device)) < 0) {
        PRINTM(FATAL, "request_firmware() failed, error code = %#x\n", err);
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }
    fw.pfw_buf = (t_u8 *) handle->firmware->data;
    fw.fw_len = handle->firmware->size;

    ret = mlan_dnld_fw(handle->pmlan_adapter, &fw);
    if (ret == MLAN_STATUS_FAILURE)
        goto done;
    PRINTM(MSG, "WLAN FW is active\n");
    handle->hardware_status = HardwareStatusFwReady;
    woal_usb_rx_init(handle);
    if (ret == MLAN_STATUS_SUCCESS)
        ret = woal_usb_tx_init(handle);
    handle->init_wait_q_woken = MFALSE;
    ret = mlan_init_fw(handle->pmlan_adapter);
    if (ret == MLAN_STATUS_FAILURE) {
        goto done;
    } else if (ret == MLAN_STATUS_SUCCESS) {
        handle->hardware_status = HardwareStatusReady;
        goto done;
    }
    /* Wait for mlan_init to complete */
    wait_event_interruptible(handle->init_wait_q, handle->init_wait_q_woken);
    if (handle->hardware_status != HardwareStatusReady) {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }
    ret = MLAN_STATUS_SUCCESS;
    woal_request_get_fw_info(handle->priv[0]);
  done:
    if (handle->firmware)
        release_firmware(handle->firmware);
    if (ret != MLAN_STATUS_SUCCESS) {
        ret = MLAN_STATUS_FAILURE;
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
        PRINTM(DATA, "packet type ETH_P_IP: %04x, tid=%#x prio=%#x\n",
               eth->h_proto, tid, skb->priority);
        break;
    case __constant_htons(ETH_P_ARP):
        PRINTM(DATA, "ARP packet %04x\n", eth->h_proto);
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
            PRINTM(FATAL, "HW not ready, wlan_open() return failure\n");
            LEAVE();
            return -EFAULT;
        }
    }
    if (!MODULE_GET) {
        LEAVE();
        return -EFAULT;
    }
    if (priv->media_connected || priv->is_adhoc_link_sensed) {
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
    PRINTM(DATA, "%lu : Data <= kernel\n", jiffies);
    if (priv->phandle->surprise_removed == MTRUE) {
        kfree(skb);
        priv->stats.tx_dropped++;
        goto done;
    }
    if (!skb->len || (skb->len > ETH_FRAME_LEN)) {
        PRINTM(ERROR, "Tx Error: Bad skb length %d : %d\n",
               skb->len, ETH_FRAME_LEN);
        kfree(skb);
        priv->stats.tx_dropped++;
        goto done;
    }
    if (skb_headroom(skb) < (MLAN_MIN_DATA_HEADER_LEN + sizeof(mlan_buffer))) {
        PRINTM(WARN, "Tx: Insufficient skb headroom %d\n", skb_headroom(skb));
        /* Insufficient skb headroom - allocate a new skb */
        new_skb =
            skb_realloc_headroom(skb,
                                 MLAN_MIN_DATA_HEADER_LEN +
                                 sizeof(mlan_buffer));
        if (unlikely(!new_skb)) {
            PRINTM(ERROR, "Tx: Cannot allocate skb\n");
            kfree(skb);
            priv->stats.tx_dropped++;
            goto done;
        }
        kfree_skb(skb);
        skb = new_skb;
        PRINTM(INFO, "new skb headroom %d\n", skb_headroom(skb));
    }
    pmbuf = (mlan_buffer *) skb->head;
    pmbuf->bss_num = priv->bss_num;
    woal_fill_mlan_buffer(pmbuf, skb);
    status = mlan_send_packet(priv->phandle->pmlan_adapter, pmbuf);
    switch (status) {
    case MLAN_STATUS_PENDING:
        atomic_inc(&priv->tx_pending);
        if (atomic_read(&priv->tx_pending) >= MAX_TX_PENDING) {
            netif_stop_queue(priv->netdev);
            dev->trans_start = jiffies;
        }
        mlan_main_process(priv->phandle->pmlan_adapter);
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
        PRINTM(ERROR, "Set MAC address failed\n");
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
    PRINTM(ERROR, "%lu : Tx timeout\n", jiffies);
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

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,29)
/** Network device handlers */
static const struct net_device_ops woal_netdev_ops = {
    .ndo_open = woal_open,
    .ndo_start_xmit = woal_hard_start_xmit,
    .ndo_stop = woal_close,
    .ndo_do_ioctl = woal_do_ioctl,
    .ndo_set_mac_address = woal_set_mac_address,
    .ndo_tx_timeout = woal_tx_timeout,
    .ndo_get_stats = woal_get_stats,
    .ndo_set_multicast_list = woal_set_multicast_list,
};
#endif

/********************************************************
		Global Functions
********************************************************/
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
        PRINTM(ERROR, "%s: Fail to alloc mlan buffer", __FUNCTION__);
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
                  sizeof(wait_queue)), GFP_ATOMIC)))
        return NULL;
    req->pbuf = (t_u8 *) req + sizeof(mlan_ioctl_req);
    req->buf_len = (t_u32) size;
    req->reserved_1 =
        ALIGN_ADDR((t_u8 *) req + sizeof(mlan_ioctl_req) + size, sizeof(int));

    LEAVE();
    return req;
}

/** 
 *  @brief This function alloc mlan_ioctl_req without wait.
 *
 *  @param size	   buffer size to allocate
 *
 *  @return        mlan_ioctl_req pointer or NULL
 */
pmlan_ioctl_req
woal_alloc_mlan_ioctl_req_no_wait(int size)
{
    mlan_ioctl_req *req = NULL;

    ENTER();

    if (!
        (req =
         (mlan_ioctl_req *)
         kzalloc((sizeof(mlan_ioctl_req) + size + sizeof(int) +
                  sizeof(wait_queue)), GFP_ATOMIC))) {
        PRINTM(ERROR, "%s: Fail to alloc ioctl buffer", __FUNCTION__);
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
            PRINTM(INFO, "Reassoc: Hardware status is not correct\n");
            continue;
        }
        PRINTM(INFO, "Reassoc: Thread waking up...\n");
        reassoc_timer_req = MFALSE;

        for (i = 0; i <= handle->priv_num && (priv = handle->priv[i]); i++) {
            if (priv->reassoc_required == MFALSE)
                continue;
            memset(&bss_info, 0, sizeof(bss_info));
            if (MLAN_STATUS_SUCCESS !=
                woal_get_bss_info(priv, MOAL_CMD_WAIT, &bss_info)) {
                PRINTM(INFO, "Ressoc: Fail to get bss info\n");
                priv->reassoc_required = MFALSE;
                continue;
            }
            if (bss_info.bss_mode != MLAN_BSS_MODE_INFRA ||
                priv->media_connected != MFALSE) {
                PRINTM(INFO, "Reassoc: ad-hoc mode or media connected\n");
                priv->reassoc_required = MFALSE;
                continue;
            }

            /* The semaphore is used to avoid reassociation thread and
               wlan_set_scan/wlan_set_essid interrupting each other.
               Reassociation should be disabled completely by application if
               wlan_set_user_scan_ioctl/wlan_set_wap is used. */
            if (MOAL_ACQ_SEMAPHORE_BLOCK(&handle->reassoc_sem)) {
                PRINTM(ERROR,
                       "Acquire semaphore error, reassociation thread\n");
                reassoc_timer_req = MTRUE;
                break;
            }
            PRINTM(INFO, "Reassoc: Required ESSID: %s\n",
                   priv->prev_ssid_bssid.ssid.ssid);
            PRINTM(INFO, "Reassoc: Performing Active Scan\n");

            memset(&req_ssid, 0, sizeof(mlan_802_11_ssid));
            memcpy(&req_ssid, &priv->prev_ssid_bssid.ssid,
                   sizeof(mlan_802_11_ssid));

            /* Do specific SSID scanning */
            if (MLAN_STATUS_SUCCESS !=
                woal_request_scan(priv, MOAL_CMD_WAIT, &req_ssid)) {
                PRINTM(ERROR, "Reassoc: Fail to do specific scan\n");
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
            PRINTM(INFO, "Reassoc: Search AP by BSSID first\n");
            memset(&ssid_bssid, 0, sizeof(mlan_ssid_bssid));
            memcpy(&ssid_bssid.bssid, &priv->prev_ssid_bssid.bssid,
                   MLAN_MAC_ADDR_LENGTH);
            status = woal_find_best_network(priv, MOAL_CMD_WAIT, &ssid_bssid);
            if (MLAN_STATUS_SUCCESS != status) {
                PRINTM(INFO, "Reassoc: AP not found in scan list\n");
                PRINTM(INFO, "Reassoc: Search AP by SSID\n");
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
                status = woal_associate(priv, MOAL_CMD_WAIT, &ssid_bssid);
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
            PRINTM(INFO,
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

    PRINTM(INFO, "reassoc_timer fired.\n");
    handle->is_reassoc_timer_set = MFALSE;

    PRINTM(INFO, "Waking Up the Reassoc Thread\n");
    wake_up_interruptible(&handle->reassoc_thread.wait_q);

    LEAVE();
    return;
}
#endif /* REASSOCIATION */

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
    struct net_device *dev = NULL;
    moal_private *priv = NULL;
    moal_handle *handle = NULL;

    ENTER();

    if (down_interruptible(&AddRemoveCardSem))
        goto exit_sem_err;

    /* Allocate an Ethernet device and register it */
    if (!(dev = alloc_etherdev(sizeof(moal_private)))) {
        PRINTM(ERROR, "Init ethernet device failed!\n");
        goto err_dev;
    }
    priv = (moal_private *) netdev_priv(dev);

    /* Allocate device name */
    if (dev_alloc_name(dev, "mlan%d") < 0) {
        PRINTM(ERROR, "Could not allocate device name!\n");
        goto err_handle;
    }

    /* Allocate buffer for moal_handle */
    if (!(handle = kmalloc(sizeof(moal_handle), GFP_ATOMIC))) {
        PRINTM(ERROR, "Allocate buffer for moal_handle failed!\n");
        goto err_handle;
    }

    /* Init moal_handle */
    memset(handle, 0, sizeof(moal_handle));
    handle->card = card;

    /* Save the priv to adapter */
    handle->priv[handle->priv_num] = priv;

    priv->phandle = handle;
    priv->netdev = dev;
    priv->bss_num = handle->priv_num;
    priv->bss_type = MLAN_BSS_TYPE_STA;
    m_handle = handle;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24)
    SET_MODULE_OWNER(dev);
#endif

    /* Setup the OS Interface to our functions */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,29)
    dev->open = woal_open;
    dev->hard_start_xmit = woal_hard_start_xmit;
    dev->stop = woal_close;
    dev->do_ioctl = woal_do_ioctl;
    dev->set_mac_address = woal_set_mac_address;
    dev->tx_timeout = woal_tx_timeout;
    dev->get_stats = woal_get_stats;
    dev->set_multicast_list = woal_set_multicast_list;
#else
    dev->netdev_ops = &woal_netdev_ops;
#endif
    dev->watchdog_timeo = MRVDRV_DEFAULT_WATCHDOG_TIMEOUT;
    dev->hard_header_len += MLAN_MIN_DATA_HEADER_LEN + sizeof(mlan_buffer);
#ifdef  WIRELESS_EXT
#if WIRELESS_EXT < 21
    dev->get_wireless_stats = woal_get_wireless_stats;
#endif
    dev->wireless_handlers = (struct iw_handler_def *) &woal_handler_def;
#endif
#define NETIF_F_DYNALLOC 16
    dev->features |= NETIF_F_DYNALLOC;
    dev->flags |= IFF_BROADCAST | IFF_MULTICAST;

    /* Init SW */
    if (woal_init_sw(handle)) {
        PRINTM(FATAL, "Software Init Failed\n");
        goto err_kmalloc;
    }

#ifdef REASSOCIATION
    PRINTM(INFO, "Starting re-association thread...\n");
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
        PRINTM(FATAL, "Failed to register wlan device!\n");
        goto err_registerdev;
    }
    SET_NETDEV_DEV(dev, handle->hotplug_device);

    /* Init FW and HW */
    if (woal_init_fw(handle)) {
        PRINTM(FATAL, "Firmware Init Failed\n");
        goto err_init_fw;
    }

    /* Register netdevice */
    if (register_netdev(dev)) {
        PRINTM(FATAL, "Cannot register network device!\n");
        goto err_init_fw;
    }
#ifdef CONFIG_PROC_FS
    /* Initialize proc fs */
    woal_proc_init(handle);
    woal_create_proc_entry(priv);
#ifdef PROC_DEBUG
    woal_debug_entry(priv);
#endif
#endif /* CONFIG_PROC_FS */
    up(&AddRemoveCardSem);
    LEAVE();
    return handle;

  err_init_fw:
  err_registerdev:
    handle->surprise_removed = MTRUE;
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
        PRINTM(INFO, "shutdown mlan\n");
        handle->init_wait_q_woken = MFALSE;
        if (MLAN_STATUS_PENDING == mlan_shutdown_fw(handle->pmlan_adapter)) {
            wait_event_interruptible(handle->init_wait_q,
                                     handle->init_wait_q_woken);
        }
    }
    woal_free_moal_handle(handle);
  err_handle:
    priv->netdev = NULL;
    free_netdev(dev);
  err_dev:
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
    struct net_device *dev = NULL;
    union iwreq_data wrqu;

    ENTER();
    if (down_interruptible(&AddRemoveCardSem))
        goto exit_sem_err;

    handle = m_handle;
    if (!handle || !handle->priv[0])
        goto exit_remove;
    priv = handle->priv[0];
    handle->surprise_removed = MTRUE;

    if (!netif_queue_stopped(priv->netdev))
        netif_stop_queue(priv->netdev);
    if (netif_carrier_ok(priv->netdev))
        netif_carrier_off(priv->netdev);
    /* Shutdown firmware */
    handle->init_wait_q_woken = MFALSE;
    status = mlan_shutdown_fw(handle->pmlan_adapter);
    if (status == MLAN_STATUS_PENDING) {
        wait_event_interruptible(handle->init_wait_q,
                                 handle->init_wait_q_woken);
    }
    if (atomic_read(&handle->rx_pending) || atomic_read(&priv->tx_pending) ||
        priv->ioctl_pending) {
        PRINTM(ERROR, "ERR: rx_pending=%d,tx_pending=%d,ioctl_pending=%lu\n",
               atomic_read(&handle->rx_pending), atomic_read(&priv->tx_pending),
               priv->ioctl_pending);
    }
    if (priv->media_connected == MTRUE) {
        priv->media_connected = MFALSE;
        memset(wrqu.ap_addr.sa_data, 0xaa, ETH_ALEN);
        wrqu.ap_addr.sa_family = ARPHRD_ETHER;
        wireless_send_event(priv->netdev, SIOCGIWAP, &wrqu, NULL);
    }
    dev = priv->netdev;
    /* Last reference is our one */
    PRINTM(INFO, "refcnt = %d\n", atomic_read(&dev->refcnt));
    PRINTM(INFO, "netdev_finish_unregister: %s%s\n", dev->name,
           (dev->features & NETIF_F_DYNALLOC) ? "" : ", old style");

    /* Unregister netdev */
    if (dev->reg_state == NETREG_REGISTERED)
        unregister_netdev(dev);
    PRINTM(INFO, "Unregister finish\n");

#ifdef REASSOCIATION
    PRINTM(INFO, "Free reassoc_timer\n");
    if (handle->is_reassoc_timer_set) {
        woal_cancel_timer(&handle->reassoc_timer);
        handle->is_reassoc_timer_set = MFALSE;
    }
    if (handle->reassoc_thread.pid) {
        wake_up_interruptible(&handle->reassoc_thread.wait_q);
    }
    /* waiting for main thread quit */
    while (handle->reassoc_thread.pid) {
        woal_sched_timeout(2);
    }
#endif /* REASSOCIATION */

#ifdef CONFIG_PROC_FS
#ifdef PROC_DEBUG
    /* Remove proc debug */
    woal_debug_remove(priv);
#endif
    woal_proc_remove(priv);
    woal_proc_exit(handle);
#endif
    /* Free adapter structure */
    PRINTM(INFO, "Free Adapter\n");
    woal_free_moal_handle(handle);

    /* Free netdevice structure */
    priv->netdev = NULL;
    free_netdev(dev);
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
    ENTER();

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
    for (i = 0; i <= handle->priv_num; i++) {
        if (handle->priv[i]->media_connected == MTRUE) {
            woal_disconnect(handle->priv[i], NULL);
        }
    }
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
module_param(mfgmode, int, 0);
MODULE_PARM_DESC(mfgmode,
                 "0: Download normal firmware; 1: Download MFG firmware");
#endif /* MFG_CMD_SUPPORT */
