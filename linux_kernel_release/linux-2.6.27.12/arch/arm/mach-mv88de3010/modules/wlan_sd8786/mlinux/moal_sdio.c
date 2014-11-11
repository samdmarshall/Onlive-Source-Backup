/** @file wlan_sdio.c
 *  @brief This file contains SDIO IF (interface) module
 *  related functions.
 * 
 * Copyright (C) 2003-2008, Marvell International Ltd. 
 *
 * This software file (the "File") is distributed by Marvell International 
 * Ltd. under the terms of the GNU General Public License Version 2, June 1991 
 * (the "License").  You may use, redistribute and/or modify this File in 
 * accordance with the terms and conditions of the License, a copy of which 
 * is available along with the File in the gpl.txt file or by writing to 
 * the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 
 * 02111-1307 or on the worldwide web at http://www.gnu.org/licenses/gpl.txt.
 *
 * THE FILE IS DISTRIBUTED AS-IS, WITHOUT WARRANTY OF ANY KIND, AND THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE 
 * ARE EXPRESSLY DISCLAIMED.  The License provides additional details about 
 * this warranty disclaimer.
 *
 */
/****************************************************
Change log:
	12/04/09: initial version to use linux sdio stack
****************************************************/

#include	"moal_sdio.h"

#include <linux/firmware.h>

/** Max retry number of CMD53 write */
#define MOAL_MAX_IOMEM_RETRY	2

/********************************************************
		Local Variables
********************************************************/

/********************************************************
		Global Variables
********************************************************/

extern u8 *fw_name;

/********************************************************
		Local Functions
********************************************************/
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
    int i = 0;
    struct sdio_mmc_card *card = (struct sdio_mmc_card *)handle->card;
    mlan_status ret = MLAN_STATUS_FAILURE;
    t_u8 card_status;
    t_u8 *buffer = (t_u8 *) (pmbuf->pbuf + pmbuf->data_offset);
    t_u8 blkmode = (port & MLAN_SDIO_BYTE_MODE_MASK) ? BYTE_MODE : BLOCK_MODE;
    t_u32 blksz = MLAN_SDIO_BLOCK_SIZE;
    t_u32 blkcnt =
        (blkmode ==
         BLOCK_MODE) ? (pmbuf->data_len /
                        MLAN_SDIO_BLOCK_SIZE) : pmbuf->data_len;
    t_u32 ioport = (port & MLAN_SDIO_IO_PORT_MASK);
    unsigned long waitio_timeout;

    ENTER();

    if (!card || !card->func) {
        PRINTM(MFATAL, "card or function is NULL!\n");
        ret = MLAN_STATUS_FAILURE;
        return ret;
    }

    do {
#if 0
	if (!host->claimed)
#endif
//      	sdio_claim_host(card->func);

	waitio_timeout = jiffies + HZ;
	while(1) {
		card_status = sdio_readb(card->func, CARD_STATUS_REG, &ret);
		if (ret) {
			PRINTM(MFATAL, "write to card, can't read card status\n");
			ret = MLAN_STATUS_FAILURE;
			goto exit;
		}
		if ((card_status & CARD_IO_READY)) // && ( card_status & DN_LD_CARD_RDY))
			break;
		if (time_after(jiffies, waitio_timeout)) {
			PRINTM(MFATAL, "write to card, wait io timeout: ");
			if ( card_status & CARD_IO_READY)
                            PRINTM(MFATAL, "download\n");
			else
			    PRINTM(MFATAL, "ioready\n");
			ret = MLAN_STATUS_FAILURE;
			goto exit;
		}
		mdelay(1);
	}

	ret = sdio_writesb(card->func, ioport,
                   buffer, blkcnt * blksz);
	if (ret < 0) {
		i++;

//        	PRINTM(MFATAL, "write_data_sync: write iomem failed: %d\n", ret);
		if (woal_write_reg(handle, CONFIGURATION_REG, 0x04) < 0) {
			PRINTM(MFATAL, "writing config reg failed(CFG)\n");
		}

		ret = MLAN_STATUS_FAILURE;
	
		if (i > MOAL_MAX_IOMEM_RETRY ) {	
			PRINTM(MFATAL, "out of maximum retry to write out\n");
        		goto exit;
		}
	} else {
		ret = MLAN_STATUS_SUCCESS;
//		PRINTM(MINFO, "writed %u * %u data to card port %u\n", (unsigned int)blkcnt, (unsigned int)blksz, ioport );
	}
    } while ( ret == MLAN_STATUS_FAILURE);

exit:
//    sdio_release_host(card->func);
    LEAVE();
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
    struct sdio_mmc_card *card = handle->card;
    mlan_status ret = MLAN_STATUS_FAILURE;
    t_u8 card_status;
    t_u8 *buffer = (t_u8 *) (pmbuf->pbuf + pmbuf->data_offset);
    t_u8 blkmode = (port & MLAN_SDIO_BYTE_MODE_MASK) ? BYTE_MODE : BLOCK_MODE;
    t_u32 blksz = MLAN_SDIO_BLOCK_SIZE;
    int i = 0;
    t_u32 blkcnt = 
        (blkmode ==
         BLOCK_MODE) ? (pmbuf->data_len /
                        MLAN_SDIO_BLOCK_SIZE) : pmbuf->data_len;
    t_u32 ioport = (port & MLAN_SDIO_IO_PORT_MASK);
    unsigned long waitio_timeout;

    ENTER();
 //   sdio_claim_host(card->func);

    do {
        i = 0;
	waitio_timeout = jiffies + HZ;
	while(1) {
		card_status = sdio_readb(card->func, CARD_STATUS_REG, &ret);
		if (ret) {
			PRINTM(MFATAL, "read from card, can't read card status\n");
			ret = MLAN_STATUS_FAILURE;
			goto exit;
		}
		if ((card_status & CARD_IO_READY)) // && ( card_status & UP_LD_CARD_RDY))
			break;
		if (time_after(jiffies, waitio_timeout)) {
			PRINTM(MFATAL, "read from card, wait io timeout: ");
			if ( card_status & CARD_IO_READY)
                            PRINTM(MFATAL, "upload\n");
			else
			    PRINTM(MFATAL, "ioready\n");
			ret = MLAN_STATUS_FAILURE;
			goto exit;
		}
		mdelay(1);
	}
	ret = sdio_readsb(card->func, buffer, ioport,
        	              blksz*blkcnt);

   	 if (ret < 0) {
		i++;
        	PRINTM(MFATAL, "read_data_sync: read iomem failed: %d\n", ret);
		if (woal_write_reg(handle, CONFIGURATION_REG, 0x04) < 0) {
			PRINTM(MFATAL, "writing config reg failed(CFG)\n");
		}

		ret = MLAN_STATUS_FAILURE;
	
		if (i > MOAL_MAX_IOMEM_RETRY ) {	
			PRINTM(MFATAL, "out of maximum retry to write out\n");
        		goto exit;
		}
	} else {
	    ret = MLAN_STATUS_SUCCESS;
//	    PRINTM(MINFO, "readed %u data from card port %d\n", (unsigned int)blksz, ioport );
	}

    }while ( ret == MLAN_STATUS_FAILURE);
    
    ret = MLAN_STATUS_SUCCESS;
  exit:
//    sdio_release_host(card->func);
    LEAVE();
    return ret;
}

/** 
 *  @brief This function enables the host interrupts mask
 *  
 *  @param priv    A pointer to wlan_private structure
 *  @param mask	   the interrupt mask
 *  @return 	   MLAN_STATUS_SUCCESS
 */
static int
enable_host_int_mask(moal_handle * handle, u8 mask)
{
    int ret = MLAN_STATUS_SUCCESS;
    u8 host_int_mask;

    ENTER();
    /* Read back the host_int_mask register */ 
 
    woal_read_reg(handle, HOST_INT_MASK_REG, &host_int_mask);

    /* Simply write the mask to the register */
    ret = woal_write_reg(handle, HOST_INT_MASK_REG, mask);

    if (ret) {
        PRINTM(MWARN, "Unable to enable the host interrupt!\n");
        ret = MLAN_STATUS_FAILURE;
    }

    LEAVE();
    return ret;
}

/**  @brief This function disables the host interrupts mask.
 *  
 *  @param priv    A pointer to wlan_private structure
 *  @param mask	   the interrupt mask
 *  @return 	   MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static int
disable_host_int_mask(moal_handle * handle, u8 mask)
{
    int ret = MLAN_STATUS_SUCCESS;
    u8 host_int_mask;

    ENTER();
      /* Read back the host_int_mask register */
    ret = woal_read_reg(handle, HOST_INT_MASK_REG, &host_int_mask);
    if (ret) {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    /* Update with the mask and write back to the register */
    host_int_mask &= ~mask;
    ret = woal_write_reg(handle, HOST_INT_MASK_REG, host_int_mask);
    if (ret < 0) {
        PRINTM(MWARN, "Unable to diable the host interrupt!\n");
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

  done:
    LEAVE();
    return ret;
}

/********************************************************
		Global Functions
********************************************************/

/** 
 *  @brief This function handles the interrupt.
 *  
 *  @param func	   A pointer to sdio_func structure.
 *  @return 	   n/a
 */
static void
woal_sdio_interrupt(struct sdio_func *func)
{
    struct sdio_mmc_card *card;
    moal_handle *handle;
    u8 ireg = -1;
    int ret = MLAN_STATUS_FAILURE;

    ENTER();
    card = sdio_get_drvdata(func);
    if ( unlikely (!card || !card->priv) ) {
        PRINTM(MINFO, "woal_sdio_interrupt card or priv is NULL, card=%p\n",
               card);
        LEAVE();
        return;
    }
    handle = card->priv;

    if (unlikely(!handle)) {
	    PRINTM(MINFO, "woal_sdio_interrupt(), can't get handle\n");
	    LEAVE();
	    return;
    }
    if (unlikely(!(handle->mp_regs))) {
	    PRINTM(MINFO, "woal_sdio_interrupt(), can't get card regs\n");
	    LEAVE();
	    return;
    }

    ret = sdio_readsb(func, handle->mp_regs, 0, 40);
    if (unlikely(ret))
    {
	handle->mp_regs[HOST_INTSTATUS_REG] = 0;
        PRINTM(MWARN, "sdio_readsb in interrupt ret = %d\n", ret);
        ireg = sdio_readb(func, HOST_INTSTATUS_REG, &ret);
	if (ireg != 0){
	    PRINTM(MFATAL, "sdio_readb in irq ret = %d\n", ret);
	    LEAVE();
	    return;
	}
	else 
            printk("\n\n%02x = %02x\n", HOST_INTSTATUS_REG, ireg);
    }
   /*   else {
        printk("Dump registers: \n");
        for (i = 0; i<40; i++)
	    printk("%02x = %02x\n", i, handle->mp_regs[i]);
    }  
    sdio_writeb(func, 0x00, 0x02, &ret); 
    //sdio_writeb(func, 0x00, 0x04, &ret); 
        ireg = sdio_readb(func, 0x02, &ret);
	printk("\n\n%02x = %02x\n", 0x02, ireg);
        ireg = sdio_readb(func, 0x04, &ret);
	printk("%02x = %02x\n", 0x04, ireg);
*/
    woal_interrupt(handle);

    if (likely(handle->mp_regs[HOST_INTSTATUS_REG] != 0) ) {
        /* 
         * DN_LD_HOST_INT_STATUS and/or UP_LD_HOST_INT_STATUS
         * Clear the interrupt status register and re-enable the interrupt
         */
        PRINTM(MINFO, "sdio_ireg = 0x%x\n", ireg);
        sdio_writeb(func,   
		       	~(handle->mp_regs[HOST_INTSTATUS_REG]) & 
			(DN_LD_HOST_INT_STATUS | UP_LD_HOST_INT_STATUS), 
			HOST_INTSTATUS_REG, &ret );
	handle->mp_regs[HOST_INTSTATUS_REG] = 0;

	if (ret)
		PRINTM(MWARN, "1st disable interrupt failed\n");
   }
   else {
	PRINTM(MWARN, "try 2nd way to disable interrupt\n");
        sdio_writeb(func,  ~(ireg) & 
			(DN_LD_HOST_INT_STATUS | UP_LD_HOST_INT_STATUS), 
			HOST_INTSTATUS_REG, &ret );

	if (ret)
		PRINTM(MWARN, "2nd disable interrupt failed\n");
   }

}

/** 
 *  @brief This function handles client driver probe.
 *  
 *  @param func	   A pointer to sdio_func structure.
 *  @param id	   A pointer to sdio_device_id structure.
 *  @return 	   MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static int
woal_sdio_probe(struct sdio_func *func, const struct sdio_device_id *id)
{
    int ret = MLAN_STATUS_FAILURE;
    struct sdio_mmc_card *card = NULL;
    moal_handle *handle = NULL;

    ENTER();

    PRINTM(MINFO, "%s: vendor=0x%4.04X device=0x%4.04X class=%d function=%d\n",
           __FUNCTION__, func->vendor, func->device, func->class, func->num);

    card = kzalloc(sizeof(struct sdio_mmc_card), GFP_KERNEL);
    if (!card) {
        ret = -ENOMEM;
        goto done;
    }

    card->func = func;

    sdio_claim_host(func);
    if ( (handle = woal_add_card(card)) == NULL) {
        PRINTM(MFATAL, "%s: wlan_add_card failed\n", __FUNCTION__);
        kfree(card);
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }
    card->priv = handle;

    //sdio_release_host(func);

    ret = MLAN_STATUS_SUCCESS;

  done:
    LEAVE();
    return ret;
}

/** 
 *  @brief This function handles client driver remove.
 *  
 *  @param func	   A pointer to sdio_func structure.
 *  @return 	   n/a
 */
static void
woal_sdio_remove(struct sdio_func *func)
{
    struct sdio_mmc_card *card;

    ENTER();

    PRINTM(MINFO, "%s: function=%d\n", __FUNCTION__, func->num);

    if (func) {
        card = sdio_get_drvdata(func);
        if (card) {
            woal_remove_card(card);
            kfree(card);
        }
    }

    LEAVE();
}

#ifdef CONFIG_PM
/** 
 *  @brief This function handles client driver suspend.
 *  
 *  @param func	   A pointer to sdio_func structure.
 *  @param state   suspend mode (PM_SUSPEND_xxx)
 *  @return 	   0
 */
int
woal_sdio_suspend(struct sdio_func *func, pm_message_t state)
{
    ENTER();
    LEAVE();
    return 0;
}

/** 
 *  @brief This function handles client driver resume.
 *  
 *  @param func	   A pointer to sdio_func structure.
 *  @return 	  0 
 */
int
woal_sdio_resume(struct sdio_func *func)
{
    ENTER();
    LEAVE();
    return 0;
}
#endif

/** Device ID for SD8786 */
#define SD_DEVICE_ID_8786	0x9116

/** WLAN IDs */
static const struct sdio_device_id wlan_ids[] = {
    {SDIO_DEVICE(SDIO_VENDOR_ID_MARVELL, SD_DEVICE_ID_8786)},
    {SDIO_DEVICE_CLASS(SDIO_CLASS_WLAN)},
    {},
};

static struct sdio_driver wlan_sdio = {
    .name = "sdio_wlan",
    .id_table = wlan_ids,
    .probe = woal_sdio_probe,
    .remove = woal_sdio_remove,
#if 0
    .suspend = woal_sdio_suspend,
    .resume = woal_sdio_resume,
#endif
};

/** 
 *  @brief This function registers the IF module in bus driver.
 *  
 *  @return 	   MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
woal_bus_register(void)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;

    ENTER();

    /* SDIO Driver Registration */
    if (sdio_register_driver(&wlan_sdio) != 0) {
        PRINTM(MFATAL, "SDIO Driver Registration Failed \n");
        ret = MLAN_STATUS_FAILURE;
    }

    LEAVE();
    return ret;
}

/** 
 *  @brief This function de-registers the IF module in bus driver.
 *  
 *  @return 	   n/a
 */
void
woal_bus_unregister(void)
{
    ENTER();

    /* SDIO Driver Unregistration */
    sdio_unregister_driver(&wlan_sdio);

    LEAVE();
}

/** 
 *  @brief This function reads the IO register.
 *  
 *  @param priv    A pointer to wlan_private structure
 *  @param reg	   register to be read
 *  @param dat	   A pointer to variable that keeps returned value
 *  @return 	   MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
woal_read_reg(moal_handle * handle, u32 reg, u8 * dat)
{
    struct sdio_mmc_card *card;
    mlan_status ret = MLAN_STATUS_FAILURE;

    ENTER();

    card = handle->card;
    if (!card || !card->func) {
        PRINTM(MFATAL, "woal_read_reg(): card or function is NULL!\n");
        goto done;
    }

//    sdio_claim_host(card->func);
    *dat = sdio_readb(card->func, reg, &ret);
//    sdio_release_host(card->func);
    if (ret) {
        PRINTM(MFATAL, "woal_read_reg(): sdio_readb failed! ret=%d\n", ret);
	ret = MLAN_STATUS_FAILURE;
        goto done;
    }

//    PRINTM(MINFO, "woal_read_reg() priv=%p func=%d reg=%#x dat=%#x\n", handle,
//           card->func->num, reg, *dat);

    ret = MLAN_STATUS_SUCCESS;
  done:
    LEAVE();
    return ret;
}

/** 
 *  @brief This function writes the IO register.
 *  
 *  @param priv    A pointer to wlan_private structure
 *  @param reg	   register to be written
 *  @param dat	   the value to be written
 *  @return 	   MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
woal_write_reg(moal_handle * handle, u32 reg, u8 dat)
{
    struct sdio_mmc_card *card;
    mlan_status ret = MLAN_STATUS_FAILURE;

    ENTER();

    card = handle->card;
    if (!card || !card->func) {
        PRINTM(MFATAL, "woal_write_reg(): card or function is NULL!\n");
        goto done;
    }

//    PRINTM(MINFO, "woal_write_reg() priv=%p func=%d reg=%#x dat=%#x\n", handle,
 //          card->func->num, reg, dat);

 //   sdio_claim_host(card->func);
    sdio_writeb(card->func, dat, reg, &ret);
  //  sdio_release_host(card->func);
    if (ret) {
        PRINTM(MFATAL, "woal_write_reg(): sdio_writeb failed! ret=%d\n", ret);
    	ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    ret = MLAN_STATUS_SUCCESS;
  done:
    LEAVE();
    return ret;
}

/** 
 *  @brief This function disables the host interrupts.
 *  
 *  @param priv    A pointer to wlan_private structure
 *  @return 	   MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
int
woal_sdio_disable_host_int(moal_handle * handle)
{
    int ret = MLAN_STATUS_FAILURE;
    struct sdio_mmc_card *card = handle->card;
    int i;
    u8 dat;
    u8 regval;

    ENTER();

    if (!card || !card->func) {
        PRINTM(MFATAL, "Error: card or function is NULL!\n");
        return ret;
    }
#if 0
    regval = sdio_f0_readb(card->func, F0_INT_MASK_REG, &ret );
    
    if ( ret){
	PRINTM(MWARN, "Error returned from sdio_f0_readb %d\n", ret);
    }
    else
        PRINTM(MWARN, "Read from F0 reg %02x val %02x\n", F0_INT_MASK_REG, regval); 

    sdio_f0_writeb(card->func, HIM_DISABLE, F0_INT_MASK_REG, &ret);

    if (ret) {
        PRINTM(MWARN, "Unable to enable the host interrupt!\n");
        ret = MLAN_STATUS_FAILURE;
    }
    else
	ret = MLAN_STATUS_SUCCESS;

    return ret;
#endif
 	//sdio_disable_func(card->func);
//    sdio_claim_host(card->func);
    //for (i = 0; i<10; i++)
    //    ret = woal_read_reg(handle, i, &dat);
 //   sdio_claim_host(card->func);

//    return MLAN_STATUS_SUCCESS;
 //   PRINTM(MWARN, "================Try to disable================\n");
    return( disable_host_int_mask(handle, HIM_DISABLE) );
}

/** 
 *  @brief This function enables the host interrupts.
 *  
 *  @param priv    A pointer to wlan_private structure
 *  @return 	   MLAN_STATUS_SUCCESS
 */
int
woal_sdio_enable_host_int(moal_handle * handle)
{
    int ret = MLAN_STATUS_FAILURE;
    struct sdio_mmc_card *card = handle->card;
    int i;
    u8 dat;
    u8 regval;

    ENTER();

    if (!card || !card->func) {
        PRINTM(MFATAL, "Error: card or function is NULL!\n");
        return ret;
    }
#if 0

    regval = sdio_f0_readb(card->func, F0_INT_MASK_REG, &ret );
    
    if ( ret){
	PRINTM(MWARN, "Error returned from sdio_f0_readb %d\n", ret);
    }
    else
        PRINTM(MWARN, "Read from F0 reg %02x val %02x\n", F0_INT_MASK_REG, regval); 

    sdio_f0_writeb(card->func, HIM_ENABLE, F0_INT_MASK_REG, &ret);

    if (ret) {
        PRINTM(MWARN, "Unable to enable the host interrupt! error %d\n", ret);
        ret = MLAN_STATUS_FAILURE;
    }
    else
	ret = MLAN_STATUS_SUCCESS;

    return ret;
#endif

  //  return( enable_host_int_mask(handle, HIM_ENABLE) );
 	//sdio_enable_func(card->func);
#if 0
    sdio_writeb(card->func, 0x03, 0x04, &ret); 
    if (ret) {
        PRINTM(MWARN, "Unable to enable the host interrupt! error %d\n", ret);
        ret = MLAN_STATUS_FAILURE;
    }
#endif
//    for (i = 0; i<10; i++)
//        ret = woal_read_reg(handle, i, &dat);
//    sdio_release_host(card->func);

    PRINTM(MWARN, "----------------------Tried enable------------------\n");
/*      if (atomic_read(&handle->int_flag))
    {
        PRINTM(MWARN, "handle->int_flag = %d\n", atomic_read(&handle->int_flag));
	atomic_set(&handle->int_flag, 0);
    }
*/
    return( enable_host_int_mask(handle, HIM_ENABLE) );

//    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function de-registers the device.
 *  
 *  @param priv    A pointer to wlan_private structure
 *  @return 	   MLAN_STATUS_SUCCESS
 */
void
woal_unregister_dev(moal_handle * handle)
{
    struct sdio_mmc_card *card = handle->card;

    ENTER();

    if (!card || !card->func) {
        PRINTM(MFATAL, "Error: card or function is NULL!\n");
        goto done;
    }

    sdio_claim_host(card->func);
    sdio_release_irq(card->func);
    sdio_disable_func(card->func);
    sdio_release_host(card->func);

    sdio_set_drvdata(card->func, NULL);

  done:
    LEAVE();
    return ;
}

/** 
 *  @brief This function registers the device.
 *  
 *  @param priv    A pointer to wlan_private structure
 *  @return 	   MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
woal_register_dev(moal_handle * handle)
{
    int ret = MLAN_STATUS_FAILURE;
    //u8 reg;
    struct sdio_mmc_card *card = handle->card;
    struct sdio_func *func;

    ENTER();

    if (!card || !card->func) {
        PRINTM(MFATAL, "Error: card or function is NULL!\n");
        goto done;
    }

    func = card->func;

    if (!card->priv)
    	card->priv = handle;

 //   sdio_claim_host(func);

    ret = sdio_enable_func(func);
    if (ret) {
        PRINTM(MFATAL, "sdio_enable_func() failed: ret=%d\n", ret);
	ret = MLAN_STATUS_FAILURE;
        goto release_host;
    }

    ret = sdio_claim_irq(func, woal_sdio_interrupt);
    if (ret) {
        PRINTM(MFATAL, "sdio_claim_irq failed: ret=%d\n", ret);
	ret = MLAN_STATUS_FAILURE;
        goto disable_func;
    }

    ret = sdio_set_block_size(card->func, MLAN_SDIO_BLOCK_SIZE);
    if (ret) {
        PRINTM(MFATAL, "%s: cannot set SDIO block size\n", __FUNCTION__);
        ret = MLAN_STATUS_FAILURE;
        goto release_irq;
    }

    handle->hotplug_device = &func->dev;

    if (fw_name == NULL) {
        fw_name = DEFAULT_FW_NAME;
    }

  //  sdio_release_host(func);
    sdio_set_drvdata(func, card);

    //atomic_set(&card->in_int, 0);
    ret = MLAN_STATUS_SUCCESS;
    PRINTM(MINFO, "register device successful\n");
    goto done;

  release_irq:
    sdio_release_irq(func);
  disable_func:
    sdio_disable_func(func);
  release_host:
 //   sdio_release_host(func);
  done:
    LEAVE();
    return ret;
}

/** 
 *  @brief This function set bus clock on/off
 *  
 *  @param priv    A pointer to wlan_private structure
 *  @param option    TRUE--on , FALSE--off
 *  @return 	   MLAN_STATUS_SUCCESS
 */
int
woal_sdio_set_bus_clock(moal_handle * handle, u8 option)
{
    ENTER();
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

void woal_sdio_release_host(moal_handle * handle)
{
    struct sdio_mmc_card * card;

    ENTER();

    if (unlikely(!handle) )
    {
	PRINTM(MFATAL, "%s, handle is NULL\n", __func__);
	return;
    }
    card = (struct sdio_mmc_card *) handle->card;

    if ( unlikely (!card || !card->func) ) {
        PRINTM(MINFO, "%s:  card or func is NULL\n", __FUNCTION__);
        LEAVE();
        return;
    }

    sdio_release_host(card->func);

    LEAVE();
}

void woal_sdio_claim_host(moal_handle * handle)
{
    struct sdio_mmc_card * card; 

    ENTER();

    if (unlikely(!handle) )
    {
	PRINTM(MFATAL, "%s, handle is NULL\n", __func__);
	return;
    }
    card = (struct sdio_mmc_card *) handle->card;

    if ( unlikely (!card || !card->func) ) {
        PRINTM(MINFO, "%s:  card or func is NULL\n",  __FUNCTION__);
        LEAVE();
        return;
    }


    sdio_claim_host(card->func);

    LEAVE();
}

void woal_sdio_unmask(moal_handle * handle)
{
    u8 ireg;
    int ret;

    ENTER();

    if ((ret = woal_read_reg(handle, HOST_INTSTATUS_REG, &ireg))) {
        PRINTM(MWARN, "woal_read_reg: read int status register failed\n");
        goto out;
    }

    if (ireg != 0) {
        /* 
         * DN_LD_HOST_INT_STATUS and/or UP_LD_HOST_INT_STATUS
         * Clear the interrupt status register and re-enable the interrupt
         */
        PRINTM(MINFO, "sdio_ireg = 0x%x\n", ireg);
        woal_write_reg(handle, HOST_INTSTATUS_REG,
                        ~(ireg) & (DN_LD_HOST_INT_STATUS |
                                   UP_LD_HOST_INT_STATUS));
   }

out:
    LEAVE();
}

mlan_status
woal_get_cardregs(moal_handle * handle, t_u8 * mp_regs)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;

    ENTER();

    if ( !handle || !mp_regs)
    {
	    PRINTM(MFATAL, "moal_get_cardregs NULL handle or card regs\n");
	    ret = MLAN_STATUS_FAILURE;
	    return ret;
    }
    handle->mp_regs = mp_regs;
    PRINTM(MINFO, "woal_get_cardregs got regs @ 0x%08x\n", mp_regs);

    LEAVE();
    return ret;
}
