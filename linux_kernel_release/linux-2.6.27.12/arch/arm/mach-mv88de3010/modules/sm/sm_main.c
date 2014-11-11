/*
 * Modified by OnLive, Inc. Oct 2010.
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/wait.h>
#include <linux/delay.h>

#include <linux/kernel.h>   /* printk() */
#include <linux/slab.h>   /* kmalloc() */
#include <linux/fs.h>       /* everything... */
#include <linux/errno.h>    /* error codes */
#include <linux/types.h>    /* size_t */
#include <linux/irqreturn.h>
#include <linux/interrupt.h>
#include <linux/poll.h>
#include <linux/ioport.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>
#include <mach/galois_generic.h>

#include <asm/io.h>
#include "apb_gpio.h"
#include "apb_gpio_ir.h"
#include "SM_common.h"
#include "SysMgr.h"
#include "galois_hal_com.h"
#include "pic_drv.h"

//#define SM_Driver_Debug
#ifdef SM_Driver_Debug
#define SMDriverprintf(x) do {printk x;}while(0);
#else
#define SMDriverprintf(x) do {}while(0);
#endif
#define GPIO_SM_IRQ 13

#define SM_Q_PUSH( pSM_Q ) {				\
	pSM_Q->m_iWrite += SM_MSG_SIZE;			\
	if( pSM_Q->m_iWrite >= SM_MSGQ_SIZE )	\
		pSM_Q->m_iWrite -= SM_MSGQ_SIZE;	\
	pSM_Q->m_iWriteTotal += SM_MSG_SIZE; }

#define SM_Q_POP( pSM_Q ) {				\
	pSM_Q->m_iRead += SM_MSG_SIZE;			\
	if( pSM_Q->m_iRead >= SM_MSGQ_SIZE )	\
		pSM_Q->m_iRead -= SM_MSGQ_SIZE;		\
	pSM_Q->m_iReadTotal += SM_MSG_SIZE; }

static MV_SM_MsgQ *Dispatch_Pool;

static unsigned int bsm_dev_id = 0x64143702;

typedef struct
{
	INT32 m_iModuleID;
	bool m_bWaitQueue;
	wait_queue_head_t * m_pQueue;
	int m_bMsgRev;
} SM_Module_Info;

SM_Module_Info SMModules[MAX_MSG_TYPE] = 
{
	{ MV_SM_ID_SYS, false, NULL,0 },
	{ MV_SM_ID_COMM, false, NULL,0 },
	{ MV_SM_ID_IR, false, NULL,0 },
	{ MV_SM_ID_KEY, false, NULL,0 },
	{ MV_SM_ID_POWER, false, NULL,0 },
	{ MV_SM_ID_WD, false, NULL,0 },
	{ MV_SM_ID_TEMP, false, NULL,0 },
	{ MV_SM_ID_VFD, false, NULL,0 },
	{ MV_SM_ID_SPI, false, NULL,0 },
	{ MV_SM_ID_I2C, false, NULL,0 },
	{ MV_SM_ID_UART, false, NULL,0 },
	{ MV_SM_ID_CEC, false, NULL,0 },
};

/*
 * IR key receving is in SM interrupt handler
 */
#ifdef CONFIG_MV88DE3010_IR_SM_INT
extern int girl_sm_int(void);
#endif

//#define BSM_POLLING_MODE

int bsm_msg_send( int iModuleID, void* pMsgBody, int iLen  )
{
	MV_SM_MsgQ *pSM_Q = (MV_SM_MsgQ*)(SOC_DTCM(SM_CPU0_INPUT_QUEUE_ADDR));	// CPU0->SM Queue start address
	MV_SM_Message *pSM_Msg;
	int msgdata=*((int*)pMsgBody);

	if( iModuleID < 1 || iModuleID > MAX_MSG_TYPE || iLen > SM_MSG_BODY_SIZE )
	{
		// illegal arguments
		//printk( "illegal arguments:iModuleid=%d,ilen=%d\n",iModuleID,iLen);
		return -1;
	}

	if( pSM_Q->m_iWrite < 0 || pSM_Q->m_iWrite >= SM_MSGQ_SIZE )
	{
		// illegal Queue arguments
		//printk(" illegal Queue arguments,pSM_Q->m_iWrite=%d\n",pSM_Q->m_iWrite);
		return -1;
	}

	if( pSM_Q->m_iRead == pSM_Q->m_iWrite && pSM_Q->m_iReadTotal != pSM_Q->m_iWriteTotal )
	{
		// message queue full, ignore the newest message
		//printk("message queue full:m_iRead=%d,m_iWrite=%d,m_iReadTotal=%d,m_iWriteTotal=%d\n",pSM_Q->m_iRead ,pSM_Q->m_iWrite,pSM_Q->m_iReadTotal,pSM_Q->m_iWriteTotal);
		return -1;
	}
	
	if((MV_SM_ID_POWER==iModuleID) && ((MV_SM_POWER_SYS_RESET==msgdata)||
		(MV_SM_POWER_WARMDOWN_REQUEST==msgdata)||(MV_SM_POWER_WARMDOWN_REQUEST_2==msgdata)))
	{
		printk("Linux warm down.\n");
		//local_irq_disable(); //disable CPU0 IRQ, from now on CPU#0 will not be able to run other tasks
		//local_fiq_disable();
		//PIC_SetPerPIC_TgtGIntE(0, INT_DISABLE_BIT, INT_DISABLE_BIT);
		/* 
		 * tell the other CPU about the emergent event
		 * send deadbeef to CPU#1
		 */
		PIC_SRCHOSTtoDSTHOST_write_data(0, 1, 0xdeadbeef);
		PIC_SRCHOSTtoDSTHOST_write_data(1, 0, 0xdeadbeef);
		PIC_SRCHOSTtoDSTHOST_write_data(0, 2, 0xdeadbeef);
		PIC_SRCHOSTtoDSTHOST_write_data(2, 0, 0xdeadbeef);
		PIC_SRCHOSTtoDSTHOST_write_int(0, 1, 1); /* send interrupt from CPU0 to CPU1 */

		mdelay(50);  //wait for 50ms to make sure that NAND transaction is done
	}
	
	pSM_Msg = (MV_SM_Message*)(&(pSM_Q->m_Queue[pSM_Q->m_iWrite]));
	pSM_Msg->m_iModuleID = iModuleID;
	pSM_Msg->m_iMsgLen = iLen;
	memcpy( pSM_Msg->m_pucMsgBody, pMsgBody, iLen );	
//	SMDriverprintf(("bsm msg send:id=%d,msg=%d\n",iModuleID,msgdata));
	SM_Q_PUSH( pSM_Q );
	
//	if((MV_SM_ID_POWER==iModuleID) && ((MV_SM_POWER_SYS_RESET==msgdata)
//		||(MV_SM_POWER_WARMDOWN_REQUEST==msgdata)||(MV_SM_POWER_WARMDOWN_REQUEST_2==msgdata)))	
//	{
//		asm volatile ("1: b 1b\n"); //CPU0 goes to hell now, just wait for SM reset.
//	}

	return 0;
}

int bsm_msg_receive( int iModuleID, void* pMsgBody, int* piLen  )
{
	MV_SM_MsgQ *pDisp_Q;
	MV_SM_Message *pDisp_Msg;

	if( iModuleID < 1 || iModuleID > MAX_MSG_TYPE )
	{
		// illegal arguments
#if 0
		//excpetion handling
#else
		return -1;
#endif
	}

	pDisp_Q = &Dispatch_Pool[iModuleID-1];
	//check whether there is new message
	if( pDisp_Q->m_iReadTotal == pDisp_Q->m_iWriteTotal)
	{
		SMDriverprintf(("kernel,no msg\n"));
		return -1;
	}
	//If some keys was sent by SM, but not received by kernel, ignore these keys, and read the newest key
	while( pDisp_Q->m_iReadTotal < (pDisp_Q->m_iWriteTotal-SM_MSG_SIZE) )
	{
		SM_Q_POP( pDisp_Q );
	}
	pDisp_Msg = (MV_SM_Message*)(&(pDisp_Q->m_Queue[pDisp_Q->m_iRead]));
	SMDriverprintf(( "bsm_msg_receive ID = %d, rt = 0x%x, wt = 0x%x, key=0x%08x.\n", iModuleID, pDisp_Q->m_iReadTotal, pDisp_Q->m_iWriteTotal,*(int*)&(pDisp_Msg->m_pucMsgBody[0]) ));
	*piLen = pDisp_Msg->m_iMsgLen;
	if( *piLen > SM_MSG_BODY_SIZE ) return -1;
	memcpy( pMsgBody, pDisp_Msg->m_pucMsgBody, *piLen );
	SM_Q_POP( pDisp_Q );
	return 0;
}

static int bsm_msg_dispatch( void )
{
	MV_SM_MsgQ *pSM_Q = (MV_SM_MsgQ*)(SOC_DTCM(SM_CPU0_OUTPUT_QUEUE_ADDR));	// SM->CPU0 Queue start address
	MV_SM_MsgQ *pDisp_Q;
	MV_SM_Message *pSM_Msg, *pDisp_Msg;
	int iMsgTypeIndex;

	if( pSM_Q->m_iRead < 0 || pSM_Q->m_iRead >= SM_MSGQ_SIZE || pSM_Q->m_iReadTotal > pSM_Q->m_iWriteTotal )
	{
		// illegal Queue arguments
#if 0
		//excpetion handling
#else
		return -1;
#endif
	}

	// if buffer was overflow written, only the last messages are saved in queue,
	// move read pointer into the same position of write pointer and keep buffer full.
	if( pSM_Q->m_iWriteTotal - pSM_Q->m_iReadTotal > SM_MSGQ_SIZE )
	{
		int iTotalDataSize = pSM_Q->m_iWriteTotal - pSM_Q->m_iReadTotal;

		pSM_Q->m_iReadTotal += iTotalDataSize - SM_MSGQ_SIZE;
		pSM_Q->m_iRead += iTotalDataSize % SM_MSGQ_SIZE;
		if( pSM_Q->m_iRead >= SM_MSGQ_SIZE )
			pSM_Q->m_iRead -= SM_MSGQ_SIZE;
		//printk("pSM_Q->m_iWriteTotal - pSM_Q->m_iReadTotal > SM_MSGQ_SIZE\n");
	}

	//printk( "Check SM msg Queue 0x%x, r=0x%x,w=0x%x,rtotal = 0x%x, wtotal = 0x%x.\n", (int)pSM_Q,pSM_Q->m_iRead,pSM_Q->m_iWrite,pSM_Q->m_iReadTotal, pSM_Q->m_iWriteTotal );
	while( pSM_Q->m_iReadTotal < pSM_Q->m_iWriteTotal )
	{
		pSM_Msg = (MV_SM_Message*)(&(pSM_Q->m_Queue[pSM_Q->m_iRead]));
		for( iMsgTypeIndex = 1 ; iMsgTypeIndex <= MAX_MSG_TYPE ; iMsgTypeIndex ++ )
		{
			//if( pSM_Msg->m_iModuleID == Msg_Type_List[iMsgTypeIndex].iModuleID )
			if( pSM_Msg->m_iModuleID == iMsgTypeIndex )
			{
				// move data from SM Queue to Dispatch queue
				pDisp_Q = &Dispatch_Pool[iMsgTypeIndex-1];
				pDisp_Msg = (MV_SM_Message*)(&(pDisp_Q->m_Queue[pDisp_Q->m_iWrite]));

				if( pDisp_Q->m_iRead == pDisp_Q->m_iWrite && pDisp_Q->m_iReadTotal != pDisp_Q->m_iWriteTotal )
				{
					//Dispatch Queue full, do nothing now, the newest message will be lost
					//printk( "Dispatch Queue[%d] full, rp = 0x%x, wp = 0x%x, rt = 0x%x, wt = 0x%x.\n", iMsgTypeIndex,pDisp_Q->m_iRead, pDisp_Q->m_iWrite, pDisp_Q->m_iReadTotal, pDisp_Q->m_iWriteTotal );
				}
				else
				{
					memcpy( pDisp_Msg, pSM_Msg, sizeof(MV_SM_Message) );
					SMDriverprintf(("Received SM msg ID %d, Len %d.\n", pSM_Msg->m_iModuleID, pSM_Msg->m_iMsgLen ));
					//printk( "Dispatch Queue[%d] status, rp = 0x%x, wp = 0x%x, rt = 0x%x, wt = 0x%x.\n", iMsgTypeIndex,
					//  pDisp_Q->m_iRead, pDisp_Q->m_iWrite, pDisp_Q->m_iReadTotal, pDisp_Q->m_iWriteTotal );
					//printk( "dispatch:Received SM msg key=0x%08x;disp Received data key=0x%08x\n", *(int*)&(pSM_Msg->m_pucMsgBody[0]),*(int*)&(pDisp_Msg->m_pucMsgBody[0]) );
					SM_Q_PUSH( pDisp_Q );
				}
				if(pSM_Msg->m_iModuleID==MV_SM_ID_IR)
				{
	#ifdef CONFIG_MV88DE3010_IR_SM_INT	
    				girl_sm_int();
	#endif		
				}
				if(SMModules[iMsgTypeIndex-1].m_bWaitQueue)
				{
					wake_up_interruptible(SMModules[iMsgTypeIndex-1].m_pQueue);
					SMModules[iMsgTypeIndex-1].m_bMsgRev=1;
				}
				break;
			}
		}

		SM_Q_POP( pSM_Q );
	}

	return 0;
}

#ifdef BSM_POLLING_MODE
static struct task_struct *bsm_msg_dispatch_task;
static int bsm_msg_dispatch_task_entry(void *arg)
{
	while( !kthread_should_stop() )
	{
		//printk("bsm_msg_dispatch_task_entry\n");
		//set_current_state(TASK_INTERRUPTIBLE);
		bsm_msg_dispatch();

		//schedule();
		msleep(100);
	}

	return 0;
}
#else
static irqreturn_t bsm_intr( int irq, void *dev_id )
{	
	int handled=0;
	T32smSysCtl_SM_CTRL reg;
	
	//Clear the SM2SoC interrupt
	reg.u32=MV_SM_READ_REG32( SM_SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_CTRL);
#ifdef CONFIG_MV88DE3010_BERLIN_B0
	reg.uSM_CTRL_SM2SOC_SW_INTR=0;//enable interrupt
#else
	reg.uSM_CTRL_CTRL_INT=0;
#endif
	MV_SM_WRITE_REG32( SM_SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_CTRL,reg.u32);
	
	SMDriverprintf(("sm int\n"));
	bsm_msg_dispatch();
	return IRQ_RETVAL(handled);
}
#endif

//#define KERNEL_DRIVER_ONLY

static int berlin_sm_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int berlin_sm_open_dev( void )
{
	int i, ret = 0;

	Dispatch_Pool = kmalloc( sizeof(MV_SM_MsgQ) * MAX_MSG_TYPE, GFP_KERNEL );

	for( i = 0 ; i < MAX_MSG_TYPE ; i ++ )
	{
		Dispatch_Pool[i].m_iWrite = Dispatch_Pool[i].m_iRead = Dispatch_Pool[i].m_iWriteTotal = Dispatch_Pool[i].m_iReadTotal = 0;
	}

#ifdef BSM_POLLING_MODE
	/* create ir thread */
	bsm_msg_dispatch_task = kthread_create( bsm_msg_dispatch_task_entry, NULL, BERLIN_SM_NAME );
	if( IS_ERR(bsm_msg_dispatch_task) )
	{
		ret = PTR_ERR( bsm_msg_dispatch_task );
		bsm_msg_dispatch_task = NULL;
		return ret;
	}

	bsm_msg_dispatch_task->flags |= PF_NOFREEZE;
	wake_up_process( bsm_msg_dispatch_task );

#else
	/* register irq */
	ret = request_irq( GPIO_SM_IRQ, bsm_intr, IRQF_SHARED, BERLIN_SM_NAME, &bsm_dev_id );
	if( ret )
	{
		//ir_gpio_exit();
		//printk("error: failed to request_irq %d.\n", GPIO_SM_IRQ);
		return ret;
	}
//	ir_gpio_init();
	mv88de3010_open_irq( GPIO_SM_IRQ );
#endif

	return ret;
}

static int berlin_sm_release(struct inode *inode, struct file *file)
{
	return 0;
}

static int berlin_sm_release_dev( void )
{
#ifdef BSM_POLLING_MODE
	kthread_stop( bsm_msg_dispatch_task );
#else
	mv88de3010_close_irq( GPIO_SM_IRQ );
//	ir_gpio_exit();
	free_irq( GPIO_SM_IRQ, &bsm_dev_id );
#endif

	kfree( Dispatch_Pool );

	return 0;
}

static ssize_t berlin_sm_read( struct file *file, char __user * buf, size_t count, loff_t *ppos )
{
	MV_SM_Message SM_Msg;
	int iModuleID = (int)(*ppos);
	int iLen, ret=0,i;
	if(count>SM_MSG_SIZE)
		return 0;
	for(i=0;i<MAX_MSG_TYPE;i++)
	{
		if(SMModules[i].m_iModuleID==iModuleID)
		{
			if(SMModules[i].m_bWaitQueue)
			{
				SMDriverprintf(("mod %d wait queue,wait here\n",SMModules[i].m_iModuleID));
				wait_event_interruptible(*(SMModules[i].m_pQueue), SMModules[i].m_bMsgRev== 1);		
				SMModules[i].m_bMsgRev = 0;
			}
			break;
		}
	}
	
	ret = bsm_msg_receive( iModuleID, SM_Msg.m_pucMsgBody, &iLen  );
	//get no msg
	if(ret<0)
		return 0;
	SM_Msg.m_iModuleID=iModuleID;
	SM_Msg.m_iMsgLen=iLen;
	ret = copy_to_user( buf, (void *)&SM_Msg, SM_MSG_SIZE );
	//copy to user return the number of bytes that can't be copied
	if( ret )
		return 0;
	else
	{
		SMDriverprintf(("kernel msg receive succeeds:imoduleid=%d,ilen=%d,data0=%d\n",iModuleID,iLen,*((int*)SM_Msg.m_pucMsgBody)));
		return count;
	}
}

static ssize_t berlin_sm_write( struct file *file, const char __user * buf, size_t count, loff_t *ppos )
{
	MV_SM_Message SM_Msg;
	int iModuleID = (int)(*ppos);
	int ret;

//	printk( "write start Offset = %d, count = %d\n", iModuleID, count );
	if( count > SM_MSG_BODY_SIZE )
		return -1;
	ret = copy_from_user( SM_Msg.m_pucMsgBody, buf, count );
	if( ret )
	{
		//printk( "copy_from_user failed\n" );
		return ret;
	}
	ret = bsm_msg_send( iModuleID, SM_Msg.m_pucMsgBody, count );
	if( ret<0 )
	{
		//printk( "bsm_msg_send failed\n" );
		return ret;
	}
	return ret;
}

static int berlin_sm_ioctl(struct inode *inode, struct file *file, 
		unsigned int cmd, unsigned long arg)
{	
	int ret = 0,i;
	switch (cmd) {
		case SM_Enable_WaitQueue:
			for(i=0;i<MAX_MSG_TYPE;i++)
			{
				if(SMModules[i].m_iModuleID==arg)
				{
					SMModules[i].m_bWaitQueue=true;
					SMModules[i].m_pQueue=(wait_queue_head_t *)kmalloc( sizeof(wait_queue_head_t), GFP_KERNEL );
					init_waitqueue_head(SMModules[i].m_pQueue);
					break;
				}
			}		
			break;
		case SM_Disable_WaitQueue:
			for(i=0;i<MAX_MSG_TYPE;i++)
			{
				if((SMModules[i].m_iModuleID==arg))
				{
					SMModules[i].m_bWaitQueue=false;
					kfree(SMModules[i].m_pQueue);
					SMModules[i].m_pQueue=NULL;
				}
				break;
			}		
			break;
		default:
			ret = -EINVAL;
			break;
	}
	return 0;
}

static unsigned int berlin_sm_poll( struct file *file, poll_table *wait )
{
	/* 
	 * poll_wait will call wait->qproc(file, &bsm_wait, wait)
	 * i.e. __pollwait(...) to add itself into &bsm_wait queue.
	 */
	//poll_wait( file, &bsm_wait, wait );
	//if( msg_rev_flag )
	//	return ( POLLIN | POLLRDNORM );
	return 0;
}

static struct file_operations berlin_sm_fops = {
	.owner		= THIS_MODULE,
	.open		= berlin_sm_open,
	.release	= berlin_sm_release,
	.write		= berlin_sm_write,
	.read		= berlin_sm_read,
	.ioctl		= berlin_sm_ioctl,
	.poll		= berlin_sm_poll,
};

static struct miscdevice sm_dev = {
	.minor	= BERLIN_SM_MISC_MINOR,
	.name	= BERLIN_SM_NAME,
	.fops	= &berlin_sm_fops,
};

static int __init berlin_sm_init(void)
{
#ifndef KERNEL_DRIVER_ONLY
	if( misc_register( &sm_dev ) )
		return -ENODEV;
#endif
	berlin_sm_open_dev();
	return 0;
}

static void __exit berlin_sm_exit(void)
{
#ifndef KERNEL_DRIVER_ONLY
	misc_deregister( &sm_dev );
#endif
	berlin_sm_release_dev();
	return;
}

#ifdef CONFIG_PM
void berlin_sm_resume(void)
{
	berlin_sm_open_dev();
}
void berlin_sm_suspend(void)
{
	berlin_sm_release_dev();
}
EXPORT_SYMBOL(berlin_sm_resume);
EXPORT_SYMBOL(berlin_sm_suspend);
#endif

EXPORT_SYMBOL(bsm_msg_send);
EXPORT_SYMBOL(bsm_msg_receive);

module_init(berlin_sm_init);
module_exit(berlin_sm_exit);

MODULE_AUTHOR("Marvell-Galois");
MODULE_DESCRIPTION("Berlin System Manager Driver");
MODULE_LICENSE("GPL");
