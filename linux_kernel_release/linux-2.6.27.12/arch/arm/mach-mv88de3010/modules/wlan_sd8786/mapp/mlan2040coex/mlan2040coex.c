/** @file  mlan2040coex.c
  *
  * @brief 11n 20/40 MHz Coex application
  * 
  *  Usage:  
  *
  *  Copyright (C) 2009, Marvell International Ltd.
  *  All Rights Reserved
  */
/************************************************************************
Change log:
     06/24/2009: initial version
************************************************************************/

#include    <stdio.h>
#include    <ctype.h>
#include    <unistd.h>
#include    <string.h>
#include    <signal.h>
#include    <fcntl.h>
#include    <stdlib.h>
#include    <errno.h>
#include    <sys/socket.h>
#include    <sys/ioctl.h>
#include    <linux/if.h>
#include    <linux/wireless.h>
#include    <linux/netlink.h>
#include    <linux/rtnetlink.h>
#include    <net/ethernet.h>
#include    "mlan2040coex.h"
#include    "mlan2040misc.h"

/** coex application's version number */
#define COEX_VER "M1.0"

/** Initial number of total private ioctl calls */
#define IW_INIT_PRIV_NUM    128
/** Maximum number of total private ioctl calls supported */
#define IW_MAX_PRIV_NUM     1024

/********************************************************
		Local Variables
********************************************************/

static t_s8 *usage[] = {
    "Usage: ",
    "	mlan2040coex [-i <intfname>] [-hvB] ",
    "	-h = help",
    "	-v = version",
    "	-B = run the process in background.",
    "	(if intfname not present then mlan0 assumed)"
};

t_s32 sockfd = 0;  /**< socket descriptor */
t_s32 nl_sk = 0;  /**< netlink socket descriptor */
t_s8 dev_name[IFNAMSIZ + 1];   /**< device name */
static struct iw_priv_args *priv_args = NULL; /**< private args */
static int we_version_compiled = 0;
                                  /**< version compiled */
/** Flag: is 2040coex command required */
int coex_cmd_req_flag = FALSE;
/** Flag: is associated */
int assoc_flag = FALSE;
/** Flag: is HT AP */
int is_ht_ap = FALSE;
/** terminate flag */
int terminate_flag = FALSE;
/********************************************************
		Global Variables
********************************************************/
/** OBSS scan parameter of associated AP */
OBSSScanParam_t cur_obss_scan_param;

/********************************************************
		Local Functions
********************************************************/
/** 
 *  @brief Get private info.
 *   
 *  @param ifname   A pointer to net name
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise --fail
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

        if (ioctl(s, SIOCGIWPRIV, &iwr)) {
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
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
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
 *  @param ifname       A pointer to net name
 *  @param priv_cmd     A pointer to priv command buffer
 *  @param ioctl_val    A pointer to return ioctl number
 *  @param subioctl_val A pointer to return sub-ioctl number
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
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

    retval =
        marvell_get_ioctl_no(dev_name, ioctl_name, ioctl_val, subioctl_val);
    return retval;
}

/** 
 *  @brief Process OBSS scan table
 *  @return     MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
int
process_scantable(void)
{
    int ioctl_val, subioctl_val;
    struct iwreq iwr;
    t_u8 *scan_rsp_buf = NULL;
    char ssid[MRVDRV_MAX_SSID_LENGTH + 1];
    unsigned int scan_start;
    int idx, i = 0, j, already_listed, ssid_len, ssid_idx;

    t_u8 *pcurrent;
    t_u8 *pnext;
    IEEEtypes_ElementId_e *pelement_id;
    t_u8 *pelement_len, ht_cap_present, intol_bit_is_set;
    int bss_info_len, ret = MLAN_STATUS_SUCCESS;

    IEEEtypes_CapInfo_t cap_info = { 0 };
    t_u8 tsf[8];
    t_u16 beacon_interval;
    IEEEtypes_HTCap_t *pht_cap;
    int scan_result_found;

    wlan_ioctl_get_scan_table_info *prsp_info;
    wlan_ioctl_get_scan_table_entry *prsp_entry;

    scan_rsp_buf = (t_u8 *) malloc(SCAN_RESP_BUF_SIZE);
    if (scan_rsp_buf == NULL) {
        printf("Error: allocate memory for scan_rsp_buf failed\n");
        return -ENOMEM;
    }
    prsp_info = (wlan_ioctl_get_scan_table_info *) scan_rsp_buf;
    memset(leg_ap_chan_list, 0, sizeof(leg_ap_chan_list));
    num_leg_ap_chan = 0;
    if (get_priv_ioctl("getscantable",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        ret = -EOPNOTSUPP;
        goto done;
    }

    scan_result_found = FALSE;
    scan_start = 1;

    do {
        prsp_info->scan_number = scan_start;

        /** Set up and execute the ioctl call */
        strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
        iwr.u.data.pointer = (caddr_t) prsp_info;
        iwr.u.data.length = SCAN_RESP_BUF_SIZE;
        iwr.u.data.flags = subioctl_val;

        if (ioctl(sockfd, ioctl_val, &iwr)) {
            perror("mlan2040coex: getscantable ioctl");
            ret = -EFAULT;
            goto done;
        }

        pcurrent = 0;
        pnext = prsp_info->scan_table_entry_buf;

        for (idx = 0; idx < prsp_info->scan_number; idx++) {

            /* 
             * Set pcurrent to pnext in case pad bytes are at the end
             * of the last IE we processed.
             */
            pcurrent = pnext;
            prsp_entry = (wlan_ioctl_get_scan_table_entry *) pcurrent;
            scan_result_found = TRUE;
            pcurrent += (sizeof(prsp_entry->fixed_field_length) +
                         prsp_entry->fixed_field_length);

            bss_info_len = prsp_entry->bss_info_length;
            pcurrent += sizeof(prsp_entry->bss_info_length);
            pnext = pcurrent + prsp_entry->bss_info_length;

            if (bss_info_len >= (sizeof(tsf)
                                 + sizeof(beacon_interval) + sizeof(cap_info))) {
                pcurrent +=
                    (sizeof(tsf) + sizeof(beacon_interval) + sizeof(cap_info));
                bss_info_len -=
                    (sizeof(tsf) + sizeof(beacon_interval) + sizeof(cap_info));
            }
            ht_cap_present = FALSE;
            intol_bit_is_set = FALSE;
            while (bss_info_len >= 2) {
                pelement_id = (IEEEtypes_ElementId_e *) pcurrent;
                pelement_len = pcurrent + 1;
                pcurrent += 2;

                switch (*pelement_id) {
                case SSID:
                    if (*pelement_len &&
                        *pelement_len <= MRVDRV_MAX_SSID_LENGTH) {
                        memcpy(ssid, pcurrent, *pelement_len);
                        ssid_len = *pelement_len;
                    }
                    break;

                case HT_CAPABILITY:
                    pht_cap = (IEEEtypes_HTCap_t *) pelement_id;
                    ht_cap_present = TRUE;
                    if (IS_INTOL_BIT_SET
                        (le16_to_cpu(pht_cap->ht_cap.ht_cap_info))) {
                        intol_bit_is_set = TRUE;
                    }
                    break;
                default:
                    break;
                }
                pcurrent += *pelement_len;
                bss_info_len -= (2 + *pelement_len);
            }
            if (!ht_cap_present || intol_bit_is_set) {
                printf("%s AP found on channel number: %-3d ",
                       intol_bit_is_set ? "40 MHZ intolerant" : "Legacy",
                       prsp_entry->fixed_fields.channel);
                if (ssid_len) {
                    printf("SSID: ");
                    /* Print out the ssid or the hex values if non-printable */
                    for (ssid_idx = 0; ssid_idx < ssid_len; ssid_idx++) {
                        if (isprint(ssid[ssid_idx])) {
                            printf("%c", ssid[ssid_idx]);
                        } else {
                            printf("\\%02x", ssid[ssid_idx]);
                        }
                    }
                }
                printf("\n");

                /* Verify that the channel is already listed or not */
                already_listed = FALSE;
                for (j = 0; j < i; j++) {
                    if (leg_ap_chan_list[j].chan_num ==
                        prsp_entry->fixed_fields.channel) {
                        already_listed = TRUE;
                        break;
                    }
                }
                if (!already_listed) {
                    /* add the channel in list */
                    leg_ap_chan_list[i].chan_num =
                        prsp_entry->fixed_fields.channel;
                    leg_ap_chan_list[i].is_intol_set = intol_bit_is_set;
                    i++;
                    coex_cmd_req_flag = TRUE;
                    num_leg_ap_chan++;
                }
            }
        }
        scan_start += prsp_info->scan_number;
        memset(ssid, 0, sizeof(ssid));
        ssid_len = 0;

    } while (prsp_info->scan_number);

  done:
    if (scan_rsp_buf)
        free(scan_rsp_buf);
    return ret;
}

/** BSS Mode any (both infra and adhoc) */
#define BSS_MODE_ANY 3

/** 
 *  @brief Issue OBSS scan command
 *  @return     MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
int
process_setuserscan(void)
{
    wlan_ioctl_user_scan_cfg scan_req;
    int ioctl_val, subioctl_val;
    struct iwreq iwr;

    memset(&scan_req, 0x00, sizeof(scan_req));
    coex_cmd_req_flag = FALSE;

    if (get_priv_ioctl("setuserscan",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    scan_req.bss_mode = BSS_MODE_ANY;
    scan_req.chan_list[0].scan_time =
        (t_u32) le16_to_cpu(cur_obss_scan_param.obss_scan_active_dwell);

    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.pointer = (caddr_t) & scan_req;
    iwr.u.data.length = sizeof(scan_req);
    iwr.u.data.flags = subioctl_val;
    if (ioctl(sockfd, ioctl_val, &iwr)) {
        perror("mlan2040coex: setuserscan ioctl");
        return -EFAULT;
    }
    /** process scan results */
    process_scantable();

    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief  get range 
 *   
 *  @return MLAN_STATUS_SUCCESS--success, otherwise --fail
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

    if (ioctl(sockfd, SIOCGIWRANGE, &iwr)) {
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
 *  @brief              get connection status 
 *  
 *  @param data         Pointer to the output buffer holding connection status
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
int
get_connstatus(int *data)
{
    struct iwreq iwr;
    struct sockaddr apaddr;
    struct ether_addr etherzero = { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00} };

    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);

    if (ioctl(sockfd, SIOCGIWAP, &iwr)) {
        fprintf(stderr, "mlan2040coex: ioctl SIOCGIWAP failed by %s\n",
                dev_name);
        return MLAN_STATUS_FAILURE;
    }

    memset(&apaddr, 0, sizeof(struct sockaddr));
    memcpy(&apaddr, &(iwr.u.ap_addr), sizeof(struct sockaddr));
    if (!memcmp
        ((struct ether_addr *) apaddr.sa_data, &etherzero,
         sizeof(struct ether_addr))) {
        /* not associated */
        *data = FALSE;
    } else {
        /* associated */
        *data = TRUE;
    }

    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief              This function parses for IWEVENT events 
 *
 *  @param h            Pointer to Netlink message header
 *  @param left         Number of bytes to be read
 *  @param evt_conn     A pointer to a output buffer. It sets TRUE when it gets
 *  					the event CUS_EVT_OBSS_SCAN_PARAM, otherwise FALSE
 *  @return             Number of bytes left to be read
 */
static int
drv_iwevt_handler(struct nlmsghdr *h, int left, int *evt_conn)
{
    int len, plen, attrlen, nlmsg_len, rta_len;
    struct ifinfomsg *ifi;
    struct rtattr *attr;

    *evt_conn = FALSE;
    while (left >= sizeof(*h)) {
        len = h->nlmsg_len;
        plen = len - sizeof(*h);
        if (len > left || plen < 0) {
            /* malformed netlink message */
            break;
        }
        if (plen < sizeof(*ifi)) {
            break;
        }
        ifi = NLMSG_DATA(h);
        nlmsg_len = NLMSG_ALIGN(sizeof(struct ifinfomsg));
        attrlen = h->nlmsg_len - nlmsg_len;
        if (attrlen < 0) {
            break;
        }

        attr = (struct rtattr *) (((char *) ifi) + nlmsg_len);
        rta_len = RTA_ALIGN(sizeof(struct rtattr));
        while (RTA_OK(attr, attrlen)) {
            if (attr->rta_type == IFLA_WIRELESS) {
                struct iw_event *iwe;
                char *pos = ((char *) attr) + rta_len;
                char *end = pos + (attr->rta_len - rta_len);
                unsigned short dlen;

                while (pos + IW_EV_LCP_LEN <= end) {
                    iwe = (struct iw_event *) pos;
                    if (iwe->len <= IW_EV_LCP_LEN)
                        break;

                    switch (iwe->cmd) {
                    case SIOCGIWAP:
                        {
                            struct ether_addr *wap;
                            struct ether_addr etherzero =
                                { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00} };
                            char buf[32];

                            wap = (struct ether_addr *)
                                ((struct sockaddr *) (&iwe->u.ap_addr))->
                                sa_data;

                            if (!memcmp
                                (wap, &etherzero, sizeof(struct ether_addr))) {
                                printf("---< Disconnected from AP >---\n");
                                memset(&cur_obss_scan_param, 0,
                                       sizeof(cur_obss_scan_param));
                                assoc_flag = FALSE;
                                is_ht_ap = FALSE;
                            } else {
                                memset(buf, 0, sizeof(buf));
                                sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X",
                                        wap->ether_addr_octet[0],
                                        wap->ether_addr_octet[1],
                                        wap->ether_addr_octet[2],
                                        wap->ether_addr_octet[3],
                                        wap->ether_addr_octet[4],
                                        wap->ether_addr_octet[5]);
                                printf("---< Connected to AP: %s >---\n", buf);
                                /** set TRUE, if connected */
                                assoc_flag = TRUE;
                            }
                            pos += iwe->len;
                        }
                        break;
                    case IWEVCUSTOM:
                        {
                            char *custom;
                            custom = pos + IW_EV_POINT_LEN;
                            if (IW_EV_POINT_LEN ==
                                IW_EV_LCP_LEN + sizeof(struct iw_point)) {
                                dlen = iwe->u.data.length;
                            } else {    /* WIRELESS_EXT >= 19 */
                                dlen =
                                    *((unsigned short *) (pos + IW_EV_LCP_LEN));
                            }
                            if (custom + dlen > end)
                                break;

                            printf("---< %s >---\n", custom);
                            if (!strncmp
                                (CUS_EVT_OBSS_SCAN_PARAM, custom,
                                 strlen(CUS_EVT_OBSS_SCAN_PARAM))) {
                                memset(&cur_obss_scan_param, 0,
                                       sizeof(cur_obss_scan_param));
                                memcpy(&cur_obss_scan_param,
                                       (custom +
                                        strlen(CUS_EVT_OBSS_SCAN_PARAM) + 1),
                                       sizeof(cur_obss_scan_param));
                                /** set TRUE, since it is an HT AP */
                                is_ht_ap = TRUE;
                                *evt_conn = TRUE;
                            }
                            pos += iwe->len;
                        }
                        break;
                    default:
                        pos += iwe->len;
                        break;
                    }
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
 *  @brief Run the application
 */
void
run_app(void)
{
    struct timeval tv;
    fd_set rfds;
    int ret = MLAN_STATUS_FAILURE;
    char msg[1024];
    int left, evt_conn;

    /* Initialize timeout value */
    tv.tv_sec = DEFAULT_SCAN_INTERVAL;
    tv.tv_usec = 0;

    /** Get connection status */
    if (get_connstatus(&assoc_flag) == MLAN_STATUS_FAILURE)
        return;

    while (!terminate_flag) {
        memset(msg, 0, sizeof(msg));
        /* Setup read fds */
        FD_ZERO(&rfds);
        FD_SET(nl_sk, &rfds);
        /* Wait for event */
        ret = select(nl_sk + 1, &rfds, NULL, NULL, &tv);
        if (ret == MLAN_STATUS_FAILURE) {
            /* Error */
            tv.tv_sec = DEFAULT_SCAN_INTERVAL;
            tv.tv_usec = 0;
            continue;
        } else if (!ret) {
            if (assoc_flag && is_ht_ap) {
                /** Issue OBSS scan */
                process_setuserscan();
                /** Invoke 2040coex command, if any legacy AP found or 
                 *  any AP has 40MHz intolarent bit set */
                if (coex_cmd_req_flag)
                    invoke_coex_command();
            }
            if (assoc_flag && is_ht_ap) {
                /* Timeout. Try again after BSS channel width triger scan
                   interval when the STA is connected with a HT AP */
                tv.tv_sec =
                    (t_u32) le16_to_cpu(cur_obss_scan_param.
                                        bss_chan_width_trigger_scan_int);
            } else {
                /* Timeout. Try again after default duration when the STA is
                   not connected with a HT AP */
                tv.tv_sec = DEFAULT_SCAN_INTERVAL;
            }
            tv.tv_usec = 0;
            continue;
        }
        if (!FD_ISSET(nl_sk, &rfds)) {
            /* Unexpected error. Try again */
            tv.tv_sec = DEFAULT_SCAN_INTERVAL;
            tv.tv_usec = 0;
            continue;
        }
        left = recv(nl_sk, msg, sizeof(msg), MSG_DONTWAIT);

        /* handle only IWEVENT events here */
        left = drv_iwevt_handler((struct nlmsghdr *) msg, left, &evt_conn);
        if (assoc_flag && is_ht_ap) {
            /** If the event is connected with an HT AP then issue OBSS scan immediately */
            if (evt_conn) {
                /** Issue OBSS scan */
                process_setuserscan();
                /** Invoke 2040coex command, if any legacy AP found or 
                 *  any AP has 40MHz intolarent bit set */
                if (coex_cmd_req_flag)
                    invoke_coex_command();
            }
            tv.tv_sec =
                (t_u32) le16_to_cpu(cur_obss_scan_param.
                                    bss_chan_width_trigger_scan_int);
        } else {
            tv.tv_sec = DEFAULT_SCAN_INTERVAL;
        }
        tv.tv_usec = 0;
    }
    return;
}

/** 
 *  @brief open netlink socket
 *  @return  socket id --success, otherwise--MLAN_STATUS_FAILURE
 */
int
open_netlink(void)
{
    int sk;
    struct sockaddr_nl addr;

    /* Open Netlink socket */
    sk = socket(PF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    if (sk < 0) {
        printf("ERR:Could not open netlink socket.\n");
        return MLAN_STATUS_FAILURE;
    }

    /* Set source address */
    memset(&addr, 0, sizeof(addr));
    addr.nl_family = AF_NETLINK;
    addr.nl_groups = RTMGRP_LINK;

    /* Bind socket with source address */
    if (bind(sk, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        printf("ERR:Could not bind socket!\n");
        return MLAN_STATUS_FAILURE;
    }
    return sk;
}

/** 
 *  @brief Terminate signal handler
 */
static t_void
terminate_handler(int signal)
{
    terminate_flag = TRUE;
}

/********************************************************
		Global Functions
********************************************************/
/** 
 *  @brief Process host command 
 *  @param cmd        Command    
 *  @param chan_list  A pointer to a channel list    
 *  @param chan_num   Number of channels in the channel list    
 *  @param reg_class  Regulatory class of the channels
 *  @param is_intol_ap_present Flag:is there any 40 MHz intolerant AP is present or not 
 *
 *  @return     MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
int
process_host_cmd(int cmd, t_u8 * chan_list, t_u8 chan_num, t_u8 reg_class,
                 t_u8 is_intol_ap_present)
{
    HostCmd_DS_GEN *hostcmd;
    struct iwreq iwr;
    int ret = MLAN_STATUS_SUCCESS;
    int ioctl_val, subioctl_val;
    t_u8 *buf = NULL;

    if (get_priv_ioctl("hostcmd",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }
    buf = (t_u8 *) malloc(MRVDRV_SIZE_OF_CMD_BUFFER);
    if (buf == NULL) {
        printf("Error: allocate memory for hostcmd failed\n");
        return -ENOMEM;
    }

    switch (cmd) {
    case CMD_2040COEX:
        prepare_coex_cmd_buff(buf, chan_list, chan_num, reg_class,
                              is_intol_ap_present);
        break;
    default:
        break;
    }

    hostcmd = (HostCmd_DS_GEN *) buf;
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.pointer = (t_u8 *) hostcmd;
    iwr.u.data.length = le16_to_cpu(hostcmd->size);

    iwr.u.data.flags = 0;
    if (ioctl(sockfd, ioctl_val, &iwr)) {
        fprintf(stderr, "mlan2040coex: MLANHOSTCMD is not supported by %s\n",
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
 *  @brief Check the STA is 40 MHz intolerant or not 
 *  @param intol	Flag: TRUE when the STA is 40 MHz intolerant, otherwise FALSE
 *  @return      	MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
int
is_intolerant_sta(int *intol)
{
    struct iwreq iwr;
    int ret = MLAN_STATUS_SUCCESS;
    int ioctl_val, subioctl_val, htcap_info;

    *intol = FALSE;
    if (get_priv_ioctl("htcapinfo",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.pointer = &htcap_info;
    iwr.u.data.length = 0;
    iwr.u.data.flags = subioctl_val;

    if (ioctl(sockfd, ioctl_val, &iwr)) {
        fprintf(stderr,
                "mlan2040coex: private command 'htcapinfo' is not supported by %s\n",
                dev_name);
        ret = MLAN_STATUS_FAILURE;
    }
    if (htcap_info & MBIT(8))
        *intol = TRUE;

    return ret;
}

/** 
 *  @brief get region code 
 *  @param reg_code	Pointer to region code
 *  @return      	MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
int
get_region_code(int *reg_code)
{
    struct iwreq iwr;
    int ret = MLAN_STATUS_SUCCESS;
    int ioctl_val, subioctl_val;

    if (get_priv_ioctl("regioncode",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.pointer = reg_code;
    iwr.u.data.length = 0;
    iwr.u.data.flags = subioctl_val;

    if (ioctl(sockfd, ioctl_val, &iwr)) {
        fprintf(stderr,
                "mlan2040coex: private command 'regioncode' is not supported by %s\n",
                dev_name);
        ret = MLAN_STATUS_FAILURE;
    }
    return ret;
}

/** No option */
#define NO_OPTION -1

/** 
 *  @brief Entry function for coex
 *  @param argc		number of arguments
 *  @param argv     A pointer to arguments array    
 *  @return      	MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
int
main(int argc, char *argv[])
{
    char *ifname = "mlan0";
    int c, daemonize = FALSE;
    for (;;) {
        c = getopt(argc, argv, "Bhi:v");
        /* check if all command-line options have been parsed */
        if (c == NO_OPTION)
            break;

        switch (c) {
        case 'B':
            daemonize = TRUE;
            break;
        case 'h':
            display_usage();
            return MLAN_STATUS_SUCCESS;
        case 'v':
            fprintf(stdout, "Marvell 20/40coex application version %s\n",
                    COEX_VER);
            return MLAN_STATUS_SUCCESS;
        case 'i':
            ifname = optarg;
            break;
        default:
            fprintf(stdout, "Invalid argument\n");
            display_usage();
            return MLAN_STATUS_SUCCESS;
        }
    }

    strncpy(dev_name, ifname, IFNAMSIZ);

    /* create a socket */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "mlan2040coex: Cannot open socket.\n");
        goto done;
    }
    if (get_range() != MLAN_STATUS_SUCCESS) {
        fprintf(stderr, "mlan2040coex: Cannot get range.\n");
        goto done;
    }
    /* create a netlink socket */
    if ((nl_sk = open_netlink()) == MLAN_STATUS_FAILURE) {
        fprintf(stderr, "mlan2040coex: Cannot open netlink socket.\n");
        goto done;
    }

    signal(SIGHUP, terminate_handler);  /* catch hangup signal */
    signal(SIGTERM, terminate_handler); /* catch kill signal */

    /** Make the process background-process */
    if (daemonize)
        daemon(0, 0);

    /** run the application */
    run_app();

  done:
    if (sockfd)
        close(sockfd);
    if (nl_sk)
        close(nl_sk);

    return MLAN_STATUS_SUCCESS;
}
