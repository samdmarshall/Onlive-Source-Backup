/** @file moal_sdio.h
  *
  * @brief This file contains definitions for SDIO interface.
  * driver. 
  *
  * Copyright (C) 2008-2009, Marvell International Ltd. 
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
/****************************************************
Change log:
****************************************************/

#ifndef	_MOAL_SDIO_H
#define	_MOAL_SDIO_H

#ifndef CONFIG_MV88DE3010_BERLIN
#include <sdio.h>
#endif
#include	<linux/mmc/sdio.h>
#include	<linux/mmc/sdio_ids.h>
#include	<linux/mmc/sdio_func.h>
#include	<linux/mmc/card.h>

#include "moal_main.h"

#ifndef BLOCK_MODE
/** Block mode */
#define BLOCK_MODE	1
#endif

#ifndef BYTE_MODE
/** Byte Mode */
#define BYTE_MODE	0
#endif

#ifndef FIXED_ADDRESS
/** Fixed address mode */
#define FIXED_ADDRESS	0
#endif

#ifdef CONFIG_MV88DE3010_BERLIN
/** Host Control Registers : Upload host interrupt mask */
#define UP_LD_HOST_INT_MASK		(0x1U)
/** Host Control Registers : Download host interrupt mask */
#define DN_LD_HOST_INT_MASK		(0x2U)

/** Enable Host interrupt mask */
#define HIM_ENABLE			(UP_LD_HOST_INT_MASK | DN_LD_HOST_INT_MASK)
/** Disable Host interrupt mask */
#define	HIM_DISABLE			0xff
/** Host Control Registers : Host interrupt mask */
#define HOST_INT_MASK_REG		0x02
/** Host Control Registers : Configuration */
#define CONFIGURATION_REG		0x00

/** Host Control Registers : Host interrupt status */
#define HOST_INTSTATUS_REG		0x03
/** Host Control Registers : Upload host interrupt status */
#define UP_LD_HOST_INT_STATUS		(0x1U)
/** Host Control Registers : Download host interrupt status */
#define DN_LD_HOST_INT_STATUS		(0x2U)

/** Card Control Registers : Card status register */
#define CARD_STATUS_REG              	0x30
/** Card Control Registers : Card I/O ready */
#define CARD_IO_READY              	(0x1U << 3)
/** Card Control Registers : CIS card ready */
#define CIS_CARD_RDY                 	(0x1U << 2)
/** Card Control Registers : Upload card ready */
#define UP_LD_CARD_RDY               	(0x1U << 1)
/** Card Control Registers : Download card ready */
#define DN_LD_CARD_RDY               	(0x1U << 0)

/** Special register in function 0 of the SDxxx card : Scratch 0 */
#define	SCRATCH_0_REG			0x80fe
/** Special register in function 0 of the SDxxx card : Scratch 1 */
#define	SCRATCH_1_REG			0x80ff
#endif
/** Default firmware name */

#define DEFAULT_FW_NAME "mrvl/sd8786.bin"

#ifndef DEFAULT_FW_NAME
#define DEFAULT_FW_NAME ""
#endif

#ifdef CONFIG_MV88DE3010_BERLIN
struct sdio_mmc_card
{
        /** sdio_func structure pointer */
    struct sdio_func *func;
        /** wlan_private structure pointer */
    moal_handle *priv;
    atomic_t in_int;
};
#endif

/********************************************************
		Global Functions
********************************************************/

/** Function to write register */
mlan_status woal_write_reg(moal_handle * handle, u32 reg, u8 data);
/** Function to read register */
mlan_status woal_read_reg(moal_handle * handle, u32 reg, u8 * data);
/** Function to write data to IO memory */
mlan_status woal_write_data_sync(moal_handle * handle, mlan_buffer * pmbuf,
                                 t_u32 port, t_u32 timeout);
/** Function to read data from IO memory */
mlan_status woal_read_data_sync(moal_handle * handle, mlan_buffer * pmbuf,
                                t_u32 port, t_u32 timeout);

/** Register to bus driver function */
mlan_status woal_bus_register(void);
/** Unregister from bus driver function */
void woal_bus_unregister(void);

/** Register device function */
mlan_status woal_register_dev(moal_handle * handle);
/** Unregister device function */
void woal_unregister_dev(moal_handle * handle);

/** Disable interrupt-to-host function */
int woal_sdio_disable_host_int(moal_handle * handle);
/** Enable interrupt-to-host function */
int woal_sdio_enable_host_int(moal_handle * handle);

int woal_sdio_set_bus_clock(moal_handle * handle, t_u8 option);

#ifdef CONFIG_MV88DE3010_BERLIN
void woal_sdio_claim_host(moal_handle * handle);
void woal_sdio_release_host(moal_handle * handle);

void woal_sdio_unmask(moal_handle * handle);

mlan_status woal_get_cardregs(moal_handle * pmlan_adapter, t_u8 * mp_regs);
#endif
#endif /* _MOAL_SDIO_H */
