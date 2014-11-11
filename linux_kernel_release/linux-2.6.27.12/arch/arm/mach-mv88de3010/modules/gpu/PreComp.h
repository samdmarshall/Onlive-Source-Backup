/****************************************************************************
 * *
 * *    Copyright (C) 2005 - 2010 by Vivante Corp.
 * *
 * *    This program is free software; you can redistribute it and/or modify
 * *    it under the terms of the GNU General Public Lisence as published by
 * *    the Free Software Foundation; either version 2 of the license, or
 * *    (at your option) any later version.
 * *
 * *    This program is distributed in the hope that it will be useful,
 * *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 * *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * *    GNU General Public Lisence for more details.
 * *
 * *    You should have received a copy of the GNU General Public License
 * *    along with this program; if not write to the Free Software
 * *    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * *
 * *****************************************************************************/
#include <linux/version.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/signal.h>
#include <linux/interrupt.h>
#include <linux/vmalloc.h>
#include <linux/dma-mapping.h>
#include <linux/kthread.h>
#include <linux/semaphore.h>
#include <asm/io.h>
#include <asm/uaccess.h>

#define gcmTRACE_ZONE galois_printk
#define gcmTRACE galois_printk

#define FIND_TASK_BY_PID(x) find_task_by_vpid(x)

#define countof(a)					(sizeof(a) / sizeof(a[0]))

#define DRV_NAME          			"galcore"
#define USE_NEW_INTERRUPT 1
