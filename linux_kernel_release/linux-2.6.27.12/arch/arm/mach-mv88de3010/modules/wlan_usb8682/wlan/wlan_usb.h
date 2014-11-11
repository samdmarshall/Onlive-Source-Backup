/** @file wlan_usb.h
 * 
 *  @brief This file contains definitions for USB interface.
 *  
 *  Copyright (C) 2003-2008, Marvell International Ltd.
 *  All Rights Reserved
 */
/********************************************************
Change log:
	10/11/05: Add Doxygen format comments
	
********************************************************/

#ifndef _WLAN_USB_H
#define _WLAN_USB_H

/** Type Command */
#define MVUSB_TYPE_CMD       		0xF00DFACE
/** Type Data */
#define MVUSB_TYPE_DATA      		0xBEADC0DE
/** Type Event */
#define MVUSB_TYPE_EVENT     		0xBEEFFACE

/** USB VID 1 */
#define USB8682_VID_1	0x1286
/** USB PID 1 */
#define USB8682_PID_1	0x2030
/** USB VID 2 */
#define USB8682_VID_2	0x1286
/** USB PID 2 */
#define USB8682_PID_2	0x2031

/** High watermark for Tx data */
#define MVUSB_TX_HIGH_WMARK    3
/** USB Rx buffer size */
#define MVUSB_RX_BUF_LEN       2048

/** Boot command : Firmware by USB */
#define BOOT_CMD_FW_BY_USB     0x01
/** Boot command : Firmware in EEPROM */
#define BOOT_CMD_FW_IN_EEPROM  0x02
/** Boot command : Update boot2 */
#define BOOT_CMD_UPDATE_BOOT2  0x03
/** Boot command : Update firmware */
#define BOOT_CMD_UPDATE_FW     0x04
/** Boot command : Magic number */
#define BOOT_CMD_MAGIC_NUMBER  0x4C56524D       /* M=>0x4D,R=>0x52,V=>0x56,L=>0x4C 
                                                 */

/** Maximum retry count for boot command */
#define MAX_BOOT_COMMAND_RETRY 2

struct BootCMDStr
{
    /** Magic number */
    u32 u32MagicNumber;
    /** Command tag */
    u8 u8CMD_Tag;
    /** Dummy */
    u8 au8Dumy[11];
};

/** Boot command response : OK */
#define BOOT_CMD_RESP_OK     0x0001
/** Boot command response : Fail */
#define BOOT_CMD_RESP_FAIL   0x0000

struct BootCMDRespStr
{
    /** Magic number */
    u32 u32MagicNumber;
    /** Command tag */
    u8 u8CMD_Tag;
    /** Result */
    u8 u8Result;
    /** Dummy */
    u8 au8Dumy[2];
};

/** USB card description structure*/
struct usb_card_rec
{
    /** Ethernet device */
    struct net_device *eth_dev;
    /** USB device */
    struct usb_device *udev;
    /** Private data */
    void *priv;

    /** Rx data URB */
    struct urb *rx_data_urb;
    /** Rx data skb */
    struct sk_buff *skb;
    /** Bulk in size */
    int bulk_in_size;
    /** Rx data endpoint address */
    u8 rx_data_endpointAddr;
    /** Rx data URB pending count */
    atomic_t rx_data_urb_pending;
    /** Rx data URB recall flag */
    BOOLEAN rx_data_urb_recall;

    /** Tx command buffer */
    u8 *tx_cmd_buffer;
    /** Bulk out size */
    int bulk_out_size;
    /** Tx data endpoint address */
    u8 tx_data_endpointAddr;
    /** Tx command endpoint address */
    u8 tx_cmd_endpointAddr;
    /** Tx data URB pending count */
    atomic_t tx_data_urb_pending;
    /** Tx command URB pending count */
    atomic_t tx_cmd_urb_pending;

    /** Bulk in endpoint address */
    u8 bulk_in_endpointAddr;
    /** Bulk out endpoint address */
    u8 bulk_out_endpointAddr;

    /** USB interrupt cause */
    u8 usb_int_cause;
    /** USB event cause */
    u32 usb_event_cause;
};

int if_usb_submit_rx_urb(wlan_private * priv);

#ifdef BIG_ENDIAN
/** Convert sequence number and command fields of fwheader to correct endian format */
#define endian_convert_syncfwheader(x); { \
		(x)->Cmd = wlan_le32_to_cpu((x)->Cmd); \
		(x)->SeqNum = wlan_le32_to_cpu((x)->SeqNum); \
	}
#else
/** Convert sequence number and command fields of fwheader to correct endian format */
#define endian_convert_syncfwheader(x)
#endif

/** FWHeader */
typedef struct _FWHeader
{
    /** FW download command */
    u32 DnldCmd;
    /** FW base address */
    u32 BaseAddr;
    /** FW data length */
    u32 DataLength;
    /** FW CRC */
    u32 CRC;
} __ATTRIB_PACK__ FWHeader, *pFWHeader;

/** Timeout for usb_bulk_msg function */
#define USB_BULK_MSG_TIMEOUT	100
/** Maximum data block size */
#define FW_MAX_DATA_BLK_SIZE	620
/** Max firmware retry */
#define MAX_FW_RETRY        	3

/** FWData */
typedef struct _FWData
{
    /** FW data header */
    FWHeader fwheader;
    /** FW data sequence number */
    u32 SeqNum;
    /** FW data buffer */
    u8 data[1];
} __ATTRIB_PACK__ FWData, pFWData;

/** FWSyncHeader */
typedef struct _FWSyncHeader
{
    /** FW sync header command */
    u32 Cmd __ATTRIB_ALIGN__;
    /** FW sync header sequence number */
    u32 SeqNum __ATTRIB_ALIGN__;
} __ATTRIB_PACK__ FWSyncHeader;

/** Firmware has last block */
#define FW_HAS_LAST_BLOCK		0x00000004

/** Firmware data transmit size */
#define FW_DATA_XMIT_SIZE \
	sizeof(FWHeader) + DataLength + sizeof(u32)

#endif /* _WLAN_USB_H */
