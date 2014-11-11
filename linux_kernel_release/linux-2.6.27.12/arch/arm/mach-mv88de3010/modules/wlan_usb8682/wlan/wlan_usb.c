/** @file wlan_usb.c
  * @brief This file contains functions used in USB interface module.
  *  
  * Copyright (C) 2003-2008, Marvell International Ltd.
  * All Rights Reserved
  */
/************************************************************************
Change log:
	09/29/05: Add Doxygen format comments
	02/01/06: Correct the usage of compile flag HELPER_IMAGE
	02/13/06: Add a FW ready flag for interoperability issue
	02/22/06: Add support for new VID/PID
	03/02/06: Add receive exception handling. Fix some memory leak issues
	03/03/06: Add FW download failure handling
	03/10/06: Fix the rmmod/insmod issue
	04/07/06: Add Linux kernel 2.6 support
	04/21/06: Add hotplug firmware loading
	06/02/06: Improve the card event handling code
	06/28/06: Add Boot command and Boot2/FW update support
	12/24/07: Separate FW download related code to wlan_usb_fw.c
	01/31/08: Tx data enhancement, remove extra copy, support multi-send
	
************************************************************************/

#include	"wlan_headers.h"
#include	"wlan_usb.h"
#include	<linux/firmware.h>

/********************************************************
		Local Variables
********************************************************/
/** Marvell USB device */
#define MARVELL_USB_DEVICE(vid, pid, name) \
           USB_DEVICE(vid, pid),  \
           .driver_info = (u32)name

/** Length of the message header in bytes */
#define MESSAGE_HEADER_LEN	4
/** Tx buffer length */
#define USB_TX_BUF_LEN    	(MAX(MRVDRV_ETH_TX_PACKET_BUFFER_SIZE, \
				MRVDRV_SIZE_OF_CMD_BUFFER) + MESSAGE_HEADER_LEN)
/** Rx buffer length */
#define USB_RX_BUF_LEN    	(MAX(MRVDRV_ETH_RX_PACKET_BUFFER_SIZE, \
				MRVDRV_SIZE_OF_CMD_BUFFER) + MESSAGE_HEADER_LEN)
/** Minimum length of Rx data */
#define USB_MIN_RX_DATA_LEN	(sizeof(RxPD) + MESSAGE_HEADER_LEN)

/** Name of the USB driver */
const char usbdriver_name[] = "usb8xxx";

/** struct This structure contains the device signature */
struct usb_device_id if_usb_table[] = {

    /* Enter the device signature inside */
    {MARVELL_USB_DEVICE
     (USB8682_VID_1, USB8682_PID_1, "Marvell WLAN USB Adapter")},
    {MARVELL_USB_DEVICE
     (USB8682_VID_2, USB8682_PID_2, "Marvell WLAN USB Adapter")},
    /* Terminating entry */
    {},
};

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,21)
static void if_usb_receive(struct urb *urb, struct pt_regs *regs);
#else
static void if_usb_receive(struct urb *urb);
#endif
static int if_usb_probe(struct usb_interface *intf,
                        const struct usb_device_id *id);
static void if_usb_disconnect(struct usb_interface *intf);
static int if_usb_suspend(struct usb_interface *intf, pm_message_t message);
static int if_usb_resume(struct usb_interface *intf);

/** Tx data URB context */
struct tx_data_context
{
    /** Pointer to wlan_private structure */
    wlan_private *priv;
    /** Pointer to current skb (if any) */
    struct sk_buff *skb;
};

/** if_usb_driver */
static struct usb_driver if_usb_driver = {
    /* driver name */
    .name = usbdriver_name,

    /* probe function name */
    .probe = if_usb_probe,

    /* disconnect function name */
    .disconnect = if_usb_disconnect,

    /* device signature table */
    .id_table = if_usb_table,

    .suspend = if_usb_suspend,

    .resume = if_usb_resume,
};

MODULE_DEVICE_TABLE(usb, if_usb_table);

/********************************************************
		Global Variables
********************************************************/

/** Default firmaware name */
#define DEFAULT_FW_NAME	"mrvl/usb8682.bin"
/** USB8682 firmware for B0 chip */
#define USB8682_B0_FW_NAME "mrvl/usb8682_B0.bin"
/** USB8682 firmware for B1 chip */
#define USB8682_B1_FW_NAME "mrvl/usb8682_B1.bin"
extern u8 *fw_name;

/********************************************************
		Local Functions
********************************************************/

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,21)
/** 
 *  @brief  call back function to handle the status of the Tx data URB
 *  @param urb 		pointer to urb structure
 *  @param regs		registers
 *  @return 	   	N/A
 */
static void
if_usb_tx_data_complete(struct urb *urb, struct pt_regs *regs)
#else
/** 
 *  @brief  call back function to handle the status of the Tx data URB
 *  @param urb 		pointer to urb structure
 *  @return 	   	N/A
 */
static void
if_usb_tx_data_complete(struct urb *urb)
#endif
{
    struct tx_data_context *context = (struct tx_data_context *) (urb->context);
    wlan_private *priv = context->priv;
    struct sk_buff *skb = context->skb;
    struct usb_card_rec *cardp = priv->wlan_dev.card;

    ENTER();

    /* Decrease pending URB counter */
    atomic_dec(&cardp->tx_data_urb_pending);

    /* Handle the transmission complete validations */
    if (urb->status != 0) {
        PRINTM(ERROR, "Tx data URB in failure status: %d\n", urb->status);
    } else {
        PRINTM(INFO, "Actual length transmitted %d\n", urb->actual_length);
        priv->wlan_dev.data_sent = FALSE;
    }

    cardp->usb_int_cause |= HIS_TxDnLdRdy;
    if (priv->adapter->PSState != PS_STATE_SLEEP)
        wlan_interrupt(priv->wlan_dev.netdev);

    /* Free skb */
    if (skb)
        dev_kfree_skb_any(skb);

    /* Free context */
    kfree(context);

    /* Free Tx data URB */
    usb_free_urb(urb);

    LEAVE();
    return;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,21)
/** 
 *  @brief  call back function to handle the status of the Tx command URB
 *  @param urb 		pointer to urb structure
 *  @param regs		registers
 *  @return 	   	N/A
 */
static void
if_usb_tx_cmd_complete(struct urb *urb, struct pt_regs *regs)
#else
/** 
 *  @brief  call back function to handle the status of the Tx command URB
 *  @param urb 		pointer to urb structure
 *  @return 	   	N/A
 */
static void
if_usb_tx_cmd_complete(struct urb *urb)
#endif
{
    wlan_private *priv = (wlan_private *) (urb->context);
    struct usb_card_rec *cardp = priv->wlan_dev.card;

    ENTER();

    /* Decrease pending URB counter */
    atomic_dec(&cardp->tx_cmd_urb_pending);

    /* Handle the transmission complete validations */
    if (urb->status != 0) {
        PRINTM(ERROR, "Tx command URB in failure status: %d\n", urb->status);
    } else {
        PRINTM(INFO, "Actual length transmitted %d\n", urb->actual_length);
        priv->wlan_dev.cmd_sent = FALSE;
    }

    cardp->usb_int_cause |= HIS_TxDnLdRdy;
    if (priv->adapter->PSState != PS_STATE_SLEEP)
        wlan_interrupt(priv->wlan_dev.netdev);

    /* Free Tx command URB */
    usb_free_urb(urb);

    LEAVE();
    return;
}

/** 
 *  @brief  free tx/rx urb, skb and rx buffer
 *  @param cardp	pointer usb_card_rec
 *  @return 	   	N/A
 */
void
if_usb_free(struct usb_card_rec *cardp)
{
    ENTER();

    /* Unlink Rx data URB */
    if (cardp->rx_data_urb && atomic_read(&cardp->rx_data_urb_pending)) {
        usb_kill_urb(cardp->rx_data_urb);
        os_sched_timeout(100);
    }

    /* Free Rx data URB */
    if (cardp->rx_data_urb) {
        usb_free_urb(cardp->rx_data_urb);
        cardp->rx_data_urb = NULL;
    }

    /* Free Rx data skb */
    if (cardp->skb) {
        kfree_skb(cardp->skb);
        cardp->skb = NULL;
    }

    /* Free Tx command buffer */
    if (cardp->tx_cmd_buffer) {
        kfree(cardp->tx_cmd_buffer);
        cardp->tx_cmd_buffer = NULL;
    }

    LEAVE();
    return;
}

/** 
 *  @brief sets the configuration values
 *  @param intf		pointer to usb_interface
 *  @param id		pointer to usb_device_id
 *  @return 	   	address of variable usb_cardp 
 */
static int
if_usb_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
    struct usb_device *udev;
    struct usb_host_interface *iface_desc;
    struct usb_endpoint_descriptor *endpoint;
    int i;
    struct usb_card_rec *usb_cardp = NULL;

    ENTER();

    udev = interface_to_usbdev(intf);
    usb_cardp = kzalloc(sizeof(struct usb_card_rec), GFP_KERNEL);
    if (!usb_cardp) {
        LEAVE();
        return (-ENOMEM);
    }

    udev->descriptor.idVendor = wlan_cpu_to_le16(udev->descriptor.idVendor);
    udev->descriptor.idProduct = wlan_cpu_to_le16(udev->descriptor.idProduct);
    udev->descriptor.bcdDevice = wlan_cpu_to_le16(udev->descriptor.bcdDevice);
    udev->descriptor.bcdUSB = wlan_cpu_to_le16(udev->descriptor.bcdUSB);

    /* Check probe is for our device */
    for (i = 0; if_usb_table[i].idVendor; i++) {
        if (udev->descriptor.idVendor == if_usb_table[i].idVendor &&
            udev->descriptor.idProduct == if_usb_table[i].idProduct) {
            PRINTM(MSG, "VID/PID = %X/%X, Boot2 version = %X\n",
                   udev->descriptor.idVendor, udev->descriptor.idProduct,
                   udev->descriptor.bcdDevice);
            break;
        }
    }

    if (if_usb_table[i].idVendor) {

        usb_cardp->udev = udev;
        iface_desc = intf->cur_altsetting;

        PRINTM(INFO, "bcdUSB = 0x%X bDeviceClass = 0x%X"
               " bDeviceSubClass = 0x%X, bDeviceProtocol = 0x%X\n",
               udev->descriptor.bcdUSB,
               udev->descriptor.bDeviceClass,
               udev->descriptor.bDeviceSubClass,
               udev->descriptor.bDeviceProtocol);

        for (i = 0; i < iface_desc->desc.bNumEndpoints; ++i) {
            endpoint = &iface_desc->endpoint[i].desc;
            if ((endpoint->bEndpointAddress & USB_ENDPOINT_DIR_MASK) &&
                ((endpoint->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) ==
                 USB_ENDPOINT_XFER_BULK)) {
                /* We found a bulk in endpoint */
                PRINTM(INFO, "Bulk IN: max packet size = %d, address = %d\n",
                       wlan_le16_to_cpu(endpoint->wMaxPacketSize),
                       endpoint->bEndpointAddress);
                // if (!(usb_cardp->rx_data_urb = usb_alloc_urb(0, GFP_KERNEL))) {
                if (!(usb_cardp->rx_data_urb = usb_alloc_urb(0, GFP_ATOMIC))) {
                        PRINTM(ERROR, "Rx URB allocation failed\n");
                    goto error_txrx;
                }
                usb_cardp->bulk_in_size =
                    wlan_le16_to_cpu(endpoint->wMaxPacketSize);
                usb_cardp->rx_data_endpointAddr =
                    (endpoint->bEndpointAddress & USB_ENDPOINT_NUMBER_MASK);
                atomic_set(&usb_cardp->rx_data_urb_pending, 0);
                usb_cardp->rx_data_urb_recall = FALSE;

                usb_cardp->bulk_in_endpointAddr =
                    (endpoint->bEndpointAddress & USB_ENDPOINT_NUMBER_MASK);
            }

            if (((endpoint->bEndpointAddress & USB_ENDPOINT_DIR_MASK) == 0x00)
                && ((endpoint->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) ==
                    USB_ENDPOINT_XFER_BULK)) {
                /* We found a bulk out endpoint */
                PRINTM(INFO, "Bulk OUT: max packet size = %d, address = %d\n",
                       wlan_le16_to_cpu(endpoint->wMaxPacketSize),
                       endpoint->bEndpointAddress);
                usb_cardp->bulk_out_size =
                    wlan_le16_to_cpu(endpoint->wMaxPacketSize);
                usb_cardp->tx_data_endpointAddr = endpoint->bEndpointAddress;
                atomic_set(&usb_cardp->tx_data_urb_pending, 0);
                /* Currently we use same endpoint for Tx data and command */
                usb_cardp->tx_cmd_endpointAddr = endpoint->bEndpointAddress;
                atomic_set(&usb_cardp->tx_cmd_urb_pending, 0);

                usb_cardp->tx_cmd_buffer = kzalloc(USB_TX_BUF_LEN, GFP_DMA);
                if (!usb_cardp->tx_cmd_buffer) {
                    PRINTM(ERROR, "Could not allocate tx buffer\n");
                    goto error_txrx;
                }
                usb_cardp->bulk_out_endpointAddr = endpoint->bEndpointAddress;
            }
        }

        /* At this point wlan_add_card() will be called */
        if ((wlan_add_card(usb_cardp)) == NULL) {
            PRINTM(ERROR, "%s: wlan_add_callback failed\n", __FUNCTION__);
            goto error;
        }

        usb_get_dev(udev);
        usb_set_intfdata(intf, usb_cardp);
        LEAVE();

        return 0;
    } else {
        PRINTM(ERROR, "Discard the Probe request\n");
        PRINTM(INFO, "VID = 0x%X PID = 0x%X\n", udev->descriptor.idVendor,
               udev->descriptor.idProduct);
    }

  error_txrx:
    PRINTM(ERROR, "Transfer buffer allocating failed\n");
  error:
    if_usb_free(usb_cardp);
    kfree(usb_cardp);
    usb_cardp = NULL;

    LEAVE();
    return (-ENOMEM);
}

/** 
 *  @brief free resource and cleanup
 *  @param intf		pointer to usb_interface
 *  @return 	   	N/A
 */
static void
if_usb_disconnect(struct usb_interface *intf)
{
    struct usb_card_rec *cardp = usb_get_intfdata(intf);
    wlan_private *priv = NULL;
    wlan_adapter *Adapter = NULL;

    ENTER();

    if (!cardp) {
        PRINTM(ERROR, "Card structure is not valid\n");
        LEAVE();
        return;
    }

    priv = (wlan_private *) cardp->priv;
    if (!priv || !priv->adapter) {
        PRINTM(ERROR, "Priv or adapter structure is not valid\n");
        LEAVE();
        return;
    }
    Adapter = priv->adapter;

    /* 
     * Update Surprise removed to TRUE  
     *  Free all the URB's allocated 
     */
    if (!Adapter->SurpriseRemoved)
        Adapter->SurpriseRemoved = TRUE;

    /* Unlink and free urb */
    if_usb_free(cardp);

    /* card is removed and we can call wlan_remove_card */
    PRINTM(INFO, "Call remove card\n");
    wlan_remove_card(cardp);

    fw_name = NULL;

    usb_set_intfdata(intf, NULL);
    usb_put_dev(interface_to_usbdev(intf));
    kfree(cardp);
    LEAVE();
    return;
}

/**  
 *  @brief This function submit the rx data to OS
 *  @param priv		pointer to wlan_private structure
 *  @return 	   	WLAN_STATUS_SUCCESS or WLAN_STATUS_FAILURE
 */
int
if_usb_submit_rx_urb(wlan_private * priv)
{
    struct usb_card_rec *cardp = priv->wlan_dev.card;
    int ret = WLAN_STATUS_FAILURE;

    ENTER();

    if (!(cardp->skb = dev_alloc_skb(USB_RX_BUF_LEN))) {
        PRINTM(ERROR, "No free skb\n");
        goto rx_ret;
    }

    /* Fill the Rx URB configuration and initialize the Rx callback */
    usb_fill_bulk_urb(cardp->rx_data_urb, cardp->udev,
                      usb_rcvbulkpipe(cardp->udev, cardp->rx_data_endpointAddr),
                      cardp->skb->tail + NET_IP_ALIGN,
                      USB_RX_BUF_LEN, if_usb_receive, priv);

    if ((ret = usb_submit_urb(cardp->rx_data_urb, GFP_ATOMIC))) {
        /* Submit URB failure */
        PRINTM(ERROR, "Submit Rx URB failed: %d\n", ret);
        ret = WLAN_STATUS_FAILURE;
    } else {
        atomic_inc(&cardp->rx_data_urb_pending);
        ret = WLAN_STATUS_SUCCESS;
    }

  rx_ret:
    LEAVE();
    return ret;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,21)
/**  
 *  @brief This function reads of the packet into the upload buff, 
 *  wake up the main thread and initialize the Rx callback.
 *  
 *  @param urb		pointer to struct urb
 *  @param regs		registers
 *  @return 	   	N/A
 */
static void
if_usb_receive(struct urb *urb, struct pt_regs *regs)
#else
/**  
 *  @brief This function reads of the packet into the upload buff, 
 *  wake up the main thread and initialize the Rx callback.
 *  
 *  @param urb		pointer to struct urb
 *  @return 	   	N/A
 */
static void
if_usb_receive(struct urb *urb)
#endif
{
    wlan_private *priv = (wlan_private *) urb->context;
    struct usb_card_rec *cardp = NULL;
    int RecvLength = urb->actual_length;
    u8 *RecvBuff = NULL;
    u32 RecvType;
    u8 *cmdBuf;

    ENTER();

    if (!priv || !priv->adapter) {
        PRINTM(ERROR, "Priv or adapter structure is not valid\n");
        LEAVE();
        return;
    }

    cardp = (struct usb_card_rec *) priv->wlan_dev.card;
    if (!cardp) {
        PRINTM(ERROR, "Card structure is not valid\n");
        LEAVE();
        return;
    }

    atomic_dec(&cardp->rx_data_urb_pending);

    if (RecvLength) {
        if (urb->status) {
            PRINTM(ERROR, "URB status is failed: %d\n", urb->status);
            if (cardp->skb) {
                kfree_skb(cardp->skb);
                cardp->skb = NULL;
            }
            goto setup_for_next;
        }

        RecvBuff = cardp->skb->data + NET_IP_ALIGN;
        memcpy(&RecvType, RecvBuff, sizeof(u32));
        RecvType = wlan_le32_to_cpu(RecvType);
        PRINTM(INFO, "Receive length = 0x%x, type = 0x%x\n", RecvLength,
               RecvType);
    } else if (urb->status) {
        if (!cardp->rx_data_urb_recall) {
            PRINTM(INFO, "Card is removed: %d\n", urb->status);
            priv->adapter->SurpriseRemoved = TRUE;
            /* Wake up main thread to handle card removal. */
            wlan_interrupt(priv->wlan_dev.netdev);
        }
        if (cardp->skb) {
            kfree_skb(cardp->skb);
            cardp->skb = NULL;
        }
        goto rx_exit;
    } else {
        if (cardp->skb) {
            kfree_skb(cardp->skb);
            cardp->skb = NULL;
        }
        goto setup_for_next;
    }

    switch (RecvType) {
    case MVUSB_TYPE_DATA:
        if (RecvLength > MRVDRV_ETH_RX_PACKET_BUFFER_SIZE + MESSAGE_HEADER_LEN
            || RecvLength < USB_MIN_RX_DATA_LEN) {
            PRINTM(INFO, "Packet length is Invalid\n");

            if (cardp->skb) {
                kfree_skb(cardp->skb);
                cardp->skb = NULL;
            }
            break;
        }
        PRINTM(DATA, "Data <= FW @ %lu\n", os_time_get());

        skb_reserve(cardp->skb, NET_IP_ALIGN);
        skb_put(cardp->skb, RecvLength);
        skb_pull(cardp->skb, MESSAGE_HEADER_LEN);

        wlan_process_rx_packet(priv, cardp->skb);

        priv->wlan_dev.upld_len = (RecvLength - MESSAGE_HEADER_LEN);

        break;

    case MVUSB_TYPE_CMD:
        if (RecvLength > MRVDRV_SIZE_OF_CMD_BUFFER + MESSAGE_HEADER_LEN) {
            PRINTM(INFO, "The command length is too large\n");
            if (cardp->skb) {
                kfree_skb(cardp->skb);
                cardp->skb = NULL;
            }
            break;
        }

        /* take care of CurCmd = NULL case by reading the data to clear the
           interrupt */
        if (!priv->adapter->CurCmd) {
            cmdBuf = priv->wlan_dev.upld_buf;
        } else {
            cmdBuf = priv->adapter->CurCmd->BufVirtualAddr;
        }

        cardp->usb_int_cause |= HIS_CmdUpLdRdy;
        priv->wlan_dev.upld_len = (RecvLength - MESSAGE_HEADER_LEN);
        memcpy(cmdBuf, RecvBuff + MESSAGE_HEADER_LEN, priv->wlan_dev.upld_len);

        if (cardp->skb) {
            kfree_skb(cardp->skb);
            cardp->skb = NULL;
        }

        wlan_interrupt(priv->wlan_dev.netdev);
        break;

    case MVUSB_TYPE_EVENT:
        /* Event cause handling */
        cardp->usb_event_cause = *(u32 *) (RecvBuff + MESSAGE_HEADER_LEN);

        cardp->usb_event_cause = wlan_le32_to_cpu(cardp->usb_event_cause);

        cardp->usb_int_cause |= HIS_CardEvent;
        if (cardp->skb) {
            kfree_skb(cardp->skb);
            cardp->skb = NULL;
        }

        wlan_interrupt(priv->wlan_dev.netdev);
        break;

    default:
        if (cardp->skb) {
            kfree_skb(cardp->skb);
            cardp->skb = NULL;
        }
        break;
    }

  setup_for_next:
    /* For card event hold on to re-submit rx urb until the event gets to be
       handled */
    if (!(cardp->usb_int_cause & HIS_CardEvent))
        if_usb_submit_rx_urb(priv);
  rx_exit:
    LEAVE();
    return;
}

/** 
 *  @brief This function downloads command to FW  
 *  @param priv     pointer to wlan_private structure
 *  @param payload  pointer to command buffer
 *  @param nb       number of bytes
 *  @return         WLAN_STATUS_SUCCESS or WLAN_STATUS_FAILURE
 */
static int
if_usb_cmd_to_card(wlan_private * priv, u8 * payload, u16 nb)
{
    int ret = WLAN_STATUS_FAILURE;
    struct usb_card_rec *cardp = (struct usb_card_rec *) priv->wlan_dev.card;
    struct urb *tx_urb = NULL;
    u32 tmp;

    ENTER();

    /* Check if device is removed */
    if (priv->adapter->SurpriseRemoved) {
        PRINTM(INFO, "Device removed\n");
        goto tx_ret;
    }

    tmp = wlan_cpu_to_le32(MVUSB_TYPE_CMD);
    memcpy(cardp->tx_cmd_buffer, (u8 *) & tmp, MESSAGE_HEADER_LEN);
    memcpy((cardp->tx_cmd_buffer + MESSAGE_HEADER_LEN), payload, nb);

    /* Allocate Tx command URB */
    if (!(tx_urb = usb_alloc_urb(0, GFP_ATOMIC))) {
        PRINTM(ERROR, "Tx command URB allocation failed\n");
        ret = WLAN_STATUS_FAILURE;
        goto tx_ret;
    }

    /* Use USB API usb_fill_bulk_urb() to set the configuration information of 
       the Tx bulk URB and initialize the Tx callback */
    usb_fill_bulk_urb(tx_urb, cardp->udev,
                      usb_sndbulkpipe(cardp->udev,
                                      cardp->tx_cmd_endpointAddr),
                      cardp->tx_cmd_buffer, nb + MESSAGE_HEADER_LEN,
                      if_usb_tx_cmd_complete, priv);

    tx_urb->transfer_flags |= URB_ZERO_PACKET;

    atomic_inc(&cardp->tx_cmd_urb_pending);
    priv->wlan_dev.cmd_sent = TRUE;
    if ((ret = usb_submit_urb(tx_urb, GFP_ATOMIC))) {
        /* Submit URB failure */
        PRINTM(ERROR, "Submit Tx command URB failed: %d\n", ret);
        usb_free_urb(tx_urb);
        tx_urb = NULL;
        atomic_dec(&cardp->tx_cmd_urb_pending);
        priv->wlan_dev.cmd_sent = FALSE;
        ret = WLAN_STATUS_FAILURE;
    } else {
        ret = WLAN_STATUS_SUCCESS;
    }

  tx_ret:
    LEAVE();
    return ret;
}

/** 
 *  @brief This function downloads data to FW  
 *  @param priv     pointer to wlan_private structure
 *  @param type     type of data
 *  @param payload  pointer to command buffer
 *  @param nb       number of bytes
 *  @return         WLAN_STATUS_SUCCESS or WLAN_STATUS_FAILURE
 */
static int
if_usb_data_to_card(wlan_private * priv, u8 type, u8 * payload, u16 nb,
                    struct sk_buff *skb)
{
    struct usb_card_rec *cardp = priv->wlan_dev.card;
    int ret = WLAN_STATUS_FAILURE;
    struct urb *tx_urb = NULL;
    struct tx_data_context *context = NULL;

    ENTER();

    /* Check if device is removed */
    if (priv->adapter->SurpriseRemoved) {
        PRINTM(INFO, "Device removed\n");
        goto tx_ret;
    }

    /* Fill header */
    *(u32 *) & payload[0] = wlan_cpu_to_le32(MVUSB_TYPE_DATA);

    /* Allocate Tx data URB */
    if (!(tx_urb = usb_alloc_urb(0, GFP_ATOMIC))) {
        PRINTM(ERROR, "Tx data URB allocation failed\n");
        ret = WLAN_STATUS_FAILURE;
        goto tx_ret;
    }

    /* Create context */
    if (!(context = (struct tx_data_context *)
          kmalloc(sizeof(struct tx_data_context), GFP_KERNEL))) {
        PRINTM(ERROR, "Cannot allocate Tx data context\n");
        ret = WLAN_STATUS_FAILURE;
        goto tx_ret;
    }
    context->priv = priv;
    context->skb = skb;

    /* Use USB API usb_fill_bulk_urb() to set the configuration information of 
       the Tx bulk URB and initialize the Tx callback */
    usb_fill_bulk_urb(tx_urb, cardp->udev,
                      usb_sndbulkpipe(cardp->udev,
                                      cardp->tx_data_endpointAddr),
                      payload, nb + MESSAGE_HEADER_LEN, if_usb_tx_data_complete,
                      (void *) context);

    tx_urb->transfer_flags |= URB_ZERO_PACKET;

    atomic_inc(&cardp->tx_data_urb_pending);
    if (atomic_read(&cardp->tx_data_urb_pending) >= MVUSB_TX_HIGH_WMARK)
        priv->wlan_dev.data_sent = TRUE;
    if ((ret = usb_submit_urb(tx_urb, GFP_ATOMIC))) {
        /* Submit URB failure */
        PRINTM(ERROR, "Submit Tx data URB failed: %d\n", ret);
        atomic_dec(&cardp->tx_data_urb_pending);
        if (priv->wlan_dev.data_sent)
            priv->wlan_dev.data_sent = FALSE;
        ret = WLAN_STATUS_FAILURE;
    } else {
        ret = WLAN_STATUS_SUCCESS;
    }

  tx_ret:
    if (ret) {
        /* Error case. Free dynamically allocated resources */
        if (tx_urb) {
            usb_free_urb(tx_urb);
            tx_urb = NULL;
        }
        if (context)
            kfree(context);
    }
    LEAVE();
    return ret;
}

/** 
 *  @brief  This function downloads FW blocks to device
 *  @param priv		pointer to wlan_private
 *  @return 	   	WLAN_STATUS_SUCCESS or WLAN_STATUS_FAILURE
 */
static int
if_usb_prog_firmware(wlan_private * priv)
{
    u8 *firmware = priv->firmware->data, *RecvBuff;
    int ret = WLAN_STATUS_FAILURE, retries = MAX_FW_RETRY, DataLength;
    int actual_length, FWSeqNum = 0, TotalBytes = 0, DnldCmd;
    FWData *fwdata;
    FWSyncHeader SyncFWHeader;
    struct usb_card_rec *cardp = priv->wlan_dev.card;

    ENTER();

    /* Allocate memory for transmit */
    fwdata = kmalloc(FW_MAX_DATA_BLK_SIZE, GFP_KERNEL);
    if (!fwdata) {
        PRINTM(ERROR, "Could not allocate buffer for FW download\n");
        goto fw_exit;
    }

    /* Allocate memory for receive */
    RecvBuff = kmalloc(MVUSB_RX_BUF_LEN, GFP_KERNEL);
    if (!RecvBuff) {
        PRINTM(ERROR, "Could not allocate buffer for FW download response\n");
        goto cleanup;
    }

    do {
        /* Copy the header of the firmware data to get the length */
        memcpy(&fwdata->fwheader, &firmware[TotalBytes], sizeof(FWHeader));

        DataLength = wlan_le32_to_cpu(fwdata->fwheader.DataLength);
        DnldCmd = wlan_le32_to_cpu(fwdata->fwheader.DnldCmd);
        TotalBytes += sizeof(FWHeader);

        /* Copy the firmware data */
        memcpy(fwdata->data, &firmware[TotalBytes], DataLength);
        fwdata->SeqNum = wlan_cpu_to_le32(FWSeqNum);
        TotalBytes += DataLength;

        /* If the send/receive fails or CRC occurs then retry */
        while (retries) {
            retries--;

            /* Send the firmware block */
            if ((ret = usb_bulk_msg(cardp->udev, usb_sndbulkpipe(cardp->udev,
                                                                 cardp->
                                                                 bulk_out_endpointAddr),
                                    (u8 *) fwdata, FW_DATA_XMIT_SIZE,
                                    &actual_length, USB_BULK_MSG_TIMEOUT))) {
                PRINTM(ERROR, "usb_blk_msg for send failed, ret %d\n", ret);
                continue;
            }

            PRINTM(INFO, "Actual length sent is %d\n", actual_length);

            /* Receive the firmware block response */
            if ((ret = usb_bulk_msg(cardp->udev, usb_rcvbulkpipe(cardp->udev,
                                                                 cardp->
                                                                 bulk_in_endpointAddr),
                                    RecvBuff, MVUSB_RX_BUF_LEN, &actual_length,
                                    USB_BULK_MSG_TIMEOUT))) {
                PRINTM(ERROR, "usb_bulk_msg failed: %d\n", ret);
                continue;
            }

            memcpy(&SyncFWHeader, RecvBuff, sizeof(FWSyncHeader));
            endian_convert_syncfwheader(&SyncFWHeader);

            /* Check the firmware block response for CRC errors */
            if (SyncFWHeader.Cmd) {
                PRINTM(INFO, "FW received Blk with CRC error %x\n",
                       SyncFWHeader.Cmd);
                ret = WLAN_STATUS_FAILURE;
                continue;
            }

            /* Everything is successful so break */
            PRINTM(INFO, "Actual length received is %d\n", actual_length);
            retries = MAX_FW_RETRY;
            break;
        }

        FWSeqNum++;
        PRINTM(INFO, "The FW download is done size is %d\n", TotalBytes);
    } while ((DnldCmd != FW_HAS_LAST_BLOCK) && retries);

  cleanup:
    if (RecvBuff)
        kfree(RecvBuff);
    if (fwdata)
        kfree(fwdata);
    if (retries)
        ret = WLAN_STATUS_SUCCESS;

  fw_exit:
    LEAVE();
    return ret;
}

/********************************************************
		Global Functions
********************************************************/

/** 
 *  @brief This function downloads data to FW  
 *  @param priv		pointer to wlan_private structure
 *  @param type		type of data
 *  @param payload	pointer to data buffer
 *  @param nb		number of bytes
 *  @param skb		pointer to current skb
 *  @return 	   	WLAN_STATUS_SUCCESS or WLAN_STATUS_FAILURE
 */
int
sbi_host_to_card(wlan_private * priv, u8 type, u8 * payload, u16 nb,
                 struct sk_buff *skb)
{
    int ret = WLAN_STATUS_FAILURE;

    ENTER();

    PRINTM(INFO, "Type = %u, Size = %d\n", type, nb);

    if (type == MV_TYPE_CMD) {
        ret = if_usb_cmd_to_card(priv, payload, nb);
    } else {
        ret = if_usb_data_to_card(priv, type, payload, nb, skb);
    }

    LEAVE();
    return ret;
}

/** Get interrupt status */
int
sbi_get_int_status(wlan_private * priv, u8 * ireg)
{
    unsigned long driver_flags;
    struct usb_card_rec *cardp = (struct usb_card_rec *) priv->wlan_dev.card;

    ENTER();

    OS_INT_DISABLE(priv, driver_flags);
    *ireg = cardp->usb_int_cause;
    cardp->usb_int_cause = 0;
    OS_INT_RESTORE(priv, driver_flags);

    PRINTM(INFO, "Int cause is 0x%X\n", *ireg);

    LEAVE();

    return WLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function gets event cause.
 *  @param priv		pointer to wlan_private
 *  @return 	   	WLAN_STATUS_SUCCESS
 */
int
sbi_read_event_cause(wlan_private * priv)
{
    struct usb_card_rec *cardp = (struct usb_card_rec *) priv->wlan_dev.card;
    priv->adapter->EventCause = cardp->usb_event_cause;
    cardp->usb_event_cause = 0;

    /* Re-submit rx urb here to avoid event lost issue */
    if (!atomic_read(&cardp->rx_data_urb_pending))
        if_usb_submit_rx_urb(priv);

    return WLAN_STATUS_SUCCESS;
}

/**  
 *  @brief This function registers driver.
 *  @return 	   	WLAN_STATUS_SUCCESS or WLAN_STATUS_FAILURE
 */
int
sbi_register(void)
{
    int ret = WLAN_STATUS_SUCCESS;
    ENTER();

    /* 
     * Make use of the global variable cardp,
     * to assign the call backs
     */

    /* 
     * API registers the Marvell USB driver
     * to the USB system
     */
    if (usb_register(&if_usb_driver) != 0) {
        PRINTM(FATAL, "USB Driver Registration Failed \n");
        ret = WLAN_STATUS_FAILURE;
    }

    LEAVE();

    return ret;
}

/**  
 *  @brief This function removes usb driver.
 *  @return 	   	N/A
 */
void
sbi_unregister(void)
{
    ENTER();

    /* API unregisters the driver from USB subsystem */
    usb_deregister(&if_usb_driver);

    LEAVE();
    return;
}

/**  
 *  @brief This function resets usb device.
 *  @return 	   	N/A
 */
void
sbi_reset_dev(wlan_private * priv)
{
    struct usb_card_rec *cardp = (struct usb_card_rec *) priv->wlan_dev.card;
    ENTER();

    usb_reset_device(cardp->udev);

    LEAVE();
    return;
}

/** 
 *  @brief  This function register usb device and initialize parameter
 *  @param priv		pointer to wlan_private
 *  @return 	   	WLAN_STATUS_SUCCESS or WLAN_STATUS_FAILURE
 */
int
sbi_register_dev(wlan_private * priv)
{
    struct usb_card_rec *cardp = (struct usb_card_rec *) priv->wlan_dev.card;
    int ret = WLAN_STATUS_SUCCESS;

    ENTER();

    cardp->priv = priv;
    cardp->eth_dev = priv->wlan_dev.netdev;

    priv->hotplug_device = &(cardp->udev->dev);
    if (fw_name == NULL) {
        if (cardp->udev->descriptor.idProduct == USB8682_PID_1)
            fw_name = USB8682_B0_FW_NAME;
        else if (cardp->udev->descriptor.idProduct == USB8682_PID_2)
            fw_name = USB8682_B1_FW_NAME;
        else
            fw_name = DEFAULT_FW_NAME;
    }

    LEAVE();
    return ret;
}

/** 
 *  @brief This function handles FW download
 *  @param priv		pointer to wlan_private
 *  @return 	   	WLAN_STATUS_SUCCESS or WLAN_STATUS_FAILURE
 */
int
sbi_prog_fw_w_helper(wlan_private * priv)
{
    int ret;

    ENTER();

    /* This function downloads the firmware */
    if ((ret = if_usb_prog_firmware(priv))) {
        PRINTM(FATAL, "FW download failure\n");
        goto prog_fw_exit;
    }

    /* Submit Rx URB for data/command/event */
    if ((ret = if_usb_submit_rx_urb(priv)) < 0)
        goto prog_fw_exit;

    /* Delay 200 ms to waiting for the FW ready */
    os_sched_timeout(200);

  prog_fw_exit:
    LEAVE();
    return ret;
}

static int
if_usb_suspend(struct usb_interface *intf, pm_message_t message)
{
    struct usb_card_rec *cardp = usb_get_intfdata(intf);

    ENTER();

    if (!cardp) {
        PRINTM(ERROR, "Card structure is not valid\n");
        LEAVE();
        return 0;
    }

    /* Exit PS mode before USB suspend */
    {
        wlan_private *priv = cardp->priv;
        if (!priv || !priv->adapter) {
            PRINTM(ERROR, "Priv or adapter structure is not valid\n");
            LEAVE();
            return 0;
        }

        if (priv->adapter->PSMode != Wlan802_11PowerModeCAM)
            if (priv->adapter->PSState != PS_STATE_FULL_POWER) {
                /* Exit PS mode */
                wlan_exit_ps(priv, 0);
                priv->adapter->PSState = PS_STATE_FULL_POWER;
            }
    }
    netif_device_detach(cardp->eth_dev);

    cardp->rx_data_urb_recall = TRUE;

    LEAVE();
    return 0;
}

static int
if_usb_resume(struct usb_interface *intf)
{
    struct usb_card_rec *cardp = usb_get_intfdata(intf);

    ENTER();

    if (!cardp) {
        PRINTM(ERROR, "Card structure is not valid\n");
        LEAVE();
        return 0;
    }

    cardp->rx_data_urb_recall = FALSE;
    if (!atomic_read(&cardp->rx_data_urb_pending))
        if_usb_submit_rx_urb(cardp->priv);

    /* Back to PS mode after USB resume */
    {
        wlan_private *priv = cardp->priv;
        if (!priv || !priv->adapter) {
            PRINTM(ERROR, "Priv or adapter structure is not valid\n");
            LEAVE();
            return 0;
        }

        if (priv->adapter->PSMode != Wlan802_11PowerModeCAM)
            if (priv->adapter->PSState == PS_STATE_FULL_POWER)
                /* Enter PS mode */
                wlan_enter_ps(priv, 0);
    }
    netif_device_attach(cardp->eth_dev);

    LEAVE();
    return 0;
}
