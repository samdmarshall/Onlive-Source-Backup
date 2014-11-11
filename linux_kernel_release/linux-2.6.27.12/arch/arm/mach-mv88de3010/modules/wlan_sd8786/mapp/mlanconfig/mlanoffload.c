/** @file  mlanoffload.c
  *
  * @brief This files contains mlanconfig offload command handling.
  *
  *  Copyright (C) 2008-2009, Marvell International Ltd.
  *  All Rights Reserved
  */
/************************************************************************
Change log:
     08/11/2009: initial version
************************************************************************/

#include    "mlanconfig.h"
#include    "mlanhostcmd.h"
#include    "mlanmisc.h"
#include    "mlanoffload.h"

/********************************************************
		Local Variables
********************************************************/

typedef struct
{
    t_u8 chanNum;         /**< Channel Number */
    t_u8 chanLoad;        /**< Channel Load fraction */
    t_s16 anpi;           /**< Channel ANPI */

} ChanRptInfo_t;

/** Stringification of rateId enumeration */
const char *rateIdStr[] = { "1", "2", "5.5", "11", "--",
    "6", "9", "12", "18", "24", "36", "48", "54", "--"
};

/********************************************************
		Global Variables
********************************************************/

extern t_s32 sockfd;  /**< socket */
extern t_s8 dev_name[IFNAMSIZ + 1]; /**< device name */

/********************************************************
		Local Functions
********************************************************/

/** 
 *  @brief Remove unwanted spaces, tabs from a line
 *  
 *  @param data     A pointer to the starting of the line    
 *  @return         NA
 */
static void
profile_param_polish(char *data)
{
    t_u8 i, j, len = 0;
    char *ptr;
    if (((ptr = rindex(data, '\r')) == NULL) &&
        ((ptr = rindex(data, '\n')) == NULL)) {
        return;
    }
    len = ptr - data;
    for (i = 0; i < len; i++) {
        if ((*(data + i) == ' ') || (*(data + i) == '\t')) {
            for (j = i; j < len; j++) {
                data[j] = data[j + 1];
            }
            i--;
            len--;
        }
    }
}

static int
ascii_value(char letter)
{
    if (letter >= '0' && letter <= '9')
        return letter - '0';
    if (letter >= 'a' && letter <= 'f')
        return letter - 'a' + 10;
    if (letter >= 'A' && letter <= 'F')
        return letter - 'A' + 10;
    return -1;
}

static char
twodigit_ascii(const char *nibble)
{
    int a, b;
    a = ascii_value(*nibble++);
    if (a < 0)
        return -1;
    b = ascii_value(*nibble++);
    if (b < 0)
        return -1;
    return (a << 4) | b;
}

/** 
 *  @brief Read a network block from the profile configuration file
 *  
 *  @param fp	    file pointer of the configuration file    
 *  @param p_head   profile head  
 *  @return         MLAN_STATUS_SUCCESS
 */
static int
profile_read_block(FILE * fp, profile_entry_t * p_head)
{
    char line[0x100];
    char *ptr, *eptr;
    t_u8 key_cnt = 0;
    t_u8 i, wep_len;
    char byte;
    unsigned int mac[ETH_ALEN];
    int tmpIdx;

    while (fgets(line, sizeof(line), fp)) {
        /* call function to remove spaces, tabs */
        profile_param_polish(line);

        if ((ptr = strstr(line, "}")) != NULL) {
            /* end of network */
            break;
        } else if (line[0] == '#') {
            /* comments go ahead */
            continue;
        } else if ((ptr = strstr(line, "bssid=")) != NULL) {
            ptr = ptr + strlen("bssid=");
            sscanf(ptr, "%2x:%2x:%2x:%2x:%2x:%2x",
                   mac + 0, mac + 1, mac + 2, mac + 3, mac + 4, mac + 5);
            for (tmpIdx = 0; tmpIdx < NELEMENTS(mac); tmpIdx++) {
                p_head->bssid[tmpIdx] = (t_u8) mac[tmpIdx];
            }
        } else if ((ptr = strstr(line, "ssid=")) != NULL) {
            ptr = ptr + strlen("ssid=");
            if (*ptr != '"') {
                fprintf(stderr, "ssid not within quotes for network.\n");
                break;
            }
            if ((eptr = rindex(ptr + 1, '"')) == NULL) {
                fprintf(stderr, "ssid not within quotes for network.\n");
                break;
            }
            p_head->ssid_len = MIN(IW_ESSID_MAX_SIZE, eptr - ptr - 1);
            strncpy((char *) p_head->ssid, ptr + 1, p_head->ssid_len);
            p_head->ssid[p_head->ssid_len] = '\0';
        } else if ((ptr = strstr(line, "psk=")) != NULL) {
            ptr = ptr + strlen("psk=");
            if (*ptr != '"') {
                p_head->psk_config = 1;
                strncpy((char *) p_head->psk, ptr, KEY_LEN);
            } else {
                if ((eptr = rindex(ptr + 1, '"')) == NULL) {
                    fprintf(stderr,
                            "passphrase not within quotes for network.\n");
                    break;
                }
                p_head->passphrase_len = MIN(PHRASE_LEN, eptr - ptr - 1);
                strncpy(p_head->passphrase, ptr + 1, p_head->passphrase_len);
            }
        } else if ((ptr = strstr(line, "wep_key")) != NULL) {
            ptr = ptr + strlen("wep_key");
            key_cnt = atoi(ptr);
            ptr++;
            if (*ptr != '=') {
                fprintf(stderr, "invalid wep_key, missing =.\n");
                break;
            }
            if (((eptr = rindex(ptr + 1, '\r')) == NULL) &&
                ((eptr = rindex(ptr + 1, '\n')) == NULL)) {
                fprintf(stderr,
                        "missing EOL from the wep_key configuration.\n");
                break;
            }
            ptr++;
            if (*ptr == '"') {
                if ((eptr = rindex(ptr + 1, '"')) == NULL) {
                    fprintf(stderr, "wep key does not end with quote.\n");
                    break;
                }
                *eptr = '\0';
                p_head->wep_key_len[key_cnt] = eptr - ptr - 1;
                strncpy((char *) p_head->wep_key[key_cnt], ptr + 1,
                        p_head->wep_key_len[key_cnt]);
            } else {
                while (*eptr == '\r' || *eptr == '\n')
                    eptr--;
                *(eptr + 1) = '\0';
                wep_len = strlen(ptr);
                if (wep_len & 0x01) {
                    fprintf(stderr, "incorrect wep key %s.\n", ptr);
                    break;
                }
                p_head->wep_key_len[key_cnt] = wep_len / 2;
                for (i = 0; i < wep_len / 2; i++) {
                    byte = twodigit_ascii(ptr);
                    if (byte == -1) {
                        fprintf(stderr, "incorrect wep key %s.\n", ptr);
                        break;
                    }
                    *(p_head->wep_key[key_cnt] + i) = byte;
                    ptr += 2;
                }
            }
        } else if ((ptr = strstr(line, "key_mgmt=")) != NULL) {
            ptr = ptr + strlen("key_mgmt=");
            if ((eptr = strstr(ptr, "WPA_EAP")) != NULL) {
                p_head->key_mgmt |= PROFILE_DB_KEY_MGMT_IEEE8021X;
            }

            if ((eptr = strstr(ptr, "WPA_PSK")) != NULL) {
                p_head->key_mgmt |= PROFILE_DB_KEY_MGMT_PSK;
            }

            if ((eptr = strstr(ptr, "FT_EAP")) != NULL) {
                p_head->key_mgmt |= PROFILE_DB_KEY_MGMT_FT_IEEE8021X;
            }
            if ((eptr = strstr(ptr, "FT_PSK")) != NULL) {
                p_head->key_mgmt |= PROFILE_DB_KEY_MGMT_FT_PSK;
            }
            if ((eptr = strstr(ptr, "NONE")) != NULL) {
                p_head->key_mgmt |= PROFILE_DB_KEY_MGMT_NONE;
            }
        } else if ((ptr = strstr(line, "proto=")) != NULL) {
            ptr = ptr + strlen("proto=");
            if ((eptr = strstr(ptr, "WPA")) != NULL) {
                p_head->protocol |= PROFILE_DB_PROTO_WPA;
            }

            if ((eptr = strstr(ptr, "RSN")) != NULL) {
                p_head->protocol |= PROFILE_DB_PROTO_WPA2;

            }
        } else if ((ptr = strstr(line, "pairwise=")) != NULL) {
            ptr = ptr + strlen("pairwise=");
            if ((eptr = strstr(ptr, "CCMP")) != NULL) {
                p_head->pairwise_cipher |= PROFILE_DB_CIPHER_CCMP;
            }
            if ((eptr = strstr(ptr, "TKIP")) != NULL) {
                p_head->pairwise_cipher |= PROFILE_DB_CIPHER_TKIP;
            }
        } else if ((ptr = strstr(line, "groupwise=")) != NULL) {
            ptr = ptr + strlen("groupwise=");
            if ((eptr = strstr(ptr, "CCMP")) != NULL) {
                p_head->groupwise_cipher |= PROFILE_DB_CIPHER_CCMP;
            }
            if ((eptr = strstr(ptr, "TKIP")) != NULL) {
                p_head->groupwise_cipher |= PROFILE_DB_CIPHER_TKIP;
            }
        } else if ((ptr = strstr(line, "wep_tx_keyidx=")) != NULL) {
            ptr = ptr + strlen("wep_tx_keyidx=");
            p_head->wep_key_idx = atoi(ptr);
        } else if ((ptr = strstr(line, "roaming=")) != NULL) {
            ptr = ptr + strlen("roaming=");
            p_head->roaming = atoi(ptr);
        } else if ((ptr = strstr(line, "auto_associate=")) != NULL) {
            ptr = ptr + strlen("auto_associate=");
            p_head->auto_associate = atoi(ptr);
        } else if ((ptr = strstr(line, "ccx=")) != NULL) {
            ptr = ptr + strlen("ccx=");
            p_head->ccx = atoi(ptr);
        } else if ((ptr = strstr(line, "mode=")) != NULL) {
            ptr = ptr + strlen("mode=");
            p_head->mode = atoi(ptr);
        }
    }
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Issue profile command to add new profile to FW 
 *  
 *  @param filename     Name of profile file
 *
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
profile_read_download(char *filename)
{
    int i = 0;
    t_u8 *buffer;
    t_u16 temp, tempc;
    FILE *fp;
    char line[0x100];
    char *ptr;
    struct iwreq iwr;
    profile_entry_t *p_head = NULL;
    int ioctl_val, subioctl_val, ret = MLAN_STATUS_SUCCESS;

    if ((fp = fopen(filename, "r")) == NULL) {
        perror("fopen");
        fprintf(stderr, "Cannot open file %s\n", filename);
        return MLAN_STATUS_FAILURE;
    }

    if (get_priv_ioctl("hostcmd",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    buffer = (t_u8 *) malloc(MRVDRV_SIZE_OF_CMD_BUFFER);
    if (buffer == NULL) {
        printf("Error: allocate memory for profile command failed\n");
        return -ENOMEM;
    }

    while (fgets(line, sizeof(line), fp)) {
        /* call function to remove spaces, tabs */
        profile_param_polish(line);
        if (((ptr = strstr(line, "network={")) == NULL) || (line[0] == '#')) {
            continue;
        }
        /* 
         * Memory allocation of every network block
         */
        p_head = (profile_entry_t *) malloc(sizeof(profile_entry_t));
        if (p_head == NULL) {
            fprintf(stderr, "Memory error.\n");
            free(buffer);
            return MLAN_STATUS_FAILURE;
        }
        memset(p_head, 0x00, sizeof(profile_entry_t));

        ret = profile_read_block(fp, p_head);
        if (ret || p_head->ssid_len == 0) {
            free(p_head);
            continue;
        }

        /* 
         * Put all the ssid parameters in the buffer
         */
        i = 0;
        memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

        /* Cmd Header : Command */
        tempc = cpu_to_le16(HostCmd_CMD_PROFILEDB);
        memcpy((buffer + i), &tempc, sizeof(t_u16));
        i = sizeof(HostCmd_DS_GEN);

        /* set action as set */
        tempc = cpu_to_le16(HostCmd_ACT_GEN_SET);
        memcpy((buffer + i), &tempc, sizeof(t_u16));
        i += 2;

        /* ssid */
        tempc = cpu_to_le16(TLV_TYPE_SSID);
        memcpy((buffer + i), &tempc, sizeof(t_u16));
        i += 2;
        temp = strlen((char *) p_head->ssid);
        tempc = cpu_to_le16(temp);
        memcpy((buffer + i), &tempc, sizeof(t_u16));
        i += 2;
        memcpy((void *) (buffer + i), p_head->ssid, temp);
        i += temp;

        if (memcmp(p_head->bssid, "\x00\x00\x00\x00\x00\x00", ETH_ALEN)) {
            /* bssid */
            tempc = cpu_to_le16(TLV_TYPE_BSSID);
            memcpy((buffer + i), &tempc, sizeof(t_u16));
            i += 2;
            temp = ETH_ALEN;
            tempc = cpu_to_le16(temp);
            memcpy((buffer + i), &tempc, sizeof(t_u16));
            i += 2;
            memcpy((void *) (buffer + i), p_head->bssid, temp);
            i += temp;
        }

        /* proto */
        if (p_head->protocol == 0) {
            p_head->protocol = 0xFFFF;
        }

        tempc = cpu_to_le16(TLV_TYPE_PROTO);
        memcpy((buffer + i), &tempc, sizeof(t_u16));
        i += 2;
        temp = 2;
        tempc = cpu_to_le16(temp);
        memcpy((buffer + i), &tempc, sizeof(t_u16));
        i += 2;
        memcpy((buffer + i), &(p_head->protocol), temp);
        i += temp;

        /* key_mgmt */
        if (p_head->key_mgmt == 0) {
            p_head->key_mgmt = 0xFFFF;
        }

        tempc = cpu_to_le16(TLV_TYPE_AKMP);
        memcpy((buffer + i), &tempc, sizeof(t_u16));
        i += 2;
        temp = 2;
        tempc = cpu_to_le16(temp);
        memcpy((buffer + i), &tempc, sizeof(t_u16));
        i += 2;
        memcpy((buffer + i), &(p_head->key_mgmt), temp);
        i += temp;

        /* pairwise */
        if (p_head->pairwise_cipher == 0) {
            p_head->pairwise_cipher = 0xFF;
        }

        /* groupwise */
        if (p_head->groupwise_cipher == 0) {
            p_head->groupwise_cipher = 0xFF;
        }

        tempc = cpu_to_le16(TLV_TYPE_CIPHER);
        memcpy((buffer + i), &tempc, sizeof(t_u16));
        i += 2;
        temp = 2;
        tempc = cpu_to_le16(temp);
        memcpy((buffer + i), &tempc, sizeof(t_u16));
        i += 2;
        buffer[i] = p_head->pairwise_cipher;
        i += 1;
        buffer[i] = p_head->groupwise_cipher;
        i += 1;

        if (p_head->passphrase_len) {
            /* passphrase */
            tempc = cpu_to_le16(TLV_TYPE_PASSPHRASE);
            memcpy((buffer + i), &tempc, sizeof(t_u16));
            i += 2;
            temp = p_head->passphrase_len;
            tempc = cpu_to_le16(temp);
            memcpy((buffer + i), &tempc, sizeof(t_u16));
            i += 2;
            memcpy((void *) (buffer + i), p_head->passphrase, temp);
            i += temp;
        }

        if (p_head->psk_config) {
            /* psk method */
            tempc = cpu_to_le16(TLV_TYPE_PMK);
            memcpy((buffer + i), &tempc, sizeof(t_u16));
            i += 2;
            temp = 32;
            tempc = cpu_to_le16(temp);
            memcpy((buffer + i), &tempc, sizeof(t_u16));
            i += 2;
            memcpy((void *) (buffer + i), p_head->psk, temp);
            i += temp;
        }

        for (ret = 0; ret < WEP_KEY_CNT; ret++) {
            if (p_head->wep_key_len[ret]) {
                /* TAG_WEP_KEY */
                tempc = cpu_to_le16(TLV_TYPE_WEP_KEY);
                memcpy((buffer + i), &tempc, sizeof(t_u16));
                i += 2;
                /* wep_key_len + sizeof(keyIndex) + sizeof(IsDefault) */
                temp = p_head->wep_key_len[ret];
                tempc = cpu_to_le16(temp + 2);
                memcpy((buffer + i), &tempc, sizeof(t_u16));
                i += 2;
                *(buffer + i) = ret;
                i += 1;
                *(buffer + i) = (ret == p_head->wep_key_idx);
                i += 1;
                memcpy((void *) (buffer + i), p_head->wep_key[ret], temp);
                i += temp;
            }
        }

        /* roaming */
        if (p_head->roaming) {
            tempc = cpu_to_le16(TLV_TYPE_OFFLOAD_ENABLE);
            memcpy((buffer + i), &tempc, sizeof(t_u16));
            i += 2;
            temp = 2;
            tempc = cpu_to_le16(temp);
            memcpy((buffer + i), &tempc, sizeof(t_u16));
            i += 2;
            tempc = 0;
            if (p_head->roaming)
                tempc |= PROFILE_DB_FEATURE_ROAMING;
            if (p_head->auto_associate)
                tempc |= PROFILE_DB_FEATURE_AUTOASSOC;
            if (p_head->ccx)
                tempc |= PROFILE_DB_FEATURE_CCX;
            if (p_head->mode)
                tempc |= PROFILE_DB_FEATURE_MODE;
            tempc = cpu_to_le16(tempc);
            memcpy((buffer + i), &tempc, sizeof(t_u16));
            i += temp;
        }

        memset(&iwr, 0, sizeof(iwr));
        strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
        iwr.u.data.pointer = (caddr_t) buffer;
        iwr.u.data.length = i;
        iwr.u.data.flags = 0;

        /* update length in the header now */
        temp = i;
        tempc = cpu_to_le16(temp);
        memcpy((buffer + 2), &tempc, sizeof(t_u16));

        fprintf(stdout, "Downloading profile: %s ... ", p_head->ssid);
        fflush(stdout);

        if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
            perror("hostcmd : profiledb ioctl");
        } else {
            /* 
             * Header: Cmd, Size, Seqnum, Result 
             * Offload of Result = 2+2+2 = 6
             */
            memcpy(&temp, ((t_u8 *) iwr.u.data.pointer) + 6, sizeof(t_u16));
            temp = le16_to_cpu(temp);
            if (temp != 0) {
                printf("hostcmd : profiledb ioctl failure, code %d\n", temp);
                if (p_head)
                    free(p_head);
                break;
            }
        }

        fprintf(stdout, "done.\n");

        if (p_head)
            free(p_head);
    }

    free(buffer);
    return ret;
}

static int
chanrpt_getValues(t_u8 chanNum, t_u32 duration,
                  t_s16 * pAnpi, t_u8 * pLoadPercent)
{
    int ioctl_val, subioctl_val, cmdlen;
    struct iwreq iwr;
    t_u8 *buf;
    t_u8 *pByte;
    int respLen;
    MrvlIEtypes_Data_t *pTlvHdr;
    HostCmd_DS_GEN *hostcmd;
    HostCmd_DS_CHAN_RPT_RSP *pChanRptRsp;
    HostCmd_DS_CHAN_RPT_REQ *pChanRptReq;

    MrvlIEtypes_ChanRptChanLoad_t *pLoadRpt;
    MrvlIEtypes_ChanRptNoiseHist_t *pNoiseRpt;

    if (get_priv_ioctl("hostcmd",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    buf = (t_u8 *) calloc(sizeof(t_u8), 1000);
    if (!buf) {
        printf("Memory allocation failed\n");
        return -ENOMEM;
    }

    cmdlen = sizeof(HostCmd_DS_GEN) + sizeof(HostCmd_DS_CHAN_RPT_REQ);

    hostcmd = (HostCmd_DS_GEN *) buf;
    hostcmd->command = cpu_to_le16(HostCmd_CMD_CHANNEL_REPORT);
    hostcmd->size = cmdlen;
    hostcmd->seq_num = 0;
    hostcmd->result = 0;

    pChanRptReq = (HostCmd_DS_CHAN_RPT_REQ *) (buf + sizeof(HostCmd_DS_GEN));
    pChanRptRsp = (HostCmd_DS_CHAN_RPT_RSP *) (buf + sizeof(HostCmd_DS_GEN));

    memset((void *) pChanRptReq, 0x00, sizeof(HostCmd_DS_CHAN_RPT_REQ));

    pChanRptReq->chanNum = chanNum;
    pChanRptReq->millisecDwellTime = duration;

    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.flags = 0;
    iwr.u.data.pointer = (caddr_t) buf;
    iwr.u.data.length = cmdlen;

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("mlanconfig: chanrpt hostcmd ioctl");
        free(buf);
        return -EFAULT;
    }

    /* TSF is a t_u64, some formatted printing libs have trouble printing long
       longs, so cast and dump as bytes */
    pByte = (t_u8 *) & pChanRptRsp->startTsf;

    pByte = pChanRptRsp->tlvBuffer;

    respLen = le16_to_cpu(hostcmd->size) - sizeof(HostCmd_DS_GEN);

    respLen -= sizeof(pChanRptRsp->commandResult);
    respLen -= sizeof(pChanRptRsp->startTsf);
    respLen -= sizeof(pChanRptRsp->duration);

    pByte = pChanRptRsp->tlvBuffer;

    while (respLen >= sizeof(pTlvHdr->header)) {
        pTlvHdr = (MrvlIEtypes_Data_t *) pByte;
        pTlvHdr->header.len = le16_to_cpu(pTlvHdr->header.len);

        switch (le16_to_cpu(pTlvHdr->header.type)) {
        case TLV_TYPE_CHANRPT_CHAN_LOAD:
            pLoadRpt = (MrvlIEtypes_ChanRptChanLoad_t *) pTlvHdr;
            *pLoadPercent = (pLoadRpt->ccaBusyFraction * 100) / 255;
            break;

        case TLV_TYPE_CHANRPT_NOISE_HIST:
            pNoiseRpt = (MrvlIEtypes_ChanRptNoiseHist_t *) pTlvHdr;
            *pAnpi = pNoiseRpt->anpi;
            break;

        default:
            break;
        }

        pByte += (pTlvHdr->header.len + sizeof(pTlvHdr->header));
        respLen -= (pTlvHdr->header.len + sizeof(pTlvHdr->header));
        respLen = (respLen > 0) ? respLen : 0;
    }

    free(buf);

    return MLAN_STATUS_SUCCESS;
}

static void
chanrpt_print_xAxis(ChanRptInfo_t * pChanRpt, int numChans)
{
    int idx;

    printf("   `-");

    for (idx = 0; idx < numChans; idx++) {
        printf("----");
    }

    printf("\n     ");

    for (idx = 0; idx < numChans; idx++) {
        printf("%03d ", (pChanRpt + idx)->chanNum);
    }

    printf("\n");
}

static void
chanrpt_print_anpi(ChanRptInfo_t * pChanRpt, int numChans)
{
    int dumpIdx;
    int yAxis;
    int yPrint;

    printf("\n");
    printf("                  Average Noise Power Indicator\n");
    printf("                  -----------------------------\n");

    yPrint = 0;
    for (yAxis = -55; yAxis >= -95; yAxis -= 2) {
        if (yPrint % 2 == 1) {
            printf("%2d| ", yAxis);
        } else {
            printf("   | ");
        }

        yPrint++;

        for (dumpIdx = 0; dumpIdx < numChans; dumpIdx++) {
            if ((pChanRpt + dumpIdx)->anpi >= yAxis) {
                printf("### ");
            } else if ((pChanRpt + dumpIdx)->anpi >= yAxis - 2) {
                printf("%3d ", (pChanRpt + dumpIdx)->anpi);
            } else {
                printf("    ");
            }
        }

        printf("\n");
    }

    chanrpt_print_xAxis(pChanRpt, numChans);
}

static void
chanrpt_print_chanLoad(ChanRptInfo_t * pChanRpt, int numChans)
{
    int dumpIdx;
    int yAxis;
    int yPrint;

    printf("\n");
    printf("                         Channel Load\n");
    printf("                         ------------\n");

    yPrint = 0;
    for (yAxis = 100; yAxis >= 0; yAxis -= 5) {
        if (yPrint % 2 == 1) {
            printf("%2d%%| ", yAxis);
        } else {
            printf("   | ");
        }

        yPrint++;

        for (dumpIdx = 0; dumpIdx < numChans; dumpIdx++) {
            if ((pChanRpt + dumpIdx)->chanLoad >= yAxis) {
                printf("### ");
            } else if ((pChanRpt + dumpIdx)->chanLoad >= yAxis - 5) {
                printf("%2d%% ", (pChanRpt + dumpIdx)->chanLoad);
            } else {
                printf("    ");
            }
        }

        printf("\n");
    }
    chanrpt_print_xAxis(pChanRpt, numChans);
}

static void
chanrpt_graph(void)
{
    int idx;
    ChanRptInfo_t chanRpt[14];

    for (idx = 0; idx < NELEMENTS(chanRpt); idx++) {
        chanRpt[idx].chanNum = idx + 1;
        chanrpt_getValues(idx + 1,
                          100, &chanRpt[idx].anpi, &chanRpt[idx].chanLoad);
    }

    chanrpt_print_anpi(chanRpt, NELEMENTS(chanRpt));
    chanrpt_print_chanLoad(chanRpt, NELEMENTS(chanRpt));
}

static void
chanrpt_graph_loop(boolean loopOnLoad, boolean loopOnAnpi, int loops)
{
    int idx;
    int loopsLeft;
    ChanRptInfo_t chanRpt[14];
    ChanRptInfo_t chanRptAvg[14];

    for (idx = 0; idx < NELEMENTS(chanRpt); idx++) {
        chanRpt[idx].chanNum = idx + 1;
        chanrpt_getValues(idx + 1,
                          100, &chanRpt[idx].anpi, &chanRpt[idx].chanLoad);
    }

    idx = 0;
    loopsLeft = loops;

    while (loopsLeft) {
        chanRpt[idx].chanNum = idx + 1;
        chanrpt_getValues(idx + 1,
                          75, &chanRpt[idx].anpi, &chanRpt[idx].chanLoad);

        chanRptAvg[idx].chanNum = idx + 1;
        chanRptAvg[idx].anpi = (chanRptAvg[idx].anpi * (loops - loopsLeft)
                                + chanRpt[idx].anpi) / (loops - loopsLeft + 1);

        chanRptAvg[idx].chanLoad
            = (chanRptAvg[idx].chanLoad * (loops - loopsLeft)
               + chanRpt[idx].chanLoad) / (loops - loopsLeft + 1);

        if (loopOnAnpi) {
            chanrpt_print_anpi(chanRpt, NELEMENTS(chanRpt));
        }

        if (loopOnLoad) {
            chanrpt_print_chanLoad(chanRpt, NELEMENTS(chanRpt));
        }

        idx = (idx + 1) % NELEMENTS(chanRpt);

        if (idx == 0) {
            loopsLeft--;
        }
    }

    if (loopOnAnpi) {
        chanrpt_print_anpi(chanRptAvg, NELEMENTS(chanRptAvg));
    }

    if (loopOnLoad) {
        chanrpt_print_chanLoad(chanRptAvg, NELEMENTS(chanRptAvg));
    }
}

/********************************************************
		Global Functions
********************************************************/

/** 
 *  @brief  select the table's regclass 
 *
 *  @param table_str  Reg channel table type
 *  @param pTable     Pointer to the Reg channel table
 *
 *  @return           TRUE if success otherwise FALSE
 */
boolean
reg_class_table_select(char *table_str, reg_chan_table_e * pTable)
{
    boolean retval = TRUE;

    if (strcmp(table_str, "user") == 0) {
        *pTable = REGTABLE_USER;
    } else if ((strcmp(table_str, "md") == 0)
               || (strncmp(table_str, "multidomain", 5) == 0)) {
        *pTable = REGTABLE_MULTIDOMAIN;
    } else if (strcmp(table_str, "ess") == 0) {
        *pTable = REGTABLE_ESS;
    } else if (strcmp(table_str, "default") == 0) {
        *pTable = REGTABLE_DEFAULT;
    } else {                    /* If no option/wrong option set to default */
        *pTable = REGTABLE_DEFAULT;
    }

    return retval;
}

/** 
 *  @brief Process sub command
 *  
 *  @param sub_cmd      Sub command
 *  @param num_sub_cmds Number of subcommands
 *  @param argc         Number of arguments
 *  @param argv         A pointer to arguments array    
 *
 *  @return             MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
int
process_sub_cmd(sub_cmd_exec_t * sub_cmd, int num_sub_cmds,
                int argc, char *argv[])
{
    int idx;
    boolean invalid_cmd = TRUE;
    int ret = MLAN_STATUS_FAILURE;

    if (argv[3]) {
        for (idx = 0; idx < num_sub_cmds; idx++) {
            if (strncmp(argv[3],
                        sub_cmd[idx].str,
                        MAX(sub_cmd[idx].match_len, strlen(argv[3]))) == 0) {
                invalid_cmd = FALSE;
                ret = sub_cmd[idx].callback(argc - 4, argv + 4);
            }
        }
    }

    if (invalid_cmd) {
        printf("\nUnknown %s command. Valid subcmds:\n", argv[2]);
        for (idx = 0; idx < num_sub_cmds; idx++) {
            printf("  - %s\n", sub_cmd[idx].str);
        }
        printf("\n");
    }

    return ret;
}

/**
 *  @brief Issue a measurement timing command
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array
 *
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
int
process_measurement(int argc, char *argv[])
{
    int ioctl_val, subioctl_val;
    struct iwreq iwr;
    HostCmd_DS_GEN *hostcmd;
    t_u8 *buf, *pos;
    HostCmd_DS_MEASUREMENT_Timing *cfg_timing_cmd;
    MrvlIETypes_MeasTiming_t *cfg_timing_tlv;
    int idx, cmd_len, rsp_len;
    t_u8 sel;
    t_u16 tlv_len;

    if ((argc < 4) ||
        strncmp(argv[3], "timing", MAX(strlen("timing"), strlen(argv[3])))) {

        printf("\nUnknown %s command. Valid subcmd: timing \n", argv[2]);
        return MLAN_STATUS_FAILURE;
    }

    timing_sel_t sel_str[] = { {"disconnected", 1}
    ,
    {"adhoc", 1}
    ,
    {"fullpower", 1}
    ,
    {"ieeeps", 1}
    ,
    {"periodic", 1}
    };

    if (get_priv_ioctl("hostcmd",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    cmd_len = (sizeof(HostCmd_DS_GEN) + sizeof(t_u16)
               + sizeof(MrvlIETypes_MeasTiming_t) * 4);

    buf = (t_u8 *) calloc(sizeof(t_u8), cmd_len);

    if (!buf) {
        printf("Memory allocation failed\n");
        return -ENOMEM;
    }

    hostcmd = (HostCmd_DS_GEN *) buf;
    hostcmd->command = cpu_to_le16(HostCmd_CMD_MEASUREMENT_TIMING_CONFIG);
    hostcmd->size = (sizeof(HostCmd_DS_GEN) + sizeof(t_u16));
    hostcmd->seq_num = 0;
    hostcmd->result = 0;
    pos = buf + sizeof(HostCmd_DS_GEN);

    cfg_timing_cmd = (HostCmd_DS_MEASUREMENT_Timing *) pos;
    if (argc == 7) {
        cfg_timing_cmd->action = cpu_to_le16(HostCmd_ACT_GEN_SET);
    } else {
        cfg_timing_cmd->action = cpu_to_le16(HostCmd_ACT_GEN_GET);
    }

    cfg_timing_tlv = (MrvlIETypes_MeasTiming_t *) cfg_timing_cmd->tlv_buffer;

    if (argc == 7) {
        cfg_timing_tlv->header.type = cpu_to_le16(TLV_TYPE_MEASUREMENT_TIMING);
        cfg_timing_tlv->header.len = (sizeof(MrvlIETypes_MeasTiming_t)
                                      - sizeof(cfg_timing_tlv->header));

        for (idx = 1; idx < NELEMENTS(sel_str); idx++) {
            if (strncmp(argv[4], sel_str[idx].str, sel_str[idx].match_len) == 0) {
                sel = idx + 1;
                break;
            }
        }

        if (idx == NELEMENTS(sel_str)) {
            printf("Wrong argument for mode selected \"%s\"\n", argv[4]);
            free(buf);
            return -EINVAL;
        }

        cfg_timing_tlv->mode = cpu_to_le32(sel);
        cfg_timing_tlv->max_off_channel = cpu_to_le32(atoi(argv[5]));
        cfg_timing_tlv->max_on_channel = cpu_to_le32(atoi(argv[6]));

        hostcmd->size += sizeof(MrvlIETypes_MeasTiming_t);
    }

    hostcmd->size = cpu_to_le16(hostcmd->size);

    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.flags = 0;
    iwr.u.data.pointer = (caddr_t) buf;
    iwr.u.data.length = cmd_len;

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("mlanconfig: measurement timing ioctl");
        free(buf);
        return -EFAULT;
    }

    printf("--------------------------------------------------\n");
    printf("%44s\n", "Measurement Timing Profiles (in ms)");
    printf("--------------------------------------------------\n");
    printf("     Profile    |  MaxOffChannel |  MaxOnChannel\n");
    printf("--------------------------------------------------\n");

    /* Changed to TLV parsing */
    rsp_len = le16_to_cpu(hostcmd->size);
    rsp_len -= (sizeof(HostCmd_DS_GEN) + sizeof(t_u16));
    pos = buf + sizeof(HostCmd_DS_GEN) + sizeof(t_u16);
    while (rsp_len > sizeof(MrvlIEtypesHeader_t)) {
        switch (le16_to_cpu(*(t_u16 *) (pos))) {
        case TLV_TYPE_MEASUREMENT_TIMING:
            cfg_timing_tlv = (MrvlIETypes_MeasTiming_t *) pos;
            tlv_len = le16_to_cpu(cfg_timing_tlv->header.len);
            printf("%15s | %14d | %13d\n",
                   sel_str[le32_to_cpu(cfg_timing_tlv->mode) - 1].str,
                   (int) le32_to_cpu(cfg_timing_tlv->max_off_channel),
                   (int) le32_to_cpu(cfg_timing_tlv->max_on_channel));
            break;
        }
        pos += tlv_len + sizeof(MrvlIEtypesHeader_t);
        rsp_len -= tlv_len + sizeof(MrvlIEtypesHeader_t);
        rsp_len = (rsp_len > 0) ? rsp_len : 0;
    }

    printf("\n");
    free(buf);

    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Issue a profile command 
 *  
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
int
process_profile_entry(int argc, char *argv[])
{
    unsigned int mac[ETH_ALEN] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    int tmpIdx;
    char *ssid = NULL;
    struct iwreq iwr;
    int i = 0;
    t_u8 *buffer;
    t_u16 temp, tempc;
    int ioctl_val, subioctl_val;

    if ((strncmp(argv[3], "delete", sizeof("delete"))) &&
        (strncmp(argv[3], "flush", sizeof("flush")))) {
        return profile_read_download(argv[3]);
    }
    /* initialize BSSID with all FFs, indicate flushall */
    for (i = 0; i < ETH_ALEN; i++) {
        mac[i] = 0xFF;
    }

    if (argc > 4) {
        if (strncmp(argv[4], "bssid=", strlen("bssid=")) == 0) {
            /* 
             *  "bssid" token string handler
             */
            sscanf(argv[4] + strlen("bssid="), "%2x:%2x:%2x:%2x:%2x:%2x",
                   mac + 0, mac + 1, mac + 2, mac + 3, mac + 4, mac + 5);
        } else if (strncmp(argv[4], "ssid=", strlen("ssid=")) == 0) {
            /* 
             *  "ssid" token string handler
             */
            ssid = argv[4] + strlen("ssid=");
        }
    } else {
        printf("Error: missing required option for command (ssid, bssid)\n");
        return -ENOMEM;
    }

    buffer = (t_u8 *) malloc(MRVDRV_SIZE_OF_CMD_BUFFER);
    if (buffer == NULL) {
        printf("Error: allocate memory for profile command failed\n");
        return -ENOMEM;
    }

    i = 0;
    memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);
    printf("Driver profile delete request\n");

    /* Cmd Header : Command */
    tempc = cpu_to_le16(HostCmd_CMD_PROFILEDB);
    memcpy((buffer + i), &tempc, sizeof(t_u16));
    i = sizeof(HostCmd_DS_GEN);

    /* set action as del */
    tempc = cpu_to_le16(HostCmd_ACT_GEN_REMOVE);
    memcpy((buffer + i), &tempc, sizeof(t_u16));
    i += 2;

    /* ssid */
    if (ssid) {
        printf("For ssid %s\n", ssid);
        tempc = cpu_to_le16(TLV_TYPE_SSID);
        memcpy((buffer + i), &tempc, sizeof(t_u16));
        i += 2;
        temp = strlen((char *) ssid);
        tempc = cpu_to_le16(temp);
        memcpy((buffer + i), &tempc, sizeof(t_u16));
        i += 2;
        memcpy((void *) (buffer + i), ssid, temp);
        i += temp;
    } else {

        /* bssid */
        if (mac[0] != 0xFF) {
            printf("For bssid %02x:%02x:%02x:%02x:%02x:%02x\n",
                   mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        }
        tempc = cpu_to_le16(TLV_TYPE_BSSID);
        memcpy((buffer + i), &tempc, sizeof(t_u16));
        i += 2;
        temp = ETH_ALEN;
        tempc = cpu_to_le16(temp);
        memcpy((buffer + i), &tempc, sizeof(t_u16));
        i += 2;
        for (tmpIdx = 0; tmpIdx < NELEMENTS(mac); tmpIdx++) {
            buffer[i + tmpIdx] = (t_u8) mac[tmpIdx];
        }
        i += temp;
    }

    if (get_priv_ioctl("hostcmd",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        free(buffer);
        return -EOPNOTSUPP;
    }

    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.pointer = (caddr_t) buffer;
    iwr.u.data.length = i;
    iwr.u.data.flags = 0;

    /* update length in the header now */
    temp = i;
    tempc = cpu_to_le16(temp);
    memcpy((buffer + 2), &tempc, sizeof(t_u16));

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("hostcmd : profiledb ioctl");
        free(buffer);
        return MLAN_STATUS_FAILURE;
    } else {
        /* 
         * Header: Cmd, Size, Seqnum, Result 
         * Offload of Result = 2+2+2 = 6
         */
        memcpy(&temp, ((t_u8 *) iwr.u.data.pointer) + 6, sizeof(t_u16));
        temp = le16_to_cpu(temp);
        if (temp != 0) {
            printf("hostcmd : profiledb ioctl failure, code %d\n", temp);
            free(buffer);
            return MLAN_STATUS_FAILURE;
        }
    }

    free(buffer);
    return MLAN_STATUS_SUCCESS;
}

int
process_chanrpt(int argc, char *argv[])
{
    int ioctl_val, subioctl_val, cmdlen;
    struct iwreq iwr;
    t_u8 *buf;
    t_u8 *pByte;
    t_u8 numBins;
    t_u8 idx;
    int respLen;
    MrvlIEtypes_Data_t *pTlvHdr;
    HostCmd_DS_GEN *hostcmd;
    HostCmd_DS_CHAN_RPT_RSP *pChanRptRsp;
    HostCmd_DS_CHAN_RPT_REQ *pChanRptReq;

    MrvlIEtypes_ChanRptBcn_t *pBcnRpt;
    MrvlIEtypes_ChanRptChanLoad_t *pLoadRpt;
    MrvlIEtypes_ChanRptNoiseHist_t *pNoiseRpt;
    MrvlIEtypes_ChanRpt11hBasic_t *pBasicRpt;
    MrvlIEtypes_ChanRptFrame_t *pFrameRpt;

    if (get_priv_ioctl("hostcmd",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    buf = (t_u8 *) calloc(sizeof(t_u8), 1000);
    if (!buf) {
        printf("Memory allocation failed\n");
        return -ENOMEM;
    }

    cmdlen = sizeof(HostCmd_DS_GEN) + sizeof(HostCmd_DS_CHAN_RPT_REQ);

    hostcmd = (HostCmd_DS_GEN *) buf;
    hostcmd->command = cpu_to_le16(HostCmd_CMD_CHANNEL_REPORT);
    hostcmd->size = cmdlen;
    hostcmd->seq_num = 0;
    hostcmd->result = 0;

    pChanRptReq = (HostCmd_DS_CHAN_RPT_REQ *) (buf + sizeof(HostCmd_DS_GEN));
    pChanRptRsp = (HostCmd_DS_CHAN_RPT_RSP *) (buf + sizeof(HostCmd_DS_GEN));

    memset((void *) pChanRptReq, 0x00, sizeof(HostCmd_DS_CHAN_RPT_REQ));

    if ((argc != 5) && (argc != 7)) {
        printf("\nchanrpt syntax:"
               " chanrpt <chan#> <millisecs> [<regDomain> <regClass>]\n\n");
        return MLAN_STATUS_FAILURE;
    }

    pChanRptReq->chanNum = atoi(argv[3]);
    pChanRptReq->millisecDwellTime = atoi(argv[4]);

    if (argc == 7) {
        pChanRptReq->regDomain = atoi(argv[5]);
        pChanRptReq->regulatoryClass = atoi(argv[6]);
    }

    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.flags = 0;
    iwr.u.data.pointer = (caddr_t) buf;
    iwr.u.data.length = cmdlen;

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("mlanconfig: chanrpt hostcmd ioctl");
        free(buf);
        return -EFAULT;
    }

    /* TSF is a t_u64, some formatted printing libs have trouble printing long
       longs, so cast and dump as bytes */
    pByte = (t_u8 *) & pChanRptRsp->startTsf;

    printf("\n");
    printf("[%03d]      TSF: 0x%02x%02x%02x%02x%02x%02x%02x%02x\n",
           atoi(argv[3]),
           pByte[7], pByte[6], pByte[5], pByte[4],
           pByte[3], pByte[2], pByte[1], pByte[0]);
    printf("[%03d]    Dwell: %u us\n",
           atoi(argv[3]), (unsigned int) pChanRptRsp->duration);

    pByte = pChanRptRsp->tlvBuffer;

    respLen = le16_to_cpu(hostcmd->size) - sizeof(HostCmd_DS_GEN);

    respLen -= sizeof(pChanRptRsp->commandResult);
    respLen -= sizeof(pChanRptRsp->startTsf);
    respLen -= sizeof(pChanRptRsp->duration);

    pByte = pChanRptRsp->tlvBuffer;

    while (respLen >= sizeof(pTlvHdr->header)) {
        pTlvHdr = (MrvlIEtypes_Data_t *) pByte;
        pTlvHdr->header.len = le16_to_cpu(pTlvHdr->header.len);

        switch (le16_to_cpu(pTlvHdr->header.type)) {
        case TLV_TYPE_CHANRPT_BCN:
            pBcnRpt = (MrvlIEtypes_ChanRptBcn_t *) pTlvHdr;
            printf("[%03d]   Beacon: scanReqId = %d\n",
                   atoi(argv[3]), pBcnRpt->scanReqId);

            break;

        case TLV_TYPE_CHANRPT_CHAN_LOAD:
            pLoadRpt = (MrvlIEtypes_ChanRptChanLoad_t *) pTlvHdr;
            printf("[%03d] ChanLoad: %d%%\n",
                   atoi(argv[3]), (pLoadRpt->ccaBusyFraction * 100) / 255);
            break;

        case TLV_TYPE_CHANRPT_NOISE_HIST:
            pNoiseRpt = (MrvlIEtypes_ChanRptNoiseHist_t *) pTlvHdr;
            numBins = pNoiseRpt->header.len - sizeof(pNoiseRpt->anpi);
            printf("[%03d]     ANPI: %d dB\n", atoi(argv[3]), pNoiseRpt->anpi);
            printf("[%03d] NoiseHst:", atoi(argv[3]));
            for (idx = 0; idx < numBins; idx++) {
                printf(" %03d", pNoiseRpt->rpiDensities[idx]);
            }
            printf("\n");
            break;

        case TLV_TYPE_CHANRPT_11H_BASIC:
            pBasicRpt = (MrvlIEtypes_ChanRpt11hBasic_t *) pTlvHdr;
            printf("[%03d] 11hBasic: BSS(%d), OFDM(%d), UnId(%d), Radar(%d): "
                   "[0x%02x]\n",
                   atoi(argv[3]),
                   pBasicRpt->map.BSS,
                   pBasicRpt->map.OFDM_Preamble,
                   pBasicRpt->map.Unidentified,
                   pBasicRpt->map.Radar, *(t_u8 *) & pBasicRpt->map);
            break;

        case TLV_TYPE_CHANRPT_FRAME:
            pFrameRpt = (MrvlIEtypes_ChanRptFrame_t *) pTlvHdr;
            printf("[%03d]    Frame: %02x:%02x:%02x:%02x:%02x:%02x  "
                   "%02x:%02x:%02x:%02x:%02x:%02x  %3d   %02d\n",
                   atoi(argv[3]),
                   pFrameRpt->sourceAddr[0], pFrameRpt->sourceAddr[1],
                   pFrameRpt->sourceAddr[2], pFrameRpt->sourceAddr[3],
                   pFrameRpt->sourceAddr[4], pFrameRpt->sourceAddr[5],
                   pFrameRpt->bssid[0], pFrameRpt->bssid[1],
                   pFrameRpt->bssid[2], pFrameRpt->bssid[3],
                   pFrameRpt->bssid[4], pFrameRpt->bssid[5],
                   pFrameRpt->rssi, pFrameRpt->frameCnt);
            break;

        default:
            printf("[%03d] Other: Id=0x%x, Size = %d\n",
                   atoi(argv[3]), pTlvHdr->header.type, pTlvHdr->header.len);

            break;
        }

        pByte += (pTlvHdr->header.len + sizeof(pTlvHdr->header));
        respLen -= (pTlvHdr->header.len + sizeof(pTlvHdr->header));
        respLen = (respLen > 0) ? respLen : 0;
    }

    printf("\n");

    free(buf);

    return MLAN_STATUS_SUCCESS;
}

int
process_chan_graph(int argc, char *argv[])
{
    if (argc == 3) {
        chanrpt_graph();
    } else if (argc == 5) {
        if (strcmp(argv[3], "load") == 0) {
            chanrpt_graph_loop(TRUE, FALSE, atoi(argv[4]));
        } else if (strcmp(argv[3], "anpi") == 0) {
            chanrpt_graph_loop(FALSE, TRUE, atoi(argv[4]));
        } else if (strcmp(argv[3], "anpiload") == 0) {
            chanrpt_graph_loop(TRUE, TRUE, atoi(argv[4]));
        } else {
            printf("\nchangraph syntax:"
                   " changraph <load | anpi | anpiload> <loops>\n\n");
        }
    }

    return MLAN_STATUS_SUCCESS;
}

int
process_assoc_timing(int argc, char *argv[])
{
    int ioctl_val, subioctl_val, cmdlen;
    struct iwreq iwr;
    t_u8 *buf, *pos;
    HostCmd_DS_GEN *hostcmd;
    HostCmd_DS_AssociationTiming_t *assoctiming;

    if (get_priv_ioctl("hostcmd",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    cmdlen = sizeof(HostCmd_DS_GEN) + sizeof(HostCmd_DS_AssociationTiming_t);

    buf = (t_u8 *) calloc(sizeof(t_u8), cmdlen);
    if (!buf) {
        printf("Memory allocation failed\n");
        return -ENOMEM;
    }

    hostcmd = (HostCmd_DS_GEN *) buf;
    hostcmd->command = cpu_to_le16(HostCmd_CMD_ASSOCIATION_TIMING);
    hostcmd->size = cpu_to_le16(cmdlen);
    hostcmd->seq_num = 0;
    hostcmd->result = 0;
    pos = buf + sizeof(HostCmd_DS_GEN);

    assoctiming = (HostCmd_DS_AssociationTiming_t *) pos;
    assoctiming->Action = cpu_to_le16(HostCmd_ACT_GEN_GET);

    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.flags = 0;
    iwr.u.data.pointer = (caddr_t) buf;
    iwr.u.data.length = cmdlen;

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("mlanconfig: scanAgentIoctl ioctl");
        free(buf);
        return -EFAULT;
    }

    if (argc > 3) {
        assoctiming->Action = cpu_to_le16(HostCmd_ACT_GEN_SET);
        switch (argc) {
        case 8:
            assoctiming->PriorApDeauthDelay = cpu_to_le16(atoi(argv[7]));
        case 7:
            assoctiming->FrameExchangeTimeout = cpu_to_le16(atoi(argv[6]));
        case 6:
            assoctiming->HandShakeTiming = cpu_to_le16(atoi(argv[5]));
        case 5:
            assoctiming->ReassocTiming = cpu_to_le16(atoi(argv[4]));
        case 4:
            assoctiming->AssocTiming = cpu_to_le16(atoi(argv[3]));
        }

        strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
        iwr.u.data.flags = 0;
        iwr.u.data.pointer = (caddr_t) buf;
        iwr.u.data.length = cmdlen;

        if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
            perror("mlanconfig: scanAgentIoctl ioctl");
            free(buf);
            return -EFAULT;
        }
    }

    puts("");
    printf("------------------------------------------------\n");
    printf("        Association Timing Parameters\n");
    printf("------------------------------------------------\n");

    printf("Association Timeout    %4lu ms\n"
           "Reassociation Timeout  %4lu ms\n"
           "Handshake Timeout      %4lu ms\n"
           "Frame Exchange Timeout %4lu ms\n"
           "Prior AP Deauth Delay  %4lu ms\n",
           assoctiming->AssocTiming,
           assoctiming->ReassocTiming,
           assoctiming->HandShakeTiming,
           assoctiming->FrameExchangeTimeout, assoctiming->PriorApDeauthDelay);
    puts("");

    free(buf);
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief Retrieve the association response from the driver
 *
 *  Retrieve the buffered (re)association management frame from the driver.
 *    The response is identical to the one received from the AP and conforms
 *    to the IEEE specification.
 *
 *  @return MLAN_STATUS_SUCCESS or ioctl error code
 */
int
process_get_assocrsp(void)
{
    int ioctl_val, subioctl_val;
    struct iwreq iwr;
    t_u8 assocRspBuffer[500];   /* Stack buffer can be as large as ioctl allows 
                                 */
    IEEEtypes_AssocRsp_t *pAssocRsp;

    pAssocRsp = (IEEEtypes_AssocRsp_t *) assocRspBuffer;

    if (get_priv_ioctl("getassocrsp",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.pointer = (caddr_t) assocRspBuffer;
    iwr.u.data.length = sizeof(assocRspBuffer);
    iwr.u.data.flags = subioctl_val;

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("mlanconfig: getassocrsp ioctl");
        return -EFAULT;
    }

    if (iwr.u.data.length) {
        printf("getassocrsp: Status[%d], Cap[0x%04x]:\n",
               pAssocRsp->StatusCode, *(t_u16 *) & pAssocRsp->Capability);
        hexdump(NULL, assocRspBuffer, iwr.u.data.length, ' ');
    } else {
        printf("getassocrsp: <empty>\n");
    }

    return MLAN_STATUS_SUCCESS;
}

int
process_link_stats(int argc, char *argv[])
{
    int ioctl_val, subioctl_val, cmdlen;
    struct iwreq iwr;
    t_u8 *buf;
    HostCmd_DS_GEN *hostcmd;
    HostCmd_DS_LINK_STATS_SUMMARY *pStats;

    if (get_priv_ioctl("hostcmd",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    cmdlen = sizeof(HostCmd_DS_GEN) + sizeof(HostCmd_DS_LINK_STATS_SUMMARY);
    buf = (t_u8 *) calloc(sizeof(t_u8), cmdlen);
    if (!buf) {
        printf("Memory allocation failed\n");
        return -ENOMEM;
    }

    hostcmd = (HostCmd_DS_GEN *) buf;
    hostcmd->command = cpu_to_le16(HostCmd_CMD_LINK_STATS_SUMMARY);
    hostcmd->size = cpu_to_le16(sizeof(HostCmd_DS_GEN));

    hostcmd->seq_num = 0;
    hostcmd->result = 0;

    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.flags = 0;
    iwr.u.data.pointer = (caddr_t) buf;
    iwr.u.data.length = cmdlen;

    pStats = (HostCmd_DS_LINK_STATS_SUMMARY *) (buf + sizeof(HostCmd_DS_GEN));

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("mlanconfig: linkstats hostcmd ioctl");
        free(buf);
        return -EFAULT;
    }

    if (argc > 3) {
        if (memcmp(argv[3], "line", strlen("line")) != 0) {
            printf("   dur  bc  bm  br bs"
                   "    rp  rr  rs"
                   "    tp    ta    tf  tr" "   vo  vi  be  bk\n");
        }

        printf("%6u "
               "%3u %3u %3d %2d "
               "%5u %3d %3d"
               " %5u %5u %5u %3s"
               "  %3u %3u %3u %3u\n",
               (int) pStats->timeSinceLastQuery_ms,
               le16_to_cpu(pStats->bcnCnt),
               le16_to_cpu(pStats->bcnMiss),
               le16_to_cpu(pStats->bcnRssiAvg),
               le16_to_cpu(pStats->bcnSnrAvg),
               (int) pStats->rxPkts,
               le16_to_cpu(pStats->rxRssiAvg),
               le16_to_cpu(pStats->rxSnrAvg),
               (int) pStats->txPkts,
               (int) pStats->txAttempts,
               (int) pStats->txFailures,
               rateIdStr[pStats->txInitRate],
               (int) pStats->txQueueDelay[WMM_AC_VO] / 1000,
               (int) pStats->txQueueDelay[WMM_AC_VI] / 1000,
               (int) pStats->txQueueDelay[WMM_AC_BE] / 1000,
               (int) pStats->txQueueDelay[WMM_AC_BK] / 1000);
    } else {
        printf("duration:   %u\n", (int) pStats->timeSinceLastQuery_ms);

        printf("bcnCnt:     %u\n", le16_to_cpu(pStats->bcnCnt));
        printf("bcnMiss:    %u\n", le16_to_cpu(pStats->bcnMiss));
        printf("bcnRssiAvg: %d\n", le16_to_cpu(pStats->bcnRssiAvg));
        printf("bcnSnrAvg:  %d\n", le16_to_cpu(pStats->bcnSnrAvg));

        printf("rxPkts:     %u\n", (int) pStats->rxPkts);
        printf("rxRssiAvg:  %d\n", le16_to_cpu(pStats->rxRssiAvg));
        printf("rxSnrAvg:   %d\n", le16_to_cpu(pStats->rxSnrAvg));

        printf("txPkts:     %u\n", (int) pStats->txPkts);
        printf("txAttempts: %u\n", (int) pStats->txAttempts);
        printf("txFailures: %u\n", (int) pStats->txFailures);
        printf("txInitRate: %s\n", rateIdStr[pStats->txInitRate]);

        printf("txAcVo:     %u [ %u ]\n",
               le16_to_cpu(pStats->txQueuePktCnt[WMM_AC_VO]),
               (int) pStats->txQueueDelay[WMM_AC_VO] / 1000);
        printf("txAcVi:     %u [ %u ]\n",
               le16_to_cpu(pStats->txQueuePktCnt[WMM_AC_VI]),
               (int) pStats->txQueueDelay[WMM_AC_VI] / 1000);
        printf("txAcBe:     %u [ %u ]\n",
               le16_to_cpu(pStats->txQueuePktCnt[WMM_AC_BE]),
               (int) pStats->txQueueDelay[WMM_AC_BE] / 1000);
        printf("txAcBk:     %u [ %u ]\n",
               le16_to_cpu(pStats->txQueuePktCnt[WMM_AC_BK]),
               (int) pStats->txQueueDelay[WMM_AC_BK] / 1000);
    }

    free(buf);

    return MLAN_STATUS_SUCCESS;
}
