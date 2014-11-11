#ifndef __MV_OS_HEADER_H__
#define __MV_OS_HEADER_H__

/*
 * Currently only three OS is supported.
 */
#if defined(__LINUX_KERNEL__)
#include "mv_os_linux.h"
#elif defined(__ECOS__)
#include "mv_os_ecos.h"
#elif defined(__UBOOT__)
#include "mv_os_uboot.h"
#elif defined(__LINUX__) && defined(BERLIN_SINGLE_CPU)
#include "mv_os_ecos.h"  //to be fixed for linux
#else
#error ERROR: OS not specified
#endif

#endif /* __MV_OS_HEADER_H__ */
