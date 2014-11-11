/** @file mlan_fw.h
 *
 *  @brief This file contains firmware specific defines.
 *  structures and declares global function prototypes used
 *  in MLAN module.
 *
 *  Copyright (C) 2008-2010, Marvell International Ltd. 
 *  All Rights Reserved
 */

/******************************************************
Change log:
    10/27/2008: initial version
******************************************************/

#ifndef _MLAN_FW_H_
#define _MLAN_FW_H_

/** Interface header length */
#define INTF_HEADER_LEN     4

/** Ethernet header */
typedef struct
{
    /** Ethernet header destination address */
    t_u8 dest_addr[MLAN_MAC_ADDR_LENGTH];
    /** Ethernet header source address */
    t_u8 src_addr[MLAN_MAC_ADDR_LENGTH];
    /** Ethernet header length */
    t_u16 h803_len;

} Eth803Hdr_t;

/** RFC 1042 header */
typedef struct
{
    /** LLC DSAP */
    t_u8 llc_dsap;
    /** LLC SSAP */
    t_u8 llc_ssap;
    /** LLC CTRL */
    t_u8 llc_ctrl;
    /** SNAP OUI */
    t_u8 snap_oui[3];
    /** SNAP type */
    t_u16 snap_type;

} Rfc1042Hdr_t;

/** Rx packet header */
typedef struct
{
    /** Etherner header */
    Eth803Hdr_t eth803_hdr;
    /** RFC 1042 header */
    Rfc1042Hdr_t rfc1042_hdr;

} RxPacketHdr_t;

/** Rates supported in band B */
#define B_SUPPORTED_RATES               5
/** Rates supported in band G */
#define G_SUPPORTED_RATES               9
/** Rates supported in band BG */
#define BG_SUPPORTED_RATES              13

/** Setup the number of rates passed in the driver/firmware API */
#define A_SUPPORTED_RATES               9

/** Setup the number of rates passed in the driver/firmware API */
#define HOSTCMD_SUPPORTED_RATES         14

#ifdef STA_SUPPORT
/** Rates supported in band N */
#define N_SUPPORTED_RATES               3
/** All bands (B, G, N) */
#define ALL_802_11_BANDS           (BAND_A | BAND_B | BAND_G | BAND_GN)
#else
/** All bands (B, G, A) */
#define ALL_802_11_BANDS        (BAND_B | BAND_G | BAND_A)
#endif /* STA_SUPPORT */

#ifdef STA_SUPPORT
/** Firmware multiple bands support */
#define FW_MULTI_BANDS_SUPPORT  (MBIT(8) | MBIT(9) | MBIT(10) | MBIT(11))
#else
/** Firmware multiple bands support */
#define FW_MULTI_BANDS_SUPPORT  (MBIT(8) | MBIT(9) | MBIT(10))
#endif /* STA_SUPPORT */
/** Check if multiple bands support is enabled in firmware */
#define IS_SUPPORT_MULTI_BANDS(_adapter)        \
        (_adapter->fw_cap_info & FW_MULTI_BANDS_SUPPORT)
/** Get default bands of the firmware */
#define GET_FW_DEFAULT_BANDS(_adapter)  \
        ((_adapter->fw_cap_info >> 8) & ALL_802_11_BANDS)

extern t_u8 SupportedRates_B[B_SUPPORTED_RATES];
extern t_u8 SupportedRates_G[G_SUPPORTED_RATES];
extern t_u8 SupportedRates_BG[BG_SUPPORTED_RATES];
extern t_u8 SupportedRates_A[A_SUPPORTED_RATES];
#ifdef STA_SUPPORT
extern t_u8 SupportedRates_N[N_SUPPORTED_RATES];
#endif /* STA_SUPPORT */
extern t_u8 AdhocRates_G[G_SUPPORTED_RATES];
extern t_u8 AdhocRates_B[B_SUPPORTED_RATES];
extern t_u8 AdhocRates_BG[BG_SUPPORTED_RATES];
extern t_u8 AdhocRates_A[A_SUPPORTED_RATES];

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
    KEY_TYPE_ID_AES,
    KEY_TYPE_ID_WAPI,
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

/** WAPI key length */
#define WAPI_KEY_LEN            50
/** KEY_INFO_WAPI*/
typedef enum _KEY_INFO_WAPI
{
    KEY_INFO_WAPI_MCAST = 0x01,
    KEY_INFO_WAPI_UNICAST = 0x02,
    KEY_INFO_WAPI_ENABLED = 0x04
} KEY_INFO_WAPI;

/** Length of SNAP header */
#define MRVDRV_SNAP_HEADER_LEN          8

/** The number of times to try when polling for status bits */
#define MAX_POLL_TRIES			100

/** The number of times to try when waiting for downloaded firmware to 
     become active when multiple interface is present */
#define MAX_MULTI_INTERFACE_POLL_TRIES  1000

/** The number of times to try when waiting for downloaded firmware to 
     become active. (polling the scratch register). */
#define MAX_FIRMWARE_POLL_TRIES		100

/** This is for firmware specific length */
#define EXTRA_LEN	36

/** Maximum ethernet frame length sans FCS */
#define MV_ETH_FRAME_LEN   1514

/** Buffer size for ethernet Tx packets */
#define MRVDRV_ETH_TX_PACKET_BUFFER_SIZE \
	(MV_ETH_FRAME_LEN + sizeof(TxPD) + EXTRA_LEN)

/** Buffer size for ethernet Rx packets */
#define MRVDRV_ETH_RX_PACKET_BUFFER_SIZE \
	(MV_ETH_FRAME_LEN + sizeof(RxPD) \
	 + MRVDRV_SNAP_HEADER_LEN + EXTRA_LEN)

/* Macros in interface module */
/** Firmware ready */
#define FIRMWARE_READY          0xfedc

/** Number of firmware blocks to transfer */
#define FIRMWARE_TRANSFER_NBLOCK	2

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
#define CAL_SNR(RSSI, NF)		((t_s16)((t_s16)(RSSI)-(t_s16)(NF)))

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

#ifdef STA_SUPPORT
/** TLV type : Domain */
#define TLV_TYPE_DOMAIN                         0x0007
#endif /* STA_SUPPORT */

/** TLV type : Power constraint */
#define TLV_TYPE_POWER_CONSTRAINT               0x0020
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
/** TLV type: WAPI IE */
#define TLV_TYPE_WAPI_IE            (PROPRIETARY_TLV_BASE_ID + 94)
/** TLV type: MGMT IE */
#define TLV_TYPE_MGMT_IE            (PROPRIETARY_TLV_BASE_ID + 105)

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

/** 2K buf size */
#define MLAN_TX_DATA_BUF_SIZE_2K        2048

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
#define IS_11N_ENABLED(priv) ((priv->adapter->config_bands & BAND_GN ||priv->adapter->config_bands & BAND_AN) \
        && priv->curr_bss_params.bss_descriptor.pht_cap)
/** Find out if we are the initiator or not */
#define INITIATOR_BIT(DelBAParamSet) (((DelBAParamSet) & \
                        MBIT(DELBA_INITIATOR_POS)) >> DELBA_INITIATOR_POS)

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
/** RESET HTExtCap : Clear RD Responder bit */
#define RESETHT_EXTCAP_RDG(HTExtCap) (HTExtCap &= ~MBIT(11))
/** SET MCS32 */
#define SETHT_MCS32(x) (x[4] |= 1)
/** Set mcs set defined bit */
#define SETHT_MCS_SET_DEFINED(x) (x[12] |= 1)
/* Set the highest Rx data rate */
#define SETHT_RX_HIGHEST_DT_SUPP(x, y) ((*(t_u16 *) (x + 10)) = y)
/** AMPDU factor size */
#define AMPDU_FACTOR_64K	0x03
/** Set AMPDU size in A-MPDU paramter field */
#define SETAMPDU_SIZE(x, y) do { \
	x = x & ~0x03; \
	x |= y & 0x03; \
} while (0) \
/** Set AMPDU spacing in A-MPDU paramter field */
#define SETAMPDU_SPACING(x, y) do { \
	x = x & ~0x1c; \
	x |= (y & 0x07) << 2; \
} while (0) \

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

#define TLV_TYPE_IEEE_ACTION_FRAME (PROPRIETARY_TLV_BASE_ID + 140)

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

/** Host Command ID : 802.22 MAC address */
#define HostCmd_CMD_802_11_MAC_ADDRESS        0x004D

/** Host Command ID : WMM Traffic Stream Status */
#define HostCmd_CMD_WMM_TS_STATUS             0x005d

#ifdef STA_SUPPORT
/** Host Command ID : 802.11 D domain information */
#define HostCmd_CMD_802_11D_DOMAIN_INFO       0x005b
#endif /* STA_SUPPORT */

/** Host Command ID : 802.11 TPC information */
#define HostCmd_CMD_802_11_TPC_INFO           0x005f
/** Host Command ID : 802.11 TPC adapt req */
#define HostCmd_CMD_802_11_TPC_ADAPT_REQ      0x0060
/** Host Command ID : 802.11 channel SW ann */
#define HostCmd_CMD_802_11_CHAN_SW_ANN        0x0061
/** Host Command ID : Measurement request */
#define HostCmd_CMD_MEASUREMENT_REQUEST       0x0062
/** Host Command ID : Measurement report */
#define HostCmd_CMD_MEASUREMENT_REPORT        0x0063

/** Host Command ID : 802.11 sleep parameters */
#define HostCmd_CMD_802_11_SLEEP_PARAMS       0x0066

/** Host Command ID : 802.11 sleep period */
#define HostCmd_CMD_802_11_SLEEP_PERIOD       0x0068
/** Host Command ID : 802.11 BCA configuration timeshare */
#define HostCmd_CMD_802_11_BCA_CONFIG_TIMESHARE  0x0069

/** Host Command ID : 802.11 BG scan query */
#define HostCmd_CMD_802_11_BG_SCAN_QUERY      0x006c

/** Host Command ID : WMM ADDTS req */
#define HostCmd_CMD_WMM_ADDTS_REQ             0x006E
/** Host Command ID : WMM DELTS req */
#define HostCmd_CMD_WMM_DELTS_REQ             0x006F
/** Host Command ID : WMM queue configuration */
#define HostCmd_CMD_WMM_QUEUE_CONFIG          0x0070
/** Host Command ID : 802.11 get status */
#define HostCmd_CMD_WMM_GET_STATUS            0x0071

/** Host Command ID : 802.11 Tx rate query */
#define HostCmd_CMD_802_11_TX_RATE_QUERY      0x007f

/** Host Command ID : WMM queue stats */
#define HostCmd_CMD_WMM_QUEUE_STATS           0x0081

/** Host Command ID : 802.11 IBSS coalescing status */
#define HostCmd_CMD_802_11_IBSS_COALESCING_STATUS 0x0083

/** Host Command ID : Memory access */
#define HostCmd_CMD_MEM_ACCESS                0x0086

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

/** Host Command ID : SDIO pull control */
#define HostCmd_CMD_SDIO_PULL_CTRL            0x0093

/** Host Command ID : ECL system clock configuration */
#define HostCmd_CMD_ECL_SYSTEM_CLOCK_CONFIG   0x0094

/** Host Command ID : 802.11 LDO configuration */
#define HostCmd_CMD_802_11_LDO_CONFIG         0x0096

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

/** Host Command ID : Soft Reset */
#define  HostCmd_CMD_SOFT_RESET		          0x00d5

/** Host Command ID : 802.11 b/g/n rate configration */
#define HostCmd_CMD_TX_RATE_CFG               0x00d6

/** Host Command ID : Enhanced PS mode */
#define HostCmd_CMD_802_11_PS_MODE_ENH        0x00e4
/** Host command action : Host sleep configuration */
#define HostCmd_CMD_802_11_HS_CFG_ENH         0x00e5

/** Host Command ID : CAU register access */
#define HostCmd_CMD_CAU_REG_ACCESS            0x00ed

/** Host Command ID : mgmt IE list */
#define  HostCmd_CMD_MGMT_IE_LIST             0x00f2

/** Host Command ID : Set BSS_MODE */
#define HostCmd_CMD_SET_BSS_MODE               0x00f7

#ifdef UAP_SUPPORT
/**  Host Command id: SYS_INFO */
#define HOST_CMD_APCMD_SYS_INFO                0x00ae
/** Host Command id: sys_reset */
#define HOST_CMD_APCMD_SYS_RESET               0x00af
/** Host Command id: SYS_CONFIGURE  */
#define HOST_CMD_APCMD_SYS_CONFIGURE           0x00b0
/** Host Command id: BSS_START */
#define HOST_CMD_APCMD_BSS_START               0x00b1
/** Host Command id: BSS_STOP  */
#define HOST_CMD_APCMD_BSS_STOP                0x00b2
/** Host Command id: sta_list */
#define HOST_CMD_APCMD_STA_LIST                0x00b3
/** Host Command id: STA_DEAUTH */
#define HOST_CMD_APCMD_STA_DEAUTH              0x00b5
#endif /* UAP_SUPPORT */

/** Enhanced PS modes */
typedef enum _ENH_PS_MODES
{
    EN_PS = 1,
    DIS_PS = 2,
    EN_AUTO_DS = 3,
    DIS_AUTO_DS = 4,
    SLEEP_CONFIRM = 5,
    GET_PS = 0,
    EN_AUTO_PS = 0xff,
    DIS_AUTO_PS = 0xfe,
} ENH_PS_MODES;

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
/** MAC action : WEP enable */
#define HostCmd_ACT_MAC_WEP_ENABLE            0x0008
/** MAC action : EthernetII enable */
#define HostCmd_ACT_MAC_ETHERNETII_ENABLE     0x0010
/** MAC action : Promiscous mode enable */
#define HostCmd_ACT_MAC_PROMISCUOUS_ENABLE    0x0080
/** MAC action : All multicast enable */
#define HostCmd_ACT_MAC_ALL_MULTICAST_ENABLE  0x0100
/** MAC action : RTS/CTS enable */
#define HostCmd_ACT_MAC_RTS_CTS_ENABLE        0x0200
/** MAC action : Strict protection enable */
#define HostCmd_ACT_MAC_STRICT_PROTECTION_ENABLE  0x0400
/** MAC action : Ad-Hoc G protection on */
#define HostCmd_ACT_MAC_ADHOC_G_PROTECTION_ON     0x2000

/* Define action or option for HostCmd_CMD_802_11_SCAN */
/** Scan type : BSS */
#define HostCmd_BSS_MODE_BSS                0x0001
/** Scan type : IBSS */
#define HostCmd_BSS_MODE_IBSS               0x0002
/** Scan type : Any */
#define HostCmd_BSS_MODE_ANY                0x0003

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

#ifdef STA_SUPPORT
/** Define bitmap conditions for HOST_SLEEP_CFG : GPIO FF */
#define HOST_SLEEP_CFG_GPIO_FF              0xff
/** Define bitmap conditions for HOST_SLEEP_CFG : GAP FF */
#define HOST_SLEEP_CFG_GAP_FF               0xff
#endif /* STA_SUPPORT */

/** Buffer Constants */
/** Number of command buffers */
#define MRVDRV_NUM_OF_CMD_BUFFER        20
/** Size of command buffer */
#define MRVDRV_SIZE_OF_CMD_BUFFER       (2 * 1024)

/** Maximum number of BSS Descriptors */
#define MRVDRV_MAX_BSSID_LIST           64

/** Host command flag in command */
#define CMD_F_HOSTCMD           (1 << 0)
/** command cancel flag in command */
#define CMD_F_CANCELED          (1 << 1)

/** Host Command ID bit mask (bit 11:0) */
#define HostCmd_CMD_ID_MASK             0x0fff

/** Host Command Sequence number mask (bit 7:0) */
#define HostCmd_SEQ_NUM_MASK            0x00ff

/** Host Command BSS number mask (bit 11:8) */
#define HostCmd_BSS_NUM_MASK            0x0f00

/** Host Command BSS type mask (bit 15:12) */
#define HostCmd_BSS_TYPE_MASK           0xf000

/** Set BSS information to Host Command */
#define HostCmd_SET_SEQ_NO_BSS_INFO(seq, num, type)    \
    (((seq) & 0x00ff) |         \
     (((num) & 0x000f) << 8)) | \
     (((type) & 0x000f) << 12)

/** Get Sequence Number from Host Command (bit 7:0) */
#define HostCmd_GET_SEQ_NO(seq)       \
        ((seq) & HostCmd_SEQ_NUM_MASK)

/** Get BSS number from Host Command (bit 11:8) */
#define HostCmd_GET_BSS_NO(seq)         \
        (((seq) & HostCmd_BSS_NUM_MASK) >> 8)

/** Get BSS type from Host Command (bit 15:12) */
#define HostCmd_GET_BSS_TYPE(seq)       \
        (((seq) & HostCmd_BSS_TYPE_MASK) >> 12)

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
#ifdef STA_SUPPORT
/** Card Event definition : Deep Sleep awake */
#define EVENT_DEEP_SLEEP_AWAKE          0x00000010
#endif /* STA_SUPPORT */
/** Card Event definition : Ad-Hoc BCN lost */
#define EVENT_ADHOC_BCN_LOST            0x00000011
/** Card Event definition : Stop Tx */
#define EVENT_STOP_TX                   0x00000013
/** Card Event definition : Start Tx */
#define EVENT_START_TX                  0x00000014
/** Card Event definition : Channel switch */
#define EVENT_CHANNEL_SWITCH            0x00000015
/** Card Event definition : MEAS report ready */
#define EVENT_MEAS_REPORT_RDY           0x00000016
/** Card Event definition : WMM status change */
#define EVENT_WMM_STATUS_CHANGE         0x00000017

/** Card Event definition : BG scan report */
#define EVENT_BG_SCAN_REPORT            0x00000018

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
/** Card Event definition : AMSDU aggr control */
#define EVENT_AMSDU_AGGR_CTRL           0x00000042
/** Card Event definition: WEP ICV error */
#define EVENT_WEP_ICV_ERR               0x00000046
/** Card Event definition : Host sleep enable */
#define EVENT_HS_ACT_REQ                0x00000047
/** Card Event definition : BW changed */
#define EVENT_BW_CHANGE                 0x00000048

/** Event definition: Host wake event when elem added to STADB */
#define EVENT_HOSTWAKE_STAIE					0x0000004d

/** Card Event definition: Channel switch pending announcment */
#define EVENT_CHANNEL_SWITCH_ANN        0x00000050

#ifdef UAP_SUPPORT
/** Event ID: STA deauth */
#define EVENT_MICRO_AP_STA_DEAUTH           0x0000002c

/** Event ID: STA assoicated */
#define EVENT_MICRO_AP_STA_ASSOC            0x0000002d
/** Event ID: BSS started */
#define EVENT_MICRO_AP_BSS_START            0x0000002e

/** Event ID: BSS idle event */
#define EVENT_MICRO_AP_BSS_IDLE	            0x00000043

/** Event ID: BSS active event */
#define EVENT_MICRO_AP_BSS_ACTIVE           0x00000044

#endif /* UAP_SUPPORT */

/** Event ID mask */
#define EVENT_ID_MASK                   0xffff

/** Get BSS number from event cause (bit 23:16) */
#define EVENT_GET_BSS_NUM(event_cause)          \
    (((event_cause) >> 16) & (MLAN_MAX_BSS_NUM - 1))

/** Get BSS type from event cause (bit 31:24) */
#define EVENT_GET_BSS_TYPE(event_cause)         \
    (((event_cause) >> 24) & 0x00ff)

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

#ifdef PRAGMA_PACK
#pragma pack(push, 1)
#endif

/** TLV related data structures*/
/** MrvlIEtypesHeader_t */
typedef MLAN_PACK_START struct _MrvlIEtypesHeader
{
    /** Header type */
    t_u16 type;
    /** Header length */
    t_u16 len;
} MLAN_PACK_END MrvlIEtypesHeader_t;

/** MrvlIEtypes_Data_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_Data_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** Data */
    t_u8 data[1];
} MLAN_PACK_END MrvlIEtypes_Data_t;

/** Bit mask for TxPD status field for null packet */
#define MRVDRV_TxPD_POWER_MGMT_NULL_PACKET 0x01
/** Bit mask for TxPD status field for last packet */
#define MRVDRV_TxPD_POWER_MGMT_LAST_PACKET 0x08

/** TxPD descriptor */
typedef MLAN_PACK_START struct _TxPD
{
    /** BSS type */
    t_u8 bss_type;
    /** BSS number */
    t_u8 bss_num;
    /** Tx packet length */
    t_u16 tx_pkt_length;
    /** Tx packet offset */
    t_u16 tx_pkt_offset;
    /** Tx packet type */
    t_u16 tx_pkt_type;
    /** Tx Control */
    t_u32 tx_control;
    /** Pkt Priority */
    t_u8 priority;
    /** Transmit Pkt Flags*/
    t_u8 flags;
    /** Amount of time the packet has been queued in the driver (units = 2ms)*/
    t_u8 pkt_delay_2ms;
    /** Reserved */
    t_u8 reserved1;
} MLAN_PACK_END TxPD, *PTxPD;

/** RxPD Descriptor */
typedef MLAN_PACK_START struct _RxPD
{
    /** BSS type */
    t_u8 bss_type;
    /** BSS number */
    t_u8 bss_num;
    /** Rx Packet Length */
    t_u16 rx_pkt_length;
    /** Rx Pkt offset */
    t_u16 rx_pkt_offset;
    /** Rx packet type */
    t_u16 rx_pkt_type;
    /** Sequence number */
    t_u16 seq_num;
    /** Packet Priority */
    t_u8 priority;
    /** Rx Packet Rate */
    t_u8 rx_rate;
    /** SNR */
    t_s8 snr;
    /** Noise Floor */
    t_s8 nf;
    /** Ht Info [Bit 0] RxRate format: LG=0, HT=1
     * [Bit 1]  HT Bandwidth: BW20 = 0, BW40 = 1
     * [Bit 2]  HT Guard Interval: LGI = 0, SGI = 1 */
    t_u8 ht_info;
    /** Reserved */
    t_u8 reserved;
} MLAN_PACK_END RxPD, *PRxPD;

#ifdef UAP_SUPPORT
/** TxPD descriptor */
typedef MLAN_PACK_START struct _UapTxPD
{
        /** BSS type */
    t_u8 bss_type;
        /** BSS number */
    t_u8 bss_num;
        /** Tx packet length */
    t_u16 tx_pkt_length;
        /** Tx packet offset */
    t_u16 tx_pkt_offset;
    /** Tx packet type */
    t_u16 tx_pkt_type;
        /** Tx Control */
    t_u32 tx_control;
        /** Pkt Priority */
    t_u8 priority;
        /** Transmit Pkt Flags*/
    t_u8 flags;
        /** Amount of time the packet has been queued in the driver (units = 2ms)*/
    t_u8 pkt_delay_2ms;
        /** Reserved */
    t_u8 reserved1;
        /** Reserved */
    t_u32 reserved;
} MLAN_PACK_END UapTxPD, *PUapTxPD;

/** RxPD Descriptor */
typedef MLAN_PACK_START struct _UapRxPD
{
        /** BSS Type */
    t_u8 bss_type;
        /** BSS number*/
    t_u8 bss_num;
        /** Rx packet length */
    t_u16 rx_pkt_length;
        /** Rx packet offset */
    t_u16 rx_pkt_offset;
        /** Rx packet type */
    t_u16 rx_pkt_type;
    /** Sequence number */
    t_u16 seq_num;
    /** Packet Priority */
    t_u8 priority;
    /** reserved */
    t_u8 reserved1;
} MLAN_PACK_END UapRxPD, *PUapRxPD;

/** IEEEtypes_FrameCtl_t*/
#ifdef BIG_ENDIAN_SUPPORT
typedef MLAN_PACK_START struct _IEEEtypes_FrameCtl_t
{
    /** Order */
    t_u8 order:1;
    /** Wep */
    t_u8 wep:1;
    /** More Data */
    t_u8 more_data:1;
    /** Power Mgmt */
    t_u8 pwr_mgmt:1;
    /** Retry */
    t_u8 retry:1;
    /** More Frag */
    t_u8 more_frag:1;
    /** From DS */
    t_u8 from_ds:1;
    /** To DS */
    t_u8 to_ds:1;
    /** Sub Type */
    t_u8 sub_type:4;
    /** Type */
    t_u8 type:2;
    /** Protocol Version */
    t_u8 protocol_version:2;
} MLAN_PACK_END IEEEtypes_FrameCtl_t;
#else
typedef MLAN_PACK_START struct _IEEEtypes_FrameCtl_t
{
    /** Protocol Version */
    t_u8 protocol_version:2;
    /** Type */
    t_u8 type:2;
    /** Sub Type */
    t_u8 sub_type:4;
    /** To DS */
    t_u8 to_ds:1;
    /** From DS */
    t_u8 from_ds:1;
    /** More Frag */
    t_u8 more_frag:1;
    /** Retry */
    t_u8 retry:1;
    /** Power Mgmt */
    t_u8 pwr_mgmt:1;
    /** More Data */
    t_u8 more_data:1;
    /** Wep */
    t_u8 wep:1;
    /** Order */
    t_u8 order:1;
} MLAN_PACK_END IEEEtypes_FrameCtl_t;
#endif

/** MrvlIETypes_MgmtFrameSet_t */
typedef MLAN_PACK_START struct _MrvlIETypes_MgmtFrameSet_t
{
    /** Type */
    t_u16 type;
    /** Length */
    t_u16 len;
    /** Frame Control */
    IEEEtypes_FrameCtl_t frame_control;
    /* t_u8 frame_contents[0]; */
} MLAN_PACK_END MrvlIETypes_MgmtFrameSet_t;

/** IEEEtypes_ReAssocRqst_t */
typedef MLAN_PACK_START struct _IEEEtypes_ReAssocRqst_t
{
    /** Capability Info */
    t_u16 cap_info;
    /** Listen Interval */
    t_u16 listen_interval;
    /* t_u8 ie_buffer[0]; */
} MLAN_PACK_END IEEEtypes_ReAssocRqst_t;
#endif /* UAP_SUPPORT */

#ifdef STA_SUPPORT
/** (Beaconsize(256)-5(IEId,len,contrystr(3))/3(FirstChan,NoOfChan,MaxPwr) */
#define MAX_NO_OF_CHAN          40

/** Channel-power table entries */
typedef MLAN_PACK_START struct _chan_power_11d
{
    /** 11D channel */
    t_u8 chan;
    /** 11D channel power */
    t_u8 pwr;
} MLAN_PACK_END chan_power_11d_t;

/** Region channel info */
typedef MLAN_PACK_START struct _parsed_region_chan_11d
{
    /** 11D band */
    t_u8 band;
    /** 11D region */
    t_u8 region;
    /** 11D country code */
    t_u8 country_code[COUNTRY_CODE_LEN];
    /** 11D channel power per channel */
    chan_power_11d_t chan_pwr[MAX_NO_OF_CHAN];
    /** 11D number of channels */
    t_u8 no_of_chan;
} MLAN_PACK_END parsed_region_chan_11d_t;
#endif /* STA_SUPPORT */

/** ChanScanMode_t */
typedef MLAN_PACK_START struct _ChanScanMode_t
{
#ifdef BIG_ENDIAN_SUPPORT
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
} MLAN_PACK_END ChanScanMode_t;

/** secondary channel is below */
#define SECOND_CHANNEL_BELOW    0x30
/** secondary channel is above */
#define SECOND_CHANNEL_ABOVE    0x10
/** ChanScanParamSet_t */
typedef MLAN_PACK_START struct _ChanScanParamSet_t
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
} MLAN_PACK_END ChanScanParamSet_t;

/** MrvlIEtypes_ChanListParamSet_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_ChanListParamSet_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** Channel scan parameters */
    ChanScanParamSet_t chan_scan_param[1];
} MLAN_PACK_END MrvlIEtypes_ChanListParamSet_t;

/** ChanScanParamSet_t */
typedef struct _ChanBandParamSet_t
{
    /** Channel scan parameter : Radio type */
    t_u8 radio_type;
    /** Channel number */
    t_u8 chan_number;
} ChanBandParamSet_t;

/** MrvlIEtypes_ChanBandListParamSet_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_ChanBandListParamSet_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** Channel Band parameters */
    ChanBandParamSet_t chan_band_param[1];
} MLAN_PACK_END MrvlIEtypes_ChanBandListParamSet_t;

/** MrvlIEtypes_RatesParamSet_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_RatesParamSet_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** Rates */
    t_u8 rates[1];
} MLAN_PACK_END MrvlIEtypes_RatesParamSet_t;

/** MrvlIEtypes_SsIdParamSet_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_SsIdParamSet_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** SSID */
    t_u8 ssid[1];
} MLAN_PACK_END MrvlIEtypes_SsIdParamSet_t;

/** MrvlIEtypes_NumProbes_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_NumProbes_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** Number of probes */
    t_u16 num_probes;
} MLAN_PACK_END MrvlIEtypes_NumProbes_t;

/** MrvlIEtypes_WildCardSsIdParamSet_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_WildCardSsIdParamSet_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** Maximum SSID length */
    t_u8 max_ssid_length;
    /** SSID */
    t_u8 ssid[1];
} MLAN_PACK_END MrvlIEtypes_WildCardSsIdParamSet_t;

/**TSF data size */
#define TSF_DATA_SIZE            8
/** Table of TSF values returned in the scan result */
typedef MLAN_PACK_START struct _MrvlIEtypes_TsfTimestamp_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** the length of each TSF data is 8 bytes, could be multiple TSF here */
    t_u8 tsf_data[1];
} MLAN_PACK_END MrvlIEtypes_TsfTimestamp_t;

/** CfParamSet_t */
typedef MLAN_PACK_START struct _CfParamSet_t
{
    /** CF parameter : Count */
    t_u8 cfp_cnt;
    /** CF parameter : Period */
    t_u8 cfp_period;
    /** CF parameter : Duration */
    t_u16 cfp_max_duration;
    /** CF parameter : Duration remaining */
    t_u16 cfp_duration_remaining;
} MLAN_PACK_END CfParamSet_t;

/** IbssParamSet_t */
typedef MLAN_PACK_START struct _IbssParamSet_t
{
    /** ATIM window value */
    t_u16 atim_window;
} MLAN_PACK_END IbssParamSet_t;

/** MrvlIEtypes_SsParamSet_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_SsParamSet_t
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
} MLAN_PACK_END MrvlIEtypes_SsParamSet_t;

/** FhParamSet_t */
typedef MLAN_PACK_START struct _FhParamSet_t
{
    /** FH parameter : Dwell time */
    t_u16 dwell_time;
    /** FH parameter : Hop set */
    t_u8 hop_set;
    /** FH parameter : Hop pattern */
    t_u8 hop_pattern;
    /** FH parameter : Hop index */
    t_u8 hop_index;
} MLAN_PACK_END FhParamSet_t;

/** DsParamSet_t */
typedef MLAN_PACK_START struct _DsParamSet_t
{
    /** Current channel number */
    t_u8 current_chan;
} MLAN_PACK_END DsParamSet_t;

/** MrvlIEtypes_PhyParamSet_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_PhyParamSet_t
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
} MLAN_PACK_END MrvlIEtypes_PhyParamSet_t;

/* Auth type to be used in the Authentication portion of an Assoc seq */
/** MrvlIEtypes_AuthType_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_AuthType_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** Authentication type */
    t_u16 auth_type;
} MLAN_PACK_END MrvlIEtypes_AuthType_t;

/** MrvlIETypes_VendorParamSet_t */
typedef MLAN_PACK_START struct _MrvlIETypes_VendorParamSet_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** Information element */
    t_u8 ie[MLAN_MAX_VSIE_LEN];
} MLAN_PACK_END MrvlIETypes_VendorParamSet_t;

/** MrvlIETypes_ActionFrame_t */
typedef MLAN_PACK_START struct
{
    MrvlIEtypesHeader_t header;   /**< Header */

    t_u8 srcAddr[MLAN_MAC_ADDR_LENGTH];
    t_u8 dstAddr[MLAN_MAC_ADDR_LENGTH];

    IEEEtypes_ActionFrame_t actionFrame;

} MLAN_PACK_END MrvlIETypes_ActionFrame_t;

/** MrvlIEtypes_RsnParamSet_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_RsnParamSet_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** RSN IE */
    t_u8 rsn_ie[1];
} MLAN_PACK_END MrvlIEtypes_RsnParamSet_t;

/** Key_param_set fixed length */
#define KEYPARAMSET_FIXED_LEN 6

/** MrvlIEtype_KeyParamSet_t */
typedef MLAN_PACK_START struct _MrvlIEtype_KeyParamSet_t
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
    t_u8 key[50];
} MLAN_PACK_END MrvlIEtype_KeyParamSet_t;

/** HostCmd_DS_802_11_KEY_MATERIAL */
typedef MLAN_PACK_START struct _HostCmd_DS_802_11_KEY_MATERIAL
{
    /** Action */
    t_u16 action;
    /** Key parameter set */
    MrvlIEtype_KeyParamSet_t key_param_set;
} MLAN_PACK_END HostCmd_DS_802_11_KEY_MATERIAL;

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

/* HostCmd_DS_802_11_SLEEP_PERIOD */
typedef MLAN_PACK_START struct _HostCmd_DS_802_11_SLEEP_PERIOD
{
    /** ACT_GET/ACT_SET */
    t_u16 action;

    /** Sleep Period in msec */
    t_u16 sleep_pd;
} MLAN_PACK_END HostCmd_DS_802_11_SLEEP_PERIOD;

/* HostCmd_DS_802_11_SLEEP_PARAMS */
typedef MLAN_PACK_START struct _HostCmd_DS_802_11_SLEEP_PARAMS
{
    /** ACT_GET/ACT_SET */
    t_u16 action;
    /** Sleep clock error in ppm */
    t_u16 error;
    /** Wakeup offset in usec */
    t_u16 offset;
    /** Clock stabilization time in usec */
    t_u16 stable_time;
    /** Control periodic calibration */
    t_u8 cal_control;
    /** Control the use of external sleep clock */
    t_u8 external_sleep_clk;
    /** Reserved field, should be set to zero */
    t_u16 reserved;
} MLAN_PACK_END HostCmd_DS_802_11_SLEEP_PARAMS;

/** Sleep response control */
typedef enum _sleep_resp_ctrl
{
    RESP_NOT_NEEDED = 0,
    RESP_NEEDED,
} sleep_resp_ctrl;

/** Structure definition for the new ieee power save parameters*/
typedef struct __ps_param
{
      /** Null packet interval */
    t_u16 null_pkt_interval;
      /** Num dtims */
    t_u16 multiple_dtims;
      /** becaon miss interval */
    t_u16 bcn_miss_timeout;
      /** local listen interval */
    t_u16 local_listen_interval;
     /** Adhoc awake period */
    t_u16 adhoc_wake_period;
     /** mode - (0x01 - firmware to automatically choose PS_POLL or NULL mode, 0x02 - PS_POLL, 0x03 - NULL mode ) */
    t_u16 mode;
     /** Delay to PS in milliseconds */
    t_u16 delay_to_ps;
} ps_param;

/** Structure definition for the new auto deep sleep command */
typedef struct __auto_ds_param
{
     /** Deep sleep inactivity timeout */
    t_u16 deep_sleep_timeout;
} auto_ds_param;

/** Structure definition for sleep confirmation in the new ps command */
typedef struct __sleep_confirm_param
{
     /** response control 0x00 - response not needed, 0x01 - response needed */
    t_u16 resp_ctrl;
} sleep_confirm_param;

/** bitmap for get auto deepsleep */
#define BITMAP_AUTO_DS         0x01
/** bitmap for sta power save */
#define BITMAP_STA_PS          0x10
/** bitmap for uap inactivity based PS */
#define BITMAP_UAP_INACT_PS    0x100
/** bitmap for uap DTIM PS */
#define BITMAP_UAP_DTIM_PS     0x200
/** Structure definition for the new ieee power save parameters*/
typedef struct _auto_ps_param
{
    /* bitmap for enable power save mode */
    t_u16 ps_bitmap;
    /* auto deep sleep parameter, sta power save parameter uap inactivity
       parameter uap DTIM parameter */
} auto_ps_param;

/** fix size for auto ps */
#define AUTO_PS_FIX_SIZE    4

/** TLV type : auto ds param */
#define TLV_TYPE_AUTO_DS_PARAM        (PROPRIETARY_TLV_BASE_ID + 113)
/** TLV type : ps param */
#define TLV_TYPE_PS_PARAM             (PROPRIETARY_TLV_BASE_ID + 114)

/** MrvlIEtypes_auto_ds_param_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_auto_ds_param_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** auto ds param */
    auto_ds_param param;
} MLAN_PACK_END MrvlIEtypes_auto_ds_param_t;

/** MrvlIEtypes_ps_param_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_ps_param_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** ps param */
    ps_param param;
} MLAN_PACK_END MrvlIEtypes_ps_param_t;

/** Structure definition for new power save command */
typedef MLAN_PACK_START struct _HostCmd_DS_PS_MODE_ENH
{
    /** Action */
    t_u16 action;
    /** Data speciifc to action */
    /* For IEEE power save data will be as UINT16 mode (0x01 - firmware to
       automatically choose PS_POLL or NULL mode, 0x02 - PS_POLL, 0x03 - NULL
       mode ) UINT16 NullpacketInterval UINT16 NumDtims UINT16
       BeaconMissInterval UINT16 locallisteninterval UINT16 adhocawakeperiod */

    /* For auto deep sleep */
    /* UINT16 Deep sleep inactivity timeout */

    /* For PS sleep confirm UINT16 responeCtrl - 0x00 - reponse from fw not
       needed, 0x01 - response from fw is needed */

    union
    {
    /** PS param definition */
        ps_param opt_ps;
    /** Auto ds param definition */
        auto_ds_param auto_ds;
    /** Sleep comfirm param definition */
        sleep_confirm_param sleep_cfm;
    /** bitmap for get PS info and Disable PS mode */
        t_u16 ps_bitmap;
    /** auto ps param */
        auto_ps_param auto_ps;
    } params;
} MLAN_PACK_END HostCmd_DS_802_11_PS_MODE_ENH;

/** HostCmd_DS_GET_HW_SPEC */
typedef MLAN_PACK_START struct _HostCmd_DS_GET_HW_SPEC
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
        /** Valid end port at init */
    t_u16 mp_end_port;
    /* Reserved */
    t_u16 reserved_4;
} MLAN_PACK_END HostCmd_DS_GET_HW_SPEC;

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
typedef MLAN_PACK_START struct _HostCmd_DS_MAC_MULTICAST_ADR
{
    /** Action */
    t_u16 action;
    /** Number of addresses */
    t_u16 num_of_adrs;
    /** List of MAC */
    t_u8 mac_list[MLAN_MAC_ADDR_LENGTH * MLAN_MAX_MULTICAST_LIST_SIZE];
} MLAN_PACK_END HostCmd_DS_MAC_MULTICAST_ADR;

/**  HostCmd_CMD_802_11_DEAUTHENTICATE */
typedef MLAN_PACK_START struct _HostCmd_DS_802_11_DEAUTHENTICATE
{
    /** MAC address */
    t_u8 mac_addr[MLAN_MAC_ADDR_LENGTH];
    /** Deauthentication resaon code */
    t_u16 reason_code;
} MLAN_PACK_END HostCmd_DS_802_11_DEAUTHENTICATE;

/** HostCmd_DS_802_11_ASSOCIATE */
typedef MLAN_PACK_START struct _HostCmd_DS_802_11_ASSOCIATE
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
} MLAN_PACK_END HostCmd_DS_802_11_ASSOCIATE;

/** HostCmd_CMD_802_11_ASSOCIATE response */
typedef MLAN_PACK_START struct _HostCmd_DS_802_11_ASSOCIATE_RSP
{
    /** Association response structure */
    IEEEtypes_AssocRsp_t assoc_rsp;
} MLAN_PACK_END HostCmd_DS_802_11_ASSOCIATE_RSP;

/** HostCmd_DS_802_11_AD_HOC_START*/
typedef MLAN_PACK_START struct _HostCmd_DS_802_11_AD_HOC_START
{
    /** AdHoc SSID */
    t_u8 ssid[MLAN_MAX_SSID_LENGTH];
    /** BSS mode */
    t_u8 bss_mode;
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
} MLAN_PACK_END HostCmd_DS_802_11_AD_HOC_START;

/**  HostCmd_CMD_802_11_AD_HOC_START response */
typedef MLAN_PACK_START struct _HostCmd_DS_802_11_AD_HOC_RESULT
{
    /** Padding */
    t_u8 pad[3];
    /** AdHoc BSSID */
    t_u8 bssid[MLAN_MAC_ADDR_LENGTH];
} MLAN_PACK_END HostCmd_DS_802_11_AD_HOC_RESULT;

/** AdHoc_BssDesc_t */
typedef MLAN_PACK_START struct _AdHoc_BssDesc_t
{
    /** BSSID */
    t_u8 bssid[MLAN_MAC_ADDR_LENGTH];
    /** SSID */
    t_u8 ssid[MLAN_MAX_SSID_LENGTH];
    /** BSS mode */
    t_u8 bss_mode;
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
} MLAN_PACK_END AdHoc_BssDesc_t;

/** HostCmd_DS_802_11_AD_HOC_JOIN */
typedef MLAN_PACK_START struct _HostCmd_DS_802_11_AD_HOC_JOIN
{
    /** AdHoc BSS descriptor */
    AdHoc_BssDesc_t bss_descriptor;
    /** Reserved field */
    t_u16 reserved1;
    /** Reserved field */
    t_u16 reserved2;
} MLAN_PACK_END HostCmd_DS_802_11_AD_HOC_JOIN;

typedef MLAN_PACK_START struct _HostCmd_DS_SDIO_PULL_CTRL
{
    /** Action */
    t_u16 action;
    /** The delay of pulling up in us */
    t_u16 pull_up;
    /** The delay of pulling down in us */
    t_u16 pull_down;
} MLAN_PACK_END HostCmd_DS_SDIO_PULL_CTRL;

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
typedef MLAN_PACK_START struct _HostCmd_TX_RATE_QUERY
{
    /** Tx rate */
    t_u8 tx_rate;
    /** Ht Info [Bit 0] RxRate format: LG=0, HT=1
     * [Bit 1]  HT Bandwidth: BW20 = 0, BW40 = 1
     * [Bit 2]  HT Guard Interval: LGI = 0, SGI = 1 */
    t_u8 ht_info;
} MLAN_PACK_END HostCmd_TX_RATE_QUERY;

/** HS Action 0x0001 - Configure enhanced host sleep mode, 0x0002 - Activate enhanced host sleep mode */
typedef enum _Host_Sleep_Action
{
    HS_CONFIGURE = 0x0001,
    HS_ACTIVATE = 0x0002,
} Host_Sleep_Action;

typedef MLAN_PACK_START struct _hs_config_param
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
} MLAN_PACK_END hs_config_param;

/** Structure definition for activating enhanced hs */
typedef MLAN_PACK_START struct __hs_activate_param
{
     /** response control 0x00 - response not needed, 0x01 - response needed */
    t_u16 resp_ctrl;
} MLAN_PACK_END hs_activate_param;

/** HostCmd_DS_802_11_HS_CFG_ENH */
typedef MLAN_PACK_START struct _HostCmd_DS_802_11_HS_CFG_ENH
{
    /** Action 0x0001 - Configure enhanced host sleep mode, 0x0002 - Activate enhanced host sleep mode */
    t_u16 action;

    union
    {
    /** Configure enhanced hs */
        hs_config_param hs_config;
    /** Activate enhanced hs */
        hs_activate_param hs_activate;
    } params;
} MLAN_PACK_END HostCmd_DS_802_11_HS_CFG_ENH;

/** HostCmd_DS_802_11_BCA_TIMESHARE */
typedef MLAN_PACK_START struct _HostCmd_DS_802_11_BCA_TIMESHARE
{
    /** Action */
    t_u16 action;
    /** Type: WLAN, BT */
    t_u16 traffic_type;
    /** 20msec - 60000msec */
    t_u32 timeshare_interval;
    /** PTA arbiter time in msec */
    t_u32 bt_time;
} MLAN_PACK_END HostCmd_DS_802_11_BCA_TIMESHARE;

/** SNMP_MIB_INDEX */
typedef enum _SNMP_MIB_INDEX
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
} SNMP_MIB_INDEX;

/** max SNMP buf size */
#define MAX_SNMP_BUF_SIZE   128

/**  HostCmd_CMD_802_11_SNMP_MIB */
typedef MLAN_PACK_START struct _HostCmd_DS_802_11_SNMP_MIB
{
    /** SNMP query type */
    t_u16 query_type;
    /** SNMP object ID */
    t_u16 oid;
    /** SNMP buffer size */
    t_u16 buf_size;
    /** Value */
    t_u8 value[1];
} MLAN_PACK_END HostCmd_DS_802_11_SNMP_MIB;

/** Radio on */
#define RADIO_ON                                0x01
/** Radio off */
#define RADIO_OFF                               0x00

/** HostCmd_CMD_802_11_RADIO_CONTROL */
typedef MLAN_PACK_START struct _HostCmd_DS_802_11_RADIO_CONTROL
{
    /** Action */
    t_u16 action;
    /** Control */
    t_u16 control;
} MLAN_PACK_END HostCmd_DS_802_11_RADIO_CONTROL;

/** MrvlRateScope_t */
typedef MLAN_PACK_START struct _MrvlRateScope_t
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
} MLAN_PACK_END MrvlRateScope_t;

/** MrvlRateDropControl_t */
typedef MLAN_PACK_START struct _MrvlRateDropControl_t
{
    /** Header Length */
    t_u16 length;
    /** Rate Information */
    t_u32 rate_info[1];
} MLAN_PACK_END MrvlRateDropControl_t;

/** MrvlRateDropPattern_t */
typedef MLAN_PACK_START struct _MrvlRateDropPattern_t
{
    /** Header Type */
    t_u16 type;
    /** Header Length */
    t_u16 length;
    /** Rate Drop Mode */
    t_u32 rate_drop_mode;
    /* MrvlRateDropControl_t RateDropControl[0]; */
} MLAN_PACK_END MrvlRateDropPattern_t;

/** HostCmd_DS_TX_RATE_CFG */
typedef MLAN_PACK_START struct _HostCmd_DS_TX_RATE_CFG
{
    /** Action */
    t_u16 action;
    /** Tx Rate Configuration Index */
    t_u16 cfg_index;
    /* MrvlRateScope_t RateScope; MrvlRateDropPattern_t RateDrop; */
} MLAN_PACK_END HostCmd_DS_TX_RATE_CFG;

/** Power_Group_t */
typedef MLAN_PACK_START struct _Power_Group_t
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
} MLAN_PACK_END Power_Group_t;

/** MrvlTypes_Power_Group_t */
typedef MLAN_PACK_START struct _MrvlTypes_Power_Group_t
{
    /** Header Type */
    t_u16 type;
    /** Header Length */
    t_u16 length;
    /* Power_Group_t PowerGroups */
} MLAN_PACK_END MrvlTypes_Power_Group_t;

/** HostCmd_CMD_TXPWR_CFG */
typedef MLAN_PACK_START struct _HostCmd_DS_TXPWR_CFG
{
    /** Action */
    t_u16 action;
    /** Power group configuration index */
    t_u16 cfg_index;
    /** Power group configuration mode */
    t_u32 mode;
    /* MrvlTypes_Power_Group_t PowerGrpCfg[0] */
} MLAN_PACK_END HostCmd_DS_TXPWR_CFG;

/** Connection type infra */
#define CONNECTION_TYPE_INFRA 	0
/** Connection type adhoc */
#define CONNECTION_TYPE_ADHOC	1
/** HostCmd_DS_SET_BSS_MODE */
typedef MLAN_PACK_START struct _HostCmd_DS_SET_BSS_MODE
{
    /** connection type */
    t_u8 con_type;
} MLAN_PACK_END HostCmd_DS_SET_BSS_MODE;

#ifdef STA_SUPPORT

/** Maximum number of channels that can be sent in user scan config */
#define WLAN_USER_SCAN_CHAN_MAX             50

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
typedef MLAN_PACK_START struct _wlan_scan_cmd_config
{
    /**
     *  BSS Type to be sent in the firmware command
     *
     *  Field can be used to restrict the types of networks returned in the
     *    scan.  Valid settings are:
     *
     *   - MLAN_SCAN_MODE_BSS  (infrastructure)
     *   - MLAN_SCAN_MODE_IBSS (adhoc)
     *   - MLAN_SCAN_MODE_ANY  (unrestricted, adhoc and infrastructure)
     */
    t_u8 bss_mode;

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
} MLAN_PACK_END wlan_scan_cmd_config;

/**
 *  @brief IOCTL channel sub-structure sent in wlan_ioctl_user_scan_cfg
 *
 *  Multiple instances of this structure are included in the IOCTL command
 *   to configure a instance of a scan on the specific channel.
 */
typedef MLAN_PACK_START struct _wlan_user_scan_chan
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
} MLAN_PACK_END wlan_user_scan_chan;

/**
 *  IOCTL SSID List sub-structure sent in wlan_ioctl_user_scan_cfg
 * 
 *  Used to specify SSID specific filters as well as SSID pattern matching
 *    filters for scan result processing in firmware.
 */
typedef MLAN_PACK_START struct _wlan_user_scan_ssid
{
    /** SSID */
    t_u8 ssid[MLAN_MAX_SSID_LENGTH + 1];
    /** Maximum length of SSID */
    t_u8 max_len;
} MLAN_PACK_END wlan_user_scan_ssid;

/**
 *  Input structure to configure an immediate scan cmd to firmware
 *
 *  Specifies a number of parameters to be used in general for the scan 
 *    as well as a channel list (wlan_user_scan_chan) for each scan period
 *    desired.
 */
typedef MLAN_PACK_START struct
{
    /**
     *  Flag set to keep the previous scan table intact
     *
     *  If set, the scan results will accumulate, replacing any previous
     *   matched entries for a BSS with the new scan data
     */
    t_u8 keep_previous_scan;
    /**
     *  BSS mode to be sent in the firmware command
     *
     *  Field can be used to restrict the types of networks returned in the
     *    scan.  Valid settings are:
     *
     *   - MLAN_SCAN_MODE_BSS  (infrastructure)
     *   - MLAN_SCAN_MODE_IBSS (adhoc)
     *   - MLAN_SCAN_MODE_ANY  (unrestricted, adhoc and infrastructure)        
     */
    t_u8 bss_mode;
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
} MLAN_PACK_END wlan_user_scan_cfg;

/**
 *  Sructure to retrieve the scan table
 */
typedef MLAN_PACK_START struct
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
} MLAN_PACK_END wlan_get_scan_table_info;

/** Generic structure defined for parsing WPA/RSN IEs for GTK/PTK OUIs */
typedef MLAN_PACK_START struct
{
        /** Group key oui */
    t_u8 GrpKeyOui[4];
        /** Number of PTKs */
    t_u8 PtkCnt[2];
        /** Ptk body starts here */
    t_u8 PtkBody[4];
} MLAN_PACK_END IEBody;
#endif /* STA_SUPPORT */

/* 
 * This scan handle Country Information IE(802.11d compliant) 
 * Define data structure for HostCmd_CMD_802_11_SCAN 
 */
/** HostCmd_DS_802_11_SCAN */
typedef MLAN_PACK_START struct _HostCmd_DS_802_11_SCAN
{
    /** BSS mode */
    t_u8 bss_mode;
    /** BSSID */
    t_u8 bssid[MLAN_MAC_ADDR_LENGTH];
    /** TLV buffer */
    t_u8 tlv_buffer[1];
    /** MrvlIEtypes_SsIdParamSet_t      SsIdParamSet; 
     *  MrvlIEtypes_ChanListParamSet_t  ChanListParamSet;
     *  MrvlIEtypes_RatesParamSet_t     OpRateSet; 
     */
} MLAN_PACK_END HostCmd_DS_802_11_SCAN;

/** HostCmd_DS_802_11_SCAN_RSP */
typedef MLAN_PACK_START struct _HostCmd_DS_802_11_SCAN_RSP
{
    /** Size of BSS descriptor */
    t_u16 bss_descript_size;
    /** Numner of sets */
    t_u8 number_of_sets;
    /** BSS descriptor and TLV buffer */
    t_u8 bss_desc_and_tlv_buffer[1];
} MLAN_PACK_END HostCmd_DS_802_11_SCAN_RSP;

/** HostCmd_DS_802_11_BG_SCAN_QUERY */
typedef MLAN_PACK_START struct _HostCmd_DS_802_11_BG_SCAN_QUERY
{
    /** Flush */
    t_u8 flush;
} MLAN_PACK_END HostCmd_DS_802_11_BG_SCAN_QUERY;

/** HostCmd_DS_802_11_BG_SCAN_QUERY_RSP */
typedef MLAN_PACK_START struct _HostCmd_DS_802_11_BG_SCAN_QUERY_RSP
{
    /** Report condition */
    t_u32 report_condition;
    /** Scan response */
    HostCmd_DS_802_11_SCAN_RSP scan_resp;
} MLAN_PACK_END HostCmd_DS_802_11_BG_SCAN_QUERY_RSP;

#ifdef STA_SUPPORT
/** MrvlIEtypes_DomainParamSet_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_DomainParamSet
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** Country code */
    t_u8 country_code[COUNTRY_CODE_LEN];
    /** Set of subbands */
    IEEEtypes_SubbandSet_t sub_band[1];
} MLAN_PACK_END MrvlIEtypes_DomainParamSet_t;

/** HostCmd_DS_802_11D_DOMAIN_INFO */
typedef MLAN_PACK_START struct _HostCmd_DS_802_11D_DOMAIN_INFO
{
    /** Action */
    t_u16 action;
    /** Domain parameter set */
    MrvlIEtypes_DomainParamSet_t domain;
} MLAN_PACK_END HostCmd_DS_802_11D_DOMAIN_INFO;

/** HostCmd_DS_802_11D_DOMAIN_INFO_RSP */
typedef MLAN_PACK_START struct _HostCmd_DS_802_11D_DOMAIN_INFO_RSP
{
    /** Action */
    t_u16 action;
    /** Domain parameter set */
    MrvlIEtypes_DomainParamSet_t domain;
} MLAN_PACK_END HostCmd_DS_802_11D_DOMAIN_INFO_RSP;
#endif /* STA_SUPPORT */

/** HostCmd_DS_11N_ADDBA_REQ */
typedef MLAN_PACK_START struct _HostCmd_DS_11N_ADDBA_REQ
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
} MLAN_PACK_END HostCmd_DS_11N_ADDBA_REQ;

/** HostCmd_DS_11N_ADDBA_RSP */
typedef MLAN_PACK_START struct _HostCmd_DS_11N_ADDBA_RSP
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
} MLAN_PACK_END HostCmd_DS_11N_ADDBA_RSP;

/** HostCmd_DS_11N_DELBA */
typedef MLAN_PACK_START struct _HostCmd_DS_11N_DELBA
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
} MLAN_PACK_END HostCmd_DS_11N_DELBA;

/** HostCmd_DS_11N_BATIMEOUT */
typedef MLAN_PACK_START struct _HostCmd_DS_11N_BATIMEOUT
{
    /** TID */
    t_u8 tid;
    /** Peer MAC address */
    t_u8 peer_mac_addr[MLAN_MAC_ADDR_LENGTH];
    /** Delete Block Ack Parameter Set */
    t_u8 origninator;
} MLAN_PACK_END HostCmd_DS_11N_BATIMEOUT;

/** HostCmd_DS_11N_CFG */
typedef MLAN_PACK_START struct _HostCmd_DS_11N_CFG
{
    /** Action */
    t_u16 action;
    /** HTTxCap */
    t_u16 ht_tx_cap;
    /** HTTxInfo */
    t_u16 ht_tx_info;
} MLAN_PACK_END HostCmd_DS_11N_CFG;

/** HostCmd_DS_TXBUF_CFG*/
typedef MLAN_PACK_START struct _HostCmd_DS_TXBUF_CFG
{
    /** Action */
    t_u16 action;
    /** Buffer Size */
    t_u16 buff_size;
    /** End Port_for Multiport */
    t_u16 mp_end_port;
    /** Reserved */
    t_u16 reserved3;
} MLAN_PACK_END HostCmd_DS_TXBUF_CFG;

/** HostCmd_DS_AMSDU_AGGR_CTRL */
typedef MLAN_PACK_START struct _HostCmd_DS_AMSDU_AGGR_CTRL
{
    /** Action */
    t_u16 action;
    /** Enable */
    t_u16 enable;
    /** Get the current Buffer Size valid */
    t_u16 curr_buf_size;
} MLAN_PACK_END HostCmd_DS_AMSDU_AGGR_CTRL;

/** HostCmd_DS_802_11_LDO_CFG */
typedef MLAN_PACK_START struct _HostCmd_DS_802_11_LDO_CFG
{
    /** Action */
    t_u16 action;
    /** PM Source: 0 = LDO_INTERNAL, 1 = LDO_EXTERNAL */
    t_u16 pmsource;
} MLAN_PACK_END HostCmd_DS_802_11_LDO_CFG;

/** HostCmd_DS_ECL_SYSTEM_CLOCK_CONFIG */
typedef MLAN_PACK_START struct _HostCmd_DS_ECL_SYSTEM_CLOCK_CONFIG
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
} MLAN_PACK_END HostCmd_DS_ECL_SYSTEM_CLOCK_CONFIG;

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

/** Size of a TSPEC.  Used to allocate necessary buffer space in commands */
#define WMM_TSPEC_SIZE              63

/** Maximum number of AC QOS queues available in the driver/firmware */
#define MAX_AC_QUEUES               4

/** Extra IE bytes allocated in messages for appended IEs after a TSPEC */
#define WMM_ADDTS_EXTRA_IE_BYTES    256

/** Extra TLV bytes allocated in messages for configuring WMM Queues */
#define WMM_QUEUE_CONFIG_EXTRA_TLV_BYTES 64

/** Number of bins in the histogram for the HostCmd_DS_WMM_QUEUE_STATS */
#define WMM_STATS_PKTS_HIST_BINS  7

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
typedef MLAN_PACK_START struct
{
    /** Queue status TLV */
    t_u8 queue_status_tlv[sizeof(MrvlIEtypes_WmmQueueStatus_t)
                          * MAX_AC_QUEUES];
    /** WMM parameter TLV */
    t_u8 wmm_param_tlv[sizeof(IEEEtypes_WmmParameter_t) + 2];
}
MLAN_PACK_END HostCmd_DS_WMM_GET_STATUS;

/**
 *  @brief Command structure for the HostCmd_CMD_WMM_ADDTS_REQ firmware command
 */
typedef MLAN_PACK_START struct
{
    mlan_cmd_result_e command_result;    /**< Command result */
    t_u32 timeout_ms;                    /**< Timeout value in milliseconds */
    t_u8 dialog_token;                   /**< Dialog token */
    t_u8 ieee_status_code;               /**< IEEE status code */
    t_u8 tspec_data[WMM_TSPEC_SIZE];     /**< TSPEC data */
    t_u8 addts_extra_ie_buf[WMM_ADDTS_EXTRA_IE_BYTES];  /**< Extra IE buffer */
} MLAN_PACK_END HostCmd_DS_WMM_ADDTS_REQ;

/**
 *  @brief Command structure for the HostCmd_CMD_WMM_DELTS_REQ firmware command
 */
typedef MLAN_PACK_START struct
{
    mlan_cmd_result_e command_result;   /**< Command result */
    t_u8 dialog_token;                  /**< Dialog token */
    t_u8 ieee_reason_code;              /**< IEEE reason code */
    t_u8 tspec_data[WMM_TSPEC_SIZE];    /**< TSPEC data */
} MLAN_PACK_END HostCmd_DS_WMM_DELTS_REQ;

/**
 *  @brief Command structure for the HostCmd_CMD_WMM_QUEUE_CONFIG firmware cmd
 *
 *  Set/Get/Default the Queue parameters for a specific AC in the firmware.
 */
typedef MLAN_PACK_START struct
{
    mlan_wmm_queue_config_action_e action; /**< Set, Get, or Default */
    mlan_wmm_ac_e access_category;         /**< WMM_AC_BK(0) to WMM_AC_VO(3) */
    /** @brief MSDU lifetime expiry per 802.11e
     *
     *   - Ignored if 0 on a set command 
     *   - Set to the 802.11e specified 500 TUs when defaulted
     */
    t_u16 msdu_lifetime_expiry;
    t_u8 tlv_buffer[WMM_QUEUE_CONFIG_EXTRA_TLV_BYTES];  /**< Not supported */
} MLAN_PACK_END HostCmd_DS_WMM_QUEUE_CONFIG;

/**
 *  @brief Command structure for the HostCmd_CMD_WMM_QUEUE_STATS firmware cmd
 *
 *  Turn statistical collection on/off for a given AC or retrieve the
 *    accumulated stats for an AC and clear them in the firmware.
 */
typedef MLAN_PACK_START struct
{
    mlan_wmm_queue_stats_action_e action;  /**< Start, Stop, or Get */
    mlan_wmm_ac_e access_category;    /**< WMM_AC_BK(0) to WMM_AC_VO(3) */
    t_u16 pkt_count;       /**< Number of successful packets transmitted */
    t_u16 pkt_loss;        /**< Packets lost; not included in pktCount */
    t_u32 avg_queue_delay; /**< Average Queue delay in microseconds */
    t_u32 avg_tx_delay;    /**< Average Transmission delay in microseconds */
    t_u16 used_time;    /**< Calculated used time - units of 32 microseconds */
    t_u16 policed_time; /**< Calculated policed time - units of 32 microseconds */
    /** @brief Queue Delay Histogram; number of packets per queue delay range
     *
     *  [0] -  0ms <= delay < 5ms
     *  [1] -  5ms <= delay < 10ms
     *  [2] - 10ms <= delay < 20ms
     *  [3] - 20ms <= delay < 30ms
     *  [4] - 30ms <= delay < 40ms
     *  [5] - 40ms <= delay < 50ms
     *  [6] - 50ms <= delay < msduLifetime (TUs)
     */
    t_u16 delay_histogram[WMM_STATS_PKTS_HIST_BINS];
    /** Reserved */
    t_u16 reserved_1;
} MLAN_PACK_END HostCmd_DS_WMM_QUEUE_STATS;

/**
 *  @brief Command structure for the HostCmd_CMD_WMM_TS_STATUS firmware cmd
 *
 *  Query the firmware to get the status of the WMM Traffic Streams
 */
typedef MLAN_PACK_START struct
{
    /** TSID: Range: 0->7 */
    t_u8 tid;
    /** TSID specified is valid */
    t_u8 valid;
    /** AC TSID is active on */
    t_u8 access_category;
    /** UP specified for the TSID */
    t_u8 user_priority;
    /** Power save mode for TSID: 0 (legacy), 1 (UAPSD) */
    t_u8 psb;
    /** Uplink(1), Downlink(2), Bidirectional(3) */
    t_u8 flow_dir;
    /** Medium time granted for the TSID */
    t_u16 medium_time;
} MLAN_PACK_END HostCmd_DS_WMM_TS_STATUS;

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

/**  Local Power Capability */
typedef MLAN_PACK_START struct _MrvlIEtypes_PowerCapability_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** Minmum power */
    t_s8 min_power;
    /** Maximum power */
    t_s8 max_power;
} MLAN_PACK_END MrvlIEtypes_PowerCapability_t;

/** HT Capabilities element */
typedef MLAN_PACK_START struct _MrvlIETypes_HTCap_t
{
    /** Header */
    MrvlIEtypesHeader_t header;

    /** HTCap struct */
    HTCap_t ht_cap;
} MLAN_PACK_END MrvlIETypes_HTCap_t;

/** HT Information element */
typedef MLAN_PACK_START struct _MrvlIETypes_HTInfo_t
{
    /** Header */
    MrvlIEtypesHeader_t header;

    /** HTInfo struct */
    HTInfo_t ht_info;
} MLAN_PACK_END MrvlIETypes_HTInfo_t;

/** 20/40 BSS Coexistence element */
typedef MLAN_PACK_START struct _MrvlIETypes_2040BSSCo_t
{
    /** Header */
    MrvlIEtypesHeader_t header;

    /** BSSCo2040_t struct */
    BSSCo2040_t bss_co_2040;
} MLAN_PACK_END MrvlIETypes_2040BSSCo_t;

/** Extended Capabilities element */
typedef MLAN_PACK_START struct _MrvlIETypes_ExtCap_t
{
    /** Header */
    MrvlIEtypesHeader_t header;

    /** ExtCap_t struct */
    ExtCap_t ext_cap;
} MLAN_PACK_END MrvlIETypes_ExtCap_t;

/** Overlapping BSS Scan Parameters element */
typedef MLAN_PACK_START struct _MrvlIETypes_OverlapBSSScanParam_t
{
    /** Header */
    MrvlIEtypesHeader_t header;

    /** OBSSScanParam_t struct */
    OBSSScanParam_t obss_scan_param;
} MLAN_PACK_END MrvlIETypes_OverlapBSSScanParam_t;

/** Set of MCS values that STA desires to use within the BSS */
typedef MLAN_PACK_START struct _MrvlIETypes_HTOperationalMCSSet_t
{
    /** Header */
    MrvlIEtypesHeader_t header;

    /** Bitmap indicating MCSs that STA desires to use within the BSS */
    t_u8 ht_operational_mcs_bitmap[16];
} MLAN_PACK_END MrvlIETypes_HTOperationalMCSSet_t;

/** MrvlIEtypes_PMK_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_PMK_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** PMK */
    t_u8 pmk[1];
} MLAN_PACK_END MrvlIEtypes_PMK_t;

/** MrvlIEtypes_Passphrase_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_Passphrase_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** Passphrase */
    char passphrase[1];
} MLAN_PACK_END MrvlIEtypes_Passphrase_t;

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
typedef MLAN_PACK_START struct _MrvlIEtypes_Cipher_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** PairCipher */
    t_u8 pair_cipher;
    /** GroupCipher */
    t_u8 group_cipher;
} MLAN_PACK_END MrvlIEtypes_Cipher_t;

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
typedef MLAN_PACK_START struct _MrvlIEtypes_EncrProto_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** EncrProto */
    t_u16 rsn_mode;
} MLAN_PACK_END MrvlIEtypes_EncrProto_t;

/** MrvlIEtypes_Bssid_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_Bssid_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** Bssid */
    t_u8 bssid[MLAN_MAC_ADDR_LENGTH];
} MLAN_PACK_END MrvlIEtypes_Bssid_t;

/*
 * This struct will handle GET,SET,CLEAR function for embedded
 * supplicant.
 * Define data structure for HostCmd_CMD_802_11_SUPPLICANT_PMK
 */
/** HostCmd_DS_802_11_SUPPLICANT_PMK */
typedef MLAN_PACK_START struct _HostCmd_DS_802_11_SUPPLICANT_PMK
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
} MLAN_PACK_END HostCmd_DS_802_11_SUPPLICANT_PMK;

/*
 * This struct will GET the Supplicant supported bitmaps
 * The GET_CURRENT action will get the network profile used
 * for the current assocation.
 * Define data structure for HostCmd_CMD_802_11_SUPPLICANT_PROFILE
 */
typedef MLAN_PACK_START struct _HostCmd_DS_802_11_SUPPLICANT_PROFILE
{
    /** GET/SET/GET_CURRENT */
    t_u16 action;
    /** Reserved */
    t_u16 reserved;
    /** TLVBuffer */
    t_u8 tlv_buf[1];
    /* MrvlIEtypes_EncrProto_t */
} MLAN_PACK_END HostCmd_DS_802_11_SUPPLICANT_PROFILE;

/** HostCmd_CMD_802_11_RF_CHANNEL */
typedef MLAN_PACK_START struct _HostCmd_DS_802_11_RF_CHANNEL
{
    /** Action */
    t_u16 action;
    /** Current channel */
    t_u16 current_channel;
    /** RF type */
    t_u16 rf_type;
    /** Reserved field */
    t_u16 reserved;
#ifdef STA_SUPPORT
    /** Reserved */
    t_u8 reserved_1[32];
#else                           /* STA_SUPPORT */
    /** List of channels */
    t_u8 channel_list[32];
#endif                          /* !STA_SUPPORT */
} MLAN_PACK_END HostCmd_DS_802_11_RF_CHANNEL;

/** HostCmd_DS_VERSION_EXT */
typedef MLAN_PACK_START struct _HostCmd_DS_VERSION_EXT
{
    /** Selected version string */
    t_u8 version_str_sel;
    /** Version string */
    char version_str[128];
} MLAN_PACK_END HostCmd_DS_VERSION_EXT;

/** HostCmd_CMD_802_11_RF_ANTENNA */
typedef MLAN_PACK_START struct _HostCmd_DS_802_11_RF_ANTENNA
{
    /** Action */
    t_u16 action;
    /**  Number of antennas or 0xffff(diversity) */
    t_u16 antenna_mode;
} MLAN_PACK_END HostCmd_DS_802_11_RF_ANTENNA;

/** HostCmd_DS_802_11_IBSS_STATUS */
typedef MLAN_PACK_START struct _HostCmd_DS_802_11_IBSS_STATUS
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
} MLAN_PACK_END HostCmd_DS_802_11_IBSS_STATUS;

/** HostCmd_DS_MGMT_IE_LIST_CFG */
typedef MLAN_PACK_START struct _HostCmd_DS_MGMT_IE_LIST
{
    /** Action */
    t_u16 action;
    /** Get/Set mgmt IE */
    mlan_ds_misc_custom_ie ds_mgmt_ie;
} MLAN_PACK_END HostCmd_DS_MGMT_IE_LIST_CFG;

/** HostCmd_CMD_MAC_REG_ACCESS */
typedef MLAN_PACK_START struct _HostCmd_DS_MAC_REG_ACCESS
{
    /** Action */
    t_u16 action;
    /** MAC register offset */
    t_u16 offset;
    /** MAC register value */
    t_u32 value;
} MLAN_PACK_END HostCmd_DS_MAC_REG_ACCESS;

/** HostCmd_CMD_BBP_REG_ACCESS */
typedef MLAN_PACK_START struct _HostCmd_DS_BBP_REG_ACCESS
{
    /** Acion */
    t_u16 action;
    /** BBP register offset */
    t_u16 offset;
    /** BBP register value */
    t_u8 value;
    /** Reserved field */
    t_u8 reserved[3];
} MLAN_PACK_END HostCmd_DS_BBP_REG_ACCESS;

/**  HostCmd_CMD_RF_REG_ACCESS */
typedef MLAN_PACK_START struct _HostCmd_DS_RF_REG_ACCESS
{
    /** Action */
    t_u16 action;
    /** RF register offset */
    t_u16 offset;
    /** RF register value */
    t_u8 value;
    /** Reserved field */
    t_u8 reserved[3];
} MLAN_PACK_END HostCmd_DS_RF_REG_ACCESS;

/**  HostCmd_CMD_PMIC_REG_ACCESS */
typedef MLAN_PACK_START struct _HostCmd_DS_PMIC_REG_ACCESS
{
    /** Action */
    t_u16 action;
    /** PMIC register offset */
    t_u16 offset;
    /** PMIC register value */
    t_u8 value;
    /** Reserved field */
    t_u8 reserved[3];
} MLAN_PACK_END HostCmd_DS_PMIC_REG_ACCESS;

/** HostCmd_DS_802_11_EEPROM_ACCESS */
typedef MLAN_PACK_START struct _HostCmd_DS_802_11_EEPROM_ACCESS
{
        /** Action */
    t_u16 action;

        /** multiple 4 */
    t_u16 offset;
        /** Number of bytes */
    t_u16 byte_count;
        /** Value */
    t_u8 value;
} MLAN_PACK_END HostCmd_DS_802_11_EEPROM_ACCESS;

/** HostCmd_DS_MEM_ACCESS */
typedef MLAN_PACK_START struct _HostCmd_DS_MEM_ACCESS
{
        /** Action */
    t_u16 action;
        /** Reserved field */
    t_u16 reserved;
        /** Address */
    t_u32 addr;
        /** Value */
    t_u32 value;
} MLAN_PACK_END HostCmd_DS_MEM_ACCESS;

/** HostCmd_DS_INACTIVITY_TIMEOUT_EXT */
typedef MLAN_PACK_START struct _HostCmd_DS_INACTIVITY_TIMEOUT_EXT
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
} MLAN_PACK_END HostCmd_DS_INACTIVITY_TIMEOUT_EXT;

/** TLV type : STA Mac address */
#define TLV_TYPE_STA_MAC_ADDRESS      (PROPRIETARY_TLV_BASE_ID + 32)

/** MrvlIEtypes_MacAddr_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_MacAddr_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** mac address */
    t_u8 mac[MLAN_MAC_ADDR_LENGTH];
} MLAN_PACK_END MrvlIEtypes_MacAddr_t;

#ifdef UAP_SUPPORT
/** TLV type : AP Channel band Config */
#define TLV_TYPE_UAP_CHAN_BAND_CONFIG (PROPRIETARY_TLV_BASE_ID + 42)
/** TLV type : AP Mac address */
#define TLV_TYPE_UAP_MAC_ADDRESS      (PROPRIETARY_TLV_BASE_ID + 43)
/** TLV type : AP Beacon period */
#define TLV_TYPE_UAP_BEACON_PERIOD    (PROPRIETARY_TLV_BASE_ID + 44)
/** TLV type : AP DTIM period */
#define TLV_TYPE_UAP_DTIM_PERIOD      (PROPRIETARY_TLV_BASE_ID + 45)
/** TLV type : AP Tx power */
#define TLV_TYPE_UAP_TX_POWER         (PROPRIETARY_TLV_BASE_ID + 47)
/** TLV type : AP SSID broadcast control */
#define TLV_TYPE_UAP_BCAST_SSID_CTL   (PROPRIETARY_TLV_BASE_ID + 48)
/** TLV type : AP Preamble control */
#define TLV_TYPE_UAP_PREAMBLE_CTL     (PROPRIETARY_TLV_BASE_ID + 49)
/** TLV type : AP Antenna control */
#define TLV_TYPE_UAP_ANTENNA_CTL      (PROPRIETARY_TLV_BASE_ID + 50)
/** TLV type : AP RTS threshold */
#define TLV_TYPE_UAP_RTS_THRESHOLD    (PROPRIETARY_TLV_BASE_ID + 51)
/** TLV type : Radio Control */
#define TLV_TYPE_UAP_RADIO_CONTROL    (PROPRIETARY_TLV_BASE_ID + 52)
/** TLV type : AP Tx data rate */
#define TLV_TYPE_UAP_TX_DATA_RATE     (PROPRIETARY_TLV_BASE_ID + 53)
/** TLV type: AP Packet forwarding control */
#define TLV_TYPE_UAP_PKT_FWD_CTL      (PROPRIETARY_TLV_BASE_ID + 54)
/** TLV type: STA information */
#define TLV_TYPE_UAP_STA_INFO         (PROPRIETARY_TLV_BASE_ID + 55)
/** TLV type: AP STA MAC address filter */
#define TLV_TYPE_UAP_STA_MAC_ADDR_FILTER (PROPRIETARY_TLV_BASE_ID + 56)
/** TLV type: AP STA ageout timer */
#define TLV_TYPE_UAP_STA_AGEOUT_TIMER (PROPRIETARY_TLV_BASE_ID + 57)
/** TLV type: AP WEP keys */
#define TLV_TYPE_UAP_WEP_KEY          (PROPRIETARY_TLV_BASE_ID + 59)
/** TLV type: AP WPA passphrase */
#define TLV_TYPE_UAP_WPA_PASSPHRASE    (PROPRIETARY_TLV_BASE_ID + 60)
/** TLV type: AP protocol */
#define TLV_TYPE_UAP_ENCRYPT_PROTOCOL (PROPRIETARY_TLV_BASE_ID + 64)
/** TLV type: AP AKMP */
#define TLV_TYPE_UAP_AKMP             (PROPRIETARY_TLV_BASE_ID + 65)
/** TLV type: AP Cipher type*/
#define TLV_TYPE_UAP_CIPHER	        (PROPRIETARY_TLV_BASE_ID + 66)
/** TLV type: AP Fragment threshold */
#define TLV_TYPE_UAP_FRAG_THRESHOLD   (PROPRIETARY_TLV_BASE_ID + 70)
/** TLV type: AP Group rekey timer */
#define TLV_TYPE_UAP_GRP_REKEY_TIME   (PROPRIETARY_TLV_BASE_ID + 71)
/**TLV type : AP Max Station number */
#define TLV_TYPE_UAP_MAX_STA_CNT      (PROPRIETARY_TLV_BASE_ID + 85)
/**TLV type : AP Retry limit */
#define TLV_TYPE_UAP_RETRY_LIMIT      (PROPRIETARY_TLV_BASE_ID + 93)
/** TLV type : AP MCBC data rate */
#define TLV_TYPE_UAP_MCBC_DATA_RATE   (PROPRIETARY_TLV_BASE_ID + 98)
/**TLV type: AP RSN replay protection */
#define TLV_TYPE_UAP_RSN_REPLAY_PROTECT  (PROPRIETARY_TLV_BASE_ID + 100)
/** TLV ID : Management Frame */
#define TLV_TYPE_UAP_MGMT_FRAME          (PROPRIETARY_TLV_BASE_ID + 104)
/**TLV type: AP pairwise handshake timeout */
#define TLV_TYPE_UAP_EAPOL_PWK_HSK_TIMEOUT  (PROPRIETARY_TLV_BASE_ID + 117)
/**TLV type: AP pairwise handshake retries */
#define TLV_TYPE_UAP_EAPOL_PWK_HSK_RETRIES  (PROPRIETARY_TLV_BASE_ID + 118)
/**TLV type: AP groupwise handshake timeout */
#define TLV_TYPE_UAP_EAPOL_GWK_HSK_TIMEOUT  (PROPRIETARY_TLV_BASE_ID + 119)
/**TLV type: AP groupwise handshake retries */
#define TLV_TYPE_UAP_EAPOL_GWK_HSK_RETRIES  (PROPRIETARY_TLV_BASE_ID + 120)
/** TLV type: AP PS STA ageout timer */
#define TLV_TYPE_UAP_PS_STA_AGEOUT_TIMER (PROPRIETARY_TLV_BASE_ID + 123)

/** Assoc Request */
#define SUBTYPE_ASSOC_REQUEST        0

/** MrvlIEtypes_beacon_period_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_beacon_period_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** beacon period */
    t_u16 beacon_period;
} MLAN_PACK_END MrvlIEtypes_beacon_period_t;

/** MrvlIEtypes_dtim_period_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_dtim_period_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** DTIM period */
    t_u8 dtim_period;
} MLAN_PACK_END MrvlIEtypes_dtim_period_t;

/** MrvlIEtypes_tx_rate_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_tx_rate_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** tx data rate */
    t_u16 tx_data_rate;
} MLAN_PACK_END MrvlIEtypes_tx_rate_t;

/** MrvlIEtypes_mcbc_rate_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_mcbc_rate_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** mcbc data rate */
    t_u16 mcbc_data_rate;
} MLAN_PACK_END MrvlIEtypes_mcbc_rate_t;

/** MrvlIEtypes_tx_power_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_tx_power_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** tx power */
    t_u8 tx_power;
} MLAN_PACK_END MrvlIEtypes_tx_power_t;

/** MrvlIEtypes_bcast_ssid_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_bcast_ssid_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** bcast ssid control*/
    t_u8 bcast_ssid_ctl;
} MLAN_PACK_END MrvlIEtypes_bcast_ssid_t;

/** MrvlIEtypes_antenna_mode_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_antenna_mode_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** which antenna */
    t_u8 which_antenna;
    /** antenna mode*/
    t_u8 antenna_mode;
} MLAN_PACK_END MrvlIEtypes_antenna_mode_t;

/** MrvlIEtypes_pkt_forward_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_pkt_forward_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** pkt foward control */
    t_u8 pkt_forward_ctl;
} MLAN_PACK_END MrvlIEtypes_pkt_forward_t;

/** MrvlIEtypes_max_sta_count_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_max_sta_count_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** max station count */
    t_u16 max_sta_count;
} MLAN_PACK_END MrvlIEtypes_max_sta_count_t;

/** MrvlIEtypes_sta_ageout_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_sta_ageout_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** station age out timer */
    t_u32 sta_ageout_timer;
} MLAN_PACK_END MrvlIEtypes_sta_ageout_t;

/** MrvlIEtypes_rts_threshold_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_rts_threshold_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** rts threshold */
    t_u16 rts_threshold;
} MLAN_PACK_END MrvlIEtypes_rts_threshold_t;

/** MrvlIEtypes_frag_threshold_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_frag_threshold_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** frag threshold */
    t_u16 frag_threshold;
} MLAN_PACK_END MrvlIEtypes_frag_threshold_t;

/** MrvlIEtypes_retry_limit_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_retry_limit_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** retry limit */
    t_u8 retry_limit;
} MLAN_PACK_END MrvlIEtypes_retry_limit_t;

/** MrvlIEtypes_eapol_pwk_hsk_timeout_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_eapol_pwk_hsk_timeout_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** pairwise update timeout */
    t_u32 pairwise_update_timeout;
} MLAN_PACK_END MrvlIEtypes_eapol_pwk_hsk_timeout_t;

/** MrvlIEtypes_eapol_pwk_hsk_retries_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_eapol_pwk_hsk_retries_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** pairwise handshake retries */
    t_u32 pwk_retries;
} MLAN_PACK_END MrvlIEtypes_eapol_pwk_hsk_retries_t;

/** MrvlIEtypes_eapol_gwk_hsk_timeout_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_eapol_gwk_hsk_timeout_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** groupwise update timeout */
    t_u32 groupwise_update_timeout;
} MLAN_PACK_END MrvlIEtypes_eapol_gwk_hsk_timeout_t;

/** MrvlIEtypes_eapol_gwk_hsk_retries_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_eapol_gwk_hsk_retries_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** groupwise handshake retries */
    t_u32 gwk_retries;
} MLAN_PACK_END MrvlIEtypes_eapol_gwk_hsk_retries_t;

/** MrvlIEtypes_mac_filter_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_mac_filter_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** Filter mode */
    t_u8 filter_mode;
    /** Number of STA MACs */
    t_u8 count;
    /** STA MAC addresses buffer */
    t_u8 mac_address[1];
} MLAN_PACK_END MrvlIEtypes_mac_filter_t;

/** MrvlIEtypes_retry_limit_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_channel_band_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** Band Configuration
     *
     * [7-6] Channel Selection Mode; 00 manual, 01 ACS
     * [3-2] Channel Width; 00 20 MHz
     * [1-0] Band Info; 00 2.4 GHz
     */
    t_u8 band_config;
    /** channel */
    t_u8 channel;
} MLAN_PACK_END MrvlIEtypes_channel_band_t;

/** MrvlIEtypes_radio_control_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_radio_control_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** Radio control: on/off */
    t_u8 radio_ctl;
} MLAN_PACK_END MrvlIEtypes_radio_control_t;

/** MrvlIEtypes_auth_type_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_auth_type_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** Authentication type */
    t_u8 auth_type;
} MLAN_PACK_END MrvlIEtypes_auth_type_t;

/** MrvlIEtypes_encrypt_protocol_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_encrypt_protocol_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** encryption protocol */
    t_u16 protocol;
} MLAN_PACK_END MrvlIEtypes_encrypt_protocol_t;

/** MrvlIEtypes_cipher_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_cipher_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** pairwise cipher */
    t_u8 pairwise_cipher;
    /** group cipher */
    t_u8 group_cipher;
} MLAN_PACK_END MrvlIEtypes_cipher_t;

/** Key_mgmt_psk */
#define KEY_MGMT_NONE   0x04
/** Key_mgmt_none */
#define KEY_MGMT_PSK    0x02

/** MrvlIEtypes_akmp_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_akmp_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** key management */
    t_u16 key_mgmt;
} MLAN_PACK_END MrvlIEtypes_akmp_t;

/** MrvlIEtypes_passphrase_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_passphrase_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** passphrase */
    t_u8 passphrase[1];
} MLAN_PACK_END MrvlIEtypes_passphrase_t;

/** MrvlIEtypes_rsn_replay_prot_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_rsn_replay_prot_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** rsn replay proection */
    t_u8 rsn_replay_prot;
} MLAN_PACK_END MrvlIEtypes_rsn_replay_prot_t;

/** MrvlIEtypes_group_rekey_time_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_group_rekey_time_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** group key rekey time */
    t_u32 gk_rekey_time;
} MLAN_PACK_END MrvlIEtypes_group_rekey_time_t;

/** MrvlIEtypes_wep_key_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_wep_key_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** key index */
    t_u8 key_index;
    /** is default */
    t_u8 is_default;
    /** key data */
    t_u8 key[1];
} MLAN_PACK_END MrvlIEtypes_wep_key_t;

/** MrvlIEtypes_preamble_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_preamble_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** preamble type, READ only */
    t_u8 preamble_type;
} MLAN_PACK_END MrvlIEtypes_preamble_t;

/** SNMP_MIB_UAP_INDEX */
typedef enum _SNMP_MIB_UAP_INDEX
{
    tkip_mic_failures = 0x0b,
    ccmp_decrypt_errors = 0x0c,
    wep_undecryptable_count = 0x0d,
    wep_icv_error_count = 0x0e,
    decrypt_failure_count = 0xf,
    dot11_failed_count = 0x12,
    dot11_retry_count = 0x13,
    dot11_multi_retry_count = 0x14,
    dot11_frame_dup_count = 0x15,
    dot11_rts_success_count = 0x16,
    dot11_rts_failure_count = 0x17,
    dot11_ack_failure_count = 0x18,
    dot11_rx_fragment_count = 0x19,
    dot11_mcast_rx_frame_count = 0x1a,
    dot11_fcs_error_count = 0x1b,
    dot11_tx_frame_count = 0x1c,
    dot11_rsna_tkip_cm_invoked = 0x1d,
    dot11_rsna_4way_hshk_failures = 0x1e,
    dot11_mcast_tx_count = 0x1f,
} SNMP_MIB_UAP_INDEX;

/** MrvlIEtypes_snmp_oid_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_snmp_oid_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** data */
    t_u32 data;
} MLAN_PACK_END MrvlIEtypes_snmp_oid_t;

/** HostCmd_SYS_CONFIG */
typedef MLAN_PACK_START struct _HostCmd_DS_SYS_CONFIG
{
        /** CMD Action GET/SET*/
    t_u16 action;
        /** Tlv buffer */
    t_u8 tlv_buffer[1];
} MLAN_PACK_END HostCmd_DS_SYS_CONFIG;

/** HostCmd_SYS_CONFIG */
typedef MLAN_PACK_START struct _HostCmd_DS_SYS_INFO
{
    /** sys info */
    t_u8 sys_info[64];
} MLAN_PACK_END HostCmd_DS_SYS_INFO;

/** HostCmd_DS_STA_DEAUTH */
typedef MLAN_PACK_START struct _HostCmd_DS_STA_DEAUTH
{
    /** mac address */
    t_u8 mac[MLAN_MAC_ADDR_LENGTH];
    /** reason code */
    t_u16 reason;
} MLAN_PACK_END HostCmd_DS_STA_DEAUTH;

/** Host Command id: POWER_MGMT  */
#define HOST_CMD_POWER_MGMT_EXT                   0x00ef
/** TLV type: AP Sleep param */
#define TLV_TYPE_AP_SLEEP_PARAM         (PROPRIETARY_TLV_BASE_ID + 106)
/** TLV type: AP Inactivity Sleep param */
#define TLV_TYPE_AP_INACT_SLEEP_PARAM   (PROPRIETARY_TLV_BASE_ID + 107)

/** Host Command id: SLEEP_CONFIRM*/
#define HOST_CMD_SLEEP_CONFIRM       	      0x00d8

/** MrvlIEtypes_sleep_param_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_sleep_param_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** control bitmap */
    t_u32 ctrl_bitmap;
    /** min_sleep */
    t_u32 min_sleep;
    /** max_sleep */
    t_u32 max_sleep;
} MLAN_PACK_END MrvlIEtypes_sleep_param_t;

/** MrvlIEtypes_inact_sleep_param_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_inact_sleep_param_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** inactivity timeout */
    t_u32 inactivity_to;

    /** min_awake */
    t_u32 min_awake;
    /** max_awake */
    t_u32 max_awake;
} MLAN_PACK_END MrvlIEtypes_inact_sleep_param_t;

/** HostCmd_DS_POWER_MGMT */
typedef MLAN_PACK_START struct _HostCmd_DS_POWER_MGMT_EXT
{
    /** CMD Action Get/Set*/
    t_u16 action;
    /** power mode */
    t_u16 power_mode;
} MLAN_PACK_END HostCmd_DS_POWER_MGMT_EXT;

/** MrvlIEtypes_ps_sta_ageout_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_ps_sta_ageout_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** station age out timer */
    t_u32 ps_sta_ageout_timer;
} MLAN_PACK_END MrvlIEtypes_ps_sta_ageout_t;

/** MrvlIEtypes_sta_info_t */
typedef MLAN_PACK_START struct _MrvlIEtypes_sta_info_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** STA MAC address */
    t_u8 mac_address[MLAN_MAC_ADDR_LENGTH];
    /** Power mfg status */
    t_u8 power_mfg_status;
    /** RSSI */
    t_s8 rssi;
} MLAN_PACK_END MrvlIEtypes_sta_info_t;

/** HostCmd_DS_STA_LIST */
typedef MLAN_PACK_START struct _HostCmd_DS_STA_LIST
{
    /** Number of STAs */
    t_u16 sta_count;
    /* MrvlIEtypes_sta_info_t sta_info[0]; */
} MLAN_PACK_END HostCmd_DS_STA_LIST;

#endif /* UAP_SUPPORT */

/**
 * @brief 802.11h Local Power Constraint Marvell extended TLV
 */
typedef MLAN_PACK_START struct
{
    MrvlIEtypesHeader_t header; /**< Marvell TLV header: ID/Len */
    t_u8 chan;                  /**< Channel local constraint applies to */

    /** Power constraint included in beacons and used by fw to offset 11d info */
    t_u8 constraint;

} MLAN_PACK_END MrvlIEtypes_LocalPowerConstraint_t;

/*
 *
 * Data structures for driver/firmware command processing
 *
 */

/**  TPC Info structure sent in CMD_802_11_TPC_INFO command to firmware */
typedef MLAN_PACK_START struct
{
    MrvlIEtypes_LocalPowerConstraint_t local_constraint;  /**< Local constraint */
    MrvlIEtypes_PowerCapability_t power_cap;              /**< Power Capability */

} MLAN_PACK_END HostCmd_DS_802_11_TPC_INFO;

/**  TPC Request structure sent in CMD_802_11_TPC_ADAPT_REQ command to firmware */
typedef MLAN_PACK_START struct
{
    t_u8 dest_mac[MLAN_MAC_ADDR_LENGTH];        /**< Destination STA address  */
    t_u16 timeout;                              /**< Response timeout in ms */
    t_u8 rate_index;                            /**< IEEE Rate index to send request */

} MLAN_PACK_END HostCmd_TpcRequest;

/**  TPC Response structure received from the CMD_802_11_TPC_ADAPT_REQ command */
typedef MLAN_PACK_START struct
{
    t_u8 tpc_ret_code;      /**< Firmware command result status code */
    t_s8 tx_power;          /**< Reported TX Power from the TPC Report element */
    t_s8 link_margin;       /**< Reported link margin from the TPC Report element */
    t_s8 rssi;              /**< RSSI of the received TPC Report frame */

} MLAN_PACK_END HostCmd_TpcResponse;

/**  CMD_802_11_TPC_ADAPT_REQ substruct. Union of the TPC request and response */
typedef MLAN_PACK_START union
{
    HostCmd_TpcRequest req;   /**< Request struct sent to firmware */
    HostCmd_TpcResponse resp; /**< Response struct received from firmware */

} MLAN_PACK_END HostCmd_DS_802_11_TPC_ADAPT_REQ;

/**  CMD_802_11_CHAN_SW_ANN firmware command substructure */
typedef MLAN_PACK_START struct
{
    t_u8 switch_mode;   /**< Set to 1 for a quiet switch request, no STA tx */
    t_u8 new_chan;      /**< Requested new channel */
    t_u8 switch_count;  /**< Number of TBTTs until the switch is to occur */
} MLAN_PACK_END HostCmd_DS_802_11_CHAN_SW_ANN;

/**        
 * @brief Enumeration of measurement types, including max supported 
 *        enum for 11h/11k
 */
typedef MLAN_PACK_START enum
{
    WLAN_MEAS_BASIC = 0,              /**< 11h: Basic */
    WLAN_MEAS_NUM_TYPES,              /**< Number of enumerated measurements */
    WLAN_MEAS_11H_MAX_TYPE = WLAN_MEAS_BASIC,          /**< Max 11h measurement */

} MLAN_PACK_END MeasType_t;

/**        
 * @brief Mode octet of the measurement request element (7.3.2.21)
 */
typedef MLAN_PACK_START struct
{
#ifdef BIG_ENDIAN_SUPPORT
    t_u8 rsvd5_7:3;               /**< Reserved */
    t_u8 duration_mandatory:1;    /**< 11k: duration spec. for meas. is mandatory */
    t_u8 report:1;                /**< 11h: en/disable report rcpt. of spec. type */
    t_u8 request:1;               /**< 11h: en/disable requests of specified type */
    t_u8 enable:1;                /**< 11h: enable report/request bits */
    t_u8 parallel:1;              /**< 11k: series or parallel with previous meas */
#else
    t_u8 parallel:1;              /**< 11k: series or parallel with previous meas */
    t_u8 enable:1;                /**< 11h: enable report/request bits */
    t_u8 request:1;               /**< 11h: en/disable requests of specified type */
    t_u8 report:1;                /**< 11h: en/disable report rcpt. of spec. type */
    t_u8 duration_mandatory:1;    /**< 11k: duration spec. for meas. is mandatory */
    t_u8 rsvd5_7:3;               /**< Reserved */
#endif                          /* BIG_ENDIAN_SUPPORT */

} MLAN_PACK_END MeasReqMode_t;

/**        
 * @brief Common measurement request structure (7.3.2.21.1 to 7.3.2.21.3)
 */
typedef MLAN_PACK_START struct
{
    t_u8 channel;      /**< Channel to measure */
    t_u64 start_time;  /**< TSF Start time of measurement (0 for immediate) */
    t_u16 duration;    /**< TU duration of the measurement */

} MLAN_PACK_END MeasReqCommonFormat_t;

/**        
 * @brief Basic measurement request structure (7.3.2.21.1)
 */
typedef MeasReqCommonFormat_t MeasReqBasic_t;

/**        
 * @brief CCA measurement request structure (7.3.2.21.2)
 */
typedef MeasReqCommonFormat_t MeasReqCCA_t;

/**        
 * @brief RPI measurement request structure (7.3.2.21.3)
 */
typedef MeasReqCommonFormat_t MeasReqRPI_t;

/**        
 * @brief Union of the availble measurement request types.  Passed in the 
 *        driver/firmware interface.
 */
typedef union
{
    MeasReqBasic_t basic; /**< Basic measurement request */
    MeasReqCCA_t cca;     /**< CCA measurement request */
    MeasReqRPI_t rpi;     /**< RPI measurement request */

} MeasRequest_t;

/**        
 * @brief Mode octet of the measurement report element (7.3.2.22)
 */
typedef MLAN_PACK_START struct
{
#ifdef BIG_ENDIAN_SUPPORT
    t_u8 rsvd3_7:5;        /**< Reserved */
    t_u8 refused:1;        /**< Measurement refused */
    t_u8 incapable:1;      /**< Incapable of performing measurement */
    t_u8 late:1;           /**< Start TSF time missed for measurement */
#else
    t_u8 late:1;           /**< Start TSF time missed for measurement */
    t_u8 incapable:1;      /**< Incapable of performing measurement */
    t_u8 refused:1;        /**< Measurement refused */
    t_u8 rsvd3_7:5;        /**< Reserved */
#endif                          /* BIG_ENDIAN_SUPPORT */

} MLAN_PACK_END MeasRptMode_t;

/**        
 * @brief Basic measurement report (7.3.2.22.1)
 */
typedef MLAN_PACK_START struct
{
    t_u8 channel;              /**< Channel to measured */
    t_u64 start_time;          /**< Start time (TSF) of measurement */
    t_u16 duration;            /**< Duration of measurement in TUs */
    MeasRptBasicMap_t map;     /**< Basic measurement report */

} MLAN_PACK_END MeasRptBasic_t;

/**        
 * @brief CCA measurement report (7.3.2.22.2)
 */
typedef MLAN_PACK_START struct
{
    t_u8 channel;                /**< Channel to measured */
    t_u64 start_time;            /**< Start time (TSF) of measurement */
    t_u16 duration;              /**< Duration of measurement in TUs  */
    t_u8 busy_fraction;          /**< Fractional duration CCA indicated chan busy */

} MLAN_PACK_END MeasRptCCA_t;

/**        
 * @brief RPI measurement report (7.3.2.22.3)
 */
typedef MLAN_PACK_START struct
{
    t_u8 channel;              /**< Channel to measured  */
    t_u64 start_time;          /**< Start time (TSF) of measurement */
    t_u16 duration;            /**< Duration of measurement in TUs  */
    t_u8 density[8];           /**< RPI Density histogram report */

} MLAN_PACK_END MeasRptRPI_t;

/**        
 * @brief Union of the availble measurement report types.  Passed in the 
 *        driver/firmware interface.
 */
typedef union
{
    MeasRptBasic_t basic;    /**< Basic measurement report */
    MeasRptCCA_t cca;        /**< CCA measurement report */
    MeasRptRPI_t rpi;        /**< RPI measurement report */

} MeasReport_t;

/**        
 * @brief Structure passed to firmware to perform a measurement
 */
typedef MLAN_PACK_START struct
{
    t_u8 mac_addr[MLAN_MAC_ADDR_LENGTH];          /**< Reporting STA address */
    t_u8 dialog_token;                   /**< Measurement dialog toke */
    MeasReqMode_t req_mode;              /**< Report mode  */
    MeasType_t meas_type;                /**< Measurement type */
    MeasRequest_t req;                   /**< Measurement request data */

} MLAN_PACK_END HostCmd_DS_MEASUREMENT_REQUEST;

/**        
 * @brief Structure passed back from firmware with a measurement report,
 *        also can be to send a measurement report to another STA
 */
typedef MLAN_PACK_START struct
{
    t_u8 mac_addr[MLAN_MAC_ADDR_LENGTH];          /**< Reporting STA address */
    t_u8 dialog_token;                   /**< Measurement dialog token */
    MeasRptMode_t rpt_mode;              /**< Report mode */
    MeasType_t meas_type;                /**< Measurement type */
    MeasReport_t rpt;                    /**< Measurement report data */

} MLAN_PACK_END HostCmd_DS_MEASUREMENT_REPORT;

/** HostCmd_DS_COMMAND */
typedef struct MLAN_PACK_START _HostCmd_DS_COMMAND
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
        /** Enhanced power save command */
        HostCmd_DS_802_11_PS_MODE_ENH psmode_enh;
        HostCmd_DS_802_11_HS_CFG_ENH opt_hs_cfg;
        /** Scan */
        HostCmd_DS_802_11_SCAN scan;
        /** Scan response */
        HostCmd_DS_802_11_SCAN_RSP scan_resp;

        HostCmd_DS_802_11_BG_SCAN_QUERY bg_scan_query;
        HostCmd_DS_802_11_BG_SCAN_QUERY_RSP bg_scan_query_resp;

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
#ifdef STA_SUPPORT
        /** Domain information */
        HostCmd_DS_802_11D_DOMAIN_INFO domain_info;
        /** Domain information response */
        HostCmd_DS_802_11D_DOMAIN_INFO_RSP domain_info_resp;
#endif                          /* STA_SUPPORT */
        HostCmd_DS_802_11_TPC_ADAPT_REQ tpc_req;
        HostCmd_DS_802_11_TPC_INFO tpc_info;
        HostCmd_DS_802_11_CHAN_SW_ANN chan_sw_ann;
        HostCmd_DS_MEASUREMENT_REQUEST meas_req;
        HostCmd_DS_MEASUREMENT_REPORT meas_rpt;
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
        /** WMM ADDTS */
        HostCmd_DS_WMM_ADDTS_REQ add_ts;
        /** WMM DELTS */
        HostCmd_DS_WMM_DELTS_REQ del_ts;
        /** WMM set/get queue config */
        HostCmd_DS_WMM_QUEUE_CONFIG queue_config;
        /** WMM on/of/get queue statistics */
        HostCmd_DS_WMM_QUEUE_STATS queue_stats;
        /** WMM get traffic stream status */
        HostCmd_DS_WMM_TS_STATUS ts_status;
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
        /** Mgmt IE list configuratoin */
        HostCmd_DS_MGMT_IE_LIST_CFG mgmt_ie_list;
        /** LDO configuration */
        HostCmd_DS_802_11_LDO_CFG ldo_cfg;
        /** System clock configuration */
        HostCmd_DS_ECL_SYSTEM_CLOCK_CONFIG sys_clock_cfg;
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
        /** Memory access */
        HostCmd_DS_MEM_ACCESS mem;

        /** Inactivity timeout extend */
        HostCmd_DS_INACTIVITY_TIMEOUT_EXT inactivity_to;
        /** BCA Timeshare */
        HostCmd_DS_802_11_BCA_TIMESHARE bca_timeshare;
#ifdef UAP_SUPPORT
        HostCmd_DS_SYS_CONFIG sys_config;
        HostCmd_DS_SYS_INFO sys_info;
        HostCmd_DS_STA_DEAUTH sta_deauth;
        HostCmd_DS_STA_LIST sta_list;
        HostCmd_DS_POWER_MGMT_EXT pm_cfg;
#endif                          /* UAP_SUPPORT */

       /** Sleep period command */
        HostCmd_DS_802_11_SLEEP_PERIOD sleep_pd;
       /** Sleep params command */
        HostCmd_DS_802_11_SLEEP_PARAMS sleep_param;

        HostCmd_DS_SET_BSS_MODE bss_mode;
    } params;
} MLAN_PACK_END HostCmd_DS_COMMAND;

/** PS_CMD_ConfirmSleep */
typedef MLAN_PACK_START struct _OPT_Confirm_Sleep
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
    /** Sleep comfirm param definition */
    sleep_confirm_param sleep_cfm;
} MLAN_PACK_END OPT_Confirm_Sleep;

typedef struct MLAN_PACK_START _opt_sleep_confirm_buffer
{
    /** Header for interface */
    t_u8 hdr[4];
    /** New power save command used to send sleep confirmation to the firmware */
    OPT_Confirm_Sleep ps_cfm_sleep;
} MLAN_PACK_END opt_sleep_confirm_buffer;

#ifdef PRAGMA_PACK
#pragma pack(pop)
#endif

#endif /* !_MLAN_FW_H_ */
