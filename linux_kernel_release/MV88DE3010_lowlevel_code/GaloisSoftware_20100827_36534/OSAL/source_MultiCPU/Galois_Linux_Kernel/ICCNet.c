/*******************************************************************************
*******************************************************************************/


#include "com_type.h"
#include "ICCNet.h"


#include <linux/netlink.h>
#include <linux/sched.h>
#include <net/sock.h>
#include <net/netlink.h>
#include <linux/proc_fs.h>

#include <linux/kernel.h>   /* printk() */

#include "MV_netlink.h"

#include <linux/spinlock.h>

extern struct sock *pMV_ICCNet_netlink_sock;

extern unsigned int uiPidTable[MV_CC_PROCESS_ID_MAX];

static __inline__ unsigned int uiPidTable_GetLinuxPID(unsigned int x)
{	
	if (x >= MV_CC_PROCESS_ID_MAX)
		return 0;
	else
		return uiPidTable[x];		
}

INT  MV_CC_ICCNetInit(pMV_CC_ICCNet_t pHandle)
{
	int res;
	
	/* Parameter Check */
	if (pHandle == NULL)
		MV_OSAL_ErrorHandle("MV_CC_ICCNetInit", E_INVALIDARG);
	
	/* Hardware Initialize */
	res = MV_CC_HAL_Init(&(pHandle->m_pHW));
	if (res < 0)
		MV_OSAL_ErrorHandle("MV_CC_ICCNetInit MV_CC_HAL_Init", res);	
	
	MV_CC_StartStatus_Clean();
	
	if (MV_OSAL_CPU_LOCAL == MV_OSAL_CPU_MASTER)
	{
		PVOID pPtr;
		MV_CC_StartStatus_set();
		
		pPtr = MV_CC_HAL_GetFIFOArrayAddr(&(pHandle->m_pHW));
		if (pPtr == NULL)
			MV_OSAL_ErrorHandle("MV_CC_ICCNetInit MV_CC_HAL_GetFIFOArrayAddr pPtr", E_POINTER);		
				
		/* ICC FIFO Array Initialize */
		res = MV_CC_ICCFIFOArrayInit(&(pHandle->m_FIFOArray), pPtr, MV_OSAL_CPU_LOCAL);
		if (res < 0)
			MV_OSAL_ErrorHandle("MV_CC_ICCNetInit MV_CC_ICCFIFOArrayInit", res);
			
		printk(KERN_ALERT "CPU-%d Check CC StartStatus = INITED (MASTER)\n", MV_OSAL_CPU_LOCAL);
	}
	else
	{
		res = MV_CC_StartStatus_Check();
		if (res == MV_CC_STATUS_OK)
		{
			PVOID pPtr;
			pPtr = MV_CC_HAL_GetFIFOArrayAddr(&(pHandle->m_pHW));
			if (pPtr == NULL)
				MV_OSAL_ErrorHandle("MV_CC_ICCNetInit MV_CC_HAL_GetFIFOArrayAddr pPtr", E_POINTER);		
			
			/* ICC FIFO Array Initialize */
			res = MV_CC_ICCFIFOArrayInit(&(pHandle->m_FIFOArray), pPtr, MV_OSAL_CPU_LOCAL);
			if (res < 0)
				MV_OSAL_ErrorHandle("MV_CC_ICCNetInit MV_CC_ICCFIFOArrayInit", res);	
				
			printk(KERN_ALERT "CPU-%d Check CC StartStatus = INITED\n", MV_OSAL_CPU_LOCAL);
		}
		else
		if (res != S_OK)
			printk(KERN_ALERT "CPU-%d Check CC StartStatus = FALSE\n", MV_OSAL_CPU_LOCAL);
		else
			printk(KERN_ALERT "CPU-%d Check CC StartStatus OK!\n", MV_OSAL_CPU_LOCAL);
	}
	
	/* Private data */
	pHandle->m_RdCpuNow = MV_OSAL_CPU_LOCAL;
	pHandle->m_LocalCPU = MV_OSAL_CPU_LOCAL;
	
	pHandle->m_RdCount = 0;
	pHandle->m_RdEmptyCount = 0;
	
	pHandle->m_WrCount = 0;	
	pHandle->m_WrFullCount = 0;
	pHandle->m_WrSemErrCount = 0;	
	
	return S_OK;
}



INT  MV_CC_ICCNetExit(pMV_CC_ICCNet_t pHandle, UINT command)
{
	int res;
	
	/* Parameter Check */
	if (pHandle == NULL)
		MV_OSAL_ErrorHandle("MV_CC_ICCNetExit", E_INVALIDARG);
	
	MV_CC_StartStatus_Clean();
	
	/* ICC FIFO Array Destroy */
	res = MV_CC_ICCFIFOArrayDestroy(&(pHandle->m_FIFOArray));
	if (res < 0)
		MV_OSAL_ErrorHandle("MV_CC_ICCNetExit MV_CC_ICCFIFOArrayDestroy", res);	
	
	/* Hardware Exit */
	res = MV_CC_HAL_Exit(&(pHandle->m_pHW), command);
	if (res < 0)
		MV_OSAL_ErrorHandle("MV_CC_ICCNetExit MV_CC_HAL_Exit", res);		

	return S_OK;
}




static INT nextcpu(MV_OSAL_CPUID_U8_t *cpuid)
{
	if (*cpuid >= MV_OSAL_CPU_NUM)
		MV_OSAL_ErrorHandle("nextcpu", E_INVALIDARG);
		
	(*cpuid)++;
		
	if ((*cpuid) == MV_OSAL_CPU_NUM)
		(*cpuid) = 0;
		
	//printk(KERN_INFO "[MV_ICCNet] CPU = %d.\n", *cpuid);
	
	return S_OK;
}

INT MV_CC_ICCNetRead(pMV_CC_ICCNet_t pHandle, UCHAR *pDatagramBuf)
{
	int i,res;
	
	/* Parameter Check */

	res = MV_CC_StartStatus_Check();
	if (res == MV_CC_STATUS_OK)
	{
		PVOID pPtr;
		pPtr = MV_CC_HAL_GetFIFOArrayAddr(&(pHandle->m_pHW));
		if (pPtr == NULL)
			MV_OSAL_ErrorHandle("MV_CC_ICCNetRead MV_CC_HAL_GetFIFOArrayAddr pPtr", E_POINTER);		
		
		/* ICC FIFO Array Initialize */
		res = MV_CC_ICCFIFOArrayInit(&(pHandle->m_FIFOArray), pPtr, MV_OSAL_CPU_LOCAL);
		if (res < 0)
			MV_OSAL_ErrorHandle("MV_CC_ICCNetRead MV_CC_ICCFIFOArrayInit", res);	
			
		printk(KERN_ALERT "CPU-%d Check CC StartStatus = INITED\n", MV_OSAL_CPU_LOCAL);
	}
	else
	if (res != S_OK)
		return E_NOTREADY;
	
	for (i = 0; i < MV_OSAL_CPU_NUM; i++ )
	{

		/* FIFO array read in order of dstcpu*/	
		res = MV_CC_ICCFIFOArrayRead(&(pHandle->m_FIFOArray),
									 pHandle->m_RdCpuNow, 
									 pDatagramBuf);
									 
		switch (res) 
		{	
			case S_OK:
				
				pHandle->m_RdCount++;
				nextcpu(&(pHandle->m_RdCpuNow));
				
				goto MV_CC_ICCNetRead_OK;
				
				break;
				
			case E_EMPTY:

				pHandle->m_RdEmptyCount++;
				nextcpu(&(pHandle->m_RdCpuNow));
							
				break;
		
			default:
				LOGPRINTF((KERN_ALERT "MV_CC_ICCNetRead MV_CC_ICCFIFOArrayRead\n"));
				break;
		}
			
	}
	
MV_CC_ICCNetRead_OK:
				
	return res;
}


INT MV_CC_ICCNetWrite(pMV_CC_ICCNet_t pHandle, UCHAR *pDatagramBuf)
{
	int res;
	MV_OSAL_CPUID_U8_t DstCpu;
	unsigned long flags;
	
	/* Parameter Check */
	if (//(pHandle == NULL) ||
		//(pDatagramBuf ==  NULL) ||
		((DstCpu = ICCP_DstCPU(pDatagramBuf)) >= MV_OSAL_CPU_NUM))
		MV_OSAL_ErrorHandle("MV_CC_ICCNetWrite", E_INVALIDARG);

	if (DstCpu != MV_OSAL_CPU_LOCAL)
	{
		res = MV_CC_StartStatus_Check();
		if (res == MV_CC_STATUS_OK)
		{
			PVOID pPtr;
			pPtr = MV_CC_HAL_GetFIFOArrayAddr(&(pHandle->m_pHW));
			if (pPtr == NULL)
				MV_OSAL_ErrorHandle("MV_CC_ICCNetWrite MV_CC_HAL_GetFIFOArrayAddr pPtr", E_POINTER);		
			
			/* ICC FIFO Array Initialize */
			res = MV_CC_ICCFIFOArrayInit(&(pHandle->m_FIFOArray), pPtr, MV_OSAL_CPU_LOCAL);
			if (res < 0)
				MV_OSAL_ErrorHandle("MV_CC_ICCNetWrite MV_CC_ICCFIFOArrayInit", res);	
				
			printk(KERN_ALERT "CPU-%d Check CC StartStatus = INITED\n", MV_OSAL_CPU_LOCAL);
		}
		else
		if (res != S_OK)
			return E_NOTREADY;
		
	  	// Close interrupt
	  	local_irq_save(flags);
	  	
		res = MV_CC_ICCFIFOArrayWrite(&(pHandle->m_FIFOArray),
									  DstCpu, 
									  pDatagramBuf);
										  
		switch (res) 
		{	
			case S_OK:
					
				pHandle->m_WrCount++;
				
				/* interrupt to Dst CPU */
				res = MV_CC_HAL_SemPost(&(pHandle->m_pHW), DstCpu);
				if (res != S_OK)
				{
					pHandle->m_WrSemErrCount++;
				}
				break;
					
			case E_FULL:
				pHandle->m_WrFullCount++;
				break;
			
			default:
				break;
		}
		
		// Restore interrupt
		local_irq_restore(flags);
	}
	else
	{
  		struct sk_buff * skb = NULL;
  		struct nlmsghdr * nlhdr = NULL;
  		int len = MV_CC_ICCFIFO_FRAME_SIZE;
  		unsigned int dst_pid;
  				
		skb = nlmsg_new(len, GFP_KERNEL);		//Linux Kernel 2.6.20
		if (skb == NULL)
	    {
			LOGPRINTF((KERN_ALERT "[MV_ICCNet] Can't create a netlink message.\n"));
			return E_OUTOFMEMORY;
	    }
    
	    nlhdr = nlmsg_put(skb, 0, 0, 0, len, 0);
		memcpy(NLMSG_DATA(nlhdr), pDatagramBuf, len);
	    NETLINK_CB(skb).pid = 0;
	    NETLINK_CB(skb).dst_group = NETLINK_GALOIS_CC_GROUP;
	
	    dst_pid = uiPidTable_GetLinuxPID(ICCP_DstProcess(pDatagramBuf));
	    if (dst_pid == 0)
	    {
		    // set MSG_DONTWAIT for a Nonblocking io, avoid sleep in netlink_broadcast
		    // set GFP_ATOMIC for a Nonblocking io, avoid sleep in netlink_broadcast	
	        res = netlink_broadcast(pMV_ICCNet_netlink_sock, skb, 0, NETLINK_GALOIS_CC_GROUP, GFP_KERNEL);
    
		}
	    else
	    {
	    	res = netlink_unicast(pMV_ICCNet_netlink_sock, skb, dst_pid, MSG_DONTWAIT);
	    	
	    }
  		
	    res = S_OK;
	}
	
	return res;
}

