/** @file mlan_util.h
 *
 *  @brief This file contains wrappers for linked-list,
 *  spinlock and timer defines.
 *
 *  Copyright (C) 2008-2009, Marvell International Ltd. 
 *  All Rights Reserved
 */

/******************************************************
Change log:
    10/28/2008: initial version
******************************************************/

#ifndef _MLAN_UTIL_H_
#define _MLAN_UTIL_H_

/** Circular doubly linked list */
typedef struct _mlan_linked_list
{
    /** Pointer to previous node */
    struct _mlan_linked_list *pprev;
    /** Pointer to next node */
    struct _mlan_linked_list *pnext;
} mlan_linked_list, *pmlan_linked_list;

/** List head */
typedef struct _mlan_list_head
{
    /** Pointer to previous node */
    struct _mlan_linked_list *pprev;
    /** Pointer to next node */
    struct _mlan_linked_list *pnext;
    /** Pointer to lock */
    t_void *plock;
} mlan_list_head, *pmlan_list_head;

/** 
 *  @brief This function initializes a list without locking
 *  
 *  @param phead		List head
 *
 *  @return			N/A
 */
static inline t_void
util_init_list(pmlan_linked_list phead)
{
    /* Both next and prev point to self */
    phead->pprev = phead->pnext = (pmlan_linked_list) phead;
}

/** 
 *  @brief This function initializes a list
 *  
 *  @param phead		List head
 *  @param lock_required	A flag for spinlock requirement
 *  @param moal_init_lock	A pointer to init lock handler
 *
 *  @return			N/A
 */
static inline t_void
util_init_list_head(pmlan_list_head phead,
                    t_u8 lock_required,
                    mlan_status(*moal_init_lock) (t_void ** pplock))
{
    /* Both next and prev point to self */
    util_init_list((pmlan_linked_list) phead);
    if (lock_required)
        moal_init_lock(&phead->plock);
    else
        phead->plock = 0;
}

/** 
 *  @brief This function frees a list
 *  
 *  @param phead		List head
 *  @param moal_free_lock	A pointer to free lock handler
 *
 *  @return			N/A
 */
static inline t_void
util_free_list_head(pmlan_list_head phead,
                    mlan_status(*moal_free_lock) (t_void * plock))
{
    phead->pprev = phead->pnext = 0;
    if (phead->plock)
        moal_free_lock(phead->plock);
}

/** 
 *  @brief This function peeks into a list
 *  
 *  @param phead		List head
 *  @param moal_spin_lock	A pointer to spin lock handler
 *  @param moal_spin_unlock	A pointer to spin unlock handler
 *
 *  @return			List node
 */
static inline pmlan_linked_list
util_peek_list(pmlan_list_head phead,
               mlan_status(*moal_spin_lock) (t_void * plock),
               mlan_status(*moal_spin_unlock) (t_void * plock))
{
    pmlan_linked_list pnode = 0;

    if (moal_spin_lock)
        moal_spin_lock(phead->plock);
    if (phead->pnext != (pmlan_linked_list) phead) {
        pnode = phead->pnext;
    }
    if (moal_spin_unlock)
        moal_spin_unlock(phead->plock);
    return pnode;
}

/** 
 *  @brief This function queues a node at the list tail
 *  
 *  @param phead		List head
 *  @param pnode		List node to queue
 *  @param moal_spin_lock	A pointer to spin lock handler
 *  @param moal_spin_unlock	A pointer to spin unlock handler
 *
 *  @return			N/A
 */
static inline t_void
util_enqueue_list_tail(pmlan_list_head phead,
                       pmlan_linked_list pnode,
                       mlan_status(*moal_spin_lock) (t_void * plock),
                       mlan_status(*moal_spin_unlock) (t_void * plock))
{
    pmlan_linked_list pold_last;

    if (moal_spin_lock)
        moal_spin_lock(phead->plock);
    pold_last = phead->pprev;
    pnode->pprev = pold_last;
    pnode->pnext = (pmlan_linked_list) phead;

    phead->pprev = pold_last->pnext = pnode;
    if (moal_spin_unlock)
        moal_spin_unlock(phead->plock);
}

/** 
 *  @brief This function adds a node at the list head
 *  
 *  @param phead		List head
 *  @param pnode		List node to add
 *  @param moal_spin_lock	A pointer to spin lock handler
 *  @param moal_spin_unlock	A pointer to spin unlock handler
 *
 *  @return			N/A
 */
static inline t_void
util_enqueue_list_head(pmlan_list_head phead,
                       pmlan_linked_list pnode,
                       mlan_status(*moal_spin_lock) (t_void * plock),
                       mlan_status(*moal_spin_unlock) (t_void * plock))
{
    pmlan_linked_list pold_first;

    if (moal_spin_lock)
        moal_spin_lock(phead->plock);
    pold_first = phead->pnext;
    pnode->pprev = (pmlan_linked_list) phead;
    pnode->pnext = pold_first;

    phead->pnext = pold_first->pprev = pnode;
    if (moal_spin_unlock)
        moal_spin_unlock(phead->plock);
}

/** 
 *  @brief This function removes a node from the list
 *  
 *  @param phead		List head
 *  @param pnode		List node to remove
 *  @param moal_spin_lock	A pointer to spin lock handler
 *  @param moal_spin_unlock	A pointer to spin unlock handler
 *
 *  @return			N/A
 */
static inline t_void
util_unlink_list(pmlan_list_head phead,
                 pmlan_linked_list pnode,
                 mlan_status(*moal_spin_lock) (t_void * plock),
                 mlan_status(*moal_spin_unlock) (t_void * plock))
{
    pmlan_linked_list pmy_prev;
    pmlan_linked_list pmy_next;

    if (moal_spin_lock)
        moal_spin_lock(phead->plock);
    pmy_prev = pnode->pprev;
    pmy_next = pnode->pnext;
    pmy_next->pprev = pmy_prev;
    pmy_prev->pnext = pmy_next;

    pnode->pnext = pnode->pprev = 0;
    if (moal_spin_unlock)
        moal_spin_unlock(phead->plock);
}

/** 
 *  @brief This function dequeues a node from the list
 *  
 *  @param phead		List head
 *  @param moal_spin_lock	A pointer to spin lock handler
 *  @param moal_spin_unlock	A pointer to spin unlock handler
 *
 *  @return			List node
 */
static inline pmlan_linked_list
util_dequeue_list(pmlan_list_head phead,
                  mlan_status(*moal_spin_lock) (t_void * plock),
                  mlan_status(*moal_spin_unlock) (t_void * plock))
{
    pmlan_linked_list pnode;

    if (moal_spin_lock)
        moal_spin_lock(phead->plock);
    pnode = phead->pnext;
    if (pnode != (pmlan_linked_list) phead) {
        util_unlink_list(phead, pnode, 0, 0);
    } else {
        pnode = 0;
    }
    if (moal_spin_unlock)
        moal_spin_unlock(phead->plock);
    return pnode;
}

#endif /* !_MLAN_UTIL_H_ */
