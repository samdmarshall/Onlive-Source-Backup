/** @file mlan_11n.h
 *
 *  @brief Interface for the 802.11n mlan_11n module implemented in mlan_11n.c
 *
 *  Driver interface functions and type declarations for the 11n module
 *    implemented in mlan_11n.c.
 *
 *  Copyright (C) 2008-2009, Marvell International Ltd.
 *  All Rights Reserved
 */

/********************************************************
Change log:
    12/01/2008: initial version
********************************************************/

#ifndef _MLAN_11N_H_
#define _MLAN_11N_H_

#include "mlan_11n_aggr.h"
#include "mlan_11n_rxreorder.h"
#include "mlan_wmm.h"

/** Print the 802.11n device capability */
void wlan_show_dot11ndevcap(pmlan_adapter pmadapter, t_u32 cap);
/** Print the 802.11n device MCS */
void wlan_show_devmcssupport(pmlan_adapter pmadapter, t_u8 support);
/** Handle the command response of a delete block ack request */
mlan_status wlan_ret_11n_delba(mlan_private * priv, HostCmd_DS_COMMAND * resp);
/** Handle the command response of an add block ack request */
mlan_status wlan_ret_11n_addba_req(mlan_private * priv,
                                   HostCmd_DS_COMMAND * resp);
/** Handle the command response of 11ncfg command */
mlan_status wlan_ret_11n_cfg(IN pmlan_private pmpriv,
                             IN HostCmd_DS_COMMAND * resp,
                             IN mlan_ioctl_req * pioctl_buf);
/** Prepare 11ncfg command */
mlan_status wlan_cmd_11n_cfg(IN pmlan_private pmpriv,
                             IN HostCmd_DS_COMMAND * cmd, IN t_u16 cmd_action,
                             IN t_void * pdata_buf);
/** Append the 802_11N tlv */
int wlan_cmd_append_11n_tlv(IN mlan_private * pmpriv,
                            IN BSSDescriptor_t * pbss_desc,
                            OUT t_u8 ** ppbuffer);
/** Reconfigure the tx buf size in firmware */
void wlan_cfg_tx_buf(mlan_private * pmpriv, BSSDescriptor_t * pbss_desc);
/** Miscellaneous configuration handler */
mlan_status wlan_11n_cfg_ioctl(IN pmlan_adapter pmadapter,
                               IN pmlan_ioctl_req pioctl_req);
/** Delete Tx BA stream table entry */
void wlan_11n_delete_txbastream_tbl_entry(mlan_private * priv,
                                          TxBAStreamTbl * ptx_tbl);
/** Delete all Tx BA stream table entries */
void wlan_11n_deleteall_txbastream_tbl(mlan_private * priv);
/** Get Tx BA stream status */
TxBAStreamTbl *wlan_11n_get_txbastream_status(mlan_private * priv,
                                              baStatus_e ba_status);
/** Get Tx BA stream table */
TxBAStreamTbl *wlan_11n_get_txbastream_tbl(mlan_private * priv, int tid,
                                           t_u8 * ra);
/** Create Tx BA stream table */
void wlan_11n_create_txbastream_tbl(mlan_private * priv, t_u8 * ra, int tid,
                                    baStatus_e ba_status);
/** Send ADD BA request */
int wlan_send_addba(mlan_private * priv, int tid, t_u8 * peer_mac);
/** Send DEL BA request */
int wlan_send_delba(mlan_private * priv, int tid, t_u8 * peer_mac,
                    int initiator);
/** This function handles the command response of delete a block ack request*/
void wlan_11n_delete_bastream(mlan_private * priv, t_u8 * del_ba);
/** This function will resend addba request to all the peer in the TxBAStreamTbl */
void wlan_11n_update_addba_request(mlan_private * priv);
/** get rx reorder table */
int wlan_get_rxreorder_tbl(mlan_private * priv, rx_reorder_tbl * buf);
/** get tx ba stream table */
int wlan_get_txbastream_tbl(mlan_private * priv, tx_ba_stream_tbl * buf);
/** AMSDU Aggr control */
mlan_status wlan_ret_amsdu_aggr_ctrl(pmlan_private pmpriv,
                                     HostCmd_DS_COMMAND * resp,
                                     mlan_ioctl_req * pioctl_buf);
/** wlan fill cap info */
void wlan_fill_cap_info(mlan_adapter * pmadapter,
                        MrvlIETypes_HTCap_t * pht_cap);

/**
 *  @brief This function checks whether current BA stream is high priority or not
 *  
 *  @param priv     A pointer to mlan_private
 *  @param tid	    TID
 *
 *  @return 	    MTRUE or MFALSE
 */
static inline t_u8
wlan_is_cur_bastream_high_prio(mlan_private * priv, int tid)
{
    TxBAStreamTbl *ptx_tbl;

    ENTER();

    if (!
        (ptx_tbl =
         (TxBAStreamTbl *) util_peek_list(&priv->tx_ba_stream_tbl_ptr,
                                          priv->adapter->callbacks.
                                          moal_spin_lock,
                                          priv->adapter->callbacks.
                                          moal_spin_unlock)))
        return MNULL;

    while (ptx_tbl != (TxBAStreamTbl *) & priv->tx_ba_stream_tbl_ptr) {
        if (priv->aggr_prio_tbl[tid].ampdu_user >
            priv->aggr_prio_tbl[ptx_tbl->tid].ampdu_user) {
            LEAVE();
            return MTRUE;
        }

        ptx_tbl = ptx_tbl->pnext;
    }

    LEAVE();
    return MFALSE;
}

/**
 *  @brief This function checks whether AMPDU is allowed or not
 *  
 *  @param priv     A pointer to mlan_private
 *  @param ptr      A pointer to RA list table
 *
 *  @return 	    MTRUE or MFALSE
 */
static inline t_u8
wlan_is_ampdu_allowed(mlan_private * priv, raListTbl * ptr)
{
    return (((priv->aggr_prio_tbl[wlan_get_tid(priv->adapter, ptr)].ampdu_ap
              != BA_STREAM_NOT_ALLOWED) && IS_11N_ENABLED(priv))
            ? MTRUE : MFALSE);
}

/**
 *  @brief This function checks whether AMSDU is allowed or not
 *  
 *  @param priv     A pointer to mlan_private
 *  @param ptr      A pointer to RA list table
 *
 *  @return 	    MTRUE or MFALSE
 */
static inline t_u8
wlan_is_amsdu_allowed(mlan_private * priv, raListTbl * ptr)
{
    return (((priv->aggr_prio_tbl[wlan_get_tid(priv->adapter, ptr)].amsdu
              != BA_STREAM_NOT_ALLOWED) && IS_11N_ENABLED(priv))
            ? MTRUE : MFALSE);
}

/**
 *  @brief This function checks whether a BA stream is available or not
 *  
 *  @param priv     A pointer to mlan_private
 *
 *  @return 	    MTRUE or MFALSE
 */
static inline t_u8
wlan_is_bastream_avail(mlan_private * priv)
{
    return ((wlan_wmm_list_len
             (priv->adapter,
              (pmlan_list_head) & priv->tx_ba_stream_tbl_ptr) <
             MLAN_MAX_BASTREAM_SUPPORTED) ? MTRUE : MFALSE);
}

/**
 *  @brief This function finds the stream to delete
 *  
 *  @param priv     A pointer to mlan_private
 *  @param ptr      A pointer to RA list table
 *  @param ptid     A pointer to TID
 *  @param ra       RA
 *
 *  @return 	    MTRUE or MFALSE
 */
static inline t_u8
wlan_find_stream_to_delete(mlan_private * priv,
                           raListTbl * ptr, int *ptid, t_u8 * ra)
{
    int tid;
    t_u8 ret = MFALSE;
    TxBAStreamTbl *ptx_tbl;

    ENTER();

    if (!
        (ptx_tbl =
         (TxBAStreamTbl *) util_peek_list(&priv->tx_ba_stream_tbl_ptr,
                                          priv->adapter->callbacks.
                                          moal_spin_lock,
                                          priv->adapter->callbacks.
                                          moal_spin_unlock))) {
        LEAVE();
        return MNULL;
    }

    tid = priv->aggr_prio_tbl[wlan_get_tid(priv->adapter, ptr)].ampdu_user;

    while (ptx_tbl != (TxBAStreamTbl *) & priv->tx_ba_stream_tbl_ptr) {
        if (tid > priv->aggr_prio_tbl[ptx_tbl->tid].ampdu_user) {
            tid = priv->aggr_prio_tbl[ptx_tbl->tid].ampdu_user;
            *ptid = ptx_tbl->tid;
            memcpy(ra, ptx_tbl->ra, MLAN_MAC_ADDR_LENGTH);
            ret = MTRUE;
        }

        ptx_tbl = ptx_tbl->pnext;
    }

    LEAVE();
    return ret;
}

/**
 *  @brief This function checks whether BA stream is required or not
 *  
 *  @param priv     A pointer to mlan_private
 *  @param ptr      A pointer to RA list table
 *
 *  @return 	    MTRUE or MFALSE
 */
static inline t_u8
wlan_is_bastream_required(mlan_private * priv, raListTbl * ptr)
{
    TxBAStreamTbl *ptx_tbl;

    ENTER();

    if (!wlan_is_ampdu_allowed(priv, ptr)) {
        LEAVE();
        return MFALSE;
    }

    if (!
        (ptx_tbl =
         (TxBAStreamTbl *) util_peek_list(&priv->tx_ba_stream_tbl_ptr,
                                          priv->adapter->callbacks.
                                          moal_spin_lock,
                                          priv->adapter->callbacks.
                                          moal_spin_unlock))) {
        LEAVE();
        return MTRUE;
    }

    if ((ptx_tbl != (TxBAStreamTbl *) & priv->tx_ba_stream_tbl_ptr) ||
        wlan_is_bastream_avail(priv) ||
        wlan_is_cur_bastream_high_prio(priv,
                                       wlan_get_tid(priv->adapter, ptr))) {
        LEAVE();
        return MTRUE;
    }

    LEAVE();
    return MFALSE;
}

/**
 *  @brief This function checks whether BA stream is setup
 *  
 *  @param priv     A pointer to mlan_private
 *  @param ptr      A pointer to RA list table
 *
 *  @return 	    MTRUE or MFALSE
 */
static int inline
wlan_is_bastream_setup(mlan_private * priv, raListTbl * ptr)
{
    TxBAStreamTbl *ptx_tbl;

    ENTER();

    if ((ptx_tbl = wlan_11n_get_txbastream_tbl(priv,
                                               wlan_get_tid(priv->adapter, ptr),
                                               ptr->ra))) {
        LEAVE();
        return IS_BASTREAM_SETUP(ptx_tbl) ? MTRUE : MFALSE;
    }

    LEAVE();
    return MFALSE;
}
#endif /* !_MLAN_11N_H_ */
