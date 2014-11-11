/*
 *	Fusion Kernel Module
 *
 *	(c) Copyright 2002-2003  Convergence GmbH
 *
 *      Written by Denis Oliver Kropp <dok@directfb.org>
 *
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 */

#ifdef HAVE_LINUX_CONFIG_H
#include <linux/config.h>
#endif
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/smp_lock.h>
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/version.h>

#include <linux/fusion.h>

#include "fusiondev.h"
#include "entries.h"


void
fusion_entries_init( FusionEntries    *entries,
                     FusionEntryClass *class,
                     void             *ctx )
{
     FUSION_ASSERT( entries != NULL );
     FUSION_ASSERT( class != NULL );
     FUSION_ASSERT( class->object_size >= sizeof(FusionEntry) );

     memset( entries, 0, sizeof(FusionEntries) );

     entries->class = class;
     entries->ctx   = ctx;

     init_MUTEX( &entries->lock );
}

void
fusion_entries_deinit( FusionEntries *entries )
{
     FusionLink       *tmp;
     FusionEntry      *entry;
     FusionEntryClass *class;

     FUSION_ASSERT( entries != NULL );
     FUSION_ASSERT( entries->class != NULL );

     class = entries->class;

     down( &entries->lock );

     fusion_list_foreach_safe (entry, tmp, entries->list) {
          if (class->Destroy)
               class->Destroy( entry, entries->ctx );

          kfree( entry );
     }

     up( &entries->lock );
}

int
fusion_entries_read_proc(char *buf, char **start, off_t offset,
                         int len, int *eof, void *private)
{
     FusionEntry      *entry;
     FusionEntryClass *class;
     FusionEntries    *entries = private;
     int               written = 0;
     struct timeval    now;

     FUSION_ASSERT( entries != NULL );
     FUSION_ASSERT( entries->class != NULL );

     class = entries->class;

     if (!class->Print)
          return -ENOTSUPP;

     if (down_interruptible (&entries->lock))
          return -EINTR;

     do_gettimeofday( &now );

     fusion_list_foreach (entry, entries->list) {
          if (entry->last_lock.tv_sec) {
               int diff = ((now.tv_sec  - entry->last_lock.tv_sec) * 1000 +
                           (now.tv_usec - entry->last_lock.tv_usec) / 1000);

               if (diff < 1000) {
                    written += sprintf( buf + written, "%3d  ms  ", diff );
               }
               else if (diff < 1000000) {
                    written += sprintf( buf + written, "%3d.%d s  ",
                                        diff / 1000, (diff % 1000) / 100 );
               }
               else {
                    diff = ( now.tv_sec  - entry->last_lock.tv_sec +
                            (now.tv_usec - entry->last_lock.tv_usec) / 1000000);

                    written += sprintf( buf + written, "%3d.%d h  ",
                                        diff / 3600, (diff % 3600) / 360 );
               }
          }
          else
               written += sprintf( buf + written, "  -.-    " );


          written += sprintf( buf + written, "(%5d) 0x%08x  ", entry->pid, entry->id );

          written += sprintf( buf + written, "%-24s  ", entry->name[0] ? entry->name : "" );

          written += class->Print( entry, entries->ctx, buf + written );

          if (written < offset) {
               offset -= written;
               written = 0;
          }

          if (written >= len)
               break;
     }

     up (&entries->lock);

     *start = buf + offset;
     written -= offset;
     if (written > len) {
          *eof = 0;
          return len;
     }

     *eof = 1;

     return (written<0) ? 0 : written;
}

int
fusion_entry_create( FusionEntries *entries,
                     int           *ret_id,
                     void          *create_ctx )
{
     int               ret;
     FusionEntry      *entry;
     FusionEntryClass *class;

     FUSION_ASSERT( entries != NULL );
     FUSION_ASSERT( entries->class != NULL );
     FUSION_ASSERT( ret_id != NULL );

     class = entries->class;

     entry = kmalloc( class->object_size, GFP_KERNEL );
     if (!entry)
          return -ENOMEM;

     memset( entry, 0, class->object_size );

     if (down_interruptible( &entries->lock )) {
          kfree( entry );
          return -EINTR;
     }

     entry->entries = entries;
     entry->id      = ++entries->ids;
     entry->pid     = current->pid;

     init_MUTEX( &entry->lock );

     init_waitqueue_head( &entry->wait );

     if (class->Init) {
          ret = class->Init( entry, entries->ctx, create_ctx );
          if (ret) {
               up( &entries->lock );
               kfree( entry );
               return ret;
          }
     }

     fusion_list_prepend( &entries->list, &entry->link );

     up( &entries->lock );

     *ret_id = entry->id;

     return 0;
}

int
fusion_entry_destroy( FusionEntries  *entries,
                      int             id )
{
     FusionEntry      *entry;
     FusionEntryClass *class;

     FUSION_ASSERT( entries != NULL );
     FUSION_ASSERT( entries->class != NULL );

     class = entries->class;

     /* Lock entries. */
     if (down_interruptible( &entries->lock ))
          return -EINTR;

     /* Lookup the entry. */
     fusion_list_foreach (entry, entries->list) {
          if (entry->id == id)
               break;
     }

     /* Check if no entry was found. */
     if (!entry) {
          up( &entries->lock );
          return -EINVAL;
     }

     /* Lock the entry. */
     if (down_interruptible( &entry->lock )) {
          up( &entries->lock );
          return -EINTR;
     }

     /* Destroy it now. */
     fusion_entry_destroy_locked( entries, entry );

     /* Unlock entries. */
     up( &entries->lock );

     return 0;
}

void
fusion_entry_destroy_locked( FusionEntries  *entries,
                             FusionEntry    *entry )
{
     FusionEntryClass *class;

     FUSION_ASSERT( entries != NULL );
     FUSION_ASSERT( entries->class != NULL );

     class = entries->class;

     /* Remove the entry from the list. */
     fusion_list_remove( &entries->list, &entry->link );

     /* Wake up any waiting process. */
     wake_up_interruptible_all( &entry->wait );

     /* Call the destroy function. */
     if (class->Destroy)
          class->Destroy( entry, entries->ctx );

     /* Unlock the entry. */
     up( &entry->lock );

     /* Deallocate the entry. */
     kfree( entry );
}

int
fusion_entry_set_info( FusionEntries         *entries,
                       const FusionEntryInfo *info )
{
     int          ret;
     FusionEntry *entry;

     FUSION_ASSERT( entries != NULL );
     FUSION_ASSERT( info != NULL );

     ret = fusion_entry_lock( entries, info->id, false, &entry );
     if (ret)
          return ret;

     snprintf( entry->name, FUSION_ENTRY_INFO_NAME_LENGTH, info->name );

     fusion_entry_unlock( entry );

     return 0;
}

int
fusion_entry_get_info( FusionEntries   *entries,
                       FusionEntryInfo *info )
{
     int          ret;
     FusionEntry *entry;

     FUSION_ASSERT( entries != NULL );
     FUSION_ASSERT( info != NULL );

     ret = fusion_entry_lock( entries, info->id, false, &entry );
     if (ret)
          return ret;

     snprintf( info->name, FUSION_ENTRY_INFO_NAME_LENGTH, entry->name );

     fusion_entry_unlock( entry );

     return 0;
}

int
fusion_entry_lock( FusionEntries  *entries,
                   int             id,
                   bool            keep_entries_lock,
                   FusionEntry   **ret_entry )
{
     FusionEntry *entry;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 24)
     struct timespec xtime;
#endif

     FUSION_ASSERT( entries != NULL );
     FUSION_ASSERT( ret_entry != NULL );

     /* Lock entries. */
     if (down_interruptible( &entries->lock ))
          return -EINTR;

     /* Lookup the entry. */
     fusion_list_foreach (entry, entries->list) {
          if (entry->id == id)
               break;
     }

     /* Check if no entry was found. */
     if (!entry) {
          up( &entries->lock );
          return -EINVAL;
     }

     FUSION_ASSUME( entry->lock_pid != current->pid );

     /* Move the entry to the front of all entries. */
     fusion_list_move_to_front( &entries->list, &entry->link );

     /* Lock the entry. */
     if (down_interruptible( &entry->lock )) {
          up( &entries->lock );
          return -EINTR;
     }

     /* Mark as locked. */
     entry->lock_pid = current->pid;

     /* Keep timestamp, but use the slightly
        inexact version to avoid performance impacts. */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0) && defined _STRUCT_TIMESPEC
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 24)
     xtime = current_kernel_time();
#endif
     entry->last_lock.tv_sec = xtime.tv_sec;
     entry->last_lock.tv_usec = xtime.tv_nsec / 1000;
#else
     entry->last_lock = xtime;
#endif

     /* Unlock entries. */
     if (!keep_entries_lock)
          up( &entries->lock );

     /* Return the locked entry. */
     *ret_entry = entry;

     return 0;
}

void
fusion_entry_unlock( FusionEntry *entry )
{
     FUSION_ASSERT( entry != NULL );
     FUSION_ASSUME( entry->lock_pid == current->pid );

     entry->lock_pid = 0;

     /* Unlock the entry. */
     up( &entry->lock );
}

int
fusion_entry_wait( FusionEntry *entry, long *timeout )
{
     int            ret;
     int            id;
     FusionEntries *entries;
     FusionEntry   *entry2;

     FUSION_ASSERT( entry != NULL );
     FUSION_ASSERT( entry->entries != NULL );
     FUSION_ASSUME( entry->lock_pid == current->pid );

     id      = entry->id;
     entries = entry->entries;

     entry->waiters++;

     entry->lock_pid = 0;
     fusion_sleep_on( &entry->wait, &entry->lock, timeout );

     entry->waiters--;

     if (signal_pending(current))
          return -EINTR;

     if (timeout && !*timeout)
          return -ETIMEDOUT;

     ret = fusion_entry_lock( entries, id, false, &entry2 );
     switch (ret) {
          case -EINVAL:
               return -EIDRM;

          case 0:
               if (entry != entry2)
                    BUG();
     }

     return ret;
}

void
fusion_entry_notify( FusionEntry *entry, bool all )
{
     FUSION_ASSERT( entry != NULL );
     FUSION_ASSUME( entry->lock_pid == current->pid );

     if (all)
          wake_up_interruptible_all( &entry->wait );
     else
          wake_up_interruptible( &entry->wait );
}

