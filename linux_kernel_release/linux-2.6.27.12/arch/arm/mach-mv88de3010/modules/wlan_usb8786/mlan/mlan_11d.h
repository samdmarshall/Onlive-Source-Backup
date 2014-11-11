/** @file mlan_11d.h
 *
 *  @brief This header file contains data structures and 
 *  function declarations of 802.11d  
 *
 *  Copyright (C) 2008-2009, Marvell International Ltd. 
 *  All Rights Reserved
 */

/******************************************************
Change log:
    10/29/2008: initial version
******************************************************/

#ifndef _MLAN_11D_H_
#define _MLAN_11D_H_

/** Universal region code */
#define UNIVERSAL_REGION_CODE   0xff

/** (Beaconsize(256)-5(IEId,len,contrystr(3))/3(FirstChan,NoOfChan,MaxPwr) */
#define MAX_NO_OF_CHAN          40

/** State of 11d */
typedef enum
{
    DISABLE_11D = 0,
    ENABLE_11D = 1,
} state_11d_t;

/** Domain regulatory information */
typedef struct _wlan_802_11d_domain_reg
{
    /** Country Code */
    t_u8 country_code[COUNTRY_CODE_LEN];
    /** No. of subband */
    t_u8 no_of_sub_band;
    /** Subband data */
    IEEEtypes_SubbandSet_t sub_band[MRVDRV_MAX_SUBBAND_802_11D];
} wlan_802_11d_domain_reg_t;

/** Channel-power table entries */
typedef struct _chan_power_11d
{
    /** 11D channel */
    t_u8 chan;
    /** 11D channel power */
    t_u8 pwr;
} __attribute__ ((packed)) chan_power_11d_t;

/** Region channel info */
typedef struct _parsed_region_chan_11d
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
} __attribute__ ((packed)) parsed_region_chan_11d_t;

/** Data for state machine */
typedef struct _wlan_802_11d_state
{
    /** True for enabling 11D */
    state_11d_t enable_11d;
    /** True for user enabling 11D */
    state_11d_t user_enable_11d;
} wlan_802_11d_state_t;

/** Region code mapping */
typedef struct _region_code_mapping
{
    /** Region */
    t_u8 region[COUNTRY_CODE_LEN];
    /** Code */
    t_u8 code;
} region_code_mapping_t;

#endif /* !_MLAN_11D_H_ */
