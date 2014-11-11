////////////////////////////////////////////////////////////////////////////////
//! \file cc_driver.c
//! \brief Shared Memory driver
//!	
//! Purpose:
//!
//!
//!	Version, Date and Author :
//!		V 1.00,	06/09/2010,	Fang Bao,   design for Single CPU system
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
#include "OSAL_api.h"
#include "CCDSS.h"
#include "CCUDP.h"
#include "CC_CBuf_type.h"

/*******************************************************************************
    Module Descrption
*/

MODULE_AUTHOR("Fang Bao");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Common Comunication Driver");

/*******************************************************************************
    Module API defined
*/

static int cc_driver_open (struct inode *inode, struct file *filp);
static int cc_driver_release(struct inode *inode, struct file *filp);
static int cc_driver_ioctl(struct inode *inode, struct file *filp,
                 unsigned int cmd, unsigned long arg);

/*******************************************************************************
    Macro Defined
*/

#ifdef ENABLE_DEBUG
    #define cc_debug(...)   printk(KERN_INFO CC_DEVICE_TAG __VA_ARGS__)
#else
    #define cc_debug(...)
#endif

#define cc_trace(...)      printk(KERN_ALERT CC_DEVICE_TAG __VA_ARGS__)
#define cc_error(...)      printk(KERN_ERR CC_DEVICE_TAG __VA_ARGS__)

/*******************************************************************************
    Module Variable
*/

//static cc_device_t *cc_device = NULL;

static struct cdev cc_dev;
static struct class *cc_dev_class;

static struct file_operations cc_ops = {
	.open    = cc_driver_open,
	.release = cc_driver_release,
	.ioctl   = cc_driver_ioctl,
	.owner   = THIS_MODULE,
};

static struct proc_dir_entry *cc_driver_procdir;

char * tag_service[MV_CC_SrvType_Max + 1] = {
    "MsgQ",
    "RPC",
    "CBuf",
    "MsgQEx",
    "RPCClnt",
    "Unknown"
};

/*******************************************************************************
    Module API
*/

static int cc_driver_open (struct inode *inode, struct file *filp)
{   
//	filp->private_data = cc_device;

    cc_debug("cc_driver_open ok\n");
	
	return 0;
}

static int cc_driver_release(struct inode *inode, struct file *filp)
{   
//    filp->private_data = NULL;

    cc_debug("cc_driver_release ok\n");

	return 0;
}

static int cc_driver_ioctl(struct inode *inode, struct file *filp,
                 unsigned int cmd, unsigned long arg)
{
	MV_CC_DSS_ServiceInfo_t SrvInfo;
	MV_CC_DSS_ServiceInfo_DataList_t	SrvInfoList;
	pMV_CC_DSS_ServiceInfo_DataList_t	pSrvInfoList;
	int res = 0;

	cc_debug("cc_driver_ioctl cmd = 0x%08x\n", cmd);

    switch(cmd) {

	  case CC_DEVICE_CMD_REG:
	  	
	  	// DSS Reg

		if (copy_from_user(&SrvInfo, (int __user *)arg, sizeof(SrvInfo)))
			return -EFAULT;

		res = MV_CC_DSS_Reg(&SrvInfo);
				
		if (copy_to_user((void __user *)arg, &SrvInfo, sizeof(SrvInfo)))
			return -EFAULT;
 
		break;

	  case CC_DEVICE_CMD_FREE:
	  	
	  	// DSS Free

		if (copy_from_user(&SrvInfo, (int __user *)arg, sizeof(SrvInfo)))
			return -EFAULT;

		res = MV_CC_DSS_Free(&SrvInfo);
 
		break;

	  case CC_DEVICE_CMD_INQUIRY:
	  	
	  	// DSS Inquiry

		if (copy_from_user(&SrvInfo, (int __user *)arg, sizeof(SrvInfo)))
			return -EFAULT;

		res = MV_CC_DSS_Inquiry(&SrvInfo);
				
		if (copy_to_user((void __user *)arg, &SrvInfo, sizeof(SrvInfo)))
			return -EFAULT;
 
		break;

	  case CC_DEVICE_CMD_GET_LIST:
	  	
	  	// DSS GetList

		if (copy_from_user(&SrvInfoList, (int __user *)arg, sizeof(MV_CC_DSS_ServiceInfo_DataList_t)))
			return -EFAULT;

		if (SrvInfoList.m_ListNum != 0)
		{
			pSrvInfoList = (pMV_CC_DSS_ServiceInfo_DataList_t)MV_OSAL_Malloc(SrvInfoList.m_ListNum * sizeof(MV_CC_DSS_ServiceInfo_t) + sizeof(MV_CC_DSS_ServiceInfo_DataList_t));
			if (pSrvInfoList == NULL)
				return -EFAULT;
			
			pSrvInfoList->m_ListNum = SrvInfoList.m_ListNum;
			pSrvInfoList->m_DataNum = 0;

			res = MV_CC_DSS_GetList(pSrvInfoList);

			if (copy_to_user((void __user *)arg, pSrvInfoList, (pSrvInfoList->m_DataNum * sizeof(MV_CC_DSS_ServiceInfo_t) + sizeof(MV_CC_DSS_ServiceInfo_DataList_t))))
			{
				MV_OSAL_Free(pSrvInfoList);
				return -EFAULT;
			}
			
			MV_OSAL_Free(pSrvInfoList);
		}
		else
			res = E_BADVALUE;
 
		break;

	  case CC_DEVICE_CMD_TEST_MSG:
    	  	// TEST MSG in Linux Kernel	
	  	{
	  	    MV_CC_MSG_t MSG;
    
    		if (copy_from_user(&SrvInfo, (int __user *)arg, sizeof(SrvInfo)))
    			return -EFAULT;
    			
    	    MSG.m_MsgID = SrvInfo.m_ServiceID;
    	    MSG.m_Param1 = SrvInfo.m_Data[0];
    	    MSG.m_Param2 = SrvInfo.m_Data[1];
    
    		res = MV_CC_MsgQ_PostMsgByID(SrvInfo.m_ServiceID, &MSG);

    		break;
	    }		

	  case CC_DEVICE_CMD_UPDATE:
	  	
	  	// DSS Update

		if (copy_from_user(&SrvInfo, (int __user *)arg, sizeof(SrvInfo)))
			return -EFAULT;

		res = MV_CC_DSS_Update(&SrvInfo);
 
		break;

	  case CC_DEVICE_CMD_CREATE_CBUF:
	  	
	  	// create CBuf

		if (copy_from_user(&SrvInfo, (int __user *)arg, sizeof(SrvInfo)))
			return -EFAULT;

		res = MV_CC_CBufSrv_Create((pMV_CC_DSS_ServiceInfo_CBuf_t)&SrvInfo);

		if (copy_to_user((void __user *)arg, &SrvInfo, sizeof(SrvInfo)))
			return -EFAULT;
 
		break;

	  case CC_DEVICE_CMD_DESTROY_CBUF:
	  	
	  	// destroy CBuf

		if (copy_from_user(&SrvInfo, (int __user *)arg, sizeof(SrvInfo)))
			return -EFAULT;

		res = MV_CC_CBufSrv_Destroy((pMV_CC_DSS_ServiceInfo_CBuf_t)&SrvInfo);

		if (copy_to_user((void __user *)arg, &SrvInfo, sizeof(SrvInfo)))
			return -EFAULT;
 
		break;
		
	  default:
	    
	    res = -ENOTTY;
	}

	cc_debug("cc_driver_ioctl res = 0x%08X\n", res);

	return res;
}

static int read_proc_status(char *page, char **start, off_t offset,
        int count, int *eof, void *data)
{
    int len = 0, res;
    
    MV_CC_DSS_Status_t status;
    res = MV_CC_DSS_GetStatus(&status);
    if (res != 0)
    {
        cc_error("read_proc_status failed. (0x%08X)\n", res);
        len += sprintf(page, "read_proc_status failed. (0x%08X)\n", res);
        goto done;
    }
    
    //!!!!! avoid buffer overflow !!!
    len += sprintf(page + len, "%-25s : %10u %s\n", "DSS Reg Count", status.m_RegCount, "");
    len += sprintf(page + len, "%-25s : %10u %s\n", "DSS Reg Err Count", status.m_RegErrCount, "");

    len += sprintf(page + len, "%-25s : %10u %s\n", "DSS Free Count", status.m_FreeCount, "");
    len += sprintf(page + len, "%-25s : %10u %s\n", "DSS Free Err Count", status.m_FreeErrCount, "");

    len += sprintf(page + len, "%-25s : %10u %s\n", "DSS Inquiry Count", status.m_InquiryCount, "");
    len += sprintf(page + len, "%-25s : %10u %s\n", "DSS Inquiry Err Count", status.m_InquiryErrCount, "");

    len += sprintf(page + len, "%-25s : %10u %s\n", "DSS Update Count", status.m_UpdateCount, "");
    len += sprintf(page + len, "%-25s : %10u %s\n", "DSS Update Err Count", status.m_UpdateErrCount, "");

    len += sprintf(page + len, "%-25s : %10u %s\n", "DSS Service Count", status.m_ServiceCount, "");
    len += sprintf(page + len, "%-25s : 0x%08X %s\n", "DSS Last Service ID", status.m_LastServiceID, "");

    len += sprintf(page + len, "%-25s : 0x%08X %s\n", "DSS Sequence ID", status.m_SeqID, "");

    cc_debug("read_proc_status OK. (%d / %d)\n", len, count);

done:

    *eof = 1;
    
    return ((count < len) ? count : len);
}

static int read_proc_detail(char *page, char **start, off_t offset,
        int count, int *eof, void *data)
{
    int i, len = 0, list_num = CC_DEVICE_LIST_NUM, res;
    char *ptr_tag;

    pMV_CC_DSS_ServiceInfo_t pSrvInfo;
    pMV_CC_DSS_ServiceInfo_DataList_t pSrvInfoList;    
    pSrvInfoList = (pMV_CC_DSS_ServiceInfo_DataList_t)MV_OSAL_Malloc(list_num * sizeof(MV_CC_DSS_ServiceInfo_t) + sizeof(MV_CC_DSS_ServiceInfo_DataList_t));
    if (pSrvInfoList == NULL)
    {
        cc_error("read_proc_detail failed. (no memory)\n");
        len += sprintf(page, "read_proc_detail failed. (no memory)\n");
        goto done_1;        
    }
    
	pSrvInfoList->m_ListNum = list_num;
	pSrvInfoList->m_DataNum = 0;

	res = MV_CC_DSS_GetList(pSrvInfoList);
    if (res != S_OK)
    {
        cc_error("MV_CC_DSS_GetList failed. (0x%08X)\n", res);
        len += sprintf(page, "MV_CC_DSS_GetList failed. (0x%08X)\n", res);
        goto done_2;        
    }

//    len += sprintf(page + len, "%-10s : %10u %s\n", "List Num", pSrvInfoList->m_ListNum, "");
    len += sprintf(page + len, "%-10s : %10u %s\n", "Data Num", pSrvInfoList->m_DataNum, "");
    len += sprintf(page + len, "%-10s : %10u %s\n", "Max Num", pSrvInfoList->m_MaxNum, "");
	
	len += sprintf(page + len, "\nGalois CC Service List\n");

    len += sprintf(page + len, "-----------------------------------------------------------------------------------\n");
    len += sprintf(page + len, "  No | Service ID |   Type  | Data[0]  Data[4]  Data[8]  Data[12] Data[16] Data[20]\n");
    len += sprintf(page + len, "-----------------------------------------------------------------------------------\n");

	for (i=0; (i<pSrvInfoList->m_DataNum) && (len < count - 100); i++)
	{	    
        pSrvInfo = &(pSrvInfoList->m_SrvInfo[i]);
        
        if (pSrvInfo->m_SrvType < MV_CC_SrvType_Max)
            ptr_tag = tag_service[pSrvInfo->m_SrvType];
        else
            ptr_tag = tag_service[MV_CC_SrvType_Max];
            
        len += sprintf(page + len, " %3d | 0x%08X | %7s | %02X%02X%02X%02X %02X%02X%02X%02X %02X%02X%02X%02X %02X%02X%02X%02X %02X%02X%02X%02X %02X%02X%02X%02X \n", 
            i+1, pSrvInfo->m_ServiceID, ptr_tag,
            pSrvInfo->m_Data[0], pSrvInfo->m_Data[1], pSrvInfo->m_Data[2], pSrvInfo->m_Data[3],
    		pSrvInfo->m_Data[4], pSrvInfo->m_Data[5], pSrvInfo->m_Data[6], pSrvInfo->m_Data[7],
    		pSrvInfo->m_Data[8], pSrvInfo->m_Data[9], pSrvInfo->m_Data[10], pSrvInfo->m_Data[11],
    		pSrvInfo->m_Data[12], pSrvInfo->m_Data[13], pSrvInfo->m_Data[14], pSrvInfo->m_Data[15],
    		pSrvInfo->m_Data[16], pSrvInfo->m_Data[17], pSrvInfo->m_Data[18], pSrvInfo->m_Data[19],
    		pSrvInfo->m_Data[20], pSrvInfo->m_Data[21], pSrvInfo->m_Data[22], pSrvInfo->m_Data[23]);
	}

    cc_debug("read_proc_detail OK. (%d / %d)\n", len, count);

done_2:
    
    MV_OSAL_Free(pSrvInfoList);
    
done_1:

    *eof = 1;
    
    return ((count < len) ? count : len);
}


/*******************************************************************************
    Module Register API
*/

static int cc_driver_setup_cdev(struct cdev *dev, int major, int minor,
		struct file_operations *fops)
{  
	cdev_init(dev, fops);
	dev->owner = THIS_MODULE;
	dev->ops = fops;
	return cdev_add (dev, MKDEV(major, minor), 1);
}

static int __init cc_driver_init(void)
{
    int res;

	/* Figure out our device number. */
	res = register_chrdev_region(MKDEV(GALOIS_CCNEW_MAJOR, 0), GALOIS_CCNEW_MINORS, CC_DEVICE_NAME);
	if (res < 0) {
		cc_error("unable to get cc device major [%d]\n", GALOIS_CCNEW_MAJOR);
		goto err_reg_device;
	}
	cc_debug("register cdev device major [%d]\n", GALOIS_CCNEW_MAJOR);

	/* Now setup cdevs. */
	res = cc_driver_setup_cdev(&cc_dev, GALOIS_CCNEW_MAJOR, GALOIS_CCNEW_MINOR, &cc_ops);	
    if (res) {
		cc_error("cc_driver_setup_cdev failed.\n");
		goto err_add_device;
	}
	cc_debug("setup cdevs device minor [%d]\n", GALOIS_CCNEW_MINOR);

	/* add cc devices to sysfs */
	cc_dev_class = class_create(THIS_MODULE, CC_DEVICE_NAME);
	if (IS_ERR(cc_dev_class)) {
		cc_error("class_create failed.\n");
		res = -ENODEV;
		goto err_add_device;
	}

	device_create(cc_dev_class, NULL, 
				MKDEV(GALOIS_CCNEW_MAJOR, GALOIS_CCNEW_MINOR),
				NULL, CC_DEVICE_NAME);
	cc_debug("create device sysfs [%s]\n", CC_DEVICE_NAME);

	/* create cc device*/
	res = MV_CC_DSS_Init();
    if (res != 0)
        cc_error("MV_CC_DSS_Init failed !!! res = 0x%08X\n", res);

	/* create cc device proc file*/
	cc_driver_procdir = proc_mkdir(CC_DEVICE_NAME, NULL);
	cc_driver_procdir->owner = THIS_MODULE;
    create_proc_read_entry(CC_DEVICE_PROCFILE_STATUS, 0, cc_driver_procdir, read_proc_status, NULL);
    create_proc_read_entry(CC_DEVICE_PROCFILE_DETAIL, 0, cc_driver_procdir, read_proc_detail, NULL);

    /* create cc kernel API */
    res = MV_CC_UDP_Init();
    if (res != 0)
        cc_error("MV_CC_UDP_Init failed !!! res = 0x%08X\n", res);
        
	cc_trace("cc_driver_init OK\n");
    
	return 0;

err_add_device:

	cdev_del(&cc_dev);
	
	unregister_chrdev_region(MKDEV(GALOIS_CCNEW_MAJOR, 0), GALOIS_CCNEW_MINORS);

err_reg_device:
    
	cc_trace("cc_driver_init failed !!! (%d)\n", res);

	return res;
}

static void __exit cc_driver_exit(void)
{
    int res;

    /* destroy cc kernel API */
	res = MV_CC_UDP_Exit();
    if (res != 0)
        cc_error("MV_CC_UDP_Exit failed !!! res = 0x%08X\n", res);
        
    /* remove cc device proc file*/
    remove_proc_entry(CC_DEVICE_PROCFILE_DETAIL, cc_driver_procdir);
    remove_proc_entry(CC_DEVICE_PROCFILE_STATUS, cc_driver_procdir);
    remove_proc_entry(CC_DEVICE_NAME, NULL);

	/* destroy cc device*/    
	res = MV_CC_DSS_Exit();
    if (res != 0)
        cc_error("MV_CC_DSS_Exit failed !!! res = 0x%08X\n", res);

	/* del sysfs entries */	
	device_destroy(cc_dev_class, MKDEV(GALOIS_CCNEW_MAJOR, GALOIS_CCNEW_MINOR));
	cc_debug("delete device sysfs [%s]\n", CC_DEVICE_NAME);	
	
	class_destroy(cc_dev_class);

	/* del cdev */
	cdev_del(&cc_dev);

	unregister_chrdev_region(MKDEV(GALOIS_CCNEW_MAJOR, 0), GALOIS_CCNEW_MINORS);
	cc_debug("unregister cdev device major [%d]\n", GALOIS_CCNEW_MAJOR);
	
    cc_trace("cc_driver_exit OK\n");
}

module_init(cc_driver_init);
module_exit(cc_driver_exit);


