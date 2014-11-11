////////////////////////////////////////////////////////////////////////////////
//! \file main.c
//! \brief CC Module device in Linux Kernel
//!	
//! Purpose:
//!
//!
//!	Version, Date and Author :
//!		V 1.00,	         ,	Fang Bao
//!		V 1.10,	7/23/2008,	Fang Bao	Update for New RPC Trace test
//!							Update for RPC Optimization
//!							Bugfixed - GetSysTime() return error value sometime
//!		V 1.11,	7/24/2008,	Fang Bao	Bugfixed in MV_ICC_Rx_action() - nlmsg_new() and netlink_broadcast() should be set by GFP_ATOMIC, to avoid sleeping in SoftIRQ
//!		V 1.12,	9/17/2008,	Fang Bao	Bugfixed for MV_CC_ICC_MSG_t
//!		V 1.13,	11/03/2008,	Fang Bao	remove value checking in SHM_ioctl_ICCEXTRA
//!		V 1.14,	7/10/2009,	Fang Bao	add CMD_ICCEXTRA_GET_CLOCK command in SHM_ioctl_ICCEXTRA() for new APIs - MV_Time_GetSysTime_NEW & MV_Time_GetSysTimeMS_NEW
//!		V 1.15,	7/28/2009,	Fang Bao	add CMD_ICCEXTRA_GET_CLOCK_ALL command in SHM_ioctl_ICCEXTRA() for new APIs - MV_Time_GetSysTime_NEW & MV_Time_GetSysTimeMS_NEW
//!		V 1.16,	10/23/2009,	Fang Bao	Add CCDSS, CCDSP support (Multi-process)
//!		V 1.17, 04/15/2010, Fang Bao    Add netlink_set_nonroot() for NETLINK_GALOIS_CC_NEW
//!     V 1.18, 04/20/2010, Fang Bao    Bugfixed for SHM_remap_mmap_CACHE(), physaddr is error
//!     V 1.19, 05/06/2010, Fang Bao    add command SHM_IOC_GetSize for Linux Device SHM ioctl (sync with GaloisSoftware revision 33171)
//!                                     add SHM_ioctl operation for Device SHM_remap_ops
//!     V 1.20, 05/10/2010, Fang Bao    add command SHM_IOC_GetAddr for Linux Device SHM ioctl (sync with GaloisSoftware revision 33229)
//!									  
//!		
//! Note:
////////////////////////////////////////////////////////////////////////////////

#include <linux/init.h>

#include <linux/module.h>
#include <linux/moduleparam.h>

#include <linux/netlink.h>
#include <linux/sched.h>
#include <net/sock.h>
#include <net/netlink.h>
#include <linux/proc_fs.h>

#include <linux/kernel.h>   /* printk() */
#include <linux/slab.h>   /* kmalloc() */
#include <linux/fs.h>       /* everything... */
#include <linux/errno.h>    /* error codes */
#include <linux/types.h>    /* size_t */
#include <linux/spinlock.h>
#include <linux/platform_device.h>

#include <linux/mm.h>
#include <linux/kdev_t.h>
#include <asm/page.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/version.h>
#include <mach/galois_generic.h>

#include "galois_cache_flush.h"		/* local definitions */

#include "ICCNet.h"
#include "CCDSS.h"
#include "MV_netlink.h"
#include "CC_RPC_type.h"

#include "com_config.h"
#include "GaloisTime.h"
#include "galois_hal_com.h"
#include "apb_timer_drv.h"

MODULE_AUTHOR("Fang Bao");
MODULE_LICENSE("Dual BSD/GPL");

/**********************************Shared Memory************************************/
static unsigned int SHM_count = 0;


/************************************************************************************/

/**********************************Netlink for CC************************************/

struct sock *pMV_ICCNet_netlink_sock;

MV_CC_ICCNet_t MV_LinuxKernel_ICCNet;
pMV_CC_ICCNet_t pMV_LinuxKernel_ICCNet = &MV_LinuxKernel_ICCNet;

MV_CC_DSP_t MV_LinuxKernel_DSP;
pMV_CC_DSP_t pMV_LinuxKernel_DSP = &MV_LinuxKernel_DSP;

unsigned int uiPidTable[MV_CC_PROCESS_ID_MAX];
spinlock_t uiPidTable_lock = SPIN_LOCK_UNLOCKED; 	/* Static way */ 
                                                  
static __inline__ unsigned int uiPidTable_GetLinuxPID_SOFTIRQ(unsigned int x)
{
	if (x >= MV_CC_PROCESS_ID_MAX)
		return 0;
	else
	{
//		printk(KERN_INFO "[MV_ICCNet] uiPidTable_GetLinuxPID_SOFTIRQ [%d => %d]\n", x, uiPidTable[x]);
		return uiPidTable[x];
	}
}
											  
/************************************************************************************/

INT TestFramePrint(UCHAR *pFrameBuf,
					UINT FrameSize)
{
	UINT i,count = 0, column;

	printk(KERN_INFO "------------------------------------------------------------\n");	
	printk(KERN_INFO "Address   |");
	for (i=0; i<16; i++)
		printk(KERN_INFO " %02X", i);		
	printk(KERN_INFO "\n");
	printk(KERN_INFO "------------------------------------------------------------\n");
		
	while (count < FrameSize) {
		
		printk(KERN_INFO "%08Xh |", count);
		
		for (column=0; column<16; column++) {
			
			printk(KERN_INFO " %02X", pFrameBuf[count]);
			
			count++;
			if (count == (FrameSize) )
				break;
		}
		printk(KERN_INFO "\n");
	}	
	
	printk(KERN_INFO "------------------------------------------------------------\n");	
	return S_OK;		
}


void inline MV_TEST_RPC_TRACE_Add(UCHAR *pBuf, UINT32 num)
{
	if ((ICCP_Protocol(pBuf) == MV_CC_Protocol_CCUDP) && (CCUDP_PacketSrvType(pBuf) == MV_CC_SrvType_RPC))
	if ((RPC_TraceDataSize(pBuf) > 0) && (RPC_TraceDataSize(pBuf) >= num * LOGTYPE_TEST_RPC_TRACE_PointSize))
	{ 
		UINT32 time_point = SYSTEM_TIMER_VALUE - TIMERx_GetCurrentValue(MV_REG_TIMER_6);
		GaloisMemcpy((pBuf + MV_CC_ICCFIFO_FRAME_SIZE - ((num + 1) * LOGTYPE_TEST_RPC_TRACE_PointSize) - 1), (UCHAR *)&time_point, LOGTYPE_TEST_RPC_TRACE_PointSize);
	}
}



/**********************************Netlink for CC************************************/

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CC_MsgQICC_Post
//!
//! Description: Post the Message into Message Queue of Inter-CPU
//!
//! \param ServiceID(IN): -- the ID of the MsgQ Global Service
//! \param pMSG 	(IN): -- the pointer to the Message
//!
//! \return Return:		S_OK
//!						E_INVALIDARG
//!						E_FAIL
//!						E_FULL
//! 
////////////////////////////////////////////////////////////////////////////////
//HRESULT MV_CC_MsgQICC_Post(MV_OSAL_ServiceID_U32_t 	ServiceID,
//						   PVOID 					pMSG)
//{
//	HRESULT res;
//	pMV_CC_ICCAddr_t pSrvAddr;
//	UCHAR MsgBuf[MV_CC_ICCFIFO_FRAME_SIZE];
//	UCHAR *pMsgBuf;
//	UCHAR MsgLen;
//
//	/* Parameter Check */
//	if (pMSG == NULL)
//		MV_OSAL_ErrorHandle("[LinuxKernel] MV_CC_MsgQICC_Post", E_INVALIDARG);		
//		
//	res = GSConfigList_Search(ServiceID);
//	if (res == E_FAIL)
//		MV_OSAL_ErrorHandle("[LinuxKernel] MV_CC_MsgQICC_Post GSConfigList_Search", res);
//
//	if (MV_CC_GSConfigList[res].m_SrvType != MV_CC_SrvType_MsgQ)
//		MV_OSAL_ErrorHandle("[LinuxKernel] MV_CC_MsgQICC_Post ServiceID - Service Type", E_INVALIDARG);
//
//	pSrvAddr = (pMV_CC_ICCAddr_t)&(MV_CC_GSConfigList[res].m_SrvAddr);
//	
//	/* assemble MsgQ Message */
//	pMsgBuf = MsgBuf;
//
//	GaloisMemClear(pMsgBuf, (MV_CC_ICCFIFO_FRAME_SIZE - MV_CC_MSGQ_DATA_BYTEOFFSET));
//
//	MsgQ_Msg_DataLen(pMsgBuf) = sizeof(MV_CC_MSG_t);
//
//	MsgLen = MsgQ_Msg_DataLen(pMsgBuf) + MV_CC_MSGQ_HEAD_BYTESIZE;
//	
//	GaloisMemcpy((pMsgBuf + MV_CC_MSGQ_DATA_BYTEOFFSET), pMSG, MsgQ_Msg_DataLen(pMsgBuf));
//	
//	/* assemble ICCP Datagram */
//	ICCP_SrcCPU(pMsgBuf) = pMV_LinuxKernel_ICCNet->m_FIFOArray.m_LocalCPU;
//	ICCP_SrcProcess(pMsgBuf) = MV_OSAL_PROCESS_DEFAULT;
//	
//	ICCP_DstCPU(pMsgBuf) = pSrvAddr->m_CPU;
//	ICCP_DstProcess(pMsgBuf) = pSrvAddr->m_Process;
//	
//	ICCP_DatagramID(pMsgBuf) = 0;
//	ICCP_Protocol(pMsgBuf) = MV_CC_Protocol_CCUDP;
//	ICCP_Padding(pMsgBuf) = 0xFF;
//	ICCP_TimeStamp(pMsgBuf) = 0x12345678;
//
//	/* assemble UDP Packet */
//	CCUDP_SrcPort(pMsgBuf) = 0;
//	CCUDP_DstPort(pMsgBuf) = pSrvAddr->m_Port;
//	
//	CCUDP_PacketLen(pMsgBuf) = MsgLen;
//	CCUDP_PacketSrvLevel(pMsgBuf) = MV_CC_SrvLevel_ICC;
//	CCUDP_PacketSrvType(pMsgBuf) = MV_CC_SrvType_MsgQ;
//	
//	LOGPRINTF((KERN_INFO "[LinuxKernel] [CPU-%d port-%d] sent UDP Parket Len[%d] to [CPU-%d port-%d]\n", 
//				ICCP_SrcCPU(pMsgBuf),
//				CCUDP_SrcPort(pMsgBuf),
//				CCUDP_PacketLen(pMsgBuf),			
//				ICCP_DstCPU(pMsgBuf),
//				CCUDP_DstPort(pMsgBuf)));
//
//	/* Send a CC-UDP Packet*/
//	res = MV_CC_ICCNetWrite(pMV_LinuxKernel_ICCNet, pMsgBuf);	
//	
//	if (res != S_OK)
//	{
//		//self->m_ComErrCount++;
//		MV_OSAL_ErrorPrint("[LinuxKernel] MV_CC_MsgQICC_Post MV_CC_UDP_SendTo", res);
//	}
//	
//	return res;
//}


#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,24)
static void MV_ICCNetWrite_netlink_recv_handler(struct sock * sk, int length)
#else
static void MV_ICCNetWrite_netlink_recv_handler(struct sk_buff* skb)
#endif
{
//  wake_up(sk->sk_sleep);
	return;
}


// Soft IRQ for ICC RX
static void MV_ICC_Rx_action(struct softirq_action *h)
{
  	struct sk_buff * skb;
  	struct nlmsghdr * nlhdr = NULL;
  	int len, res;
  	unsigned int dst_pid;

	UCHAR RxFrameBuf[MV_CC_ICCFIFO_FRAME_SIZE];
	UCHAR *pRxFrameBuf = RxFrameBuf;

	/*
	 * NOTE: Be sure that MV_ICC_Rx_action is NOT re-entrant, softirq is quolified for this.
	 * Don't need disable irq again.
	 */
	//local_irq_disable();

	do {
		res = MV_CC_ICCNetRead(pMV_LinuxKernel_ICCNet, pRxFrameBuf);
		if (res == E_EMPTY)
			break;
		else
		if (res != S_OK)
		{
			printk(KERN_ALERT "[MV_ICCNet] MV_CC_ICCNetRead Error[0x%08x].\n", res);
			break;
		}

		/* ICCP Datagram Handle by Protocol CC DSP */
		if ( (ICCP_Protocol(pRxFrameBuf)) == MV_CC_Protocol_CCDSP )
		{
			res = MV_CC_DSS_DSPOnRecvDemux(pMV_LinuxKernel_DSP, pRxFrameBuf);
			continue;
		}
			
		MV_TEST_RPC_TRACE_Add(pRxFrameBuf, LOGTYPE_TEST_RPC_TRACE_5);
		
		len = MV_CC_ICCFIFO_FRAME_SIZE;
		skb = NULL;
		skb = nlmsg_new(len, GFP_ATOMIC);		// It is unacceptable that GFP_KERNEL will call to kmalloc() which sleep while kernel is trying to find memory
		if (skb == NULL)
	    {
			printk(KERN_ALERT "[MV_ICCNet] Can't create a netlink message.\n");
			break;
	    }
	    
	    nlhdr = nlmsg_put(skb, 0, 0, 0, len, 0);
		memcpy(NLMSG_DATA(nlhdr), pRxFrameBuf, len);
	    NETLINK_CB(skb).pid = 0;
	    NETLINK_CB(skb).dst_group = NETLINK_GALOIS_CC_GROUP;
	    
	    dst_pid = uiPidTable_GetLinuxPID_SOFTIRQ(ICCP_DstProcess(pRxFrameBuf));
	    if (dst_pid == 0)
	    {
		    // set MSG_DONTWAIT for a Nonblocking io, avoid sleep in netlink_broadcast
		    // set GFP_ATOMIC for a Nonblocking io, avoid sleep in netlink_broadcast	    
	        res = netlink_broadcast(pMV_ICCNet_netlink_sock, skb, 0, NETLINK_GALOIS_CC_GROUP, GFP_ATOMIC);
	    	if (res)
	    		printk(KERN_ALERT "[MV_ICCNet] MV_ICC_Rx_action netlink_broadcast error(%d)\n", res);
	        
//	    	printk(KERN_INFO "[MV_ICCNet] MV_ICC_Rx_action netlink_broadcast [%d]\n", dst_pid);
		}
	    else
	    {
	    	res = netlink_unicast(pMV_ICCNet_netlink_sock, skb, dst_pid, MSG_DONTWAIT);
	    		    	
//	    	printk(KERN_INFO "[MV_ICCNet] MV_ICC_Rx_action netlink_unicast [%d]\n", dst_pid);
	    }

	}while(1);
	
	//local_irq_enable();
		
	return;
}

static int MV_ICCNet_netlink_init(void)
{
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,24)
  pMV_ICCNet_netlink_sock = netlink_kernel_create( NETLINK_GALOIS_CC_NEW, 0, MV_ICCNetWrite_netlink_recv_handler, THIS_MODULE);
#else
  pMV_ICCNet_netlink_sock = netlink_kernel_create(&init_net, NETLINK_GALOIS_CC_NEW, 0, MV_ICCNetWrite_netlink_recv_handler, NULL, THIS_MODULE);
  netlink_set_nonroot(NETLINK_GALOIS_CC_NEW, NL_NONROOT_RECV);
#endif

  if (pMV_ICCNet_netlink_sock == NULL) 
  {
    printk(KERN_ALERT "[MV_ICCNet] Fail to create netlink socket.\n");
    return 1;
  }
  
  if (MV_CC_ICCNetInit(pMV_LinuxKernel_ICCNet) != S_OK)
  {
    printk(KERN_ALERT "[MV_ICCNet] Fail to init ICCNet.\n");
    
    sock_release(pMV_ICCNet_netlink_sock->sk_socket); 
    return 1;
  }
  
  if (MV_CC_DSS_Constructor(pMV_LinuxKernel_DSP, pMV_LinuxKernel_ICCNet) != S_OK)
  {
    printk(KERN_ALERT "[MV_ICCNet] Fail to MV_CC_DSS_Constructor.\n");
    return 1;
  } 
  
  // Register Soft IRQ for ICC RX
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,24)
  open_softirq(ICC_RX_SOFTIRQ, MV_ICC_Rx_action, NULL);
#else
  open_softirq(ICC_RX_SOFTIRQ, MV_ICC_Rx_action);
#endif
  printk(KERN_ALERT "[MV_ICCNet] Netlink socket start.\n");
  
  return 0;
}


static void MV_ICCNet_netlink_exit(void)
{
  if (pMV_ICCNet_netlink_sock != NULL)
  {
	HRESULT res;
	UCHAR MsgBuf[MV_CC_ICCFIFO_FRAME_SIZE];
	UCHAR *pMsgBuf = MsgBuf;
	  
	/* assemble ICCP Datagram */
	ICCP_SrcCPU(pMsgBuf) = 0;
	ICCP_SrcProcess(pMsgBuf) = MV_OSAL_PROCESS_DEFAULT;
	
	ICCP_DstCPU(pMsgBuf) = 0;
	ICCP_DstProcess(pMsgBuf) = MV_OSAL_PROCESS_DEFAULT;
	
	ICCP_DatagramID(pMsgBuf) = 0;
	ICCP_Protocol(pMsgBuf) = MV_CC_Protocol_CCUDP;
	ICCP_Padding(pMsgBuf) = 0xFF;
	ICCP_TimeStamp(pMsgBuf) = 0x12345678;
		  
    res = MV_CC_ICCNetWrite(pMV_LinuxKernel_ICCNet, pMsgBuf);
    if (res != S_OK)
    {
    	 printk(KERN_ALERT "[MV_ICCNet] Fail exit to MV_CC_ICCNetWrite [0x%08x]\n", res);
    }
    
	sock_release(pMV_ICCNet_netlink_sock->sk_socket);      
  }
  
  if (MV_CC_DSS_Destructor(pMV_LinuxKernel_DSP) != S_OK)
	    printk(KERN_ALERT "[MV_ICCNet] Fail to MV_CC_DSS_Destructor.\n");  

  if (MV_CC_ICCNetExit(pMV_LinuxKernel_ICCNet, MV_CC_NET_CMD_NODO) != S_OK)
	    printk(KERN_ALERT "[MV_ICCNet] Fail to exit ICCNet.\n");

  printk(KERN_ALERT "[MV_ICCNet] Netlink socket exit.\n");
}


ssize_t MV_ICCNetWrite_fop(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
    unsigned char framebuf[MV_CC_ICCFIFO_FRAME_SIZE];
    ssize_t retval = -ENOMEM; 		/* value used in "goto out" statements */
    int res;
    
    if (count != MV_CC_ICCFIFO_FRAME_SIZE)
    {
    	retval = -EINVAL;
    	goto MV_ICCNetWrite_fop_exit;
	}	
    
    if (copy_from_user(framebuf, buf, count))
    {
        retval = -EFAULT;
        goto MV_ICCNetWrite_fop_exit;
    }
    
    MV_TEST_RPC_TRACE_Add(framebuf, LOGTYPE_TEST_RPC_TRACE_1);

	res = MV_CC_ICCNetWrite(pMV_LinuxKernel_ICCNet, framebuf);
	if (res != S_OK)
    {
		printk(KERN_ALERT "[MV_ICCNet] MV_CC_ICCNetWrite error[%08x].\n", res);
		retval = -EFAULT;
		goto MV_ICCNetWrite_fop_exit;
    }

    retval = count;

MV_ICCNetWrite_fop_exit:

    return retval;
}

/************************************************************************************/


/**********************************Shared Memory************************************/


/*
 * Open the device; in fact, there's nothing to do here.
 */
static int SHM_open (struct inode *inode, struct file *filp)
{
//	printk(KERN_ALERT "[Galois][MV_CC_SHM]SHM Device Open!\n");	
	
	return 0;
}


/*
 * Closing is just as simpler.
 */
static int SHM_release(struct inode *inode, struct file *filp)
{
//	printk(KERN_ALERT "[Galois][MV_CC_SHM]SHM Device Close!\n");	

	return 0;
}



/*
 * Common VMA ops.
 */

void SHM_vma_open(struct vm_area_struct *vma)
{
	SHM_count++;
//	printk(KERN_ALERT "[MV_CC_SHM]SHM VMA open, virt %lx, phys %lx\n",
//			vma->vm_start, vma->vm_pgoff << PAGE_SHIFT);
}

void SHM_vma_close(struct vm_area_struct *vma)
{
	SHM_count--;
//	printk(KERN_ALERT "[MV_CC_SHM]SHM VMA close.\n");
}

static struct vm_operations_struct SHM_remap_vm_ops = {
	.open =  SHM_vma_open,
	.close = SHM_vma_close,
};

static int SHM_remap_mmap(struct file *filp, struct vm_area_struct *vma)
{
	unsigned long off = vma->vm_pgoff << PAGE_SHIFT;
	unsigned long physaddr = MV_SHM_Start_PhysAddr + off;
	unsigned long pfn = physaddr >> PAGE_SHIFT;
	unsigned long vsize = vma->vm_end - vma->vm_start;
	unsigned long psize = MV_SHM_FullSize - off;
	
	//Don't set it!!!!!!
//	vma->vm_flags = VM_IO | VM_RESERVED;

// Eventually remove these printks
//	printk (KERN_ALERT "---- SHM_remap_mmap, off %lx size %lx\n", off, vsize);
//	printk (KERN_ALERT "Phys address is %p\n", (void *)physaddr);
//	printk (KERN_ALERT "Page frame is %ld\n", pfn);
//	printk (KERN_ALERT "Phys Size is %ld\n", psize);
	
	if (vsize > psize)
		return -EINVAL; // spans too high
		
	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);	
	if (remap_pfn_range(vma, vma->vm_start, pfn, vsize, vma->vm_page_prot))
		return -EAGAIN;
	
	vma->vm_ops = &SHM_remap_vm_ops;
	
	SHM_vma_open(vma);
	
	return 0;
}


static int SHM_remap_mmap_CACHE(struct file *filp, struct vm_area_struct *vma)
{
	unsigned long off = vma->vm_pgoff << PAGE_SHIFT;
	unsigned long physaddr = MV_SHM_Start_PhysAddr_CACHE + off;
	unsigned long pfn = physaddr >> PAGE_SHIFT;
	unsigned long vsize = vma->vm_end - vma->vm_start;
	unsigned long psize = MV_SHM_FullSize - off;
	
	//Don't set it!!!!!!
//	vma->vm_flags = VM_IO | VM_RESERVED;

// Eventually remove these printks
//	printk (KERN_ALERT "---- SHM_remap_mmap_CACHE, off %lx size %lx\n", off, vsize);
//	printk (KERN_ALERT "Phys address is %p\n", (void *)physaddr);
//	printk (KERN_ALERT "Page frame is %ld\n", pfn);
//	printk (KERN_ALERT "Phys Size is %ld\n", psize);
	
	if (vsize > psize)
		return -EINVAL; // spans too high
	
	//! For Cache Memroy
//	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);	
	if (remap_pfn_range(vma, vma->vm_start, pfn, vsize, vma->vm_page_prot))
		return -EAGAIN;
	
	vma->vm_ops = &SHM_remap_vm_ops;
	
	SHM_vma_open(vma);
	
	return 0;
}

int SHM_ioctl(struct inode *inode, struct file *filp,
                 unsigned int cmd, unsigned long arg)
{
    unsigned int shm_size;
    unsigned int shm_addr;

    switch(cmd) {

      case SHM_IOC_GetSize:
        
        shm_size = MV_SHM_FullSize;
		if (copy_to_user((void __user *)arg, &shm_size, sizeof(shm_size)))
			return -EFAULT;
		else
			return 0;        
       
        break;

      case SHM_IOC_GetAddr:
        
        // return Non-Cache physical Address
        shm_addr = MV_SHM_Start_PhysAddr;
		if (copy_to_user((void __user *)arg, &shm_addr, sizeof(shm_addr)))
			return -EFAULT;
		else
			return 0;
       
        break;
        
	  default:  /* redundant, as cmd was checked against MAXNR */
		return -ENOTTY;
	}

	return 0;
}

int SHM_ioctl_CACHE(struct inode *inode, struct file *filp,
                 unsigned int cmd, unsigned long arg)
{

//	int err = 0, tmp;
//	int retval = 0;

	SHM_cache_operation_t op;
    unsigned int shm_size;
    unsigned int shm_addr;

    switch(cmd) {

      case SHM_IOC_GetSize:
        
        shm_size = MV_SHM_FullSize;
		if (copy_to_user((void __user *)arg, &shm_size, sizeof(shm_size)))
			return -EFAULT;
		else
			return 0;        
       
        break;

      case SHM_IOC_GetAddr:
        
        // return Cache physical Address
        shm_addr = MV_SHM_Start_PhysAddr_CACHE;
		if (copy_to_user((void __user *)arg, &shm_addr, sizeof(shm_addr)))
			return -EFAULT;
		else
			return 0;
       
        break;

	  case SHM_IOC_Invalidate:
	  	
//	  	retval = __get_user(&op, (int __user *)arg);
//		if (retval == 0)
		if (copy_from_user(&op, (int __user *)arg, sizeof(SHM_cache_operation_t)) == 0)
		{
			//printk (KERN_ALERT "SHM_IOC_Invalidate, adr=%08xh size %d\n", op.adr, op.size);
			//do action
			 InvalidateDCacheRegion((int *)(op.adr), op.size);
		}
		else
			return -EFAULT;
					
		break;

	  case SHM_IOC_Clean:
	  	        
//	  	retval = __get_user(&op, (int __user *)arg);
//		if (retval == 0)
		if (copy_from_user(&op, (int __user *)arg, sizeof(SHM_cache_operation_t)) == 0)
		{
			//printk (KERN_ALERT "SHM_IOC_Clean, adr=%08xh size %d\n", op.adr, op.size);
			//do action
			 CleanDCacheRegion((int *)(op.adr), op.size);
		}
		else
			return -EFAULT;
					
		break;

	  case SHM_IOC_CleanAndInvalidate:
	  	
//	  	retval = __get_user(&op, (int __user *)arg);
//		if (retval == 0)
		if (copy_from_user(&op, (int __user *)arg, sizeof(SHM_cache_operation_t)) == 0)
		{
			//printk (KERN_ALERT "SHM_IOC_CleanAndInvalidate, adr=%08xh size %d\n", op.adr, op.size);
			//do action
			 CleanAndInvalidateDCacheRegion((int *)(op.adr), op.size);
		}
		else
			return -EFAULT;
		
		break;

	  default:  /* redundant, as cmd was checked against MAXNR */
		return -ENOTTY;
	}
	
//	return retval;
	
	return 0;
}


int SHM_ioctl_ICCEXTRA(struct inode *inode, struct file *filp,
                 unsigned int cmd, unsigned long arg)
{
	volatile unsigned long time_high, time_low;
	 unsigned long flags;
	 MV_TimeSpec_t SysTime;
	 
	switch(cmd) {

	  case CMD_ICCEXTRA_GET_SYSTIME:
	  	
	  	// Close interrupt
	  	local_irq_save(flags);
	  	
		time_low	= TIMERx_GetCurrentValue(TIMER_SYSTEM);
		time_high	= *(unsigned long *)(MV_ICC_EXTRA_ADDR_def_KernVirt + MV_ADDR_system_timer_jiffies);
	
		// Restore interrupt
		local_irq_restore(flags);
		
//		if (time_low > SYSTEM_TIMER_DELTA)
//		{
//			SysTime.tv_sec = 0;
//			SysTime.tv_nsec = (long)time_low;
//
//			if (copy_to_user((void __user *)arg, &SysTime, sizeof(SysTime)))
//				return -EFAULT;		
//			else
//				return EINVAL;
//		}
//		else
//		{
			SysTime.tv_sec = (long)time_high;
			SysTime.tv_nsec = (long)time_low;
			
			if (copy_to_user((void __user *)arg, &SysTime, sizeof(SysTime)))
				return -EFAULT;
			else
				return 0;
//		}
 	
		break;

	case CMD_ICCEXTRA_TEST_01:
		
		return EINVAL;
		break;

	  case CMD_ICCEXTRA_GET_CLOCK:
	  	
	  	// Close interrupt
	  	local_irq_save(flags);
	  	
	  	// read Clock_Second first
		time_high	= *(unsigned long *)(MV_ICC_EXTRA_ADDR_def_KernVirt + MV_ADDR_system_Clock_Second);
		time_low	= *(unsigned long *)(MV_ICC_EXTRA_ADDR_def_KernVirt + MV_ADDR_system_Clock_Timer);
	
		// Restore interrupt
		local_irq_restore(flags);
		
			SysTime.tv_sec = (long)time_high;
			SysTime.tv_nsec = (long)time_low;
			
			if (copy_to_user((void __user *)arg, &SysTime, sizeof(SysTime)))
				return -EFAULT;
			else
				return 0;
 	
		break;

	  case CMD_ICCEXTRA_GET_CLOCK_ALL:
	  	{
	  		volatile unsigned long clock_second_2, hardware_timer;
		  	system_clock_data_t system_clock;
		 	int i;
		  	// Close interrupt
		  	local_irq_save(flags);
		  	
		  	// read Clock_Second first
			time_high	= *(unsigned long *)(MV_ICC_EXTRA_ADDR_def_KernVirt + MV_ADDR_system_Clock_Second);
			time_low	= *(unsigned long *)(MV_ICC_EXTRA_ADDR_def_KernVirt + MV_ADDR_system_Clock_Timer);
	
			// delay 10 instructions
			for (i=0;i<2;i++);
			
			clock_second_2 = *(unsigned long *)(MV_ICC_EXTRA_ADDR_def_KernVirt + MV_ADDR_system_Clock_Second);
			if (time_high != clock_second_2)
				time_low	= *(unsigned long *)(MV_ICC_EXTRA_ADDR_def_KernVirt + MV_ADDR_system_Clock_Timer);
					
			hardware_timer	= TIMERx_GetCurrentValue(TIMER_SYSTEM);
			
			// Restore interrupt
			local_irq_restore(flags);
			
			system_clock.m_Clock_Second = time_high;
			system_clock.m_Clock_Timer = time_low;
			system_clock.m_Hardware_Timer = hardware_timer;
			
			if (copy_to_user((void __user *)arg, &system_clock, sizeof(system_clock)))
				return -EFAULT;
			else
				return 0;
	 	
			break;
		}
	  default:  /* redundant, as cmd was checked against MAXNR */
		return -ENOTTY;
	}
	
	return 0;
}

int SHM_ioctl_ICCNet(struct inode *inode, struct file *filp,
                 unsigned int cmd, unsigned long arg)
{
	unsigned int pid, i;
	MV_CC_DSS_ServiceInfo_t SrvInfo;
	MV_CC_DSS_ServiceInfo_DataList_t	SrvInfoList;
	pMV_CC_DSS_ServiceInfo_DataList_t	pSrvInfoList;
	HRESULT res;
	 
	switch(cmd) {

	  case CMD_ICCNET_PID_MAP:
	  	
	  	// Map PID(linux) to PID(CC)
		if (copy_from_user(&pid, (int __user *)arg, sizeof(pid)))
			return -EFAULT;
		
//		printk(KERN_INFO "SHM_ioctl_ICCNet(CMD_ICCNET_PID_MAP), pid(linux)=%d\n", pid);
		
		// search pid table to get an unused CC pid
		spin_lock(&uiPidTable_lock);
		
		for (i=MV_CC_PROCESS_ID_MIN; i<MV_CC_PROCESS_ID_MAX; i++)
			if (uiPidTable[i] == 0)
				break;
				
		if (i>= MV_CC_PROCESS_ID_MAX)
		{
			printk(KERN_ALERT "SHM_ioctl_ICCNet(CMD_ICCNET_PID_MAP), pid table is full\n");
			return -EFAULT;
		}
		
		uiPidTable[i] = pid;
		pid = i;
		
		spin_unlock(&uiPidTable_lock);
		
//		printk(KERN_INFO "SHM_ioctl_ICCNet(CMD_ICCNET_PID_MAP), pid(CC)=%d is mapped OK.\n", pid);
		
		if (copy_to_user((void __user *)arg, &pid, sizeof(pid)))
			return -EFAULT;
		else
			return 0;
 
		break;

	case CMD_ICCNET_PID_UNMAP:
		
		// Unmap PID(linux) from PID(CC)
		if (copy_from_user(&pid, (int __user *)arg, sizeof(pid)))
			return -EFAULT;
		
//		printk(KERN_INFO "SHM_ioctl_ICCNet(CMD_ICCNET_PID_UNMAP), pid(linux)=%d\n", pid);
		
		// search pid table to unmap CC pid
		spin_lock(&uiPidTable_lock);
		
		for (i=MV_CC_PROCESS_ID_MIN; i<MV_CC_PROCESS_ID_MAX; i++)
			if (uiPidTable[i] == pid)
				break;
				
		if (i>= MV_CC_PROCESS_ID_MAX)
		{
			printk(KERN_ALERT "SHM_ioctl_ICCNet(CMD_ICCNET_PID_UNMAP), pid(%d) is not found in table\n", pid);
			return -EFAULT;
		}
		
		uiPidTable[i] = 0;
		pid = i;
		
		spin_unlock(&uiPidTable_lock);
		
//		printk(KERN_INFO "SHM_ioctl_ICCNet(CMD_ICCNET_PID_UNMAP), pid(CC)=%d is unmapped OK\n", pid);
		
		if (copy_to_user((void __user *)arg, &pid, sizeof(pid)))
			return -EFAULT;
		else
			return 0;
		break;

	  case CMD_ICCNET_DSS_REG:
	  	
	  	// DSS Reg

		if (copy_from_user(&SrvInfo, (int __user *)arg, sizeof(SrvInfo)))
			return -EFAULT;
		
//		printk(KERN_INFO "SHM_ioctl_ICCNet(CMD_ICCNET_DSS_REG), SID=0x%08x\n", SrvInfo.m_ServiceID);
		
		// call DSS API to do DSS Reg
		res = MV_CC_DSS_Reg(&SrvInfo);
		
//		printk(KERN_INFO "SHM_ioctl_ICCNet(CMD_ICCNET_DSS_REG), SID=0x%08x, res=0x%08x\n", SrvInfo.m_ServiceID, res);
		
		if (copy_to_user((void __user *)arg, &SrvInfo, sizeof(SrvInfo)))
			return -EFAULT;
		
		return res;
 
		break;

	  case CMD_ICCNET_DSS_FREE:
	  	
	  	// DSS Free

		if (copy_from_user(&SrvInfo, (int __user *)arg, sizeof(SrvInfo)))
			return -EFAULT;
		
//		printk(KERN_INFO "SHM_ioctl_ICCNet(CMD_ICCNET_DSS_FREE), SID=0x%08x\n", SrvInfo.m_ServiceID);
		
		// call DSS API to do DSS Free
		res = MV_CC_DSS_Free(&SrvInfo);
		
//		printk(KERN_INFO "SHM_ioctl_ICCNet(CMD_ICCNET_DSS_FREE), SID=0x%08x, res=0x%08x\n", SrvInfo.m_ServiceID, res);
		
		if (copy_to_user((void __user *)arg, &SrvInfo, sizeof(SrvInfo)))
			return -EFAULT;
		
		return res;
 
		break;

	  case CMD_ICCNET_DSS_INQUIRY:
	  	
	  	// DSS Inquiry

		if (copy_from_user(&SrvInfo, (int __user *)arg, sizeof(SrvInfo)))
			return -EFAULT;
		
//		printk(KERN_INFO "SHM_ioctl_ICCNet(CMD_ICCNET_DSS_INQUIRY), SID=0x%08x\n", SrvInfo.m_ServiceID);
		
		// call DSS API to do DSS Inquiry
		res = MV_CC_DSS_Inquiry(&SrvInfo);
		
//		printk(KERN_INFO "SHM_ioctl_ICCNet(CMD_ICCNET_DSS_INQUIRY), SID=0x%08x, res=0x%08x\n", SrvInfo.m_ServiceID, res);
		
		if (copy_to_user((void __user *)arg, &SrvInfo, sizeof(SrvInfo)))
			return -EFAULT;
		
		return res;
 
		break;

	  case CMD_ICCNET_DSS_GETLIST:
	  	
	  	// DSS Inquiry

		if (copy_from_user(&SrvInfoList, (int __user *)arg, sizeof(MV_CC_DSS_ServiceInfo_DataList_t)))
			return -EFAULT;
		
//		printk(KERN_INFO "SHM_ioctl_ICCNet(CMD_ICCNET_DSS_GETLIST)\n");

		if (SrvInfoList.m_ListNum != 0)
		{
			pSrvInfoList = (pMV_CC_DSS_ServiceInfo_DataList_t)MV_OSAL_Malloc(SrvInfoList.m_ListNum * sizeof(MV_CC_DSS_ServiceInfo_t) + sizeof(MV_CC_DSS_ServiceInfo_DataList_t));
			if (pSrvInfoList == NULL)
				return -EFAULT;
			
			pSrvInfoList->m_ListNum = SrvInfoList.m_ListNum;
			pSrvInfoList->m_DataNum = 0;
			
			// call DSS API to do DSS Inquiry
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
			
		return res;
 
		break;
		
	default:  /* redundant, as cmd was checked against MAXNR */

		return -ENOTTY;
	}
	
	return 0;
}



// Device uses remap_pfn_range
static struct file_operations SHM_remap_ops = {
	.open    = SHM_open,
	.release = SHM_release,
	.mmap    = SHM_remap_mmap,
	.ioctl   = SHM_ioctl,  		/* the function for NonCache Memory Control*/
	.owner   = THIS_MODULE,
};

// Device uses remap_pfn_range
static struct file_operations SHM_remap_ops_CACHE = {
	.open    = SHM_open,
	.release = SHM_release,
	.mmap    = SHM_remap_mmap_CACHE,
	.ioctl   = SHM_ioctl_CACHE,  		/* the function for Cache Memory Control*/
	.owner   = THIS_MODULE,
};

// Device uses remap_pfn_range
static struct file_operations SHM_remap_ops_ICCEXTRA = {
	.owner   = THIS_MODULE,
	.open    = SHM_open,
	.release = SHM_release,
	.ioctl   = SHM_ioctl_ICCEXTRA,
};

// Device uses remap_pfn_range
static struct file_operations SHM_remap_ops_ICC = {
	.open    = SHM_open,
	.release = SHM_release,
	.write   = MV_ICCNetWrite_fop,
	.owner   = THIS_MODULE,
	.ioctl   = SHM_ioctl_ICCNet,
};

//
// Set up the cdev structure for a device.
//
static int SHM_setup_cdev(struct cdev *dev, int major, int minor,
		struct file_operations *fops)
{
	int err;
    
	cdev_init(dev, fops);
	dev->owner = THIS_MODULE;
	dev->ops = fops;
	err = cdev_add (dev, MKDEV(major, minor), 1);

	return err;
}



/*
 * We export two simple devices.  There's no need for us to maintain any
 * special housekeeping info, so we just deal with raw cdevs.
 */
static struct cdev SHMDevs;
static struct cdev SHMDevs_CACHE;
static struct cdev SHMDevs_ICCEXTRA;
static struct cdev SHMDevs_ICC;
static struct class *SHMDevs_class;

/* a unique entry for easy extension */
static struct {
	int minor;
	char *name;
	struct cdev *cdev;
	struct file_operations *fops;
} SHM_Devlist[] = {
	{GALOIS_CC_SHM0_MINOR, GALOIS_CC_SHM0_NAME, &SHMDevs, &SHM_remap_ops},
	{GALOIS_CC_SHM1_MINOR, GALOIS_CC_SHM1_NAME, &SHMDevs_CACHE, &SHM_remap_ops_CACHE},
	{GALOIS_CC_SHM2_MINOR, GALOIS_CC_SHM2_NAME, &SHMDevs_ICCEXTRA, &SHM_remap_ops_ICCEXTRA},
	{GALOIS_CC_ICC_MINOR, GALOIS_CC_ICC_NAME, &SHMDevs_ICC, &SHM_remap_ops_ICC},
};

/*
 * Module housekeeping.
 */
static int SHM_init(void)
{
	int result;
	int i;

	printk(KERN_INFO "[Galois][MV_CC_SHM] SHM device major is %d\n", GALOIS_CC_MAJOR);	

	/* Figure out our device number. */
	result = register_chrdev_region(MKDEV(GALOIS_CC_MAJOR, 0), GALOIS_CC_MINORS, "MV_SHM");
	if (result < 0) {
		printk(KERN_ALERT "[Galois][MV_CC_SHM] unable to get SHM major %d\n", GALOIS_CC_MAJOR);
		goto err_reg_device;
	}

	/* Now set up cdevs. */
	for (i = 0; i < ARRAY_SIZE(SHM_Devlist); i++) {
		result = SHM_setup_cdev(SHM_Devlist[i].cdev, 
				GALOIS_CC_MAJOR, SHM_Devlist[i].minor, SHM_Devlist[i].fops);
		if (result) {
			printk(KERN_ALERT "[Galois][MV_CC_SHM] SHM_setup_cdev failed.\n");
			goto err_add_device;
		}
	}

	/* add SHM devices to sysfs */
	SHMDevs_class = class_create(THIS_MODULE, "MV_SHM");
	if (IS_ERR(SHMDevs_class)) {
		printk(KERN_ALERT "[Galois][MV_CC_SHM] class_create failed.\n");
		result = -ENODEV;
		goto err_add_device;
	}
	for (i = 0; i < ARRAY_SIZE(SHM_Devlist); i++) {
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,24)
		device_create(SHMDevs_class, NULL, 
				MKDEV(GALOIS_CC_MAJOR, SHM_Devlist[i].minor), SHM_Devlist[i].name);
#else
		device_create(SHMDevs_class, NULL, 
				MKDEV(GALOIS_CC_MAJOR, SHM_Devlist[i].minor), 
				NULL, SHM_Devlist[i].name);
#endif

	}
	
	printk(KERN_INFO "[Galois][MV_CC_SHM] SHM_init OK\n");

	return 0;

err_add_device:
	for (i = 0; i < ARRAY_SIZE(SHM_Devlist); i++) {
		cdev_del(SHM_Devlist[i].cdev);
	}
	unregister_chrdev_region(MKDEV(GALOIS_CC_MAJOR, 0), GALOIS_CC_MINORS);

err_reg_device:
	return result;
}


static void SHM_cleanup(void)
{
	int i;
	
	if (SHM_count != 0)
		printk(KERN_ALERT "[Galois][MV_CC_SHM] SHM_count != 0\n");

	/* del sysfs entries */	
	for (i = 0; i < ARRAY_SIZE(SHM_Devlist); i++) {
		device_destroy(SHMDevs_class, MKDEV(GALOIS_CC_MAJOR, SHM_Devlist[i].minor));
	}
	class_destroy(SHMDevs_class);

	/* del cdev */
	for (i = 0; i < ARRAY_SIZE(SHM_Devlist); i++) {
		cdev_del(SHM_Devlist[i].cdev);
	}

	unregister_chrdev_region(MKDEV(GALOIS_CC_MAJOR, 0), GALOIS_CC_MINORS);
	
	printk(KERN_INFO "[Galois][MV_CC_SHM] SHM_cleanup OK\n");
}



/************************************************************************************/

#ifdef CONFIG_PM
static int MV_CC_Module_suspend(struct platform_device *pdev,
			pm_message_t state)
{
    MV_ICCNet_netlink_exit();

	printk("[Galois][MV_CC_Mod] %s OK\n", __func__);
	return 0;
}

static int MV_CC_Module_resume(struct platform_device *pdev)
{
	int i;

	for (i=MV_CC_PROCESS_ID_START; i<MV_CC_PROCESS_ID_MAX; i++)
		uiPidTable[i] = 0;
	//uiPidTable[MV_CC_PROCESS_ID_BROADCAST] = 0;

	if (MV_ICCNet_netlink_init() != 0)
		return -ENODEV;

	printk("[Galois][MV_CC_Mod] %s OK\n", __func__);
	return 0;
}
#endif

static struct platform_driver MV_CC_Module_driver = {
#ifdef CONFIG_PM
	.suspend = MV_CC_Module_suspend,
	.resume = MV_CC_Module_resume,
#endif
	.driver = {
		.name = "MV_CC_Module",
		.owner = THIS_MODULE,
	},
};

static int __init MV_CC_Module_init(void)
{
	// clean the pid table of ICC Net
	int i;
	
	for (i=MV_CC_PROCESS_ID_START; i<MV_CC_PROCESS_ID_MAX; i++)
		uiPidTable[i] = 0;
	//uiPidTable[MV_CC_PROCESS_ID_BROADCAST] = 0;

	if (SHM_init() != 0)
		return -ENODEV;

	if (MV_ICCNet_netlink_init() != 0)
		return -ENODEV;

	platform_driver_register(&MV_CC_Module_driver);

	printk(KERN_ALERT "[Galois][MV_CC_Mod] MV_CC_Module_init OK\n");

	return 0;
}

static void __exit MV_CC_Module_exit(void)
{
    MV_ICCNet_netlink_exit();

    SHM_cleanup();

	platform_driver_unregister(&MV_CC_Module_driver);

    printk(KERN_ALERT "[Galois][MV_CC_Mod] MV_CC_Module_exit OK\n");
}

//EXPORT_SYMBOL(TestFramePrint);
//EXPORT_SYMBOL(MV_CC_MsgQICC_Post);
//EXPORT_SYMBOL(pMV_ICCNet_netlink_sock);

module_init(MV_CC_Module_init);
module_exit(MV_CC_Module_exit);
