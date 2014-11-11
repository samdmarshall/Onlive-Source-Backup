/** @file mlan_11h.c
 *
 *  @brief This file contains functions for 802.11H.
 *
 *  Copyright (C) 2008-2010, Marvell International Ltd. 
 *  All Rights Reserved
 */

/*************************************************************
Change Log:
    03/26/2009: initial version
************************************************************/

#include "mlan.h"
#include "mlan_join.h"
#include "mlan_util.h"
#include "mlan_fw.h"
#include "mlan_main.h"
#include "mlan_ioctl.h"
#include "mlan_meas.h"
#include "mlan_11h.h"

/*
 * Constants
 */

/** Default IBSS DFS recovery interval (in TBTTs); used for adhoc start */
#define WLAN_11H_DEFAULT_DFS_RECOVERY_INTERVAL   100

/** Default 11h power constraint used to offset the maximum transmit power */
#define WLAN_11H_TPC_POWERCONSTRAINT  0

/** 11h TPC Power capability minimum setting, sent in TPC_INFO command to fw */
#define WLAN_11H_TPC_POWERCAPABILITY_MIN     5

/** 11h TPC Power capability maximum setting, sent in TPC_INFO command to fw */
#define WLAN_11H_TPC_POWERCAPABILITY_MAX     20

/** Regulatory requirement for the duration of a channel availability check */
#define WLAN_11H_CHANNEL_AVAIL_CHECK_DURATION    60000  /* in ms */

/** U-NII sub-band config : Start Channel = 36, NumChans = 4 */
static const
    IEEEtypes_SupportChan_Subband_t wlan_11h_unii_lower_band = { 36, 4 };

/** U-NII sub-band config : Start Channel = 52, NumChans = 4 */
static const
    IEEEtypes_SupportChan_Subband_t wlan_11h_unii_middle_band = { 52, 4 };

/** U-NII sub-band config : Start Channel = 100, NumChans = 11 */
static const
    IEEEtypes_SupportChan_Subband_t wlan_11h_unii_mid_upper_band = { 100, 11 };

/** U-NII sub-band config : Start Channel = 149, NumChans = 4 */
static const
    IEEEtypes_SupportChan_Subband_t wlan_11h_unii_upper_band = { 149, 4 };

/*
 * Local types
 */

/** Internally passed structure used to send a CMD_802_11_TPC_INFO command */
typedef struct
{
    t_u8 chan;                  /**< Channel to which the power constraint applies */
    t_u8 power_constraint;      /**< Local power constraint to send to firmware */
} wlan_11h_tpc_info_param_t;

/**
 *  @brief Utility function to get a random number based on the underlying OS
 *
 *  @param pmadapter Pointer to mlan_adapter
 *  @return random integer
 */
static t_u32
wlan_11h_get_random_num(pmlan_adapter pmadapter)
{
    t_u32 sec, usec;

    pmadapter->callbacks.moal_get_system_time(pmadapter->pmoal_handle, &sec,
                                              &usec);
    sec = (sec & 0xFFFF) + (sec >> 16);
    usec = (usec & 0xFFFF) + (usec >> 16);

    return ((sec << 16) + usec);
}

/**
 *  @brief Convert an IEEE formatted IE to 16-bit ID/Len Marvell
 *         proprietary format
 *  
 *  @param pmadapter Pointer to mlan_adapter
 *  @param pout_buf Output parameter: Buffer to output Marvell formatted IE
 *  @param pin_ie   Pointer to IEEE IE to be converted to Marvell format
 *
 *  @return         Number of bytes output to pout_buf parameter return
 */
static int
wlan_11h_convert_ieee_to_mrvl_ie(mlan_adapter * pmadapter,
                                 char *pout_buf, const char *pin_ie)
{
    MrvlIEtypesHeader_t mrvl_ie_hdr;
    char *ptmp_buf = pout_buf;

    ENTER();
    /* Assign the Element Id and Len to the Marvell struct attributes */
    mrvl_ie_hdr.type = wlan_cpu_to_le16(pin_ie[0]);
    mrvl_ie_hdr.len = wlan_cpu_to_le16(pin_ie[1]);

    /* If the element ID is zero, return without doing any copying */
    if (!mrvl_ie_hdr.type)
        return 0;

    /* Copy the header to the buffer pointer */
    memcpy(pmadapter, ptmp_buf, &mrvl_ie_hdr, sizeof(mrvl_ie_hdr));

    /* Increment the temp buffer pointer by the size appended */
    ptmp_buf += sizeof(mrvl_ie_hdr);

    /* Append the data section of the IE; length given by the IEEE IE length */
    memcpy(pmadapter, ptmp_buf, pin_ie + 2, pin_ie[1]);

    LEAVE();
    /* Return the number of bytes appended to pout_buf */
    return (sizeof(mrvl_ie_hdr) + pin_ie[1]);
}

/**
 *  @brief Setup the IBSS DFS element passed to the firmware in adhoc start
 *         and join commands
 *
 *  The DFS Owner and recovery fields are set to be our MAC address and
 *    a predetermined constant recovery value.  If we are joining an adhoc
 *    network, these values are replaced with the existing IBSS values.
 *    They are valid only when starting a new IBSS.
 *
 *  The IBSS DFS Element is variable in size based on the number of
 *    channels supported in our current region.
 *
 *  @param priv Private driver information structure
 *  @param pdfs Output parameter: Pointer to the IBSS DFS element setup by
 *              this function.
 *
 *  @return
 *    - Length of the returned element in pdfs output parameter
 *    - 0 if returned element is not setup
 */
static int
wlan_11h_set_ibss_dfs_ie(mlan_private * priv, IEEEtypes_IBSS_DFS_t * pdfs)
{
    int num_chans = 0;
    MeasRptBasicMap_t initial_map;
    mlan_adapter *adapter = priv->adapter;

    ENTER();

    PRINTM(MINFO,
           "11h: Setup IBSS DFS element, 11d parsed region: %c%c (0x%x)\n",
           adapter->parsed_region_chan.country_code[0],
           adapter->parsed_region_chan.country_code[1],
           adapter->parsed_region_chan.region);

    memset(adapter, pdfs, 0x00, sizeof(IEEEtypes_IBSS_DFS_t));

    /* 
     * A basic measurement report is included with each channel in the
     *   map field.  Initial value for the map for each supported channel
     *   is with only the unmeasured bit set.
     */
    memset(adapter, &initial_map, 0x00, sizeof(initial_map));
    initial_map.unmeasured = 1;

    /* Set the DFS Owner and recovery interval fields */
    memcpy(adapter, pdfs->dfs_owner, priv->curr_addr, sizeof(pdfs->dfs_owner));
    pdfs->dfs_recovery_interval = WLAN_11H_DEFAULT_DFS_RECOVERY_INTERVAL;

    for (; (num_chans < adapter->parsed_region_chan.no_of_chan)
         && (num_chans < WLAN_11H_MAX_IBSS_DFS_CHANNELS); num_chans++) {
        pdfs->channel_map[num_chans].channel_number =
            adapter->parsed_region_chan.chan_pwr[num_chans].chan;

        /* 
         * Set the initial map field with a basic measurement
         */
        pdfs->channel_map[num_chans].rpt_map = initial_map;
    }

    /* 
     * If we have an established channel map, include it and return
     *   a valid DFS element
     */
    if (num_chans) {
        PRINTM(MINFO, "11h: Added %d channels to IBSS DFS Map\n", num_chans);

        pdfs->element_id = IBSS_DFS;
        pdfs->len =
            (sizeof(pdfs->dfs_owner) + sizeof(pdfs->dfs_recovery_interval)
             + num_chans * sizeof(IEEEtypes_ChannelMap_t));

        return (pdfs->len + sizeof(pdfs->len) + sizeof(pdfs->element_id));
    }

    /* Ensure the element is zeroed out for an invalid return */
    memset(adapter, pdfs, 0x00, sizeof(IEEEtypes_IBSS_DFS_t));

    LEAVE();
    return 0;
}

/**
 *  @brief Setup the Supported Channel IE sent in association requests
 *
 *  The Supported Channels IE is required to be sent when the spectrum
 *    management capability (11h) is enabled.  The element contains a
 *    starting channel and number of channels tuple for each sub-band
 *    the STA supports.  This information is based on the operating region.
 *
 *  @param priv      Private driver information structure
 *  @param psup_chan Output parameter: Pointer to the Supported Chan element
 *                   setup by this function.
 *
 *  @return
 *    - Length of the returned element in psup_chan output parameter
 *    - 0 if returned element is not setup
 */
static int
wlan_11h_set_supp_channels_ie(mlan_private * priv,
                              IEEEtypes_SupportedChannels_t * psup_chan)
{
    int num_subbands = 0;
    int ret_len = 0;

    ENTER();
    memset(priv->adapter, psup_chan, 0x00,
           sizeof(IEEEtypes_SupportedChannels_t));

    /* 
     * Set the supported channel elements based on the region code,
     *   incrementing num_subbands for each sub-band we append to the
     *   element.
     */
    switch (priv->adapter->region_code) {
    case 0x10:                 /* USA FCC */
    case 0x20:                 /* Canada IC */
        psup_chan->subband[num_subbands++] = wlan_11h_unii_lower_band;
        psup_chan->subband[num_subbands++] = wlan_11h_unii_middle_band;
        psup_chan->subband[num_subbands++] = wlan_11h_unii_mid_upper_band;
        psup_chan->subband[num_subbands++] = wlan_11h_unii_upper_band;
        break;
    case 0x30:                 /* Europe ETSI */
        psup_chan->subband[num_subbands++] = wlan_11h_unii_lower_band;
        psup_chan->subband[num_subbands++] = wlan_11h_unii_middle_band;
        psup_chan->subband[num_subbands++] = wlan_11h_unii_mid_upper_band;
        break;
    default:
        break;
    }

    /* 
     * If we have setup any supported subbands in the element, return a
     *    valid IE along with its size, else return 0.
     */
    if (num_subbands) {
        psup_chan->element_id = SUPPORTED_CHANNELS;
        psup_chan->len = num_subbands * sizeof(IEEEtypes_SupportChan_Subband_t);

        ret_len = (psup_chan->len
                   + sizeof(psup_chan->len) + sizeof(psup_chan->element_id));

        HEXDUMP("11h: SupChan", (t_u8 *) psup_chan, ret_len);
    }

    LEAVE();
    return ret_len;
}

/**
 *  @brief Prepare CMD_802_11_TPC_ADAPT_REQ firmware command
 *
 *  @param priv      Private driver information structure
 *  @param pcmd_ptr  Output parameter: Pointer to the command being prepared
 *                   for the firmware
 *  @param pinfo_buf HostCmd_DS_802_11_TPC_ADAPT_REQ passed as void data block
 *
 *  @return          MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static int
wlan_11h_cmd_tpc_request(mlan_private * priv,
                         HostCmd_DS_COMMAND * pcmd_ptr,
                         const t_void * pinfo_buf)
{
    ENTER();

    memcpy(priv->adapter, &pcmd_ptr->params.tpc_req, pinfo_buf,
           sizeof(HostCmd_DS_802_11_TPC_ADAPT_REQ));

    pcmd_ptr->params.tpc_req.req.timeout =
        wlan_cpu_to_le16(pcmd_ptr->params.tpc_req.req.timeout);

    /* Converted to little endian in wlan_11h_cmd_process */
    pcmd_ptr->size = sizeof(HostCmd_DS_802_11_TPC_ADAPT_REQ) + S_DS_GEN;

    HEXDUMP("11h: 11_TPC_ADAPT_REQ:", (t_u8 *) pcmd_ptr, (int) pcmd_ptr->size);

    LEAVE();

    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief Prepare CMD_802_11_TPC_INFO firmware command
 *
 *  @param priv      Private driver information structure
 *  @param pcmd_ptr  Output parameter: Pointer to the command being prepared 
 *                   for the firmware
 *  @param pinfo_buf wlan_11h_tpc_info_param_t passed as void data block
 *
 *  @return          MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static int
wlan_11h_cmd_tpc_info(mlan_private * priv,
                      HostCmd_DS_COMMAND * pcmd_ptr, const t_void * pinfo_buf)
{
    HostCmd_DS_802_11_TPC_INFO *ptpc_info = &pcmd_ptr->params.tpc_info;
    MrvlIEtypes_LocalPowerConstraint_t *pconstraint =
        &ptpc_info->local_constraint;
    MrvlIEtypes_PowerCapability_t *pcap = &ptpc_info->power_cap;

    wlan_11h_state_t *pstate = &priv->adapter->state_11h;
    const wlan_11h_tpc_info_param_t *ptpc_info_param =
        (wlan_11h_tpc_info_param_t *) pinfo_buf;

    ENTER();

    pcap->min_power = pstate->min_tx_power_capability;
    pcap->max_power = pstate->max_tx_power_capability;
    pcap->header.len = wlan_cpu_to_le16(2);
    pcap->header.type = wlan_cpu_to_le16(TLV_TYPE_POWER_CAPABILITY);

    pconstraint->chan = ptpc_info_param->chan;
    pconstraint->constraint = ptpc_info_param->power_constraint;
    pconstraint->header.type = wlan_cpu_to_le16(TLV_TYPE_POWER_CONSTRAINT);
    pconstraint->header.len = wlan_cpu_to_le16(2);

    /* Converted to little endian in wlan_11h_cmd_process */
    pcmd_ptr->size = sizeof(HostCmd_DS_802_11_TPC_INFO) + S_DS_GEN;

    HEXDUMP("11h: TPC INFO", (t_u8 *) pcmd_ptr, (int) pcmd_ptr->size);

    LEAVE();

    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief  Prepare CMD_802_11_CHAN_SW_ANN firmware command
 *
 *  @param priv      Private driver information structure
 *  @param pcmd_ptr  Output parameter: Pointer to the command being 
 *                   prepared to for firmware
 *  @param pinfo_buf HostCmd_DS_802_11_CHAN_SW_ANN passed as void data block
 *
 *  @return          MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static int
wlan_11h_cmd_chan_sw_ann(mlan_private * priv,
                         HostCmd_DS_COMMAND * pcmd_ptr,
                         const t_void * pinfo_buf)
{
    const HostCmd_DS_802_11_CHAN_SW_ANN *pch_sw_ann =
        (HostCmd_DS_802_11_CHAN_SW_ANN *) pinfo_buf;

    ENTER();

    /* Converted to little endian in wlan_11h_cmd_process */
    pcmd_ptr->size = sizeof(HostCmd_DS_802_11_CHAN_SW_ANN) + S_DS_GEN;

    memcpy(priv->adapter, &pcmd_ptr->params.chan_sw_ann, pch_sw_ann,
           sizeof(HostCmd_DS_802_11_CHAN_SW_ANN));

    PRINTM(MINFO, "11h: ChSwAnn: %#x-%u, Seq=%u, Ret=%u\n",
           pcmd_ptr->command, pcmd_ptr->size, pcmd_ptr->seq_num,
           pcmd_ptr->result);
    PRINTM(MINFO, "11h: ChSwAnn: Ch=%d, Cnt=%d, Mode=%d\n",
           pch_sw_ann->new_chan, pch_sw_ann->switch_count,
           pch_sw_ann->switch_mode);

    LEAVE();

    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief Set the local power capability and constraint TLV 
 *
 *  @param ppbuffer                The buffer to add these two TLVs
 *  @param channel                 Channel to which the power constraint applies
 *  @param power_constraint        Power constraint to be applied on the channel
 *  @param min_tx_power_capability Min. Tx Power in Power Capability IE
 *  @param max_tx_power_capability Max. Tx Power in Power Capability IE
 *
 *  @return                        The len increased
 */
static int
wlan_11h_set_local_power_constraint_tlv(t_u8 ** ppbuffer,
                                        t_u8 channel,
                                        t_u8 power_constraint,
                                        t_u8 min_tx_power_capability,
                                        t_u8 max_tx_power_capability)
{
    MrvlIEtypes_PowerCapability_t *pcap;
    MrvlIEtypes_LocalPowerConstraint_t *pconstraint;
    t_u8 *startPtr = *ppbuffer;

    ENTER();

    /* Null Checks */
    if (!ppbuffer)
        return 0;
    if (!(*ppbuffer))
        return 0;

    PRINTM(MINFO,
           "11h: Set local power constraint = %d channel=%d min_tx_pwr=%d max_tx_pwr=%d\n",
           power_constraint, channel, min_tx_power_capability,
           max_tx_power_capability);

    pcap = (MrvlIEtypes_PowerCapability_t *) * ppbuffer;
    pcap->header.type = wlan_cpu_to_le16(TLV_TYPE_POWER_CAPABILITY);
    pcap->header.len = wlan_cpu_to_le16(2);
    pcap->min_power = min_tx_power_capability;
    pcap->max_power = max_tx_power_capability;
    *ppbuffer += sizeof(MrvlIEtypesHeader_t) + 2;

    pconstraint = (MrvlIEtypes_LocalPowerConstraint_t *) * ppbuffer;
    pconstraint->header.type = wlan_cpu_to_le16(TLV_TYPE_POWER_CONSTRAINT);
    pconstraint->header.len = wlan_cpu_to_le16(2);
    pconstraint->chan = channel;
    pconstraint->constraint = power_constraint;
    *ppbuffer += sizeof(MrvlIEtypesHeader_t) + 2;

    LEAVE();

    return (int) (*ppbuffer - startPtr);
}

/**
 *  @brief  Utility function to process a join to an infrastructure BSS
 *
 *  @param priv          Private driver information structure
 *  @param ppbuffer      Output parameter: Pointer to the TLV output buffer,
 *                       modified on return to point after the appended 11h TLVs
 *  @param channel       Channel on which we are joining the BSS
 *  @param p11h_bss_info Pointer to the 11h BSS information for this network
 *                       that was parsed out of the scan response.
 *
 *  @return              Integer number of bytes appended to the TLV output
 *                       buffer (ppbuffer)
 */
static int
wlan_11h_process_infra_join(mlan_private * priv,
                            t_u8 ** ppbuffer,
                            t_u32 channel, wlan_11h_bss_info_t * p11h_bss_info)
{
    MrvlIEtypesHeader_t ie_header;
    IEEEtypes_SupportedChannels_t sup_chan_ie;
    int ret_len = 0;
    int sup_chan_len = 0;

    ENTER();

    /* Null Checks */
    if (!ppbuffer)
        return 0;
    if (!(*ppbuffer))
        return 0;

    ret_len += wlan_11h_set_local_power_constraint_tlv(ppbuffer, channel,
                                                       p11h_bss_info->
                                                       power_constraint.
                                                       local_constraint,
                                                       priv->adapter->state_11h.
                                                       min_tx_power_capability,
                                                       priv->adapter->state_11h.
                                                       max_tx_power_capability);

    /* Setup the Supported Channels IE */
    sup_chan_len = wlan_11h_set_supp_channels_ie(priv, &sup_chan_ie);

    /* 
     * If we returned a valid Supported Channels IE, wrap and append it
     */
    if (sup_chan_len) {
        /* Wrap the supported channels IE with a passthrough TLV type */
        ie_header.type = wlan_cpu_to_le16(TLV_TYPE_PASSTHROUGH);
        ie_header.len = sup_chan_len;
        memcpy(priv->adapter, *ppbuffer, &ie_header, sizeof(ie_header));

        /* Increment the return size and the return buffer pointer param */
        *ppbuffer += sizeof(ie_header);
        ret_len += sizeof(ie_header);

        /* Copy the supported channels IE to the output buf, advance pointer */
        memcpy(priv->adapter, *ppbuffer, &sup_chan_ie, sup_chan_len);
        *ppbuffer += sup_chan_len;
        ret_len += sup_chan_len;
    }

    LEAVE();
    return ret_len;
}

/**
 *  @brief Utility function to process a start or join to an adhoc network
 *
 *  Add the elements to the TLV buffer needed in the start/join adhoc commands:
 *       - IBSS DFS IE
 *       - Quiet IE
 *
 *  Also send the local constraint to the firmware in a TPC_INFO command.
 *
 *  @param priv          Private driver information structure
 *  @param ppbuffer      Output parameter: Pointer to the TLV output buffer,
 *                       modified on return to point after the appended 11h TLVs
 *  @param channel       Channel on which we are starting/joining the IBSS
 *  @param p11h_bss_info Pointer to the 11h BSS information for this network
 *                       that was parsed out of the scan response.  NULL
 *                       indicates we are starting the adhoc network
 *
 *  @return              Integer number of bytes appended to the TLV output
 *                       buffer (ppbuffer)
 */
static int
wlan_11h_process_adhoc(mlan_private * priv,
                       t_u8 ** ppbuffer,
                       t_u32 channel, wlan_11h_bss_info_t * p11h_bss_info)
{
    IEEEtypes_IBSS_DFS_t dfs_elem;
    int size_appended;
    int ret_len = 0;
    t_s8 local_constraint = 0;
    mlan_adapter *adapter = priv->adapter;

    ENTER();

    /* Format our own IBSS DFS Element.  Include our channel map fields */
    wlan_11h_set_ibss_dfs_ie(priv, &dfs_elem);

    if (p11h_bss_info) {
        /* 
         * Copy the DFS Owner/Recovery Interval from the BSS we are joining
         */
        memcpy(adapter, dfs_elem.dfs_owner,
               p11h_bss_info->ibss_dfs.dfs_owner, sizeof(dfs_elem.dfs_owner));
        dfs_elem.dfs_recovery_interval =
            p11h_bss_info->ibss_dfs.dfs_recovery_interval;
    }

    /* Append the dfs element to the TLV buffer */
    size_appended =
        wlan_11h_convert_ieee_to_mrvl_ie(adapter, (char *) *ppbuffer,
                                         (char *) &dfs_elem);

    HEXDUMP("11h: IBSS-DFS", (t_u8 *) * ppbuffer, size_appended);
    *ppbuffer += size_appended;
    ret_len += size_appended;

    /* 
     * Check to see if we are joining a network.  Join is indicated by the
     *   BSS Info pointer being valid (not NULL)
     */
    if (p11h_bss_info) {
        /* 
         * If there was a quiet element, include it in adhoc join command
         */
        if (p11h_bss_info->quiet.element_id == QUIET) {
            size_appended
                = wlan_11h_convert_ieee_to_mrvl_ie(adapter, (char *) *ppbuffer,
                                                   (char *) &p11h_bss_info->
                                                   quiet);
            HEXDUMP("11h: Quiet", (t_u8 *) * ppbuffer, size_appended);
            *ppbuffer += size_appended;
            ret_len += size_appended;
        }

        /* Copy the local constraint from the network */
        local_constraint = p11h_bss_info->power_constraint.local_constraint;
    } else {
        /* 
         * If we are the adhoc starter, we can add a quiet element
         */
        if (adapter->state_11h.quiet_ie.quiet_period) {
            size_appended =
                wlan_11h_convert_ieee_to_mrvl_ie(adapter, (char *) *ppbuffer,
                                                 (char *) &adapter->state_11h.
                                                 quiet_ie);
            HEXDUMP("11h: Quiet", (t_u8 *) * ppbuffer, size_appended);
            *ppbuffer += size_appended;
            ret_len += size_appended;

            /* Use the local_constraint configured in the driver state */
            local_constraint = adapter->state_11h.usr_def_power_constraint;
        }
    }

    PRINTM(MINFO, "WEILIE 1: ppbuffer = %p\n", *ppbuffer);

    ret_len +=
        wlan_11h_set_local_power_constraint_tlv(ppbuffer, channel,
                                                local_constraint,
                                                priv->adapter->state_11h.
                                                min_tx_power_capability,
                                                priv->adapter->state_11h.
                                                max_tx_power_capability);
    PRINTM(MINFO, "WEILIE 2: ppbuffer = %p\n", *ppbuffer);

    LEAVE();
    return ret_len;
}

/**
 *  @brief Return whether the driver is currently setup to use 11h for
 *         adhoc start.
 *
 *  Association/Join commands are dynamic in that they enable 11h in the
 *    driver/firmware when they are detected in the existing BSS.
 *
 *  @param priv  Private driver information structure
 *
 *  @return
 *    - MTRUE if 11h is enabled
 *    - MFALSE otherwise
 */
static int
wlan_11h_is_enabled(mlan_private * priv)
{
    wlan_11h_state_t *pstate_11h = &priv->adapter->state_11h;

    return (pstate_11h->is_11h_enabled ? MTRUE : MFALSE);
}

/**
 *  @brief Query 11h firmware enabled state.
 *
 *  Return whether the firmware currently has 11h extensions enabled
 *
 *  @param priv  Private driver information structure
 *
 *  @return
 *    - MTRUE if 11h has been activated in the firmware
 *    - MFALSE otherwise
 *
 *  @sa wlan_11h_activate
 */
int
wlan_11h_is_active(mlan_private * priv)
{
    wlan_11h_state_t *pstate_11h = &priv->adapter->state_11h;

    return (pstate_11h->is_11h_active ? MTRUE : MFALSE);
}

/**
 *  @brief Disable the transmit interface and record the state.
 *
 *  @param priv  Private driver information structure
 *
 *  @return      t_void
 */
t_void
wlan_11h_tx_disable(mlan_private * priv)
{
    wlan_11h_state_t *pstate_11h = &priv->adapter->state_11h;

    ENTER();
    pstate_11h->tx_disabled = MTRUE;
    LEAVE();
}

/**
 *  @brief Enable the transmit interface and record the state.
 *
 *  @param priv  Private driver information structure
 *
 *  @return      t_void
 */
t_void
wlan_11h_tx_enable(mlan_private * priv)
{
    wlan_11h_state_t *pstate_11h = &priv->adapter->state_11h;

    ENTER();
    pstate_11h->tx_disabled = MFALSE;
    LEAVE();
}

/**
 *  @brief Enable or Disable the 11h extensions in the firmware
 *
 *  @param priv  Private driver information structure
 *  @param flag  Enable 11h if MTRUE, disable otherwise
 *
 *  @return      MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
int
wlan_11h_activate(mlan_private * priv, t_u32 flag)
{
    wlan_11h_state_t *pstate_11h = &priv->adapter->state_11h;
    int enable = flag ? 1 : 0;
    int ret = MLAN_STATUS_SUCCESS;

    ENTER();

    /* 
     * Send cmd to FW to enable/disable 11h function in firmware
     */
    ret = wlan_prepare_cmd(priv,
                           HostCmd_CMD_802_11_SNMP_MIB,
                           HostCmd_ACT_GEN_SET, Dot11H_i, MNULL, &enable);

    if (ret)
        ret = MLAN_STATUS_FAILURE;
    else
        /* Set boolean flag in driver 11h state */
        pstate_11h->is_11h_active = flag;

    PRINTM(MINFO, "11h: %s\n", enable ? "Activate" : "Deactivate");

    LEAVE();

    return ret;
}

/**
 *
 *  @brief Initialize the 11h parameters and enable 11h when starting an IBSS
 *
 *  @param adapter mlan_adapter structure
 *
 *  @return      t_void
 */
t_void
wlan_11h_init(mlan_adapter * adapter)
{
    wlan_11h_state_t *pstate_11h = &adapter->state_11h;
    IEEEtypes_Quiet_t *pquiet = &adapter->state_11h.quiet_ie;

    ENTER();

    pstate_11h->usr_def_power_constraint = WLAN_11H_TPC_POWERCONSTRAINT;
    pstate_11h->min_tx_power_capability = WLAN_11H_TPC_POWERCAPABILITY_MIN;
    pstate_11h->max_tx_power_capability = WLAN_11H_TPC_POWERCAPABILITY_MAX;

    /* 
     * By default, the driver should have its preference set as 11h being
     *    activated when starting an ad hoc network.  For infrastructure
     *    and ad hoc join, 11h will be sensed and activated accordingly.
     */
    pstate_11h->is_11h_enabled = MTRUE;

    /* 
     * On power up, the firmware should have 11h support inactive.
     */
    pstate_11h->is_11h_active = MFALSE;

    /* Initialize quiet_ie */
    memset(adapter, pquiet, 0, sizeof(IEEEtypes_Quiet_t));

    pquiet->element_id = QUIET;
    pquiet->len = (sizeof(pquiet->quiet_count) + sizeof(pquiet->quiet_period)
                   + sizeof(pquiet->quiet_duration)
                   + sizeof(pquiet->quiet_offset));
    LEAVE();
}

/**
 *  @brief Retrieve a randomly selected starting channel if needed for 11h
 *
 *  If 11h is enabled and an A-Band channel start band preference
 *    configured in the driver, the start channel must be random in order
 *    to meet with
 *
 *  @param priv  Private driver information structure
 *
 *  @return      Integer starting channel
 */
int
wlan_11h_get_adhoc_start_channel(mlan_private * priv)
{
    unsigned int start_chn;
    mlan_adapter *adapter = priv->adapter;
    int region;
    t_u32 rand_entry;
    region_chan_t *chn_tbl;

    ENTER();

    /* 
     * Set start_chn to the Default.  Used if 11h is disabled or the band
     *   does not require 11h support.
     */
    start_chn = DEFAULT_AD_HOC_CHANNEL;

    /* 
     * Check that we are looking for a channel in the A Band
     */
    if ((adapter->adhoc_start_band & BAND_A)
        || (adapter->adhoc_start_band & BAND_AN)
        ) {
        /* 
         * Set default to the A Band default. Used if random selection fails
         *   or if 11h is not enabled
         */
        start_chn = DEFAULT_AD_HOC_CHANNEL_A;

        /* 
         * Check that 11h is enabled in the driver
         */
        if (wlan_11h_is_enabled(priv)) {
            /* 
             * Search the region_channel tables for a channel table
             *   that is marked for the A Band.
             */
            for (region = 0; (region < MAX_REGION_CHANNEL_NUM); region++) {
                chn_tbl = &adapter->region_channel[region];

                /* Check if table is valid and marked for A Band */
                if (chn_tbl->valid
                    && chn_tbl->region == adapter->region_code
                    && chn_tbl->band & BAND_A) {
                    /* 
                     * Set the start channel.  Get a random number and
                     *   use it to pick an entry in the table between 0
                     *   and the number of channels in the table (NumCFP).
                     */
                    rand_entry =
                        wlan_11h_get_random_num(adapter) % chn_tbl->num_cfp;
                    start_chn = chn_tbl->pcfp[rand_entry].channel;
                }
            }
        }
    }

    PRINTM(MINFO, "11h: %s: AdHoc Channel set to %u\n",
           wlan_11h_is_enabled(priv) ? "Enabled" : "Disabled", start_chn);

    LEAVE();

    return start_chn;
}

/**
 *  @brief Check if the current region's regulations require the input channel
 *         to be scanned for radar.
 *
 *  Based on statically defined requirements for sub-bands per regulatory
 *    agency requirements.
 *
 *  Used in adhoc start to determine if channel availability check is required
 *
 *  @param priv    Private driver information structure
 *  @param channel Channel to determine radar detection requirements
 *
 *  @return
 *    - MTRUE if radar detection is required
 *    - MFALSE otherwise
 *
 *  @sa wlan_11h_radar_detected
 */
int
wlan_11h_radar_detect_required(mlan_private * priv, t_u8 channel)
{
    int required;

    ENTER();

    /* 
     * Assume that radar detection is required unless exempted below.
     *   No checks for 11h or measurement code  being enabled is placed here
     *   since regulatory requirements exist whether we support them or not.
     */
    required = MTRUE;

    switch (priv->adapter->region_code) {
    case 0x10:                 /* USA FCC */
    case 0x20:                 /* Canada IC */
        /* 
         * FCC does not yet require radar detection in the
         *   5.25-5.35 (U-NII middle) band
         */
        if (channel < wlan_11h_unii_middle_band.start_chan ||
            channel >= wlan_11h_unii_mid_upper_band.start_chan) {
            required = MFALSE;
        }
        break;
    case 0x30:                 /* Europe ETSI */
        /* 
         * Radar detection is not required in the
         *   5.15-5.25 (U-NII lower) and 5.725-5.825 (U-NII upper) bands
         */
        if (channel < wlan_11h_unii_middle_band.start_chan ||
            channel >= wlan_11h_unii_upper_band.start_chan) {
            /* Radar detection not required */
            required = MFALSE;
        }
        break;
    default:
        break;
    }

    PRINTM(MINFO, "11h: Radar detection in region %#02x "
           "is %srequired for channel %d\n",
           priv->adapter->region_code, (required ? "" : "not "), channel);

    if (required == MTRUE && priv->media_connected == MTRUE
        && priv->curr_bss_params.bss_descriptor.channel == channel) {
        required = MFALSE;

        PRINTM(MINFO, "11h: Radar detection not required. "
               "Already operating on the channel");
    }

    LEAVE();
    return required;
}

/**
 *  @brief Perform a radar measurement and report the result if required on
 *         given channel
 *
 *  Check to see if the provided channel requires a channel availability
 *    check (60 second radar detection measurement).  If required, perform
 *    measurement, stalling calling thread until the measurement completes
 *    and then report result.
 *
 *  Used when starting an adhoc network.
 *
 *  @param priv         Private driver information structure
 *  @param pioctl_req   Pointer to IOCTL request buffer
 *  @param channel      Channel on which to perform radar measurement
 *
 *  @return
 *    - MTRUE if radar has been detected
 *    - MFALSE if radar detection is not required or radar has not been detected
 *
 *  @sa wlan_11h_radar_detect_required
 */
int
wlan_11h_radar_detected(mlan_private * priv, pmlan_ioctl_req pioctl_req,
                        t_u8 channel)
{
    int ret;
    HostCmd_DS_MEASUREMENT_REQUEST meas_req;
    HostCmd_DS_MEASUREMENT_REPORT meas_rpt;

    ENTER();

    /* 
     * If the channel requires radar, default the return value to it being
     *   detected.
     */
    ret = wlan_11h_radar_detect_required(priv, channel);

    memset(priv->adapter, &meas_req, 0x00, sizeof(meas_req));
    memset(priv->adapter, &meas_rpt, 0x00, sizeof(meas_rpt));

    /* 
     * Send a basic measurement request on the indicated channel for the
     *   required channel availability check time.
     */
    meas_req.meas_type = WLAN_MEAS_BASIC;
    meas_req.req.basic.channel = channel;
    meas_req.req.basic.duration = WLAN_11H_CHANNEL_AVAIL_CHECK_DURATION;

    /* 
     * Set the STA that we are requesting the measurement from to our own
     *   mac address, causing our firmware to perform the measurement itself
     */
    memcpy(priv->adapter, meas_req.mac_addr, priv->curr_addr,
           sizeof(meas_req.mac_addr));

    /* 
     * Send the measurement request and timeout duration to wait for
     *   the command to spend until the measurement report is received
     *   from the firmware.  If the command fails, the default ret value set
     *   above will be returned.
     */
    ret = wlan_meas_util_send_req(priv, &meas_req, meas_req.req.basic.duration,
                                  pioctl_req, &meas_rpt);

    LEAVE();
    return ret;
}

/**
 *  @brief Process an TLV buffer for a pending BSS Adhoc start command.
 *
 *  Activate 11h functionality in the firmware if driver has is enabled
 *    for 11h (configured by the application via IOCTL).
 *
 *  @param priv          Private driver information structure
 *  @param ppbuffer      Output parameter: Pointer to the TLV output buffer, 
 *                       modified on return to point after the appended 11h TLVs
 *  @param pcap_info     Pointer to the capability info for the BSS to join
 *  @param channel       Channel on which we are starting the IBSS
 *  @param p11h_bss_info Input/Output parameter: Pointer to the 11h BSS 
 *                       information for this network that we are establishing.
 *                       11h sensed flag set on output if warranted.
 *
 *  @return              Integer number of bytes appended to the TLV output
 *                       buffer (ppbuffer)
 *
 */
int
wlan_11h_process_start(mlan_private * priv,
                       t_u8 ** ppbuffer,
                       IEEEtypes_CapInfo_t * pcap_info,
                       t_u32 channel, wlan_11h_bss_info_t * p11h_bss_info)
{
    mlan_adapter *adapter = priv->adapter;
    int ret = 0;

    ENTER();
    if (wlan_11h_is_enabled(priv)
        && ((adapter->adhoc_start_band & BAND_A)
            || (adapter->adhoc_start_band & BAND_AN)
        )
        ) {
        if (wlan_11d_get_state(priv) == DISABLE_11D) {
            /* No use having 11h enabled without 11d enabled */
            wlan_11d_enable(priv, MNULL, ENABLE_11D);
            wlan_11d_create_dnld_countryinfo(priv, adapter->adhoc_start_band);
        }

        /* Activate 11h functions in firmware, turns on capability bit */
        wlan_11h_activate(priv, MTRUE);
        pcap_info->spectrum_mgmt = 1;

        /* Set flag indicating this BSS we are starting is using 11h */
        p11h_bss_info->sensed_11h = MTRUE;

        ret = wlan_11h_process_adhoc(priv, ppbuffer, channel, MNULL);
    } else {
        /* Deactivate 11h functions in the firmware */
        wlan_11h_activate(priv, MFALSE);
        pcap_info->spectrum_mgmt = 0;
    }
    LEAVE();
    return ret;
}

/**
 *  @brief Process an TLV buffer for a pending BSS Join command for
 *         both adhoc and infra networks
 *
 *  The TLV command processing for a BSS join for either adhoc or
 *    infrastructure network is performed with this function.  The
 *    capability bits are inspected for the IBSS flag and the appropriate
 *    local routines are called to setup the necessary TLVs.
 *
 *  Activate 11h functionality in the firmware if the spectrum management
 *    capability bit is found in the network information for the BSS we are
 *    joining.
 *
 *  @param priv          Private driver information structure
 *  @param ppbuffer      Output parameter: Pointer to the TLV output buffer, 
 *                       modified on return to point after the appended 11h TLVs
 *  @param pcap_info     Pointer to the capability info for the BSS to join
 *  @param channel       Channel on which we are joining the BSS
 *  @param p11h_bss_info Pointer to the 11h BSS information for this
 *                       network that was parsed out of the scan response.
 *
 *  @return              Integer number of bytes appended to the TLV output
 *                       buffer (ppbuffer), MLAN_STATUS_FAILURE (-1), 
 *                       or MLAN_STATUS_SUCCESS (0)
 */
int
wlan_11h_process_join(mlan_private * priv,
                      t_u8 ** ppbuffer,
                      IEEEtypes_CapInfo_t * pcap_info,
                      t_u32 channel, wlan_11h_bss_info_t * p11h_bss_info)
{
    int ret = 0;

    ENTER();

    if (priv->media_connected == MTRUE) {
        if (wlan_11h_is_active(priv) == p11h_bss_info->sensed_11h) {
            /* Assume DFS parameters are the same for roaming as long as the
               current & next APs have the same spectrum mgmt capability bit
               setting */
            ret = MLAN_STATUS_SUCCESS;

        } else {
            /* No support for roaming between DFS/non-DFS yet */
            ret = MLAN_STATUS_FAILURE;
        }

        LEAVE();
        return ret;
    }

    if (p11h_bss_info->sensed_11h) {
        /* No use having 11h enabled without 11d enabled */
        wlan_11d_enable(priv, MNULL, ENABLE_11D);
        wlan_11d_parse_dnld_countryinfo(priv, priv->pattempted_bss_desc);

        /* Activate 11h functions in firmware, turns on capability bit */
        wlan_11h_activate(priv, MTRUE);
        pcap_info->spectrum_mgmt = 1;

        if (pcap_info->ibss) {
            PRINTM(MINFO, "11h: Adhoc join: Sensed\n");
            ret = wlan_11h_process_adhoc(priv, ppbuffer, channel,
                                         p11h_bss_info);
        } else {
            PRINTM(MINFO, "11h: Infra join: Sensed\n");
            ret = wlan_11h_process_infra_join(priv, ppbuffer,
                                              channel, p11h_bss_info);
        }
    } else {
        /* Deactivate 11h functions in the firmware */
        wlan_11h_activate(priv, MFALSE);
        pcap_info->spectrum_mgmt = 0;
    }

    LEAVE();

    return ret;
}

/**
 *
 *  @brief  Prepare the HostCmd_DS_Command structure for an 11h command.
 *
 *  Use the Command field to determine if the command being set up is for
 *     11h and call one of the local command handlers accordingly for:
 *
 *        - HostCmd_CMD_802_11_TPC_ADAPT_REQ
 *        - HostCmd_CMD_802_11_TPC_INFO
 *        - HostCmd_CMD_802_11_CHAN_SW_ANN
 *
 *  @param priv      Private driver information structure
 *  @param pcmd_ptr  Output parameter: Pointer to the command being prepared 
 *                   for the firmware
 *  @param pinfo_buf Void buffer pass through with data necessary for a
 *                   specific command type
 *
 *  @return          MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 *
 *  @sa wlan_11h_cmd_tpc_request
 *  @sa wlan_11h_cmd_tpc_info
 *  @sa wlan_11h_cmd_chan_sw_ann
 */
int
wlan_11h_cmd_process(mlan_private * priv,
                     HostCmd_DS_COMMAND * pcmd_ptr, const t_void * pinfo_buf)
{
    int ret = MLAN_STATUS_SUCCESS;

    ENTER();
    switch (pcmd_ptr->command) {
    case HostCmd_CMD_802_11_TPC_ADAPT_REQ:
        ret = wlan_11h_cmd_tpc_request(priv, pcmd_ptr, pinfo_buf);
        break;
    case HostCmd_CMD_802_11_TPC_INFO:
        ret = wlan_11h_cmd_tpc_info(priv, pcmd_ptr, pinfo_buf);
        break;
    case HostCmd_CMD_802_11_CHAN_SW_ANN:
        ret = wlan_11h_cmd_chan_sw_ann(priv, pcmd_ptr, pinfo_buf);
        break;
    default:
        ret = MLAN_STATUS_FAILURE;
    }

    pcmd_ptr->command = wlan_cpu_to_le16(pcmd_ptr->command);
    pcmd_ptr->size = wlan_cpu_to_le16(pcmd_ptr->size);
    LEAVE();
    return ret;
}

/**
 *  @brief Handle the command response from the firmware if from an 11h command
 *
 *  Use the Command field to determine if the command response being
 *    is for 11h.  Call the local command response handler accordingly for:
 *
 *        - HostCmd_CMD_802_11_TPC_ADAPT_REQ
 *        - HostCmd_CMD_802_11_TPC_INFO
 *        - HostCmd_CMD_802_11_CHAN_SW_ANN
 *
 *  @param priv  Private driver information structure
 *  @param resp  HostCmd_DS_COMMAND struct returned from the firmware
 *               command
 *
 *  @return      MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
int
wlan_11h_cmdresp_process(mlan_private * priv, const HostCmd_DS_COMMAND * resp)
{
    int ret = MLAN_STATUS_SUCCESS;

    ENTER();
    switch (resp->command) {
    case HostCmd_CMD_802_11_TPC_ADAPT_REQ:
        HEXDUMP("11h: TPC REQUEST Rsp:", (t_u8 *) resp, (int) resp->size);
        memcpy(priv->adapter, priv->adapter->curr_cmd->pdata_buf,
               &resp->params.tpc_req, sizeof(HostCmd_DS_802_11_TPC_ADAPT_REQ));
        break;

    case HostCmd_CMD_802_11_TPC_INFO:
        HEXDUMP("11h: TPC INFO Rsp Data:", (t_u8 *) resp, (int) resp->size);
        break;

    case HostCmd_CMD_802_11_CHAN_SW_ANN:
        PRINTM(MINFO, "11h: Ret ChSwAnn: Sz=%u, Seq=%u, Ret=%u\n",
               resp->size, resp->seq_num, resp->result);
        break;

    default:
        ret = MLAN_STATUS_FAILURE;
    }
    LEAVE();
    return ret;
}

/**
 *  @brief Process an element from a scan response, copy relevant info for 11h
 *
 *  @param pmadapter Pointer to mlan_adapter
 *  @param p11h_bss_info Output parameter: Pointer to the 11h BSS information
 *                       for the network that is being processed
 *  @param pelement      Pointer to the current IE we are inspecting for 11h
 *                       relevance
 *
 *  @return              MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
int
wlan_11h_process_bss_elem(mlan_adapter * pmadapter,
                          wlan_11h_bss_info_t * p11h_bss_info,
                          const t_u8 * pelement)
{
    int ret = MLAN_STATUS_SUCCESS;

    ENTER();
    switch (*pelement) {
    case POWER_CONSTRAINT:
        PRINTM(MINFO, "11h: Power Constraint IE Found\n");
        p11h_bss_info->sensed_11h = 1;
        memcpy(pmadapter, &p11h_bss_info->power_constraint, pelement,
               sizeof(IEEEtypes_PowerConstraint_t));
        break;

    case POWER_CAPABILITY:
        PRINTM(MINFO, "11h: Power Capability IE Found\n");
        p11h_bss_info->sensed_11h = 1;
        memcpy(pmadapter, &p11h_bss_info->power_capability, pelement,
               sizeof(IEEEtypes_PowerCapability_t));
        break;

    case TPC_REPORT:
        PRINTM(MINFO, "11h: Tpc Report IE Found\n");
        p11h_bss_info->sensed_11h = 1;
        memcpy(pmadapter, &p11h_bss_info->tpc_report, pelement,
               sizeof(IEEEtypes_TPCReport_t));
        break;

    case CHANNEL_SWITCH_ANN:
        p11h_bss_info->sensed_11h = 1;
        PRINTM(MINFO, "11h: Channel Switch Ann IE Found\n");
        break;

    case QUIET:
        PRINTM(MINFO, "11h: Quiet IE Found\n");
        p11h_bss_info->sensed_11h = 1;
        memcpy(pmadapter, &p11h_bss_info->quiet, pelement,
               sizeof(IEEEtypes_Quiet_t));
        break;

    case IBSS_DFS:
        PRINTM(MINFO, "11h: Ibss Dfs IE Found\n");
        p11h_bss_info->sensed_11h = 1;
        memcpy(pmadapter, &p11h_bss_info->ibss_dfs, pelement,
               sizeof(IEEEtypes_IBSS_DFS_t));
        break;

    case SUPPORTED_CHANNELS:
    case TPC_REQUEST:
        /* 
         * These elements are not in beacons/probe responses.  Included here
         *   to cover set of enumerated 11h elements.
         */
        break;

    default:
        ret = MLAN_STATUS_FAILURE;
    }
    LEAVE();
    return ret;
}
