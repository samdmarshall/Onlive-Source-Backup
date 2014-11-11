#include "Galois_memmap.h"
#include "mv_include.h"
#include "linux_main.h"
#include "hba_mod.h"
#include <linux/pci.h>

/*
 * Pre-defined module function table
 * Please match this predefined module set with the enum Module_Id
 * Pay attention: If you want to change the following data structure,
 * please change Module_Id as well.
 */
Module_Interface module_set[MAX_MODULE_NUMBER] = 
{
	{
		MODULE_HBA,
		HBA_ModuleGetResourceQuota,
		HBA_ModuleInitialize,
		HBA_ModuleStart,
		HBA_ModuleShutdown,
		NULL, /* - USE ME AND DIE!! HBA_ModuleNotification, */
		HBA_ModuleSendRequest,
		HBA_ModuleReset,
		HBA_ModuleMonitor,
	#ifdef SUPPORT_VIRTUAL_AND_PHYSICAL_SG
		HBA_ModuleGetVirtualSG
	#endif
	},
	{
		MODULE_CORE,
		Core_ModuleGetResourceQuota,
		Core_ModuleInitialize,
		Core_ModuleStart,
		Core_ModuleShutdown,
		Core_ModuleNotification,
		Core_ModuleSendRequest,
		Core_ModuleReset,
		Core_ModuleMonitor,
	#ifdef SUPPORT_VIRTUAL_AND_PHYSICAL_SG
		NULL
	#endif
	}
};

void Module_InitializeAll(PModule_Manage p_module_manage, MV_U16 max_io)
{
	MV_I8 i = 0;
	MV_PVOID module_extension = NULL;
	MV_U32 extension_size = 0;

	/* Module initialization is one synchronized function. */
	for ( i=MAX_MODULE_NUMBER-1; i>=0; i-- )
	{
		/* I use this chance to check whether the module_set matches with Module_Id */
		MV_ASSERT( module_set[i].module_id==i );

		if ( module_set[i].module_initialize )
		{
			module_extension = p_module_manage->resource[i].module_extension;
			extension_size = p_module_manage->resource[i].extension_size;
			module_set[i].module_initialize(module_extension, extension_size, max_io);
		}
	}
}

void Module_StartAll(PModule_Manage p_module_manage, MV_U8 begin_module)
{
	MV_I8 i = 0;

	/* 
	 * Start module from the lower level, the first one is the core driver.
	 * Every time we only start one module.
	 */
	for ( i=begin_module; i>=0; i-- )
	{
		MV_ASSERT(begin_module<MAX_MODULE_NUMBER);
		if ( module_set[i].module_start )
		{
			module_set[i].module_start(p_module_manage->resource[i].module_extension);
			return;
		}

		/* If the module_start function is NULL, continue to the next. */
		p_module_manage->status |= (1<<i);
	}
}

void Module_ShutdownAll(PModule_Manage p_module_manage)
{
	MV_I8 i = 0;
	MV_PVOID module_extension = NULL;

	/* Module stop is one synchronized function. */
	for ( i=MAX_MODULE_NUMBER-1; i>=0; i-- )
	{
		if ( module_set[i].module_stop )
		{
			module_extension = p_module_manage->resource[i].module_extension;
			module_set[i].module_stop(module_extension);
		}
	}
}

/* 
 * Initialize Module_Header of each module, and for each module i, 
 * assign its module_extension to phba->Module_Manage[i].module_extension
 */
void Module_AssignModuleExtension(MV_PVOID module_header, MV_U16 max_io)
{
	MV_PTR_INTEGER ptemp = (MV_PTR_INTEGER)module_header;
	PHBA_Extension pHBA = NULL;
	PModule_Manage module_manage = NULL;
	PModule_Header header = NULL;
	MV_U8 module_id;
	MV_U32 require;

	MV_ASSERT(MODULE_HBA==0);
	pHBA = (PHBA_Extension)( (MV_PTR_INTEGER)module_header+MODULE_HEADER_SIZE );
	module_manage = &pHBA->Module_Manage;

	for (module_id=0; module_id<MAX_MODULE_NUMBER; module_id++) {
		if (module_set[module_id].get_mem_size==NULL )
			continue;

		require = module_set[module_id].get_mem_size(RESOURCE_CACHED_MEMORY, max_io);
		require = ROUNDING(require, 8);
		
		header = (PModule_Header)ptemp;
		header->extension_size = require;
		header->header_size = MODULE_HEADER_SIZE;
		header->module_id = module_id;
		header->hba_extension = pHBA;

		module_manage->resource[module_id].module_extension = 
				(MV_PVOID)(ptemp + MODULE_HEADER_SIZE);
		module_manage->resource[module_id].extension_size = require;

		ptemp += MODULE_HEADER_SIZE + require;
	}
}

/*
 * HBA and HBA's modules initialization
 */
//void *mv_hba_init_ext(struct platform_device *pdev)
void *mv_hba_init_ext(void)
{
	int i;

	PModule_Header pheader;
	PHBA_Extension phba;
	PModule_Manage pmod_manage;

	unsigned long total_size = 0;
	unsigned long size = 0;

	//struct resource *mem_res;
	dma_addr_t    dma_addr;
	BUS_ADDRESS   bus_addr;
	MV_PHYSICAL_ADDR phy_addr;
	

	/* allocate normal (CACHED) mem */
	for (i=0; i<MAX_MODULE_NUMBER; i++) {
		size = module_set[i].get_mem_size(RESOURCE_CACHED_MEMORY, MAX_REQUEST_NUMBER);
		if ( 0 != size )
			total_size += ROUNDING(size, 8);
		
		WARN_ON(size != ROUNDING(size, 8));
	}

	/* init hba ext structure */
	total_size += ROUNDING(MODULE_HEADER_SIZE * MAX_MODULE_NUMBER, 8);

	MV_DBG(DMSG_HBA, "THOR : Memory quota is 0x%lx bytes.\n", total_size);

	pheader = (PModule_Header) vmalloc(total_size);
	if ( NULL == pheader )
		return NULL;

	memset(pheader, 0, total_size);
	Module_AssignModuleExtension(pheader, MAX_REQUEST_NUMBER);
	
	phba = (PHBA_Extension) head_to_hba(pheader);
	phba->host_data = pheader;
	phba->Vendor_Id = 0x12;
	phba->Device_Id = 0x34;
	phba->Revision_Id = 0x56;

	/*
	mem_res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "sata_mem");
	if (!mem_res) {
		MV_PRINT("There's no memory resource for platform device.\n");
		return NULL;
	}
	phba->Base_Address = (MV_PVOID)mem_res->start;
	*/
	phba->Base_Address = (MV_PVOID)MEMMAP_SATA_REG_BASE;
	
	/* allocate consistent dma memory (uncached) */
	size = 0;
	total_size = 0;
	pmod_manage = &phba->Module_Manage;
	
	for (i=0; i<MAX_MODULE_NUMBER; i++) {
		size = module_set[i].get_mem_size(RESOURCE_UNCACHED_MEMORY, MAX_REQUEST_NUMBER);
		if (0 == size) 
			continue;
		
		WARN_ON(size != ROUNDING(size, 8));
		size = ROUNDING(size, 8);

		pmod_manage->resource[i].uncached_address = 
			(MV_PVOID) pci_alloc_consistent(NULL, size, &dma_addr);

		if ( NULL == pmod_manage->resource[i].uncached_address )
			goto ext_err_dma;
		
		pmod_manage->resource[i].uncached_size = size;

		bus_addr = (BUS_ADDRESS) dma_addr;
		phy_addr.low  = LO_BUSADDR(bus_addr);
		phy_addr.high = HI_BUSADDR(bus_addr);
		pmod_manage->resource[i].uncached_physical_address = phy_addr;
	}

	MV_DBG(DMSG_HBA, "THOR : HBA ext struct init'ed at %p.\n", phba);
	return phba;

ext_err_dma:
	for (i=0; i<MAX_MODULE_NUMBER; i++) {
		if ( pmod_manage->resource[i].uncached_size ) {
			phy_addr = pmod_manage->resource[i].uncached_physical_address;
			dma_addr = (dma_addr_t) ( phy_addr.low | ((u64) phy_addr.high)<<32 );
			pci_free_consistent(NULL,
					    pmod_manage->resource[i].uncached_size,
					    pmod_manage->resource[i].uncached_address,
					    dma_addr);
		}
	}
	vfree(pheader);
	return NULL;
}

void mv_hba_release_ext(PHBA_Extension phba)
{
	dma_addr_t dma_addr;
	MV_PHYSICAL_ADDR phy_addr;
	int i;
	
	PModule_Manage pmod_manage = &phba->Module_Manage;
	
	for (i=0; i<MAX_MODULE_NUMBER; i++) {
		if ( pmod_manage->resource[i].uncached_size ) {
			phy_addr = pmod_manage->resource[i].uncached_physical_address;
			dma_addr = (dma_addr_t) ( phy_addr.low | ((u64) phy_addr.high)<<32 );
			pci_free_consistent(NULL, 
					    pmod_manage->resource[i].uncached_size,
					    pmod_manage->resource[i].uncached_address,
					    dma_addr);
		}
	}
	
	vfree(phba->host_data);
	return;
}
