#ifndef __MV_CONFIGURATION_H__
#define __MV_CONFIGURATION_H__
#include "com_define.h"

/*
 * Galois specific bug&feature macro
 */
#define GALOIS_ERROR_DEBUG 1
#define GALOIS_HANG_DEBUG 1
#define GALOIS_MAX_HANG_RETRY 1 /* totally GALOIS_MAX_HANG_RETRY times */
//#define GALOIS_NCQ_PRDTL_HACK 1 /* merge multiple PRD to one for NCQ,dangerous */
#define GALOIS_PRD_8BYTES_ALIGN_BUG 1
#define BERLINA_DEBUG_AACS 1
//#define BERLIN_SUPPORT_SPRUCE_DRIVE 1
#define BERLIN_ATAPI_GET_READY 1
#define BERLIN_SPRUCE_INTEGRATION 1

//#define BERLIN_POWER_OFFON_DRIVE 1
//#define HACK_GPIO21_DRIVE_POWER 1

/*
 * in 100ms, poll status after hba reset
 * for 2T WD hard drive, it needs 16s to receive signature FIS.
 */
#define HBARESET_STATUS_POLL_COUNT 160

/*
 * diff the first and later init/probe sata controller
 * later init/probe should be atomic.
 */
#define GALOIS_SATA_ATOMIC	0x1
#define GALOIS_SATA_SLEEP	0x0

/*
 * Galois playback event definitions
 */
#define GALOIS_EVENT_L2H_DRIVEINIT	0x00000001
#define GALOIS_EVENT_L2H_DRIVEDONE	0x00000002
#define GALOIS_EVENT_L2H_DRIVEHANG	0x00000011
#define GALOIS_EVENT_L2H_DRIVERESET	0x00000012
#define GALOIS_EVENT_L2H_HOTPLUGIN	0x00000013
#define GALOIS_EVENT_L2H_HOTPLUGOUT	0x00000014
#define GALOIS_EVENT_L2H_DEADDRIVE	0x00000015
#define GALOIS_EVENT_L2H_NOSIG		0x00000016
#define GALOIS_EVENT_L2H_REQDROP	0x00000017
#define GALOIS_EVENT_L2H_REQTIMEOUT	0x00000018
#define GALOIS_EVENT_L2H_BADDISC	0x00000019
#define GALOIS_EVENT_H2L_PLAYFILE	0x00002100
#define GALOIS_EVENT_H2L_NOTPLAYFILE	0x00002200

#define GALOIS_EVENT_L2H_MASK		0x000000ff
#define GALOIS_EVENT_H2L_MASK		0x0000ff00
#define GALOIS_EVENT_PORT_MASK		0x80000000
#define GALOIS_L2H_EVENT(x)	((GALOIS_EVENT_L2H_MASK & (x)) && !(GALOIS_EVENT_H2L_MASK & (x)))
#define GALOIS_H2L_EVENT(x)	((GALOIS_EVENT_H2L_MASK & (x)) && !(GALOIS_EVENT_L2H_MASK & (x)))
#define GALOIS_EVENT_TO_PORT(x)			((GALOIS_EVENT_PORT_MASK & (x)) >> 31)
#define GALOIS_EVENT_FROM_PORT(port)	((port << 31) & GALOIS_EVENT_PORT_MASK)

int galois_sata_event(unsigned int event, MV_PVOID port, MV_PVOID device);

/* 
 * HBA macro definition
 */
#define MV_MAX_TRANSFER_SIZE    (4*1024*1024) //maximum transfer byte count is 4MB, old size is 128*1024
#define MV_MAX_TRANSFER_SECTOR  (MV_MAX_TRANSFER_SIZE/512)
#define MAX_REQUEST_NUMBER      32

/* 
 * Core driver macro definition
 */
#define MAX_DEVICE_SUPPORTED    32
#define MAX_HD_NUMBER           32
#define MAX_SG_ENTRY            128
#define MAX_SG_ENTRY_REDUCED    128

/*
 * SATA controller features macro
 */
//#define USE_DMA_FOR_ALL_PACKET_COMMAND /* It's dangerous */
//#define PRD_SIZE_WORD_ALIGN

/* hot plug & PM */
#define SUPPORT_HOT_PLUG        1

#define SUPPORT_ERROR_HANDLING  1

#endif /* __MV_CONFIGURATION_H__ */
