#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>

#include <linux/kernel.h>   /* printk() */
#include <linux/slab.h>   /* kmalloc() */
#include <linux/fs.h>       /* everything... */
#include <linux/errno.h>    /* error codes */
#include <linux/types.h>    /* size_t */
#include <linux/ioport.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/irqreturn.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>
#include <mach/galois_generic.h>
#include <mach/spi_bus.h>

#include "spi_master.h"

#define SPI_BUS 0
#define IOMAPPER_ID 0

static int galois_iomapper_open(struct inode *inode, struct file *file)
{
	spi_master_init_iomapper(SPI_BUS);
	return 0;
}

static int galois_iomapper_release(struct inode *inode, struct file *file)
{
	return 0;
}

static ssize_t galois_iomapper_read(struct file *file, char __user * buf,
		size_t count, loff_t *ppos)
{
	printk("error: please use ioctl.\n");
	return -EFAULT;
}

static ssize_t galois_iomapper_write(struct file *file, const char __user * buf,
		size_t count, loff_t *ppos)
{
	printk("error: please use ioctl.\n");
	return -EFAULT;
}

static int galois_iomapper_ioctl(struct inode *inode, struct file *file, 
		unsigned int cmd, unsigned long arg)
{
	galois_spi_cmd_t user_spi_cmd;
	unsigned int kbuf[1];

	switch(cmd) {
		case SPI_IOCTL_READ:
			if (copy_from_user(&user_spi_cmd, (void __user *)arg, sizeof(galois_spi_cmd_t)))
				return -EFAULT;
			if (user_spi_cmd.count != 1) {
				printk("error rd count.\n");
				return -EPERM;
			}

			spi_master_rw_iomapper(SPI_BUS, IOMAPPER_ID, 1, user_spi_cmd.sub_cmd, kbuf);

			if (copy_to_user((void __user *)user_spi_cmd.buf, (void *)kbuf, 4)) {
				return -EFAULT;
			}
			break;
		case SPI_IOCTL_WRITE:
			if (copy_from_user(&user_spi_cmd, (void __user *)arg, sizeof(galois_spi_cmd_t)))
				return -EFAULT;
			if (user_spi_cmd.count != 1) {
				printk("error rd count.\n");
				return -EPERM;
			}
			if (copy_from_user((void *)kbuf, (void __user *)user_spi_cmd.buf, 4))
				return -EFAULT;

			spi_master_rw_iomapper(SPI_BUS, IOMAPPER_ID, 0, user_spi_cmd.sub_cmd, kbuf);
			break;
		default:
			return -EPERM;
	}
	return 0;
}

static struct file_operations galois_iomapper_fops = {
	.owner		= THIS_MODULE,
	.open		= galois_iomapper_open,
	.release	= galois_iomapper_release,
	.write		= galois_iomapper_write,
	.read		= galois_iomapper_read,
	.ioctl		= galois_iomapper_ioctl,
};

static struct miscdevice iomapper_dev = {
	.minor	= GALOIS_IOMAPPER_MISC_MINOR,
	.name	= GALOIS_IOMAPPER_NAME,
	.fops	= &galois_iomapper_fops,
};

static int __init galois_iomapper_init(void)
{
	if (misc_register(&iomapper_dev))
		return -ENODEV;
	else
		return 0;
}

static void __exit galois_iomapper_exit(void)
{
	misc_deregister(&iomapper_dev);
	return;
}

module_init(galois_iomapper_init);
module_exit(galois_iomapper_exit);

MODULE_AUTHOR("Marvell-Galois");
MODULE_DESCRIPTION("Galois SPI IO-mapper Driver");
MODULE_LICENSE("GPL");
