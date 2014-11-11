/** @file mlan_11h.h
 *
 *  @brief This header file contains data structures and 
 *  function declarations of 802.11h
 *
 *  Copyright (C) 2008-2010, Marvell International Ltd. 
 *  All Rights Reserved
 */

/*************************************************************
Change Log:
    03/26/2009: initial creation
*************************************************************/

#ifndef _MLAN_11H_
#define _MLAN_11H_

/**
 *  11H APIs
 */

/** Initialize the 11h software module */
extern void wlan_11h_init(mlan_adapter * pmadapter);

/** Return 1 if 11h is active in the firmware, 0 if it is inactive */
extern int wlan_11h_is_active(mlan_private * priv);

/** Activate 11h extensions in the firmware */
extern int wlan_11h_activate(mlan_private * priv, t_u32 flag);

/** Enable the tx interface and record the new transmit state */
extern void wlan_11h_tx_enable(mlan_private * priv);

/** Enable the tx interface and record the new transmit state */
extern void wlan_11h_tx_disable(mlan_private * priv);

/** Check if radar detection is required on the specified channel */
extern int wlan_11h_radar_detect_required(mlan_private * priv, t_u8 channel);

/** Perform a standard availibility check on the specified channel */
extern int wlan_11h_radar_detected(mlan_private * priv,
                                   pmlan_ioctl_req pioctl_req, t_u8 channel);

/** Get an initial random channel to start an adhoc network on */
extern int wlan_11h_get_adhoc_start_channel(mlan_private * priv);

/** Add any 11h TLVs necessary to complete a join command (adhoc or infra) */
extern int wlan_11h_process_join(mlan_private * priv,
                                 t_u8 ** ppbuffer,
                                 IEEEtypes_CapInfo_t * pcap_info,
                                 t_u32 channel,
                                 wlan_11h_bss_info_t * p11h_bss_info);

/** Add any 11h TLVs necessary to complete an adhoc start command */
extern int wlan_11h_process_start(mlan_private * priv,
                                  t_u8 ** ppbuffer,
                                  IEEEtypes_CapInfo_t * pcap_info,
                                  t_u32 channel,
                                  wlan_11h_bss_info_t * p11h_bss_info);

/** Receive IEs from scan processing and record any needed info for 11h */
int wlan_11h_process_bss_elem(mlan_adapter * pmadapter,
                              wlan_11h_bss_info_t * p11h_bss_info,
                              const t_u8 * pelement);

/** Complete the firmware command preparation for an 11h command function */
extern int wlan_11h_cmd_process(mlan_private * priv,
                                HostCmd_DS_COMMAND * pcmd_ptr,
                                const t_void * pinfo_buf);

/** Process the response of an 11h firmware command */
extern int wlan_11h_cmdresp_process(mlan_private * priv,
                                    const HostCmd_DS_COMMAND * resp);

#endif /*_MLAN_11H_ */
