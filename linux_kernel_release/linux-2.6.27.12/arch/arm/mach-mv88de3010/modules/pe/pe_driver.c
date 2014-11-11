////////////////////////////////////////////////////////////////////////////////
//! \file pe_driver.c
//! \brief Shared Memory driver
//!	
//! Purpose:
//!
//!
//!	Version, Date and Author :
//!		V 1.00,	         ,	Fang Bao
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
#include "linux/time.h"
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <asm/uaccess.h>

/*******************************************************************************
    Local head files
*/

#include <mach/galois_generic.h>

#include "OSAL_api.h"
#include "pic_drv.h"
#include "drmdmx.h"
#include "zspWrapper.h"

#include "api_avio_dhub.h"
#include "avioDhub.h"
#include "cinclude.h"
#include "shm_api.h"
#include "GaloisTime.h"
/*******************************************************************************
    Module Descrption
*/

MODULE_AUTHOR("marvell");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("pe module template");

/*******************************************************************************
    Module API defined
*/

#define PE_DEVICE_NAME                      "galois_pe" 
#define PE_DEVICE_TAG                       "[Galois][pe_driver] " 
#define PE_DEVICE_PATH                      ("/dev/" PE_DEVICE_NAME)

#define PE_DEVICE_PROCFILE_STATUS           "status"
#define PE_DEVICE_PROCFILE_DETAIL           "detail"

#define MV_BERLIN_CPU0                      0
#define PE_MODULE_MSG_ID_VPP                MV_GS_VPP_Serv
#define PE_MODULE_MSG_ID_VDEC               MV_GS_VDEC_Serv
#define PE_MODULE_MSG_ID_AUD                MV_GS_AUD_Serv
#define PE_MODULE_MSG_ID_ZSP                MV_GS_ZSP_Serv
#define	PE_MODULE_MSG_ID_RLE                MV_GS_RLE_Serv

#define ADSP_ZSPINT2Soc_IRQ0                0	

#define PIC_INT_DISABLE 					(0)
#define PIC_INT_ENABLE 						(1)

#define VPP_IOCTL_VBI_DMA_CFGQ 0xbeef0001
#define VPP_IOCTL_VBI_BCM_CFGQ 0xbeef0002
#define VPP_IOCTL_VDE_BCM_CFGQ 0xbeef0003
#define VPP_IOCTL_TIMING       0xbeef0004
#define VPP_IOCTL_START_BCM_TRANSACTION 0xbeef0005

static int pe_driver_open (struct inode *inode, struct file *filp);
static int pe_driver_release(struct inode *inode, struct file *filp);
int pe_driver_ioctl(struct inode *inode, struct file *filp,
                 unsigned int cmd, unsigned long arg);

/*******************************************************************************
    Macro Defined
*/

#ifdef ENABLE_DEBUG
    #define pe_debug(...)   printk(KERN_INFO PE_DEVICE_TAG __VA_ARGS__)
#else
    #define pe_debug(...)
#endif

#define pe_trace(...)      printk(KERN_ALERT PE_DEVICE_TAG __VA_ARGS__)
#define pe_error(...)      printk(KERN_ERR PE_DEVICE_TAG __VA_ARGS__)

//#define VPP_DBG

/*******************************************************************************
    Module Variable
*/

//static cc_device_t *cc_device = NULL;

static struct cdev pe_dev;
static struct class *pe_dev_class;

static struct file_operations pe_ops = {
	.open    = pe_driver_open,
	.release = pe_driver_release,
	.ioctl   = pe_driver_ioctl,
	.owner   = THIS_MODULE,
};

typedef struct VPP_DMA_INFO_T {
    UINT32 DmaAddr;
    UINT32 DmaLen;
    UINT32 cpcbID;
}VPP_DMA_INFO;

VPP_DMA_INFO dma_info[3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
VPP_DMA_INFO vbi_bcm_info[3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
VPP_DMA_INFO vde_bcm_info[3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

static struct proc_dir_entry *pe_driver_procdir;
static int vpp_cpcb0_vbi_int_cnt = 0;
static int vpp_cpcb2_vbi_int_cnt = 0;

spinlock_t bcm_spinlock;

int dma_proc[3000] = {0};
int dma_cnt = 0; 

static void pe_vpp_do_tasklet(unsigned long);
static void pe_vdec_do_tasklet(unsigned long);

static void pe_ma_do_tasklet(unsigned long);
static void pe_st_do_tasklet(unsigned long);
static void pe_spdif_do_tasklet(unsigned long);
static void pe_app_do_tasklet(unsigned long);
static void pe_zsp_do_tasklet(unsigned long);
static void pe_pg_dhub_done_tasklet(unsigned long);
static void pe_rle_do_err_tasklet(unsigned long);
static void pe_rle_do_done_tasklet(unsigned long);

DECLARE_TASKLET_DISABLED(pe_vpp_tasklet, pe_vpp_do_tasklet, 0);
DECLARE_TASKLET_DISABLED(pe_vdec_tasklet, pe_vdec_do_tasklet, 0);

DECLARE_TASKLET_DISABLED(pe_ma_tasklet, pe_ma_do_tasklet, 0);
DECLARE_TASKLET_DISABLED(pe_st_tasklet, pe_st_do_tasklet, 0);
DECLARE_TASKLET_DISABLED(pe_spdif_tasklet, pe_spdif_do_tasklet, 0);
DECLARE_TASKLET_DISABLED(pe_app_tasklet, pe_app_do_tasklet, 0);
DECLARE_TASKLET_DISABLED(pe_zsp_tasklet, pe_zsp_do_tasklet, 0);
DECLARE_TASKLET_DISABLED(pe_pg_done_tasklet, pe_pg_dhub_done_tasklet, 0);
DECLARE_TASKLET_DISABLED(pe_rle_err_tasklet, pe_rle_do_err_tasklet, 0);
DECLARE_TASKLET_DISABLED(pe_rle_done_tasklet, pe_rle_do_done_tasklet, 0);

#if (0)
#define CLEAR_FIGO_INTR    \
{(*(volatile UINT*)(MEMMAP_TSI_REG_BASE+RA_DRMDMX_figoSys+RA_FigoSys_FIGO0+BA_FigoReg_figoIntrLvl_st)) = 1;}
#define INTRA_MSG_DMX_FIGO_IRQ              0x3000
#endif

static void pe_vpp_do_tasklet (unsigned long unused)
{
    MV_CC_MSG_t msg = {0};
    UINT32 val;

    msg.m_MsgID = unused;
//    msg.m_Param1 = MV_Time_GetTIMER7(); it's not in kernel symbol temp use others

    GA_REG_WORD32_READ(0xf7e82C00+0x04 + 7*0x14, &val);
    msg.m_Param1 = DEBUG_TIMER_VALUE - val;

    MV_CC_MsgQ_PostMsgByID(PE_MODULE_MSG_ID_VPP, &msg);
//    pe_trace("VPP ISR conext irq:%d %x\n", vpp_cpcb0_vbi_int_cnt, unused);
}

static void pe_vdec_do_tasklet (unsigned long unused)
{
    MV_CC_MSG_t msg = {0};

    MV_CC_MsgQ_PostMsgByID(PE_MODULE_MSG_ID_VDEC, &msg);
    pe_trace("VDEC ISR conext irq\n");
}

static void MV_VPP_action(struct softirq_action *h)
{
    MV_CC_MSG_t msg = {0,};

    MV_CC_MsgQ_PostMsgByID(PE_MODULE_MSG_ID_VPP, &msg);
    pe_trace("ISR conext irq:%d\n", vpp_cpcb0_vbi_int_cnt);
}

DECLARE_MUTEX(vpp_sem);

static void start_vbi_bcm_transaction(int cpcbID)
{
    UINT32 cnt;
    UINT32 *ptr;

    ptr = (UINT32 *)vbi_bcm_info[cpcbID].DmaAddr;
    for (cnt = 0; cnt < vbi_bcm_info[cpcbID].DmaLen; cnt++){
        dhub_channel_write_cmd(&(VPP_dhubHandle.dhub), avioDhubChMap_vpp_BCM_R, (INT)*ptr, (INT)*(ptr + 1), 0, 0, 0, 1, 0, 0);
        ptr += 2;
    }

    /* invalid vbi_bcm_info*/
    vbi_bcm_info[cpcbID].DmaLen = 0;
}

static void start_vde_bcm_transaction(int cpcbID)
{
    UINT32 cnt;
    UINT32 *ptr;

    ptr = (UINT32 *)vde_bcm_info[cpcbID].DmaAddr;
    for (cnt = 0; cnt < vde_bcm_info[cpcbID].DmaLen; cnt++){
        dhub_channel_write_cmd(&(VPP_dhubHandle.dhub), avioDhubChMap_vpp_BCM_R, (INT)*ptr, (INT)*(ptr + 1), 0, 0, 0, 1, 0, 0);
        ptr += 2;
    }
    /* invalid vde_bcm_info*/
    vde_bcm_info[cpcbID].DmaLen = 0;
}

static void start_vbi_dma_transaction(int cpcbID)
{
    UINT32 cnt;
    UINT32 *ptr;

    ptr = (UINT32 *)dma_info[cpcbID].DmaAddr;
    for (cnt = 0; cnt < dma_info[cpcbID].DmaLen; cnt++){
        *((volatile int *)*(ptr+1)) = *ptr;
        ptr += 2;
    }
    /* invalid dma_info*/
    dma_info[cpcbID].DmaLen = 0;
}

static void pe_ma_do_tasklet (unsigned long unused)
{
    MV_CC_MSG_t msg = {0,};

    msg.m_MsgID = 1 << avioDhubChMap_ag_MA0_R;
    MV_CC_MsgQ_PostMsgByID(PE_MODULE_MSG_ID_AUD, &msg);

}

static void pe_st_do_tasklet (unsigned long unused)
{
    MV_CC_MSG_t msg = {0,};

    msg.m_MsgID = 1 << avioDhubChMap_ag_SA_R;
    MV_CC_MsgQ_PostMsgByID(PE_MODULE_MSG_ID_AUD, &msg);
}

static void pe_spdif_do_tasklet (unsigned long unused)
{
    MV_CC_MSG_t msg = {0,};

    msg.m_MsgID = 1 << avioDhubChMap_ag_SPDIF_R;
    MV_CC_MsgQ_PostMsgByID(PE_MODULE_MSG_ID_AUD, &msg);
}

static void pe_app_do_tasklet (unsigned long unused)
{
    MV_CC_MSG_t msg = {0,};

    msg.m_MsgID = 1 << avioDhubChMap_ag_APPDAT_W;
    MV_CC_MsgQ_PostMsgByID(PE_MODULE_MSG_ID_AUD, &msg);
}

static void pe_zsp_do_tasklet (unsigned long unused)
{
    MV_CC_MSG_t msg = {0,};

    msg.m_MsgID = 1 << ADSP_ZSPINT2Soc_IRQ0;
    MV_CC_MsgQ_PostMsgByID(PE_MODULE_MSG_ID_ZSP, &msg);
}

static void pe_pg_dhub_done_tasklet(unsigned long unused)
{
    MV_CC_MSG_t msg = {0,};

    msg.m_MsgID = 1 << avioDhubChMap_ag_PG_ENG_W;
    MV_CC_MsgQ_PostMsgByID(PE_MODULE_MSG_ID_RLE, &msg);
}

static void pe_rle_do_err_tasklet(unsigned long unused)
{
    MV_CC_MSG_t msg = {0,};

    msg.m_MsgID = 1 << avioDhubSemMap_ag_spu_intr0;
    MV_CC_MsgQ_PostMsgByID(PE_MODULE_MSG_ID_RLE, &msg);
}

static void pe_rle_do_done_tasklet(unsigned long unused)
{
    MV_CC_MSG_t msg = {0,};

    msg.m_MsgID = 1 << avioDhubSemMap_ag_spu_intr1;
    MV_CC_MsgQ_PostMsgByID(PE_MODULE_MSG_ID_RLE, &msg);
}

static irqreturn_t pe_devices_vpp_isr(int irq, void *dev_id)
{
    INT instat;
    HDL_semaphore *pSemHandle;

#if 0
    GA_REG_WORD32_READ(0xf7e82C00+0x04 + 7*0x14, &val);
    vpp_int_time = DEBUG_TIMER_VALUE - val;
#endif

    /* disable interrupt */
    PIC_SetPerPIC_PerVector(CPUINDEX, irq, PIC_INT_DISABLE, PIC_INT_DISABLE);

    /* VPP interrupt handling  */
    pSemHandle = dhub_semaphore(&VPP_dhubHandle.dhub);
    instat = semaphore_chk_full(pSemHandle, -1);

    if (bTST(instat, avioDhubSemMap_vpp_vppCPCB0_intr))
    {
        /* CPCB-0 interrupt */
        /* clear interrupt */
        semaphore_pop(pSemHandle, avioDhubSemMap_vpp_vppCPCB0_intr, 1);
        semaphore_clr_full(pSemHandle, avioDhubSemMap_vpp_vppCPCB0_intr);

        /* Clear the bits for CPCB0 VDE interrupt */
        if (bTST(instat, avioDhubSemMap_vpp_vppOUT4_intr)){
            semaphore_pop(pSemHandle, avioDhubSemMap_vpp_vppOUT4_intr, 1);
            semaphore_clr_full(pSemHandle, avioDhubSemMap_vpp_vppOUT4_intr);
            bCLR(instat, avioDhubSemMap_vpp_vppOUT4_intr);
        }


        start_vbi_dma_transaction(0);
        start_vbi_bcm_transaction(0);
    }

    if (bTST(instat, avioDhubSemMap_vpp_vppCPCB1_intr)){
        /* CPCB-1 interrupt */
        /* clear interrupt */
        semaphore_pop(pSemHandle, avioDhubSemMap_vpp_vppCPCB1_intr, 1);
        semaphore_clr_full(pSemHandle, avioDhubSemMap_vpp_vppCPCB1_intr);

        /* Clear the bits for CPCB1 VDE interrupt */
        if (bTST(instat, avioDhubSemMap_vpp_vppOUT5_intr)){
            semaphore_pop(pSemHandle, avioDhubSemMap_vpp_vppOUT5_intr, 1);
            semaphore_clr_full(pSemHandle, avioDhubSemMap_vpp_vppOUT5_intr);
            bCLR(instat, avioDhubSemMap_vpp_vppOUT5_intr);
        }
        start_vbi_dma_transaction(1);
        start_vbi_bcm_transaction(1);
    }
  
    if (bTST(instat, avioDhubSemMap_vpp_vppCPCB2_intr)){
        /* CPCB-2 interrupt */
        /* clear interrupt */
        semaphore_pop(pSemHandle, avioDhubSemMap_vpp_vppCPCB2_intr, 1);
        semaphore_clr_full(pSemHandle, avioDhubSemMap_vpp_vppCPCB2_intr);

        /* Clear the bits for CPCB2 VDE interrupt */
        if (bTST(instat, avioDhubSemMap_vpp_vppOUT6_intr)){
            semaphore_pop(pSemHandle, avioDhubSemMap_vpp_vppOUT6_intr, 1);
            semaphore_clr_full(pSemHandle, avioDhubSemMap_vpp_vppOUT6_intr);
            bCLR(instat, avioDhubSemMap_vpp_vppOUT6_intr);
        }

        start_vbi_dma_transaction(2);
        start_vbi_bcm_transaction(2);
    }
  
    if (bTST(instat, avioDhubSemMap_vpp_vppOUT4_intr)){
        /* CPCB-0 VDE interrupt */
        /* clear interrupt */
        semaphore_pop(pSemHandle, avioDhubSemMap_vpp_vppOUT4_intr, 1);
        semaphore_clr_full(pSemHandle, avioDhubSemMap_vpp_vppOUT4_intr);

        start_vde_bcm_transaction(0);
    }
  
    if (bTST(instat, avioDhubSemMap_vpp_vppOUT5_intr)){
        /* CPCB-1 VDE interrupt */
        /* clear interrupt */
        semaphore_pop(pSemHandle, avioDhubSemMap_vpp_vppOUT5_intr, 1);
        semaphore_clr_full(pSemHandle, avioDhubSemMap_vpp_vppOUT5_intr);

        start_vde_bcm_transaction(1);
    }
  
    if (bTST(instat, avioDhubSemMap_vpp_vppOUT6_intr)){
        /* CPCB-2 VDE interrupt */
        /* clear interrupt */
        semaphore_pop(pSemHandle, avioDhubSemMap_vpp_vppOUT6_intr, 1);
        semaphore_clr_full(pSemHandle, avioDhubSemMap_vpp_vppOUT6_intr);

        start_vde_bcm_transaction(2);
    }
  
    if (bTST(instat, avioDhubSemMap_vpp_vppOUT3_intr)){
        /* VOUT3 interrupt */
        /* clear interrupt */
        semaphore_pop(pSemHandle, avioDhubSemMap_vpp_vppOUT3_intr, 1);
        semaphore_clr_full(pSemHandle, avioDhubSemMap_vpp_vppOUT3_intr);
    }
  
    if (bTST(instat, avioDhubSemMap_vpp_CH10_intr)){
        /* HDMI audio interrupt */
        /* clear interrupt */
        semaphore_pop(pSemHandle, avioDhubSemMap_vpp_CH10_intr, 1);
        semaphore_clr_full(pSemHandle, avioDhubSemMap_vpp_CH10_intr);
    }
  
    if (bTST(instat, avioDhubSemMap_vpp_vppOUT0_intr)){
        /* VOUT0 interrupt */
        /* clear interrupt */
        semaphore_pop(pSemHandle, avioDhubSemMap_vpp_vppOUT0_intr, 1);
        semaphore_clr_full(pSemHandle, avioDhubSemMap_vpp_vppOUT0_intr);
    }
  
    if (bTST(instat, avioDhubSemMap_vpp_vppOUT1_intr)){
        /* VOUT1 interrupt */
        /* clear interrupt */
        semaphore_pop(pSemHandle, avioDhubSemMap_vpp_vppOUT1_intr, 1);
        semaphore_clr_full(pSemHandle, avioDhubSemMap_vpp_vppOUT1_intr);
    }

#ifdef VPP_DBG
    pe_trace("ISR instat:%x\n", instat);
#endif

    pe_vpp_tasklet.data = instat;    
    tasklet_hi_schedule(&pe_vpp_tasklet);

     // enable ICC RX Soft IRQ
//     raise_softirq(VPP_SOFTIRQ);
//    up(&vpp_sem);

    /* enable again */
    PIC_SetPerPIC_PerVector(CPUINDEX, irq, PIC_INT_ENABLE, PIC_INT_ENABLE);

    return IRQ_HANDLED;
}

static irqreturn_t pe_devices_vdec_isr(int irq, void *dev_id)
{
    /* disable interrupt */
    PIC_SetPerPIC_PerVector(CPUINDEX, irq, PIC_INT_DISABLE, PIC_INT_DISABLE);

    tasklet_hi_schedule(&pe_vdec_tasklet);

    /* enable again */
    PIC_SetPerPIC_PerVector(CPUINDEX, irq, PIC_INT_ENABLE, PIC_INT_ENABLE);

    return IRQ_HANDLED;
}

static irqreturn_t pe_devices_aout_isr(int irq, void *dev_id)
{
    int instat;
    UNSG32 chanId;
    HDL_semaphore *pSemHandle;

    PIC_SetPerPIC_PerVector(CPUINDEX, irq, PIC_INT_DISABLE, PIC_INT_DISABLE);

    pSemHandle = dhub_semaphore(&AG_dhubHandle.dhub);
    instat = semaphore_chk_full(pSemHandle, -1);

    for (chanId = avioDhubChMap_ag_MA0_R; chanId <= avioDhubChMap_ag_MA3_R; chanId++)
    {
        if (bTST(instat, chanId))
        {
                semaphore_pop(pSemHandle, chanId, 1);
                semaphore_clr_full(pSemHandle, chanId); // clear source

                if(chanId == avioDhubChMap_ag_MA0_R)
                {
                        tasklet_hi_schedule(&pe_ma_tasklet);
                }
        }
    }

    chanId = avioDhubChMap_ag_SA_R;
    {
        if (bTST(instat, chanId))
        {
                semaphore_pop(pSemHandle, chanId, 1);
                semaphore_clr_full(pSemHandle, chanId);
                tasklet_hi_schedule(&pe_st_tasklet);
        }
    }

    chanId = avioDhubChMap_ag_SPDIF_R;
    {
        if (bTST(instat, chanId))
        {
                semaphore_pop(pSemHandle, chanId, 1);
                semaphore_clr_full(pSemHandle, chanId);
                tasklet_hi_schedule(&pe_spdif_tasklet);
        }
    }

    chanId = avioDhubChMap_ag_APPDAT_W;
    if (bTST(instat, chanId))
    {
                semaphore_pop(pSemHandle, chanId, 1);
                semaphore_clr_full(pSemHandle, chanId);
                tasklet_hi_schedule(&pe_app_tasklet);
    }

    chanId = avioDhubChMap_ag_PG_ENG_W;
    if (bTST(instat, chanId))
    {
	semaphore_pop(pSemHandle, chanId, 1);
	semaphore_clr_full(pSemHandle, chanId);
	tasklet_hi_schedule(&pe_pg_done_tasklet);
    }

    chanId = avioDhubSemMap_ag_spu_intr0;
    if (bTST(instat, chanId))
    {
	semaphore_pop(pSemHandle, chanId, 1);
	semaphore_clr_full(pSemHandle, chanId);
	tasklet_hi_schedule(&pe_rle_err_tasklet);
    }

    chanId = avioDhubSemMap_ag_spu_intr1;
    if (bTST(instat, chanId))
    {
	semaphore_pop(pSemHandle, chanId, 1);
	semaphore_clr_full(pSemHandle, chanId);
	tasklet_hi_schedule(&pe_rle_done_tasklet);
    }

    PIC_SetPerPIC_PerVector(CPUINDEX, irq, PIC_INT_ENABLE, PIC_INT_ENABLE);

    return IRQ_HANDLED;
}

static irqreturn_t pe_devices_zsp_isr(int irq, void *dev_id)
{
    UNSG32 addr, v_id;
    T32ZspInt2Soc_status reg;

    printk("kernel : ZSP intr received!.\n");
    PIC_SetPerPIC_PerVector(CPUINDEX, irq, PIC_INT_DISABLE, PIC_INT_DISABLE);

    addr = MEMMAP_ZSP_REG_BASE + RA_ZspRegs_Int2Soc + RA_ZspInt2Soc_status;
    GA_REG_WORD32_READ(addr, &(reg.u32));

    addr = MEMMAP_ZSP_REG_BASE + RA_ZspRegs_Int2Soc + RA_ZspInt2Soc_clear;
    v_id = ADSP_ZSPINT2Soc_IRQ0;
    if ((reg.u32) & (1 << v_id)) 
    {
    	GA_REG_WORD32_WRITE(addr, v_id);
    }
    
    tasklet_hi_schedule(&pe_zsp_tasklet);	// this is audio zsp, video zsp interrupt will use another tasklet to post msg.

    PIC_SetPerPIC_PerVector(CPUINDEX, irq, PIC_INT_ENABLE, PIC_INT_ENABLE);

    return IRQ_HANDLED;
}

static int pe_device_init(unsigned int cpu_id, void *pHandle)
{
    unsigned int vec_num; 
    int err;

    vpp_cpcb0_vbi_int_cnt = 0;

    tasklet_enable(&pe_vpp_tasklet);
    tasklet_enable(&pe_vdec_tasklet);

    tasklet_enable(&pe_ma_tasklet);
    tasklet_enable(&pe_st_tasklet);
    tasklet_enable(&pe_spdif_tasklet);
    tasklet_enable(&pe_app_tasklet);
    tasklet_enable(&pe_zsp_tasklet);
    tasklet_enable(&pe_pg_done_tasklet);
    tasklet_enable(&pe_rle_err_tasklet);
    tasklet_enable(&pe_rle_done_tasklet);

//    open_softirq(VPP_SOFTIRQ, MV_VPP_action);

    /* enable VPP interrupt */
    vec_num = G_IRQ_dHubIntrAvio0;
    PIC_SetPerPIC_PerVector(cpu_id, vec_num, PIC_INT_ENABLE, PIC_INT_ENABLE);

    /* register VPP ISR */
    err = request_irq(vec_num, pe_devices_vpp_isr, IRQF_DISABLED, "pe_module_vpp", pHandle);
    if (unlikely(err < 0))
        return E_FAIL;

    /* enable VDEC interrupt */
    vec_num = IRQ_dHubIntrVpro;
    PIC_SetPerPIC_PerVector(cpu_id, vec_num, PIC_INT_ENABLE, PIC_INT_ENABLE);

    /* register VDEC ISR */
    err = request_irq(vec_num, pe_devices_vdec_isr, IRQF_DISABLED, "pe_module_vdec", pHandle);
    if (unlikely(err < 0))
        return E_FAIL;


    vec_num = G_IRQ_dHubIntrAvio1;
    PIC_SetPerPIC_PerVector(cpu_id, vec_num, PIC_INT_ENABLE, PIC_INT_ENABLE);

    err = request_irq(vec_num, pe_devices_aout_isr, IRQF_DISABLED, "pe_module_aout", pHandle);
    if (unlikely(err < 0))
        return E_FAIL;

    vec_num = G_IRQ_zspInt;
    PIC_SetPerPIC_PerVector(cpu_id, vec_num, PIC_INT_ENABLE, PIC_INT_ENABLE);
    
    err = request_irq(vec_num, pe_devices_zsp_isr, IRQF_DISABLED, "pe_module_zsp", pHandle);
    if (unlikely(err < 0))
        return E_FAIL;


    /* initialize dhub */
    DhubInitialization(cpu_id, VPP_DHUB_BASE, VPP_HBO_SRAM_BASE, &VPP_dhubHandle, VPP_config, VPP_NUM_OF_CHANNELS);
    DhubInitialization(cpu_id, AG_DHUB_BASE, AG_HBO_SRAM_BASE, &AG_dhubHandle, AG_config, AG_NUM_OF_CHANNELS);

    spin_lock_init(&bcm_spinlock);

    return S_OK;
}

static int pe_device_exit(unsigned int cpu_id, void *pHandle)
{   
    free_irq(G_IRQ_dHubIntrAvio0, pHandle);
    free_irq(IRQ_dHubIntrVpro, pHandle);
    free_irq(G_IRQ_dHubIntrAvio1, pHandle);
    free_irq(G_IRQ_zspInt, pHandle);

    tasklet_disable(&pe_vpp_tasklet);
    tasklet_disable(&pe_vdec_tasklet);

    tasklet_disable(&pe_ma_tasklet);
    tasklet_disable(&pe_st_tasklet);
    tasklet_disable(&pe_spdif_tasklet);
    tasklet_disable(&pe_app_tasklet);
    tasklet_disable(&pe_zsp_tasklet);
    tasklet_disable(&pe_pg_done_tasklet);
    tasklet_disable(&pe_rle_err_tasklet);
    tasklet_disable(&pe_rle_done_tasklet);

    return S_OK;
}

/*******************************************************************************
    Module API
*/

static int pe_driver_open (struct inode *inode, struct file *filp)
{   
//	filp->private_data = cc_device;

    pe_debug("pe_driver_open ok\n");
	
	return 0;
}

static int pe_driver_release(struct inode *inode, struct file *filp)
{   
//    filp->private_data = NULL;

    pe_debug("pe_driver_release ok\n");

	return 0;
}

int pe_driver_ioctl(struct inode *inode, struct file *filp,
                 unsigned int cmd, unsigned long arg)
{
    VPP_DMA_INFO user_dma_info;
    int irqstat, bcmbuf_info[2];

    pe_debug("pe_driver_ioctl cmd = %d\n", cmd);

    switch(cmd) {
         case VPP_IOCTL_VBI_DMA_CFGQ:
            if (copy_from_user(&user_dma_info, (void __user *)arg, sizeof(VPP_DMA_INFO)))
                return -EFAULT;

            dma_info[user_dma_info.cpcbID].DmaAddr = (UINT32)MV_SHM_GetNonCacheVirtAddr(user_dma_info.DmaAddr);
            dma_info[user_dma_info.cpcbID].DmaLen = user_dma_info.DmaLen;
            break;

         case VPP_IOCTL_VBI_BCM_CFGQ:
            if (copy_from_user(&user_dma_info, (void __user *)arg, sizeof(VPP_DMA_INFO)))
                return -EFAULT;

            vbi_bcm_info[user_dma_info.cpcbID].DmaAddr = (UINT32)MV_SHM_GetNonCacheVirtAddr(user_dma_info.DmaAddr);
            vbi_bcm_info[user_dma_info.cpcbID].DmaLen = user_dma_info.DmaLen;
            break;

         case VPP_IOCTL_VDE_BCM_CFGQ:
            if (copy_from_user(&user_dma_info, (void __user *)arg, sizeof(VPP_DMA_INFO)))
                return -EFAULT;

            vde_bcm_info[user_dma_info.cpcbID].DmaAddr = (UINT32)MV_SHM_GetNonCacheVirtAddr(user_dma_info.DmaAddr);
            vde_bcm_info[user_dma_info.cpcbID].DmaLen = user_dma_info.DmaLen;
            break;

         case VPP_IOCTL_TIMING:
	      down(&vpp_sem);
            break;

         case VPP_IOCTL_START_BCM_TRANSACTION:
            if (copy_from_user(bcmbuf_info, (void __user *)arg, 2*sizeof(int)))
                return -EFAULT;
            spin_lock_irqsave(&bcm_spinlock, irqstat);
            dhub_channel_write_cmd(&(VPP_dhubHandle.dhub), avioDhubChMap_vpp_BCM_R, bcmbuf_info[0], bcmbuf_info[1], 0, 0, 0, 1, 0, 0);
            spin_unlock_irqrestore(&bcm_spinlock, irqstat);
            break;

	  default:
            break;  
	}

	return 0;
}

int read_proc_status(char *page, char **start, off_t offset,
        int count, int *eof, void *data)
{
    int len = 0, res;
    int cnt;

    len += sprintf(page + len, "%-25s : %d %d  %x\n", "PE Module IRQ cnt", vpp_cpcb0_vbi_int_cnt, dma_cnt/3, (UINT32)__pa(dma_proc));

    for (cnt = 0; cnt < 1000; cnt++){

        len += sprintf(page + len, "%s :  %x, %x, %x\n", "DMA", dma_proc[3*cnt], dma_proc[3*cnt + 1], dma_proc[3*cnt + 2]);
    }


    pe_debug("read_proc_status OK. (%d / %d)\n", len, count);

done:

    *eof = 1;
    
    return ((count < len) ? count : len);
}

int read_proc_detail(char *page, char **start, off_t offset,
        int count, int *eof, void *data)
{
    int len = 0;

    len += sprintf(page + len, "%-25s : %d\n", "PE Module IRQ cnt", vpp_cpcb0_vbi_int_cnt);

    pe_debug("read_proc_status OK. (%d / %d)\n", len, count);

    *eof = 1;
    
    return ((count < len) ? count : len);
}


/*******************************************************************************
    Module Register API
*/

static int pe_driver_setup_cdev(struct cdev *dev, int major, int minor,
		struct file_operations *fops)
{  
    cdev_init(dev, fops);
    dev->owner = THIS_MODULE;
    dev->ops = fops;
    return cdev_add (dev, MKDEV(major, minor), 1);
}

static int __init pe_driver_init(void)
{
    int res;

    /* Figure out our device number. */
    res = register_chrdev_region(MKDEV(GALOIS_PE_MAJOR, 0), GALOIS_PE_MINORS, PE_DEVICE_NAME);
    if (res < 0) {
        pe_error("unable to get pe device major [%d]\n", GALOIS_PE_MAJOR);
        goto err_reg_device;
    }
    pe_debug("register cdev device major [%d]\n", GALOIS_PE_MAJOR);

    /* Now setup cdevs. */
    res = pe_driver_setup_cdev(&pe_dev, GALOIS_PE_MAJOR, GALOIS_PE_MINOR, &pe_ops);	
    if (res) {
        pe_error("pe_driver_setup_cdev failed.\n");
        goto err_add_device;
    }
    pe_debug("setup cdevs device minor [%d]\n", GALOIS_PE_MINOR);

    /* add PE devices to sysfs */
    pe_dev_class = class_create(THIS_MODULE, PE_DEVICE_NAME);
    if (IS_ERR(pe_dev_class)) {
        pe_error("class_create failed.\n");
        res = -ENODEV;
        goto err_add_device;
    }

    device_create(pe_dev_class, NULL, 
            MKDEV(GALOIS_PE_MAJOR, GALOIS_PE_MINOR),
            NULL, PE_DEVICE_NAME);
    pe_debug("create device sysfs [%s]\n", PE_DEVICE_NAME);

    /* create PE device*/
    res = pe_device_init(CPUINDEX, 0);
    if (res != 0)
        pe_error("pe_int_init failed !!! res = 0x%08X\n", res);

    /* create PE device proc file*/
    pe_driver_procdir = proc_mkdir(PE_DEVICE_NAME, NULL);
    pe_driver_procdir->owner = THIS_MODULE;
    create_proc_read_entry(PE_DEVICE_PROCFILE_STATUS, 0, pe_driver_procdir, read_proc_status, NULL);
    create_proc_read_entry(PE_DEVICE_PROCFILE_DETAIL, 0, pe_driver_procdir, read_proc_detail, NULL);

    pe_trace("pe_driver_init OK\n");

    return 0;

err_add_device:

    cdev_del(&pe_dev);
	
    unregister_chrdev_region(MKDEV(GALOIS_PE_MAJOR, 0), GALOIS_PE_MINORS);

err_reg_device:
    
    pe_trace("pe_driver_init failed !!! (%d)\n", res);

    return res;
}

static void __exit pe_driver_exit(void)
{
    int res;

    /* destroy PE kernel API */
    res = pe_device_exit(0, 0);
    if (res != 0)
        pe_error("pe_device_exit failed !!! res = 0x%08X\n", res);
        
    /* remove PE device proc file*/
    remove_proc_entry(PE_DEVICE_PROCFILE_DETAIL, pe_driver_procdir);
    remove_proc_entry(PE_DEVICE_PROCFILE_STATUS, pe_driver_procdir);
    remove_proc_entry(PE_DEVICE_NAME, NULL);

    /* del sysfs entries */	
    device_destroy(pe_dev_class, MKDEV(GALOIS_PE_MAJOR, GALOIS_PE_MINOR));
    pe_debug("delete device sysfs [%s]\n", PE_DEVICE_NAME);	
	
    class_destroy(pe_dev_class);

    /* del cdev */
    cdev_del(&pe_dev);

    unregister_chrdev_region(MKDEV(GALOIS_PE_MAJOR, 0), GALOIS_PE_MINORS);
    pe_debug("unregister cdev device major [%d]\n", GALOIS_PE_MAJOR);

    pe_trace("pe_driver_exit OK\n");
}

module_init(pe_driver_init);
module_exit(pe_driver_exit);


