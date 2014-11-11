#ifndef _SPI_MASTER_H_
#define _SPI_MASTER_H_

// error code definitions
#define SSI_OK           1
#define SSI_EBADPARAM   -1      // bad input parameter
#define SSI_EBUSRW      -2      // bus read/write error
#define SSI_EBUSY       -3      // SSI master is busy

#define IOMAPPER_SPI_MASTER	0	// master id = 0
#define IOMAPPER_SPI_DEVID	0	// CSn=0

#ifdef __cplusplus__
extern "C" {
#endif

/**********************************************
 * global function declarations
 *********************************************/
/*********************************************************
 * FUNCTION: judge if the spi_bus is idle
 * PARAMS: master_id - id of SPI master to init
 * RETURN: SSI_OK - idle
 *         SSI_EBADPARAM - invalid parameter
 *         SSI_EBUSY - used by others
 ********************************************************/
int spi_master_is_idle(int master_id);

/**********************************************************************
 * FUNCTION: Initialize SSI master. 
 *           SSI master is disabled after init. But no manual enable
 *           required to start a transaction.
 *           More than one SSI masters could exist in system.
 * PARAMETERS: master_id - ID of SSI master to initialize
 * RETURN: SSI_OK - succeed
 *         SSI_EBADPARAM - invalid parameters
 *********************************************************************/
int spi_master_init(int master_id);

/*******************************************************************
 * FUNCTION: Disable SSI master.
 *           More than one SSI masters could exist in system.
 * PARAMETERS: master_id - ID of SSI master to initialize
 * RETURN: SSI_OK - succeed
 *         SSI_EBADPARAM - invalid parameters
 *******************************************************************/
int spi_master_disable(int master_id);

/************************************************************************
 * FUNCTION: Enable SSI master.
 *           This and above disable function are used to enable/disable
 *           SSI master for SPI handler.
 *           More than one SSI masters could exist in system.
 * PARAMETERS: master_id - ID of SSI master to initialize
 * RETURN: SSI_OK - succeed
 *         SSI_EBADPARAM - invalid parameters
 ************************************************************************/
int spi_master_enable(int master_id);

/*******************************************************************
 * FUNCTION: Set SPI master clock mode.
 * PARAMETERS: master_id - ID of SSI master to config
 *             mode - 0: polarity 0 & phase 0
 *                    1: polarity 0 & phase 1
 *                    2: polarity 1 & phase 0
 *                    3: polarity 1 & phase 1
 * RETURN: >=0 - master clock mode
 *         SSI_EBADPARAM - invalid parameters
 *         SSI_EBUSY - master is busy
 *******************************************************************/
int spi_master_set_clock_mode(int master_id, int mode);

/*******************************************************************
 * FUNCTION: Set SPI master speed.
 * PARAMETERS: master_id - ID of SSI master to config
 *             speed - master speed in unit of KHz
 * RETURN: >0 - master speed
 *         SSI_EBADPARAM - invalid parameters
 *         SSI_EBUSY - master is busy
 *******************************************************************/
int spi_master_set_speed(int master_id, int speed);

/*******************************************************************
 * FUNCTION: Set SPI master data frame width.
 * PARAMETERS: master_id - ID of SSI master to config
 *             width - data frame width in unit of bit
 * RETURN: >0 - master data frame width
 *         SSI_EBADPARAM - invalid parameters
 *         SSI_EBUSY - master is busy
 * NOTE: valid frame width is 4~16, while most SPI device work
 *       in 8-bit frame mode.
 *******************************************************************/
int spi_master_set_framewidth(int master_id, int width);

/*************************************************************************
 * FUNCTION: Select a SPI slave device to operate.
 * PARAMETERS: master_id - ID of SSI master
 *             dev_id - ID of SPI slave device
 *             flag - 1: select
 *                    0: de-select
 * RETURN: >=0 - ID of slave device selected
 *         SSI_EBADPARAM - invalid parameters
 * NOTE: this is a fake function, only used to validate the device id
 *       we keep this function is to be compatable with the middleware
 ************************************************************************/
int spi_master_device_select(int master_id, int dev_id, int flag);

/*******************************************************************
 * FUNCTION: half-duplex read/write frames.
 * PARAMETERS: master_id - ID of SSI master
 *             pwbuf - pointer to transmit data buffer
 *             wnum - number of frames to transmit
 *             prbuf - pointer to receive data buffer
 *             rnum - number of frames to receive
 * RETURN: SSI_OK - transfer succeed
 *         SSI_EBADPARAM - invalid parameters
 *         SSI_EBUSY - master is busy
 *         SSI_EBUSRW - master read/write fail
 * NOTE: the maximum value of wnum is 256+4,
 *       while the maximum value of rnum is 256
 *******************************************************************/
int spi_master_halfduplex_writeread_frames(int master_id, int dev_id, int *pwbuf, int wnum, int *prbuf, int rnum);

/**********************************************
 * SPI master interrupt service routine
 *********************************************/
void spi_master_irs(int master_id);

/**********************************************
 * To support IO-mapper programming -Yufeng
 **********************************************/
int spi_master_init_iomapper(int master_id);
int spi_master_exit_iomapper(int master_id);
int spi_master_rw_iomapper(int master_id, int dev_id, int rd, 
		unsigned int reg, unsigned int *val);

#ifdef __cplusplus__
}
#endif

#endif
