#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>
#include <linux/kernel.h>   /* printk() */
#include <linux/slab.h>   /* kmalloc() */
#include <linux/fs.h>       /* everything... */
#include <linux/errno.h>    /* error codes */
#include <linux/types.h>    /* size_t */

#include <asm/io.h>
#include "Galois_memmap.h"
#include "galois_io.h"
#include "global.h"
#include "SysMgr.h"
#include "apb_perf_base.h"
#include "pinmux.h"
#include "gpio.h"

#ifdef CONFIG_MV88DE3010_LINUX_SETPINMUX
/*
 * mv_general_setup_pinmux: setup general pinmux for MV88DE3010
 */
void mv_general_setup_pinmux(void)
{
	Customization_Simple_PinMuxInitSetting();
}

/*
 * mv_gpio_setup_pinmux: setup GPIO pin's pinmux to GPIO mode
 * @port: specific GPIO pin num
 */
int mv_gpio_setup_pinmux(int port)
{
	return GPIO_PinmuxInit(port);
}

/*
 * mv_uart_setup_pinmux: setup UART interface pinmux
 */
void mv_uart_setup_pinmux(void)
{
	T32smSysCtl_SM_GSM_SEL reg;

#ifdef CONFIG_MV88DE3010_UART0
	/*
	 * set UART0 RXD/TXD signals(SM GPIO 8,9) pinmux
	 */
	GA_REG_WORD32_READ(SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_GSM_SEL, &(reg.u32));
	reg.uSM_GSM_SEL_GSM2_SEL = 1; 
	GA_REG_WORD32_WRITE(SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_GSM_SEL, (reg.u32));
#endif
	
#ifdef CONFIG_MV88DE3010_UART1
	/*
	 * set UART1 RXD/TXD signals(SM GPIO 10,11) pinmux
	 */
	//GA_REG_WORD32_READ(SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_GSM_SEL, &(reg.u32));
	//reg.uSM_GSM_SEL_GSM5_SEL = 1; 
	//GA_REG_WORD32_WRITE(SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_GSM_SEL, (reg.u32));
#endif

#ifdef CONFIG_MV88DE3010_UART2
	/*
	 * set UART2 RXD/TXD signals(SM GPIO 6,7) pinmux, but they conflict with 
	 * UART0 CTSn/RTSn signals.
	 */
	//GA_REG_WORD32_READ(SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_GSM_SEL, &(reg.u32));
	//reg.uSM_GSM_SEL_GSM3_SEL = 2; 
	//reg.uSM_GSM_SEL_GSM4_SEL = 2; 
	//GA_REG_WORD32_WRITE(SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_GSM_SEL, (reg.u32));
#endif
}

/*
 * mv_pcie_setup_pinmux: setup PCIe interface pinmux
 */
void mv_pcie_setup_pinmux(void)
{
#define MV_GPIO_PCIE_RESET		12
#define MV_GPIO_PCIE_POWER		14
	GPIO_PinmuxInit(MV_GPIO_PCIE_RESET);
	GPIO_PinmuxInit(MV_GPIO_PCIE_POWER);
}

/*
 * mv_usb_setup_pinmux: setup USB interface pinmux
 */
void mv_usb_setup_pinmux(void)
{
#define MV_GPIO_POWER0 			(12)
#define MV_GPIO_POWER1 			(14)
	GPIO_PinmuxInit(MV_GPIO_POWER0);
#ifndef CONFIG_MV88DE3010_PM_DISABLE_USB1
	GPIO_PinmuxInit(MV_GPIO_POWER1);
#endif
}

/*
 * mv_sdio_setup_pinmux: setup SDIO interface pinmux
 */
void mv_sdio_setup_pinmux(void)
{
	T32Gbl_pinMux reg;
	T32smSysCtl_SM_GSM_SEL smreg;

	reg.u32 = readl(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux);
	/* SD_CLK, SD_DAT1, SD_DAT2. SD_CDn, and SD_WP */
	reg.upinMux_gp9 = 2;
	/* SD_DAT0 and SD_CMD */
	reg.upinMux_gp4 = 2;
	/* SD_DAT3 */
	reg.upinMux_gp1=2;
	writel(reg.u32,MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux);

	smreg.u32 = readl(SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_GSM_SEL);
	smreg.uSM_GSM_SEL_GSM7_SEL = 0;   //SM_GPIO[12] for PEK power enable
	writel((smreg.u32),SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_GSM_SEL);
}

/*
 * set_sw_twsi_pinmux: setup TWSI bus pinmux to S/W mode
 * @bus: TWSI bus num
 */
int set_sw_twsi_pinmux(int bus)
{
	T32Gbl_pinMux reg;
	T32smSysCtl_SM_GSM_SEL smreg;

	/* pinmux setting */
	if (bus == 0) {
		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux),&(reg.u32)); // read current pinmux
		// gp10, GPIO 25/24(TW0)
		reg.upinMux_gp10 = Gbl_pinMux_gp10_MODE_2;
		GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux),(reg.u32));
	} else if (bus == 1) {
		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux),&(reg.u32)); // read current pinmux
		// gp11, GPIO 27/26(TW1)
		reg.upinMux_gp11 = Gbl_pinMux_gp11_MODE_2;
		GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux),(reg.u32));
	} else if (bus == 2) {
		GA_REG_WORD32_READ((SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_GSM_SEL), &(smreg.u32));
		smreg.uSM_GSM_SEL_GSM0_SEL = 0; //SM GPIO 4
		smreg.uSM_GSM_SEL_GSM1_SEL = 0; //SM GPIO 5
		GA_REG_WORD32_WRITE((SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_GSM_SEL), (smreg.u32));
	} else if (bus == 3) {
		// SM GPIO 11/10(TW3)
		GA_REG_WORD32_READ((SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_GSM_SEL), &(smreg.u32));
		smreg.uSM_GSM_SEL_GSM5_SEL = 0;
		GA_REG_WORD32_WRITE((SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_GSM_SEL), (smreg.u32));
	} else {
		printk("set_sw_twsi_pinmux: wrong bus %d\n", bus);
		return -ENODEV;
	}
	return 0;
}

/*
 * set_hw_twsi_pinmux: setup TWSI bus pinmux to H/W mode
 * @bus: TWSI bus num
 */
int set_hw_twsi_pinmux(int bus)
{
	T32Gbl_pinMux reg;
	T32smSysCtl_SM_GSM_SEL smreg;

	/* pinmux setting */
	if (bus == 0) {
		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux),&(reg.u32)); // read current pinmux
		// gp10, GPIO 25/24(TW0)
		reg.upinMux_gp10 = Gbl_pinMux_gp10_MODE_1;
		GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux),(reg.u32));
	} else if (bus == 1) {
		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux),&(reg.u32)); // read current pinmux
		// gp11, GPIO 27/26(TW1)
		reg.upinMux_gp11 = Gbl_pinMux_gp11_MODE_1;
		GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux),(reg.u32));
	} else if (bus == 2) {
		GA_REG_WORD32_READ((SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_GSM_SEL), &(smreg.u32));
		smreg.uSM_GSM_SEL_GSM0_SEL = 2; //SM GPIO 4
		smreg.uSM_GSM_SEL_GSM1_SEL = 2; //SM GPIO 5
		GA_REG_WORD32_WRITE((SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_GSM_SEL), (smreg.u32));
	} else if (bus == 3) {
		// SM GPIO 11/10(TW3)
		GA_REG_WORD32_READ((SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_GSM_SEL), &(smreg.u32));
		smreg.uSM_GSM_SEL_GSM5_SEL = 3;
		GA_REG_WORD32_WRITE((SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_GSM_SEL), (smreg.u32));
	} else {
		printk("set_hw_twsi_pinmux: wrong bus %d\n", bus);
		return -ENODEV;
	}
	return 0;
}

/*
 * set_spi_pinmux: SPI bus pinmux setting
 * @bus: SPI bus number
 */
int set_spi_pinmux(int bus)
{
	T32Gbl_pinMux reg;
	T32smSysCtl_SM_GSM_SEL smreg;

	/* pinmux setting */
	if (bus == 1) {
		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux),&(reg.u32)); // read current pinmux
		// gp10, GPIO 25/24(TW0)
		//reg.upinMux_gp0 = Gbl_pinMux_gp0_MODE_0; //SS0n
		//reg.upinMux_gp1 = Gbl_pinMux_gp1_MODE_0; //SS1n
		//reg.upinMux_gp2 = Gbl_pinMux_gp2_MODE_1; //SS2n
		//reg.upinMux_gp3 = Gbl_pinMux_gp3_MODE_1; //SS3n
		reg.upinMux_gp4 = Gbl_pinMux_gp4_MODE_0; //SCLK, SDO, SDI
		GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux),(reg.u32));
	} else if (bus == 2) {
#ifndef CONFIG_MV88DE3010_BERLIN
		printk("set_spi_pinmux: wrong bus %d\n", bus);
		return -ENODEV;
#else
		/* SDI is fixed, no need pinmux */
		GA_REG_WORD32_READ((SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_GSM_SEL), &(smreg.u32));
		//smreg.uSM_GSM_SEL_GSM0_SEL = 1; //SM GPIO 4, SS2n
		//smreg.uSM_GSM_SEL_GSM1_SEL = 1; //SM GPIO 5, SS3n
		smreg.uSM_GSM_SEL_GSM6_SEL = 1; //SM SCLK, SDO
		//smreg.uSM_GSM_SEL_GSM7_SEL = 1; //SM SS0n
		//smreg.uSM_GSM_SEL_GSM8_SEL = 1; //SM SS1n
		GA_REG_WORD32_WRITE((SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_GSM_SEL), (smreg.u32));
#endif
	} else {
		printk("set_spi_pinmux: wrong bus %d\n", bus);
		return -ENODEV;
	}
	return 0;
}

/*
 * set_spi_bus_slave: SPI bus pinmux setting for SSn0~SSn3
 * @bus: SPI bus number
 * @slave: SSn number
 */
int set_spi_slave_pinmux(int bus, int slave)
{
	T32Gbl_pinMux reg;
	T32smSysCtl_SM_GSM_SEL smreg;

	/* pinmux setting */
	if (bus == 1) {
		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux),&(reg.u32)); // read current pinmux
		//reg.upinMux_gp4 = Gbl_pinMux_gp4_MODE_0; //SCLK, SDO, SDI
		if (slave == 0)
			reg.upinMux_gp0 = Gbl_pinMux_gp0_MODE_0; //SS0n
		else if (slave == 1)
			reg.upinMux_gp1 = Gbl_pinMux_gp1_MODE_0; //SS1n
		else if (slave == 2)
			reg.upinMux_gp2 = Gbl_pinMux_gp2_MODE_1; //SS2n
		else if (slave == 3)
			reg.upinMux_gp3 = Gbl_pinMux_gp3_MODE_1; //SS3n
		else
			printk("set_spi_slave_pinmux: wrong slave %d on bus %d\n", slave, bus);
		GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux),(reg.u32));
	} else if (bus == 2) {
#ifndef CONFIG_MV88DE3010_BERLIN
		printk("set_spi_slave_pinmux: wrong bus %d\n", bus);
		return -ENODEV;
#else
		/* SDI is fixed, no need pinmux */
		GA_REG_WORD32_READ((SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_GSM_SEL), &(smreg.u32));
		//smreg.uSM_GSM_SEL_GSM6_SEL = 1; //SM SCLK, SDO
		if (slave == 0)
			smreg.uSM_GSM_SEL_GSM7_SEL = 1; //SM SS0n
		else if (slave == 1)
			smreg.uSM_GSM_SEL_GSM8_SEL = 1; //SM SS1n
		else if (slave == 2)
			smreg.uSM_GSM_SEL_GSM0_SEL = 1; //SM GPIO 4, SS2n
		else if (slave == 3)
			smreg.uSM_GSM_SEL_GSM1_SEL = 1; //SM GPIO 5, SS3n
		else
			printk("set_spi_slave_pinmux: wrong slave %d on bus %d\n", slave, bus);
		GA_REG_WORD32_WRITE((SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_GSM_SEL), (smreg.u32));
#endif
	} else {
		printk("set_spi_slave_pinmux: wrong bus %d\n", bus);
		return -ENODEV;
	}
	return 0;

}

/*
 * PWM pinmux setting: data structures
 */
#define GALOIS_PWMCH0		0
#define GALOIS_PWMCH1		1
#define GALOIS_PWMCH2		2
#define GALOIS_PWMCH3		3
#define GALOIS_PWMCH_NUM	4
typedef union _pinmux_reg_t {
	T32Gbl_pinMux reg;
	T32Gbl_pinMux1 reg1;
}pinmux_reg_t;
static pinmux_reg_t pinmux_to_set, pinmux_to_restore;

/*
 * set_pwm_pinmux: setup PWM pinmux
 * @index: PWM channel
 */
int set_pwm_pinmux(int index)
{
	/* pinmux setting */
	switch(index){
		case GALOIS_PWMCH0:	
			GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux1),&(pinmux_to_set.reg1.u32)); // read current pinmux
			pinmux_to_restore.reg1.u32 = pinmux_to_set.reg1.u32;
			pinmux_to_set.reg1.upinMux_gp21 = Gbl_pinMux_gp21_MODE_3;
			GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux1),(pinmux_to_set.reg1.u32));
			break;
		case GALOIS_PWMCH1:
			GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux1),&(pinmux_to_set.reg1.u32)); // read current pinmux
			pinmux_to_restore.reg1.u32 = pinmux_to_set.reg1.u32;
			pinmux_to_set.reg1.upinMux_gp23 = Gbl_pinMux_gp23_MODE_3;
			GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux1),(pinmux_to_set.reg1.u32));
			break;
		case GALOIS_PWMCH2:
			GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux),&(pinmux_to_set.reg.u32)); // read current pinmux
			pinmux_to_restore.reg.u32 = pinmux_to_set.reg.u32;
			pinmux_to_set.reg.upinMux_gp2 = Gbl_pinMux_gp2_MODE_2;
			pinmux_to_set.reg.upinMux_gp13 = Gbl_pinMux_gp13_MODE_2;
			GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux),(pinmux_to_set.reg.u32));
			break;
		case GALOIS_PWMCH3:
			GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux),&(pinmux_to_set.reg.u32)); // read current pinmux
			pinmux_to_restore.reg.u32 = pinmux_to_set.reg.u32;
			pinmux_to_set.reg.upinMux_gp8 = Gbl_pinMux_gp8_MODE_2;
			GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux),(pinmux_to_set.reg.u32));
			break;
		default:
			printk("set_pwm_pinmux: wrong index %d\n", index);
			return -ENODEV;
	}
	
	return 0;
}

/*
 * restore_pwm_pinmux: restore PWM pinmux, obsoleted, don't use it.
 * @index: PWM channel
 */
int restore_pwm_pinmux(int index)
{
	/* pinmux restore */
	switch(index){
		case GALOIS_PWMCH0:	
			GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux1),(pinmux_to_restore.reg1.u32));
			break;
		case GALOIS_PWMCH1:
			GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux1),(pinmux_to_restore.reg1.u32));
			break;
		case GALOIS_PWMCH2:
			GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux),(pinmux_to_restore.reg.u32));
			break;
		case GALOIS_PWMCH3:
			GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux),(pinmux_to_restore.reg.u32));
			break;
		default:
			printk("restore_pwm_pinmux: wrong index %d\n", index);
			return -ENODEV;
	}
	
	return 0;
}

#else  /* #ifdef CONFIG_MV88DE3010_LINUX_SETPINMUX */

void mv_general_setup_pinmux(void) 
{
}
int mv_gpio_setup_pinmux(int port)
{
	return 0;
}
void mv_uart_setup_pinmux(void)
{
}
void mv_pcie_setup_pinmux(void)
{
}
void mv_usb_setup_pinmux(void)
{
}
void mv_sdio_setup_pinmux(void)
{
}
int set_sw_twsi_pinmux(int bus)
{
	return 0;
}
int set_hw_twsi_pinmux(int bus)
{
	return 0;
}
int set_spi_pinmux(int bus)
{
	return 0;
}
int set_spi_slave_pinmux(int bus, int slave)
{
	return 0;
}
int set_pwm_pinmux(int index)
{
	return 0;
}
int restore_pwm_pinmux(int index)
{
	return 0;
}

#endif /* #ifdef CONFIG_MV88DE3010_LINUX_SETPINMUX */

EXPORT_SYMBOL(mv_general_setup_pinmux);
EXPORT_SYMBOL(mv_gpio_setup_pinmux);
EXPORT_SYMBOL(mv_uart_setup_pinmux);
EXPORT_SYMBOL(mv_pcie_setup_pinmux);
EXPORT_SYMBOL(mv_usb_setup_pinmux);
EXPORT_SYMBOL(mv_sdio_setup_pinmux);
EXPORT_SYMBOL(set_sw_twsi_pinmux);
EXPORT_SYMBOL(set_hw_twsi_pinmux);
EXPORT_SYMBOL(set_spi_pinmux);
EXPORT_SYMBOL(set_spi_slave_pinmux);
EXPORT_SYMBOL(set_pwm_pinmux);
EXPORT_SYMBOL(restore_pwm_pinmux);

