/** @file moal_shim.c
  *
  * @brief This file contains the callback functions registered to MLAN
  * 
  * Copyright (C) 2008-2009, Marvell International Ltd. 
  * All Rights Reserved
  */

/********************************************************
Change log:
    10/21/2008: initial version
********************************************************/

#include	"moal_main.h"
#include	"moal_usb.h"

/********************************************************
		Local Variables
********************************************************/
/** moal_lock */
typedef struct _moal_lock
{
        /** Lock */
    spinlock_t lock;
        /** Flags */
    unsigned long flags;
} moal_lock;

/********************************************************
		Global Variables
********************************************************/
extern moal_handle *m_handle;

/********************************************************
		Local Functions
********************************************************/
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
    PRINTM(INFO, "Wireless event %s is sent to application\n", str);

    LEAVE();
    return;
}
#endif

#if WIRELESS_EXT >= 18
/** 
 *  @brief This function sends mic error event to application.
 *  
 *  @param priv    A pointer to moal_private structure
 *  @param event   MIC ERROR EVENT. 
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

/********************************************************
		Global Functions
********************************************************/
/** 
 *  @brief Alloc a buffer 
 *   
 *  @param size 	The size of the buffer to be allocated
 *  @param ppbuf	Pointer to a buffer location to store buffer pointer allocated
 *
 *  @return    		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
moal_malloc(IN t_u32 size, OUT t_u8 ** ppbuf)
{
    if (!(*ppbuf = kmalloc(size, GFP_ATOMIC))) {
        PRINTM(ERROR, "%s: allocate  buffer %d failed!\n", __FUNCTION__,
               (int) size);
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }
    m_handle->malloc_count++;

    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Free a buffer 
 *   
 *  @param pbuf		Pointer to the buffer to be freed
 *
 *  @return    		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
moal_mfree(IN t_u8 * pbuf)
{

    if (!pbuf)
        return MLAN_STATUS_FAILURE;
    kfree(pbuf);
    m_handle->malloc_count--;
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Fill memory with constant byte 
 *   
 *  @param pmem		Pointer to the memory area
 *  @param byte		A constant byte
 *  @param num		Number of bytes to fill
 *
 *  @return    		Pointer to the memory area
 */
t_void *
moal_memset(IN t_void * pmem, IN t_u8 byte, IN t_u32 num)
{
    t_void *p = pmem;

    if (pmem && num)
        p = memset(pmem, byte, num);

    return p;
}

/** 
 *  @brief Copy memory from one area to another
 *   
 *  @param pdest	Pointer to the dest memory
 *  @param psrc		Pointer to the src memory
 *  @param num		Number of bytes to move
 *
 *  @return    		Pointer to the dest memory
 */
t_void *
moal_memcpy(IN t_void * pdest, IN const t_void * psrc, IN t_u32 num)
{
    t_void *p = pdest;

    if (pdest && psrc && num)
        p = memcpy(pdest, psrc, num);

    return p;
}

/** 
 *  @brief Move memory from one area to another
 *   
 *  @param pdest	Pointer to the dest memory
 *  @param psrc		Pointer to the src memory
 *  @param num		Number of bytes to move
 *
 *  @return    		Pointer to the dest memory
 */
t_void *
moal_memmove(IN t_void * pdest, IN const t_void * psrc, IN t_u32 num)
{
    t_void *p = pdest;

    if (pdest && psrc && num)
        p = memmove(pdest, psrc, num);

    return p;
}

/** 
 *  @brief Compare two memory areas
 *   
 *  @param pmem1	Pointer to the first memory
 *  @param pmem2	Pointer to the second memory
 *  @param num		Number of bytes to compare
 *
 *  @return    		Compare result returns by memcmp
 */
t_s32
moal_memcmp(IN const t_void * pmem1, IN const t_void * pmem2, IN t_u32 num)
{
    t_s32 result;

    result = memcmp(pmem1, pmem2, num);

    return result;
}

/** 
 *  @brief Retrieves the current system time
 *   
 *  @param psec		Pointer to buf for the seconds of system time
 *  @param pusec 	Pointer to buf the micro seconds of system time
 *
 *  @return    		MLAN_STATUS_SUCCESS 
 */
mlan_status
moal_get_system_time(OUT t_u32 * psec, OUT t_u32 * pusec)
{
    struct timeval t;

    do_gettimeofday(&t);
    *psec = (t_u32) t.tv_sec;
    *pusec = (t_u32) t.tv_usec;

    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Initializes the timer
 *   
 *  @param pptimer	Pointer to the timer
 *  @param callback 	Pointer to callback function
 *  @param pcontext 	Pointer to context
 *
 *  @return    		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE 
 */
mlan_status
moal_init_timer(OUT t_void ** pptimer,
                IN t_void(*callback) (t_void * pcontext), IN t_void * pcontext)
{
    moal_drv_timer *timer = NULL;

    if (!
        (timer =
         (moal_drv_timer *) kmalloc(sizeof(moal_drv_timer), GFP_ATOMIC))) {
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }
    woal_initialize_timer(timer, callback, pcontext);
    *pptimer = (t_void *) timer;

    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Free the timer
 *   
 *  @param ptimer	Pointer to the timer
 *
 *  @return    		MLAN_STATUS_SUCCESS 
 */
mlan_status
moal_free_timer(IN t_void * ptimer)
{
    moal_drv_timer *timer = (moal_drv_timer *) ptimer;

    if (timer) {
        if ((timer->timer_is_canceled == MFALSE) && timer->time_period) {
            PRINTM(ERROR, "mlan try to free timer without stop timer!\n");
            woal_cancel_timer(timer);
        }
        kfree(timer);
    }

    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Start the timer
 *   
 *  @param ptimer	Pointer to the timer
 *  @param periodic     Periodic timer
 *  @param msec		Timer value in milliseconds
 *
 *  @return    		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
moal_start_timer(IN t_void * ptimer, IN t_u8 periodic, IN t_u32 msec)
{
    if (!ptimer)
        return MLAN_STATUS_FAILURE;

    ((moal_drv_timer *) ptimer)->timer_is_periodic = periodic;
    woal_mod_timer((moal_drv_timer *) ptimer, msec);

    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Stop the timer
 *   
 *  @param ptimer	Pointer to the timer
 *
 *  @return    		MLAN_STATUS_SUCCESS 
 */
mlan_status
moal_stop_timer(IN t_void * ptimer)
{

    if (!ptimer) {
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }
    woal_cancel_timer((moal_drv_timer *) ptimer);

    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Initializes the lock
 *   
 *  @param pplock	Pointer to the lock
 *
 *  @return    		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE 
 */
mlan_status
moal_init_lock(OUT t_void ** pplock)
{
    moal_lock *mlock = NULL;

    if (!(mlock = (moal_lock *) kmalloc(sizeof(moal_lock), GFP_ATOMIC))) {
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }
    spin_lock_init(&mlock->lock);
    *pplock = (t_void *) mlock;

    m_handle->lock_count++;

    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Free the lock
 *   
 *  @param plock	Lock
 *
 *  @return    		MLAN_STATUS_SUCCESS
 */
mlan_status
moal_free_lock(IN t_void * plock)
{
    moal_lock *mlock = plock;

    if (mlock) {
        kfree(mlock);
        m_handle->lock_count--;
    }

    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Request a spin lock
 *   
 *  @param plock	Pointer to the lock
 *
 *  @return    		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
moal_spin_lock(IN t_void * plock)
{
    moal_lock *mlock = plock;

    if (mlock) {
        mlock->flags = 0;
        spin_lock_irqsave(&mlock->lock, mlock->flags);

        return MLAN_STATUS_SUCCESS;
    } else {
        return MLAN_STATUS_FAILURE;
    }
}

/** 
 *  @brief Request a spin_unlock
 *     
 *  @param plock	Pointer to the lock
 *
 *  @return    		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
moal_spin_unlock(IN t_void * plock)
{
    moal_lock *mlock = (moal_lock *) plock;

    if (mlock) {
        spin_unlock_irqrestore(&mlock->lock, mlock->flags);

        return MLAN_STATUS_SUCCESS;
    } else {
        return MLAN_STATUS_FAILURE;
    }
}

/** 
 *  @brief This function is called when MLAN completes the initialization firmware.
 *   
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param status	The status code for mlan_init_fw request
 *
 *  @return    		MLAN_STATUS_SUCCESS 
 */
mlan_status
moal_init_fw_complete(IN t_void * pmoal_handle, IN mlan_status status)
{
    moal_handle *handle = (moal_handle *) pmoal_handle;
    ENTER();
    if (status == MLAN_STATUS_SUCCESS)
        handle->hardware_status = HardwareStatusReady;
    handle->init_wait_q_woken = MTRUE;
    wake_up_interruptible(&handle->init_wait_q);
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function is called when MLAN shutdown firmware is completed.
 *   
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param status	The status code for mlan_shutdown request
 *
 *  @return    		MLAN_STATUS_SUCCESS 
 */
mlan_status
moal_shutdown_fw_complete(IN t_void * pmoal_handle, IN mlan_status status)
{
    moal_handle *handle = (moal_handle *) pmoal_handle;
    ENTER();
    handle->hardware_status = HardwareStatusNotReady;
    handle->init_wait_q_woken = MTRUE;
    wake_up_interruptible(&handle->init_wait_q);
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function is called when an MLAN IOCTL is completed.
 *   
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param pioctl_req	pointer to strutcture mlan_ioctl_req 
 *  @param status	The status code for mlan_ioctl request
 *
 *  @return    		MLAN_STATUS_SUCCESS 
 */
mlan_status
moal_ioctl_complete(IN t_void * pmoal_handle,
                    IN pmlan_ioctl_req pioctl_req, IN mlan_status status)
{
    moal_handle *handle = (moal_handle *) pmoal_handle;
    moal_private *priv = NULL;
    wait_queue *wait;
    ENTER();
    priv = woal_bss_num_to_priv(handle, pioctl_req->bss_num);
    if (priv)
        priv->ioctl_pending--;

    wait = (wait_queue *) pioctl_req->reserved_1;
    PRINTM(CMND,
           "IOCTL completed: id=0x%lx action=%d,  status=%d, status_code=0x%lx\n",
           pioctl_req->req_id, (int) pioctl_req->action, status,
           pioctl_req->status_code);
    if (wait) {
        *wait->condition = MTRUE;
        wait->status = status;
        if ((status != MLAN_STATUS_SUCCESS) &&
            (pioctl_req->status_code == MLAN_ERROR_CMD_TIMEOUT)) {
            PRINTM(ERROR, "IOCTL: command timeout\n");
        } else {
            wake_up_interruptible(wait->wait);
        }
    } else {
        if ((status == MLAN_STATUS_SUCCESS) &&
            (pioctl_req->action == MLAN_ACT_GET))
            woal_process_ioctl_resp(priv, pioctl_req);
        if (status != MLAN_STATUS_SUCCESS)
            PRINTM(ERROR,
                   "IOCTL failed: id=0x%lx, action=%d, status_code=0x%lx\n",
                   pioctl_req->req_id, (int) pioctl_req->action,
                   pioctl_req->status_code);
        kfree(pioctl_req);
    }
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function is called when MLAN complete send data packet.
 *   
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param pmbuf	Pointer to the mlan buffer structure
 *  @param status	The status code for mlan_send_packet request
 *
 *  @return    		MLAN_STATUS_SUCCESS 
 */
mlan_status
moal_send_packet_complete(IN t_void * pmoal_handle,
                          IN pmlan_buffer pmbuf, IN mlan_status status)
{
    moal_private *priv = NULL;
    struct sk_buff *skb = NULL;
    ENTER();
    if (pmbuf) {
        priv = woal_bss_num_to_priv(pmoal_handle, pmbuf->bss_num);
        skb = (struct sk_buff *) pmbuf->pdesc;
        if (priv) {
            priv->netdev->trans_start = jiffies;
            if (skb) {
                if (status == MLAN_STATUS_SUCCESS) {
                    priv->stats.tx_packets++;
                    priv->stats.tx_bytes += skb->len;
                } else {
                    priv->stats.tx_errors++;
                }
                atomic_dec(&priv->tx_pending);
                if (priv->media_connected || priv->is_adhoc_link_sensed) {
                    if (netif_queue_stopped(priv->netdev))
                        netif_wake_queue(priv->netdev);
                }
            }
        }
        if (skb)
            dev_kfree_skb_any(skb);
    }
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function is called when MLAN complete receiving 
 *  	   data/event/command
 *
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param pmbuf	Pointer to the mlan buffer structure
 *  @param port 	Port number for receive
 *  @param status	The status code for mlan_receive request
 *
 *  @return    		MLAN_STATUS_SUCCESS 
 */
mlan_status
moal_recv_complete(IN t_void * pmoal_handle,
                   IN pmlan_buffer pmbuf, IN t_u32 port, IN mlan_status status)
{
    moal_private *priv = NULL;
    moal_handle *handle = (moal_handle *) pmoal_handle;
    ENTER();
    atomic_dec(&handle->rx_pending);
    if (pmbuf) {
        priv = woal_bss_num_to_priv(pmoal_handle, pmbuf->bss_num);
        if (priv && (pmbuf->buf_type == MLAN_BUF_TYPE_DATA) &&
            (status == MLAN_STATUS_FAILURE)) {
            priv->stats.rx_dropped++;
        }
        /* Reuse the buffer in case of command/event */
        if (port == MLAN_USB_EP_CMD_EVENT)
            woal_submit_rx_urb(handle, port);
        else
            woal_free_mlan_buffer(pmbuf);
    }
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function write a command/data packet to card.
 *   
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param pmbuf	Pointer to the mlan buffer structure
 *  @param port 	Port number for sent
 *
 *  @return    		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE or MLAN_STATUS_PENDING or MLAN_STATUS_RESOURCE
 */
mlan_status
moal_write_data_async(IN t_void * pmoal_handle,
                      IN pmlan_buffer pmbuf, IN t_u32 port)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    ENTER();
    ret =
        woal_write_data_async((moal_handle *) pmoal_handle, pmbuf, (t_u8) port);
    LEAVE();
    return ret;
}

/** 
 *  @brief This function write a command/data packet to card.
 *         This function blocks the call until it finishes
 *
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param pmbuf	Pointer to the mlan buffer structure
 *  @param port 	Port number for sent
 *  @param timeout 	Timeout value
 *
 *  @return    		MLAN_STATUS_SUCCESS 
 */
mlan_status
moal_write_data_sync(IN t_void * pmoal_handle,
                     IN pmlan_buffer pmbuf, IN t_u32 port, IN t_u32 timeout)
{
    return woal_write_data_sync((moal_handle *) pmoal_handle, pmbuf, port,
                                timeout);
}

/** 
 *  @brief This function read data packet/event/command from card.
 *         This function blocks the call until it finish
 *
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param pmbuf	Pointer to the mlan buffer structure
 *  @param port 	Port number for read
 *  @param timeout 	Timeout value
 *
 *  @return    		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
moal_read_data_sync(IN t_void * pmoal_handle,
                    IN OUT pmlan_buffer pmbuf, IN t_u32 port, IN t_u32 timeout)
{
    return woal_read_data_sync((moal_handle *) pmoal_handle, pmbuf, port,
                               timeout);
}

/** 
 *  @brief This function uploads the packet to the network stack
 *
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param pmbuf	Pointer to the mlan buffer structure
 *
 *  @return    		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
moal_recv_packet(IN t_void * pmoal_handle, IN pmlan_buffer pmbuf)
{
    mlan_status status = MLAN_STATUS_SUCCESS;
    moal_private *priv = NULL;
    struct sk_buff *skb = NULL;
    ENTER();
    if (pmbuf) {
        priv = woal_bss_num_to_priv(pmoal_handle, pmbuf->bss_num);
        skb = (struct sk_buff *) pmbuf->pdesc;
        if (priv) {
            if (skb) {
                skb_reserve(skb, pmbuf->data_offset);
                skb_put(skb, pmbuf->data_len);
                pmbuf->pdesc = NULL;
                pmbuf->pbuf = NULL;
                pmbuf->data_offset = pmbuf->data_len = 0;
            } else {
                if (!(skb = dev_alloc_skb(pmbuf->data_len + MLAN_NET_IP_ALIGN))) {
                    PRINTM(ERROR, "%s fail to alloc skb", __FUNCTION__);
                    status = MLAN_STATUS_FAILURE;
                    priv->stats.rx_dropped++;
                    goto done;
                }
                skb_reserve(skb, MLAN_NET_IP_ALIGN);
                memcpy(skb->data, (t_u8 *) (pmbuf->pbuf + pmbuf->data_offset),
                       pmbuf->data_len);
                skb_put(skb, pmbuf->data_len);
            }
            skb->dev = priv->netdev;
            skb->protocol = eth_type_trans(skb, priv->netdev);
            skb->ip_summed = CHECKSUM_UNNECESSARY;
            /* This is only required only in case of 11n and USB as we alloc a
               buffer of 4K only if its 11N (to be able to receive 4K AMSDU
               packets). In case of SD we allocate buffers based on the size of 
               packet and hence this is not needed. */
            /* Modifying the truesize here as our allocation for each skb is 4K
               but we only receive 2K packets and this cause the kernel to start
               dropping packets in case where application has allocated buffer
               based on 2K size i.e. if there a 64K packet received (in IP
               fragments and application allocates 64K to receive this packet but 
               this packet would almost double up because we allocate each 1.5K
               fragment in 4K and pass it up. As soon as the 64K limit hits
               kernel will start to drop rest of the fragments. Currently we fail 
               the Filesndl-ht.scr script for UDP, hence this fix */
            if (skb->truesize > MLAN_RX_DATA_BUF_SIZE)
                skb->truesize += (skb->len - MLAN_RX_DATA_BUF_SIZE);
            priv->stats.rx_bytes += skb->len;
            priv->stats.rx_packets++;
            if (in_interrupt())
                netif_rx(skb);
            else
                netif_rx_ni(skb);
        }
    }
  done:
    LEAVE();
    return status;
}

/** 
 *  @brief This function handles event receive
 *
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param pmevent	Pointer to the mlan event structure
 *
 *  @return    		MLAN_STATUS_SUCCESS 
 */
mlan_status
moal_recv_event(IN t_void * pmoal_handle, IN pmlan_event pmevent)
{
    moal_private *priv = NULL;
    union iwreq_data wrqu;

    ENTER();

    PRINTM(EVENT, "event id:0x%x\n", pmevent->event_id);
    priv = woal_bss_num_to_priv(pmoal_handle, pmevent->bss_num);
    if (priv == NULL) {
        PRINTM(ERROR, "%s: priv is null\n", __FUNCTION__);
        goto done;
    }

    switch (pmevent->event_id) {
    case MLAN_EVENT_ID_FW_ADHOC_LINK_SENSED:
        priv->is_adhoc_link_sensed = MTRUE;
        if (!netif_carrier_ok(priv->netdev))
            netif_carrier_on(priv->netdev);
        if (netif_queue_stopped(priv->netdev))
            netif_wake_queue(priv->netdev);
        woal_send_iwevcustom_event(priv, CUS_EVT_ADHOC_LINK_SENSED);
        break;
    case MLAN_EVENT_ID_FW_ADHOC_LINK_LOST:
        if (!netif_queue_stopped(priv->netdev))
            netif_stop_queue(priv->netdev);
        if (netif_carrier_ok(priv->netdev))
            netif_carrier_off(priv->netdev);
        priv->is_adhoc_link_sensed = MFALSE;
        woal_send_iwevcustom_event(priv, CUS_EVT_ADHOC_LINK_LOST);
        break;
    case MLAN_EVENT_ID_DRV_CONNECTED:
        if (pmevent->event_len == ETH_ALEN) {
            memset(wrqu.ap_addr.sa_data, 0x00, ETH_ALEN);
            memcpy(wrqu.ap_addr.sa_data, pmevent->event_buf, ETH_ALEN);
            wrqu.ap_addr.sa_family = ARPHRD_ETHER;
            wireless_send_event(priv->netdev, SIOCGIWAP, &wrqu, NULL);
        }
        priv->media_connected = MTRUE;
        if (!netif_carrier_ok(priv->netdev))
            netif_carrier_on(priv->netdev);
        if (netif_queue_stopped(priv->netdev))
            netif_wake_queue(priv->netdev);
        break;
    case MLAN_EVENT_ID_DRV_OBSS_SCAN_PARAM:
        memset(&wrqu, 0, sizeof(union iwreq_data));
        memmove((pmevent->event_buf + strlen(CUS_EVT_OBSS_SCAN_PARAM) + 1),
                pmevent->event_buf, pmevent->event_len);
        memcpy(pmevent->event_buf, (t_u8 *) CUS_EVT_OBSS_SCAN_PARAM,
               strlen(CUS_EVT_OBSS_SCAN_PARAM));
        pmevent->event_buf[strlen(CUS_EVT_OBSS_SCAN_PARAM)] = 0;

        wrqu.data.pointer = pmevent->event_buf;
        wrqu.data.length =
            pmevent->event_len + strlen(CUS_EVT_OBSS_SCAN_PARAM) +
            IW_EV_LCP_LEN + 1;
        wireless_send_event(priv->netdev, IWEVCUSTOM, &wrqu,
                            pmevent->event_buf);
        break;
    case MLAN_EVENT_ID_FW_BW_CHANGED:
        memset(&wrqu, 0, sizeof(union iwreq_data));
        memmove((pmevent->event_buf + strlen(CUS_EVT_BW_CHANGED) + 1),
                pmevent->event_buf, pmevent->event_len);
        memcpy(pmevent->event_buf, (t_u8 *) CUS_EVT_BW_CHANGED,
               strlen(CUS_EVT_BW_CHANGED));
        pmevent->event_buf[strlen(CUS_EVT_BW_CHANGED)] = 0;

        wrqu.data.pointer = pmevent->event_buf;
        wrqu.data.length =
            pmevent->event_len + strlen(CUS_EVT_BW_CHANGED) + IW_EV_LCP_LEN + 1;
        wireless_send_event(priv->netdev, IWEVCUSTOM, &wrqu,
                            pmevent->event_buf);
        break;
    case MLAN_EVENT_ID_FW_DISCONNECTED:
        priv->media_connected = MFALSE;
        if (!netif_queue_stopped(priv->netdev))
            netif_stop_queue(priv->netdev);
        if (netif_carrier_ok(priv->netdev))
            netif_carrier_off(priv->netdev);
        memset(wrqu.ap_addr.sa_data, 0x00, ETH_ALEN);
        wrqu.ap_addr.sa_family = ARPHRD_ETHER;
        wireless_send_event(priv->netdev, SIOCGIWAP, &wrqu, NULL);
        /* Reset wireless stats signal info */
        priv->w_stats.qual.level = 0;
        priv->w_stats.qual.noise = 0;
#ifdef REASSOCIATION
        if (priv->phandle->reassoc_on == MTRUE) {
            PRINTM(INFO, "Reassoc: trigger the timer\n");
            priv->reassoc_required = MTRUE;
            priv->phandle->is_reassoc_timer_set = MTRUE;
            woal_mod_timer(&priv->phandle->reassoc_timer, 50);
        } else {
            priv->rate_index = AUTO_RATE;
        }
#endif /* REASSOCIATION */
        break;
    case MLAN_EVENT_ID_FW_MIC_ERR_UNI:
#if WIRELESS_EXT >= 18
        woal_send_mic_error_event(priv, MLAN_EVENT_ID_FW_MIC_ERR_UNI);
#else
        woal_send_iwevcustom_event(priv, CUS_EVT_MLME_MIC_ERR_UNI);
#endif
        break;
    case MLAN_EVENT_ID_FW_MIC_ERR_MUL:
#if WIRELESS_EXT >= 18
        woal_send_mic_error_event(priv, MLAN_EVENT_ID_FW_MIC_ERR_MUL);
#else
        woal_send_iwevcustom_event(priv, CUS_EVT_MLME_FW_MIC_ERR_UNI);
#endif
        break;
    case MLAN_EVENT_ID_FW_BCN_RSSI_LOW:
        woal_send_iwevcustom_event(priv, CUS_EVT_BEACON_RSSI_LOW);
        break;
    case MLAN_EVENT_ID_FW_BCN_RSSI_HIGH:
        woal_send_iwevcustom_event(priv, CUS_EVT_BEACON_RSSI_HIGH);
        break;
    case MLAN_EVENT_ID_FW_BCN_SNR_LOW:
        woal_send_iwevcustom_event(priv, CUS_EVT_BEACON_SNR_LOW);
        break;
    case MLAN_EVENT_ID_FW_BCN_SNR_HIGH:
        woal_send_iwevcustom_event(priv, CUS_EVT_BEACON_SNR_HIGH);
        break;
    case MLAN_EVENT_ID_FW_MAX_FAIL:
        woal_send_iwevcustom_event(priv, CUS_EVT_MAX_FAIL);
        break;
    case MLAN_EVENT_ID_FW_DATA_RSSI_LOW:
        woal_send_iwevcustom_event(priv, CUS_EVT_DATA_RSSI_LOW);
        break;
    case MLAN_EVENT_ID_FW_DATA_SNR_LOW:
        woal_send_iwevcustom_event(priv, CUS_EVT_DATA_SNR_LOW);
        break;
    case MLAN_EVENT_ID_FW_DATA_RSSI_HIGH:
        woal_send_iwevcustom_event(priv, CUS_EVT_DATA_RSSI_HIGH);
        break;
    case MLAN_EVENT_ID_FW_DATA_SNR_HIGH:
        woal_send_iwevcustom_event(priv, CUS_EVT_DATA_SNR_HIGH);
        break;
    case MLAN_EVENT_ID_FW_LINK_QUALITY:
        woal_send_iwevcustom_event(priv, CUS_EVT_LINK_QUALITY);
        break;
    case MLAN_EVENT_ID_FW_PORT_RELEASE:
        woal_send_iwevcustom_event(priv, CUS_EVT_PORT_RELEASE);
        break;
    case MLAN_EVENT_ID_FW_PRE_BCN_LOST:
        woal_send_iwevcustom_event(priv, CUS_EVT_PRE_BEACON_LOST);
        break;
    case MLAN_EVENT_ID_FW_HS_WAKEUP:
        woal_send_iwevcustom_event(priv, CUS_EVT_HS_WAKEUP);
        /* simulate HSCFG_CANCEL command */
        woal_hs_cfg_cancel(priv);
        break;
    case MLAN_EVENT_ID_FW_WMM_CONFIG_CHANGE:
        woal_send_iwevcustom_event(priv, WMM_CONFIG_CHANGE_INDICATION);
        break;
    case MLAN_EVENT_ID_FW_HEART_BEAT:
        woal_send_iwevcustom_event(priv, CUS_EVT_HEART_BEAT);
        break;
    case MLAN_EVENT_ID_FW_WEP_ICV_ERR:
        DBG_HEXDUMP(CMD_D, "WEP ICV error", pmevent->event_buf,
                    pmevent->event_len);
        woal_send_iwevcustom_event(priv, CUS_EVT_WEP_ICV_ERR);
        break;

    case MLAN_EVENT_ID_DRV_DEFER_HANDLING:
        mlan_main_process(priv->phandle->pmlan_adapter);
        break;
    case MLAN_EVENT_ID_DRV_HS_ACTIVATED:
        woal_send_iwevcustom_event(priv, CUS_EVT_HS_ACTIVATED);
        break;
    case MLAN_EVENT_ID_DRV_HS_DEACTIVATED:
        woal_send_iwevcustom_event(priv, CUS_EVT_HS_DEACTIVATED);
        break;
    default:
        break;
    }

  done:
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function prints the debug message in mlan
 *
 *  @param level	debug level
 *  @param pformat	point to string format buf
 *
 *  @return    		N/A 
 */
t_void
moal_print(IN t_u32 level, IN t_s8 * pformat, IN ...)
{
#ifdef	DEBUG_LEVEL1
    va_list args;

    if (level & DBG_HEX_DUMP) {
        t_u8 *buf = NULL;
        int len = 0;

        va_start(args, pformat);
        buf = (t_u8 *) va_arg(args, t_u8 *);
        len = (int) va_arg(args, int);
        va_end(args);

#ifdef DEBUG_LEVEL2
        if (level & DBG_INFO)
            HEXDUMP((char *) pformat, buf, len);
        else
#endif /* DEBUG_LEVEL2 */
        {
            if (level & DBG_CMD_D)
                DBG_HEXDUMP(CMD_D, (char *) pformat, buf, len);
            if (level & DBG_DAT_D)
                DBG_HEXDUMP(DAT_D, (char *) pformat, buf, len);
            if (level & DBG_IF_D)
                DBG_HEXDUMP(IF_D, (char *) pformat, buf, len);
            if (level & DBG_FW_D)
                DBG_HEXDUMP(FW_D, (char *) pformat, buf, len);
        }
    } else if (drvdbg & level) {
        va_start(args, pformat);
#ifndef DEBUG_LEVEL2
        if (!(level & (DBG_INFO | DBG_WARN | DBG_ENTRY)))
#endif /* DEBUG_LEVEL2 */
            vprintk(pformat, args);
        va_end(args);
    }
#endif /* DEBUG_LEVEL1 */
}
