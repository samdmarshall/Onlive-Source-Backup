/** @file mlan_11d.c
 *
 *  @brief This file contains functions for 802.11D.
 * 
 *  Copyright (C) 2008-2009, Marvell International Ltd. 
 *  All Rights Reserved
 */
/********************************************************
Change log:
    10/21/2008: initial version
********************************************************/

#include "mlan.h"
#include "mlan_11d.h"
#include "mlan_join.h"
#include "mlan_scan.h"
#include "mlan_util.h"
#include "mlan_fw.h"
#include "mlan_main.h"

/********************************************************
                Local Variables
********************************************************/

/** Default Tx power */
#define TX_PWR_DEFAULT  10

/** Region code mapping table */
static region_code_mapping_t region_code_mapping[] = {
    {"US ", 0x10},              /* US FCC */
    {"CA ", 0x20},              /* IC Canada */
    {"SG ", 0x10},              /* Singapore */
    {"EU ", 0x30},              /* ETSI */
    {"AU ", 0x30},              /* Australia */
    {"KR ", 0x30},              /* Republic Of Korea */
    {"FR ", 0x32},              /* France */
    {"JP ", 0x40},              /* Japan */
    {"JP ", 0x41},              /* Japan */
    {"JP ", 0xFF},              /* Japan special */
};

/* Following two structures define the supported channels */
/** Channels for 802.11b/g */
static chan_freq_power_t channel_freq_power_UN_BG[] = {
    {1, 2412, TX_PWR_DEFAULT},
    {2, 2417, TX_PWR_DEFAULT},
    {3, 2422, TX_PWR_DEFAULT},
    {4, 2427, TX_PWR_DEFAULT},
    {5, 2432, TX_PWR_DEFAULT},
    {6, 2437, TX_PWR_DEFAULT},
    {7, 2442, TX_PWR_DEFAULT},
    {8, 2447, TX_PWR_DEFAULT},
    {9, 2452, TX_PWR_DEFAULT},
    {10, 2457, TX_PWR_DEFAULT},
    {11, 2462, TX_PWR_DEFAULT},
    {12, 2467, TX_PWR_DEFAULT},
    {13, 2472, TX_PWR_DEFAULT},
    {14, 2484, TX_PWR_DEFAULT}
};

/********************************************************
                Global Variables
********************************************************/

/********************************************************
                Local Functions
********************************************************/
/** 
 *  @brief This function converts a lowercase character to uppercase
 *
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param c            Input character
 *
 *  @return     Corresponding uppercase character
 */
static t_u8
wlan_11d_lower_to_upper(pmlan_adapter pmadapter, t_u8 c)
{
    t_u8 upper;

    ENTER();

    if (c >= 'a' && c <= 'z')
        upper = c - 0x20;
    else
        upper = c;

    LEAVE();
    return upper;
}

/** 
 *  @brief This function convert region string to code integer
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param region   	Region string
 *
 *  @return		Region id
 */
static t_u8
wlan_11d_region_2_code(pmlan_adapter pmadapter, t_u8 * region)
{
    t_u8 i;
    t_u8 size = sizeof(region_code_mapping) / sizeof(region_code_mapping_t);

    ENTER();

    for (i = 0; i < COUNTRY_CODE_LEN && region[i]; i++)
        region[i] = wlan_11d_lower_to_upper(pmadapter, region[i]);

    /* Look for region in mapping table */
    for (i = 0; i < size; i++) {
        if (!memcmp(region, region_code_mapping[i].region, COUNTRY_CODE_LEN)) {
            LEAVE();
            return (region_code_mapping[i].code);
        }
    }

    LEAVE();
    /* Default is US */
    return (region_code_mapping[0].code);
}

/** 
 *  @brief This function converts interger code to region string
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param code     	Region code
 *
 *  @return		Region string
 */
static t_u8 *
wlan_11d_code_2_region(pmlan_adapter pmadapter, t_u8 code)
{
    t_u8 i;
    t_u8 size = sizeof(region_code_mapping) / sizeof(region_code_mapping_t);

    ENTER();

    /* Look for code in mapping table */
    for (i = 0; i < size; i++) {
        if (region_code_mapping[i].code == code) {
            LEAVE();
            return (region_code_mapping[i].region);
        }
    }

    LEAVE();
    /* Default is US */
    return (region_code_mapping[0].region);
}

/** 
 *  @brief This function Checks if chan txpwr is learned from AP/IBSS
 *
 *  @param pmadapter            A pointer to mlan_adapter structure
 *  @param chan                 Channel number
 *  @param parsed_region_chan   Pointer to parsed_region_chan_11d_t     
 *
 *  @return                     MTRUE or MFALSE
 */
static t_u8
wlan_11d_channel_known(pmlan_adapter pmadapter,
                       t_u8 chan, parsed_region_chan_11d_t * parsed_region_chan)
{
    chan_power_11d_t *pchan_pwr = parsed_region_chan->chan_pwr;
    t_u8 no_of_chan = parsed_region_chan->no_of_chan;
    t_u8 i = 0;

    ENTER();

    HEXDUMP("11D: parsed_region_chan", (t_u8 *) pchan_pwr,
            sizeof(chan_power_11d_t) * no_of_chan);

    /* Search channel */
    for (i = 0; i < no_of_chan; i++) {
        if (chan == pchan_pwr[i].chan) {
            PRINTM(INFO, "11D: Found chan:%d\n", chan);
            LEAVE();
            return MTRUE;
        }
    }

    PRINTM(ERROR, "11D: Could not find chan:%d\n", chan);
    LEAVE();
    return MFALSE;
}

/** 
 *  @brief This function sets domain info to FW
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status
wlan_11d_set_domain_info(mlan_private * pmpriv)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;

    ENTER();

    /* Send cmd to FW to set domain info */
    ret = wlan_prepare_cmd(pmpriv,
                           HostCmd_CMD_802_11D_DOMAIN_INFO,
                           HostCmd_ACT_GEN_SET, 0, MNULL, MNULL);
    if (ret)
        PRINTM(ERROR, "11D: Failed to download domain Info\n");

    LEAVE();
    return ret;
}

/** 
 *  @brief This function generates parsed_region_chan from Domain Info
 *           learned from AP/IBSS
 *
 *  @param pmadapter            Pointer to mlan_adapter structure
 *  @param region_chan          Pointer to region_chan_t
 *  @param parsed_region_chan   Pointer to parsed_region_chan_11d_t
 *
 *  @return                     N/A
 */
static t_void
wlan_11d_generate_parsed_region_chan(pmlan_adapter pmadapter,
                                     region_chan_t * region_chan,
                                     parsed_region_chan_11d_t *
                                     parsed_region_chan)
{
    chan_freq_power_t *cfp;
    t_u8 i;

    ENTER();

    /* Region channel must be provided */
    if (!region_chan) {
        PRINTM(INFO, "11D: region_chan is MNULL\n");
        LEAVE();
        return;
    }

    /* Get channel-frequecy-power trio */
    cfp = region_chan->pcfp;
    if (!cfp) {
        PRINTM(INFO, "11D: cfp equal MNULL \n");
        LEAVE();
        return;
    }

    /* Set band, region and country code */
    parsed_region_chan->band = region_chan->band;
    parsed_region_chan->region = region_chan->region;
    memcpy(parsed_region_chan->country_code,
           wlan_11d_code_2_region(pmadapter, region_chan->region),
           COUNTRY_CODE_LEN);

    PRINTM(INFO, "11D: region[0x%x] band[%d]\n", parsed_region_chan->region,
           parsed_region_chan->band);

    /* Set channel and power */
    for (i = 0; i < region_chan->num_cfp; i++, cfp++) {
        parsed_region_chan->chan_pwr[i].chan = cfp->channel;
        parsed_region_chan->chan_pwr[i].pwr = cfp->max_tx_power;
        PRINTM(INFO, "11D: Chan[%d] Pwr[%d]\n",
               parsed_region_chan->chan_pwr[i].chan,
               parsed_region_chan->chan_pwr[i].pwr);
    }
    parsed_region_chan->no_of_chan = region_chan->num_cfp;

    PRINTM(INFO, "11D: no_of_chan[%d]\n", parsed_region_chan->no_of_chan);

    LEAVE();
    return;
}

/** 
 *  @brief This function generates domain_info from parsed_region_chan
 *
 *  @param pmadapter            Pointer to mlan_adapter structure
 *  @param parsed_region_chan   Pointer to parsed_region_chan_11d_t
 *  @param domain_info          Pointer to wlan_802_11d_domain_reg_t
 *
 *  @return                     MLAN_STATUS_SUCCESS
 */
static mlan_status
wlan_11d_generate_domain_info(pmlan_adapter pmadapter,
                              parsed_region_chan_11d_t * parsed_region_chan,
                              wlan_802_11d_domain_reg_t * domain_info)
{
    t_u8 no_of_sub_band = 0;
    t_u8 no_of_chan = parsed_region_chan->no_of_chan;
    t_u8 no_of_parsed_chan = 0;
    t_u8 first_chan = 0, next_chan = 0, max_pwr = 0;
    t_u8 i, flag = 0;

    ENTER();

    /* Set country code */
    memcpy(domain_info->country_code,
           parsed_region_chan->country_code, COUNTRY_CODE_LEN);

    PRINTM(INFO, "11D: no_of_chan=%d\n", no_of_chan);
    HEXDUMP("11D: parsed_region_chan", (t_u8 *) parsed_region_chan,
            sizeof(parsed_region_chan_11d_t));

    /* Set channel and power */
    for (i = 0; i < no_of_chan; i++) {
        if (!flag) {
            flag = 1;
            next_chan = first_chan = parsed_region_chan->chan_pwr[i].chan;
            max_pwr = parsed_region_chan->chan_pwr[i].pwr;
            no_of_parsed_chan = 1;
            continue;
        }

        if (parsed_region_chan->chan_pwr[i].chan == next_chan + 1 &&
            parsed_region_chan->chan_pwr[i].pwr == max_pwr) {
            next_chan++;
            no_of_parsed_chan++;
        } else {
            domain_info->sub_band[no_of_sub_band].first_chan = first_chan;
            domain_info->sub_band[no_of_sub_band].no_of_chan =
                no_of_parsed_chan;
            domain_info->sub_band[no_of_sub_band].max_tx_pwr = max_pwr;
            no_of_sub_band++;
            no_of_parsed_chan = 1;
            next_chan = first_chan = parsed_region_chan->chan_pwr[i].chan;
            max_pwr = parsed_region_chan->chan_pwr[i].pwr;
        }
    }

    if (flag) {
        domain_info->sub_band[no_of_sub_band].first_chan = first_chan;
        domain_info->sub_band[no_of_sub_band].no_of_chan = no_of_parsed_chan;
        domain_info->sub_band[no_of_sub_band].max_tx_pwr = max_pwr;
        no_of_sub_band++;
    }
    domain_info->no_of_sub_band = no_of_sub_band;

    PRINTM(INFO, "11D: no_of_sub_band=0x%x\n", domain_info->no_of_sub_band);
    HEXDUMP("11D: domain_info", (t_u8 *) domain_info,
            COUNTRY_CODE_LEN + 1 +
            sizeof(IEEEtypes_SubbandSet_t) * no_of_sub_band);
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function updates the channel power table with the channel 
 *            present in BSSDescriptor.
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param pbss_desc    A pointer to BSSDescriptor_t
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status
wlan_11d_update_chan_pwr_table(mlan_private * pmpriv,
                               BSSDescriptor_t * pbss_desc)
{
    mlan_adapter *pmadapter = pmpriv->adapter;
    parsed_region_chan_11d_t *parsed_region_chan =
        &pmadapter->parsed_region_chan;
    t_u16 i;
    t_u8 tx_power = 0;
    t_u8 chan;

    ENTER();

    chan = pbss_desc->phy_param_set.ds_param_set.current_chan;

    tx_power = wlan_get_txpwr_of_chan_from_cfp(pmpriv, chan);

    if (!tx_power) {
        PRINTM(MSG, "11D: Invalid channel\n");
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }

    /* Check whether the channel already exists in channel power table of
       parsed region */
    for (i = 0; ((i < parsed_region_chan->no_of_chan) &&
                 (i < MAX_NO_OF_CHAN)); i++) {
        if (parsed_region_chan->chan_pwr[i].chan == chan) {
            /* Channel already exists update the tx_power */
            parsed_region_chan->chan_pwr[i].pwr =
                MIN(parsed_region_chan->chan_pwr[i].pwr, tx_power);
            break;
        }
    }

    if (i == parsed_region_chan->no_of_chan && i < MAX_NO_OF_CHAN) {
        /* Channel not found. Update the channel in the channel-power table */
        parsed_region_chan->chan_pwr[i].chan = chan;
        parsed_region_chan->chan_pwr[i].pwr = tx_power;
        parsed_region_chan->no_of_chan++;
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function finds the no_of_chan-th chan after the first_chan
 *
 *  @param pmadapter  A pointer to mlan_adapter structure
 *  @param band       Band
 *  @param first_chan First channel number
 *  @param no_of_chan Number of channels
 *  @param chan       Pointer to the returned no_of_chan-th chan number
 *
 *  @return           MTRUE or MFALSE
 */
static t_u8
wlan_11d_get_chan(pmlan_adapter pmadapter, t_u8 band, t_u8 first_chan,
                  t_u8 no_of_chan, t_u8 * chan)
{
    chan_freq_power_t *cfp = MNULL;
    t_u8 i;
    t_u8 cfp_no = 0;

    ENTER();
    if (band & (BAND_B | BAND_G | BAND_GN)) {
        cfp = channel_freq_power_UN_BG;
        cfp_no = sizeof(channel_freq_power_UN_BG) / sizeof(chan_freq_power_t);
    } else {
        PRINTM(ERROR, "11D: Wrong Band[%d]\n", band);
        LEAVE();
        return MFALSE;
    }
    /* Locate the first_chan */
    for (i = 0; i < cfp_no; i++) {
        if (cfp && ((cfp + i)->channel == first_chan)) {
            PRINTM(INFO, "11D: first_chan found\n");
            break;
        }
    }

    if (i < cfp_no) {
        /* Check if beyond the boundary */
        if (i + no_of_chan < cfp_no) {
            /* Get first_chan + no_of_chan */
            *chan = (cfp + i + no_of_chan)->channel;
            LEAVE();
            return MTRUE;
        }
    }

    LEAVE();
    return MFALSE;
}

/** 
 *  @brief This function parses country information for region channel
 *
 *  @param pmadapter            Pointer to mlan_adapter structure
 *  @param country_info         Country information
 *  @param band                 Chan band
 *  @param parsed_region_chan   Pointer to parsed_region_chan_11d_t     
 *
 *  @return                     MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_11d_parse_domain_info(pmlan_adapter pmadapter,
                           IEEEtypes_CountryInfoFullSet_t * country_info,
                           t_u8 band,
                           parsed_region_chan_11d_t * parsed_region_chan)
{
    t_u8 no_of_sub_band, no_of_chan;
    t_u8 last_chan, first_chan, cur_chan = 0;
    t_u8 idx = 0;
    t_u8 j, i;

    ENTER();

    /* 
     * Validation Rules:
     *    1. Valid Region Code
     *    2. First Chan increment
     *    3. Channel range no overlap
     *    4. Channel is valid?
     *    5. Channel is supported by Region?
     *    6. Others
     */

    HEXDUMP("country_info", (t_u8 *) country_info, 30);

    if (!(*(country_info->country_code)) ||
        (country_info->len <= COUNTRY_CODE_LEN)) {
        /* No region info or wrong region info: treat as no 11D info */
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }

    /* Step 1: Check region_code */
    parsed_region_chan->region =
        wlan_11d_region_2_code(pmadapter, country_info->country_code);

    PRINTM(INFO, "11D: region code=0x%x\n", (t_u8) parsed_region_chan->region);
    HEXDUMP("11D: Country Code", (t_u8 *) country_info->country_code,
            COUNTRY_CODE_LEN);

    parsed_region_chan->band = band;

    memcpy(parsed_region_chan->country_code,
           country_info->country_code, COUNTRY_CODE_LEN);

    no_of_sub_band = (country_info->len - COUNTRY_CODE_LEN) /
        sizeof(IEEEtypes_SubbandSet_t);

    for (j = 0, last_chan = 0; j < no_of_sub_band; j++) {

        if (country_info->sub_band[j].first_chan <= last_chan) {
            /* Step2&3: Check First Chan Num increment and no overlap */
            PRINTM(INFO, "11D: Chan[%d>%d] Overlap\n",
                   country_info->sub_band[j].first_chan, last_chan);
            continue;
        }

        first_chan = country_info->sub_band[j].first_chan;
        no_of_chan = country_info->sub_band[j].no_of_chan;

        for (i = 0; idx < MAX_NO_OF_CHAN && i < no_of_chan; i++) {
            /* Step 4 : Channel is supported? */
            if (wlan_11d_get_chan(pmadapter, band, first_chan, i, &cur_chan) ==
                MFALSE) {
                /* Chan is not found in UN table */
                PRINTM(WARN, "11D: chan is not supported: %d\n", i);
                break;
            }

            last_chan = cur_chan;

            /* Step 5: We don't need to check if cur_chan is supported by mrvl
               in region */
            parsed_region_chan->chan_pwr[idx].chan = cur_chan;
            parsed_region_chan->chan_pwr[idx].pwr =
                country_info->sub_band[j].max_tx_pwr;
            idx++;
        }

        /* Step 6: Add other checking if any */
    }

    parsed_region_chan->no_of_chan = idx;

    PRINTM(INFO, "11D: no_of_chan=0x%x\n", parsed_region_chan->no_of_chan);
    HEXDUMP("11D: parsed_region_chan", (t_u8 *) parsed_region_chan,
            2 + COUNTRY_CODE_LEN + sizeof(parsed_region_chan_11d_t) * idx);

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function processes the country info present in BSSDescriptor.
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param pbss_desc     A pointer to BSSDescriptor_t
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status
wlan_11d_process_country_info(mlan_private * pmpriv,
                              BSSDescriptor_t * pbss_desc)
{
    mlan_adapter *pmadapter = pmpriv->adapter;
    parsed_region_chan_11d_t region_chan;
    parsed_region_chan_11d_t *parsed_region_chan =
        &pmadapter->parsed_region_chan;
    t_u16 i, j, num_chan_added = 0;

    ENTER();

    memset(&region_chan, 0, sizeof(parsed_region_chan_11d_t));

    /* Parse 11D country info */
    wlan_11d_parse_domain_info(pmadapter, &pbss_desc->country_info,
                               pbss_desc->bss_band, &region_chan);

    if (parsed_region_chan->no_of_chan != 0) {
        /* 
         * Check if the channel number already exists in the 
         * chan-power table of parsed_region_chan
         */
        for (i = 0; (i < region_chan.no_of_chan && i < MAX_NO_OF_CHAN); i++) {
            for (j = 0; (j < parsed_region_chan->no_of_chan &&
                         j < MAX_NO_OF_CHAN); j++) {
                /* 
                 * Channel already exists, update the tx power with minimum 
                 * value among existing tx_power and new tx power
                 */
                if (region_chan.chan_pwr[i].chan ==
                    parsed_region_chan->chan_pwr[j].chan) {
                    parsed_region_chan->chan_pwr[j].pwr =
                        MIN(parsed_region_chan->chan_pwr[j].pwr,
                            region_chan.chan_pwr[i].pwr);
                    break;
                }
            }

            if (j == parsed_region_chan->no_of_chan && j < MAX_NO_OF_CHAN) {
                /* 
                 * Channel does not exist in the channel power table, 
                 * update this new chan and tx_power to the channel power table
                 */
                parsed_region_chan->chan_pwr[parsed_region_chan->no_of_chan +
                                             num_chan_added].chan =
                    region_chan.chan_pwr[i].chan;
                parsed_region_chan->chan_pwr[parsed_region_chan->no_of_chan +
                                             num_chan_added].pwr =
                    region_chan.chan_pwr[i].pwr;
                num_chan_added++;
            }
        }
        parsed_region_chan->no_of_chan += num_chan_added;
    } else {
        /* Parsed region is empty, copy the first one */
        memcpy(parsed_region_chan,
               &region_chan, sizeof(parsed_region_chan_11d_t));
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/********************************************************
                Global functions
********************************************************/

/** 
 *  @brief This function converts channel to frequency
 *
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param chan         Channel number
 *  @param band         Band
 *
 *  @return             Channel frequency
 */
t_u32
wlan_11d_chan_2_freq(pmlan_adapter pmadapter, t_u8 chan, t_u8 band)
{
    chan_freq_power_t *cf;
    t_u16 cnt;
    t_u16 i;
    t_u32 freq = 0;

    ENTER();

    /* Get channel-frequency-power trios */
    {
        cf = channel_freq_power_UN_BG;
        cnt = sizeof(channel_freq_power_UN_BG) / sizeof(chan_freq_power_t);
    }

    /* Locate channel and return corresponding frequency */
    for (i = 0; i < cnt; i++) {
        if (chan == cf[i].channel)
            freq = cf[i].freq;
    }

    LEAVE();
    return freq;
}

/** 
 *  @brief This function setups scan channels
 *
 *  @param pmpriv       Pointer to mlan_private structure
 *  @param band         Band
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status
wlan_11d_set_universaltable(mlan_private * pmpriv, t_u8 band)
{
    mlan_adapter *pmadapter = pmpriv->adapter;
    t_u16 size = sizeof(chan_freq_power_t);
    t_u16 i = 0;

    ENTER();

    memset(pmadapter->universal_channel, 0,
           sizeof(pmadapter->universal_channel));

    if (band & (BAND_B | BAND_G | BAND_GN))
        /* If band B, G or N */
    {
        /* Set channel-frequency-power */
        pmadapter->universal_channel[i].num_cfp =
            sizeof(channel_freq_power_UN_BG) / size;
        PRINTM(INFO, "11D: BG-band num_cfp=%d\n",
               pmadapter->universal_channel[i].num_cfp);

        pmadapter->universal_channel[i].pcfp = channel_freq_power_UN_BG;
        pmadapter->universal_channel[i].valid = MTRUE;

        /* Set region code */
        pmadapter->universal_channel[i].region = UNIVERSAL_REGION_CODE;

        /* Set band */
        if (band & BAND_GN)
            pmadapter->universal_channel[i].band = BAND_G;
        else
            pmadapter->universal_channel[i].band =
                (band & BAND_G) ? BAND_G : BAND_B;
        i++;
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function gets if 11D is enabled
 *
 *  @param pmpriv       Pointer to mlan_private structure
 *
 *  @return             ENABLE_11D or DISABLE_11D
 */
state_11d_t
wlan_11d_get_state(mlan_private * pmpriv)
{
    mlan_adapter *pmadapter = pmpriv->adapter;
    wlan_802_11d_state_t *state = &pmadapter->state_11d;
    ENTER();
    LEAVE();
    return (state->enable_11d);
}

/** 
 *  @brief This function calculates the scan type for channels
 *
 *  @param pmadapter            A pointer to mlan_adapter structure
 *  @param chan                 Chan number
 *  @param parsed_region_chan   Pointer to parsed_region_chan_11d_t     
 *
 *  @return                     PASSIVE if chan is unknown; ACTIVE if chan is known
 */
t_u8
wlan_11d_get_scan_type(pmlan_adapter pmadapter,
                       t_u8 chan, parsed_region_chan_11d_t * parsed_region_chan)
{
    t_u8 scan_type = HostCmd_SCAN_TYPE_PASSIVE;

    ENTER();

    if (wlan_11d_channel_known(pmadapter, chan, parsed_region_chan)) {
        /* Channel found */
        PRINTM(INFO, "11D: Channel found and doing Active Scan\n");
        scan_type = HostCmd_SCAN_TYPE_ACTIVE;
    } else
        PRINTM(INFO, "11D: Channel not found and doing Passive Scan\n");

    LEAVE();
    return scan_type;
}

/** 
 *  @brief Initialize internal variable for 11D
 *
 *  @param pmadapter    Pointer to mlan_adapter structure
 *
 *  @return             N/A
 */
t_void
wlan_11d_init(mlan_adapter * pmadapter)
{
    wlan_802_11d_state_t *state = &pmadapter->state_11d;

    ENTER();

    /* Start in disabled mode */
    state->enable_11d = DISABLE_11D;

    memset(&(pmadapter->parsed_region_chan), 0,
           sizeof(parsed_region_chan_11d_t));
    memset(&(pmadapter->universal_channel), 0, sizeof(region_chan_t));
    memset(&(pmadapter->domain_reg), 0, sizeof(wlan_802_11d_domain_reg_t));

    LEAVE();
    return;
}

/** 
 *  @brief This function enable/disable 11D
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param pioctl_buf   A pointer to MLAN IOCTl Request buffer
 *  @param flag         11D status
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_11d_enable(mlan_private * pmpriv, t_void * pioctl_buf, state_11d_t flag)
{
    mlan_adapter *pmadapter = pmpriv->adapter;
    wlan_802_11d_state_t *state = &pmadapter->state_11d;
    mlan_status ret = MLAN_STATUS_SUCCESS;
    state_11d_t enable = flag;

    ENTER();

    memset(&pmadapter->parsed_region_chan, 0, sizeof(parsed_region_chan_11d_t));

    /* Send cmd to FW to enable/disable 11D function */
    ret = wlan_prepare_cmd(pmpriv,
                           HostCmd_CMD_802_11_SNMP_MIB,
                           HostCmd_ACT_GEN_SET,
                           Dot11D_i, (t_void *) pioctl_buf, &enable);

    if (ret) {
        if (flag)
            PRINTM(ERROR, "11D: Failed to enable 11D\n");
        else
            PRINTM(ERROR, "11D: Failed to disable 11D\n");
    } else {
        state->enable_11d = flag;
    }

    LEAVE();
    return ret;
}

/** 
 *  @brief This function setups scan channels
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param band         band
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
int
wlan_set_universal_table(mlan_private * pmpriv, t_u8 band)
{
    mlan_adapter *pmadapter = pmpriv->adapter;
    t_u16 size = sizeof(chan_freq_power_t);
    t_u16 i = 0;

    ENTER();

    memset(pmadapter->universal_channel, 0,
           sizeof(pmadapter->universal_channel));

    if (band & (BAND_B | BAND_G | BAND_GN)) {
        pmadapter->universal_channel[i].num_cfp =
            sizeof(channel_freq_power_UN_BG) / size;
        PRINTM(INFO, "11D: BG-band num_cfp=%d\n",
               pmadapter->universal_channel[i].num_cfp);

        pmadapter->universal_channel[i].pcfp = channel_freq_power_UN_BG;
        pmadapter->universal_channel[i].valid = MTRUE;
        pmadapter->universal_channel[i].region = UNIVERSAL_REGION_CODE;
        if (band & BAND_GN)
            pmadapter->universal_channel[i].band = BAND_G;
        else
            pmadapter->universal_channel[i].band =
                (band & BAND_G) ? BAND_G : BAND_B;
        i++;
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function implements command CMD_802_11D_DOMAIN_INFO
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param pcmd         A pointer to HostCmd_DS_COMMAND structure of 
 *                        command buffer
 *  @param cmd_action   Command action 
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_cmd_802_11d_domain_info(mlan_private * pmpriv,
                             HostCmd_DS_COMMAND * pcmd, t_u16 cmd_action)
{
    mlan_adapter *pmadapter = pmpriv->adapter;
    HostCmd_DS_802_11D_DOMAIN_INFO *pdomain_info = &pcmd->params.domain_info;
    MrvlIEtypes_DomainParamSet_t *domain = &pdomain_info->domain;
    t_u8 no_of_sub_band = pmadapter->domain_reg.no_of_sub_band;

    ENTER();

    PRINTM(INFO, "11D: no_of_sub_band=0x%x\n", no_of_sub_band);

    pcmd->command = wlan_cpu_to_le16(HostCmd_CMD_802_11D_DOMAIN_INFO);
    pdomain_info->action = wlan_cpu_to_le16(cmd_action);
    if (cmd_action == HostCmd_ACT_GEN_GET) {
        /* Dump domain info */
        pcmd->size = wlan_cpu_to_le16(sizeof(pdomain_info->action) + S_DS_GEN);
        HEXDUMP("11D: 802_11D_DOMAIN_INFO", (t_u8 *) pcmd,
                wlan_le16_to_cpu(pcmd->size));
        LEAVE();
        return MLAN_STATUS_SUCCESS;
    }

    /* Set domain info fields */
    domain->header.type = wlan_cpu_to_le16(TLV_TYPE_DOMAIN);
    memcpy(domain->country_code,
           pmadapter->domain_reg.country_code, sizeof(domain->country_code));

    domain->header.len = ((no_of_sub_band * sizeof(IEEEtypes_SubbandSet_t)) +
                          sizeof(domain->country_code));

    if (no_of_sub_band) {
        memcpy(domain->sub_band,
               pmadapter->domain_reg.sub_band,
               no_of_sub_band * sizeof(IEEEtypes_SubbandSet_t));

        pcmd->size = wlan_cpu_to_le16(sizeof(pdomain_info->action) +
                                      domain->header.len +
                                      sizeof(MrvlIEtypesHeader_t) + S_DS_GEN);
    } else {
        pcmd->size = wlan_cpu_to_le16(sizeof(pdomain_info->action) + S_DS_GEN);
    }
    domain->header.len = wlan_cpu_to_le16(domain->header.len);

    HEXDUMP("11D: 802_11D_DOMAIN_INFO", (t_u8 *) pcmd,
            wlan_le16_to_cpu(pcmd->size));

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function handle response of CMD_802_11D_DOMAIN_INFO
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         Pointer to command response buffer
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_ret_802_11d_domain_info(mlan_private * pmpriv, HostCmd_DS_COMMAND * resp)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    HostCmd_DS_802_11D_DOMAIN_INFO_RSP *domain_info =
        &resp->params.domain_info_resp;
    MrvlIEtypes_DomainParamSet_t *domain = &domain_info->domain;
    t_u16 action = wlan_le16_to_cpu(domain_info->action);
    t_u8 no_of_sub_band = 0;

    ENTER();

    /* Dump domain info response data */
    HEXDUMP("11D: DOMAIN Info Rsp Data", (t_u8 *) resp, resp->size);

    no_of_sub_band =
        (wlan_le16_to_cpu(domain->header.len) -
         3) / sizeof(IEEEtypes_SubbandSet_t);
    /* Country code is 3 bytes */

    PRINTM(INFO, "11D Domain Info Resp: no_of_sub_band=%d\n", no_of_sub_band);

    if (no_of_sub_band > MRVDRV_MAX_SUBBAND_802_11D) {
        PRINTM(ERROR, "11D: Invalid number of subbands returned!!\n");
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }

    switch (action) {
    case HostCmd_ACT_GEN_SET:  /* Proc Set Action */
        break;
    case HostCmd_ACT_GEN_GET:
        break;
    default:
        PRINTM(ERROR, "11D: Invalid Action:%d\n", domain_info->action);
        ret = MLAN_STATUS_FAILURE;
        break;
    }

    LEAVE();
    return ret;
}

/** 
 *  @brief This function generates 11D info from user specified regioncode 
 *         and download to FW
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param band         Band to create
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_11d_create_dnld_countryinfo(mlan_private * pmpriv, t_u8 band)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_adapter *pmadapter = pmpriv->adapter;
    region_chan_t *region_chan;
    parsed_region_chan_11d_t parsed_region_chan;
    t_u8 j;

    ENTER();

    PRINTM(INFO, "11D: Band[%d]\n", band);

    /* Update parsed_region_chan; download domain info to FW */

    /* Find region channel */
    for (j = 0;
         j <
         sizeof(pmadapter->region_channel) /
         sizeof(pmadapter->region_channel[0]); j++) {
        region_chan = &pmadapter->region_channel[j];

        PRINTM(INFO, "11D: [%d] region_chan->Band[%d]\n", j, region_chan->band);

        if (!region_chan || !region_chan->valid || !region_chan->pcfp)
            continue;
        switch (region_chan->band) {
        case BAND_B:
        case BAND_G:
            switch (band) {
            case BAND_B:
            case BAND_G:
            case BAND_G | BAND_B:
                break;
            default:
                continue;
            }
            break;
        default:
            continue;
        }
        break;
    }

    /* Check if region channel found */
    if (j >= sizeof(pmadapter->region_channel) /
        sizeof(pmadapter->region_channel[0])) {
        PRINTM(ERROR, "11D: region_chan not found. Band[%d]\n", band);
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }

    /* Generate parsed region channel info from region channel */
    memset(&parsed_region_chan, 0, sizeof(parsed_region_chan_11d_t));
    wlan_11d_generate_parsed_region_chan(pmadapter, region_chan,
                                         &parsed_region_chan);

    /* Generate domain info from parsed region channel info */
    memset(&pmadapter->domain_reg, 0, sizeof(wlan_802_11d_domain_reg_t));
    wlan_11d_generate_domain_info(pmadapter, &parsed_region_chan,
                                  &pmadapter->domain_reg);

    /* Set domain info */
    ret = wlan_11d_set_domain_info(pmpriv);
    if (ret) {
        PRINTM(ERROR, "11D: Error setting domain info in FW\n");
    }

    LEAVE();
    return ret;
}

/** 
 *  @brief This function parses country info from AP and 
 *           download country info to FW
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param pbss_desc     A pointer to BSS descriptor
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_11d_parse_dnld_countryinfo(mlan_private * pmpriv,
                                BSSDescriptor_t * pbss_desc)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_adapter *pmadapter = pmpriv->adapter;
    parsed_region_chan_11d_t region_chan;
    parsed_region_chan_11d_t bssdesc_region_chan;
    t_u32 i, j;

    ENTER();

    /* Only valid if 11D is enabled */
    if (wlan_11d_get_state(pmpriv) == ENABLE_11D) {

        memset(&pmadapter->domain_reg, 0, sizeof(wlan_802_11d_domain_reg_t));
        memset(&region_chan, 0, sizeof(parsed_region_chan_11d_t));
        memset(&bssdesc_region_chan, 0, sizeof(parsed_region_chan_11d_t));

        memcpy(&region_chan,
               &pmadapter->parsed_region_chan,
               sizeof(parsed_region_chan_11d_t));

        if (pbss_desc) {
            /* Parse domain info if available */
            ret =
                wlan_11d_parse_domain_info(pmadapter, &pbss_desc->country_info,
                                           0, &bssdesc_region_chan);

            if (ret == MLAN_STATUS_SUCCESS) {
                /* Update the channel-power table */
                for (i = 0; ((i < bssdesc_region_chan.no_of_chan)
                             && (i < MAX_NO_OF_CHAN)); i++) {

                    for (j = 0; ((j < region_chan.no_of_chan)
                                 && (j < MAX_NO_OF_CHAN)); j++) {
                        /* 
                         * Channel already exists, so overwrite existing 
                         * tx power with the tx_power received from 
                         * country info of the current AP
                         */
                        if (region_chan.chan_pwr[i].chan ==
                            bssdesc_region_chan.chan_pwr[j].chan) {
                            region_chan.chan_pwr[j].pwr =
                                bssdesc_region_chan.chan_pwr[i].pwr;
                            break;
                        }
                    }
                }
            }
        }

        /* Generate domain info */
        wlan_11d_generate_domain_info(pmadapter, &region_chan,
                                      &pmadapter->domain_reg);

        /* Set domain info */
        ret = wlan_11d_set_domain_info(pmpriv);
        if (ret) {
            PRINTM(ERROR, "11D: Error setting domain info in FW\n");
        }
    }

    LEAVE();
    return ret;
}

/** 
 *  @brief This function prepares domain info from scan table and 
 *         downloads the domain info command to the FW.
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_11d_prepare_dnld_domain_info_cmd(mlan_private * pmpriv)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_adapter *pmadapter = pmpriv->adapter;
    IEEEtypes_CountryInfoFullSet_t *pcountry_full = MNULL;
    t_u32 idx;

    ENTER();

    /* Only valid if 11D is enabled */
    if (wlan_11d_get_state(pmpriv) == ENABLE_11D &&
        pmadapter->num_in_scan_table != 0) {
        for (idx = 0; idx < pmadapter->num_in_scan_table; idx++) {
            pcountry_full = &pmadapter->pscan_table[idx].country_info;

            if (*(pcountry_full->country_code) == 0 ||
                (pcountry_full->len <= COUNTRY_CODE_LEN)) {
                /* Country info not found in the BSS descriptor */
                ret =
                    wlan_11d_update_chan_pwr_table(pmpriv,
                                                   &pmadapter->
                                                   pscan_table[idx]);
            } else {
                /* Country info found in the BSS Descriptor */
                ret =
                    wlan_11d_process_country_info(pmpriv,
                                                  &pmadapter->pscan_table[idx]);
            }
        }

        /* Check if connected */
        if (pmpriv->media_connected == MTRUE) {
            ret =
                wlan_11d_parse_dnld_countryinfo(pmpriv,
                                                &pmpriv->curr_bss_params.
                                                bss_descriptor);
        } else {
            ret = wlan_11d_parse_dnld_countryinfo(pmpriv, MNULL);
        }
    }

    LEAVE();
    return ret;
}
