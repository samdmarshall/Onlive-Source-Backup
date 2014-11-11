/** @file  mlanmisc.c
  *
  * @brief Program to prepare command buffer
  * 
  *  Copyright (C) 2008-2009, Marvell International Ltd.
  *  All Rights Reserved
  */

/************************************************************************
Change log:
     03/10/2009: initial version
************************************************************************/

#include    "mlanconfig.h"
#include    "mlanhostcmd.h"
#include    "mlanmisc.h"

/********************************************************
		Local Variables
********************************************************/

/********************************************************
		Global Variables
********************************************************/

extern t_s32 sockfd;  /**< socket */
extern t_s8 dev_name[IFNAMSIZ + 1]; /**< device name */

/********************************************************
		Local Functions
********************************************************/

/**
 *  @brief Helper function for process_getscantable_idx
 *
 *  @param pbuf     A pointer to the buffer
 *  @param buf_len  buffer length    
 *
 *  @return         NA
 *
 */
static void
dump_scan_elems(const t_u8 * pbuf, uint buf_len)
{
    uint idx;
    uint marker = 2 + pbuf[1];

    for (idx = 0; idx < buf_len; idx++) {
        if (idx % 0x10 == 0) {
            printf("\n%04x: ", idx);
        }

        if (idx == marker) {
            printf("|");
            marker = idx + pbuf[idx + 1] + 2;
        } else {
            printf(" ");
        }

        printf("%02x ", pbuf[idx]);
    }

    printf("\n");
}

/**
 *  @brief Helper function for process_getscantable_idx
 *  Find next element
 *
 *  @param pp_ie_out    pointer of a IEEEtypes_Generic_t structure pointer              
 *  @param p_buf_left   integer pointer, which contains the number of left p_buf
 *
 *  @return             MLAN_STATUS_SUCCESS on success, otherwise MLAN_STATUS_FAILURE
 */
static int
scantable_elem_next(IEEEtypes_Generic_t ** pp_ie_out, int *p_buf_left)
{
    IEEEtypes_Generic_t *pie_gen;
    t_u8 *p_next;

    if (*p_buf_left < 2) {
        return MLAN_STATUS_FAILURE;
    }

    pie_gen = *pp_ie_out;

    p_next = (t_u8 *) pie_gen + (pie_gen->ieee_hdr.len
                                 + sizeof(pie_gen->ieee_hdr));
    *p_buf_left -= (p_next - (t_u8 *) pie_gen);

    *pp_ie_out = (IEEEtypes_Generic_t *) p_next;

    if (*p_buf_left <= 0) {
        return MLAN_STATUS_FAILURE;
    }

    return MLAN_STATUS_SUCCESS;
}

 /**
  *  @brief Helper function for process_getscantable_idx
  *         scantable find element
  *
  *  @param ie_buf       pointer of the IE buffer                
  *  @param ie_buf_len   IE buffer length
  *  @param ie_type      IE type
  *  @param ppie_out     pointer to the IEEEtypes_Generic_t structure pointer
  *  @return             MLAN_STATUS_SUCCESS on success, otherwise MLAN_STATUS_FAILURE
  */
static int
scantable_find_elem(t_u8 * ie_buf,
                    unsigned int ie_buf_len,
                    IEEEtypes_ElementId_e ie_type,
                    IEEEtypes_Generic_t ** ppie_out)
{
    int found;
    unsigned int ie_buf_left;

    ie_buf_left = ie_buf_len;

    found = FALSE;

    *ppie_out = (IEEEtypes_Generic_t *) ie_buf;

    do {
        found = ((*ppie_out)->ieee_hdr.element_id == ie_type);

    } while (!found &&
             (scantable_elem_next(ppie_out, (int *) &ie_buf_left) == 0));

    if (!found) {
        *ppie_out = NULL;
    }

    return (found ? MLAN_STATUS_SUCCESS : MLAN_STATUS_FAILURE);
}

 /**
  *  @brief Helper function for process_getscantable_idx
  *         It gets SSID from IE 
  *
  *  @param ie_buf       IE buffer
  *  @param ie_buf_len   IE buffer length
  *  @param pssid        SSID
  *  @param ssid_buf_max size of SSID    
  *  @return             MLAN_STATUS_SUCCESS on success, otherwise MLAN_STATUS_FAILURE
  */
static int
scantable_get_ssid_from_ie(t_u8 * ie_buf,
                           unsigned int ie_buf_len,
                           t_u8 * pssid, unsigned int ssid_buf_max)
{
    int retval;
    IEEEtypes_Generic_t *pie_gen;

    retval = scantable_find_elem(ie_buf, ie_buf_len, SSID, &pie_gen);

    memcpy(pssid, pie_gen->data, MIN(pie_gen->ieee_hdr.len, ssid_buf_max));

    return retval;
}

/** 
 *  @brief Provision the driver with a IEEE IE for use in the next join cmd
 *
 *  @param table_idx table index
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
process_getscantable_idx(int table_idx)
{
    int ioctl_val, subioctl_val;
    struct iwreq iwr;
    t_u8 *pcurrent;
    int bss_info_len, ret = 0;
    char ssid[33];
    t_u16 tmp_cap;
    t_u8 tsf[8];
    t_u16 beacon_interval;
    t_u8 *scan_rsp_buf = NULL;
    t_u16 cap_info;
    wlan_ioctl_get_scan_table_info *prsp_info;
    wlan_ioctl_get_scan_table_entry *prsp_entry;

    scan_rsp_buf = (t_u8 *) malloc(SCAN_RESP_BUF_SIZE);
    if (scan_rsp_buf == NULL) {
        printf("Error: allocate memory for scan_rsp_buf failed\n");
        return -ENOMEM;
    }
    memset(ssid, 0x00, sizeof(ssid));

    prsp_info = (wlan_ioctl_get_scan_table_info *) scan_rsp_buf;

    prsp_info->scan_number = table_idx;

    if (get_priv_ioctl("getscantable",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        ret = -EOPNOTSUPP;
        goto done;
    }

    /* 
     * Set up and execute the ioctl call
     */
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.pointer = (caddr_t) prsp_info;
    iwr.u.data.length = SCAN_RESP_BUF_SIZE;
    iwr.u.data.flags = subioctl_val;

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("mlanconfig: getscantable ioctl");
        ret = -EFAULT;
        goto done;
    }

    prsp_entry
        = (wlan_ioctl_get_scan_table_entry *) prsp_info->scan_table_entry_buf;

    if (prsp_info->scan_number == 0) {
        printf("mlanconfig: getscantable ioctl - index out of range\n");
        ret = -EINVAL;
        goto done;
    }

    pcurrent = prsp_info->scan_table_entry_buf;
    pcurrent += (sizeof(prsp_entry->fixed_field_length) +
                 prsp_entry->fixed_field_length);

    bss_info_len = prsp_entry->bss_info_length;
    pcurrent += sizeof(prsp_entry->bss_info_length);

    /* time stamp is 8 byte long */
    memcpy(tsf, pcurrent, sizeof(tsf));
    pcurrent += sizeof(tsf);
    bss_info_len -= sizeof(tsf);

    /* beacon interval is 2 byte long */
    memcpy(&beacon_interval, pcurrent, sizeof(beacon_interval));
    /* endian convert needed here */
    beacon_interval = le16_to_cpu(beacon_interval);
    pcurrent += sizeof(beacon_interval);
    bss_info_len -= sizeof(beacon_interval);

    /* capability information is 2 byte long */
    memcpy(&cap_info, pcurrent, sizeof(cap_info));
    /* endian convert needed here */
    cap_info = le16_to_cpu(cap_info);
    pcurrent += sizeof(cap_info);
    bss_info_len -= sizeof(cap_info);

    scantable_get_ssid_from_ie(pcurrent,
                               bss_info_len, (t_u8 *) ssid, sizeof(ssid));

    printf("\n*** [%s], %02x:%02x:%02x:%02x:%02x:%2x\n",
           ssid,
           prsp_entry->fixed_fields.bssid[0], prsp_entry->fixed_fields.bssid[1],
           prsp_entry->fixed_fields.bssid[2], prsp_entry->fixed_fields.bssid[3],
           prsp_entry->fixed_fields.bssid[4],
           prsp_entry->fixed_fields.bssid[5]);
    memcpy(&tmp_cap, &cap_info, sizeof(tmp_cap));
    printf("Channel = %d, SS = %d, CapInfo = 0x%04x, BcnIntvl = %d\n",
           prsp_entry->fixed_fields.channel,
           255 - prsp_entry->fixed_fields.rssi, tmp_cap, beacon_interval);

    printf("TSF Values: AP(0x%02x%02x%02x%02x%02x%02x%02x%02x), ",
           tsf[7], tsf[6], tsf[5], tsf[4], tsf[3], tsf[2], tsf[1], tsf[0]);

    printf("Network(0x%016llx)\n", prsp_entry->fixed_fields.network_tsf);
    printf("\n");
    printf("Element Data (%d bytes)\n", bss_info_len);
    printf("------------");
    dump_scan_elems(pcurrent, bss_info_len);
    printf("\n");

  done:
    if (scan_rsp_buf)
        free(scan_rsp_buf);
    return ret;
}

/********************************************************
		Global Functions
********************************************************/
/** 
 *  @brief SD comand52 read/write
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
int
process_sdcmd52rw(int argc, char *argv[])
{
    struct iwreq iwr;
    t_u8 buf[6];
    t_u32 tmp;
    int ioctl_val, subioctl_val;

    if (get_priv_ioctl("sdcmd52rw",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    if (argc == 5) {
        buf[0] = 0;             /* CMD52 read */
        iwr.u.data.length = 6;
    } else if (argc == 6) {
        buf[0] = 1;             /* CMD52 write */
        buf[6] = atoval(argv[5]);       /* dat */
        iwr.u.data.length = 7;
    } else {
        fprintf(stderr, "Invalid number of parameters!\n");
        return MLAN_STATUS_FAILURE;
    }
    buf[1] = atoval(argv[3]);   /* func */
    tmp = atoval(argv[4]);      /* reg */
    buf[2] = tmp & 0xff;
    buf[3] = (tmp >> 8) & 0xff;
    buf[4] = (tmp >> 16) & 0xff;
    buf[5] = (tmp >> 24) & 0xff;
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.flags = subioctl_val;
    iwr.u.data.pointer = (caddr_t) buf;

    if (ioctl(sockfd, ioctl_val, &iwr)) {
        perror("mlanconfig");
        fprintf(stderr,
                "mlanconfig: CMD52 R/W not supported by "
                "interface %s\n", dev_name);
        return MLAN_STATUS_FAILURE;
    }
    printf("sdcmd52rw returns 0x%02X\n", buf[0]);

    return MLAN_STATUS_SUCCESS;
}

/** Maximum SDIO command-53 buffer length */
#define CMD53_BUF_LEN    2000

/** 
 *  @brief SD comand53 read/write
 *  
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
int
process_sdcmd53rw(int argc, char *argv[])
{
    struct iwreq iwr;
    t_s8 *buf = NULL;
    int addr, mode, blklen, blknum, i, rw;
    int ioctl_val, subioctl_val;

    if (get_priv_ioctl("sdcmd53rw",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    if (argc < 8) {
        fprintf(stderr, "Invalid number of parameters!\n");
        return MLAN_STATUS_FAILURE;
    }

    if (!(buf = malloc(CMD53_BUF_LEN)))
        return -ENOMEM;
    memset(buf, 0, CMD53_BUF_LEN);

    if (argc == 8) {
        rw = buf[0] = 0;        /* CMD53 read */
    } else {
        rw = buf[0] = 1;        /* CMD53 write */
    }
    buf[1] = atoval(argv[3]);   /* func */
    addr = atoval(argv[4]);     /* address */
    buf[2] = addr & 0xff;
    buf[3] = (addr >> 8) & 0xff;
    buf[4] = (addr >> 16) & 0xff;
    buf[5] = (addr >> 24) & 0xff;
    mode = atoval(argv[5]);     /* mode */
    buf[6] = (t_u8) mode;
    blklen = atoval(argv[6]);   /* block size */
    buf[7] = blklen & 0xff;
    buf[8] = (blklen >> 8) & 0xff;
    blknum = atoval(argv[7]);   /* block number or byte number */
    buf[9] = blknum & 0xff;
    buf[10] = (blknum >> 8) & 0xff;
    iwr.u.data.length = 11;
    if (buf[0]) {
        for (i = 0; i < (argc - 9); i++)
            buf[11 + i] = atoval(argv[8 + i]);
        iwr.u.data.length += (argc - 9);
    }
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.flags = subioctl_val;
    iwr.u.data.pointer = (caddr_t) buf;

    if (ioctl(sockfd, ioctl_val, &iwr)) {
        perror("mlanconfig");
        fprintf(stderr,
                "mlanconfig: CMD53 R/W not supported by "
                "interface %s\n", dev_name);
        free(buf);
        return MLAN_STATUS_FAILURE;
    }

    if (mode) {
        fprintf(stderr, "CMD53rw blklen = %d, blknum = %d\n", blklen, blknum);
    } else {
        blklen = 1;
        fprintf(stderr, "CMD53rw bytelen = %d\n", blknum);
    }
    if (!rw)
        hexdump("data", buf, blklen * blknum, ' ');

    free(buf);
    return MLAN_STATUS_SUCCESS;
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
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
int
process_getscantable(int argc, char *argv[])
{
    int ioctl_val, subioctl_val;
    struct iwreq iwr;
    t_u8 *scan_rsp_buf = NULL;

    unsigned int scan_start;
    int idx, ret = 0;

    t_u8 *pcurrent;
    t_u8 *pnext;
    IEEEtypes_ElementId_e *pelement_id;
    t_u8 *pelement_len;
    int bss_info_len;
    int ssid_idx;
    t_u8 *pbyte;
    char ssid[33];
    int ssid_len = 0;

    IEEEtypes_CapInfo_t cap_info = { 0 };
    t_u16 tmp_cap;
    t_u8 tsf[8];
    t_u16 beacon_interval;

    IEEEtypes_VendorSpecific_t *pwpa_ie;
    const t_u8 wpa_oui[4] = { 0x00, 0x50, 0xf2, 0x01 };

    IEEEtypes_WmmParameter_t *pwmm_ie;
    const t_u8 wmm_oui[4] = { 0x00, 0x50, 0xf2, 0x02 };
    IEEEtypes_VendorSpecific_t *pwps_ie;
    const t_u8 wps_oui[4] = { 0x00, 0x50, 0xf2, 0x04 };
    char wmm_cap;
    char wps_cap;
    char dot11k_cap;
    char dot11r_cap;
    char priv_cap;
    char ht_cap;

    int displayed_info;

    wlan_ioctl_get_scan_table_info *prsp_info;
    wlan_ioctl_get_scan_table_entry *prsp_entry;

    scan_rsp_buf = (t_u8 *) malloc(SCAN_RESP_BUF_SIZE);
    if (scan_rsp_buf == NULL) {
        printf("Error: allocate memory for scan_rsp_buf failed\n");
        return -ENOMEM;
    }
    prsp_info = (wlan_ioctl_get_scan_table_info *) scan_rsp_buf;

    if (get_priv_ioctl("getscantable",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        ret = -EOPNOTSUPP;
        goto done;
    }

    if (argc > 3 && (strcmp(argv[3], "tsf") != 0)
        && (strcmp(argv[3], "help") != 0)) {

        idx = strtol(argv[3], NULL, 10);

        if (idx >= 0) {
            if (scan_rsp_buf)
                free(scan_rsp_buf);
            return process_getscantable_idx(idx);
        }
    }

    displayed_info = FALSE;
    scan_start = 1;

    printf("---------------------------------------");
    printf("---------------------------------------\n");
    printf("# | ch  | ss  |       bssid       |   cap    |   SSID \n");
    printf("---------------------------------------");
    printf("---------------------------------------\n");

    do {
        prsp_info->scan_number = scan_start;

        /* 
         * Set up and execute the ioctl call
         */
        strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
        iwr.u.data.pointer = (caddr_t) prsp_info;
        iwr.u.data.length = SCAN_RESP_BUF_SIZE;
        iwr.u.data.flags = subioctl_val;

        if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
            perror("mlanconfig: getscantable ioctl");
            ret = -EFAULT;
            goto done;
        }

        pcurrent = 0;
        pnext = prsp_info->scan_table_entry_buf;

        for (idx = 0; idx < prsp_info->scan_number; idx++) {

            /* 
             * Set pcurrent to pnext in case pad bytes are at the end
             *   of the last IE we processed.
             */
            pcurrent = pnext;

            prsp_entry = (wlan_ioctl_get_scan_table_entry *) pcurrent;

            printf("%02u| %03d | %03d | %02x:%02x:%02x:%02x:%02x:%02x |",
                   scan_start + idx,
                   prsp_entry->fixed_fields.channel,
                   255 - prsp_entry->fixed_fields.rssi,
                   prsp_entry->fixed_fields.bssid[0],
                   prsp_entry->fixed_fields.bssid[1],
                   prsp_entry->fixed_fields.bssid[2],
                   prsp_entry->fixed_fields.bssid[3],
                   prsp_entry->fixed_fields.bssid[4],
                   prsp_entry->fixed_fields.bssid[5]);

            displayed_info = TRUE;

            pcurrent += (sizeof(prsp_entry->fixed_field_length) +
                         prsp_entry->fixed_field_length);

            bss_info_len = prsp_entry->bss_info_length;
            pcurrent += sizeof(prsp_entry->bss_info_length);
            pnext = pcurrent + prsp_entry->bss_info_length;

            if (bss_info_len >= (sizeof(tsf)
                                 + sizeof(beacon_interval) + sizeof(cap_info))) {
                /* time stamp is 8 byte long */
                memcpy(tsf, pcurrent, sizeof(tsf));
                pcurrent += sizeof(tsf);
                bss_info_len -= sizeof(tsf);

                /* beacon interval is 2 byte long */
                memcpy(&beacon_interval, pcurrent, sizeof(beacon_interval));
                /* endian convert needed here */
                beacon_interval = le16_to_cpu(beacon_interval);
                pcurrent += sizeof(beacon_interval);
                bss_info_len -= sizeof(beacon_interval);

                /* capability information is 2 byte long */
                memcpy(&tmp_cap, pcurrent, sizeof(tmp_cap));
                /* endian convert needed here */
                tmp_cap = le16_to_cpu(tmp_cap);
                memcpy(&cap_info, &tmp_cap, sizeof(cap_info));
                pcurrent += sizeof(cap_info);
                bss_info_len -= sizeof(cap_info);
            }

            wmm_cap = ' ';      /* M (WMM), C (WMM-Call Admission Control) */
            wps_cap = ' ';      /* "S" */
            dot11k_cap = ' ';   /* "K" */
            dot11r_cap = ' ';   /* "R" */
            ht_cap = ' ';       /* "H" */

            /* "P" for Privacy (WEP) since "W" is WPA, and "2" is RSN/WPA2 */
            priv_cap = cap_info.privacy ? 'P' : ' ';

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

                case WPA_IE:
                    pwpa_ie = (IEEEtypes_VendorSpecific_t *) pelement_id;
                    if ((memcmp
                         (pwpa_ie->vend_hdr.oui, wpa_oui,
                          sizeof(pwpa_ie->vend_hdr.oui)) == 0)
                        && (pwpa_ie->vend_hdr.oui_type == wpa_oui[3])) {
                        /* WPA IE found, 'W' for WPA */
                        priv_cap = 'W';
                    } else {
                        pwmm_ie = (IEEEtypes_WmmParameter_t *) pelement_id;
                        if ((memcmp(pwmm_ie->vend_hdr.oui,
                                    wmm_oui,
                                    sizeof(pwmm_ie->vend_hdr.oui)) == 0)
                            && (pwmm_ie->vend_hdr.oui_type == wmm_oui[3])) {
                            /* Check the subtype: 1 == parameter, 0 == info */
                            if ((pwmm_ie->vend_hdr.oui_subtype == 1)
                                && pwmm_ie->ac_params[WMM_AC_VO].aci_aifsn.acm) {
                                /* Call admission on VO; 'C' for CAC */
                                wmm_cap = 'C';
                            } else {
                                /* No CAC; 'M' for uh, WMM */
                                wmm_cap = 'M';
                            }
                        } else {
                            pwps_ie =
                                (IEEEtypes_VendorSpecific_t *) pelement_id;
                            if ((memcmp
                                 (pwps_ie->vend_hdr.oui, wps_oui,
                                  sizeof(pwps_ie->vend_hdr.oui)) == 0)
                                && (pwps_ie->vend_hdr.oui_type == wps_oui[3])) {
                                wps_cap = 'S';
                            }
                        }
                    }
                    break;

                case RSN_IE:
                    /* RSN IE found; '2' for WPA2 (RSN) */
                    priv_cap = '2';
                    break;
                case HT_CAPABILITY:
                    ht_cap = 'N';
                    break;

                default:
                    break;
                }

                pcurrent += *pelement_len;
                bss_info_len -= (2 + *pelement_len);
            }

            /* "A" for Adhoc "I" for Infrastructure, "D" for DFS (Spectrum
               Mgmt) */
            printf(" %c%c%c%c%c%c%c%c | ", cap_info.ibss ? 'A' : 'I', priv_cap, /* P 
                                                                                   (WEP), 
                                                                                   W 
                                                                                   (WPA), 
                                                                                   2 
                                                                                   (WPA2) 
                                                                                 */
                   cap_info.spectrum_mgmt ? 'D' : ' ', wmm_cap, /* M (WMM), C
                                                                   (WMM-Call
                                                                   Admission
                                                                   Control) */
                   dot11k_cap,  /* K */
                   dot11r_cap,  /* R */
                   wps_cap,     /* S */
                   ht_cap);     /* N */

            /* Print out the ssid or the hex values if non-printable */
            for (ssid_idx = 0; ssid_idx < ssid_len; ssid_idx++) {
                if (isprint(ssid[ssid_idx])) {
                    printf("%c", ssid[ssid_idx]);
                } else {
                    printf("\\%02x", ssid[ssid_idx]);
                }
            }

            printf("\n");

            if (argc > 3 && strcmp(argv[3], "tsf") == 0) {
                /* TSF is a u64, some formatted printing libs have trouble
                   printing long longs, so cast and dump as bytes */
                pbyte = (t_u8 *) & prsp_entry->fixed_fields.network_tsf;
                printf("    TSF=%02x%02x%02x%02x%02x%02x%02x%02x\n",
                       pbyte[7], pbyte[6], pbyte[5], pbyte[4],
                       pbyte[3], pbyte[2], pbyte[1], pbyte[0]);
            }
        }

        scan_start += prsp_info->scan_number;

    } while (prsp_info->scan_number);

    if (displayed_info == TRUE) {
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
                   " K [ 11k ]\n"
                   " R [ 11r ]\n" " S [ WPS ]\n" " N [ HT (11n) ]\n" "\n\n");
        }
    } else {
        printf("< No Scan Results >\n");
    }

  done:
    if (scan_rsp_buf)
        free(scan_rsp_buf);
    return ret;
}

/** Maximum channel scratch */
#define MAX_CHAN_SCRATCH  100

/** Maximum number of probes to send on each channel */
#define MAX_PROBES  10
/**
 *  @brief Request a scan from the driver and display the scan table afterwards
 *
 *  Command line interface for performing a specific immediate scan based
 *    on the following keyword parsing:
 *
 *     chan=[chan#][band][mode] where band is [a,b,g,n] and mode is 
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
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
int
process_setuserscan(int argc, char *argv[])
{
    wlan_ioctl_user_scan_cfg scan_req;
    int ioctl_val, subioctl_val;
    struct iwreq iwr;
    char *parg_tok;
    char *pchan_tok;
    char *parg_cookie;
    char *pchan_cookie;
    int arg_idx;
    int chan_parse_idx;
    int chan_cmd_idx;
    char chan_scratch[MAX_CHAN_SCRATCH];
    char *pscratch;
    int tmp_idx;
    int scan_time;
    int num_ssid;
    unsigned int mac[ETH_ALEN];

    memset(&scan_req, 0x00, sizeof(scan_req));
    chan_cmd_idx = 0;
    scan_time = 0;
    num_ssid = 0;

    if (get_priv_ioctl("setuserscan",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    for (arg_idx = 0; arg_idx < argc; arg_idx++) {
        if (strncmp(argv[arg_idx], "chan=", strlen("chan=")) == 0) {
            /* 
             *  "chan" token string handler
             */
            parg_tok = argv[arg_idx] + strlen("chan=");

            if (strlen(parg_tok) > MAX_CHAN_SCRATCH) {

                printf("Error: Specified channels exceeds max limit\n");
                return MLAN_STATUS_FAILURE;
            }

            while ((parg_tok = strtok_r(parg_tok, ",", &parg_cookie)) != NULL) {

                memset(chan_scratch, 0x00, sizeof(chan_scratch));
                pscratch = chan_scratch;

                for (chan_parse_idx = 0;
                     chan_parse_idx < strlen(parg_tok); chan_parse_idx++) {
                    if (isalpha(*(parg_tok + chan_parse_idx))) {
                        *pscratch++ = ' ';
                    }

                    *pscratch++ = *(parg_tok + chan_parse_idx);
                }
                *pscratch = 0;
                parg_tok = NULL;

                pchan_tok = chan_scratch;

                while ((pchan_tok = strtok_r(pchan_tok, " ",
                                             &pchan_cookie)) != NULL) {
                    if (isdigit(*pchan_tok)) {
                        scan_req.chan_list[chan_cmd_idx].chan_number
                            = atoi(pchan_tok);
                    } else {
                        switch (toupper(*pchan_tok)) {
                        case 'B':
                        case 'G':
                        case 'N':
                            scan_req.chan_list[chan_cmd_idx].radio_type = 0;
                            break;
                        case 'P':
                            scan_req.chan_list[chan_cmd_idx].scan_type = 1;
                            break;
                        default:
                            printf("Error: Band type not supported!\n");
                            return -EOPNOTSUPP;
                        }
                    }
                    pchan_tok = NULL;
                }
                chan_cmd_idx++;
            }
        } else if (strncmp(argv[arg_idx], "bssid=", strlen("bssid=")) == 0) {
            /* 
             *  "bssid" token string handler
             */
            sscanf(argv[arg_idx] + strlen("bssid="), "%2x:%2x:%2x:%2x:%2x:%2x",
                   mac + 0, mac + 1, mac + 2, mac + 3, mac + 4, mac + 5);

            for (tmp_idx = 0; tmp_idx < NELEMENTS(mac); tmp_idx++) {
                scan_req.specific_bssid[tmp_idx] = (t_u8) mac[tmp_idx];
            }
        } else if (strncmp(argv[arg_idx], "keep=", strlen("keep=")) == 0) {
            /* 
             *  "keep" token string handler
             */
            scan_req.keep_previous_scan = atoi(argv[arg_idx] + strlen("keep="));
        } else if (strncmp(argv[arg_idx], "dur=", strlen("dur=")) == 0) {
            /* 
             *  "dur" token string handler
             */
            scan_time = atoi(argv[arg_idx] + strlen("dur="));
            scan_req.chan_list[0].scan_time = scan_time;
        } else if (strncmp(argv[arg_idx], "ssid=", strlen("ssid=")) == 0) {
            /* 
             *  "ssid" token string handler
             */
            if (num_ssid < MRVDRV_MAX_SSID_LIST_LENGTH) {
                strncpy(scan_req.ssid_list[num_ssid].ssid,
                        argv[arg_idx] + strlen("ssid="),
                        sizeof(scan_req.ssid_list[num_ssid].ssid));

                scan_req.ssid_list[num_ssid].max_len = 0;

                num_ssid++;
            }
        } else if (strncmp(argv[arg_idx], "wc=", strlen("wc=")) == 0) {

            if (num_ssid < MRVDRV_MAX_SSID_LIST_LENGTH) {
                /* 
                 *  "wc" token string handler
                 */
                pscratch = rindex(argv[arg_idx], ',');

                if (pscratch) {
                    *pscratch = 0;
                    pscratch++;

                    if (isdigit(*pscratch)) {
                        scan_req.ssid_list[num_ssid].max_len = atoi(pscratch);
                    } else {
                        scan_req.ssid_list[num_ssid].max_len = *pscratch;
                    }
                } else {
                    /* Standard wildcard matching */
                    scan_req.ssid_list[num_ssid].max_len = 0xFF;
                }

                strncpy(scan_req.ssid_list[num_ssid].ssid,
                        argv[arg_idx] + strlen("wc="),
                        sizeof(scan_req.ssid_list[num_ssid].ssid));

                num_ssid++;
            }
        } else if (strncmp(argv[arg_idx], "probes=", strlen("probes=")) == 0) {
            /* 
             *  "probes" token string handler
             */
            scan_req.num_probes = atoi(argv[arg_idx] + strlen("probes="));
            if (scan_req.num_probes > MAX_PROBES) {
                fprintf(stderr, "Invalid probes (> %d)\n", MAX_PROBES);
                return -EOPNOTSUPP;
            }
        } else if (strncmp(argv[arg_idx], "type=", strlen("type=")) == 0) {
            /* 
             *  "type" token string handler
             */
            scan_req.bss_mode = atoi(argv[arg_idx] + strlen("type="));
            switch (scan_req.bss_mode) {
            case MLAN_SCAN_MODE_BSS:
            case MLAN_SCAN_MODE_IBSS:
                break;
            case MLAN_SCAN_MODE_ANY:
            default:
                /* Set any unknown types to ANY */
                scan_req.bss_mode = MLAN_SCAN_MODE_ANY;
                break;
            }
        }
    }

    /* 
     * Update all the channels to have the same scan time
     */
    for (tmp_idx = 1; tmp_idx < chan_cmd_idx; tmp_idx++) {
        scan_req.chan_list[tmp_idx].scan_time = scan_time;
    }

    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.pointer = (caddr_t) & scan_req;
    iwr.u.data.length = sizeof(scan_req);
    iwr.u.data.flags = subioctl_val;

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("mlanconfig: setuserscan ioctl");
        return -EFAULT;
    }

    process_getscantable(0, 0);

    return MLAN_STATUS_SUCCESS;
}
