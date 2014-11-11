#if !defined(MODULE_MANAGE_H)

#define MODULE_MANAGE_H

#include "hba_exp.h"
#include "hba_timer.h"

/*
 * Module definition
 */
typedef struct _Module_Interface
{
	MV_U8 module_id;
	MV_U32 (*get_mem_size)(enum Resource_Type type, MV_U16 max_io);
	void (*module_initialize)(MV_PVOID extension, MV_U32 extension_size, MV_U16 max_io);
	void (*module_start)(MV_PVOID extension);
	void (*module_stop)(MV_PVOID extension);
	void (*module_notification)(MV_PVOID extension, enum Module_Event event, MV_PVOID param);
	void (*module_sendrequest)(MV_PVOID extension, PMV_Request pReq);
	void (*module_reset)(MV_PVOID extension);
	void (*module_monitor)(MV_PVOID extension);
} Module_Interface, *PModule_Interface;

/*
 * Module Management
 */
typedef struct _Module_Header
{
	/* 
	 * Here is the hidden module header. 
	 * Module is not aware of this except for HBA module management.
	 */
	MV_U8		header_size;		/* Module header size */
	MV_U8		module_id;			/* It's also the module is in enum Module_Id */
	MV_U8		reserved0[2];
	MV_U32		extension_size;		/* size of the extension, header is not included. */
	MV_PVOID	hba_extension;		/* point to the pHBA extension, not the header. */
} Module_Header, * PModule_Header;

/* Size must be 64 bit rounded. */
#define MODULE_HEADER_SIZE	ROUNDING(sizeof(Module_Header), 8)

#define Module_GetModuleHeader(extension)	\
	((PModule_Header)((MV_PTR_INTEGER)extension-MODULE_HEADER_SIZE))

#define Module_GetModuleId(extension)		\
	(Module_GetModuleHeader(extension)->module_id)

#define Module_GetHBAExtension(extension)	\
	(Module_GetModuleHeader(extension)->hba_extension)

#define Module_IsStarted(p_module_manage, module_id)	\
	(p_module_manage->status&=(1<<(module_id)))

#define head_to_hba(head) ((MV_PTR_INTEGER)head+MODULE_HEADER_SIZE)

typedef struct _Module_Resource
{
	/* Extension assigned to this module */
	MV_PVOID	    module_extension;
	MV_U32		    extension_size;	/* Extension size */
	MV_U32		    uncached_size;	/* Uncached memory size */
        /* Uncached memory virtual address */
	MV_PVOID	    uncached_address;	
	/* Uncached memory physical address */
	MV_PHYSICAL_ADDR    uncached_physical_address;	
} Module_Resource, *PModule_Resource;

typedef struct _Module_Manage
{
	Module_Resource resource[MAX_MODULE_NUMBER];
        /* One bit for one module. If started, the bit is set. */
	MV_U8			status;	
	MV_U8			reserved0[7];
} Module_Manage, *PModule_Manage;

#define DRIVER_STATUS_IDLE      1    /* The first status */
#define DRIVER_STATUS_STARTING  2    /* Begin to start all modules */
#define DRIVER_STATUS_STARTED   3    /* All modules are all settled. */

struct _HBA_Extension
{
	/* Device extention */
	MV_PVOID host_data;
	struct platform_device *pdev;

	struct list_head        next;
	spinlock_t              lock;
	struct semaphore	sem;
	struct timer_list	timer;
	struct Scsi_Host	*host;

	MV_PVOID 	uncached_virtual_address[MAX_MODULE_NUMBER];
	MV_U32          uncached_size[MAX_MODULE_NUMBER];
	dma_addr_t      uncached_physical[MAX_MODULE_NUMBER];

	struct completion       cmpl;

	/* System resource */
	MV_PVOID                Base_Address;

	MV_U32                  State;	
	/* Is OS during hibernation or crash dump? */
	MV_BOOLEAN              Is_Dump;
	/* Outstanding requests count */
	MV_U8                   Io_Count;
	/* Maximum requests number we can handle */
	MV_U16                  Max_Io;

	/* Adapter information */
	MV_U8                   Adapter_Bus_Number;
	MV_U8                   Adapter_Device_Number;
	MV_U16                  Vendor_Id;
	MV_U16                  Device_Id;
	MV_U8                   Revision_Id;
	MV_U8                   Reserved0;

	/* Module management related variables */
	struct _Module_Manage   Module_Manage;

	/* Timer module */
	struct _Timer_Module    Timer_Module;

	/* Free MV_Request queue */
	List_Head               Free_Request;
	/* MV_Request waiting queue */
	List_Head               Waiting_Request;

#ifdef SUPPORT_EVENT
	List_Head               Stored_Events;
	List_Head               Free_Events;
	MV_U32	                SequenceNumber;
	MV_U8                   Num_Stored_Events;
#endif /* SUPPORT_EVENT */

	/* 
	 * Memory pool can be used as variable data structures like timer 
	 * This item must always be put at the end of this data structure.
	 */
	MV_U8                   Memory_Pool[1];
};
typedef struct _HBA_Extension HBA_Extension, *PHBA_Extension;

/*
 * HBA Modules primitives
 */
void HBA_ModuleStarted(MV_PVOID This);

void Module_InitializeAll(IN PModule_Manage p_module_manage, IN MV_U16 max_io);

void Module_StartAll(IN PModule_Manage p_module_manage, IN MV_U8 begin_module);

void Module_ShutdownAll(IN PModule_Manage p_module_manage);

void Module_AssignModuleExtension(IN MV_PVOID module_header, IN MV_U16 max_io);

void *mv_hba_init_ext(void);
void mv_hba_release_ext(PHBA_Extension phba);

#endif /* MODULE_MANAGE_H */

