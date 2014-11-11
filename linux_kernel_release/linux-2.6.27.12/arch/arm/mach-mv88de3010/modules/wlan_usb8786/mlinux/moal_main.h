/** @file moal_main.h
  *
  * @brief This file contains wlan driver specific defines etc.
  * 
  * Copyright (C) 2008-2009, Marvell International Ltd.  
  * All Rights Reserved
  */

/********************************************************
Change log:
    10/21/2008: initial version
********************************************************/

#ifndef _MOAL_MAIN_H
#define _MOAL_MAIN_H

/* warnfix for FS redefination if any? */
#ifdef FS
#undef FS
#endif

/* Linux header files */
#include        <linux/kernel.h>
#include        <linux/module.h>
#include        <linux/init.h>
#include        <linux/version.h>
#include        <linux/param.h>
#include        <linux/delay.h>
#include        <linux/slab.h>
#include        <linux/mm.h>
#include        <linux/types.h>
#include        <linux/sched.h>
#include        <linux/timer.h>
#include        <linux/ioport.h>
#include        <linux/pci.h>
#include        <linux/ctype.h>
#include        <linux/proc_fs.h>
#include	<linux/ptrace.h>
#include	<linux/string.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,18)
#include       <linux/config.h>
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,22)
#include	    <linux/freezer.h>
#endif
#include	    <linux/usb.h>

/* ASM files */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27)
#include        <linux/semaphore.h>
#else
#include        <asm/semaphore.h>
#endif
#include        <asm/byteorder.h>
#include        <asm/irq.h>
#include        <asm/uaccess.h>
#include        <asm/io.h>
#include        <asm/system.h>

/* Net header files */
#include        <linux/wireless.h>
#include        <linux/netdevice.h>
#include        <linux/net.h>
#include        <linux/ip.h>
#include        <linux/skbuff.h>
#include        <linux/if_arp.h>
#include        <linux/if_ether.h>
#include        <linux/etherdevice.h>
#include        <net/arp.h>
#include        <linux/rtnetlink.h>

#include	<linux/firmware.h>

/* Wireless header */
#include        <net/iw_handler.h>
#include        "mlan.h"
#include 	"moal_shim.h"
#include	"moal_wext.h"
#include	"moal_priv.h"

/** Define BOOLEAN */
typedef t_u8 BOOLEAN;

/** Driver version */
extern const char driver_version[];

/** Private structure for MOAL */
typedef struct _moal_private moal_private;
/** Handle data structure for MOAL  */
typedef struct _moal_handle moal_handle;

/** Hardware status codes */
typedef enum _MOAL_HARDWARE_STATUS
{
    HardwareStatusReady,
    HardwareStatusInitializing,
    HardwareStatusFwReady,
    HardwareStatusReset,
    HardwareStatusClosing,
    HardwareStatusNotReady
} MOAL_HARDWARE_STATUS;

/** moal_wait_option */
enum
{
    MOAL_NO_WAIT,
    MOAL_IOCTL_WAIT,
    MOAL_CMD_WAIT,
    MOAL_PROC_WAIT,
    MOAL_WSTATS_WAIT
};

#ifndef MIN
/** Find minimum */
#define MIN(a,b)		((a) < (b) ? (a) : (b))
#endif

/* NET IP alignment */
#define MLAN_NET_IP_ALIGN        0

/** moal_802_11_rates  */
typedef struct _moal_802_11_rates
{
        /** Num of rates */
    t_u8 num_of_rates;
        /** Rates */
    t_u8 rates[MLAN_SUPPORTED_RATES];
} moal_802_11_rates;

/*
 * OS timer specific
 */

/** Timer structure */
typedef struct _moal_drv_timer
{
        /** Timer list */
    struct timer_list tl;
        /** Timer function */
    void (*timer_function) (void *context);
        /** Timer function context */
    void *function_context;
        /** Time period */
    t_u32 time_period;
        /** Is timer periodic ? */
    t_u32 timer_is_periodic;
        /** Is timer cancelled ? */
    t_u32 timer_is_canceled;
} moal_drv_timer, *pmoal_drv_timer;

/** 
 *  @brief Timer handler
 *  
 *  @param fcontext	Timer context
 *
 *  @return		N/A
 */
static inline void
woal_timer_handler(unsigned long fcontext)
{
    pmoal_drv_timer timer = (pmoal_drv_timer) fcontext;

    timer->timer_function(timer->function_context);

    if (timer->timer_is_periodic == MTRUE) {
        mod_timer(&timer->tl, jiffies + ((timer->time_period * HZ) / 1000));
    }
}

/** 
 *  @brief Initialize timer
 *  
 *  @param timer		Timer structure
 *  @param TimerFunction	Timer function
 *  @param FunctionContext	Timer function context
 *
 *  @return			N/A
 */
static inline void
woal_initialize_timer(pmoal_drv_timer timer,
                      void (*TimerFunction) (void *context),
                      void *FunctionContext)
{
    /* First, setup the timer to trigger the wlan_timer_handler proxy */
    init_timer(&timer->tl);
    timer->tl.function = woal_timer_handler;
    timer->tl.data = (t_u32) timer;

    /* Then tell the proxy which function to call and what to pass it */
    timer->timer_function = TimerFunction;
    timer->function_context = FunctionContext;
    timer->timer_is_canceled = MTRUE;
    timer->time_period = 0;
    timer->timer_is_periodic = MFALSE;
}

/** 
 *  @brief Modify timer
 *  
 *  @param timer		Timer structure
 *  @param MillisecondPeriod	Time period in millisecond
 *
 *  @return			N/A
 */
static inline void
woal_mod_timer(pmoal_drv_timer timer, t_u32 MillisecondPeriod)
{
    timer->time_period = MillisecondPeriod;
    mod_timer(&timer->tl, jiffies + (MillisecondPeriod * HZ) / 1000);
    timer->timer_is_canceled = MFALSE;
}

/** 
 *  @brief Cancel timer
 *  
 *  @param timer	Timer structure
 *
 *  @return		N/A
 */
static inline void
woal_cancel_timer(moal_drv_timer * timer)
{
    del_timer(&timer->tl);
    timer->timer_is_canceled = MTRUE;
    timer->time_period = 0;
}

/*
 * OS Thread Specific
 */

#include	<linux/kthread.h>

/** Kernel thread structure */
typedef struct _moal_thread
{
    /** Task control structrue */
    struct task_struct *task;
    /** Pointer to wait_queue_head */
    wait_queue_head_t wait_q;
    /** PID */
    pid_t pid;
    /** Pointer to moal_handle */
    void *handle;
} moal_thread;

/** 
 *  @brief Activate thread
 *  
 *  @param thr			Thread structure
 *  @return			N/A
 */
static inline void
woal_activate_thread(moal_thread * thr)
{
    /** Record the thread pid */
    thr->pid = current->pid;

    /** Initialize the wait queue */
    init_waitqueue_head(&thr->wait_q);
}

/** 
 *  @brief De-activate thread
 *  
 *  @param thr			Thread structure
 *  @return			N/A
 */
static inline void
woal_deactivate_thread(moal_thread * thr)
{
    /* Reset the pid */
    thr->pid = 0;
}

/** 
 *  @brief Create and run the thread
 *
 *  @param threadfunc		Thread function
 *  @param thr			Thread structure
 *  @param name			Thread name
 *  @return			N/A
 */
static inline void
woal_create_thread(int (*threadfunc) (void *), moal_thread * thr, char *name)
{
    /* Create and run the thread */
    thr->task = kthread_run(threadfunc, thr, "%s", name);
}

/** 
 *  @brief Schedule timeout
 *  
 *  @param millisec	Timeout duration in milli second
 *
 *  @return		N/A
 */
static inline void
woal_sched_timeout(t_u32 millisec)
{
    set_current_state(TASK_INTERRUPTIBLE);

    schedule_timeout((millisec * HZ) / 1000);
}

#ifndef __ATTRIB_ALIGN__
#define __ATTRIB_ALIGN__ __attribute__((aligned(4)))
#endif

#ifndef __ATTRIB_PACK__
#define __ATTRIB_PACK__ __attribute__ ((packed))
#endif

/** Get module */
#define MODULE_GET	try_module_get(THIS_MODULE)
/** Put module */
#define MODULE_PUT	module_put(THIS_MODULE)

/** Initialize semaphore */
#define MOAL_INIT_SEMAPHORE(x)    	init_MUTEX(x)
/** Acquire semaphore and with blocking */
#define MOAL_ACQ_SEMAPHORE_BLOCK(x)	down_interruptible(x)
/** Acquire semaphore without blocking */
#define MOAL_ACQ_SEMAPHORE_NOBLOCK(x)	down_trylock(x)
/** Release semaphore */
#define MOAL_REL_SEMAPHORE(x) 		up(x)

/** Max loop count (* 100ms) for waiting device ready at init time */
#define MAX_WAIT_DEVICE_READY_COUNT	50

/** Default watchdog timeout */
#define MRVDRV_DEFAULT_WATCHDOG_TIMEOUT (5 * HZ)

/** 10 seconds */
#define MOAL_TIMER_10S                10000
/** 5 seconds */
#define MOAL_TIMER_5S                 5000
/** 1 second */
#define MOAL_TIMER_1S                 1000

/** RSSI-related defines */
/*	RSSI constants are used to implement 802.11 RSSI threshold 
*	indication. if the Rx packet signal got too weak for 5 consecutive
*	times, miniport driver (driver) will report this event to wrapper
*/

/** NF value for default scan */
#define MRVDRV_NF_DEFAULT_SCAN_VALUE		(-96)

/** MAX PENDING Tx data */
#define MAX_TX_PENDING    	60

/** wait_queue structure */
typedef struct _wait_queue
{
        /** Pointer to wait_queue_head */
    wait_queue_head_t *wait;
        /** Pointer to wait condition */
    t_u16 *condition;
        /** Start time */
    t_u32 start_time;
        /** Status from MLAN */
    mlan_status status;
} wait_queue, *pwait_queue;

/** Auto Rate */
#define AUTO_RATE 0xFF
/** Private structure for MOAL */
struct _moal_private
{
        /** Handle structure */
    moal_handle *phandle;
        /** Tx packet pending count in mlan */
    atomic_t tx_pending;
        /** IOCTL pending count in mlan */
    t_u32 ioctl_pending;
        /** Tx timeout count */
    t_u32 num_tx_timeout;
        /** BSS number */
    t_u8 bss_num;
        /** BSS type */
    t_u8 bss_type;
        /** MAC address information */
    t_u8 current_addr[ETH_ALEN];
        /** Media connection status */
    BOOLEAN media_connected;
        /** Nickname */
    t_u8 nick_name[16];
        /** AdHoc link sensed flag */
    BOOLEAN is_adhoc_link_sensed;
        /** Net device pointer */
    struct net_device *netdev;
        /** Net device statistics structure */
    struct net_device_stats stats;
        /** IW statistics */
    struct iw_statistics w_stats;
        /** IOCTL wait queue token */
    t_u16 ioctl_wait_q_woken;
        /** ioctl wait queue */
    wait_queue_head_t ioctl_wait_q __ATTRIB_ALIGN__;
        /** Cmd wait queue token */
    t_u16 cmd_wait_q_woken;
        /** IOCTL wait queue */
    wait_queue_head_t cmd_wait_q __ATTRIB_ALIGN__;
        /** w_stats wait queue token */
    t_u16 w_stats_wait_q_woken;
        /** w_stats wait queue */
    wait_queue_head_t w_stats_wait_q __ATTRIB_ALIGN__;
#ifdef CONFIG_PROC_FS
        /** Proc entry */
    struct proc_dir_entry *proc_entry;
        /** Proc entry name */
    t_s8 proc_entry_name[IFNAMSIZ];
        /** Proc wait queue token */
    t_u16 proc_wait_q_woken;
        /** IOCTL wait queue */
    wait_queue_head_t proc_wait_q __ATTRIB_ALIGN__;
#endif                          /* CONFIG_PROC_FS */
        /** Current WEP key index */
    t_u16 current_key_index;
#ifdef REASSOCIATION
    mlan_ssid_bssid prev_ssid_bssid;
        /** Re-association required */
    BOOLEAN reassoc_required;
#endif
        /** Rate index */
    t_u16 rate_index;
};

/** Handle data structure for MOAL */
struct _moal_handle
{
        /** MLAN adapter structure */
    t_void *pmlan_adapter;
        /** Private pointer */
    moal_private *priv[MLAN_MAX_BSS_NUM];
        /** Priv number */
    t_u8 priv_num;
#ifdef CONFIG_PROC_FS
        /** Proc top level directory entry */
    struct proc_dir_entry *proc_mwlan;
#endif
        /** Firmware */
    const struct firmware *firmware;
        /** Hotplug device */
    struct device *hotplug_device;
        /** STATUS variables */
    MOAL_HARDWARE_STATUS hardware_status;
        /** POWER MANAGEMENT AND PnP SUPPORT */
    BOOLEAN surprise_removed;
        /** Firmware release number */
    t_u32 fw_release_number;
        /** Init wait queue token */
    t_u16 init_wait_q_woken;
        /** Init wait queue */
    wait_queue_head_t init_wait_q __ATTRIB_ALIGN__;
        /** Card pointer */
    t_void *card;
        /** Rx pending in MLAN */
    atomic_t rx_pending;
        /** Malloc count */
    t_u32 malloc_count;
        /** lock count */
    t_u32 lock_count;
#ifdef REASSOCIATION
        /** Re-association thread */
    moal_thread reassoc_thread;
        /** Re-association timer set flag */
    BOOLEAN is_reassoc_timer_set;
        /** Re-association timer */
    moal_drv_timer reassoc_timer __ATTRIB_ALIGN__;
        /**  */
    struct semaphore reassoc_sem;
        /** Flag of re-association on/off */
    BOOLEAN reassoc_on;
#endif
};

/** Debug Macro definition*/
#ifdef	DEBUG_LEVEL1
extern t_u32 drvdbg;
extern t_u32 ifdbg;
/** Debug message control bit definition for drvdbg */
#define	DBG_MSG		MBIT(0)
#define DBG_FATAL	MBIT(1)
#define DBG_ERROR	MBIT(2)
#define DBG_DATA	MBIT(3)
#define DBG_CMND	MBIT(4)
#define DBG_EVENT	MBIT(5)
#define DBG_INTR	MBIT(6)

#define DBG_DAT_D	MBIT(16)
#define DBG_CMD_D	MBIT(17)
#define DBG_FW_D	MBIT(18)

#define DBG_ENTRY	MBIT(28)
#define DBG_WARN	MBIT(29)
#define DBG_INFO	MBIT(30)
#define DBG_HEX_DUMP	MBIT(31)

/** Debug message control bit definition for ifdbg */
#define DBG_IF_D	MBIT(0)

#ifdef	DEBUG_LEVEL2
#define	PRINTM_INFO(msg...)  do {if (drvdbg & DBG_INFO) printk(KERN_DEBUG msg);} while(0)
#define	PRINTM_WARN(msg...)  do {if (drvdbg & DBG_WARN) printk(KERN_DEBUG msg);} while(0)
#define	PRINTM_ENTRY(msg...) do {if (drvdbg & DBG_ENTRY) printk(KERN_DEBUG msg);} while(0)
#else
#define	PRINTM_INFO(msg...)  do {} while (0)
#define	PRINTM_WARN(msg...)  do {} while (0)
#define	PRINTM_ENTRY(msg...) do {} while (0)
#endif /* DEBUG_LEVEL2 */

#define	PRINTM_FW_D(msg...) do {if (drvdbg & DBG_FW_D) printk(KERN_DEBUG msg);} while(0)
#define	PRINTM_CMD_D(msg...) do {if (drvdbg & DBG_CMD_D) printk(KERN_DEBUG msg);} while(0)
#define	PRINTM_DAT_D(msg...) do {if (drvdbg & DBG_DAT_D) printk(KERN_DEBUG msg);} while(0)

#define	PRINTM_INTR(msg...)  do {if (drvdbg & DBG_INTR) printk(KERN_DEBUG msg);} while(0)
#define	PRINTM_EVENT(msg...) do {if (drvdbg & DBG_EVENT) printk(msg);} while(0)
#define	PRINTM_CMND(msg...)  do {if (drvdbg & DBG_CMND) printk(KERN_DEBUG msg);} while(0)
#define	PRINTM_DATA(msg...)  do {if (drvdbg & DBG_DATA) printk(KERN_DEBUG msg);} while(0)
#define	PRINTM_ERROR(msg...) do {if (drvdbg & DBG_ERROR) printk(KERN_DEBUG msg);} while(0)
#define	PRINTM_FATAL(msg...) do {if (drvdbg & DBG_FATAL) printk(KERN_DEBUG msg);} while(0)
#define	PRINTM_MSG(msg...)   do {if (drvdbg & DBG_MSG) printk(KERN_ALERT msg);} while(0)

#define	PRINTM_IF_D(msg...) do {if (ifdbg & DBG_IF_D) printk(KERN_DEBUG msg);} while(0)

#define	PRINTM(level,msg...) PRINTM_##level(msg)

#else

#define	PRINTM(level,msg...) do {} while (0)

#endif /* DEBUG_LEVEL1 */

/** Wait until a condition becomes true */
#define ASSERT(cond)						\
do {								\
	if (!(cond))						\
		PRINTM(INFO, "ASSERT: %s:%i\n", __FUNCTION__);	\
} while(0)

/** Log entry point for debugging */
#define	ENTER()			PRINTM(ENTRY, "Enter: %s\n", \
                                    __FUNCTION__)
/** Log exit point for debugging */
#define	LEAVE()			PRINTM(ENTRY, "Leave: %s\n", \
                                    __FUNCTION__)

#ifdef DEBUG_LEVEL1
#define DBG_DUMP_BUF_LEN 	64
#define MAX_DUMP_PER_LINE	16

static inline void
hexdump(char *prompt, t_u8 * buf, int len)
{
    int i;
    char dbgdumpbuf[DBG_DUMP_BUF_LEN];
    char *ptr = dbgdumpbuf;

    printk(KERN_DEBUG "%s:\n", prompt);
    for (i = 1; i <= len; i++) {
        ptr += sprintf(ptr, "%02x ", *buf);
        buf++;
        if (i % MAX_DUMP_PER_LINE == 0) {
            *ptr = 0;
            printk(KERN_DEBUG "%s\n", dbgdumpbuf);
            ptr = dbgdumpbuf;
        }
    }
    if (len % MAX_DUMP_PER_LINE) {
        *ptr = 0;
        printk(KERN_DEBUG "%s\n", dbgdumpbuf);
    }
}

#define DBG_HEXDUMP_CMD_D(x,y,z)    do {if (drvdbg & DBG_CMD_D) hexdump(x,y,z);} while(0)
#define DBG_HEXDUMP_DAT_D(x,y,z)    do {if (drvdbg & DBG_DAT_D) hexdump(x,y,z);} while(0)
#define DBG_HEXDUMP_IF_D(x,y,z)     do {if (ifdbg & DBG_IF_D) hexdump(x,y,z);} while(0)
#define DBG_HEXDUMP_FW_D(x,y,z)     do {if (drvdbg & DBG_FW_D) hexdump(x,y,z);} while(0)
#define	DBG_HEXDUMP(level,x,y,z)    DBG_HEXDUMP_##level(x,y,z)

#else
/** Do nothing since debugging is not turned on */
#define DBG_HEXDUMP(level,x,y,z)    do {} while (0)
#endif

#ifdef DEBUG_LEVEL2
#define HEXDUMP(x,y,z)              do {if (drvdbg & DBG_INFO) hexdump(x,y,z);} while(0)
#else
/** Do nothing since debugging is not turned on */
#define HEXDUMP(x,y,z)              do {} while (0)
#endif

#ifdef BIG_ENDIAN
/** Convert from 16 bit little endian format to CPU format */
#define woal_le16_to_cpu(x) le16_to_cpu(x)
/** Convert from 32 bit little endian format to CPU format */
#define woal_le32_to_cpu(x) le32_to_cpu(x)
/** Convert from 64 bit little endian format to CPU format */
#define woal_le64_to_cpu(x) le64_to_cpu(x)
/** Convert to 16 bit little endian format from CPU format */
#define woal_cpu_to_le16(x) cpu_to_le16(x)
/** Convert to 32 bit little endian format from CPU format */
#define woal_cpu_to_le32(x) cpu_to_le32(x)
/** Convert to 64 bit little endian format from CPU format */
#define woal_cpu_to_le64(x) cpu_to_le64(x)
#else
/** Do nothing */
#define woal_le16_to_cpu(x) x
/** Do nothing */
#define woal_le32_to_cpu(x) x
/** Do nothing */
#define woal_le64_to_cpu(x) x
/** Do nothing */
#define woal_cpu_to_le16(x) x
/** Do nothing */
#define woal_cpu_to_le32(x) x
/** Do nothing */
#define woal_cpu_to_le64(x) x
#endif

/** Allocate buffer */
pmlan_buffer woal_alloc_mlan_buffer(int size);
/** Allocate IOCTL request buffer */
pmlan_ioctl_req woal_alloc_mlan_ioctl_req(int size);
/** Allocate IOCTL request buffer without wait functionality */
pmlan_ioctl_req woal_alloc_mlan_ioctl_req_no_wait(int size);
/** Free buffer */
void woal_free_mlan_buffer(pmlan_buffer pmbuf);
/** Get private structure of a BSS by index */
moal_private *woal_bss_num_to_priv(moal_handle * handle, t_u8 bss_num);
/* Functions in interface module */
/** Add card */
moal_handle *woal_add_card(void *card);
/** Remove card */
mlan_status woal_remove_card(void *card);

/** Get driver version */
void woal_get_version(moal_handle * handle, char *version, int maxlen);
/** Request open */
mlan_status woal_request_open(moal_private * priv);
/** Request close */
mlan_status woal_request_close(moal_private * priv);
/** Request MAC address setting */
mlan_status woal_request_set_mac_address(moal_private * priv);
/** Request multicast list setting */
void woal_request_set_multicast_list(moal_private * priv,
                                     struct net_device *dev);
/** Request firmware information */
void woal_request_get_fw_info(moal_private * priv);
/** Request IOCTL action */
mlan_status woal_request_ioctl(moal_private * priv, mlan_ioctl_req * req,
                               t_u8 wait_option);
/** Get Host Sleep parameters */
mlan_status woal_get_hs_params(moal_private * priv, t_u8 wait_option,
                               mlan_ds_hs_cfg * hscfg);
/** Cancel Host Sleep configuration */
mlan_status woal_hs_cfg_cancel(moal_private * priv);
/** Get mode */
t_u32 woal_get_mode(moal_private * priv, t_u8 wait_option);
/** Get signal information */
mlan_status woal_get_signal_info(moal_private * priv, t_u8 wait_option,
                                 mlan_ds_get_signal * signal);
/** Get statistics information */
mlan_status woal_get_stats_info(moal_private * priv, t_u8 wait_option,
                                mlan_ds_get_stats * stats);
/** Get data rates */
mlan_status woal_get_data_rates(moal_private * priv, t_u8 wait_option,
                                moal_802_11_rates * m_rates);
/** Get channel list */
mlan_status woal_get_channel_list(moal_private * priv, t_u8 wait_option,
                                  mlan_chan_list * chanlist);
/** Get BSS information */
mlan_status woal_get_bss_info(moal_private * priv, t_u8 wait_option,
                              mlan_bss_info * bss_info);
#ifdef PROC_DEBUG
/** Get debug information */
mlan_status woal_get_debug_info(moal_private * priv, t_u8 wait_option,
                                mlan_debug_info * debug_info);
/** Set debug information */
mlan_status woal_set_debug_info(moal_private * priv, t_u8 wait_option,
                                mlan_debug_info * debug_info);
#endif
/** Get scan table */
mlan_status woal_get_scan_table(moal_private * priv, t_u8 wait_option,
                                mlan_scan_resp * scanresp);
/** Set authentication mode */
mlan_status woal_set_auth_mode(moal_private * priv, t_u8 wait_option,
                               t_u32 auth_mode);
/** Get authentication mode */
mlan_status woal_get_auth_mode(moal_private * priv, t_u8 wait_option,
                               t_u32 * auth_mode);
/** Set encryption mode */
mlan_status woal_set_encrypt_mode(moal_private * priv, t_u8 wait_option,
                                  t_u32 encrypt_mode);
/** Get encryption mode */
mlan_status woal_get_encrypt_mode(moal_private * priv, t_u8 wait_option,
                                  t_u32 * encrypt_mode);
/** Enable wep key */
mlan_status woal_enable_wep_key(moal_private * priv, t_u8 wait_option);
/** Set WPA enable */
mlan_status woal_set_wpa_enable(moal_private * priv, t_u8 wait_option,
                                t_u32 enable);
/** Get WPA state */
mlan_status woal_get_wpa_enable(moal_private * priv, t_u8 wait_option,
                                t_u32 * enable);
/** Find best network to connect */
mlan_status woal_find_best_network(moal_private * priv, t_u8 wait_option,
                                   mlan_ssid_bssid * ssid_bssid);
/** Request a network scan */
mlan_status woal_request_scan(moal_private * priv, t_u8 wait_option,
                              mlan_802_11_ssid * req_ssid);
/** Set E-Supplicant mode */
mlan_status woal_set_ewpa_mode(moal_private * priv, t_u8 wait_option,
                               mlan_ssid_bssid * ssid_bssid);
/** Set Ad-Hoc channel */
mlan_status woal_change_adhoc_chan(moal_private * priv, int channel);
/** Disconnect */
mlan_status woal_disconnect(moal_private * priv, t_u8 * mac);
/** associate */
mlan_status woal_associate(moal_private * priv, t_u8 wait_option,
                           mlan_ssid_bssid * ssid_bssid);

/** Set radio on/off */
int woal_set_radio(moal_private * priv, t_u8 option);

/** Initialize sw */
mlan_status woal_init_sw(moal_handle * handle);
/** Initialize priv */
void woal_init_priv(moal_private * priv);
/** Initialize proc fs */
void woal_proc_init(moal_handle * handle);
/** Clean up proc fs */
void woal_proc_exit(moal_handle * handle);
/** Create proc entry */
void woal_create_proc_entry(moal_private * priv);
/** Remove proc entry */
void woal_proc_remove(moal_private * priv);

#ifdef PROC_DEBUG
/** Create debug proc fs */
void woal_debug_entry(moal_private * priv);
/** Remove debug proc fs */
void woal_debug_remove(moal_private * priv);
#endif /* PROC_DEBUG */

#ifdef REASSOCIATION
int woal_reassociation_thread(void *data);
void woal_reassoc_timer_func(void *context);
#endif

#endif /* _MOAL_MAIN_H */
