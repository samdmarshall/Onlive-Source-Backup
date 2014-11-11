/** @file mlan_11n_aggr.h
 *
 *  @brief This file contains related macros, enum, and struct
 *  of 11n aggregation functionalities
 *
 *  Copyright (C) 2008-2009, Marvell International Ltd.
 *  All Rights Reserved
 */

/********************************************************
Change log:
    11/10/2008: initial version
********************************************************/

#ifndef _MLAN_11N_AGGR_H_
#define _MLAN_11N_AGGR_H_

/** AMSDU packet type */
#define PKT_TYPE_AMSDU	0xE6

/** Aggregate 11N packets */
mlan_status wlan_11n_deaggregate_pkt(pmlan_private priv, pmlan_buffer pmbuf);
/** Deaggregate 11N packets */
int wlan_11n_aggregate_pkt(mlan_private * priv, raListTbl * ptr,
                           int headroom, int ptrindex);

#endif /* !_MLAN_11N_AGGR_H_ */
