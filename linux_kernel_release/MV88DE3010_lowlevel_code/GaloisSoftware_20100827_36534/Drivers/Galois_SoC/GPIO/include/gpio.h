#ifndef _GPIO_H_
#define _GPIO_H_

#define PORT_DDR_IN		0
#define PORT_DDR_OUT	1

/****************************************************
 * FUNCTION: toggle GPIO port between high and low
 * PARAMS: port - GPIO port # (0 ~ 31)
 *         value - 1: high; 0: low
 * RETURN: 0 - succeed
 *        -1 - fail
 ***************************************************/
int GPIO_PortWrite(int port, int value);

/****************************************************
 * FUNCTION: read GPIO port status
 * PARAMS: port - GPIO port # (0 ~ 31)
 *         *value - pointer to port status
 * RETURN: 0 - succeed
 *        -1 - fail
 ***************************************************/
int GPIO_PortRead(int port, int *value);

/****************************************************
 * FUNCTION: pinmux init for the pin of GPIO port
 * PARAMS: port - GPIO port # (0 ~ 31)
 * RETURN: 0 - succeed
 *        -1 - fail
 * NOTE: Be sure that spi_master_init_iomapper is done.
 ***************************************************/
int GPIO_PinmuxInit(int port);

/****************************************************
 * FUNCTION: Configure IOmapper for GPIO port
 * PARAMS: port - GPIO port # (0 ~ 31)
 * 		   in - Set GPIO pin as IN or OUT
 * RETURN: 0 - succeed
 *        -1 - fail
 * NOTE: Be sure that spi_master_init_iomapper is done.
 ***************************************************/
int GPIO_IOmapperSetInOut(int port, int in);

/****************************************************
 * FUNCTION: Get the in/out status of GPIO pin at IOmapper
 * PARAMS: port - GPIO port # (0 ~ 31)
 * 		   *inout - return PORT_DDR_IN or PORT_DDR_OUT
 * RETURN: 0 - succeed
 *        -1 - fail
 * NOTE: Be sure that spi_master_init_iomapper is done.
 ***************************************************/
int GPIO_IOmapperGetInOut(int port, int *inout);

/****************************************************
 * FUNCTION: Set Galois GPIO pin as in or out
 * PARAMS: port - GPIO port # (0 ~ 31)
 * 		   in - 1: IN, 0: OUT
 * RETURN: 0 - succeed
 *        -1 - fail
 ***************************************************/
int GPIO_PortSetInOut(int port, int in);

/****************************************************
 * FUNCTION: Get direction of Galois GPIO pin: in or out
 * PARAMS: port - GPIO port # (0 ~ 31)
 * 		   *inout - PORT_DDR_IN: IN, PORT_DDR_OUT: OUT
 * RETURN: 0 - succeed
 *        -1 - fail
 ***************************************************/
int GPIO_PortGetInOut(int port, int *inout);

/****************************************************
 * FUNCTION: Get data of Galois GPIO pin
 * PARAMS: port - GPIO port # (0 ~ 31)
 * 		   *data - the data in APB_GPIO_SWPORTA_DR
 * RETURN: 0 - succeed
 *        -1 - fail
 ***************************************************/
int GPIO_PortGetData(int port, int *data);

/****************************************************
 * FUNCTION: Init interrupt for Galois GPIO pin, and set 
 * 			 interrupt level or edge, but keep interrupt closed.
 * PARAMS: port - GPIO port # (0 ~ 31)
 * 		   int_edge - 1: edge triggered, 0: level triggered.
 * 		   int_polarity - 1: rise edge/high level triggered.
 * 		   				  0: fall edge/low level triggered.
 * RETURN: 0 - succeed
 *        -1 - fail
 ***************************************************/
int GPIO_PortInitIRQ(int port, int int_edge, int int_polarity);

/****************************************************
 * FUNCTION: Enable interrupt for Galois GPIO pin
 * PARAMS: port - GPIO port # (0 ~ 31)
 * RETURN: 0 - succeed
 *        -1 - fail
 * NOTE: You also need to enable GPIO interrupt in ICTL.
 ***************************************************/
int GPIO_PortEnableIRQ(int port);

/****************************************************
 * FUNCTION: Disable interrupt for Galois GPIO pin
 * PARAMS: port - GPIO port # (0 ~ 31)
 * RETURN: 0 - succeed
 *        -1 - fail
 ***************************************************/
int GPIO_PortDisableIRQ(int port);

/****************************************************
 * FUNCTION: Lookup if there's interrupt for Galois GPIO pin
 * PARAMS: port - GPIO port # (0 ~ 31)
 * RETURN: 1 - yes, there's interrupt pending.
 * 		   0 - no, there's no interrupt pending.
 *        -1 - fail.
 ***************************************************/
int GPIO_PortHasInterrupt(int port);

/****************************************************
 * FUNCTION: Clear interrupt for Galois GPIO pin
 * PARAMS: port - GPIO port # (0 ~ 31)
 * RETURN: 0 - succeed.
 *        -1 - fail.
 ***************************************************/
int GPIO_PortClearInterrupt(int port);

#if defined(BERLIN)
//////////////////////////////////////////////////////////
// Only port 0-7 can support SM_GPIO interrupt
//////////////////////////////////////////////////////////

/****************************************************
 * FUNCTION: toggle SM_GPIO port between high and low
 * PARAMS: port - SM_GPIO port # (0 ~ 11)
 *         value - 1: high; 0: low
 * RETURN: 0 - succeed
 *        -1 - fail
 ***************************************************/
int SM_GPIO_PortWrite(int port, int value);

/****************************************************
 * FUNCTION: read SM_GPIO port status
 * PARAMS: port - SM_GPIO port # (0 ~ 11)
 *         *value - pointer to port status
 * RETURN: 0 - succeed
 *        -1 - fail
 ***************************************************/
int SM_GPIO_PortRead(int port, int *value);

/****************************************************
 * FUNCTION: Set Galois SM_GPIO pin as in or out
 * PARAMS: port - SM_GPIO port # (0 ~ 11)
 * 		   in - 1: IN, 0: OUT
 * RETURN: 0 - succeed
 *        -1 - fail
 ***************************************************/
int SM_GPIO_PortSetInOut(int port, int in);

/****************************************************
 * FUNCTION: Get direction of Galois SM_GPIO pin: in or out
 * PARAMS: port - SM_GPIO port # (0 ~ 11)
 * 		   *inout - PORT_DDR_IN: IN, PORT_DDR_OUT: OUT
 * RETURN: 0 - succeed
 *        -1 - fail
 ***************************************************/
int SM_GPIO_PortGetInOut(int port, int *inout);

/****************************************************
 * FUNCTION: Get data of Galois SM_GPIO pin
 * PARAMS: port - SM_GPIO port # (0 ~ 11)
 * 		   *data - the data in APB_GPIO_SWPORTA_DR
 * RETURN: 0 - succeed
 *        -1 - fail
 ***************************************************/
int SM_GPIO_PortGetData(int port, int *data);

/****************************************************
 * FUNCTION: Init interrupt for Galois SM_GPIO pin, and set 
 * 			 interrupt level or edge, but keep interrupt closed.
 * PARAMS: port - SM_GPIO port # (0 ~ 7)
 * 		   int_edge - 1: edge triggered, 0: level triggered.
 * 		   int_polarity - 1: rise edge/high level triggered.
 * 		   				  0: fall edge/low level triggered.
 * RETURN: 0 - succeed
 *        -1 - fail
 ***************************************************/
int SM_GPIO_PortInitIRQ(int port, int int_edge, int int_polarity);

/****************************************************
 * FUNCTION: Enable interrupt for Galois SM_GPIO pin
 * PARAMS: port - SM_GPIO port # (0 ~ 7)
 * RETURN: 0 - succeed
 *        -1 - fail
 * NOTE: You also need to enable SM_GPIO interrupt in ICTL.
 ***************************************************/
int SM_GPIO_PortEnableIRQ(int port);

/****************************************************
 * FUNCTION: Disable interrupt for Galois SM_GPIO pin
 * PARAMS: port - SM_GPIO port # (0 ~ 7)
 * RETURN: 0 - succeed
 *        -1 - fail
 ***************************************************/
int SM_GPIO_PortDisableIRQ(int port);

/****************************************************
 * FUNCTION: Lookup if there's interrupt for Galois SM_GPIO pin
 * PARAMS: port - SM_GPIO port # (0 ~ 7)
 * RETURN: 1 - yes, there's interrupt pending.
 * 		   0 - no, there's no interrupt pending.
 *        -1 - fail.
 ***************************************************/
int SM_GPIO_PortHasInterrupt(int port);

/****************************************************
 * FUNCTION: Clear interrupt for Galois SM_GPIO pin
 * PARAMS: port - SM_GPIO port # (0 ~ 7)
 * RETURN: 0 - succeed.
 *        -1 - fail.
 ***************************************************/
int SM_GPIO_PortClearInterrupt(int port);
#endif

#endif

