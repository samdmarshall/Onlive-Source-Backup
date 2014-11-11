/** @file  mlanconfig.c
  * @brief Program to configure addition parameters into the mlandriver
  *
  * Copyright (C) 2008, Marvell International Ltd.
  *
  * This software file (the "File") is distributed by Marvell International
  * Ltd. under the terms of the GNU General Public License Version 2, June 1991
  * (the "License").  You may use, redistribute and/or modify this File in
  * accordance with the terms and conditions of the License, a copy of which
  * is available by writing to the Free Software Foundation, Inc., 59 Temple
  * Place, Suite 330, Boston, MA 02111-1307 or on the worldwide web at
  * http://www.gnu.org/licenses/gpl.txt.
  *
  * THE FILE IS DISTRIBUTED AS-IS, WITHOUT WARRANTY OF ANY KIND, AND THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE
  * ARE EXPRESSLY DISCLAIMED.  The License provides additional details about
  * this warranty disclaimer.
  *
  */
/************************************************************************
Change log:
     11/26/2008: initial version
************************************************************************/
#include    <stdio.h>
#include    <unistd.h>
#include    <sys/socket.h>
#include    <string.h>
#include    <stdlib.h>
#include    <sys/ioctl.h>
#include    <errno.h>
#include    <linux/if.h>
#include    <linux/wireless.h>
#include	"mlanconfig.h"
#include	"mlanhostcmd.h"

/** mlanconfig version number */
#define MLANCONFIG_VER "1.0"
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
};

static t_s8 *commands[] = {
    "hostcmd",
    "arpfilter",
};

static t_s8 *usage[] = {
    "Usage: ",
    "   	mlanconfig -v  (version)",
    "	mlanconfig <mlanX> <cmd> [...]",
    "	where",
    "	mlanX	: wireless network interface",
    "	cmd	: hostcmd",
    "       : arpfilter",
    "		: additional parameter for hostcmd",
    "		: 	<filename> <cmd>",
    "       : additional parameter for arpfilter",
    "       :   <filename>",
};

static FILE *fp; /**< socket */
static t_s32 sockfd;  /**< socket */
static t_s8 dev_name[IFNAMSIZ + 1];     /**< device name */
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
    struct iw_priv_args *pPriv = NULL;
    struct iw_priv_args *newPriv;
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
        newPriv = realloc(pPriv, size * sizeof(pPriv[0]));
        if (newPriv == NULL) {
            printf("Error: Buffer allocation failed\n");
            ret = MLAN_STATUS_FAILURE;
            break;
        }
        pPriv = newPriv;

        iwr.u.data.pointer = (caddr_t) pPriv;
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
            priv_args = pPriv;
            ret = iwr.u.data.length;
            break;
        }
    } while (size <= IW_MAX_PRIV_NUM);

    if ((ret == MLAN_STATUS_FAILURE) && (pPriv))
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
static int
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
    size_t buflen;

    buflen = sizeof(struct iw_range) + 500;
    range = malloc(buflen);
    if (range == NULL) {
        printf("Error: allocate memory for iw_range failed\n");
        return -ENOMEM;
    }
    memset(range, 0, buflen);
    memset(&iwr, 0, sizeof(struct iwreq));
    iwr.u.data.pointer = (caddr_t) range;
    iwr.u.data.length = buflen;
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
    default:
        fprintf(stderr, "Invalid command specified!\n");
        display_usage();
        exit(1);
    }

    return MLAN_STATUS_SUCCESS;
}
