/** @file mlan_wmm.h
 *
 *  @brief This file contains related macros, enum, and struct
 *  of wmm functionalities
 *
 *  Copyright (C) 2008-2009, Marvell International Ltd. 
 *  All Rights Reserved
 */

/****************************************************
Change log:
    10/24/2008: initial version
****************************************************/

#ifndef _MLAN_WMM_H_
#define _MLAN_WMM_H_

/**
 *  @brief This function gets the TID
 *  
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param ptr          A pointer to RA list table
 *
 *  @return 	    TID
 */
static inline int
wlan_get_tid(pmlan_adapter pmadapter, raListTbl * ptr)
{
    pmlan_buffer mbuf;

    ENTER();

    mbuf = (pmlan_buffer) util_peek_list(&ptr->buf_head,
                                         pmadapter->callbacks.moal_spin_lock,
                                         pmadapter->callbacks.moal_spin_unlock);
    LEAVE();
    return mbuf->priority;
}

/**
 *  @brief This function gets the length of a list
 *  
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param head         A pointer to mlan_list_head
 *
 *  @return 	    Length of list
 */
static inline int
wlan_wmm_list_len(pmlan_adapter pmadapter, pmlan_list_head head)
{
    pmlan_linked_list pos;
    int count = 0;

    ENTER();

    pos = head->pnext;

    while (pos != (pmlan_linked_list) head) {
        ++count;
        pos = pos->pnext;
    }

    LEAVE();
    return count;
}

/** Allocate RA list node */
raListTbl *wlan_wmm_allocate_ralist_node(pmlan_adapter pmadapter, t_u8 * ra);
/** Add buffer to WMM Tx queue */
void wlan_wmm_add_buf_txqueue(pmlan_adapter pmadapter, pmlan_buffer pmbuf);
/** Delete all RA lists */
void wlan_wmm_delete_all_ralist(pmlan_private priv);
/** Add to RA list */
void wlan_ralist_add(mlan_private * priv, t_u8 * ra);

/** Map and add WMM buffer */
extern t_void wmm_map_and_add_buf(pmlan_adapter pmadapter, pmlan_buffer pmbuf);

/** Clean up WMM node */
void wlan_wmm_cleanup_node(pmlan_private priv);
/** WMM status change command handler */
extern int wlan_cmd_wmm_status_change(pmlan_private priv);
/** Check if WMM lists are empty */
extern int wlan_wmm_lists_empty(pmlan_adapter pmadapter);
/** Clean up WMM queues */
extern void wlan_wmm_cleanup_queues(pmlan_private priv);
/** Process WMM transmission */
extern void wlan_wmm_process_tx(pmlan_adapter pmadapter);
/** Test to see if the ralist ptr is valid */
int wlan_is_ralist_valid(mlan_private * priv, raListTbl * ra_list, int tid);

/** Initialize WMM */
extern void wlan_wmm_init(pmlan_adapter pmadapter);
/** Setup WMM queues */
extern void wlan_wmm_setup_queues(pmlan_private priv);
/** Check if WMM queue is stopped */
extern int wlan_wmm_is_queue_stopped(pmlan_adapter pmadapter);

/** Process WMM association request */
extern t_u32 wlan_wmm_process_association_req(pmlan_private priv,
                                              t_u8 ** ppAssocBuf,
                                              IEEEtypes_WmmParameter_t * pWmmIE,
                                              IEEEtypes_HTCap_t * pHTCap);

/* 
 *  Functions used in the cmd handling routine
 */
/** WMM get status command handler */
extern int wlan_cmd_wmm_get_status(pmlan_private priv,
                                   HostCmd_DS_COMMAND * cmd, t_void * InfoBuf);

/* 
 *  Functions used in the cmdresp handling routine
 */
/** WMM get status command response handler */
extern mlan_status wlan_cmdresp_wmm_get_status(pmlan_private priv,
                                               const HostCmd_DS_COMMAND * resp);

#endif /* !_MLAN_WMM_H_ */
