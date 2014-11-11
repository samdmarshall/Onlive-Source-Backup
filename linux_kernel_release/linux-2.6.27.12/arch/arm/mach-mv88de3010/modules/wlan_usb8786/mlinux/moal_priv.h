/** @file moal_priv.h
 *
 * @brief This file contains definition for extended private IOCTL call.
 *  
 * Copyright (C) 2008-2009, Marvell International Ltd.  
 *  All Rights Reserved
 */

/********************************************************
Change log:
    10/31/2008: initial version
********************************************************/

#ifndef _WOAL_PRIV_H_
#define _WOAL_PRIV_H_

/** Offset for subcommand */
#define SUBCMD_OFFSET               4

/** Command disabled */
#define	CMD_DISABLED				0
/** Command enabled */
#define	CMD_ENABLED				1
/** Command get */
#define	CMD_GET					2

/** HostCmd_Header */
typedef struct _HostCmd_Header
{
    /** Command */
    t_u16 command;
    /** Size */
    t_u16 size;
} HostCmd_Header;

/** RF antenna select 1 */
#define RF_ANTENNA_1		0x1
/** RF antenna select 2 */
#define RF_ANTENNA_2		0x2
/** RF antenna auto select */
#define RF_ANTENNA_AUTO		0xFFFF
/** 2K bytes */
#define WOAL_2K_BYTES       2000

/** PRIVATE CMD ID */
#define WOAL_IOCTL                  0x8BE0

/** Private command ID to set one int/get word char */
#define WOAL_SETONEINT_GETWORDCHAR  (WOAL_IOCTL + 1)
/** Private command ID to get version */
#define WOAL_VERSION                1
/** Private command ID to get extended version */
#define WOAL_VEREXT                 2

/** Private command ID to set/get none */
#define WOAL_SETNONE_GETNONE        (WOAL_IOCTL + 2)
/** Private command ID for warm reset */
#define WOAL_WARMRESET              1

/** Private command ID to set/get sixteen int */
#define WOAL_SET_GET_SIXTEEN_INT    (WOAL_IOCTL + 3)
/** Private command ID to set/get TX power configurations */
#define WOAL_TX_POWERCFG            1
#ifdef DEBUG_LEVEL1
/** Private command ID to set/get driver debug */
#define WOAL_DRV_DBG                2
#endif
/** Private command ID to set/get beacon interval */
#define WOAL_BEACON_INTERVAL        3
/** Private command ID to set/get ATIM window */
#define WOAL_ATIM_WINDOW            4
/** Private command ID to get RSSI */
#define WOAL_SIGNAL                 5
/** Private command ID to enable/disable heart beat */
#define WOAL_HEART_BEAT             6
/** Private command ID for 11n ht configration */
#define WOAL_11N_TX_CFG             8
/** Private command ID for 11n usr ht configration */
#define WOAL_11N_HTCAP_CFG          9

/** Private command ID for TX Aggregation */
#define WOAL_PRIO_TBL               10
/** Private command ID for Updating ADDBA variables */
#define WOAL_ADDBA_UPDT             11
/** Private command ID to set/get Host Sleep configuration */
#define WOAL_HS_CFG                 12
/** Private command ID to set Host Sleep parameters */
#define WOAL_HS_SETPARA             13
/** Private command ID to read/write registers */
#define WOAL_REG_READ_WRITE         14
/** Private command ID to set/get band/adhocband */
#define WOAL_BAND_CFG               15
/** Private command ID for TX Aggregation */
#define WOAL_11N_AMSDU_AGGR_CTRL    17
/** Private command ID to set/get Inactivity timeout */
#define WOAL_INACTIVITY_TIMEOUT_EXT 18
/** Private command ID to set/get scan configuration parameter */
#define WOAL_SCAN_CFG               19
/** Private command ID for Updating ADDBA variables */
#define WOAL_ADDBA_REJECT           20

#ifdef MFG_CMD_SUPPORT
/** Private command ID for MFG command support */
#define	WOAL_MFG_CMD                (WOAL_IOCTL + 4)
#endif

/** Private command ID to set one int/get one int */
#define WOAL_SETONEINT_GETONEINT    (WOAL_IOCTL + 5)
/** Private command ID to set/get Tx rate */
#define WOAL_SET_GET_TXRATE         1
/** Private command ID to set/get region code */
#define WOAL_SET_GET_REGIONCODE     2
/** Private command ID to turn on/off radio */
#define WOAL_SET_RADIO              3
/** Private command ID to enable WMM */
#define WOAL_WMM_ENABLE	    	    4
/** Private command ID to enable 802.11D */
#define WOAL_11D_ENABLE	    	    5
/** Private command ID to set/get tx/rx antenna */
#define WOAL_SET_GET_TX_RX_ANT      6
/** Private command ID to set/get QoS configuration */
#define WOAL_SET_GET_QOS_CFG        7
#ifdef REASSOCIATION
/** Private command ID to set/get reassociation setting */
#define WOAL_SET_GET_REASSOC        9
#endif /* REASSOCIATION */
/** Private command ID for Updating Transmit buffer configration */
#define WOAL_TXBUF_CFG              10
/** Private command ID to set/get WWS mode */
#define	WOAL_SET_GET_WWS_CFG        11
/** Private command ID to set/get auth type */
#define WOAL_AUTH_TYPE              12

/** Private command ID to get log */
#define WOALGETLOG                  (WOAL_IOCTL + 7)

/** Private command ID to set a wext address variable */
#define WOAL_SETADDR_GETNONE        (WOAL_IOCTL + 8)
/** Private command ID to send deauthentication */
#define WOAL_DEAUTH                 1

/** Private command to get/set 256 chars */
#define WOAL_SET_GET_256_CHAR       (WOAL_IOCTL + 9)
/** Private command to read/write passphrase */
#define WOAL_PASSPHRASE             1
/** Private command to get/set Ad-Hoc AES */
#define WOAL_ADHOC_AES              2

/** Get log buffer size */
#define GETLOG_BUFSIZE              512

/** Private command ID to set none/get tewlve chars*/
#define WOAL_SETNONE_GETTWELVE_CHAR (WOAL_IOCTL + 11)
/** Private command ID for WPS session */
#define WOAL_WPS_SESSION            1

/** Private command ID to set none/get four int */
#define WOAL_SETNONE_GET_FOUR_INT   (WOAL_IOCTL + 13)
/** Private command ID to get data rates */
#define WOAL_DATA_RATE              1
/** Private command ID to get E-Supplicant mode */
#define WOAL_ESUPP_MODE             2

/** Private command to get/set 64 ints */
#define WOAL_SET_GET_64_INT         (WOAL_IOCTL + 15)
/** Private command ID to set/get ECL system clock */
#define WOAL_ECL_SYS_CLOCK          1

/** Private command ID for hostcmd */
#define WOAL_HOST_CMD               (WOAL_IOCTL + 17)

/** Private command ID for arpfilter */
#define WOAL_ARP_FILTER             (WOAL_IOCTL + 19)

/** Private command ID to set ints and get chars */
#define WOAL_SET_INTS_GET_CHARS     (WOAL_IOCTL + 21)
/** Private command ID to read EEPROM data */
#define WOAL_READ_EEPROM            1

/** Private command ID to set/get 2K bytes */
#define WOAL_SET_GET_2K_BYTES       (WOAL_IOCTL + 23)
/** Private command ID for setuserscan */
#define WOAL_SET_USER_SCAN          1
/** Private command ID for getscantable */
#define WOAL_GET_SCAN_TABLE         2
/** Private command ID for vsiecfg */
#define WOAL_VSIE_CFG               3

/**
 * iwpriv ioctl handlers
 */
static const struct iw_priv_args woal_private_args[] = {
    {
     WOAL_SETONEINT_GETWORDCHAR,
     IW_PRIV_TYPE_INT | 1,
     IW_PRIV_TYPE_CHAR | 128,
     ""},
    {
     WOAL_VERSION,
     IW_PRIV_TYPE_INT | 1,
     IW_PRIV_TYPE_CHAR | 128,
     "version"},
    {
     WOAL_VEREXT,
     IW_PRIV_TYPE_INT | 1,
     IW_PRIV_TYPE_CHAR | 128,
     "verext"},
    {
     WOAL_SETNONE_GETNONE,
     IW_PRIV_TYPE_NONE,
     IW_PRIV_TYPE_NONE,
     ""},
    {
     WOAL_WARMRESET,
     IW_PRIV_TYPE_NONE,
     IW_PRIV_TYPE_NONE,
     "warmreset"},
    {
     WOAL_SETONEINT_GETONEINT,
     IW_PRIV_TYPE_INT | 1,
     IW_PRIV_TYPE_INT | 1,
     ""},
    {
     WOAL_SET_GET_TXRATE,
     IW_PRIV_TYPE_INT | 1,
     IW_PRIV_TYPE_INT | 1,
     "txratecfg"},
    {
     WOAL_SET_GET_REGIONCODE,
     IW_PRIV_TYPE_INT | 1,
     IW_PRIV_TYPE_INT | 1,
     "regioncode"},
    {
     WOAL_SET_RADIO,
     IW_PRIV_TYPE_INT | 1,
     IW_PRIV_TYPE_INT | 1,
     "radioctrl"},
    {
     WOAL_WMM_ENABLE,
     IW_PRIV_TYPE_INT | 1,
     IW_PRIV_TYPE_INT | 1,
     "wmmcfg"},
    {
     WOAL_11D_ENABLE,
     IW_PRIV_TYPE_INT | 1,
     IW_PRIV_TYPE_INT | 1,
     "11dcfg"},
    {
     WOAL_SET_GET_TX_RX_ANT,
     IW_PRIV_TYPE_INT | 1,
     IW_PRIV_TYPE_INT | 1,
     "antcfg"},
    {
     WOAL_SET_GET_QOS_CFG,
     IW_PRIV_TYPE_INT | 1,
     IW_PRIV_TYPE_INT | 1,
     "qoscfg"},
    {
     WOAL_SET_GET_WWS_CFG,
     IW_PRIV_TYPE_INT | 1,
     IW_PRIV_TYPE_INT | 1,
     "wwscfg"},
#ifdef REASSOCIATION
    {
     WOAL_SET_GET_REASSOC,
     IW_PRIV_TYPE_INT | 1,
     IW_PRIV_TYPE_INT | 1,
     "reassoctrl"},
#endif
    {
     WOAL_TXBUF_CFG,
     IW_PRIV_TYPE_INT | 1,
     IW_PRIV_TYPE_INT | 1,
     "txbufcfg"},
    {
     WOAL_AUTH_TYPE,
     IW_PRIV_TYPE_INT | 1,
     IW_PRIV_TYPE_INT | 1,
     "authtype"},
    {
     WOAL_SET_GET_SIXTEEN_INT,
     IW_PRIV_TYPE_INT | 16,
     IW_PRIV_TYPE_INT | 16,
     ""},
    {
     WOAL_TX_POWERCFG,
     IW_PRIV_TYPE_INT | 16,
     IW_PRIV_TYPE_INT | 16,
     "txpowercfg"},
#ifdef DEBUG_LEVEL1
    {
     WOAL_DRV_DBG,
     IW_PRIV_TYPE_INT | 16,
     IW_PRIV_TYPE_INT | 16,
     "drvdbg"},
#endif
    {
     WOAL_BEACON_INTERVAL,
     IW_PRIV_TYPE_INT | 16,
     IW_PRIV_TYPE_INT | 16,
     "bcninterval"},
    {
     WOAL_ATIM_WINDOW,
     IW_PRIV_TYPE_INT | 16,
     IW_PRIV_TYPE_INT | 16,
     "atimwindow"},
    {
     WOAL_SIGNAL,
     IW_PRIV_TYPE_INT | 16,
     IW_PRIV_TYPE_INT | 16,
     "getsignal"},
    {
     WOAL_HEART_BEAT,
     IW_PRIV_TYPE_INT | 16,
     IW_PRIV_TYPE_INT | 16,
     "heartbeat"},
    {
     WOAL_11N_TX_CFG,
     IW_PRIV_TYPE_INT | 16,
     IW_PRIV_TYPE_INT | 16,
     "httxcfg"},
    {
     WOAL_11N_HTCAP_CFG,
     IW_PRIV_TYPE_INT | 16,
     IW_PRIV_TYPE_INT | 16,
     "htcapinfo"},
    {
     WOAL_PRIO_TBL,
     IW_PRIV_TYPE_INT | 16,
     IW_PRIV_TYPE_INT | 16,
     "aggrpriotbl"},
    {
     WOAL_11N_AMSDU_AGGR_CTRL,
     IW_PRIV_TYPE_INT | 16,
     IW_PRIV_TYPE_INT | 16,
     "amsduaggrctrl"},
    {
     WOAL_ADDBA_UPDT,
     IW_PRIV_TYPE_INT | 16,
     IW_PRIV_TYPE_INT | 16,
     "addbapara"},
    {
     WOAL_ADDBA_REJECT,
     IW_PRIV_TYPE_INT | 16,
     IW_PRIV_TYPE_INT | 16,
     "addbareject"},
    {
     WOAL_HS_CFG,
     IW_PRIV_TYPE_INT | 16,
     IW_PRIV_TYPE_INT | 16,
     "hscfg"},
    {
     WOAL_HS_SETPARA,
     IW_PRIV_TYPE_INT | 16,
     IW_PRIV_TYPE_INT | 16,
     "hssetpara"},
    {
     WOAL_REG_READ_WRITE,
     IW_PRIV_TYPE_INT | 16,
     IW_PRIV_TYPE_INT | 16,
     "regrdwr"},
    {
     WOAL_BAND_CFG,
     IW_PRIV_TYPE_INT | 16,
     IW_PRIV_TYPE_INT | 16,
     "bandcfg"},
    {
     WOAL_INACTIVITY_TIMEOUT_EXT,
     IW_PRIV_TYPE_INT | 16,
     IW_PRIV_TYPE_INT | 16,
     "inactivityto"},
    {
     WOAL_SCAN_CFG,
     IW_PRIV_TYPE_INT | 16,
     IW_PRIV_TYPE_INT | 16,
     "scancfg"},
    {
     WOALGETLOG,
     IW_PRIV_TYPE_NONE,
     IW_PRIV_TYPE_CHAR | GETLOG_BUFSIZE,
     "getlog"},
    {
     WOAL_SETADDR_GETNONE,
     IW_PRIV_TYPE_ADDR | 1,
     IW_PRIV_TYPE_NONE,
     ""},
    {
     WOAL_DEAUTH,
     IW_PRIV_TYPE_ADDR | 1,
     IW_PRIV_TYPE_NONE,
     "deauth"},
    {
     WOAL_SET_GET_256_CHAR,
     IW_PRIV_TYPE_CHAR | 256,
     IW_PRIV_TYPE_CHAR | 256,
     ""},
    {
     WOAL_PASSPHRASE,
     IW_PRIV_TYPE_CHAR | 256,
     IW_PRIV_TYPE_CHAR | 256,
     "passphrase"},
    {
     WOAL_ADHOC_AES,
     IW_PRIV_TYPE_CHAR | 256,
     IW_PRIV_TYPE_CHAR | 256,
     "adhocaes"},
    {
     WOAL_SETNONE_GETTWELVE_CHAR,
     IW_PRIV_TYPE_NONE,
     IW_PRIV_TYPE_CHAR | 12,
     ""},
    {
     WOAL_WPS_SESSION,
     IW_PRIV_TYPE_NONE,
     IW_PRIV_TYPE_CHAR | 12,
     "wpssession"},
    {
     WOAL_SETNONE_GET_FOUR_INT,
     IW_PRIV_TYPE_NONE,
     IW_PRIV_TYPE_INT | 4,
     ""},
    {
     WOAL_DATA_RATE,
     IW_PRIV_TYPE_NONE,
     IW_PRIV_TYPE_INT | 4,
     "getdatarate"},
    {
     WOAL_ESUPP_MODE,
     IW_PRIV_TYPE_NONE,
     IW_PRIV_TYPE_INT | 4,
     "esuppmode"},
    {
     WOAL_SET_GET_64_INT,
     IW_PRIV_TYPE_INT | 64,
     IW_PRIV_TYPE_INT | 64,
     ""},
    {
     WOAL_ECL_SYS_CLOCK,
     IW_PRIV_TYPE_INT | 64,
     IW_PRIV_TYPE_INT | 64,
     "sysclock"},
    {
     WOAL_HOST_CMD,
     IW_PRIV_TYPE_BYTE | 2047,
     IW_PRIV_TYPE_BYTE | 2047,
     "hostcmd"},
    {
     WOAL_ARP_FILTER,
     IW_PRIV_TYPE_BYTE | 2047,
     IW_PRIV_TYPE_BYTE | 2047,
     "arpfilter"},
    {
     WOAL_SET_INTS_GET_CHARS,
     IW_PRIV_TYPE_INT | 16,
     IW_PRIV_TYPE_BYTE | 256,
     ""},
    {
     WOAL_READ_EEPROM,
     IW_PRIV_TYPE_INT | 16,
     IW_PRIV_TYPE_BYTE | 256,
     "rdeeprom"},
    {
     WOAL_SET_GET_2K_BYTES,
     IW_PRIV_TYPE_BYTE | WOAL_2K_BYTES,
     IW_PRIV_TYPE_BYTE | WOAL_2K_BYTES,
     ""},
    {
     WOAL_SET_USER_SCAN,
     IW_PRIV_TYPE_BYTE | WOAL_2K_BYTES,
     IW_PRIV_TYPE_BYTE | WOAL_2K_BYTES,
     "setuserscan"},
    {
     WOAL_GET_SCAN_TABLE,
     IW_PRIV_TYPE_BYTE | WOAL_2K_BYTES,
     IW_PRIV_TYPE_BYTE | WOAL_2K_BYTES,
     "getscantable"},
    {
     WOAL_VSIE_CFG,
     IW_PRIV_TYPE_BYTE | WOAL_2K_BYTES,
     IW_PRIV_TYPE_BYTE | WOAL_2K_BYTES,
     "vsiecfg"},
};

int woal_do_ioctl(struct net_device *dev, struct ifreq *req, int i);
#endif /* _WOAL_PRIV_H_ */
