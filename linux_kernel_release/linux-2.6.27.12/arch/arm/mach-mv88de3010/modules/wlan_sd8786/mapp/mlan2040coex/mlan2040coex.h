/** @file  mlan2040coex.h
  *
  * @brief This file contains definitions for application
  * 
  *  Copyright (C) 2009, Marvell International Ltd.
  *  All Rights Reserved
  */
/************************************************************************
Change log:
     06/24/2009: initial version
************************************************************************/
#ifndef _COEX_H_
#define _COEX_H_

#if (BYTE_ORDER == LITTLE_ENDIAN)
#undef BIG_ENDIAN
#endif

/** 16 bits byte swap */
#define swap_byte_16(x) \
((t_u16)((((t_u16)(x) & 0x00ffU) << 8) | \
         (((t_u16)(x) & 0xff00U) >> 8)))

/** 32 bits byte swap */
#define swap_byte_32(x) \
((t_u32)((((t_u32)(x) & 0x000000ffUL) << 24) | \
         (((t_u32)(x) & 0x0000ff00UL) <<  8) | \
         (((t_u32)(x) & 0x00ff0000UL) >>  8) | \
         (((t_u32)(x) & 0xff000000UL) >> 24)))

/** Convert to correct endian format */
#ifdef 	BIG_ENDIAN
/** CPU to little-endian convert for 16-bit */
#define 	cpu_to_le16(x)	swap_byte_16(x)
/** CPU to little-endian convert for 32-bit */
#define		cpu_to_le32(x)  swap_byte_32(x)
/** Little-endian to CPU convert for 16-bit */
#define 	le16_to_cpu(x)	swap_byte_16(x)
/** Little-endian to CPU convert for 32-bit */
#define		le32_to_cpu(x)  swap_byte_32(x)
#else
/** Do nothing */
#define		cpu_to_le16(x)	(x)
/** Do nothing */
#define		cpu_to_le32(x)  (x)
/** Do nothing */
#define 	le16_to_cpu(x)	(x)
/** Do nothing */
#define 	le32_to_cpu(x)	(x)
#endif

/** Character, 1 byte */
typedef char t_s8;
/** Unsigned character, 1 byte */
typedef unsigned char t_u8;

/** Short integer */
typedef signed short t_s16;
/** Unsigned short integer */
typedef unsigned short t_u16;

/** Long integer */
typedef signed long t_s32;
/** Unsigned long integer */
typedef unsigned long t_u32;

/** Long long integer */
typedef signed long long t_s64;
/** Unsigned long long integer */
typedef unsigned long long t_u64;

/** Void pointer (4-bytes) */
typedef void t_void;

#ifdef FALSE
#undef FALSE
#endif

#ifdef TRUE
#undef TRUE
#endif

#ifndef MIN
/** Find minimum value */
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif /* MIN */

/** Type definition: boolean */
typedef enum
{ FALSE, TRUE } boolean;

/** Find number of elements */
#define NELEMENTS(x) (sizeof(x)/sizeof(x[0]))
/** Success */
#define MLAN_STATUS_SUCCESS         (0)
/** Failure */
#define MLAN_STATUS_FAILURE         (-1)

/** Enumeration for host-command index */
enum COMMANDS
{
    CMD_2040COEX = 1,
};
/** Maximum number of channels that can be sent in a setuserscan ioctl */
#define WLAN_IOCTL_USER_SCAN_CHAN_MAX  50

/** Structure defination of chan_intol_t*/
typedef struct _chan_intol_t
{
    /** Channel numer */
    t_u8 chan_num;
    /** Flag: Is any 40MHz intolerant AP found in this channel */
    t_u8 is_intol_set;
} chan_intol_t;

/** Legacy APs channel list */
chan_intol_t leg_ap_chan_list[WLAN_IOCTL_USER_SCAN_CHAN_MAX];
/** Total number of channel present in Legacy APs channel list */
t_u8 num_leg_ap_chan;
int get_region_code(int *reg_code);
int process_host_cmd(int cmd, t_u8 * chan_list, t_u8 chan_num, t_u8 reg_class,
                     t_u8 is_intol_ap_present);
int is_intolerant_sta(int *intol);

#endif /* _COEX_H_ */
