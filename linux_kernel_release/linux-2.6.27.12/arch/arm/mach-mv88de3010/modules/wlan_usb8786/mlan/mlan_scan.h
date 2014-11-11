/** @file mlan_scan.h
 *
 *  @brief This file defines the interface for the WLAN network scan routines.
 *
 *  Driver interface functions and type declarations for the scan module
 *  implemented in mlan_scan.c.
 *    
 *  Copyright (C) 2008-2009, Marvell International Ltd. 
 *  All Rights Reserved
 *
 *  @sa mlan_scan.c
 */

/******************************************************
Change log:
    10/13/2008: initial version
******************************************************/

#ifndef _MLAN_SCAN_H
#define _MLAN_SCAN_H

/** Maximum number of channels that can be sent in user scan config */
#define WLAN_USER_SCAN_CHAN_MAX             50

/** Infrastructure BSS scan type in wlan_scan_cmd_config */
#define WLAN_SCAN_BSS_TYPE_BSS              1

/** Adhoc BSS scan type in wlan_scan_cmd_config */
#define WLAN_SCAN_BSS_TYPE_IBSS             2

/** Adhoc or Infrastructure BSS scan type in wlan_scan_cmd_config, no filter */
#define WLAN_SCAN_BSS_TYPE_ANY              3

/** Scan time specified in the channel TLV for each channel for passive scans */
#define MRVDRV_PASSIVE_SCAN_CHAN_TIME       200

/** Scan time specified in the channel TLV for each channel for active scans */
#define MRVDRV_ACTIVE_SCAN_CHAN_TIME        200

/** Scan time specified in the channel TLV for each channel for specific scans */
#define MRVDRV_SPECIFIC_SCAN_CHAN_TIME      110

/**
 * Max total scan time in milliseconds
 * The total scan time should be less than scan command timeout value (10s)
 */
#define MRVDRV_MAX_TOTAL_SCAN_TIME     (MRVDRV_TIMER_10S - MRVDRV_TIMER_1S)

/** Maximum length of SSID list */
#define MRVDRV_MAX_SSID_LIST_LENGTH         10

/**
 * @brief Structure used internally in the wlan driver to configure a scan.
 *
 * Sent to the command process module to configure the firmware
 *   scan command prepared by wlan_cmd_802_11_scan.
 *
 * @sa wlan_scan_networks
 *
 */
typedef struct _wlan_scan_cmd_config
{
    /**
     *  BSS Type to be sent in the firmware command
     *
     *  Field can be used to restrict the types of networks returned in the
     *    scan.  Valid settings are:
     *
     *   - WLAN_SCAN_BSS_TYPE_BSS  (infrastructure)
     *   - WLAN_SCAN_BSS_TYPE_IBSS (adhoc)
     *   - WLAN_SCAN_BSS_TYPE_ANY  (unrestricted, adhoc and infrastructure)
     */
    t_u8 bss_type;

    /**
     *  Specific BSSID used to filter scan results in the firmware
     */
    t_u8 specific_bssid[MLAN_MAC_ADDR_LENGTH];

    /**
     *  Length of TLVs sent in command starting at tlvBuffer
     */
    t_u32 tlv_buf_len;

    /**
     *  SSID TLV(s) and ChanList TLVs to be sent in the firmware command
     *
     *  TLV_TYPE_CHANLIST, MrvlIEtypes_ChanListParamSet_t
     *  TLV_TYPE_SSID, MrvlIEtypes_SsIdParamSet_t
     */
    t_u8 tlv_buf[1];            /* SSID TLV(s) and ChanList TLVs are stored
                                   here */
} __attribute__ ((packed)) wlan_scan_cmd_config;

/**
 *  @brief IOCTL channel sub-structure sent in wlan_ioctl_user_scan_cfg
 *
 *  Multiple instances of this structure are included in the IOCTL command
 *   to configure a instance of a scan on the specific channel.
 */
typedef struct _wlan_user_scan_chan
{
    /** Channel Number to scan */
    t_u8 chan_number;
    /** Radio type: 'B/G' Band = 0, 'A' Band = 1 */
    t_u8 radio_type;
    /** Scan type: Active = 0, Passive = 1 */
    t_u8 scan_type;
    /** Reserved */
    t_u8 reserved;
    /** Scan duration in milliseconds; if 0 default used */
    t_u32 scan_time;
} __attribute__ ((packed)) wlan_user_scan_chan;

/**
 *  IOCTL SSID List sub-structure sent in wlan_ioctl_user_scan_cfg
 * 
 *  Used to specify SSID specific filters as well as SSID pattern matching
 *    filters for scan result processing in firmware.
 */
typedef struct _wlan_user_scan_ssid
{
    /** SSID */
    t_u8 ssid[MLAN_MAX_SSID_LENGTH + 1];
    /** Maximum length of SSID */
    t_u8 max_len;
} __attribute__ ((packed)) wlan_user_scan_ssid;

/**
 *  Input structure to configure an immediate scan cmd to firmware
 *
 *  Specifies a number of parameters to be used in general for the scan 
 *    as well as a channel list (wlan_user_scan_chan) for each scan period
 *    desired.
 */
typedef struct
{
    /**
     *  Flag set to keep the previous scan table intact
     *
     *  If set, the scan results will accumulate, replacing any previous
     *   matched entries for a BSS with the new scan data
     */
    t_u8 keep_previous_scan;
    /**
     *  BSS Type to be sent in the firmware command
     *
     *  Field can be used to restrict the types of networks returned in the
     *    scan.  Valid settings are:
     *
     *   - WLAN_SCAN_BSS_TYPE_BSS  (infrastructure)
     *   - WLAN_SCAN_BSS_TYPE_IBSS (adhoc)
     *   - WLAN_SCAN_BSS_TYPE_ANY  (unrestricted, adhoc and infrastructure)
     */
    t_u8 bss_type;
    /**
     *  Configure the number of probe requests for active chan scans
     */
    t_u8 num_probes;
    /**
     *  @brief Reserved
     */
    t_u8 reserved;
    /**
     *  @brief BSSID filter sent in the firmware command to limit the results
     */
    t_u8 specific_bssid[MLAN_MAC_ADDR_LENGTH];
    /**
     *  SSID filter list used in the to limit the scan results
     */
    wlan_user_scan_ssid ssid_list[MRVDRV_MAX_SSID_LIST_LENGTH];
    /**
     *  Variable number (fixed maximum) of channels to scan up
     */
    wlan_user_scan_chan chan_list[WLAN_USER_SCAN_CHAN_MAX];
} __attribute__ ((packed)) wlan_user_scan_cfg;

/**
 *  Sructure to retrieve the scan table
 */
typedef struct
{
    /**
     *  - Zero based scan entry to start retrieval in command request
     *  - Number of scans entries returned in command response
     */
    t_u32 scan_number;
    /**
     * Buffer marker for multiple wlan_ioctl_get_scan_table_entry structures.
     *   Each struct is padded to the nearest 32 bit boundary.
     */
    t_u8 scan_table_entry_buf[1];
} __attribute__ ((packed)) wlan_get_scan_table_info;

/** Offset for GTK as it has version to skip past for GTK */
#define RSN_GTK_OUI_OFFSET 2

/** If OUI is not found */
#define MLAN_OUI_NOT_PRESENT 0
/** If OUI is found */
#define MLAN_OUI_PRESENT 1

/** Generic structure defined for parsing WPA/RSN IEs for GTK/PTK OUIs */
typedef struct
{
        /** Group key oui */
    t_u8 GrpKeyOui[4];
        /** Number of PTKs */
    t_u8 PtkCnt[2];
        /** Ptk body starts here */
    t_u8 PtkBody[4];
} __attribute__ ((packed)) IEBody;

#endif /* _MLAN_SCAN_H */
