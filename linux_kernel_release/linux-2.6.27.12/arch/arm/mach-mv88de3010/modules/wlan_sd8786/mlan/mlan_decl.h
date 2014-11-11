/** @file mlan_decl.h
 *
 *  @brief This file declares the generic data structures and APIs.
 *
 *  Copyright (C) 2008-2009, Marvell International Ltd. 
 *  All Rights Reserved
 */

/******************************************************
Change log:
    11/07/2008: initial version
******************************************************/

#ifndef _MLAN_DECL_H_
#define _MLAN_DECL_H_

#include "asm/atomic.h"
/** MLAN release version */
#define MLAN_RELEASE_VERSION		"M2612043"

/** Re-define generic data types for MLAN/MOAL */
/** Signed char (1-byte) */
typedef char t_s8;
/** Unsigned char (1-byte) */
typedef unsigned char t_u8;
/** Signed short (2-bytes) */
typedef short t_s16;
/** Unsigned short (2-bytes) */
typedef unsigned short t_u16;
/** Signed long (4-bytes) */
typedef long t_s32;
/** Unsigned long (4-bytes) */
typedef unsigned long t_u32;
/** Signed long long 8-bytes) */
typedef long long t_s64;
/** Unsigned long long 8-bytes) */
typedef unsigned long long t_u64;
/** Void pointer (4-bytes) */
typedef void t_void;

/** Constants below */

#ifdef __GNUC__
/** Structure packing begins */
#define MLAN_PACK_START
/** Structure packeing end */
#define MLAN_PACK_END  __attribute__ ((packed))
#else
/** Structure packing begins */
#define MLAN_PACK_START   __packed
/** Structure packeing end */
#define MLAN_PACK_END
#endif

/** MLAN TRUE */
#define MTRUE                    (1)
/** MLAN FALSE */
#define MFALSE                   (0)

/** Macros for Data Alignment : size */
#define ALIGN_SZ(p, a)	\
	(((p) + ((a) - 1)) & ~((a) - 1))

/** Macros for Data Alignment : address */
#define ALIGN_ADDR(p, a)	\
	((((t_u32)(p)) + (((t_u32)(a)) - 1)) & ~(((t_u32)(a)) - 1))

/** Maximum BSS numbers */
#define MLAN_MAX_BSS_NUM         (1)

/* NET IP alignment */
#define MLAN_NET_IP_ALIGN        0

/** Minimum data header length */
#define MLAN_MIN_DATA_HEADER_LEN 32     // (sizeof(mlan_txpd))

/** This is current limit on Maximum AMPDU allowed */
#define MLAN_MAX_BASTREAM_SUPPORTED     2

/** Default Win size attached during ADDBA request */
#define MLAN_AMPDU_DEF_TXWINSIZE        64
/** Default Win size attached during ADDBA response */
#define MLAN_AMPDU_DEF_RXWINSIZE        64
/** Block ack timeout value */
#define MLAN_DEFAULT_BLOCK_ACK_TIMEOUT  0xffff

/** Rate index for HR/DSSS 0 */
#define MLAN_RATE_INDEX_HRDSSS0 0
/** Rate index for HR/DSSS 3 */
#define MLAN_RATE_INDEX_HRDSSS3 3
/** Rate index for OFDM 0 */
#define MLAN_RATE_INDEX_OFDM0   4
/** Rate index for OFDM 7 */
#define MLAN_RATE_INDEX_OFDM7   11
/** Rate index for MCS 0 */
#define MLAN_RATE_INDEX_MCS0    12
/** Rate index for MCS 7 */
#define MLAN_RATE_INDEX_MCS7    19
/** Rate index for MCS 32 */
#define MLAN_RATE_INDEX_MCS32   44
/** Rate index for MCS 127 */
#define MLAN_RATE_INDEX_MCS127  139

/** Rate bitmap for OFDM 0 */
#define MLAN_RATE_BITMAP_OFDM0  16
/** Rate bitmap for OFDM 7 */
#define MLAN_RATE_BITMAP_OFDM7  23
/** Rate bitmap for MCS 0 */
#define MLAN_RATE_BITMAP_MCS0   32
/** Rate bitmap for MCS 127 */
#define MLAN_RATE_BITMAP_MCS127 159

/** Size of rx data buffer */
#define MLAN_RX_DATA_BUF_SIZE     (4 * 1024)
/** Size of rx command buffer */
#define MLAN_RX_CMD_BUF_SIZE      (2 * 1024)

/** MLAN MAC Address Length */
#define MLAN_MAC_ADDR_LENGTH     (6)
/** MLAN 802.11 MAC Address */
typedef t_u8 mlan_802_11_mac_addr[MLAN_MAC_ADDR_LENGTH];

/** MLAN Maximum SSID Length */
#define MLAN_MAX_SSID_LENGTH     (32)

/** RTS/FRAG related defines */
/** Minimum RTS value */
#define MLAN_RTS_MIN_VALUE              (0)
/** Maximum RTS value */
#define MLAN_RTS_MAX_VALUE              (2347)
/** Minimum FRAG value */
#define MLAN_FRAG_MIN_VALUE             (256)
/** Maximum FRAG value */
#define MLAN_FRAG_MAX_VALUE             (2346)

/** Minimum tx retry count */
#define MLAN_TX_RETRY_MIN 		(0)
/** Maximum tx retry count */
#define MLAN_TX_RETRY_MAX 		(14)

/** define SDIO block size for firmware download */
#define MLAN_SDIO_BLOCK_SIZE_FW_DNLD	256

/** define SDIO block size for data Tx/Rx */
/* We support up to 480-byte block size due to FW buffer limitation. */
#define MLAN_SDIO_BLOCK_SIZE		256

/** define allocated buffer size */
#define ALLOC_BUF_SIZE           (4 * 1024)

/** Max retry number of CMD53 write */
#define MAX_WRITE_IOMEM_RETRY		2

/** SDIO IO Port mask */
#define MLAN_SDIO_IO_PORT_MASK		0xfffff
/** SDIO Block/Byte mode mask */
#define MLAN_SDIO_BYTE_MODE_MASK	0x80000000

/** IN parameter */
#define IN
/** OUT parameter */
#define OUT

/** BIT value */
#define MBIT(x)    (((t_u32)1) << (x))

/** Buffer flag for requeued packet */
#define MLAN_BUF_FLAG_REQUEUED_PKT      MBIT(0)

#ifdef DEBUG_LEVEL1
/** Debug level bit definition */
#define	MMSG        MBIT(0)
#define MFATAL      MBIT(1)
#define MERROR      MBIT(2)
#define MDATA       MBIT(3)
#define MCMND       MBIT(4)
#define MEVENT      MBIT(5)
#define MINTR       MBIT(6)

#define MDAT_D      MBIT(16)
#define MCMD_D      MBIT(17)
#define MFW_D       MBIT(18)

#define MENTRY      MBIT(28)
#define MWARN       MBIT(29)
#define MINFO       MBIT(30)
#define MHEX_DUMP   MBIT(31)
#endif /* DEBUG_LEVEL1 */

/** mlan_status */
typedef enum _mlan_status
{
    MLAN_STATUS_FAILURE = 0xffffffff,
    MLAN_STATUS_SUCCESS = 0,
    MLAN_STATUS_PENDING,
    MLAN_STATUS_RESOURCE,
} mlan_status;

/** mlan_error_code */
typedef enum _mlan_error_code
{
    /** No error */
    MLAN_ERROR_NO_ERROR = 0,
    /** Firmware errors below (MSB=0) */
    MLAN_ERROR_FW_NOT_READY = 0x00000001,
    MLAN_ERROR_FW_BUSY,
    MLAN_ERROR_FW_CMDRESP,
    /** Driver errors below (MSB=1) */
    MLAN_ERROR_PKT_SIZE_INVALID = 0x80000001,
    MLAN_ERROR_PKT_TIMEOUT,
    MLAN_ERROR_CMD_INVALID,
    MLAN_ERROR_CMD_TIMEOUT,
    MLAN_ERROR_CMD_DNLD_FAIL,
    MLAN_ERROR_CMD_CANCEL,
    MLAN_ERROR_ASSOC_FAIL,
    MLAN_ERROR_EVENT_UNKNOWN,
    /** More to add */
} mlan_error_code;

/** mlan_buf_type */
typedef enum _mlan_buf_type
{
    MLAN_BUF_TYPE_CMD = 1,
    MLAN_BUF_TYPE_DATA,
    MLAN_BUF_TYPE_EVENT,
} mlan_buf_type;

/** mlan_bss_type */
typedef enum _mlan_bss_type
{
    MLAN_BSS_TYPE_STA = 0,
    MLAN_BSS_TYPE_UAP,
    MLAN_BSS_TYPE_ANY = 0xff,
} mlan_bss_type;

/** mlan_data_frame_type */
typedef enum _mlan_data_frame_type
{
    MLAN_DATA_FRAME_TYPE_ETH_II = 0,
    MLAN_DATA_FRAME_TYPE_802_11,
} mlan_data_frame_type;

/** mlan_event_id */
typedef enum _mlan_event_id
{
    /* Event generated by firmware (MSB=0) */
    MLAN_EVENT_ID_FW_UNKNOWN = 0x00000001,
    MLAN_EVENT_ID_FW_ADHOC_LINK_SENSED,
    MLAN_EVENT_ID_FW_ADHOC_LINK_LOST,
    MLAN_EVENT_ID_FW_DISCONNECTED,
    MLAN_EVENT_ID_FW_MIC_ERR_UNI,
    MLAN_EVENT_ID_FW_MIC_ERR_MUL,
    MLAN_EVENT_ID_FW_BCN_RSSI_LOW,
    MLAN_EVENT_ID_FW_BCN_RSSI_HIGH,
    MLAN_EVENT_ID_FW_BCN_SNR_LOW,
    MLAN_EVENT_ID_FW_BCN_SNR_HIGH,
    MLAN_EVENT_ID_FW_MAX_FAIL,
    MLAN_EVENT_ID_FW_DATA_RSSI_LOW,
    MLAN_EVENT_ID_FW_DATA_RSSI_HIGH,
    MLAN_EVENT_ID_FW_DATA_SNR_LOW,
    MLAN_EVENT_ID_FW_DATA_SNR_HIGH,
    MLAN_EVENT_ID_FW_LINK_QUALITY,
    MLAN_EVENT_ID_FW_PORT_RELEASE,
    MLAN_EVENT_ID_FW_PRE_BCN_LOST,
    MLAN_EVENT_ID_FW_WMM_CONFIG_CHANGE,
    MLAN_EVENT_ID_FW_HS_WAKEUP,
    MLAN_EVENT_ID_FW_DS_AWAKE,
    MLAN_EVENT_ID_FW_BG_SCAN,
    MLAN_EVENT_ID_FW_WEP_ICV_ERR,
    MLAN_EVENT_ID_FW_BW_CHANGED,

    /* Event generated by MLAN driver (MSB=1) */
    MLAN_EVENT_ID_DRV_CONNECTED = 0x80000001,
    MLAN_EVENT_ID_DRV_DEFER_HANDLING,
    MLAN_EVENT_ID_DRV_HS_ACTIVATED,
    MLAN_EVENT_ID_DRV_HS_DEACTIVATED,
    MLAN_EVENT_ID_DRV_OBSS_SCAN_PARAM,
    MLAN_EVENT_ID_DRV_PASSTHU,
} mlan_event_id;

/** Data Structures */
/** mlan_image data structure */
typedef struct _mlan_fw_image
{
    /** Helper image buffer pointer */
    t_u8 *phelper_buf;
    /** Helper image length */
    t_u32 helper_len;
    /** Firmware image buffer pointer */
    t_u8 *pfw_buf;
    /** Firmware image length */
    t_u32 fw_len;
} mlan_fw_image, *pmlan_fw_image;

/** mlan_event data structure */
typedef struct _mlan_event
{
    /** BSS index number for multiple BSS support */
    t_u32 bss_num;
    /** Event ID */
    mlan_event_id event_id;
    /** Event length */
    t_u32 event_len;
    /** Event buffer */
    t_u8 event_buf[1];
} mlan_event, *pmlan_event;

/** mlan_ioctl_req data structure */
typedef struct _mlan_ioctl_req
{
    /** Status code from firmware/driver */
    t_u32 status_code;
    /** BSS index number for multiple BSS support */
    t_u32 bss_num;
    /** Request id */
    t_u32 req_id;
    /** Action: set or get */
    t_u32 action;
    /** Pointer to buffer */
    t_u8 *pbuf;
    /** Length of buffer */
    t_u32 buf_len;
    /** Length of the data read/written in buffer */
    t_u32 data_read_written;
    /** Length of buffer needed */
    t_u32 buf_len_needed;
    /** Reserved for MOAL module */
    t_u32 reserved_1;
} mlan_ioctl_req, *pmlan_ioctl_req;

/** mlan_buffer data structure */
typedef struct _mlan_buffer
{
    /** Pointer to previous mlan_buffer */
    struct _mlan_buffer *pprev;
    /** Pointer to next mlan_buffer */
    struct _mlan_buffer *pnext;
    /** Status code from firmware/driver */
    t_u32 status_code;
    /** Flags for this buffer */
    t_u32 flags;
    /** BSS number */
    t_u32 bss_num;
    /** Buffer descriptor, e.g. skb in Linux */
    t_void *pdesc;
    /** Pointer to buffer */
    t_u8 *pbuf;
    /** Offset to data */
    t_u32 data_offset;
    /** Data length */
    t_u32 data_len;
    /** Buffer type: data, cmd, event etc. */
    mlan_buf_type buf_type;

    /** Fields below are valid for data packet only */
    /** QoS priority */
    t_u32 priority;
    /** Time stamp when packet is received (seconds) */
    t_u32 in_ts_sec;
    /** Time stamp when packet is received (micro seconds) */
    t_u32 in_ts_usec;
    /** Time stamp when packet is processed (seconds) */
    t_u32 out_ts_sec;
    /** Time stamp when packet is processed (micro seconds) */
    t_u32 out_ts_usec;

    /** Fields below are valid for MLAN module only */
    /** Pointer to parent mlan_buffer */
    struct _mlan_buffer *pparent;
    /** Use count for this buffer */
    t_u32 use_count;
} mlan_buffer, *pmlan_buffer;

/** mlan_bss_attr data structure */
typedef struct _mlan_bss_attr
{
    /** BSS type */
    t_u32 bss_type;
    /** Data frame type: Ethernet II, 802.11, etc. */
    t_u32 frame_type;
    /** The BSS is active (non-0) or not (0). */
    t_u32 active;
    /** BSS Priority */
    t_u32 bss_priority;
} mlan_bss_attr, *pmlan_bss_attr;

/** mlan_callbacks data structure */
typedef struct _mlan_callbacks
{
    /** moal_init_fw_complete */
    mlan_status(*moal_init_fw_complete) (IN t_void * pmoal_handle,
                                         IN mlan_status status);
    /** moal_shutdown_fw_complete */
    mlan_status(*moal_shutdown_fw_complete) (IN t_void * pmoal_handle,
                                             IN mlan_status status);
    /** moal_send_packet_complete */
    mlan_status(*moal_send_packet_complete) (IN t_void * pmoal_handle,
                                             IN pmlan_buffer pmbuf,
                                             IN mlan_status status);
    /** moal_recv_complete */
    mlan_status(*moal_recv_complete) (IN t_void * pmoal_handle,
                                      IN pmlan_buffer pmbuf,
                                      IN t_u32 port, IN mlan_status status);
    /** moal_recv_packet */
    mlan_status(*moal_recv_packet) (IN t_void * pmoal_handle,
                                    IN pmlan_buffer pmbuf);
    /** moal_recv_event */
    mlan_status(*moal_recv_event) (IN t_void * pmoal_handle,
                                   IN pmlan_event pmevent);
    /** moal_ioctl_complete */
    mlan_status(*moal_ioctl_complete) (IN t_void * pmoal_handle,
                                       IN pmlan_ioctl_req pioctl_req,
                                       IN mlan_status status);
    /** moal_alloc_mlan_buffer */
    mlan_status(*moal_alloc_mlan_buffer) (IN t_u32 size,
                                          OUT pmlan_buffer * pmbuf);
    /** moal_free_mlan_buffer */
    mlan_status(*moal_free_mlan_buffer) (IN pmlan_buffer pmbuf);
    /** moal_write_reg */
    mlan_status(*moal_write_reg) (IN t_void * pmoal_handle,
                                  IN t_u32 reg, IN t_u32 data);
    /** moal_read_reg */
    mlan_status(*moal_read_reg) (IN t_void * pmoal_handle,
                                 IN t_u32 reg, OUT t_u32 * data);
    /** moal_write_data_sync */
    mlan_status(*moal_write_data_sync) (IN t_void * pmoal_handle,
                                        IN pmlan_buffer pmbuf,
                                        IN t_u32 port, IN t_u32 timeout);
    /** moal_read_data_sync */
    mlan_status(*moal_read_data_sync) (IN t_void * pmoal_handle,
                                       IN OUT pmlan_buffer pmbuf,
                                       IN t_u32 port, IN t_u32 timeout);
    /** moal_malloc */
    mlan_status(*moal_malloc) (IN t_u32 size, OUT t_u8 ** ppbuf);
    /** moal_mfree */
    mlan_status(*moal_mfree) (IN t_u8 * pbuf);
    /** moal_memset */
    t_void *(*moal_memset) (IN t_void * pmem, IN t_u8 byte, IN t_u32 num);
    /** moal_memcpy */
    t_void *(*moal_memcpy) (IN t_void * pdest,
                            IN const t_void * psrc, IN t_u32 num);
    /** moal_memmove */
    t_void *(*moal_memmove) (IN t_void * pdest,
                             IN const t_void * psrc, IN t_u32 num);
    /** moal_memcmp */
      t_s32(*moal_memcmp) (IN const t_void * pmem1,
                           IN const t_void * pmem2, IN t_u32 num);
    /** moal_get_system_time */
      mlan_status(*moal_get_system_time) (OUT t_u32 * psec, OUT t_u32 * pusec);
    /** moal_init_timer*/
      mlan_status(*moal_init_timer) (OUT t_void ** pptimer,
                                     IN t_void(*callback) (t_void * pcontext),
                                     IN t_void * pcontext);
    /** moal_free_timer */
      mlan_status(*moal_free_timer) (IN t_void * ptimer);
    /** moal_start_timer*/
      mlan_status(*moal_start_timer) (IN t_void * ptimer,
                                      IN t_u8 periodic, IN t_u32 msec);
    /** moal_stop_timer*/
      mlan_status(*moal_stop_timer) (IN t_void * ptimer);
    /** moal_init_lock */
      mlan_status(*moal_init_lock) (OUT t_void ** pplock);
    /** moal_free_lock */
      mlan_status(*moal_free_lock) (IN t_void * plock);
    /** moal_spin_lock */
      mlan_status(*moal_spin_lock) (IN t_void * plock);
    /** moal_spin_unlock */
      mlan_status(*moal_spin_unlock) (IN t_void * plock);
    /** moal_print */
      t_void(*moal_print) (IN t_u32 level, IN t_s8 * pformat, IN ...);

      /** moal_get_cardregs  **/
    mlan_status(*moal_get_cardregs) (IN t_void * pmoal_handle, IN t_u8 *reg);

    atomic_t * int_flag;
} mlan_callbacks, *pmlan_callbacks;

/** mlan_device data structure */
typedef struct _mlan_device
{
    /** MOAL Handle */
    t_void *pmoal_handle;
    /** BSS Attributes */
    mlan_bss_attr bss_attr[MLAN_MAX_BSS_NUM];
    /** Callbacks */
    mlan_callbacks callbacks;
#ifdef MFG_CMD_SUPPORT
    /** MFG mode */
    t_u32 mfg_mode;
#endif
    /** SDIO interrupt mode */
    t_u32 int_mode;
    /** GPIO interrupt pin */
    t_u32 gpio_pin;

} mlan_device, *pmlan_device;

/** MLAN API function prototype */
#define MLAN_API

/** Registration */
MLAN_API mlan_status mlan_register(IN pmlan_device pmdevice,
                                   OUT t_void ** ppmlan_adapter);

/** Un-registration */
MLAN_API mlan_status mlan_unregister(IN t_void * pmlan_adapter);

/** Firmware Downloading */
MLAN_API mlan_status mlan_dnld_fw(IN t_void * pmlan_adapter,
                                  IN pmlan_fw_image pmfw);

/** Firmware Initialization */
MLAN_API mlan_status mlan_init_fw(IN t_void * pmlan_adapter);

/** Firmware Shutdown */
MLAN_API mlan_status mlan_shutdown_fw(IN t_void * pmlan_adapter);

/** Main Process */
MLAN_API mlan_status mlan_main_process(IN t_void * pmlan_adapter);

/** Interrupt helper, do read/write in SDIO softirq */
MLAN_API mlan_status wlan_process_int_status(IN t_void * pmlan_adapter);

/** Packet Transmission */
MLAN_API mlan_status mlan_send_packet(IN t_void * pmlan_adapter,
                                      IN pmlan_buffer pmbuf);

/** interrupt handler */
MLAN_API t_void mlan_interrupt(IN t_void * pmlan_adapter);

/** GPIO IRQ callback function */
MLAN_API t_void mlan_hs_callback(IN t_void * pctx);

/** mlan ioctl */
MLAN_API mlan_status mlan_ioctl(IN t_void * pmlan_adapter,
                                IN pmlan_ioctl_req pioctl_req);

#endif /* !_MLAN_DECL_H_ */
