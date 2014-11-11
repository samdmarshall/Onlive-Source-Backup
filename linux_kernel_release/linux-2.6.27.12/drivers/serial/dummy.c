/*
 *  linux/drivers/serial/dummy.c
 *
 *  Driver for an empty console
 *
 *  Based on drivers/serial/8250.c, by Russell King
 *
 *  Copyright (c) 2010 OnLive, Inc. Remi Machet
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/ioport.h>
#include <linux/init.h>
#include <linux/console.h>
#include <linux/sysrq.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/serial_reg.h>
#include <linux/serial_core.h>
#include <linux/serial.h>
#include <linux/serial_8250.h>
#include <linux/nmi.h>
#include <linux/mutex.h>

/* Implement tty part */

static int con_open(struct tty_struct *tty, struct file *filp)
{
	return 0;
}

static void con_close(struct tty_struct *tty, struct file *filp)
{
}

static int con_write(struct tty_struct *tty, const unsigned char *buf, int count)
{
	return count;
}

static int con_write_room(struct tty_struct *tty)
{
	if (tty->stopped)
		return 0;
	return 4096;		/* No limit, really; we're not buffering */
}

static int con_put_char(struct tty_struct *tty, unsigned char ch)
{
	return 1;
}

static int con_ioctl(struct tty_struct *tty, struct file *filp, unsigned int cmd,
	   unsigned long arg)
{
	return -ENOIOCTLCMD;
}

static void con_throttle(struct tty_struct *tty)
{
}

static void con_unthrottle(struct tty_struct *tty)
{
}

static void con_stop(struct tty_struct *tty)
{
}

static void con_start(struct tty_struct *tty)
{
}

static void con_flush_chars(struct tty_struct *tty)
{
	return;
}

static int con_chars_in_buffer(struct tty_struct *tty)
{
	return 0;
}

struct tty_driver *console_driver;

static const struct tty_operations con_ops = {
	.open = con_open,
	.close = con_close,
	.write = con_write,
	.write_room = con_write_room,
	.put_char = con_put_char,
	.flush_chars = con_flush_chars,
	.chars_in_buffer = con_chars_in_buffer,
	.ioctl = con_ioctl,
	.stop = con_stop,
	.start = con_start,
	.throttle = con_throttle,
	.unthrottle = con_unthrottle,
};

/* Note: tty are usually initialized from tty_init but here we only want 
 * it for the console, so no need to hook it up there 
 */
int __init dummytty_init(void)
{
	console_driver = alloc_tty_driver(1);
	if (!console_driver)
		panic("Couldn't allocate console driver\n");
	console_driver->owner = THIS_MODULE;
	console_driver->name = "tty";
	console_driver->name_base = 1;
	console_driver->major = TTY_MAJOR;
	console_driver->minor_start = 1;
	console_driver->type = TTY_DRIVER_TYPE_CONSOLE;
	console_driver->init_termios = tty_std_termios;
	console_driver->flags = TTY_DRIVER_REAL_RAW | TTY_DRIVER_RESET_TERMIOS;
	tty_set_operations(console_driver, &con_ops);
	if (tty_register_driver(console_driver))
		panic("Couldn't register console driver\n");
	return 0;
}

module_init(dummytty_init);

/* Implement console part */

static void serialdummy_console_write(struct console *co, const char *s, unsigned int count)
{	
	return;
}

static struct tty_driver *serialdummy_console_device(struct console *c, int *index)
{
	*index = c->index ? c->index-1 : 0;
	return console_driver;
}

static struct console serialdummy_console = {
	.name		= "dummy",
	.write		= serialdummy_console_write,
	.device		= serialdummy_console_device,
	.flags		= 0,
	.index		= -1,
};

static int __init serialdummy_console_init(void)
{
	register_console(&serialdummy_console);
	return 0;
}
console_initcall(serialdummy_console_init);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Dummy serial driver");
MODULE_ALIAS_CHARDEV_MAJOR(TTY_MAJOR);
