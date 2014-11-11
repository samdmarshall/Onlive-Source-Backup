#ifndef COM_DEFINE_H
#define COM_DEFINE_H


/*
 *	This file defines Marvell OS independent primary data type for all OS.
 *
 *	We have macros to differentiate different CPU and OS.
 *
 *	CPU definitions:
 *	_CPU_X86_16B	
 *	Specify 16bit x86 platform, this is used for BIOS and DOS utility.
 *	_CPU_X86_32B
 *	Specify 32bit x86 platform, this is used for most OS drivers.
 *	_CPU_IA_64B
 *	Specify 64bit IA64 platform, this is used for IA64 OS drivers.
 *	_CPU_AMD_64B
 *	Specify 64bit AMD64 platform, this is used for AMD64 OS drivers.
 *
 *	OS definitions:
 *	_OS_WINDOWS
 *	_OS_LINUX
 *	_OS_FREEBSD
 *	_OS_BIOS
 */
 
#if !defined(IN)
	#define IN
#endif

#if !defined(OUT)
	#define OUT
#endif

#if defined(_OS_LINUX)
	#define BUFFER_PACKED		__attribute__((packed))
#elif defined(_OS_WINDOWS)
	#define BUFFER_PACKED
#elif defined(_OS_BIOS)
	#define BUFFER_PACKED
#endif

#define MV_BIT(x)			(1L << (x))

#if !defined(NULL)
#define NULL 0
#endif 

#define MV_TRUE							1
#define MV_FALSE						0

typedef unsigned char	MV_BOOLEAN, *MV_PBOOLEAN;
typedef unsigned char	MV_U8, *MV_PU8;
typedef signed char	MV_I8, *MV_PI8;

typedef unsigned short	MV_U16, *MV_PU16;
typedef signed short	MV_I16, *MV_PI16;

typedef void		MV_VOID, *MV_PVOID;

typedef void		*MV_LPVOID;

/* Pre-define segment in C code*/
#define BASEATTR 

/* For debug version only */
#define MV_DUMPC32(_x_)
#define MV_DUMPC16(_x_)
#define MV_DUMPC8(_x_)	
#define MV_DUMPE32(_x_) 
#define MV_DUMPE16(_x_) 
#define MV_DUMPE8(_x_) 
#define MV_DUMP32(_x_) 
#define MV_DUMP16(_x_)
#define MV_DUMP8(_x_)
#define MV_DUMPRUN(_x_) 
#define MV_HALTKEY
#define MV_ENTERLINE

	/*#include <linux/types.h>*/
	/** unsigned/signed long is 64bit for AMD64, so use unsigned int instead */
typedef unsigned int MV_U32, *MV_PU32;
typedef   signed int MV_I32, *MV_PI32;
typedef unsigned long MV_ULONG, *MV_PULONG;
typedef   signed long MV_ILONG, *MV_PILONG;

typedef unsigned long long _MV_U64;
typedef   signed long long _MV_I64;

#ifdef _64_SYS_
	#define _SUPPORT_64_BIT
#else
	#ifdef _SUPPORT_64_BIT
		#error Error 64_BIT CPU Macro
	#endif
#endif

/*
 * Primary Data Type
 */
/* Windows and Linux compiler supports 64 bit data structure. */
typedef union {
	struct {
		MV_U32 low;
		MV_U32 high;
	};
	_MV_U64 value;
} MV_U64, *PMV_U64;

/* PTR_INTEGER is necessary to convert between pointer and integer. */
#if defined(_SUPPORT_64_BIT)
	typedef _MV_U64 MV_PTR_INTEGER;
#else
	typedef MV_U32 MV_PTR_INTEGER;
#endif

/* LBA is the logical block access */
typedef MV_U64 MV_LBA;

#if defined(_CPU_16B)
	typedef MV_U32 MV_PHYSICAL_ADDR;
#else
	typedef MV_U64 MV_PHYSICAL_ADDR;
#endif

#endif /* COM_DEFINE_H */

