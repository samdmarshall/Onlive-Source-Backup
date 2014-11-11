/** @file mlan_fw.h
 *
 *  @brief This file contains firmware specific defines.
 *  structures and declares global function prototypes used
 *  in MLAN module.
 *
 *  Copyright (C) 2008-2009, Marvell International Ltd. 
 *  All Rights Reserved
 */

/******************************************************
Change log:
    10/27/2008: initial version
******************************************************/

#ifndef _MLAN_FW_H_
#define _MLAN_FW_H_

/** Rates supported in band B */
#define B_SUPPORTED_RATES               5
/** Rates supported in band G */
#define G_SUPPORTED_RATES               9
/** Rates supported in band BG */
#define BG_SUPPORTED_RATES              13

/** Setup the number of rates passed in the driver/firmware API.*/
#define HOSTCMD_SUPPORTED_RATES         14

/** Rates supported in band N */
#define N_SUPPORTED_RATES               3
/** All bands (B, G, N) */
#define ALL_802_11_BANDS        (BAND_B | BAND_G | BAND_GN)

/** Firmware multiple bands support */
#define FW_MULTI_BANDS_SUPPORT  (MBIT(8) | MBIT(9) | MBIT(10) | MBIT(11))
/** Check if multiple bands support is enabled in firmware */
#define IS_SUPPORT_MULTI_BANDS(_adapter)        \
        (_adapter->fw_cap_info & FW_MULTI_BANDS_SUPPORT)
/** Get default bands of the firmware */
#define GET_FW_DEFAULT_BANDS(_adapter)  \
        ((_adapter->fw_cap_info >> 8) & ALL_802_11_BANDS)

extern t_u8 SupportedRates_B[B_SUPPORTED_RATES];
extern t_u8 SupportedRates_G[G_SUPPORTED_RATES];
extern t_u8 SupportedRates_BG[BG_SUPPORTED_RATES];
extern t_u8 SupportedRates_N[N_SUPPORTED_RATES];
extern t_u8 AdhocRates_G[G_SUPPORTED_RATES];
extern t_u8 AdhocRates_B[B_SUPPORTED_RATES];
extern t_u8 AdhocRates_BG[BG_SUPPORTED_RATES];

/** WEP Key index mask */
#define HostCmd_WEP_KEY_INDEX_MASK              0x3fff

/** Key information enabled */
#define KEY_INFO_ENABLED        0x01
/** KEY_TYPE_ID */
typedef enum _KEY_TYPE_ID
{
    /** Key type : WEP */
    KEY_TYPE_ID_WEP = 0,
    /** Key type : TKIP */
    KEY_TYPE_ID_TKIP,
    /** Key type : AES */
    KEY_TYPE_ID_AES
} KEY_TYPE_ID;

/** KEY_INFO_WEP*/
typedef enum _KEY_INFO_WEP
{
    KEY_INFO_WEP_MCAST = 0x01,
    KEY_INFO_WEP_UNICAST = 0x02,
    KEY_INFO_WEP_ENABLED = 0x04
} KEY_INFO_WEP;

/** KEY_INFO_TKIP */
typedef enum _KEY_INFO_TKIP
{
    KEY_INFO_TKIP_MCAST = 0x01,
    KEY_INFO_TKIP_UNICAST = 0x02,
    KEY_INFO_TKIP_ENABLED = 0x04
} KEY_INFO_TKIP;

/** KEY_INFO_AES*/
typedef enum _KEY_INFO_AES
{
    KEY_INFO_AES_MCAST = 0x01,
    KEY_INFO_AES_UNICAST = 0x02,
    KEY_INFO_AES_ENABLED = 0x04
} KEY_INFO_AES;

/** WPA AES key length */
#define WPA_AES_KEY_LEN                 16
/** WPA TKIP key length */
#define WPA_TKIP_KEY_LEN                32

/** Enumeration definition*/
/** WLAN_802_11_PRIVACY_FILTER */
typedef enum _WLAN_802_11_PRIVACY_FILTER
{
    Wlan802_11PrivFilterAcceptAll,
    Wlan802_11PrivFilter8021xWEP
} WLAN_802_11_PRIVACY_FILTER;

/** WLAN_802_11_WEP_STATUS */
typedef enum _WLAN_802_11_WEP_STATUS
{
    Wlan802_11WEPEnabled,
    Wlan802_11WEPDisabled,
    Wlan802_11WEPKeyAbsent,
    Wlan802_11WEPNotSupported
} WLAN_802_11_WEP_STATUS;

/** SNR calculation */
#define CAL_SNR(RSSI, NF)    ((t_s16)((t_s16)(RSSI)-(t_s16)(NF)))

/** TLV  type ID definition */
#define PROPRIETARY_TLV_BASE_ID                 0x0100

/** Terminating TLV Type */
#define MRVL_TERMINATE_TLV_ID                   0xffff

/** TLV type : SSID */
#define TLV_TYPE_SSID                           0x0000
/** TLV type : Rates */
#define TLV_TYPE_RATES                          0x0001
/** TLV type : PHY FH */
#define TLV_TYPE_PHY_FH                         0x0002
/** TLV type : PHY DS */
#define TLV_TYPE_PHY_DS                         0x0003
/** TLV type : CF */
#define TLV_TYPE_CF                             0x0004
/** TLV type : IBSS */
#define TLV_TYPE_IBSS                           0x0006

/** TLV type : Domain */
#define TLV_TYPE_DOMAIN                         0x0007

/** TLV type : Power capability */
#define TLV_TYPE_POWER_CAPABILITY               0x0021

/** TLV type : Key material */
#define TLV_TYPE_KEY_MATERIAL       (PROPRIETARY_TLV_BASE_ID + 0)
/** TLV type : Channel list */
#define TLV_TYPE_CHANLIST           (PROPRIETARY_TLV_BASE_ID + 1)
/** TLV type : Number of probes */
#define TLV_TYPE_NUMPROBES          (PROPRIETARY_TLV_BASE_ID + 2)
/** TLV type : Beacon RSSI low */
#define TLV_TYPE_RSSI_LOW           (PROPRIETARY_TLV_BASE_ID + 4)
/** TLV type : Beacon SNR low */
#define TLV_TYPE_SNR_LOW            (PROPRIETARY_TLV_BASE_ID + 5)
/** TLV type : Fail count */
#define TLV_TYPE_FAILCOUNT          (PROPRIETARY_TLV_BASE_ID + 6)
/** TLV type : BCN miss */
#define TLV_TYPE_BCNMISS            (PROPRIETARY_TLV_BASE_ID + 7)
/** TLV type : LED behavior */
#define TLV_TYPE_LEDBEHAVIOR        (PROPRIETARY_TLV_BASE_ID + 9)
/** TLV type : Passthrough */
#define TLV_TYPE_PASSTHROUGH        (PROPRIETARY_TLV_BASE_ID + 10)
/** TLV type : Power TBL 2.4 Ghz */
#define TLV_TYPE_POWER_TBL_2_4GHZ   (PROPRIETARY_TLV_BASE_ID + 12)
/** TLV type : Power TBL 5 GHz */
#define TLV_TYPE_POWER_TBL_5GHZ     (PROPRIETARY_TLV_BASE_ID + 13)
/** TLV type : WMM queue status */
#define TLV_TYPE_WMMQSTATUS         (PROPRIETARY_TLV_BASE_ID + 16)
/** TLV type : Wildcard SSID */
#define TLV_TYPE_WILDCARDSSID       (PROPRIETARY_TLV_BASE_ID + 18)
/** TLV type : TSF timestamp */
#define TLV_TYPE_TSFTIMESTAMP       (PROPRIETARY_TLV_BASE_ID + 19)
/** TLV type : Beacon RSSI high */
#define TLV_TYPE_RSSI_HIGH          (PROPRIETARY_TLV_BASE_ID + 22)
/** TLV type : Beacon SNR high */
#define TLV_TYPE_SNR_HIGH           (PROPRIETARY_TLV_BASE_ID + 23)

/** TLV type : Start BG scan later */
#define TLV_TYPE_STARTBGSCANLATER   (PROPRIETARY_TLV_BASE_ID + 30)
/** TLV type : Authentication type */
#define TLV_TYPE_AUTH_TYPE          (PROPRIETARY_TLV_BASE_ID + 31)
/** TLV type : Link Quality */
#define TLV_TYPE_LINK_QUALITY       (PROPRIETARY_TLV_BASE_ID + 36)
/** TLV type : Data RSSI low */
#define TLV_TYPE_RSSI_LOW_DATA      (PROPRIETARY_TLV_BASE_ID + 38)
/** TLV type : Data SNR low */
#define TLV_TYPE_SNR_LOW_DATA       (PROPRIETARY_TLV_BASE_ID + 39)
/** TLV type : Data RSSI high */
#define TLV_TYPE_RSSI_HIGH_DATA     (PROPRIETARY_TLV_BASE_ID + 40)
/** TLV type : Data SNR high */
#define TLV_TYPE_SNR_HIGH_DATA      (PROPRIETARY_TLV_BASE_ID + 41)

/** TLV type : Channel band list */
#define TLV_TYPE_CHANNELBANDLIST    (PROPRIETARY_TLV_BASE_ID + 42)

/** TLV type : Encryption Protocol TLV */
#define TLV_TYPE_ENCRYPTION_PROTO   (PROPRIETARY_TLV_BASE_ID + 64)
/** TLV type : Cipher TLV */
#define TLV_TYPE_CIPHER             (PROPRIETARY_TLV_BASE_ID + 66)
/** TLV type : PMK */
#define TLV_TYPE_PMK                (PROPRIETARY_TLV_BASE_ID + 68)
/** TLV type : Passphrase */
#define TLV_TYPE_PASSPHRASE         (PROPRIETARY_TLV_BASE_ID + 60)
/** TLV type : BSSID */
#define TLV_TYPE_BSSID              (PROPRIETARY_TLV_BASE_ID + 35)
/** TLV type : HT Capabilities */
#define TLV_TYPE_HT_CAP                  (PROPRIETARY_TLV_BASE_ID + 74)
/** TLV type : HT Information */
#define TLV_TYPE_HT_INFO                 (PROPRIETARY_TLV_BASE_ID + 75)
/** TLV type : Secondary Channel Offset */
#define TLV_SECONDARY_CHANNEL_OFFSET     (PROPRIETARY_TLV_BASE_ID + 76)
/** TLV type : 20/40 BSS Coexistence */
#define TLV_TYPE_2040BSS_COEXISTENCE     (PROPRIETARY_TLV_BASE_ID + 77)
/** TLV type : Overlapping BSS Scan Parameters */
#define TLV_TYPE_OVERLAP_BSS_SCAN_PARAM  (PROPRIETARY_TLV_BASE_ID + 78)
/** TLV type : Extended capabilities */
#define TLV_TYPE_EXTCAP                  (PROPRIETARY_TLV_BASE_ID + 79)
/** TLV type : Set of MCS values that STA desires to use within the BSS */
#define TLV_TYPE_HT_OPERATIONAL_MCS_SET  (PROPRIETARY_TLV_BASE_ID + 80)

/** ADDBA TID mask */
#define ADDBA_TID_MASK   (MBIT(2) | MBIT(3) | MBIT(4) | MBIT(5))
/** DELBA TID mask */
#define DELBA_TID_MASK   (MBIT(12) | MBIT(13) | MBIT(14) | MBIT(15))
/** ADDBA Starting Sequence Number Mask */
#define SSN_MASK         0xfff0

/** Block Ack result status */
/** Block Ack Result : Success */
#define BA_RESULT_SUCCESS        0x0
/** Block Ack Result : Execution failure */
#define BA_RESULT_FAILURE        0x1
/** Block Ack Result : Timeout */
#define BA_RESULT_TIMEOUT        0x2
/** Block Ack Result : Data invalid */
#define BA_RESULT_DATA_INVALID   0x3

/** Get the baStatus (NOT_SETUP, COMPLETE, IN_PROGRESS)
 *  in Tx BA stream table */
#define IS_BASTREAM_SETUP(ptr)  (ptr->ba_status)

/** An AMPDU/AMSDU could be disallowed for certain TID. 0xff means
 *  no aggregation is enabled for the assigned TID */
#define BA_STREAM_NOT_ALLOWED   0xff

/** Test if 11n is enabled by checking the HTCap IE */
#define IS_11N_ENABLED(priv) ((priv->adapter->config_bands & BAND_GN) && priv->curr_bss_params.bss_descriptor.pht_cap)
/** Find out if we are the initiator or not */
#define INITIATOR_BIT(DelBAParamSet) (((DelBAParamSet) & \
                        MBIT(DELBA_INITIATOR_POS)) >> DELBA_INITIATOR_POS)

/** 2K buf size */
#define MLAN_TX_DATA_BUF_SIZE_2K        2048
/** 4K buf size */
#define MLAN_TX_DATA_BUF_SIZE_4K        4096
/** 8K buf size */
#define MLAN_TX_DATA_BUF_SIZE_8K        8192
/** Max Rx AMPDU Size */
#define MAX_RX_AMPDU_SIZE_64K   0x03
/** Non green field station */
#define NON_GREENFIELD_STAS     0x04

/** Greenfield support */
#define HWSPEC_GREENFIELD_SUPP	 MBIT(29)
/** Channel width 40Mhz support */
#define HWSPEC_RXSTBC_SUPP	 MBIT(26)
/** ShortGI @ 40Mhz support */
#define HWSPEC_SHORTGI40_SUPP	 MBIT(24)
/** ShortGI @ 20Mhz support */
#define HWSPEC_SHORTGI20_SUPP	 MBIT(23)
/** Channel width 40Mhz support */
#define HWSPEC_CHANBW40_SUPP	 MBIT(17)
/** 40Mhz intolarent enable */
#define CAPINFO_40MHZ_INTOLARENT MBIT(8)

/** Default 11n capability mask */
#define DEFAULT_11N_CAP_MASK	(HWSPEC_SHORTGI20_SUPP | HWSPEC_RXSTBC_SUPP)
/** Bits to ignore in hw_dev_cap as these bits are set in get_hw_spec */
#define IGN_HW_DEV_CAP		(CAPINFO_40MHZ_INTOLARENT)

/** HW_SPEC FwCapInfo */
#define ISSUPP_11NENABLED(FwCapInfo) (FwCapInfo & MBIT(11))

/** HW_SPEC Dot11nDevCap : MAX AMSDU supported */
#define ISSUPP_MAXAMSDU(Dot11nDevCap) (Dot11nDevCap & MBIT(31))
/** HW_SPEC Dot11nDevCap : Beamforming support */
#define ISSUPP_BEAMFORMING(Dot11nDevCap) (Dot11nDevCap & MBIT(30))
/** HW_SPEC Dot11nDevCap : Green field support */
#define ISSUPP_GREENFIELD(Dot11nDevCap) (Dot11nDevCap & MBIT(29))
/** HW_SPEC Dot11nDevCap : AMPDU support */
#define ISSUPP_AMPDU(Dot11nDevCap) (Dot11nDevCap & MBIT(28))
/** HW_SPEC Dot11nDevCap : MIMO PS support  */
#define ISSUPP_MIMOPS(Dot11nDevCap) (Dot11nDevCap & MBIT(27))
/** HW_SPEC Dot11nDevCap : Rx STBC support */
#define ISSUPP_RXSTBC(Dot11nDevCap) (Dot11nDevCap & MBIT(26))
/** HW_SPEC Dot11nDevCap : Tx STBC support */
#define ISSUPP_TXSTBC(Dot11nDevCap) (Dot11nDevCap & MBIT(25))
/** HW_SPEC Dot11nDevCap : Short GI @ 40Mhz support */
#define ISSUPP_SHORTGI40(Dot11nDevCap) (Dot11nDevCap & MBIT(24))
/** HW_SPEC Dot11nDevCap : Short GI @ 20Mhz support */
#define ISSUPP_SHORTGI20(Dot11nDevCap) (Dot11nDevCap & MBIT(23))
/** HW_SPEC Dot11nDevCap : Rx LDPC support */
#define ISSUPP_RXLDPC(Dot11nDevCap) (Dot11nDevCap & MBIT(22))
/** HW_SPEC Dot11nDevCap : Delayed ACK */
#define GET_DELAYEDBACK(Dot11nDevCap) (((Dot11nDevCap >> 20) & 0x03))
/** HW_SPEC Dot11nDevCap : Immediate ACK */
#define GET_IMMEDIATEBACK(Dot11nDevCap) (((Dot11nDevCap >> 18) & 0x03))
/** HW_SPEC Dot11nDevCap : Channel BW support @ 40Mhz  support */
#define ISSUPP_CHANWIDTH40(Dot11nDevCap) (Dot11nDevCap & MBIT(17))
/** HW_SPEC Dot11nDevCap : Channel BW support @ 20Mhz  support */
#define ISSUPP_CHANWIDTH20(Dot11nDevCap) (Dot11nDevCap & MBIT(16))
/** HW_SPEC Dot11nDevCap : Channel BW support @ 10Mhz  support */
#define ISSUPP_CHANWIDTH10(Dot11nDevCap) (Dot11nDevCap & MBIT(15))
/** Dot11nUsrCap : 40Mhz intolarance enabled */
#define ISENABLED_40MHZ_INTOLARENT(Dot11nDevCap) (Dot11nDevCap & MBIT(8))
/** HW_SPEC Dot11nDevCap : Rx AntennaD support */
#define ISSUPP_RXANTENNAD(Dot11nDevCap) (Dot11nDevCap & MBIT(7))
/** HW_SPEC Dot11nDevCap : Rx AntennaC support */
#define ISSUPP_RXANTENNAC(Dot11nDevCap) (Dot11nDevCap & MBIT(6))
/** HW_SPEC Dot11nDevCap : Rx AntennaB support */
#define ISSUPP_RXANTENNAB(Dot11nDevCap) (Dot11nDevCap & MBIT(5))
/** HW_SPEC Dot11nDevCap : Rx AntennaA support */
#define ISSUPP_RXANTENNAA(Dot11nDevCap) (Dot11nDevCap & MBIT(4))
/** HW_SPEC Dot11nDevCap : Tx AntennaD support */
#define ISSUPP_TXANTENNAD(Dot11nDevCap) (Dot11nDevCap & MBIT(3))
/** HW_SPEC Dot11nDevCap : Tx AntennaC support */
#define ISSUPP_TXANTENNAC(Dot11nDevCap) (Dot11nDevCap & MBIT(2))
/** HW_SPEC Dot11nDevCap : Tx AntennaB support */
#define ISSUPP_TXANTENNAB(Dot11nDevCap) (Dot11nDevCap & MBIT(1))
/** HW_SPEC Dot11nDevCap : Tx AntennaA support */
#define ISSUPP_TXANTENNAA(Dot11nDevCap) (Dot11nDevCap & MBIT(0))

/** HW_SPEC Dot11nDevCap : Set support of channel bw @ 40Mhz */
#define SETSUPP_CHANWIDTH40(Dot11nDevCap) (Dot11nDevCap |= MBIT(17))
/** HW_SPEC Dot11nDevCap : Reset support of channel bw @ 40Mhz */
#define RESETSUPP_CHANWIDTH40(Dot11nDevCap) (Dot11nDevCap &= ~MBIT(17))

/** DevMCSSupported : Tx MCS supported */
#define GET_TXMCSSUPP(DevMCSSupported) (DevMCSSupported >> 4)
/** DevMCSSupported : Rx MCS supported */
#define GET_RXMCSSUPP(DevMCSSupported) (DevMCSSupported & 0x0f)

/** GET HTCapInfo : Supported Channel BW */
#define GETHT_SUPPCHANWIDTH(HTCapInfo) (HTCapInfo & MBIT(1))
/** GET HTCapInfo : Support for Greenfield */
#define GETHT_GREENFIELD(HTCapInfo) (HTCapInfo & MBIT(4))
/** GET HTCapInfo : Support for Short GI @ 20Mhz */
#define GETHT_SHORTGI20(HTCapInfo) (HTCapInfo & MBIT(5))
/** GET HTCapInfo : Support for Short GI @ 40Mhz */
#define GETHT_SHORTGI40(HTCapInfo) (HTCapInfo & MBIT(6))
/** GET HTCapInfo : Support for Tx STBC */
#define GETHT_TXSTBC(HTCapInfo) (HTCapInfo & MBIT(7))
/** GET HTCapInfo : Support for Rx STBC */
#define GETHT_RXSTBC(HTCapInfo) ((HTCapInfo >> 8) & 0x03)
/** GET HTCapInfo : Support for Delayed ACK */
#define GETHT_DELAYEDBACK(HTCapInfo) (HTCapInfo & MBIT(10))
/** GET HTCapInfo : Support for Max AMSDU */
#define GETHT_MAXAMSDU(HTCapInfo) (HTCapInfo & MBIT(11))

/** SET HTCapInfo : Set support for Channel BW */
#define SETHT_SUPPCHANWIDTH(HTCapInfo) (HTCapInfo |= MBIT(1))
/** SET HTCapInfo : Set support for Greenfield */
#define SETHT_GREENFIELD(HTCapInfo) (HTCapInfo |= MBIT(4))
/** SET HTCapInfo : Set support for Short GI @ 20Mhz */
#define SETHT_SHORTGI20(HTCapInfo) (HTCapInfo |= MBIT(5))
/** SET HTCapInfo : Set support for Short GI @ 40Mhz */
#define SETHT_SHORTGI40(HTCapInfo) (HTCapInfo |= MBIT(6))
/** SET HTCapInfo : Set support for Tx STBC */
#define SETHT_TXSTBC(HTCapInfo) (HTCapInfo |= MBIT(7))
/** SET HTCapInfo : Set support for Rx STBC */
#define SETHT_RXSTBC(HTCapInfo, value) (HTCapInfo |= (value << 8))
/** SET HTCapInfo : Set support for delayed block ack */
#define SETHT_DELAYEDBACK(HTCapInfo) (HTCapInfo |= MBIT(10))
/** SET HTCapInfo : Set support for Max size AMSDU */
#define SETHT_MAXAMSDU(HTCapInfo) (HTCapInfo |= MBIT(11))
/** SET HTCapInfo : Set support for DSSS/CCK Rates @ 40Mhz */
#define SETHT_DSSSCCK40(HTCapInfo) (HTCapInfo |= MBIT(12))
/** SET HTCapInfo : Enable 40Mhz Intolarence */
#define SETHT_40MHZ_INTOLARANT(HTCapInfo) (HTCapInfo |= MBIT(14))

/** RESET HTCapInfo : Set support for Channel BW */
#define RESETHT_SUPPCHANWIDTH(HTCapInfo) (HTCapInfo &= ~MBIT(1))
/** RESET HTCapInfo : Set support for Greenfield */
#define RESETHT_GREENFIELD(HTCapInfo) (HTCapInfo &= ~MBIT(4))
/** RESET HTCapInfo : Set support for Short GI @ 20Mhz */
#define RESETHT_SHORTGI20(HTCapInfo) (HTCapInfo &= ~MBIT(5))
/** RESET HTCapInfo : Set support for Short GI @ 40Mhz */
#define RESETHT_SHORTGI40(HTCapInfo) (HTCapInfo &= ~MBIT(6))
/** RESET HTCapInfo : Set support for Tx STBC */
#define RESETHT_TXSTBC(HTCapInfo) (HTCapInfo &= ~MBIT(7))
/** RESET HTCapInfo : Set support for Rx STBC */
#define RESETHT_RXSTBC(HTCapInfo) (HTCapInfo &= ~(0x03 << 8))
/** RESET HTCapInfo : Set support for delayed block ack */
#define RESETHT_DELAYEDBACK(HTCapInfo) (HTCapInfo &= ~MBIT(10))
/** RESET HTCapInfo : Set support for Max size AMSDU */
#define RESETHT_MAXAMSDU(HTCapInfo) (HTCapInfo &= ~MBIT(11))
/** RESET HTCapInfo : Disable 40Mhz Intolarence */
#define RESETHT_40MHZ_INTOLARANT(HTCapInfo) (HTCapInfo &= ~MBIT(14))
/** SET MCS32 */
#define SETHT_MCS32(x) (x[4] |= 1)
/** Set mcs set defined bit */
#define SETHT_MCS_SET_DEFINED(x) (x[12] |= 1)
/* Set the highest Rx data rate */
#define SETHT_RX_HIGHEST_DT_SUPP(x, y) ((*(t_u16 *) (x + 10)) = y)

/** RadioType : Support for Band A */
#define ISSUPP_BANDA(FwCapInfo) (FwCapInfo & MBIT(10))
/** RadioType : Support for 40Mhz channel BW */
#define ISALLOWED_CHANWIDTH40(Field2) (Field2 & MBIT(2))
/** RadioType : Set support 40Mhz channel */
#define SET_CHANWIDTH40(Field2) (Field2 |= MBIT(2))
/** RadioType : Reset support 40Mhz channel */
#define RESET_CHANWIDTH40(Field2) (Field2 &= ~(MBIT(0) | MBIT(1) | MBIT(2)))
/** RadioType : Get secondary channel */
#define GET_SECONDARYCHAN(Field2) (Field2 & (MBIT(0) | MBIT(1)))
/** RadioType : Set secondary channel */
#define SET_SECONDARYCHAN(RadioType, SECCHAN) (RadioType |= (SECCHAN << 4))

/** LLC/SNAP header len   */
#define LLC_SNAP_LEN    8

/** TLV type : Rate scope */
#define TLV_TYPE_RATE_DROP_PATTERN  (PROPRIETARY_TLV_BASE_ID + 81)
/** TLV type : Rate drop pattern */
#define TLV_TYPE_RATE_DROP_CONTROL  (PROPRIETARY_TLV_BASE_ID + 82)
/** TLV type : Rate scope */
#define TLV_TYPE_RATE_SCOPE         (PROPRIETARY_TLV_BASE_ID + 83)
/** TLV type : Power group */
#define TLV_TYPE_POWER_GROUP        (PROPRIETARY_TLV_BASE_ID + 84)

/** Modulation class for DSSS Rates */
#define MOD_CLASS_HR_DSSS       0x03
/** Modulation class for OFDM Rates */
#define MOD_CLASS_OFDM          0x07
/** Modulation class for HT Rates */
#define MOD_CLASS_HT            0x08
/** HT bandwidth 20 MHz */
#define HT_BW_20    0
/** HT bandwidth 40 MHz */
#define HT_BW_40    1

/** DSSS Start index */
#define MOD_HR_DSSS_START 0
/** DSSS End index */
#define MOD_HR_DSSS_END   3
/** OFDM Start index */
#define MOD_OFDM_START    4
/** OFDM End index */
#define MOD_OFDM_END      11
/** HT Start index */
#define MOD_HT_START      12
/** HT End index */
#define MOD_HT_END        127

/** Firmware Host Command ID Constants */
/** Host Command ID : Get hardware specifications */
#define HostCmd_CMD_GET_HW_SPEC               0x0003
/** Host Command ID : 802.11 scan */
#define HostCmd_CMD_802_11_SCAN               0x0006
/** Host Command ID : 802.11 get log */
#define HostCmd_CMD_802_11_GET_LOG            0x000b
/** Host Command ID : MAC multicast address */
#define HostCmd_CMD_MAC_MULTICAST_ADR         0x0010
/** Host Command ID : 802.11 EEPROM access */
#define HostCmd_CMD_802_11_EEPROM_ACCESS      0x0059
/** Host Command ID : 802.11 associate */
#define HostCmd_CMD_802_11_ASSOCIATE          0x0012
/** Host Command ID : 802.11 SNMP MIB */
#define HostCmd_CMD_802_11_SNMP_MIB           0x0016
/** Host Command ID : MAC register access */
#define HostCmd_CMD_MAC_REG_ACCESS            0x0019
/** Host Command ID : BBP register access */
#define HostCmd_CMD_BBP_REG_ACCESS            0x001a
/** Host Command ID : RF register access */
#define HostCmd_CMD_RF_REG_ACCESS             0x001b
/** Host Command ID : PMIC register access */
#define HostCmd_CMD_PMIC_REG_ACCESS           0x00ad
/** Host Command ID : 802.11 radio control */
#define HostCmd_CMD_802_11_RADIO_CONTROL      0x001c
/** Host Command ID : 802.11 RF channel */
#define HostCmd_CMD_802_11_RF_CHANNEL         0x001d
/** Host Command ID : 802.11 RF antenna */
#define HostCmd_CMD_802_11_RF_ANTENNA         0x0020

/** Host Command ID : 802.11 Power Save mode */
#define HostCmd_CMD_802_11_PS_MODE            0x0021

/** Host Command ID : 802.11 deauthenticate */
#define HostCmd_CMD_802_11_DEAUTHENTICATE     0x0024
/** Host Command ID : MAC control */
#define HostCmd_CMD_MAC_CONTROL               0x0028
/** Host Command ID : 802.11 Ad-Hoc start */
#define HostCmd_CMD_802_11_AD_HOC_START       0x002b
/** Host Command ID : 802.11 Ad-Hoc join */
#define HostCmd_CMD_802_11_AD_HOC_JOIN        0x002c

/** Host Command ID : 802.11 key material */
#define HostCmd_CMD_802_11_KEY_MATERIAL       0x005e

/** Host Command ID : 802.11 Ad-Hoc stop */
#define HostCmd_CMD_802_11_AD_HOC_STOP        0x0040

/** Host Command ID : 802.11 Host Sleep configuration */
#define HostCmd_CMD_802_11_HOST_SLEEP_CFG     0x0043
/** Host Command ID : 802.11 Wakeup confirm */
#define HostCmd_CMD_802_11_WAKEUP_CONFIRM     0x0044
/** Host Command ID : 802.11 Host Sleep activated */
#define HostCmd_CMD_802_11_HOST_SLEEP_ACTIVATE  0x0045

/** Host Command ID : 802.22 MAC address */
#define HostCmd_CMD_802_11_MAC_ADDRESS        0x004D

/** Host Command ID : 802.11 D domain information */
#define HostCmd_CMD_802_11D_DOMAIN_INFO       0x005b

/** Host Command ID : 802.11 sleep parameters */
#define HostCmd_CMD_802_11_SLEEP_PARAMS       0x0066

/** Host Command ID : 802.11 sleep period */
#define HostCmd_CMD_802_11_SLEEP_PERIOD       0x0068

/** Host Command ID : 802.11 get status */
#define HostCmd_CMD_WMM_GET_STATUS            0x0071

/** Host Command ID : 802.11 Tx rate query */
#define HostCmd_CMD_802_11_TX_RATE_QUERY      0x007f

/** Host Command ID : 802.11 IBSS coalescing status */
#define HostCmd_CMD_802_11_IBSS_COALESCING_STATUS 0x0083

#ifdef MFG_CMD_SUPPORT
/** Host Command ID : Mfg command */
#define HostCmd_CMD_MFG_COMMAND               0x0089
#endif
/** Host Command ID : Inactivity timeout ext */
#define HostCmd_CMD_INACTIVITY_TIMEOUT_EXT    0x008a

/** Host Command ID : DBGS configuration */
#define HostCmd_CMD_DBGS_CFG                  0x008b
/** Host Command ID : Get memory */
#define HostCmd_CMD_GET_MEM                   0x008c

/** Host Command ID : ECL system clock configuration */
#define HostCmd_CMD_ECL_SYSTEM_CLOCK_CONFIG   0x0094

/** Host Command ID : Extended version */
#define HostCmd_CMD_VERSION_EXT               0x0097

/** Host Command ID : 802.11 RSSI INFO*/
#define HostCmd_CMD_RSSI_INFO                 0x00a4

/** Host Command ID : Function initialization */
#define HostCmd_CMD_FUNC_INIT                 0x00a9
/** Host Command ID : Function shutdown */
#define HostCmd_CMD_FUNC_SHUTDOWN             0x00aa

/** Host Command ID: SUPPLICANT_PMK */
#define HostCmd_CMD_SUPPLICANT_PMK            0x00c4
/** Host Command ID: SUPPLICANT_PROFILE */
#define HostCmd_CMD_SUPPLICANT_PROFILE        0x00c5

/** Host Command ID : Add Block Ack Request */
#define HostCmd_CMD_11N_ADDBA_REQ             0x00ce
/** Host Command ID : Delete a Block Ack Request */
#define HostCmd_CMD_11N_CFG                   0x00cd
/** Host Command ID : Add Block Ack Response */
#define HostCmd_CMD_11N_ADDBA_RSP             0x00cf
/** Host Command ID : Delete a Block Ack Request */
#define HostCmd_CMD_11N_DELBA                 0x00d0
/** Host Command ID: Configure Tx Buf size */
#define HostCmd_CMD_RECONFIGURE_TX_BUFF       0x00d9
/** Host Command ID: AMSDU Aggr Ctrl */
#define HostCmd_CMD_AMSDU_AGGR_CTRL	      	  0x00df

/** Host Command ID : 802.11 TX power configuration */
#define HostCmd_CMD_TXPWR_CFG                 0x00d1

/** Host Command ID : 802.11 b/g/n rate configration */
#define HostCmd_CMD_TX_RATE_CFG               0x00d6

/** Host Command ID : Heart Beat Configuration */
#define HostCmd_CMD_HEART_BEAT                0x00da

/** Host Command ID : Hostcmd */
#define	HostCmd_CMD_HOST_COMMAND			  0xffff

/* For the IEEE Power Save */
/** Host Subcommand ID : Enter power save */
#define HostCmd_SubCmd_Enter_PS               0x0030
/** Host Subcommand ID : Exit power save */
#define HostCmd_SubCmd_Exit_PS                0x0031
/** Host Subcommand ID : Sleep confirmed */
#define HostCmd_SubCmd_Sleep_Confirmed        0x0034
/** Host Subcommand ID : Full power down */
#define HostCmd_SubCmd_Full_PowerDown         0x0035
/** Host Subcommand ID : Full power up */
#define HostCmd_SubCmd_Full_PowerUp           0x0036

/** Command RET code, MSB is set to 1 */
#define HostCmd_RET_BIT                       0x8000

/** General purpose action : Get */
#define HostCmd_ACT_GEN_GET                   0x0000
/** General purpose action : Set */
#define HostCmd_ACT_GEN_SET                   0x0001
/** General purpose action : Get_Current */
#define HostCmd_ACT_GEN_GET_CURRENT           0x0003
/** General purpose action : Remove */
#define HostCmd_ACT_GEN_REMOVE                0x0004

/** Host command action : Set both Rx and Tx */
#define HostCmd_ACT_SET_BOTH                  0x0003
/** Host command action : Get Rx */
#define HostCmd_ACT_GET_RX                    0x0004
/** Host command action : Get Tx */
#define HostCmd_ACT_GET_TX                    0x0008
/** Host command action : Get both Rx and Tx */
#define HostCmd_ACT_GET_BOTH                  0x000c

/** General Result Code*/
/** General result code OK */
#define HostCmd_RESULT_OK                     0x0000
/** Genenral error */
#define HostCmd_RESULT_ERROR                  0x0001
/** Command is not valid */
#define HostCmd_RESULT_NOT_SUPPORT            0x0002
/** Command is pending */
#define HostCmd_RESULT_PENDING                0x0003
/** System is busy (command ignored) */
#define HostCmd_RESULT_BUSY                   0x0004
/** Data buffer is not big enough */
#define HostCmd_RESULT_PARTIAL_DATA           0x0005

/* Define action or option for HostCmd_CMD_MAC_CONTROL */
/** MAC action : Rx on */
#define HostCmd_ACT_MAC_RX_ON                 0x0001
/** MAC action : Tx on */
#define HostCmd_ACT_MAC_TX_ON                 0x0002
/** MAC action : Loopback on */
#define HostCmd_ACT_MAC_LOOPBACK_ON           0x0004
/** MAC action : WEP enable */
#define HostCmd_ACT_MAC_WEP_ENABLE            0x0008
/** MAC action : EthernetII enable */
#define HostCmd_ACT_MAC_ETHERNETII_ENABLE     0x0010
/** MAC action : Promiscous mode enable */
#define HostCmd_ACT_MAC_PROMISCUOUS_ENABLE    0x0080
/** MAC action : All multicast enable */
#define HostCmd_ACT_MAC_ALL_MULTICAST_ENABLE  0x0100

/** MAC action : Strict protection enable */
#define HostCmd_ACT_MAC_STRICT_PROTECTION_ENABLE  0x0400
/** MAC action : Ad-Hoc G protection on */
#define HostCmd_ACT_MAC_ADHOC_G_PROTECTION_ON     0x2000

/* Define action or option for HostCmd_CMD_802_11_SCAN */
/** Scan type : BSS */
#define HostCmd_BSS_TYPE_BSS                0x0001
/** Scan type : IBSS */
#define HostCmd_BSS_TYPE_IBSS               0x0002
/** Scan type : Any */
#define HostCmd_BSS_TYPE_ANY                0x0003

/* Define action or option for HostCmd_CMD_802_11_SCAN */
/** Scan type : Active */
#define HostCmd_SCAN_TYPE_ACTIVE            0x0000
/** Scan type : Passive */
#define HostCmd_SCAN_TYPE_PASSIVE           0x0001

/* Radio type definitions for the channel TLV */
/** Radio type BG */
#define HostCmd_SCAN_RADIO_TYPE_BG          0
/** Radio type A */
#define HostCmd_SCAN_RADIO_TYPE_A           1

/** Define bitmap conditions for HOST_SLEEP_CFG : GPIO FF */
#define HOST_SLEEP_CFG_GPIO_FF              0xff
/** Define bitmap conditions for HOST_SLEEP_CFG : GAP FF */
#define HOST_SLEEP_CFG_GAP_FF               0xff

/** Buffer Constants */
/** Number of command buffers */
#define MRVDRV_NUM_OF_CMD_BUFFER        10
/** Size of command buffer */
#define MRVDRV_SIZE_OF_CMD_BUFFER       (2 * 1024)

/** Maximum number of BSS Descriptors */
#define MRVDRV_MAX_BSSID_LIST           64

/** Host command flag in command */
#define CMD_F_HOSTCMD           (1 << 0)

/** Host Command ID bit mask (bit 11:0) */
#define HostCmd_CMD_ID_MASK             0x0fff

/** Set BSS number to Host Command (bit 14:12) */
#define HostCmd_SET_BSS_NO(cmd, bss)    \
    (((cmd) & HostCmd_CMD_ID_MASK) | \
        (((bss) & (MLAN_MAX_BSS_NUM - 1)) << 12))

/** Get BSS number from Host Command (bit 14:12) */
#define HostCmd_GET_BSS_NO(cmd)         \
        (((cmd) >> 12) & (MLAN_MAX_BSS_NUM - 1))

/** Card Event definition : Dummy host wakeup signal */
#define EVENT_DUMMY_HOST_WAKEUP_SIGNAL  0x00000001
/** Card Event definition : Link lost with scan */
#define EVENT_LINK_LOST_WITH_SCAN       0x00000002
/** Card Event definition : Link lost */
#define EVENT_LINK_LOST                 0x00000003
/** Card Event definition : Link sensed */
#define EVENT_LINK_SENSED               0x00000004
/** Card Event definition : MIB changed */
#define EVENT_MIB_CHANGED               0x00000006
/** Card Event definition : Init done */
#define EVENT_INIT_DONE                 0x00000007
/** Card Event definition : Deauthenticated */
#define EVENT_DEAUTHENTICATED           0x00000008
/** Card Event definition : Disassociated */
#define EVENT_DISASSOCIATED             0x00000009
/** Card Event definition : Power save awake */
#define EVENT_PS_AWAKE                  0x0000000a
/** Card Event definition : Power save sleep */
#define EVENT_PS_SLEEP                  0x0000000b
/** Card Event definition : MIC error multicast */
#define EVENT_MIC_ERR_MULTICAST         0x0000000d
/** Card Event definition : MIC error unicast */
#define EVENT_MIC_ERR_UNICAST           0x0000000e
/** Card Event definition : WM awake */
#define EVENT_WM_AWAKE                  0x0000000f
/** Card Event definition : Ad-Hoc BCN lost */
#define EVENT_ADHOC_BCN_LOST            0x00000011
/** Card Event definition : Host Sleep awake */
#define EVENT_HOST_SLEEP_AWAKE          0x00000012
/** Card Event definition : WMM status change */
#define EVENT_WMM_STATUS_CHANGE         0x00000017
/** Card Event definition : Beacon RSSI low */
#define EVENT_RSSI_LOW                  0x00000019
/** Card Event definition : Beacon SNR low */
#define EVENT_SNR_LOW                   0x0000001a
/** Card Event definition : Maximum fail */
#define EVENT_MAX_FAIL                  0x0000001b
/** Card Event definition : Beacon RSSI high */
#define EVENT_RSSI_HIGH                 0x0000001c
/** Card Event definition : Beacon SNR high */
#define EVENT_SNR_HIGH                  0x0000001d
/** Card Event definition : IBSS coalsced */
#define EVENT_IBSS_COALESCED            0x0000001e
/** Card Event definition : Data RSSI low */
#define EVENT_DATA_RSSI_LOW             0x00000024
/** Card Event definition : Data SNR low */
#define EVENT_DATA_SNR_LOW              0x00000025
/** Card Event definition : Data RSSI high */
#define EVENT_DATA_RSSI_HIGH            0x00000026
/** Card Event definition : Data SNR high */
#define EVENT_DATA_SNR_HIGH             0x00000027
/** Card Event definition : Link Quality */
#define EVENT_LINK_QUALITY              0x00000028
/** Card Event definition : Port release event */
#define EVENT_PORT_RELEASE              0x0000002b
/** Card Event definition : Pre-Beacon Lost */
#define EVENT_PRE_BEACON_LOST           0x00000031
/** Card Event definition : Add BA event */
#define EVENT_ADDBA                     0x00000033
/** Card Event definition : Del BA event */
#define EVENT_DELBA                     0x00000034
/** Card Event definition: BA stream timeout*/
#define EVENT_BA_STREAM_TIEMOUT         0x00000037
/** Card Event definition : Heart Beat event */
#define EVENT_HEART_BEAT                0x00000035
/** Card Event definition : AMSDU aggr control */
#define EVENT_AMSDU_AGGR_CTRL           0x00000042
/** Card Event definition: WEP ICV error */
#define EVENT_WEP_ICV_ERR               0x00000046
/** Card Event definition : BW changed */
#define EVENT_BW_CHANGE                 0x00000048
/** event ID mask */
#define EVENT_ID_MASK                   0x0fff

/** Get BSS number from event cause (bit 14:12) */
#define EVENT_GET_BSS_NUM(event_cause)          \
    (((event_cause) >> 12) & (MLAN_MAX_BSS_NUM - 1))

/** Event_WEP_ICV_ERR structure */
typedef struct _Event_WEP_ICV_ERR
{
    /** Reason code */
    t_u16 reason_code;
    /** Source MAC address */
    t_u8 src_mac_addr[MLAN_MAC_ADDR_LENGTH];
    /** WEP decryption used key */
    t_u8 wep_key_index;
    /** WEP key length */
    t_u8 wep_key_length;
    /** WEP key */
    t_u8 key[MAX_WEP_KEY_SIZE];
} Event_WEP_ICV_ERR;

/** WLAN_802_11_FIXED_IEs */
typedef struct _WLAN_802_11_FIXED_IEs
{
    /** Timestamp */
    t_u8 time_stamp[8];
    /** Beacon interval */
    t_u16 beacon_interval;
    /** Capabilities*/
    t_u16 capabilities;
} WLAN_802_11_FIXED_IEs;

/** WLAN_802_11_VARIABLE_IEs */
typedef struct _WLAN_802_11_VARIABLE_IEs
{
    /** Element ID */
    t_u8 element_id;
    /** Length */
    t_u8 length;
    /** IE data */
    t_u8 data[1];
} WLAN_802_11_VARIABLE_IEs;

/** TLV related data structures*/
/** MrvlIEtypesHeader_t */
typedef struct _MrvlIEtypesHeader
{
    /** Header type */
    t_u16 type;
    /** Header length */
    t_u16 len;
} __attribute__ ((packed)) MrvlIEtypesHeader_t;

/** MrvlIEtypes_Data_t */
typedef struct _MrvlIEtypes_Data_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** Data */
    t_u8 data[1];
} __attribute__ ((packed)) MrvlIEtypes_Data_t;

/** ChanScanMode_t */
typedef struct _ChanScanMode_t
{
#ifdef BIG_ENDIAN
    /** Reserved */
    t_u8 reserved_2_7:6;
    /** Disble channel filtering flag */
    t_u8 disable_chan_filt:1;
    /** Channel scan mode passive flag */
    t_u8 passive_scan:1;
#else
    /** Channel scan mode passive flag */
    t_u8 passive_scan:1;
    /** Disble channel filtering flag */
    t_u8 disable_chan_filt:1;
    /** Reserved */
    t_u8 reserved_2_7:6;
#endif
} __attribute__ ((packed)) ChanScanMode_t;

/** ChanScanParamSet_t */
typedef struct _ChanScanParamSet_t
{
    /** Channel scan parameter : Radio type */
    t_u8 radio_type;
    /** Channel scan parameter : Channel number */
    t_u8 chan_number;
    /** Channel scan parameter : Channel scan mode */
    ChanScanMode_t chan_scan_mode;
    /** Channel scan parameter : Minimum scan time */
    t_u16 min_scan_time;
    /** Channel scan parameter : Maximum scan time */
    t_u16 max_scan_time;
} __attribute__ ((packed)) ChanScanParamSet_t;

/** MrvlIEtypes_ChanListParamSet_t */
typedef struct _MrvlIEtypes_ChanListParamSet_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** Channel scan parameters */
    ChanScanParamSet_t chan_scan_param[1];
} __attribute__ ((packed)) MrvlIEtypes_ChanListParamSet_t;

/** ChanScanParamSet_t */
typedef struct _ChanBandParamSet_t
{
    /** Channel scan parameter : Radio type */
    t_u8 radio_type;
    /** Channel number */
    t_u8 chan_number;
} ChanBandParamSet_t;

/** MrvlIEtypes_ChanBandListParamSet_t */
typedef struct _MrvlIEtypes_ChanBandListParamSet_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** Channel Band parameters */
    ChanBandParamSet_t chan_band_param[1];
} __attribute__ ((packed)) MrvlIEtypes_ChanBandListParamSet_t;

/** MrvlIEtypes_RatesParamSet_t */
typedef struct _MrvlIEtypes_RatesParamSet_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** Rates */
    t_u8 rates[1];
} __attribute__ ((packed)) MrvlIEtypes_RatesParamSet_t;

/** MrvlIEtypes_SsIdParamSet_t */
typedef struct _MrvlIEtypes_SsIdParamSet_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** SSID */
    t_u8 ssid[1];
} __attribute__ ((packed)) MrvlIEtypes_SsIdParamSet_t;

/** MrvlIEtypes_NumProbes_t */
typedef struct _MrvlIEtypes_NumProbes_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** Number of probes */
    t_u16 num_probes;
} __attribute__ ((packed)) MrvlIEtypes_NumProbes_t;

/** MrvlIEtypes_WildCardSsIdParamSet_t */
typedef struct _MrvlIEtypes_WildCardSsIdParamSet_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** Maximum SSID length */
    t_u8 max_ssid_length;
    /** SSID */
    t_u8 ssid[1];
} __attribute__ ((packed)) MrvlIEtypes_WildCardSsIdParamSet_t;

/**TSF data size */
#define TSF_DATA_SIZE            8
/** Table of TSF values returned in the scan result */
typedef struct _MrvlIEtypes_TsfTimestamp_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** the length of each TSF data is 8 bytes, could be multiple TSF here */
    t_u8 tsf_data[1];
} __attribute__ ((packed)) MrvlIEtypes_TsfTimestamp_t;

/** CfParamSet_t */
typedef struct _CfParamSet_t
{
    /** CF parameter : Count */
    t_u8 cfp_cnt;
    /** CF parameter : Period */
    t_u8 cfp_period;
    /** CF parameter : Duration */
    t_u16 cfp_max_duration;
    /** CF parameter : Duration remaining */
    t_u16 cfp_duration_remaining;
} __attribute__ ((packed)) CfParamSet_t;

/** IbssParamSet_t */
typedef struct _IbssParamSet_t
{
    /** ATIM window value */
    t_u16 atim_window;
} __attribute__ ((packed)) IbssParamSet_t;

/** MrvlIEtypes_SsParamSet_t */
typedef struct _MrvlIEtypes_SsParamSet_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** CF/IBSS parameters sets */
    union
    {
        /** CF parameter set */
        CfParamSet_t cf_param_set[1];
        /** IBSS parameter set */
        IbssParamSet_t ibss_param_set[1];
    } cf_ibss;
} __attribute__ ((packed)) MrvlIEtypes_SsParamSet_t;

/** FhParamSet_t */
typedef struct _FhParamSet_t
{
    /** FH parameter : Dwell time */
    t_u16 dwell_time;
    /** FH parameter : Hop set */
    t_u8 hop_set;
    /** FH parameter : Hop pattern */
    t_u8 hop_pattern;
    /** FH parameter : Hop index */
    t_u8 hop_index;
} __attribute__ ((packed)) FhParamSet_t;

/** DsParamSet_t */
typedef struct _DsParamSet_t
{
    /** Current channel number */
    t_u8 current_chan;
} __attribute__ ((packed)) DsParamSet_t;

/** MrvlIEtypes_PhyParamSet_t */
typedef struct _MrvlIEtypes_PhyParamSet_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** FH/DS parameters */
    union
    {
        /** FH parameter set */
        FhParamSet_t fh_param_set[1];
        /** DS parameter set */
        DsParamSet_t ds_param_set[1];
    } fh_ds;
} __attribute__ ((packed)) MrvlIEtypes_PhyParamSet_t;

/* Auth type to be used in the Authentication portion of an Assoc seq */
/** MrvlIEtypes_AuthType_t */
typedef struct _MrvlIEtypes_AuthType_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** Authentication type */
    t_u16 auth_type;
} __attribute__ ((packed)) MrvlIEtypes_AuthType_t;

/** MrvlIETypes_VendorParamSet_t */
typedef struct _MrvlIETypes_VendorParamSet_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** Information element */
    t_u8 ie[MLAN_MAX_VSIE_LEN];
} MrvlIETypes_VendorParamSet_t;

/** MrvlIEtypes_RsnParamSet_t */
typedef struct _MrvlIEtypes_RsnParamSet_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** RSN IE */
    t_u8 rsn_ie[1];
} __attribute__ ((packed)) MrvlIEtypes_RsnParamSet_t;

/** MrvlIEtype_KeyParamSet_t */
typedef struct _MrvlIEtype_KeyParamSet_t
{
    /** Type ID */
    t_u16 type;
    /** Length of Payload */
    t_u16 length;
    /** Type of Key: WEP=0, TKIP=1, AES=2 */
    t_u16 key_type_id;
    /** Key Control Info specific to a key_type_id */
    t_u16 key_info;
    /** Length of key */
    t_u16 key_len;
    /** Key material of size key_len */
    t_u8 key[32];
} __attribute__ ((packed)) MrvlIEtype_KeyParamSet_t;

/** HostCmd_DS_802_11_KEY_MATERIAL */
typedef struct _HostCmd_DS_802_11_KEY_MATERIAL
{
    /** Action */
    t_u16 action;
    /** Key parameter set */
    MrvlIEtype_KeyParamSet_t key_param_set;
} __attribute__ ((packed)) HostCmd_DS_802_11_KEY_MATERIAL;

/* Definition of firmware host command */
/** HostCmd_DS_GEN */
typedef struct _HostCmd_DS_GEN
{
    /** Command */
    t_u16 command;
    /** Size */
    t_u16 size;
    /** Sequence number */
    t_u16 seq_num;
    /** Result */
    t_u16 result;
} HostCmd_DS_GEN;

/** Size of HostCmd_DS_GEN */
#define S_DS_GEN        sizeof(HostCmd_DS_GEN)

/** HostCmd_DS_GET_HW_SPEC */
typedef struct _HostCmd_DS_GET_HW_SPEC
{
    /** HW Interface version number */
    t_u16 hw_if_version;
    /** HW version number */
    t_u16 version;
    /** Reserved field */
    t_u16 reserved;
    /** Max no of Multicast address  */
    t_u16 num_of_mcast_adr;
    /** MAC address */
    t_u8 permanent_addr[MLAN_MAC_ADDR_LENGTH];
    /** Region Code */
    t_u16 region_code;
    /** Number of antenna used */
    t_u16 number_of_antenna;
    /** FW release number, example 0x1234=1.2.3.4 */
    t_u32 fw_release_number;
    /** Reserved field */
    t_u32 reserved_1;
    /** Reserved field */
    t_u32 reserved_2;
    /** Reserved field */
    t_u32 reserved_3;
    /** FW/HW Capability */
    t_u32 fw_cap_info;
    /** 802.11n Device Capabilities */
    t_u32 dot_11n_dev_cap;
    /** MIMO abstraction of MCSs supported by device */
    t_u8 dev_mcs_support;
} __attribute__ ((packed)) HostCmd_DS_GET_HW_SPEC;

/**  HostCmd_DS_CMD_802_11_RSSI_INFO */
typedef struct _HostCmd_DS_802_11_RSSI_INFO
{
    /** Action */
    t_u16 action;
    /** Parameter used for exponential averaging for Data */
    t_u16 ndata;
    /** Parameter used for exponential averaging for Beacon */
    t_u16 nbcn;
    /** Reserved field 0 */
    t_u16 reserved[9];
    /** Reserved field 1 */
    t_u64 reserved_1;
} HostCmd_DS_802_11_RSSI_INFO;

/** HostCmd_DS_802_11_RSSI_INFO_RSP */
typedef struct _HostCmd_DS_802_11_RSSI_INFO_RSP
{
    /** Action */
    t_u16 action;
    /** Parameter used for exponential averaging for Data */
    t_u16 ndata;
    /** Parameter used for exponential averaging for beacon */
    t_u16 nbcn;
    /** Last Data RSSI in dBm */
    t_s16 data_rssi_last;
    /** Last Data NF in dBm */
    t_s16 data_nf_last;
    /** AVG DATA RSSI in dBm */
    t_s16 data_rssi_avg;
    /** AVG DATA NF in dBm */
    t_s16 data_nf_avg;
    /** Last BEACON RSSI in dBm */
    t_s16 bcn_rssi_last;
    /** Last BEACON NF in dBm */
    t_s16 bcn_nf_last;
    /** AVG BEACON RSSI in dBm */
    t_s16 bcn_rssi_avg;
    /** AVG BEACON NF in dBm */
    t_s16 bcn_nf_avg;
    /** Last RSSI Beacon TSF */
    t_u64 tsf_bcn;
} HostCmd_DS_802_11_RSSI_INFO_RSP;

/** HostCmd_DS_802_11_MAC_ADDRESS */
typedef struct _HostCmd_DS_802_11_MAC_ADDRESS
{
    /** Action */
    t_u16 action;
    /** MAC address */
    t_u8 mac_addr[MLAN_MAC_ADDR_LENGTH];
} HostCmd_DS_802_11_MAC_ADDRESS;

/** HostCmd_DS_MAC_CONTROL */
typedef struct _HostCmd_DS_MAC_CONTROL
{
    /** Action */
    t_u16 action;
    /** Reserved field */
    t_u16 reserved;
} HostCmd_DS_MAC_CONTROL;

/**  HostCmd_CMD_MAC_MULTICAST_ADR */
typedef struct _HostCmd_DS_MAC_MULTICAST_ADR
{
    /** Action */
    t_u16 action;
    /** Number of addresses */
    t_u16 num_of_adrs;
    /** List of MAC */
    t_u8 mac_list[MLAN_MAC_ADDR_LENGTH * MLAN_MAX_MULTICAST_LIST_SIZE];
} __attribute__ ((packed)) HostCmd_DS_MAC_MULTICAST_ADR;

/**  HostCmd_CMD_802_11_DEAUTHENTICATE */
typedef struct _HostCmd_DS_802_11_DEAUTHENTICATE
{
    /** MAC address */
    t_u8 mac_addr[MLAN_MAC_ADDR_LENGTH];
    /** Deauthentication resaon code */
    t_u16 reason_code;
} __attribute__ ((packed)) HostCmd_DS_802_11_DEAUTHENTICATE;

/** HostCmd_DS_802_11_ASSOCIATE */
typedef struct _HostCmd_DS_802_11_ASSOCIATE
{
    /** Peer STA address */
    t_u8 peer_sta_addr[MLAN_MAC_ADDR_LENGTH];
    /** Capability information */
    IEEEtypes_CapInfo_t cap_info;
    /** Listen interval */
    t_u16 listen_interval;
    /** Beacon period */
    t_u16 beacon_period;
    /** DTIM period */
    t_u8 dtim_period;

    /**
     *  MrvlIEtypes_SsIdParamSet_t  SsIdParamSet;
     *  MrvlIEtypes_PhyParamSet_t   PhyParamSet;
     *  MrvlIEtypes_SsParamSet_t    SsParamSet;
     *  MrvlIEtypes_RatesParamSet_t RatesParamSet;
     */
} __attribute__ ((packed)) HostCmd_DS_802_11_ASSOCIATE;

/** HostCmd_CMD_802_11_ASSOCIATE response */
typedef struct _HostCmd_DS_802_11_ASSOCIATE_RSP
{
    /** Association response structure */
    IEEEtypes_AssocRsp_t assoc_rsp;
} __attribute__ ((packed)) HostCmd_DS_802_11_ASSOCIATE_RSP;

/** HostCmd_DS_802_11_AD_HOC_START*/
typedef struct _HostCmd_DS_802_11_AD_HOC_START
{
    /** AdHoc SSID */
    t_u8 ssid[MLAN_MAX_SSID_LENGTH];
    /** BSS type */
    t_u8 bss_type;
    /** Beacon period */
    t_u16 beacon_period;
    /** DTIM period */
    t_u8 dtim_period;
    /** SS parameter set */
    IEEEtypes_SsParamSet_t ss_param_set;
    /** PHY parameter set */
    IEEEtypes_PhyParamSet_t phy_param_set;
    /** Reserved field */
    t_u16 reserved1;
    /** Capability information */
    IEEEtypes_CapInfo_t cap;
    /** Supported data rates */
    t_u8 DataRate[HOSTCMD_SUPPORTED_RATES];
} __attribute__ ((packed)) HostCmd_DS_802_11_AD_HOC_START;

/**  HostCmd_CMD_802_11_AD_HOC_START response */
typedef struct _HostCmd_DS_802_11_AD_HOC_RESULT
{
    /** Padding */
    t_u8 pad[3];
    /** AdHoc BSSID */
    t_u8 bssid[MLAN_MAC_ADDR_LENGTH];
} __attribute__ ((packed)) HostCmd_DS_802_11_AD_HOC_RESULT;

/** AdHoc_BssDesc_t */
typedef struct _AdHoc_BssDesc_t
{
    /** BSSID */
    t_u8 bssid[MLAN_MAC_ADDR_LENGTH];
    /** SSID */
    t_u8 ssid[MLAN_MAX_SSID_LENGTH];
    /** BSS type */
    t_u8 bss_type;
    /** Beacon period */
    t_u16 beacon_period;
    /** DTIM period */
    t_u8 dtim_period;
    /** Timestamp */
    t_u8 time_stamp[8];
    /** Local time */
    t_u8 local_time[8];
    /** PHY parameter set */
    IEEEtypes_PhyParamSet_t phy_param_set;
    /** SS parameter set */
    IEEEtypes_SsParamSet_t ss_param_set;
    /** Capability information */
    IEEEtypes_CapInfo_t cap;
    /** Supported data rates */
    t_u8 data_rates[HOSTCMD_SUPPORTED_RATES];

    /* 
     *  DO NOT ADD ANY FIELDS TO THIS STRUCTURE.
     *  It is used in the Adhoc join command and will cause a 
     *  binary layout mismatch with the firmware 
     */
} __attribute__ ((packed)) AdHoc_BssDesc_t;

/** HostCmd_DS_802_11_AD_HOC_JOIN */
typedef struct _HostCmd_DS_802_11_AD_HOC_JOIN
{
    /** AdHoc BSS descriptor */
    AdHoc_BssDesc_t bss_descriptor;
    /** Reserved field */
    t_u16 reserved1;
    /** Reserved field */
    t_u16 reserved2;
} __attribute__ ((packed)) HostCmd_DS_802_11_AD_HOC_JOIN;

/** HostCmd_DS_802_11_GET_LOG */
typedef struct _HostCmd_DS_802_11_GET_LOG
{
    /** Number of multicast transmitted frames */
    t_u32 mcast_tx_frame;
    /** Number of failures */
    t_u32 failed;
    /** Number of retries */
    t_u32 retry;
    /** Number of multiretries */
    t_u32 multiretry;
    /** Number of duplicate frames */
    t_u32 frame_dup;
    /** Number of RTS success */
    t_u32 rts_success;
    /** Number of RTS failure */
    t_u32 rts_failure;
    /** Number of acknowledgement failure */
    t_u32 ack_failure;
    /** Number of fragmented packets received */
    t_u32 rx_frag;
    /** Number of multicast frames received */
    t_u32 mcast_rx_frame;
    /** FCS error */
    t_u32 fcs_error;
    /** Number of transmitted frames */
    t_u32 tx_frame;
    /** Reserved field */
    t_u32 reserved;
    /** Number of WEP icv error for each key */
    t_u32 wep_icv_err_cnt[4];
} HostCmd_DS_802_11_GET_LOG;

/**_HostCmd_TX_RATE_QUERY */
typedef struct _HostCmd_TX_RATE_QUERY
{
    /** Tx rate */
    t_u8 tx_rate;
    /** Ht Info [Bit 0] RxRate format: LG=0, HT=1
     * [Bit 1]  HT Bandwidth: BW20 = 0, BW40 = 1
     * [Bit 2]  HT Guard Interval: LGI = 0, SGI = 1 */
    t_u8 ht_info;
} __attribute__ ((packed)) HostCmd_TX_RATE_QUERY;

/** HostCmd_CMD_802_11_PS_MODE */
typedef struct _HostCmd_DS_802_11_PS_MODE
{
    /** Action */
    t_u16 action;
    /** NULL packet interval */
    t_u16 null_pkt_interval;
    /** Multiple DTIM */
    t_u16 multiple_dtim;
    /** Beacon miss timeout */
    t_u16 bcn_miss_time_out;
    /** Local listen interval */
    t_u16 local_listen_interval;
    /** AdHoc awake period */
    t_u16 adhoc_awake_period;
} __attribute__ ((packed)) HostCmd_DS_802_11_PS_MODE;

/** PS_CMD_ConfirmSleep */
typedef struct _PS_CMD_ConfirmSleep
{
    /** Command */
    t_u16 command;
    /** Size */
    t_u16 size;
    /** Sequence number */
    t_u16 seq_num;
    /** Result */
    t_u16 result;

    /** Action */
    t_u16 action;
    /** Reserved */
    t_u16 reserved1;
    /** Multiple DTIM */
    t_u16 multiple_dtim;
    /** Reserved */
    t_u16 reserved;
    /** Local listen interval */
    t_u16 local_listen_interval;
} __attribute__ ((packed)) PS_CMD_ConfirmSleep;

/** HostCmd_DS_802_11_HOST_SLEEP_CFG */
typedef struct _HostCmd_DS_802_11_HOST_SLEEP_CFG
{
    /** bit0=1: non-unicast data
      * bit1=1: unicast data
      * bit2=1: mac events
      * bit3=1: magic packet
      */
    t_u32 conditions;

    /** GPIO */
    t_u8 gpio;

    /** in milliseconds */
    t_u8 gap;
} __attribute__ ((packed)) HostCmd_DS_802_11_HOST_SLEEP_CFG;

/** SNMP_MIB_INDEX_e */
typedef enum _SNMP_MIB_INDEX_e
{
    OpRateSet_i = 1,
    DtimPeriod_i = 3,
    RtsThresh_i = 5,
    ShortRetryLim_i = 6,
    LongRetryLim_i = 7,
    FragThresh_i = 8,
    Dot11D_i = 9,
    Dot11H_i = 10,
    WwsMode_i = 17
} SNMP_MIB_INDEX_e;

/**  HostCmd_CMD_802_11_SNMP_MIB */
typedef struct _HostCmd_DS_802_11_SNMP_MIB
{
    /** SNMP query type */
    t_u16 query_type;
    /** SNMP object ID */
    t_u16 oid;
    /** SNMP buffer size */
    t_u16 buf_size;
    /** Value */
    t_u8 value[128];
} __attribute__ ((packed)) HostCmd_DS_802_11_SNMP_MIB;

/** Radio on */
#define RADIO_ON                                0x01
/** Radio off */
#define RADIO_OFF                               0x00

/** HostCmd_CMD_802_11_RADIO_CONTROL */
typedef struct _HostCmd_DS_802_11_RADIO_CONTROL
{
    /** Action */
    t_u16 action;
    /** Control */
    t_u16 control;
} __attribute__ ((packed)) HostCmd_DS_802_11_RADIO_CONTROL;

/** MrvlRateScope_t */
typedef struct _MrvlRateScope_t
{
    /** Header Type */
    t_u16 type;
    /** Header Length */
    t_u16 length;
    /** Bitmap of HR/DSSS rates */
    t_u16 hr_dsss_rate_bitmap;
    /** Bitmap of OFDM rates */
    t_u16 ofdm_rate_bitmap;
    /** Bitmap of HT-MCSs allowed for initial rate */
    t_u16 ht_mcs_rate_bitmap[8];
} __attribute__ ((packed)) MrvlRateScope_t;

/** MrvlRateDropControl_t */
typedef struct _MrvlRateDropControl_t
{
    /** Header Length */
    t_u16 length;
    /** Rate Information */
    t_u32 rate_info[1];
} __attribute__ ((packed)) MrvlRateDropControl_t;

/** MrvlRateDropPattern_t */
typedef struct _MrvlRateDropPattern_t
{
    /** Header Type */
    t_u16 type;
    /** Header Length */
    t_u16 length;
    /** Rate Drop Mode */
    t_u32 rate_drop_mode;
    /* MrvlRateDropControl_t RateDropControl[0]; */
} __attribute__ ((packed)) MrvlRateDropPattern_t;

/** HostCmd_DS_TX_RATE_CFG */
typedef struct _HostCmd_DS_TX_RATE_CFG
{
    /** Action */
    t_u16 action;
    /** Tx Rate Configuration Index */
    t_u16 cfg_index;
    /* MrvlRateScope_t RateScope; MrvlRateDropPattern_t RateDrop; */
} __attribute__ ((packed)) HostCmd_DS_TX_RATE_CFG;

/** Power_Group_t */
typedef struct _Power_Group_t
{
    /** Modulation Class */
    t_u8 modulation_class;
    /** MCS Code or Legacy RateID */
    t_u8 first_rate_code;
    /** MCS Code or Legacy RateID */
    t_u8 last_rate_code;
    /** Power Adjustment Step */
    t_s8 power_step;
    /** Minimal Tx Power Level [dBm] */
    t_s8 power_min;
    /** Maximal Tx Power Level [dBm] */
    t_s8 power_max;
    /** 0: HTBW20, 1: HTBW40 */
    t_u8 ht_bandwidth;
    /** Reserved */
    t_u8 reserved;
} __attribute__ ((packed)) Power_Group_t;

/** MrvlTypes_Power_Group_t */
typedef struct _MrvlTypes_Power_Group_t
{
    /** Header Type */
    t_u16 type;
    /** Header Length */
    t_u16 length;
    /* Power_Group_t PowerGroups */
} __attribute__ ((packed)) MrvlTypes_Power_Group_t;

/** HostCmd_CMD_TXPWR_CFG */
typedef struct _HostCmd_DS_TXPWR_CFG
{
    /** Action */
    t_u16 action;
    /** Power group configuration index */
    t_u16 cfg_index;
    /** Power group configuration mode */
    t_u32 mode;
    /* MrvlTypes_Power_Group_t PowerGrpCfg[0] */
} __attribute__ ((packed)) HostCmd_DS_TXPWR_CFG;

/* 
 * This scan handle Country Information IE(802.11d compliant) 
 * Define data structure for HostCmd_CMD_802_11_SCAN 
 */
/** HostCmd_DS_802_11_SCAN */
typedef struct _HostCmd_DS_802_11_SCAN
{
    /** BSS type */
    t_u8 bss_type;
    /** BSSID */
    t_u8 bssid[MLAN_MAC_ADDR_LENGTH];
    /** TLV buffer */
    t_u8 tlv_buffer[1];
    /** MrvlIEtypes_SsIdParamSet_t      SsIdParamSet; 
     *  MrvlIEtypes_ChanListParamSet_t  ChanListParamSet;
     *  MrvlIEtypes_RatesParamSet_t     OpRateSet; 
     */
} __attribute__ ((packed)) HostCmd_DS_802_11_SCAN;

/** HostCmd_DS_802_11_SCAN_RSP */
typedef struct _HostCmd_DS_802_11_SCAN_RSP
{
    /** Size of BSS descriptor */
    t_u16 bss_descript_size;
    /** Numner of sets */
    t_u8 number_of_sets;
    /** BSS descriptor and TLV buffer */
    t_u8 bss_desc_and_tlv_buffer[1];
} __attribute__ ((packed)) HostCmd_DS_802_11_SCAN_RSP;

/** HostCmd_DS_802_11_AD_HOC_STOP */
typedef struct _HostCmd_DS_802_11_AD_HOC_STOP
{
} __attribute__ ((packed)) HostCmd_DS_802_11_AD_HOC_STOP;

/** MrvlIEtypes_DomainParamSet_t */
typedef struct _MrvlIEtypes_DomainParamSet
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** Country code */
    t_u8 country_code[COUNTRY_CODE_LEN];
    /** Set of subbands */
    IEEEtypes_SubbandSet_t sub_band[1];
} __attribute__ ((packed)) MrvlIEtypes_DomainParamSet_t;

/** HostCmd_DS_802_11D_DOMAIN_INFO */
typedef struct _HostCmd_DS_802_11D_DOMAIN_INFO
{
    /** Action */
    t_u16 action;
    /** Domain parameter set */
    MrvlIEtypes_DomainParamSet_t domain;
} __attribute__ ((packed)) HostCmd_DS_802_11D_DOMAIN_INFO;

/** HostCmd_DS_802_11D_DOMAIN_INFO_RSP */
typedef struct _HostCmd_DS_802_11D_DOMAIN_INFO_RSP
{
    /** Action */
    t_u16 action;
    /** Domain parameter set */
    MrvlIEtypes_DomainParamSet_t domain;
} __attribute__ ((packed)) HostCmd_DS_802_11D_DOMAIN_INFO_RSP;

/** HostCmd_DS_11N_ADDBA_REQ */
typedef struct _HostCmd_DS_11N_ADDBA_REQ
{
    /** Result of the ADDBA Request Operation */
    t_u8 add_req_result;
    /** Peer MAC address */
    t_u8 peer_mac_addr[MLAN_MAC_ADDR_LENGTH];
    /** Dialog Token */
    t_u8 dialog_token;
    /** Block Ack Parameter Set */
    t_u16 block_ack_param_set;
    /** Block Act Timeout Value */
    t_u16 block_ack_tmo;
    /** Starting Sequence Number */
    t_u16 ssn;
} __attribute__ ((packed)) HostCmd_DS_11N_ADDBA_REQ;

/** HostCmd_DS_11N_ADDBA_RSP */
typedef struct _HostCmd_DS_11N_ADDBA_RSP
{
    /** Result of the ADDBA Response Operation */
    t_u8 add_rsp_result;
    /** Peer MAC address */
    t_u8 peer_mac_addr[MLAN_MAC_ADDR_LENGTH];
    /** Dialog Token */
    t_u8 dialog_token;
    /** Status Code */
    t_u16 status_code;
    /** Block Ack Parameter Set */
    t_u16 block_ack_param_set;
    /** Block Act Timeout Value */
    t_u16 block_ack_tmo;
    /** Starting Sequence Number */
    t_u16 ssn;
} __attribute__ ((packed)) HostCmd_DS_11N_ADDBA_RSP;

/** HostCmd_DS_11N_DELBA */
typedef struct _HostCmd_DS_11N_DELBA
{
    /** Result of the ADDBA Request Operation */
    t_u8 del_result;
    /** Peer MAC address */
    t_u8 peer_mac_addr[MLAN_MAC_ADDR_LENGTH];
    /** Delete Block Ack Parameter Set */
    t_u16 del_ba_param_set;
    /** Reason Code sent for DELBA */
    t_u16 reason_code;
    /** Reserved */
    t_u8 reserved;
} __attribute__ ((packed)) HostCmd_DS_11N_DELBA;

/** HostCmd_DS_11N_BATIMEOUT */
typedef struct _HostCmd_DS_11N_BATIMEOUT
{
    /** TID */
    t_u8 tid;
    /** Peer MAC address */
    t_u8 peer_mac_addr[MLAN_MAC_ADDR_LENGTH];
    /** Delete Block Ack Parameter Set */
    t_u8 origninator;
} __attribute__ ((packed)) HostCmd_DS_11N_BATIMEOUT;

/** HostCmd_DS_11N_CFG */
typedef struct _HostCmd_DS_11N_CFG
{
    /** Action */
    t_u16 action;
    /** HTTxCap */
    t_u16 ht_tx_cap;
    /** HTTxInfo */
    t_u16 ht_tx_info;
} __attribute__ ((packed)) HostCmd_DS_11N_CFG;

/** HostCmd_DS_TXBUF_CFG*/
typedef struct _HostCmd_DS_TXBUF_CFG
{
    /** Action */
    t_u16 action;
    /** Buffer Size */
    t_u16 buff_size;
} __attribute__ ((packed)) HostCmd_DS_TXBUF_CFG;

/** HostCmd_DS_AMSDU_AGGR_CTRL */
typedef struct _HostCmd_DS_AMSDU_AGGR_CTRL
{
    /** Action */
    t_u16 action;
    /** Enable */
    t_u16 enable;
    /** Get the current Buffer Size valid */
    t_u16 curr_buf_size;
} __attribute__ ((packed)) HostCmd_DS_AMSDU_AGGR_CTRL;

/** HostCmd_DS_ECL_SYSTEM_CLOCK_CONFIG */
typedef struct _HostCmd_DS_ECL_SYSTEM_CLOCK_CONFIG
{
    /** Action */
    t_u16 action;
    /** Current system clock */
    t_u16 cur_sys_clk;
    /** Clock type */
    t_u16 sys_clk_type;
    /** Length of clocks */
    t_u16 sys_clk_len;
    /** System clocks */
    t_u16 sys_clk[16];
} __attribute__ ((packed)) HostCmd_DS_ECL_SYSTEM_CLOCK_CONFIG;

/** MrvlIEtypes_WmmParamSet_t */
typedef struct _MrvlIEtypes_WmmParamSet_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** WMM IE */
    t_u8 wmm_ie[1];
} MrvlIEtypes_WmmParamSet_t;

/** MrvlIEtypes_WmmQueueStatus_t */
typedef struct
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** Queue index */
    t_u8 queue_index;
    /** Disabled flag */
    t_u8 disabled;
    /** Medium time allocation in 32us units*/
    t_u16 medium_time;
    /** Flow required flag */
    t_u8 flow_required;
    /** Flow created flag */
    t_u8 flow_created;
    /** Reserved */
    t_u32 reserved;
} MrvlIEtypes_WmmQueueStatus_t;

/**
 *  @brief Firmware command structure to retrieve the firmware WMM status.
 *
 *  Used to retrieve the status of each WMM AC Queue in TLV 
 *    format (MrvlIEtypes_WmmQueueStatus_t) as well as the current WMM
 *    parameter IE advertised by the AP.  
 *  
 *  Used in response to a EVENT_WMM_STATUS_CHANGE event signaling
 *    a QOS change on one of the ACs or a change in the WMM Parameter in
 *    the Beacon.
 *
 *  TLV based command, byte arrays used for max sizing purpose. There are no 
 *    arguments sent in the command, the TLVs are returned by the firmware.
 */
typedef struct
{
    /** Queue status TLV */
    t_u8 queue_status_tlv[sizeof(MrvlIEtypes_WmmQueueStatus_t) * MAX_AC_QUEUES];
    /** WMM parameter TLV */
    t_u8 wmm_param_tlv[sizeof(IEEEtypes_WmmParameter_t) + 2];
}
HostCmd_DS_WMM_GET_STATUS;

/** 
 *  @brief IOCTL sub structure for a specific WMM AC Status
 */
typedef struct
{
    /** WMM Acm */
    t_u8 wmm_acm;
    /** Flow required flag */
    t_u8 flow_required;
    /** Flow created flag */
    t_u8 flow_created;
    /** Disabled flag */
    t_u8 disabled;
} wlan_ioctl_wmm_queue_status_ac_t;

/**
 *  @brief IOCTL structure to retrieve the WMM AC Queue status
 *
 *  IOCTL structure from the application layer to retrieve:
 *     - ACM bit setting for the AC
 *     - Firmware status (flow required, flow created, flow disabled)
 *
 *  @sa wlan_wmm_queue_status_ioctl
 */
typedef struct
{
    /** WMM AC queue status */
    wlan_ioctl_wmm_queue_status_ac_t ac_status[MAX_AC_QUEUES];
} wlan_ioctl_wmm_queue_status_t;

/** Firmware status for a specific AC */
typedef struct
{
    /** Disabled flag */
    t_u8 disabled;
    /** Flow required flag */
    t_u8 flow_required;
    /** Flow created flag */
    t_u8 flow_created;
} WmmAcStatus_t;

/** HT Capabilities element */
typedef struct _MrvlIETypes_HTCap_t
{
    /** Header */
    MrvlIEtypesHeader_t header;

    /** HTCap struct */
    HTCap_t ht_cap;
} __attribute__ ((packed)) MrvlIETypes_HTCap_t;

/** HT Information element */
typedef struct _MrvlIETypes_HTInfo_t
{
    /** Header */
    MrvlIEtypesHeader_t header;

    /** HTInfo struct */
    HTInfo_t ht_info;
} __attribute__ ((packed)) MrvlIETypes_HTInfo_t;

/** 20/40 BSS Coexistence element */
typedef struct _MrvlIETypes_2040BSSCo_t
{
    /** Header */
    MrvlIEtypesHeader_t header;

    /** BSSCo2040_t struct */
    BSSCo2040_t bss_co_2040;
} __attribute__ ((packed)) MrvlIETypes_2040BSSCo_t;

/** Extended Capabilities element */
typedef struct _MrvlIETypes_ExtCap_t
{
    /** Header */
    MrvlIEtypesHeader_t header;

    /** ExtCap_t struct */
    ExtCap_t ext_cap;
} __attribute__ ((packed)) MrvlIETypes_ExtCap_t;

/** Overlapping BSS Scan Parameters element */
typedef struct _MrvlIETypes_OverlapBSSScanParam_t
{
    /** Header */
    MrvlIEtypesHeader_t header;

    /** OBSSScanParam_t struct */
    OBSSScanParam_t obss_scan_param;
} __attribute__ ((packed)) MrvlIETypes_OverlapBSSScanParam_t;

/** Set of MCS values that STA desires to use within the BSS */
typedef struct _MrvlIETypes_HTOperationalMCSSet_t
{
    /** Header */
    MrvlIEtypesHeader_t header;

    /** Bitmap indicating MCSs that STA desires to use within the BSS */
    t_u8 ht_operational_mcs_bitmap[16];
} __attribute__ ((packed)) MrvlIETypes_HTOperationalMCSSet_t;

/** MrvlIEtypes_PMK_t */
typedef struct _MrvlIEtypes_PMK_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** PMK */
    t_u8 pmk[1];
} __attribute__ ((packed)) MrvlIEtypes_PMK_t;
/** MrvlIEtypes_Passphrase_t */
typedef struct _MrvlIEtypes_Passphrase_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** Passphrase */
    char passphrase[1];
} __attribute__ ((packed)) MrvlIEtypes_Passphrase_t;
/* unicastCipher -
 *      Bit 0   : RFU
 *      Bit 1   : RFU
 *      Bit 2   : TKIP
 *      Bit 3   : AES CCKM
 *      Bit 2-7 : RFU
 * multicastCipher -
 *      Bit 0   : WEP40
 *      Bit 1   : WEP104
 *      Bit 2   : TKIP
 *      Bit 3   : AES
 *      Bit 4-7 : Reserved for now
 */
/** MrvlIEtypes_Cipher_t */
typedef struct _MrvlIEtypes_Cipher_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** PairCipher */
    t_u8 pair_cipher;
    /** GroupCipher */
    t_u8 group_cipher;
} __attribute__ ((packed)) MrvlIEtypes_Cipher_t;
/* rsnMode -    
 *      Bit 0    : No RSN
 *      Bit 1-2  : RFU
 *      Bit 3    : WPA
 *      Bit 4    : WPA-NONE
 *      Bit 5    : WPA2
 *      Bit 6    : AES CCKM
 *      Bit 7-15 : RFU
 */
/** MrvlIEtypes_Cipher_t */
typedef struct _MrvlIEtypes_EncrProto_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** EncrProto */
    t_u16 rsn_mode;
} __attribute__ ((packed)) MrvlIEtypes_EncrProto_t;

/** MrvlIEtypes_Bssid_t */
typedef struct _MrvlIEtypes_Bssid_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** Bssid */
    t_u8 bssid[MLAN_MAC_ADDR_LENGTH];
} __attribute__ ((packed)) MrvlIEtypes_Bssid_t;

/*
 * This struct will handle GET,SET,CLEAR function for embedded
 * supplicant.
 * Define data structure for HostCmd_CMD_802_11_SUPPLICANT_PMK
 */
/** HostCmd_DS_802_11_SUPPLICANT_PMK */
typedef struct _HostCmd_DS_802_11_SUPPLICANT_PMK
{
    /** CMD Action GET/SET/CLEAR */
    t_u16 action;
    /** CacheResult initialized to 0 */
    t_u16 cache_result;
    /** TLV Buffer */
    t_u8 tlv_buffer[1];
    /** MrvlIEtypes_SsidParamSet_t  SsidParamSet;
      * MrvlIEtypes_PMK_t           Pmk;
      * MrvlIEtypes_Passphrase_t    Passphrase;
      * MrvlIEtypes_Bssid_t         Bssid;
      **/
} __attribute__ ((packed)) HostCmd_DS_802_11_SUPPLICANT_PMK;

/*
 * This struct will GET the Supplicant supported bitmaps
 * The GET_CURRENT action will get the network profile used
 * for the current assocation.
 * Define data structure for HostCmd_CMD_802_11_SUPPLICANT_PROFILE
 */
typedef struct _HostCmd_DS_802_11_SUPPLICANT_PROFILE
{
    /** GET/SET/GET_CURRENT */
    t_u16 action;
    /** Reserved */
    t_u16 reserved;
    /** TLVBuffer */
    t_u8 tlv_buf[1];
    /* MrvlIEtypes_EncrProto_t */
} __attribute__ ((packed)) HostCmd_DS_802_11_SUPPLICANT_PROFILE;

/** HostCmd_CMD_802_11_RF_CHANNEL */
typedef struct _HostCmd_DS_802_11_RF_CHANNEL
{
    /** Action */
    t_u16 action;
    /** Current channel */
    t_u16 current_channel;
    /** RF type */
    t_u16 rf_type;
    /** Reserved field */
    t_u16 reserved;
    /** Reserved */
    t_u8 reserved_1[32];
} __attribute__ ((packed)) HostCmd_DS_802_11_RF_CHANNEL;

/** HostCmd_DS_VERSION_EXT */
typedef struct _HostCmd_DS_VERSION_EXT
{
    /** Selected version string */
    t_u8 version_str_sel;
    /** Version string */
    char version_str[128];
} __attribute__ ((packed)) HostCmd_DS_VERSION_EXT;

/** HostCmd_CMD_802_11_RF_ANTENNA */
typedef struct _HostCmd_DS_802_11_RF_ANTENNA
{
    /** Action */
    t_u16 action;
    /**  Number of antennas or 0xffff(diversity) */
    t_u16 antenna_mode;
} __attribute__ ((packed)) HostCmd_DS_802_11_RF_ANTENNA;

/** HostCmd_DS_802_11_IBSS_STATUS */
typedef struct _HostCmd_DS_802_11_IBSS_STATUS
{
    /** Action */
    t_u16 action;
    /** Enable */
    t_u16 enable;
    /** BSSID */
    t_u8 bssid[MLAN_MAC_ADDR_LENGTH];
    /** Beacon interval */
    t_u16 beacon_interval;
    /** ATIM window interval */
    t_u16 atim_window;
    /** User G rate protection */
    t_u16 use_g_rate_protect;
} __attribute__ ((packed)) HostCmd_DS_802_11_IBSS_STATUS;

/** HostCmd_DS_HEART_BEAT */
typedef struct _HostCmd_DS_HEART_BEAT
{
    /** Action */
    t_u16 action;
    /** Timer for the host */
    t_u16 h2d_timer;
    /** Timer for the device */
    t_u16 d2h_timer;
} __attribute__ ((packed)) HostCmd_DS_HEART_BEAT;

/** HostCmd_CMD_MAC_REG_ACCESS */
typedef struct _HostCmd_DS_MAC_REG_ACCESS
{
    /** Action */
    t_u16 action;
    /** MAC register offset */
    t_u16 offset;
    /** MAC register value */
    t_u32 value;
} __attribute__ ((packed)) HostCmd_DS_MAC_REG_ACCESS;

/** HostCmd_CMD_BBP_REG_ACCESS */
typedef struct _HostCmd_DS_BBP_REG_ACCESS
{
    /** Acion */
    t_u16 action;
    /** BBP register offset */
    t_u16 offset;
    /** BBP register value */
    t_u8 value;
    /** Reserved field */
    t_u8 reserved[3];
} __attribute__ ((packed)) HostCmd_DS_BBP_REG_ACCESS;

/**  HostCmd_CMD_RF_REG_ACCESS */
typedef struct _HostCmd_DS_RF_REG_ACCESS
{
    /** Action */
    t_u16 action;
    /** RF register offset */
    t_u16 offset;
    /** RF register value */
    t_u8 value;
    /** Reserved field */
    t_u8 reserved[3];
} __attribute__ ((packed)) HostCmd_DS_RF_REG_ACCESS;

/**  HostCmd_CMD_PMIC_REG_ACCESS */
typedef struct _HostCmd_DS_PMIC_REG_ACCESS
{
    /** Action */
    t_u16 action;
    /** RF register offset */
    t_u16 offset;
    /** RF register value */
    t_u8 value;
    /** Reserved field */
    t_u8 reserved[3];
} __attribute__ ((packed)) HostCmd_DS_PMIC_REG_ACCESS;

/** HostCmd_DS_802_11_EEPROM_ACCESS */
typedef struct _HostCmd_DS_802_11_EEPROM_ACCESS
{
        /** Action */
    t_u16 action;

        /** multiple 4 */
    t_u16 offset;
        /** Number of bytes */
    t_u16 byte_count;
        /** Value */
    t_u8 value;
} __attribute__ ((packed)) HostCmd_DS_802_11_EEPROM_ACCESS;

/** HostCmd_DS_INACTIVITY_TIMEOUT_EXT */
typedef struct _HostCmd_DS_INACTIVITY_TIMEOUT_EXT
{
    /** ACT_GET/ACT_SET */
    t_u16 action;
    /** uS, 0 means 1000uS(1ms) */
    t_u16 timeout_unit;
    /** Inactivity timeout for unicast data */
    t_u16 unicast_timeout;
    /** Inactivity timeout for multicast data */
    t_u16 mcast_timeout;
    /** Timeout for additional RX traffic after Null PM1 packet exchange */
    t_u16 ps_entry_timeout;
    /** Reserved to further expansion */
    t_u16 reserved;
} __attribute__ ((packed)) HostCmd_DS_INACTIVITY_TIMEOUT_EXT;

/** HostCmd_DS_COMMAND */
typedef struct _HostCmd_DS_COMMAND
{
    /** Command Header : Command */
    t_u16 command;
    /** Command Header : Size */
    t_u16 size;
    /** Command Header : Sequence number */
    t_u16 seq_num;
    /** Command Header : Result */
    t_u16 result;
    /** Command Body */
    union
    {
        /** Hardware specifications */
        HostCmd_DS_GET_HW_SPEC hw_spec;
        /** MAC control */
        HostCmd_DS_MAC_CONTROL mac_ctrl;
        /** MAC address */
        HostCmd_DS_802_11_MAC_ADDRESS mac_addr;
        /** MAC muticast address */
        HostCmd_DS_MAC_MULTICAST_ADR mc_addr;
        /** Get log */
        HostCmd_DS_802_11_GET_LOG get_log;
        /** RSSI information */
        HostCmd_DS_802_11_RSSI_INFO rssi_info;
        /** RSSI information response */
        HostCmd_DS_802_11_RSSI_INFO_RSP rssi_info_rsp;
        /** SNMP MIB */
        HostCmd_DS_802_11_SNMP_MIB smib;
        /** Radio control */
        HostCmd_DS_802_11_RADIO_CONTROL radio;
        /** RF channel */
        HostCmd_DS_802_11_RF_CHANNEL rf_channel;
        /** Tx rate query */
        HostCmd_TX_RATE_QUERY tx_rate;
        /** Tx rate configuration */
        HostCmd_DS_TX_RATE_CFG tx_rate_cfg;
        /** Tx power configuration */
        HostCmd_DS_TXPWR_CFG txp_cfg;
        /** RF antenna */
        HostCmd_DS_802_11_RF_ANTENNA antenna;
        /** Power Save mode */
        HostCmd_DS_802_11_PS_MODE ps_mode;
        HostCmd_DS_802_11_HOST_SLEEP_CFG hs_cfg;
        /** Scan */
        HostCmd_DS_802_11_SCAN scan;
        /** Scan response */
        HostCmd_DS_802_11_SCAN_RSP scan_resp;
        /** Associate */
        HostCmd_DS_802_11_ASSOCIATE associate;
        /** Associate response */
        HostCmd_DS_802_11_ASSOCIATE_RSP associate_rsp;
        /** Deauthenticate */
        HostCmd_DS_802_11_DEAUTHENTICATE deauth;
        /** Ad-Hoc start */
        HostCmd_DS_802_11_AD_HOC_START adhoc_start;
        /** Ad-Hoc result */
        HostCmd_DS_802_11_AD_HOC_RESULT adhoc_result;
        /** Ad-Hoc join */
        HostCmd_DS_802_11_AD_HOC_JOIN adhoc_join;
        /** Ad-Hoc stop */
        HostCmd_DS_802_11_AD_HOC_STOP adhoc_stop;
        /** Domain information */
        HostCmd_DS_802_11D_DOMAIN_INFO domain_info;
        /** Domain information response */
        HostCmd_DS_802_11D_DOMAIN_INFO_RSP domain_info_resp;
        /** Add BA request */
        HostCmd_DS_11N_ADDBA_REQ add_ba_req;
        /** Add BA response */
        HostCmd_DS_11N_ADDBA_RSP add_ba_rsp;
        /** Delete BA entry */
        HostCmd_DS_11N_DELBA del_ba;
        /** Tx buffer configuration */
        HostCmd_DS_TXBUF_CFG tx_buf;
        /** AMSDU Aggr Ctrl configuration */
        HostCmd_DS_AMSDU_AGGR_CTRL amsdu_aggr_ctrl;
        /** 11n configuration */
        HostCmd_DS_11N_CFG htcfg;
        /** WMM status get */
        HostCmd_DS_WMM_GET_STATUS get_wmm_status;
        /** Key material */
        HostCmd_DS_802_11_KEY_MATERIAL key_material;
        /** E-Supplicant PSK */
        HostCmd_DS_802_11_SUPPLICANT_PMK esupplicant_psk;
        /** E-Supplicant profile */
        HostCmd_DS_802_11_SUPPLICANT_PROFILE esupplicant_profile;
        /** Extended version */
        HostCmd_DS_VERSION_EXT verext;
        /** Adhoc Coalescing */
        HostCmd_DS_802_11_IBSS_STATUS ibss_coalescing;
        /** System clock configuration */
        HostCmd_DS_ECL_SYSTEM_CLOCK_CONFIG sys_clock_cfg;
        /** Heart Beat configuration */
        HostCmd_DS_HEART_BEAT heart_beat;
        /** MAC register access */
        HostCmd_DS_MAC_REG_ACCESS mac_reg;
        /** BBP register access */
        HostCmd_DS_BBP_REG_ACCESS bbp_reg;
        /** RF register access */
        HostCmd_DS_RF_REG_ACCESS rf_reg;
        /** PMIC register access */
        HostCmd_DS_PMIC_REG_ACCESS pmic_reg;
        /** EEPROM register access */
        HostCmd_DS_802_11_EEPROM_ACCESS eeprom;
        /** Inactivity timeout extend */
        HostCmd_DS_INACTIVITY_TIMEOUT_EXT inactivity_to;
    } params;
} HostCmd_DS_COMMAND;

#endif /* !_MLAN_FW_H_ */
