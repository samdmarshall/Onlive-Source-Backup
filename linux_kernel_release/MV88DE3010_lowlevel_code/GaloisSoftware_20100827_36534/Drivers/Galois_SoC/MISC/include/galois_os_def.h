#ifndef __GALOIS_OS_DEF_H__
#define __GALOIS_OS_DEF_H__
#if (BERLIN_CHIP_VERSION >= BERLIN_BG2)
#define GALOIS_CPU_ID   0	
#else
#define GALOIS_CPU_ID	CPUINDEX
#endif

#endif //__GALOIS_OS_DEF_H__
