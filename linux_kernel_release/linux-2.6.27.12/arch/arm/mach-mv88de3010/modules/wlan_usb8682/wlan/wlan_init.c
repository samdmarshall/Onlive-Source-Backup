/** @file wlan_init.c
  * @brief This file contains the initialization for FW
  * and HW 
  * 
  * Copyright (C) 2003-2008, Marvell International Ltd. 
  * All Rights Reserved
  */
/********************************************************
Change log:
	09/28/05: Add Doxygen format comments
	01/05/06: Add kernel 2.6.x support	
	01/11/06: Conditionalize new scan/join functions.
	          Cleanup association response handler initialization.
	01/06/05: Add FW file read
	05/08/06: Remove the second GET_HW_SPEC command and TempAddr/PermanentAddr
	06/30/06: Replace MODULE_PARM(name, type) with module_param(name, type, perm)

********************************************************/

#include	"wlan_headers.h"

#include	<linux/firmware.h>

/********************************************************
		Local Variables
********************************************************/

/** Firmware name */
char *fw_name = NULL;

#ifdef MFG_CMD_SUPPORT
int mfgmode = 0;
#endif

#ifdef USB_DELAY
int usb_ms_delay = 100;
#endif

/********************************************************
		Global Variables
********************************************************/

/********************************************************
		Local Functions
********************************************************/

/** 
 *  @brief This function allocates buffer for the member of adapter
 *  structure like command buffer and BSSID list.
 *  
 *  @param priv    A pointer to wlan_private structure
 *  @return 	   WLAN_STATUS_SUCCESS or WLAN_STATUS_FAILURE
 */
static int
wlan_allocate_adapter(wlan_private * priv)
{
    u32 ulBufSize;
    wlan_adapter *Adapter = priv->adapter;
    BSSDescriptor_t *pTempScanTable;

    ENTER();

    /* Allocate buffer to store the BSSID list */
    ulBufSize = sizeof(BSSDescriptor_t) * MRVDRV_MAX_BSSID_LIST;

    if (!(pTempScanTable = kmalloc(ulBufSize, GFP_KERNEL))) {
        PRINTM(ERROR, "Failed to allocate %d bytes pTempScanTable\n", ulBufSize);
        LEAVE();
        return WLAN_STATUS_FAILURE;
    }

    Adapter->ScanTable = pTempScanTable;
    memset(Adapter->ScanTable, 0, ulBufSize);

    if (!(Adapter->bgScanConfig =
          kmalloc(sizeof(HostCmd_DS_802_11_BG_SCAN_CONFIG), GFP_KERNEL))) {
            PRINTM(ERROR, "Failed to allocate %d bytes bgScanConfig\n", sizeof(HostCmd_DS_802_11_BG_SCAN_CONFIG));
        LEAVE();
        return WLAN_STATUS_FAILURE;
    }
    Adapter->bgScanConfigSize = sizeof(HostCmd_DS_802_11_BG_SCAN_CONFIG);
    memset(Adapter->bgScanConfig, 0, Adapter->bgScanConfigSize);

    spin_lock_init(&Adapter->QueueSpinLock);
    INIT_LIST_HEAD(&Adapter->CmdFreeQ);
    INIT_LIST_HEAD(&Adapter->CmdPendingQ);

    /* Allocate the command buffers */
    if (wlan_alloc_cmd_buffer(priv) != WLAN_STATUS_SUCCESS)
        return WLAN_STATUS_FAILURE;

    memset(&Adapter->PSConfirmSleep, 0, sizeof(PS_CMD_ConfirmSleep));
    Adapter->PSConfirmSleep.SeqNum = wlan_cpu_to_le16(++Adapter->SeqNum);
    Adapter->PSConfirmSleep.Command =
        wlan_cpu_to_le16(HostCmd_CMD_802_11_PS_MODE);
    Adapter->PSConfirmSleep.Size =
        wlan_cpu_to_le16(sizeof(PS_CMD_ConfirmSleep));
    Adapter->PSConfirmSleep.Result = 0;
    Adapter->PSConfirmSleep.Action =
        wlan_cpu_to_le16(HostCmd_SubCmd_Sleep_Confirmed);

    LEAVE();
    return WLAN_STATUS_SUCCESS;
}

/********************************************************
		Global Functions
********************************************************/

/**
 *  @brief This function initializes the adapter structure
 *  and set default value to the member of adapter.
 *  
 *  @param priv    A pointer to wlan_private structure
 *  @return 	   n/a
 */
void
wlan_init_adapter(wlan_private * priv)
{
    wlan_adapter *Adapter = priv->adapter;
    int i;

    ENTER();

    Adapter->ScanProbes = 0;

    Adapter->bcn_avg_factor = DEFAULT_BCN_AVG_FACTOR;
    Adapter->data_avg_factor = DEFAULT_DATA_AVG_FACTOR;

    /* ATIM params */
    Adapter->AtimWindow = 0;
    Adapter->ATIMEnabled = FALSE;

    Adapter->MediaConnectStatus = WlanMediaStateDisconnected;
    memset(Adapter->CurrentAddr, 0xff, ETH_ALEN);

    /* Status variables */
    Adapter->HardwareStatus = WlanHardwareStatusInitializing;

    /* scan type */
    Adapter->ScanType = HostCmd_SCAN_TYPE_ACTIVE;

    /* scan mode */
    Adapter->ScanMode = HostCmd_BSS_TYPE_ANY;

    /* scan time */
    Adapter->SpecificScanTime = MRVDRV_SPECIFIC_SCAN_CHAN_TIME;
    Adapter->ActiveScanTime = MRVDRV_ACTIVE_SCAN_CHAN_TIME;
    Adapter->PassiveScanTime = MRVDRV_PASSIVE_SCAN_CHAN_TIME;

    /* 802.11 specific */
    Adapter->SecInfo.WEPStatus = Wlan802_11WEPDisabled;
    for (i = 0; i < sizeof(Adapter->WepKey) / sizeof(Adapter->WepKey[0]); i++)
        memset(&Adapter->WepKey[i], 0, sizeof(MRVL_WEP_KEY));
    Adapter->CurrentWepKeyIndex = 0;
    Adapter->SecInfo.AuthenticationMode = Wlan802_11AuthModeOpen;
    Adapter->SecInfo.EncryptionMode = CIPHER_NONE;
    Adapter->AdhocAESEnabled = FALSE;
    Adapter->AdhocState = ADHOC_IDLE;
    Adapter->InfrastructureMode = Wlan802_11Infrastructure;

    Adapter->NumInScanTable = 0;
    Adapter->pAttemptedBSSDesc = NULL;
#ifdef REASSOCIATION
    OS_INIT_SEMAPHORE(&Adapter->ReassocSem);
#endif
    Adapter->pBeaconBufEnd = Adapter->beaconBuffer;

    Adapter->HisRegCpy |= HIS_TxDnLdRdy;

    memset(&Adapter->CurBssParams, 0, sizeof(Adapter->CurBssParams));

    /* PnP and power profile */
    Adapter->SurpriseRemoved = FALSE;

    Adapter->CurrentPacketFilter =
        HostCmd_ACT_MAC_RX_ON | HostCmd_ACT_MAC_TX_ON |
        HostCmd_ACT_MAC_ETHERNETII_ENABLE;

    Adapter->RadioOn = RADIO_ON;
#ifdef REASSOCIATION
#if (WIRELESS_EXT >= 18)
    Adapter->Reassoc_on = FALSE;
#else
    Adapter->Reassoc_on = TRUE;
#endif
#endif /* REASSOCIATION */

    Adapter->HWRateDropMode = HW_TABLE_RATE_DROP;
    Adapter->Is_DataRate_Auto = TRUE;
    Adapter->BeaconPeriod = MRVDRV_BEACON_INTERVAL;

    Adapter->AdhocChannel = DEFAULT_AD_HOC_CHANNEL;

    Adapter->PSMode = Wlan802_11PowerModeCAM;
    Adapter->MultipleDtim = MRVDRV_DEFAULT_MULTIPLE_DTIM;

    Adapter->ListenInterval = MRVDRV_DEFAULT_LISTEN_INTERVAL;

    Adapter->PSState = PS_STATE_FULL_POWER;

    Adapter->NeedToWakeup = FALSE;
    Adapter->LocalListenInterval = 0;   /* default value in firmware will be
                                           used */
    Adapter->fwWakeupMethod = WAKEUP_FW_UNCHANGED;

    Adapter->IsEnhancedPSEnabled = FALSE;

    Adapter->bWakeupDevRequired = FALSE;
    Adapter->WakeupTries = 0;
    Adapter->bHostSleepConfigured = FALSE;
    Adapter->HSCfg.conditions = HOST_SLEEP_CFG_CANCEL;
    Adapter->HSCfg.gpio = 0;
    Adapter->HSCfg.gap = 0;

    Adapter->DataRate = 0;      /* Initially indicate the rate as auto */

    Adapter->adhoc_grate_enabled = FALSE;

    Adapter->IntCounter = Adapter->IntCounterSaved = 0;

    Adapter->gen_null_pkg = TRUE;       /* Enable NULL Pkg generation */

    init_waitqueue_head(&Adapter->ds_awake_q);

    spin_lock_init(&Adapter->CurrentTxLock);
    spin_lock_init(&Adapter->driver_lock);

    Adapter->PktTxCtrl = 0;

    /* Initialize 802.11d */
    wlan_init_11d(priv);

    LEAVE();
    return;
}

/** 
 *  @brief This function initializes software
 *  
 *  @param priv    A pointer to wlan_private structure
 *  @return 	   WLAN_STATUS_SUCCESS or WLAN_STATUS_FAILURE
 */
int
wlan_init_sw(wlan_private * priv)
{
    int ret = WLAN_STATUS_SUCCESS;
    wlan_adapter *Adapter = priv->adapter;

    ENTER();

    /* Allocate adapter structure */
    if ((ret = wlan_allocate_adapter(priv)) != WLAN_STATUS_SUCCESS)
        goto done;

    /* Initialize adapter structure */
    wlan_init_adapter(priv);

    /* Initialize the timer for command handling */
    wlan_initialize_timer(&Adapter->MrvDrvCommandTimer,
                          wlan_cmd_timeout_func, priv);
    Adapter->CommandTimerIsSet = FALSE;

#ifdef REASSOCIATION
    /* Initialize the timer for the reassociation */
    wlan_initialize_timer(&Adapter->MrvDrvTimer, wlan_reassoc_timer_func, priv);
    Adapter->ReassocTimerIsSet = FALSE;
#endif /* REASSOCIATION */

  done:
    LEAVE();
    return ret;
}

/** 
 *  @brief This function initializes firmware
 *  
 *  @param priv    A pointer to wlan_private structure
 *  @return 	   WLAN_STATUS_SUCCESS or WLAN_STATUS_FAILURE
 */
int
wlan_init_fw(wlan_private * priv)
{
    int ret = WLAN_STATUS_SUCCESS;
    wlan_adapter *Adapter = priv->adapter;

    ENTER();

    /* Send USB Boot Command to device */
    //mdelay(100);
    sbi_send_boot_command(priv);

    if ((ret =
         request_firmware(&priv->firmware, fw_name,
                          priv->hotplug_device)) < 0) {
        PRINTM(FATAL, "request_firmware() failed, error code = %#x\n", ret);
        goto done;
    }

    /* Download the main firmware via the helper firmware */
    if (sbi_prog_fw_w_helper(priv)) {
        PRINTM(ERROR, "Wlan FW download failed!\n");
        ret = WLAN_STATUS_FAILURE;
        goto done;
    }

#define RF_REG_OFFSET 0x07
#define RF_REG_VALUE  0xc8

#ifdef MFG_CMD_SUPPORT
    if (mfgmode == 0) {
#endif

        /*
         * Read MAC address from HW
         */
        memset(Adapter->CurrentAddr, 0xff, ETH_ALEN);

        ret = wlan_prepare_cmd(priv, HostCmd_CMD_GET_HW_SPEC,
                               0,
                               HostCmd_OPTION_WAITFORRSP |
                               HostCmd_OPTION_TIMEOUT, 0, NULL);

        if (ret) {
            PRINTM(ERROR, "Wlan Get HW Spec failed!\n");
            ret = WLAN_STATUS_FAILURE;
            goto done;
        }

        ret = wlan_prepare_cmd(priv,
                               HostCmd_CMD_MAC_CONTROL,
                               0,
                               HostCmd_OPTION_WAITFORRSP |
                               HostCmd_OPTION_TIMEOUT, 0,
                               &Adapter->CurrentPacketFilter);

        if (ret) {
                PRINTM(ERROR, "Wlan MAC Control failed!\n");
            ret = WLAN_STATUS_FAILURE;
            goto done;
        }

        ret = wlan_prepare_cmd(priv,
                               HostCmd_CMD_802_11_FW_WAKE_METHOD,
                               HostCmd_ACT_GEN_GET,
                               HostCmd_OPTION_WAITFORRSP |
                               HostCmd_OPTION_TIMEOUT, 0,
                               &priv->adapter->fwWakeupMethod);

        if (ret) {
                PRINTM(ERROR,"Wlan 802.11 FW Wake method failed!\n");
            ret = WLAN_STATUS_FAILURE;
            goto done;
        }

        ret = wlan_prepare_cmd(priv,
                               HostCmd_CMD_802_11_RATE_ADAPT_RATESET,
                               HostCmd_ACT_GEN_GET,
                               HostCmd_OPTION_WAITFORRSP |
                               HostCmd_OPTION_TIMEOUT, 0, NULL);
        if (ret) {
                PRINTM(ERROR, "Wlan 802.11 adapter rate reset failed!\n");
            ret = WLAN_STATUS_FAILURE;
            goto done;
        }
        priv->adapter->DataRate = 0;

        ret = wlan_prepare_cmd(priv,
                               HostCmd_CMD_802_11_RF_TX_POWER,
                               HostCmd_ACT_GEN_GET,
                               HostCmd_OPTION_WAITFORRSP |
                               HostCmd_OPTION_TIMEOUT, 0, NULL);
        if (ret) {
                PRINTM(ERROR, "Wlan 802.11 RF TX power failed!\n");
            ret = WLAN_STATUS_FAILURE;
            goto done;
        }
#ifdef MFG_CMD_SUPPORT
    }
#endif

    Adapter->HardwareStatus = WlanHardwareStatusReady;
    ret = WLAN_STATUS_SUCCESS;
  done:
    if (priv->firmware)
        release_firmware(priv->firmware);

    if (ret != WLAN_STATUS_SUCCESS) {
        Adapter->HardwareStatus = WlanHardwareStatusNotReady;
        ret = WLAN_STATUS_FAILURE;
    }
    LEAVE();
    return ret;
}

/** 
 *  @brief This function frees the structure of adapter
 *    
 *  @param priv    A pointer to wlan_private structure
 *  @return 	   n/a
 */
void
wlan_free_adapter(wlan_private * priv)
{
    wlan_adapter *Adapter = priv->adapter;

    ENTER();

    if (!Adapter) {
        PRINTM(INFO, "Why double free adapter?:)\n");
        LEAVE();
        return;
    }

    PRINTM(INFO, "Free Command buffer\n");
    wlan_free_cmd_buffer(priv);

    PRINTM(INFO, "Free CommandTimer\n");
    if (Adapter->CommandTimerIsSet) {
        wlan_cancel_timer(&Adapter->MrvDrvCommandTimer);
        Adapter->CommandTimerIsSet = FALSE;
    }
    FreeTimer(&Adapter->MrvDrvCommandTimer);
#ifdef REASSOCIATION
    PRINTM(INFO, "Free MrvDrvTimer\n");
    if (Adapter->ReassocTimerIsSet) {
        wlan_cancel_timer(&Adapter->MrvDrvTimer);
        Adapter->ReassocTimerIsSet = FALSE;
    }
    FreeTimer(&Adapter->MrvDrvTimer);
#endif /* REASSOCIATION */

    if (Adapter->bgScanConfig) {
        kfree(Adapter->bgScanConfig);
        Adapter->bgScanConfig = NULL;
    }

    OS_FREE_LOCK(&Adapter->CurrentTxLock);
    OS_FREE_LOCK(&Adapter->QueueSpinLock);

    PRINTM(INFO, "Free ScanTable\n");
    if (Adapter->ScanTable) {
        kfree(Adapter->ScanTable);
        Adapter->ScanTable = NULL;
    }

    PRINTM(INFO, "Free Adapter\n");

    /* Free the adapter object itself */
    kfree(Adapter);
    priv->adapter = NULL;
    LEAVE();
}

/** 
 *  @brief Clears all pending commands
 *   
 *  @param priv		A pointer to wlan_private structure
 *  @return    		NA
 */
void
wlan_clear_pending_cmd(wlan_private * priv)
{
    wlan_adapter *Adapter = priv->adapter;
    CmdCtrlNode *CmdNode = NULL;
    ulong flags;

    ENTER();

    /* Release the current command */
    if (Adapter->CurCmd) {
        /* set error code that will be transferred back to wlan_prepare_cmd() */
        PRINTM(INFO, "Wake up current cmdwait_q\n");
        Adapter->CurCmdRetCode = WLAN_STATUS_FAILURE;
        wlan_insert_cmd_to_free_q(priv, Adapter->CurCmd);
        spin_lock_irqsave(&Adapter->QueueSpinLock, flags);
        Adapter->CurCmd = NULL;
        spin_unlock_irqrestore(&Adapter->QueueSpinLock, flags);
    }

    /* Release all the pending commands in queue */
    while (TRUE) {
        spin_lock_irqsave(&Adapter->QueueSpinLock, flags);
        if (list_empty(&Adapter->CmdPendingQ)) {
            spin_unlock_irqrestore(&Adapter->QueueSpinLock, flags);
            break;
        }
        CmdNode = (CmdCtrlNode *) Adapter->CmdPendingQ.next;
        spin_unlock_irqrestore(&Adapter->QueueSpinLock, flags);
        if (!CmdNode)
            break;
        list_del((struct list_head *) CmdNode);
        wlan_insert_cmd_to_free_q(priv, CmdNode);
    }

    LEAVE();
    return;
}

module_param(fw_name, charp, 0);
MODULE_PARM_DESC(fw_name, "Firmware name");

#ifdef MFG_CMD_SUPPORT
module_param(mfgmode, int, 0);
MODULE_PARM_DESC(mfgmode,
                 "0: Download normal firmware; 1: Download MFG firmware");
#endif /* MFG_CMD_SUPPORT */

#ifdef USB_DELAY
module_param(usb_ms_delay, int, 0);
#endif /* USB_DELAY */
