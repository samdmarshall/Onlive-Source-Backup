/** @file mlan_module.c
 *
 *  @brief This file declares the exported symbols from MLAN.
 *
 *  Copyright (C) 2008-2009, Marvell International Ltd. 
 *  All Rights Reserved
 */

/******************************************************
Change log:
    12/08/2008: initial version
******************************************************/

#ifdef LINUX
#include <linux/module.h>
#include "mlan_decl.h"
#include "mlan_ioctl.h"

EXPORT_SYMBOL(mlan_register);
EXPORT_SYMBOL(mlan_unregister);
EXPORT_SYMBOL(mlan_init_fw);
EXPORT_SYMBOL(mlan_dnld_fw);
EXPORT_SYMBOL(mlan_shutdown_fw);
EXPORT_SYMBOL(mlan_write_data_async_complete);
EXPORT_SYMBOL(mlan_send_packet);
EXPORT_SYMBOL(mlan_recv);
EXPORT_SYMBOL(mlan_ioctl);
EXPORT_SYMBOL(mlan_main_process);
#endif /* LINUX */
