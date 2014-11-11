/** @file  wlanconfig.c
  * @brief Program to configure addition parameters into the wlan driver
  * 
  *  Usage: wlanconfig ethX cmd [...] 
  *
  *  Copyright (C) 2003-2008, Marvell International Ltd.
  *  All Rights Reserved
  */
/************************************************************************
Change log:
	10/12/05: Add Doxygen format comments
	11/03/05: Load priv ioctls on demand, ifdef code for features in driver
	11/04/05: Add crypto_test 
	12/14/05: Support wildcard SSID in BGSCAN	
	01/11/06: Add getscantable, setuserscan, setmrvltlv, getassocrsp 
	01/31/06: Add support to selectively enabe the FW Scan channel filter	
	02/24/06: fix getscanlist function not work on linux 2.6.15 X86
	04/06/06: Add TSPEC, queue metrics, and MSDU expiry support
	04/10/06: Add hostcmd generic API and power_adapt_cfg_ext command
	04/18/06: Remove old Subscrive Event and add new Subscribe Event
		  implementation through generic hostcmd API
	05/03/06: Add auto_tx hostcmd
	05/15/06: Support EMBEDDED_TCPIP with Linux 2.6.9
	08/28/06: Add LED_CTRL hostcmd
	10/30/07: Use dynamic allocated buffer for private ioctl info
************************************************************************/

#include    <stdio.h>
#include    <unistd.h>
#include    <time.h>
#include    <ctype.h>
#include    <sys/types.h>
#include    <sys/socket.h>
#include    <string.h>
#include    <stdlib.h>
#include    <linux/if.h>
#include    <sys/ioctl.h>
#include    <linux/wireless.h>
#include    <linux/if_ether.h>
#include    <linux/byteorder/swab.h>
#include    <errno.h>
#include    <linux/rtnetlink.h>

/** wlanconfig Version number */
#define WLANCONFIG_VER "3.07"

#ifndef __ATTRIB_ALIGN__
#define __ATTRIB_ALIGN__ __attribute__((aligned(4)))
#endif

#ifndef __ATTRIB_PACK__
#define __ATTRIB_PACK__  __attribute__((packed))
#endif

/*
 *  ctype from older glib installations defines BIG_ENDIAN.  Check it 
 *   and undef it if necessary to correctly process the wlan header
 *   files
 */
#if (BYTE_ORDER == LITTLE_ENDIAN)
#undef BIG_ENDIAN
#endif

/** Convert to correct endian format */
#ifdef 	BIG_ENDIAN
/** CPU to little-endian convert for 16-bit */
#define 	cpu_to_le16(x)	__swab16(x)
/** CPU to little-endian convert for 32-bit */
#define		cpu_to_le32(x)  __swab32(x)
/** Little-endian to CPU convert for 16-bit */
#define 	le16_to_cpu(x)	__swab16(x)
/** Little-endian to CPU convert for 32-bit */
#define		le32_to_cpu(x)  __swab32(x)
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

#include    "wlan_linux.h"
#include	"wlan_fw.h"
#include    "wlan_wmm.h"
#include    "wlan_11d.h"

#include	"wlan_scan.h"
#include	"wlan_wext.h"
#include	"wlan_priv.h"
#include	"wlanconfig.h"

#ifndef MIN
/** Find minimum value */
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif /* MIN */

/** Private ioctl commands */
enum COMMANDS
{
    CMD_MEFCFG,
    CMD_HOSTCMD,
    CMD_RDMAC,
    CMD_WRMAC,
    CMD_RDBBP,
    CMD_WRBBP,
    CMD_RDRF,
    CMD_WRRF,
    CMD_RDPMIC,
    CMD_WRPMIC,
    CMD_RDEEPROM,
    CMD_BG_SCAN_CONFIG,
    CMD_BG_SCAN_CFG,
    CMD_ADDTS,
    CMD_DELTS,
    CMD_QCONFIG,
    CMD_QSTATS,
    CMD_TS_STATUS,
    CMD_WMM_QSTATUS,
    CMD_TX_PKT_STATS,
    CMD_SLEEPPARAMS,
    CMD_BCA_TS,
    CMD_EXTSCAN,
    CMD_SCAN_LIST,
    CMD_CRYPTO_TEST,
    CMD_SET_GEN_IE,
    CMD_GET_SCAN_RSP,
    CMD_SET_USER_SCAN,
    CMD_SET_MRVL_TLV,
    CMD_GET_ASSOC_RSP,
    CMD_GET_TSF,
    CMD_HS_TEST,
    CMD_GET_CFP_TABLE,
    CMD_GET_FW_MEM,
    CMD_ARPFILTER,
    CMD_CFG_DATA,
};

/** Initial number of total private ioctl calls */
#define IW_INIT_PRIV_NUM    128
/** Maximum number of total private ioctl calls supported */
#define IW_MAX_PRIV_NUM     1024

/** Maximum message size */
#define MAX_MSG_SIZE 1024

/** Host Sleep states */
enum HS_STATE
{
    /** Host Sleep state deactivated */
    HS_STATE_DEACTIVATED,
    /** Host Sleep state activated */
    HS_STATE_ACTIVATED,
    /** Host Sleep state wakeup */
    HS_STATE_WAKEUP,
};

static int hs_state = HS_STATE_DEACTIVATED;

/********************************************************
		Local Variables
********************************************************/
static s8 *commands[] = {
    "mefcfg",
    "hostcmd",
    "rdmac",
    "wrmac",
    "rdbbp",
    "wrbbp",
    "rdrf",
    "wrrf",
    "rdpmic",
    "wrpmic",
    "rdeeprom",
    "bgscanconfig",
    "bgscfg",
    "addts",
    "delts",
    "qconfig",
    "qstats",
    "ts_status",
    "qstatus",
    "txpktstats",
    "sleepparams",
    "bca-ts",
    "extscan",
    "getscanlist",
    "crypto_test",
    "setgenie",
    "getscantable",
    "setuserscan",
    "setmrvltlv",
    "getassocrsp",
    "gettsf",
    "hstest",
    "getcfptable",
    "getfwmem",
    "arpfilter",
    "cfg_data",
};

typedef struct cfg_data_s
{
        /** Command */
    s8 cmd[10];
        /** Command type */
    u16 cmdType;
} __ATTRIB_PACK__ cfg_data_t;

static cfg_data_t cfg_data_cmds[] = {
    {"or", 1},                  /* cmdtype for "or" is 1 */
    {"cal", 2},                 /* cmdtype for "cal" is 2 */
    {"pmic", 3},                /* cmdtype for "pmic" is 3 */
};

static s8 *usage[] = {
    "Usage: ",
    "   	wlanconfig -v  (version)",
    "	wlanconfig <ethX> <cmd> [...]",
    "	where",
    "	ethX	: wireless network interface",
    "	cmd	: hostcmd, rdmac, wrmac, rdbbp, wrbbp, rdrf, wrrf",
    "		  rdpmic,wrpmic",
    "		: rdeeprom, sleepparams",
    "		: bca-ts",
    "		: bgscanconfig",
    "		: bgscfg",
    "		: qstatus, qconfig, qstats",
    "		: addts, delts, ts_status",
    "		: txpktstats",
    "		: crypto_test",
    "		: setgenie",
    "		: getscantable, setuserscan",
    "		: setmrvltlv, getassocrsp",
    "		: hstest",
    "		: getcfptable",
    "		: mefcfg",
    "		: getfwmem",
    "		: arpfilter",
    "		: cfg_data",
    "	[...]	: additional parameters for read registers are",
    "		:	<offset>",
    "		: additional parameters for write registers are",
    "		:	<offset> <value>",
    "		: additional parameter for hostcmd",
    "		: 	<filename> <cmd>",
    "		: additional parameters for crypto_test are:",
    "		:	<EncDec> <Algorithm> <KeyIV> <Key> <Data>",
    "		: additional parameters for mefcfg are:"
        "		:	<filename>",
    "		: additional parameters for getfwmem are:"
        "		:	<StartAddr> <Len>",
    "		: additional parameter for arpfilter",
    "		: 	<filename>",
    "		: additional parameter for cfg_data",
    "		: 	<type> <filename>",
};

static s32 sockfd;  /**< socket */
static s8 dev_name[IFNAMSIZ + 1];       /**< device name */
static struct iw_priv_args *Priv_args = NULL;      /**< private args */
static int we_version_compiled = 0;
                                  /**< version compiled */
/** EV point offset */
#define MRV_EV_POINT_OFF (((char *) &(((struct iw_point *) NULL)->length)) - \
			  (char *) NULL)

static u16 TLVChanSize; /**< TLV channel size */
static u16 TLVSsidSize; /**< TLV ssid size */
static u16 TLVProbeSize; /**< TLV probe size */
static u16 TLVSnrSize; /**< TLV snr size */
static u16 TLVBcProbeSize; /**< TLV Bc probe size */
static u16 TLVNumSsidProbeSize; /**< TLV Num ssid probe size */
static u16 TLVStartBGScanLaterSize; /**< TLV start bgscan later size */

/** Normal BG scan configuration */
#define NORMAL_BG_SCAN_CFG   	0x00
/** PPS/UAPSD BG scan configuration */
#define UAPSD_PPS_BG_SCAN_CFG	0x01
/** All BG scan configuration */
#define ALL_BG_SCAN_CFGS     	0xFF

static u16 ActualPos = sizeof(HostCmd_DS_802_11_BG_SCAN_CONFIG);

/** Supported rates bitmap */
char rate_bitmap[16][16] =
    { "1", "2", "5.5", "11", "reserved", "6", "9", "12", "18", "24", "36", "48",
"54", "reserved", "reserved", "reserved" };

/** Firmware states */
char fw_state[9][32] = { "Disconnected",
    "Scanning",
    "Connected-Awake",
    "Connected-Sleep",
    "Deep Sleep",
    "Connected-Single",
    "Disconnected-Link Loss",
    "Disconnected-Disassociated",
    "Exception"
};

/** LED states */
char led_state[3][16] = { "Steady OFF",
    "Steady ON",
    "Blinking"
};

/** LED duty factors */
char led_duty_factor[5][8] = { "1/2", "1/4", "1/8", "1/16", "1/32" };

/** LED blink periods */
char led_blink_period[6][8] =
    { "37ms", "74ms", "149ms", "298ms", "596ms", "1192ms" };

static s8 *wlan_config_get_line(s8 * s, s32 size, FILE * stream, int *line);

/********************************************************
		Global Variables
********************************************************/

/********************************************************
		Local Functions
********************************************************/
/** 
 *  @brief convert char to hex integer
 * 
 *  @param chr 		char to convert
 *  @return      	hex integer or 0
 */
static int
hexval(s32 chr)
{
    if (chr >= '0' && chr <= '9')
        return chr - '0';
    if (chr >= 'A' && chr <= 'F')
        return chr - 'A' + 10;
    if (chr >= 'a' && chr <= 'f')
        return chr - 'a' + 10;

    return 0;
}

/** 
 *  @brief Hump hex data
 *
 *  @param prompt	A pointer prompt buffer
 *  @param p		A pointer to data buffer
 *  @param len		the len of data buffer
 *  @param delim	delim char
 *  @return            	hex integer
 */
static void
hexdump(s8 * prompt, void *p, s32 len, s8 delim)
{
    s32 i;
    u8 *s = p;

    if (prompt) {
        printf("%s: len=%d\n", prompt, (int) len);
    }
    for (i = 0; i < len; i++) {
        if (i != len - 1)
            printf("%02x%c", *s++, delim);
        else
            printf("%02x\n", *s);
        if ((i + 1) % 16 == 0)
            printf("\n");
    }
}

/** 
 *  @brief convert char to hex integer
 *
 *  @param chr		char
 *  @return            	hex integer
 */
static u8
hexc2bin(s8 chr)
{
    if (chr >= '0' && chr <= '9')
        chr -= '0';
    else if (chr >= 'A' && chr <= 'F')
        chr -= ('A' - 10);
    else if (chr >= 'a' && chr <= 'f')
        chr -= ('a' - 10);

    return chr;
}

/** 
 *  @brief convert string to hex integer
 *
 *  @param s		A pointer string buffer
 *  @return            	hex integer
 */
static u32
a2hex(s8 * s)
{
    u32 val = 0;
    while (*s && isxdigit(*s)) {
        val = (val << 4) + hexc2bin(*s++);
    }

    return val;
}

/* 
 *  @brief convert String to integer
 *  
 *  @param value	A pointer to string
 *  @return             integer
 */
static u32
a2hex_or_atoi(s8 * value)
{
    if (value[0] == '0' && (value[1] == 'X' || value[1] == 'x')) {
        return a2hex(value + 2);
    } else if (isdigit(*value)) {
        return atoi(value);
    } else {
        return *value;
    }
}

/** 
 *  @brief convert string to integer
 * 
 *  @param ptr		A pointer to data buffer
 *  @param chr 		A pointer to return integer
 *  @return      	A pointer to next data field
 */
s8 *
convert2hex(s8 * ptr, u8 * chr)
{
    u8 val;

    for (val = 0; *ptr && isxdigit(*ptr); ptr++) {
        val = (val * 16) + hexval(*ptr);
    }

    *chr = val;

    return ptr;
}

/** 
 *  @brief Get private info.
 *   
 *  @param ifname   A pointer to net name
 *  @return 	    WLAN_STATUS_SUCCESS--success, otherwise --fail
 */
static int
get_private_info(const s8 * ifname)
{
    /* This function sends the SIOCGIWPRIV command, which is handled by the
       kernel and gets the total number of private ioctl's available in the
       host driver. */
    struct iwreq iwr;
    int s, ret = WLAN_STATUS_SUCCESS;
    struct iw_priv_args *pPriv = NULL;
    struct iw_priv_args *newPriv;
    int result = 0;
    size_t size = IW_INIT_PRIV_NUM;

    s = socket(PF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
        perror("socket[PF_INET,SOCK_DGRAM]");
        return WLAN_STATUS_FAILURE;
    }

    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, ifname, IFNAMSIZ);

    do {
        /* (Re)allocate the buffer */
        newPriv = realloc(pPriv, size * sizeof(pPriv[0]));
        if (newPriv == NULL) {
            printf("Error: Buffer allocation failed\n");
            ret = WLAN_STATUS_FAILURE;
            break;
        }
        pPriv = newPriv;

        iwr.u.data.pointer = (caddr_t) pPriv;
        iwr.u.data.length = size;
        iwr.u.data.flags = 0;

        if (ioctl(s, SIOCGIWPRIV, &iwr) < 0) {
            result = errno;
            ret = WLAN_STATUS_FAILURE;
            if (result == E2BIG) {
                /* We need a bigger buffer. Check if kernel gave us any hints. */
                if (iwr.u.data.length > size) {
                    /* Kernel provided required size */
                    size = iwr.u.data.length;
                } else {
                    /* No hint from kernel, double the buffer size */
                    size *= 2;
                }
            } else {
                /* ioctl error */
                perror("ioctl[SIOCGIWPRIV]");
                break;
            }
        } else {
            /* Success. Return the number of private ioctls */
            Priv_args = pPriv;
            ret = iwr.u.data.length;
            break;
        }
    } while (size <= IW_MAX_PRIV_NUM);

    if ((ret == WLAN_STATUS_FAILURE) && (pPriv))
        free(pPriv);

    close(s);

    return ret;
}

/** 
 *  @brief Get Sub command ioctl number
 *   
 *  @param i        command index
 *  @param priv_cnt     Total number of private ioctls availabe in driver
 *  @param ioctl_val    A pointer to return ioctl number
 *  @param subioctl_val A pointer to return sub-ioctl number
 *  @return 	        WLAN_STATUS_SUCCESS or WLAN_STATUS_FAILURE
 */
static int
marvell_get_subioctl_no(s32 i, s32 priv_cnt, int *ioctl_val, int *subioctl_val)
{
    s32 j;

    if (Priv_args[i].cmd >= SIOCDEVPRIVATE) {
        *ioctl_val = Priv_args[i].cmd;
        *subioctl_val = 0;
        return WLAN_STATUS_SUCCESS;
    }

    j = -1;

    /* Find the matching *real* ioctl */

    while ((++j < priv_cnt)
           && ((Priv_args[j].name[0] != '\0') ||
               (Priv_args[j].set_args != Priv_args[i].set_args) ||
               (Priv_args[j].get_args != Priv_args[i].get_args))) {
    }

    /* If not found... */
    if (j == priv_cnt) {
        printf("%s: Invalid private ioctl definition for: 0x%x\n",
               dev_name, Priv_args[i].cmd);
        return WLAN_STATUS_FAILURE;
    }

    /* Save ioctl numbers */
    *ioctl_val = Priv_args[j].cmd;
    *subioctl_val = Priv_args[i].cmd;

    return WLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Get ioctl number
 *   
 *  @param ifname   	A pointer to net name
 *  @param priv_cmd	A pointer to priv command buffer
 *  @param ioctl_val    A pointer to return ioctl number
 *  @param subioctl_val A pointer to return sub-ioctl number
 *  @return 	        WLAN_STATUS_SUCCESS or WLAN_STATUS_FAILURE
 */
static int
marvell_get_ioctl_no(const s8 * ifname,
                     const s8 * priv_cmd, int *ioctl_val, int *subioctl_val)
{
    s32 i;
    s32 priv_cnt;
    int ret = WLAN_STATUS_FAILURE;

    priv_cnt = get_private_info(ifname);

    /* Are there any private ioctls? */
    if (priv_cnt <= 0 || priv_cnt > IW_MAX_PRIV_NUM) {
        /* Could skip this message ? */
        printf("%-8.8s  no private ioctls.\n", ifname);
    } else {
        for (i = 0; i < priv_cnt; i++) {
            if (Priv_args[i].name[0] && !strcmp(Priv_args[i].name, priv_cmd)) {
                ret = marvell_get_subioctl_no(i, priv_cnt,
                                              ioctl_val, subioctl_val);
                break;
            }
        }
    }

    if (Priv_args) {
        free(Priv_args);
        Priv_args = NULL;
    }

    return ret;
}

/** 
 *  @brief Retrieve the ioctl and sub-ioctl numbers for the given ioctl string
 *   
 *  @param ioctl_name   Private IOCTL string name
 *  @param ioctl_val    A pointer to return ioctl number
 *  @param subioctl_val A pointer to return sub-ioctl number
 *
 *  @return             WLAN_STATUS_SUCCESS or WLAN_STATUS_FAILURE
 */
static int
get_priv_ioctl(char *ioctl_name, int *ioctl_val, int *subioctl_val)
{
    int retval;

    retval = marvell_get_ioctl_no(dev_name,
                                  ioctl_name, ioctl_val, subioctl_val);

    return retval;
}

/** 
 *  @brief  get range 
 *   
 *  @return	WLAN_STATUS_SUCCESS--success, otherwise --fail
 */
static int
get_range(void)
{
    struct iw_range *range;
    struct iwreq iwr;
    size_t buflen;
    WCON_HANDLE mhandle, *pHandle = &mhandle;

    buflen = sizeof(struct iw_range) + 500;
    range = malloc(buflen);
    if (range == NULL)
        return WLAN_STATUS_FAILURE;
    memset(range, 0, buflen);

    memset(pHandle, 0, sizeof(WCON_HANDLE));
    memset(&iwr, 0, sizeof(struct iwreq));

    iwr.u.data.pointer = (caddr_t) range;
    iwr.u.data.length = buflen;

    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);

    if ((ioctl(sockfd, SIOCGIWRANGE, &iwr)) < 0) {
        printf("Get Range Results Failed\n");
        free(range);
        return WLAN_STATUS_FAILURE;
    }
    we_version_compiled = range->we_version_compiled;
    printf("Driver build with Wireless Extension %d\n",
           range->we_version_compiled);
    free(range);
    return WLAN_STATUS_SUCCESS;
}

/** 
 *  @brief get hostcmd data
 *  
 *  @param fp			A pointer to file stream
 *  @param ln			A pointer to line number
 *  @param buf			A pointer to hostcmd data
 *  @param size			A pointer to the return size of hostcmd buffer
 *  @return      		WLAN_STATUS_SUCCESS
 */
static int
wlan_get_hostcmd_data(FILE * fp, int *ln, u8 * buf, u16 * size)
{
    s32 errors = 0, i;
    s8 line[256], *pos, *pos1, *pos2, *pos3;
    u16 len;

    while ((pos = wlan_config_get_line(line, sizeof(line), fp, ln))) {
        (*ln)++;
        if (strcmp(pos, "}") == 0) {
            break;
        }

        pos1 = strchr(pos, ':');
        if (pos1 == NULL) {
            printf("Line %d: Invalid hostcmd line '%s'\n", *ln, pos);
            errors++;
            continue;
        }
        *pos1++ = '\0';

        pos2 = strchr(pos1, '=');
        if (pos2 == NULL) {
            printf("Line %d: Invalid hostcmd line '%s'\n", *ln, pos);
            errors++;
            continue;
        }
        *pos2++ = '\0';

        len = a2hex_or_atoi(pos1);
        if (len < 1 || len > MRVDRV_SIZE_OF_CMD_BUFFER) {
            printf("Line %d: Invalid hostcmd line '%s'\n", *ln, pos);
            errors++;
            continue;
        }

        *size += len;

        if (*pos2 == '"') {
            pos2++;
            if ((pos3 = strchr(pos2, '"')) == NULL) {
                printf("Line %d: invalid quotation '%s'\n", *ln, pos);
                errors++;
                continue;
            }
            *pos3 = '\0';
            memset(buf, 0, len);
            memmove(buf, pos2, MIN(strlen(pos2), len));
            buf += len;
        } else if (*pos2 == '\'') {
            pos2++;
            if ((pos3 = strchr(pos2, '\'')) == NULL) {
                printf("Line %d: invalid quotation '%s'\n", *ln, pos);
                errors++;
                continue;
            }
            *pos3 = ',';
            for (i = 0; i < len; i++) {
                if ((pos3 = strchr(pos2, ',')) != NULL) {
                    *pos3 = '\0';
                    *buf++ = (u8) a2hex_or_atoi(pos2);
                    pos2 = pos3 + 1;
                } else
                    *buf++ = 0;
            }
        } else if (*pos2 == '{') {
            u16 *tlvlen = (u16 *) buf;
            wlan_get_hostcmd_data(fp, ln, buf + len, tlvlen);
            *size += *tlvlen;
            buf += len + *tlvlen;
        } else {
            u32 value = a2hex_or_atoi(pos2);
            while (len--) {
                *buf++ = (u8) (value & 0xff);
                value >>= 8;
            }
        }
    }
    return WLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Process host_cmd 
 *  @param argc		number of arguments
 *  @param argv		A pointer to arguments array    
 *  @return      	WLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
process_host_cmd(int argc, char *argv[])
{
    s8 line[256], cmdname[256], *pos;
    u8 *buf;
    FILE *fp;
    HostCmd_DS_GEN *hostcmd;
    struct iwreq iwr;
    int ln = 0;
    int cmdname_found = 0, cmdcode_found = 0;
    int ret = WLAN_STATUS_SUCCESS;
    int ioctl_val, subioctl_val;

    if (get_priv_ioctl("hostcmd",
                       &ioctl_val, &subioctl_val) == WLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    if (argc < 5) {
        printf("Error: invalid no of arguments\n");
        printf("Syntax: ./wlanconfig eth1 hostcmd <hostcmd.conf> <cmdname>\n");
        exit(1);
    }

    if ((fp = fopen(argv[3], "r")) == NULL) {
        fprintf(stderr, "Cannot open file %s\n", argv[3]);
        exit(1);
    }

    buf = (u8 *) malloc(MRVDRV_SIZE_OF_CMD_BUFFER);
    if (buf == NULL) {
        printf("Error: allocate memory for hostcmd failed\n");
        return -ENOMEM;
    }

    memset(buf, 0, MRVDRV_SIZE_OF_CMD_BUFFER);
    hostcmd = (HostCmd_DS_GEN *) buf;

    hostcmd->Command = 0xffff;

    sprintf(cmdname, "%s={", argv[4]);
    cmdname_found = 0;
    while ((pos = wlan_config_get_line(line, sizeof(line), fp, &ln))) {
        if (strcmp(pos, cmdname) == 0) {
            cmdname_found = 1;
            sprintf(cmdname, "CmdCode=");
            cmdcode_found = 0;
            while ((pos = wlan_config_get_line(line, sizeof(line), fp, &ln))) {
                if (strncmp(pos, cmdname, strlen(cmdname)) == 0) {
                    cmdcode_found = 1;
                    hostcmd->Command = a2hex_or_atoi(pos + strlen(cmdname));
                    hostcmd->Size = S_DS_GEN;
                    wlan_get_hostcmd_data(fp, &ln, buf + hostcmd->Size,
                                          &hostcmd->Size);
                    break;
                }
            }
            if (!cmdcode_found) {
                fprintf(stderr, "wlanconfig: CmdCode not found in file '%s'\n",
                        argv[3]);
            }
            break;
        }
    }

    fclose(fp);

    if (!cmdname_found)
        fprintf(stderr, "wlanconfig: cmdname '%s' not found in file '%s'\n",
                argv[4], argv[3]);

    if (!cmdname_found || !cmdcode_found) {
        ret = WLAN_STATUS_FAILURE;
        goto _exit_;
    }

    buf = (u8 *) hostcmd;

    hostcmd->SeqNum = 0;
    hostcmd->Result = 0;
    hostcmd->Command = wlan_cpu_to_le16(hostcmd->Command);
    hostcmd->Size = wlan_cpu_to_le16(hostcmd->Size);

    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.pointer = (u8 *) hostcmd;
    iwr.u.data.length = wlan_le16_to_cpu(hostcmd->Size);

    iwr.u.data.flags = 0;
    if (ioctl(sockfd, ioctl_val, &iwr)) {
        fprintf(stderr, "wlanconfig: WLANHOSTCMD is not supported by %s\n",
                dev_name);
        ret = WLAN_STATUS_FAILURE;
        goto _exit_;
    }

    hostcmd->Command = wlan_le16_to_cpu(hostcmd->Command);
    hostcmd->Size = wlan_le16_to_cpu(hostcmd->Size);

    hostcmd->Command &= ~HostCmd_RET_BIT;
    if (!wlan_le16_to_cpu(hostcmd->Result)) {
        switch (hostcmd->Command) {
        case HostCmd_CMD_MEM_ACCESS:
            {
                HostCmd_DS_MEM_ACCESS *ma =
                    (HostCmd_DS_MEM_ACCESS *) (buf + S_DS_GEN);
                if (wlan_le16_to_cpu(ma->Action) == HostCmd_ACT_GEN_GET) {
                    printf("Address: %#08lx   Value=%#08lx\n",
                           wlan_le32_to_cpu(ma->Addr),
                           wlan_le32_to_cpu(ma->Value));
                }
                break;
            }
        case HostCmd_CMD_802_11_POWER_ADAPT_CFG_EXT:
            {
                HostCmd_DS_802_11_POWER_ADAPT_CFG_EXT *pace =
                    (HostCmd_DS_802_11_POWER_ADAPT_CFG_EXT *) (buf + S_DS_GEN);
                int i, j;
                printf("EnablePA=%#04x\n", wlan_le16_to_cpu(pace->EnablePA));
                pace->PowerAdaptGroup.Header.Len =
                    wlan_le16_to_cpu(pace->PowerAdaptGroup.Header.Len);
                for (i = 0;
                     i < pace->PowerAdaptGroup.Header.Len / sizeof(PA_Group_t);
                     i++) {
                    pace->PowerAdaptGroup.PA_Group[i].PowerAdaptLevel =
                        wlan_le16_to_cpu(pace->PowerAdaptGroup.PA_Group[i].
                                         PowerAdaptLevel);
                    pace->PowerAdaptGroup.PA_Group[i].RateBitmap =
                        wlan_le16_to_cpu(pace->PowerAdaptGroup.PA_Group[i].
                                         RateBitmap);
                    printf("PA Group #%d: level=%ddbm, Rate Bitmap=%#04x (", i,
                           pace->PowerAdaptGroup.PA_Group[i].PowerAdaptLevel,
                           pace->PowerAdaptGroup.PA_Group[i].RateBitmap);
                    for (j =
                         8 *
                         sizeof(pace->PowerAdaptGroup.PA_Group[i].RateBitmap) -
                         1; j >= 0; j--) {
                        if ((j == 4) || (j >= 13))      /* reserved */
                            continue;
                        if (pace->PowerAdaptGroup.PA_Group[i].
                            RateBitmap & (1 << j))
                            printf("%s ", rate_bitmap[j]);
                    }
                    printf("Mbps)\n");
                }
                break;
            }
        case HostCmd_CMD_802_11_SUBSCRIBE_EVENT:
            {
                HostCmd_DS_802_11_SUBSCRIBE_EVENT *se =
                    (HostCmd_DS_802_11_SUBSCRIBE_EVENT *) (buf + S_DS_GEN);
                if (wlan_le16_to_cpu(se->Action) == HostCmd_ACT_GEN_GET) {
                    int len =
                        S_DS_GEN + sizeof(HostCmd_DS_802_11_SUBSCRIBE_EVENT);
                    printf("\nEvent\t\tValue\tFreq\tsubscribed\n\n");
                    while (len < hostcmd->Size) {
                        MrvlIEtypesHeader_t *header =
                            (MrvlIEtypesHeader_t *) (buf + len);
                        switch (wlan_le16_to_cpu(header->Type)) {
                        case TLV_TYPE_RSSI_LOW:
                            {
                                MrvlIEtypes_RssiThreshold_t *LowRssi =
                                    (MrvlIEtypes_RssiThreshold_t *) (buf + len);
                                printf("Beacon Low RSSI\t%d\t%d\t%s\n",
                                       LowRssi->RSSIValue,
                                       LowRssi->RSSIFreq,
                                       (wlan_le16_to_cpu(se->Events) & 0x0001) ?
                                       "yes" : "no");
                                break;
                            }
                        case TLV_TYPE_SNR_LOW:
                            {
                                MrvlIEtypes_SnrThreshold_t *LowSnr =
                                    (MrvlIEtypes_SnrThreshold_t *) (buf + len);
                                printf("Beacon Low SNR\t%d\t%d\t%s\n",
                                       LowSnr->SNRValue,
                                       LowSnr->SNRFreq,
                                       (wlan_le16_to_cpu(se->Events) & 0x0002) ?
                                       "yes" : "no");
                                break;
                            }
                        case TLV_TYPE_FAILCOUNT:
                            {
                                MrvlIEtypes_FailureCount_t *FailureCount =
                                    (MrvlIEtypes_FailureCount_t *) (buf + len);
                                printf("Failure Count\t%d\t%d\t%s\n",
                                       FailureCount->FailValue,
                                       FailureCount->FailFreq,
                                       (wlan_le16_to_cpu(se->Events) & 0x0004) ?
                                       "yes" : "no");
                                break;
                            }
                        case TLV_TYPE_BCNMISS:
                            {
                                MrvlIEtypes_BeaconsMissed_t *BcnMissed =
                                    (MrvlIEtypes_BeaconsMissed_t *) (buf + len);
                                printf("Beacon Missed\t%d\tN/A\t%s\n",
                                       BcnMissed->BeaconMissed,
                                       (wlan_le16_to_cpu(se->Events) & 0x0008) ?
                                       "yes" : "no");
                                break;
                            }
                        case TLV_TYPE_RSSI_HIGH:
                            {
                                MrvlIEtypes_RssiThreshold_t *HighRssi =
                                    (MrvlIEtypes_RssiThreshold_t *) (buf + len);
                                printf("Bcn High RSSI\t%d\t%d\t%s\n",
                                       HighRssi->RSSIValue,
                                       HighRssi->RSSIFreq,
                                       (wlan_le16_to_cpu(se->Events) & 0x0010) ?
                                       "yes" : "no");
                                break;
                            }
                        case TLV_TYPE_SNR_HIGH:
                            {
                                MrvlIEtypes_SnrThreshold_t *HighSnr =
                                    (MrvlIEtypes_SnrThreshold_t *) (buf + len);
                                printf("Beacon High SNR\t%d\t%d\t%s\n",
                                       HighSnr->SNRValue,
                                       HighSnr->SNRFreq,
                                       (wlan_le16_to_cpu(se->Events) & 0x0020) ?
                                       "yes" : "no");
                                break;
                            }
                        case TLV_TYPE_RSSI_LOW_DATA:
                            {
                                MrvlIEtypes_RssiThreshold_t *LowRssi =
                                    (MrvlIEtypes_RssiThreshold_t *) (buf + len);
                                printf("Data Low RSSI\t%d\t%d\t%s\n",
                                       LowRssi->RSSIValue,
                                       LowRssi->RSSIFreq,
                                       (wlan_le16_to_cpu(se->Events) & 0x0040) ?
                                       "yes" : "no");
                                break;
                            }
                        case TLV_TYPE_SNR_LOW_DATA:
                            {
                                MrvlIEtypes_SnrThreshold_t *LowSnr =
                                    (MrvlIEtypes_SnrThreshold_t *) (buf + len);
                                printf("Data Low SNR\t%d\t%d\t%s\n",
                                       LowSnr->SNRValue,
                                       LowSnr->SNRFreq,
                                       (wlan_le16_to_cpu(se->Events) & 0x0080) ?
                                       "yes" : "no");
                                break;
                            }
                        case TLV_TYPE_RSSI_HIGH_DATA:
                            {
                                MrvlIEtypes_RssiThreshold_t *HighRssi =
                                    (MrvlIEtypes_RssiThreshold_t *) (buf + len);
                                printf("Data High RSSI\t%d\t%d\t%s\n",
                                       HighRssi->RSSIValue,
                                       HighRssi->RSSIFreq,
                                       (wlan_le16_to_cpu(se->Events) & 0x0100) ?
                                       "yes" : "no");
                                break;
                            }
                        case TLV_TYPE_SNR_HIGH_DATA:
                            {
                                MrvlIEtypes_SnrThreshold_t *HighSnr =
                                    (MrvlIEtypes_SnrThreshold_t *) (buf + len);
                                printf("Data High SNR\t%d\t%d\t%s\n",
                                       HighSnr->SNRValue,
                                       HighSnr->SNRFreq,
                                       (wlan_le16_to_cpu(se->Events) & 0x0200) ?
                                       "yes" : "no");
                                break;
                            }
                        case TLV_TYPE_LINK_QUALITY:
                            {
                                MrvlIEtypes_LinkQuality_t *LinkQual =
                                    (MrvlIEtypes_LinkQuality_t *) (buf + len);
                                printf("Link Quality Parameters:\n");
                                printf("------------------------\n");
                                printf("Link Quality Event Subscribed\t%s\n",
                                       (wlan_le16_to_cpu(se->Events) & 0x0400) ?
                                       "yes" : "no");
                                printf("Link SNR Threshold   = %d\n",
                                       wlan_le16_to_cpu(LinkQual->LinkSNRThrs));
                                printf("Link SNR Frequency   = %d\n",
                                       wlan_le16_to_cpu(LinkQual->LinkSNRFreq));
                                printf("Min Rate Value       = %d\n",
                                       wlan_le16_to_cpu(LinkQual->MinRateVal));
                                printf("Min Rate Frequency   = %d\n",
                                       wlan_le16_to_cpu(LinkQual->MinRateFreq));
                                printf("Tx Latency Value     = %ld\n",
                                       wlan_le32_to_cpu(LinkQual->
                                                        TxLatencyVal));
                                printf("Tx Latency Threshold = %ld\n",
                                       wlan_le32_to_cpu(LinkQual->
                                                        TxLatencyThrs));

                                break;
                            }
                        case TLV_TYPE_PRE_BEACON_LOST:
                            {
                                MrvlIEtypes_PreBeaconLost_t *PreBeaconLost =
                                    (MrvlIEtypes_PreBeaconLost_t *) (buf + len);
                                printf("------------------------\n");
                                printf("Pre-Beacon Lost Event Subscribed\t%s\n",
                                       (wlan_le16_to_cpu(se->Events) & 0x0800) ?
                                       "yes" : "no");
                                printf("Pre-Beacon Lost: %d\n",
                                       PreBeaconLost->PreBeaconLost);

                                break;
                            }
                        default:
                            printf
                                ("unknown subscribed event TLV Type=%#x, Len=%d\n",
                                 wlan_le16_to_cpu(header->Type),
                                 wlan_le16_to_cpu(header->Len));
                            break;
                        }
                        len +=
                            sizeof(MrvlIEtypesHeader_t) +
                            wlan_le16_to_cpu(header->Len);
                    }
                }
                break;
            }
        case HostCmd_CMD_802_11_AUTO_TX:
            {
                HostCmd_DS_802_11_AUTO_TX *at =
                    (HostCmd_DS_802_11_AUTO_TX *) (buf + S_DS_GEN);
                if (wlan_le16_to_cpu(at->Action) == HostCmd_ACT_GEN_GET) {
                    if (S_DS_GEN + sizeof(at->Action) == hostcmd->Size) {
                        printf("auto_tx not configured\n");
                    } else {
                        MrvlIEtypesHeader_t *header = &at->AutoTx.Header;
                        header->Type = wlan_le16_to_cpu(header->Type);
                        header->Len = wlan_le16_to_cpu(header->Len);
                        if ((S_DS_GEN + sizeof(at->Action) +
                             sizeof(MrvlIEtypesHeader_t) + header->Len ==
                             hostcmd->Size) &&
                            (header->Type == TLV_TYPE_AUTO_TX)) {
                            AutoTx_MacFrame_t *atmf =
                                &at->AutoTx.AutoTx_MacFrame;

                            printf("Interval: %d second(s)\n",
                                   wlan_le16_to_cpu(atmf->Interval));
                            printf("Priority: %#x\n", atmf->Priority);

                            printf("Frame Length: %d\n",
                                   wlan_le16_to_cpu(atmf->FrameLen));
                            printf
                                ("Dest Mac Address: %02x:%02x:%02x:%02x:%02x:%02x\n",
                                 atmf->DestMacAddr[0], atmf->DestMacAddr[1],
                                 atmf->DestMacAddr[2], atmf->DestMacAddr[3],
                                 atmf->DestMacAddr[4], atmf->DestMacAddr[5]);
                            printf
                                ("Src Mac Address: %02x:%02x:%02x:%02x:%02x:%02x\n",
                                 atmf->SrcMacAddr[0], atmf->SrcMacAddr[1],
                                 atmf->SrcMacAddr[2], atmf->SrcMacAddr[3],
                                 atmf->SrcMacAddr[4], atmf->SrcMacAddr[5]);

                            hexdump("Frame Payload", atmf->Payload,
                                    wlan_le16_to_cpu(atmf->FrameLen) -
                                    ETH_ALEN * 2, ' ');
                        } else {
                            printf("incorrect auto_tx command response\n");
                        }
                    }
                }
                break;
            }
        case HostCmd_CMD_802_11_LED_CONTROL:
            {
                HostCmd_DS_802_11_LED_CTRL *led =
                    (HostCmd_DS_802_11_LED_CTRL *) (buf + S_DS_GEN);

                led->Action = wlan_le16_to_cpu(led->Action);
                led->LedNums = wlan_le16_to_cpu(led->LedNums);

                if (led->Action == HostCmd_ACT_GEN_GET) {
                    MrvlIEtypesHeader_t *header = &led->LedGpio.Header;
                    header->Type = wlan_le16_to_cpu(header->Type);
                    header->Len = wlan_le16_to_cpu(header->Len);
                    printf("LedNums=%d\n", led->LedNums);
                    if ((S_DS_GEN + sizeof(led->Action) + sizeof(led->LedNums)
                         == hostcmd->Size) || !led->LedNums ||
                        (led->LedNums > MAX_LEDS)
                        || (led->LedNums != header->Len / sizeof(LedGpio_t))) {
                        printf("LED CTRL not configured\n");
                    } else {
                        if (header->Type == TLV_TYPE_LED_GPIO) {
                            int i;

                            for (i = 0; i < led->LedNums; i++) {
                                LedGpio_t *ledgpio = &led->LedGpio.LedGpio[i];

                                if (ledgpio->GpioNum > 0 &&
                                    ledgpio->GpioNum < LED_DISABLED)
                                    printf
                                        ("LED #%d is mapped to GPIO pin #%d\n",
                                         ledgpio->LedNum, ledgpio->GpioNum);
                                else
                                    printf("LED #%d is disabled\n",
                                           ledgpio->LedNum);
                            }
                        } else if (header->Type == MRVL_TERMINATE_TLV_ID)
                            break;
                        else
                            printf("incorrect LED_GPIO TLV type\n");

                        char *pos =
                            (char *) (&led->LedGpio.LedGpio[0]) + header->Len;
                        char *end = (char *) (buf + hostcmd->Size);
                        printf
                            ("\nLED#\tLEDState\tDutyFactor\tBlinkPeriod\tFirmwareState\n");
                        while (end - pos >= sizeof(MrvlIEtypes_LedBehavior_t)) {
                            MrvlIEtypes_LedBehavior_t *ledb =
                                (MrvlIEtypes_LedBehavior_t *) pos;

                            ledb->Header.Type =
                                wlan_le16_to_cpu(ledb->Header.Type);
                            ledb->Header.Len =
                                wlan_le16_to_cpu(ledb->Header.Len);

                            if (ledb->Header.Type == TLV_TYPE_LEDBEHAVIOR) {
                                printf("%d\t%s\t%s\t\t%s\t\t%s\n",
                                       ledb->LedNum,
                                       led_state[ledb->LedState],
                                       (ledb->LedState == LED_BLINKING) ?
                                       led_duty_factor[ledb->
                                                       LedArgs >> 4] : "N/A",
                                       (ledb->LedState ==
                                        LED_BLINKING) ? led_blink_period[ledb->
                                                                         LedArgs
                                                                         & 0x0f]
                                       : "N/A", fw_state[ledb->FirmwareState]);
                            } else if (header->Type == MRVL_TERMINATE_TLV_ID)
                                break;
                            else
                                printf("incorrect LEDBEHAVIOR TLV type\n");

                            pos += mrvl_tlv_len(ledb);
                        }
                    }
                }
                break;
            }
        default:
            printf("HOSTCMD_RESP: ReturnCode=%#04x, Result=%#04x\n",
                   wlan_le16_to_cpu(hostcmd->Command),
                   wlan_le16_to_cpu(hostcmd->Result));
            break;
        }
    } else {
        printf("HOSTCMD failed: ReturnCode=%#04x, Result=%#04x\n",
               wlan_le16_to_cpu(hostcmd->Command),
               wlan_le16_to_cpu(hostcmd->Result));
    }

  _exit_:
    if (buf)
        free(buf);

    return ret;
}

/** 
 *  @brief Check the Hex String
 *  @param s		A pointer to the string      
 *  @return      	WLAN_STATUS_SUCCESS --HexString, WLAN_STATUS_FAILURE --not HexString
 */
static int
ishexstring(s8 * s)
{
    int ret = WLAN_STATUS_FAILURE;
    s32 tmp;

    while (*s) {
        tmp = toupper(*s);
        if (tmp >= 'A' && tmp <= 'F') {
            ret = WLAN_STATUS_SUCCESS;
            break;
        }
        s++;
    }

    return ret;
}

/** 
 *  @brief Convert String to Integer
 *  @param buf		A pointer to the string      
 *  @return      	Integer
 */
static int
atoval(s8 * buf)
{
    if (!strncasecmp(buf, "0x", 2))
        return a2hex(buf + 2);
    else if (!ishexstring(buf))
        return a2hex(buf);
    else
        return atoi(buf);
}

/** 
 *  @brief Display sleep params
 *  @param sp		A pointer to wlan_ioctl_sleep_params_config structure    
 *  @return      	NA
 */
void
display_sleep_params(wlan_ioctl_sleep_params_config * sp)
{
    printf("Sleep Params for %s:\n", sp->Action ? "set" : "get");
    printf("----------------------------------------\n");
    printf("Error		: %u\n", sp->Error);
    printf("Offset		: %u\n", sp->Offset);
    printf("StableTime	: %u\n", sp->StableTime);
    printf("CalControl	: %u\n", sp->CalControl);
    printf("ExtSleepClk	: %u\n", sp->ExtSleepClk);
    printf("Reserved	: %u\n", sp->Reserved);
}

/** 
 *  @brief Process sleep params
 *  @param argc		number of arguments
 *  @param argv         A pointer to arguments array    
 *  @return      	WLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
process_sleep_params(int argc, char *argv[])
{
    struct iwreq iwr;
    int ret;
    wlan_ioctl_sleep_params_config sp;
    int ioctl_val, subioctl_val;

    if (get_priv_ioctl("sleepparams",
                       &ioctl_val, &subioctl_val) == WLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    if (argc < 4) {
        printf("Error: invalid no of arguments\n");
        printf("Syntax: ./wlanconfig eth1 sleepparams get/set <p1>"
               " <p2> <p3> <p4> <p5> <p6>\n");
        exit(1);
    }

    memset(&sp, 0, sizeof(wlan_ioctl_sleep_params_config));
    if (!strcmp(argv[3], "get")) {
        sp.Action = 0;
    } else if (!strncmp(argv[3], "set", 3)) {
        if (argc != 10) {
            printf("Error: invalid no of arguments\n");
            printf("Syntax: ./wlanconfig eth1 sleepparams get/set"
                   "<p1> <p2> <p3> <p4> <p5> <p6>\n");
            exit(1);
        }

        sp.Action = 1;
        if ((ret = atoval(argv[4])) < 0)
            return -EINVAL;
        sp.Error = (u16) ret;
        if ((ret = atoval(argv[5])) < 0)
            return -EINVAL;
        sp.Offset = (u16) ret;
        if ((ret = atoval(argv[6])) < 0)
            return -EINVAL;
        sp.StableTime = (u16) ret;
        if ((ret = atoval(argv[7])) < 0)
            return -EINVAL;
        sp.CalControl = (u8) ret;
        if ((ret = atoval(argv[8])) < 0)
            return -EINVAL;
        sp.ExtSleepClk = (u8) ret;
        if ((ret = atoval(argv[9])) < 0)
            return -EINVAL;
        sp.Reserved = (u16) ret;
    } else {
        return -EINVAL;
    }

    memset(&iwr, 0, sizeof(iwr));

    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.pointer = (caddr_t) & sp;
    iwr.u.data.length = sizeof(wlan_ioctl_sleep_params_config);
    iwr.u.data.flags = subioctl_val;

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("wlanconfig");
        return -EFAULT;
    }

    display_sleep_params(&sp);

    return WLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Display BCA Time Share Params
 *  @param bca_ts	A point to wlan_ioctl_bca_timeshare_config structure    
 *  @return      	NA
 */
static void
display_bca_ts_params(wlan_ioctl_bca_timeshare_config * bca_ts)
{
    printf("BCA Time Share Params for %s:\n", bca_ts->Action ? "set" : "get");
    printf("----------------------------------------\n");
    printf("TrafficType		: %u\n", bca_ts->TrafficType);
    printf("TimeShareInterval	: %lu\n", bca_ts->TimeShareInterval);
    printf("BTTime			: %lu\n", bca_ts->BTTime);
}

/** BCA Traffic type low */
#define TRAFFIC_TYPE_LOW	0x0
/** BCA Traffic type high */
#define TRAFFIC_TYPE_HIGH	0x1
/** BCA Traffic type medium */
#define TRAFFIC_TYPE_MEDIUM	0x02
/** BCA Traffic type medium high */
#define TRAFFIC_TYPE_MEDIUMHIGH	0x03
/** BCA Reset Traffic type */
#define TRAFFIC_TYPE_RESET	0xffff
/** 
 *  @brief Process BCA Time Share Params
 *  @param argc		number of arguments
 *  @param argv         A pointer to arguments array    
 *  @return      	WLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
process_bca_ts(int argc, char *argv[])
{
    int ret, i;
    struct iwreq iwr;
    wlan_ioctl_bca_timeshare_config bca_ts;
    int ioctl_val, subioctl_val;

    if (get_priv_ioctl("bca-ts",
                       &ioctl_val, &subioctl_val) == WLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    if (argc < 5) {
        printf("Error: invalid no of arguments\n");
        printf("Syntax: ./wlanconfig eth1 bca_ts get/set <p1>" " <p2> <p3>\n");
        exit(1);
    }

    memset(&bca_ts, 0, sizeof(wlan_ioctl_bca_timeshare_config));

    if ((ret = atoval(argv[4])) < 0)
        return -EINVAL;
    if ((ret > TRAFFIC_TYPE_MEDIUMHIGH) && (ret != TRAFFIC_TYPE_RESET))
        return -EINVAL;
    bca_ts.TrafficType = (u16) ret;     /* 0 or 1 */

    if (!strcmp(argv[3], "get")) {
        bca_ts.Action = 0;
    } else if (!strncmp(argv[3], "set", 3)) {
        if ((argc != 7) && (bca_ts.TrafficType != TRAFFIC_TYPE_RESET)) {
            printf("Error: invalid no of arguments\n");
            printf("Syntax: ./wlanconfig eth1 bca_ts get/set"
                   " <p1> <p2> <p3>\n");
            exit(1);
        }

        bca_ts.Action = 1;
        if (argc == 7) {
            if ((ret = atoval(argv[5])) < 0)
                return -EINVAL;
            /* If value is not multiple of 10 then take the floor value */
            i = ret % 10;
            ret -= i;
            /* Valid Range for TimeShareInterval: < 20 ... 60_000 > ms */
            if (ret < 20 || ret > 60000) {
                printf("Invalid TimeShareInterval Range:"
                       " < 20 ... 60000 > ms\n");
                return -EINVAL;
            }
            bca_ts.TimeShareInterval = (u32) ret;

            if ((ret = atoval(argv[6])) < 0)
                return -EINVAL;
            /* If value is not multiple of 10 then take the floor value */
            i = ret % 10;
            ret -= i;

            if (ret > bca_ts.TimeShareInterval) {
                printf("Invalid BTTime"
                       "  Range: < 0 .. TimeShareInterval > ms\n");
                return -EINVAL;
            }
            bca_ts.BTTime = (u32) ret;
        }
    } else {
        return -EINVAL;
    }

    memset(&iwr, 0, sizeof(iwr));

    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.pointer = (caddr_t) & bca_ts;
    iwr.u.data.length = sizeof(wlan_ioctl_bca_timeshare_config);
    iwr.u.data.flags = subioctl_val;

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("wlanconfig");
        return -EFAULT;
    }

    display_bca_ts_params(&bca_ts);

    return WLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Provision the driver with a IEEE IE for use in the next join cmd
 *
 *    Test function used to check the ioctl and driver functionality 
 *  
 *  @return WLAN_STATUS_SUCCESS or ioctl error code
 */
#if (WIRELESS_EXT >= 18)
static int
process_setgenie(void)
{
    struct iwreq iwr;

    u8 testIE[] = { 0xdd, 0x09,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99
    };

    memset(&iwr, 0x00, sizeof(iwr));
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.pointer = (caddr_t) testIE;
    iwr.u.data.length = sizeof(testIE);

    if (ioctl(sockfd, SIOCSIWGENIE, &iwr) < 0) {
        perror("wlanconfig: setgenie ioctl");
        return -EFAULT;
    }

    return WLAN_STATUS_SUCCESS;
}
#else
static int
process_setgenie(void)
{
    int ioctl_val, subioctl_val;
    struct iwreq iwr;

    u8 testIE[] = { 0xdd, 0x09,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99
    };

    if (get_priv_ioctl("setgenie",
                       &ioctl_val, &subioctl_val) == WLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.pointer = (caddr_t) testIE;
    iwr.u.data.length = sizeof(testIE);
    iwr.u.data.flags = subioctl_val;

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("wlanconfig: setgenie ioctl");
        return -EFAULT;
    }

    return WLAN_STATUS_SUCCESS;
}
#endif

/**
 *  @brief Helper function for process_getscantable_idx
 *
 *  @param pBuf     A pointer to the buffer
 *  @param buf_len  buffer length    
 *
 *  @return         NA
 *
 */
static void
dump_scan_elems(const u8 * pBuf, uint buf_len)
{
    uint idx;
    uint marker = 2 + pBuf[1];

    for (idx = 0; idx < buf_len; idx++) {
        if (idx % 0x10 == 0) {
            printf("\n%04x: ", idx);
        }

        if (idx == marker) {
            printf("|");
            marker = idx + pBuf[idx + 1] + 2;
        } else {
            printf(" ");
        }

        printf("%02x ", pBuf[idx]);
    }

    printf("\n");
}

/**
 *  @brief Helper function for process_getscantable_idx
 *	Find next element
 *
 *  @param pp_ie_out    pointer of a IEEEtypes_Generic_t structure pointer              
 *  @param p_buf_left   integer pointer, which contains the number of left p_buf
 *
 *  @return             WLAN_STATUS_SUCCESS on success, otherwise WLAN_STATUS_FAILURE
 */
static int
scantable_elem_next(IEEEtypes_Generic_t ** pp_ie_out, int *p_buf_left)
{
    IEEEtypes_Generic_t *pIeGen;
    u8 *p_next;

    if (*p_buf_left < 2) {
        return WLAN_STATUS_FAILURE;
    }

    pIeGen = *pp_ie_out;

    p_next = (u8 *) pIeGen + (pIeGen->IeeeHdr.Len + sizeof(pIeGen->IeeeHdr));
    *p_buf_left -= (p_next - (u8 *) pIeGen);

    *pp_ie_out = (IEEEtypes_Generic_t *) p_next;

    if (*p_buf_left <= 0) {
        return WLAN_STATUS_FAILURE;
    }

    return WLAN_STATUS_SUCCESS;
}

 /**
  *  @brief Helper function for process_getscantable_idx
  *         scantable find element
  *
  *  @param ie_buf       pointer of the IE buffer                
  *  @param ie_buf_len   IE buffer length
  *  @param ie_type      IE type
  *  @param ppIeOut      pointer to the IEEEtypes_Generic_t structure pointer
  *  @return             WLAN_STATUS_SUCCESS on success, otherwise WLAN_STATUS_FAILURE
  */
static int
scantable_find_elem(u8 * ie_buf,
                    unsigned int ie_buf_len,
                    IEEEtypes_ElementId_e ie_type,
                    IEEEtypes_Generic_t ** ppIeOut)
{
    int found;
    unsigned int ie_buf_left;

    ie_buf_left = ie_buf_len;

    found = FALSE;

    *ppIeOut = (IEEEtypes_Generic_t *) ie_buf;

    do {
        found = ((*ppIeOut)->IeeeHdr.ElementId == ie_type);

    } while (!found &&
             (scantable_elem_next(ppIeOut, (int *) &ie_buf_left) == 0));

    if (!found) {
        *ppIeOut = NULL;
    }

    return (found ? WLAN_STATUS_SUCCESS : WLAN_STATUS_FAILURE);
}

 /**
  *  @brief Helper function for process_getscantable_idx
  *         It gets SSID from IE 
  *
  *  @param ie_buf       IE buffer
  *  @param ie_buf_len   IE buffer length
  *  @param pSsid        SSID
  *  @param ssid_buf_max size of SSID    
  *  @return             WLAN_STATUS_SUCCESS on success, otherwise WLAN_STATUS_FAILURE
  */
static int
scantable_get_ssid_from_ie(u8 * ie_buf,
                           unsigned int ie_buf_len,
                           u8 * pSsid, unsigned int ssid_buf_max)
{
    int retval;
    IEEEtypes_Generic_t *pIeGen;

    retval = scantable_find_elem(ie_buf, ie_buf_len, SSID, &pIeGen);

    memcpy(pSsid, pIeGen->Data, MIN(pIeGen->IeeeHdr.Len, ssid_buf_max));

    return retval;
}

/** 
 *  @brief Provision the driver with a IEEE IE for use in the next join cmd
 *
 *  @param tableIdx table index
 *  @return         WLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
process_getscantable_idx(int tableIdx)
{
    int ioctl_val, subioctl_val;
    struct iwreq iwr;
    u8 *pCurrent;
    int bssInfoLen;
    char ssid[33];
    u16 tmp_cap;
    u8 tsf[8];
    u16 beaconInterval;
    u8 scanRspBuffer[500];      /* Stack buffer can be as large as ioctl allows 
                                 */
    IEEEtypes_CapInfo_t capInfo;
    wlan_ioctl_get_scan_table_info *pRspInfo;
    wlan_ioctl_get_scan_table_entry *pRspEntry;

    memset(ssid, 0x00, sizeof(ssid));

    pRspInfo = (wlan_ioctl_get_scan_table_info *) scanRspBuffer;

    pRspInfo->scanNumber = tableIdx;

    if (get_priv_ioctl("getscantable",
                       &ioctl_val, &subioctl_val) == WLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    /* 
     * Set up and execute the ioctl call
     */
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.pointer = (caddr_t) pRspInfo;
    iwr.u.data.length = sizeof(scanRspBuffer);
    iwr.u.data.flags = subioctl_val;

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("wlanconfig: getscantable ioctl");
        return -EFAULT;
    }

    pRspEntry
        = (wlan_ioctl_get_scan_table_entry *) pRspInfo->scan_table_entry_buffer;

    if (pRspInfo->scanNumber == 0) {
        printf("wlanconfig: getscantable ioctl - index out of range\n");
        return -EINVAL;
    }

    pCurrent = pRspInfo->scan_table_entry_buffer;
    pCurrent += (sizeof(pRspEntry->fixedFieldLength) +
                 pRspEntry->fixedFieldLength);

    bssInfoLen = pRspEntry->bssInfoLength;
    pCurrent += sizeof(pRspEntry->bssInfoLength);

    /* time stamp is 8 byte long */
    memcpy(tsf, pCurrent, sizeof(tsf));
    pCurrent += sizeof(tsf);
    bssInfoLen -= sizeof(tsf);

    /* beacon interval is 2 byte long */
    memcpy(&beaconInterval, pCurrent, sizeof(beaconInterval));
    pCurrent += sizeof(beaconInterval);
    bssInfoLen -= sizeof(beaconInterval);

    /* capability information is 2 byte long */
    memcpy(&capInfo, pCurrent, sizeof(capInfo));
    pCurrent += sizeof(capInfo);
    bssInfoLen -= sizeof(capInfo);

    scantable_get_ssid_from_ie(pCurrent, bssInfoLen, (u8 *) ssid, sizeof(ssid));

    printf("\n*** [%s], %02x:%02x:%02x:%02x:%02x:%2x\n",
           ssid,
           pRspEntry->fixedFields.bssid[0], pRspEntry->fixedFields.bssid[1],
           pRspEntry->fixedFields.bssid[2], pRspEntry->fixedFields.bssid[3],
           pRspEntry->fixedFields.bssid[4], pRspEntry->fixedFields.bssid[5]);
    memcpy(&tmp_cap, &capInfo, sizeof(tmp_cap));
    printf("Channel = %d, SS = %d, CapInfo = 0x%04x, BcnIntvl = %d\n",
           pRspEntry->fixedFields.channel,
           255 - pRspEntry->fixedFields.rssi, tmp_cap, beaconInterval);

    printf("TSF Values: AP(0x%02x%02x%02x%02x%02x%02x%02x%02x), ",
           tsf[7], tsf[6], tsf[5], tsf[4], tsf[3], tsf[2], tsf[1], tsf[0]);

    printf("Network(0x%016llx)\n", pRspEntry->fixedFields.networkTSF);
    printf("\n");
    printf("Element Data (%d bytes)\n", bssInfoLen);
    printf("------------");
    dump_scan_elems(pCurrent, bssInfoLen);
    printf("\n");

    return WLAN_STATUS_SUCCESS;
}

/**
 *  @brief Retrieve and display the contents of the driver scan table.
 *
 *  The ioctl to retrieve the scan table contents will be invoked, and portions
 *   of the scan data will be displayed on stdout.  The entire beacon or 
 *   probe response is also retrieved (if available in the driver).  This 
 *   data would be needed in case the application was explicitly controlling
 *   the association (inserting IEs, TLVs, etc).
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *
 *  @return         WLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
process_getscantable(int argc, char *argv[])
{
    int ioctl_val, subioctl_val;
    struct iwreq iwr;
    u8 scanRspBuffer[500];      /* Stack buffer can be as large as ioctl allows 
                                 */

    unsigned int scanStart;
    unsigned int idx;

    u8 *pCurrent;
    u8 *pNext;
    IEEEtypes_ElementId_e *pElementId;
    u8 *pElementLen;
    int bssInfoLen;
    int ssidIdx;
    u8 *pByte;
    char ssid[33];
    int ssidLen = 0;

    IEEEtypes_CapInfo_t capInfo = { 0 };
    u8 tsf[8];
    u16 beaconInterval;

    IEEEtypes_VendorSpecific_t *pWpaIe;
    const u8 wpa_oui[4] = { 0x00, 0x50, 0xf2, 0x01 };

    IEEEtypes_WmmParameter_t *pWmmIe;
    const u8 wmm_oui[4] = { 0x00, 0x50, 0xf2, 0x02 };
    char wmmCap;
    char wscCap;
    char dot11kCap;
    char dot11rCap;
    char privCap;

    int displayedInfo;

    wlan_ioctl_get_scan_table_info *pRspInfo;
    wlan_ioctl_get_scan_table_entry *pRspEntry;

    pRspInfo = (wlan_ioctl_get_scan_table_info *) scanRspBuffer;

    if (get_priv_ioctl("getscantable",
                       &ioctl_val, &subioctl_val) == WLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    if (argc > 3 && (strcmp(argv[3], "tsf") != 0)
        && (strcmp(argv[3], "help") != 0)) {

        idx = strtol(argv[3], NULL, 10);

        if (idx >= 0) {
            return process_getscantable_idx(idx);
        }
    }

    displayedInfo = FALSE;
    scanStart = 1;

    printf("---------------------------------------");
    printf("---------------------------------------\n");
    printf("# | ch  | ss  |       bssid       |   cap   |   SSID \n");
    printf("---------------------------------------");
    printf("---------------------------------------\n");

    do {
        pRspInfo->scanNumber = scanStart;

        /* 
         * Set up and execute the ioctl call
         */
        strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
        iwr.u.data.pointer = (caddr_t) pRspInfo;
        iwr.u.data.length = sizeof(scanRspBuffer);
        iwr.u.data.flags = subioctl_val;

        if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
            perror("wlanconfig: getscantable ioctl");
            return -EFAULT;
        }

        pCurrent = 0;
        pNext = pRspInfo->scan_table_entry_buffer;

        for (idx = 0; idx < pRspInfo->scanNumber; idx++) {

            /* 
             * Set pCurrent to pNext in case pad bytes are at the end
             *   of the last IE we processed.
             */
            pCurrent = pNext;

            pRspEntry = (wlan_ioctl_get_scan_table_entry *) pCurrent;

            printf("%02u| %03d | %03d | %02x:%02x:%02x:%02x:%02x:%02x |",
                   scanStart + idx,
                   pRspEntry->fixedFields.channel,
                   255 - pRspEntry->fixedFields.rssi,
                   pRspEntry->fixedFields.bssid[0],
                   pRspEntry->fixedFields.bssid[1],
                   pRspEntry->fixedFields.bssid[2],
                   pRspEntry->fixedFields.bssid[3],
                   pRspEntry->fixedFields.bssid[4],
                   pRspEntry->fixedFields.bssid[5]);

            displayedInfo = TRUE;

            pCurrent += (sizeof(pRspEntry->fixedFieldLength) +
                         pRspEntry->fixedFieldLength);

            bssInfoLen = pRspEntry->bssInfoLength;
            pCurrent += sizeof(pRspEntry->bssInfoLength);
            pNext = pCurrent + pRspEntry->bssInfoLength;

            if (bssInfoLen >= (sizeof(tsf)
                               + sizeof(beaconInterval) + sizeof(capInfo))) {
                /* time stamp is 8 byte long */
                memcpy(tsf, pCurrent, sizeof(tsf));
                pCurrent += sizeof(tsf);
                bssInfoLen -= sizeof(tsf);

                /* beacon interval is 2 byte long */
                memcpy(&beaconInterval, pCurrent, sizeof(beaconInterval));
                pCurrent += sizeof(beaconInterval);
                bssInfoLen -= sizeof(beaconInterval);

                /* capability information is 2 byte long */
                memcpy(&capInfo, pCurrent, sizeof(capInfo));
                pCurrent += sizeof(capInfo);
                bssInfoLen -= sizeof(capInfo);
            }

            wmmCap = ' ';       /* M (WMM), C (WMM-Call Admission Control) */
            wscCap = ' ';       /* "S" */
            dot11kCap = ' ';    /* "K" */
            dot11rCap = ' ';    /* "R" */

            /* "P" for Privacy (WEP) since "W" is WPA, and "2" is RSN/WPA2 */
            privCap = capInfo.Privacy ? 'P' : ' ';

            while (bssInfoLen >= 2) {
                pElementId = (IEEEtypes_ElementId_e *) pCurrent;
                pElementLen = pCurrent + 1;
                pCurrent += 2;

                switch (*pElementId) {

                case SSID:
                    if (*pElementLen && *pElementLen <= MRVDRV_MAX_SSID_LENGTH) {
                        memcpy(ssid, pCurrent, *pElementLen);
                        ssidLen = *pElementLen;
                    }
                    break;

                case WPA_IE:
                    pWpaIe = (IEEEtypes_VendorSpecific_t *) pElementId;
                    if ((memcmp
                         (pWpaIe->VendHdr.Oui, wpa_oui,
                          sizeof(pWpaIe->VendHdr.Oui)) == 0)
                        && (pWpaIe->VendHdr.OuiType == wpa_oui[3])) {
                        /* WPA IE found, 'W' for WPA */
                        privCap = 'W';
                    } else {
                        pWmmIe = (IEEEtypes_WmmParameter_t *) pElementId;
                        if ((memcmp(pWmmIe->VendHdr.Oui,
                                    wmm_oui, sizeof(pWmmIe->VendHdr.Oui)) == 0)
                            && (pWmmIe->VendHdr.OuiType == wmm_oui[3])) {
                            /* Check the subtype: 1 == parameter, 0 == info */
                            if ((pWmmIe->VendHdr.OuiSubtype == 1)
                                && pWmmIe->AcParams[WMM_AC_VO].AciAifsn.Acm) {
                                /* Call admission on VO; 'C' for CAC */
                                wmmCap = 'C';
                            } else {
                                /* No CAC; 'M' for uh, WMM */
                                wmmCap = 'M';
                            }
                        }
                    }
                    break;

                case RSN_IE:
                    /* RSN IE found; '2' for WPA2 (RSN) */
                    privCap = '2';
                    break;

                default:
                    break;
                }

                pCurrent += *pElementLen;
                bssInfoLen -= (2 + *pElementLen);
            }

            /* "A" for Adhoc "I" for Infrastructure, "D" for DFS (Spectrum
               Mgmt) */
            printf(" %c%c%c%c%c%c%c | ", capInfo.Ibss ? 'A' : 'I', privCap,     /* P 
                                                                                   (WEP), 
                                                                                   W 
                                                                                   (WPA), 
                                                                                   2 
                                                                                   (WPA2) 
                                                                                 */
                   capInfo.SpectrumMgmt ? 'D' : ' ', wmmCap,    /* M (WMM), C
                                                                   (WMM-Call
                                                                   Admission
                                                                   Control) */
                   dot11kCap,   /* K */
                   dot11rCap,   /* R */
                   wscCap);     /* S */

            /* Print out the ssid or the hex values if non-printable */
            for (ssidIdx = 0; ssidIdx < ssidLen; ssidIdx++) {
                if (isprint(ssid[ssidIdx])) {
                    printf("%c", ssid[ssidIdx]);
                } else {
                    printf("\\%02x", ssid[ssidIdx]);
                }
            }

            printf("\n");

            if (argc > 3 && strcmp(argv[3], "tsf") == 0) {
                /* TSF is a u64, some formatted printing libs have trouble
                   printing long longs, so cast and dump as bytes */
                pByte = (u8 *) & pRspEntry->fixedFields.networkTSF;
                printf("    TSF=%02x%02x%02x%02x%02x%02x%02x%02x\n",
                       pByte[7], pByte[6], pByte[5], pByte[4],
                       pByte[3], pByte[2], pByte[1], pByte[0]);
            }
        }

        scanStart += pRspInfo->scanNumber;

    } while (pRspInfo->scanNumber);

    if (displayedInfo == TRUE) {
        if (argc > 3 && strcmp(argv[3], "help") == 0) {
            printf("\n\n"
                   "Capability Legend (Not all may be supported)\n"
                   "-----------------\n"
                   " I [ Infrastructure ]\n"
                   " A [ Ad-hoc ]\n"
                   " W [ WPA IE ]\n"
                   " 2 [ WPA2/RSN IE ]\n"
                   " M [ WMM IE ]\n"
                   " C [ Call Admission Control - WMM IE, VO ACM set ]\n"
                   " D [ Spectrum Management - DFS (11h) ]\n"
                   " K [ 11k ]\n" " R [ 11r ]\n" " S [ WSC ]\n" "\n\n");
        }
    } else {
        printf("< No Scan Results >\n");
    }

    return WLAN_STATUS_SUCCESS;
}

/** Maximum channel scratch */
#define MAX_CHAN_SCRATCH  100
/**
 *  @brief Request a scan from the driver and display the scan table afterwards
 *
 *  Command line interface for performing a specific immediate scan based
 *    on the following keyword parsing:
 *
 *     chan=[chan#][band][mode] where band is [a,b,g] and mode is 
 *                              blank for active or 'p' for passive
 *     bssid=xx:xx:xx:xx:xx:xx  specify a BSSID filter for the scan
 *     ssid="[SSID]"            specify a SSID filter for the scan
 *     keep=[0 or 1]            keep the previous scan results (1), discard (0)
 *     dur=[scan time]          time to scan for each channel in milliseconds
 *     probes=[#]               number of probe requests to send on each chan
 *     type=[1,2,3]             BSS type: 1 (Infra), 2(Adhoc), 3(Any)
 *
 *  Any combination of the above arguments can be supplied on the command line.
 *    If the chan token is absent, a full channel scan will be completed by 
 *    the driver.  If the dur or probes tokens are absent, the drivers default
 *    setting will be used.  The bssid and ssid fields, if blank, 
 *    will produce an unfiltered scan. The type field will default to 3 (Any)
 *    and the keep field will default to 0 (Discard).  
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *
 *  @return         WLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
process_setuserscan(int argc, char *argv[])
{
    wlan_ioctl_user_scan_cfg scanReq;
    int ioctl_val, subioctl_val;
    struct iwreq iwr;
    char *pArgTok;
    char *pChanTok;
    char *pArgCookie;
    char *pChanCookie;
    int argIdx;
    int chanParseIdx;
    int chanCmdIdx;
    char chanScratch[MAX_CHAN_SCRATCH];
    char *pScratch;
    int tmpIdx;
    int scanTime;
    int numSsid;
    unsigned int mac[ETH_ALEN];

    memset(&scanReq, 0x00, sizeof(scanReq));
    chanCmdIdx = 0;
    scanTime = 0;
    numSsid = 0;

    if (get_priv_ioctl("setuserscan",
                       &ioctl_val, &subioctl_val) == WLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    for (argIdx = 0; argIdx < argc; argIdx++) {
        if (strncmp(argv[argIdx], "chan=", strlen("chan=")) == 0) {
            /* 
             *  "chan" token string handler
             */
            pArgTok = argv[argIdx] + strlen("chan=");

            if (strlen(pArgTok) > MAX_CHAN_SCRATCH) {

                printf("Error: Specified channels exceeds max limit\n");
                return WLAN_STATUS_FAILURE;
            }

            while ((pArgTok = strtok_r(pArgTok, ",", &pArgCookie)) != NULL) {

                memset(chanScratch, 0x00, sizeof(chanScratch));
                pScratch = chanScratch;

                for (chanParseIdx = 0;
                     chanParseIdx < strlen(pArgTok); chanParseIdx++) {
                    if (isalpha(*(pArgTok + chanParseIdx))) {
                        *pScratch++ = ' ';
                    }

                    *pScratch++ = *(pArgTok + chanParseIdx);
                }
                *pScratch = 0;
                pArgTok = NULL;

                pChanTok = chanScratch;

                while ((pChanTok = strtok_r(pChanTok, " ",
                                            &pChanCookie)) != NULL) {
                    if (isdigit(*pChanTok)) {
                        scanReq.chanList[chanCmdIdx].chanNumber
                            = atoi(pChanTok);
                    } else {
                        switch (toupper(*pChanTok)) {
                        case 'A':
                            scanReq.chanList[chanCmdIdx].radioType = 1;
                            break;
                        case 'B':
                        case 'G':
                            scanReq.chanList[chanCmdIdx].radioType = 0;
                            break;
                        case 'P':
                            scanReq.chanList[chanCmdIdx].scanType = 1;
                            break;
                        }
                    }
                    pChanTok = NULL;
                }
                chanCmdIdx++;
            }
        } else if (strncmp(argv[argIdx], "bssid=", strlen("bssid=")) == 0) {
            /* 
             *  "bssid" token string handler
             */
            sscanf(argv[argIdx] + strlen("bssid="), "%2x:%2x:%2x:%2x:%2x:%2x",
                   mac + 0, mac + 1, mac + 2, mac + 3, mac + 4, mac + 5);

            for (tmpIdx = 0; tmpIdx < NELEMENTS(mac); tmpIdx++) {
                scanReq.specificBSSID[tmpIdx] = (u8) mac[tmpIdx];
            }
        } else if (strncmp(argv[argIdx], "keep=", strlen("keep=")) == 0) {
            /* 
             *  "keep" token string handler
             */
            scanReq.keepPreviousScan = atoi(argv[argIdx] + strlen("keep="));
        } else if (strncmp(argv[argIdx], "dur=", strlen("dur=")) == 0) {
            /* 
             *  "dur" token string handler
             */
            scanTime = atoi(argv[argIdx] + strlen("dur="));
        } else if (strncmp(argv[argIdx], "ssid=", strlen("ssid=")) == 0) {
            /* 
             *  "ssid" token string handler
             */
            if (numSsid < MRVDRV_MAX_SSID_LIST_LENGTH) {
                strncpy(scanReq.ssidList[numSsid].ssid,
                        argv[argIdx] + strlen("ssid="),
                        sizeof(scanReq.ssidList[numSsid].ssid));

                scanReq.ssidList[numSsid].maxLen = 0;

                numSsid++;
            }
        } else if (strncmp(argv[argIdx], "wc=", strlen("wc=")) == 0) {

            if (numSsid < MRVDRV_MAX_SSID_LIST_LENGTH) {
                /* 
                 *  "wc" token string handler
                 */
                pScratch = rindex(argv[argIdx], ',');

                if (pScratch) {
                    *pScratch = 0;
                    pScratch++;

                    if (isdigit(*pScratch)) {
                        scanReq.ssidList[numSsid].maxLen = atoi(pScratch);
                    } else {
                        scanReq.ssidList[numSsid].maxLen = *pScratch;
                    }
                } else {
                    /* Standard wildcard matching */
                    scanReq.ssidList[numSsid].maxLen = 0xFF;
                }

                strncpy(scanReq.ssidList[numSsid].ssid,
                        argv[argIdx] + strlen("wc="),
                        sizeof(scanReq.ssidList[numSsid].ssid));

                numSsid++;
            }
        } else if (strncmp(argv[argIdx], "probes=", strlen("probes=")) == 0) {
            /* 
             *  "probes" token string handler
             */
            scanReq.numProbes = atoi(argv[argIdx] + strlen("probes="));
        } else if (strncmp(argv[argIdx], "type=", strlen("type=")) == 0) {
            /* 
             *  "type" token string handler
             */
            scanReq.bssType = atoi(argv[argIdx] + strlen("type="));
            switch (scanReq.bssType) {
            case WLAN_SCAN_BSS_TYPE_BSS:
            case WLAN_SCAN_BSS_TYPE_IBSS:
                break;

            default:
            case WLAN_SCAN_BSS_TYPE_ANY:
                /* Set any unknown types to ANY */
                scanReq.bssType = WLAN_SCAN_BSS_TYPE_ANY;
            }
        }
    }

    /* 
     * Update all the channels to have the same scan time
     */
    for (tmpIdx = 0; tmpIdx < chanCmdIdx; tmpIdx++) {
        scanReq.chanList[tmpIdx].scanTime = scanTime;
    }

    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.pointer = (caddr_t) & scanReq;
    iwr.u.data.length = sizeof(scanReq);
    iwr.u.data.flags = subioctl_val;

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("wlanconfig: setuserscan ioctl");
        return -EFAULT;
    }

    process_getscantable(0, 0);

    return WLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Provision the driver with a Marvell TLV for use in the next join cmd
 *
 *    Test function used to check the ioctl and driver functionality 
 *  
 *  @return WLAN_STATUS_SUCCESS or ioctl error code
 */
static int
process_setmrvltlv(void)
{
    int ioctl_val, subioctl_val;
    struct iwreq iwr;

    u8 testTlv[] = { 0x0A, 0x01, 0x0C, 0x00,
        0xdd, 10, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
    };

    if (get_priv_ioctl("setmrvltlv",
                       &ioctl_val, &subioctl_val) == WLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.pointer = (caddr_t) testTlv;
    iwr.u.data.length = sizeof(testTlv);
    iwr.u.data.flags = subioctl_val;

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("wlanconfig: setmrvltlv ioctl");
        return -EFAULT;
    }

    return WLAN_STATUS_SUCCESS;
}

/**
 *  @brief Retrieve the association response from the driver
 *
 *  Retrieve the buffered (re)association management frame from the driver.
 *    The response is identical to the one received from the AP and conforms
 *    to the IEEE specification.
 *
 *  @return WLAN_STATUS_SUCCESS or ioctl error code
 */
static int
process_getassocrsp(void)
{
    int ioctl_val, subioctl_val;
    struct iwreq iwr;
    u8 assocRspBuffer[500];     /* Stack buffer can be as large as ioctl allows 
                                 */
    IEEEtypes_AssocRsp_t *pAssocRsp;

    pAssocRsp = (IEEEtypes_AssocRsp_t *) assocRspBuffer;

    if (get_priv_ioctl("getassocrsp",
                       &ioctl_val, &subioctl_val) == WLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.pointer = (caddr_t) assocRspBuffer;
    iwr.u.data.length = sizeof(assocRspBuffer);
    iwr.u.data.flags = subioctl_val;

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("wlanconfig: getassocrsp ioctl");
        return -EFAULT;
    }

    if (iwr.u.data.length) {
        printf("getassocrsp: Status[%d], Cap[0x%04x]: ",
               pAssocRsp->StatusCode, *(u16 *) & pAssocRsp->Capability);
        hexdump(NULL, assocRspBuffer, iwr.u.data.length, ' ');
    } else {
        printf("getassocrsp: <empty>\n");
    }

    return WLAN_STATUS_SUCCESS;
}

/**
 *  @brief Get TSF
 *
 *  @return WLAN_STATUS_SUCCESS or ioctl error code
 */
static int
process_gettsf(void)
{
    int ioctl_val, subioctl_val;
    struct iwreq iwr;
    u8 tsf[8];

    if (get_priv_ioctl("gettsf",
                       &ioctl_val, &subioctl_val) == WLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.pointer = (caddr_t) tsf;
    iwr.u.data.length = sizeof(u64);
    iwr.u.data.flags = subioctl_val;

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("gettsf ioctl");
        return -EFAULT;
    }

    printf("TSF=%02x%02x%02x%02x%02x%02x%02x%02x\n",
           tsf[7], tsf[6], tsf[5], tsf[4], tsf[3], tsf[2], tsf[1], tsf[0]);

    return WLAN_STATUS_SUCCESS;
}

/** 
 *  @brief scan network with specific ssid
 *  @param argc		number of arguments
 *  @param argv         A pointer to arguments array    
 *  @return      	WLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
process_extscan(int argc, char *argv[])
{
    struct iwreq iwr;
    WCON_SSID Ssid;
    int ioctl_val, subioctl_val;

    if (get_priv_ioctl("extscan",
                       &ioctl_val, &subioctl_val) == WLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    if (argc != 4) {
        printf("Error: invalid no of arguments\n");
        printf("Syntax: ./wlanconfig eth1 extscan <SSID>\n");
        exit(1);
    }

    printf("Ssid: %s\n", argv[3]);

    memset(&Ssid, 0, sizeof(Ssid));
    memset(&iwr, 0, sizeof(iwr));

    if (strlen(argv[3]) <= IW_ESSID_MAX_SIZE) {
        Ssid.ssid_len = strlen(argv[3]);
    } else {
        printf("Invalid SSID:Exceeds more than 32 charecters\n");
        exit(1);
    }
    memcpy(Ssid.ssid, argv[3], Ssid.ssid_len);

    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.pointer = (caddr_t) & Ssid;
    iwr.u.data.length = sizeof(Ssid);
    iwr.u.data.flags = subioctl_val;

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("wlanconfig");
        return -EFAULT;
    }

    return WLAN_STATUS_SUCCESS;
}

#if WIRELESS_EXT > 14
/** 
 *  @brief parse custom info
 *  @param pHandle	A pointer to WCON_HANDLE
 *  @param data         A pointer to iw_point structure
 *  @param idx          AP index
 *  @return      	NA
 */
static void
parse_custom_info(WCON_HANDLE * pHandle, struct iw_point *data, s32 idx)
{
    s32 i = 0;
    s8 *custom_cmd[] = { "wpa_ie", "rsn_ie", NULL };

    if (!data->pointer || !data->length) {
        printf("iw_point: Invalid Pointer/Length\n");
        return;
    }

    if (!strncmp(data->pointer, "wmm_ie", strlen("wmm_ie"))) {
        pHandle->ScanList[idx].Wmm = WCON_WMM_ENABLED;
    }

    while (custom_cmd[i]) {
        if (!strncmp(data->pointer, custom_cmd[i], strlen(custom_cmd[i]))) {
            pHandle->ScanList[idx].WpaAP = WCON_WPA_ENABLED;
            break;
        }
        i++;
    }

    printf("Wpa:\t %s\n", pHandle->ScanList[idx].WpaAP ?
           "enabled" : "disabled");
    printf("Wmm:\t %s\n", pHandle->ScanList[idx].Wmm ? "enabled" : "disabled");
}
#endif

/** 
 *  @brief parse scan info
 *  @param pHandle	A pointer to WCON_HANDLE
 *  @param buffer       A pointer to scan result buffer
 *  @param length       length of scan result buffer
 *  @return      	NA
 */
static void
parse_scan_info(WCON_HANDLE * pHandle, u8 buffer[], s32 length)
{
    s32 len = 0;
    u32 ap_index = 0;
    int new_index = FALSE;
    s8 *mode[4] = { "auto", "ad-hoc", "infra", "master" };
    struct iw_event iwe;
    struct iw_point iwp;

    memset(pHandle->ScanList, 0, sizeof(pHandle->ScanList));
    memset((u8 *) & iwe, 0, sizeof(struct iw_event));
    memset((u8 *) & iwp, 0, sizeof(struct iw_point));
    pHandle->ApNum = 0;

    while (len + IW_EV_LCP_LEN < length) {
        memcpy((s8 *) & iwe, buffer + len, sizeof(struct iw_event));
        if ((iwe.cmd == SIOCGIWESSID) || (iwe.cmd == SIOCGIWENCODE) ||
            (iwe.cmd == IWEVCUSTOM)) {
            if (we_version_compiled > 18)
                memcpy((s8 *) & iwp,
                       buffer + len + IW_EV_LCP_LEN - MRV_EV_POINT_OFF,
                       sizeof(struct iw_point));
            else
                memcpy((s8 *) & iwp, buffer + len + IW_EV_LCP_LEN,
                       sizeof(struct iw_point));
            iwp.pointer = buffer + len + IW_EV_POINT_LEN;
        }
        switch (iwe.cmd) {
        case SIOCGIWAP:
            if (new_index && ap_index < IW_MAX_AP - 1)
                ap_index++;
            memcpy(pHandle->ScanList[ap_index].Bssid,
                   iwe.u.ap_addr.sa_data, ETH_ALEN);
            printf("\nBSSID:\t %02X:%02X:%02X:%02X:%02X:%02X\n",
                   HWA_ARG(pHandle->ScanList[ap_index].Bssid));
            new_index = TRUE;
            break;

        case SIOCGIWESSID:
            if ((iwp.pointer) && (iwp.length)) {
                memcpy(pHandle->ScanList[ap_index].Ssid.ssid,
                       (s8 *) iwp.pointer, iwp.length);
                pHandle->ScanList[ap_index].Ssid.ssid_len = iwp.length;
            }
            printf("SSID:\t %s\n", pHandle->ScanList[ap_index].Ssid.ssid);
            break;

        case SIOCGIWENCODE:
            if (!(iwp.flags & IW_ENCODE_DISABLED)) {
                pHandle->ScanList[ap_index].Privacy = WCON_ENC_ENABLED;
            }
            printf("Privacy: %s\n",
                   pHandle->ScanList[ap_index].Privacy ?
                   "enabled" : "disabled");
            break;

        case SIOCGIWMODE:
            pHandle->ScanList[ap_index].NetMode = iwe.u.mode;
            printf("NetMode: %s\n", mode[pHandle->ScanList[ap_index].NetMode]);
            break;

#if WIRELESS_EXT > 14
        case IWEVCUSTOM:
            parse_custom_info(pHandle, &iwp, ap_index);
            break;
#endif

        case IWEVQUAL:
            pHandle->ScanList[ap_index].Rssi = iwe.u.qual.level;
            printf("Quality: %d\n", pHandle->ScanList[ap_index].Rssi);
            break;
        }

        len += iwe.len;
    }
    if (new_index)
        pHandle->ApNum = ap_index + 1;
    printf("\nNo of AP's = %d\n", pHandle->ApNum);

    return;
}

/** 
 *  @brief Process scan results
 *  @param argc		number of arguments
 *  @param argv         A pointer to arguments array    
 *  @return      	WLAN_STATUS_SUCCESS--success, otherwise--WLAN_STATUS_FAILURE
 */
static int
process_scanlist(int argc, char *argv[])
{
    u8 *buffer = NULL;
    u8 *newbuf = NULL;
    int buflen = IW_SCAN_MAX_DATA;
    struct iwreq iwr;
    WCON_HANDLE mhandle, *pHandle = &mhandle;

    memset(pHandle, 0, sizeof(WCON_HANDLE));
    memset(&iwr, 0, sizeof(struct iwreq));
  realloc:
    /* (Re)allocate the buffer - realloc(NULL, len) == malloc(len) */
    newbuf = realloc(buffer, buflen);
    if (newbuf == NULL) {
        if (buffer)
            free(buffer);
        fprintf(stderr, "%s: Allocation failed\n", __FUNCTION__);
        return WLAN_STATUS_FAILURE;
    }
    buffer = newbuf;

    iwr.u.data.pointer = buffer;
    iwr.u.data.length = buflen;
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);

    if ((ioctl(sockfd, SIOCGIWSCAN, &iwr)) < 0) {
        if ((errno == E2BIG)) {
            /* Some driver may return very large scan results, either * because 
               there are many cells, or because they have many * large elements 
               in cells (like IWEVCUSTOM). Most will * only need the regular
               sized buffer. We now use a dynamic * allocation of the buffer to 
               satisfy everybody. Of course, * as we don't know in advance the
               size of the array, we try * various increasing sizes. */

            /* Check if the driver gave us any hints. */
            if (iwr.u.data.length > buflen)
                buflen = iwr.u.data.length;
            else
                buflen *= 2;

            /* Try again */
            goto realloc;
        }
        printf("Get Scan Results Failed\n");
        free(buffer);
        return WLAN_STATUS_FAILURE;
    }

    parse_scan_info(pHandle, buffer, iwr.u.data.length);
    free(buffer);
    return WLAN_STATUS_SUCCESS;
}

/** 
 *  @brief convert string to hex integer
 *
 *  @param d		A pointer to dest buffer
 *  @param s		A pointer to source string buffer 
 *  @param dlen         max len of dest buffer
 *  @return            	WLAN_STATUS_SUCCESS or WLAN_STATUS_FAILURE
 */
static int
ascii2hex(u8 * d, s8 * s, u32 dlen)
{
    s32 i, j, len;
    u8 n;

    memset(d, 0x00, dlen);
    len = strlen(s);

    for (i = 0, j = 0; (i < len) && (j < dlen * 2); i++) {
        if (isxdigit(s[i])) {
            n = hexc2bin(s[i]);
            if ((j % 2) == 0)
                n <<= 4;
            d[j / 2] += n;
            j++;
        }
    }

    return j;
}

/** Max data size for crypto algorithm RC4,AES and AES_WRAP */
#define MAX_DATA_SIZE 64
/** Max data dize for crypto algorithm AES CCM */
#define MAX_DATA_SIZE_AES_CCM 32
/** 
 *  @brief Process crypto test
 *  @param arac		number of arguments
 *  @param argv         A pointer to arguments array    
 *  @return      	WLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
process_crypto_test(int argc, char *argv[])
{
    struct iwreq iwr;
    u8 buf[1024];
    HostCmd_DS_802_11_CRYPTO *cmd;
    HostCmd_DS_802_11_CRYPTO_AES_CCM *cmd_aes_ccm;
    MrvlIEtypes_Data_t *data;
    int ioctl_val, subioctl_val;
    u16 alg;

    if (get_priv_ioctl("crypto_test",
                       &ioctl_val, &subioctl_val) == WLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    memset(buf, 0, sizeof(buf));

    alg = atoi(argv[4]);        /* RC4 =1, AES=2, AES_WRAP=3 */
    if ((alg != CIPHER_TEST_RC4) &&
        (alg != CIPHER_TEST_AES) &&
        (alg != CIPHER_TEST_AES_KEY_WRAP) && (alg != CIPHER_TEST_AES_CCM)) {
        printf("Invalid Algorithm\n");
        return WLAN_STATUS_FAILURE;
    }
    if (alg == CIPHER_TEST_RC4 || alg == CIPHER_TEST_AES ||
        alg == CIPHER_TEST_AES_KEY_WRAP) {
        if (argc != 8) {
            printf("Invalid Number of Arguments\n");
            return WLAN_STATUS_FAILURE;
        }
    }
    if (alg == CIPHER_TEST_AES_CCM) {
        if (argc != 9) {
            printf("Invalid Number of Arguments\n");
            return WLAN_STATUS_FAILURE;
        }
    }

    if (alg != CIPHER_TEST_AES_CCM) {
        cmd = (HostCmd_DS_802_11_CRYPTO *) buf;
        cmd->EncDec = atoi(argv[3]);    /* Encrypt = 1, Decrypt = 0 */
        if ((cmd->EncDec != 1) && (cmd->EncDec != 0))
            return WLAN_STATUS_FAILURE;
        cmd->Algorithm = alg;
        cmd->KeyIVLength =
            ascii2hex(cmd->KeyIV, argv[5], sizeof(cmd->KeyIV)) / 2;
        cmd->KeyLength = ascii2hex(cmd->Key, argv[6], sizeof(cmd->Key)) / 2;
        data = (MrvlIEtypes_Data_t *) (buf + sizeof(HostCmd_DS_802_11_CRYPTO));
        data->Header.Len = ascii2hex(data->Data, argv[7], MAX_DATA_SIZE) / 2;
        data->Header.Type = TLV_TYPE_CRYPTO_DATA;

        printf
            ("crypto_test: EncDec=%d Algorithm=%d,IvLen=%d, KeyLen=%d,dataLen=%d\n",
             cmd->EncDec, cmd->Algorithm, cmd->KeyIVLength, cmd->KeyLength,
             data->Header.Len);
        hexdump("KeyIV", cmd->KeyIV, cmd->KeyIVLength, ' ');
        hexdump("Key", cmd->Key, cmd->KeyLength, ' ');
        hexdump("Data", data->Data, data->Header.Len, ' ');
        strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
        iwr.u.data.pointer = (caddr_t) cmd;
        iwr.u.data.length =
            sizeof(HostCmd_DS_802_11_CRYPTO) + sizeof(MrvlIEtypesHeader_t) +
            data->Header.Len;
    } else {
        cmd_aes_ccm = (HostCmd_DS_802_11_CRYPTO_AES_CCM *) buf;
        cmd_aes_ccm->EncDec = atoi(argv[3]);    /* Encrypt = 1, Decrypt = 0 */
        if ((cmd_aes_ccm->EncDec != 1) && (cmd_aes_ccm->EncDec != 0))
            return WLAN_STATUS_FAILURE;
        cmd_aes_ccm->Algorithm = alg;

        cmd_aes_ccm->KeyLength =
            ascii2hex(cmd_aes_ccm->Key, argv[5], sizeof(cmd_aes_ccm->Key)) / 2;
        cmd_aes_ccm->NonceLength =
            ascii2hex(cmd_aes_ccm->Nonce, argv[6],
                      sizeof(cmd_aes_ccm->Nonce)) / 2;
        cmd_aes_ccm->AADLength =
            ascii2hex(cmd_aes_ccm->AAD, argv[7], sizeof(cmd_aes_ccm->AAD)) / 2;

        data =
            (MrvlIEtypes_Data_t *) (buf +
                                    sizeof(HostCmd_DS_802_11_CRYPTO_AES_CCM));
        data->Header.Len =
            ascii2hex(data->Data, argv[8], MAX_DATA_SIZE_AES_CCM) / 2;
        data->Header.Type = TLV_TYPE_CRYPTO_DATA;

        printf
            ("crypto_test: EncDec=%d Algorithm=%d,KeyLen=%d, NonceLen=%d,AADLen=%d,dataLen=%d\n",
             cmd_aes_ccm->EncDec, cmd_aes_ccm->Algorithm,
             cmd_aes_ccm->KeyLength, cmd_aes_ccm->NonceLength,
             cmd_aes_ccm->AADLength, data->Header.Len);
        hexdump("Key", cmd_aes_ccm->Key, cmd_aes_ccm->KeyLength, ' ');
        hexdump("Nonce", cmd_aes_ccm->Nonce, cmd_aes_ccm->NonceLength, ' ');
        hexdump("AAD", cmd_aes_ccm->AAD, cmd_aes_ccm->AADLength, ' ');
        hexdump("Data", data->Data, data->Header.Len, ' ');
        strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
        iwr.u.data.pointer = (caddr_t) cmd_aes_ccm;
        iwr.u.data.length =
            sizeof(HostCmd_DS_802_11_CRYPTO_AES_CCM) +
            sizeof(MrvlIEtypesHeader_t) + data->Header.Len;
    }
    iwr.u.data.flags = subioctl_val;
    if (ioctl(sockfd, ioctl_val, &iwr) < 0)
        return WLAN_STATUS_FAILURE;

    if (alg != CIPHER_TEST_AES_CCM) {
        cmd = (HostCmd_DS_802_11_CRYPTO *) iwr.u.data.pointer;
        printf
            ("crypto_result: EncDec=%d Algorithm=%d,IvLen=%d, KeyLen=%d,dataLen=%d\n",
             cmd->EncDec, cmd->Algorithm, cmd->KeyIVLength, cmd->KeyLength,
             data->Header.Len);
        hexdump("KeyIV", cmd->KeyIV, cmd->KeyIVLength, ' ');
        hexdump("Key", cmd->Key, cmd->KeyLength, ' ');
        hexdump("Data", data->Data, data->Header.Len, ' ');
    } else {
        cmd_aes_ccm = (HostCmd_DS_802_11_CRYPTO_AES_CCM *) iwr.u.data.pointer;
        printf
            ("crypto_result: EncDec=%d Algorithm=%d,KeyLen=%d, NonceLen=%d,AADLen=%d,dataLen=%d\n",
             cmd_aes_ccm->EncDec, cmd_aes_ccm->Algorithm,
             cmd_aes_ccm->KeyLength, cmd_aes_ccm->NonceLength,
             cmd_aes_ccm->AADLength, data->Header.Len);
        hexdump("Key", cmd_aes_ccm->Key, cmd_aes_ccm->KeyLength, ' ');
        hexdump("Nonce", cmd_aes_ccm->Nonce, cmd_aes_ccm->NonceLength, ' ');
        hexdump("AAD", cmd_aes_ccm->AAD, cmd_aes_ccm->AADLength, ' ');
        hexdump("Data", data->Data, data->Header.Len, ' ');
    }
    return WLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Process read eeprom
 *
 *  @param stroffset	A pointer to the offset string
 *  @param strnob	A pointer to NOB string
 *  @return      	WLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
process_read_eeprom(s8 * stroffset, s8 * strnob)
{
    s8 buffer[MAX_EEPROM_DATA];
    struct ifreq userdata;
    wlan_ioctl_regrdwr *reg = (wlan_ioctl_regrdwr *) buffer;
    int ioctl_val, subioctl_val;

    if (get_priv_ioctl("regrdwr",
                       &ioctl_val, &subioctl_val) == WLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    memset(buffer, 0, sizeof(buffer));
    reg->WhichReg = REG_EEPROM;
    reg->Action = 0;

    if (!strncasecmp(stroffset, "0x", 2))
        reg->Offset = a2hex((stroffset + 2));
    else
        reg->Offset = atoi(stroffset);

    if (!strncasecmp(strnob, "0x", 2))
        reg->NOB = a2hex((strnob + 2));
    else
        reg->NOB = atoi(strnob);

    if ((reg->NOB + sizeof(wlan_ioctl_regrdwr)) > MAX_EEPROM_DATA) {
        fprintf(stderr, "Number of bytes exceeds MAX EEPROM Read size\n");
        return WLAN_STATUS_FAILURE;
    }

    strncpy(userdata.ifr_name, dev_name, IFNAMSIZ);
    userdata.ifr_data = buffer;

    if (ioctl(sockfd, ioctl_val, &userdata)) {
        perror("wlanconfig");
        fprintf(stderr,
                "wlanconfig: EEPROM read not possible "
                "by interface %s\n", dev_name);
        return WLAN_STATUS_FAILURE;
    }

    hexdump("RD EEPROM", &reg->Value, MIN(reg->NOB, MAX_EEPROM_DATA), ' ');

    return WLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Display usage
 *  
 *  @return       NA
 */
static void
display_usage(void)
{
    s32 i;

    for (i = 0; i < NELEMENTS(usage); i++)
        fprintf(stderr, "%s\n", usage[i]);
}

/** 
 *  @brief Find command
 *  
 *  @param maxcmds	max command number
 *  @param cmds		A pointer to commands buffer
 *  @param cmd		A pointer to command buffer
 *  @return      	index of command or WLAN_STATUS_FAILURE
 */
static int
findcommand(s32 maxcmds, s8 * cmds[], s8 * cmd)
{
    s32 i;

    for (i = 0; i < maxcmds; i++) {
        if (!strcasecmp(cmds[i], cmd)) {
            return i;
        }
    }

    return WLAN_STATUS_FAILURE;
}

/** 
 *  @brief Find command type
 *  
 *  @param cmd          command 
 *  @param pu16CmdType  A pointer to command type
 *  @return             WLAN_STATUS_SUCCESS on success otherwise WLAN_STATUS_FAILURE
 */
static int
get_cfg_data_cmdType(s8 * cmd, u16 * pu16CmdType)
{
    s32 i;

    for (i = 0; i < NELEMENTS(cfg_data_cmds); i++) {
        if (!strcasecmp(cfg_data_cmds[i].cmd, cmd)) {
            *pu16CmdType = cfg_data_cmds[i].cmdType;
            return WLAN_STATUS_SUCCESS;
        }
    }

    return WLAN_STATUS_FAILURE;
}

/** 
 *  @brief Get the current status of the WMM Queues
 *  
 *  Command: wlanconfig eth1 qstatus
 *
 *  Retrieve the following information for each AC if wmm is enabled:
 *        - WMM IE ACM Required
 *        - Firmware Flow Required 
 *        - Firmware Flow Established
 *        - Firmware Queue Enabled
 *  
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *
 *  @return         WLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
process_wmm_qstatus(int argc, char *argv[])
{
    int ioctl_val, subioctl_val;
    struct iwreq iwr;
    wlan_ioctl_wmm_queue_status_t qstatus;
    wlan_wmm_ac_e acVal;

    if (get_priv_ioctl("qstatus",
                       &ioctl_val, &subioctl_val) == WLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    memset(&qstatus, 0x00, sizeof(qstatus));

    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.flags = subioctl_val;
    iwr.u.data.pointer = (caddr_t) & qstatus;
    iwr.u.data.length = (sizeof(qstatus));

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("wlanconfig: qstatus ioctl");
        return -EFAULT;
    }

    for (acVal = WMM_AC_BK; acVal <= WMM_AC_VO; acVal++) {
        switch (acVal) {
        case WMM_AC_BK:
            printf("BK: ");
            break;
        case WMM_AC_BE:
            printf("BE: ");
            break;
        case WMM_AC_VI:
            printf("VI: ");
            break;
        case WMM_AC_VO:
            printf("VO: ");
            break;
        default:
            printf("??: ");
        }

        printf("ACM[%c], FlowReq[%c], FlowCreated[%c], Enabled[%c]\n",
               (qstatus.acStatus[acVal].wmmAcm ? 'X' : ' '),
               (qstatus.acStatus[acVal].flowRequired ? 'X' : ' '),
               (qstatus.acStatus[acVal].flowCreated ? 'X' : ' '),
               (qstatus.acStatus[acVal].disabled ? ' ' : 'X'));
    }

    return WLAN_STATUS_SUCCESS;
}

/** 
 *  @brief read current command
 *  @param ptr		A pointer to data
 *  @param curCmd       A pointer to the buf which will hold current command    
 *  @return      	NULL or the pointer to the left command buf
 */
static s8 *
readCurCmd(s8 * ptr, s8 * curCmd)
{
    s32 i = 0;
/** Maximum command size */
#define MAX_CMD_SIZE 64

    while (*ptr != ']' && i < (MAX_CMD_SIZE - 1))
        curCmd[i++] = *(++ptr);

    if (*ptr != ']')
        return NULL;

    curCmd[i - 1] = '\0';

    return ++ptr;
}

/** 
 *  @brief parse command and hex data
 *  @param fp 		A pointer to FILE stream
 *  @param dst       	A pointer to the dest buf
 *  @param cmd		A pointer to command buf for search
 *  @return            	length of hex data or WLAN_STATUS_FAILURE  		
 */
static int
fparse_for_cmd_and_hex(FILE * fp, u8 * dst, u8 * cmd)
{
    s8 *ptr;
    u8 *dptr;
    s8 buf[256], curCmd[64];
    s32 isCurCmd = 0;

    dptr = dst;
    while (fgets(buf, sizeof(buf), fp)) {
        ptr = buf;

        while (*ptr) {
            /* skip leading spaces */
            while (*ptr && isspace(*ptr))
                ptr++;

            /* skip blank lines and lines beginning with '#' */
            if (*ptr == '\0' || *ptr == '#')
                break;

            if (*ptr == '[' && *(ptr + 1) != '/') {
                if (!(ptr = readCurCmd(ptr, curCmd)))
                    return WLAN_STATUS_FAILURE;

                if (strcasecmp(curCmd, (char *) cmd))   /* Not equal */
                    isCurCmd = 0;
                else
                    isCurCmd = 1;
            }

            /* Ignore the rest if it is not correct cmd */
            if (!isCurCmd)
                break;

            if (*ptr == '[' && *(ptr + 1) == '/')
                return (dptr - dst);

            if (isxdigit(*ptr)) {
                ptr = convert2hex(ptr, dptr++);
            } else {
                /* Invalid character on data line */
                ptr++;
            }

        }
    }

    return WLAN_STATUS_FAILURE;
}

/**
 *  @brief Send an ADDTS command to the associated AP
 *
 *  Process a given conf file for a specific TSPEC data block.  Send the
 *    TSPEC along with any other IEs to the driver/firmware for transmission
 *    in an ADDTS request to the associated AP.  
 *
 *  Return the execution status of the command as well as the ADDTS response
 *    from the AP if any.
 *  
 *  wlanconfig ethX addts <filename.conf> <section# of tspec> <timeout in ms>
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *
 *  @return         WLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
process_addts(int argc, char *argv[])
{
    int ioctl_val, subioctl_val;
    struct iwreq iwr;
    unsigned int ieBytes;
    wlan_ioctl_wmm_addts_req_t addtsReq;

    FILE *fp;
    char filename[48];
    char config_id[20];

    memset(&addtsReq, 0x00, sizeof(addtsReq));
    memset(filename, 0x00, sizeof(filename));

    if (get_priv_ioctl("addts",
                       &ioctl_val, &subioctl_val) == WLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    if (argc != 6) {
        fprintf(stderr, "Invalid number of parameters!\n");
        return -EINVAL;
    }

    ieBytes = 0;

    strncpy(filename, argv[3], MIN(sizeof(filename) - 1, strlen(argv[3])));
    if ((fp = fopen(filename, "r")) == NULL) {
        perror("fopen");
        fprintf(stderr, "Cannot open file %s\n", argv[3]);
        exit(1);
    }

    sprintf(config_id, "tspec%d", atoi(argv[4]));

    ieBytes = fparse_for_cmd_and_hex(fp, addtsReq.tspecData, (u8 *) config_id);

    if (ieBytes > 0) {
        printf("Found %d bytes in the %s section of conf file %s\n",
               ieBytes, config_id, filename);
    } else {
        fprintf(stderr, "section %s not found in %s\n", config_id, filename);
        exit(1);
    }

    addtsReq.timeout_ms = atoi(argv[5]);

    printf("Cmd Input:\n");
    hexdump(config_id, addtsReq.tspecData, ieBytes, ' ');

    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.flags = subioctl_val;
    iwr.u.data.pointer = (caddr_t) & addtsReq;
    iwr.u.data.length = (sizeof(addtsReq.timeout_ms)
                         + sizeof(addtsReq.commandResult)
                         + sizeof(addtsReq.ieeeStatusCode)
                         + ieBytes);

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("wlanconfig: addts ioctl");
        return -EFAULT;
    }

    ieBytes = iwr.u.data.length - (sizeof(addtsReq.timeout_ms)
                                   + sizeof(addtsReq.commandResult)
                                   + sizeof(addtsReq.ieeeStatusCode));
    printf("Cmd Output:\n");
    printf("ADDTS Command Result = %d\n", addtsReq.commandResult);
    printf("ADDTS IEEE Status    = %d\n", addtsReq.ieeeStatusCode);
    hexdump(config_id, addtsReq.tspecData, ieBytes, ' ');

    return WLAN_STATUS_SUCCESS;
}

/**
 *  @brief Send a DELTS command to the associated AP
 *
 *  Process a given conf file for a specific TSPEC data block.  Send the
 *    TSPEC along with any other IEs to the driver/firmware for transmission
 *    in a DELTS request to the associated AP.  
 *
 *  Return the execution status of the command.  There is no response to a
 *    DELTS from the AP.
 *  
 *  wlanconfig ethX delts <filename.conf> <section# of tspec>
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *
 *  @return         WLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
process_delts(int argc, char *argv[])
{
    int ioctl_val, subioctl_val;
    struct iwreq iwr;
    unsigned int ieBytes;
    wlan_ioctl_wmm_delts_req_t deltsReq;

    FILE *fp;
    char filename[48];
    char config_id[20];

    memset(&deltsReq, 0x00, sizeof(deltsReq));
    memset(filename, 0x00, sizeof(filename));

    if (get_priv_ioctl("delts",
                       &ioctl_val, &subioctl_val) == WLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    if (argc != 5) {
        fprintf(stderr, "Invalid number of parameters!\n");
        return -EINVAL;
    }

    ieBytes = 0;

    strncpy(filename, argv[3], MIN(sizeof(filename) - 1, strlen(argv[3])));
    if ((fp = fopen(filename, "r")) == NULL) {
        perror("fopen");
        fprintf(stderr, "Cannot open file %s\n", argv[3]);
        exit(1);
    }

    sprintf(config_id, "tspec%d", atoi(argv[4]));

    ieBytes = fparse_for_cmd_and_hex(fp, deltsReq.tspecData, (u8 *) config_id);

    if (ieBytes > 0) {
        printf("Found %d bytes in the %s section of conf file %s\n",
               ieBytes, config_id, filename);
    } else {
        fprintf(stderr, "section %s not found in %s\n", config_id, filename);
        exit(1);
    }

    deltsReq.ieeeReasonCode = 0x20;     /* 32, unspecified QOS reason */

    printf("Cmd Input:\n");
    hexdump(config_id, deltsReq.tspecData, ieBytes, ' ');

    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.flags = subioctl_val;
    iwr.u.data.pointer = (caddr_t) & deltsReq;
    iwr.u.data.length = (sizeof(deltsReq.commandResult)
                         + sizeof(deltsReq.ieeeReasonCode)
                         + ieBytes);

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("wlanconfig: delts ioctl");
        return -EFAULT;
    }

    printf("Cmd Output:\n");
    printf("DELTS Command Result = %d\n", deltsReq.commandResult);

    return WLAN_STATUS_SUCCESS;
}

/**
 *  @brief Send a WMM AC Queue configuration command to get/set/default params
 *
 *  Configure or get the parameters of a WMM AC queue. The command takes
 *    an optional Queue Id as a last parameter.  Without the queue id, all
 *    queues will be acted upon.
 *  
 *  wlanconfig ethX qconfig set msdu <lifetime in TUs> [Queue Id: 0-3]
 *  wlanconfig ethX qconfig get [Queue Id: 0-3]
 *  wlanconfig ethX qconfig def [Queue Id: 0-3]
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *
 *  @return         WLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
process_qconfig(int argc, char *argv[])
{
    int ioctl_val, subioctl_val;
    struct iwreq iwr;
    wlan_ioctl_wmm_queue_config_t queue_config_cmd;
    wlan_wmm_ac_e ac_idx;
    wlan_wmm_ac_e ac_idx_start;
    wlan_wmm_ac_e ac_idx_stop;

    const char *ac_str_tbl[] = { "BK", "BE", "VI", "VO" };

    if (argc < 4) {
        fprintf(stderr, "Invalid number of parameters!\n");
        return -EINVAL;
    }

    if (get_priv_ioctl("qconfig",
                       &ioctl_val, &subioctl_val) == WLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    memset(&queue_config_cmd, 0x00, sizeof(queue_config_cmd));
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.pointer = (caddr_t) & queue_config_cmd;
    iwr.u.data.length = sizeof(queue_config_cmd);
    iwr.u.data.flags = subioctl_val;

    if (strcmp(argv[3], "get") == 0) {
        /* 3 4 5 */
        /* qconfig get [qid] */
        if (argc == 4) {
            ac_idx_start = WMM_AC_BK;
            ac_idx_stop = WMM_AC_VO;
        } else if (argc == 5) {
            ac_idx_start = atoi(argv[4]);
            ac_idx_stop = ac_idx_start;
        } else {
            fprintf(stderr, "Invalid number of parameters!\n");
            return -EINVAL;
        }
        queue_config_cmd.action = WMM_QUEUE_CONFIG_ACTION_GET;

        for (ac_idx = ac_idx_start; ac_idx <= ac_idx_stop; ac_idx++) {
            queue_config_cmd.accessCategory = ac_idx;
            if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
                perror("qconfig ioctl");
            } else {
                printf("qconfig %s(%d): MSDU Lifetime GET = 0x%04x (%d)\n",
                       ac_str_tbl[ac_idx],
                       ac_idx,
                       queue_config_cmd.msduLifetimeExpiry,
                       queue_config_cmd.msduLifetimeExpiry);
            }
        }
    } else if (strcmp(argv[3], "set") == 0) {
        if (strcmp(argv[4], "msdu") == 0) {
            /* 3 4 5 6 7 */
            /* qconfig set msdu <value> [qid] */
            if (argc == 6) {
                ac_idx_start = WMM_AC_BK;
                ac_idx_stop = WMM_AC_VO;
            } else if (argc == 7) {
                ac_idx_start = atoi(argv[6]);
                ac_idx_stop = ac_idx_start;
            } else {
                fprintf(stderr, "Invalid number of parameters!\n");
                return -EINVAL;
            }
            queue_config_cmd.action = WMM_QUEUE_CONFIG_ACTION_SET;
            queue_config_cmd.msduLifetimeExpiry = atoi(argv[5]);

            for (ac_idx = ac_idx_start; ac_idx <= ac_idx_stop; ac_idx++) {
                queue_config_cmd.accessCategory = ac_idx;
                if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
                    perror("qconfig ioctl");
                } else {
                    printf("qconfig %s(%d): MSDU Lifetime SET = 0x%04x (%d)\n",
                           ac_str_tbl[ac_idx],
                           ac_idx,
                           queue_config_cmd.msduLifetimeExpiry,
                           queue_config_cmd.msduLifetimeExpiry);
                }
            }
        } else {
            /* Only MSDU Lifetime provisioning accepted for now */
            fprintf(stderr, "Invalid set parameter: s/b [msdu]\n");
            return -EINVAL;
        }
    } else if (strncmp(argv[3], "def", strlen("def")) == 0) {
        /* 3 4 5 */
        /* qconfig def [qid] */
        if (argc == 4) {
            ac_idx_start = WMM_AC_BK;
            ac_idx_stop = WMM_AC_VO;
        } else if (argc == 5) {
            ac_idx_start = atoi(argv[4]);
            ac_idx_stop = ac_idx_start;
        } else {
            fprintf(stderr, "Invalid number of parameters!\n");
            return -EINVAL;
        }
        queue_config_cmd.action = WMM_QUEUE_CONFIG_ACTION_DEFAULT;

        for (ac_idx = ac_idx_start; ac_idx <= ac_idx_stop; ac_idx++) {
            queue_config_cmd.accessCategory = ac_idx;
            if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
                perror("qconfig ioctl");
            } else {
                printf("qconfig %s(%d): MSDU Lifetime DEFAULT = 0x%04x (%d)\n",
                       ac_str_tbl[ac_idx],
                       ac_idx,
                       queue_config_cmd.msduLifetimeExpiry,
                       queue_config_cmd.msduLifetimeExpiry);
            }
        }
    } else {
        fprintf(stderr, "Invalid qconfig command; s/b [set, get, default]\n");
        return -EINVAL;
    }

    return WLAN_STATUS_SUCCESS;
}

/**
 *  @brief Turn on/off or retrieve and clear the queue statistics for an AC
 *
 *  Turn the queue statistics collection on/off for a given AC or retrieve the
 *    current accumulated stats and clear them from the firmware.  The command
 *    takes an optional Queue Id as a last parameter.  Without the queue id,
 *    all queues will be acted upon.
 *
 *  wlanconfig ethX qstats on [Queue Id: 0-3]
 *  wlanconfig ethX qstats off [Queue Id: 0-3]
 *  wlanconfig ethX qstats get [Queue Id: 0-3]
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *
 *  @return         WLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
process_qstats(int argc, char *argv[])
{
    int ioctl_val, subioctl_val;
    struct iwreq iwr;
    wlan_ioctl_wmm_queue_stats_t queue_stats_cmd;
    wlan_wmm_ac_e ac_idx;
    wlan_wmm_ac_e ac_idx_start;
    wlan_wmm_ac_e ac_idx_stop;
    u16 usedTime[MAX_AC_QUEUES];
    u16 policedTime[MAX_AC_QUEUES];

    const char *ac_str_tbl[] = { "BK", "BE", "VI", "VO" };

    if (argc < 3) {
        fprintf(stderr, "Invalid number of parameters!\n");
        return -EINVAL;
    }

    if (get_priv_ioctl("qstats",
                       &ioctl_val, &subioctl_val) == WLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    printf("\n");

    memset(usedTime, 0x00, sizeof(usedTime));
    memset(policedTime, 0x00, sizeof(policedTime));
    memset(&queue_stats_cmd, 0x00, sizeof(queue_stats_cmd));

    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.pointer = (caddr_t) & queue_stats_cmd;
    iwr.u.data.length = sizeof(queue_stats_cmd);
    iwr.u.data.flags = subioctl_val;

    if ((argc > 3) && strcmp(argv[3], "on") == 0) {
        if (argc == 4) {
            ac_idx_start = WMM_AC_BK;
            ac_idx_stop = WMM_AC_VO;
        } else if (argc == 5) {
            ac_idx_start = atoi(argv[4]);
            ac_idx_stop = ac_idx_start;
        } else {
            fprintf(stderr, "Invalid number of parameters!\n");
            return -EINVAL;
        }
        queue_stats_cmd.action = WMM_STATS_ACTION_START;
        for (ac_idx = ac_idx_start; ac_idx <= ac_idx_stop; ac_idx++) {
            queue_stats_cmd.accessCategory = ac_idx;
            if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
                perror("qstats ioctl");
            } else {
                printf("qstats %s(%d) turned on\n", ac_str_tbl[ac_idx], ac_idx);
            }
        }
    } else if ((argc > 3) && strcmp(argv[3], "off") == 0) {
        if (argc == 4) {
            ac_idx_start = WMM_AC_BK;
            ac_idx_stop = WMM_AC_VO;
        } else if (argc == 5) {
            ac_idx_start = atoi(argv[4]);
            ac_idx_stop = ac_idx_start;
        } else {
            fprintf(stderr, "Invalid number of parameters!\n");
            return -EINVAL;
        }
        queue_stats_cmd.action = WMM_STATS_ACTION_STOP;
        for (ac_idx = ac_idx_start; ac_idx <= ac_idx_stop; ac_idx++) {
            queue_stats_cmd.accessCategory = ac_idx;
            if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
                perror("qstats ioctl");
            } else {
                printf("qstats %s(%d) turned off\n",
                       ac_str_tbl[ac_idx], ac_idx);
            }
        }
    } else if (((argc > 3) || (argc == 3))
               && ((argc == 3) ? 1 : (strcmp(argv[3], "get") == 0))) {
        /* If the user types: "wlanconfig eth1 qstats" without get argument.
           The wlanconfig application invokes "get" option for all queues */
        printf("AC  Count   Loss  TxDly   QDly"
               "    <=5   <=10   <=20   <=30   <=40   <=50    >50\n");
        printf("----------------------------------"
               "---------------------------------------------\n");
        if ((argc == 4) || (argc == 3)) {
            ac_idx_start = WMM_AC_BK;
            ac_idx_stop = WMM_AC_VO;
        } else if (argc == 5) {
            ac_idx_start = atoi(argv[4]);
            ac_idx_stop = ac_idx_start;
        } else {
            fprintf(stderr, "Invalid number of parameters!\n");
            return -EINVAL;
        }
        queue_stats_cmd.action = WMM_STATS_ACTION_GET_CLR;

        for (ac_idx = ac_idx_start; ac_idx <= ac_idx_stop; ac_idx++) {
            queue_stats_cmd.accessCategory = ac_idx;
            if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
                perror("qstats ioctl");
            } else {
                printf("%s  %5u  %5u %6u %6u"
                       "  %5u  %5u  %5u  %5u  %5u  %5u  %5u\n",
                       ac_str_tbl[ac_idx],
                       queue_stats_cmd.pktCount,
                       queue_stats_cmd.pktLoss,
                       (unsigned int) queue_stats_cmd.avgTxDelay,
                       (unsigned int) queue_stats_cmd.avgQueueDelay,
                       queue_stats_cmd.delayHistogram[0],
                       queue_stats_cmd.delayHistogram[1],
                       queue_stats_cmd.delayHistogram[2],
                       queue_stats_cmd.delayHistogram[3],
                       queue_stats_cmd.delayHistogram[4],
                       queue_stats_cmd.delayHistogram[5],
                       queue_stats_cmd.delayHistogram[6]);

                usedTime[ac_idx] = queue_stats_cmd.usedTime;
                policedTime[ac_idx] = queue_stats_cmd.policedTime;
            }
        }

        printf("----------------------------------"
               "---------------------------------------------\n");
        printf("\nAC      UsedTime      PolicedTime\n");
        printf("---------------------------------\n");

        for (ac_idx = ac_idx_start; ac_idx <= ac_idx_stop; ac_idx++) {
            printf("%s        %6u           %6u\n",
                   ac_str_tbl[ac_idx],
                   (unsigned int) usedTime[ac_idx],
                   (unsigned int) policedTime[ac_idx]);
        }
    } else {

        fprintf(stderr, "Invalid qstats command;\n");
        return -EINVAL;
    }
    printf("\n");

    return WLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Get the current status of the WMM Traffic Streams
 *  
 *  Command: wlanconfig eth1 ts_status
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *
 *  @return         WLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
process_wmm_ts_status(int argc, char *argv[])
{
    int ioctl_val, subioctl_val;
    struct iwreq iwr;
    wlan_ioctl_wmm_ts_status_t ts_status;
    int tid;

    if (get_priv_ioctl("ts_status",
                       &ioctl_val, &subioctl_val) == WLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    printf("\nTID   Valid    AC   UP   PSB   FlowDir  MediumTime\n");
    printf("---------------------------------------------------\n");

    for (tid = 0; tid <= 7; tid++) {
        memset(&ts_status, 0x00, sizeof(ts_status));
        ts_status.tid = tid;

        strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
        iwr.u.data.flags = subioctl_val;
        iwr.u.data.pointer = (caddr_t) & ts_status;
        iwr.u.data.length = (sizeof(ts_status));

        if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
            perror("wlanconfig: ts_status ioctl");
            return -EFAULT;
        }

        printf(" %02d      %-3s    %u    %u     %c    ",
               ts_status.tid,
               (ts_status.valid ? "Yes" : "No"),
               ts_status.accessCategory,
               ts_status.userPriority, (ts_status.psb ? 'U' : 'L'));

        if ((ts_status.flowDir & 0x03) == 0) {
            printf("%s", " ---- ");
        } else {
            printf("%2s%4s",
                   (ts_status.flowDir & 0x01 ? "Up" : ""),
                   (ts_status.flowDir & 0x02 ? "Down" : ""));
        }

        printf("%12u\n", ts_status.mediumTime);
    }

    printf("\n");

    return WLAN_STATUS_SUCCESS;
}

/**
 *  @brief Retrieve and clear the TX Packet statistics
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *
 *  @return         WLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
process_txpktstats(int argc, char *argv[])
{
    int ioctl_val, subioctl_val;
    struct iwreq iwr;
    HostCmd_DS_TX_PKT_STATS tx_pkt_stats;
    u32 totInit, totSuccess, totAttempts, totFailRetry, totFailExpiry;
    int idx;

    totInit = totSuccess = totAttempts = totFailRetry = totFailExpiry = 0;

    const char *rateIdStr[] = { "1", "2", "5.5", "11", "--",
        "6", "9", "12", "18", "24", "36", "48", "54", "--"
    };

    if (argc != 3) {
        fprintf(stderr, "Invalid number of parameters!\n");
        return -EINVAL;
    }

    if (get_priv_ioctl("txpktstats",
                       &ioctl_val, &subioctl_val) == WLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    printf("\n");

    memset(&tx_pkt_stats, 0x00, sizeof(tx_pkt_stats));
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.pointer = (caddr_t) & tx_pkt_stats;
    iwr.u.data.length = sizeof(tx_pkt_stats);
    iwr.u.data.flags = subioctl_val;

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("txpktstats ioctl");
        return WLAN_STATUS_SUCCESS;
    }

    printf("  Rate    InitialTx    SuccessTx     Attempts"
           "    FailRetry   FailExpiry\n");
    printf("--------------------------------------------"
           "---------------------------\n");

    for (idx = 0; idx < NELEMENTS(tx_pkt_stats.StatEntry); idx++) {
        printf(" %5s   %10u   %10u   %10u   %10u   %10u\n",
               rateIdStr[idx],
               (unsigned int) tx_pkt_stats.StatEntry[idx].PktInitCnt,
               (unsigned int) tx_pkt_stats.StatEntry[idx].PktSuccessCnt,
               (unsigned int) tx_pkt_stats.StatEntry[idx].TxAttempts,
               (unsigned int) tx_pkt_stats.StatEntry[idx].RetryFailure,
               (unsigned int) tx_pkt_stats.StatEntry[idx].ExpiryFailure);

        totInit += tx_pkt_stats.StatEntry[idx].PktInitCnt;
        totSuccess += tx_pkt_stats.StatEntry[idx].PktSuccessCnt;
        totAttempts += tx_pkt_stats.StatEntry[idx].TxAttempts;
        totFailRetry += tx_pkt_stats.StatEntry[idx].RetryFailure;
        totFailExpiry += tx_pkt_stats.StatEntry[idx].ExpiryFailure;
    }

    printf("--------------------------------------------"
           "---------------------------\n");
    printf("         %10u   %10u   %10u   %10u   %10u\n",
           (unsigned int) totInit,
           (unsigned int) totSuccess,
           (unsigned int) totAttempts,
           (unsigned int) totFailRetry, (unsigned int) totFailExpiry);

    printf("\n");

    return WLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Get one line from the File
 *  
 *  @param s	        Storage location for data.
 *  @param size 	Maximum number of characters to read. 
 *  @param stream 	File stream	  	
 *  @param line		A pointer to return current line number
 *  @return             returns string or NULL 
 */
static s8 *
wlan_config_get_line(s8 * s, s32 size, FILE * stream, int *line)
{
    s8 *pos, *end, *sstart;

    while (fgets(s, size, stream)) {
        (*line)++;
        s[size - 1] = '\0';
        pos = s;

        while (*pos == ' ' || *pos == '\t')
            pos++;
        if (*pos == '#' || (*pos == '\r' && *(pos + 1) == '\n') ||
            *pos == '\n' || *pos == '\0')
            continue;

        /* Remove # comments unless they are within a double quoted * string.
           Remove trailing white space. */
        sstart = strchr(pos, '"');
        if (sstart)
            sstart = strchr(sstart + 1, '"');
        if (!sstart)
            sstart = pos;
        end = strchr(sstart, '#');
        if (end)
            *end-- = '\0';
        else
            end = pos + strlen(pos) - 1;
        while (end > pos && (*end == '\r' || *end == '\n' ||
                             *end == ' ' || *end == '\t')) {
            *end-- = '\0';
        }
        if (*pos == '\0')
            continue;
        return pos;
    }

    return NULL;
}

/** 
 *  @brief convert hex char to integer
 *  
 *  @param c		char
 *  @return      	integer or WLAN_STATUS_FAILURE
 */
static int
hex2num(s8 c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return WLAN_STATUS_FAILURE;
}

/** 
 *  @brief convert hex char to integer
 *  
 *  @param c		char
 *  @return      	integer or WLAN_STATUS_FAILURE
 */
static int
hex2byte(const s8 * hex)
{
    s32 a, b;
    a = hex2num(*hex++);
    if (a < 0)
        return WLAN_STATUS_FAILURE;
    b = hex2num(*hex++);
    if (b < 0)
        return WLAN_STATUS_FAILURE;
    return (a << 4) | b;
}

/** 
 *  @brief convert hex char to integer
 *  
 *  @param hex		A pointer to hex string
 *  @param buf		buffer to storage the data
 *  @param len		
 *  @return      	WLAN_STATUS_SUCCESS or WLAN_STATUS_FAILURE
 */
static int
hexstr2bin(const s8 * hex, u8 * buf, size_t len)
{
    s32 i, a;
    const s8 *ipos = hex;
    s8 *opos = (s8 *) buf;

    for (i = 0; i < len; i++) {
        a = hex2byte(ipos);
        if (a < 0)
            return WLAN_STATUS_FAILURE;
        *opos++ = a;
        ipos += 2;
    }
    return WLAN_STATUS_SUCCESS;
}

/** 
 *  @brief parse string
 *  
 *  @param value	A pointer to string
 *  @param buf		buffer to storage the data
 *  @param str		buffer to hold return string
 *  @param len 		use to return the length of parsed string
 *  @param maxlen	use to return the max length of ssid
 *  @return      	WLAN_STATUS_SUCCESS or WLAN_STATUS_FAILURE
 */
static int
wlan_config_parse_string(const s8 * value, s8 * str, size_t * len,
                         size_t * maxlen)
{
    s8 *p;
    p = strchr(value, ',');
    if (p) {
        *maxlen = (u16) a2hex_or_atoi(p + 1);
        *p = '\0';
    }

    if (*value == '"') {
        s8 *pos;
        value++;
        pos = strchr(value, '"');

        if (pos == NULL || pos[1] != '\0') {
            value--;
            return WLAN_STATUS_FAILURE;
        }
        *pos = '\0';
        *len = strlen(value);
        strcpy(str, value);
        return WLAN_STATUS_SUCCESS;
    } else {
        s32 hlen = strlen(value);

        if (hlen % 1)
            return WLAN_STATUS_FAILURE;
        *len = hlen / 2;
        if (str == NULL)
            return WLAN_STATUS_FAILURE;
        if (hexstr2bin(value, (u8 *) str, *len)) {
            return WLAN_STATUS_FAILURE;
        }

        return WLAN_STATUS_SUCCESS;
    }
}

/** 
 *  @brief parse bgscan action
 *  
 *  @param CmdBuf	A pointer to command buffer
 *  @param line	        line number
 *  @param value	A pointer to Action buffer
 *  @return      	WLAN_STATUS_SUCCESS
 */
static int
bgscan_parse_action(u8 * CmdBuf, s32 line, s8 * value)
{
    HostCmd_DS_802_11_BG_SCAN_CONFIG *bgscan_config =
        (HostCmd_DS_802_11_BG_SCAN_CONFIG *) CmdBuf;

    bgscan_config->Action = (u8) a2hex_or_atoi(value);
    return WLAN_STATUS_SUCCESS;
}

/** 
 *  @brief parse bgscan enable parameter
 *  
 *  @param CmdBuf	A pointer to command buffer
 *  @param line	        line number
 *  @param value	A pointer to Enable buffer
 *  @return      	WLAN_STATUS_SUCCESS
 */
static int
bgscan_parse_enable(u8 * CmdBuf, s32 line, s8 * value)
{
    HostCmd_DS_802_11_BG_SCAN_CONFIG *bgscan_config =
        (HostCmd_DS_802_11_BG_SCAN_CONFIG *) CmdBuf;

    bgscan_config->Enable = (u8) a2hex_or_atoi(value);

    return WLAN_STATUS_SUCCESS;
}

/** 
 *  @brief parse bgscan BssType
 *  
 *  @param CmdBuf	A pointer to command buffer
 *  @param line	        line number
 *  @param value	A pointer to BssType buffer
 *  @return      	WLAN_STATUS_SUCCESS
 */
static int
bgscan_parse_bsstype(u8 * CmdBuf, s32 line, s8 * value)
{
    HostCmd_DS_802_11_BG_SCAN_CONFIG *bgscan_config =
        (HostCmd_DS_802_11_BG_SCAN_CONFIG *) CmdBuf;

    bgscan_config->BssType = (u8) a2hex_or_atoi(value);

    return WLAN_STATUS_SUCCESS;
}

/** 
 *  @brief parse bgscan channels per scan parameter
 *  
 *  @param CmdBuf	A pointer to command buffer
 *  @param line	        line number
 *  @param value	A pointer to channels per scan buffer
 *  @return      	WLAN_STATUS_SUCCESS
 */
static int
bgscan_parse_channelsperscan(u8 * CmdBuf, s32 line, s8 * value)
{
    HostCmd_DS_802_11_BG_SCAN_CONFIG *bgscan_config =
        (HostCmd_DS_802_11_BG_SCAN_CONFIG *) CmdBuf;

    bgscan_config->ChannelsPerScan = (u8) a2hex_or_atoi(value);

    return WLAN_STATUS_SUCCESS;
}

/** 
 *  @brief parse bgscan scan interval parameter
 *  
 *  @param CmdBuf	A pointer to command buffer
 *  @param line	        line number
 *  @param value	A pointer to scan interval buffer
 *  @return      	WLAN_STATUS_SUCCESS
 */
static int
bgscan_parse_scaninterval(u8 * CmdBuf, s32 line, s8 * value)
{
    HostCmd_DS_802_11_BG_SCAN_CONFIG *bgscan_config =
        (HostCmd_DS_802_11_BG_SCAN_CONFIG *) CmdBuf;

    bgscan_config->ScanInterval = (u32) a2hex_or_atoi(value);

    return WLAN_STATUS_SUCCESS;
}

/** 
 *  @brief parse bgscan store condition parameter
 *  
 *  @param CmdBuf	A pointer to command buffer
 *  @param line	        line number
 *  @param value	A pointer to store condition buffer
 *  @return      	WLAN_STATUS_SUCCESS
 */
static int
bgscan_parse_storecondition(u8 * CmdBuf, s32 line, s8 * value)
{
    printf("Warning:"
           " StoreCondition is deprecated in this version of BGScan\n");

    return WLAN_STATUS_SUCCESS;
}

/** 
 *  @brief parse bgscan report conditions parameter
 *  
 *  @param CmdBuf	A pointer to command buffer
 *  @param line	        line number
 *  @param value	A pointer to report conditions buffer
 *  @return      	WLAN_STATUS_SUCCESS
 */
static int
bgscan_parse_reportconditions(u8 * CmdBuf, s32 line, s8 * value)
{
    HostCmd_DS_802_11_BG_SCAN_CONFIG *bgscan_config =
        (HostCmd_DS_802_11_BG_SCAN_CONFIG *) CmdBuf;

    bgscan_config->ReportConditions = a2hex_or_atoi(value);

    return WLAN_STATUS_SUCCESS;
}

/** 
 *  @brief parse bgscan ssid parameter
 *  
 *  @param CmdBuf   A pointer to command buffer
 *  @param line     line number
 *  @param value    A pointer to ssid buffer
 *  @return         WLAN_STATUS_SUCCESS or WLAN_STATUS_FAILURE
 */
static int
bgscan_parse_ssid(u8 * CmdBuf, s32 line, s8 * value)
{
    static int ssidCnt;

    s8 *buf = NULL;
    size_t len = 0;
    size_t maxlen = 0;

    MrvlIEtypes_WildCardSsIdParamSet_t *WildcardSsIdParamSet = NULL;
    WildcardSsIdParamSet = (MrvlIEtypes_WildCardSsIdParamSet_t *) (CmdBuf
                                                                   + ActualPos);
    if (value != NULL) {
        buf = (s8 *) malloc(strlen(value));
        if (buf == NULL)
            return WLAN_STATUS_FAILURE;
    } else {
        printf("Invalid SSID\n");
        return WLAN_STATUS_FAILURE;
    }
    memset(buf, 0, strlen(value));

    if (ssidCnt >= MRVDRV_MAX_SSID_LIST_LENGTH) {
        printf("Too many SSIDs\n");
        free(buf);
        return WLAN_STATUS_FAILURE;
    }

    if (wlan_config_parse_string(value, buf, &len, &maxlen)) {
        printf("Invalid SSID\n");
        free(buf);
        return WLAN_STATUS_FAILURE;
    }
    ssidCnt++;
    if (!strlen(buf)) {
        printf("The %dth SSID is NULL.\n", ssidCnt);
        free(buf);
        return WLAN_STATUS_FAILURE;
    }
    /* It will be treated as specific scan in firmware only if MaxSsidLength=0 */
    WildcardSsIdParamSet->Header.Type = wlan_cpu_to_le16(TLV_TYPE_WILDCARDSSID);
    WildcardSsIdParamSet->Header.Len =
        strlen(buf) + sizeof(WildcardSsIdParamSet->MaxSsidLength);
    WildcardSsIdParamSet->MaxSsidLength = maxlen;
    TLVSsidSize +=
        WildcardSsIdParamSet->Header.Len + sizeof(MrvlIEtypesHeader_t);
    ActualPos += WildcardSsIdParamSet->Header.Len + sizeof(MrvlIEtypesHeader_t);
    WildcardSsIdParamSet->Header.Len =
        wlan_cpu_to_le16(WildcardSsIdParamSet->Header.Len);
    memcpy(WildcardSsIdParamSet->SsId, buf, strlen(buf));

    free(buf);
    return WLAN_STATUS_SUCCESS;
}

/** 
 *  @brief parse bgscan probes parameter
 *  
 *  @param CmdBuf	A pointer to command buffer
 *  @param line	        line number
 *  @param value	A pointer to probes buffer
 *  @return      	WLAN_STATUS_SUCCESS 
 */
static int
bgscan_parse_probes(u8 * CmdBuf, s32 line, s8 * value)
{
    MrvlIEtypes_NumProbes_t *Probes = NULL;
/** probes payload size */
#define PROBES_PAYLOAD_SIZE 2

    Probes = (MrvlIEtypes_NumProbes_t *) (CmdBuf + ActualPos);

    Probes->Header.Type = wlan_cpu_to_le16(TLV_TYPE_NUMPROBES);
    Probes->Header.Len = PROBES_PAYLOAD_SIZE;

    Probes->NumProbes = (u16) a2hex_or_atoi(value);

    if (Probes->NumProbes) {
        TLVProbeSize += sizeof(MrvlIEtypesHeader_t) + Probes->Header.Len;
    } else {
        TLVProbeSize = 0;
    }

    Probes->NumProbes = wlan_cpu_to_le16(Probes->NumProbes);
    Probes->Header.Len = wlan_cpu_to_le16(Probes->Header.Len);

    ActualPos += TLVProbeSize;
    return WLAN_STATUS_SUCCESS;
}

/** 
 *  @brief parse bgscan channel list parameter
 *  
 *  @param CmdBuf   A pointer to command buffer
 *  @param line     line number
 *  @param value    A pointer to channel list buffer
 *  @return         WLAN_STATUS_SUCCESS or WLAN_STATUS_FAILURE
 */
static int
bgscan_parse_channellist(u8 * CmdBuf, s32 line, s8 * value)
{
    MrvlIEtypes_ChanListParamSet_t *chan;
    char *buf, *grp0, *grp1;
    s32 len, idx;

    chan = (MrvlIEtypes_ChanListParamSet_t *) (CmdBuf + ActualPos);

    len = strlen(value) + 1;
    buf = malloc(len);

    if (buf == NULL) {
        return WLAN_STATUS_FAILURE;
    }

    memset(buf, 0, len);
    strcpy(buf, value);

    chan->Header.Type = wlan_cpu_to_le16(TLV_TYPE_CHANLIST);
    grp1 = buf;
    idx = 0;
    while ((grp1 != NULL) && (*grp1 != 0)) {

        grp0 = strsep(&grp1, "\";");

        if ((grp0 != NULL) && (*grp0 != 0)) {
            char *ps8Resultstr = NULL;

            ps8Resultstr = strtok(grp0, ",");
            if (ps8Resultstr == NULL) {
                goto failure;
            }
            chan->ChanScanParam[idx].RadioType = atoi(ps8Resultstr);
            ps8Resultstr = strtok(NULL, ",");
            if (ps8Resultstr == NULL) {
                goto failure;
            }
            chan->ChanScanParam[idx].ChanNumber = atoi(ps8Resultstr);

            ps8Resultstr = strtok(NULL, ",");
            if (ps8Resultstr == NULL) {
                goto failure;
            }
            chan->ChanScanParam[idx].ChanScanMode.PassiveScan
                = atoi(ps8Resultstr);

            chan->ChanScanParam[idx].ChanScanMode.DisableChanFilt = 1;

            ps8Resultstr = strtok(NULL, ",");
            if (ps8Resultstr == NULL) {
                goto failure;
            }
            chan->ChanScanParam[idx].MinScanTime = atoi(ps8Resultstr);
            chan->ChanScanParam[idx].MinScanTime =
                wlan_cpu_to_le16(chan->ChanScanParam[idx].MinScanTime);

            ps8Resultstr = strtok(NULL, ",");
            if (ps8Resultstr == NULL) {
                goto failure;
            }
            chan->ChanScanParam[idx].MaxScanTime = atoi(ps8Resultstr);
            chan->ChanScanParam[idx].MaxScanTime =
                wlan_cpu_to_le16(chan->ChanScanParam[idx].MaxScanTime);
            idx++;
        }
    }

    chan->Header.Len = (idx * sizeof(ChanScanParamSet_t));
    TLVChanSize += (chan->Header.Len + sizeof(MrvlIEtypesHeader_t));
    chan->Header.Len = wlan_cpu_to_le16(chan->Header.Len);
    ActualPos += TLVChanSize;

    free(buf);
    return WLAN_STATUS_SUCCESS;
  failure:
    free(buf);
    printf("Invalid string:Check the bg_scan config file\n");
    return WLAN_STATUS_FAILURE;
}

/** 
 *  @brief parse bgscan snr threshold parameter
 *  
 *  @param CmdBuf	A pointer to command buffer
 *  @param line	    line number
 *  @param value	A pointer to bgscan snr threshold buffer
 *  @return      	WLAN_STATUS_SUCCESS
 */
static int
bgscan_parse_snrthreshold(u8 * CmdBuf, s32 line, s8 * value)
{
    MrvlIEtypes_SnrThreshold_t *SnrThreshold = NULL;
    u32 tmp;

    SnrThreshold = (MrvlIEtypes_SnrThreshold_t *) (CmdBuf + ActualPos);

    SnrThreshold->Header.Type = wlan_cpu_to_le16(TLV_TYPE_SNR_LOW);
    SnrThreshold->Header.Len = (sizeof(MrvlIEtypes_SnrThreshold_t)
                                - sizeof(SnrThreshold->Header));
    SnrThreshold->Header.Len = wlan_cpu_to_le16(SnrThreshold->Header.Len);

    tmp = (u16) a2hex_or_atoi(value);
    SnrThreshold->SNRValue = tmp & 0xff;
    SnrThreshold->SNRFreq = (tmp >> 8) & 0xff;

    TLVSnrSize += sizeof(MrvlIEtypes_SnrThreshold_t);
    ActualPos += sizeof(MrvlIEtypes_SnrThreshold_t);
    return WLAN_STATUS_SUCCESS;
}

/** 
 *  @brief parse bgscan rssi threshold parameter
 *  
 *  @param CmdBuf	A pointer to command buffer
 *  @param line	    line number
 *  @param value	A pointer to bgscan snr threshold buffer
 *  @return      	WLAN_STATUS_SUCCESS
 */
static int
bgscan_parse_rssithreshold(u8 * CmdBuf, s32 line, s8 * value)
{
    MrvlIEtypes_RssiThreshold_t *RssiThreshold = NULL;
    u32 tmp;

    RssiThreshold = (MrvlIEtypes_RssiThreshold_t *) (CmdBuf + ActualPos);

    RssiThreshold->Header.Type = wlan_cpu_to_le16(TLV_TYPE_RSSI_LOW);
    RssiThreshold->Header.Len = (sizeof(MrvlIEtypes_RssiThreshold_t)
                                 - sizeof(RssiThreshold->Header));
    RssiThreshold->Header.Len = wlan_cpu_to_le16(RssiThreshold->Header.Len);

    tmp = (u16) a2hex_or_atoi(value);
    RssiThreshold->RSSIValue = tmp & 0xff;
    RssiThreshold->RSSIFreq = (tmp >> 8) & 0xff;

    TLVSnrSize += sizeof(MrvlIEtypes_RssiThreshold_t);
    ActualPos += sizeof(MrvlIEtypes_RssiThreshold_t);
    return WLAN_STATUS_SUCCESS;
}

/** 
 *  @brief parse bgscan start later parameter
 *  
 *  @param CmdBuf	A pointer to command buffer
 *  @param line	        line number
 *  @param value	A pointer to bgscan start later buffer
 *  @return      	WLAN_STATUS_SUCCESS
 */
static int
bgscan_parse_startlater(u8 * CmdBuf, s32 line, s8 * value)
{
    MrvlIEtypes_StartBGScanLater_t *StartBGScanLater = NULL;

    StartBGScanLater = (MrvlIEtypes_StartBGScanLater_t *) (CmdBuf + ActualPos);

    StartBGScanLater->Header.Type = wlan_cpu_to_le16(TLV_TYPE_STARTBGSCANLATER);
    StartBGScanLater->Header.Len = PROBES_PAYLOAD_SIZE;

    StartBGScanLater->StartLater = (u16) a2hex_or_atoi(value);
    StartBGScanLater->StartLater =
        wlan_cpu_to_le16(StartBGScanLater->StartLater);

    TLVStartBGScanLaterSize
        = sizeof(MrvlIEtypesHeader_t) + StartBGScanLater->Header.Len;
    ActualPos += TLVStartBGScanLaterSize;
    StartBGScanLater->Header.Len =
        wlan_cpu_to_le16(StartBGScanLater->Header.Len);
    return WLAN_STATUS_SUCCESS;
}

typedef struct
{
    /** Field name */
    s8 *name;
    /** Parser function pointer */
    int (*parser) (u8 * CmdBuf, s32 line, s8 * value);
} bgscan_field_func;

static bgscan_field_func bgscan_fields[] = {
    {"Action", bgscan_parse_action},
    {"Enable", bgscan_parse_enable},
    {"BssType", bgscan_parse_bsstype},
    {"ChannelsPerScan", bgscan_parse_channelsperscan},
    {"ScanInterval", bgscan_parse_scaninterval},
    {"ReportConditions", bgscan_parse_reportconditions},
    {"SSID", bgscan_parse_ssid},
    {"Probes", bgscan_parse_probes},
    {"ChannelList", bgscan_parse_channellist},
    {"SnrThreshold", bgscan_parse_snrthreshold},
    {"StoreCondition", bgscan_parse_storecondition},
    {"RssiThreshold", bgscan_parse_rssithreshold},
    {"StartLater", bgscan_parse_startlater},
};

/** 
 *  @brief get bgscan data
 *  
 *  @param fp			A pointer to file stream
 *  @param line	        	A pointer to line number
 *  @param bgscan_config	A pointer to HostCmd_DS_802_11_BG_SCAN_CONFIG structure
 *  @return      		WLAN_STATUS_SUCCESS
 */
static int
wlan_get_bgscan_data(FILE * fp, int *line,
                     HostCmd_DS_802_11_BG_SCAN_CONFIG * bgscan_config)
{
    s32 errors = 0, i, end = 0;
    s8 buf[256], *pos, *pos2;

    while ((pos = wlan_config_get_line(buf, sizeof(buf), fp, line))) {
        if (strcmp(pos, "}") == 0) {
            end = 1;
            break;
        }

        pos2 = strchr(pos, '=');
        if (pos2 == NULL) {
            printf("Line %d: Invalid bgscan line '%s'.", *line, pos);
            errors++;
            continue;
        }

        *pos2++ = '\0';
        if (*pos2 == '"') {
            if (strchr(pos2 + 1, '"') == NULL) {
                printf("Line %d: invalid quotation '%s'.", *line, pos2);
                errors++;
                continue;
            }
        }

        for (i = 0; i < NELEMENTS(bgscan_fields); i++) {
            if (strncmp(pos, bgscan_fields[i].name,
                        strlen(bgscan_fields[i].name)) == 0) {
                if (bgscan_fields[i].parser((u8 *) bgscan_config, *line, pos2)) {
                    printf("Line %d: failed to parse %s"
                           "'%s'.", *line, pos, pos2);
                    errors++;
                }
                break;
            }
        }
        if (i == NELEMENTS(bgscan_fields)) {
            printf("Line %d: unknown bgscan field '%s'.\n", *line, pos);
            errors++;
        }
    }
    return WLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Process bgscan config 
 *  @param argc		number of arguments
 *  @param argv         A pointer to arguments array    
 *  @return      	WLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
process_bg_scan_config(int argc, char *argv[])
{
    u8 scanCfg[256], *pos, *buf = NULL;
    s8 filename[48] = "";
    FILE *fp;
    HostCmd_DS_802_11_BG_SCAN_CONFIG *bgscan_config;
    struct ifreq userdata;
    int line = 0;
    s32 CmdNum = BG_SCAN_CONFIG;
    s32 Action;
    u16 Size;
    int ioctl_val, subioctl_val;

    if (get_priv_ioctl("setgetconf",
                       &ioctl_val, &subioctl_val) == WLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    if (argc != 4) {
        printf("Error: invalid no of arguments\n");
        printf("Syntax: ./wlanconfig eth1 bgscanconfig <filename>\n");
        exit(1);
    }

    strncpy(filename, argv[3], MIN(sizeof(filename) - 1, strlen(argv[3])));
    if ((fp = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "Cannot open file %s\n", filename);
        exit(1);
    }

    buf = (u8 *) malloc(MAX_SETGET_CONF_SIZE);
    if (buf == NULL) {
        printf("Error: allocate memory for bgscan fail\n");
        fclose(fp);
        return -ENOMEM;
    }
    memset(buf, 0, MAX_SETGET_CONF_SIZE);

    bgscan_config = (HostCmd_DS_802_11_BG_SCAN_CONFIG *)
        (buf + sizeof(s32) + sizeof(u16));

    while ((pos =
            (u8 *) wlan_config_get_line((s8 *) scanCfg, sizeof(scanCfg), fp,
                                        &line))) {
        if (strcmp((char *) pos, "bgscan={") == 0) {
            wlan_get_bgscan_data(fp, &line, bgscan_config);
        }
    }

    fclose(fp);

    Action = bgscan_config->Action;

    Size = sizeof(HostCmd_DS_802_11_BG_SCAN_CONFIG) +
        TLVSsidSize + TLVProbeSize + TLVChanSize + TLVSnrSize + TLVBcProbeSize +
        TLVNumSsidProbeSize;
    Size += TLVStartBGScanLaterSize;

    memcpy(buf, &CmdNum, sizeof(s32));
    memcpy(buf + sizeof(s32), &Size, sizeof(u16));

    strncpy(userdata.ifr_name, dev_name, IFNAMSIZ);
    userdata.ifr_data = buf;

    if (ioctl(sockfd, ioctl_val, &userdata)) {
        fprintf(stderr, "wlanconfig: BG_SCAN is not supported by %s\n",
                dev_name);
        return -EFAULT;
    }

    if (Action == HostCmd_ACT_GEN_GET) {
        Size = *(u16 *) buf;
        hexdump("BGSCAN Configuration setup", &buf[SKIP_TYPE_SIZE], Size, ' ');
    }

    free(buf);

    return WLAN_STATUS_SUCCESS;

}

/** 
 *  @brief Validates bgscan config arguments 
 *  @param argc		number of arguments
 *  @param argv         A pointer to arguments array    
 *  @return      	WLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
bgscfg_validate_args(int argc, char *argv[])
{
    if (argc > 6) {
        printf("Error: invalid no of arguments\n");
        return WLAN_STATUS_FAILURE;
    }

    if (argc == 4) {
        if (strlen(argv[3]) != 1) {
            printf("Error: Argument 3 is invalid \n");
            return WLAN_STATUS_FAILURE;
        }
        if (atoi(argv[3]) != 0) {
            printf("Error: invalid arguments\n");
            return WLAN_STATUS_FAILURE;
        }
    } else if (argc == 5) {
        if (strlen(argv[3]) != 1) {
            printf("Error: Argument 3 is invalid \n");
            return WLAN_STATUS_FAILURE;
        }
        if (atoi(argv[3]) != 0) {
            printf("Error: invalid arguments\n");
            return WLAN_STATUS_FAILURE;
        }
        if (strlen(argv[4]) != 1) {
            printf("Error: Argument 4 is invalid \n");
            return WLAN_STATUS_FAILURE;
        }
    } else if (argc == 6) {
        if (strlen(argv[3]) != 1) {
            printf("Error: Argument 3 is invalid \n");
            return WLAN_STATUS_FAILURE;
        }
        if (strlen(argv[4]) != 1) {
            printf("Error: Argument 4 is invalid \n");
            return WLAN_STATUS_FAILURE;
        }
    }
    return WLAN_STATUS_SUCCESS;

}

/** 
 *  @brief Process bgscan config 
 *  @param argc		number of arguments
 *  @param argv         A pointer to arguments array    
 *  @return      	WLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
process_bgscfg(int argc, char *argv[])
{
    u8 scanCfg[256], *pos, *buf = NULL;
    s8 filename[48] = "";
    FILE *fp;
    HostCmd_DS_802_11_BG_SCAN_CONFIG *bgscan_config;
    struct ifreq userdata;
    int line = 0;
    s32 CmdNum = BG_SCAN_CFG;
    u8 arrConfigType[2] = { NORMAL_BG_SCAN_CFG, UAPSD_PPS_BG_SCAN_CFG };
    u16 Size = 0;
    int ioctl_val, subioctl_val;
    int ii = 0;

    if (get_priv_ioctl("setgetconf",
                       &ioctl_val, &subioctl_val) == WLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    if (bgscfg_validate_args(argc, argv)) {
        printf("Syntax: ./wlanconfig eth1 bgscfg [e] [c] [f]\n");
        return WLAN_STATUS_FAILURE;
    }

    buf = (u8 *) malloc(MAX_SETGET_CONF_SIZE);
    if (buf == NULL) {
        printf("Error: allocate memory for bgscan fail\n");
        return -ENOMEM;
    }
    memset(buf, 0, MAX_SETGET_CONF_SIZE);
    bgscan_config = (HostCmd_DS_802_11_BG_SCAN_CONFIG *)
        (buf + sizeof(s32) + sizeof(u16));

    memcpy(buf, &CmdNum, sizeof(s32));
    strncpy(userdata.ifr_name, dev_name, IFNAMSIZ);
    userdata.ifr_data = buf;

    if (argc == 3) {
        for (ii = 0; ii < NELEMENTS(arrConfigType); ii++) {
            /* Get both default bgscan and PPS/UAPSD bgscan config */
            bgscan_config->Action = HostCmd_ACT_GEN_GET;
            Size = sizeof(HostCmd_DS_802_11_BG_SCAN_CONFIG);
            bgscan_config->ConfigType = arrConfigType[ii];
            bgscan_config->Enable = TRUE;
            memcpy(buf + sizeof(s32), &Size, sizeof(u16));
            if (ioctl(sockfd, ioctl_val, &userdata)) {
                fprintf(stderr,
                        "wlanconfig: BG_SCAN get is not supported by %s\n",
                        dev_name);
                free(buf);
                return -EFAULT;
            }
            Size = *(u16 *) & buf[SKIP_CMDNUM];
            hexdump("BGSCAN Configuration setup", &buf[SKIP_CMDNUM + SKIP_SIZE],
                    Size, ' ');
        }
        free(buf);
        return WLAN_STATUS_SUCCESS;
    } else if (argc == 4) {
        /* Disable both UAPSD/PPS and normal bgscan config */
        bgscan_config->Enable = atoi(argv[3]);
        bgscan_config->Action = HostCmd_ACT_GEN_SET;
        bgscan_config->ConfigType = ALL_BG_SCAN_CFGS;
    } else if (argc == 5) {
        /* Disable either UAPSD/PPS or normal bgscan config */
        bgscan_config->Enable = atoi(argv[3]);
        bgscan_config->Action = HostCmd_ACT_GEN_SET;
        bgscan_config->ConfigType = atoi(argv[4]);
    }
    if (argc == 6) {
        strncpy(filename, argv[5], MIN(sizeof(filename) - 1, strlen(argv[5])));
        if ((fp = fopen(filename, "r")) == NULL) {
            fprintf(stderr, "Cannot open file %s\n", filename);
            free(buf);
            exit(1);
        }
        while ((pos =
                (u8 *) wlan_config_get_line((s8 *) scanCfg, sizeof(scanCfg), fp,
                                            &line))) {
            if (strcmp((char *) pos, "bgscan={") == 0) {
                wlan_get_bgscan_data(fp, &line, bgscan_config);
            }
        }
        Size = sizeof(HostCmd_DS_802_11_BG_SCAN_CONFIG) +
            TLVSsidSize + TLVProbeSize + TLVChanSize + TLVSnrSize +
            TLVBcProbeSize + TLVNumSsidProbeSize;
        Size += TLVStartBGScanLaterSize;
        bgscan_config->Enable = atoi(argv[3]);
        bgscan_config->Action = HostCmd_ACT_GEN_SET;
        bgscan_config->ConfigType = atoi(argv[4]);
        fclose(fp);
    }

    if (argc != 6) {
        Size = sizeof(HostCmd_DS_802_11_BG_SCAN_CONFIG);
    }
    memcpy(buf + sizeof(s32), &Size, sizeof(u16));

    if (ioctl(sockfd, ioctl_val, &userdata)) {
        fprintf(stderr, "wlanconfig: BG_SCAN is not supported by %s\n",
                dev_name);
        free(buf);
        return -EFAULT;
    }
    free(buf);
    return WLAN_STATUS_SUCCESS;
}

/** 
 *  @brief parse hex data
 *  @param fp 		A pointer to FILE stream
 *  @param dst		A pointer to receive hex data
 *  @return            	length of hex data
 */
static int
fparse_for_hex(FILE * fp, u8 * dst)
{
    s8 *ptr;
    u8 *dptr;
    s8 buf[256];

    dptr = dst;
    while (fgets(buf, sizeof(buf), fp)) {
        ptr = buf;

        while (*ptr) {
            /* skip leading spaces */
            while (*ptr && (isspace(*ptr) || *ptr == '\t'))
                ptr++;

            /* skip blank lines and lines beginning with '#' */
            if (*ptr == '\0' || *ptr == '#')
                break;

            if (isxdigit(*ptr)) {
                ptr = convert2hex(ptr, dptr++);
            } else {
                /* Invalid character on data line */
                ptr++;
            }
        }
    }

    return (dptr - dst);
}

/** 
 *  @brief read register
 *  @param cmd 		the type of register
 *  @param stroffset	A pointer to register index string
 *  @return            	WLAN_STATUS_SUCCESS or WLAN_STATUS_FAILURE
 */
static int
process_read_register(s32 cmd, s8 * stroffset)
{
    struct ifreq userdata;
    wlan_ioctl_regrdwr reg;
    s8 *whichreg;
    int ioctl_val, subioctl_val;

    if (get_priv_ioctl("regrdwr",
                       &ioctl_val, &subioctl_val) == WLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }
    memset(&reg, 0, sizeof(reg));
    switch (cmd) {
    case CMD_RDMAC:
        /* 
         * HostCmd_CMD_MAC_REG_ACCESS 
         */
        reg.WhichReg = REG_MAC;
        whichreg = "MAC";
        break;
    case CMD_RDBBP:
        /* 
         * HostCmd_CMD_BBP_REG_ACCESS 
         */
        reg.WhichReg = REG_BBP;
        whichreg = "BBP";
        break;
    case CMD_RDRF:
        /* 
         * HostCmd_CMD_RF_REG_ACCESS 
         */
        reg.WhichReg = REG_RF;
        whichreg = "RF";
        break;
    case CMD_RDPMIC:
        /* 
         * HostCmd_CMD_PMIC_REG_ACCESS
         */
        reg.WhichReg = REG_PMIC;
        whichreg = "PMIC";
        break;
    default:
        fprintf(stderr, "Invalid Register set specified.\n");
        return WLAN_STATUS_FAILURE;
    }

    reg.Action = 0;             /* READ */

    if (!strncasecmp(stroffset, "0x", 2))
        reg.Offset = a2hex((stroffset + 2));
    else
        reg.Offset = atoi(stroffset);

    strncpy(userdata.ifr_name, dev_name, IFNAMSIZ);
    userdata.ifr_data = (s8 *) & reg;

    if (ioctl(sockfd, ioctl_val, &userdata)) {
        perror("wlanconfig");
        fprintf(stderr,
                "wlanconfig: Register Reading not supported by"
                "interface %s\n", dev_name);
        return WLAN_STATUS_FAILURE;
    }

    printf("%s[0x%04lx] = 0x%08lx\n", whichreg, reg.Offset, reg.Value);

    return WLAN_STATUS_SUCCESS;
}

/** 
 *  @brief write register
 *  @param cmd 		the type of register
 *  @param stroffset	A pointer to register index string
 *  @param strvalue	A pointer to the register value
 *  @return            	WLAN_STATUS_SUCCESS or WLAN_STATUS_FAILURE
 */
static int
process_write_register(s32 cmd, s8 * stroffset, s8 * strvalue)
{
    struct ifreq userdata;
    wlan_ioctl_regrdwr reg;
    s8 *whichreg;
    int ioctl_val, subioctl_val;

    if (get_priv_ioctl("regrdwr",
                       &ioctl_val, &subioctl_val) == WLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    switch (cmd) {
    case CMD_WRMAC:
        /* 
         * HostCmd_CMD_MAC_REG_ACCESS 
         */
        reg.WhichReg = REG_MAC;
        whichreg = "MAC";
        break;
    case CMD_WRBBP:
        /* 
         * HostCmd_CMD_BBP_REG_ACCESS 
         */
        reg.WhichReg = REG_BBP;
        whichreg = "BBP";
        break;
    case CMD_WRRF:
        /* 
         * HostCmd_CMD_RF_REG_ACCESS 
         */
        reg.WhichReg = REG_RF;
        whichreg = "RF";
        break;
    case CMD_WRPMIC:
        /* 
         * HostCmd_CMD_PMIC_REG_ACCESS
         */
        reg.WhichReg = REG_PMIC;
        whichreg = "PMIC";
        break;
    default:
        fprintf(stderr, "Invalid register set specified.\n");
        return WLAN_STATUS_FAILURE;
    }

    reg.Action = 1;             /* WRITE */

    if (!strncasecmp(stroffset, "0x", 2))
        reg.Offset = a2hex((stroffset + 2));
    else
        reg.Offset = atoi(stroffset);

    if (!strncasecmp(strvalue, "0x", 2))
        reg.Value = a2hex((strvalue + 2));
    else
        reg.Value = atoi(strvalue);

    printf("Writing %s Register 0x%04lx with 0x%08lx\n", whichreg,
           reg.Offset, reg.Value);

    strncpy(userdata.ifr_name, dev_name, IFNAMSIZ);
    userdata.ifr_data = (s8 *) & reg;

    if (ioctl(sockfd, ioctl_val, &userdata)) {
        perror("wlanconfig");
        fprintf(stderr,
                "wlanconfig: Register Writing not supported "
                "by interface %s\n", dev_name);
        return WLAN_STATUS_FAILURE;
    }

    printf("%s[0x%04lx] = 0x%08lx\n", whichreg, reg.Offset, reg.Value);

    return WLAN_STATUS_SUCCESS;
}

/**
 *  @brief iwpriv command hscfg
 *  @param cond		condition
 *  @param gpio         GPIO number
 *  @param gap          Gap
 *  @return      	WLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
do_hscfg(u32 cond, u8 gpio, u8 gap)
{
    struct iwreq iwr;
    int buf[3] = { -1, 0xff, 0xff };
    int ioctl_val, subioctl_val;

    if (get_priv_ioctl("hscfg",
                       &ioctl_val, &subioctl_val) == WLAN_STATUS_FAILURE) {
        return WLAN_STATUS_FAILURE;
    }

    buf[0] = cond;
    buf[1] = gpio;
    buf[2] = gap;

    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.pointer = (u8 *) buf;
    iwr.u.data.length = 3;
    iwr.u.data.flags = subioctl_val;

    if (ioctl(sockfd, ioctl_val, &iwr)) {
        perror("wlanconfig");
        fprintf(stderr,
                "wlanconfig: hscfg not supported by "
                "interface %s\n", dev_name);
        return WLAN_STATUS_FAILURE;
    }

    return WLAN_STATUS_SUCCESS;
}

/** 
 *  @brief decode wireless custom event
 *
 *  @param h		A pointer to structure nlmsghdr
 *  @param left		number of message for decoding
 *  @param queue_id	message queue ID
 *  @return            	number of message leftover
 */
static int
decode_iwevcustom(struct nlmsghdr *h, int left)
{
    int len, plen, attrlen, nlmsg_len, rta_len;
    struct ifinfomsg *ifi;
    struct rtattr *attr;

    while (left >= sizeof(*h)) {
        len = h->nlmsg_len;
        plen = len - sizeof(*h);
        if (len > left || plen < 0) {
            printf("malformed netlink message: len=%d left=%d plen=%d",
                   len, left, plen);
            break;
        }
        if (h->nlmsg_type != RTM_NEWLINK) {
            break;
        }
        if (plen < sizeof(*ifi)) {
            printf("[plen=%d] < [sizeof(strcut ifinfomsg)=%d]\n",
                   h->nlmsg_type, sizeof(*ifi));
            break;
        }

        ifi = NLMSG_DATA(h);
        nlmsg_len = NLMSG_ALIGN(sizeof(struct ifinfomsg));
        attrlen = h->nlmsg_len - nlmsg_len;
        if (attrlen < 0) {
            printf("h->nlmsg_len=%d nlmsg_len=%d attrlen=%d\n",
                   h->nlmsg_len, nlmsg_len, attrlen);
            break;
        }

        attr = (struct rtattr *) (((char *) ifi) + nlmsg_len);
        rta_len = RTA_ALIGN(sizeof(struct rtattr));
        while (RTA_OK(attr, attrlen)) {
            if (attr->rta_type == IFLA_WIRELESS) {
                struct iw_event *iwe;
                char *pos = ((char *) attr) + rta_len;
                char *end = pos + (attr->rta_len - rta_len);
                char *custom;
                u16 dlen;

                while (pos + IW_EV_LCP_LEN <= end) {
                    iwe = (struct iw_event *) pos;
                    if ((iwe->len <= IW_EV_LCP_LEN)
                        || (iwe->cmd != IWEVCUSTOM))
                        break;

                    custom = pos + IW_EV_POINT_LEN;
                    if (IW_EV_POINT_LEN ==
                        IW_EV_LCP_LEN + sizeof(struct iw_point)) {
                        dlen = iwe->u.data.length;
                    } else {    /* WIRELESS_EXT >= 19 */
                        dlen = *((u16 *) (pos + IW_EV_LCP_LEN));
                    }
                    if (custom + dlen > end) {
                        printf("corrupt custom wireless event: '%s'\n", custom);
                        break;
                    }

                    if (strlen(CUS_EVT_HS_ACTIVATED) == strlen(custom)
                        && !strncmp(CUS_EVT_HS_ACTIVATED, custom,
                                    strlen(CUS_EVT_HS_ACTIVATED))) {
                        hs_state = HS_STATE_ACTIVATED;
                        printf("Host Sleep Activated\n");
                    } else if (strlen(CUS_EVT_HS_DEACTIVATED) == strlen(custom)
                               && !strncmp(CUS_EVT_HS_DEACTIVATED, custom,
                                           strlen(CUS_EVT_HS_DEACTIVATED))) {
                        hs_state = HS_STATE_DEACTIVATED;
                        printf("Host Sleep Deactivated\n");
                    } else if (strlen(CUS_EVT_HS_WAKEUP) == strlen(custom)
                               && !strncmp(CUS_EVT_HS_WAKEUP, custom,
                                           strlen(CUS_EVT_HS_WAKEUP))) {
                        if (hs_state != HS_STATE_WAKEUP) {
                            hs_state = HS_STATE_WAKEUP;
                            if (do_hscfg(HOST_SLEEP_CFG_CANCEL, 0, 0)
                                != WLAN_STATUS_SUCCESS) {
                                printf("do_hscfg(cancel) failed\n");
                            }
                        }
                    }
                    pos += iwe->len;
                }
            }
            attr = RTA_NEXT(attr, attrlen);
        }

        len = NLMSG_ALIGN(len);
        left -= len;
        h = (struct nlmsghdr *) ((char *) h + len);
    }

    return left;
}

/** 
 *  @brief NETLINK message receiver
 *
 *  @return            	WLAN_STATUS_SUCCESS or WLAN_STATUS_FAILURE
 */
static int
netlink_msg_receiver(void)
{
    struct sockaddr_nl local;
    int sock_nl = socket(PF_NETLINK, SOCK_RAW, NETLINK_ROUTE);

    if (sock_nl < 0) {
        fprintf(stderr,
                "wlanconfig hstest: socket(PF_NETLINK,SOCK_RAW,NETLINK_ROUTE)"
                "returned %d\n", sock_nl);
        return WLAN_STATUS_FAILURE;
    }

    memset(&local, 0, sizeof(local));
    local.nl_family = AF_NETLINK;
    local.nl_groups = RTMGRP_LINK;
    if (bind(sock_nl, (struct sockaddr *) &local, sizeof(local)) < 0) {
        fprintf(stderr, "wlanconfig hstest: bind(%d) failed\n", sock_nl);
        close(sock_nl);
        return WLAN_STATUS_FAILURE;
    }

    while (1) {
        fd_set rfds;
        int retval;
        struct sockaddr_nl from;
        socklen_t fromlen = sizeof(from);
        char msg[MAX_MSG_SIZE];
        int left;

        FD_ZERO(&rfds);
        FD_SET(sock_nl, &rfds);
        retval = select(sock_nl + 1, &rfds, NULL, NULL, NULL);
        if ((retval < 0 && errno != EINTR) || !FD_ISSET(sock_nl, &rfds)) {
            fprintf(stderr, "wlanconfig hstest: select() failed, errno=%d\n",
                    errno);
            close(sock_nl);
            return WLAN_STATUS_FAILURE;
        }

        left = recvfrom(sock_nl, msg, sizeof(msg), MSG_DONTWAIT,
                        (struct sockaddr *) &from, &fromlen);
        if (left < 0) {
            fprintf(stderr, "wlanconfig hstest: recvfrom() failed, errno=%d\n",
                    errno);
            close(sock_nl);
            return WLAN_STATUS_FAILURE;
        }

        left = decode_iwevcustom((struct nlmsghdr *) msg, left);
    }
    close(sock_nl);

    return WLAN_STATUS_SUCCESS;
}

/** 
 *  @brief test Host Sleep
 *
 *  @return            	WLAN_STATUS_SUCCESS or WLAN_STATUS_FAILURE
 */
static int
process_hs_test(void)
{
    netlink_msg_receiver();

    return WLAN_STATUS_SUCCESS;
}

/**
 *  @brief Get the CFP table based on the region code
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *
 *  @return         WLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
process_getcfptable(int argc, char *argv[])
{
    pwlan_ioctl_cfp_table cfptable;
    int ioctl_val, subioctl_val;
    struct iwreq iwr;
    int region = 0;
    int i;

    if (get_priv_ioctl("getcfptable",
                       &ioctl_val, &subioctl_val) == WLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    if (argc > 4) {
        printf("Error: invalid number of arguments\n");
        printf("Syntax: ./wlanconfig ethX getcfptable [regioncode]");
        return -EINVAL;
    }

    if (argc == 4) {
        if ((region = atoval(argv[3])) < 0)
            return -EINVAL;
    }

    cfptable = (pwlan_ioctl_cfp_table) malloc(sizeof(wlan_ioctl_cfp_table));
    if (cfptable == NULL) {
        printf("Error: allocate memory for CFP table failed\n");
        return -ENOMEM;
    }
    memset(cfptable, 0, sizeof(wlan_ioctl_cfp_table));

    cfptable->region = region;
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.pointer = (caddr_t) cfptable;
    iwr.u.data.length = sizeof(wlan_ioctl_cfp_table);
    iwr.u.data.flags = subioctl_val;

    if (ioctl(sockfd, ioctl_val, &iwr)) {
        fprintf(stderr, "wlanconfig: get CFP table failed\n");
        free(cfptable);
        return WLAN_STATUS_FAILURE;
    }

    printf("-------------------------------\n");
    printf(" #  | ch  | freq | max_tx_power\n");
    printf("-------------------------------\n");

    for (i = 0; i < cfptable->cfp_no; i++) {
        printf(" %02u | %03d | %04ld | %02d\n",
               i + 1,
               cfptable->cfp[i].Channel,
               cfptable->cfp[i].Freq, cfptable->cfp[i].MaxTxPower);
    }

    free(cfptable);
    return WLAN_STATUS_SUCCESS;
}

#define STACK_NBYTES            	100     /**< Number of bytes in stack */
#define MAX_BYTESEQ 		       	6       /**< Maximum byte sequence */
#define TYPE_DNUM           		1 /**< decimal number */
#define TYPE_BYTESEQ        		2 /**< byte sequence */
#define MAX_OPERAND         		0x40    /**< Maximum operands */
#define TYPE_EQ         		(MAX_OPERAND+1) /**< byte comparison:    == operator */
#define TYPE_EQ_DNUM    		(MAX_OPERAND+2) /**< decimal comparison: =d operator */
#define TYPE_EQ_BIT     		(MAX_OPERAND+3) /**< bit comparison:     =b operator */
#define TYPE_AND        		(MAX_OPERAND+4) /**< && operator */
#define TYPE_OR         		(MAX_OPERAND+5) /**< || operator */
typedef struct
{
    u16 sp;                         /**< Stack pointer */
    u8 byte[STACK_NBYTES];          /**< Stack */
} stack_t;

typedef struct
{
    u8 type;                    /**< Type */
    u8 reserve[3];       /**< so 4-byte align val array */
    /* byte sequence is the largest among all the operands and operators. */
    /* byte sequence format: 1 byte of num of bytes, then variable num bytes */
    u8 val[MAX_BYTESEQ + 1];    /**< Value */
} op_t;

/** 
 *  @brief push data to stack
 *  
 *  @param s			a pointer to stack_t structure
 *  
 *  @param nbytes		number of byte to push to stack  
 *  
 *  @param val			a pointer to data buffer	
 *  
 *  @return			TRUE-- sucess , FALSE -- fail
 *  			
 */
static int
push_n(stack_t * s, u8 nbytes, u8 * val)
{
    if ((s->sp + nbytes) < STACK_NBYTES) {
        memcpy((void *) (s->byte + s->sp), (const void *) val, (size_t) nbytes);
        s->sp += nbytes;
        /* printf("push: n %d sp %d\n", nbytes, s->sp); */
        return TRUE;
    } else                      /* stack full */
        return FALSE;
}

/** 
 *  @brief push data to stack
 *  
 *  @param s			a pointer to stack_t structure
 *  
 *  @param op			a pointer to op_t structure  
 *  
 *  @return			TRUE-- sucess , FALSE -- fail
 *  			
 */
static int
push(stack_t * s, op_t * op)
{
    u8 nbytes;
    switch (op->type) {
    case TYPE_DNUM:
        if (push_n(s, 4, op->val))
            return (push_n(s, 1, &op->type));
        return FALSE;
    case TYPE_BYTESEQ:
        nbytes = op->val[0];
        if (push_n(s, nbytes, op->val + 1) &&
            push_n(s, 1, op->val) && push_n(s, 1, &op->type))
            return TRUE;
        return FALSE;
    default:
        return (push_n(s, 1, &op->type));
    }
}

/** 
 *  @brief parse RPN string
 *  
 *  @param s			a pointer to Null-terminated string to scan. 
 *  
 *  @param first_time		a pointer to return first_time  
 *  
 *  @return			A pointer to the last token found in string.   
 *  				NULL is returned when there are no more tokens to be found. 
 *  			
 */
static char *
getop(char *s, int *first_time)
{
    const char delim[] = " \t\n";
    char *p;
    if (*first_time) {
        p = strtok(s, delim);
        *first_time = FALSE;
    } else {
        p = strtok(NULL, delim);
    }
    return (p);
}

/** 
 *  @brief Verify hex digit.
 *  
 *  @param c			input ascii char 
 *  @param h			a pointer to return integer value of the digit char. 
 *  @return			TURE -- c is hex digit, FALSE -- c is not hex digit.
 */
static int
ishexdigit(char c, u8 * h)
{
    if (c >= '0' && c <= '9') {
        *h = c - '0';
        return (TRUE);
    } else if (c >= 'a' && c <= 'f') {
        *h = c - 'a' + 10;
        return (TRUE);
    } else if (c >= 'A' && c <= 'F') {
        *h = c - 'A' + 10;
        return (TRUE);
    }
    return (FALSE);
}

/** 
 *  @brief convert hex string to integer.
 *  
 *  @param s			A pointer to hex string, string length up to 2 digits. 
 *  @return			integer value.
 */
static u8
hex_atoi(char *s)
{
    int i;
    u8 digit;                   /* digital value */
    u8 t = 0;                   /* total value */

    for (i = 0, t = 0; ishexdigit(s[i], &digit) && i < 2; i++)
        t = 16 * t + digit;
    return (t);
}

/** 
 *  @brief Parse byte sequence in hex format string to a byte sequence.
 *  
 *  @param opstr		A pointer to byte sequence in hex format string, with ':' as delimiter between two byte. 
 *  @param val			A pointer to return byte sequence string
 *  @return			NA
 */
static void
parse_hex(char *opstr, u8 * val)
{
    char delim = ':';
    char *p;
    char *q;
    u8 i;

    /* +1 is for skipping over the preceding h character. */
    p = opstr + 1;

    /* First byte */
    val[1] = hex_atoi(p++);

    /* Parse subsequent bytes. */
    /* Each byte is preceded by the : character. */
    for (i = 1; *p; i++) {
        q = strchr(p, delim);
        if (!q)
            break;
        p = q + 1;
        val[i + 1] = hex_atoi(p);
    }
    /* Set num of bytes */
    val[0] = i;
}

/** 
 *  @brief str2bin, convert RPN string to binary format
 *  
 *  @param str			A pointer to rpn string
 *  @param stack		A pointer to stack_t structure
 *  @return			WLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
str2bin(char *str, stack_t * stack)
{
    int first_time;
    char *opstr;
    op_t op;                    /* operator/operand */
    int dnum;
    int ret = WLAN_STATUS_SUCCESS;

    memset(stack, 0, sizeof(stack_t));
    first_time = TRUE;
    while ((opstr = getop(str, &first_time)) != NULL) {
        if (isdigit(*opstr)) {
            op.type = TYPE_DNUM;
            dnum = atoi(opstr);
            *(u32 *) op.val = (u32) dnum;
            if (!push(stack, &op)) {
                printf("push decimal number failed\n");
                ret = WLAN_STATUS_FAILURE;
                break;
            }
        } else if (*opstr == 'h') {
            op.type = TYPE_BYTESEQ;
            parse_hex(opstr, op.val);
            if (!push(stack, &op)) {
                printf("push byte sequence failed\n");
                ret = WLAN_STATUS_FAILURE;
                break;
            }
        } else if (!strcmp(opstr, "==")) {
            op.type = TYPE_EQ;
            if (!push(stack, &op)) {
                printf("push byte cmp operator failed\n");
                ret = WLAN_STATUS_FAILURE;
                break;
            }
        } else if (!strcmp(opstr, "=d")) {
            op.type = TYPE_EQ_DNUM;
            if (!push(stack, &op)) {
                printf("push decimal cmp operator failed\n");
                ret = WLAN_STATUS_FAILURE;
                break;
            }
        } else if (!strcmp(opstr, "=b")) {
            op.type = TYPE_EQ_BIT;
            if (!push(stack, &op)) {
                printf("push bit cmp operator failed\n");
                ret = WLAN_STATUS_FAILURE;
                break;
            }
        } else if (!strcmp(opstr, "&&")) {
            op.type = TYPE_AND;
            if (!push(stack, &op)) {
                printf("push AND operator failed\n");
                ret = WLAN_STATUS_FAILURE;
                break;
            }
        } else if (!strcmp(opstr, "||")) {
            op.type = TYPE_OR;
            if (!push(stack, &op)) {
                printf("push OR operator failed\n");
                ret = WLAN_STATUS_FAILURE;
                break;
            }
        } else {
            printf("Unknown operand\n");
            ret = WLAN_STATUS_FAILURE;
            break;
        }
    }
    return ret;
}

#define FILTER_BYTESEQ 		TYPE_EQ /**< byte sequence */
#define FILTER_DNUM    		TYPE_EQ_DNUM /**< decimal number */
#define FILTER_BITSEQ		TYPE_EQ_BIT /**< bit sequence */
#define FILTER_TEST		FILTER_BITSEQ+1 /**< test */

#define NAME_TYPE		1           /**< Field name 'type' */
#define NAME_PATTERN		2       /**< Field name 'pattern' */
#define NAME_OFFSET		3           /**< Field name 'offset' */
#define NAME_NUMBYTE		4       /**< Field name 'numbyte' */
#define NAME_REPEAT		5           /**< Field name 'repeat' */
#define NAME_BYTE		6           /**< Field name 'byte' */
#define NAME_MASK		7           /**< Field name 'mask' */
#define NAME_DEST		8           /**< Field name 'dest' */
static struct mef_fields
{
    s8 *name;
            /**< Name */
    s8 nameid;/**< Name Id. */
} mef_fields[] = {
    {
    "type", NAME_TYPE}, {
    "pattern", NAME_PATTERN}, {
    "offset", NAME_OFFSET}, {
    "numbyte", NAME_NUMBYTE}, {
    "repeat", NAME_REPEAT}, {
    "byte", NAME_BYTE}, {
    "mask", NAME_MASK}, {
    "dest", NAME_DEST}
};

/** 
 *  @brief get filter data
 *  
 *  @param fp			A pointer to file stream
 *  @param ln			A pointer to line number
 *  @param buf			A pointer to hostcmd data
 *  @param size			A pointer to the return size of hostcmd buffer
 *  @return			WLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
wlan_get_filter_data(FILE * fp, int *ln, u8 * buf, u16 * size)
{
    s32 errors = 0, i;
    s8 line[256], *pos, *pos1;
    u16 type = 0;
    u32 pattern = 0;
    u16 repeat = 0;
    u16 offset = 0;
    s8 byte_seq[50];
    s8 mask_seq[50];
    u16 numbyte = 0;
    s8 type_find = 0;
    s8 pattern_find = 0;
    s8 offset_find = 0;
    s8 numbyte_find = 0;
    s8 repeat_find = 0;
    s8 byte_find = 0;
    s8 mask_find = 0;
    s8 dest_find = 0;
    s8 dest_seq[50];

    *size = 0;
    while ((pos = wlan_config_get_line(line, sizeof(line), fp, ln))) {
        if (strcmp(pos, "}") == 0) {
            break;
        }
        pos1 = strchr(pos, '=');
        if (pos1 == NULL) {
            printf("Line %d: Invalid mef_filter line '%s'\n", *ln, pos);
            errors++;
            continue;
        }
        *pos1++ = '\0';
        for (i = 0; i < NELEMENTS(mef_fields); i++) {
            if (strncmp(pos, mef_fields[i].name, strlen(mef_fields[i].name)) ==
                0) {
                switch (mef_fields[i].nameid) {
                case NAME_TYPE:
                    type = a2hex_or_atoi(pos1);
                    if ((type != FILTER_DNUM) && (type != FILTER_BYTESEQ)
                        && (type != FILTER_BITSEQ) && (type != FILTER_TEST)) {
                        printf("Invalid filter type:%d\n", type);
                        return WLAN_STATUS_FAILURE;
                    }
                    type_find = 1;
                    break;
                case NAME_PATTERN:
                    pattern = a2hex_or_atoi(pos1);
                    pattern_find = 1;
                    break;
                case NAME_OFFSET:
                    offset = a2hex_or_atoi(pos1);
                    offset_find = 1;
                    break;
                case NAME_NUMBYTE:
                    numbyte = a2hex_or_atoi(pos1);
                    numbyte_find = 1;
                    break;
                case NAME_REPEAT:
                    repeat = a2hex_or_atoi(pos1);
                    repeat_find = 1;
                    break;
                case NAME_BYTE:
                    memset(byte_seq, 0, sizeof(byte_seq));
                    strcpy(byte_seq, pos1);
                    byte_find = 1;
                    break;
                case NAME_MASK:
                    memset(mask_seq, 0, sizeof(mask_seq));
                    strcpy(mask_seq, pos1);
                    mask_find = 1;
                    break;
                case NAME_DEST:
                    memset(dest_seq, 0, sizeof(dest_seq));
                    strcpy(dest_seq, pos1);
                    dest_find = 1;
                    break;
                }
                break;
            }
        }
        if (i == NELEMENTS(mef_fields)) {
            printf("Line %d: unknown mef field '%s'.\n", *line, pos);
            errors++;
        }
    }
    if (type_find == 0) {
        printf("Can not find filter type\n");
        return WLAN_STATUS_FAILURE;
    }
    switch (type) {
    case FILTER_DNUM:
        if (!pattern_find || !offset_find || !numbyte_find) {
            printf
                ("Missing field for FILTER_DNUM: pattern=%d,offset=%d,numbyte=%d\n",
                 pattern_find, offset_find, numbyte_find);
            return WLAN_STATUS_FAILURE;
        }
        memset(line, 0, sizeof(line));
        sprintf(line, "%ld %d %d =d ", pattern, offset, numbyte);
        break;
    case FILTER_BYTESEQ:
        if (!byte_find || !offset_find || !repeat_find) {
            printf
                ("Missing field for FILTER_BYTESEQ: byte=%d,offset=%d,repeat=%d\n",
                 byte_find, offset_find, repeat_find);
            return WLAN_STATUS_FAILURE;
        }
        memset(line, 0, sizeof(line));
        sprintf(line, "%d h%s %d == ", repeat, byte_seq, offset);
        break;
    case FILTER_BITSEQ:
        if (!byte_find || !offset_find || !mask_find) {
            printf
                ("Missing field for FILTER_BYTESEQ: byte=%d,offset=%d,repeat=%d\n",
                 byte_find, offset_find, mask_find);
            return WLAN_STATUS_FAILURE;
        }
        if (strlen(byte_seq) != strlen(mask_seq)) {
            printf("byte string's length is different with mask's length!\n");
            return WLAN_STATUS_FAILURE;
        }
        memset(line, 0, sizeof(line));
        sprintf(line, "h%s %d h%s =b ", byte_seq, offset, mask_seq);
        break;
    case FILTER_TEST:
        if (!byte_find || !offset_find || !repeat_find || !dest_find) {
            printf
                ("Missing field for FILTER_TEST: byte=%d,offset=%d,repeat=%d,dest=%d\n",
                 byte_find, offset_find, repeat_find, dest_find);
            return WLAN_STATUS_FAILURE;
        }
        memset(line, 0, sizeof(line));
        sprintf(line, "h%s %d h%s %d ", dest_seq, repeat, byte_seq, offset);
        break;
    }
    memcpy(buf, line, strlen(line));
    *size = strlen(line);
    return WLAN_STATUS_SUCCESS;
}

#define NAME_MODE	1       /**< Field name 'mode' */
#define NAME_ACTION	2       /**< Field name 'action' */
#define NAME_FILTER_NUM	3   /**< Field name 'filter_num' */
#define NAME_RPN	4       /**< Field name 'RPN' */
static struct mef_entry_fields
{
    s8 *name;
            /**< Name */
    s8 nameid;/**< Name id */
} mef_entry_fields[] = {
    {
    "mode", NAME_MODE}, {
    "action", NAME_ACTION}, {
    "filter_num", NAME_FILTER_NUM}, {
"RPN", NAME_RPN},};

typedef struct _MEF_ENTRY
{
    /** Mode */
    u8 Mode;
    /** Size */
    u8 Action;
    /** Size of expression */
    u16 ExprSize;
} MEF_ENTRY;

/** 
 *  @brief get mef_entry data
 *  
 *  @param fp			A pointer to file stream
 *  @param ln			A pointer to line number
 *  @param buf			A pointer to hostcmd data
 *  @param size			A pointer to the return size of hostcmd buffer
 *  @return			WLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
wlan_get_mef_entry_data(FILE * fp, int *ln, u8 * buf, u16 * size)
{
    s8 line[256], *pos, *pos1;
    u8 mode, action, filter_num = 0;
    s8 rpn[256];
    s8 mode_find = 0;
    s8 action_find = 0;
    s8 filter_num_find = 0;
    s8 rpn_find = 0;
    s8 rpn_str[256];
    int rpn_len = 0;
    s8 filter_name[50];
    s8 name_found = 0;
    u16 len = 0;
    int i;
    int first_time = TRUE;
    char *opstr;
    s8 filter_action[10];
    s32 errors = 0;
    MEF_ENTRY *pMefEntry = (MEF_ENTRY *) buf;
    stack_t stack;
    while ((pos = wlan_config_get_line(line, sizeof(line), fp, ln))) {
        if (strcmp(pos, "}") == 0) {
            break;
        }
        pos1 = strchr(pos, '=');
        if (pos1 == NULL) {
            printf("Line %d: Invalid mef_entry line '%s'\n", *ln, pos);
            errors++;
            continue;
        }
        *pos1++ = '\0';
        if (!mode_find || !action_find || !filter_num_find || !rpn_find) {
            for (i = 0; i < NELEMENTS(mef_entry_fields); i++) {
                if (strncmp
                    (pos, mef_entry_fields[i].name,
                     strlen(mef_entry_fields[i].name)) == 0) {
                    switch (mef_entry_fields[i].nameid) {
                    case NAME_MODE:
                        mode = a2hex_or_atoi(pos1);
                        if (mode & ~0x7) {
                            printf("invalid mode=%d\n", mode);
                            return WLAN_STATUS_FAILURE;
                        }
                        pMefEntry->Mode = mode;
                        mode_find = 1;
                        break;
                    case NAME_ACTION:
                        action = a2hex_or_atoi(pos1);
                        if (action & ~0xff) {
                            printf("invalid action=%d\n", action);
                            return WLAN_STATUS_FAILURE;
                        }
                        pMefEntry->Action = action;
                        action_find = 1;
                        break;
                    case NAME_FILTER_NUM:
                        filter_num = a2hex_or_atoi(pos1);
                        filter_num_find = 1;
                        break;
                    case NAME_RPN:
                        memset(rpn, 0, sizeof(rpn));
                        strcpy(rpn, pos1);
                        rpn_find = 1;
                        break;
                    }
                    break;
                }
            }
            if (i == NELEMENTS(mef_fields)) {
                printf("Line %d: unknown mef_entry field '%s'.\n", *line, pos);
                return WLAN_STATUS_FAILURE;
            }
        }
        if (mode_find && action_find && filter_num_find && rpn_find) {
            for (i = 0; i < filter_num; i++) {
                opstr = getop(rpn, &first_time);
                if (opstr == NULL)
                    break;
                sprintf(filter_name, "%s={", opstr);
                name_found = 0;
                while ((pos = wlan_config_get_line(line, sizeof(line), fp, ln))) {
                    if (strncmp(pos, filter_name, strlen(filter_name)) == 0) {
                        name_found = 1;
                        break;
                    }
                }
                if (!name_found) {
                    fprintf(stderr, "wlanconfig: %s not found in file\n",
                            filter_name);
                    return WLAN_STATUS_FAILURE;
                }
                if (WLAN_STATUS_FAILURE ==
                    wlan_get_filter_data(fp, ln, (u8 *) (rpn_str + rpn_len),
                                         &len))
                    break;
                rpn_len += len;
                if (i > 0) {
                    memcpy(rpn_str + rpn_len, filter_action,
                           strlen(filter_action));
                    rpn_len += strlen(filter_action);
                }
                opstr = getop(rpn, &first_time);
                if (opstr == NULL)
                    break;
                memset(filter_action, 0, sizeof(filter_action));
                sprintf(filter_action, "%s ", opstr);
            }
            /* Remove the last space */
            if (rpn_len > 0) {
                rpn_len--;
                rpn_str[rpn_len] = 0;
            }
            if (WLAN_STATUS_FAILURE == str2bin(rpn_str, &stack)) {
                printf("Fail on str2bin!\n");
                return WLAN_STATUS_FAILURE;
            }
            *size = sizeof(MEF_ENTRY);
            pMefEntry->ExprSize = stack.sp;
            memmove(buf + sizeof(MEF_ENTRY), stack.byte, stack.sp);
            *size += stack.sp;
            break;
        } else if (mode_find && action_find && filter_num_find &&
                   (filter_num == 0)) {
            pMefEntry->ExprSize = 0;
            *size = sizeof(MEF_ENTRY);
            break;
        }
    }
    return WLAN_STATUS_SUCCESS;
}

/**
 *  @brief Process mef cfg 
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *  @return         WLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
process_mef_cfg(int argc, char *argv[])
{
    int ioctl_val, subioctl_val;
    FILE *fp;
    s8 line[256], cmdname[256], *pos;
    int cmdname_found = 0, name_found = 0;
    int ln = 0;
    int ret = WLAN_STATUS_SUCCESS;
    int i;
    u8 *buf;
    u16 buf_len = 0;
    u16 len;
    struct iwreq iwr;
    HostCmd_DS_MEF_CFG *mefcmd;
    if (argc < 4) {
        printf("Error: invalid no of arguments\n");
        printf("Syntax: ./wlanconfig eth1 mefcfg <mef.conf>\n");
        exit(1);
    }
    if (get_priv_ioctl("mefcfg",
                       &ioctl_val, &subioctl_val) == WLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }
    sprintf(cmdname, "%s={", argv[2]);
    cmdname_found = 0;
    if ((fp = fopen(argv[3], "r")) == NULL) {
        fprintf(stderr, "Cannot open file %s\n", argv[4]);
        exit(1);
    }
    buf = (u8 *) malloc(MRVDRV_SIZE_OF_CMD_BUFFER);
    if (buf == NULL) {
        fclose(fp);
        fprintf(stderr, "Cannot alloc memory\n");
        exit(1);
    }
    memset(buf, 0, MRVDRV_SIZE_OF_CMD_BUFFER);
    mefcmd = (HostCmd_DS_MEF_CFG *) buf;
    buf_len = sizeof(HostCmd_DS_MEF_CFG);
    while ((pos = wlan_config_get_line(line, sizeof(line), fp, &ln))) {
        if (strcmp(pos, cmdname) == 0) {
            cmdname_found = 1;
            sprintf(cmdname, "Criteria=");
            name_found = 0;
            while ((pos = wlan_config_get_line(line, sizeof(line), fp, &ln))) {
                if (strncmp(pos, cmdname, strlen(cmdname)) == 0) {
                    name_found = 1;
                    mefcmd->Criteria = a2hex_or_atoi(pos + strlen(cmdname));
                    break;
                }
            }
            if (!name_found) {
                fprintf(stderr, "wlanconfig: criteria not found in file '%s'\n",
                        argv[3]);
                break;
            }
            sprintf(cmdname, "NumEntries=");
            name_found = 0;
            while ((pos = wlan_config_get_line(line, sizeof(line), fp, &ln))) {
                if (strncmp(pos, cmdname, strlen(cmdname)) == 0) {
                    name_found = 1;
                    mefcmd->NumEntries = a2hex_or_atoi(pos + strlen(cmdname));
                    break;
                }
            }
            if (!name_found) {
                fprintf(stderr,
                        "wlanconfig: NumEntries not found in file '%s'\n",
                        argv[3]);
                break;
            }
            for (i = 0; i < mefcmd->NumEntries; i++) {
                sprintf(cmdname, "mef_entry_%d={", i);
                name_found = 0;
                while ((pos =
                        wlan_config_get_line(line, sizeof(line), fp, &ln))) {
                    if (strncmp(pos, cmdname, strlen(cmdname)) == 0) {
                        name_found = 1;
                        break;
                    }
                }
                if (!name_found) {
                    fprintf(stderr, "wlanconfig: %s not found in file '%s'\n",
                            cmdname, argv[3]);
                    break;
                }
                if (WLAN_STATUS_FAILURE ==
                    wlan_get_mef_entry_data(fp, &ln, buf + buf_len, &len)) {
                    ret = WLAN_STATUS_FAILURE;
                    break;
                }
                buf_len += len;
            }
            break;
        }
    }
    fclose(fp);
    /* hexdump("mef_cfg",buf,buf_len, ' '); */
    if (!cmdname_found)
        fprintf(stderr, "wlanconfig: cmdname '%s' not found in file '%s'\n",
                argv[4], argv[3]);

    if (!cmdname_found || !name_found) {
        ret = WLAN_STATUS_FAILURE;
        goto mef_exit;
    }
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.pointer = buf;
    iwr.u.data.length = buf_len;
    iwr.u.data.flags = subioctl_val;

    if (ioctl(sockfd, ioctl_val, &iwr))
        fprintf(stderr, "wlanconfig: set mefcfg failed\n");
  mef_exit:
    if (buf)
        free(buf);
    return ret;

}

#define VITALS_DUMP	1       /**< Dump vitals */
#define TRACE_MEM	2       /**< Memory trace */
/**
 *  @brief Get the fw mem
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *
 *  @return         WLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
process_getfwmem(int argc, char *argv[])
{
    HostCmd_DS_GET_MEM *pGetMem;
    int ioctl_val, subioctl_val;
    u32 StartAddr;
    u16 Len;
    struct iwreq iwr;

    if (get_priv_ioctl("getmem",
                       &ioctl_val, &subioctl_val) == WLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    if (argc != 5) {
        printf("Error: invalid number of arguments\n");
        printf("Syntax: ./wlanconfig ethX getfwmem <StartAddr> <Len>");
        return -EINVAL;
    }
    StartAddr = a2hex_or_atoi(argv[3]);
    Len = (u16) a2hex_or_atoi(argv[4]);
    if ((StartAddr == VITALS_DUMP) || (StartAddr == TRACE_MEM))
        Len = 0;
    if (Len > (2000 - sizeof(HostCmd_DS_GET_MEM)))
        Len = 2000 - sizeof(HostCmd_DS_GET_MEM);
    pGetMem = (HostCmd_DS_GET_MEM *) malloc(2000);
    if (pGetMem == NULL) {
        printf("Error: allocate memory for getfwmem failed\n");
        return -ENOMEM;
    }
    memset(pGetMem, 0, 2000);

    pGetMem->StartAddr = StartAddr;
    pGetMem->Len = Len;
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.pointer = (u8 *) pGetMem;
    iwr.u.data.length = sizeof(HostCmd_DS_GET_MEM);
    iwr.u.data.flags = subioctl_val;

    while (TRUE) {
        if (ioctl(sockfd, ioctl_val, &iwr)) {
            fprintf(stderr, "wlanconfig: getfwmem failed\n");
            free(pGetMem);
            return WLAN_STATUS_FAILURE;
        }
        hexdump("Fw Mem", (u8 *) pGetMem + sizeof(HostCmd_DS_GET_MEM),
                iwr.u.data.length - sizeof(HostCmd_DS_GET_MEM), ' ');
        if (pGetMem->StartAddr == 0)
            break;
    }

    free(pGetMem);
    return WLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Process arpfilter
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *  @return	    WLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
process_arpfilter(int argc, char *argv[])
{
    s8 line[256], *pos;
    u8 *buf;
    FILE *fp;
    struct iwreq iwr;
    int ln = 0;
    int arpfilter_found = 0;
    u16 length = 0;
    int ret = WLAN_STATUS_SUCCESS;
    int ioctl_val, subioctl_val;

    if (get_priv_ioctl("arpfilter",
                       &ioctl_val, &subioctl_val) == WLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    if (argc < 4) {
        printf("Error: invalid no of arguments\n");
        printf("Syntax: ./wlanconfig eth1 arpfilter <arpfilter.conf>\n");
        exit(1);
    }

    if ((fp = fopen(argv[3], "r")) == NULL) {
        fprintf(stderr, "Cannot open file %s\n", argv[3]);
        exit(1);
    }

    buf = (u8 *) malloc(MRVDRV_SIZE_OF_CMD_BUFFER);
    if (buf == NULL) {
        printf("Error: allocate memory for arpfilter failed\n");
        return -ENOMEM;
    }
    memset(buf, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

    arpfilter_found = 0;
    while ((pos = wlan_config_get_line(line, sizeof(line), fp, &ln))) {
        if (strcmp(pos, "arpfilter={") == 0) {
            arpfilter_found = 1;
            wlan_get_hostcmd_data(fp, &ln, buf + length, &length);
            break;
        }
    }

    fclose(fp);

    if (!arpfilter_found) {
        fprintf(stderr, "wlanconfig: 'arpfilter' not found in file '%s'\n",
                argv[3]);
        ret = WLAN_STATUS_FAILURE;
        goto _exit_;
    }

    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.pointer = buf;
    iwr.u.data.length = length;

    iwr.u.data.flags = 0;
    if (ioctl(sockfd, ioctl_val, &iwr)) {
        fprintf(stderr, "wlanconfig: WLANARPFILTER failed\n");
        ret = WLAN_STATUS_FAILURE;
        goto _exit_;
    }

  _exit_:
    if (buf)
        free(buf);

    return ret;
}

/** Config data header length */
#define CFG_DATA_HEADER_LEN	6
/** 
 *  @brief Process configuration data
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *  @return	        WLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
process_cfg_data(int argc, char *argv[])
{
    s32 count = 0;
    u8 *buf = NULL;
    FILE *fp;
    u16 cmdType;
    int status;
    s8 filename[48] = "";
    HostCmd_DS_GEN *hostcmd;
    HostCmd_DS_802_11_CFG_DATA *pstcfgData;
    struct iwreq iwr;
    int ioctl_val, subioctl_val;

    if (get_priv_ioctl("hostcmd",
                       &ioctl_val, &subioctl_val) == WLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    if (argc < 4 || argc > 5) {
        printf("Error: invalid no of arguments\n");
        printf("Syntax: ./wlanconfig eth1 cfg_data <Type> <filename>\n");
        exit(1);
    }

    buf = malloc(MAX_SETGET_CONF_SIZE);
    if (buf == NULL)
        return WLAN_STATUS_FAILURE;

    memset(buf, 0, MAX_SETGET_CONF_SIZE);
    hostcmd = (HostCmd_DS_GEN *) buf;

    hostcmd->Command = wlan_cpu_to_le16(HostCmd_CMD_CFG_DATA);
    pstcfgData = (HostCmd_DS_802_11_CFG_DATA *) (buf + S_DS_GEN);

    if (argc != 4) {

        strncpy(filename, argv[4], MIN(sizeof(filename) - 1, strlen(argv[4])));
        if ((fp = fopen(filename, "r")) == NULL) {
            fprintf(stderr, "Cannot open file %s\n", argv[1]);
            free(buf);
            exit(1);
        }
        count = fparse_for_hex(fp, pstcfgData->u8Data);
        fclose(fp);
    }

    pstcfgData->u16Action =
        (argc == 4) ? HostCmd_ACT_GEN_GET : HostCmd_ACT_GEN_SET;

    status = get_cfg_data_cmdType(argv[3], &cmdType);

    if (status != WLAN_STATUS_SUCCESS) {
        printf("wlanconfig: Argument 3 is invalid\n");
        free(buf);
        return WLAN_STATUS_FAILURE;
    } else {
        pstcfgData->u16Type = cmdType;
    }
    pstcfgData->u16DataLen = count;
    hostcmd->Size =
        wlan_cpu_to_le16(pstcfgData->u16DataLen + S_DS_GEN +
                         CFG_DATA_HEADER_LEN);
    pstcfgData->u16DataLen = wlan_cpu_to_le16(pstcfgData->u16DataLen);
    pstcfgData->u16Type = wlan_cpu_to_le16(pstcfgData->u16Type);
    pstcfgData->u16Action = wlan_cpu_to_le16(pstcfgData->u16Action);

    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.pointer = (u8 *) buf;
    iwr.u.data.length = wlan_le16_to_cpu(hostcmd->Size);

    iwr.u.data.flags = 0;

    if (ioctl(sockfd, ioctl_val, &iwr)) {
        fprintf(stderr, "wlanconfig: cfg data ioctl failed %s\n", dev_name);
        free(buf);
        return WLAN_STATUS_FAILURE;
    }

    pstcfgData->u16DataLen = wlan_le16_to_cpu(pstcfgData->u16DataLen);
    pstcfgData->u16Action = wlan_le16_to_cpu(pstcfgData->u16Action);

    if (pstcfgData->u16Action == HostCmd_ACT_GEN_GET) {
        hexdump("cfg_data", pstcfgData->u8Data, pstcfgData->u16DataLen, ' ');
    }
    free(buf);
    return WLAN_STATUS_SUCCESS;
}

/********************************************************
		Global Functions
********************************************************/
/** 
 *  @brief Entry function for wlanconfig
 *  @param argc		number of arguments
 *  @param argv         A pointer to arguments array    
 *  @return      	WLAN_STATUS_SUCCESS--success, otherwise--fail
 */
int
main(int argc, char *argv[])
{
    s32 cmd;

    if ((argc == 2) && (strcmp(argv[1], "-v") == 0)) {
        fprintf(stdout, "Marvell wlanconfig version %s\n", WLANCONFIG_VER);
        exit(0);
    }
    if (argc < 3) {
        fprintf(stderr, "Invalid number of parameters!\n");
        display_usage();
        exit(1);
    }

    strncpy(dev_name, argv[1], IFNAMSIZ);

    /* 
     * create a socket 
     */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "wlanconfig: Cannot open socket.\n");
        exit(1);
    }
    if (get_range() < 0) {
        fprintf(stderr, "wlanconfig: Cannot get range.\n");
        exit(1);
    }
    switch ((cmd = findcommand(NELEMENTS(commands), commands, argv[2]))) {
    case CMD_MEFCFG:
        process_mef_cfg(argc, argv);
        break;
    case CMD_HOSTCMD:
        process_host_cmd(argc, argv);
        break;
    case CMD_RDMAC:
    case CMD_RDBBP:
    case CMD_RDRF:
    case CMD_RDPMIC:
        if (argc < 4) {
            fprintf(stderr, "Register offset required!\n");
            display_usage();
            exit(1);
        }

        if (process_read_register(cmd, argv[3])) {
            fprintf(stderr, "Read command failed!\n");
            exit(1);
        }
        break;
    case CMD_WRMAC:
    case CMD_WRBBP:
    case CMD_WRRF:
    case CMD_WRPMIC:
        if (argc < 5) {
            fprintf(stderr, "Register offset required & value!\n");
            display_usage();
            exit(1);
        }
        if (process_write_register(cmd, argv[3], argv[4])) {
            fprintf(stderr, "Write command failed!\n");
            exit(1);
        }
        break;
    case CMD_BG_SCAN_CONFIG:
        process_bg_scan_config(argc, argv);
        break;
    case CMD_BG_SCAN_CFG:
        process_bgscfg(argc, argv);
        break;
    case CMD_WMM_QSTATUS:
        process_wmm_qstatus(argc, argv);
        break;
    case CMD_ADDTS:
        if (process_addts(argc, argv)) {
            exit(1);
        }
        break;
    case CMD_DELTS:
        if (process_delts(argc, argv)) {
            exit(1);
        }
        break;
    case CMD_QCONFIG:
        if (process_qconfig(argc, argv)) {
            exit(1);
        }
        break;
    case CMD_QSTATS:
        if (process_qstats(argc, argv)) {
            exit(1);
        }
        break;
    case CMD_TS_STATUS:
        if (process_wmm_ts_status(argc, argv)) {
            exit(1);
        }
        break;
    case CMD_TX_PKT_STATS:
        process_txpktstats(argc, argv);
        break;
    case CMD_RDEEPROM:
        printf("process read eeprom\n");

        if (argc < 5) {
            fprintf(stderr, "Register offset, number of bytes required\n");
            display_usage();
            exit(1);
        }

        if (process_read_eeprom(argv[3], argv[4])) {
            fprintf(stderr, "EEPROM Read failed\n");
            display_usage();
            exit(1);
        }
        break;
    case CMD_SLEEPPARAMS:
        if (process_sleep_params(argc, argv)) {
            fprintf(stderr, "Sleep Params Failed\n");
            display_usage();
            exit(1);
        }
        break;
    case CMD_BCA_TS:
        if (process_bca_ts(argc, argv)) {
            fprintf(stderr, "SetBcaTs Failed\n");
            display_usage();
            exit(1);
        }
        break;
    case CMD_EXTSCAN:
        if (process_extscan(argc, argv)) {
            fprintf(stderr, "ExtScan Failed\n");
            display_usage();
            exit(1);
        }
        break;
    case CMD_SCAN_LIST:
        if (process_scanlist(argc, argv)) {
            fprintf(stderr, "getscanlist Failed\n");
            display_usage();
            exit(1);
        }
        break;
    case CMD_CRYPTO_TEST:
        if (argc < 8) {
            fprintf(stderr, "Invalid number of parameters!\n");
            display_usage();
            exit(1);
        }
        if (process_crypto_test(argc, argv)) {
            fprintf(stderr, "crypto_test Failed\n");
            display_usage();
            exit(1);
        }
        break;
    case CMD_SET_GEN_IE:
        if (process_setgenie()) {
            exit(1);
        }
        break;
    case CMD_GET_SCAN_RSP:
        if (process_getscantable(argc, argv)) {
            exit(1);
        }
        break;

    case CMD_SET_USER_SCAN:
        if (process_setuserscan(argc, argv)) {
            exit(1);
        }
        break;

    case CMD_SET_MRVL_TLV:
        if (process_setmrvltlv()) {
            exit(1);
        }
        break;

    case CMD_GET_ASSOC_RSP:
        if (process_getassocrsp()) {
            exit(1);
        }
        break;
    case CMD_HS_TEST:
        if (process_hs_test()) {
            exit(1);
        }
        break;
    case CMD_GET_TSF:
        if (process_gettsf()) {
            exit(1);
        }
        break;
    case CMD_GET_CFP_TABLE:
        if (process_getcfptable(argc, argv)) {
            exit(1);
        }
        break;
    case CMD_GET_FW_MEM:
        if (process_getfwmem(argc, argv)) {
            exit(1);
        }
        break;
    case CMD_ARPFILTER:
        process_arpfilter(argc, argv);
        break;
    case CMD_CFG_DATA:
        if (process_cfg_data(argc, argv)) {
            exit(1);
        }
        break;
    default:
        fprintf(stderr, "Invalid command specified!\n");
        display_usage();
        exit(1);
    }

    return WLAN_STATUS_SUCCESS;
}
