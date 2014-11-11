#ifndef _LINUX_MAIN_H
#define _LINUX_MAIN_H

#include "mv_os.h"
#include "mv_include.h"

extern PHBA_Extension g_Linux_HBA;

struct _MV_SCP {
	MV_U16           mapped;
	MV_U16           map_atomic;
	BUS_ADDRESS bus_address;
};

#define MV_SCp(cmd) ((struct _MV_SCP *) &((struct scsi_cmnd *)cmd)->SCp)

#define get_hba_ext_header(phost) (*(PModule_Header *)phost->hostdata)

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
#define mv_scmd_host(cmd)    cmd->device->host
#define mv_scmd_channel(cmd) cmd->device->channel
#define mv_scmd_target(cmd)  cmd->device->id
#define mv_scmd_lun(cmd)     cmd->device->lun
#else
#define mv_scmd_host(cmd)    cmd->host
#define mv_scmd_channel(cmd) cmd->channel
#define mv_scmd_target(cmd)  cmd->target
#define mv_scmd_lun(cmd)     cmd->lun
#endif

#define SCSI_IS_INSTANT(cmd) \
             ((cmd == SCSI_CMD_INQUIRY) || \
	      (cmd == SCSI_CMD_READ_CAPACITY_10) || \
	      (cmd == SCSI_CMD_READ_CAPACITY_16) || \
	      (cmd == SCSI_CMD_REPORT_LUN) || \
	      (cmd == SCSI_CMD_MODE_SENSE_6) || \
	      (cmd == SCSI_CMD_MODE_SENSE_10))

#define LO_BUSADDR(x) ((MV_U32)(x))
#define HI_BUSADDR(x) (sizeof(BUS_ADDRESS)>4? (x)>>32 : 0)

/*
 * kernel ioctl interface
 */
#define SENSE_INFO_BUFFER_SIZE		32
#define MAX_COMMAND_SIZE		16

/* For Character Device Interface */
#define MV_DEVICE_MAX_SLOT 4

#define LDINFO_NUM MAX_LD_SUPPORTED * MAX_NUM_ADAPTERS
#define HDINFO_NUM MAX_HD_SUPPORTED * MAX_NUM_ADAPTERS

typedef struct _SCSI_PASS_THROUGH_DIRECT {
	unsigned short Length;
	unsigned char  ScsiStatus;
	unsigned char  PathId;
	unsigned char  TargetId;
	unsigned char  Lun;
	unsigned char  CdbLength;
	unsigned char  SenseInfoLength;
	unsigned char  DataIn;
	unsigned long  DataTransferLength;
	unsigned long  TimeOutValue;
	void           *DataBuffer;
	unsigned long  SenseInfoOffset;
	unsigned char  Cdb[16];
} SCSI_PASS_THROUGH_DIRECT, *PSCSI_PASS_THROUGH_DIRECT;

typedef struct _SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER{
	SCSI_PASS_THROUGH_DIRECT        sptd;
	unsigned long                   Filler;
	unsigned char                   Sense_Buffer[SENSE_INFO_BUFFER_SIZE];
} SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER, *PSCSI_PASS_THROUGH_DIRECT_WITH_BUFFER;

#endif /*_LINUX_MAIN_H*/
