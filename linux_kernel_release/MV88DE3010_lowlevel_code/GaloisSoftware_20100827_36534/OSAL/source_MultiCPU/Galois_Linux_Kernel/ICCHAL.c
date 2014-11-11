/*******************************************************************************
*******************************************************************************/



#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/version.h>

#include "OSAL_Local_CPU_def.h"
#include "ICCHAL.h"

#include "pic_drv.h"

typedef struct {

	int res;

} PC_CC_HW_t;


typedef PC_CC_HW_t 	MV_CC_HW_t;
typedef PC_CC_HW_t 	*pMV_CC_HW_t;

MV_CC_HW_t PCSim_HW;
pMV_CC_HW_t pPCSim_HW = &PCSim_HW;


#define GALOIS_CPU0 						(MV_OSAL_CPU_0)
#define GALOIS_CPU1 						(MV_OSAL_CPU_1)
#define CPU1TOCPU0_IRQ 						(0x1e) 			/* ? */
#define PCIETOCPU0_IRQ 						(0x1f)			/* ? */
#define INT_DISABLE_BIT 					(0)
#define INT_ENABLE_BIT 						(1)


static irqreturn_t MV_CC_Interrupt_ISR(int irq, void *dev_id)
{

	/* Ack the interrupt from CPU#1 to CPU#0 */
	PIC_SRCHOSTtoDSTHOST_write_int(GALOIS_CPU1, GALOIS_CPU0, INT_DISABLE_BIT);


	// enable ICC RX Soft IRQ
	raise_softirq(ICC_RX_SOFTIRQ);
	
	return IRQ_HANDLED;
}

static int initialize_inter_host_int(unsigned int cpu_id, MV_CC_HANDLE_HW_t *pHandle)
{
    unsigned int vec_num; 
	int err;
    
    PIC_SRCHOSTtoDSTHOST_write_int(1-cpu_id, cpu_id, INT_DISABLE_BIT);

	err = request_irq(CPU1TOCPU0_IRQ, MV_CC_Interrupt_ISR, IRQF_DISABLED, "intercpu", pHandle);
	if (unlikely(err < 0))
		return E_FAIL;

    // enable bit30 IRQ: CPU1TOCPU0
    vec_num = CPU1TOCPU0_IRQ;
    PIC_SetPerPIC_PerVector(cpu_id, vec_num, INT_ENABLE_BIT, INT_ENABLE_BIT);
	return S_OK;
}

static int exit_inter_host_int(unsigned int cpu_id, MV_CC_HANDLE_HW_t *pHandle)
{   
    free_irq(CPU1TOCPU0_IRQ, pHandle);

    return S_OK;
}

HRESULT  inline MV_CC_HW_Constructor(MV_CC_HANDLE_HW_t 	*self)
{
	*self = (MV_CC_HANDLE_HW_t *)pPCSim_HW;
	
	return S_OK;
}

HRESULT inline MV_CC_HW_Destructor(MV_CC_HANDLE_HW_t 	*self)
{
	return S_OK;
}

INT MV_CC_HAL_Init(MV_CC_HANDLE_HW_t *pHandle)
{
	INT res;

	if (pHandle == NULL)
		MV_OSAL_ErrorHandle("MV_CC_HAL_Init",E_INVALIDARG);
		
	res = MV_CC_HW_Constructor(pHandle);
	if (res != S_OK)
		MV_OSAL_ErrorHandle("MV_CC_HAL_Init MV_CC_HW_Constructor", res);	

	/* Initialize CPU#0 hardware -Yufeng*/
	res = initialize_inter_host_int(GALOIS_CPU0, pHandle);
	if (res != S_OK)
		MV_OSAL_ErrorHandle("MV_CC_HAL_Init initialize_inter_host_int", res);	
	
	return res;
}

INT MV_CC_HAL_Exit(MV_CC_HANDLE_HW_t *pHandle, UINT command)
{
	INT res;

	if (pHandle == NULL)
		MV_OSAL_ErrorHandle("MV_CC_HAL_Exit",E_INVALIDARG);

    res = exit_inter_host_int(GALOIS_CPU0, pHandle);
	if (res != S_OK)
		MV_OSAL_ErrorHandle("MV_CC_HAL_Exit exit_inter_host_int", res);	

	res = MV_CC_HW_Destructor(pHandle);
	if (res != S_OK)
		MV_OSAL_ErrorHandle("MV_CC_HAL_Init MV_CC_HW_Destructor", res);	
	
	return S_OK;
}

PVOID inline MV_CC_HAL_GetFIFOArrayAddr(MV_CC_HANDLE_HW_t *pHandle)
{
	/* This address is from core.c, reserved for CPU#0 -Yufeng*/
	return (PVOID)MV_ICC_FIFO_ADDR_def_KernVirt;
}

INT inline MV_CC_HAL_SemWait(MV_CC_HANDLE_HW_t *pHandle)
{
	printk(KERN_ALERT"[Galois]MV_CC_HAL_SemWait No function\n");

	

	return S_OK;
}

INT inline MV_CC_HAL_SemPost(MV_CC_HANDLE_HW_t *pHandle, MV_OSAL_CPUID_U8_t DstCpu)
{
	if (DstCpu != MV_OSAL_CPU_LOCAL)
	{
		/* Send interrupt to CPU#1 */
		PIC_SRCHOSTtoDSTHOST_write_int( GALOIS_CPU0, GALOIS_CPU1,  INT_ENABLE_BIT );
	}
	else
	{
		printk(KERN_ALERT"[Galois]MV_CC_HAL_SemPost self CPU failed\n");	
	}

	return S_OK;
}

MV_CC_HANDLE_HW_t inline MV_CC_HAL_GetHW(void)
{
	return 	(pPCSim_HW);
}

UINT32	MV_CC_Status = 0;

VOID MV_CC_StartStatus_Clean()
{
	MV_CC_BitClean(MV_CC_Status, MV_CC_STATUS_BIT_START);
}

VOID MV_CC_StartStatus_set()
{
	UINT32 reg_data;
	
	// Set the CC Status Flag Bit of Start
	MV_CC_BitSet(MV_CC_Status, MV_CC_STATUS_BIT_START);
	// Set the CC Data Register Start Bit
	reg_data = PIC_SRCHOSTtoDSTHOST_read_data(MV_CC_STATUS_SrcCPU, MV_CC_STATUS_DstCPU);
	MV_CC_BitSet(reg_data, MV_CC_STATUS_BIT_START);
	PIC_SRCHOSTtoDSTHOST_write_data(MV_CC_STATUS_SrcCPU, MV_CC_STATUS_DstCPU, reg_data);
}

HRESULT MV_CC_StartStatus_Check()
{
	UINT32 reg_data;
	
	//1. Check CC internal Status Flag of Start, if true return S_OK
	if (MV_CC_BitGet(MV_CC_Status, MV_CC_STATUS_BIT_START))
		return S_OK;
		
	//2. Update CC internal Status from CC Data Register
	//3. Check CC internal Status Flag of Start again, if true return S_OK
	//4. if false, return E_FAIL
	reg_data = PIC_SRCHOSTtoDSTHOST_read_data(MV_CC_STATUS_SrcCPU, MV_CC_STATUS_DstCPU);
	if (MV_CC_BitGet(reg_data, MV_CC_STATUS_BIT_START))
	{
		MV_CC_BitSet(MV_CC_Status, MV_CC_STATUS_BIT_START);
		return MV_CC_STATUS_OK;
	}
	else
		return E_FAIL;
}

