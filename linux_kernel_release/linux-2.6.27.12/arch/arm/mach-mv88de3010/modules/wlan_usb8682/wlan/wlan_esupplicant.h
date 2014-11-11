/** @file wlan_esupplicant.h
 * @brief This file contains related macros, enum, and struct
 * of esupplicant functionalities
 *
 * Copyright (C) 2003-2008, Marvell International Ltd. 
 * All Rights Reserved
 */
/****************************************************
Change log:
    03/21/08: add this file 
****************************************************/

#ifndef __WLAN_ESSUPLICANT_H
/** __WLAN_ESUPPLICANT_H */
#define __WLAN_ESUPPLICANT_H

/** Set the passphrase/pmk for a give SSID/BSSID */
int wlan_setpassphrase(wlan_private * priv, struct iwreq *wrq);
/** Return the rsnmode of current association */
int wlan_get_rsnmode_ioctl(wlan_private * priv, struct ifreq *req);
/** Return the current pairwise cipher of current association */
int wlan_get_currentpairwisecipher_ioctl(wlan_private * priv,
                                         struct ifreq *req);
/** Return the current group cipher of current association */
int wlan_get_currentgroupcipher_ioctl(wlan_private * priv, struct ifreq *req);
/** Command to send supplicant PMK command */
int wlan_cmd_802_11_supplicant_pmk(wlan_private * priv,
                                   HostCmd_DS_COMMAND * cmd, u16 option,
                                   void *buf);
/** Command to send supplicant profile command */
int wlan_cmd_802_11_supplicant_profile(wlan_private * priv,
                                       HostCmd_DS_COMMAND * cmd, u16 action,
                                       void *buf);
/** Command to process supplicant PMK response */
int wlan_ret_802_11_supplicant_profile(wlan_private * priv,
                                       HostCmd_DS_COMMAND * resp);
/** Command to process supplicant profile response */
int wlan_ret_802_11_supplicant_pmk(wlan_private * priv,
                                   HostCmd_DS_COMMAND * resp);
BOOLEAN isSTASecEnabled(wlan_private * priv, WLAN_802_11_SSID ssid, u8 * mac);

#endif /* __WLAN_ESUPPLICANT_H */
