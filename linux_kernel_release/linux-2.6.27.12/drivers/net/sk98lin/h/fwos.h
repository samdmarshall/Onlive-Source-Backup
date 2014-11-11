/******************************************************************************
 *
 * Name:    fwos.h
 * Project: Gigabit Ethernet Adapters, Common Modules
 * Version: $Revision: 1.1 $
 * Date:    $Date: 2009/10/29 14:17:05 $
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

#ifndef __SK_FWOS_H__
#define __SK_FWOS_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define SK_FW_HANDLE const struct firmware *

extern SK_FW_HANDLE FwOsOpenFile(SK_AC *, char *);
extern int          FwOsCloseFile(SK_AC *, SK_FW_HANDLE);
extern SK_U32       FwOsGetFileSize(SK_AC *, SK_FW_HANDLE);
extern SK_U8        *FwOsAllocMemory(SK_AC *, size_t);
extern int          FwOsFreeMemory(SK_AC *, SK_U8 *);
extern int          FwOsFileRead(SK_AC *, SK_FW_HANDLE, SK_U8 *, SK_U32, SK_U32);
extern void         FwOsSleep(int);

/*******************************************************************************
 *
 * OS specific FW structure
 *
 ******************************************************************************/

typedef struct s_FwOs {
	char    *pFilePathName;
	SK_U32  FileSize;
	SK_U32  *pFileHandle;
	SK_U8   *pFileData;
#ifdef SK_USE_ISR_FW_ISR
	SK_U8   *pFwData;
#endif
	int     FileIndex;
} SK_FWOS;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SK_FWOS_H__ */

