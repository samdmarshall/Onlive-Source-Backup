#ifndef _I2C_MASTER_H_
#define _I2C_MASTER_H_

// error code definitions
#ifndef I2C_OK
#define I2C_OK		1
#endif
#ifndef I2C_EBADPARAM
#define I2C_EBADPARAM	-1
#endif
#ifndef I2C_EUNSUPPORT
#define I2C_EUNSUPPORT	-2
#endif
#ifndef I2C_EBUSRW
#define I2C_EBUSRW	-8
#endif
#ifndef I2C_EBUSY
#define I2C_EBUSY	-9
#endif

// I2C bus speed types
#define I2C_STANDARD_SPEED	0 // only 100K, 400K and 3.4MHz are supported in standard type
#define I2C_CUSTOM_SPEED	1 // any speed can be specified by users in custom type

// I2C slave address type
#define I2C_7BIT_SLAVE_ADDR	0
#define I2C_10BIT_SLAVE_ADDR	1

#ifdef __cplusplus__
extern "C" {
#endif

/******************************************
 *  global function declarations
 *****************************************/
/*********************************************************
 * FUNCTION: judge if the i2c_bus is idle
 * PARAMS: master_id - id of I2C master to init
 * RETURN: I2C_OK - idle
 *         I2C_EBADPARAM - invalid parameter
 *         I2C_EBUSY - used by others
 ********************************************************/
int i2c_master_is_idle(int master_id);

/*********************************************************
 * FUNCTION: init I2C master: set default bus speed
 * PARAMS: master_id - id of I2C master to init
 * RETURN: I2C_OK - succeed
 *         I2C_EBADPARAM - invalid parameter
 ********************************************************/
int i2c_master_init(int master_id);

/********************************************************************
 * FUNCTION: set I2C master bus speed
 * PARAMS: master_id - id of I2C master to config
 *         type - STANDARD or CUSTOM
 *         speed - in unit of KHz
 * RETURN: I2C_OK - succeed
 *         I2C_EBUSY - I2C module is enabled
 *         I2C_EUNSUPPORT - not support
 * NOTE: in STANDARD type, only 100 and 400 KHz speed are supported
 ********************************************************************/
int i2c_master_set_speed(int master_id, int type, int speed);

/***********************************************************************
 * FUNCTION: read bytes from slave device
 * PARAMS: master_id - id of I2C master to operate
 *         slave_addr - address of slave device
 *         addr_type - address type: I2C_7BIT_SLAVE_ADDR or I2C_10BIT_SLAVE_ADDR
 *         pwbuf - pointer of write buffer
 *         wnum - number of bytes to write
 *         prbuf - pointer of read buffer
 *         rnum - number of bytes to read
 * RETURN: I2C_OK - succeed  *         I2C_EBUSY - I2C module is enabled
 *         I2C_EBUSRW - read fail
 * NOTE: maximum write bytes is 260, maximum read bytes is 256 in a single
 *       transaction
 ***********************************************************************/
int i2c_master_writeread_bytes(int master_id, int slave_addr, int addr_type, unsigned char *pwbuf, int wnum, unsigned char *prbuf, int rnum, int dum );

/************************************************
 *  i2c master interrupt service routine
 ***********************************************/
void i2c_master_irs(int master_id);

#ifdef __cplusplus__
}
#endif

#endif
