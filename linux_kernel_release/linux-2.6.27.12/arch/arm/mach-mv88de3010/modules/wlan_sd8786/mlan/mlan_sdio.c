/** @file mlan_sdio.c
 *
 *  @brief This file contains SDIO specific code
 * 
 *  Copyright (C) 2008-2009, Marvell International Ltd. 
 *  All Rights Reserved
 */

/********************************************************
Change log:
    10/27/2008: initial version
********************************************************/

#include "mlan.h"
#include "mlan_11d.h"
#include "mlan_join.h"
#include "mlan_scan.h"
#include "mlan_util.h"
#include "mlan_fw.h"
#include "mlan_main.h"
#include "mlan_init.h"
#include "mlan_tx.h"
#include "mlan_rx.h"
#include "mlan_wmm.h"
#include "mlan_11n.h"
#include "mlan_sdio.h"

/********************************************************
		Local Variables
********************************************************/

/********************************************************
		Global Variables
********************************************************/

/********************************************************
		Local Functions
********************************************************/
/** 
 *  @brief This function initialize the SDIO port
 *  
 *  @param pmadapter  A pointer to mlan_adapter structure
 *  @return 	   	  MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status
wlan_sdio_init_ioport(mlan_adapter * pmadapter)
{
    t_u32 reg;
    pmlan_callbacks pcb = &pmadapter->callbacks;

    pmadapter->ioport = 0;

    /* Read the IO port */
    if (MLAN_STATUS_SUCCESS ==
        pcb->moal_read_reg(pmadapter->pmoal_handle, IO_PORT_0_REG, &reg))
        pmadapter->ioport |= (reg & 0xff);
    else
        return MLAN_STATUS_FAILURE;

    if (MLAN_STATUS_SUCCESS ==
        pcb->moal_read_reg(pmadapter->pmoal_handle, IO_PORT_1_REG, &reg))
        pmadapter->ioport |= ((reg & 0xff) << 8);
    else
        return MLAN_STATUS_FAILURE;

    if (MLAN_STATUS_SUCCESS ==
        pcb->moal_read_reg(pmadapter->pmoal_handle, IO_PORT_2_REG, &reg))
        pmadapter->ioport |= ((reg & 0xff) << 16);
    else
        return MLAN_STATUS_FAILURE;

    PRINTM(MINFO, "SDIO FUNC1 IO port: 0x%x\n", pmadapter->ioport);

#define SDIO_INT_MASK       0x3F
    /* Set Host interrupt reset to read to clear */
    if (MLAN_STATUS_SUCCESS ==
        pcb->moal_read_reg(pmadapter->pmoal_handle, HOST_INT_RSR_REG, &reg)) {
        pcb->moal_write_reg(pmadapter->pmoal_handle, HOST_INT_RSR_REG,
                            reg | SDIO_INT_MASK);
    } else
        return MLAN_STATUS_FAILURE;

    /* Dnld/Upld ready set to auto reset */
    if (MLAN_STATUS_SUCCESS ==
        pcb->moal_read_reg(pmadapter->pmoal_handle, CARD_MISC_CFG_REG, &reg)) {
        pcb->moal_write_reg(pmadapter->pmoal_handle, CARD_MISC_CFG_REG,
                            reg | AUTO_RE_ENABLE_INT);
    } else
        return MLAN_STATUS_FAILURE;

    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function gets available SDIO port for reading cmd/data
 *  
 *  @param pmadapter  A pointer to mlan_adapter structure
 *  @param pport      A pointer to port number
 *  @return 	   	  MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status
wlan_get_rd_port(mlan_adapter * pmadapter, t_u8 * pport)
{
    t_u16 rd_bitmap = pmadapter->mp_rd_bitmap;

    PRINTM(MDATA, "wlan_get_rd_port: mp_rd_bitmap=0x%04x\n", rd_bitmap);

    if (!(rd_bitmap & (CTRL_PORT_MASK | DATA_PORT_MASK)))
        return MLAN_STATUS_FAILURE;

    if (pmadapter->mp_rd_bitmap & CTRL_PORT_MASK) {
        pmadapter->mp_rd_bitmap &= (t_u16) (~CTRL_PORT_MASK);
        *pport = CTRL_PORT;
        PRINTM(MDATA, "wlan_get_rd_port: port=%d mp_rd_bitmap=0x%04x\n", *pport,
               pmadapter->mp_rd_bitmap);
    } else {
        if (pmadapter->mp_rd_bitmap & (1 << pmadapter->curr_rd_port)) {
            pmadapter->mp_rd_bitmap &=
                (t_u16) (~(1 << pmadapter->curr_rd_port));
            *pport = pmadapter->curr_rd_port;

            if (++pmadapter->curr_rd_port == MAX_PORT)
                pmadapter->curr_rd_port = 1;
        } else {
            return MLAN_STATUS_FAILURE;
        }

        PRINTM(MDATA, "port=%d mp_rd_bitmap=0x%04x -> 0x%04x\n",
               *pport, rd_bitmap, pmadapter->mp_rd_bitmap);
    }
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function gets available SDIO port for writing data
 *  
 *  @param pmadapter  A pointer to mlan_adapter structure
 *  @param pport      A pointer to port number
 *  @return 	   	  MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status
wlan_get_wr_port_data(mlan_adapter * pmadapter, t_u8 * pport)
{
    t_u16 wr_bitmap = pmadapter->mp_wr_bitmap;

    PRINTM(MDATA, "wlan_get_wr_port_data: mp_wr_bitmap=0x%04x\n", wr_bitmap);

    if (!(wr_bitmap & pmadapter->mp_data_port_mask))
        return MLAN_STATUS_FAILURE;

    if (pmadapter->mp_wr_bitmap & (1 << pmadapter->curr_wr_port)) {
        pmadapter->mp_wr_bitmap &= (t_u16) (~(1 << pmadapter->curr_wr_port));
        *pport = pmadapter->curr_wr_port;
        if (++pmadapter->curr_wr_port == pmadapter->mp_end_port)
            pmadapter->curr_wr_port = 1;
    } else {
        pmadapter->data_sent = MTRUE;
        return MLAN_STATUS_RESOURCE;
    }

    PRINTM(MDATA, "port=%d mp_wr_bitmap=0x%04x -> 0x%04x\n",
           *pport, wr_bitmap, pmadapter->mp_wr_bitmap);

    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function polls the card status register.
 *  
 *  @param pmadapter  A pointer to mlan_adapter structure
 *  @param bits    	  the bit mask
 *  @return 	   	  MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status
wlan_sdio_poll_card_status(mlan_adapter * pmadapter, t_u8 bits)
{
    pmlan_callbacks pcb = &pmadapter->callbacks;
    t_u32 tries;
    t_u32 cs = 0;

    for (tries = 0; tries < MAX_POLL_TRIES; tries++) {
        if (pcb->moal_read_reg(pmadapter->pmoal_handle, CARD_STATUS_REG, &cs) !=
            MLAN_STATUS_SUCCESS)
            break;
        else if ((cs & bits) == bits)
            return MLAN_STATUS_SUCCESS;
        wlan_udelay(pmadapter, 10);
    }

    PRINTM(MWARN, "wlan_sdio_poll_card_status failed, tries = %d\n", tries);
    return MLAN_STATUS_FAILURE;
}

/** 
 *  @brief This function reads firmware status registers
 *  
 *  @param pmadapter A pointer to mlan_adapter structure
 *  @param dat	   A pointer to keep returned data
 *  @return 	   MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status
wlan_sdio_read_fw_status(mlan_adapter * pmadapter, t_u16 * dat)
{
    pmlan_callbacks pcb = &pmadapter->callbacks;
    t_u32 fws0 = 0, fws1 = 0;

    ENTER();
    if (MLAN_STATUS_SUCCESS !=
        pcb->moal_read_reg(pmadapter->pmoal_handle, CARD_FW_STATUS0_REG, &fws0))
        return MLAN_STATUS_FAILURE;

    if (MLAN_STATUS_SUCCESS !=
        pcb->moal_read_reg(pmadapter->pmoal_handle, CARD_FW_STATUS1_REG, &fws1))
        return MLAN_STATUS_FAILURE;

    *dat = (fws1 << 8) | fws0;
 
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**  @brief This function disables the host interrupts mask.
 *  
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param mask	   the interrupt mask
 *  @return 	   MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status
wlan_sdio_disable_host_int_mask(pmlan_adapter pmadapter, t_u8 mask)
{
    t_u32 host_int_mask = 0;
    pmlan_callbacks pcb = &pmadapter->callbacks;

    ENTER();

    /* Read back the host_int_mask register */
    if (MLAN_STATUS_SUCCESS !=
        pcb->moal_read_reg(pmadapter->pmoal_handle, HOST_INT_MASK_REG,
                           &host_int_mask)) {
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }

    /* Update with the mask and write back to the register */
    host_int_mask &= ~mask;

    if (MLAN_STATUS_SUCCESS !=
        pcb->moal_write_reg(pmadapter->pmoal_handle, HOST_INT_MASK_REG,
                            host_int_mask)) {
        PRINTM(MWARN, "Disable host interrupt failed\n");
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function enables the host interrupts mask
 *  
 *  @param pmadapter A pointer to mlan_adapter structure
 *  @param mask	   the interrupt mask
 *  @return 	   MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status
wlan_sdio_enable_host_int_mask(pmlan_adapter pmadapter, t_u8 mask)
{
    pmlan_callbacks pcb = &pmadapter->callbacks;

    ENTER();

    /* Simply write the mask to the register */
    if (MLAN_STATUS_SUCCESS !=
        pcb->moal_write_reg(pmadapter->pmoal_handle, HOST_INT_MASK_REG, mask)) {
        PRINTM(MWARN, "Enable host interrupt failed\n");
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function reads data from the card.
 *  
 *  @param pmadapter 	A pointer to mlan_adapter structure
 *  @param type	   	A pointer to keep type as data or command
 *  @param nb		A pointer to keep the data/cmd length returned in buffer
 *  @param pmbuf 	A pointer to the SDIO data/cmd buffer
 *  @param npayload	the length of data/cmd buffer
 *  @param ioport	the SDIO ioport
 *  @return 	   	MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status
wlan_sdio_card_to_host(mlan_adapter * pmadapter,
                       t_u32 * type, t_u32 * nb, pmlan_buffer pmbuf,
                       t_u32 npayload, t_u32 ioport)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    pmlan_callbacks pcb = &pmadapter->callbacks;

    ENTER();

    if (!pmbuf) {
        PRINTM(MWARN, "pmbuf NULL pointer received!\n");
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    ret = pcb->moal_read_data_sync(pmadapter->pmoal_handle, pmbuf, ioport, 0);

    if (ret != MLAN_STATUS_SUCCESS) {
        PRINTM(MERROR, "card_to_host, read iomem failed: %d\n", ret);
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }
    *nb = wlan_le16_to_cpu(*(t_u16 *) (pmbuf->pbuf + pmbuf->data_offset));
    if (*nb > npayload) {
        PRINTM(MERROR, "invalid packet, *nb=%d, npayload=%d\n", *nb, npayload);
        ret = MLAN_STATUS_FAILURE;
        goto exit;
    }

    DBG_HEXDUMP(MDAT_D, "SDIO Blk Rd", pmbuf->pbuf,
                MIN(npayload, MAX_DATA_DUMP_LEN));
    
    *type = wlan_le16_to_cpu(*(t_u16 *) (pmbuf->pbuf + pmbuf->data_offset + 2));

  exit:
    LEAVE();
    return ret;
}

/********************************************************
		Global functions
********************************************************/
/**
 *  @brief  This function downloads FW blocks to device
 *
 *  @param pmadapter	A pointer to mlan_adapter
 *  @param pmfw			A pointer to firmware image
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status
wlan_prog_fw_w_helper(IN pmlan_adapter pmadapter, IN pmlan_fw_image pmfw)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    pmlan_callbacks pcb = &pmadapter->callbacks;
    t_u8 *firmware = pmfw->pfw_buf;
    t_u32 firmwarelen = pmfw->fw_len;
    t_u32 offset = 0;
    t_u32 base0, base1;
    t_void *tmpfwbuf = MNULL;
    t_u32 tmpfwbufsz;
    t_u8 *fwbuf;
    mlan_buffer mbuf;
    t_u16 len = 0;
    t_u32 txlen = 0, tx_blocks = 0, tries = 0;
    t_u32 i = 0;

    ENTER();

    if (!firmwarelen) {
        PRINTM(MMSG, "No firmware image found! Terminating download\n");
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }

    PRINTM(MINFO, "Downloading FW image (%d bytes)\n", firmwarelen);

    tmpfwbufsz = WLAN_UPLD_SIZE;
    ret = pcb->moal_malloc(tmpfwbufsz, (t_u8 **) & tmpfwbuf);
    if ((ret != MLAN_STATUS_SUCCESS) || !tmpfwbuf) {
        PRINTM(MERROR,
               "Unable to allocate buffer for firmware. Terminating download\n");
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }
    memset(tmpfwbuf, 0, tmpfwbufsz);
    fwbuf = (t_u8 *) tmpfwbuf;

    /* Perform firmware data transfer */
    do {
        /* The host polls for the DN_LD_CARD_RDY and CARD_IO_READY bits */
        ret =
            wlan_sdio_poll_card_status(pmadapter,
                                       CARD_IO_READY | DN_LD_CARD_RDY);
        if (ret != MLAN_STATUS_SUCCESS) {
            PRINTM(MFATAL, "FW download with helper poll status timeout @ %d\n",
                   offset);
            goto done;
        }

        /* More data? */
        if (offset >= firmwarelen)
            break;

        for (tries = 0; tries < MAX_POLL_TRIES; tries++) {
            if ((ret = pcb->moal_read_reg(pmadapter->pmoal_handle,
                                          HOST_F1_RD_BASE_0,
                                          &base0)) != MLAN_STATUS_SUCCESS) {
                PRINTM(MWARN,
                       "Dev BASE0 register read failed:"
                       " base0=0x%04X(%d). Terminating download\n", base0,
                       base0);
                goto done;
            }
            if ((ret = pcb->moal_read_reg(pmadapter->pmoal_handle,
                                          HOST_F1_RD_BASE_1,
                                          &base1)) != MLAN_STATUS_SUCCESS) {
                PRINTM(MWARN,
                       "Dev BASE1 register read failed:"
                       " base1=0x%04X(%d). Terminating download\n", base1,
                       base1);
                goto done;
            }
            len = ((base1 & 0xff) << 8) | (base0 & 0xff);

            if (len)
                break;
            wlan_udelay(pmadapter, 10);
        }

        if (!len)
            break;
        else if (len > WLAN_UPLD_SIZE) {
            PRINTM(MFATAL, "FW download failure @ %d, invalid length %d\n",
                   offset, len);
            ret = MLAN_STATUS_FAILURE;
            goto done;
        }

        txlen = len;

        if (len & MBIT(0)) {
            i++;
            if (i > MAX_WRITE_IOMEM_RETRY) {
                PRINTM(MFATAL,
                       "FW download failure @ %d, over max retry count\n",
                       offset);
                ret = MLAN_STATUS_FAILURE;
                goto done;
            }
            PRINTM(MERROR, "FW CRC error indicated by the helper:"
                   " len = 0x%04X, txlen = %d\n", len, txlen);
            len &= ~MBIT(0);
            /* Setting this to 0 to resend from same offset */
            txlen = 0;
        } else {
            i = 0;

            /* Set blocksize to transfer - checking for last block */
            if (firmwarelen - offset < txlen) {
                txlen = firmwarelen - offset;
            }
            PRINTM(MINFO, ".");

            tx_blocks =
                (txlen + MLAN_SDIO_BLOCK_SIZE_FW_DNLD -
                 1) / MLAN_SDIO_BLOCK_SIZE_FW_DNLD;

            /* Copy payload to buffer */
            memmove(fwbuf, &firmware[offset], txlen);
        }

        /* Send data */
        memset(&mbuf, 0, sizeof(mlan_buffer));
        mbuf.pbuf = (t_u8 *) fwbuf;
        mbuf.data_len = tx_blocks * MLAN_SDIO_BLOCK_SIZE_FW_DNLD;

        ret =
            pcb->moal_write_data_sync(pmadapter->pmoal_handle, &mbuf,
                                      pmadapter->ioport, 0);
        if (ret != MLAN_STATUS_SUCCESS) {
            PRINTM(MERROR, "FW download, write iomem (%d) failed @ %d\n", i,
                   offset);
            if (pcb->
                moal_write_reg(pmadapter->pmoal_handle, CONFIGURATION_REG,
                               0x04) != MLAN_STATUS_SUCCESS) {
                PRINTM(MERROR, "write CFG reg failed\n");
            }
            ret = MLAN_STATUS_FAILURE;
            goto done;
        }

 //   	DBG_HEXDUMP(MDAT_D, "SDIO FW before", fwbuf,
 //            MAX_DATA_DUMP_LEN );

        offset += txlen;
    } while (MTRUE);

    PRINTM(MINFO, "\nFW download over, size %d bytes\n", offset);

    ret = MLAN_STATUS_SUCCESS;
  done:
    if (tmpfwbuf)
        pcb->moal_mfree((t_u8 *) tmpfwbuf);

    LEAVE();
    return ret;
}

/** 
 *  @brief This function checks if the firmware is ready to accept
 *  command or not.
 *  
 *  @param pmadapter  A pointer to mlan_adapter structure
 *  @param pollnum    Maximum polling number
 *  @return           MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_check_fw_status(mlan_adapter * pmadapter, t_u32 pollnum)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    t_u16 firmwarestat;
    t_u32 tries;

    ENTER();

    /* Wait for firmware initialization event */
    for (tries = 0; tries < pollnum; tries++) {
        if (MLAN_STATUS_SUCCESS !=
            (ret = wlan_sdio_read_fw_status(pmadapter, &firmwarestat)))
            continue;
        if (firmwarestat == FIRMWARE_READY) {
            ret = MLAN_STATUS_SUCCESS;
            break;
        } else {
            wlan_mdelay(pmadapter, 100);
            ret = MLAN_STATUS_FAILURE;
        }
    }

    if (ret != MLAN_STATUS_SUCCESS)
        goto done;

  done:
    LEAVE();
    return ret;
}

/**
 *  @brief  This function downloads firmware to card
 *
 *  @param pmadapter	A pointer to mlan_adapter
 *  @param pmfw			A pointer to firmware image
 *
 *  @return		MLAN_STATUS_SUCCESS or error code
 */
mlan_status
wlan_dnld_fw(IN pmlan_adapter pmadapter, IN pmlan_fw_image pmfw)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;

    ENTER();

    /* Download the firmware image via helper */
    ret = wlan_prog_fw_w_helper(pmadapter, pmfw);
    if (ret != MLAN_STATUS_SUCCESS) {
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }

    LEAVE();
    return ret;
}

/** 
 *  @brief This function probes the driver
 *  
 *  @param pmadapter  A pointer to mlan_adapter structure
 *  @return 	      MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_sdio_probe(pmlan_adapter pmadapter)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    t_u32 chiprev = 0;
    t_u32 bic = 0;
    pmlan_callbacks pcb = &pmadapter->callbacks;

    /* Get H/W revision number */
    if (MLAN_STATUS_SUCCESS !=
        pcb->moal_read_reg(pmadapter->pmoal_handle, CARD_REVISION_REG,
                           &chiprev)) {
        PRINTM(MFATAL, "cannot read hardware revision number from card\n");
    } else {
        PRINTM(MINFO, "revision=0x%x\n", chiprev);
        /* 
         * Enable async interrupt mode
         * It's only needed for SPI mode, not necessary for 1-bit/4-bit mode.
         */
        if (MLAN_STATUS_SUCCESS !=
            pcb->moal_read_reg(pmadapter->pmoal_handle,
                               BUS_INTERFACE_CONTROL_REG, &bic))
            return MLAN_STATUS_FAILURE;

        bic |= ASYNC_INT_MODE;
        if (MLAN_STATUS_SUCCESS !=
            pcb->moal_write_reg(pmadapter->pmoal_handle,
                                BUS_INTERFACE_CONTROL_REG, bic))
            return MLAN_STATUS_FAILURE;
    }
    /* Disable host interrupt mask register for SDIO */
    ret = wlan_disable_host_int(pmadapter);
    if (ret != MLAN_STATUS_SUCCESS)
        return MLAN_STATUS_FAILURE;

    /* Get SDIO ioport */
    ret = wlan_sdio_init_ioport(pmadapter);

    return ret;
}

/** 
 *  @brief This function gets interrupt status.
 *  
 *  @param adapter	A pointer to mlan_adapter structure
 *  @return 	    None
 */
t_void
mlan_interrupt(t_void * adapter)
{
    mlan_adapter *pmadapter = (mlan_adapter *) adapter;
    pmlan_callbacks pcb = &pmadapter->callbacks;
    mlan_buffer mbuf;
    t_u32 sdio_ireg = 0;
    unsigned int i;

    ENTER();

    memset(&mbuf, 0, sizeof(mlan_buffer));
    mbuf.pbuf = pmadapter->mp_regs;
    mbuf.data_len = MAX_MP_REGS;

#if 0

    if (MLAN_STATUS_SUCCESS !=
        pcb->moal_read_data_sync(pmadapter->pmoal_handle, &mbuf,
                                 REG_PORT | MLAN_SDIO_BYTE_MODE_MASK, 0)) {
        PRINTM(MWARN, "moal_read_data_sync: read registers failed\n");
        goto done;
    }

    for (i = 0; i< MAX_MP_REGS; i++)
    {
        if (MLAN_STATUS_SUCCESS !=
            pcb->moal_read_reg(pmadapter->pmoal_handle, i,
                                     &(pmadapter->mp_regs[i])) ) {
            PRINTM(MWARN, "moal_read_data_sync: read registers failed\n");
            goto done;
        }
    }

#endif
    DBG_HEXDUMP(MDAT_D, "SDIO MP Registers", pmadapter->mp_regs, MAX_MP_REGS);
    sdio_ireg = pmadapter->mp_regs[HOST_INTSTATUS_REG];
    if (sdio_ireg) {
        /* 
         * DN_LD_HOST_INT_STATUS and/or UP_LD_HOST_INT_STATUS
         * Clear the interrupt status register
         */
        PRINTM(MINTR, "sdio_ireg = 0x%x\n", sdio_ireg);
        pmadapter->sdio_ireg |= sdio_ireg;
    }
  done:
    LEAVE();
}

/** 
 *  @brief This function disables the host interrupts.
 *  
 *  @param pmadapter A pointer to mlan_adapter structure
 *  @return 	   MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_disable_host_int(pmlan_adapter pmadapter)
{
    mlan_status ret;

    ENTER();
    ret = wlan_sdio_disable_host_int_mask(pmadapter, HIM_DISABLE);
    LEAVE();
    return ret;
}

/** 
 *  @brief This function enables the host interrupts.
 *  
 *  @param pmadapter A pointer to mlan_adapter structure
 *  @return 	   MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_enable_host_int(pmlan_adapter pmadapter)
{
    mlan_status ret;

    ENTER();
    ret = wlan_sdio_enable_host_int_mask(pmadapter, HIM_ENABLE);
    LEAVE();
    return ret;
}

/** 
 *  @brief This function decodes the rx packet & 
 *  calls corresponding handlers according to the packet type
 *  
 *  @param pmadapter A pointer to mlan_adapter structure
 *  @param pmbuf      A pointer to the SDIO data/cmd buffer
 *  @param upld_typ  Type of rx packet
 *  @return 	   MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_decode_rx_packet(mlan_adapter * pmadapter, mlan_buffer * pmbuf,
                      t_u32 upld_typ)
{
    pmlan_callbacks pcb = &pmadapter->callbacks;
    t_u8 *cmdBuf;
    t_u32 event;

    switch (upld_typ) {
    case MLAN_TYPE_DATA:
        PRINTM(MINFO, "--- Rx: Data packet ---\n");
        pmbuf->data_len = (pmadapter->upld_len - INTF_HEADER_LEN);
        pmbuf->data_offset += INTF_HEADER_LEN;
        wlan_handle_rx_packet(pmadapter, pmbuf);
        break;

    case MLAN_TYPE_CMD:
        PRINTM(MINFO, "--- Rx: Cmd Response ---\n");
        DBG_HEXDUMP(MDAT_D, "Cmd RESP BUFFER", pmbuf->pbuf,
                    pmadapter->upld_len);
        /* take care of curr_cmd = NULL case */
        if (!pmadapter->curr_cmd) {
            cmdBuf = pmadapter->upld_buf;
            if (pmadapter->ps_state == PS_STATE_SLEEP_CFM) {
                wlan_process_sleep_confirm_resp(pmadapter,
                                                pmbuf->pbuf +
                                                pmbuf->data_offset +
                                                INTF_HEADER_LEN,
                                                pmadapter->upld_len -
                                                INTF_HEADER_LEN);
            }
            pmadapter->upld_len -= INTF_HEADER_LEN;
            memcpy(cmdBuf, pmbuf->pbuf + pmbuf->data_offset + INTF_HEADER_LEN,
                   MIN(MRVDRV_SIZE_OF_CMD_BUFFER,
                       pmadapter->upld_len - INTF_HEADER_LEN));
            pcb->moal_free_mlan_buffer(pmbuf);
        } else {
            pmadapter->cmd_resp_received = MTRUE;
            pmadapter->upld_len -= INTF_HEADER_LEN;
            pmbuf->data_len -= INTF_HEADER_LEN;
            pmbuf->data_offset += INTF_HEADER_LEN;
            pmadapter->curr_cmd->respbuf = pmbuf;
        }
        break;

    case MLAN_TYPE_EVENT:
        PRINTM(MINFO, "--- Rx: Event ---\n");

        event = *(t_u32 *) & pmbuf->pbuf[4];
        pmadapter->event_cause = wlan_le32_to_cpu(event);
        if ((pmadapter->upld_len > MLAN_EVENT_HEADER_LEN) &&
            ((pmadapter->upld_len - MLAN_EVENT_HEADER_LEN) < MAX_EVENT_SIZE)) {
            memcpy(pmadapter->event_body, pmbuf->pbuf + MLAN_EVENT_HEADER_LEN,
                   pmadapter->upld_len - MLAN_EVENT_HEADER_LEN);
        }

        /* event cause has been saved to adapter->event_cause */
        pmadapter->event_received = MTRUE;
        pmbuf->data_len = pmadapter->upld_len;
        pmadapter->pmlan_buffer_event = pmbuf;

        /* remove SDIO header */
        pmbuf->data_offset += INTF_HEADER_LEN;
        pmbuf->data_len -= INTF_HEADER_LEN;
        break;

    default:
        PRINTM(MERROR, "SDIO unknown upload type = 0x%x\n", upld_typ);
        pcb->moal_free_mlan_buffer(pmbuf);
        break;
    }

    return MLAN_STATUS_SUCCESS;
}

#ifdef SDIO_MULTI_PORT_RX_AGGR
/** 
 *  @brief This function receives data from the card in aggregate mode.
 *  
 *  @param pmadapter A pointer to mlan_adapter structure
 *  @param pmbuf      A pointer to the SDIO data/cmd buffer
 *  @param port      Current port on which packet needs to be rxed
 *  @param rx_len    Length of received packet
 *  @return 	     MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_sdio_card_to_host_mp_aggr(mlan_adapter * pmadapter, mlan_buffer
                               * pmbuf, t_u8 port, t_u16 rx_len)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    pmlan_callbacks pcb = &pmadapter->callbacks;
    t_s32 f_do_rx_aggr = 0;
    t_s32 f_do_rx_cur = 0;
    t_s32 f_aggr_cur = 0;
    mlan_buffer mbuf_aggr;
    mlan_buffer *mbuf_deaggr;
    t_s32 pind = 0;
    t_u32 pkt_len, pkt_type = 0;
    t_u8 *curr_ptr;

    ENTER();

    if (port == CTRL_PORT) {
        /* Read the command Resp without aggr */
        PRINTM(MINFO, "card_2_host_mp_aggr: No aggregation for cmd response\n");

        f_do_rx_cur = 1;
        goto rx_curr_single;
    }

    if (!pmadapter->mpa_rx.enabled) {
        PRINTM(MINFO, "card_2_host_mp_aggr: rx aggregation disabled !\n");

        f_do_rx_cur = 1;
        goto rx_curr_single;
    }

    if (pmadapter->mp_rd_bitmap & (~((t_u16) CTRL_PORT_MASK))) {
        /* Some more data RX pending */
        PRINTM(MINFO, "card_2_host_mp_aggr: Not last packet\n");

        if (MP_RX_AGGR_IN_PROGRESS(pmadapter)) {
            if (MP_RX_AGGR_BUF_HAS_ROOM(pmadapter, rx_len)) {
                f_aggr_cur = 1;
            } else {
                /* No room in Aggr buf, do rx aggr now */
                f_do_rx_aggr = 1;
                f_do_rx_cur = 1;
            }
        } else {
            /* Rx aggr not in progress */
            f_aggr_cur = 1;
        }

    } else {
        /* No more data RX pending */
        PRINTM(MINFO, "card_2_host_mp_aggr: Last packet\n");

        if (MP_RX_AGGR_IN_PROGRESS(pmadapter)) {
            f_do_rx_aggr = 1;
            if (MP_RX_AGGR_BUF_HAS_ROOM(pmadapter, rx_len)) {
                f_aggr_cur = 1;
            } else {
                /* No room in Aggr buf, do rx aggr now */
                f_do_rx_cur = 1;
            }
        } else {
            f_do_rx_cur = 1;
        }

    }

    if (f_aggr_cur) {
        PRINTM(MINFO, "Current packet aggregation.\n");
        /* Curr pkt can be aggregated */
        MP_RX_AGGR_SETUP(pmadapter, pmbuf, port, rx_len);

        if (MP_RX_AGGR_PKT_LIMIT_REACHED(pmadapter) ||
            MP_RX_AGGR_PORT_LIMIT_REACHED(pmadapter)) {
            PRINTM(MINFO,
                   "card_2_host_mp_aggr: Aggregation Packet limit reached\n");
            /* No more pkts allowed in Aggr buf, rx it */
            f_do_rx_aggr = 1;
        }

    }

    if (f_do_rx_aggr) {
        /* do aggr RX now */
        PRINTM(MINFO, "do_rx_aggr: num of packets: %d\n",
               pmadapter->mpa_rx.pkt_cnt);

        memset(&mbuf_aggr, 0, sizeof(mlan_buffer));

        mbuf_aggr.pbuf = (t_u8 *) pmadapter->mpa_rx.buf;
        mbuf_aggr.data_len = pmadapter->mpa_rx.buf_len;

        if (MLAN_STATUS_SUCCESS !=
            pcb->moal_read_data_sync(pmadapter->pmoal_handle, &mbuf_aggr,
                                     (pmadapter->ioport | 0x1000 |
                                      (pmadapter->mpa_rx.ports << 4)) +
                                     pmadapter->mpa_rx.start_port, 0)) {
            ret = MLAN_STATUS_FAILURE;
            goto done;
        }

        curr_ptr = pmadapter->mpa_rx.buf;

        for (pind = 0; pind < pmadapter->mpa_rx.pkt_cnt; pind++) {

            /* get curr PKT len & type */
            pkt_len = wlan_le16_to_cpu(*(t_u16 *) & curr_ptr[0]);
            pkt_type = wlan_le16_to_cpu(*(t_u16 *) & curr_ptr[2]);

            PRINTM(MINFO, "RX: [%d] pktlen: %d pkt_type: 0x%x\n", pind,
                   pkt_len, pkt_type);

            /* copy pkt to deaggr buf */
            mbuf_deaggr = pmadapter->mpa_rx.mbuf_arr[pind];
            memcpy(mbuf_deaggr->pbuf, curr_ptr, pkt_len);

            pmadapter->upld_len = pkt_len;
            /* Process de-aggr packet */
            wlan_decode_rx_packet(pmadapter, mbuf_deaggr, pkt_type);

            curr_ptr += pmadapter->mpa_rx.len_arr[pind];
        }

        MP_RX_AGGR_BUF_RESET(pmadapter);

    }

  rx_curr_single:
    if (f_do_rx_cur) {
        PRINTM(MINFO, "RX: f_do_rx_cur: port: %d rx_len: %d\n", port, rx_len);

        if (MLAN_STATUS_SUCCESS != wlan_sdio_card_to_host(pmadapter, &pkt_type,
                                                          (t_u32 *) &
                                                          pmadapter->upld_len,
                                                          pmbuf, rx_len,
                                                          pmadapter->ioport +
                                                          port)) {
            ret = MLAN_STATUS_FAILURE;
            goto done;
        }

        wlan_decode_rx_packet(pmadapter, pmbuf, pkt_type);

    }

  done:
    LEAVE();
    return ret;

}
#endif
/** 
 *  @brief This function checks the interrupt status and handle it accordingly.
 *  
 *  @param pmadapter A pointer to mlan_adapter structure
 *  @return 	   MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_process_int_status(t_void* adapter)
{
    mlan_adapter *pmadapter = (mlan_adapter *) adapter;
    mlan_status ret = MLAN_STATUS_SUCCESS;
    pmlan_callbacks pcb = &pmadapter->callbacks;
    t_u8 sdio_ireg = pmadapter->sdio_ireg;
    mlan_buffer *pmbuf = MNULL;
    t_u8 *tmp_buf = MNULL;
    t_u8 port = CTRL_PORT;
    t_u32 len_reg_l, len_reg_u;
    t_u16 rx_blocks;
    t_u16 rx_len;
#ifndef SDIO_MULTI_PORT_RX_AGGR
    t_u32 upld_typ = 0;
#endif

    ENTER();

    pcb->moal_spin_lock(pmadapter->pmain_proc_lock);

    /* Check if already processing */
    if (pmadapter->mlan_processing) {
        pcb->moal_spin_unlock(pmadapter->pmain_proc_lock);
	PRINTM(MFATAL, "Can't get IRQ handled\n");
        //goto done;
    } else {
        pmadapter->mlan_processing = MTRUE;
        pcb->moal_spin_unlock(pmadapter->pmain_proc_lock);
    }

    sdio_ireg = pmadapter->mp_regs[HOST_INTSTATUS_REG];
    if (sdio_ireg) {
        /* 
         * DN_LD_HOST_INT_STATUS and/or UP_LD_HOST_INT_STATUS
         * Clear the interrupt status register
         */
        PRINTM(MINTR, "sdio_ireg = 0x%x\n", sdio_ireg);
        pmadapter->sdio_ireg |= sdio_ireg;
    }

    if (!pmadapter->sdio_ireg)
        goto done;

    if (sdio_ireg & DN_LD_HOST_INT_STATUS) {
        pmadapter->mp_wr_bitmap =
            ((t_u16) pmadapter->mp_regs[WR_BITMAP_U]) << 8;
        pmadapter->mp_wr_bitmap |= (t_u16) pmadapter->mp_regs[WR_BITMAP_L];
        PRINTM(MINTR, "DNLD: wr_bitmap=0x%04x\n", pmadapter->mp_wr_bitmap);
        if (pmadapter->data_sent &&
            (pmadapter->mp_wr_bitmap & pmadapter->mp_data_port_mask)) {
            PRINTM(MINFO, " <--- Tx DONE Interrupt --->\n");
            pmadapter->data_sent = MFALSE;
        }
    }

    /* As firmware will not generate download ready interrupt if the port
       updated is command port only, cmd_sent should be done for any SDIO
       interrupt. */
    if (pmadapter->cmd_sent == MTRUE) {
        /* Check if firmware has attach buffer at command port and update just
           that in wr_bit_map. */
        pmadapter->mp_wr_bitmap |=
            (t_u16) pmadapter->mp_regs[WR_BITMAP_L] & CTRL_PORT_MASK;
        if (pmadapter->mp_wr_bitmap & CTRL_PORT_MASK)
            pmadapter->cmd_sent = MFALSE;

    }

    PRINTM(MINFO, "cmd_sent=%d data_sent=%d\n",
           pmadapter->cmd_sent, pmadapter->data_sent);
    if (sdio_ireg & UP_LD_HOST_INT_STATUS) {
        pmadapter->mp_rd_bitmap =
            ((t_u16) pmadapter->mp_regs[RD_BITMAP_U]) << 8;
        pmadapter->mp_rd_bitmap |= (t_u16) pmadapter->mp_regs[RD_BITMAP_L];
        PRINTM(MINTR, "UPLD: rd_bitmap=0x%04x\n", pmadapter->mp_rd_bitmap);

        while (MTRUE) {
            ret = wlan_get_rd_port(pmadapter, &port);
            if (ret != MLAN_STATUS_SUCCESS) {
                PRINTM(MINFO, "no more rd_port available\n");
                break;
            }
            len_reg_l = RD_LEN_P0_L + (port << 1);
            len_reg_u = RD_LEN_P0_U + (port << 1);
            rx_len = ((t_u16) pmadapter->mp_regs[len_reg_u]) << 8;
            rx_len |= (t_u16) pmadapter->mp_regs[len_reg_l];
            PRINTM(MINFO, "RX: port=%d rx_len=%u\n", port, rx_len);
            rx_blocks =
                (rx_len + MLAN_SDIO_BLOCK_SIZE - 1) / MLAN_SDIO_BLOCK_SIZE;
            if (rx_len <= INTF_HEADER_LEN ||
                (rx_blocks * MLAN_SDIO_BLOCK_SIZE) > ALLOC_BUF_SIZE) {
                PRINTM(MERROR, "invalid rx_len=%d\n", rx_len);
                ret = MLAN_STATUS_FAILURE;
                goto done;
            }
            rx_len = rx_blocks * MLAN_SDIO_BLOCK_SIZE;
            if (MLAN_STATUS_SUCCESS !=
                pcb->moal_alloc_mlan_buffer(rx_len + HEADER_ALIGNMENT,
                                            &pmbuf)) {
                PRINTM(MWARN, "Failed to allocate 'mlan_buffer'\n");
                ret = MLAN_STATUS_FAILURE;
                goto done;
            }
            tmp_buf =
                (t_u8 *) ALIGN_ADDR(pmbuf->pbuf + pmbuf->data_offset,
                                    HEADER_ALIGNMENT);
            pmbuf->data_offset += tmp_buf - (pmbuf->pbuf + pmbuf->data_offset);
            pmbuf->data_len = rx_len;
            PRINTM(MINFO, "rx_len = %d\n", rx_len);

#ifdef SDIO_MULTI_PORT_RX_AGGR
            if (MLAN_STATUS_SUCCESS !=
                wlan_sdio_card_to_host_mp_aggr(pmadapter, pmbuf, port,
                                               rx_len)) {
#else
            /* Transfer data from card */
            if (MLAN_STATUS_SUCCESS !=
                wlan_sdio_card_to_host(pmadapter, &upld_typ,
                                       (t_u32 *) & pmadapter->upld_len, pmbuf,
                                       rx_len, pmadapter->ioport + port)) {
#endif /* SDIO_MULTI_PORT_RX_AGGR */

                t_u32 cr = 0;

                PRINTM(MERROR, "Card to host failed: int status=0x%x\n",
                       sdio_ireg);
                if (MLAN_STATUS_SUCCESS !=
                    pcb->moal_read_reg(pmadapter->pmoal_handle,
                                       CONFIGURATION_REG, &cr))
                    PRINTM(MERROR, "read CFG reg failed\n");

                PRINTM(MINFO, "Config Reg val = %d\n", cr);
                if (MLAN_STATUS_SUCCESS !=
                    pcb->moal_write_reg(pmadapter->pmoal_handle,
                                        CONFIGURATION_REG, (cr | 0x04)))
                    PRINTM(MERROR, "write CFG reg failed\n");

                PRINTM(MINFO, "write success\n");
                if (MLAN_STATUS_SUCCESS !=
                    pcb->moal_read_reg(pmadapter->pmoal_handle,
                                       CONFIGURATION_REG, &cr))
                    PRINTM(MERROR, "read CFG reg failed\n");

                PRINTM(MINFO, "Config reg val =%x\n", cr);
                ret = MLAN_STATUS_FAILURE;
                pcb->moal_free_mlan_buffer(pmbuf);
                goto done;
            }
#ifndef SDIO_MULTI_PORT_RX_AGGR
            wlan_decode_rx_packet(pmadapter, pmbuf, upld_typ);
#endif

        }
    }

    ret = MLAN_STATUS_SUCCESS;

    pcb->moal_spin_lock(pmadapter->pmain_proc_lock);
    pmadapter->mlan_processing = MFALSE;
    pcb->moal_spin_unlock(pmadapter->pmain_proc_lock);
  done:
    pmadapter->sdio_ireg = 0;

    pcb->moal_spin_lock(pmadapter->pmain_proc_lock);
    pmadapter->mlan_processing = MFALSE;
    pcb->moal_spin_unlock(pmadapter->pmain_proc_lock);

    LEAVE();
    return ret;
}

/** 
 *  @brief This function sends data to the card.
 *  
 *  @param pmadapter A pointer to mlan_adapter structure
 *  @param type	     data or command
 *  @param pmbuf     A pointer to mlan_buffer (pmbuf->data_len should include SDIO header)
 *  @param tx_param  A pointer to mlan_tx_param
 *  @return 	     MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_sdio_host_to_card(mlan_adapter * pmadapter, t_u8 type, mlan_buffer * pmbuf,
                       mlan_tx_param * tx_param)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    pmlan_callbacks pcb = &pmadapter->callbacks;
    t_u32 buf_block_len;
    t_u32 blksz;
    t_u8 port = CTRL_PORT;
    t_u32 i = 0;
    t_u8 *payload = pmbuf->pbuf + pmbuf->data_offset;

    ENTER();

    /* Allocate buffer and copy payload */
    blksz = MLAN_SDIO_BLOCK_SIZE;
    buf_block_len = (pmbuf->data_len + blksz - 1) / blksz;
    *(t_u16 *) & payload[0] = wlan_cpu_to_le16(pmbuf->data_len);
    *(t_u16 *) & payload[2] = wlan_cpu_to_le16(type);

    /* 
     * This is SDIO specific header
     *  t_u16 length,
     *  t_u16 type (MLAN_TYPE_DATA = 0, MLAN_TYPE_CMD = 1, MLAN_TYPE_EVENT = 3) 
     */
    if (type == MLAN_TYPE_DATA) {
        ret = wlan_get_wr_port_data(pmadapter, &port);
        if (ret != MLAN_STATUS_SUCCESS) {
            PRINTM(MERROR, "no wr_port available: %d\n", ret);
            goto exit;
        }
    } else {
        pmadapter->cmd_sent = MTRUE;
        /* Type must be MLAN_TYPE_CMD */

        if (pmbuf->data_len <= INTF_HEADER_LEN ||
            pmbuf->data_len > WLAN_UPLD_SIZE)
            PRINTM(MWARN,
                   "wlan_sdio_host_to_card(): Error: payload=%p, nb=%d\n",
                   payload, pmbuf->data_len);
    }

    do {
        /* Transfer data to card */
        pmbuf->data_len = buf_block_len * blksz;

#ifdef SDIO_MULTI_PORT_TX_AGGR
        if (tx_param)
            ret =
                wlan_host_to_card_mp_aggr(pmadapter, pmbuf, port,
                                          tx_param->next_pkt_len);
        else
            ret = wlan_host_to_card_mp_aggr(pmadapter, pmbuf, port, 0);
#else
        ret =
            pcb->moal_write_data_sync(pmadapter->pmoal_handle, pmbuf,
                                      pmadapter->ioport + port, 0);
#endif /* SDIO_MULTI_PORT_TX_AGGR */
        if (ret != MLAN_STATUS_SUCCESS) {
            i++;
            PRINTM(MERROR, "host_to_card, write iomem (%d) failed: %d\n", i,
                   ret);
            if (MLAN_STATUS_SUCCESS !=
                pcb->moal_write_reg(pmadapter->pmoal_handle, CONFIGURATION_REG,
                                    0x04)) {
                PRINTM(MERROR, "write CFG reg failed\n");
            }
            ret = MLAN_STATUS_FAILURE;
            if (i > MAX_WRITE_IOMEM_RETRY) {
                if (type == MLAN_TYPE_CMD)
                    pmadapter->cmd_sent = MFALSE;
                if (type == MLAN_TYPE_DATA)
                    pmadapter->data_sent = MFALSE;
                goto exit;
            }
        } else {
            if (type == MLAN_TYPE_DATA) {
                if (!(pmadapter->mp_wr_bitmap & (1 << pmadapter->curr_wr_port)))
                    pmadapter->data_sent = MTRUE;
                else
                    pmadapter->data_sent = MFALSE;
            }
            DBG_HEXDUMP(MDAT_D, "SDIO Blk Wr", pmbuf->pbuf + pmbuf->data_offset,
                        MIN(pmbuf->data_len, MAX_DATA_DUMP_LEN));
        }
    } while (ret == MLAN_STATUS_FAILURE);

  exit:
    LEAVE();
    return ret;
}

#ifdef SDIO_MULTI_PORT_TX_AGGR
/** 
 *  @brief This function sends data to the card in SDIO aggregated mode.
 *  
 *  @param pmadapter A pointer to mlan_adapter structure
 *  @param mbuf      A pointer to the SDIO data/cmd buffer
 *  @param port	     current port for aggregation
 *  @param next_pkt_len Length of next packet used for multiport aggregation
 *  @return 	     MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_host_to_card_mp_aggr(mlan_adapter * pmadapter, mlan_buffer * mbuf,
                          t_u8 port, t_u32 next_pkt_len)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    pmlan_callbacks pcb = &pmadapter->callbacks;
    t_s32 f_send_aggr_buf = 0;
    t_s32 f_send_cur_buf = 0;
    t_s32 f_precopy_cur_buf = 0;
    t_s32 f_postcopy_cur_buf = 0;
    mlan_buffer mbuf_aggr;

    ENTER();

    PRINTM(MDAT_D, "host_2_card_mp_aggr: next_pkt_len: %d curr_port:%d\n",
           next_pkt_len, port);

    if ((!pmadapter->mpa_tx.enabled) || (port == CTRL_PORT)) {
        PRINTM(MINFO, "host_2_card_mp_aggr: tx aggregation disabled !\n");

        f_send_cur_buf = 1;
        goto tx_curr_single;
    }

    if (next_pkt_len) {
        /* More pkt in TX queue */
        PRINTM(MINFO, "host_2_card_mp_aggr: More packets in Queue.\n");

        if (MP_TX_AGGR_IN_PROGRESS(pmadapter)) {
            if (!MP_TX_AGGR_PORT_LIMIT_REACHED(pmadapter) &&
                MP_TX_AGGR_BUF_HAS_ROOM(pmadapter, mbuf, mbuf->data_len)) {
                f_precopy_cur_buf = 1;

                if (!(pmadapter->mp_wr_bitmap & (1 << pmadapter->curr_wr_port))
                    || !MP_TX_AGGR_BUF_HAS_ROOM(pmadapter, mbuf,
                                                next_pkt_len)) {
                    f_send_aggr_buf = 1;
                }
            } else {
                /* No room in Aggr buf, send it */
                f_send_aggr_buf = 1;

                if (MP_TX_AGGR_PORT_LIMIT_REACHED(pmadapter) ||
                    !(pmadapter->
                      mp_wr_bitmap & (1 << pmadapter->curr_wr_port))) {
                    f_send_cur_buf = 1;
                } else {
                    f_postcopy_cur_buf = 1;
                }
            }
        } else {
            if (MP_TX_AGGR_BUF_HAS_ROOM(pmadapter, mbuf, mbuf->data_len) &&
                (pmadapter->mp_wr_bitmap & (1 << pmadapter->curr_wr_port)))
                f_precopy_cur_buf = 1;
            else
                f_send_cur_buf = 1;
        }
    } else {
        /* Last pkt in TX queue */
        PRINTM(MINFO, "host_2_card_mp_aggr: Last packet in Tx Queue.\n");

        if (MP_TX_AGGR_IN_PROGRESS(pmadapter)) {
            /* some packs in Aggr buf already */
            f_send_aggr_buf = 1;

            if (MP_TX_AGGR_BUF_HAS_ROOM(pmadapter, mbuf, mbuf->data_len)) {
                f_precopy_cur_buf = 1;
            } else {
                /* No room in Aggr buf, send it */
                f_send_cur_buf = 1;
            }
        } else {
            f_send_cur_buf = 1;
        }
    }

    if (f_precopy_cur_buf) {
        PRINTM(MDATA, "host_2_card_mp_aggr: Precopy current buffer\n");
        MP_TX_AGGR_BUF_PUT(pmadapter, mbuf, port);

        if (MP_TX_AGGR_PKT_LIMIT_REACHED(pmadapter) ||
            MP_TX_AGGR_PORT_LIMIT_REACHED(pmadapter)) {
            PRINTM(MDAT_D,
                   "host_2_card_mp_aggr: Aggregation Pkt limit reached\n");
            /* No more pkts allowed in Aggr buf, send it */
            f_send_aggr_buf = 1;
        }
    }

    if (f_send_aggr_buf) {
        PRINTM(MDATA, "host_2_card_mp_aggr: Send aggregation buffer."
               "%d %d\n", pmadapter->mpa_tx.start_port,
               pmadapter->mpa_tx.ports);

        memset(&mbuf_aggr, 0, sizeof(mlan_buffer));

        mbuf_aggr.pbuf = (t_u8 *) pmadapter->mpa_tx.buf;
        mbuf_aggr.data_len = pmadapter->mpa_tx.buf_len;

        ret = pcb->moal_write_data_sync(pmadapter->pmoal_handle, &mbuf_aggr,
                                        (pmadapter->ioport | 0x1000 |
                                         (pmadapter->mpa_tx.ports << 4)) +
                                        pmadapter->mpa_tx.start_port, 0);

        MP_TX_AGGR_BUF_RESET(pmadapter);
    }

  tx_curr_single:
    if (f_send_cur_buf) {
        PRINTM(MDATA, "host_2_card_mp_aggr: Send current buffer %d\n", port);
        ret = pcb->moal_write_data_sync(pmadapter->pmoal_handle, mbuf,
                                        pmadapter->ioport + port, 0);
    }

    if (f_postcopy_cur_buf) {
        PRINTM(MDATA, "host_2_card_mp_aggr: Postcopy current buffer\n");
        MP_TX_AGGR_BUF_PUT(pmadapter, mbuf, port);
    }

    LEAVE();
    return ret;
}
#endif /* SDIO_MULTI_PORT_TX_AGGR */

#if defined(SDIO_MULTI_PORT_TX_AGGR) || defined(SDIO_MULTI_PORT_RX_AGGR)
/** 
 *  @brief This function allocates buffer for the SDIO aggregation buffer 
 *  		related members of adapter structure 
 *  
 *  @param pmadapter       A pointer to mlan_adapter structure
 *  @param mpa_tx_buf_size Tx buffer size to allocate
 *  @param mpa_rx_buf_size Rx buffer size to allocate
 *
 *  @return        MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_alloc_sdio_mpa_buffers(IN mlan_adapter * pmadapter,
                            t_u32 mpa_tx_buf_size, t_u32 mpa_rx_buf_size)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    pmlan_callbacks pcb = &pmadapter->callbacks;

#ifdef SDIO_MULTI_PORT_TX_AGGR
    ret =
        pcb->moal_malloc(mpa_tx_buf_size + HEADER_ALIGNMENT,
                         (t_u8 **) & pmadapter->mpa_tx.head_ptr);
    if (ret != MLAN_STATUS_SUCCESS || !pmadapter->mpa_tx.head_ptr) {
        PRINTM(MERROR, "Could not allocate buffer for SDIO MP TX aggr\n");
        ret = MLAN_STATUS_FAILURE;
        goto error;
    }
    pmadapter->mpa_tx.buf =
        (t_u8 *) ALIGN_ADDR(pmadapter->mpa_tx.head_ptr, HEADER_ALIGNMENT);
    pmadapter->mpa_tx.buf_size = mpa_tx_buf_size;
#endif /* SDIO_MULTI_PORT_TX_AGGR */

#ifdef SDIO_MULTI_PORT_RX_AGGR
    ret =
        pcb->moal_malloc(mpa_rx_buf_size + HEADER_ALIGNMENT,
                         (t_u8 **) & pmadapter->mpa_rx.head_ptr);
    if (ret != MLAN_STATUS_SUCCESS || !pmadapter->mpa_rx.head_ptr) {
        PRINTM(MERROR, "Could not allocate buffer for SDIO MP RX aggr\n");
        ret = MLAN_STATUS_FAILURE;
        goto error;
    }
    pmadapter->mpa_rx.buf =
        (t_u8 *) ALIGN_ADDR(pmadapter->mpa_rx.head_ptr, HEADER_ALIGNMENT);
    pmadapter->mpa_rx.buf_size = mpa_rx_buf_size;
#endif /* SDIO_MULTI_PORT_RX_AGGR */
  error:
    if (ret != MLAN_STATUS_SUCCESS) {
        wlan_free_sdio_mpa_buffers(pmadapter);
    }

    LEAVE();
    return ret;
}

/** 
 *  @brief This function frees buffers for the SDIO aggregation
 *  
 *  @param pmadapter       A pointer to mlan_adapter structure
 *
 *  @return        MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_free_sdio_mpa_buffers(IN mlan_adapter * pmadapter)
{
    pmlan_callbacks pcb = &pmadapter->callbacks;

#ifdef SDIO_MULTI_PORT_TX_AGGR
    if (pmadapter->mpa_tx.buf) {
        pcb->moal_mfree((t_u8 *) pmadapter->mpa_tx.head_ptr);
        pmadapter->mpa_tx.head_ptr = MNULL;
        pmadapter->mpa_tx.buf = MNULL;
        pmadapter->mpa_tx.buf_size = 0;
    }
#endif /* SDIO_MULTI_PORT_TX_AGGR */

#ifdef SDIO_MULTI_PORT_RX_AGGR
    if (pmadapter->mpa_rx.buf) {
        pcb->moal_mfree((t_u8 *) pmadapter->mpa_rx.head_ptr);
        pmadapter->mpa_rx.head_ptr = MNULL;
        pmadapter->mpa_rx.buf = MNULL;
        pmadapter->mpa_rx.buf_size = 0;
    }
#endif /* SDIO_MULTI_PORT_RX_AGGR */

    return MLAN_STATUS_SUCCESS;
}
#endif /* SDIO_MULTI_PORT_TX_AGGR || SDIO_MULTI_PORT_RX_AGGR */

/** GPIO IRQ callback function */
t_void
mlan_hs_callback(IN t_void * pctx)
{
    t_u32 i;
    mlan_adapter *pmadapter = (mlan_adapter *) pctx;

    ENTER();

    for (i = 0; i <= MLAN_MAX_BSS_NUM; i++) {
        if (pmadapter->priv[i])
            wlan_host_sleep_wakeup_event(pmadapter->priv[i]);
    }

    LEAVE();
}

/**
 *  @brief  This function issues commands to initialize firmware
 *
 *  @param priv     	A pointer to mlan_private structure
 *
 *  @return		MLAN_STATUS_SUCCESS or error code
 */
mlan_status
wlan_set_sdio_gpio_int(IN pmlan_private priv)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    pmlan_adapter pmadapter = priv->adapter;
    HostCmd_DS_SDIO_GPIO_INT_CONFIG sdio_int_cfg;

    ENTER();

    if (pmadapter->int_mode == INTMODE_GPIO) {
        PRINTM(MINFO, "SDIO_GPIO_INT_CONFIG: interrupt mode is GPIO\n");
        sdio_int_cfg.action = HostCmd_ACT_GEN_SET;
        sdio_int_cfg.gpio_pin = pmadapter->gpio_pin;
        sdio_int_cfg.gpio_int_edge = INT_FALLING_EDGE;
        sdio_int_cfg.gpio_pulse_width = DELAY_1_US;
        ret = wlan_prepare_cmd(priv, HostCmd_CMD_SDIO_GPIO_INT_CONFIG,
                               HostCmd_ACT_GEN_SET, 0, MNULL, &sdio_int_cfg);

        if (ret) {
            PRINTM(MERROR, "SDIO_GPIO_INT_CONFIG: send command fail\n");
            ret = MLAN_STATUS_FAILURE;
        }
    } else {
        PRINTM(MINFO, "SDIO_GPIO_INT_CONFIG: interrupt mode is SDIO\n");
    }

    LEAVE();
    return ret;
}

/**
 *  @brief This function prepares command of SDIO GPIO interrupt
 *  
 *  @param pmpriv	A pointer to mlan_private structure
 *  @param cmd	   	A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   The action: GET or SET
 *  @param pdata_buf    A pointer to data buffer
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_cmd_sdio_gpio_int(pmlan_private pmpriv,
                       IN HostCmd_DS_COMMAND * cmd,
                       IN t_u16 cmd_action, IN t_void * pdata_buf)
{
    HostCmd_DS_SDIO_GPIO_INT_CONFIG *psdio_gpio_int =
        &cmd->params.sdio_gpio_int;

    ENTER();

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_SDIO_GPIO_INT_CONFIG);
    cmd->size =
        wlan_cpu_to_le16((sizeof(HostCmd_DS_SDIO_GPIO_INT_CONFIG)) + S_DS_GEN);

    memset(psdio_gpio_int, 0, sizeof(HostCmd_DS_SDIO_GPIO_INT_CONFIG));
    if (cmd_action == HostCmd_ACT_GEN_SET) {
        memcpy(psdio_gpio_int, pdata_buf,
               sizeof(HostCmd_DS_SDIO_GPIO_INT_CONFIG));
        psdio_gpio_int->action = wlan_cpu_to_le16(psdio_gpio_int->action);
        psdio_gpio_int->gpio_pin = wlan_cpu_to_le16(psdio_gpio_int->gpio_pin);
        psdio_gpio_int->gpio_int_edge =
            wlan_cpu_to_le16(psdio_gpio_int->gpio_int_edge);
        psdio_gpio_int->gpio_pulse_width =
            wlan_cpu_to_le16(psdio_gpio_int->gpio_pulse_width);
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}
