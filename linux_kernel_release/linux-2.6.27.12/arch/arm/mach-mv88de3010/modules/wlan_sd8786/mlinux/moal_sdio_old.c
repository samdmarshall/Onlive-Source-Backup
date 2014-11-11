/** @file moal_sdio.c
 *
 *  @brief This file contains SDIO IF (interface) module
 *  related functions.
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
	01/02/09: Initial creation -
		  This file supports SDIO SYSKT only
****************************************************/

#include <linux/firmware.h>

#include	"moal_sdio.h"

/** define marvell vendor id */
#define MARVELL_VENDOR_ID 0x02df

/** Set GPIO port to high */
#define GPIO_PORT_TO_HIGH()	sd_set_gpo((psd_device)handle->card, 1)
/** Set GPIO port to low */
#define GPIO_PORT_TO_LOW()	sd_set_gpo((psd_device)handle->card, 0)

/********************************************************
		Local Variables
********************************************************/

/** SDIO Function */
#define SDIO_FUNC(x)			(x)
/** SDIO Function 0 */
#define SDIO_FUNC0			SDIO_FUNC(0)
/** SDIO Function 1 */
#define SDIO_FUNC1			SDIO_FUNC(1)

/** SDIO func 1 */
static t_u8 sdio_func_wlan = SDIO_FUNC1;

/********************************************************
		Global Variables
********************************************************/

/** ISR */
sd_int_handler isr_function;
/** GPIO interrupt callback request */
int request_gpio_irq_callback(void (*callback) (void *), void *arg);
/** GPIO interrupt callback release */
int release_gpio_irq_callback(void (*callback) (void *), void *arg);

/********************************************************
		Local Functions
********************************************************/

/********************************************************
		Global Functions
********************************************************/

/** 
 *  @brief This function writes data into card register
 *
 *  @param handle   A Pointer to the moal_handle structure
 *  @param reg      Register offset
 *  @param data     Value
 *
 *  @return    		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
woal_write_reg(moal_handle * handle, t_u32 reg, t_u32 data)
{
    mlan_status ret = MLAN_STATUS_FAILURE;

    if (!sdio_write_ioreg
        ((sd_device *) handle->card, sdio_func_wlan, reg, (t_u8) data))
        ret = MLAN_STATUS_SUCCESS;

    return ret;
}

/** 
 *  @brief This function reads data from card register
 *
 *  @param handle   A Pointer to the moal_handle structure
 *  @param reg      Register offset
 *  @param data     Value
 *
 *  @return    		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
woal_read_reg(moal_handle * handle, t_u32 reg, t_u32 * data)
{
    mlan_status ret = MLAN_STATUS_FAILURE;
    t_u8 val;

    if (!sdio_read_ioreg((sd_device *) handle->card, sdio_func_wlan, reg, &val)) {
        *data = val;
        ret = MLAN_STATUS_SUCCESS;
    }

    return ret;
}

/**
 *  @brief This function writes multiple bytes into card memory
 *
 *  @param handle   A Pointer to the moal_handle structure
 *  @param pmbuf	Pointer to mlan_buffer structure
 *  @param port		Port
 *  @param timeout 	Time out value
 *
 *  @return    		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
woal_write_data_sync(moal_handle * handle, mlan_buffer * pmbuf, t_u32 port,
                     t_u32 timeout)
{
    mlan_status ret = MLAN_STATUS_FAILURE;
    t_u8 *buffer = (t_u8 *) (pmbuf->pbuf + pmbuf->data_offset);
    t_u8 blkmode = (port & MLAN_SDIO_BYTE_MODE_MASK) ? BYTE_MODE : BLOCK_MODE;
    t_u32 blksz = MLAN_SDIO_BLOCK_SIZE;
    t_u32 blkcnt =
        (blkmode ==
         BLOCK_MODE) ? (pmbuf->data_len /
                        MLAN_SDIO_BLOCK_SIZE) : pmbuf->data_len;
    t_u32 ioport = (port & MLAN_SDIO_IO_PORT_MASK);

    if (!sdio_write_iomem
        ((sd_device *) handle->card, sdio_func_wlan, ioport, blkmode,
         FIXED_ADDRESS, blkcnt, blksz, buffer))
        ret = MLAN_STATUS_SUCCESS;

    return ret;
}

/**
 *  @brief This function reads multiple bytes from card memory
 *
 *  @param handle   A Pointer to the moal_handle structure
 *  @param pmbuf	Pointer to mlan_buffer structure
 *  @param port		Port
 *  @param timeout 	Time out value
 *
 *  @return    		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
woal_read_data_sync(moal_handle * handle, mlan_buffer * pmbuf, t_u32 port,
                    t_u32 timeout)
{
    mlan_status ret = MLAN_STATUS_FAILURE;
    t_u8 *buffer = (t_u8 *) (pmbuf->pbuf + pmbuf->data_offset);
    t_u8 blkmode = (port & MLAN_SDIO_BYTE_MODE_MASK) ? BYTE_MODE : BLOCK_MODE;
    t_u32 blksz = MLAN_SDIO_BLOCK_SIZE;
    t_u32 blkcnt =
        (blkmode ==
         BLOCK_MODE) ? (pmbuf->data_len /
                        MLAN_SDIO_BLOCK_SIZE) : pmbuf->data_len;
    t_u32 ioport = (port & MLAN_SDIO_IO_PORT_MASK);

    if (!sdio_read_iomem
        ((sd_device *) handle->card, sdio_func_wlan, ioport, blkmode,
         FIXED_ADDRESS, blkcnt, blksz, buffer))
        ret = MLAN_STATUS_SUCCESS;

    return ret;
}

/** SDIO interrupt */
/** 
 *  @brief This function handles the interrupt
 *  
 *  @param dev	    Device
 *  @param id	    Device Id
 *  @param context	Context
 *  @return 	    None
 */
void
woal_sdio_interrupt(void *dev, sd_device_id * id, void *context)
{
    moal_handle *handle;

    ENTER();

    handle = (moal_handle *) context;
    PRINTM(MINFO, "*** IN SDIO IRQ ***\n");
    woal_interrupt(handle);

    LEAVE();
}

/** 
 *  @brief Client driver probe handler
 *  
 *  @param dev	    A pointer to _sd_device structure
 *  @param id	    SD Device Id
 *  @return 	    MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
int
woal_sdio_probe(struct _sd_device *dev, sd_device_id * id)
{
    int ret = MLAN_STATUS_SUCCESS;
    psd_device_id pIds;
    moal_handle *handle = NULL;

    ENTER();

    pIds = id;

    PRINTM(MINFO,
           "%s for %s : vendor=0x%4.04X device=0x%4.04X class=%d fn=%d\n",
           __FUNCTION__, dev->drv->name, pIds->vendor, pIds->device,
           pIds->class, pIds->fn);

    dev->pCurrent_Ids = pIds;
    sdio_func_wlan = pIds->fn;

    /* Note: card specific probe initialization is deferred until
       mlan_register() */

    if (NULL != (handle = woal_add_card(dev)))
        dev->priv = handle;
    else
        ret = MLAN_STATUS_FAILURE;
    LEAVE();
    return ret;
}

/** 
 *  @brief Client driver remove handler
 *  
 *  @param dev	    A pointer to _sd_device structure
 *  @return 	    0
 */
int
woal_sdio_remove(struct _sd_device *dev)
{
    ENTER();

    if (dev) {
        if (!dev->dev) {
            PRINTM(MINFO, "card removed from sd slot\n");
            if (dev->priv)
                ((moal_handle *) (dev->priv))->surprise_removed = MTRUE;
        }
    }
    woal_remove_card(dev);
    LEAVE();
    return 0;
}

#ifdef CONFIG_PM
/** 
 *  @brief Client driver suspend handler
 *  
 *  @param dev	A pointer to _sd_device structure
 *  @return 	0
 */
int
woal_sdio_suspend(struct _sd_device *dev)
{
    ENTER();
    LEAVE();
    return 0;
}

/** 
 *  @brief Client driver resume handler
 *  
 *  @param dev	A pointer to _sd_device structure
 *  @return 	0
 */
int
woal_sdio_resume(struct _sd_device *dev)
{
    ENTER();
    LEAVE();
    return 0;
}
#endif

/** ISR fct */
sd_function wlan_isr_fct = {
    .int_handler = woal_sdio_interrupt,
    .context = NULL
};

/** Device ID for SD8786 */
#define SD_DEVICE_ID_8786	0x9116

/** WLAN IDs */
sd_device_id wlan_ids[] = {
    {.vendor = MARVELL_VENDOR_ID,.device = SD_DEVICE_ID_8786,.class =
     SD_CLASS_ANY}
    ,
    {0, 0, 0}
};

static sd_driver sdio_wlan = {
    .name = "sdio_wlan",
    .ids = wlan_ids,
    .probe = woal_sdio_probe,
    .remove = woal_sdio_remove,
#ifdef CONFIG_PM
    .suspend = woal_sdio_suspend,
    .resume = woal_sdio_resume,
#endif
};

/** 
 *  @brief This function registers the IF module in bus driver
 *  
 *  @return	   MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
woal_bus_register(void)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;

    ENTER();

    /* SD Driver Registration */
    if (sd_driver_register(&sdio_wlan)) {
        PRINTM(MFATAL, "SD Driver Registration Failed \n");
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }

    LEAVE();
    return ret;
}

/** 
 *  @brief This function de-registers the IF module in bus driver
 *  
 *  @return 	   None
 */
void
woal_bus_unregister(void)
{
    ENTER();

    /* SD Driver Registration */
    if (sd_driver_unregister(&sdio_wlan)) {
        PRINTM(MFATAL, "SD Driver unregister Failed \n");
        LEAVE();
        return;
    }

    LEAVE();
}

/** 
 *  @brief This function disables the host interrupts
 *  
 *  @param handle A pointer to moal_handle structure
 *  @return 	   MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
int
woal_sdio_disable_host_int(moal_handle * handle)
{
    int ret = MLAN_STATUS_SUCCESS;

    ENTER();
    ret = sd_disable_int((sd_device *) handle->card,
                         ((sd_device *) handle->card)->pCurrent_Ids);
    LEAVE();
    return ret;
}

/** 
 *  @brief This function enables the host interrupts
 *  
 *  @param handle A pointer to moal_handle structure
 *  @return 	   MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
int
woal_sdio_enable_host_int(moal_handle * handle)
{
    int ret = MLAN_STATUS_SUCCESS;

    ENTER();
    ret = sd_enable_int((sd_device *) handle->card,
                        ((sd_device *) handle->card)->pCurrent_Ids);
    LEAVE();
    return ret;
}

/** 
 *  @brief This function de-registers the device
 *  
 *  @param handle A pointer to moal_handle structure
 *  @return 	  None
 */
void
woal_unregister_dev(moal_handle * handle)
{
    ENTER();
    if (handle->card) {
        /* Release the SDIO IRQ */
        sd_release_int(handle->card, wlan_ids);

        if (release_gpio_irq_callback(mlan_hs_callback, handle->pmlan_adapter))
            PRINTM(MFATAL, "Failed to release GPIO IRQ callback\n");
        PRINTM(MWARN, "Making the sdio dev card as NULL\n");
    }

    LEAVE();
}

/** 
 *  @brief This function registers the device
 *  
 *  @param handle  A pointer to moal_handle structure
 *  @return 	   MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
woal_register_dev(moal_handle * handle)
{
    int ret = MLAN_STATUS_SUCCESS;
    psd_device card = (psd_device) handle->card;
    sd_device_id *id = wlan_ids;

    ENTER();

    GPIO_PORT_TO_HIGH();

    handle->hotplug_device = card->dev;

    /* Disable host interrupt first. */
    if (MLAN_STATUS_SUCCESS != (ret = woal_sdio_disable_host_int(handle))) {
        PRINTM(MWARN, "Warning: unable to disable host interrupt!\n");
    }

    /* Request the SDIO IRQ */
    PRINTM(MINFO, "Before request_irq Address is if==>%p\n",
           wlan_isr_fct.int_handler);
    wlan_isr_fct.context = handle;
    id->fn = sdio_func_wlan;

    if (sd_request_int(card, wlan_ids, &wlan_isr_fct)) {
        PRINTM(MFATAL, "Failed to request IRQ on SDIO bus\n");
        goto failed;
    }
    if (request_gpio_irq_callback(mlan_hs_callback, handle->pmlan_adapter))
        PRINTM(MFATAL, "Failed to request GPIO IRQ callback\n");

    LEAVE();
    return MLAN_STATUS_SUCCESS;

  failed:
    handle->card = NULL;
    LEAVE();
    return MLAN_STATUS_FAILURE;
}

/** 
 *  @brief This function set bus clock on/off
 *  
 *  @param handle    A pointer to moal_handle structure
 *  @param option    TRUE--on , FALSE--off
 *  @return 	   MLAN_STATUS_SUCCESS
 */
int
woal_sdio_set_bus_clock(moal_handle * handle, t_u8 option)
{
    ENTER();

    if (option == MTRUE)
        sd_start_clock((psd_device) handle->card);
    else
        sd_stop_clock((psd_device) handle->card);

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}
