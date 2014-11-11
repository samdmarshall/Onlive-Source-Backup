/** @file wlanconfig.h
 * 
 * @brief This file contains definitions for application
 *
 * Copyright (C) 2003-2008, Marvell International Ltd. 
 * All Rights Reserved
 */
/****************************************************************
Change log:
	09/26/05: add Doxygen format comments
****************************************************************/

#ifndef _WLANCONFIG_H_
#define _WLANCONFIG_H_

/** A NULL BSSID */
#define NULLBSSID		"\x00\x00\x00\x00\x00\x00"

/** Calculate actual length of the TLV */
#define mrvl_tlv_len(x) ((x)->Header.Len + sizeof(MrvlIEtypesHeader_t))

/** to create pointers to 6-byte hardware address */
#define HWA_ARG(x)		*(((u8 *)x + 0)), *(((u8 *)x + 1)), \
				*(((u8 *)x + 2)), *(((u8 *)x + 3)), \
				*(((u8 *)x + 4)), *(((u8 *)x + 5))

/** Signifies encryption disabled */
#define WCON_ENC_DISABLED	0
/** Signifies encryption enables */
#define WCON_ENC_ENABLED	1

/** Signifies WPA disabled */
#define WCON_WPA_DISABLED	0
/** Signifies WPA enabled */
#define WCON_WPA_ENABLED	1

/** Signifies WMM disabled */
#define WCON_WMM_DISABLED	0
/** Signifies WMM enabled */
#define WCON_WMM_ENABLED	1

/** struct of SSID network name */
typedef struct _WCON_SSID
{
        /** SSID name length */
    u32 ssid_len;
        /** SSID name string */
    u8 ssid[IW_ESSID_MAX_SIZE + 1];
} WCON_SSID;

/** BSSID network name */
typedef u8 WCON_BSSID[ETH_ALEN];

/** struct of SSID network information */
typedef struct _WCON_NET_INFO
{
        /** SSID network name struct */
    WCON_SSID Ssid;
        /** hardware address of the SSID network */
    WCON_BSSID Bssid;
        /** rssi value */
    unsigned int Rssi;
        /**  network operating mode */
    int NetMode;
        /** network privacy mode */
    int Privacy;
        /** WPA enable */
    int WpaAP;
        /** WMM enable */
    int Wmm;
} WCON_NET_INFO;

/** struct of SSID list from scan */
typedef struct _WCON_HANDLE
{
        /** list of scan result */
    WCON_NET_INFO ScanList[IW_MAX_AP];
        /** Number of APs found */
    int ApNum;
} WCON_HANDLE;

#endif /* _WLANCONFIG_H_ */
