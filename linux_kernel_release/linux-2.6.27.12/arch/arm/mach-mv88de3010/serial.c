#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/serial.h>
#include <linux/serial_reg.h>
#include <linux/serial_core.h>
#include <linux/serial_8250.h>
#include <linux/console.h>

#include "apb_perf_base.h"
#include "apb_uart.h"
#include "apb_uart_drv.h"
#include "pinmux.h"
#include "SysMgr.h"
#include <mach/irqs.h>
#include <mach/uncompress.h>
#include <mach/galois_generic.h>
#include <mach/galois_platform.h>

struct uart_port mv88de3010_uart_port;
static void __init serial_initialize(void)
{
	/*
	 * setup pinmux of UART0/1/2
	 */	
	mv_uart_setup_pinmux();

#ifdef CONFIG_MV88DE3010_UART0
#ifndef CONFIG_MV88DE3010_BERLIN_UARTBUG
	mv88de3010_open_irq(IRQ_APB_UARTINST0);
#endif
#endif
	
#ifdef CONFIG_MV88DE3010_UART1
#ifndef CONFIG_MV88DE3010_BERLIN_UARTBUG
	mv88de3010_open_irq(IRQ_APB_UARTINST1);
#endif
#endif

#ifdef CONFIG_MV88DE3010_UART2
#ifndef CONFIG_MV88DE3010_BERLIN_UARTBUG
	mv88de3010_open_irq(IRQ_APB_UARTINST2);
#endif
#endif

	/* following commented codes are moved to platform_device.c, Yufeng -2010/08/10*/
#if 0
	/* Prapare uart_port for console */
	memset(&mv88de3010_uart_port, 0, sizeof(mv88de3010_uart_port));
	mv88de3010_uart_port.membase = (unsigned char *)(APB_UART_INST0_BASE);
	mv88de3010_uart_port.irq = irq; /* 0 to not use int */
	mv88de3010_uart_port.uartclk = GALOIS_UART_TCLK;
	mv88de3010_uart_port.regshift = 2;
	mv88de3010_uart_port.iotype = UPIO_DWAPB;
	mv88de3010_uart_port.flags = ASYNC_BOOT_AUTOCONF | ASYNC_SKIP_TEST;
	mv88de3010_uart_port.line = 0;
	mv88de3010_uart_port.private_data = (void *)(APB_UART_INST0_BASE + APB_UART_USR); /* read USR to clear busy int */

	/*
	 * register to serial8250_ports
	 */
	if (early_serial_setup(&mv88de3010_uart_port) != 0)
		printk("%s: Early serial init of UART port #0 failed.\n", __FUNCTION__);
#endif
	return;
}

static void mv88de3010_uart_console_write(struct console *co, 
		const char *s, u_int count)
{
	int i;
#ifdef CONFIG_ARCH_MV88DE3010_debug /* print [~] */
	APB_UART_putc(GALOIS_UART_CHANNEL, '[');
	APB_UART_putc(GALOIS_UART_CHANNEL, '~');
	APB_UART_putc(GALOIS_UART_CHANNEL, ']');
#endif 
	for (i = 0; i < count; i++, s++) {
		APB_UART_putc(GALOIS_UART_CHANNEL, *s);
		if (*s == '\n')
			APB_UART_putc(GALOIS_UART_CHANNEL, '\r');
	}
}

static int __init mv88de3010_uart_console_setup(struct console *co, char *options)
{
	APB_UART_init(GALOIS_UART_CHANNEL, GALOIS_UART_TCLK, GALOIS_UART_BAUDRATE);

	return 0;
}

/*
 * This function is a copy of uart_console_device()
 */
struct tty_driver *mv88de3010_uart_console_device(struct console *co, int *index)
{
	struct uart_driver *p = co->data;
	*index = co->index;
	return p->tty_driver;
}

static struct console mv88de3010_uart_console;
static struct uart_driver mv88de3010_uart_driver = {
	.owner			= THIS_MODULE,
	.driver_name	= "ttyG",
	.dev_name		= "ttyG",
	.major			= MV88DE3010_SERIAL_MAJOR,
	.minor			= MV88DE3010_SERIAL_MINOR,
	.nr				= 1,
	.cons			= &mv88de3010_uart_console,
};

static struct console mv88de3010_uart_console = {
	.name	= "ttyG",
	.write	= mv88de3010_uart_console_write,
	/* .read	= mv88de3010_uart_console_read, */
	.device	= mv88de3010_uart_console_device,	/* return struct uart_driver */
	.setup	= mv88de3010_uart_console_setup,
	.flags	= CON_PRINTBUFFER,
	.index	= -1,
	.data	= &mv88de3010_uart_driver,
};

int __init mv88de3010_uart_console_init(void)
{
	serial_initialize();
	register_console(&mv88de3010_uart_console);
	return 0;
}
console_initcall(mv88de3010_uart_console_init);

/*
 * fix Berlin UART baudrate to some value
 */
void mv88de3010_fix_uartbaud(unsigned int baud)
{
	unsigned int quot, cval;
	unsigned int base = APB_UART_INST0_BASE;

	GA_REG_WORD32_READ(base + APB_UART_LCR, &cval);
	GA_REG_WORD32_WRITE(base + APB_UART_LCR, cval | UART_LCR_DLAB);

	quot = (GALOIS_UART_TCLK + (8 * baud)) / (16 * baud);
	GA_REG_WORD32_WRITE(base + APB_UART_DLL, quot & 0xff);
	GA_REG_WORD32_WRITE(base + APB_UART_DLH, (quot >> 8) & 0xff);

	GA_REG_WORD32_WRITE(base + APB_UART_LCR, cval);
	return;
}
EXPORT_SYMBOL(mv88de3010_fix_uartbaud);

#if 0
static int __init mv88de3010_uart_init(void)
{
	int ret, i;

	ret = uart_register_driver(&mv88de3010_uart_driver);
	if (ret != 0)
		return ret;

	/* bug: mv88de3010_uart_port isn't fully initialized. */
	ret = uart_add_one_port(&mv88de3010_uart_driver, &mv88de3010_uart_port);

	return ret;
}

module_init(mv88de3010_uart_init);

MODULE_DESCRIPTION("MV88DE3010 simple serial driver");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yufeng Zheng");
#endif
