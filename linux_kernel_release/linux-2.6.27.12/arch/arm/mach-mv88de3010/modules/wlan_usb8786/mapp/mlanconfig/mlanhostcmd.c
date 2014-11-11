/** @file  mlanhostcmd.c
  * @brief This file contains mlanconfig helper functions
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
#include    <ctype.h>
#include    <string.h>
#include    <stdlib.h>
#include	"mlanconfig.h"
#include	"mlanhostcmd.h"

#ifndef MIN
/** Find minimum value */
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif /* MIN */

/********************************************************
		Local Variables
********************************************************/

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
hexval(t_s32 chr)
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
static t_void
hexdump(t_s8 * prompt, t_void * p, t_s32 len, t_s8 delim)
{
    t_s32 i;
    t_u8 *s = p;

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
static t_u8
hexc2bin(t_s8 chr)
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
static t_u32
a2hex(t_s8 * s)
{
    t_u32 val = 0;
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
static t_u32
a2hex_or_atoi(t_s8 * value)
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
t_s8 *
convert2hex(t_s8 * ptr, t_u8 * chr)
{
    t_u8 val;

    for (val = 0; *ptr && isxdigit(*ptr); ptr++) {
        val = (val * 16) + hexval(*ptr);
    }

    *chr = val;

    return ptr;
}

/** 
 *  @brief get hostcmd data
 *  
 *  @param ln			A pointer to line number
 *  @param buf			A pointer to hostcmd data
 *  @param size			A pointer to the return size of hostcmd buffer
 *  @return      		MLAN_STATUS_SUCCESS
 */
static int
mlan_get_hostcmd_data(int *ln, t_u8 * buf, t_u16 * size)
{
    t_s32 errors = 0, i;
    t_s8 line[256], *pos, *pos1, *pos2, *pos3;
    t_u16 len;

    while ((pos = mlan_config_get_line(line, sizeof(line), ln))) {
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
                    *buf++ = (t_u8) a2hex_or_atoi(pos2);
                    pos2 = pos3 + 1;
                } else
                    *buf++ = 0;
            }
        } else if (*pos2 == '{') {
            t_u16 *tlvlen = (t_u16 *) buf;
            mlan_get_hostcmd_data(ln, buf + len, tlvlen);
            *size += *tlvlen;
            buf += len + *tlvlen;
        } else {
            t_u32 value = a2hex_or_atoi(pos2);
            while (len--) {
                *buf++ = (t_u8) (value & 0xff);
                value >>= 8;
            }
        }
    }
    return MLAN_STATUS_SUCCESS;
}

/********************************************************
		Global Functions
********************************************************/
/** 
 *  @brief Prepare host-command buffer 
 *  @param cmd_name	Command name
 *  @param buf		A pointer to comand buffer    
 *  @return      	MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
int
prepare_host_cmd_buffer(char *cmd_name, t_u8 * buf)
{
    t_s8 line[256], cmdname[256], *pos, cmdcode[10];
    HostCmd_DS_GEN *hostcmd;
    int ln = 0;
    int cmdname_found = 0, cmdcode_found = 0;

    memset(buf, 0, MRVDRV_SIZE_OF_CMD_BUFFER);
    hostcmd = (HostCmd_DS_GEN *) buf;
    hostcmd->command = 0xffff;

    sprintf(cmdname, "%s={", cmd_name);
    cmdname_found = 0;
    while ((pos = mlan_config_get_line(line, sizeof(line), &ln))) {
        if (strcmp(pos, cmdname) == 0) {
            cmdname_found = 1;
            sprintf(cmdcode, "CmdCode=");
            cmdcode_found = 0;
            while ((pos = mlan_config_get_line(line, sizeof(line), &ln))) {
                if (strncmp(pos, cmdcode, strlen(cmdcode)) == 0) {
                    cmdcode_found = 1;
                    hostcmd->command = a2hex_or_atoi(pos + strlen(cmdcode));
                    hostcmd->size = S_DS_GEN;
                    mlan_get_hostcmd_data(&ln, buf + hostcmd->size,
                                          &hostcmd->size);
                    break;
                }
            }
            if (!cmdcode_found) {
                fprintf(stderr, "mlanconfig: CmdCode not found in conf file\n");
                return MLAN_STATUS_FAILURE;
            }
            break;
        }
    }

    if (!cmdname_found) {
        fprintf(stderr, "mlanconfig: cmdname '%s' is not found in conf file\n",
                cmd_name);
        return MLAN_STATUS_FAILURE;
    }

    hostcmd->seq_num = 0;
    hostcmd->result = 0;
    hostcmd->command = cpu_to_le16(hostcmd->command);
    hostcmd->size = cpu_to_le16(hostcmd->size);
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Process host_cmd response
 *  @param buf		A pointer to the response buffer
 *  @return      	MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
int
process_host_cmd_resp(t_u8 * buf)
{
    HostCmd_DS_GEN *hostcmd = (HostCmd_DS_GEN *) buf;
    int ret = MLAN_STATUS_SUCCESS;

    hostcmd->command = le16_to_cpu(hostcmd->command);
    hostcmd->size = le16_to_cpu(hostcmd->size);

    hostcmd->command &= ~HostCmd_RET_BIT;
    if (!le16_to_cpu(hostcmd->result)) {
        switch (hostcmd->command) {
        case HostCmd_CMD_CFG_DATA:
            {
                HostCmd_DS_802_11_CFG_DATA *pstcfgData =
                    (HostCmd_DS_802_11_CFG_DATA *) (buf + S_DS_GEN);
                pstcfgData->data_len = le16_to_cpu(pstcfgData->data_len);
                pstcfgData->action = le16_to_cpu(pstcfgData->action);

                if (pstcfgData->action == HostCmd_ACT_GEN_GET) {
                    hexdump("cfg_data", pstcfgData->data, pstcfgData->data_len,
                            ' ');
                }
                break;
            }
        case HostCmd_CMD_802_11_CRYPTO:
            {
                t_u16 alg =
                    le16_to_cpu((t_u16) * (buf + S_DS_GEN + sizeof(t_u16)));
                if (alg != CIPHER_TEST_AES_CCM) {
                    HostCmd_DS_802_11_CRYPTO *cmd =
                        (HostCmd_DS_802_11_CRYPTO *) (buf + S_DS_GEN);
                    cmd->encdec = le16_to_cpu(cmd->encdec);
                    cmd->algorithm = le16_to_cpu(cmd->algorithm);
                    cmd->key_IV_length = le16_to_cpu(cmd->key_IV_length);
                    cmd->key_length = le16_to_cpu(cmd->key_length);
                    cmd->data.header.type = le16_to_cpu(cmd->data.header.type);
                    cmd->data.header.len = le16_to_cpu(cmd->data.header.len);

                    printf
                        ("crypto_result: encdec=%d algorithm=%d,KeyIVLen=%d, KeyLen=%d,dataLen=%d\n",
                         cmd->encdec, cmd->algorithm, cmd->key_IV_length,
                         cmd->key_length, cmd->data.header.len);
                    hexdump("KeyIV", cmd->keyIV, cmd->key_IV_length, ' ');
                    hexdump("Key", cmd->key, cmd->key_length, ' ');
                    hexdump("Data", cmd->data.data, cmd->data.header.len, ' ');
                } else {
                    HostCmd_DS_802_11_CRYPTO_AES_CCM *cmd_aes_ccm =
                        (HostCmd_DS_802_11_CRYPTO_AES_CCM *) (buf + S_DS_GEN);

                    cmd_aes_ccm->encdec = le16_to_cpu(cmd_aes_ccm->encdec);
                    cmd_aes_ccm->algorithm =
                        le16_to_cpu(cmd_aes_ccm->algorithm);
                    cmd_aes_ccm->key_length =
                        le16_to_cpu(cmd_aes_ccm->key_length);
                    cmd_aes_ccm->nonce_length =
                        le16_to_cpu(cmd_aes_ccm->nonce_length);
                    cmd_aes_ccm->AAD_length =
                        le16_to_cpu(cmd_aes_ccm->AAD_length);
                    cmd_aes_ccm->data.header.type =
                        le16_to_cpu(cmd_aes_ccm->data.header.type);
                    cmd_aes_ccm->data.header.len =
                        le16_to_cpu(cmd_aes_ccm->data.header.len);

                    printf
                        ("crypto_result: encdec=%d algorithm=%d,KeyLen=%d, NonceLen=%d,AADLen=%d,dataLen=%d\n",
                         cmd_aes_ccm->encdec, cmd_aes_ccm->algorithm,
                         cmd_aes_ccm->key_length, cmd_aes_ccm->nonce_length,
                         cmd_aes_ccm->AAD_length, cmd_aes_ccm->data.header.len);
                    hexdump("Key", cmd_aes_ccm->key, cmd_aes_ccm->key_length,
                            ' ');
                    hexdump("Nonce", cmd_aes_ccm->nonce,
                            cmd_aes_ccm->nonce_length, ' ');
                    hexdump("AAD", cmd_aes_ccm->AAD, cmd_aes_ccm->AAD_length,
                            ' ');
                    hexdump("Data", cmd_aes_ccm->data.data,
                            cmd_aes_ccm->data.header.len, ' ');
                }
                break;
            }
        case HostCmd_CMD_802_11_AUTO_TX:
            {
                HostCmd_DS_802_11_AUTO_TX *at =
                    (HostCmd_DS_802_11_AUTO_TX *) (buf + S_DS_GEN);

                if (le16_to_cpu(at->action) == HostCmd_ACT_GEN_GET) {
                    if (S_DS_GEN + sizeof(at->action) == hostcmd->size) {
                        printf("auto_tx not configured\n");
                    } else {
                        MrvlIEtypesHeader_t *header = &at->auto_tx.header;
                        header->type = le16_to_cpu(header->type);
                        header->len = le16_to_cpu(header->len);
                        if ((S_DS_GEN + sizeof(at->action) +
                             sizeof(MrvlIEtypesHeader_t) + header->len ==
                             hostcmd->size) &&
                            (header->type == TLV_TYPE_AUTO_TX)) {
                            AutoTx_MacFrame_t *atmf =
                                &at->auto_tx.auto_tx_mac_frame;
                            printf("Interval: %d second(s)\n",
                                   le16_to_cpu(atmf->interval));
                            printf("Priority: %#x\n", atmf->priority);
                            printf("Frame Length: %d\n",
                                   le16_to_cpu(atmf->frame_len));
                            printf
                                ("Dest Mac Address: %02x:%02x:%02x:%02x:%02x:%02x\n",
                                 atmf->dest_mac_addr[0], atmf->dest_mac_addr[1],
                                 atmf->dest_mac_addr[2], atmf->dest_mac_addr[3],
                                 atmf->dest_mac_addr[4],
                                 atmf->dest_mac_addr[5]);
                            printf
                                ("Src Mac Address: %02x:%02x:%02x:%02x:%02x:%02x\n",
                                 atmf->src_mac_addr[0], atmf->src_mac_addr[1],
                                 atmf->src_mac_addr[2], atmf->src_mac_addr[3],
                                 atmf->src_mac_addr[4], atmf->src_mac_addr[5]);

                            hexdump("Frame Payload", atmf->payload,
                                    le16_to_cpu(atmf->frame_len) -
                                    MLAN_MAC_ADDR_LENGTH * 2, ' ');
                        } else {
                            printf("incorrect auto_tx command response\n");
                        }
                    }
                }
                break;
            }
        case HostCmd_CMD_802_11_SUBSCRIBE_EVENT:
            {
                HostCmd_DS_802_11_SUBSCRIBE_EVENT *se =
                    (HostCmd_DS_802_11_SUBSCRIBE_EVENT *) (buf + S_DS_GEN);
                if (le16_to_cpu(se->action) == HostCmd_ACT_GEN_GET) {
                    int len =
                        S_DS_GEN + sizeof(HostCmd_DS_802_11_SUBSCRIBE_EVENT);
                    printf("\nEvent\t\tValue\tFreq\tsubscribed\n\n");
                    while (len < hostcmd->size) {
                        MrvlIEtypesHeader_t *header =
                            (MrvlIEtypesHeader_t *) (buf + len);
                        switch (le16_to_cpu(header->type)) {
                        case TLV_TYPE_RSSI_LOW:
                            {
                                MrvlIEtypes_RssiThreshold_t *low_rssi =
                                    (MrvlIEtypes_RssiThreshold_t *) (buf + len);
                                printf("Beacon Low RSSI\t%d\t%d\t%s\n",
                                       low_rssi->RSSI_value,
                                       low_rssi->RSSI_freq,
                                       (le16_to_cpu(se->events) & 0x0001) ?
                                       "yes" : "no");
                                break;
                            }
                        case TLV_TYPE_SNR_LOW:
                            {
                                MrvlIEtypes_SnrThreshold_t *low_snr =
                                    (MrvlIEtypes_SnrThreshold_t *) (buf + len);
                                printf("Beacon Low SNR\t%d\t%d\t%s\n",
                                       low_snr->SNR_value,
                                       low_snr->SNR_freq,
                                       (le16_to_cpu(se->events) & 0x0002) ?
                                       "yes" : "no");
                                break;
                            }
                        case TLV_TYPE_FAILCOUNT:
                            {
                                MrvlIEtypes_FailureCount_t *failure_count =
                                    (MrvlIEtypes_FailureCount_t *) (buf + len);
                                printf("Failure Count\t%d\t%d\t%s\n",
                                       failure_count->fail_value,
                                       failure_count->fail_freq,
                                       (le16_to_cpu(se->events) & 0x0004) ?
                                       "yes" : "no");
                                break;
                            }
                        case TLV_TYPE_BCNMISS:
                            {
                                MrvlIEtypes_BeaconsMissed_t *bcn_missed =
                                    (MrvlIEtypes_BeaconsMissed_t *) (buf + len);
                                printf("Beacon Missed\t%d\tN/A\t%s\n",
                                       bcn_missed->beacon_missed,
                                       (le16_to_cpu(se->events) & 0x0008) ?
                                       "yes" : "no");
                                break;
                            }
                        case TLV_TYPE_RSSI_HIGH:
                            {
                                MrvlIEtypes_RssiThreshold_t *high_rssi =
                                    (MrvlIEtypes_RssiThreshold_t *) (buf + len);
                                printf("Bcn High RSSI\t%d\t%d\t%s\n",
                                       high_rssi->RSSI_value,
                                       high_rssi->RSSI_freq,
                                       (le16_to_cpu(se->events) & 0x0010) ?
                                       "yes" : "no");
                                break;
                            }

                        case TLV_TYPE_SNR_HIGH:
                            {
                                MrvlIEtypes_SnrThreshold_t *high_snr =
                                    (MrvlIEtypes_SnrThreshold_t *) (buf + len);
                                printf("Beacon High SNR\t%d\t%d\t%s\n",
                                       high_snr->SNR_value,
                                       high_snr->SNR_freq,
                                       (le16_to_cpu(se->events) & 0x0020) ?
                                       "yes" : "no");
                                break;
                            }
                        case TLV_TYPE_RSSI_LOW_DATA:
                            {
                                MrvlIEtypes_RssiThreshold_t *low_rssi =
                                    (MrvlIEtypes_RssiThreshold_t *) (buf + len);
                                printf("Data Low RSSI\t%d\t%d\t%s\n",
                                       low_rssi->RSSI_value,
                                       low_rssi->RSSI_freq,
                                       (le16_to_cpu(se->events) & 0x0040) ?
                                       "yes" : "no");
                                break;
                            }
                        case TLV_TYPE_SNR_LOW_DATA:
                            {
                                MrvlIEtypes_SnrThreshold_t *low_snr =
                                    (MrvlIEtypes_SnrThreshold_t *) (buf + len);
                                printf("Data Low SNR\t%d\t%d\t%s\n",
                                       low_snr->SNR_value,
                                       low_snr->SNR_freq,
                                       (le16_to_cpu(se->events) & 0x0080) ?
                                       "yes" : "no");
                                break;
                            }
                        case TLV_TYPE_RSSI_HIGH_DATA:
                            {
                                MrvlIEtypes_RssiThreshold_t *high_rssi =
                                    (MrvlIEtypes_RssiThreshold_t *) (buf + len);
                                printf("Data High RSSI\t%d\t%d\t%s\n",
                                       high_rssi->RSSI_value,
                                       high_rssi->RSSI_freq,
                                       (le16_to_cpu(se->events) & 0x0100) ?
                                       "yes" : "no");
                                break;
                            }
                        case TLV_TYPE_SNR_HIGH_DATA:
                            {
                                MrvlIEtypes_SnrThreshold_t *high_snr =
                                    (MrvlIEtypes_SnrThreshold_t *) (buf + len);
                                printf("Data High SNR\t%d\t%d\t%s\n",
                                       high_snr->SNR_value,
                                       high_snr->SNR_freq,
                                       (le16_to_cpu(se->events) & 0x0200) ?
                                       "yes" : "no");
                                break;
                            }
                        case TLV_TYPE_LINK_QUALITY:
                            {
                                MrvlIEtypes_LinkQuality_t *link_qual =
                                    (MrvlIEtypes_LinkQuality_t *) (buf + len);
                                printf("Link Quality Parameters:\n");
                                printf("------------------------\n");
                                printf("Link Quality Event Subscribed\t%s\n",
                                       (le16_to_cpu(se->events) & 0x0400) ?
                                       "yes" : "no");
                                printf("Link SNR Threshold   = %d\n",
                                       le16_to_cpu(link_qual->link_SNR_thrs));
                                printf("Link SNR Frequency   = %d\n",
                                       le16_to_cpu(link_qual->link_SNR_freq));
                                printf("Min Rate Value       = %d\n",
                                       le16_to_cpu(link_qual->min_rate_val));
                                printf("Min Rate Frequency   = %d\n",
                                       le16_to_cpu(link_qual->min_rate_freq));
                                printf("Tx Latency Value     = %ld\n",
                                       le32_to_cpu(link_qual->tx_latency_val));
                                printf("Tx Latency Threshold = %ld\n",
                                       le32_to_cpu(link_qual->tx_latency_thrs));

                                break;
                            }
                        case TLV_TYPE_PRE_BEACON_LOST:
                            {
                                MrvlIEtypes_PreBeaconLost_t *pre_bcn_lost =
                                    (MrvlIEtypes_PreBeaconLost_t *) (buf + len);
                                printf("------------------------\n");
                                printf("Pre-Beacon Lost Event Subscribed\t%s\n",
                                       (le16_to_cpu(se->events) & 0x0800) ?
                                       "yes" : "no");
                                printf("Pre-Beacon Lost: %d\n",
                                       pre_bcn_lost->pre_beacon_lost);

                                break;
                            }
                        default:
                            printf
                                ("unknown subscribed event TLV Type=%#x, Len=%d\n",
                                 le16_to_cpu(header->type),
                                 le16_to_cpu(header->len));
                            break;
                        }
                        len +=
                            sizeof(MrvlIEtypesHeader_t) +
                            le16_to_cpu(header->len);
                    }
                }
                break;
            }

        default:
            printf("HOSTCMD_RESP: ReturnCode=%#04x, Result=%#04x\n",
                   le16_to_cpu(hostcmd->command), le16_to_cpu(hostcmd->result));
            break;
        }
    } else {
        printf("HOSTCMD failed: ReturnCode=%#04x, Result=%#04x\n",
               le16_to_cpu(hostcmd->command), le16_to_cpu(hostcmd->result));
    }
    return ret;
}

/** 
 *  @brief Prepare ARP filter buffer 
 *  @param buf		A pointer to the buffer    
 *  @param length	A pointer to the length of buffer     
 *  @return      	MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
int
prepare_arp_filter_buffer(t_u8 * buf, t_u16 * length)
{
    t_s8 line[256], *pos;
    int ln = 0;
    int ret = MLAN_STATUS_SUCCESS;
    int arpfilter_found = 0;

    memset(buf, 0, MRVDRV_SIZE_OF_CMD_BUFFER);
    while ((pos = mlan_config_get_line(line, sizeof(line), &ln))) {
        if (strcmp(pos, "arpfilter={") == 0) {
            arpfilter_found = 1;
            mlan_get_hostcmd_data(&ln, buf, length);
            break;
        }
    }
    if (!arpfilter_found) {
        fprintf(stderr, "wlanconfig: 'arpfilter' not found in conf file");
        ret = MLAN_STATUS_FAILURE;
    }
    return ret;
}
