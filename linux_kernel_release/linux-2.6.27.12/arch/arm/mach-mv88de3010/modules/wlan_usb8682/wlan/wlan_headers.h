/** @file wlan_headers.h
 * 
 * @brief This file contains all the necessary include file.
 *  
  * Copyright (C) 2003-2008, Marvell International Ltd.  
 * All Rights Reserved
 */
/********************************************************
Change log:
	05/30/07: Initial creation
********************************************************/

#ifndef _WLAN_HEADERS_H
#define _WLAN_HEADERS_H

#ifndef __ATTRIB_ALIGN__
#define __ATTRIB_ALIGN__ __attribute__((aligned(4)))
#endif

#ifndef __ATTRIB_PACK__
#define __ATTRIB_PACK__ __attribute__ ((packed))
#endif

/* Linux header files */
#include        <linux/kernel.h>
#include        <linux/module.h>
#include        <linux/init.h>
#include        <linux/version.h>
#include        <linux/param.h>
#include        <linux/delay.h>
#include        <linux/slab.h>
#include        <linux/mm.h>
#include        <linux/types.h>
#include        <linux/sched.h>
#include        <linux/timer.h>
#include        <linux/types.h>
#include        <linux/ioport.h>
#include        <linux/pci.h>
#include        <linux/ctype.h>
#include        <linux/proc_fs.h>
#include	<linux/ptrace.h>
#include	<linux/string.h>
#include		<linux/freezer.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,18)
#include	<linux/config.h>
#endif

#include	<linux/ioport.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,22)
#include	<linux/freezer.h>
#endif
#include	<linux/usb.h>

/* ASM files */
//#include        <asm/semaphore.h>
#include        <asm/byteorder.h>
#include        <asm/irq.h>
#include        <asm/uaccess.h>
#include        <asm/io.h>
#include	<asm/system.h>

/* Net header files */
#include        <linux/wireless.h>
#include        <linux/netdevice.h>
#include        <linux/net.h>
#include        <linux/ip.h>
#include        <linux/skbuff.h>
#include        <linux/if_arp.h>
#include        <linux/if_ether.h>
#include        <linux/etherdevice.h>
#include        <net/arp.h>
#include        <linux/rtnetlink.h>

/* Wireless header */
#include        <net/iw_handler.h>

#include    "wlan_linux.h"
#include    "wlan_fw.h"

#include    "wlan_wmm.h"
#include    "wlan_11d.h"

#include     "wlan_esupplicant.h"

#include    "wlan_scan.h"
#include    "wlan_join.h"

#include    "wlan_drv.h"

#include    "wlan_wext.h"
#include    "wlan_priv.h"
#include    "wlan_decl.h"
#endif /* _WLAN_HEADERS_H */
