/** @file moal_usb.h
 * 
 *  @brief This file contains definitions for USB interface.
 *  
 *  Copyright (C) 2008-2009, Marvell International Ltd.
 *  All Rights Reserved
 */
/*************************************************************
Change Log:
    10/21/2008: initial version
************************************************************/

#ifndef _MOAL_USB_H
#define _MOAL_USB_H

/** USB VID 1 */
#define USB8786_VID_1	0x1286
/** USB PID 1 */
#define USB8786_PID_1	0x203c

/** High watermark for Tx data */
#define MVUSB_TX_HIGH_WMARK    6

/** Number of Rx data URB */
#define MVUSB_RX_DATA_URB   6

/** urb context */
typedef struct _urb_context
{
        /** Pointer to moal_handle structure */
    moal_handle *handle;
        /** Pointer to mlan buffer */
    mlan_buffer *pmbuf;
        /** URB */
    struct urb *urb;
        /** EP */
    t_u8 ep;
} urb_context;

/** USB card description structure*/
struct usb_card_rec
{
        /** USB device */
    struct usb_device *udev;
        /** MOAL handle */
    void *phandle;
        /** Rx data endpoint address */
    t_u8 rx_cmd_ep;
        /** Rx cmd contxt */
    urb_context rx_cmd;
        /** Rx command URB pending count */
    atomic_t rx_cmd_urb_pending;
        /** Rx data context list */
    urb_context rx_data_list[MVUSB_RX_DATA_URB];
        /** Rx data endpoint address */
    t_u8 rx_data_ep;
        /** Rx data URB pending count */
    atomic_t rx_data_urb_pending;
        /** Rx URB recall flag */
    BOOLEAN rx_urb_recall;
        /** Tx data endpoint address */
    t_u8 tx_data_ep;
        /** Tx command endpoint address */
    t_u8 tx_cmd_ep;
        /** Tx data URB pending count */
    atomic_t tx_data_urb_pending;
        /** Tx command URB pending count */
    atomic_t tx_cmd_urb_pending;
        /** Bulk out max packet size */
    int bulk_out_maxpktsize;
        /** Pre-allocated urb for command */
    urb_context tx_cmd;
        /** Index to point to next data urb to use */
    int tx_data_ix;
        /** Pre-allocated urb for data */
    urb_context tx_data_list[MVUSB_TX_HIGH_WMARK];
};

mlan_status woal_write_data_async(moal_handle * handle, mlan_buffer * pmbuf,
                                  t_u8 ep);
mlan_status woal_write_data_sync(moal_handle * handle, mlan_buffer * pmbuf,
                                 t_u8 ep, t_u32 timeout);
mlan_status woal_read_data_sync(moal_handle * handle, mlan_buffer * pmbuf,
                                t_u8 ep, t_u32 timeout);
mlan_status woal_usb_rx_init(moal_handle * handle);
mlan_status woal_usb_tx_init(moal_handle * handle);
void woal_submit_rx_urb(moal_handle * handle, t_u8 ep);
void woal_bus_unregister(void);
mlan_status woal_bus_register(void);
mlan_status woal_register_dev(moal_handle * handle);
#endif /*_MOAL_USB_H */
