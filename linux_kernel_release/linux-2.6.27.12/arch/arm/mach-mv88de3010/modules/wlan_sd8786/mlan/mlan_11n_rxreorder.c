/** @file mlan_11n_rxreorder.c
 *
 *  @brief This file contains the handling of RxReordering in wlan
 *  driver.
 * 
 *  Copyright (C) 2008-2009, Marvell International Ltd. 
 *  All Rights Reserved
 */

/********************************************************
Change log:
    11/10/2008: initial version
********************************************************/

#include "mlan.h"
#include "mlan_11d.h"
#include "mlan_join.h"
#include "mlan_scan.h"
#include "mlan_util.h"
#include "mlan_fw.h"
#include "mlan_main.h"
#include "mlan_wmm.h"
#include "mlan_11n.h"
#include "mlan_11n_rxreorder.h"

/********************************************************
    Local Variables
********************************************************/
/** ADDBA response status : Reject */
#define ADDBA_RSP_STATUS_REJECT 1
/** ADDBA response status : Accept */
#define ADDBA_RSP_STATUS_ACCEPT 0

/********************************************************
    Global Variables
********************************************************/

/********************************************************
    Local Functions
********************************************************/

/**
 *  @brief This function will process the rx packet and 
 *  		forward it to kernel/upper layer
 *  
 *  @param priv    	A pointer to mlan_private
 *  @param payload  A pointer to rx packet payload
 *
 *  @return 	   	MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status
wlan_11n_dispatch_pkt(t_void * priv, t_void * payload)
{
    pmlan_adapter pmadapter = ((pmlan_private) priv)->adapter;

    return wlan_process_rx_packet(pmadapter, (pmlan_buffer) payload);
}

/**
 *  @brief This function dispatchs all the packets in the buffer.
 *  		There could be holes in the buffer.
 *    
 *  @param priv    	        A pointer to mlan_private
 *  @param rx_reor_tbl_ptr  A pointer to structure RxReorderTbl
 *  @param start_win        Start window
 *
 *  @return 	   	        MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status
wlan_11n_dispatch_pkt_until_start_win(t_void * priv,
                                      RxReorderTbl * rx_reor_tbl_ptr,
                                      int start_win)
{
    int no_pkt_to_send, i, xchg;
    mlan_status ret = MLAN_STATUS_SUCCESS;

    ENTER();

    no_pkt_to_send = (start_win > rx_reor_tbl_ptr->start_win) ?
        MIN((start_win - rx_reor_tbl_ptr->start_win),
            rx_reor_tbl_ptr->win_size) : rx_reor_tbl_ptr->win_size;

    for (i = 0; i < no_pkt_to_send; ++i) {
        if (rx_reor_tbl_ptr->rx_reorder_ptr[i]) {
            wlan_11n_dispatch_pkt(priv, rx_reor_tbl_ptr->rx_reorder_ptr[i]);
            rx_reor_tbl_ptr->rx_reorder_ptr[i] = MNULL;
        }
    }

    /* 
     * We don't have a circular buffer, hence use rotation to simulate
     * circular buffer
     */
    xchg = rx_reor_tbl_ptr->win_size - no_pkt_to_send;
    for (i = 0; i < xchg; ++i) {
        rx_reor_tbl_ptr->rx_reorder_ptr[i] =
            rx_reor_tbl_ptr->rx_reorder_ptr[no_pkt_to_send + i];
        rx_reor_tbl_ptr->rx_reorder_ptr[no_pkt_to_send + i] = MNULL;
    }

    rx_reor_tbl_ptr->start_win = start_win;

    LEAVE();
    return ret;
}

/**
 *  @brief This function will display the rxReorder table
 *      
 *  @param pmadapter          A pointer to mlan_adapter structure
 *  @param rx_reor_tbl_ptr    A pointer to structure RxReorderTbl
 *
 *  @return 	   	N/A
 */
static t_void
wlan_11n_display_tbl_ptr(pmlan_adapter pmadapter,
                         RxReorderTbl * rx_reor_tbl_ptr)
{
    ENTER();

    DBG_HEXDUMP(MDAT_D, "Reorder ptr", rx_reor_tbl_ptr->rx_reorder_ptr,
                rx_reor_tbl_ptr->win_size);

    LEAVE();
}

/**
 *  @brief This function will dispatch all packets sequentially 
 *  		from start_win until a hole is found and adjust the 
 *  		start_win appropriately
 *
 *  @param priv    	        A pointer to mlan_private
 *  @param rx_reor_tbl_ptr  A pointer to structure RxReorderTbl
 *
 *  @return 	   	        MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status
wlan_11n_scan_and_dispatch(t_void * priv, RxReorderTbl * rx_reor_tbl_ptr)
{
    int i, j, xchg;
    mlan_status ret = MLAN_STATUS_SUCCESS;

    ENTER();

    for (i = 0; i < rx_reor_tbl_ptr->win_size; ++i) {
        if (!rx_reor_tbl_ptr->rx_reorder_ptr[i])
            break;

        wlan_11n_dispatch_pkt(priv, rx_reor_tbl_ptr->rx_reorder_ptr[i]);
        rx_reor_tbl_ptr->rx_reorder_ptr[i] = MNULL;
    }

    /* 
     * We don't have a circular buffer, hence use rotation to simulate
     * circular buffer
     */
    if (i > 0) {
        xchg = rx_reor_tbl_ptr->win_size - i;
        for (j = 0; j < xchg; ++j) {
            rx_reor_tbl_ptr->rx_reorder_ptr[j] =
                rx_reor_tbl_ptr->rx_reorder_ptr[i + j];
            rx_reor_tbl_ptr->rx_reorder_ptr[i + j] = MNULL;
        }
    }
    rx_reor_tbl_ptr->start_win = (rx_reor_tbl_ptr->start_win + i)
        % MAX_TID_VALUE;

    LEAVE();
    return ret;
}

/**
 *  @brief This function delete rxreorder table's entry 
 *         	and free the memory
 *
 *  @param priv    	        A pointer to mlan_private
 *  @param rx_reor_tbl_ptr  A pointer to structure RxReorderTbl
 *
 *  @return 	   	        N/A
 */
static t_void
wlan_11n_delete_rxreorder_tbl_entry(mlan_private * priv,
                                    RxReorderTbl * rx_reor_tbl_ptr)
{
    pmlan_adapter pmadapter = priv->adapter;

    ENTER();

    if (!rx_reor_tbl_ptr) {
        LEAVE();
        return;
    }

    wlan_11n_dispatch_pkt_until_start_win(priv, rx_reor_tbl_ptr,
                                          (rx_reor_tbl_ptr->start_win +
                                           rx_reor_tbl_ptr->win_size)
                                          % MAX_TID_VALUE);

    if (rx_reor_tbl_ptr->timer_context.timer) {
        if (rx_reor_tbl_ptr->timer_context.timer_is_set)
            priv->adapter->callbacks.moal_stop_timer(rx_reor_tbl_ptr->
                                                     timer_context.timer);
        priv->adapter->callbacks.moal_free_timer(rx_reor_tbl_ptr->
                                                 timer_context.timer);
    }

    PRINTM(MDAT_D, "Delete rx_reor_tbl_ptr: %p\n", rx_reor_tbl_ptr);
    util_unlink_list(&priv->rx_reorder_tbl_ptr,
                     (pmlan_linked_list) rx_reor_tbl_ptr,
                     pmadapter->callbacks.moal_spin_lock,
                     pmadapter->callbacks.moal_spin_unlock);

    pmadapter->callbacks.moal_mfree((t_u8 *) rx_reor_tbl_ptr->rx_reorder_ptr);
    pmadapter->callbacks.moal_mfree((t_u8 *) rx_reor_tbl_ptr);

    LEAVE();
}

/**
 *  @brief This function will return the pointer to a entry in rx reordering 
 *  		table which matches the give TA/TID pair
 *  
 *  @param priv    A pointer to mlan_private
 *  @param ta      ta to find in reordering table
 *  @param tid	   tid to find in reordering table
 *
 *  @return	   A pointer to structure RxReorderTbl
 */
RxReorderTbl *
wlan_11n_get_rxreorder_tbl(mlan_private * priv, int tid, t_u8 * ta)
{
    RxReorderTbl *rx_reor_tbl_ptr;

    ENTER();

    if (!
        (rx_reor_tbl_ptr =
         (RxReorderTbl *) util_peek_list(&priv->rx_reorder_tbl_ptr,
                                         priv->adapter->callbacks.
                                         moal_spin_lock,
                                         priv->adapter->callbacks.
                                         moal_spin_unlock))) {
        LEAVE();
        return MNULL;
    }

    while (rx_reor_tbl_ptr != (RxReorderTbl *) & priv->rx_reorder_tbl_ptr) {
        if ((!memcmp(rx_reor_tbl_ptr->ta, ta, MLAN_MAC_ADDR_LENGTH)) &&
            (rx_reor_tbl_ptr->tid == tid)) {
            LEAVE();
            return rx_reor_tbl_ptr;
        }

        rx_reor_tbl_ptr = rx_reor_tbl_ptr->pnext;
    }

    LEAVE();
    return MNULL;
}

static int
wlan_11n_find_last_seqnum(RxReorderTbl * rxReorderTblPtr)
{
    int i;

    for (i = (rxReorderTblPtr->win_size - 1); i >= 0; --i) {
        if (rxReorderTblPtr->rx_reorder_ptr[i]) {
            return i;
        }
    }

    return -1;
}

static t_void
wlan_flush_data(t_void * context)
{
    reorder_tmr_cnxt_t *reorder_cnxt = (reorder_tmr_cnxt_t *) context;
    int startWin;

    reorder_cnxt->timer_is_set = MFALSE;
    wlan_11n_display_tbl_ptr(reorder_cnxt->priv->adapter, reorder_cnxt->ptr);

    if ((startWin = wlan_11n_find_last_seqnum(reorder_cnxt->ptr)) >= 0) {
        PRINTM(MINFO, "Flush data %d\n", startWin);
        wlan_11n_dispatch_pkt_until_start_win(reorder_cnxt->priv,
                                              reorder_cnxt->ptr,
                                              ((reorder_cnxt->ptr->start_win +
                                                startWin + 1) % MAX_TID_VALUE));
    }

    wlan_11n_display_tbl_ptr(reorder_cnxt->priv->adapter, reorder_cnxt->ptr);
}

/**
 *  @brief This function will create a entry in rx reordering table for the 
 *  		given ta/tid and will initialize it with seq_num, win_size
 *  
 *  @param priv     A pointer to mlan_private
 *  @param ta       ta to find in reordering table
 *  @param tid	    tid to find in reordering table
 *  @param win_size win_size for the give ta/tid pair.
 *  @param seq_num  Starting sequence number for current entry.
 *
 *  @return 	    N/A
 */
t_void
wlan_11n_create_rxreorder_tbl(mlan_private * priv, t_u8 * ta, int tid,
                              int win_size, int seq_num)
{
    int i;
    pmlan_adapter pmadapter = priv->adapter;
    RxReorderTbl *rx_reor_tbl_ptr, *new_node;

    ENTER();

    /* 
     * If we get a TID, ta pair which is already present dispatch all the
     * the packets and move the window size until the ssn
     */
    if ((rx_reor_tbl_ptr = wlan_11n_get_rxreorder_tbl(priv, tid, ta))) {
        wlan_11n_dispatch_pkt_until_start_win(priv, rx_reor_tbl_ptr, seq_num);
    } else {
        PRINTM(MDAT_D, "%s: seq_num %d, tid %d, ta %02x:%02x:%02x:%02x:"
               "%02x:%02x, win_size %d\n", __FUNCTION__,
               seq_num, tid, ta[0], ta[1], ta[2], ta[3],
               ta[4], ta[5], win_size);
        if (pmadapter->callbacks.moal_malloc(sizeof(RxReorderTbl),
                                             (t_u8 **) & new_node)) {
            PRINTM(MERROR, "Rx reorder memory allocation failed\n");
            return;
        }

        util_init_list((pmlan_linked_list) new_node);
        new_node->tid = tid;
        memcpy(new_node->ta, ta, MLAN_MAC_ADDR_LENGTH);
        new_node->start_win = seq_num;
        new_node->win_size = win_size;

        if (pmadapter->callbacks.moal_malloc(sizeof(t_void *) * win_size,
                                             (t_u8 **) & new_node->
                                             rx_reorder_ptr)) {
            PRINTM(MERROR, "Rx reorder table memory allocation" "failed\n");
            pmadapter->callbacks.moal_mfree((t_u8 *) new_node);
            return;
        }

        PRINTM(MDAT_D, "Create ReorderPtr: %p\n", new_node);
        new_node->timer_context.ptr = new_node;
        new_node->timer_context.priv = priv;
        new_node->timer_context.timer_is_set = MFALSE;

        pmadapter->callbacks.moal_init_timer(&new_node->timer_context.
                                             timer, wlan_flush_data,
                                             &new_node->timer_context);

        for (i = 0; i < win_size; ++i)
            new_node->rx_reorder_ptr[i] = MNULL;

        util_enqueue_list_tail(&priv->rx_reorder_tbl_ptr,
                               (pmlan_linked_list) new_node,
                               pmadapter->callbacks.moal_spin_lock,
                               pmadapter->callbacks.moal_spin_unlock);
    }

    LEAVE();
}

/********************************************************
    Global Functions
********************************************************/

/** 
  *  @brief This function prepares command for adding a block ack
  *  		request.
  *  
  *  @param priv        A pointer to mlan_private structure
  *  @param cmd         A pointer to HostCmd_DS_COMMAND structure
  *  @param pdata_buf   A pointer to data buffer
  *
  *  @return            MLAN_STATUS_SUCCESS
  */
mlan_status
wlan_cmd_11n_addba_req(mlan_private * priv,
                       HostCmd_DS_COMMAND * cmd, t_void * pdata_buf)
{
    HostCmd_DS_11N_ADDBA_REQ *padd_ba_req = (HostCmd_DS_11N_ADDBA_REQ *)
        & cmd->params.add_ba_req;
    ENTER();

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_11N_ADDBA_REQ);
    cmd->size = wlan_cpu_to_le16(sizeof(HostCmd_DS_11N_ADDBA_REQ) + S_DS_GEN);

    memcpy(padd_ba_req, pdata_buf, sizeof(HostCmd_DS_11N_ADDBA_REQ));
    padd_ba_req->block_ack_param_set =
        wlan_cpu_to_le16(padd_ba_req->block_ack_param_set);
    padd_ba_req->block_ack_tmo = wlan_cpu_to_le16(padd_ba_req->block_ack_tmo);
    padd_ba_req->ssn = wlan_cpu_to_le16(padd_ba_req->ssn);

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
  *  @brief This function prepares command for adding a block ack
  *  		response.
  *  
  *  @param priv        A pointer to mlan_private structure
  *  @param cmd         A pointer to HostCmd_DS_COMMAND structure
  *  @param pdata_buf   A pointer to data buffer
  *
  *  @return            MLAN_STATUS_SUCCESS
  */
mlan_status
wlan_cmd_11n_addba_rspgen(mlan_private * priv,
                          HostCmd_DS_COMMAND * cmd, void *pdata_buf)
{
    HostCmd_DS_11N_ADDBA_RSP *padd_ba_rsp = (HostCmd_DS_11N_ADDBA_RSP *)
        & cmd->params.add_ba_rsp;
    HostCmd_DS_11N_ADDBA_REQ *pevt_addba_req =
        (HostCmd_DS_11N_ADDBA_REQ *) pdata_buf;
    t_u8 tid;

    ENTER();

    pevt_addba_req->block_ack_param_set =
        wlan_le16_to_cpu(pevt_addba_req->block_ack_param_set);
    pevt_addba_req->block_ack_tmo =
        wlan_le16_to_cpu(pevt_addba_req->block_ack_tmo);
    pevt_addba_req->ssn = wlan_le16_to_cpu(pevt_addba_req->ssn);

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_11N_ADDBA_RSP);
    cmd->size = wlan_cpu_to_le16(sizeof(HostCmd_DS_11N_ADDBA_RSP) + S_DS_GEN);

    memcpy(padd_ba_rsp->peer_mac_addr, pevt_addba_req->peer_mac_addr,
           MLAN_MAC_ADDR_LENGTH);
    padd_ba_rsp->dialog_token = pevt_addba_req->dialog_token;
    padd_ba_rsp->block_ack_tmo =
        wlan_cpu_to_le16(pevt_addba_req->block_ack_tmo);
    padd_ba_rsp->ssn = wlan_cpu_to_le16(pevt_addba_req->ssn);

    tid = (padd_ba_rsp->block_ack_param_set & BLOCKACKPARAM_TID_MASK)
        >> BLOCKACKPARAM_TID_POS;
    padd_ba_rsp->status_code = wlan_cpu_to_le16(priv->addba_reject[tid]);
    padd_ba_rsp->block_ack_param_set = pevt_addba_req->block_ack_param_set;
    padd_ba_rsp->block_ack_param_set &= ~BLOCKACKPARAM_WINSIZE_MASK;
    /* We donot support AMSDU inside AMPDU, hence reset the bit */
    padd_ba_rsp->block_ack_param_set &= ~BLOCKACKPARAM_AMSDU_SUPP_MASK;
    padd_ba_rsp->block_ack_param_set |= (priv->add_ba_param.rx_win_size <<
                                         BLOCKACKPARAM_WINSIZE_POS);
    padd_ba_rsp->block_ack_param_set =
        wlan_cpu_to_le16(padd_ba_rsp->block_ack_param_set);
    padd_ba_rsp->block_ack_tmo = wlan_cpu_to_le16(priv->add_ba_param.timeout);

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
  *  @brief This function prepares command for deleting a block ack
  *  		request.
  *  
  *  @param priv       A pointer to mlan_private structure
  *  @param cmd        A pointer to HostCmd_DS_COMMAND structure
  *  @param pdata_buf  A pointer to data buffer
  *
  *  @return           MLAN_STATUS_SUCCESS
  */
mlan_status
wlan_cmd_11n_delba(mlan_private * priv,
                   HostCmd_DS_COMMAND * cmd, void *pdata_buf)
{
    HostCmd_DS_11N_DELBA *pdel_ba = (HostCmd_DS_11N_DELBA *)
        & cmd->params.del_ba;

    ENTER();

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_11N_DELBA);
    cmd->size = wlan_cpu_to_le16(sizeof(HostCmd_DS_11N_DELBA) + S_DS_GEN);

    memcpy(pdel_ba, pdata_buf, sizeof(HostCmd_DS_11N_DELBA));
    pdel_ba->del_ba_param_set = wlan_cpu_to_le16(pdel_ba->del_ba_param_set);
    pdel_ba->reason_code = wlan_cpu_to_le16(pdel_ba->reason_code);

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function will identify if RxReodering is needed for the packet
 *  		and will do the reordering if required before sending it to kernel
 *  
 *  @param priv     A pointer to mlan_private
 *  @param seq_num  Seqence number of the current packet
 *  @param tid	    Tid of the current packet
 *  @param ta	    Transmiter address of the current packet
 *  @param pkt_type Packetype for the current packet (to identify if its a BAR)
 *  @param payload  Pointer to the payload
 *
 *  @return 	    MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
mlan_11n_rxreorder_pkt(void *priv, t_u16 seq_num, t_u16 tid,
                       t_u8 * ta, t_u8 pkt_type, void *payload)
{
    RxReorderTbl *rx_reor_tbl_ptr;
    int start_win, end_win, win_size;
    mlan_status ret = MLAN_STATUS_SUCCESS;
    pmlan_adapter pmadapter = ((mlan_private *) priv)->adapter;

    ENTER();

    if (!
        (rx_reor_tbl_ptr =
         wlan_11n_get_rxreorder_tbl((mlan_private *) priv, tid, ta))) {
        LEAVE();
        if (pkt_type == PKT_TYPE_BAR)
            return ret;
        else {
            wlan_11n_dispatch_pkt(priv, payload);
            return ret;
        }
    } else {
        if (pkt_type == PKT_TYPE_BAR)
            PRINTM(MDAT_D, "BAR ");

        start_win = rx_reor_tbl_ptr->start_win;
        win_size = rx_reor_tbl_ptr->win_size;
        end_win = ((start_win + win_size) - 1) & ~(MAX_TID_VALUE);
        if (rx_reor_tbl_ptr->timer_context.timer_is_set)
            pmadapter->callbacks.moal_stop_timer(rx_reor_tbl_ptr->timer_context.
                                                 timer);
        pmadapter->callbacks.moal_start_timer(rx_reor_tbl_ptr->timer_context.
                                              timer, MFALSE,
                                              MIN_FLUSH_TIMER_MS * win_size);
        rx_reor_tbl_ptr->timer_context.timer_is_set = MTRUE;

        PRINTM(MDAT_D, "TID %d, TA %02x:%02x:%02x:%02x:%02x:%02x\n",
               tid, ta[0], ta[1], ta[2], ta[3], ta[4], ta[5]);
        PRINTM(MDAT_D, "1:seq_num %d start_win %d win_size %d end_win %d\n",
               seq_num, start_win, win_size, end_win);
        /* 
         * If seq_num is less then starting win then ignore and drop the
         * packet
         */
        if ((start_win + TWOPOW11) > (MAX_TID_VALUE - 1)) {     /* Wrap */
            if (seq_num >= ((start_win + (TWOPOW11)) &
                            ~(MAX_TID_VALUE)) && (seq_num < start_win)) {
                LEAVE();
                return MLAN_STATUS_FAILURE;
            }
        } else if ((seq_num < start_win) ||
                   (seq_num > (start_win + (TWOPOW11)))) {
            LEAVE();
            return MLAN_STATUS_FAILURE;
        }

        /* 
         * If this packet is a BAR we adjust seq_num as
         * WinStart = seq_num
         */
        if (pkt_type == PKT_TYPE_BAR)
            seq_num = ((seq_num + win_size) - 1) & ~(MAX_TID_VALUE);

        PRINTM(MDAT_D, "2:seq_num %d start_win %d win_size %d end_win %d\n",
               seq_num, start_win, win_size, end_win);

        if (((end_win < start_win) && (seq_num < (TWOPOW11 -
                                                  (MAX_TID_VALUE - start_win)))
             && (seq_num > end_win))
            || ((end_win > start_win) &&
                ((seq_num > end_win) || (seq_num < start_win)))) {
            end_win = seq_num;
            if (((seq_num - win_size) + 1) >= 0)
                start_win = (end_win - win_size) + 1;
            else
                start_win = (MAX_TID_VALUE - (win_size - seq_num)) + 1;
            if ((ret = wlan_11n_dispatch_pkt_until_start_win(priv,
                                                             rx_reor_tbl_ptr,
                                                             start_win))) {
                LEAVE();
                return ret;
            }
        }

        PRINTM(MDAT_D, "3:seq_num %d start_win %d win_size %d"
               " end_win %d\n", seq_num, start_win, win_size, end_win);
        if (pkt_type != PKT_TYPE_BAR) {
            if (seq_num >= start_win)
                rx_reor_tbl_ptr->rx_reorder_ptr[seq_num - start_win] = payload;
            else                /* Wrap condition */
                rx_reor_tbl_ptr->rx_reorder_ptr[(seq_num
                                                 + (MAX_TID_VALUE)) -
                                                start_win] = payload;
        }

        wlan_11n_display_tbl_ptr(pmadapter, rx_reor_tbl_ptr);

        /* 
         * Dispatch all packets sequentially from start_win until a
         * hole is found and adjust the start_win appropriately
         */
        ret = wlan_11n_scan_and_dispatch(priv, rx_reor_tbl_ptr);

        wlan_11n_display_tbl_ptr(pmadapter, rx_reor_tbl_ptr);
    }

    LEAVE();
    return ret;
}

/**
 *  @brief This function will delete an entry for a given tid/ta pair. tid/ta
 *  		are taken from delba_event body
 *  
 *  @param priv    	    A pointer to mlan_private
 *  @param tid		    tid to send delba
 *  @param peer_mac	    MAC address to send delba
 *  @param type 	    TYPE_DELBA_SENT	or TYPE_DELBA_RECEIVE	
 *  @param initiator    MTRUE if we are initiator of ADDBA, MFALSE otherwise
 *
 *  @return 	   	    N/A
 */
void
mlan_11n_delete_bastream_tbl(mlan_private * priv, int tid,
                             t_u8 * peer_mac, t_u8 type, int initiator)
{
    RxReorderTbl *rx_reor_tbl_ptr;
    TxBAStreamTbl *ptxtbl;
    t_u8 cleanup_rx_reorder_tbl;

    ENTER();

    if (type == TYPE_DELBA_RECEIVE)
        cleanup_rx_reorder_tbl = (initiator) ? MTRUE : MFALSE;
    else
        cleanup_rx_reorder_tbl = (initiator) ? MFALSE : MTRUE;

    PRINTM(MEVENT, "DELBA: %02x:%02x:%02x:%02x:%02x:%02x tid=%d,"
           "initiator=%d\n", peer_mac[0],
           peer_mac[1], peer_mac[2],
           peer_mac[3], peer_mac[4], peer_mac[5], tid, initiator);

    if (cleanup_rx_reorder_tbl) {
        if (!(rx_reor_tbl_ptr = wlan_11n_get_rxreorder_tbl(priv, tid,
                                                           peer_mac))) {
            PRINTM(MERROR, "TID,TA not found in table!\n");
            LEAVE();
            return;
        }
        wlan_11n_delete_rxreorder_tbl_entry(priv, rx_reor_tbl_ptr);
    } else {
        if (!(ptxtbl = wlan_11n_get_txbastream_tbl(priv, tid, peer_mac))) {
            PRINTM(MERROR, "TID,RA not found in table!\n");
            LEAVE();
            return;
        }

        wlan_11n_delete_txbastream_tbl_entry(priv, ptxtbl);
    }

    LEAVE();
}

/** 
 *  @brief This function handles the command response of
 *  		a block ack response
 *  
 *  @param priv    A pointer to mlan_private structure
 *  @param resp    A pointer to HostCmd_DS_COMMAND
 *
 *  @return        MLAN_STATUS_SUCCESS
 */
mlan_status
wlan_ret_11n_addba_resp(mlan_private * priv, HostCmd_DS_COMMAND * resp)
{
    HostCmd_DS_11N_ADDBA_RSP *padd_ba_rsp = (HostCmd_DS_11N_ADDBA_RSP *)
        & resp->params.add_ba_rsp;
    int tid, win_size;

    ENTER();

    padd_ba_rsp->status_code = wlan_le16_to_cpu(padd_ba_rsp->status_code);
    padd_ba_rsp->block_ack_param_set =
        wlan_le16_to_cpu(padd_ba_rsp->block_ack_param_set);
    padd_ba_rsp->block_ack_tmo = wlan_le16_to_cpu(padd_ba_rsp->block_ack_tmo);
    padd_ba_rsp->ssn = wlan_le16_to_cpu(padd_ba_rsp->ssn);

    tid = (padd_ba_rsp->block_ack_param_set & BLOCKACKPARAM_TID_MASK)
        >> BLOCKACKPARAM_TID_POS;
    win_size = (padd_ba_rsp->block_ack_param_set & BLOCKACKPARAM_WINSIZE_MASK)
        >> BLOCKACKPARAM_WINSIZE_POS;

    wlan_11n_create_rxreorder_tbl(priv, padd_ba_rsp->peer_mac_addr, tid,
                                  win_size, padd_ba_rsp->ssn);

    PRINTM(MEVENT, "ADDBA: %02x:%02x:%02x:%02x:%02x:%02x tid=%d\n",
           padd_ba_rsp->peer_mac_addr[0], padd_ba_rsp->peer_mac_addr[1],
           padd_ba_rsp->peer_mac_addr[2], padd_ba_rsp->peer_mac_addr[3],
           padd_ba_rsp->peer_mac_addr[4], padd_ba_rsp->peer_mac_addr[5], tid);

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function handles ba_stream_timeout event
 *  
 *  @param priv    	A pointer to mlan_private
 *  @param event    A pointer to structure HostCmd_DS_11N_BATIMEOUT
 *
 *  @return 	   	N/A
 */
void
wlan_11n_ba_stream_timeout(mlan_private * priv,
                           HostCmd_DS_11N_BATIMEOUT * event)
{
    HostCmd_DS_11N_DELBA delba;

    ENTER();

    DBG_HEXDUMP(MCMD_D, "Event:", (t_u8 *) event, 20);

    memset(&delba, 0, sizeof(HostCmd_DS_11N_DELBA));
    memcpy(delba.peer_mac_addr, event->peer_mac_addr, MLAN_MAC_ADDR_LENGTH);

    delba.del_ba_param_set |= (t_u16) event->tid << DELBA_TID_POS;
    delba.del_ba_param_set |= (t_u16) event->origninator << DELBA_INITIATOR_POS;
    delba.reason_code = REASON_CODE_STA_TIMEOUT;
    wlan_prepare_cmd(priv, HostCmd_CMD_11N_DELBA, 0, 0, MNULL, &delba);

    LEAVE();
    return;
}

/**
 *  @brief This function cleans up reorder tbl
 *  
 *  @param priv    	A pointer to mlan_private
 *
 *  @return 	   	N/A
 */
void
wlan_11n_cleanup_reorder_tbl(mlan_private * priv)
{
    RxReorderTbl *del_tbl_ptr;

    ENTER();

    while ((del_tbl_ptr = (RxReorderTbl *)
            util_peek_list(&priv->rx_reorder_tbl_ptr,
                           priv->adapter->callbacks.moal_spin_lock,
                           priv->adapter->callbacks.moal_spin_unlock))) {
        wlan_11n_delete_rxreorder_tbl_entry(priv, del_tbl_ptr);
    }

    util_init_list((pmlan_linked_list) & priv->rx_reorder_tbl_ptr);

    LEAVE();
}
