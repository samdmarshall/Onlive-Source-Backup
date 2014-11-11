/*******************************************************************************
*******************************************************************************/



#ifndef _ICCHAL_PLATFORM_H_
#define _ICCHAL_PLATFORM_H_

#define MV_CC_HAL_FlushMemory_Platform(x)	{			\
	volatile UINT i;										\
	i = (UINT)(x);											\
}

#endif	// #ifndef _ICCHAL_PLATFORM_H_
