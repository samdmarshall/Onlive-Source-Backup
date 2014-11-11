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

#ifdef STA_SUPPORT
/** WLAN supported rates extension*/
#define WLAN_SUPPORTED_RATES_EXT            32
#endif /* STA_SUPPORT */

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

#ifdef PRAGMA_PACK
#pragma pack(push, 1)
#endif

/** IEEE Type definitions  */
typedef MLAN_PACK_START enum _IEEEtypes_ElementId_e
{
    SSID = 0,
    SUPPORTED_RATES = 1,
    FH_PARAM_SET = 2,
    DS_PARAM_SET = 3,
    CF_PARAM_SET = 4,

    IBSS_PARAM_SET = 6,

#ifdef STA_SUPPORT
    COUNTRY_INFO = 7,
#endif /* STA_SUPPORT */

    POWER_CONSTRAINT = 32,
    POWER_CAPABILITY = 33,
    TPC_REQUEST = 34,
    TPC_REPORT = 35,
    SUPPORTED_CHANNELS = 36,
    CHANNEL_SWITCH_ANN = 37,
    QUIET = 40,
    IBSS_DFS = 41,
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
    WAPI_IE = 68,
} MLAN_PACK_END IEEEtypes_ElementId_e;

/** IEEE IE header */
typedef MLAN_PACK_START struct _IEEEtypes_Header_t
{
    /** Element ID */
    t_u8 element_id;
    /** Length */
    t_u8 len;
} MLAN_PACK_END IEEEtypes_Header_t, *pIEEEtypes_Header_t;

/** Vendor specific IE header */
typedef MLAN_PACK_START struct _IEEEtypes_VendorHeader_t
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
} MLAN_PACK_END IEEEtypes_VendorHeader_t, *pIEEEtypes_VendorHeader_t;

/** Vendor specific IE */
typedef MLAN_PACK_START struct _IEEEtypes_VendorSpecific_t
{
    /** Vendor specific IE header */
    IEEEtypes_VendorHeader_t vend_hdr;
    /** IE Max - size of previous fields */
    t_u8 data[IEEE_MAX_IE_SIZE - sizeof(IEEEtypes_VendorHeader_t)];
}
MLAN_PACK_END IEEEtypes_VendorSpecific_t, *pIEEEtypes_VendorSpecific_t;

/** IEEE IE */
typedef MLAN_PACK_START struct _IEEEtypes_Generic_t
{
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    /** IE Max - size of previous fields */
    t_u8 data[IEEE_MAX_IE_SIZE - sizeof(IEEEtypes_Header_t)];
}
MLAN_PACK_END IEEEtypes_Generic_t, *pIEEEtypes_Generic_t;

/** Capability information mask */
#define CAPINFO_MASK    (~(MBIT(15) | MBIT(14) |            \
                           MBIT(12) | MBIT(11) | MBIT(9)))

/** Capability Bit Map*/
#ifdef BIG_ENDIAN_SUPPORT
typedef MLAN_PACK_START struct _IEEEtypes_CapInfo_t
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
} MLAN_PACK_END IEEEtypes_CapInfo_t, *pIEEEtypes_CapInfo_t;
#else
typedef MLAN_PACK_START struct _IEEEtypes_CapInfo_t
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
} MLAN_PACK_END IEEEtypes_CapInfo_t, *pIEEEtypes_CapInfo_t;
#endif /* BIG_ENDIAN_SUPPORT */

/** IEEEtypes_CfParamSet_t */
typedef MLAN_PACK_START struct _IEEEtypes_CfParamSet_t
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
} MLAN_PACK_END IEEEtypes_CfParamSet_t, *pIEEEtypes_CfParamSet_t;

/** IEEEtypes_IbssParamSet_t */
typedef MLAN_PACK_START struct _IEEEtypes_IbssParamSet_t
{
    /** Element ID */
    t_u8 element_id;
    /** Length */
    t_u8 len;
    /** ATIM window value */
    t_u16 atim_window;
} MLAN_PACK_END IEEEtypes_IbssParamSet_t, *pIEEEtypes_IbssParamSet_t;

/** IEEEtypes_SsParamSet_t */
typedef MLAN_PACK_START union _IEEEtypes_SsParamSet_t
{
    /** SS parameter : CF parameter set */
    IEEEtypes_CfParamSet_t cf_param_set;
    /** SS parameter : IBSS parameter set */
    IEEEtypes_IbssParamSet_t ibss_param_set;
} MLAN_PACK_END IEEEtypes_SsParamSet_t, *pIEEEtypes_SsParamSet_t;

/** IEEEtypes_FhParamSet_t */
typedef MLAN_PACK_START struct _IEEEtypes_FhParamSet_t
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
} MLAN_PACK_END IEEEtypes_FhParamSet_t, *pIEEEtypes_FhParamSet_t;

/** IEEEtypes_DsParamSet_t */
typedef MLAN_PACK_START struct _IEEEtypes_DsParamSet_t
{
    /** DS parameter : Element ID */
    t_u8 element_id;
    /** DS parameter : Length */
    t_u8 len;
    /** DS parameter : Current channel */
    t_u8 current_chan;
} MLAN_PACK_END IEEEtypes_DsParamSet_t, *pIEEEtypes_DsParamSet_t;

/** IEEEtypes_PhyParamSet_t */
typedef MLAN_PACK_START union _IEEEtypes_PhyParamSet_t
{
    /** FH parameter set */
    IEEEtypes_FhParamSet_t fh_param_set;
    /** DS parameter set */
    IEEEtypes_DsParamSet_t ds_param_set;
} MLAN_PACK_END IEEEtypes_PhyParamSet_t, *pIEEEtypes_PhyParamSet_t;

/** IEEEtypes_ERPInfo_t */
typedef MLAN_PACK_START struct _IEEEtypes_ERPInfo_t
{
    /** Element ID */
    t_u8 element_id;
    /** Length */
    t_u8 len;
    /** ERP flags */
    t_u8 erp_flags;
} MLAN_PACK_END IEEEtypes_ERPInfo_t, *pIEEEtypes_ERPInfo_t;

/** IEEEtypes_AId_t */
typedef t_u16 IEEEtypes_AId_t;

/** IEEEtypes_StatusCode_t */
typedef t_u16 IEEEtypes_StatusCode_t;

/** IEEEtypes_AssocRsp_t */
typedef MLAN_PACK_START struct _IEEEtypes_AssocRsp_t
{
    /** Capability information */
    IEEEtypes_CapInfo_t capability;
    /** Association response status code */
    IEEEtypes_StatusCode_t status_code;
    /** Association ID */
    IEEEtypes_AId_t a_id;
    /** IE data buffer */
    t_u8 ie_buffer[1];
} MLAN_PACK_END IEEEtypes_AssocRsp_t, *pIEEEtypes_AssocRsp_t;

/** 802.11 supported rates */
typedef t_u8 WLAN_802_11_RATES[WLAN_SUPPORTED_RATES];

/** Maximum number of AC QOS queues available in the driver/firmware */
#define MAX_AC_QUEUES 4

/** Data structure of WMM QoS information */
typedef MLAN_PACK_START struct _IEEEtypes_WmmQosInfo_t
{
#ifdef BIG_ENDIAN_SUPPORT
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
#endif                          /* BIG_ENDIAN_SUPPORT */
} MLAN_PACK_END IEEEtypes_WmmQosInfo_t, *pIEEEtypes_WmmQosInfo_t;

/** Data structure of WMM Aci/Aifsn */
typedef MLAN_PACK_START struct _IEEEtypes_WmmAciAifsn_t
{
#ifdef BIG_ENDIAN_SUPPORT
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
#endif                          /* BIG_ENDIAN_SUPPORT */
} MLAN_PACK_END IEEEtypes_WmmAciAifsn_t, *pIEEEtypes_WmmAciAifsn_t;

/** Data structure of WMM ECW */
typedef MLAN_PACK_START struct _IEEEtypes_WmmEcw_t
{
#ifdef BIG_ENDIAN_SUPPORT
    /** Maximum Ecw */
    t_u8 ecw_max:4;
    /** Minimum Ecw */
    t_u8 ecw_min:4;
#else
    /** Minimum Ecw */
    t_u8 ecw_min:4;
    /** Maximum Ecw */
    t_u8 ecw_max:4;
#endif                          /* BIG_ENDIAN_SUPPORT */
} MLAN_PACK_END IEEEtypes_WmmEcw_t, *pIEEEtypes_WmmEcw_t;

/** Data structure of WMM AC parameters  */
typedef MLAN_PACK_START struct _IEEEtypes_WmmAcParameters_t
{
    IEEEtypes_WmmAciAifsn_t aci_aifsn;       /**< AciAifSn */
    IEEEtypes_WmmEcw_t ecw;                 /**< Ecw */
    t_u16 tx_op_limit;                        /**< Tx op limit */
} MLAN_PACK_END IEEEtypes_WmmAcParameters_t, *pIEEEtypes_WmmAcParameters_t;

/** Data structure of WMM Info IE  */
typedef MLAN_PACK_START struct _IEEEtypes_WmmInfo_t
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

} MLAN_PACK_END IEEEtypes_WmmInfo_t, *pIEEEtypes_WmmInfo_t;

/** Data structure of WMM parameter IE  */
typedef MLAN_PACK_START struct _IEEEtypes_WmmParameter_t
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
} MLAN_PACK_END IEEEtypes_WmmParameter_t, *pIEEEtypes_WmmParameter_t;

typedef MLAN_PACK_START enum
{

    TSPEC_DIR_UPLINK = 0,
    TSPEC_DIR_DOWNLINK = 1,
    /* 2 is a reserved value */
    TSPEC_DIR_BIDIRECT = 3,

} MLAN_PACK_END IEEEtypes_WMM_TSPEC_TS_Info_Direction_e;

typedef MLAN_PACK_START enum
{

    TSPEC_PSB_LEGACY = 0,
    TSPEC_PSB_TRIG = 1,

} MLAN_PACK_END IEEEtypes_WMM_TSPEC_TS_Info_PSB_e;

typedef MLAN_PACK_START enum
{

    TSPEC_ACKPOLICY_NORMAL = 0,
    TSPEC_ACKPOLICY_NOACK = 1,
    /* 2 is reserved */
    TSPEC_ACKPOLICY_BLOCKACK = 3,

} MLAN_PACK_END IEEEtypes_WMM_TSPEC_TS_Info_AckPolicy_e;

typedef MLAN_PACK_START enum
{

    TSPEC_TRAFFIC_APERIODIC = 0,
    TSPEC_TRAFFIC_PERIODIC = 1,

} MLAN_PACK_END IEEEtypes_WMM_TSPEC_TS_TRAFFIC_TYPE_e;

typedef MLAN_PACK_START struct
{
#ifdef BIG_ENDIAN_SUPPORT
    t_u8 Reserved17_23:7;       // ! Reserved
    t_u8 Schedule:1;
    IEEEtypes_WMM_TSPEC_TS_Info_AckPolicy_e AckPolicy:2;
    t_u8 UserPri:3;             // ! 802.1d User Priority
    IEEEtypes_WMM_TSPEC_TS_Info_PSB_e PowerSaveBehavior:1;      // !
                                                                // Legacy/Trigg
    t_u8 Aggregation:1;         // ! Reserved
    t_u8 AccessPolicy2:1;       // ! 
    t_u8 AccessPolicy1:1;       // ! 
    IEEEtypes_WMM_TSPEC_TS_Info_Direction_e Direction:2;
    t_u8 TID:4;                 // ! Unique identifier
    IEEEtypes_WMM_TSPEC_TS_TRAFFIC_TYPE_e TrafficType:1;
#else
    IEEEtypes_WMM_TSPEC_TS_TRAFFIC_TYPE_e TrafficType:1;
    t_u8 TID:4;                 // ! Unique identifier
    IEEEtypes_WMM_TSPEC_TS_Info_Direction_e Direction:2;
    t_u8 AccessPolicy1:1;       // ! 
    t_u8 AccessPolicy2:1;       // ! 
    t_u8 Aggregation:1;         // ! Reserved
    IEEEtypes_WMM_TSPEC_TS_Info_PSB_e PowerSaveBehavior:1;      // !
                                                                // Legacy/Trigg
    t_u8 UserPri:3;             // ! 802.1d User Priority
    IEEEtypes_WMM_TSPEC_TS_Info_AckPolicy_e AckPolicy:2;
    t_u8 Schedule:1;
    t_u8 Reserved17_23:7;       // ! Reserved
#endif
} MLAN_PACK_END IEEEtypes_WMM_TSPEC_TS_Info_t;

typedef MLAN_PACK_START struct
{
#ifdef BIG_ENDIAN_SUPPORT
    t_u16 Fixed:1;              // ! 1: Fixed size given in Size, 0: Var, size
                                // is nominal
    t_u16 Size:15;              // ! Nominal size in octets
#else
    t_u16 Size:15;              // ! Nominal size in octets
    t_u16 Fixed:1;              // ! 1: Fixed size given in Size, 0: Var, size
                                // is nominal
#endif
} MLAN_PACK_END IEEEtypes_WMM_TSPEC_NomMSDUSize_t;

typedef MLAN_PACK_START struct
{
#ifdef BIG_ENDIAN_SUPPORT
    t_u16 Whole:3;              // ! Whole portion
    t_u16 Fractional:13;        // ! Fractional portion
#else
    t_u16 Fractional:13;        // ! Fractional portion
    t_u16 Whole:3;              // ! Whole portion
#endif
} MLAN_PACK_END IEEEtypes_WMM_TSPEC_SBWA;

typedef MLAN_PACK_START struct
{

    IEEEtypes_WMM_TSPEC_TS_Info_t TSInfo;
    IEEEtypes_WMM_TSPEC_NomMSDUSize_t NomMSDUSize;
    t_u16 MaximumMSDUSize;
    t_u32 MinServiceInterval;
    t_u32 MaxServiceInterval;
    t_u32 InactivityInterval;
    t_u32 SuspensionInterval;
    t_u32 ServiceStartTime;
    t_u32 MinimumDataRate;
    t_u32 MeanDataRate;
    t_u32 PeakDataRate;
    t_u32 MaxBurstSize;
    t_u32 DelayBound;
    t_u32 MinPHYRate;
    IEEEtypes_WMM_TSPEC_SBWA SurplusBWAllowance;
    t_u16 MediumTime;
} MLAN_PACK_END IEEEtypes_WMM_TSPEC_Body_t;

typedef MLAN_PACK_START struct
{
    t_u8 ElementId;
    t_u8 Len;
    t_u8 OuiType[4];            /* 00:50:f2:02 */
    t_u8 OuiSubType;            /* 01 */
    t_u8 Version;

    IEEEtypes_WMM_TSPEC_Body_t TspecBody;

} MLAN_PACK_END IEEEtypes_WMM_TSPEC_t;

typedef MLAN_PACK_START enum
{

    IEEE_MGMT_ACTION_CATEGORY_SPECTRUM_MGMT = 0,
    IEEE_MGMT_ACTION_CATEGORY_QOS = 1,
    IEEE_MGMT_ACTION_CATEGORY_DLS = 2,
    IEEE_MGMT_ACTION_CATEGORY_BLOCK_ACK = 3,
    IEEE_MGMT_ACTION_CATEGORY_PUBLIC = 4,
    IEEE_MGMT_ACTION_CATEGORY_RADIO_RSRC = 5,
    IEEE_MGMT_ACTION_CATEGORY_FAST_BSS_TRANS = 6,
    IEEE_MGMT_ACTION_CATEGORY_HT = 7,

    IEEE_MGMT_ACTION_CATEGORY_WNM = 10,
    IEEE_MGMT_ACTION_CATEGORY_UNPROTECT_WNM = 11,

    IEEE_MGMT_ACTION_CATEGORY_WMM_TSPEC = 17
} MLAN_PACK_END IEEEtypes_ActionCategory_e;

typedef MLAN_PACK_START enum
{

    TSPEC_ACTION_CODE_ADDTS_REQ = 0,
    TSPEC_ACTION_CODE_ADDTS_RSP = 1,
    TSPEC_ACTION_CODE_DELTS = 2,

} MLAN_PACK_END IEEEtypes_WMM_Tspec_Action_e;

typedef MLAN_PACK_START struct
{

    IEEEtypes_ActionCategory_e category;
    IEEEtypes_WMM_Tspec_Action_e action;
    t_u8 dialogToken;

} MLAN_PACK_END IEEEtypes_WMM_Tspec_Action_Base_Tspec_t;

typedef MLAN_PACK_START struct
{

    IEEEtypes_WMM_Tspec_Action_Base_Tspec_t tspecAct;
    t_u8 statusCode;
    IEEEtypes_WMM_TSPEC_t tspecIE;

    /* Place holder for additional elements after the TSPEC */
    t_u8 subElem[256];

} MLAN_PACK_END IEEEtypes_Action_WMM_AddTsReq_t;

typedef MLAN_PACK_START struct
{
    IEEEtypes_WMM_Tspec_Action_Base_Tspec_t tspecAct;
    t_u8 statusCode;
    IEEEtypes_WMM_TSPEC_t tspecIE;

    /* Place holder for additional elements after the TSPEC */
    t_u8 subElem[256];

} MLAN_PACK_END IEEEtypes_Action_WMM_AddTsRsp_t;

typedef MLAN_PACK_START struct
{
    IEEEtypes_WMM_Tspec_Action_Base_Tspec_t tspecAct;
    t_u8 reasonCode;
    IEEEtypes_WMM_TSPEC_t tspecIE;

} MLAN_PACK_END IEEEtypes_Action_WMM_DelTs_t;

typedef MLAN_PACK_START union
{
    IEEEtypes_WMM_Tspec_Action_Base_Tspec_t tspecAct;

    IEEEtypes_Action_WMM_AddTsReq_t addTsReq;
    IEEEtypes_Action_WMM_AddTsRsp_t addTsRsp;
    IEEEtypes_Action_WMM_DelTs_t delTs;

} MLAN_PACK_END IEEEtypes_Action_WMMAC_t;

typedef MLAN_PACK_START union
{
    IEEEtypes_ActionCategory_e category;

    IEEEtypes_Action_WMMAC_t wmmAc;

} MLAN_PACK_END IEEEtypes_ActionFrame_t;

#ifdef STA_SUPPORT

/** Maximum subbands for 11d */
#define MRVDRV_MAX_SUBBAND_802_11D              83
/** Country code length */
#define COUNTRY_CODE_LEN                        3

/** Data structure for subband set */
typedef MLAN_PACK_START struct _IEEEtypes_SubbandSet_t
{
    /** First channel */
    t_u8 first_chan;
    /** Number of channels */
    t_u8 no_of_chan;
    /** Maximum Tx power */
    t_u8 max_tx_pwr;
} MLAN_PACK_END IEEEtypes_SubbandSet_t, *pIEEEtypes_SubbandSet_t;

/** Data structure for Country IE */
typedef MLAN_PACK_START struct _IEEEtypes_CountryInfoSet_t
{
    /** Element ID */
    t_u8 element_id;
    /** Length */
    t_u8 len;
    /** Country code */
    t_u8 country_code[COUNTRY_CODE_LEN];
    /** Set of subbands */
    IEEEtypes_SubbandSet_t sub_band[1];
} MLAN_PACK_END IEEEtypes_CountryInfoSet_t, *pIEEEtypes_CountryInfoSet_t;

/** Data structure for Country IE full set */
typedef MLAN_PACK_START struct _IEEEtypes_CountryInfoFullSet_t
{
    /** Element ID */
    t_u8 element_id;
    /** Length */
    t_u8 len;
    /** Country code */
    t_u8 country_code[COUNTRY_CODE_LEN];
    /** Set of subbands */
    IEEEtypes_SubbandSet_t sub_band[MRVDRV_MAX_SUBBAND_802_11D];
} MLAN_PACK_END IEEEtypes_CountryInfoFullSet_t,
    *pIEEEtypes_CountryInfoFullSet_t;

#endif /* STA_SUPPORT */

/** HT Capabilities Data */
typedef struct MLAN_PACK_START _HTCap_t
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
} MLAN_PACK_END HTCap_t, *pHTCap_t;

/** HT Information Data */
typedef struct MLAN_PACK_START _HTInfo_t
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
} MLAN_PACK_END HTInfo_t, *pHTInfo_t;

/** 20/40 BSS Coexistence Data */
typedef struct MLAN_PACK_START _BSSCo2040_t
{
    /** 20/40 BSS Coexistence value */
    t_u8 bss_co_2040_value;
} MLAN_PACK_END BSSCo2040_t, *pBSSCo2040_t;

/** Extended Capabilities Data */
typedef struct MLAN_PACK_START _ExtCap_t
{
    /** Extended Capabilities value */
    t_u8 ext_cap_value;
} MLAN_PACK_END ExtCap_t, *pExtCap_t;

/** Overlapping BSS Scan Parameters Data */
typedef struct MLAN_PACK_START _OverlapBSSScanParam_t
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
} MLAN_PACK_END OBSSScanParam_t, *pOBSSScanParam_t;

/** HT Capabilities IE */
typedef MLAN_PACK_START struct _IEEEtypes_HTCap_t
{
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    /** HTCap struct */
    HTCap_t ht_cap;
} MLAN_PACK_END IEEEtypes_HTCap_t, *pIEEEtypes_HTCap_t;

/** HT Information IE */
typedef MLAN_PACK_START struct _IEEEtypes_HTInfo_t
{
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    /** HTInfo struct */
    HTInfo_t ht_info;
} MLAN_PACK_END IEEEtypes_HTInfo_t, *pIEEEtypes_HTInfo_t;

/** 20/40 BSS Coexistence IE */
typedef MLAN_PACK_START struct _IEEEtypes_2040BSSCo_t
{
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    /** BSSCo2040_t struct */
    BSSCo2040_t bss_co_2040;
} MLAN_PACK_END IEEEtypes_2040BSSCo_t, *pIEEEtypes_2040BSSCo_t;

/** Extended Capabilities IE */
typedef MLAN_PACK_START struct _IEEEtypes_ExtCap_t
{
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    /** ExtCap_t struct */
    ExtCap_t ext_cap;
} MLAN_PACK_END IEEEtypes_ExtCap_t, *pIEEEtypes_ExtCap_t;

/** Overlapping BSS Scan Parameters IE */
typedef MLAN_PACK_START struct _IEEEtypes_OverlapBSSScanParam_t
{
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    /** OBSSScanParam_t struct */
    OBSSScanParam_t obss_scan_param;
} MLAN_PACK_END IEEEtypes_OverlapBSSScanParam_t,
    *pIEEEtypes_OverlapBSSScanParam_t;

/** Maximum number of subbands in the IEEEtypes_SupportedChannels_t structure */
#define WLAN_11H_MAX_SUBBANDS  5

/** Maximum number of DFS channels configured in IEEEtypes_IBSS_DFS_t */
#define WLAN_11H_MAX_IBSS_DFS_CHANNELS 25

/**  IEEE Power Constraint element (7.3.2.15) */
typedef MLAN_PACK_START struct
{
    t_u8 element_id;        /**< IEEE Element ID = 32 */
    t_u8 len;               /**< Element length after id and len */
    t_u8 local_constraint;  /**< Local power constraint applied to 11d chan info */
} MLAN_PACK_END IEEEtypes_PowerConstraint_t;

/**  IEEE Power Capability element (7.3.2.16) */
typedef MLAN_PACK_START struct
{
    t_u8 element_id;                /**< IEEE Element ID = 33 */
    t_u8 len;                       /**< Element length after id and len */
    t_s8 min_tx_power_capability;   /**< Minimum Transmit power (dBm) */
    t_s8 max_tx_power_capability;   /**< Maximum Transmit power (dBm) */
} MLAN_PACK_END IEEEtypes_PowerCapability_t;

/**  IEEE TPC Report element (7.3.2.18) */
typedef MLAN_PACK_START struct
{
    t_u8 element_id;    /**< IEEE Element ID = 35 */
    t_u8 len;           /**< Element length after id and len */
    t_s8 tx_power;      /**< Max power used to transmit the TPC Report frame (dBm) */
    t_s8 link_margin;   /**< Link margin when TPC Request received (dB) */
} MLAN_PACK_END IEEEtypes_TPCReport_t;

/*  IEEE Supported Channel sub-band description (7.3.2.19) */
/**  
 *  Sub-band description used in the supported channels element. 
 */
typedef MLAN_PACK_START struct
{
    t_u8 start_chan;    /**< Starting channel in the subband */
    t_u8 num_chans;     /**< Number of channels in the subband */

} MLAN_PACK_END IEEEtypes_SupportChan_Subband_t;

/*  IEEE Supported Channel element (7.3.2.19) */
/**
 *  Sent in association requests. Details the sub-bands and number
 *    of channels supported in each subband
 */
typedef MLAN_PACK_START struct
{
    t_u8 element_id;    /**< IEEE Element ID = 36 */
    t_u8 len;           /**< Element length after id and len */

    /** Configured sub-bands information in the element */
    IEEEtypes_SupportChan_Subband_t subband[WLAN_11H_MAX_SUBBANDS];

} MLAN_PACK_END IEEEtypes_SupportedChannels_t;

/*  IEEE Quiet Period Element (7.3.2.23) */
/**
 *  Provided in beacons and probe responses.  Indicates times during 
 *    which the STA should not be transmitting data.  Only starting STAs in
 *    an IBSS and APs are allowed to originate a quiet element. 
 */
typedef MLAN_PACK_START struct
{
    t_u8 element_id;        /**< IEEE Element ID = 40 */
    t_u8 len;               /**< Element length after id and len */
    t_u8 quiet_count;       /**< Number of TBTTs until beacon with the quiet period */
    t_u8 quiet_period;      /**< Regular quiet period, # of TBTTS between periods */
    t_u16 quiet_duration;   /**< Duration of the quiet period in TUs */
    t_u16 quiet_offset;     /**< Offset in TUs from the TBTT for the quiet period */

} MLAN_PACK_END IEEEtypes_Quiet_t;

/**
***  @brief Map octet of the basic measurement report (7.3.2.22.1)
**/
typedef MLAN_PACK_START struct
{
#ifdef BIG_ENDIAN_SUPPORT
    t_u8 rsvd5_7:3;                /**< Reserved */
    t_u8 unmeasured:1;             /**< Channel is unmeasured */
    t_u8 radar:1;                  /**< Radar detected on channel */
    t_u8 unidentified_sig:1;       /**< Unidentified signal found on channel */
    t_u8 ofdm_preamble:1;          /**< OFDM preamble detected on channel */
    t_u8 bss:1;                    /**< At least one valid MPDU received on channel */
#else
    t_u8 bss:1;                    /**< At least one valid MPDU received on channel */
    t_u8 ofdm_preamble:1;          /**< OFDM preamble detected on channel */
    t_u8 unidentified_sig:1;       /**< Unidentified signal found on channel */
    t_u8 radar:1;                  /**< Radar detected on channel */
    t_u8 unmeasured:1;             /**< Channel is unmeasured */
    t_u8 rsvd5_7:3;                /**< Reserved */
#endif                          /* BIG_ENDIAN_SUPPORT */

} MLAN_PACK_END MeasRptBasicMap_t;

/*  IEEE DFS Channel Map field (7.3.2.24) */
/**
 *  Used to list supported channels and provide a octet "map" field which 
 *    contains a basic measurement report for that channel in the 
 *    IEEEtypes_IBSS_DFS_t element
 */
typedef MLAN_PACK_START struct
{
    t_u8 channel_number;        /**< Channel number */
    MeasRptBasicMap_t rpt_map;  /**< Basic measurement report for the channel */

} MLAN_PACK_END IEEEtypes_ChannelMap_t;

/*  IEEE IBSS DFS Element (7.3.2.24) */
/**
 *  IBSS DFS element included in ad hoc beacons and probe responses.  
 *    Provides information regarding the IBSS DFS Owner as well as the
 *    originating STAs supported channels and basic measurement results.
 */
typedef MLAN_PACK_START struct
{
    t_u8 element_id;                        /**< IEEE Element ID = 41 */
    t_u8 len;                               /**< Element length after id and len */
    t_u8 dfs_owner[MLAN_MAC_ADDR_LENGTH];   /**< DFS Owner STA Address */
    t_u8 dfs_recovery_interval;             /**< DFS Recovery time in TBTTs */

    /** Variable length map field, one Map entry for each supported channel */
    IEEEtypes_ChannelMap_t channel_map[WLAN_11H_MAX_IBSS_DFS_CHANNELS];

} MLAN_PACK_END IEEEtypes_IBSS_DFS_t;

/* 802.11h BSS information kept for each BSSID received in scan results */
/**
 * IEEE BSS information needed from scan results for later processing in
 *    join commands 
 */
typedef struct
{
    t_u8 sensed_11h;  /**< Capability bit set or 11h IE found in this BSS */

    IEEEtypes_PowerConstraint_t power_constraint; /**< Power Constraint IE */
    IEEEtypes_PowerCapability_t power_capability; /**< Power Capability IE */
    IEEEtypes_TPCReport_t tpc_report;             /**< TPC Report IE */
    IEEEtypes_Quiet_t quiet;                      /**< Quiet IE */
    IEEEtypes_IBSS_DFS_t ibss_dfs;                /**< IBSS DFS Element IE */

} wlan_11h_bss_info_t;

#ifdef PRAGMA_PACK
#pragma pack(pop)
#endif

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

    /** 802.11h BSS information */
    wlan_11h_bss_info_t wlan_11h_bss_info;

    /** Indicate disabling 11n when associate with AP */
    t_u8 disable_11n;
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

#ifdef STA_SUPPORT
    /** Country information set */
    IEEEtypes_CountryInfoFullSet_t country_info;
#endif                          /* STA_SUPPORT */

    /** WPA IE */
    IEEEtypes_VendorSpecific_t *pwpa_ie;
    /** WPA IE offset in the beacon buffer */
    t_u16 wpa_offset;
    /** RSN IE */
    IEEEtypes_Generic_t *prsn_ie;
    /** RSN IE offset in the beacon buffer */
    t_u16 rsn_offset;
#ifdef STA_SUPPORT
    /** WAPI IE */
    IEEEtypes_Generic_t *pwapi_ie;
    /** WAPI IE offset in the beacon buffer */
    t_u16 wapi_offset;
#endif

    /** Pointer to the returned scan response */
    t_u8 *pbeacon_buf;
    /** Length of the stored scan response */
    t_u32 beacon_buf_size;
    /** Max allocated size for updated scan response */
    t_u32 beacon_buf_size_max;

} BSSDescriptor_t, *pBSSDescriptor_t;

#endif /* !_MLAN_IEEE_H_ */
