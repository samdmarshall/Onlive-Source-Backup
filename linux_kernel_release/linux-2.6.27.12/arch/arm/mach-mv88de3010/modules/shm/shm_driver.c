////////////////////////////////////////////////////////////////////////////////
//! \file shm_driver.c
//! \brief Shared Memory driver
//!	
//! Purpose:
//!
//!
//!	Version, Date and Author :
//!		V 1.00,	         ,	Fang Bao
//!     V 1.01,	9/02/2010,	Fang Bao,   Fix bug: InvalidateCache(), CleanCache(), CleanAndInvalidateCache() should accept virtual address of cache memory, not physical address of cache memory
//!	                                    Fix bug: MV_SHM_Malloc missed Clean Cache for Non-Cache and Cahce usage of Shared memory
//!     V 1.02,	9/08/2010,	Fang Bao,   Fix bug: shm_driver_init missed error handler of MV_SHM_Init failure
//!                                     Fix bug: MV_SHM_Init should clean and invalidate cache for new memory region, clean is not enough
//!     V 1.03,	9/26/2010,	Fang Bao,   Fix bug: the cache operation parameter in user space should be virtual address of user space, it can't be replaced by virtual address of kernel even though they are mapped to same physical address.
//!                                     move flush cache operation of MV_SHM_Malloc() from kernel driver to user space because of bug.
//!	
//!		
//! Note:
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
    System head files
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>
#include <net/sock.h>
#include <linux/proc_fs.h>

#include <linux/kernel.h>   /* printk() */
#include <linux/slab.h>     /* kmalloc() */
#include <linux/fs.h>       /* everything... */
#include <linux/errno.h>    /* error codes */
#include <linux/types.h>    /* size_t */

#include <linux/mm.h>
#include <linux/kdev_t.h>
#include <asm/page.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/version.h>

/*******************************************************************************
    Local head files
*/

#include <mach/galois_generic.h>
#include "shm_type.h"
#include "shm_device.h"
#include "shm_api_kernel.h"

/*******************************************************************************
    Module Descrption
*/

MODULE_AUTHOR("Fang Bao");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Shared Memory Driver");

/*******************************************************************************
    Module API defined
*/

static int shm_driver_open (struct inode *inode, struct file *filp);
static int shm_driver_release(struct inode *inode, struct file *filp);
static int shm_driver_mmap_nocache(struct file *filp, struct vm_area_struct *vma);
static int shm_driver_mmap_cache(struct file *filp, struct vm_area_struct *vma);
static int shm_driver_ioctl(struct inode *inode, struct file *filp,
                 unsigned int cmd, unsigned long arg);
                 

/*******************************************************************************
    Module Parameter
*/

static uint shm_size = CONFIG_MV88DE3010_SHMMEM_SIZE;
module_param(shm_size, uint, 0644);

static uint shm_base = CONFIG_MV88DE3010_SHMMEM_START;
module_param(shm_base, uint, 0644);

/*******************************************************************************
    Module Variable
*/

static shm_device_t *shm_device = NULL;

static struct cdev shm_dev_nocache;
static struct cdev shm_dev_cache;
static struct class *shm_dev_class;

static struct file_operations shm_ops_nocache = {
	.open    = shm_driver_open,
	.release = shm_driver_release,
	.mmap    = shm_driver_mmap_nocache,
	.ioctl   = shm_driver_ioctl,
	.owner   = THIS_MODULE,
};

static struct file_operations shm_ops_cache = {
	.open    = shm_driver_open,
	.release = shm_driver_release,
	.mmap    = shm_driver_mmap_cache,
	.ioctl   = shm_driver_ioctl,
	.owner   = THIS_MODULE,
};

static shm_driver_cdev_t shm_driver_dev_list[] = {
	{GALOIS_SHM0_MINOR, SHM_DEVICE_NAME_NONCACHE, &shm_dev_nocache, &shm_ops_nocache},
	{GALOIS_SHM1_MINOR, SHM_DEVICE_NAME_CACHE, &shm_dev_cache, &shm_ops_cache},
};

static struct proc_dir_entry *shm_driver_procdir;

/*******************************************************************************
    Module API
*/

static int shm_driver_open (struct inode *inode, struct file *filp)
{   
	filp->private_data = shm_device;

    shm_debug("shm_driver_open ok\n");
	
	return 0;
}

static int shm_driver_release(struct inode *inode, struct file *filp)
{   
    filp->private_data = NULL;

    shm_debug("shm_driver_release ok\n");

	return 0;
}

static int shm_driver_mmap_nocache(struct file *filp, struct vm_area_struct *vma)
{
    unsigned long pfn;
    unsigned long vsize;
    
    if (filp->private_data == NULL)
        return -ENOTTY;
        
	pfn = ((shm_device_t *)(filp->private_data))->m_base_nocache >> PAGE_SHIFT;
	vsize = vma->vm_end - vma->vm_start;
	
	shm_debug("shm_driver_mmap_nocache size = 0x%08lX\n", vsize);

	if (vsize > shm_size)
		return -EINVAL;
		
	vma->vm_pgoff = 0;                                          // skip offset
	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);    // set memory non-cache

	if (remap_pfn_range(vma, vma->vm_start, pfn, vsize, vma->vm_page_prot))
		return -EAGAIN;

	return 0;
}

static int shm_driver_mmap_cache(struct file *filp, struct vm_area_struct *vma)
{
    unsigned long pfn;
    unsigned long vsize;
    
    if (filp->private_data == NULL)
        return -ENOTTY;
        
	pfn = ((shm_device_t *)(filp->private_data))->m_base_cache >> PAGE_SHIFT;
	vsize = vma->vm_end - vma->vm_start;

	shm_debug("shm_driver_mmap_cache size = 0x%08lX\n", vsize);

	if (vsize > shm_size)
		return -EINVAL;
		
	vma->vm_pgoff = 0;                                          // skip offset

	if (remap_pfn_range(vma, vma->vm_start, pfn, vsize, vma->vm_page_prot))
		return -EAGAIN;

	return 0;
}

static int shm_driver_ioctl(struct inode *inode, struct file *filp,
                 unsigned int cmd, unsigned long arg)
{
	shm_driver_operation_t op;
	int res = 0;

	shm_debug("shm_driver_ioctl cmd = 0x%08x\n", cmd);

    switch(cmd) {

      case SHM_DEVICE_CMD_GET_MEMINFO: {
        
        MV_SHM_MemInfo_t meminfo;
        res = shm_device_get_meminfo(shm_device, &meminfo);
        if (res == 0)
            res = copy_to_user((void __user *)arg, &meminfo, sizeof(meminfo));
       
        break;
        }

      case SHM_DEVICE_CMD_GET_BASEINFO: {

        MV_SHM_BaseInfo_t baseinfo;
        res = shm_device_get_baseinfo(shm_device, &baseinfo);
        if (res == 0)
            res = copy_to_user((void __user *)arg, &baseinfo, sizeof(baseinfo));
       
        break;
        }

      case SHM_DEVICE_CMD_ALLOCATE:
                
        res = copy_from_user(&op, (int __user *)arg, sizeof(op));
        if (res != 0)
            break;       
        op.m_param1 = shm_device_allocate(shm_device, op.m_param1, op.m_param2);
	res = copy_to_user((void __user *)arg, &op, sizeof(op));
      
        break;

      case SHM_DEVICE_CMD_FREE:
                
        res = copy_from_user(&op, (int __user *)arg, sizeof(op));
        if (res != 0)
            break;       
        
        res = shm_device_free(shm_device, op.m_param1);

        break;
        
	  case SHM_DEVICE_CMD_INVALIDATE:
	  	
	  	res = copy_from_user(&op, (int __user *)arg, sizeof(op));
		if ( res == 0)
			 res = shm_device_cache_invalidate((void *)op.m_param1, op.m_param2);

		break;

	  case SHM_DEVICE_CMD_CLEAN:
	  	        
	  	res = copy_from_user(&op, (int __user *)arg, sizeof(op));
		if ( res == 0)
			res = shm_device_cache_clean((void *)op.m_param1, op.m_param2);

		break;

	  case SHM_DEVICE_CMD_CLEANANDINVALIDATE:
	  	
	  	res = copy_from_user(&op, (int __user *)arg, sizeof(op));
		if ( res == 0)
			 res = shm_device_cache_clean_and_invalidate((void *)op.m_param1, op.m_param2);

		break;

	  default:
	    
	    res = -ENOTTY;
	}

	shm_debug("shm_driver_ioctl res = %d\n", res);

	return res;
}

static int read_proc_meminfo(char *page, char **start, off_t offset,
        int count, int *eof, void *data)
{
    int len = 0, res;
    
    MV_SHM_MemInfo_t meminfo;
    res = shm_device_get_meminfo(shm_device, &meminfo);
    if (res != 0)
    {
        shm_error("shm_driver_read_proc_meminfo failed. (%d)\n", res);
        len += sprintf(page, "shm_driver_read_proc_meminfo failed. (%d)\n", res);
        goto done;
    }
    
    //!!!!! avoid buffer overflow !!!
    len += sprintf(page + len, "%20s : %10u %s\n", "total mem", meminfo.m_totalmem, "Bytes");
    len += sprintf(page + len, "%20s : %10u %s\n", "free mem", meminfo.m_freemem, "Bytes");
    len += sprintf(page + len, "%20s : %10u %s\n", "used mem", meminfo.m_usedmem, "Bytes");
    len += sprintf(page + len, "%20s : %10u %s\n", "peak used mem", meminfo.m_peak_usedmem, "Bytes");
    
//    len += sprintf(page + len, "%20s : %10u %s\n", "max free block", meminfo.m_max_freeblock, "Bytes");
//    len += sprintf(page + len, "%20s : %10u %s\n", "min used block", meminfo.m_min_freeblock, "Bytes");
//    len += sprintf(page + len, "%20s : %10u %s\n", "max used block", meminfo.m_max_usedblock, "Bytes");
//    len += sprintf(page + len, "%20s : %10u %s\n", "min used block", meminfo.m_min_usedblock, "Bytes");
    len += sprintf(page + len, "%20s : %10u %s\n", "num free block", meminfo.m_num_freeblock, "Blocks");
    len += sprintf(page + len, "%20s : %10u %s\n", "num used block", meminfo.m_num_usedblock, "Blocks");

    shm_debug("read_proc_meminfo OK. (%d / %d)\n", len, count);

done:

    *eof = 1;
    
    return ((count < len) ? count : len);
}

static int read_proc_baseinfo(char *page, char **start, off_t offset,
        int count, int *eof, void *data)
{
    int len = 0, res;
    
    MV_SHM_BaseInfo_t baseinfo;
    res = MV_SHM_GetBaseInfo(&baseinfo);
    if (res != 0)
    {
        shm_error("shm_device_get_baseinfo failed. (%d)\n", res);
        len += sprintf(page, "shm_device_get_baseinfo failed. (%d)\n", res);
        goto done;
    }

    //!!!!! avoid buffer overflow !!!    
    len += sprintf(page + len, "%20s : %10u %s\n", "memory size", baseinfo.m_size, "Bytes");
    len += sprintf(page + len, "%20s : %10u %s\n", "threshold", baseinfo.m_threshold, "Bytes");
    len += sprintf(page + len, "------------ physical address ------------\n");
    len += sprintf(page + len, "%20s : 0x%08X\n", "cache physaddr", baseinfo.m_base_physaddr_cache);
    len += sprintf(page + len, "%20s : 0x%08X\n", "non-cache physaddr", baseinfo.m_base_physaddr_nocache);
    len += sprintf(page + len, "-------- kernel virtual address ----------\n");
    len += sprintf(page + len, "%20s : 0x%08X\n", "cache virtaddr", baseinfo.m_base_virtaddr_cache);
    len += sprintf(page + len, "%20s : 0x%08X\n", "non-cache virtaddr", baseinfo.m_base_virtaddr_nocache);

    shm_debug("read_proc_baseinfo OK. (%d / %d)\n", len, count);

done:

    *eof = 1;
    
    return ((count < len) ? count : len);
}

static int read_proc_detail(char *page, char **start, off_t offset,
        int count, int *eof, void *data)
{
    int len = 0;

    len = shm_device_get_detail(shm_device, page, count);
    if (len <= 0)
    {
        shm_error("read_proc_detail failed. (%d)\n", len);
        len += sprintf(page, "read_proc_detail failed. (%d)\n", len);
        goto done;
    }

    shm_debug("read_proc_detail OK. (%d / %d)\n", len, count);
    
done:

    *eof = 1;
    
    return ((count < len) ? count : len);
}

/*******************************************************************************
    Module Register API
*/

static int shm_driver_setup_cdev(struct cdev *dev, int major, int minor,
		struct file_operations *fops)
{  
	cdev_init(dev, fops);
	dev->owner = THIS_MODULE;
	dev->ops = fops;
	return cdev_add (dev, MKDEV(major, minor), 1);
}

static int __init shm_driver_init(void)
{
    int res, i;

	/* Figure out our device number. */
	res = register_chrdev_region(MKDEV(GALOIS_SHM_MAJOR, 0), GALOIS_SHM_MINORS, SHM_DEVICE_NAME);
	if (res < 0) {
		shm_error("unable to get shm device major [%d]\n", GALOIS_SHM_MAJOR);
		goto err_reg_device;
	}
	shm_debug("register cdev device major [%d]\n", GALOIS_SHM_MAJOR);

	/* Now setup cdevs. */
	for (i = 0; i < ARRAY_SIZE(shm_driver_dev_list); i++) {
		res = shm_driver_setup_cdev(shm_driver_dev_list[i].cdev, 
				GALOIS_SHM_MAJOR, shm_driver_dev_list[i].minor, shm_driver_dev_list[i].fops);
		if (res) {
			shm_error("shm_driver_setup_cdev failed in [%d].\n", i);
			goto err_add_device;
		}
		shm_debug("setup cdevs device minor [%d]\n", shm_driver_dev_list[i].minor);
	}

	/* add shm devices to sysfs */
	shm_dev_class = class_create(THIS_MODULE, SHM_DEVICE_NAME);
	if (IS_ERR(shm_dev_class)) {
		shm_error("class_create failed.\n");
		res = -ENODEV;
		goto err_add_device;
	}
	
	for (i = 0; i < ARRAY_SIZE(shm_driver_dev_list); i++) {
		device_create(shm_dev_class, NULL, 
				MKDEV(GALOIS_SHM_MAJOR, shm_driver_dev_list[i].minor), 
				NULL, shm_driver_dev_list[i].name);
	    shm_debug("create device sysfs [%s]\n", shm_driver_dev_list[i].name);
	}

	/* create shm device*/
    res = shm_device_create(&shm_device, shm_base, shm_size, SHM_DEVICE_THRESHOLD);
    if (res != 0) {
        shm_error("shm_device_create failed.\n");
        goto err_add_device;
    }

    /* create shm kernel API */
    res = MV_SHM_Init(shm_device);
    if (res != 0) {
        shm_error("MV_SHM_Init failed !!!\n");
        goto err_SHM_Init;
    }
    
	/* create shm device proc file*/
	shm_driver_procdir = proc_mkdir(SHM_DEVICE_NAME, NULL);
	shm_driver_procdir->owner = THIS_MODULE;
    create_proc_read_entry(SHM_DEVICE_PROCFILE_MEMINFO, 0, shm_driver_procdir, read_proc_meminfo, NULL);
    create_proc_read_entry(SHM_DEVICE_PROCFILE_BASEINFO, 0, shm_driver_procdir, read_proc_baseinfo, NULL);
    create_proc_read_entry(SHM_DEVICE_PROCFILE_DETAIL, 0, shm_driver_procdir, read_proc_detail, NULL);

	shm_trace("shm_driver_init OK\n");
    
	return 0;

err_SHM_Init:
    
    shm_trace("shm_driver_init Undo ...\n");
       
    shm_device_destroy(&shm_device);

	/* del sysfs entries */	
	for (i = 0; i < ARRAY_SIZE(shm_driver_dev_list); i++) {
		device_destroy(shm_dev_class, MKDEV(GALOIS_SHM_MAJOR, shm_driver_dev_list[i].minor));
		shm_debug("delete device sysfs [%s]\n", shm_driver_dev_list[i].name);
	}
	class_destroy(shm_dev_class);
	
err_add_device:
    
	for (i = 0; i < ARRAY_SIZE(shm_driver_dev_list); i++) {
		cdev_del(shm_driver_dev_list[i].cdev);
	}
	unregister_chrdev_region(MKDEV(GALOIS_SHM_MAJOR, 0), GALOIS_SHM_MINORS);

err_reg_device:
    
	shm_trace("shm_driver_init failed !!! (%d)\n", res);

	return res;
}

static void __exit shm_driver_exit(void)
{
    int res, i;

    /* destroy shm kernel API */
    res = MV_SHM_Exit();
    if (res != 0)
        shm_error("MV_SHM_Exit failed !!!\n");

    /* remove shm device proc file*/
    remove_proc_entry(SHM_DEVICE_PROCFILE_DETAIL, shm_driver_procdir);
    remove_proc_entry(SHM_DEVICE_PROCFILE_BASEINFO, shm_driver_procdir);
    remove_proc_entry(SHM_DEVICE_PROCFILE_MEMINFO, shm_driver_procdir);
    remove_proc_entry(SHM_DEVICE_NAME, NULL);
    
    if (shm_device_destroy(&shm_device) != 0)
        shm_error("shm_device_destroy failed.\n");

	/* del sysfs entries */	
	for (i = 0; i < ARRAY_SIZE(shm_driver_dev_list); i++) {
		device_destroy(shm_dev_class, MKDEV(GALOIS_SHM_MAJOR, shm_driver_dev_list[i].minor));
		shm_debug("delete device sysfs [%s]\n", shm_driver_dev_list[i].name);
	}
	class_destroy(shm_dev_class);

	/* del cdev */
	for (i = 0; i < ARRAY_SIZE(shm_driver_dev_list); i++) {
		cdev_del(shm_driver_dev_list[i].cdev);
		shm_debug("delete cdevs device minor [%d]\n", shm_driver_dev_list[i].minor);
	}

	unregister_chrdev_region(MKDEV(GALOIS_SHM_MAJOR, 0), GALOIS_SHM_MINORS);	
	shm_debug("unregister cdev device major [%d]\n", GALOIS_SHM_MAJOR);
	
    shm_trace("shm_driver_exit OK\n");
}

module_init(shm_driver_init);
module_exit(shm_driver_exit);


