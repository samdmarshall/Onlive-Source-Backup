/** @file moal_uap.h
  *
  * @brief This file contains uap driver specific defines etc.
  * 
  * Copyright (C) 2009, Marvell International Ltd.  
  *
  * This software file (the "File") is distributed by Marvell International 
  * Ltd. under the terms of the GNU General Public License Version 2, June 1991 
  * (the "License").  You may use, redistribute and/or modify this File in 
  * accordance with the terms and conditions of the License, a copy of which 
  * is available by writing to the Free Software Foundation, Inc.,
  * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA or on the
  * worldwide web at http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt.
  *
  * THE FILE IS DISTRIBUTED AS-IS, WITHOUT WARRANTY OF ANY KIND, AND THE 
  * IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE 
  * ARE EXPRESSLY DISCLAIMED.  The License provides additional details about 
  * this warranty disclaimer.
  *
  */

/********************************************************
Change log:
    02/02/2009: initial version
********************************************************/

#ifndef _MOAL_UAP_H
#define _MOAL_UAP_H

/** Private command ID to Host command */
#define	UAP_HOSTCMD			(SIOCDEVPRIVATE + 1)
/** Private command ID to send ioctl */
#define UAP_IOCTL_CMD		(SIOCDEVPRIVATE + 2)
/** Updating ADDBA variables */
#define UAP_ADDBA_PARA		0
/** Updating priority table for AMPDU/AMSDU */
#define UAP_AGGR_PRIOTBL    1
/** Updating addbareject table */
#define UAP_ADDBA_REJECT    2
#define UAP_DEEP_SLEEP      3
/** Private command ID to Power Mode */
#define	UAP_POWER_MODE			(SIOCDEVPRIVATE + 3)

/** Private command id to start/stop/reset bss */
#define UAP_BSS_CTRL        (SIOCDEVPRIVATE + 4)
/** BSS START */
#define UAP_BSS_START               0
/** BSS STOP */
#define UAP_BSS_STOP                1
/** BSS RESET */
#define UAP_BSS_RESET               2

/** Private command ID to BSS config */
#define	UAP_BSS_CONFIG          (SIOCDEVPRIVATE + 6)

/** deauth station */
#define	UAP_STA_DEAUTH	            (SIOCDEVPRIVATE + 7)

/** uap get station list */
#define UAP_GET_STA_LIST            (SIOCDEVPRIVATE + 11)

/** Private command ID to set/get custom IE buffer */
#define	UAP_CUSTOM_IE               (SIOCDEVPRIVATE + 13)

/** HS WAKE UP event id */
#define UAP_EVENT_ID_HS_WAKEUP             0x80000001
/** HS_ACTIVATED event id */
#define UAP_EVENT_ID_DRV_HS_ACTIVATED      0x80000002
/** HS DEACTIVATED event id */
#define UAP_EVENT_ID_DRV_HS_DEACTIVATED    0x80000003

/*Private command ID to set/get Host Sleep configuration */
#define UAP_HS_CFG                  (SIOCDEVPRIVATE + 14)
#define HS_CFG_FLAG_GET         0
#define HS_CFG_FLAG_SET         1
#define HS_CFG_FLAG_CONDITION   2
#define HS_CFG_FLAG_GPIO        4
#define HS_CFG_FLAG_GAP         8
#define HS_CFG_FLAG_ALL         0x0f
#define HS_CFG_CONDITION_MASK   0x0f
/** ds_hs_cfg */
typedef struct _ds_hs_cfg
{
    /** Bit0: 0 - Get, 1 Set
     *  Bit1: 1 - conditions is valid
     *  Bit2: 2 - gpio is valid
     *  Bit3: 3 - gap is valid
     */
    t_u32 flags;
    /** Host sleep config condition */
    /** Bit0: non-unicast data
     *  Bit1: unicast data
     *  Bit2: mac events
     *  Bit3: magic packet 
     */
    t_u32 conditions;
    /** GPIO */
    t_u32 gpio;
    /** Gap in milliseconds */
    t_u32 gap;
} ds_hs_cfg;

/** Private command ID to get BSS type */
#define	UAP_GET_BSS_TYPE            (SIOCDEVPRIVATE + 15)

/** addba_param */
typedef struct _addba_param
{
    /** subcmd */
    t_u32 subcmd;
    /** Set/Get */
    t_u32 action;
    /** block ack timeout for ADDBA request */
    t_u32 timeout;
    /** Buffer size for ADDBA request */
    t_u32 txwinsize;
    /** Buffer size for ADDBA response */
    t_u32 rxwinsize;
} addba_param;

/** aggr_prio_tbl */
typedef struct _aggr_prio_tbl
{
    /** subcmd */
    t_u32 subcmd;
    /** Set/Get */
    t_u32 action;
    /** ampdu priority table */
    t_u8 ampdu[MAX_NUM_TID];
    /** amsdu priority table */
    t_u8 amsdu[MAX_NUM_TID];
} aggr_prio_tbl;

/** addba_reject parameters */
typedef struct _addba_reject_para
{
    /** subcmd */
    t_u32 subcmd;
    /** Set/Get */
    t_u32 action;
    /** BA Reject paramters */
    t_u8 addba_reject[MAX_NUM_TID];
} addba_reject_para;

/** deep_sleep parameters */
typedef struct _deep_sleep_para
{
    /** subcmd */
    t_u32 subcmd;
    /** Set/Get */
    t_u32 action;
    /** enable/disable deepsleep*/
    t_u16 deep_sleep;
    /** idle_time */
    t_u16 idle_time;
} deep_sleep_para;

void woal_uap_set_multicast_list(struct net_device *dev);
int woal_uap_do_ioctl(struct net_device *dev, struct ifreq *req, int cmd);
void woal_uap_get_version(moal_private * priv, char *version, int max_len);
mlan_status woal_uap_get_stats(moal_private * priv, t_u8 wait_option,
                               mlan_ds_uap_stats * ustats);
#endif /* _MOAL_UAP_H */
