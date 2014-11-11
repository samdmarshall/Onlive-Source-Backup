#define _GPIO_C_
/*
 * Programming sample:
 * 1. spi_master_init_iomapper(IOMAPPER_SPI_MASTER), init IOmapper
 * 2. GPIO_PinmuxInit(port), setting both Galois and IOmapper pinmux
 * 3.1 GPIO_IOmapperSetInOut(port, in), setting GPIO pin as in / out.
 * 3.2 GPIO_PortSetInOut(port, in), setting GPIO pin as in /out.
 * 4. GPIO_PortWrite, GPIOPortRead
 *
 * NOTE: GPIO_PinmuxInit shouldn't setup GPIO[13:10], it's SPI#0 for
 * programming IOmapper. Galois GPIO in / out setting is done in 
 * GPIO_PortWrite / GPIOPortRead, just for campatible forward with Berlin.
 *
 * NOTE: for Berlin, #1.spi_master_init_iomapper and #3.1 is descarded.
 */

#include "com_type.h"
#include "apb_perf_base.h"
#include "galois_io.h"
#include "apb_gpio.h"
#include "global.h"
#include "spi_master.h"
#include "dv_iomapper.h"
#include "gpio.h"
#ifdef __LINUX_KERNEL__
#include <linux/module.h>
#endif

/****************************************************
 * FUNCTION: toggle GPIO port between high and low
 * PARAMS: port - GPIO port # (0 ~ 31)
 *         value - 1: high; 0: low
 * RETURN: 0 - succeed
 *        -1 - fail
 ***************************************************/
int GPIO_PortWrite(int port, int value)
{
    int reg_ddr, reg_dr;
    int ddr, dr;

    if((port >= 0) && (port < 8)){
        reg_ddr = APB_GPIO_INST0_BASE + APB_GPIO_SWPORTA_DDR;
        reg_dr = APB_GPIO_INST0_BASE + APB_GPIO_SWPORTA_DR;
    } else if ((port >= 8) && (port < 16)){
        reg_ddr = APB_GPIO_INST1_BASE + APB_GPIO_SWPORTA_DDR;
        reg_dr = APB_GPIO_INST1_BASE + APB_GPIO_SWPORTA_DR;
        port -= 8;
    } else if ((port >= 16) && (port < 24)){
        reg_ddr = APB_GPIO_INST2_BASE + APB_GPIO_SWPORTA_DDR;
        reg_dr = APB_GPIO_INST2_BASE + APB_GPIO_SWPORTA_DR;
        port -= 16;
    } else if ((port >= 24) && (port < 32)){
        reg_ddr = APB_GPIO_INST3_BASE + APB_GPIO_SWPORTA_DDR;
        reg_dr = APB_GPIO_INST3_BASE + APB_GPIO_SWPORTA_DR;
        port -= 24;
    } else
        return -1;

    /* set port to output mode */
    GA_REG_WORD32_READ(reg_ddr, &ddr);
    ddr |= (1<<port);
    GA_REG_WORD32_WRITE(reg_ddr, ddr);

    /* set port value */
    GA_REG_WORD32_READ(reg_dr, &dr);
    if (value){
        dr |= (1<<port);
    } else {
        dr &= ~(1<<port);
    }
    GA_REG_WORD32_WRITE(reg_dr, dr);

    return 0;
}

/****************************************************
 * FUNCTION: read GPIO port status
 * PARAMS: port - GPIO port # (0 ~ 31)
 *         *value - pointer to port status
 * RETURN: 0 - succeed
 *        -1 - fail
 ***************************************************/
int GPIO_PortRead(int port, int *value)
{
    int reg_ddr, reg_ext;
    int ddr, ext;

    if((port >= 0) && (port < 8)){
        reg_ddr = APB_GPIO_INST0_BASE + APB_GPIO_SWPORTA_DDR;
        reg_ext = APB_GPIO_INST0_BASE + APB_GPIO_EXT_PORTA;
    } else if ((port >= 8) && (port < 16)){
        reg_ddr = APB_GPIO_INST1_BASE + APB_GPIO_SWPORTA_DDR;
        reg_ext = APB_GPIO_INST1_BASE + APB_GPIO_EXT_PORTA;
        port -= 8;
    } else if ((port >= 16) && (port < 24)){
        reg_ddr = APB_GPIO_INST2_BASE + APB_GPIO_SWPORTA_DDR;
        reg_ext = APB_GPIO_INST2_BASE + APB_GPIO_EXT_PORTA;
        port -= 16;
    } else if ((port >= 24) && (port < 32)){
        reg_ddr = APB_GPIO_INST3_BASE + APB_GPIO_SWPORTA_DDR;
        reg_ext = APB_GPIO_INST3_BASE + APB_GPIO_EXT_PORTA;
        port -= 24;
    } else
        return -1;

    /* set port to input mode */
    GA_REG_WORD32_READ(reg_ddr, &ddr);
    ddr &= ~(1<<port);
    GA_REG_WORD32_WRITE(reg_ddr, ddr);

    /* get port value */
    GA_REG_WORD32_READ(reg_ext, &ext);
    if (ext & (1<<port))
        *value = 1;
    else
        *value = 0;

    return 0;
}

/****************************************************
 * FUNCTION: pinmux init for the pin of GPIO port
 * PARAMS: port - GPIO port # (0 ~ 31)
 * RETURN: 0 - succeed
 *        -1 - fail
 * NOTE: Be sure that spi_master_init_iomapper is done.
 ***************************************************/
#ifdef BERLIN
int GPIO_PinmuxInit(int port)
{
	T32Gbl_pinMux reg;
#if (BERLIN_CHIP_VERSION >= BERLIN_B_0)
	T32Gbl_pinMux1 reg1;
#endif

	/* readout Galois pin mux */
	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux), &(reg.u32));
#if (BERLIN_CHIP_VERSION >= BERLIN_B_0)
	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux1), &(reg1.u32));
#endif

	/* setting Galois&IOmapper pin mux */
	switch (port) {
		case 0: /* gp0:m1 */
			reg.upinMux_gp0 = Gbl_pinMux_gp0_MODE_1;
			break;
		case 1: /* gp1:m1 */
			reg.upinMux_gp1 = Gbl_pinMux_gp1_MODE_1;
			break;
		case 2: /* gp2:m0 */
			reg.upinMux_gp2 = Gbl_pinMux_gp2_MODE_0;
			break;
		case 3: /* gp3:m2 */
			reg.upinMux_gp3 = Gbl_pinMux_gp3_MODE_2;
			break;
		case 4: /* gp4:m1 */
		case 5:
		case 6:
			reg.upinMux_gp4 = Gbl_pinMux_gp4_MODE_1;
			break;
		case 7: /* gp5:m0 */
		case 8:
		case 9:
		case 10:
		case 11:
			reg.upinMux_gp5 = Gbl_pinMux_gp5_MODE_0;
			break;
		case 12: /* gp6:m0 */
		case 13:
		case 14:
		case 15:
		case 16:
			reg.upinMux_gp6 = Gbl_pinMux_gp6_MODE_0;
			break;
		case 17: /* gp7:m0 */
			reg.upinMux_gp7 = Gbl_pinMux_gp7_MODE_0;
			break;
		case 18: /* gp8:m0 */
			reg.upinMux_gp8 = Gbl_pinMux_gp8_MODE_0;
			break;
		case 19: /* gp9:m0 */
		case 20:
		case 21:
		case 22:
		case 23:
			reg.upinMux_gp9 = Gbl_pinMux_gp9_MODE_0;
			break;
		case 24: /* gp10:m2 */
		case 25:
			reg.upinMux_gp10 = Gbl_pinMux_gp10_MODE_2;
			break;
		case 26: /* gp11:m2 */
		case 27:
			reg.upinMux_gp11 = Gbl_pinMux_gp11_MODE_2;
			break;
		case 28: /* gp14:m0 */
			reg.upinMux_gp14 = Gbl_pinMux_gp14_MODE_0;
			break;
		case 29: /* gp15:m0 */
			reg.upinMux_gp15 = Gbl_pinMux_gp15_MODE_0;
			break;
		case 30: /* gp16:m7 */
			reg.upinMux_gp16 = Gbl_pinMux_gp16_MODE_7;
			break;
		case 31: /* gp17:m7 */
#if (BERLIN_CHIP_VERSION >= BERLIN_B_0)
			reg1.upinMux_gp17 = Gbl_pinMux_gp17_MODE_7;
#else
			reg.upinMux_gp17 = Gbl_pinMux_gp17_MODE_7;
#endif
			break;
		default:
			return -1;
	}

	/* write Galois pin mux */
	GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux), (reg.u32));
#if (BERLIN_CHIP_VERSION >= BERLIN_B_0)
	GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux1), (reg1.u32));
#endif

	return 0;
}

#else /* BERLIN */

int GPIO_PinmuxInit(int port)
{
	T32Gbl_pinMux reg;
	T32Gbl_pinMux1 reg1;
	UNSG32 tmp_reg0, tmp_reg1, tmp_reg2, tmp_reg3;
	iomapper_reg_0_t iomapper_reg0;
	iomapper_reg_1_t iomapper_reg1;
	iomapper_reg_2_t iomapper_reg2;
	iomapper_reg_3_t iomapper_reg3;

	/* readout Galois pin mux */
	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux), &(reg.u32));
	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux1), &(reg1.u32));

	/* readout IOmapper pin mux */
	spi_master_rw_iomapper(IOMAPPER_SPI_MASTER, IOMAPPER_SPI_DEVID, 
			1, IOMAPPER_REG_MAP_0, &tmp_reg0);
	spi_master_rw_iomapper(IOMAPPER_SPI_MASTER, IOMAPPER_SPI_DEVID, 
			1, IOMAPPER_REG_MAP_1, &tmp_reg1);
	spi_master_rw_iomapper(IOMAPPER_SPI_MASTER, IOMAPPER_SPI_DEVID, 
			1, IOMAPPER_REG_MAP_2, &tmp_reg2);
	spi_master_rw_iomapper(IOMAPPER_SPI_MASTER, IOMAPPER_SPI_DEVID, 
			1, IOMAPPER_REG_MAP_3, &tmp_reg3);
	iomapper_reg0.u16 = tmp_reg0 & 0xffff;
	iomapper_reg1.u16 = tmp_reg1 & 0xffff;
	iomapper_reg2.u16 = tmp_reg2 & 0xffff;
	iomapper_reg3.u16 = tmp_reg3 & 0xffff;

	/* setting Galois&IOmapper pin mux */
	switch (port) {
		case 0:
		case 1:
			reg.upinMux_gp4 = Gbl_pinMux_gp4_MODE_0;
			iomapper_reg0.g4 = 0;
			break;
		case 2:
		case 3:
			reg.upinMux_gp9 = Gbl_pinMux_gp9_MODE_0;
			iomapper_reg0.g9 = 0;
			break;
		case 4:
		case 5:
			reg.upinMux_gp10 = Gbl_pinMux_gp10_MODE_0;
			iomapper_reg0.g10 = 0;
			break;
		case 6:
		case 7:
			reg.upinMux_gp11 = Gbl_pinMux_gp11_MODE_0;
			iomapper_reg2.g11 = 0;
			break;
		case 8:
		case 9:
			reg.upinMux_gp12 = Gbl_pinMux_gp12_MODE_0;
			iomapper_reg2.g12 = 0;
			break;
		case 10:
		case 11:
		case 12:
			reg.upinMux_gp13 = Gbl_pinMux_gp13_MODE_0;
			iomapper_reg2.g13 = 0;
			break;
		case 13:
			reg1.upinMux_gp37 = Gbl_pinMux_gp37_MODE_0; /* reg1 */
			iomapper_reg1.g37 = 0;
			break;
		case 14:
			reg.upinMux_gp14 = Gbl_pinMux_gp14_MODE_0;
			iomapper_reg2.g14 = 0;
			break;
		case 15:
			reg.upinMux_gp15 = Gbl_pinMux_gp15_MODE_0;
			iomapper_reg2.g15 = 0;
			break;
		case 16:
			reg.upinMux_gp16 = Gbl_pinMux_gp16_MODE_0;
			iomapper_reg2.g16 = 0;
			break;
		case 17:
			reg.upinMux_gp19 = Gbl_pinMux_gp19_MODE_0;
			iomapper_reg2.g19 = 0;
			break;
		case 18:
			reg.upinMux_gp20 = Gbl_pinMux_gp20_MODE_0;
			iomapper_reg2.g20 = 0;
			break;
		/* reg1 starts from gp21 */
		case 19:
			reg1.upinMux_gp23 = Gbl_pinMux_gp23_MODE_0;
			iomapper_reg0.g23 = 0;
			break;
		case 20:
			reg1.upinMux_gp24 = Gbl_pinMux_gp24_MODE_0;
			iomapper_reg3.g24 = 0;
			break;
		case 21:
			reg1.upinMux_gp25 = Gbl_pinMux_gp25_MODE_0;
			iomapper_reg3.g25 = 0;
			break;
		case 22:
		case 23:
			reg1.upinMux_gp27 = Gbl_pinMux_gp27_MODE_0;
			iomapper_reg3.g27 = 0;
			break;
		case 24:
		case 25:
			reg1.upinMux_gp28 = Gbl_pinMux_gp28_MODE_0;
			iomapper_reg3.g28 = 0;
			break;
		case 26:
			reg1.upinMux_gp31 = Gbl_pinMux_gp31_MODE_0;
			iomapper_reg1.g31 = 0;
			break;
		case 27:
			reg1.upinMux_gp32 = Gbl_pinMux_gp32_MODE_0;
			iomapper_reg1.g32 = 0;
			break;
		case 28:
			reg1.upinMux_gp33 = Gbl_pinMux_gp33_MODE_0;
			iomapper_reg1.g33 = 0;
			break;
		case 29:
			reg1.upinMux_gp34 = Gbl_pinMux_gp34_MODE_0;
			iomapper_reg1.g34 = 0;
			break;
		case 30:
			reg1.upinMux_gp35 = Gbl_pinMux_gp35_MODE_0;
			iomapper_reg1.g35 = 0;
			break;
		case 31:
			reg1.upinMux_gp36 = Gbl_pinMux_gp36_MODE_0;
			iomapper_reg1.g36 = 0;
			break;
		default:
			return -1;
	}

	/* write Galois pin mux */
	GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux), (reg.u32));
	GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux1), (reg1.u32));

	/* write IOmapper pin mux */
	tmp_reg0 = iomapper_reg0.u16;
	tmp_reg1 = iomapper_reg1.u16;
	tmp_reg2 = iomapper_reg2.u16;
	tmp_reg3 = iomapper_reg3.u16;
	spi_master_rw_iomapper(IOMAPPER_SPI_MASTER, IOMAPPER_SPI_DEVID, 
			0, IOMAPPER_REG_MAP_0, &tmp_reg0);
	spi_master_rw_iomapper(IOMAPPER_SPI_MASTER, IOMAPPER_SPI_DEVID, 
			0, IOMAPPER_REG_MAP_1, &tmp_reg1);
	spi_master_rw_iomapper(IOMAPPER_SPI_MASTER, IOMAPPER_SPI_DEVID, 
			0, IOMAPPER_REG_MAP_2, &tmp_reg2);
	spi_master_rw_iomapper(IOMAPPER_SPI_MASTER, IOMAPPER_SPI_DEVID, 
			0, IOMAPPER_REG_MAP_3, &tmp_reg3);

	return 0;
}

/****************************************************
 * FUNCTION: Configure IOmapper for GPIO port
 * PARAMS: port - GPIO port # (0 ~ 31)
 * 		   in - Set GPIO pin as IN or OUT
 * RETURN: 0 - succeed
 *        -1 - fail
 * NOTE: Be sure that spi_master_init_iomapper is done.
 ***************************************************/
int GPIO_IOmapperSetInOut(int port, int in)
{
    int iomapper_reg;
	UNSG32 value = 0;

	if ((port >= 0) && (port < 16)) {
		iomapper_reg = IOMAPPER_REG_MAP_4;
	} else if ((port >= 16) && (port < 32)) {
		iomapper_reg = IOMAPPER_REG_MAP_5;
		port -= 16;
	} else
        return -1;

	spi_master_rw_iomapper(IOMAPPER_SPI_MASTER, IOMAPPER_SPI_DEVID, 1, iomapper_reg, &value);
	if (in)
		value |= (1 << port);
	else
		value &= ~(1 << port);
	spi_master_rw_iomapper(IOMAPPER_SPI_MASTER, IOMAPPER_SPI_DEVID, 0, iomapper_reg, &value);

    return 0;
}

/****************************************************
 * FUNCTION: Get the in/out status of GPIO pin at IOmapper
 * PARAMS: port - GPIO port # (0 ~ 31)
 * 		   *inout - return PORT_DDR_IN or PORT_DDR_OUT
 * RETURN: 0 - succeed
 *        -1 - fail
 * NOTE: Be sure that spi_master_init_iomapper is done.
 ***************************************************/
int GPIO_IOmapperGetInOut(int port, int *inout)
{
    int iomapper_reg;

	if ((port >= 0) && (port < 16)) {
		iomapper_reg = IOMAPPER_REG_MAP_4;
	} else if ((port >= 16) && (port < 32)) {
		iomapper_reg = IOMAPPER_REG_MAP_5;
		port -= 16;
	} else
        return -1;

	spi_master_rw_iomapper(IOMAPPER_SPI_MASTER, IOMAPPER_SPI_DEVID, 1, iomapper_reg, inout);
	*inout = (*inout & (1 << port))? PORT_DDR_IN : PORT_DDR_OUT;

    return 0;
}
#endif /* BERLIN */

/****************************************************
 * FUNCTION: Set Galois GPIO pin as in or out
 * PARAMS: port - GPIO port # (0 ~ 31)
 * 		   in - 1: IN, 0: OUT
 * RETURN: 0 - succeed
 *        -1 - fail
 ***************************************************/
int GPIO_PortSetInOut(int port, int in)
{
	int reg_ddr, reg_ctl;
	int ddr, ctl;

	if((port >= 0) && (port < 8)){
		reg_ddr = APB_GPIO_INST0_BASE + APB_GPIO_SWPORTA_DDR;
		reg_ctl = APB_GPIO_INST0_BASE + APB_GPIO_PORTA_CTL;
	} else if ((port >= 8) && (port < 16)){
		reg_ddr = APB_GPIO_INST1_BASE + APB_GPIO_SWPORTA_DDR;
		reg_ctl = APB_GPIO_INST1_BASE + APB_GPIO_PORTA_CTL;
		port -= 8;
	} else if ((port >= 16) && (port < 24)){
		reg_ddr = APB_GPIO_INST2_BASE + APB_GPIO_SWPORTA_DDR;
		reg_ctl = APB_GPIO_INST2_BASE + APB_GPIO_PORTA_CTL;
		port -= 16;
	} else if ((port >= 24) && (port < 32)){
		reg_ddr = APB_GPIO_INST3_BASE + APB_GPIO_SWPORTA_DDR;
		reg_ctl = APB_GPIO_INST3_BASE + APB_GPIO_PORTA_CTL;
		port -= 24;
	} else
		return -1;

	/* software mode */
	GA_REG_WORD32_READ(reg_ctl, &ctl);
	ctl &= ~(1 << port);
	GA_REG_WORD32_WRITE(reg_ctl, ctl);

	/* set port to output mode */
	GA_REG_WORD32_READ(reg_ddr, &ddr);
	if (in)
		ddr &= ~(1 << port);
	else
		ddr |= (1 << port);
	GA_REG_WORD32_WRITE(reg_ddr, ddr);

	return 0;
}

/****************************************************
 * FUNCTION: Get direction of Galois GPIO pin: in or out
 * PARAMS: port - GPIO port # (0 ~ 31)
 * 		   *inout - PORT_DDR_IN: IN, PORT_DDR_OUT: OUT
 * RETURN: 0 - succeed
 *        -1 - fail
 ***************************************************/
int GPIO_PortGetInOut(int port, int *inout)
{
	int reg_ddr;

	if((port >= 0) && (port < 8)){
		reg_ddr = APB_GPIO_INST0_BASE + APB_GPIO_SWPORTA_DDR;
	} else if ((port >= 8) && (port < 16)){
		reg_ddr = APB_GPIO_INST1_BASE + APB_GPIO_SWPORTA_DDR;
		port -= 8;
	} else if ((port >= 16) && (port < 24)){
		reg_ddr = APB_GPIO_INST2_BASE + APB_GPIO_SWPORTA_DDR;
		port -= 16;
	} else if ((port >= 24) && (port < 32)){
		reg_ddr = APB_GPIO_INST3_BASE + APB_GPIO_SWPORTA_DDR;
		port -= 24;
	} else
		return -1;

	/* set port to output mode */
	GA_REG_WORD32_READ(reg_ddr, inout);
	*inout = (*inout & (1 << port))? PORT_DDR_OUT : PORT_DDR_IN;

	return 0;
}

/****************************************************
 * FUNCTION: Get data of Galois GPIO pin
 * PARAMS: port - GPIO port # (0 ~ 31)
 * 		   *data - the data in APB_GPIO_SWPORTA_DR
 * RETURN: 0 - succeed
 *        -1 - fail
 ***************************************************/
int GPIO_PortGetData(int port, int *data)
{
	int reg_dr;

	if((port >= 0) && (port < 8)){
		reg_dr = APB_GPIO_INST0_BASE + APB_GPIO_SWPORTA_DR;
	} else if ((port >= 8) && (port < 16)){
		reg_dr = APB_GPIO_INST1_BASE + APB_GPIO_SWPORTA_DR;
		port -= 8;
	} else if ((port >= 16) && (port < 24)){
		reg_dr = APB_GPIO_INST2_BASE + APB_GPIO_SWPORTA_DR;
		port -= 16;
	} else if ((port >= 24) && (port < 32)){
		reg_dr = APB_GPIO_INST3_BASE + APB_GPIO_SWPORTA_DR;
		port -= 24;
	} else
		return -1;

	/* set port to output mode */
	GA_REG_WORD32_READ(reg_dr, data);
	*data = (*data & (1 << port))? 1: 0;

	return 0;
}

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
int GPIO_PortInitIRQ(int port, int int_edge, int int_polarity)
{
	int reg_ddr, reg_debounce, reg_edge, reg_polarity;
	int reg_mask;//, reg_en, reg_eoi;
	int value;

	if((port >= 0) && (port < 8)){
		reg_ddr = APB_GPIO_INST0_BASE + APB_GPIO_SWPORTA_DDR;
		reg_mask = APB_GPIO_INST0_BASE + APB_GPIO_INTMASK;
		//reg_en = APB_GPIO_INST0_BASE + APB_GPIO_INTEN;
		//reg_eoi = APB_GPIO_INST0_BASE + APB_GPIO_PORTA_EOI;
		reg_debounce = APB_GPIO_INST0_BASE + APB_GPIO_DEBOUNCE;
		reg_edge = APB_GPIO_INST0_BASE + APB_GPIO_INTTYPE_LEVEL;
		reg_polarity = APB_GPIO_INST0_BASE + APB_GPIO_INT_POLARITY;
	} else if ((port >= 8) && (port < 16)){
		reg_ddr = APB_GPIO_INST1_BASE + APB_GPIO_SWPORTA_DDR;
		reg_mask = APB_GPIO_INST1_BASE + APB_GPIO_INTMASK;
		//reg_en = APB_GPIO_INST1_BASE + APB_GPIO_INTEN;
		//reg_eoi = APB_GPIO_INST1_BASE + APB_GPIO_PORTA_EOI;
		reg_debounce = APB_GPIO_INST1_BASE + APB_GPIO_DEBOUNCE;
		reg_edge = APB_GPIO_INST1_BASE + APB_GPIO_INTTYPE_LEVEL;
		reg_polarity = APB_GPIO_INST1_BASE + APB_GPIO_INT_POLARITY;
		port -= 8;
	} else if ((port >= 16) && (port < 24)){
		reg_ddr = APB_GPIO_INST2_BASE + APB_GPIO_SWPORTA_DDR;
		reg_mask = APB_GPIO_INST2_BASE + APB_GPIO_INTMASK;
		//reg_en = APB_GPIO_INST2_BASE + APB_GPIO_INTEN;
		//reg_eoi = APB_GPIO_INST2_BASE + APB_GPIO_PORTA_EOI;
		reg_debounce = APB_GPIO_INST2_BASE + APB_GPIO_DEBOUNCE;
		reg_edge = APB_GPIO_INST2_BASE + APB_GPIO_INTTYPE_LEVEL;
		reg_polarity = APB_GPIO_INST2_BASE + APB_GPIO_INT_POLARITY;
		port -= 16;
	} else if ((port >= 24) && (port < 32)){
		reg_ddr = APB_GPIO_INST3_BASE + APB_GPIO_SWPORTA_DDR;
		reg_mask = APB_GPIO_INST3_BASE + APB_GPIO_INTMASK;
		//reg_en = APB_GPIO_INST3_BASE + APB_GPIO_INTEN;
		//reg_eoi = APB_GPIO_INST3_BASE + APB_GPIO_PORTA_EOI;
		reg_debounce = APB_GPIO_INST3_BASE + APB_GPIO_DEBOUNCE;
		reg_edge = APB_GPIO_INST3_BASE + APB_GPIO_INTTYPE_LEVEL;
		reg_polarity = APB_GPIO_INST3_BASE + APB_GPIO_INT_POLARITY;
		port -= 24;
	} else
		return -1;

	/* Mask interrupt */
	GA_REG_WORD32_READ(reg_mask, &value);
	value |= (1 << port);
	GA_REG_WORD32_WRITE(reg_mask, value);

	/* Direction is input */
	GA_REG_WORD32_READ(reg_ddr, &value);
	value &= ~(1 << port);
	GA_REG_WORD32_WRITE(reg_ddr, value);

	/* Enable debounce */
	GA_REG_WORD32_READ(reg_debounce, &value);
	value |= (1 << port);
	GA_REG_WORD32_WRITE(reg_debounce, value);
	
	/* int_edge=1: Edge triggered interrupt */
	GA_REG_WORD32_READ(reg_edge, &value);
	value |= (int_edge << port);
	GA_REG_WORD32_WRITE(reg_edge, value);

	/* int_polarity=1: rise-edge triggered interrupt */
	GA_REG_WORD32_READ(reg_polarity, &value);
	value |= (int_polarity << port);
	GA_REG_WORD32_WRITE(reg_polarity, value);

	/* Enable interrupt */
	//GA_REG_WORD32_READ(reg_en, &value);
	//value |= (1 << port);
	//GA_REG_WORD32_WRITE(reg_en, value);

	/* Write-only, write 1 to clear interrupt */
	//value = (1 << port);
	//GA_REG_WORD32_WRITE(reg_eoi, value);

	/* Unmask interrupt */
	//GA_REG_WORD32_READ(reg_mask, &value);
	//value &= ~(1 << port);
	//GA_REG_WORD32_WRITE(reg_mask, value);

	return 0;
}

/****************************************************
 * FUNCTION: Enable interrupt for Galois GPIO pin
 * PARAMS: port - GPIO port # (0 ~ 31)
 * RETURN: 0 - succeed
 *        -1 - fail
 * NOTE: You also need to enable GPIO interrupt in ICTL.
 ***************************************************/
int GPIO_PortEnableIRQ(int port)
{
	int reg_mask, reg_en, reg_eoi;
	int value;

	if((port >= 0) && (port < 8)){
		reg_mask = APB_GPIO_INST0_BASE + APB_GPIO_INTMASK;
		reg_en = APB_GPIO_INST0_BASE + APB_GPIO_INTEN;
		reg_eoi = APB_GPIO_INST0_BASE + APB_GPIO_PORTA_EOI;
	} else if ((port >= 8) && (port < 16)){
		reg_mask = APB_GPIO_INST1_BASE + APB_GPIO_INTMASK;
		reg_en = APB_GPIO_INST1_BASE + APB_GPIO_INTEN;
		reg_eoi = APB_GPIO_INST1_BASE + APB_GPIO_PORTA_EOI;
		port -= 8;
	} else if ((port >= 16) && (port < 24)){
		reg_mask = APB_GPIO_INST2_BASE + APB_GPIO_INTMASK;
		reg_en = APB_GPIO_INST2_BASE + APB_GPIO_INTEN;
		reg_eoi = APB_GPIO_INST2_BASE + APB_GPIO_PORTA_EOI;
		port -= 16;
	} else if ((port >= 24) && (port < 32)){
		reg_mask = APB_GPIO_INST3_BASE + APB_GPIO_INTMASK;
		reg_en = APB_GPIO_INST3_BASE + APB_GPIO_INTEN;
		reg_eoi = APB_GPIO_INST3_BASE + APB_GPIO_PORTA_EOI;
		port -= 24;
	} else
		return -1;

	/* Enable interrupt */
	GA_REG_WORD32_READ(reg_en, &value);
	value |= (1 << port);
	GA_REG_WORD32_WRITE(reg_en, value);

	/* Write-only, write 1 to clear interrupt */
	value = (1 << port);
	GA_REG_WORD32_WRITE(reg_eoi, value);

	/* Unmask interrupt */
	GA_REG_WORD32_READ(reg_mask, &value);
	value &= ~(1 << port);
	GA_REG_WORD32_WRITE(reg_mask, value);

	return 0;
}

/****************************************************
 * FUNCTION: Disable interrupt for Galois GPIO pin
 * PARAMS: port - GPIO port # (0 ~ 31)
 * RETURN: 0 - succeed
 *        -1 - fail
 ***************************************************/
int GPIO_PortDisableIRQ(int port)
{
	int reg_mask, reg_en;
	int value;

	if((port >= 0) && (port < 8)){
		reg_mask = APB_GPIO_INST0_BASE + APB_GPIO_INTMASK;
		reg_en = APB_GPIO_INST0_BASE + APB_GPIO_INTEN;
	} else if ((port >= 8) && (port < 16)){
		reg_mask = APB_GPIO_INST1_BASE + APB_GPIO_INTMASK;
		reg_en = APB_GPIO_INST1_BASE + APB_GPIO_INTEN;
		port -= 8;
	} else if ((port >= 16) && (port < 24)){
		reg_mask = APB_GPIO_INST2_BASE + APB_GPIO_INTMASK;
		reg_en = APB_GPIO_INST2_BASE + APB_GPIO_INTEN;
		port -= 16;
	} else if ((port >= 24) && (port < 32)){
		reg_mask = APB_GPIO_INST3_BASE + APB_GPIO_INTMASK;
		reg_en = APB_GPIO_INST3_BASE + APB_GPIO_INTEN;
		port -= 24;
	} else
		return -1;

	/* Mask interrupt */
	GA_REG_WORD32_READ(reg_mask, &value);
	value |= (1 << port);
	GA_REG_WORD32_WRITE(reg_mask, value);

	/* Disable interrupt */
	GA_REG_WORD32_READ(reg_en, &value);
	value &= ~(1 << port);
	GA_REG_WORD32_WRITE(reg_en, value);

	return 0;
}

/****************************************************
 * FUNCTION: Lookup if there's interrupt for Galois GPIO pin
 * PARAMS: port - GPIO port # (0 ~ 31)
 * RETURN: 1 - yes, there's interrupt pending.
 * 		   0 - no, there's no interrupt pending.
 *        -1 - fail.
 ***************************************************/
int GPIO_PortHasInterrupt(int port)
{
	int reg_status;
	int value;

	if((port >= 0) && (port < 8)){
		reg_status = APB_GPIO_INST0_BASE + APB_GPIO_INTSTATUS;
	} else if ((port >= 8) && (port < 16)){
		reg_status = APB_GPIO_INST1_BASE + APB_GPIO_INTSTATUS;
		port -= 8;
	} else if ((port >= 16) && (port < 24)){
		reg_status = APB_GPIO_INST2_BASE + APB_GPIO_INTSTATUS;
		port -= 16;
	} else if ((port >= 24) && (port < 32)){
		reg_status = APB_GPIO_INST3_BASE + APB_GPIO_INTSTATUS;
		port -= 24;
	} else
		return -1;

	GA_REG_WORD32_READ(reg_status, &value);
	if (value & (0x1 << port))
		return 1;
	else 
		return 0;
}

/****************************************************
 * FUNCTION: Clear interrupt for Galois GPIO pin
 * PARAMS: port - GPIO port # (0 ~ 31)
 * RETURN: 0 - succeed.
 *        -1 - fail.
 ***************************************************/
int GPIO_PortClearInterrupt(int port)
{
	int reg_eoi;
	int value;

	if((port >= 0) && (port < 8)){
		reg_eoi = APB_GPIO_INST0_BASE + APB_GPIO_PORTA_EOI;
	} else if ((port >= 8) && (port < 16)){
		reg_eoi = APB_GPIO_INST1_BASE + APB_GPIO_PORTA_EOI;
		port -= 8;
	} else if ((port >= 16) && (port < 24)){
		reg_eoi = APB_GPIO_INST2_BASE + APB_GPIO_PORTA_EOI;
		port -= 16;
	} else if ((port >= 24) && (port < 32)){
		reg_eoi = APB_GPIO_INST3_BASE + APB_GPIO_PORTA_EOI;
		port -= 24;
	} else
		return -1;

	/* see above, write 1 to clear interrupt */
	value = (1 << port);
	GA_REG_WORD32_WRITE(reg_eoi, value);
	return 0;
}

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
int SM_GPIO_PortWrite(int port, int value)
{
    int reg_ddr, reg_dr, reg_ctl;
    int ddr, dr, ctl;

    if((port >= 0) && (port < 8)){
        reg_ddr = SM_APB_GPIO_BASE + APB_GPIO_SWPORTA_DDR;
        reg_dr = SM_APB_GPIO_BASE + APB_GPIO_SWPORTA_DR;
		reg_ctl = SM_APB_GPIO_BASE + APB_GPIO_PORTA_CTL;
    } else if ((port >= 8) && (port < 16)){
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)
        reg_ddr = SM_APB_GPIO1_BASE + APB_GPIO_SWPORTA_DDR;
        reg_dr = SM_APB_GPIO1_BASE + APB_GPIO_SWPORTA_DR;
		reg_ctl = SM_APB_GPIO1_BASE + APB_GPIO_PORTA_CTL;
#else
        reg_ddr = SM_APB_GPIO_BASE + APB_GPIO_SWPORTB_DDR;
        reg_dr = SM_APB_GPIO_BASE + APB_GPIO_SWPORTB_DR;
		reg_ctl = SM_APB_GPIO_BASE + APB_GPIO_PORTB_CTL;
#endif
        port -= 8;
    } else
        return -1;

	/* software mode */
	GA_REG_WORD32_READ(reg_ctl, &ctl);
	ctl &= ~(1 << port);
	GA_REG_WORD32_WRITE(reg_ctl, ctl);

    /* set port to output mode */
    GA_REG_WORD32_READ(reg_ddr, &ddr);
    ddr |= (1<<port);
    GA_REG_WORD32_WRITE(reg_ddr, ddr);

    /* set port value */
    GA_REG_WORD32_READ(reg_dr, &dr);
    if (value){
        dr |= (1<<port);
    } else {
        dr &= ~(1<<port);
    }
    GA_REG_WORD32_WRITE(reg_dr, dr);

    return 0;
}

/****************************************************
 * FUNCTION: read SM_GPIO port status
 * PARAMS: port - SM_GPIO port # (0 ~ 11)
 *         *value - pointer to port status
 * RETURN: 0 - succeed
 *        -1 - fail
 ***************************************************/
int SM_GPIO_PortRead(int port, int *value)
{
    int reg_ddr, reg_ext, reg_ctl;
    int ddr, ext, ctl;

    if((port >= 0) && (port < 8)){
        reg_ddr = SM_APB_GPIO_BASE + APB_GPIO_SWPORTA_DDR;
        reg_ext = SM_APB_GPIO_BASE + APB_GPIO_EXT_PORTA;
		reg_ctl = SM_APB_GPIO_BASE + APB_GPIO_PORTA_CTL;
    } else if ((port >= 8) && (port < 12)){
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)
        reg_ddr = SM_APB_GPIO1_BASE + APB_GPIO_SWPORTA_DDR;
        reg_ext = SM_APB_GPIO1_BASE + APB_GPIO_EXT_PORTA;
		reg_ctl = SM_APB_GPIO1_BASE + APB_GPIO_PORTA_CTL;
#else
        reg_ddr = SM_APB_GPIO_BASE + APB_GPIO_SWPORTB_DDR;
        reg_ext = SM_APB_GPIO_BASE + APB_GPIO_EXT_PORTB;
		reg_ctl = SM_APB_GPIO_BASE + APB_GPIO_PORTB_CTL;
#endif
        port -= 8;
    } else
        return -1;

	/* software mode */
	GA_REG_WORD32_READ(reg_ctl, &ctl);
	ctl &= ~(1 << port);
	GA_REG_WORD32_WRITE(reg_ctl, ctl);

    /* set port to input mode */
    GA_REG_WORD32_READ(reg_ddr, &ddr);
    ddr &= ~(1<<port);
    GA_REG_WORD32_WRITE(reg_ddr, ddr);

    /* get port value */
    GA_REG_WORD32_READ(reg_ext, &ext);
    if (ext & (1<<port))
        *value = 1;
    else
        *value = 0;

    return 0;
}



/****************************************************
 * FUNCTION: Set Galois SM_GPIO pin as in or out
 * PARAMS: port - SM_GPIO port # (0 ~ 11)
 * 		   in - 1: IN, 0: OUT
 * RETURN: 0 - succeed
 *        -1 - fail
 ***************************************************/
int SM_GPIO_PortSetInOut(int port, int in)
{
	int reg_ddr, reg_ctl;
	int ddr, ctl;

	if((port >= 0) && (port < 8)){
		reg_ddr = SM_APB_GPIO_BASE + APB_GPIO_SWPORTA_DDR;
		reg_ctl = SM_APB_GPIO_BASE + APB_GPIO_PORTA_CTL;
	} else if ((port >= 8) && (port < 12)){
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)
		reg_ddr = SM_APB_GPIO1_BASE + APB_GPIO_SWPORTA_DDR;
		reg_ctl = SM_APB_GPIO1_BASE + APB_GPIO_PORTA_CTL;
#else
		reg_ddr = SM_APB_GPIO_BASE + APB_GPIO_SWPORTB_DDR;
		reg_ctl = SM_APB_GPIO_BASE + APB_GPIO_PORTB_CTL;
#endif
		port -= 8;
	} else
		return -1;

	/* software mode */
	GA_REG_WORD32_READ(reg_ctl, &ctl);
	ctl &= ~(1 << port);
	GA_REG_WORD32_WRITE(reg_ctl, ctl);

	/* set port to output mode */
	GA_REG_WORD32_READ(reg_ddr, &ddr);
	if (in)
		ddr &= ~(1 << port);
	else
		ddr |= (1 << port);
	GA_REG_WORD32_WRITE(reg_ddr, ddr);

	return 0;
}

/****************************************************
 * FUNCTION: Get direction of Galois SM_GPIO pin: in or out
 * PARAMS: port - SM_GPIO port # (0 ~ 11)
 * 		   *inout - PORT_DDR_IN: IN, PORT_DDR_OUT: OUT
 * RETURN: 0 - succeed
 *        -1 - fail
 ***************************************************/
int SM_GPIO_PortGetInOut(int port, int *inout)
{
	int reg_ddr;

	if((port >= 0) && (port < 8)){
		reg_ddr = SM_APB_GPIO_BASE + APB_GPIO_SWPORTA_DDR;
	} else if ((port >= 8) && (port < 12)){
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)
		reg_ddr = SM_APB_GPIO1_BASE + APB_GPIO_SWPORTA_DDR;
#else
		reg_ddr = SM_APB_GPIO_BASE + APB_GPIO_SWPORTB_DDR;
#endif
		port -= 8;
	} else
		return -1;

	/* set port to output mode */
	GA_REG_WORD32_READ(reg_ddr, inout);
	*inout = (*inout & (1 << port))? PORT_DDR_OUT : PORT_DDR_IN;

	return 0;
}

/****************************************************
 * FUNCTION: Get data of Galois SM_GPIO pin
 * PARAMS: port - SM_GPIO port # (0 ~ 11)
 * 		   *data - the data in APB_GPIO_SWPORTA_DR
 * RETURN: 0 - succeed
 *        -1 - fail
 ***************************************************/
int SM_GPIO_PortGetData(int port, int *data)
{
	int reg_dr;

	if((port >= 0) && (port < 8)){
		reg_dr = SM_APB_GPIO_BASE + APB_GPIO_SWPORTA_DR;
	} else if ((port >= 8) && (port < 12)){
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)
		reg_dr = SM_APB_GPIO1_BASE + APB_GPIO_SWPORTA_DR;
#else
		reg_dr = SM_APB_GPIO_BASE + APB_GPIO_SWPORTB_DR;
#endif
		port -= 8;
	} else
		return -1;

	/* set port to output mode */
	GA_REG_WORD32_READ(reg_dr, data);
	*data = (*data & (1 << port))? 1: 0;

	return 0;
}

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
int SM_GPIO_PortInitIRQ(int port, int int_edge, int int_polarity)
{
	int reg_ddr, reg_debounce, reg_edge, reg_polarity;
	int reg_mask;//, reg_en, reg_eoi;
	int value;

	if((port >= 0) && (port < 8)){
		reg_ddr = SM_APB_GPIO_BASE + APB_GPIO_SWPORTA_DDR;
		reg_mask = SM_APB_GPIO_BASE + APB_GPIO_INTMASK;
		//reg_en = SM_APB_GPIO_BASE + APB_GPIO_INTEN;
		//reg_eoi = SM_APB_GPIO_BASE + APB_GPIO_PORTA_EOI;
		reg_debounce = SM_APB_GPIO_BASE + APB_GPIO_DEBOUNCE;
		reg_edge = SM_APB_GPIO_BASE + APB_GPIO_INTTYPE_LEVEL;
		reg_polarity = SM_APB_GPIO_BASE + APB_GPIO_INT_POLARITY;
	} else if ((port >= 8) && (port < 12)){
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)
		reg_ddr = SM_APB_GPIO1_BASE + APB_GPIO_SWPORTA_DDR;
		reg_mask = SM_APB_GPIO1_BASE + APB_GPIO_INTMASK;
		//reg_en = SM_APB_GPIO1_BASE + APB_GPIO_INTEN;
		//reg_eoi = SM_APB_GPIO1_BASE + APB_GPIO_PORTA_EOI;
		reg_debounce = SM_APB_GPIO1_BASE + APB_GPIO_DEBOUNCE;
		reg_edge = SM_APB_GPIO1_BASE + APB_GPIO_INTTYPE_LEVEL;
		reg_polarity = SM_APB_GPIO1_BASE + APB_GPIO_INT_POLARITY;
		port -= 8;
#else
		return -1;
#endif
	} else {
		return -1;
	}

	/* Mask interrupt */
	GA_REG_WORD32_READ(reg_mask, &value);
	value |= (1 << port);
	GA_REG_WORD32_WRITE(reg_mask, value);

	/* Direction is input */
	GA_REG_WORD32_READ(reg_ddr, &value);
	value &= ~(1 << port);
	GA_REG_WORD32_WRITE(reg_ddr, value);

	/* Enable debounce */
	GA_REG_WORD32_READ(reg_debounce, &value);
	value |= (1 << port);
	GA_REG_WORD32_WRITE(reg_debounce, value);
	
	/* int_edge=1: Edge triggered interrupt */
	GA_REG_WORD32_READ(reg_edge, &value);
	value |= (int_edge << port);
	GA_REG_WORD32_WRITE(reg_edge, value);

	/* int_polarity=1: rise-edge triggered interrupt */
	GA_REG_WORD32_READ(reg_polarity, &value);
	value |= (int_polarity << port);
	GA_REG_WORD32_WRITE(reg_polarity, value);

	/* Enable interrupt */
	//GA_REG_WORD32_READ(reg_en, &value);
	//value |= (1 << port);
	//GA_REG_WORD32_WRITE(reg_en, value);

	/* Write-only, write 1 to clear interrupt */
	//value = (1 << port);
	//GA_REG_WORD32_WRITE(reg_eoi, value);

	/* Unmask interrupt */
	//GA_REG_WORD32_READ(reg_mask, &value);
	//value &= ~(1 << port);
	//GA_REG_WORD32_WRITE(reg_mask, value);

	return 0;
}

/****************************************************
 * FUNCTION: Enable interrupt for Galois SM_GPIO pin
 * PARAMS: port - SM_GPIO port # (0 ~ 7)
 * RETURN: 0 - succeed
 *        -1 - fail
 * NOTE: You also need to enable SM_GPIO interrupt in ICTL.
 ***************************************************/
int SM_GPIO_PortEnableIRQ(int port)
{
	int reg_mask, reg_en, reg_eoi;
	int value;

	if((port >= 0) && (port < 8)){
		reg_mask = SM_APB_GPIO_BASE + APB_GPIO_INTMASK;
		reg_en = SM_APB_GPIO_BASE + APB_GPIO_INTEN;
		reg_eoi = SM_APB_GPIO_BASE + APB_GPIO_PORTA_EOI;
	} else if ((port >= 8) && (port < 12)){
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)
		reg_mask = SM_APB_GPIO1_BASE + APB_GPIO_INTMASK;
		reg_en = SM_APB_GPIO1_BASE + APB_GPIO_INTEN;
		reg_eoi = SM_APB_GPIO1_BASE + APB_GPIO_PORTA_EOI;
		port -= 8;
#else
		return -1;
#endif
	} else {
		return -1;
	}

	/* Enable interrupt */
	GA_REG_WORD32_READ(reg_en, &value);
	value |= (1 << port);
	GA_REG_WORD32_WRITE(reg_en, value);

	/* Write-only, write 1 to clear interrupt */
	value = (1 << port);
	GA_REG_WORD32_WRITE(reg_eoi, value);

	/* Unmask interrupt */
	GA_REG_WORD32_READ(reg_mask, &value);
	value &= ~(1 << port);
	GA_REG_WORD32_WRITE(reg_mask, value);

	return 0;
}

/****************************************************
 * FUNCTION: Disable interrupt for Galois SM_GPIO pin
 * PARAMS: port - SM_GPIO port # (0 ~ 7)
 * RETURN: 0 - succeed
 *        -1 - fail
 ***************************************************/
int SM_GPIO_PortDisableIRQ(int port)
{
	int reg_mask, reg_en;
	int value;

	if((port >= 0) && (port < 8)){
		reg_mask = SM_APB_GPIO_BASE + APB_GPIO_INTMASK;
		reg_en = SM_APB_GPIO_BASE + APB_GPIO_INTEN;
	} else if ((port >= 8) && (port < 12)){
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)
		reg_mask = SM_APB_GPIO1_BASE + APB_GPIO_INTMASK;
		reg_en = SM_APB_GPIO1_BASE + APB_GPIO_INTEN;
		port -= 8;
#else
		return -1;
#endif
	} else {
		return -1;
	}

	/* Mask interrupt */
	GA_REG_WORD32_READ(reg_mask, &value);
	value |= (1 << port);
	GA_REG_WORD32_WRITE(reg_mask, value);

	/* Disable interrupt */
	GA_REG_WORD32_READ(reg_en, &value);
	value &= ~(1 << port);
	GA_REG_WORD32_WRITE(reg_en, value);

	return 0;
}

/****************************************************
 * FUNCTION: Lookup if there's interrupt for Galois SM_GPIO pin
 * PARAMS: port - SM_GPIO port # (0 ~ 7)
 * RETURN: 1 - yes, there's interrupt pending.
 * 		   0 - no, there's no interrupt pending.
 *        -1 - fail.
 ***************************************************/
int SM_GPIO_PortHasInterrupt(int port)
{
	int reg_status;
	int value;

	if((port >= 0) && (port < 8)){
		reg_status = SM_APB_GPIO_BASE + APB_GPIO_INTSTATUS;
	} else if ((port >= 8) && (port < 12)){
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)
		reg_status = SM_APB_GPIO1_BASE + APB_GPIO_INTSTATUS;
		port -= 8;
#else
		return -1;
#endif
	} else {
		return -1;
	}

	GA_REG_WORD32_READ(reg_status, &value);
	if (value & (0x1 << port))
		return 1;
	else 
		return 0;
}

/****************************************************
 * FUNCTION: Clear interrupt for Galois SM_GPIO pin
 * PARAMS: port - SM_GPIO port # (0 ~ 7)
 * RETURN: 0 - succeed.
 *        -1 - fail.
 ***************************************************/
int SM_GPIO_PortClearInterrupt(int port)
{
	int reg_eoi;
	int value;

	if((port >= 0) && (port < 8)){
		reg_eoi = SM_APB_GPIO_BASE + APB_GPIO_PORTA_EOI;
	} else if ((port >= 8) && (port < 12)){
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)
		reg_eoi = SM_APB_GPIO1_BASE + APB_GPIO_PORTA_EOI;
		port -= 8;
#else
		return -1;
#endif
	} else {
		return -1;
	}

	/* see above, write 1 to clear interrupt */
	value = (1 << port);
	GA_REG_WORD32_WRITE(reg_eoi, value);
	return 0;
}
#endif /* BERLIN */

/*
 * the low-level GPIO api may be used by linux kernel driver.
 * so it's exported.
 */
#ifdef __LINUX_KERNEL__
EXPORT_SYMBOL(GPIO_PortWrite);
EXPORT_SYMBOL(GPIO_PortRead);
EXPORT_SYMBOL(GPIO_PinmuxInit);
EXPORT_SYMBOL(GPIO_PortSetInOut);
EXPORT_SYMBOL(GPIO_PortGetInOut);
EXPORT_SYMBOL(GPIO_PortGetData);
EXPORT_SYMBOL(GPIO_PortInitIRQ);
EXPORT_SYMBOL(GPIO_PortEnableIRQ);
EXPORT_SYMBOL(GPIO_PortDisableIRQ);
EXPORT_SYMBOL(GPIO_PortHasInterrupt);
EXPORT_SYMBOL(GPIO_PortClearInterrupt);
EXPORT_SYMBOL(SM_GPIO_PortWrite);
EXPORT_SYMBOL(SM_GPIO_PortRead);
EXPORT_SYMBOL(SM_GPIO_PortSetInOut);
EXPORT_SYMBOL(SM_GPIO_PortGetInOut);
EXPORT_SYMBOL(SM_GPIO_PortGetData);
EXPORT_SYMBOL(SM_GPIO_PortInitIRQ);
EXPORT_SYMBOL(SM_GPIO_PortEnableIRQ);
EXPORT_SYMBOL(SM_GPIO_PortDisableIRQ);
EXPORT_SYMBOL(SM_GPIO_PortHasInterrupt);
EXPORT_SYMBOL(SM_GPIO_PortClearInterrupt);
#endif /* __LINUX_KERNEL__ */

