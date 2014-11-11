/** @file wlan_usb_bt.c
  * @brief This file contains functions used in USB boot command 
  *        and Boot2/FW update
  *
  * Copyright (C) 2003-2008, Marvell International Ltd.
  * All Rights Reserved
  */
/********************************************************
Change log:
	06/29/06: Init Version
	07/20/06: Use kernel function to request firmware
	          Add Boot2 update by MFG FW support
********************************************************/

#include	"wlan_headers.h"
#include	"wlan_usb.h"
#ifdef USB_DELAY
extern int usb_ms_delay;
#endif
/********************************************************
		Local Variables
********************************************************/

/********************************************************
		Global Variables
********************************************************/

/** 
 *  @brief This function issues boot command to the Boot2 code
 *  @param iValue   1: Boot from FW by USB-Download
 *                  2: Boot from FW in EEPROM
 *                  3: Update Storage Boot2
 *                  4: Update Storage FW
 *  @return         WLAN_STATUS_SUCCESS or WLAN_STATUS_FAILURE
 */
static int
us_usb_issue_boot_command(struct usb_card_rec *cardp, int iValue)
{
    int ret, actual_length;
    struct BootCMDStr sBootCMD;
    struct BootCMDRespStr* sBootCMDResp;

    ENTER();
    /* Prepare Command */
    memset((void *) &sBootCMD, 0x00, sizeof(sBootCMD));
    sBootCMD.u32MagicNumber = wlan_cpu_to_le32(BOOT_CMD_MAGIC_NUMBER);
    sBootCMD.u8CMD_Tag = iValue;

    /* Issue Command */
    if ((ret = usb_bulk_msg(cardp->udev, usb_sndbulkpipe(cardp->udev,
                                                         cardp->
                                                         bulk_out_endpointAddr),
                            (u8 *) & sBootCMD, sizeof(struct BootCMDStr),
                            &actual_length, USB_BULK_MSG_TIMEOUT))) {
        PRINTM(ERROR, "usb_bulk_msg send failed, ret %d\n", ret);
        LEAVE();
        return ret;
    }

    PRINTM(INFO, "Actual length sent is %d\n", actual_length);

    //memset((void *) &sBootCMDResp, 0x00, sizeof(sBootCMDResp));
    sBootCMDResp = kmalloc(MVUSB_RX_BUF_LEN, GFP_KERNEL);
    memset((void *) sBootCMDResp, 0x00, MVUSB_RX_BUF_LEN);

    if ((ret = usb_bulk_msg(cardp->udev, usb_rcvbulkpipe(cardp->udev,
                                                         cardp->
                                                         bulk_in_endpointAddr),
                            (u8 *) sBootCMDResp,  MVUSB_RX_BUF_LEN,
                            &actual_length, USB_BULK_MSG_TIMEOUT))) {
        PRINTM(ERROR, "usb_bulk_msg send failed, ret %d\n", ret);
	if(sBootCMDResp)
		kfree(sBootCMDResp);
        LEAVE();
        return ret;
    }

    if (wlan_le32_to_cpu(sBootCMDResp->u32MagicNumber) != BOOT_CMD_MAGIC_NUMBER) {
        PRINTM(FATAL, "Boot command response MAGIC_NUMBER error (0x%x)\n",
               sBootCMDResp->u32MagicNumber);
	if(sBootCMDResp)
		kfree(sBootCMDResp);
        LEAVE();
        return WLAN_STATUS_FAILURE;
    }
    
    if(sBootCMDResp)
	kfree(sBootCMDResp);

    PRINTM(INFO, "Boot command response success\n");
    LEAVE();
    return WLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function sends a boot command to device
 *  @param priv		pointer to wlan_private
 *  @return 	   	WLAN_STATUS_SUCCESS or WLAN_STATUS_FAILURE
 */
int
sbi_send_boot_command(wlan_private * priv)
{
    struct usb_card_rec *cardp = priv->wlan_dev.card;
    int ret = WLAN_STATUS_FAILURE;
    int i = 0;

    ENTER();
#ifdef USB_DELAY
    mdelay(usb_ms_delay);
#endif /* USB_DELAY */
    do {
      i++;
        /* Issue boot command 1: Download FW */
      ret = us_usb_issue_boot_command(cardp, BOOT_CMD_FW_BY_USB);
    } while ( (ret == WLAN_STATUS_FAILURE) && (i <= MAX_BOOT_COMMAND_RETRY));
    // } while( ret && i < MAX_BOOT_COMMAND_RETRY);

#ifdef USB_DELAY
    mdelay(usb_ms_delay);
#endif /*USB_DELAY*/

    /* For backward compatible, we always set success */
    ret = WLAN_STATUS_SUCCESS;

    LEAVE();
    return ret;
}
