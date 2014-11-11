/******************************************************************************
 *
 * Name:    fwos.c
 * Project: Gigabit Ethernet Adapters, Common Modules
 * Version: $Revision: 1.2 $
 * Date:    $Date: 2010/03/08 08:29:34 $
 * Purpose: Kernel mode file read functions.
 *
 ******************************************************************************/

/******************************************************************************
 *
 *	LICENSE:
 *	(C)Copyright Marvell.
 *	
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *	
 *	The information in this file is provided "AS IS" without warranty.
 *	/LICENSE
 *
 *****************************************************************************/

#if (defined(DEBUG) || ((!defined(LINT)) && (!defined(SK_SLIM))))
static const char SysKonnectFileId[] =
"$Header: /data/cvs/sweprojects/yukon2/fwcommon/linux/fwos.c,v 1.2 2010/03/08 08:29:34 tschilli Exp $" ;
#endif

#include "h/skdrv1st.h"
#include "h/skdrv2nd.h"

/********************************************************
		Local Variables
********************************************************/

/********************************************************
		Global Variables
********************************************************/

/********************************************************
		Local Functions
********************************************************/

/********************************************************
		Global Functions
********************************************************/

/**
 *  @brief This function opens the FW image file and stores
 *         the data in an allocated buffer.
 *
 *  @param pAC           Pointer to adapter context
 *  @param pFilePathName Pointer to full pathname of FW image file
 *  @param FileHandle    FileHandle of the FW image file
 *  @return              FileHandle
 */
SK_FW_HANDLE FwOsOpenFile(
SK_AC *pAC,
char  *pFilePathName
)
{
	SK_FW_HANDLE pSkFirmware = NULL;

	if (request_firmware(&pSkFirmware, FW_FILE_NAME, &pAC->PciDev->dev) == 0) {
		printk("sk98lin: Using flash image from binary file. Size: %d\n", (int) pSkFirmware->size);
    } else {
		printk("sk98lin: Firmware image NOT found.\n");
	}

	return pSkFirmware;
} /* FwOsOpenFile */

/**
 *  @brief This function closes the FW image file.
 *
 *  @param pAC        Pointer to adapter context
 *  @param FileHandle FileHandle of the FW image file
 *  @return           SK_TRUE or SK_FALSE
 */
int FwOsCloseFile(
SK_AC 			*pAC,
SK_FW_HANDLE	pFileHandle)
{
	release_firmware(pFileHandle);
	return SK_TRUE;
} /* FwOsCloseFile */

/** 
 *  @brief This function returns the size of the FW image file.
 *  
 *  @param pAC      Pointer to adapter context
 *  @return         Size of FW image file
 */
SK_U32 FwOsGetFileSize(SK_AC *pAC, SK_FW_HANDLE FileHandle)
{
	return FileHandle->size;
} /* FwOsGetFileSize */

/** 
 *  @brief This function allocates kernel memory.
 *  
 *  @param pAC      Pointer to adapter context
 *  @return         Pointer to allocated memory
 */
SK_U8 *FwOsAllocMemory(SK_AC *pAC, size_t Size)
{
	SK_U8 *pMemory;

	pMemory = kmalloc(Size, GFP_KERNEL);

	return pMemory;
} /* FwOsAllocMemory */

/**
 *  @brief This function frees previously allocated memory.
 *
 *  @param pAC      Pointer to adapter context
 *  @return         SK_TRUE or SK_FALSE
 */
int FwOsFreeMemory(SK_AC *pAC, SK_U8 *pMemory)
{
	if (pMemory != NULL) {
		kfree(pMemory);
	}

	return SK_TRUE;
} /* FwOsFreeMemory */

/**
 *  @brief This function reads data from the FW image file.
 *
 *  @param pAC        Pointer to adapter context
 *  @param FileHandle Pointer to adapter context
 *  @param pData      Pointer to data buffer
 *  @param Offset     Start offset in flash eprom for operation
 *  @param Length     Length in flash eprom
 *  @return           SK_TRUE or SK_FALSE
 */
int FwOsFileRead(
SK_AC  			*pAC,
SK_FW_HANDLE 	FileHandle,
SK_U8			*pData,
SK_U32			Offset,
SK_U32			Length)
{
	SK_U8 *pFileData;
	SK_U32 i;

	pFileData = FileHandle->data;
	pFileData += Offset;

	for (i = 0; i < Length; i++) {
		*(pData + i) = *(pFileData + i);
	}
	return SK_TRUE;
} /* FwOsFileRead */

/**
 *  @brief This function frees previously allocated memory.
 *
 *  @param Delay    Time to sleep
 *  @return         SK_TRUE or SK_FALSE
 */
void FwOsSleep(int Delay)
{
	/* Time to sleep in milliseconds */
	msleep((unsigned int)(Delay/1000));
} /* FwOsSleep */

