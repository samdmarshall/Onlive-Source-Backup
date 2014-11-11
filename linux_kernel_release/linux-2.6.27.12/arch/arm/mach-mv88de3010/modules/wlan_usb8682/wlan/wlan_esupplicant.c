/** @file  wlan_esupplicant.c
  * @brief This file contains user specific private ioctl functions
  *
  * Copyright (C) 2003-2008, Marvell International Ltd.
  * All Rights Reserved
  */
/********************************************************
Change log:
    03/21/08: Created from wlan_esupplicant.c
********************************************************/

#include    "wlan_headers.h"

/** Define PMK_LEN */
#define	PMK_LEN	64

int ascii2hex(u8 * a, char *b, u32 l);

/**
 *  @brief Extension of strsep lib command. This function will also take care
 *	   escape character
 *  @param s         A pointer to array of chars to process
 *  @param delim     The delimiter character to end the string
 *  @param esc       The escape character to ignore for delimiter
 *  @return          Pointer to the seperated string if delim found, else NULL
 */
static char *
wlan_strsep(char **s, char delim, char esc)
{
    char *se = *s, *sb;

    if (*s == NULL)
        return NULL;

    for (sb = *s; *sb != '\0'; ++sb) {
        if (*sb == esc && *(sb + 1) == delim) {
            /* We get a delim + esc seq then keep the delim and chop off the
               esc character */
            memmove(sb, sb + 1, strlen(sb));
            continue;
        }
        if (*sb == delim)
            break;
    }

    if (*sb == '\0')
        sb = NULL;
    else
        *sb++ = '\0';

    *s = sb;

    return se;
}

/**
 *  @brief Return hex value of a give character
 *
 *  @param chr	    character to be converted
 *  @return 	    the converted chanrater if chr is a valid hex, else 0
 */
static int
hexval(char chr)
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
 *  @brief Return hex value of a given ascii string
 *
 *  @param a	    string to be converted to ascii
 *  @return 	    the converted chanrater if a is a valid hex, else 0
 */
static int
atox(char *a)
{
    int i = 0;

    while (isxdigit(*a))
        i = i * 16 + hexval(*a++);

    return i;
}

/**
 *  @brief Convert mac address from string to u8 buffer.
 *
 *  @param mac_addr The buffer to store the mac address in.
 *  @param buf      The source of mac address which is a string.
 *  @return 	    void
 */
static void
mac2u8(u8 * mac_addr, char *buf)
{
    char *begin = buf, *end;
    int i;

    for (i = 0; i < ETH_ALEN; ++i) {
        end = wlan_strsep(&begin, ':', '/');
        mac_addr[i] = atox(end);
    }
}

/**
 *  @brief Set WPA passphrase and SSID
 *
 *  @param priv	    A pointer to wlan_private structure
 *  @param wrq	    A pointer to user data
 *  @return 	    WLAN_STATUS_SUCCESS on Success, Failure cause
 */
int
wlan_setpassphrase(wlan_private * priv, struct iwreq *wrq)
{
    u16 tlv, len = 0;
    static char buf[256];
    char *begin, *end, *tlvbuf, *obuf;
    MrvlIEtypes_PMK_t *pmk_tlv = NULL;
    MrvlIEtypes_Passphrase_t *passphrase_tlv = NULL;
    MrvlIEtypes_SsIdParamSet_t *ssid_tlv = NULL;
    MrvlIEtypes_Bssid_t *bssid_tlv = NULL;
    int ret = 0, action, tlvbuflen, i;

    ENTER();

    if (!wrq->u.data.length) {
        PRINTM(INFO, "Argument missing for setpassphrase\n");
        LEAVE();
        return -EINVAL;
    }

    if (copy_from_user(buf, wrq->u.data.pointer, wrq->u.data.length)) {
        PRINTM(INFO, "Copy from user failed\n");
        LEAVE();
        return -EFAULT;
    }

    buf[wrq->u.data.length] = '\0';

    if (wrq->u.data.length <= 1) {
        PRINTM(ERROR, "No valid arguments\n");
        LEAVE();
        return -EINVAL;
    }

    /* Parse the buf to get the cmd_action */
    begin = buf;
    end = wlan_strsep(&begin, ';', '/');
    action = atox(end);
    if (action < 0 && action > 2) {
        PRINTM(ERROR, "Invalid action argument %d\n", action);
        LEAVE();
        return -EINVAL;
    }

    ret = wlan_prepare_cmd(priv,
                           HostCmd_CMD_SUPPLICANT_PMK, action,
                           HostCmd_OPTION_WAITFORRSP, 0, (u8 *) begin);

    if ((action == HostCmd_ACT_GEN_GET) && begin) {
        tlvbuflen = *(u16 *) begin;
        tlvbuf = (u8 *) & begin[2];
#define MAX_OUTPUT_SIZE 256
        if (!(obuf = kmalloc(MAX_OUTPUT_SIZE, GFP_KERNEL))) {
            PRINTM(ERROR, "ESupp:Allocation for output buffer" "failed\n");
            LEAVE();
            return -ENOMEM;
        }

        do {
            tlv = wlan_le16_to_cpu((*(u16 *) tlvbuf));
            switch (tlv) {
            case TLV_TYPE_SSID:
                ssid_tlv = (MrvlIEtypes_SsIdParamSet_t *) tlvbuf;
                len += sprintf(obuf + len, "ssid:");
                ssid_tlv->Header.Len = wlan_le16_to_cpu(ssid_tlv->Header.Len);
                memcpy(obuf + len, ssid_tlv->SsId, ssid_tlv->Header.Len);
                len += ssid_tlv->Header.Len;
                len += sprintf(obuf + len, " ");
                tlvbuf += ssid_tlv->Header.Len + sizeof(MrvlIEtypesHeader_t);
                tlvbuflen -= (ssid_tlv->Header.Len +
                              sizeof(MrvlIEtypesHeader_t));
                break;
            case TLV_TYPE_PMK:
                pmk_tlv = (MrvlIEtypes_PMK_t *) tlvbuf;
                len += sprintf(obuf + len, "psk:");
                pmk_tlv->Header.Len = wlan_le16_to_cpu(pmk_tlv->Header.Len);
                for (i = 0; i < pmk_tlv->Header.Len; ++i)
                    len += sprintf(obuf + len, "%02x", pmk_tlv->PMK[i]);
                len += sprintf(obuf + len, "\n");
                tlvbuf += pmk_tlv->Header.Len + sizeof(MrvlIEtypesHeader_t);
                tlvbuflen -= (pmk_tlv->Header.Len +
                              sizeof(MrvlIEtypesHeader_t));
                break;
            case TLV_TYPE_PASSPHRASE:
                passphrase_tlv = (MrvlIEtypes_Passphrase_t *) tlvbuf;
                len += sprintf(obuf + len, "passphrase:%s ",
                               passphrase_tlv->Passphrase);
                passphrase_tlv->Header.Len =
                    wlan_le16_to_cpu(passphrase_tlv->Header.Len);
                tlvbuf +=
                    passphrase_tlv->Header.Len + sizeof(MrvlIEtypesHeader_t);
                tlvbuflen -=
                    (passphrase_tlv->Header.Len + sizeof(MrvlIEtypesHeader_t));
                break;
            case TLV_TYPE_BSSID:
                bssid_tlv = (MrvlIEtypes_Bssid_t *) tlvbuf;
                len += sprintf(obuf + len, "bssid:");
                for (i = 0; i < ETH_ALEN - 1; ++i)
                    len += sprintf(obuf + len, "%02x:", bssid_tlv->Bssid[i]);
                len += sprintf(obuf + len, "%02x ", bssid_tlv->Bssid[i]);
                bssid_tlv->Header.Len = wlan_le16_to_cpu(bssid_tlv->Header.Len);
                tlvbuf += bssid_tlv->Header.Len + sizeof(MrvlIEtypesHeader_t);
                tlvbuflen -= (bssid_tlv->Header.Len +
                              sizeof(MrvlIEtypesHeader_t));
                break;
            }
        } while (tlvbuflen > 0);

        if (wrq->u.data.pointer) {
            if (copy_to_user(wrq->u.data.pointer, obuf, len)) {
                PRINTM(FATAL, "Copy to user failed, len %d\n", len);
                ret = -EFAULT;
            }
            wrq->u.data.length = len;
        }

        kfree(obuf);
    }

    LEAVE();

    return ret;
}

/**
 *  @brief Get current security mode (WPA-PSK, WPA2-PSK etc.)
 *  @param priv         A pointer to wlan_private structure
 *  @param req          A pointer to iwreq structure
 *  @return             WLAN_STATUS_SUCCESS --success, otherwise fail
 */
int
wlan_get_rsnmode_ioctl(wlan_private * priv, struct ifreq *req)
{
    struct iwreq *wrq = (struct iwreq *) req;
    u8 buf[sizeof(MrvlIEtypes_EncrProto_t)];
    MrvlIEtypes_EncrProto_t *encrProto;
    int *pdata, ret;

    ENTER();

    if (priv->adapter->MediaConnectStatus != WlanMediaStateConnected) {
        LEAVE();
        return -EOPNOTSUPP;
    }

    encrProto = (MrvlIEtypes_EncrProto_t *) buf;
    encrProto->Header.Type = wlan_cpu_to_le16(TLV_TYPE_ENCRYPTION_PROTO);
    encrProto->Header.Len = wlan_cpu_to_le16(sizeof(u16));
    encrProto->rsnMode = 0;

    if ((ret = wlan_prepare_cmd(priv, HostCmd_CMD_SUPPLICANT_PROFILE,
                                HostCmd_ACT_GEN_GET_CURRENT,
                                HostCmd_OPTION_WAITFORRSP, 0, buf))) {
        PRINTM(ERROR, "Supplicant profile: Error in cmd\n");
        LEAVE();
        return -EFAULT;
    }

    pdata = (int *) wrq->u.name;

    /* 
     * rsnMode -
     *           Bit 0    : No RSN
     *           Bit 1-2  : RFU
     *           Bit 3    : WPA
     *           Bit 4    : WPA-NONE
     *           Bit 5    : WPA2
     *           Bit 6    : AES CCKM
     *           Bit 7-15 : RFU
     */

    *pdata = wlan_le16_to_cpu(encrProto->rsnMode);

    LEAVE();
    return WLAN_STATUS_SUCCESS;
}

/**
 *  @brief Get current active pairwise cipher for WPA/WPA2 mode.
 *  @param priv         A pointer to wlan_private structure
 *  @param req          A pointer to iwreq structure
 *  @return             WLAN_STATUS_SUCCESS --success, otherwise fail
 */
int
wlan_get_currentpairwisecipher_ioctl(wlan_private * priv, struct ifreq *req)
{
    struct iwreq *wrq = (struct iwreq *) req;
    u8 buf[sizeof(MrvlIEtypes_Cipher_t)];
    MrvlIEtypes_Cipher_t *cipher;
    int *pdata, ret;

    ENTER();

    if (priv->adapter->MediaConnectStatus != WlanMediaStateConnected) {
        LEAVE();
        return -EOPNOTSUPP;
    }

    cipher = (MrvlIEtypes_Cipher_t *) buf;
    cipher->Header.Type = wlan_cpu_to_le16(TLV_TYPE_CIPHER);
    cipher->Header.Len = wlan_cpu_to_le16(sizeof(u16));

    if ((ret = wlan_prepare_cmd(priv, HostCmd_CMD_SUPPLICANT_PROFILE,
                                HostCmd_ACT_GEN_GET_CURRENT,
                                HostCmd_OPTION_WAITFORRSP, 0, buf))) {
        PRINTM(ERROR, "Supplicant profile: Error in cmd\n");
        LEAVE();
        return -EFAULT;
    }

    pdata = (int *) wrq->u.name;

    /* 
     * unicastCipher -
     *           Bit 0   : RFU
     *           Bit 1   : RFU
     *           Bit 2   : TKIP
     *           Bit 3   : AES CCKM
     *           Bit 2-7 : RFU
     */

    *pdata = wlan_le16_to_cpu(cipher->pairCipher);

    LEAVE();
    return WLAN_STATUS_SUCCESS;
}

/**
 *  @brief Get current active group cipher for WPA/WPA2 mode.
 *  @param priv         A pointer to wlan_private structure
 *  @param req          A pointer to iwreq structure
 *  @return             WLAN_STATUS_SUCCESS --success, otherwise fail
 */
int
wlan_get_currentgroupcipher_ioctl(wlan_private * priv, struct ifreq *req)
{
    struct iwreq *wrq = (struct iwreq *) req;
    u8 buf[sizeof(MrvlIEtypes_Cipher_t)];
    MrvlIEtypes_Cipher_t *cipher;
    int *pdata, ret;

    ENTER();

    if (priv->adapter->MediaConnectStatus != WlanMediaStateConnected) {
        LEAVE();
        return -EOPNOTSUPP;
    }

    cipher = (MrvlIEtypes_Cipher_t *) buf;
    cipher->Header.Type = wlan_cpu_to_le16(TLV_TYPE_CIPHER);
    cipher->Header.Len = wlan_cpu_to_le16(sizeof(u16));

    if ((ret = wlan_prepare_cmd(priv, HostCmd_CMD_SUPPLICANT_PROFILE,
                                HostCmd_ACT_GEN_GET_CURRENT,
                                HostCmd_OPTION_WAITFORRSP, 0, buf))) {
        PRINTM(ERROR, "Supplicant profile: Error in cmd\n");
        LEAVE();
        return -EFAULT;
    }

    pdata = (int *) wrq->u.name;

    /* 
     * multicastCipher -
     *        Bit 0   : WEP40
     *        Bit 1   : WEP104
     *        Bit 2   : TKIP
     *        Bit 3   : AES
     *        Bit 4-7 : RFU
     */

    *pdata = wlan_le16_to_cpu(cipher->groupCipher);

    LEAVE();
    return WLAN_STATUS_SUCCESS;
}

/**
 *  @brief Handle the supplicant pmk command
 *
 *  @param priv	    A pointer to wlan_private structure
 *  @param cmd	    A pointer to command buffer
 *  @param option   Action
 *  @param buf      Data for the command
 *  @return 	    WLAN_STATUS_SUCCESS on Success, Failure cause
 */
int
wlan_cmd_802_11_supplicant_pmk(wlan_private * priv,
                               HostCmd_DS_COMMAND * cmd, u16 option, void *buf)
{
    MrvlIEtypes_PMK_t *pmk_tlv = NULL;
    MrvlIEtypes_Passphrase_t *passphrase_tlv = NULL;
    MrvlIEtypes_SsIdParamSet_t *ssid_tlv = NULL;
    MrvlIEtypes_Bssid_t *bssid_tlv = NULL;
    HostCmd_DS_802_11_SUPPLICANT_PMK *esupplicant_psk =
        &cmd->params.esupplicant_psk;
    u8 *tlvBuffer = (u8 *) & esupplicant_psk->TlvBuffer;
    char *begin = buf, *end, *opt;

    ENTER();
    /* 
     * Parse the rest of the buf here
     *      1)  <ssid="valid ssid"> - This will get the passphrase, AKMP
     *          for specified ssid, if none specified then it will get all.
     *          Eg: iwpriv <ethx> passphrase 0:ssid=marvell
     *      2)  <psk="psk">:<passphrase="passphare">:<bssid="00:50:43:ef:23:f3">
     *          <ssid="valid ssid"> - Passphrase and psk cannot be provided to
     *          the same SSID, Takes one SSID at a time, If ssid= is present
     *          the it should contain a passphrase or psk. If no arguments are
     *          provided then AKMP=802.1x, and passphrase should be provided
     *          after association.
     *          End of each parameter should be followed by a ':'(except for the
     *          last parameter) as the delimiter. If ':' has to be used in
     *          an SSID then a '/' should be preceded to ':' as a escape.
     *          Eg:iwpriv <ethx> passphrase
     *                    "1:ssid=mrvl AP:psk=abcdefgh:bssid=00:50:43:ef:23:f3"
     *          iwpriv <ethx> passphrase
     *                 "1:ssid=mrvl/: AP:psk=abcdefgd:bssid=00:50:43:ef:23:f3"
     *          iwpriv <ethx> passphrase "1:ssid=mrvlAP:psk=abcdefgd"
     *      3)  <ssid="valid ssid"> - This will clear the passphrase
     *          for specified ssid, if none specified then it will clear all.
     *          Eg: iwpriv <ethx> passphrase 2:ssid=marvell
     */

    /* -1 is for u8 TlvBuffer[1] as this should not be included */
    cmd->Size = sizeof(HostCmd_DS_802_11_SUPPLICANT_PMK) + S_DS_GEN - 1;
    if (begin == NULL) {
        /* ACTION should be clear or AKMP is assumed as 802.1x */
    }

    while (begin != NULL) {
        end = wlan_strsep(&begin, ';', '/');
        opt = wlan_strsep(&end, '=', '/');

        if (!strnicmp(opt, "ssid", strlen(opt))) {
            if (strlen(end) > MRVDRV_MAX_SSID_LENGTH) {
                PRINTM(INFO, "SSID length exceeds max" "ssid length\n");
                LEAVE();
                return -EFAULT;
            }

            ssid_tlv = (MrvlIEtypes_SsIdParamSet_t *) tlvBuffer;
            ssid_tlv->Header.Type = wlan_cpu_to_le16(TLV_TYPE_SSID);
            ssid_tlv->Header.Len = strlen(end);
            strcpy(ssid_tlv->SsId, end);
            tlvBuffer += (ssid_tlv->Header.Len + sizeof(MrvlIEtypesHeader_t));
            cmd->Size += (ssid_tlv->Header.Len + sizeof(MrvlIEtypesHeader_t));
            ssid_tlv->Header.Len = wlan_cpu_to_le16(ssid_tlv->Header.Len);
        } else if (!strnicmp(opt, "psk", strlen(opt))
                   && option == HostCmd_ACT_GEN_SET) {
            if (strlen(end) != PMK_LEN) {
                PRINTM(ERROR, "Invalid PMK length\n");
                LEAVE();
                return -EINVAL;
            }

            pmk_tlv = (MrvlIEtypes_PMK_t *) tlvBuffer;
            pmk_tlv->Header.Type = wlan_cpu_to_le16(TLV_TYPE_PMK);
            pmk_tlv->Header.Len = PMK_LEN / 2;
            ascii2hex(pmk_tlv->PMK, end, PMK_LEN);
            tlvBuffer += (pmk_tlv->Header.Len + sizeof(MrvlIEtypesHeader_t));
            cmd->Size += (pmk_tlv->Header.Len + sizeof(MrvlIEtypesHeader_t));
            pmk_tlv->Header.Len = wlan_cpu_to_le16(pmk_tlv->Header.Len);
        } else if (!strnicmp(opt, "passphrase", strlen(opt))) {
#define MIN_PASSPHRASE_LEN 8
#define MAX_PASSPHRASE_LEN 63
            if ((option == HostCmd_ACT_GEN_SET) &&
                (strlen(end) < MIN_PASSPHRASE_LEN
                 || strlen(end) > MAX_PASSPHRASE_LEN)) {
                PRINTM(ERROR, "Invalid length for passphrase\n");
                LEAVE();
                return -EINVAL;
            }

            passphrase_tlv = (MrvlIEtypes_Passphrase_t *) tlvBuffer;
            passphrase_tlv->Header.Type = wlan_cpu_to_le16(TLV_TYPE_PASSPHRASE);
            passphrase_tlv->Header.Len = strlen(end);
            strcpy(passphrase_tlv->Passphrase, end);
            tlvBuffer += (passphrase_tlv->Header.Len +
                          sizeof(MrvlIEtypesHeader_t));
            cmd->Size += (passphrase_tlv->Header.Len +
                          sizeof(MrvlIEtypesHeader_t));
            passphrase_tlv->Header.Len =
                wlan_cpu_to_le16(passphrase_tlv->Header.Len);
        } else if (!strnicmp(opt, "bssid", strlen(opt))) {
            bssid_tlv = (MrvlIEtypes_Bssid_t *) tlvBuffer;
            bssid_tlv->Header.Type = wlan_cpu_to_le16(TLV_TYPE_BSSID);
            bssid_tlv->Header.Len = ETH_ALEN;
            mac2u8(bssid_tlv->Bssid, end);
            tlvBuffer += (bssid_tlv->Header.Len + sizeof(MrvlIEtypesHeader_t));
            cmd->Size += (bssid_tlv->Header.Len + sizeof(MrvlIEtypesHeader_t));
            bssid_tlv->Header.Len = wlan_cpu_to_le16(bssid_tlv->Header.Len);
        } else {
            PRINTM(ERROR, "Invalid option %s\n", opt);
            LEAVE();
            return -EINVAL;
        }
    }

    if ((option == HostCmd_ACT_GEN_SET) && (ssid_tlv != NULL &&
                                            ((pmk_tlv == NULL) &&
                                             (passphrase_tlv == NULL)))) {
        PRINTM(ERROR, "Invalid case,ssid present without pmk or passphrase\n");
        LEAVE();
        return -EINVAL;
    }

    cmd->Command = wlan_cpu_to_le16(HostCmd_CMD_SUPPLICANT_PMK);
    esupplicant_psk->Action = wlan_cpu_to_le16(option);
    esupplicant_psk->CacheResult = 0;
    cmd->Size = wlan_cpu_to_le16(cmd->Size);

    LEAVE();
    return WLAN_STATUS_SUCCESS;
}

/**
 *  @brief Check if the current SSID has any passphrase set
 *
 *  @param priv	    A pointer to wlan_private structure
 *  @param ssidBuf  Ssid
 *  @param mac		Mac address
 *  @return 	    TRUE on Success, FALSE on Failure
 */
BOOLEAN
isSTASecEnabled(wlan_private * priv, WLAN_802_11_SSID ssidBuf, u8 * mac)
{
    u16 tlv;
    u8 passphraseBuf[9], *tlvbuf;
    int ret = 0, tlvbuflen;
    MrvlIEtypesHeader_t *head = NULL;
    char begin[128] = "passphrase=aaaaaaaaa", ssid[42];
    MrvlIEtypes_Passphrase_t *passphrase_tlv = NULL;

    ENTER();
    if ((ret = wlan_prepare_cmd(priv,
                                HostCmd_CMD_SUPPLICANT_PMK, HostCmd_ACT_GEN_GET,
                                HostCmd_OPTION_WAITFORRSP, 0, (u8 *) begin))) {
        LEAVE();
        return FALSE;
    }

    tlvbuflen = *(u16 *) begin;
    tlvbuf = (u8 *) & begin[2];

    if (tlvbuf) {
                /** Assuming that passphrase is the first TLV returned */
        passphrase_tlv = (MrvlIEtypes_Passphrase_t *) tlvbuf;
        memset(passphraseBuf, 0, sizeof(passphraseBuf));
        if (memcmp(passphrase_tlv->Passphrase, passphraseBuf,
                   sizeof(passphraseBuf))) {
            LEAVE();
            return TRUE;
        }
    }

    strcpy(begin, "ssid=");
    memcpy(ssid, ssidBuf.Ssid, ssidBuf.SsidLength);
    ssid[ssidBuf.SsidLength] = '\0';
    strcat(begin, ssid);

    if ((ret = wlan_prepare_cmd(priv,
                                HostCmd_CMD_SUPPLICANT_PMK, HostCmd_ACT_GEN_GET,
                                HostCmd_OPTION_WAITFORRSP, 0, (u8 *) begin))) {
        LEAVE();
        return FALSE;
    }

    tlvbuflen = *(u16 *) begin;
    tlvbuf = (u8 *) & begin[2];

    while (tlvbuf && tlvbuflen > 0) {
        tlv = (*tlvbuf);
        tlv = tlv | (*(tlvbuf + 1) << 8);
        switch (tlv) {
        case TLV_TYPE_PMK:
            LEAVE();
            return TRUE;
        default:
            head = (MrvlIEtypesHeader_t *) tlvbuf;
            head->Len = wlan_le16_to_cpu(head->Len);
            tlvbuf += head->Len + sizeof(MrvlIEtypesHeader_t);
            tlvbuflen -= head->Len + sizeof(MrvlIEtypesHeader_t);
            break;
        }
    }

    if (!mac) {
        LEAVE();
        return FALSE;
    }

    strcpy(begin, "bssid=");
    sprintf(begin + strlen(begin), "%02x:%02x:%02x:%02x:%02x:%02x",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    if ((ret = wlan_prepare_cmd(priv,
                                HostCmd_CMD_SUPPLICANT_PMK, HostCmd_ACT_GEN_GET,
                                HostCmd_OPTION_WAITFORRSP, 0, (u8 *) begin))) {
        LEAVE();
        return FALSE;
    }

    tlvbuflen = *(u16 *) begin;
    tlvbuf = (u8 *) & begin[2];

    while (tlvbuf && tlvbuflen > 0) {
        tlv = (*tlvbuf);
        tlv = tlv | (*(tlvbuf + 1) << 8);
        switch (tlv) {
        case TLV_TYPE_PMK:
            LEAVE();
            return TRUE;
        default:
            head = (MrvlIEtypesHeader_t *) tlvbuf;
            head->Len = wlan_le16_to_cpu(head->Len);
            tlvbuf += head->Len + sizeof(MrvlIEtypesHeader_t);
            tlvbuflen -= head->Len + sizeof(MrvlIEtypesHeader_t);
            break;
        }
    }

    LEAVE();
    return FALSE;
}

/**
 *  @brief Handle the supplicant profile command
 *
 *  @param priv	    A pointer to wlan_private structure
 *  @param cmd	    A pointer to command buffer
 *  @param action   Action
 *  @param buf	    buffer for input
 *  @return 	    WLAN_STATUS_SUCCESS on Success, Failure cause
 */
int
wlan_cmd_802_11_supplicant_profile(wlan_private * priv,
                                   HostCmd_DS_COMMAND * cmd, u16 action,
                                   void *buf)
{
    HostCmd_DS_802_11_SUPPLICANT_PROFILE *supProfile =
        &cmd->params.esupplicant_profile;

    ENTER();

    cmd->Size = wlan_cpu_to_le16(sizeof(HostCmd_DS_802_11_SUPPLICANT_PROFILE) +
                                 S_DS_GEN - 1);
    cmd->Command = wlan_cpu_to_le16(HostCmd_CMD_SUPPLICANT_PROFILE);
    supProfile->Action = wlan_cpu_to_le16(action);

    LEAVE();

    return WLAN_STATUS_SUCCESS;
}

/**
 *  @brief Handle the supplicant profile response
 *
 *  @param priv	    A pointer to wlan_private structure
 *  @param resp	    A pointer to command response buffer
 *  @return 	    WLAN_STATUS_SUCCESS on Success, Failure cause
 */
int
wlan_ret_802_11_supplicant_profile(wlan_private * priv,
                                   HostCmd_DS_COMMAND * resp)
{
    HostCmd_DS_802_11_SUPPLICANT_PROFILE *supProfile_resp =
        &resp->params.esupplicant_profile;
    MrvlIEtypesHeader_t *head;
    u8 *tlv;
    int len;

    ENTER();
    len = resp->Size - S_DS_GEN - sizeof(u16);
    tlv = supProfile_resp->tlvBuf;

    while (len > 0) {
        head = (MrvlIEtypesHeader_t *) tlv;
        if (memcmp(priv->adapter->CurCmd->pdata_buf,
                   head, sizeof(MrvlIEtypesHeader_t)) == 0) {
            memcpy(priv->adapter->CurCmd->pdata_buf, head,
                   head->Len + sizeof(MrvlIEtypesHeader_t));
            break;
        }

        len -= (head->Len - sizeof(MrvlIEtypesHeader_t));
        tlv = tlv + (head->Len + sizeof(MrvlIEtypesHeader_t));
    }
    LEAVE();
    return WLAN_STATUS_SUCCESS;
}

/**
 *  @brief Handle the supplicant pmk response
 *
 *  @param priv	    A pointer to wlan_private structure
 *  @param resp	    A pointer to command response buffer
 *  @return 	    WLAN_STATUS_SUCCESS on Success, Failure cause
 */
int
wlan_ret_802_11_supplicant_pmk(wlan_private * priv, HostCmd_DS_COMMAND * resp)
{
    HostCmd_DS_802_11_SUPPLICANT_PMK *supplicant_pmk_resp =
        &resp->params.esupplicant_psk;
    u16 *buf = (u16 *) priv->adapter->CurCmd->pdata_buf;
    u16 tlvbuflen;

    ENTER();
    if (supplicant_pmk_resp->Action == HostCmd_ACT_GEN_GET) {
        tlvbuflen = resp->Size - (sizeof(HostCmd_DS_802_11_SUPPLICANT_PMK) +
                                  S_DS_GEN - 1);

        if (!tlvbuflen) {
            PRINTM(INFO, "No tlv buffers in response GET\n");
            LEAVE();
            return WLAN_STATUS_FAILURE;
        }

        buf[0] = tlvbuflen;
        memcpy((u8 *) & buf[1],
               (u8 *) & supplicant_pmk_resp->TlvBuffer, tlvbuflen);
    }

    LEAVE();
    return WLAN_STATUS_SUCCESS;
}
