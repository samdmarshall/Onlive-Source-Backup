/** @file  mlanconfig.c
  *
  * @brief Program to configure addition parameters into the mlandriver
  * 
  *  Usage: mlanconfig mlanX cmd [...] 
  *
  *  Copyright (C) 2008-2009, Marvell International Ltd.
  *  All Rights Reserved
  */
/************************************************************************
Change log:
     11/26/2008: initial version
     3/10/2009:  add setuserscan, getscantable etc. commands
************************************************************************/

#include    "mlanconfig.h"
#include    "mlanhostcmd.h"
#include    "mlanmisc.h"

/** mlanconfig version number */
#define MLANCONFIG_VER "M1.1"

/** Initial number of total private ioctl calls */
#define IW_INIT_PRIV_NUM    128
/** Maximum number of total private ioctl calls supported */
#define IW_MAX_PRIV_NUM     1024

/********************************************************
		Local Variables
********************************************************/

/** Private ioctl commands */
enum COMMANDS
{
    CMD_HOSTCMD,
    CMD_ARPFILTER,
    CMD_CFG_DATA,
    CMD_CMD52RW,
    CMD_CMD53RW,
    CMD_GET_SCAN_RSP,
    CMD_SET_USER_SCAN,
    CMD_ADD_TS,
    CMD_DEL_TS,
    CMD_QCONFIG,
    CMD_QSTATS,
    CMD_TS_STATUS,
    CMD_WMM_QSTATUS,
};

static t_s8 *commands[] = {
    "hostcmd",
    "arpfilter",
    "cfgdata",
    "sdcmd52rw",
    "sdcmd53rw",
    "getscantable",
    "setuserscan",
    "addts",
    "delts",
    "qconfig",
    "qstats",
    "ts_status",
    "qstatus",
};

static t_s8 *usage[] = {
    "Usage: ",
    "	mlanconfig -v  (version)",
    "	mlanconfig <mlanX> <cmd> [...]",
    "	where",
    "	mlanX : wireless network interface",
    "	cmd : hostcmd",
    "       : arpfilter",
    "       : cfgdata",
    "       : sdcmd52rw, sdcmd53rw",
    "       : getscantable, setuserscan",
    "       : addts, delts, qconfig, qstats, ts_status, qstatus",
    "       : additional parameter for hostcmd",
    "       :   <filename> <cmd>",
    "       : additional parameter for arpfilter",
    "       :   <filename>",
    "       : additional parameter for cfgdata",
    "       :   <type> <filename>",
    "       : additional parameter for sdcmd52rw",
    "       :   <function> <reg addr.> <data>",
    "       : additional parameter for sdcmd53rw",
    "       :   <func> <addr> <mode> <blksiz> <blknum> <data1> ... ...<dataN> ",
};

static FILE *fp; /**< socket */
t_s32 sockfd;  /**< socket */
t_s8 dev_name[IFNAMSIZ + 1];    /**< device name */
static struct iw_priv_args *priv_args = NULL;      /**< private args */
static int we_version_compiled = 0;
                                  /**< version compiled */

/********************************************************
		Global Variables
********************************************************/

/********************************************************
		Local Functions
********************************************************/
/** 
 *  @brief Get private info.
 *   
 *  @param ifname   A pointer to net name
 *  @return 	    MLAN_STATUS_SUCCESS--success, otherwise --fail
 */
static int
get_private_info(const t_s8 * ifname)
{
    /* This function sends the SIOCGIWPRIV command, which is handled by the
       kernel and gets the total number of private ioctl's available in the
       host driver. */
    struct iwreq iwr;
    int s, ret = MLAN_STATUS_SUCCESS;
    struct iw_priv_args *ppriv = NULL;
    struct iw_priv_args *new_priv;
    int result = 0;
    size_t size = IW_INIT_PRIV_NUM;

    s = socket(PF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
        perror("socket[PF_INET,SOCK_DGRAM]");
        return MLAN_STATUS_FAILURE;
    }

    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, ifname, IFNAMSIZ);

    do {
        /* (Re)allocate the buffer */
        new_priv = realloc(ppriv, size * sizeof(ppriv[0]));
        if (new_priv == NULL) {
            printf("Error: Buffer allocation failed\n");
            ret = MLAN_STATUS_FAILURE;
            break;
        }
        ppriv = new_priv;

        iwr.u.data.pointer = (caddr_t) ppriv;
        iwr.u.data.length = size;
        iwr.u.data.flags = 0;

        if (ioctl(s, SIOCGIWPRIV, &iwr) < 0) {
            result = errno;
            ret = MLAN_STATUS_FAILURE;
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
            priv_args = ppriv;
            ret = iwr.u.data.length;
            break;
        }
    } while (size <= IW_MAX_PRIV_NUM);

    if ((ret == MLAN_STATUS_FAILURE) && (ppriv))
        free(ppriv);

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
 *  @return 	        MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static int
marvell_get_subioctl_no(t_s32 i,
                        t_s32 priv_cnt, int *ioctl_val, int *subioctl_val)
{
    t_s32 j;

    if (priv_args[i].cmd >= SIOCDEVPRIVATE) {
        *ioctl_val = priv_args[i].cmd;
        *subioctl_val = 0;
        return MLAN_STATUS_SUCCESS;
    }

    j = -1;

    /* Find the matching *real* ioctl */

    while ((++j < priv_cnt)
           && ((priv_args[j].name[0] != '\0') ||
               (priv_args[j].set_args != priv_args[i].set_args) ||
               (priv_args[j].get_args != priv_args[i].get_args))) {
    }

    /* If not found... */
    if (j == priv_cnt) {
        printf("%s: Invalid private ioctl definition for: 0x%x\n",
               dev_name, priv_args[i].cmd);
        return MLAN_STATUS_FAILURE;
    }

    /* Save ioctl numbers */
    *ioctl_val = priv_args[j].cmd;
    *subioctl_val = priv_args[i].cmd;

    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Get ioctl number
 *   
 *  @param ifname   	A pointer to net name
 *  @param priv_cmd	A pointer to priv command buffer
 *  @param ioctl_val    A pointer to return ioctl number
 *  @param subioctl_val A pointer to return sub-ioctl number
 *  @return 	        MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static int
marvell_get_ioctl_no(const t_s8 * ifname,
                     const t_s8 * priv_cmd, int *ioctl_val, int *subioctl_val)
{
    t_s32 i;
    t_s32 priv_cnt;
    int ret = MLAN_STATUS_FAILURE;

    priv_cnt = get_private_info(ifname);

    /* Are there any private ioctls? */
    if (priv_cnt <= 0 || priv_cnt > IW_MAX_PRIV_NUM) {
        /* Could skip this message ? */
        printf("%-8.8s  no private ioctls.\n", ifname);
    } else {
        for (i = 0; i < priv_cnt; i++) {
            if (priv_args[i].name[0] && !strcmp(priv_args[i].name, priv_cmd)) {
                ret = marvell_get_subioctl_no(i, priv_cnt,
                                              ioctl_val, subioctl_val);
                break;
            }
        }
    }

    if (priv_args) {
        free(priv_args);
        priv_args = NULL;
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
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
int
get_priv_ioctl(char *ioctl_name, int *ioctl_val, int *subioctl_val)
{
    int retval;

    retval = marvell_get_ioctl_no(dev_name,
                                  ioctl_name, ioctl_val, subioctl_val);

    return retval;
}

/** 
 *  @brief Process host_cmd 
 *  @param argc		number of arguments
 *  @param argv		A pointer to arguments array    
 *  @return      	MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
process_host_cmd(int argc, char *argv[])
{
    t_s8 cmdname[256];
    t_u8 *buf;
    HostCmd_DS_GEN *hostcmd;
    struct iwreq iwr;
    int ret = MLAN_STATUS_SUCCESS;
    int ioctl_val, subioctl_val;

    if (get_priv_ioctl("hostcmd",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    if (argc < 5) {
        printf("Error: invalid no of arguments\n");
        printf("Syntax: ./mlanconfig mlanX hostcmd <hostcmd.conf> <cmdname>\n");
        exit(1);
    }

    if ((fp = fopen(argv[3], "r")) == NULL) {
        fprintf(stderr, "Cannot open file %s\n", argv[3]);
        exit(1);
    }

    buf = (t_u8 *) malloc(MRVDRV_SIZE_OF_CMD_BUFFER);
    if (buf == NULL) {
        printf("Error: allocate memory for hostcmd failed\n");
        fclose(fp);
        return -ENOMEM;
    }
    sprintf(cmdname, "%s", argv[4]);
    ret = prepare_host_cmd_buffer(cmdname, buf);
    fclose(fp);

    if (ret == MLAN_STATUS_FAILURE)
        goto _exit_;

    hostcmd = (HostCmd_DS_GEN *) buf;
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.pointer = (t_u8 *) hostcmd;
    iwr.u.data.length = le16_to_cpu(hostcmd->size);

    iwr.u.data.flags = 0;
    if (ioctl(sockfd, ioctl_val, &iwr)) {
        fprintf(stderr, "mlanconfig: MLANHOSTCMD is not supported by %s\n",
                dev_name);
        ret = MLAN_STATUS_FAILURE;
        goto _exit_;
    }
    ret = process_host_cmd_resp(buf);

  _exit_:
    if (buf)
        free(buf);

    return ret;
}

/** 
 *  @brief  get range 
 *   
 *  @return	MLAN_STATUS_SUCCESS--success, otherwise --fail
 */
static int
get_range(t_void)
{
    struct iw_range *range;
    struct iwreq iwr;
    size_t buf_len;

    buf_len = sizeof(struct iw_range) + 500;
    range = malloc(buf_len);
    if (range == NULL) {
        printf("Error: allocate memory for iw_range failed\n");
        return -ENOMEM;
    }
    memset(range, 0, buf_len);
    memset(&iwr, 0, sizeof(struct iwreq));
    iwr.u.data.pointer = (caddr_t) range;
    iwr.u.data.length = buf_len;
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);

    if ((ioctl(sockfd, SIOCGIWRANGE, &iwr)) < 0) {
        printf("Get Range Results Failed\n");
        free(range);
        return MLAN_STATUS_FAILURE;
    }
    we_version_compiled = range->we_version_compiled;
    printf("Driver build with Wireless Extension %d\n",
           range->we_version_compiled);
    free(range);
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Display usage
 *  
 *  @return       NA
 */
static t_void
display_usage(t_void)
{
    t_s32 i;
    for (i = 0; i < NELEMENTS(usage); i++)
        fprintf(stderr, "%s\n", usage[i]);
}

/** 
 *  @brief Find command
 *  
 *  @param maxcmds  max command number
 *  @param cmds     A pointer to commands buffer
 *  @param cmd      A pointer to command buffer
 *  @return         index of command or MLAN_STATUS_FAILURE
 */
static int
findcommand(t_s32 maxcmds, t_s8 * cmds[], t_s8 * cmd)
{
    t_s32 i;

    for (i = 0; i < maxcmds; i++) {
        if (!strcasecmp(cmds[i], cmd)) {
            return i;
        }
    }

    return MLAN_STATUS_FAILURE;
}

/** 
 *  @brief Process arpfilter
 *  @param argc   number of arguments
 *  @param argv   A pointer to arguments array    
 *  @return     MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
process_arpfilter(int argc, char *argv[])
{
    t_u8 *buf;
    struct iwreq iwr;
    t_u16 length = 0;
    int ret = MLAN_STATUS_SUCCESS;
    int ioctl_val, subioctl_val;

    if (get_priv_ioctl("arpfilter",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    if (argc < 4) {
        printf("Error: invalid no of arguments\n");
        printf("Syntax: ./mlanconfig mlanX arpfilter <arpfilter.conf>\n");
        exit(1);
    }

    if ((fp = fopen(argv[3], "r")) == NULL) {
        fprintf(stderr, "Cannot open file %s\n", argv[3]);
        return MLAN_STATUS_FAILURE;
    }
    buf = (t_u8 *) malloc(MRVDRV_SIZE_OF_CMD_BUFFER);
    if (buf == NULL) {
        printf("Error: allocate memory for arpfilter failed\n");
        fclose(fp);
        return -ENOMEM;
    }
    ret = prepare_arp_filter_buffer(buf, &length);
    fclose(fp);

    if (ret == MLAN_STATUS_FAILURE)
        goto _exit_;

    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.pointer = buf;
    iwr.u.data.length = length;
    iwr.u.data.flags = 0;
    if (ioctl(sockfd, ioctl_val, &iwr)) {
        fprintf(stderr,
                "mlanconfig: arpfilter command is not supported by %s\n",
                dev_name);
        ret = MLAN_STATUS_FAILURE;
        goto _exit_;
    }

  _exit_:
    if (buf)
        free(buf);

    return ret;
}

/** 
 *  @brief Process cfgdata 
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
process_cfg_data(int argc, char *argv[])
{
    t_u8 *buf;
    HostCmd_DS_GEN *hostcmd;
    struct iwreq iwr;
    int ret = MLAN_STATUS_SUCCESS;
    int ioctl_val, subioctl_val;

    if (get_priv_ioctl("hostcmd",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    if (argc < 4 || argc > 5) {
        printf("Error: invalid no of arguments\n");
        printf
            ("Syntax: ./mlanconfig mlanX cfgdata <register type> <filename>\n");
        exit(1);
    }

    if (argc == 5) {
        if ((fp = fopen(argv[4], "r")) == NULL) {
            fprintf(stderr, "Cannot open file %s\n", argv[3]);
            exit(1);
        }
    }
    buf = (t_u8 *) malloc(MRVDRV_SIZE_OF_CMD_BUFFER);
    if (buf == NULL) {
        printf("Error: allocate memory for hostcmd failed\n");
        fclose(fp);
        return -ENOMEM;
    }
    ret = prepare_cfg_data_buffer(argc, argv, buf);
    if (argc == 5)
        fclose(fp);

    if (ret == MLAN_STATUS_FAILURE)
        goto _exit_;

    hostcmd = (HostCmd_DS_GEN *) buf;
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.pointer = (t_u8 *) hostcmd;
    iwr.u.data.length = le16_to_cpu(hostcmd->size);

    iwr.u.data.flags = 0;
    if (ioctl(sockfd, ioctl_val, &iwr)) {
        fprintf(stderr, "mlanconfig: MLANHOSTCMD is not supported by %s\n",
                dev_name);
        ret = MLAN_STATUS_FAILURE;
        goto _exit_;
    }
    ret = process_host_cmd_resp(buf);

  _exit_:
    if (buf)
        free(buf);

    return ret;
}

/** 
 *  @brief read current command
 *  @param ptr      A pointer to data
 *  @param curCmd   A pointer to the buf which will hold current command    
 *  @return         NULL or the pointer to the left command buf
 */
static t_s8 *
readCurCmd(t_s8 * ptr, t_s8 * curCmd)
{
    t_s32 i = 0;
#define MAX_CMD_SIZE 64 /**< Max command size */

    while (*ptr != ']' && i < (MAX_CMD_SIZE - 1))
        curCmd[i++] = *(++ptr);

    if (*ptr != ']')
        return NULL;

    curCmd[i - 1] = '\0';

    return ++ptr;
}

/** 
 *  @brief parse command and hex data
 *  @param fp       A pointer to FILE stream
 *  @param dst      A pointer to the dest buf
 *  @param cmd      A pointer to command buf for search
 *  @return         Length of hex data or MLAN_STATUS_FAILURE  		
 */
static int
fparse_for_cmd_and_hex(FILE * fp, t_u8 * dst, t_u8 * cmd)
{
    t_s8 *ptr;
    t_u8 *dptr;
    t_s8 buf[256], curCmd[64];
    t_s32 isCurCmd = 0;

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
                    return MLAN_STATUS_FAILURE;

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

    return MLAN_STATUS_FAILURE;
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
 *  mlanconfig mlanX addts <filename.conf> <section# of tspec> <timeout in ms>
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
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
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
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

    ieBytes =
        fparse_for_cmd_and_hex(fp, addtsReq.tspecData, (t_u8 *) config_id);

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
        perror("mlanconfig: addts ioctl");
        return -EFAULT;
    }

    ieBytes = iwr.u.data.length - (sizeof(addtsReq.timeout_ms)
                                   + sizeof(addtsReq.commandResult)
                                   + sizeof(addtsReq.ieeeStatusCode));
    printf("Cmd Output:\n");
    printf("ADDTS Command Result = %d\n", addtsReq.commandResult);
    printf("ADDTS IEEE Status    = %d\n", addtsReq.ieeeStatusCode);
    hexdump(config_id, addtsReq.tspecData, ieBytes, ' ');

    return MLAN_STATUS_SUCCESS;
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
 *  mlanconfig mlanX delts <filename.conf> <section# of tspec>
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
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
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
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

    ieBytes =
        fparse_for_cmd_and_hex(fp, deltsReq.tspecData, (t_u8 *) config_id);

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
        perror("mlanconfig: delts ioctl");
        return -EFAULT;
    }

    printf("Cmd Output:\n");
    printf("DELTS Command Result = %d\n", deltsReq.commandResult);

    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief Send a WMM AC Queue configuration command to get/set/default params
 *
 *  Configure or get the parameters of a WMM AC queue. The command takes
 *    an optional Queue Id as a last parameter.  Without the queue id, all
 *    queues will be acted upon.
 *  
 *  mlanconfig mlanX qconfig set msdu <lifetime in TUs> [Queue Id: 0-3]
 *  mlanconfig mlanX qconfig get [Queue Id: 0-3]
 *  mlanconfig mlanX qconfig def [Queue Id: 0-3]
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
process_qconfig(int argc, char *argv[])
{
    int ioctl_val, subioctl_val;
    struct iwreq iwr;
    wlan_ioctl_wmm_queue_config_t queue_config_cmd;
    mlan_wmm_ac_e ac_idx;
    mlan_wmm_ac_e ac_idx_start;
    mlan_wmm_ac_e ac_idx_stop;

    const char *ac_str_tbl[] = { "BK", "BE", "VI", "VO" };

    if (argc < 4) {
        fprintf(stderr, "Invalid number of parameters!\n");
        return -EINVAL;
    }

    if (get_priv_ioctl("qconfig",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
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

    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief Turn on/off or retrieve and clear the queue statistics for an AC
 *
 *  Turn the queue statistics collection on/off for a given AC or retrieve the
 *    current accumulated stats and clear them from the firmware.  The command
 *    takes an optional Queue Id as a last parameter.  Without the queue id,
 *    all queues will be acted upon.
 *
 *  mlanconfig mlanX qstats on [Queue Id: 0-3]
 *  mlanconfig mlanX qstats off [Queue Id: 0-3]
 *  mlanconfig mlanX qstats get [Queue Id: 0-3]
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
process_qstats(int argc, char *argv[])
{
    int ioctl_val, subioctl_val;
    struct iwreq iwr;
    wlan_ioctl_wmm_queue_stats_t queue_stats_cmd;
    mlan_wmm_ac_e ac_idx;
    mlan_wmm_ac_e ac_idx_start;
    mlan_wmm_ac_e ac_idx_stop;
    t_u16 usedTime[MAX_AC_QUEUES];
    t_u16 policedTime[MAX_AC_QUEUES];

    const char *ac_str_tbl[] = { "BK", "BE", "VI", "VO" };

    if (argc < 3) {
        fprintf(stderr, "Invalid number of parameters!\n");
        return -EINVAL;
    }

    if (get_priv_ioctl("qstats",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
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
        /* If the user types: "mlanconfig eth1 qstats" without get argument.
           The mlanconfig application invokes "get" option for all queues */
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

    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Get the current status of the WMM Traffic Streams
 *  
 *  Command: mlanconfig eth1 ts_status
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
process_wmm_ts_status(int argc, char *argv[])
{
    int ioctl_val, subioctl_val;
    struct iwreq iwr;
    wlan_ioctl_wmm_ts_status_t ts_status;
    int tid;

    const char *ac_str_tbl[] = { "BK", "BE", "VI", "VO" };

    if (get_priv_ioctl("ts_status",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
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
            perror("mlanconfig: ts_status ioctl");
            return -EFAULT;
        }

        printf(" %02d     %3s    %2s    %u     %c    ",
               ts_status.tid,
               (ts_status.valid ? "Yes" : "No"),
               (ts_status.valid ? ac_str_tbl[ts_status.accessCategory] : "--"),
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

    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Get the current status of the WMM Queues
 *  
 *  Command: mlanconfig eth1 qstatus
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
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
process_wmm_qstatus(int argc, char *argv[])
{
    int ioctl_val, subioctl_val;
    struct iwreq iwr;
    wlan_ioctl_wmm_queue_status_t qstatus;
    mlan_wmm_ac_e acVal;

    if (get_priv_ioctl("qstatus",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    memset(&qstatus, 0x00, sizeof(qstatus));

    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.flags = subioctl_val;
    iwr.u.data.pointer = (caddr_t) & qstatus;
    iwr.u.data.length = (sizeof(qstatus));

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("mlanconfig: qstatus ioctl");
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

        printf("ACM[%c], FlowReq[%c], FlowCreated[%c], Enabled[%c],"
               " DE[%c], TE[%c]\n",
               (qstatus.acStatus[acVal].wmmAcm ? 'X' : ' '),
               (qstatus.acStatus[acVal].flowRequired ? 'X' : ' '),
               (qstatus.acStatus[acVal].flowCreated ? 'X' : ' '),
               (qstatus.acStatus[acVal].disabled ? ' ' : 'X'),
               (qstatus.acStatus[acVal].deliveryEnabled ? 'X' : ' '),
               (qstatus.acStatus[acVal].triggerEnabled ? 'X' : ' '));
    }

    return MLAN_STATUS_SUCCESS;
}

/********************************************************
		Global Functions
********************************************************/
/** 
 *  @brief Get one line from the File
 *  
 *  @param s	    Storage location for data.
 *  @param size 	Maximum number of characters to read. 
 *  @param line		A pointer to return current line number
 *  @return         returns string or NULL 
 */
t_s8 *
mlan_config_get_line(t_s8 * s, t_s32 size, int *line)
{
    t_s8 *pos, *end, *sstart;

    while (fgets(s, size, fp)) {
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
 *  @brief parse hex data
 *  @param dst      A pointer to receive hex data
 *  @return         length of hex data
 */
int
fparse_for_hex(t_u8 * dst)
{
    t_s8 *ptr;
    t_u8 *dptr;
    t_s8 buf[256];

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
 *  @brief Entry function for mlanconfig
 *  @param argc		number of arguments
 *  @param argv     A pointer to arguments array    
 *  @return      	MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
int
main(int argc, char *argv[])
{
    t_s32 cmd;

    if ((argc == 2) && (strcmp(argv[1], "-v") == 0)) {
        fprintf(stdout, "Marvell mlanconfig version %s\n", MLANCONFIG_VER);
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
        fprintf(stderr, "mlanconfig: Cannot open socket.\n");
        exit(1);
    }
    if (get_range() < 0) {
        fprintf(stderr, "mlanconfig: Cannot get range.\n");
        exit(1);
    }
    switch ((cmd = findcommand(NELEMENTS(commands), commands, argv[2]))) {
    case CMD_HOSTCMD:
        process_host_cmd(argc, argv);
        break;
    case CMD_ARPFILTER:
        process_arpfilter(argc, argv);
        break;
    case CMD_CFG_DATA:
        process_cfg_data(argc, argv);
        break;
    case CMD_CMD52RW:
        process_sdcmd52rw(argc, argv);
        break;
    case CMD_CMD53RW:
        process_sdcmd53rw(argc, argv);
        break;
    case CMD_GET_SCAN_RSP:
        process_getscantable(argc, argv);
        break;
    case CMD_SET_USER_SCAN:
        process_setuserscan(argc, argv);
        break;
    case CMD_ADD_TS:
        process_addts(argc, argv);
        break;
    case CMD_DEL_TS:
        process_delts(argc, argv);
        break;
    case CMD_QCONFIG:
        process_qconfig(argc, argv);
        break;
    case CMD_QSTATS:
        process_qstats(argc, argv);
        break;
    case CMD_TS_STATUS:
        process_wmm_ts_status(argc, argv);
        break;
    case CMD_WMM_QSTATUS:
        process_wmm_qstatus(argc, argv);
        break;

    default:
        fprintf(stderr, "Invalid command specified!\n");
        display_usage();
        exit(1);
    }

    return MLAN_STATUS_SUCCESS;
}
