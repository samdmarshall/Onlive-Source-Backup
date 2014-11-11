#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>

#include <linux/kernel.h>   /* printk() */
#include <linux/slab.h>   /* kmalloc() */
#include <linux/fs.h>       /* everything... */
#include <linux/errno.h>    /* error codes */
#include <linux/types.h>    /* size_t */

#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>
#include <mach/galois_generic.h>
#include <mach/galois_platform.h>

#include "GaloisTime.h"
#include "galois_hal_com.h"
#include "apb_timer_drv.h"

static int pftimer_open (struct inode *inode, struct file *filp)
{
	printk("Performance timer open!\n");
	return 0;
}

static int pftimer_release(struct inode *inode, struct file *file)
{
	printk("Performance timer close!\n");
	return 0;
}

#if 0
unsigned int TIMERx_GetCurrentValue(unsigned int timer_N)
{
    UNSG32 base;
    UNSG32 offset;
    UNSG32 val;
                                                                                                                             
    base = APB_TIMER_INST_BASE;
    offset = APB_TIMER_1_CURRENTVALUE + timer_N*APD_TIMER_REG_RANGE;
    GA_REG_WORD32_READ(base+offset, &val);
    return val;
}
#endif

static int pftimer_ioctl(struct inode *inode, struct file *filp, 
		unsigned int cmd, unsigned long arg)
{
	unsigned int value;

	switch (cmd) {
		case CMD_PFTIMER_GET_VALUE:
			value = TIMERx_GetCurrentValue(GALOIS_DEBUG_TIMER);
			/* printk("[Galois]pftimer value: 0x%08x\n", value); */
			if (copy_to_user((void __user *)arg, &value, sizeof(value)))
				return -EFAULT;
			break;
		default:
			return -EINVAL;
	}

	return 0;
}

static int pftimer_memmap(struct file *filp, struct vm_area_struct *vma)
{
	return 0;
}

static struct file_operations pftimer_ops = {
	.owner	= THIS_MODULE,
	.open	= pftimer_open,
	.release= pftimer_release,
	.mmap	= pftimer_memmap,
	.ioctl	= pftimer_ioctl,
};

static struct miscdevice pftimer_dev = {
	.minor	= GALOIS_PFTIMER_MISC_MINOR,
	.name	= GALOIS_PFTIMER_NAME,
	.fops	= &pftimer_ops,
};

static int pftimer_init(void)
{
	if (misc_register(&pftimer_dev)) {
		return -ENODEV;
	}
#if 0
	printk("pftimer init.\n");
	mv88de3010_init_pftimer(GALOIS_DEBUG_TIMER);
#endif

	return 0;
}

static void __exit pftimer_exit(void)
{
#if 0
	printk("pftimer exit.\n");
	mv88de3010_exit_pftimer(GALOIS_DEBUG_TIMER);
#endif
	misc_deregister(&pftimer_dev);
}

MODULE_AUTHOR("Yufeng Zheng");
MODULE_LICENSE("GPL");

module_init(pftimer_init);
module_exit(pftimer_exit);
