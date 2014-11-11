/*
 *  reset.c
 *
 *  Copyright (c) 2010 OnLive, Inc. Remi Machet
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>
#include <linux/kernel.h>   /* printk() */
#include <linux/slab.h>   /* kmalloc() */
#include <linux/fs.h>       /* everything... */
#include <linux/errno.h>    /* error codes */
#include <linux/types.h>    /* size_t */
#include <linux/delay.h>
#include <asm/io.h>
#include "Galois_memmap.h"
#include "galois_io.h"
#include "global.h"
#include "SM_common.h"
#include "mach/galois_generic.h"
#include "apb_perf_base.h"

#define mem_write(addr, val)	(*((volatile uint32_t *)(addr)) = val)

void arch_reset(char mode)
{
	int msgbody[2];
	printk(KERN_NOTICE "Send command to SM: restart\n");
	msgbody[0]=MV_SM_POWER_SYS_RESET;
	msgbody[1]=0;
	bsm_msg_send(MV_SM_ID_POWER,msgbody,sizeof(int)*2);
	mdelay(100);
	printk(KERN_NOTICE "Force a watchdog triggered reset\n");
	// In case SM is dead we now try to reset ourselves
	mem_write(SM_SYS_CTRL_REG_BASE | 0x24, 0x2e); // WDT0 reset SM , WDT1 reset SOC
	mem_write(SM_SYS_CTRL_REG_BASE | 0x20, 0xff); //  tell bootflow to do system reset
	mem_write(SM_APB_WDT1_BASE | 0x04, 0x11); //  set TORR (i+i<<4)
	mem_write(SM_APB_WDT0_BASE | 0x04, 0x22); //  set TORR (i+i<<4)
	mem_write(SM_APB_WDT1_BASE | 0x0c, 0x76); //  restart Counter
	mem_write(SM_APB_WDT0_BASE | 0x0c, 0x76); //  restart Counter
	// start WDT
	mem_write(SM_APB_WDT1_BASE | 0x00, 0x1D); // start WDT1
	mem_write(SM_APB_WDT0_BASE | 0x00, 0x1D); // start WDT0
	printk(KERN_NOTICE "Good bye\n");
	while(1);	// Probably not necessary
}
