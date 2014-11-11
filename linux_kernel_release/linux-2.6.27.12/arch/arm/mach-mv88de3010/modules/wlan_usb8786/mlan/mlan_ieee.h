/** @file mlan_ieee.h
 *
 *  @brief This file contains IEEE information element related 
 *  definitions used in MLAN and MOAL module.
 *
 *  Copyright (C) 2008-2009, Marvell International Ltd. 
 *  All Rights Reserved
 */

/******************************************************
Change log:
    11/03/2008: initial version
******************************************************/

#ifndef _MLAN_IEEE_H_
#define _MLAN_IEEE_H_

/** WLAN supported rates */
#define WLAN_SUPPORTED_RATES            	14

/** WLAN supported rates extension*/
#define WLAN_SUPPORTED_RATES_EXT            32

/** Enumeration definition*/
/** WLAN_802_11_NETWORK_TYPE */
typedef enum _WLAN_802_11_NETWORK_TYPE
{
    Wlan802_11FH,
    Wlan802_11DS,
    /* Defined as upper bound */
    Wlan802_11NetworkTypeMax
} WLAN_802_11_NETWORK_TYPE;

/** Maximum size of IEEE Information Elements */
#define IEEE_MAX_IE_SIZE      256

/** IEEE Type definitions  */
typedef enum _IEEEtypes_ElementId_e
{
    SSID = 0,
    SUPPORTED_RATES = 1,
    FH_PARAM_SET = 2,
    DS_PARAM_SET = 3,
    CF_PARAM_SET = 4,

    IBSS_PARAM_SET = 6,

    COUNTRY_INFO = 7,

    HT_CAPABILITY = 45,
    HT_OPERATION = 61,
    BSSCO_2040 = 72,
    OVERLAPBSSSCANPARAM = 74,
    EXT_CAPABILITY = 127,

    ERP_INFO = 42,
    EXTENDED_SUPPORTED_RATES = 50,

    VENDOR_SPECIFIC_221 = 221,
    WMM_IE = VENDOR_SPECIFIC_221,

    WPS_IE = VENDOR_SPECIFIC_221,

    WPA_IE = VENDOR_SPECIFIC_221,
    RSN_IE = 48,
    VS_IE = VENDOR_SPECIFIC_221,
} __attribute__ ((packed)) IEEEtypes_ElementId_e;

/** IEEE IE header */
typedef struct _IEEEtypes_Header_t
{
    /** Element ID */
    t_u8 element_id;
    /** Length */
    t_u8 len;
} __attribute__ ((packed)) IEEEtypes_Header_t, *pIEEEtypes_Header_t;

/** Vendor specific IE header */
typedef struct _IEEEtypes_VendorHeader_t
{
    /** Element ID */
    t_u8 element_id;
    /** Length */
    t_u8 len;
    /** OUI */
    t_u8 oui[3];
    /** OUI type */
    t_u8 oui_type;
    /** OUI subtype */
    t_u8 oui_subtype;
    /** Version */
    t_u8 version;
} __attribute__ ((packed)) IEEEtypes_VendorHeader_t, *pIEEEtypes_VendorHeader_t;

/** Vendor specific IE */
typedef struct _IEEEtypes_VendorSpecific_t
{
    /** Vendor specific IE header */
    IEEEtypes_VendorHeader_t vend_hdr;
    /** IE Max - size of previous fields */
    t_u8 data[IEEE_MAX_IE_SIZE - sizeof(IEEEtypes_VendorHeader_t)];
}
__attribute__ ((packed)) IEEEtypes_VendorSpecific_t,
    *pIEEEtypes_VendorSpecific_t;

/** IEEE IE */
typedef struct _IEEEtypes_Generic_t
{
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    /** IE Max - size of previous fields */
    t_u8 data[IEEE_MAX_IE_SIZE - sizeof(IEEEtypes_Header_t)];
}
__attribute__ ((packed)) IEEEtypes_Generic_t, *pIEEEtypes_Generic_t;

/** Capability information mask */
#define CAPINFO_MASK    (~(MBIT(15) | MBIT(14) |            \
                           MBIT(12) | MBIT(11) | MBIT(9)))

/** Capability Bit Map*/
#ifdef BIG_ENDIAN
typedef struct _IEEEtypes_CapInfo_t
{
    t_u8 rsrvd1:2;
    t_u8 dsss_ofdm:1;
    t_u8 rsvrd2:2;
    t_u8 short_slot_time:1;
    t_u8 rsrvd3:1;
    t_u8 spectrum_mgmt:1;
    t_u8 chan_agility:1;
    t_u8 pbcc:1;
    t_u8 short_preamble:1;
    t_u8 privacy:1;
    t_u8 cf_poll_rqst:1;
    t_u8 cf_pollable:1;
    t_u8 ibss:1;
    t_u8 ess:1;
} __attribute__ ((packed)) IEEEtypes_CapInfo_t, *pIEEEtypes_CapInfo_t;
#else
typedef struct _IEEEtypes_CapInfo_t
{
    /** Capability Bit Map : ESS */
    t_u8 ess:1;
    /** Capability Bit Map : IBSS */
    t_u8 ibss:1;
    /** Capability Bit Map : CF pollable */
    t_u8 cf_pollable:1;
    /** Capability Bit Map : CF poll request */
    t_u8 cf_poll_rqst:1;
    /** Capability Bit Map : privacy */
    t_u8 privacy:1;
    /** Capability Bit Map : Short preamble */
    t_u8 short_preamble:1;
    /** Capability Bit Map : PBCC */
    t_u8 pbcc:1;
    /** Capability Bit Map : Channel agility */
    t_u8 chan_agility:1;
    /** Capability Bit Map : Spectrum management */
    t_u8 spectrum_mgmt:1;
    /** Capability Bit Map : Reserved */
    t_u8 rsrvd3:1;
    /** Capability Bit Map : Short slot time */
    t_u8 short_slot_time:1;
    /** Capability Bit Map : APSD */
    t_u8 Apsd:1;
    /** Capability Bit Map : Reserved */
    t_u8 rsvrd2:1;
    /** Capability Bit Map : DSS OFDM */
    t_u8 dsss_ofdm:1;
    /** Capability Bit Map : Reserved */
    t_u8 rsrvd1:2;
} __attribute__ ((packed)) IEEEtypes_CapInfo_t, *pIEEEtypes_CapInfo_t;
#endif /* BIG_ENDIAN */

/** IEEEtypes_CfParamSet_t */
typedef struct _IEEEtypes_CfParamSet_t
{
    /** CF peremeter : Element ID */
    t_u8 element_id;
    /** CF peremeter : Length */
    t_u8 len;
    /** CF peremeter : Count */
    t_u8 cfp_cnt;
    /** CF peremeter : Period */
    t_u8 cfp_period;
    /** CF peremeter : Maximum duration */
    t_u16 cfp_max_duration;
    /** CF peremeter : Remaining duration */
    t_u16 cfp_duration_remaining;
} __attribute__ ((packed)) IEEEtypes_CfParamSet_t, *pIEEEtypes_CfParamSet_t;

/** IEEEtypes_IbssParamSet_t */
typedef struct _IEEEtypes_IbssParamSet_t
{
    /** Element ID */
    t_u8 element_id;
    /** Length */
    t_u8 len;
    /** ATIM window value */
    t_u16 atim_window;
} __attribute__ ((packed)) IEEEtypes_IbssParamSet_t, *pIEEEtypes_IbssParamSet_t;

/** IEEEtypes_SsParamSet_t */
typedef union _IEEEtypes_SsParamSet_t
{
    /** SS parameter : CF parameter set */
    IEEEtypes_CfParamSet_t cf_param_set;
    /** SS parameter : IBSS parameter set */
    IEEEtypes_IbssParamSet_t ibss_param_set;
} __attribute__ ((packed)) IEEEtypes_SsParamSet_t, *pIEEEtypes_SsParamSet_t;

/** IEEEtypes_FhParamSet_t */
typedef struct _IEEEtypes_FhParamSet_t
{
    /** FH parameter : Element ID */
    t_u8 element_id;
    /** FH parameter : Length */
    t_u8 len;
    /** FH parameter : Dwell time */
    t_u16 dwell_time;
    /** FH parameter : Hop set */
    t_u8 hop_set;
    /** FH parameter : Hop pattern */
    t_u8 hop_pattern;
    /** FH parameter : Hop index */
    t_u8 hop_index;
} __attribute__ ((packed)) IEEEtypes_FhParamSet_t, *pIEEEtypes_FhParamSet_t;

/** IEEEtypes_DsParamSet_t */
typedef struct _IEEEtypes_DsParamSet_t
{
    /** DS parameter : Element ID */
    t_u8 element_id;
    /** DS parameter : Length */
    t_u8 len;
    /** DS parameter : Current channel */
    t_u8 current_chan;
} __attribute__ ((packed)) IEEEtypes_DsParamSet_t, *pIEEEtypes_DsParamSet_t;

/** IEEEtypes_PhyParamSet_t */
typedef union _IEEEtypes_PhyParamSet_t
{
    /** FH parameter set */
    IEEEtypes_FhParamSet_t fh_param_set;
    /** DS parameter set */
    IEEEtypes_DsParamSet_t ds_param_set;
} __attribute__ ((packed)) IEEEtypes_PhyParamSet_t, *pIEEEtypes_PhyParamSet_t;

/** IEEEtypes_ERPInfo_t */
typedef struct _IEEEtypes_ERPInfo_t
{
    /** Element ID */
    t_u8 element_id;
    /** Length */
    t_u8 len;
    /** ERP flags */
    t_u8 erp_flags;
} __attribute__ ((packed)) IEEEtypes_ERPInfo_t, *pIEEEtypes_ERPInfo_t;

/** IEEEtypes_AId_t */
typedef t_u16 IEEEtypes_AId_t;

/** IEEEtypes_StatusCode_t */
typedef t_u16 IEEEtypes_StatusCode_t;

/** IEEEtypes_AssocRsp_t */
typedef struct _IEEEtypes_AssocRsp_t
{
    /** Capability information */
    IEEEtypes_CapInfo_t capability;
    /** Association response status code */
    IEEEtypes_StatusCode_t status_code;
    /** Association ID */
    IEEEtypes_AId_t a_id;
    /** IE data buffer */
    t_u8 ie_buffer[1];
} __attribute__ ((packed)) IEEEtypes_AssocRsp_t, *pIEEEtypes_AssocRsp_t;

/** 802.11 supported rates */
typedef t_u8 WLAN_802_11_RATES[WLAN_SUPPORTED_RATES];

/** Maximum number of AC QOS queues available in the driver/firmware */
#define MAX_AC_QUEUES 4

/** Data structure of WMM QoS information */
typedef struct _IEEEtypes_WmmQosInfo_t
{
#ifdef BIG_ENDIAN
    /** QoS UAPSD */
    t_u8 qos_uapsd:1;
    /** Reserved */
    t_u8 reserved:3;
    /** Parameter set count */
    t_u8 para_set_count:4;
#else
    /** Parameter set count */
    t_u8 para_set_count:4;
    /** Reserved */
    t_u8 reserved:3;
    /** QoS UAPSD */
    t_u8 qos_uapsd:1;
#endif
} __attribute__ ((packed)) IEEEtypes_WmmQosInfo_t, *pIEEEtypes_WmmQosInfo_t;

/** Data structure of WMM Aci/Aifsn */
typedef struct _IEEEtypes_WmmAciAifsn_t
{
#ifdef BIG_ENDIAN
    /** Reserved */
    t_u8 reserved:1;
    /** Aci */
    t_u8 aci:2;
    /** Acm */
    t_u8 acm:1;
    /** Aifsn */
    t_u8 aifsn:4;
#else
    /** Aifsn */
    t_u8 aifsn:4;
    /** Acm */
    t_u8 acm:1;
    /** Aci */
    t_u8 aci:2;
    /** Reserved */
    t_u8 reserved:1;
#endif
} __attribute__ ((packed)) IEEEtypes_WmmAciAifsn_t, *pIEEEtypes_WmmAciAifsn_t;

/** Data structure of WMM ECW */
typedef struct _IEEEtypes_WmmEcw_t
{
#ifdef BIG_ENDIAN
    /** Maximum Ecw */
    t_u8 ecw_max:4;
    /** Minimum Ecw */
    t_u8 ecw_min:4;
#else
    /** Minimum Ecw */
    t_u8 ecw_min:4;
    /** Maximum Ecw */
    t_u8 ecw_max:4;
#endif
} __attribute__ ((packed)) IEEEtypes_WmmEcw_t, *pIEEEtypes_WmmEcw_t;

/** Data structure of WMM AC parameters  */
typedef struct _IEEEtypes_WmmAcParameters_t
{
    IEEEtypes_WmmAciAifsn_t aci_aifsn;       /**< AciAifSn */
    IEEEtypes_WmmEcw_t ecw;                 /**< Ecw */
    t_u16 tx_op_limit;                        /**< Tx op limit */
} __attribute__ ((packed)) IEEEtypes_WmmAcParameters_t,
    *pIEEEtypes_WmmAcParameters_t;

/** Data structure of WMM Info IE  */
typedef struct _IEEEtypes_WmmInfo_t
{

    /**
     * WMM Info IE - Vendor Specific Header:
     *   element_id  [221/0xdd]
     *   Len         [7] 
     *   Oui         [00:50:f2]
     *   OuiType     [2]
     *   OuiSubType  [0]
     *   Version     [1]
     */
    IEEEtypes_VendorHeader_t vend_hdr;

    /** QoS information */
    IEEEtypes_WmmQosInfo_t qos_info;

} __attribute__ ((packed)) IEEEtypes_WmmInfo_t, *pIEEEtypes_WmmInfo_t;

/** Data structure of WMM parameter IE  */
typedef struct _IEEEtypes_WmmParameter_t
{
    /**
     * WMM Parameter IE - Vendor Specific Header:
     *   element_id  [221/0xdd]
     *   Len         [24] 
     *   Oui         [00:50:f2]
     *   OuiType     [2]
     *   OuiSubType  [1]
     *   Version     [1]
     */
    IEEEtypes_VendorHeader_t vend_hdr;

    /** QoS information */
    IEEEtypes_WmmQosInfo_t qos_info;
    /** Reserved */
    t_u8 reserved;

    /** AC Parameters Record WMM_AC_BE, WMM_AC_BK, WMM_AC_VI, WMM_AC_VO */
    IEEEtypes_WmmAcParameters_t ac_params[MAX_AC_QUEUES];

} __attribute__ ((packed)) IEEEtypes_WmmParameter_t, *pIEEEtypes_WmmParameter_t;

/** Maximum subbands for 11d */
#define MRVDRV_MAX_SUBBAND_802_11D              83
/** Country code length */
#define COUNTRY_CODE_LEN                        3

/** Data structure for subband set */
typedef struct _IEEEtypes_SubbandSet_t
{
    /** First channel */
    t_u8 first_chan;
    /** Number of channels */
    t_u8 no_of_chan;
    /** Maximum Tx power */
    t_u8 max_tx_pwr;
} __attribute__ ((packed)) IEEEtypes_SubbandSet_t, *pIEEEtypes_SubbandSet_t;

/** Data structure for Country IE */
typedef struct _IEEEtypes_CountryInfoSet_t
{
    /** Element ID */
    t_u8 element_id;
    /** Length */
    t_u8 len;
    /** Country code */
    t_u8 country_code[COUNTRY_CODE_LEN];
    /** Set of subbands */
    IEEEtypes_SubbandSet_t sub_band[1];
} __attribute__ ((packed)) IEEEtypes_CountryInfoSet_t,
    *pIEEEtypes_CountryInfoSet_t;

/** Data structure for Country IE full set */
typedef struct _IEEEtypes_CountryInfoFullSet_t
{
    /** Element ID */
    t_u8 element_id;
    /** Length */
    t_u8 len;
    /** Country code */
    t_u8 country_code[COUNTRY_CODE_LEN];
    /** Set of subbands */
    IEEEtypes_SubbandSet_t sub_band[MRVDRV_MAX_SUBBAND_802_11D];
} __attribute__ ((packed)) IEEEtypes_CountryInfoFullSet_t,
    *pIEEEtypes_CountryInfoFullSet_t;

/** HT Capabilities Data */
typedef struct _HTCap_t
{
    /** HT Capabilities Info field */
    t_u16 ht_cap_info;
    /** A-MPDU Parameters field */
    t_u8 ampdu_param;
    /** Supported MCS Set field */
    t_u8 supported_mcs_set[16];
    /** HT Extended Capabilities field */
    t_u16 ht_ext_cap;
    /** Transmit Beamforming Capabilities field */
    t_u32 tx_bf_cap;
    /** Antenna Selection Capability field */
    t_u8 asel;
    /** Reserved set to 0 */
    t_u16 reserved;
} HTCap_t, *pHTCap_t;

/** HT Information Data */
typedef struct _HTInfo_t
{
    /** Primary channel */
    t_u8 pri_chan;
    /** Field 2 */
    t_u8 field2;
    /** Field 3 */
    t_u16 field3;
    /** Field 4 */
    t_u16 field4;
    /** Bitmap indicating MCSs supported by all HT STAs in the BSS */
    t_u8 basic_mcs_set[16];
    /** Reserved set to 0 */
    t_u16 reserved;
} HTInfo_t, *pHTInfo_t;

/** 20/40 BSS Coexistence Data */
typedef struct _BSSCo2040_t
{
    /** 20/40 BSS Coexistence value */
    t_u8 bss_co_2040_value;
    /** Reserve field */
    t_u8 reserved[3];
} BSSCo2040_t, *pBSSCo2040_t;

/** Extended Capabilities Data */
typedef struct _ExtCap_t
{
    /** Extended Capabilities value */
    t_u8 ext_cap_value;
    /** Reserved field */
    t_u8 reserved[3];
} ExtCap_t, *pExtCap_t;

/** Overlapping BSS Scan Parameters Data */
typedef struct _OverlapBSSScanParam_t
{
    /** OBSS Scan Passive Dwell */
    t_u16 obss_scan_passive_dwell;
    /** OBSS Scan Active Dwell */
    t_u16 obss_scan_active_dwell;
    /** BSS Channel Width Trigger Scan Interval */
    t_u16 bss_chan_width_trigger_scan_int;
    /** OBSS Scan Passive Total Per Channel */
    t_u16 obss_scan_passive_total;
    /** OBSS Scan Active Total Per Channel */
    t_u16 obss_scan_active_total;
    /** BSS Width Channel Transition Delay Factor */
    t_u16 bss_width_chan_trans_delay;
    /** OBSS Scan Activity Threshold */
    t_u16 obss_scan_active_threshold;
    /** Reserved Field */
    t_u16 reserved;
} OBSSScanParam_t, *pOBSSScanParam_t;

/** HT Capabilities IE */
typedef struct _IEEEtypes_HTCap_t
{
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    /** HTCap struct */
    HTCap_t ht_cap;
} __attribute__ ((packed)) IEEEtypes_HTCap_t, *pIEEEtypes_HTCap_t;

/** HT Information IE */
typedef struct _IEEEtypes_HTInfo_t
{
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    /** HTInfo struct */
    HTInfo_t ht_info;
} __attribute__ ((packed)) IEEEtypes_HTInfo_t, *pIEEEtypes_HTInfo_t;

/** 20/40 BSS Coexistence IE */
typedef struct _IEEEtypes_2040BSSCo_t
{
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    /** BSSCo2040_t struct */
    BSSCo2040_t bss_co_2040;
} __attribute__ ((packed)) IEEEtypes_2040BSSCo_t, *pIEEEtypes_2040BSSCo_t;

/** Extended Capabilities IE */
typedef struct _IEEEtypes_ExtCap_t
{
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    /** ExtCap_t struct */
    ExtCap_t ext_cap;
} __attribute__ ((packed)) IEEEtypes_ExtCap_t, *pIEEEtypes_ExtCap_t;

/** Overlapping BSS Scan Parameters IE */
typedef struct _IEEEtypes_OverlapBSSScanParam_t
{
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    /** OBSSScanParam_t struct */
    OBSSScanParam_t obss_scan_param;
} __attribute__ ((packed)) IEEEtypes_OverlapBSSScanParam_t,
    *pIEEEtypes_OverlapBSSScanParam_t;

/** BSSDescriptor_t
 *    Structure used to store information for beacon/probe response
 */
typedef struct _BSSDescriptor_t
{
    /** MAC address */
    mlan_802_11_mac_addr mac_address;

    /** SSID */
    mlan_802_11_ssid ssid;

    /** WEP encryption requirement */
    t_u32 privacy;

    /** Receive signal strength in dBm */
    t_s32 rssi;

    /** Channel */
    t_u32 channel;

    /** Freq */
    t_u32 freq;

    /** Beacon period */
    t_u16 beacon_period;

    /** ATIM window */
    t_u32 atim_window;

    /** ERP flags */
    t_u8 erp_flags;

    /** Type of network in use */
    WLAN_802_11_NETWORK_TYPE network_type_use;

    /** Network infrastructure mode */
    t_u32 bss_mode;

    /** Network supported rates */
    WLAN_802_11_RATES supported_rates;

    /** Supported data rates */
    t_u8 data_rates[WLAN_SUPPORTED_RATES];

    /** Network band.
     * BAND_B(0x01): 'b' band
     * BAND_G(0x02): 'g' band
     * BAND_A(0X04): 'a' band
     */
    t_u16 bss_band;

    /** TSF timestamp from the current firmware TSF */
    t_u64 network_tsf;

    /** TSF value included in the beacon/probe response */
    t_u8 time_stamp[8];

    /** PHY parameter set */
    IEEEtypes_PhyParamSet_t phy_param_set;

    /** SS parameter set */
    IEEEtypes_SsParamSet_t ss_param_set;

    /** Capability information */
    IEEEtypes_CapInfo_t cap_info;

    /** WMM IE */
    IEEEtypes_WmmParameter_t wmm_ie;

    /** 802.11n BSS information */
    /** HT Capabilities IE */
    IEEEtypes_HTCap_t *pht_cap;
    /** HT Capabilities Offset */
    t_u16 ht_cap_offset;
    /** HT Information IE */
    IEEEtypes_HTInfo_t *pht_info;
    /** HT Information Offset */
    t_u16 ht_info_offset;
    /** 20/40 BSS Coexistence IE */
    IEEEtypes_2040BSSCo_t *pbss_co_2040;
    /** 20/40 BSS Coexistence Offset */
    t_u16 bss_co_2040_offset;
    /** Extended Capabilities IE */
    IEEEtypes_ExtCap_t *pext_cap;
    /** Extended Capabilities Offset */
    t_u16 ext_cap_offset;
    /** Overlapping BSS Scan Parameters IE */
    IEEEtypes_OverlapBSSScanParam_t *poverlap_bss_scan_param;
    /** Overlapping BSS Scan Parameters Offset */
    t_u16 overlap_bss_offset;

    /** Country information set */
    IEEEtypes_CountryInfoFullSet_t country_info;

    /** WPA IE */
    IEEEtypes_VendorSpecific_t *pwpa_ie;
    /** WPA IE offset in the beacon buffer */
    t_u16 wpa_offset;
    /** RSN IE */
    IEEEtypes_Generic_t *prsn_ie;
    /** RSN IE offset in the beacon buffer */
    t_u16 rsn_offset;
    /** WPS IE */
    IEEEtypes_VendorSpecific_t *pwps_ie;
    /** WPS IE offset in the beacon buffer */
    t_u16 wps_offset;

    /** Pointer to the returned scan response */
    t_u8 *pbeacon_buf;
    /** Length of the stored scan response */
    t_u32 beacon_buf_size;
    /** Max allocated size for updated scan response */
    t_u32 beacon_buf_size_max;

} BSSDescriptor_t, *pBSSDescriptor_t;

#endif /* !_MLAN_IEEE_H_ */
