/*
 * Linux Driver for 61xx
 * Copyright (C) 2006 Marvell Technology Group Ltd.. All Rights Reserved.
 * linux_main.c
 * lily initialized on Feb. 15 2006
 *
 *  ioctl handler has been implemented.
 *  June 2006, Zax Liu < zaxl at marvell dot com >
 *
 *  implement ioctl the 2.6.11 plus way ( not rely on BKL )
 *  July 2006, Albert Ke < ake at marvell dot com >
 *
 */

#include <linux/kthread.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include <mach/galois_platform.h>
#include <mach/galois_generic.h>
#include <mach/gpio.h>
#include <linux/cdrom.h>
#include <linux/dma-mapping.h>
#include <linux/poll.h>

#include "mv_os.h"
#include "mv_include.h"

#include "linux_main.h"

#include "com_define.h" 
#include "com_type.h"
#include "mv_config.h"
#include "mv_include.h"

#define GALOIS_CHECK_POWER_CMD 1
#define GALOIS_CHECK_MODE_CMD 1
#define GALOIS_DUMP_CSS_CMD 1
//#define GALOIS_DUMP_ALL_CMD 1
//#define GALOIS_DUMP_RW_CMD 1

/*
 * Galois player playback control
 */
//int port0_read10_to_read12 = 0;
//int port1_read10_to_read12 = 0;
//static struct scsi_cmnd *timeout_cmd = NULL;
//static int timeout_cnt = 0;

/* 
 * module parameter 
 * refer to ../common/com_dbg.h 
 */
#if LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 7)
unsigned int mv_dbg_opts = 0;
module_param(mv_dbg_opts, uint, S_IRWXU | S_IRWXG);
#endif /* LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 7) */

/* TODO : try to get rid of this - A.C. */
PHBA_Extension mv_device_extension_list[MV_DEVICE_MAX_SLOT];

unsigned int mv_device_count;
static struct list_head mv_hba_ext_list;

#if 0 //GALOIS_USER_DEBUG_SATA
#define IOCTL_BUF_LEN 8192
static unsigned char ioctl_buf[IOCTL_BUF_LEN];
#endif

static int sata_status_flag = 0;
static DECLARE_WAIT_QUEUE_HEAD(mv_sata_wait);

volatile int enable_disc_ready = 0;
EXPORT_SYMBOL(enable_disc_ready );

/*
 * Some misc functions
 */
void HBA_Translate_Req_Status_To_OS_Status( PHBA_Extension pHBA, struct scsi_cmnd *pSCmd,
		PMV_Request pReq);
	
MV_BOOLEAN TranslateOSRequest( IN PHBA_Extension pHBA, IN struct scsi_cmnd * pSCmd,
		OUT PMV_Request pReq );

int galois_sata_event(unsigned int event, MV_PVOID port, MV_PVOID device)
{
	int error = 0;
	PDomain_Port pPort = (PDomain_Port)port;
	PDomain_Device pDevice = (PDomain_Device)device;

	if ((pPort == NULL) && (pDevice == NULL)) {
		error = -1;
		galois_printk("sata event error: no port, no device\n");
		goto SATA_EVENT_OUT;
	}

	galois_printk("[Berlin]sata event 0x%08x(port=%d): ", event, GALOIS_EVENT_TO_PORT(event));
	if (GALOIS_L2H_EVENT(event)) {
		switch (event & GALOIS_EVENT_L2H_MASK) {
			case GALOIS_EVENT_L2H_DRIVEINIT:
				galois_printk("drive init\n");
				pPort->port_device_status = GALOIS_DEVICE_STATUS_INIT;
				break;
			case GALOIS_EVENT_L2H_DRIVEDONE:
				galois_printk("drive done\n");
				pPort->port_device_status = GALOIS_DEVICE_STATUS_DONE;
				break;
			case GALOIS_EVENT_L2H_NOSIG:
				galois_printk("No signature\n");
				pPort->port_device_status = GALOIS_DEVICE_STATUS_DEAD;
				break;
			case GALOIS_EVENT_L2H_HOTPLUGIN:
				galois_printk("hotplug in\n");
				pPort->port_device_status = GALOIS_DEVICE_STATUS_NULL;
				break;
			case GALOIS_EVENT_L2H_HOTPLUGOUT:
				galois_printk("hotplug out\n");
				pPort->port_device_status = GALOIS_DEVICE_STATUS_DEAD;
				break;
			case GALOIS_EVENT_L2H_DEADDRIVE:
				galois_printk("dead drive\n");
				pPort->port_device_status = GALOIS_DEVICE_STATUS_DEAD;
				break;
			case GALOIS_EVENT_L2H_DRIVEHANG:
				galois_printk("drive hang\n");
				pPort->port_device_status = GALOIS_DEVICE_STATUS_HANG;
				break;
			case GALOIS_EVENT_L2H_DRIVERESET:
				galois_printk("drive reset\n");
				pPort->port_device_status = GALOIS_DEVICE_STATUS_RESET;
				break;
			case GALOIS_EVENT_L2H_REQDROP:
				galois_printk("request dropped\n");
				pPort->port_device_status = GALOIS_DEVICE_STATUS_NULL;
				break;
			case GALOIS_EVENT_L2H_REQTIMEOUT:
				galois_printk("request timeout\n");
				pPort->port_device_status = GALOIS_DEVICE_STATUS_NULL;
				break;
			case GALOIS_EVENT_L2H_BADDISC:
				galois_printk("bad disc after reset/power-cycle\n");
				pPort->port_device_status = GALOIS_DEVICE_STATUS_DEAD;
				break;
			default:
				error = -1;
				galois_printk("error L2H galois event.\n");
				break;
		}
	} else if (GALOIS_H2L_EVENT(event)) {
		if ((pPort == NULL) || (pDevice == NULL) || !(pDevice->Device_Type & DEVICE_TYPE_ATAPI)) {
			error = -1;
			galois_printk("sata event error: no port or device, or not ATAPI devicet\n");
			goto SATA_EVENT_OUT;
		}
		switch (event & GALOIS_EVENT_H2L_MASK) {
			case GALOIS_EVENT_H2L_PLAYFILE:
				galois_printk("port%d read 10->12 switch on\n", pPort->Id);
				pPort->port_read10_to_read12 = 1;
				pPort->port_device_status = GALOIS_DEVICE_STATUS_NULL;
				break;
			case GALOIS_EVENT_H2L_NOTPLAYFILE:
				galois_printk("port%d read 10->12 switch off\n", pPort->Id);
				pPort->port_read10_to_read12 = 0;
				pPort->port_device_status = GALOIS_DEVICE_STATUS_NULL;
				break;
			default:
				error = -1;
				galois_printk("error H2L galois event.\n");
				break;
		}
	} else {
		error = -1;
		galois_printk("error galois event.\n");
	}

	/* wake up the threads waiting on the event */
	if (error == 0) {
		sata_status_flag = 1;
		wake_up_interruptible(&mv_sata_wait);
	}

SATA_EVENT_OUT:
	return error;
}

/**********************************************************************
 * Character Device Interface.
 **********************************************************************/
static int mv_sata_open(struct inode *inode, struct file *file)
{
	printk("[diag]Open sata device for user operation directly.\n");
	return 0;
}

static int mv_sata_release(struct inode *inode, struct file *file)
{
	return 0;
}

static unsigned int mv_sata_poll(struct file *file, poll_table *wait)
{
	/* 
	 * poll_wait will call wait->qproc(file, &girl_wait, wait)
	 * i.e. __pollwait(...) to add itself into &girl_wait queue.
	 */
	poll_wait(file, &mv_sata_wait, wait);
	if (sata_status_flag)
		return (POLLIN | POLLRDNORM);
	return 0;

}

#if 0 //GALOIS_USER_DEBUG_SATA
#define NONNCQ_CMD_NUM 32
static void galois_sata_read(MV_PVOID extension)
{
	PHBA_Extension pHBA = (PHBA_Extension)Module_GetHBAExtension(extension);
	MV_U16 module_id = 1;
	PCore_Driver_Extension pCore;
	PDomain_Port pPort;
	MV_LPVOID portMmio;
	PDomain_Device pDevice;
	PMV_Command_Header header;
	PMV_Command_Table pCmdTable;
	PSATA_FIS_REG_H2D pFIS;
	PMV_SG_Entry pSGEntry;
	int sector_cnt;
	MV_U32 slots;
	MV_U8 tag;
	int i;

	/* HBA_GetNextModuleExtension(pHBA, &pCore) */
	pCore = pHBA->Module_Manage.resource[module_id].module_extension;
	pPort = &pCore->Ports[0];
	pDevice = &pPort->Device[0];
	portMmio = pPort->Mmio_Base;

	tag = 0;
	slots = 0;
	sector_cnt = 1;
	for (i = 0; i < NONNCQ_CMD_NUM; i++) {
		tag = i;

		header = SATA_GetCommandHeader(pPort, tag);
		pCmdTable = Port_GetCommandTable(pPort, tag);
		pFIS = (PSATA_FIS_REG_H2D)pCmdTable->FIS;

		/* setup command header */
		header->FIS_Length = FIS_REG_H2D_SIZE_IN_DWORD;
		header->Packet_Command = 0; /* ATA or ATAPI */
		header->Reset = 0;
		header->NCQ = 0;	/* NCQ or non-NCQ */
		header->PM_Port = 0;
		header->PRD_Entry_Count = 1; /* PRD count */
		header->Table_Address = pPort->Cmd_Table_DMA.low + SATA_CMD_TABLE_SIZE*tag;
		header->Table_Address_High = pPort->Cmd_Table_DMA.high;
		if (pPort->Cmd_Table_DMA.high) {
			printk("Strange high!=0!!!!!!!!!!!!!!!!!!!\n");
		}

		/* setup FIS in command table */
		pFIS->FIS_Type = SATA_FIS_TYPE_REG_H2D;
		pFIS->PM_Port = 0;
		pFIS->C = 1;
		pFIS->Command = ATA_CMD_READ_DMA_EXT;
		pFIS->Features = 0; /* don't care */
		pFIS->Device = MV_BIT(6); /* if slave, | MV_BIT(4) */
		pFIS->Control = 0; /* don't care */
		pFIS->LBA_Low = tag * sector_cnt;
		pFIS->LBA_Mid = 0x00;
		pFIS->LBA_High = 0x00;
		pFIS->Sector_Count = sector_cnt; /* sector count */
		pFIS->LBA_Low_Exp = 0;
		pFIS->LBA_Mid_Exp = 0;
		pFIS->LBA_High_Exp = 0;
		pFIS->Features_Exp = 0;
		pFIS->Sector_Count_Exp = 0;

		pSGEntry = &pCmdTable->PRD_Entry[0];		/* PRD=1 */
		pSGEntry->Base_Address = tag * (512 * sector_cnt);
		pSGEntry->Base_Address_High = 0x0;
		pSGEntry->Size = (512 * sector_cnt) - 1;	/* PRD=1 */

		slots |= (1<<tag);
	}

	/* send out command */
#if 0
	for (i = 0; i < NONNCQ_CMD_NUM; i++) {
		tag = i;
		printk("[diag]send out to non-NCQ 0x%08x slot\n", 1<<tag);
		MV_REG_WRITE_DWORD(portMmio, PORT_CMD_ISSUE, 1<<tag);
		MV_REG_READ_DWORD(portMmio, PORT_CMD_ISSUE);
	}
	printk("CMD_ISSUE:0x%08x\n", 
			MV_REG_READ_DWORD(portMmio, PORT_CMD_ISSUE));
#else
	printk("[diag]send out to non-NCQ slot 0x%08x\n", slots);
	MV_REG_WRITE_DWORD(portMmio, PORT_CMD_ISSUE, slots);
	printk("[diag]check the slots sent out: 0x%08x\n",
			MV_REG_READ_DWORD(portMmio, PORT_CMD_ISSUE));
#endif
}

#define NCQ_CMD_NUM 32
static void galois_sata_readq(MV_PVOID extension)
{
	PHBA_Extension pHBA = (PHBA_Extension)Module_GetHBAExtension(extension);
	MV_U16 module_id = 1;
	PCore_Driver_Extension pCore;
	PDomain_Port pPort;
	MV_LPVOID portMmio;
	PDomain_Device pDevice;
	PMV_Command_Header header;
	PMV_Command_Table pCmdTable;
	PSATA_FIS_REG_H2D pFIS;
	PMV_SG_Entry pSGEntry;
	MV_U8 tag;
	MV_U32 slots;
	int sector_cnt;
	int i;

	/* HBA_GetNextModuleExtension(pHBA, &pCore) */
	pCore = pHBA->Module_Manage.resource[module_id].module_extension;
	pPort = &pCore->Ports[0];
	pDevice = &pPort->Device[0];
	portMmio = pPort->Mmio_Base;

	tag = 0;
	slots = 0;
	sector_cnt = 1;
	for (i = 0; i < NCQ_CMD_NUM; i++) {
		tag = i;

		header = SATA_GetCommandHeader(pPort, tag);
		pCmdTable = Port_GetCommandTable(pPort, tag);
		pFIS = (PSATA_FIS_REG_H2D)pCmdTable->FIS;

		/* setup command header */
		header->FIS_Length = FIS_REG_H2D_SIZE_IN_DWORD;
		header->Packet_Command = 0; /* ATA */
		header->Reset = 0;
		header->NCQ = 1;	/* NCQ */
		header->PM_Port = 0;
		header->PRD_Entry_Count = 1; /* PRD count */
		header->Table_Address = pPort->Cmd_Table_DMA.low + SATA_CMD_TABLE_SIZE*tag;
		header->Table_Address_High = pPort->Cmd_Table_DMA.high;
		if (pPort->Cmd_Table_DMA.high) {
			printk("Strange high!=0!!!!!!!!!!!!!!!!!!!\n");
		}

		/* setup FIS in command table */
		pFIS->FIS_Type = SATA_FIS_TYPE_REG_H2D;
		pFIS->PM_Port = 0;
		pFIS->C = 1;
		pFIS->Command = ATA_CMD_READ_FPDMA_QUEUED; /* FPDMA */
		pFIS->Features = sector_cnt; /* sector count */
		pFIS->Features_Exp = 0;
		pFIS->Sector_Count = tag << 3;
		pFIS->Sector_Count_Exp = 0;
		pFIS->Device = MV_BIT(6); /* if slave, | MV_BIT(4) */
		pFIS->Control = 0; /* don't care */
		pFIS->LBA_Low = tag * sector_cnt; /* LBA unit: in sector */
		pFIS->LBA_Mid = 0x00;
		pFIS->LBA_High = 0x00;
		pFIS->LBA_Low_Exp = 0;
		pFIS->LBA_Mid_Exp = 0;
		pFIS->LBA_High_Exp = 0;

		pSGEntry = &pCmdTable->PRD_Entry[0];		/* PRD=1 */
		pSGEntry->Base_Address = tag * (512 * sector_cnt);
		pSGEntry->Base_Address_High = 0x0;
		pSGEntry->Size = (512 * sector_cnt) - 1;	/* PRD=1 */

		slots |= (1 << tag);
	}

	/* send out command */
#if 0
	for (i = 0; i < NCQ_CMD_NUM; i++) {
		//printk("[diag]send out to NCQ slots: 0x%08x\n", 1<<i);
		MV_REG_WRITE_DWORD(portMmio, PORT_SCR_ACT, 1<<i);
		MV_REG_READ_DWORD(portMmio, PORT_SCR_ACT);
		MV_REG_WRITE_DWORD(portMmio, PORT_CMD_ISSUE, 1<<i);
		MV_REG_READ_DWORD(portMmio, PORT_CMD_ISSUE);
	}
	printk("SCR_ACT:0x%08x, CMD_ISSUE:0x%08x\n", 
			MV_REG_READ_DWORD(portMmio, PORT_SCR_ACT),
			MV_REG_READ_DWORD(portMmio, PORT_CMD_ISSUE));
#else
	printk("[diag]send out to NCQ slots: 0x%08x\n", slots);
	MV_REG_WRITE_DWORD(portMmio, PORT_SCR_ACT, slots);
	printk("[diag]check the slots sent out: 0x%08x\n",
			MV_REG_READ_DWORD(portMmio, PORT_SCR_ACT));
	MV_REG_WRITE_DWORD(portMmio, PORT_CMD_ISSUE, slots);
	printk("[diag]check the slots sent out: 0x%08x\n",
			MV_REG_READ_DWORD(portMmio, PORT_CMD_ISSUE));
#endif
}

/*
 * Request's callback function, it release the completion lock
 */
static void ioctlcallback(MV_PVOID This, PMV_Request pReq)
{
	PHBA_Extension pHBA = (PHBA_Extension)This;

	List_Add(&pReq->Queue_Pointer, &pHBA->Free_Request);
	pHBA->Io_Count--;
	complete(&pHBA->cmpl);
}

static int mv_sata_ioctl(struct inode *inode, struct file *file, unsigned int cmd, 
		    unsigned long arg)
{
	PHBA_Extension	pHBA;
	PCore_Driver_Extension pCore;
	PDomain_Port pPort;
	PDomain_Device pDevice;
	PMV_Request    pReq = NULL;
	int error = 0;
	int ret   = 0;
	int sptdwb_size = sizeof(SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER);
	unsigned long flags;
	PSCSI_PASS_THROUGH_DIRECT_WITH_BUFFER psptdwb = NULL;
	int devid;
	int portid;
   
	pHBA = mv_device_extension_list[0];
	
	devid = (cmd >> 16) & 0xff;
	portid = (cmd >> 24) & 0xff;
	cmd = (cmd & 0xffff);

	if (cmd == 2) {
		if ( copy_to_user((void *)arg, (void *)&mv_device_count, sizeof(unsigned int)) != 0 ) {
			MV_DBG( DMSG_IOCTL, "Marvell : Get Device Number Error.\n" );
			return -EIO;
		}
		return 0;
	}

	/* non-NCQ ATA */
	if (cmd == 3) {
		galois_sata_read(pHBA);
		return 0;
	}

	/* NCQ ATA */
	if (cmd == 4) {
		galois_sata_readq(pHBA);
		return 0;
	}

	/* SATA registers info */
	if (cmd == 5) {
		PCore_Driver_Extension pCore;
		PDomain_Port pPort;
		MV_LPVOID portMmio;

		pCore = pHBA->Module_Manage.resource[1].module_extension;
		pPort = &pCore->Ports[portid];
		portMmio = pPort->Mmio_Base;

		printk("dev(controller):%d, port: %d\n"
			"0x%02x PORT_LST_ADDR:0x%08x\n"
		   	"0x%02x PORT_LST_ADDR_HI:0x%08x\n"
			"0x%02x PORT_FIS_ADDR:0x%08x\n"
			"0x%02x PORT_FIS_ADDR_HI:0x%08x\n"
			"0x%02x PORT_IRQ_STAT:0x%08x\n"
			"0x%02x PORT_IRQ_MASK:0x%08x\n"
			"0x%02x PORT_CMD:0x%08x\n"
			"0x%02x PORT_TFDATA:0x%08x\n"
			"0x%02x PORT_SIG:0x%08x\n"
			"0x%02x PORT_CMD_ISSUE:0x%08x\n"
			"0x%02x PORT_FIFO_CTL:0x%08x\n"
			"0x%02x PORT_SCR_STAT:0x%08x\n"
			"0x%02x PORT_SCR_CTL:0x%08x\n"
			"0x%02x PORT_SCR_ERR:0x%08x\n"
			"0x%02x PORT_SCR_ACT:0x%08x\n"
			"0x%02x PORT_PM_FIS_0:0x%08x\n"
			"0x%02x PORT_PM_FIS_1:0x%08x\n",
			devid, portid,
			PORT_LST_ADDR, MV_REG_READ_DWORD(portMmio, PORT_LST_ADDR),
			PORT_LST_ADDR_HI, MV_REG_READ_DWORD(portMmio, PORT_LST_ADDR_HI),
			PORT_FIS_ADDR, MV_REG_READ_DWORD(portMmio, PORT_FIS_ADDR),
			PORT_FIS_ADDR_HI, MV_REG_READ_DWORD(portMmio, PORT_FIS_ADDR_HI),
			PORT_IRQ_STAT, MV_REG_READ_DWORD(portMmio, PORT_IRQ_STAT),
			PORT_IRQ_MASK, MV_REG_READ_DWORD(portMmio, PORT_IRQ_MASK),
			PORT_CMD, MV_REG_READ_DWORD(portMmio, PORT_CMD),
			PORT_TFDATA, MV_REG_READ_DWORD(portMmio, PORT_TFDATA),
			PORT_SIG, MV_REG_READ_DWORD(portMmio, PORT_SIG),
			PORT_CMD_ISSUE, MV_REG_READ_DWORD(portMmio, PORT_CMD_ISSUE),
			PORT_FIFO_CTL, MV_REG_READ_DWORD(portMmio, PORT_FIFO_CTL),
			PORT_SCR_STAT, MV_REG_READ_DWORD(portMmio, PORT_SCR_STAT),
			PORT_SCR_CTL, MV_REG_READ_DWORD(portMmio, PORT_SCR_CTL),
			PORT_SCR_ERR, MV_REG_READ_DWORD(portMmio, PORT_SCR_ERR),
			PORT_SCR_ACT, MV_REG_READ_DWORD(portMmio, PORT_SCR_ACT),
			PORT_PM_FIS_0, MV_REG_READ_DWORD(portMmio, PORT_PM_FIS_0),
			PORT_PM_FIS_1, MV_REG_READ_DWORD(portMmio, PORT_PM_FIS_1));

		return 0;
	}

	/* port reset test*/
	if (cmd == 6) {
		pCore = pHBA->Module_Manage.resource[1].module_extension;
		pPort = &pCore->Ports[0];
		printk("Port base address 0x%08x\n", (unsigned int)pPort->Mmio_Base);
		if (SATA_PortResetDevice(pPort) == MV_TRUE)
			return 0;
		else
			return -1;
	}

	/* port reset test*/
	if (cmd == 7) {
		extern void Core_ResetChannel(MV_PVOID);

		pCore = pHBA->Module_Manage.resource[1].module_extension;
		pPort = &pCore->Ports[0];
		pDevice = &pPort->Device[0];
		printk("Port base address 0x%08x\n", (unsigned int)pPort->Mmio_Base);

		if (!SATA_ResetHost(pCore, GALOIS_SATA_ATOMIC))
			printk("[Galois]SATA_ResetHost failed.\n");
		SATA_InitHost(pCore, GALOIS_SATA_ATOMIC);
		Core_ResetChannel(pDevice);
		return 0;
	}

	/* port power off and on */
	if (cmd == 8) {
		unsigned int status;
		unsigned int old_inten, old_intsts;
		int i;

		pCore = pHBA->Module_Manage.resource[1].module_extension;
		pPort = &pCore->Ports[0];
		pDevice = &pPort->Device[0];
		printk("Port base address 0x%08x\n", (unsigned int)pPort->Mmio_Base);

		/* disable interrupt and power on off */
		MV_ERROR_PRINT("Core_ResetChannel: disable interrupt\n");
		old_inten = MV_REG_READ_DWORD(pPort->Mmio_Base, PORT_IRQ_MASK);
		MV_REG_WRITE_DWORD(pPort->Mmio_Base, PORT_IRQ_MASK, 0x0);

		/* TODO: power off and delay 2s */
		MV_ERROR_PRINT("Core_ResetChannel: power off drive(%d)\n", pPort->Id);
		mv_gpio_setup_pinmux(21);
		GPIO_PortSetInOut(21, 1); /* output */
		GPIO_PortWrite(21, 0); /* write 0 to turn off */
		HBA_DelayMillisecond(pCore, 500);

		/* clear interrupt status and re-enable interrupts */
		old_intsts = MV_REG_READ_DWORD(pPort->Mmio_Base, PORT_IRQ_STAT);
		MV_ERROR_PRINT("Core_ResetChannel: clear interrupt status(0x%08x) and enable interrupt(0x%08x)\n", old_intsts, old_inten);
		MV_REG_WRITE_DWORD(pPort->Mmio_Base, PORT_IRQ_STAT, old_intsts);
		MV_REG_WRITE_DWORD(pPort->Mmio_Base, PORT_IRQ_MASK, old_inten);

		return 0;
	}

	/* power on drive */
	if (cmd == 9) {
		unsigned int status;
		unsigned int old_inten, old_intsts;
		unsigned int signature;
		int i;

		pCore = pHBA->Module_Manage.resource[1].module_extension;
		pPort = &pCore->Ports[0];
		pDevice = &pPort->Device[0];
		printk("Port base address 0x%08x\n", (unsigned int)pPort->Mmio_Base);

		/* disable interrupt and power on off */
		MV_ERROR_PRINT("Core_ResetChannel: disable interrupt\n");
		old_inten = MV_REG_READ_DWORD(pPort->Mmio_Base, PORT_IRQ_MASK);
		MV_REG_WRITE_DWORD(pPort->Mmio_Base, PORT_IRQ_MASK, 0x0);

		/* TODO: power on and delay 1s */
		MV_ERROR_PRINT("Core_ResetChannel: power on drive(%d)\n", pPort->Id);
		GPIO_PortWrite(21, 1); /* write 1 to turn off */
		HBA_DelayMillisecond(pCore, 500);

		/* polling for the status register, wait for device ready */
		i = 0;
		while (i < HBARESET_STATUS_POLL_COUNT) {
			status = MV_REG_READ_DWORD(pPort->Mmio_Base, PORT_TFDATA);
			status &= 0xFF;
			if ((!(status & PORT_TF_STATUS_BSY)) 
					/*&& (status & PORT_TF_STATUS_DRDY)*/ 
					&& (status != 0x7F)) {
				break;
			} else {
				HBA_DelayMillisecond(pCore, 100);
			}
			i++;
		}
		if (i >= HBARESET_STATUS_POLL_COUNT)
			MV_ERROR_PRINT("Core_ResetChannel(%d after power cycle): not ready in %dms(0x%02x)\n", pPort->Id, i*100, status);

		/* clear interrupt status and re-enable interrupts */
		old_intsts = MV_REG_READ_DWORD(pPort->Mmio_Base, PORT_IRQ_STAT);
		MV_ERROR_PRINT("Core_ResetChannel: clear interrupt status(0x%08x) and enable interrupt(0x%08x)\n", old_intsts, old_inten);
		MV_REG_WRITE_DWORD(pPort->Mmio_Base, PORT_IRQ_STAT, old_intsts);
		MV_REG_WRITE_DWORD(pPort->Mmio_Base, PORT_IRQ_MASK, old_inten);

		/* read signature */
		MV_ERROR_PRINT("Core_ResetChannel: delay 500ms more before reading signature.\n");
		HBA_DelayMillisecond(pCore, 500);
		signature = MV_REG_READ_DWORD(pPort->Mmio_Base, PORT_SIG);
		MV_ERROR_PRINT("reading signature 0x%08x.\n", signature);

		return 0;
	}

	/*
	 * else ioctl cmds
	 */
	psptdwb = kmalloc(sptdwb_size, GFP_ATOMIC);
	if ( NULL == psptdwb ) 
		return -ENOMEM;

	error = copy_from_user(psptdwb, (void *)arg, sptdwb_size);
	if (error) {
		ret = -EIO;
		goto clean_psp;
	}

	if (psptdwb->sptd.DataTransferLength) {
		if ( psptdwb->sptd.DataTransferLength > IOCTL_BUF_LEN ) {
			MV_DBG(DMSG_HBA, "__MV__ not enough buf space.\n");
			ret = -ENOMEM;
			goto clean_psp;
		}
			
		psptdwb->sptd.DataBuffer = ioctl_buf;
		memset(ioctl_buf, 0, psptdwb->sptd.DataTransferLength);
		
		error = copy_from_user( psptdwb->sptd.DataBuffer,
					((PSCSI_PASS_THROUGH_DIRECT_WITH_BUFFER)arg)->sptd.DataBuffer,
					psptdwb->sptd.DataTransferLength);
		if (error) {
			ret = -EIO;
			goto clean_pspbuf;
		}
	} else {
		psptdwb->sptd.DataBuffer = NULL;
	}

	spin_lock_irqsave(&pHBA->lock, flags);
	
	/*Translate request to MV_REQUEST*/	
	pReq = List_GetFirstEntry((&pHBA->Free_Request), MV_Request, Queue_Pointer);	
	if ( NULL == pReq ) {
		ret = -ENOMEM;
		spin_unlock_irqrestore(&pHBA->lock, flags);
		goto clean_pspbuf;
	}

	pReq->Cmd_Initiator = pHBA;
	pReq->Org_Req = pReq;/*No ideas.*/
	pReq->Scsi_Status = psptdwb->sptd.ScsiStatus;
	pReq->Device_Id = psptdwb->sptd.TargetId;
	pReq->Cmd_Flag = 0;

	if (psptdwb->sptd.DataTransferLength == 0) {
		pReq->Cmd_Flag |= CMD_FLAG_NON_DATA;
	} else {
		if (SCSI_IS_READ(psptdwb->sptd.Cdb[0]))
			pReq->Cmd_Flag |= CMD_FLAG_DATA_IN;
		if ( SCSI_IS_READ(psptdwb->sptd.Cdb[0]) || SCSI_IS_WRITE(psptdwb->sptd.Cdb[0]) )
			pReq->Cmd_Flag |= CMD_FLAG_DMA;
	}

	pReq->Data_Transfer_Length = psptdwb->sptd.DataTransferLength;
	pReq->Data_Buffer = psptdwb->sptd.DataBuffer;
	pReq->Sense_Info_Buffer = psptdwb->Sense_Buffer;

	SGTable_Init(&pReq->SG_Table, 0);
	if ( psptdwb->sptd.DataTransferLength ) {
		/*GenerateSGTable(pHBA, pSCmd, pReq, &pReq->SG_Table);*/
	}
	
	memcpy(pReq->Cdb, psptdwb->sptd.Cdb, MAX_CDB_SIZE);
	pReq->Context = NULL;
	pReq->LBA.value = 0;
	pReq->Sector_Count = 0;
	pReq->Completion = ioctlcallback;

	List_Add(&pReq->Queue_Pointer, &pHBA->Waiting_Request);
	pHBA->Io_Count++;

	/*Handle Request.*/
	HBA_HandleWaitingList(pHBA);
	spin_unlock_irqrestore(&pHBA->lock, flags);

	if ( !wait_for_completion_timeout(&pHBA->cmpl, HBA_REQ_TIMER_IOCTL*HZ) ) {
		MV_DBG(DMSG_HBA, "__MV__ ioctl req timed out.\n");
	        ret = -EIO;
	        goto clean_pspbuf;
	}

	if (psptdwb->sptd.DataTransferLength) {
		error = copy_to_user(((PSCSI_PASS_THROUGH_DIRECT_WITH_BUFFER)arg)->sptd.DataBuffer,
				     psptdwb->sptd.DataBuffer,
				     psptdwb->sptd.DataTransferLength);
		if (error) {
			ret = -EIO;
			goto clean_pspbuf;
		}
	}
		
	error = copy_to_user((void*)arg, psptdwb, sptdwb_size);
	
	if (error)
		ret = -EIO;

clean_pspbuf:
/* use static buf instead.
	if (psptdwb->sptd.DataBuffer)
	kfree(psptdwb->sptd.DataBuffer);
*/
clean_psp:
	kfree(psptdwb);
	return ret;
}

#else /* GALOIS_USER_DEBUG_SATA */

/*
 * Request's callback function, it release the completion lock
 */
static void ioctlcallback(MV_PVOID This, PMV_Request pReq)
{
	PHBA_Extension pHBA = (PHBA_Extension)This;

	printk("pReq: 0x%08x, pHBA: 0x%08x\n", (unsigned int)pReq, (unsigned int)pHBA);
	List_Add(&pReq->Queue_Pointer, &pHBA->Free_Request);
	pHBA->Io_Count--;
	complete(&pHBA->cmpl);
}

static int mv_sata_ioctl(struct inode *inode, struct file *file, unsigned int cmd, 
		    unsigned long arg)
{
	PHBA_Extension	pHBA = mv_device_extension_list[0];
	PCore_Driver_Extension pCore = pHBA->Module_Manage.resource[1].module_extension;
	PDomain_Port pPort;
	PDomain_Device pDevice;

	int portid;
	int devid;
	unsigned int event;
	int err = 0;
   
	portid = (cmd >> 24) & 0xff;
	devid = (cmd >> 16) & 0xff; /* always is 0x0 */
	event = (cmd & 0xffff);

	if ((portid != 0) && (portid != 1)) {
		printk("mv_sata_ioctl: error port id\n");
		return -1;
	}
	pPort = &pCore->Ports[portid];

	if (devid != 0) {
		printk("mv_sata_ioctl: error dev id\n");
		return -1;
	}
	pDevice = &pPort->Device[devid];

	/* 
	 * handle event 
	 */
	switch (event) {
		case CDROM_SEND_PACKET: {
			struct cdrom_generic_command cgc;
			char *kbuf = NULL;
			int size;
			dma_addr_t dma_addr;
			unsigned int spruce_exec_offset = 0;
			unsigned int spruce_transfer_len = 0x100000;
			MV_PU8 spruce_transfer_buf = (MV_PU8)0x100000;
			unsigned long flags;
			PMV_Request ioctl_pReq = NULL;
			PMV_SG_Table pSGTable;

			err = -EFAULT;
			if (copy_from_user(&cgc, (void *)arg, sizeof(cgc)))
				break;
			err = 0;

			size = cgc.buflen;
			kbuf = dma_alloc_coherent(NULL, size, &dma_addr, GFP_KERNEL);
			if (!kbuf) {
				err = -ENOMEM;
				break;;
			}
			if (copy_from_user(kbuf, cgc.buffer, size)) {
				dma_free_coherent(NULL, size, kbuf, dma_addr);
				err = -EFAULT;
				break;
			}

			spin_lock_irqsave(&pHBA->lock, flags);
	
			/*Translate request to MV_REQUEST*/	
			ioctl_pReq = List_GetFirstEntry((&pHBA->Free_Request), MV_Request, Queue_Pointer);
			if ( NULL == ioctl_pReq ) {
				err = -ENOMEM;
				dma_free_coherent(NULL, size, kbuf, dma_addr);
				spin_unlock_irqrestore(&pHBA->lock, flags);
			} else {
				printk("pReq: 0x%08x, pHBA: 0x%08x\n", (unsigned int)ioctl_pReq, (unsigned int)pHBA);
				MV_ZeroMvRequest(ioctl_pReq);
				pSGTable = &ioctl_pReq->SG_Table;
				spruce_exec_offset = 0x0;
				spruce_transfer_len = size;
				spruce_transfer_buf = (MV_PU8)dma_addr;
#if 0
				if ((dma_addr_t)virt_to_phys(kbuf) != dma_addr) {
					printk("virt_to_phys address 0x%08x != dma address 0x%08x\n", (dma_addr_t)virt_to_phys(kbuf), dma_addr);
				}
#endif
				MV_ERROR_PRINT("Download Spruce Microcode(port=%d), len 0x%08x, start virt addr 0x%08x(phys 0x%08x)\n", 
						pPort->Id, spruce_transfer_len, (unsigned int)kbuf, (unsigned int)spruce_transfer_buf);

				/* Prepare set UDMA mode task */
				ioctl_pReq->Cdb[0] = SCSI_CMD_SPRUCE_DOWNLOAD_CODE;
				ioctl_pReq->Cdb[1] = 0x0;	/* reserved, should be 0*/

				ioctl_pReq->Cdb[2] = (spruce_exec_offset >> 24) & 0xFF;
				ioctl_pReq->Cdb[3] = (spruce_exec_offset >> 16) & 0xFF;
				ioctl_pReq->Cdb[4] = (spruce_exec_offset >> 8) & 0xFF;
				ioctl_pReq->Cdb[5] = spruce_exec_offset & 0xFF;

				ioctl_pReq->Cdb[6] = (spruce_transfer_len >> 24) & 0xFF;
				ioctl_pReq->Cdb[7] = (spruce_transfer_len >> 16) & 0xFF;
				ioctl_pReq->Cdb[8] = (spruce_transfer_len >> 8) & 0xFF;
				ioctl_pReq->Cdb[9] = spruce_transfer_len & 0xFF;

				ioctl_pReq->Cdb[10] = 0x0;	/* reserved */
				ioctl_pReq->Cdb[11] = 0x0;	/* control */

				ioctl_pReq->Org_Req = ioctl_pReq;/*No ideas.*/
				ioctl_pReq->Device_Id = pDevice->Id;
				ioctl_pReq->Cmd_Initiator = pHBA;
				ioctl_pReq->Data_Transfer_Length = spruce_transfer_len;
				ioctl_pReq->Data_Buffer = kbuf;
				ioctl_pReq->Scsi_Status = REQ_STATUS_PENDING;
				ioctl_pReq->Completion = ioctlcallback;
			
				/* Make SG table */
				SGTable_Init(pSGTable, 0);
				SGTable_Append(pSGTable, 
					(unsigned int)spruce_transfer_buf, 
					0,
					spruce_transfer_len
				); 

				List_Add(&ioctl_pReq->Queue_Pointer, &pHBA->Waiting_Request);
				pHBA->Io_Count++;

				/*Handle Request.*/
				if ( pHBA->State != DRIVER_STATUS_STARTED ) {
					err = -EIO;
					printk("SATA is not DRIVER_STATUS_STARTED\n");
				} else {
#if 0
					int i = 0;
					printk("0x%08x:  ", (unsigned int)kbuf);
					while (i < 128) {
						if ((i!=0) && (i%16 == 0))
							printk("\n0x%08x:  ", (unsigned int)&kbuf[i]);
						printk("%02x ", kbuf[i]);
						i++;
					}
					printk("\n\n");
#endif
					HBA_HandleWaitingList(pHBA);
				}
				spin_unlock_irqrestore(&pHBA->lock, flags);

				if ( !wait_for_completion_timeout(&pHBA->cmpl, 20*HZ) ) {
					err = -EIO;
					printk("command is not completed\n");
				}
				printk("\nATAPI command is completed\n");
#if 0
				{
					int i = 0;
					printk("0x%08x:  ", (unsigned int)kbuf);
					while (i < 128) {
						if ((i!=0) && (i%16 == 0))
							printk("\n0x%08x:  ", (unsigned int)&kbuf[i]);
						printk("%02x ", kbuf[i]);
						i++;
					}
					printk("\n\n");
				}
#endif
				dma_free_coherent(NULL, size, kbuf, dma_addr);
			}
			break;
			}
		
		case IOCTL_HOST_RESET: {
			extern void Core_ResetChannel(MV_PVOID);
			printk("Port base address 0x%08x\n", (unsigned int)pPort->Mmio_Base);
			if (!SATA_ResetHost(pCore, GALOIS_SATA_ATOMIC))
				printk("[Galois]SATA_ResetHost failed.\n");
			SATA_InitHost(pCore, GALOIS_SATA_ATOMIC);
			Core_ResetChannel(pDevice);
			printk("Host reset done.\n");
		    break;
			}

		case IOCTL_SW_RESET: {
			printk("Port base address 0x%08x\n", (unsigned int)pPort->Mmio_Base);
			SATA_SoftResetDevice(pPort, 0x0);
			printk("Soft reset done.\n");
			break;
			}

		case IOCTL_DEVICE_STATUS: {
			unsigned int status;
			int ret;
			
			ret = wait_event_interruptible(mv_sata_wait, sata_status_flag == 1);
			if (ret)
				return ret;
			sata_status_flag = 0;

			status = (pCore->Ports[1].port_device_status << 8) | pCore->Ports[0].port_device_status;
			if (copy_to_user((void __user *)arg, &status, sizeof(status)))
				return -EFAULT;
			break;
			}

		case IOCTL_DISC_READY: {
			enable_disc_ready = 1;
			printk("enable enable_disc_ready=%d\n", enable_disc_ready);
			break;
		  	}
		default: 
			if (galois_sata_event(event, pPort, pDevice) < 0) {
				err = -1;
				printk("SATA: error ioctl command.\n");
			}
			break;
	}
	
	return err;
}
#endif /* GALOIS_USER_DEBUG_SATA */

/*
 * char device operations
 */
static struct file_operations mv_sata_fops = {
	.owner   =    THIS_MODULE,
	.open    =    mv_sata_open,
	.release =    mv_sata_release,
	.ioctl   =    mv_sata_ioctl,
	.poll    =    mv_sata_poll,
};

/*
 * misc device of sata
 */
static struct miscdevice mv_sata_dev = {
	.minor  = GALOIS_SATA_MISC_MINOR,
	.name   = GALOIS_SATA_NAME,
	.fops   = &mv_sata_fops,
};

/**********************************************************************
 * Processing request and status.
 * When create a request, register a callback function to request
 * and insert the command to HBA's waiting list, then calls 
 * HBA_HandleWaitingList() to send the request to HBA and wait 
 * on a completion lock for the status returns.
 **********************************************************************/
void HBARequestCallback(
	MV_PVOID This,
	PMV_Request pReq
	)
{
	PHBA_Extension pHBA = (PHBA_Extension)This;
	struct scsi_cmnd *pSCmd = (struct scsi_cmnd *)pReq->Org_Req;

	/* Return this request to OS. */
	if (pSCmd->serial_number)
		HBA_Translate_Req_Status_To_OS_Status(pHBA, pSCmd, pReq);
	else
		galois_printk("\tCdb[%2x,%2x,%2x,%2x, %2x,%2x,%2x,%2x, %2x,%2x,%2x,%2x] timed out.\n",
				pReq->Cdb[0], pReq->Cdb[1], pReq->Cdb[2], pReq->Cdb[3], 
				pReq->Cdb[4], pReq->Cdb[5], pReq->Cdb[6], pReq->Cdb[7],
				pReq->Cdb[8], pReq->Cdb[9], pReq->Cdb[10], pReq->Cdb[11]
		);

	List_Add(&pReq->Queue_Pointer, &pHBA->Free_Request);
	pHBA->Io_Count--;
}

/*
 * Called by TranslateSCSIRequest
 */
void GenerateSGTable(
	IN PHBA_Extension pHBA,
	IN struct scsi_cmnd *SCpnt,
	IN PMV_Request pReq,
	OUT PMV_SG_Table pSGTable
	)
{
	struct scatterlist *sg, *s;
	int i, sg_count = 0;
	BUS_ADDRESS busaddr = 0;
	MV_U32 length;

#if LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 24)
	if (SCpnt->request_bufflen > (mv_scmd_host(SCpnt)->max_sectors << 9)) {
		printk("[#########]request_bufflen = %d\n", SCpnt->request_bufflen);
#else
	if (scsi_bufflen(SCpnt) > (mv_scmd_host(SCpnt)->max_sectors << 9)) {
		printk("[#########]request_bufflen = %d\n", scsi_bufflen(SCpnt));
#endif
		MV_DBG(DMSG_SCSI, "ERROR: request length exceeds "
		       "the maximum alowed value, %x %x\n",
		       pHBA->Device_Id, pHBA->Revision_Id);
	}
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 24)
	if (SCpnt->use_sg) {
		sg = (struct scatterlist *) SCpnt->request_buffer;

		if (MV_SCp(SCpnt)->mapped == 0) {
			MV_DBG(DMSG_FREQ,"__MV__ call dma_map_sg.\n");
			sg_count = dma_map_sg(NULL, sg, SCpnt->use_sg, SCpnt->sc_data_direction);
			if (sg_count != SCpnt->use_sg) {
				MV_PRINT("WARNING sg_count(%d) != SCpnt->use_sg(%d)\n",
					 sg_count, SCpnt->use_sg);
			}
			MV_SCp(SCpnt)->mapped = 1;
		}
#else /* KERNEL_VERSION > 2.6.24 */
    if (scsi_sg_count(SCpnt)) {
        sg = scsi_sglist(SCpnt);
        if(MV_SCp(SCpnt)->mapped == 0) {
            sg_count = dma_map_sg(NULL, sg, scsi_sg_count(SCpnt), SCpnt->sc_data_direction);
            if (sg_count != scsi_sg_count(SCpnt) ) {
               	MV_PRINT("WARNING sg_count(%d) != scsi_sg_count(SCpnt)(%d)\n",
                         sg_count, scsi_sg_count(SCpnt));
            }
            MV_SCp(SCpnt)->mapped = 1;
        }
#endif  /* KERNEL_VERSION > 2.6.24 */
		for_each_sg(sg, s, sg_count, i) {
			busaddr = sg_dma_address(s);
			length = sg_dma_len(s);

#ifdef GALOIS_PRD_8BYTES_ALIGN_BUG
			if (SCSI_IS_READ(pReq->Cdb[0]) || SCSI_IS_WRITE(pReq->Cdb[0])) {
				if (busaddr & 0x7) {
					printk(KERN_ERR "[BUG8B-sg]PRD entry's base address 0x%08x(length 0x%08x)\n",
							LO_BUSADDR(busaddr), length);
					busaddr &= ~(0x7);
					length += (busaddr & 0x7);
				}
				if (length & 0x7) {
					printk(KERN_ERR "[BUG8B-sg]PRD entry's length 0x%08x(base address 0x%08x)\n",
							LO_BUSADDR(busaddr), length);
					length &= ~(0x7);
					length += 0x8;
				}
			}
#endif
#if 0
			if ((pReq->Cdb[0] == 0xa4) || (pReq->Cdb[0] == 0xad) || (pReq->Cdb[0] == 0xa3)) {
				printk("[AACS]PRD entry's base address 0x%08x(length 0x%08x)\n",
							LO_BUSADDR(busaddr), length);
			}
#endif
			SGTable_Append(pSGTable, LO_BUSADDR(busaddr), HI_BUSADDR(busaddr), length);
		}
	} else {
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 24)
       if (MV_SCp(SCpnt)->mapped == 0) {
			MV_DBG(DMSG_SCSI_FREQ, "_MV_ dma_map_single for scmd.\n");

			busaddr = dma_map_single(NULL, SCpnt->request_buffer, SCpnt->request_bufflen,
					SCpnt->sc_data_direction);
			MV_SCp(SCpnt)->bus_address = busaddr;
			MV_SCp(SCpnt)->mapped = 1;
		}
       length = SCpnt->request_bufflen;
#else
       if (MV_SCp(SCpnt)->mapped == 0) {
            MV_DBG(DMSG_SCSI_FREQ, "_MV_ dma_map_single for scmd.\n");
			busaddr = dma_map_single(NULL, scsi_sglist(SCpnt), scsi_bufflen(SCpnt),
					SCpnt->sc_data_direction);
			MV_SCp(SCpnt)->bus_address = busaddr;
			MV_SCp(SCpnt)->mapped = 1;
            printk( "_MV_ dma_map_single for scmd, at %p len %d, busaddr %llu.\n",
                    scsi_sglist(SCpnt), scsi_bufflen(SCpnt), busaddr);
		}
       length = scsi_bufflen(SCpnt);
#endif

#ifdef GALOIS_PRD_8BYTES_ALIGN_BUG
		if (SCSI_IS_READ(pReq->Cdb[0]) || SCSI_IS_WRITE(pReq->Cdb[0])) {
			if (busaddr & 0x7) {
				printk(KERN_ERR "[BUG8B]PRD entry's base address 0x%08x(length 0x%08x)\n", 
						LO_BUSADDR(busaddr), length);
				busaddr &= ~(0x7);
				length += (busaddr & 0x7);
			}
			if (length & 0x7) {
				printk(KERN_ERR "[BUG8B]PRD entry's length 0x%08x(base address 0x%08x)\n", 
						LO_BUSADDR(busaddr), length);
				length &= ~(0x7);
				length += 0x8;
			}
		}
#endif
#if 0
		if ((pReq->Cdb[0] == 0xa4) || (pReq->Cdb[0] == 0xad) || (pReq->Cdb[0] == 0xa3)) {
			printk("[AACS]PRD entry's base address 0x%08x(length 0x%08x)\n", 
						LO_BUSADDR(busaddr), length);
		}
#endif
		SGTable_Append(pSGTable, LO_BUSADDR(busaddr), HI_BUSADDR(busaddr), length);
	}
}

/*
 * Translate struct scsi_cmnd structure to MV_Request format
 */
MV_BOOLEAN TranslateSCSIRequest(PHBA_Extension pHBA, struct scsi_cmnd *pSCmd, PMV_Request pReq)
{
	PCore_Driver_Extension pCore = pHBA->Module_Manage.resource[1].module_extension;
	PDomain_Port pPort = NULL;
	PDomain_Device pDevice = NULL;
	MV_U8 portId, deviceId;

	/*
	 * pReq->Device_Id = pSCmd->device->id is SATA port id for me.
	 */
	pReq->Device_Id = mv_scmd_target(pSCmd);
	portId = PATA_MapPortId(pReq->Device_Id);
	if (portId >= MAX_PORT_NUMBER)
	{
		MV_ERROR_PRINT("TranslateSCSIRequest: bad portId %d\n", portId);
		return MV_FALSE;
	}

	deviceId = PATA_MapDeviceId(pReq->Device_Id);
	pPort = &pCore->Ports[portId];
	pDevice = &pPort->Device[deviceId];

	/* Cmd_Flag */	//TBD: For Linux: Is that possible to set these flags or need read the Cdb
	pReq->Cmd_Flag = 0;

	/*
	 * Set three flags: CMD_FLAG_NON_DATA, CMD_FLAG_DATA_IN and CMD_FLAG_DMA
	 */
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 24)
	if ( pSCmd->request_bufflen==0 ) //TBD lily
#else
    if ( scsi_bufflen(pSCmd) == 0 )
#endif
	{
		pReq->Cmd_Flag |= CMD_FLAG_NON_DATA;
	}
	else
	{
		//if ( Srb->SrbFlags&SRB_FLAGS_DATA_IN )
		//	pReq->Cmd_Flag |= CMD_FLAG_DATA_IN; TBD ?? Lily
		/*We need to optimize the flags setting. Lily*/
		if(SCSI_IS_READ(pSCmd->cmnd[0]))
			pReq->Cmd_Flag |= CMD_FLAG_DATA_IN; /*NOTE!possible to result in ERROR */
		if ( SCSI_IS_READ(pSCmd->cmnd[0]) || SCSI_IS_WRITE(pSCmd->cmnd[0]) )
			pReq->Cmd_Flag |= CMD_FLAG_DMA;
	}

	pReq->Sense_Info_Buffer_Length = SCSI_SENSE_BUFFERSIZE;  //TBD
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 24)
	pReq->Data_Transfer_Length = pSCmd->request_bufflen;
#else
	pReq->Data_Transfer_Length = scsi_bufflen(pSCmd);
#endif

	//To handle some special CMDs,lily
	memset(pReq->Cdb, 0, MAX_CDB_SIZE);

	switch (pSCmd->sc_data_direction) {
	case DMA_FROM_DEVICE:
		pReq->Cmd_Flag = CMD_FLAG_DATA_IN | CMD_FLAG_DMA;	/* XXX */
		break;
	default:
		break;
	}

	/*
	 * parse commands
	 */
	switch(pSCmd->cmnd[0]){
/*
	case READ_TOC:
		pReq->Cdb[0] = READ_TOC;
		pReq->Cdb[1] = pSCmd->cmnd[1];
		pReq->Cdb[2] = pSCmd->cmnd[2];
		pReq->Cdb[6] = pSCmd->cmnd[6];
		pReq->Cdb[7] = pSCmd->cmnd[7];
		pReq->Cdb[8] = pSCmd->cmnd[8];
		break;
*/
	//case REQUEST_SENSE:
	//	break;
	case MODE_SELECT:
		pReq->Cdb[0] = MODE_SELECT_10;
		pReq->Cdb[1] = pSCmd->cmnd[1];
		pReq->Cdb[8] = pSCmd->cmnd[4];
		break;
/*		
	case FORMAT_UNIT:
		pReq->Cdb[0] = 0x24; //ATAPI opcodes
		break;
*/
/*		
	case READ_CAPACITY:
		pReq->Cdb[0] = pSCmd->cmnd[0];
		break;
*/
/*
	case TEST_UNIT_READY:	//TBD
		pReq->Cdb[0] = pSCmd->cmnd[0];
		break;
*/
	case READ_6:
		pReq->Cdb[0] = READ_10;
		pReq->Cdb[3] = pSCmd->cmnd[1]&0x1f;
		pReq->Cdb[4] = pSCmd->cmnd[2];
		pReq->Cdb[5] = pSCmd->cmnd[3];
		pReq->Cdb[8] = pSCmd->cmnd[4];
		pReq->Cdb[9] = pSCmd->cmnd[5];
		printk(KERN_WARNING "[Galois]pSCmd->cmnd[0] is READ_6, translated to READ_10\n");
		break;

	case WRITE_6:
		pReq->Cdb[0] = WRITE_10;
		pReq->Cdb[3] = pSCmd->cmnd[1]&0x1f;
		pReq->Cdb[4] = pSCmd->cmnd[2];
		pReq->Cdb[5] = pSCmd->cmnd[3];
		pReq->Cdb[8] = pSCmd->cmnd[4];
		pReq->Cdb[9] = pSCmd->cmnd[5];
		printk(KERN_WARNING "[Galois]pSCmd->cmnd[0] is WRITE_6, translated to WRITE_10\n");
		break;

	case READ_12:
		{
			extern int berlin_spruce_loader;
			if (berlin_spruce_loader == 0) {
				pReq->Cdb[0] = READ_10;
				pReq->Cdb[1] = 0;
				pReq->Cdb[2] = pSCmd->cmnd[2];
				pReq->Cdb[3] = pSCmd->cmnd[3];
				pReq->Cdb[4] = pSCmd->cmnd[4];
				pReq->Cdb[5] = pSCmd->cmnd[5];
				pReq->Cdb[6] = 0;
				pReq->Cdb[7] = pSCmd->cmnd[8];
				pReq->Cdb[8] = pSCmd->cmnd[9];
				pReq->Cdb[9] = 0x80;
			} else {
				memcpy(pReq->Cdb, pSCmd->cmnd, MAX_CDB_SIZE);
			}
			break;
		}
#if 0
	case WRITE_12:
		pReq->Cdb[0] = WRITE_10;
		pReq->Cdb[1] = pSCmd->cmnd[1];
		pReq->Cdb[2] = pSCmd->cmnd[2];
		pReq->Cdb[3] = pSCmd->cmnd[3];
		pReq->Cdb[4] = pSCmd->cmnd[4];
		pReq->Cdb[5] = pSCmd->cmnd[5];
		pReq->Cdb[7] = pSCmd->cmnd[8];
		pReq->Cdb[8] = pSCmd->cmnd[9];
		pReq->Cdb[9] = pSCmd->cmnd[11];
		printk(KERN_WARNING "[Galois]pSCmd->cmnd[0] is WRITE_12, translated to WRITE_10\n");
		break;
#endif
	default:
		memcpy(pReq->Cdb, pSCmd->cmnd, MAX_CDB_SIZE);
		break;
	}

	/*
	 * When starting playback a stream file, translate READ10 to READ12
	 */
	if ((pPort->port_read10_to_read12) && (pReq->Cdb[0] == READ_10) && (pDevice->Device_Type & DEVICE_TYPE_ATAPI)) {
		//printk(".");
		pReq->Cdb[0] = READ_12;
		pReq->Cdb[1] = 0;
		pReq->Cdb[11] = pReq->Cdb[9];
		pReq->Cdb[10] = 0x80;	/* Streaming = 1 */
		pReq->Cdb[9] = pReq->Cdb[8];
		pReq->Cdb[8] = pReq->Cdb[7];
		pReq->Cdb[7] = 0;
		pReq->Cdb[6] = 0;
	}

	/*
	 * check start-stop unit command
	 */
#ifdef GALOIS_CHECK_POWER_CMD
	if ((pReq->Cdb[0] == 0x1b) && ((pReq->Cdb[4] & 0xF0) != 0x00)) {
		int i;
		galois_printk("Ignore drive power conditions cmd: ");
		for (i = 0; i < 16; i++) {
			galois_printk("%02x ", pReq->Cdb[i]);
		}
		/* change to TEST UNIT READY command */
		memset(pReq->Cdb, 0, MAX_CDB_SIZE);
		galois_printk("\n");
	}
#endif
#ifdef GALOIS_CHECK_MODE_CMD
	if ((pDevice->Device_Type & DEVICE_TYPE_ATAPI) && 
			((pReq->Cdb[0] == 0x55) || (pReq->Cdb[0] == 0x5a) || (pReq->Cdb[0] == 0x15) || (pReq->Cdb[0] == 0x1a))) {
		int i;
		galois_printk("MODE cmd: ");
		for (i = 0; i < 16; i++) {
			galois_printk("%02x ", pReq->Cdb[i]);
		}
		galois_printk("\n");
	}
#endif

	/*
	 * print out AACS, non-RW, or RW commands
	 */
#ifdef GALOIS_DUMP_CSS_CMD
	if ((pReq->Cdb[0] == 0xa4) || (pReq->Cdb[0] == 0xad) || (pReq->Cdb[0] == 0xa3)) {
		int i;
		printk("AACS/CSS cmd: ");
		for (i = 0; i < 16; i++) {
			printk("%02x ", pReq->Cdb[i]);
		}
		printk("\n");
	}
#endif
#ifdef GALOIS_DUMP_ALL_CMD
	if (!SCSI_IS_READ(pReq->Cdb[0]) && !SCSI_IS_WRITE(pReq->Cdb[0])) {
		int i;
		printk("[target=%d send]: ", mv_scmd_target(pSCmd));
		for (i = 0; i < 16; i++) {
			printk("%02x ", pSCmd->cmnd[i]);
		}
		printk("\n");
	}
#endif
#ifdef GALOIS_DUMP_RW_CMD
	if (SCSI_IS_READ(pReq->Cdb[0]) || SCSI_IS_WRITE(pReq->Cdb[0])) {
		int i;
		printk("[target=%d send]: ", mv_scmd_target(pSCmd));
		for (i = 0; i < 16; i++) {
			printk("%02x ", pSCmd->cmnd[i]);
		}
		printk("\n");
	}
#endif
	/*
	 * remove DMA for non-READ non-WRITE commands
	 */
#ifdef GALOIS_PRD_8BYTES_ALIGN_BUG
	if ((pReq->Cmd_Flag & CMD_FLAG_DMA) && !SCSI_IS_READ(pReq->Cdb[0]) && !SCSI_IS_WRITE(pReq->Cdb[0])) {
//		int i;
		pReq->Cmd_Flag &= (~CMD_FLAG_DMA);
//		printk("[BERLIN warning: Linux requests DMA transfering non-READ/WRTE10 commands]fix to PIO mode.\n");
//		for (i = 0; i < 16; i++) {
//			printk("%02x ", pSCmd->cmnd[i]);
//		}
//		printk("\n");
	}
#endif

#if LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 24)
	if (SCSI_IS_INSTANT(pSCmd->cmnd[0]) && pSCmd->use_sg) {
		struct scatterlist *sg = (struct scatterlist *) pSCmd->request_buffer;
#else
	if (SCSI_IS_INSTANT(pSCmd->cmnd[0]) && scsi_bufflen(pSCmd) ) {
		struct scatterlist *sg = scsi_sglist(pSCmd);
#endif
		pReq->Data_Buffer = kzalloc(sg->length, GFP_ATOMIC);
		if ( NULL == pReq->Data_Buffer )
			return MV_FALSE;

		pReq->Data_Transfer_Length = sg->length;
		MV_SCp(pSCmd)->map_atomic = 1;
	} else {
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 24)
		pReq->Data_Buffer = pSCmd->request_buffer;
#else
		pReq->Data_Buffer = scsi_sglist(pSCmd);
#endif
	}

	pReq->Sense_Info_Buffer = pSCmd->sense_buffer;

	/* Init the SG table first no matter it's data command or non-data command. */
	SGTable_Init(&pReq->SG_Table, 0);

#if LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 24)
	if ( pSCmd->request_bufflen )
#else
    if ( scsi_bufflen(pSCmd) )
#endif
	{
		GenerateSGTable(pHBA, pSCmd, pReq, &pReq->SG_Table);
	}

	pReq->Org_Req = pSCmd;
	pReq->Context = NULL;

	pReq->LBA.value = 0;
	pReq->Sector_Count = 0;

	pReq->Tag = pSCmd->tag;
	pReq->Scsi_Status = REQ_STATUS_PENDING;

	pReq->Completion = HBARequestCallback;

	return MV_TRUE;
}

/*
 * Called by mv_linux_queue_command(), i.e. scsi_host_template->queuecommand()
 */
MV_BOOLEAN TranslateOSRequest(
	IN PHBA_Extension pHBA,
	IN struct scsi_cmnd * pSCmd,
	OUT PMV_Request pReq
	)
{
	pReq->Cmd_Initiator = pHBA; //TODO
	pReq->Org_Req = pSCmd;
	pReq->Scsi_Status = REQ_STATUS_INVALID_REQUEST; //TBD??

	pReq->eh_flag = 0;
	pReq->retry_cnt = 0;
 	return TranslateSCSIRequest(pHBA, pSCmd, pReq);
}

/*
 * Called by HBA_Translate_Req_Status_To_OS_Status
 * Release sg buffers after req status returns
 */
void HBA_kunmap_sg(void* pReq)
{
	void *buf;
	struct scsi_cmnd *scmd = NULL;
	struct scatterlist *sg = NULL;
	PMV_Request        req = NULL;

	req  = (PMV_Request) pReq;
	scmd = (struct scsi_cmnd *) req->Org_Req;

	if (scmd)
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 24)
		sg = (struct scatterlist *) scmd->request_buffer;
#else
    	sg = scsi_sglist(scmd);
#endif

	if (NULL == sg) {
		MV_DBG(DMSG_HBA, "no org_req found in the req.\n");
		return;
	}

	if (MV_SCp(scmd)->map_atomic) {
		WARN_ON(!irqs_disabled());
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 24)
		buf = kmap_atomic(sg->page, KM_IRQ0) + sg->offset;
#else
        buf = kmap_atomic(sg_page(sg), KM_IRQ0) + sg->offset;
#endif
		memcpy(buf, req->Data_Buffer, sg->length);
		kunmap_atomic(buf, KM_IRQ0);
		kfree(req->Data_Buffer);
		/* other process might want access to it ... */
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 24)
		req->Data_Buffer = scmd->request_buffer;
#else
        req->Data_Buffer = scsi_sglist(scmd);
#endif
		MV_SCp(scmd)->map_atomic = 0;
	}
}

/*
 * Called by HBARequestCallback(), which is the call back function
 * of MV_Request, i.e. MV_Request->Completion().
 */
void HBA_Translate_Req_Status_To_OS_Status(
	IN PHBA_Extension pHBA,
	IN struct scsi_cmnd *scmd,
	IN PMV_Request pReq
	)
{
	PSENSE_DATA  senseBuffer = (PSENSE_DATA) scmd->sense_buffer;
	int i;
	unsigned char *buf;
	struct scatterlist *sg;

	/* we really wanna do this? */
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 24)
	if (scmd && (SCSI_CMD_MODE_SENSE_6 == scmd->cmnd[0]) && scmd->use_sg 
#else
    if (scmd && (SCSI_CMD_MODE_SENSE_6 == scmd->cmnd[0]) && scsi_bufflen(scmd)
#endif
        && !(MV_SCp(scmd)->map_atomic)) {
		/* 
		 * MODE_SENSE is an instant cmd for SATA devices, thus 
		 * map_atomic should be 1 before we call HBA_kunmap_sg.
		 * while for ATAPI it'll call HBA_kunmap_sg before handling
		 * the command, so its map_atomic should be 0, and we'll need
		 * to copy its buffer from sg list.
		 * - this is how we tell ATAPI from ATA/SATA mode sense -
		 */
		sg = (struct scatterlist *) pReq->Data_Buffer;
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 24)
		buf =(unsigned char *) kmap_atomic(sg->page, KM_IRQ0) + sg->offset;
#else
        buf = kmap_atomic(sg_page(sg), KM_IRQ0) + sg->offset;
#endif
		/* 
		 * ATAPI's Mode parameter header is always 8 bytes 
		 * while MODE_SENSE_6's is 4 bytes.
		 */
		for (i=4;i<pReq->Data_Transfer_Length-4;i++){
			*(buf+i) = *(buf+i+4);
		}
			
		kunmap_atomic(buf, KM_IRQ0);
	}

	HBA_kunmap_sg(pReq);

	if (MV_SCp(scmd)->mapped) {
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 24)
		if (scmd->use_sg) {
			MV_DBG(DMSG_FREQ, "__MV__ call dma_unmap_sg.\n");
			dma_unmap_sg(NULL,
				     (struct scatterlist *)scmd->request_buffer,
				     scmd->use_sg,
				     scmd->sc_data_direction);
		} else {
			MV_DBG(DMSG_FREQ,"__MV__ call dma_unmap_single.\n");
			dma_unmap_single(NULL,
					 MV_SCp(scmd)->bus_address,
					 scmd->request_bufflen,
					 scmd->sc_data_direction);
		}
#else
		if (scsi_sg_count(scmd)) {
			MV_DBG(DMSG_FREQ, "__MV__ call dma_unmap_sg.\n");
			dma_unmap_sg(NULL,
				     scsi_sglist(scmd),
				     scsi_sg_count(scmd),
				     scmd->sc_data_direction);
		} else {
			MV_DBG(DMSG_FREQ,"__MV__ call dma_unmap_single.\n");
			dma_unmap_single(NULL,
					 MV_SCp(scmd)->bus_address,
					 scsi_bufflen(scmd),
					 scmd->sc_data_direction);
		}
#endif
	}

	MV_DBG(DMSG_SCSI_FREQ,
	       "HBA_Translate_Req_Status_To_OS_Status:"
	       " pReq->Scsi_Status = %x pcmd = %p.\n", 
	       pReq->Scsi_Status, scmd);
	
	switch(pReq->Scsi_Status) {
	case REQ_STATUS_SUCCESS:
		scmd->result = (DID_OK<<16);
		break;
	case REQ_STATUS_MEDIA_ERROR: //TBD
		scmd->result = (DID_BAD_TARGET<<16);
		break;
	case REQ_STATUS_BUSY:
		scmd->result = (DID_BUS_BUSY<<16);
		break;
	case REQ_STATUS_NO_DEVICE:
		scmd->result = (DID_NO_CONNECT<<16);
		break;
	case REQ_STATUS_HAS_SENSE:
		/* Sense buffer data is valid already. */
		scmd->result  = (DRIVER_SENSE << 24) | (DID_OK << 16);
		senseBuffer->Valid = 1;

		MV_ERROR_PRINT("MV Sense: Response:0x%x, Sense Key(0x%x):%s, length:%x, ASC:%x ASCQ:%x\n", 
				((MV_PU8)senseBuffer)[0],
				senseBuffer->SenseKey, MV_DumpSenseKey(senseBuffer->SenseKey),
				senseBuffer->AdditionalSenseLength,
				senseBuffer->AdditionalSenseCode, 
				senseBuffer->AdditionalSenseCodeQualifier);
		break;
	default:
		scmd->result = (DRIVER_INVALID|SUGGEST_ABORT)<<24;
		scmd->result |= DID_ABORT<<16;
		break;
	}

#ifdef GALOIS_DUMP_ALL_CMD
	if (!SCSI_IS_READ(scmd->cmnd[0]) && !SCSI_IS_WRITE(scmd->cmnd[0])) {
		int i;
		printk("[target=%d return]: ", mv_scmd_target(scmd));
		for (i = 0; i < 16; i++) {
			printk("%02x ", scmd->cmnd[i]);
		}
		printk("\n");
	}
#endif
	scmd->scsi_done(scmd);
}


/****************************************************
 * Shutdown request's callback function.
 ****************************************************/
static void hba_shutdown_req_cb(MV_PVOID this, PMV_Request req)
{
	PHBA_Extension phba = (PHBA_Extension) this;

	List_Add(&req->Queue_Pointer, &phba->Free_Request);
	phba->Io_Count--;
	complete(&phba->cmpl);
}

/*
 * Called by mv_sata_remove 
 */
void hba_send_shutdown_req(PHBA_Extension phba)
{
	unsigned long flags;
	PMV_Request pReq;

	/*Send MV_REQUEST to do something.*/	
	pReq = kmalloc(sizeof(MV_Request), GFP_ATOMIC);

	/* should we reserve a req for this ? */
	if ( NULL == pReq ) {
		printk("THOR : cannot allocate memory for req.\n");
		return;
	}

	pReq->Cmd_Initiator = phba;
	pReq->Org_Req = pReq; /*no ideas.*/
	pReq->Scsi_Status = REQ_STATUS_INVALID_REQUEST;
	pReq->Completion = hba_shutdown_req_cb;
	
	pReq->Device_Id = 0;
	pReq->Cmd_Flag = 0;
	pReq->Cmd_Flag |= CMD_FLAG_NON_DATA;
	pReq->Sense_Info_Buffer_Length = 0;  
	pReq->Data_Transfer_Length = 0;
	pReq->Data_Buffer = NULL;
	pReq->Sense_Info_Buffer = NULL;
	SGTable_Init(&pReq->SG_Table, 0);
	pReq->Cdb[0] = SCSI_CMD_MARVELL_SPECIFIC;
	pReq->Cdb[1] = CDB_CORE_MODULE;
	pReq->Cdb[2] = CDB_CORE_SHUTDOWN;
	pReq->Context = NULL;
	pReq->LBA.value = 0;
	pReq->Sector_Count = 0;
	pReq->Scsi_Status = REQ_STATUS_PENDING;

	spin_lock_irqsave(&phba->lock, flags);
	List_AddTail(&pReq->Queue_Pointer, &phba->Waiting_Request);
	phba->Io_Count++;
	printk("[Galois]Sending out shutdown command.\n");
	HBA_HandleWaitingList(phba);
	spin_unlock_irqrestore(&phba->lock, flags);

	wait_for_completion_timeout(&phba->cmpl, 10*HZ);
}


/**********************************************************************
 * scsi_host_template member functions:
 * For mv_linux_proc_info()
 **********************************************************************/
int mv_linux_proc_info(struct Scsi_Host *pSHost, char *pBuffer, 
		       char **ppStart,off_t offset, int length, int inout)
{
	int len = 0;
	int datalen = 0;/*use as a temp flag.*/

	if (!pSHost || !pBuffer)
	        return (-ENOSYS);
	if (inout == 1) {
	/* User write is not supported. */
		return (-ENOSYS);
	}

	len = sprintf(pBuffer, "Marvell Thor Driver , Version %s\n", mv_version_linux);
	
	datalen = len - offset;
	if (datalen < 0 ) {
		datalen = 0;
		*ppStart = pBuffer + len;
	} else {
		*ppStart = pBuffer + offset;
	}
	return datalen;
} 


/*
 * scsi_host_template's timeout function
 */
static enum scsi_eh_timer_return mv_linux_timed_out(struct scsi_cmnd *cmd)
{
	PHBA_Extension	pHBA = mv_device_extension_list[0];
	PCore_Driver_Extension pCore = pHBA->Module_Manage.resource[1].module_extension;
	PDomain_Port pPort;
	PDomain_Device pDevice;
	MV_U32 overall_devid, devid, portid;

	overall_devid = mv_scmd_target(cmd);
	portid = PATA_MapPortId(overall_devid);
	if (portid >= MAX_PORT_NUMBER)
	{
		MV_ERROR_PRINT("mv_linux_timed_out: bad portid %d\n", portid);
		return EH_NOT_HANDLED;
	}

	devid = PATA_MapDeviceId(overall_devid);
	pPort = &pCore->Ports[portid];
	pDevice = &pPort->Device[devid];

	galois_sata_event(GALOIS_EVENT_L2H_REQTIMEOUT | GALOIS_EVENT_FROM_PORT(mv_scmd_target(cmd)), pPort, pDevice);
#if 0
	if (timeout_cmd == cmd) {
		timeout_cnt++;
	} else {
		timeout_cmd = cmd;
		timeout_cnt = 0;
	}

	galois_printk("time out (#%d): ", timeout_cnt);
#endif
	galois_printk("%p (%d/%d/%d cdb=0x(%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x)).\n", 
		cmd, mv_scmd_channel(cmd),
		mv_scmd_target(cmd), mv_scmd_lun(cmd),
		*(cmd->cmnd), *(cmd->cmnd+1), *(cmd->cmnd+2), *(cmd->cmnd+3), *(cmd->cmnd+4),
		*(cmd->cmnd+5), *(cmd->cmnd+6), *(cmd->cmnd+7), *(cmd->cmnd+8),
		*(cmd->cmnd+9), *(cmd->cmnd+10), *(cmd->cmnd+11), *(cmd->cmnd+12),
		*(cmd->cmnd+13), *(cmd->cmnd+14), *(cmd->cmnd+15));

	/* failed or reset timer */
//	if (timeout_cnt > 10)
		return EH_NOT_HANDLED;
//	else
//		return EH_RESET_TIMER;
}


/* 
 * scsi_host_templa's queuecommand
 * Call TranslateOSRequest, HBARequestCallback
 */
static int mv_linux_queue_command(struct scsi_cmnd *pSCmd, 
				  void (*done) (struct scsi_cmnd *))
{
	struct Scsi_Host *phost = mv_scmd_host(pSCmd);
	PModule_Header pheader = get_hba_ext_header(phost);
	PHBA_Extension pHBA = (PHBA_Extension)head_to_hba(pheader);
	PMV_Request pReq;
	unsigned long flags;

	if ( done == NULL ) {
		galois_printk( ": in queuecommand, done function can't be NULL\n");
		return 0;
    	}

	spin_lock_irqsave(&pHBA->lock, flags);

	MV_DBG(DMSG_SCSI_FREQ,
    	   "mv_linux_queue_command %p (%d/%d/%d/%d cdb=(%x-%x-%x))\n", 
	       pSCmd, phost->host_no, mv_scmd_channel(pSCmd), 
	       mv_scmd_target(pSCmd), mv_scmd_lun(pSCmd),
	       *(pSCmd->cmnd), *(pSCmd->cmnd+1), 
	       *(pSCmd->cmnd+2));

	pSCmd->result = 0;
 	pSCmd->scsi_done = done;
	MV_SCp(pSCmd)->bus_address = 0;
	MV_SCp(pSCmd)->mapped = 0;
	MV_SCp(pSCmd)->map_atomic = 0;

	/*
	 * only one HBA channel: cmd->device->channel == 0
	 */	
	if (mv_scmd_channel(pSCmd)) {
		pSCmd->result = DID_BAD_TARGET << 16;
		goto done;
	}

	/* 
	 * Get mv_request resource and translate the scsi_cmnd request to mv_request.
	 */
	MV_DASSERT( !List_Empty(&pHBA->Free_Request) );
	pReq = List_GetFirstEntry((&pHBA->Free_Request), MV_Request, Queue_Pointer);
	if ( pReq == NULL ) {
		spin_unlock_irqrestore(&pHBA->lock, flags);
		return SCSI_MLQUEUE_HOST_BUSY;
	}

	MV_ZeroMvRequest(pReq);
	if (!TranslateOSRequest(pHBA,pSCmd, pReq)) { /* if translation fails */
		/* 
		 * Even TranslateOSRequest failed, 
		 * it still should set some of the variables to the MV_Request
		 * especially MV_Request.Org_Req and MV_Request.Scsi_Status;
		 */
		printk( "ERROR - Translation from OS Request failed.\n" );
		pHBA->Io_Count++;

		HBARequestCallback(pHBA, pReq);
		spin_unlock_irqrestore(&pHBA->lock, flags);
		return 0;
	}

	/* 
	 * Queue this request. 
	 * Cannot return with BUSY when core driver is not ready. It'll fail hibernation. 
	 */
	List_AddTail(&pReq->Queue_Pointer, &pHBA->Waiting_Request);
	pHBA->Io_Count++;

	if ( pHBA->State != DRIVER_STATUS_STARTED ) {
		MV_ASSERT(0);
		/*if ( pHBA->State==DRIVER_STATUS_IDLE ) {
		  pHBA->State = DRIVER_STATUS_STARTING;
		  Module_StartAll(module_manage, MODULE_CORE);
		  }*/
	} else {
		HBA_HandleWaitingList(pHBA);
	}
	spin_unlock_irqrestore(&pHBA->lock, flags);

	return 0;
done:
	pSCmd->scsi_done(pSCmd);
	spin_unlock_irqrestore(&pHBA->lock, flags);
	return 0;
}

#if 0
static int mv_linux_reset (struct scsi_cmnd *cmd)
{
	MV_PRINT("__MV__ reset handler %p.\n", cmd);
	return FAILED;
}
#endif

struct scsi_host_template mv_driver_template = {
	module:                         THIS_MODULE,                         
	name:                           "Marvell 88SE61xx Storage Controller",
	proc_name:                      GALOIS_SATA_NAME,
	proc_info:                      mv_linux_proc_info,
	queuecommand:                   mv_linux_queue_command,
#if 0
	eh_abort_handler:			mv_linux_abort,
	eh_device_reset_handler:	mv_linux_reset,
	eh_bus_reset_handler:		mv_linux_reset,
	eh_host_reset_handler:		mv_linux_reset,
#endif /* 0 */
	/* save 2 for ioctl */
	can_queue:                      MAX_REQUEST_NUMBER-31,
	this_id:                        -1,
	max_sectors:                    MV_MAX_TRANSFER_SECTOR,
	sg_tablesize:                   MAX_SG_ENTRY,
	cmd_per_lun:                    MAX_REQUEST_NUMBER-31,
	use_clustering:                 DISABLE_CLUSTERING,
	emulated:                       0
};

static struct scsi_transport_template mv_transport_template = {
        .eh_timed_out   =  mv_linux_timed_out,
};

/**********************************************************************
 * linux scsi init.
 * notifier block to get notified on system shutdown/halt/reboot/down
 **********************************************************************/
static int mv_linux_halt(struct notifier_block *nb, unsigned long event, void *buf)
{
	PHBA_Extension phba = NULL;
	unsigned long flags;

	switch (event) {
	case SYS_RESTART:
	case SYS_HALT:
	case SYS_POWER_OFF:
		list_for_each_entry(phba, &mv_hba_ext_list, next) {
			hba_send_shutdown_req(phba);
			
			spin_lock_irqsave(&phba->lock, flags);
			del_timer_sync(&phba->timer);
			spin_unlock_irqrestore(&phba->lock, flags);
		}
		break;
	default:
		break;
	}

	return NOTIFY_OK;
}

static struct notifier_block mv_linux_notifier = {
	mv_linux_halt, NULL, 0
};

/*
 * interrupt handler
 */
static irqreturn_t mv_intr_handler(int irq, void *dev_id)
{
	/* MV_FALSE should be equal to IRQ_NONE (0) */
	PHBA_Extension phba = (PHBA_Extension)dev_id;
	PModule_Manage module_manage = &phba->Module_Manage;
	MV_PVOID pcore;
	unsigned long flags;
	irqreturn_t retval = MV_FALSE;
	
	spin_lock_irqsave(&phba->lock, flags);
	pcore = module_manage->resource[MODULE_CORE].module_extension;
	retval = Core_InterruptServiceRoutine(pcore);
	spin_unlock_irqrestore(&phba->lock, flags);

	return IRQ_RETVAL(retval);
}

/*
 * Global sata init entry
 */
static PHBA_Extension mv_phba = NULL;
volatile int lowlevel_probe_done = 0;
volatile int highlevel_probe_done = 0;
volatile int wait_loader_ready = 0; /* only for Spruce flash-less */
volatile int all_initcalls_done = 0; /* initcalls done in do_basic_setup */
EXPORT_SYMBOL(lowlevel_probe_done );
EXPORT_SYMBOL(highlevel_probe_done );
EXPORT_SYMBOL(wait_loader_ready );
EXPORT_SYMBOL(all_initcalls_done);
static DECLARE_WAIT_QUEUE_HEAD(lowlevel_probe_waitqueue);

/*
 * almost copied from mv_sata_probe, but run in thread instead in initcalls
 * and add a delay to wait for flashless
 */
static int mv_sata_highlevel_register(void)
{
	unsigned int ret = 0;
	struct Scsi_Host *shost = NULL;
	int old;

	highlevel_probe_done = 0;

	/*
	 * add 3 seconds to wait for Spruce loader
	 */
	if (wait_loader_ready || !all_initcalls_done) {
		int wait_cnt;
		galois_printk("Flash-less loader(%d) or initcalls not done(%d), wait...\n", wait_loader_ready, !all_initcalls_done);
		msleep(2500);
		/* wait 20s  for initcalls */
		for (wait_cnt = 0; wait_cnt < 10; wait_cnt++) {
			if (all_initcalls_done)
				break;
			galois_printk("initcalls not done, wait...");
			msleep(2000);
		}
	}

	old = galois_printk_enable();
	shost = scsi_host_alloc(&mv_driver_template, sizeof(void *));
	if ( NULL == shost ) {
		printk("THOR : Unable to allocate a scsi host.\n");
		goto err_host_alloc;
	}
	
	/* TODO : a saner way is needed - A.C. */
	*((PModule_Header *)shost->hostdata) = Module_GetModuleHeader(mv_phba);

	mv_phba->host = shost;

	shost->irq          = IRQ_SATAINTR;
	shost->max_id       = MV_MAX_TARGET_NUMBER; //2
	shost->max_lun      = MV_MAX_LUN_NUMBER; //1
	shost->max_channel  = 0;
	shost->max_cmd_len  = 16;
	shost->transportt   = &mv_transport_template;

	if ((ret = request_irq(IRQ_SATAINTR, mv_intr_handler, IRQF_SHARED,
				GALOIS_SATA_NAME, mv_phba)) < 0) {
		printk("[Galois]: Error upon requesting IRQ %d.\n", IRQ_SATAINTR);
		goto err_request_irq;
	}

	/* wait for MODULE(CORE,RAID,HBA) init */
	if (0 == wait_for_completion_timeout(&mv_phba->cmpl, (60+10) * HZ)) { /* 60 is internal cmd timeout */
		printk("SATA HBA doesn't complete, exit\n");
		ret = -ENODEV;
		goto err_wait_irq;
	}

	if (mv_device_count == 0) {
		register_reboot_notifier(&mv_linux_notifier); 
		hba_house_keeper_run();
	}

	/*
	 * register the device to system
	 */	
	if (misc_register(&mv_sata_dev)) {
		ret = -ENODEV;
		goto err_register_chrdev;
	}

	mv_device_extension_list[mv_device_count++] = mv_phba;
	
	if (0 != (ret = scsi_add_host(shost, NULL)))
		goto err_add_host;

	scsi_scan_host(shost);

	highlevel_probe_done = 1;
	galois_printk_restore(old);
	return 0;

err_add_host:
	misc_deregister(&mv_sata_dev);

err_register_chrdev:
	if (mv_device_count == 0) {
		unregister_reboot_notifier(&mv_linux_notifier); 
	}
	
err_wait_irq:
	free_irq(IRQ_SATAINTR, mv_phba);	

err_request_irq:
	scsi_host_put(shost);

err_host_alloc:
	Module_ShutdownAll(&mv_phba->Module_Manage);
	list_del(&mv_phba->next);
	mv_hba_release_ext(mv_phba);

	highlevel_probe_done = 1;
	galois_printk_restore(old);
	return ret;
}

static int mv_sata_lowlevel_probe(void *void_data)
{
	INIT_LIST_HEAD(&mv_hba_ext_list);
	/* bg thread init - refer to hba_timer.[ch] */
	hba_house_keeper_init();

	mv_phba = (PHBA_Extension) mv_hba_init_ext();
	if ( NULL == mv_phba ) {
		return -ENOMEM;
	}
	
	list_add_tail(&mv_phba->next, &mv_hba_ext_list);
		
	spin_lock_init(&mv_phba->lock);
	init_timer(&mv_phba->timer);
	sema_init(&mv_phba->sem, 0);
	init_completion(&mv_phba->cmpl);

	Module_InitializeAll(&mv_phba->Module_Manage, MAX_REQUEST_NUMBER);
	
	//spin_lock_irq(&mv_phba->lock);
	Module_StartAll(&mv_phba->Module_Manage, MODULE_CORE);
	//spin_unlock_irq(&mv_phba->lock);

	/*
	 * wake up upper thread
	 */
	galois_printk("SATA lowlevel init done.\n");
	lowlevel_probe_done = 1;
	//wake_up(&lowlevel_probe_waitqueue);
	mv_sata_highlevel_register();

	galois_printk("SATA highlevel init done\n");
	return 0;
}

static int mv_sata_lowlevel_init(void)
{
	struct task_struct *sataprobe_task;
	int ret = 0;

	galois_printk("SATA lowlevel init start.\n");
	lowlevel_probe_done = 0;
	sataprobe_task = kthread_run(mv_sata_lowlevel_probe, NULL, "sataprobe");
	if (IS_ERR(sataprobe_task)) {
		ret = mv_sata_lowlevel_probe(NULL);
	}

	return ret;
}

#if 0
static void wait_for_lowlevel_probe(void)
{
	DEFINE_WAIT(wait);

	if (lowlevel_probe_done)
		return;
	while (!lowlevel_probe_done) {
		prepare_to_wait(&lowlevel_probe_waitqueue, &wait, TASK_UNINTERRUPTIBLE);
		if (!lowlevel_probe_done)
			schedule();
	}
	finish_wait(&lowlevel_probe_waitqueue, &wait);
	return;
}

static int mv_sata_probe(struct platform_device *pdev)
{
	unsigned int ret = 0;
	struct Scsi_Host *shost = NULL;
	struct resource *irq_res;
	int old;

	highlevel_probe_done = 0;
	old = galois_printk_enable();

	/* Wait for lowlevel_probe */
	if (mv_phba) {
		galois_printk("SATA host: wait lowlevel init.\n");
		mv_phba->pdev = pdev;
	} else {
		galois_printk("SATA host: not exist, exit.\n");
		galois_printk_restore(old);
		highlevel_probe_done = 1;
		return -ENOMEM;
	}
#if 0
	while (!lowlevel_probe_done) {
		//galois_printk(".");
		msleep(200);
	}
#else
	wait_for_lowlevel_probe();
#endif

	irq_res = platform_get_resource_byname(pdev, IORESOURCE_IRQ, "sata_irq");
	if (!irq_res) {
		printk("There's no irq resource for platform device.\n");
		galois_printk_restore(old);
		highlevel_probe_done = 1;
		return -ENODEV;
	}

	shost = scsi_host_alloc(&mv_driver_template, sizeof(void *));
	if ( NULL == shost ) {
		printk("THOR : Unable to allocate a scsi host.\n");
		goto err_host_alloc;
	}
	
	/* TODO : a saner way is needed - A.C. */
	*((PModule_Header *)shost->hostdata) = Module_GetModuleHeader(mv_phba);

	mv_phba->host = shost;

	shost->irq          = irq_res->start;
	shost->max_id       = MV_MAX_TARGET_NUMBER; //2
	shost->max_lun      = MV_MAX_LUN_NUMBER; //1
	shost->max_channel  = 0;
	shost->max_cmd_len  = 16;
	shost->transportt   = &mv_transport_template;

	if ((ret = request_irq(shost->irq, mv_intr_handler, IRQF_SHARED,
				GALOIS_SATA_NAME, mv_phba)) < 0) {
		printk("[Galois]: Error upon requesting IRQ %d.\n", shost->irq);
		goto err_request_irq;
	}

	/* wait for MODULE(CORE,RAID,HBA) init */
	if (0 == wait_for_completion_timeout(&mv_phba->cmpl, 30 * HZ)) {
		ret = -ENODEV;
		goto err_wait_irq;
	}

	if (mv_device_count == 0) {
		register_reboot_notifier(&mv_linux_notifier); 
		hba_house_keeper_run();
	}

	/*
	 * register the device to system
	 */	
	if (misc_register(&mv_sata_dev)) {
		ret = -ENODEV;
		goto err_register_chrdev;
	}

	mv_device_extension_list[mv_device_count++] = mv_phba;
	
	if (0 != (ret = scsi_add_host(shost, NULL)))
		goto err_add_host;

	scsi_scan_host(shost);

	highlevel_probe_done = 1;
	galois_printk_restore(old);
	return 0;

err_add_host:
	misc_deregister(&mv_sata_dev);

err_register_chrdev:
	if (mv_device_count == 0) {
		unregister_reboot_notifier(&mv_linux_notifier); 
	}
	
err_wait_irq:
	free_irq(irq_res->start, mv_phba);	

err_request_irq:
	scsi_host_put(shost);

err_host_alloc:
	Module_ShutdownAll(&mv_phba->Module_Manage);
	list_del(&mv_phba->next);
	mv_hba_release_ext(mv_phba);

	highlevel_probe_done = 1;
	galois_printk_restore(old);
	return ret;
}

static int mv_sata_remove(struct platform_device *pdev)
{
	PHBA_Extension phba;
	unsigned long flags;
	struct resource *irq_res;

#if 0
	printk("[Galois]mv_sata_remove.\n");	
	list_for_each_entry(phba, &mv_hba_ext_list, next) {
		if (phba->pdev == pdev) {
			hba_send_shutdown_req(phba);
			break;
		}
	}
#endif

	irq_res = platform_get_resource_byname(pdev, IORESOURCE_IRQ, "sata_irq");
	if (!irq_res) {
		printk("There's no irq resource for platform device.\n");
		return -ENODEV;
	}

	list_for_each_entry(phba, &mv_hba_ext_list, next) {
		if (phba->pdev == pdev) {
			//Module_ShutdownAll(&phba->Module_Manage);
			list_del(&phba->next);
		
			scsi_remove_host(phba->host);
			scsi_host_put(phba->host);

			phba->host = NULL;
			hba_send_shutdown_req(phba);

			spin_lock_irqsave(&phba->lock, flags);
			del_timer_sync(&phba->timer);
			spin_unlock_irqrestore(&phba->lock, flags);

			free_irq(irq_res->start, phba);
		
			//iounmap(phba->Base_Address);
			mv_hba_release_ext(phba);
			mv_device_count--;
			break;
		}
	}

	if (mv_device_count == 0)
		unregister_reboot_notifier(&mv_linux_notifier);

	return 0;
}
#endif

#ifdef CONFIG_PM
static int mv_sata_suspend(struct platform_device *dev, pm_message_t state)
{	
	extern void CompleteRequestAndSlot(
		IN PCore_Driver_Extension pCore,
		IN PDomain_Port pPort,
		IN PMV_Request pReq,
		IN PATA_TaskFile pTaskFile,
		IN MV_U8 slotId
	);
	PHBA_Extension	pHBA = mv_device_extension_list[0];
	PCore_Driver_Extension pCore = pHBA->Module_Manage.resource[1].module_extension;
	PDomain_Port pPort;

	PMV_Request pReq;
	MV_LPVOID portMmio;
	MV_U8 i, j;
	MV_U32 temp;

	for(i=0; i<pCore->Port_Num; i++) {
		pPort = &pCore->Ports[i];
		portMmio = pPort->Mmio_Base;

		if(SATA_PortDeviceDetected(pPort)) {
			temp = MV_REG_READ_DWORD( portMmio, PORT_CMD );
			MV_REG_WRITE_DWORD( portMmio, PORT_CMD, temp & ~MV_BIT(0));

			SATA_PortReportNoDevice(pCore, pPort); /* remove scd */
			
			/* Device is gone. Return the Running_Req */
			for ( j=0; j<MAX_SLOT_NUMBER; j++ )
			{
				pReq =  pPort->Running_Req[j];
				if ( pReq !=NULL )
				{
					pReq->Scsi_Status = REQ_STATUS_NO_DEVICE;
					CompleteRequestAndSlot(pCore, pPort, pReq, NULL, j);
				}
			}
			
			if( pPort->Type == PORT_TYPE_PM )
			{
				pPort->Setting &= ~PORT_SETTING_PM_FUNCTIONAL;
				pPort->Setting &= ~PORT_SETTING_PM_EXISTING;
			}
			galois_sata_event(GALOIS_EVENT_L2H_HOTPLUGOUT | (GALOIS_EVENT_FROM_PORT(pPort->Id)), pPort, NULL);
		}
	}
	return 0;
}

static int mv_sata_resume(struct platform_device *dev)
{
	extern void mvHandleDevicePlugin(PCore_Driver_Extension pCore, PDomain_Port pPort);
	PHBA_Extension	pHBA = mv_device_extension_list[0];
	PCore_Driver_Extension pCore = pHBA->Module_Manage.resource[1].module_extension;
	PDomain_Port pPort;
	int i;

	for(i=0; i<pCore->Port_Num; i++) {
		pPort = &pCore->Ports[i];
		if (SATA_PortDeviceDetected(pPort))
			mvHandleDevicePlugin(pCore, pPort);
	}
	return 0;
}
#endif

static struct platform_driver mv_sata_driver = {
#if 0
	.probe		= mv_sata_probe,
	.remove		= mv_sata_remove,
#endif
#ifdef CONFIG_PM
	.suspend	= mv_sata_suspend,
	.resume		= mv_sata_resume,
#endif
	.driver 	= {
		.name	= "mv88de3010_sata",
		//.multithread_probe = 1,
	},
};

int early_host_reset= 0;
EXPORT_SYMBOL(early_host_reset);
static int __init setup_early_host_reset(char *str)
{
	early_host_reset = 1;
	return 1;
}
__setup("sata_reset_done", setup_early_host_reset);

static int __init mv_linux_driver_init(void)
{
	/* default to only show no msg */
	mv_dbg_opts = 0;

	return platform_driver_register(&mv_sata_driver);
}

static void __exit mv_linux_driver_exit(void)
{
	misc_deregister(&mv_sata_dev);

	hba_house_keeper_exit();
	platform_driver_unregister(&mv_sata_driver);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR ("Marvell Semiconductor Inc.,");
MODULE_DESCRIPTION ("thor SATA hba driver");

arch_initcall(mv_sata_lowlevel_init);
module_init(mv_linux_driver_init);
//module_exit(mv_linux_driver_exit);

