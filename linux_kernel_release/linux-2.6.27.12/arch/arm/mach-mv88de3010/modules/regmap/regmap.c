#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>

#include <linux/kernel.h>   /* printk() */
#include <linux/mm.h>
#include <linux/pfn.h>
#include <linux/fs.h>       /* everything... */
#include <linux/errno.h>    /* error codes */
#include <linux/types.h>    /* size_t */

#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>
#include <mach/galois_generic.h>
#include "Galois_memmap.h"

#define GALOIS_EXORT_REG_BASE	0x00000000

static int regmap_open (struct inode *inode, struct file *filp)
{
	printk("Galois memory map device opened, it's very dangerous!\n");
	return 0;
}

static int regmap_memmap(struct file *filp, struct vm_area_struct *vma)
{
	unsigned long pfn = (GALOIS_EXORT_REG_BASE >> PAGE_SHIFT) + vma->vm_pgoff;
	unsigned long size = vma->vm_end - vma->vm_start;

	if ((GALOIS_EXORT_REG_BASE + (vma->vm_pgoff<<PAGE_SHIFT) + size) < GALOIS_EXORT_REG_BASE) {
		printk("offset+size over flow.\n");
		return -EOVERFLOW;
	}

	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
	if (remap_pfn_range(vma, vma->vm_start, pfn, size, vma->vm_page_prot))
		return -EAGAIN;
	return 0;
}

static struct file_operations regmap_ops = {
	.owner	= THIS_MODULE,
	.open	= regmap_open,
	.mmap	= regmap_memmap,
};

static struct miscdevice regmap_dev = {
	.minor	= GALOIS_REGMAP_MISC_MINOR,
	.name	= GALOIS_REGMAP_NAME,
	.fops	= &regmap_ops,
};

static int regmap_init(void)
{
	printk("u're preparing to export space 0x%08x to user, it's very dangerous!\n", 
			GALOIS_EXORT_REG_BASE);
	if (misc_register(&regmap_dev)) {
		return -ENODEV;
	}
	return 0;
}

static void __exit regmap_exit(void)
{
	misc_deregister(&regmap_dev);
}

MODULE_AUTHOR("Yufeng Zheng");
MODULE_LICENSE("GPL");

module_init(regmap_init);
module_exit(regmap_exit);
