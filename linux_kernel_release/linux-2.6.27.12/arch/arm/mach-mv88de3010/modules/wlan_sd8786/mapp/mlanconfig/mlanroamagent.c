/** @file  mlanroamagent.c
  *
  * @brief This files contains mlanconfig roamagent command handling.
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
#include    "mlanroamagent.h"

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
 *  @brief          Issue getra failcnt command 
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
roamGetFailureCount(int argc, char *argv[])
{
    int ioctl_val, subioctl_val, cmd_len;
    int failvalue, state = STATE_HARDROAM, i;
    struct iwreq iwr;
    t_u8 *buf, *pos, *tlvptr;
    HostCmd_DS_GEN *hostcmd;
    HostCmd_DS_CMD_ROAMAGENT_STATISTICS_THRESHOLD *statsthreshold;
    MrvlIEtypes_FailureCount_t *pFailureCount;
    MrvlIEtypesHeader_t *pTlvHdr;

    const char *states[] =
        { "Stable", "Degrading", "Unacceptable", "Hardroam" };

    if (get_priv_ioctl("hostcmd",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    cmd_len = sizeof(HostCmd_DS_GEN) + sizeof(statsthreshold->action) +
        sizeof(statsthreshold->State) + sizeof(statsthreshold->Profile);

    buf = (t_u8 *) calloc(sizeof(t_u8), cmd_len);
    if (!buf) {
        printf("Memory allocation failed\n");
        return -ENOMEM;
    }

    hostcmd = (HostCmd_DS_GEN *) buf;
    hostcmd->command = cpu_to_le16(HostCmd_CMD_ROAMAGENT_STATISTICS_THRESHOLD);
    hostcmd->size = cpu_to_le16(cmd_len);
    hostcmd->seq_num = 0;
    hostcmd->result = 0;

    pos = buf + sizeof(HostCmd_DS_GEN);

    statsthreshold = (HostCmd_DS_CMD_ROAMAGENT_STATISTICS_THRESHOLD *) pos;
    statsthreshold->action = cpu_to_le16(HostCmd_ACT_GEN_GET);

    /* Can be extended to all states later */
    for (state = STATE_HARDROAM - 1; state < STATE_HARDROAM; state++) {
        statsthreshold->State = state + 1;
        /* 
         * ENDIANNESS 
         */

        /* 
         * Prepare wireless request struct and fire ioctl
         */
        memset(&iwr, 0, sizeof(iwr));
        strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
        iwr.u.data.flags = 0;
        iwr.u.data.pointer = (caddr_t) buf;
        iwr.u.data.length = cmd_len;

        if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
            perror("mlanconfig: roamstatistics ioctl");
            free(buf);
            return -EFAULT;
        }

        i = le16_to_cpu(hostcmd->size);
        i -= cmd_len;
        tlvptr = statsthreshold->TlvBuffer;

        while (i > 2) {
            pTlvHdr = (MrvlIEtypesHeader_t *) tlvptr;

            switch (le16_to_cpu(pTlvHdr->type)) {
            case TLV_TYPE_FAILCOUNT:
                pFailureCount = (MrvlIEtypes_FailureCount_t *) pTlvHdr;
                failvalue = pFailureCount->fail_value;
                break;
            }

            tlvptr += (le16_to_cpu(pTlvHdr->len)
                       + sizeof(MrvlIEtypesHeader_t));
            i -= (le16_to_cpu(pTlvHdr->len)
                  + sizeof(MrvlIEtypesHeader_t));
        }

        printf("State: %-8s. Fail value %d\n", states[state], failvalue);
    }

    free(buf);
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief          Issue getra bcnmiss command 
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
roamGetPreBeaconMiss(int argc, char *argv[])
{
    int ioctl_val, subioctl_val, cmd_len;
    int bcnmiss, state = STATE_HARDROAM, i;
    struct iwreq iwr;
    t_u8 *buf, *pos, *tlvptr;
    HostCmd_DS_GEN *hostcmd;
    HostCmd_DS_CMD_ROAMAGENT_STATISTICS_THRESHOLD *statsthreshold;
    MrvlIEtypes_BeaconsMissed_t *pBeaconMissed;
    MrvlIEtypesHeader_t *pTlvHdr;

    const char *states[] =
        { "Stable", "Degrading", "Unacceptable", "Hardroam" };

    if (get_priv_ioctl("hostcmd",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    cmd_len = sizeof(HostCmd_DS_GEN) + sizeof(statsthreshold->action) +
        sizeof(statsthreshold->State) + sizeof(statsthreshold->Profile);

    buf = (t_u8 *) calloc(sizeof(t_u8), cmd_len);
    if (!buf) {
        printf("Memory allocation failed\n");
        return -ENOMEM;
    }

    hostcmd = (HostCmd_DS_GEN *) buf;
    hostcmd->command = cpu_to_le16(HostCmd_CMD_ROAMAGENT_STATISTICS_THRESHOLD);
    hostcmd->size = cpu_to_le16(cmd_len);
    hostcmd->seq_num = 0;
    hostcmd->result = 0;

    pos = buf + sizeof(HostCmd_DS_GEN);

    statsthreshold = (HostCmd_DS_CMD_ROAMAGENT_STATISTICS_THRESHOLD *) pos;
    statsthreshold->action = cpu_to_le16(HostCmd_ACT_GEN_GET);

    /* Can be extended to all states later */
    for (state = STATE_HARDROAM - 1; state < STATE_HARDROAM; state++) {
        statsthreshold->State = state + 1;
        /* 
         * ENDIANNESS 
         */

        /* 
         * Prepare wireless request struct and fire ioctl
         */
        memset(&iwr, 0, sizeof(iwr));
        strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
        iwr.u.data.flags = 0;
        iwr.u.data.pointer = (caddr_t) buf;
        iwr.u.data.length = cmd_len;

        if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
            perror("mlanconfig: beacon miss ioctl");
            free(buf);
            return -EFAULT;
        }

        i = le16_to_cpu(hostcmd->size);
        i -= cmd_len;
        tlvptr = statsthreshold->TlvBuffer;

        while (i > 2) {
            pTlvHdr = (MrvlIEtypesHeader_t *) tlvptr;

            switch (le16_to_cpu(pTlvHdr->type)) {

            case TLV_TYPE_PRE_BEACON_LOST:
                pBeaconMissed = (MrvlIEtypes_BeaconsMissed_t *) pTlvHdr;
                bcnmiss = pBeaconMissed->beacon_missed;
            }

            tlvptr += (le16_to_cpu(pTlvHdr->len)
                       + sizeof(MrvlIEtypesHeader_t));
            i -= (le16_to_cpu(pTlvHdr->len)
                  + sizeof(MrvlIEtypesHeader_t));
        }

        printf("State: %-8s. Pre Beacon missed threshold %d\n", states[state],
               bcnmiss);
    }

    free(buf);
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief          Issue getra rssi/snr command 
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *  @param type     RSSI/SNR threshold type
 *
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
roamGetStatsThreshold(int argc, char *argv[], int type)
{
    int ioctl_val, subioctl_val, cmd_len, cmdresplen;
    int state = 0, i = 0, profile = 1;
    struct iwreq iwr;
    t_u8 *buf, *pos;
    HostCmd_DS_GEN *hostcmd;
    HostCmd_DS_CMD_ROAMAGENT_STATISTICS_THRESHOLD *statsthreshold;
    MrvlIEtypes_BeaconHighRssiThreshold_t *pHighRssiThreshold;
    MrvlIEtypes_BeaconLowRssiThreshold_t *pLowRssiThreshold;
    MrvlIEtypes_BeaconHighSnrThreshold_t *pHighSnrThreshold;
    MrvlIEtypes_BeaconLowSnrThreshold_t *pLowSnrThreshold;
    t_u8 high, low, *tlvptr;
    t_u16 tlv;
    const char *states[] = { "Stable", "Degrading", "Unacceptable" };

    if (get_priv_ioctl("hostcmd",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    if (argc) {
        if (strncmp(argv[0], "configured", strlen("config")) == 0) {
            profile = 0;
        } else if (strncmp(argv[0], "active", strlen("active"))) {
            printf("Incorrect parameter %s for getra command\n", argv[0]);
            return MLAN_STATUS_FAILURE;
        }
    }
    /* 
     * STATISTICS_THRESHOLD 
     */
    cmd_len = cmdresplen = sizeof(HostCmd_DS_GEN) +
        sizeof(statsthreshold->action) + sizeof(statsthreshold->State) +
        sizeof(statsthreshold->Profile);
    if (type == RSSI_THRESHOLD) {
        cmdresplen += sizeof(MrvlIEtypes_BeaconHighRssiThreshold_t) +
            sizeof(MrvlIEtypes_BeaconLowRssiThreshold_t);
    } else {
        cmdresplen += sizeof(MrvlIEtypes_BeaconHighSnrThreshold_t) +
            sizeof(MrvlIEtypes_BeaconLowSnrThreshold_t);
    }

    buf = (t_u8 *) calloc(sizeof(t_u8), cmdresplen);
    if (!buf) {
        printf("Memory allocation failed\n");
        return -ENOMEM;
    }

    hostcmd = (HostCmd_DS_GEN *) buf;
    hostcmd->command = HostCmd_CMD_ROAMAGENT_STATISTICS_THRESHOLD;
    hostcmd->size = 0;
    hostcmd->seq_num = 0;
    hostcmd->result = 0;

    pos = buf + sizeof(HostCmd_DS_GEN);

    statsthreshold = (HostCmd_DS_CMD_ROAMAGENT_STATISTICS_THRESHOLD *) pos;
    statsthreshold->action = HostCmd_ACT_GEN_GET;

    if (type == RSSI_THRESHOLD) {
        printf("Dump of RSSI thresholds=>\n");
    } else {
        printf("Dump of SNR thresholds=>\n");
    }

    for (state = STATE_STABLE - 1; state < STATE_UNACCEPTABLE; state++) {
        statsthreshold->State = state + 1;
        statsthreshold->Profile = profile;

        /* 
         * ENDIANNESS 
         */
        hostcmd->command = cpu_to_le16(hostcmd->command);
        hostcmd->size = cpu_to_le16(cmd_len);
        statsthreshold->action = cpu_to_le16(statsthreshold->action);

        /* 
         * Prepare wireless request struct and fire ioctl
         */
        memset(&iwr, 0, sizeof(iwr));
        strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
        iwr.u.data.flags = 0;
        iwr.u.data.pointer = (caddr_t) buf;
        iwr.u.data.length = cmd_len;

        if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
            perror("mlanconfig: roamstatistics ioctl");
            free(buf);
            return -EFAULT;
        }

        if (le16_to_cpu(hostcmd->result)) {
            printf("HOSTCMD failed: ReturnCode=%#04x, Result=%#04x\n",
                   le16_to_cpu(hostcmd->command), le16_to_cpu(hostcmd->result));
            free(buf);
            return MLAN_STATUS_FAILURE;
        }

        i = le16_to_cpu(hostcmd->size);
        i -= cmd_len;
        tlvptr = statsthreshold->TlvBuffer;
        while (i > 2) {
            /* 
             * ENDIANNESS for Response 
             */
            tlv = le16_to_cpu(*((t_u16 *) (tlvptr)));
            switch (tlv) {
            case TLV_TYPE_RSSI_HIGH:
                high = *(t_u8 *) (tlvptr + sizeof(t_u16) + sizeof(t_u16));
                pHighRssiThreshold =
                    (MrvlIEtypes_BeaconHighRssiThreshold_t *) (tlvptr);
                tlvptr +=
                    (le16_to_cpu(pHighRssiThreshold->Header.len) +
                     sizeof(MrvlIEtypesHeader_t));
                i -= (le16_to_cpu(pHighRssiThreshold->Header.len) +
                      sizeof(MrvlIEtypesHeader_t));
                break;
            case TLV_TYPE_RSSI_LOW:
                low = *(t_u8 *) (tlvptr + sizeof(t_u16) + sizeof(t_u16));
                pLowRssiThreshold =
                    (MrvlIEtypes_BeaconLowRssiThreshold_t *) (tlvptr);
                tlvptr +=
                    (le16_to_cpu(pLowRssiThreshold->Header.len) +
                     sizeof(MrvlIEtypesHeader_t));
                i -= (le16_to_cpu(pLowRssiThreshold->Header.len) +
                      sizeof(MrvlIEtypesHeader_t));
                break;
            case TLV_TYPE_SNR_HIGH:
                high = *(t_u8 *) (tlvptr + sizeof(t_u16) + sizeof(t_u16));
                pHighSnrThreshold =
                    (MrvlIEtypes_BeaconHighSnrThreshold_t *) (tlvptr);
                tlvptr +=
                    (le16_to_cpu(pHighSnrThreshold->Header.len) +
                     sizeof(MrvlIEtypesHeader_t));
                i -= (le16_to_cpu(pHighSnrThreshold->Header.len) +
                      sizeof(MrvlIEtypesHeader_t));
                break;
            case TLV_TYPE_SNR_LOW:
                low = *(t_u8 *) (tlvptr + sizeof(t_u16) + sizeof(t_u16));
                pLowSnrThreshold =
                    (MrvlIEtypes_BeaconLowSnrThreshold_t *) (tlvptr);
                tlvptr +=
                    (le16_to_cpu(pLowSnrThreshold->Header.len) +
                     sizeof(MrvlIEtypesHeader_t));
                i -= (le16_to_cpu(pLowSnrThreshold->Header.len) +
                      sizeof(MrvlIEtypesHeader_t));
                break;
            }
        }
        printf("State: %-13s.   Low = %d dBm, High = %d dBm\n", states[state],
               low, high);
    }
    free(buf);
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief          Issue getra rssi/snr command 
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
roamGetRssiStatsThreshold(int argc, char *argv[])
{
    return roamGetStatsThreshold(argc, argv, RSSI_THRESHOLD);
}

/** 
 *  @brief          Coverts ms to exactTime
 *
 *  @param ms       number of milliseconds
 *  @param t        converted into this structure   
 *
 *  @return         none
 */
static void
ms2exactTime(t_u32 ms, ExactTime_t * t)
{
    memset(t, 0, sizeof(ExactTime_t));

    t->msecs = ms % 1000;
    if (ms >= 1000) {
        ms = (ms - t->msecs) / 1000;
        t->secs = ms % 60;
        if (ms >= 60) {
            ms = (ms - t->secs) / 60;
            t->mins = ms % 60;
            if (ms >= 60) {
                ms = (ms - t->mins) / 60;
                t->hrs = ms;
            }
        }
    }
    return;
}

/** 
 *  @brief          Issue getra neighbor assessment command 
 *
 *  @pNborAssessment neighbour assessment struct 
 *
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
getNborAssessment(HostCmd_DS_CMD_ROAMAGENT_NEIGHBOR_ASSESSMENT *
                  pNborAssessmentParam)
{
    int ioctl_val, cmd_len, cmdresplen, subioctl_val;
    struct iwreq iwr;
    t_u8 *buf, *pos;
    HostCmd_DS_GEN *hostcmd;
    HostCmd_DS_CMD_ROAMAGENT_NEIGHBOR_ASSESSMENT *pNborAssessment;

    if (get_priv_ioctl("hostcmd",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }
    /* 
     * NEIGHBOR_ASSESSMENT
     */
    cmd_len = cmdresplen = sizeof(HostCmd_DS_GEN) +
        sizeof(pNborAssessment->action);
    cmdresplen += sizeof(pNborAssessment->QualifyingNumNeighbor) +
        sizeof(pNborAssessment->ConfQualSignalStrength) +
        sizeof(pNborAssessment->ActiveQualSignalStrength) +
        sizeof(pNborAssessment->ShortBlacklistPeriod) +
        sizeof(pNborAssessment->LongBlacklistPeriod) +
        sizeof(pNborAssessment->StaleCount) +
        sizeof(pNborAssessment->StalePeriod);

    buf = (t_u8 *) calloc(sizeof(t_u8), cmdresplen);
    if (!buf) {
        printf("Memory allocation failed\n");
        return -ENOMEM;
    }

    hostcmd = (HostCmd_DS_GEN *) buf;
    hostcmd->command = HostCmd_CMD_ROAMAGENT_NEIGHBOR_ASSESSMENT;
    hostcmd->size = 0;
    hostcmd->seq_num = 0;
    hostcmd->result = 0;

    pos = buf + sizeof(HostCmd_DS_GEN);

    pNborAssessment = (HostCmd_DS_CMD_ROAMAGENT_NEIGHBOR_ASSESSMENT *) pos;
    pNborAssessment->action = HostCmd_ACT_GEN_GET;

    /* 
     * ENDIANNESS 
     */
    hostcmd->command = cpu_to_le16(hostcmd->command);
    hostcmd->size = cpu_to_le16(cmd_len);
    pNborAssessment->action = cpu_to_le16(pNborAssessment->action);

    /* 
     * Prepare wireless request struct and fire ioctl
     */
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.flags = 0;
    iwr.u.data.pointer = (caddr_t) buf;
    iwr.u.data.length = cmd_len;

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("mlanconfig: GetNeibhorAssesinfo ioctl");
        free(buf);
        return -EFAULT;
    }
    /* 
     * ENDIANNESS for Response 
     */
    pNborAssessment->QualifyingNumNeighbor =
        le16_to_cpu(pNborAssessment->QualifyingNumNeighbor);
    pNborAssessment->ShortBlacklistPeriod =
        le32_to_cpu(pNborAssessment->ShortBlacklistPeriod);
    pNborAssessment->LongBlacklistPeriod =
        le32_to_cpu(pNborAssessment->LongBlacklistPeriod);
    pNborAssessment->StaleCount = le16_to_cpu(pNborAssessment->StaleCount);
    pNborAssessment->StalePeriod = le32_to_cpu(pNborAssessment->StalePeriod);

    memcpy((void *) pNborAssessmentParam, (void *) pNborAssessment,
           sizeof(HostCmd_DS_CMD_ROAMAGENT_NEIGHBOR_ASSESSMENT));
    free(buf);
    return (MLAN_STATUS_SUCCESS);
}

/** 
 *  @brief          Display exactTime structure elements
 *
 *  @param t        ExactTime_t struct
 *
 *  @return         None 
 */
static void
printExactTime2stdout(ExactTime_t * t)
{
    int flag = 0;
    if (t->hrs) {
        printf("%dh ", t->hrs);
        flag = 1;
    }
    if (t->mins) {
        printf("%dm ", t->mins);
        flag = 1;
    }
    if (t->secs) {
        printf("%ds ", t->secs);
        flag = 1;
    }
    if (t->msecs) {
        printf("%dms", t->msecs);
        flag = 1;
    }
    if (!flag) {
        printf(" 0");
    }
}

/** 
 *  @brief          Issue getra neighbors command 
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
roamGetNeighbors(int argc, char *argv[])
{
    int ioctl_val, subioctl_val, i, cmd_len, cmdresplen;
    struct iwreq iwr;
    char neighflag[6];
    ExactTime_t t;
    t_u8 *buf, *pos, *tlvptr;
    t_u16 tlv;
    HostCmd_DS_GEN *hostcmd;
    HostCmd_DS_CMD_ROAMAGENT_NEIGHBOR_ASSESSMENT nborAssessment;
    HostCmd_DS_CMD_ROAMAGENT_NEIGHBORLIST *pNborList;
    MrvlIEtypes_NeighborEntry_t *pNeighbor;

    if (get_priv_ioctl("hostcmd",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    /* 
     * NEIGHBOR_LIST
     */
    cmd_len = sizeof(HostCmd_DS_GEN) + sizeof(pNborList->action);

    buf = (t_u8 *) calloc(sizeof(t_u8), MRVDRV_SIZE_OF_CMD_BUFFER);
    if (!buf) {
        printf("Memory allocation failed\n");
        return -ENOMEM;
    }

    hostcmd = (HostCmd_DS_GEN *) buf;
    hostcmd->command = HostCmd_CMD_ROAMAGENT_NEIGHBORLIST;
    hostcmd->size = 0;
    hostcmd->seq_num = 0;
    hostcmd->result = 0;

    pos = buf + sizeof(HostCmd_DS_GEN);

    pNborList = (HostCmd_DS_CMD_ROAMAGENT_NEIGHBORLIST *) pos;
    pNborList->action = HostCmd_ACT_GEN_GET;

    /* 
     * ENDIANNESS 
     */
    hostcmd->command = cpu_to_le16(hostcmd->command);
    hostcmd->size = cpu_to_le16(cmd_len);
    pNborList->action = cpu_to_le16(pNborList->action);

    /* 
     * Prepare wireless request struct and fire ioctl
     */
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.flags = 0;
    iwr.u.data.pointer = (caddr_t) buf;
    iwr.u.data.length = cmd_len;

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("mlanconfig: roamneighborlist ioctl");
        free(buf);
        return -EFAULT;
    }

    cmdresplen = le16_to_cpu(hostcmd->size);
    cmdresplen -= cmd_len + sizeof(pNborList->Reserved);
    tlvptr = pNborList->TlvBuffer;
    i = 0;
    while (cmdresplen > 2) {
        if (i == 0) {
            i++;

            printf("----------------------------------------------------"
                   "--------------\n");
            printf("       BSSID       | RSSI |  Age  | Qualified |"
                   " Blacklist Duration\n");
            printf("----------------------------------------------------"
                   "--------------\n");
        }
        /* 
         * ENDIANNESS for Response 
         */
        tlv = le16_to_cpu(*((t_u16 *) (tlvptr)));
        switch (tlv) {
        case TLV_TYPE_NEIGHBOR_ENTRY:
            pNeighbor = (MrvlIEtypes_NeighborEntry_t *) (tlvptr);
            pNeighbor->SignalStrength = le16_to_cpu(pNeighbor->SignalStrength);
            pNeighbor->Age = le16_to_cpu(pNeighbor->Age);
            pNeighbor->QualifiedNeighborBitmap =
                le32_to_cpu(pNeighbor->QualifiedNeighborBitmap);

            pNeighbor->BlackListDuration =
                le32_to_cpu(pNeighbor->BlackListDuration);
            ms2exactTime(pNeighbor->BlackListDuration, &t);
            neighflag[0] = '\0';

            if ((pNeighbor->QualifiedNeighborBitmap
                 & (BIT_NEIGHFLAG_RSSI
                    | BIT_NEIGHFLAG_AGE
                    | BIT_NEIGHFLAG_BLACKLIST
                    | BIT_NEIGHFLAG_ADMISSION_CAP | BIT_NEIGHFLAG_UPLINK_RSSI))
                == (BIT_NEIGHFLAG_RSSI
                    | BIT_NEIGHFLAG_AGE
                    | BIT_NEIGHFLAG_BLACKLIST
                    | BIT_NEIGHFLAG_ADMISSION_CAP
                    | BIT_NEIGHFLAG_UPLINK_RSSI)) {
                strcat(neighflag, "Yes");
            } else {
                strcat(neighflag, "No: ");
                if (!(pNeighbor->QualifiedNeighborBitmap & BIT_NEIGHFLAG_RSSI)) {
                    strcat(neighflag, "R");
                }
                if (!(pNeighbor->QualifiedNeighborBitmap & BIT_NEIGHFLAG_AGE)) {
                    strcat(neighflag, "S");
                }
                if (!(pNeighbor->QualifiedNeighborBitmap
                      & BIT_NEIGHFLAG_BLACKLIST)) {
                    strcat(neighflag, "B");
                }
                if (!(pNeighbor->QualifiedNeighborBitmap
                      & BIT_NEIGHFLAG_ADMISSION_CAP)) {
                    strcat(neighflag, "A");
                }
                if (!(pNeighbor->QualifiedNeighborBitmap
                      & BIT_NEIGHFLAG_UPLINK_RSSI)) {
                    strcat(neighflag, "U");
                }
            }
            printf(" %02x:%02x:%02x:%02x:%02x:%02x | %3d  | %5d | %9s | ",
                   pNeighbor->Bssid[0], pNeighbor->Bssid[1],
                   pNeighbor->Bssid[2], pNeighbor->Bssid[3],
                   pNeighbor->Bssid[4], pNeighbor->Bssid[5],
                   pNeighbor->SignalStrength, pNeighbor->Age, neighflag);
            if (pNeighbor->BlackListDuration) {
                printExactTime2stdout(&t);
            } else {
                printf("Not Blacklisted");
            }
            printf("\n");

            tlvptr += (le16_to_cpu(pNeighbor->Header.len) +
                       sizeof(MrvlIEtypesHeader_t));
            cmdresplen -= (le16_to_cpu(pNeighbor->Header.len) +
                           sizeof(MrvlIEtypesHeader_t));
            break;

        default:
            printf("Incorrect response.\n");
            break;
        }
    }

    /* 
     * NEIGHBOR_ASSESSMENT
     */
    if (getNborAssessment(&nborAssessment) != MLAN_STATUS_SUCCESS) {
        return MLAN_STATUS_FAILURE;
    }

    puts("");
    printf("------------------------------------------------\n");
    printf("        Neighbor Threshold Parameters\n");
    printf("------------------------------------------------\n");
    printf(" Neighbors needed for tracking mode = %d\n",
           nborAssessment.QualifyingNumNeighbor);
    printf(" Config RSSI qualification offset   = %d dB\n",
           nborAssessment.ConfQualSignalStrength);
    printf(" Active RSSI qualification offset   = %d dB\n",
           nborAssessment.ActiveQualSignalStrength);
    printf(" Short black list period            = %d ms\n",
           (int) nborAssessment.ShortBlacklistPeriod);
    printf(" Long black list period             = %d ms\n",
           (int) nborAssessment.LongBlacklistPeriod);
    printf(" Stale count                        = %d\n",
           (int) nborAssessment.StaleCount);
    printf(" Stale period                       = %d ms\n",
           (int) nborAssessment.StalePeriod);
    puts("");

    free(buf);
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief          Issue getra metrics command 
 * 
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *  
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
roamGetMetrics(int argc, char *argv[])
{
    int ioctl_val, subioctl_val, cmd_len, i;
    struct iwreq iwr;
    t_u8 *buf, *pos;
    t_u16 Metrics = 0;
    HostCmd_DS_GEN *hostcmd;
    HostCmd_DS_CMD_ROAMAGENT_ADV_METRIC_THRESHOLD *metricscmd;

    const char *metricslist[] = { "beacon", "data", "per", "fer" };

    if (argc != 0) {
        fprintf(stderr, "Incorrect number of arguments.\n");
        return MLAN_STATUS_FAILURE;
    }

    if (get_priv_ioctl("hostcmd",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    cmd_len = (sizeof(HostCmd_DS_GEN)
               + sizeof(HostCmd_DS_CMD_ROAMAGENT_ADV_METRIC_THRESHOLD));

    buf = (t_u8 *) calloc(sizeof(t_u8), cmd_len);
    if (!buf) {
        printf("Memory allocation failed\n");
        return -ENOMEM;
    }

    hostcmd = (HostCmd_DS_GEN *) buf;
    hostcmd->command = HostCmd_CMD_ROAMAGENT_ADV_METRIC_THRESHOLD;
    hostcmd->size = 0;
    hostcmd->seq_num = 0;
    hostcmd->result = 0;

    pos = buf + sizeof(HostCmd_DS_GEN);

    metricscmd = (HostCmd_DS_CMD_ROAMAGENT_ADV_METRIC_THRESHOLD *) pos;
    metricscmd->action = HostCmd_ACT_GEN_GET;

    /* 
     * ENDIANNESS 
     */
    hostcmd->command = cpu_to_le16(hostcmd->command);
    hostcmd->size = cpu_to_le16(cmd_len);
    metricscmd->action = cpu_to_le16(metricscmd->action);

    /* 
     * Prepare wireless request struct and fire ioctl
     */
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.flags = 0;
    iwr.u.data.pointer = (caddr_t) buf;
    iwr.u.data.length = cmd_len;
    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("mlanconfig: roam set matrics ioctl");
        free(buf);
        return -EFAULT;
    }

    Metrics = le16_to_cpu(metricscmd->Metrics);

    if (le16_to_cpu(hostcmd->result) == MLAN_STATUS_SUCCESS) {
        printf("\n Metrics Activated: ");
        for (i = 0; i < NELEMENTS(metricslist); i++) {
            if (Metrics & BIT(i)) {
                printf(" %s ", metricslist[i]);
            }
        }
        printf("\n");

        if (Metrics & BIT(3)) {
            printf("FER Threshold : %u %% \n", metricscmd->UcFerThresholdValue);
            printf("FER Packet Threshold: %ld \n",
                   le32_to_cpu(metricscmd->UiFerPktThreshold));
            printf
                ("FER period. Stable : %ld ms, Degrading : %ld ms, Unacceptable : %ld ms\n",
                 le32_to_cpu(metricscmd->StableFERPeriod_ms),
                 le32_to_cpu(metricscmd->DegradingFERPeriod_ms),
                 le32_to_cpu(metricscmd->UnacceptableFERPeriod_ms));
        }

        if (Metrics & BIT(2)) {
            printf("PER Threshold : %u %% \n", metricscmd->UcPerThresholdValue);
            printf("PER Packet Threshold: %ld \n",
                   le32_to_cpu(metricscmd->UiPerPktThreshold));
            printf
                ("PER period. Stable : %ld ms, Degrading : %ld ms, Unacceptable : %ld ms\n",
                 le32_to_cpu(metricscmd->StablePERPeriod_ms),
                 le32_to_cpu(metricscmd->DegradingPERPeriod_ms),
                 le32_to_cpu(metricscmd->UnacceptablePERPeriod_ms));
        }

        if (Metrics & BIT(1)) {
            printf("Data Packet Threshold: %ld \n",
                   le32_to_cpu(metricscmd->UiRxPktThreshold));
        }

        if ((Metrics & BIT(1)) || (Metrics & BIT(2)) || (Metrics & BIT(3))) {

            printf("Inactivity Period: %ld ms \n",
                   le32_to_cpu(metricscmd->InactivityPeriodThreshold_ms));
        }
    } else {
        printf("command response failure %d.\n", le16_to_cpu(hostcmd->result));
    }

    free(buf);
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief          Issue getra scanperiod command 
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
roamGetScanPeriod(int argc, char *argv[])
{
    int ioctl_val, subioctl_val, cmd_len, cmdresplen;
    int state = 0, scanmode = 0;
    struct iwreq iwr;
    t_u8 *buf, *pos, *tlvptr;
    t_u16 tlv;
    HostCmd_DS_GEN *hostcmd;
    HostCmd_DS_CMD_ROAMAGENT_NEIGHBOR_SCANPERIOD_RSP *scanPeriodInfo;
    MrvlIEtypes_NeighborScanPeriod_t *pscanperiod;
    const char *states[] = { "Stable", "Degrading", "Unacceptable" };
    const char *scanmodes[] = { "Discovery", "Tracking" };
    /* scanperiodValues[state][scanmode] */
    t_u32 values[3][2];

    if (get_priv_ioctl("hostcmd",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    /* 
     * NEIGHBOR_SCANPERIOD
     */
    cmd_len = sizeof(HostCmd_DS_GEN) + sizeof(scanPeriodInfo->action);

    buf = (t_u8 *) calloc(sizeof(t_u8), MRVDRV_SIZE_OF_CMD_BUFFER);
    if (!buf) {
        printf("Memory allocation failed\n");
        return -ENOMEM;
    }

    hostcmd = (HostCmd_DS_GEN *) buf;
    hostcmd->command = HostCmd_CMD_ROAMAGENT_NEIGHBOR_SCAN_PERIOD;
    hostcmd->size = 0;
    hostcmd->seq_num = 0;
    hostcmd->result = 0;

    pos = buf + sizeof(HostCmd_DS_GEN);

    scanPeriodInfo = (HostCmd_DS_CMD_ROAMAGENT_NEIGHBOR_SCANPERIOD_RSP *) pos;
    scanPeriodInfo->action = HostCmd_ACT_GEN_GET;

    /* 
     * ENDIANNESS 
     */
    hostcmd->command = cpu_to_le16(hostcmd->command);
    hostcmd->size = cpu_to_le16(cmd_len);
    scanPeriodInfo->action = cpu_to_le16(scanPeriodInfo->action);

    /* 
     * Prepare wireless request struct and fire ioctl
     */
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.flags = 0;
    iwr.u.data.pointer = (caddr_t) buf;
    iwr.u.data.length = cmd_len;

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("mlanconfig: roamscanperiod ioctl");
        free(buf);
        return -EFAULT;
    }

    cmdresplen = le16_to_cpu(hostcmd->size);
    cmdresplen -= cmd_len + sizeof(scanPeriodInfo->Reserved);
    tlvptr = scanPeriodInfo->TlvBuffer;
    while (cmdresplen > 2) {
        /* 
         * ENDIANNESS for Response 
         */
        tlv = le16_to_cpu(*((t_u16 *) (tlvptr)));
        switch (tlv) {
        case TLV_TYPE_NEIGHBOR_SCANPERIOD:
            pscanperiod = (MrvlIEtypes_NeighborScanPeriod_t *) (tlvptr);
            pscanperiod->SearchMode = le16_to_cpu(pscanperiod->SearchMode);
            pscanperiod->State = le16_to_cpu(pscanperiod->State);
            pscanperiod->ScanPeriod = le32_to_cpu(pscanperiod->ScanPeriod);
            state = pscanperiod->State;
            scanmode = pscanperiod->SearchMode;
            if ((state < STATE_STABLE) || (state > STATE_UNACCEPTABLE)) {
                printf("Incorrect state in response.\n");
            }
            if ((scanmode < DISCOVERY_MODE) || (scanmode > TRACKING_MODE)) {
                printf("Incorrect scanmode in response.\n");
            }
            values[state - 1][scanmode - 1] = pscanperiod->ScanPeriod;
            tlvptr += (le16_to_cpu(pscanperiod->Header.len) +
                       sizeof(MrvlIEtypesHeader_t));
            cmdresplen -= (le16_to_cpu(pscanperiod->Header.len) +
                           sizeof(MrvlIEtypesHeader_t));
            break;

        default:
            printf("Incorrect response.\n");
            break;
        }
    }

    for (state = STATE_STABLE - 1; state < STATE_UNACCEPTABLE; state++) {
        printf("\nState: %-14s  ", states[state]);
        for (scanmode = DISCOVERY_MODE - 1;
             scanmode < TRACKING_MODE; scanmode++) {
            printf("%s = %6d ms\t",
                   scanmodes[scanmode], (int) values[state][scanmode]);
        }
    }
    printf("\n");
    free(buf);
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief          Issue getra command 
 *
 *  @param pcontrol control struct to return 
 *
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
getControl(HostCmd_DS_CMD_ROAMAGENT_CONTROL * pcontrol)
{
    int ioctl_val, subioctl_val, cmd_len, cmdresplen;
    struct iwreq iwr;
    t_u8 *buf, *pos;
    HostCmd_DS_GEN *hostcmd;
    HostCmd_DS_CMD_ROAMAGENT_CONTROL *roamcontrolcmd;

    if (get_priv_ioctl("hostcmd",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    cmd_len = cmdresplen = sizeof(HostCmd_DS_GEN) +
        sizeof(roamcontrolcmd->action);
    cmdresplen += sizeof(HostCmd_DS_CMD_ROAMAGENT_CONTROL) -
        sizeof(roamcontrolcmd->action);

    buf = (t_u8 *) calloc(sizeof(t_u8), cmdresplen);
    if (!buf) {
        printf("Memory allocation failed\n");
        return -ENOMEM;
    }

    hostcmd = (HostCmd_DS_GEN *) buf;
    hostcmd->command = HostCmd_CMD_ROAMAGENT_CONTROL;
    hostcmd->size = 0;
    hostcmd->seq_num = 0;
    hostcmd->result = 0;

    pos = buf + sizeof(HostCmd_DS_GEN);

    roamcontrolcmd = (HostCmd_DS_CMD_ROAMAGENT_CONTROL *) pos;

    roamcontrolcmd->action = HostCmd_ACT_GEN_GET;

    /* 
     * ENDIANNESS 
     */
    hostcmd->command = cpu_to_le16(hostcmd->command);
    hostcmd->size = cpu_to_le16(cmd_len);

    roamcontrolcmd->action = cpu_to_le16(roamcontrolcmd->action);

    /* 
     * Prepare wireless request struct and fire ioctl
     */
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.flags = 0;
    iwr.u.data.pointer = (caddr_t) buf;
    iwr.u.data.length = cmd_len;

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("mlanconfig: roamcontrol ioctl");
        free(buf);
        return -EFAULT;
    }

    memcpy((void *) pcontrol, (void *) roamcontrolcmd,
           sizeof(HostCmd_DS_CMD_ROAMAGENT_CONTROL));
    free(buf);
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief          Issue getra command 
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *  @param control  roma control indicator value
 *
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
roamGetEventBitmap(int argc, char *argv[])
{
    t_u8 hostevent;
    HostCmd_DS_CMD_ROAMAGENT_CONTROL *roamcontrolcmd;

    if (getControl(roamcontrolcmd) != MLAN_STATUS_SUCCESS) {
        return MLAN_STATUS_FAILURE;
    }

    hostevent = roamcontrolcmd->HostEvent;
    if (!(hostevent | 0)) {
        printf("Host events are disabled.\n");
        return MLAN_STATUS_SUCCESS;
    }
    printf("Host events enabled: ");
    if (hostevent & HOST_EVENT_NBOR_ENABLE)
        printf("neighbor ");

    if (hostevent & HOST_EVENT_ROAM_ENABLE)
        printf("roam ");

    if (hostevent & HOST_EVENT_STATE_ENABLE)
        printf("state");

    printf("\n");
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief          Issue getra command 
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *  @param control  roma control indicator value
 *
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
roamGetControl(int argc, char *argv[])
{
    t_u8 control;
    HostCmd_DS_CMD_ROAMAGENT_CONTROL *roamcontrolcmd;

    if (getControl(roamcontrolcmd) != MLAN_STATUS_SUCCESS) {
        return MLAN_STATUS_FAILURE;
    }

    control = roamcontrolcmd->Control;
    printf("Global roaming agent state: ");
    if (control & ROAM_CONTROL_ENABLE) {
        printf("enabled, ");
        if (control & ROAM_CONTROL_SUSPEND)
            printf("suspend.\n");
        else
            printf("resume.\n");
    } else
        printf("disabled.\n");

    printf("Crossband: ");
    if (control & CROSSBAND_ENABLE)
        printf("enabled.\n");
    else
        printf("disabled.\n");

    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief                    Internal funtion to issue getra backoff
 *
 *  @param roambackoffcmd     Backoff command structure
 *  @return                   MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
getBackOff(HostCmd_DS_CMD_ROAMAGENT_BACKOFF * roambackoffcmdParam)
{
    int ioctl_val, subioctl_val, cmd_len, cmdresplen;
    struct iwreq iwr;
    t_u8 *buf, *pos;
    HostCmd_DS_GEN *hostcmd;
    HostCmd_DS_CMD_ROAMAGENT_BACKOFF *roambackoffcmd;

    if (get_priv_ioctl("hostcmd",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    cmd_len = cmdresplen = sizeof(HostCmd_DS_GEN) +
        sizeof(roambackoffcmd->action);
    cmdresplen += sizeof(HostCmd_DS_CMD_ROAMAGENT_BACKOFF) -
        sizeof(roambackoffcmd->action);

    buf = (t_u8 *) calloc(sizeof(t_u8), cmdresplen);
    if (!buf) {
        printf("Memory allocation failed\n");
        return -ENOMEM;
    }

    hostcmd = (HostCmd_DS_GEN *) buf;
    hostcmd->command = HostCmd_CMD_ROAMAGENT_BACKOFF;
    hostcmd->size = 0;
    hostcmd->seq_num = 0;
    hostcmd->result = 0;

    pos = buf + sizeof(HostCmd_DS_GEN);

    roambackoffcmd = (HostCmd_DS_CMD_ROAMAGENT_BACKOFF *) pos;

    roambackoffcmd->action = HostCmd_ACT_GEN_GET;

    /* 
     * ENDIANNESS 
     */
    hostcmd->command = cpu_to_le16(hostcmd->command);
    hostcmd->size = cpu_to_le16(cmd_len);

    roambackoffcmd->action = cpu_to_le16(roambackoffcmd->action);

    /* 
     * Prepare wireless request struct and fire ioctl
     */
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.flags = 0;
    iwr.u.data.pointer = (caddr_t) buf;
    iwr.u.data.length = cmd_len;

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("mlanconfig: roambackoff ioctl");
        free(buf);
        return -EFAULT;
    }
    /* 
     * ENDIANNESS for Response 
     */
    roambackoffcmd->Scans = le16_to_cpu(roambackoffcmd->Scans);
    roambackoffcmd->Period = le32_to_cpu(roambackoffcmd->Period);

    memcpy((void *) roambackoffcmdParam, (void *) roambackoffcmd,
           sizeof(HostCmd_DS_CMD_ROAMAGENT_BACKOFF));

    free(buf);
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief          Issue getra backoff command 
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *  @param control  roma control indicator value
 *
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
roamGetBackOff(int argc, char *argv[])
{
    HostCmd_DS_CMD_ROAMAGENT_BACKOFF backoffcmd;

    if (getBackOff(&backoffcmd) != MLAN_STATUS_SUCCESS) {
        return MLAN_STATUS_FAILURE;
    }

    puts("");
    printf("----------------------------------------------------\n");
    printf("           Scanning Backoff Parameters              \n");
    printf("----------------------------------------------------\n");
    printf(" Backoff period (max time in tracking) = %ld ms\n",
           backoffcmd.Period);
    printf(" # of discovery scans before backoff   = %d\n", backoffcmd.Scans);
    puts("");

    if (backoffcmd.Scans) {
        printf(" - Discovery backoff mode is enabled.\n"
               "   After %d discovery scans without a change in the number\n"
               "   of neighbors, the RA will only track the existing\n"
               "   neighbors until the backoff period expires.\n",
               backoffcmd.Scans);
    } else {
        printf(" - Discovery backoff mode is disabled.\n"
               "   The RA will only move to tracking mode if a minimum\n"
               "   number of good neighbors have been found. See the\n"
               "   getra neighbor command for the current min setting.\n");
    }
    puts("");

    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Get the Roaming agent configuration parameters from FW.
 *  
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
int
process_get_ra_config(int argc, char *argv[])
{
    if (argc == 3) {
        return roamGetControl(argc, argv);
    }
    sub_cmd_exec_t subCmds[] = { {"rssi", 1, roamGetRssiStatsThreshold}
    ,
    {"prebcnmiss", 1, roamGetPreBeaconMiss}
    ,
    {"failcnt", 1, roamGetFailureCount}
    ,
    {"backoff", 1, roamGetBackOff}
    ,
    {"neighbor", 1, roamGetNeighbors}
    ,
    {"scanperiod", 1, roamGetScanPeriod}
    ,
    {"metrics", 1, roamGetMetrics}
    ,
    {"event", 1, roamGetEventBitmap}
    };

    return (process_sub_cmd(subCmds, NELEMENTS(subCmds), argc, argv));
}

/** 
 *  @brief          Issue setra bcnmiss command 
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
roamSetPreBeaconLoss(int argc, char *argv[])
{
    int ioctl_val, subioctl_val, cmd_len;
    int bcnmiss;
    struct iwreq iwr;
    t_u8 *buf, *pos;
    HostCmd_DS_GEN *hostcmd;
    HostCmd_DS_CMD_ROAMAGENT_STATISTICS_THRESHOLD *statsthreshold;
    MrvlIEtypes_BeaconsMissed_t *beaconmiss;

    if (argc != 1) {
        fprintf(stderr, "Incorrect number of arguments.\n");
        return MLAN_STATUS_FAILURE;
    }

    bcnmiss = atoi(argv[0]);

    if (get_priv_ioctl("hostcmd",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    cmd_len = sizeof(HostCmd_DS_GEN) + sizeof(statsthreshold->action) +
        sizeof(statsthreshold->State) + sizeof(statsthreshold->Profile) +
        sizeof(MrvlIEtypes_BeaconsMissed_t);

    buf = (t_u8 *) calloc(sizeof(t_u8), cmd_len);
    if (!buf) {
        printf("Memory allocation failed\n");
        return -ENOMEM;
    }

    hostcmd = (HostCmd_DS_GEN *) buf;
    hostcmd->command = cpu_to_le16(HostCmd_CMD_ROAMAGENT_STATISTICS_THRESHOLD);
    hostcmd->size = 0;
    hostcmd->seq_num = 0;
    hostcmd->result = 0;

    pos = buf + sizeof(HostCmd_DS_GEN);

    statsthreshold = (HostCmd_DS_CMD_ROAMAGENT_STATISTICS_THRESHOLD *) pos;
    statsthreshold->action = cpu_to_le16(HostCmd_ACT_GEN_SET);
    statsthreshold->State = STATE_HARDROAM;

    beaconmiss = (MrvlIEtypes_BeaconsMissed_t *) & statsthreshold->TlvBuffer;
    beaconmiss->beacon_missed = bcnmiss;
    beaconmiss->header.type = cpu_to_le16(TLV_TYPE_PRE_BEACON_LOST);
    beaconmiss->header.len =
        cpu_to_le16(sizeof(MrvlIEtypes_BeaconsMissed_t) -
                    sizeof(MrvlIEtypesHeader_t));

    hostcmd->size = cpu_to_le16(cmd_len);

    /* 
     * Prepare wireless request struct and fire ioctl
     */
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.flags = 0;
    iwr.u.data.pointer = (caddr_t) buf;
    iwr.u.data.length = cmd_len;

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("mlanconfig: setra ioctl");
        free(buf);
        return -EFAULT;
    }

    if (!le16_to_cpu(hostcmd->result)) {
        printf("HOSTCMD_RESP: ReturnCode=%#04x, Result=%#04x\n",
               le16_to_cpu(hostcmd->command), le16_to_cpu(hostcmd->result));
    } else {
        printf("HOSTCMD failed: ReturnCode=%#04x, Result=%#04x\n",
               le16_to_cpu(hostcmd->command), le16_to_cpu(hostcmd->result));
    }

    free(buf);
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief          Issue setra failurecnt command 
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
roamSetFailureCount(int argc, char *argv[])
{
    int ioctl_val, subioctl_val, cmd_len;
    int value;
    struct iwreq iwr;
    t_u8 *buf, *pos;
    HostCmd_DS_GEN *hostcmd;
    HostCmd_DS_CMD_ROAMAGENT_STATISTICS_THRESHOLD *statsthreshold;
    MrvlIEtypes_FailureCount_t *failcnt;

    if (argc != 1) {
        fprintf(stderr, "Incorrect number of arguments.\n");
        return MLAN_STATUS_FAILURE;
    }

    value = atoi(argv[0]);

    if (get_priv_ioctl("hostcmd",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    cmd_len = sizeof(HostCmd_DS_GEN) + sizeof(statsthreshold->action) +
        sizeof(statsthreshold->State) + sizeof(statsthreshold->Profile) +
        sizeof(MrvlIEtypes_FailureCount_t);

    buf = (t_u8 *) calloc(sizeof(t_u8), cmd_len);
    if (!buf) {
        printf("Memory allocation failed\n");
        return -ENOMEM;
    }

    hostcmd = (HostCmd_DS_GEN *) buf;
    hostcmd->command = cpu_to_le16(HostCmd_CMD_ROAMAGENT_STATISTICS_THRESHOLD);
    hostcmd->size = 0;
    hostcmd->seq_num = 0;
    hostcmd->result = 0;

    pos = buf + sizeof(HostCmd_DS_GEN);

    statsthreshold = (HostCmd_DS_CMD_ROAMAGENT_STATISTICS_THRESHOLD *) pos;
    statsthreshold->action = cpu_to_le16(HostCmd_ACT_GEN_SET);
    statsthreshold->State = STATE_HARDROAM;

    failcnt = (MrvlIEtypes_FailureCount_t *) & statsthreshold->TlvBuffer;
    failcnt->fail_value = value;
    failcnt->header.type = cpu_to_le16(TLV_TYPE_FAILCOUNT);
    failcnt->header.len =
        cpu_to_le16(sizeof(MrvlIEtypes_FailureCount_t) -
                    sizeof(MrvlIEtypesHeader_t));

    hostcmd->size = cpu_to_le16(cmd_len);

    /* 
     * Prepare wireless request struct and fire ioctl
     */
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.flags = 0;
    iwr.u.data.pointer = (caddr_t) buf;
    iwr.u.data.length = cmd_len;

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("mlanconfig: setra ioctl");
        free(buf);
        return -EFAULT;
    }

    if (!le16_to_cpu(hostcmd->result)) {
        printf("HOSTCMD_RESP: ReturnCode=%#04x, Result=%#04x\n",
               le16_to_cpu(hostcmd->command), le16_to_cpu(hostcmd->result));
    } else {
        printf("HOSTCMD failed: ReturnCode=%#04x, Result=%#04x\n",
               le16_to_cpu(hostcmd->command), le16_to_cpu(hostcmd->result));
    }

    free(buf);
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief          Issue setra rssi/snr command 
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *  @param type     RSSI/SNR threshold type
 *
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
roamSetStatsThreshold(int argc, char *argv[], int type)
{
    int ioctl_val, subioctl_val, cmd_len;
    int i, state = 0, low = 0, high = 0, lowval, highval;
    struct iwreq iwr;
    t_u8 *buf, *pos;
    HostCmd_DS_GEN *hostcmd;
    HostCmd_DS_CMD_ROAMAGENT_STATISTICS_THRESHOLD *statsthreshold;
    MrvlIEtypes_BeaconHighRssiThreshold_t *bcnhighrssi;
    MrvlIEtypes_BeaconLowRssiThreshold_t *bcnlowrssi;
    MrvlIEtypes_BeaconHighSnrThreshold_t *bcnhighsnr;
    MrvlIEtypes_BeaconLowSnrThreshold_t *bcnlowsnr;

    if (argv[0] == NULL) {
        printf("\nUnknown setra command... \n");
        return MLAN_STATUS_FAILURE;
    }

    if (argv[0] && strncmp(argv[0], "stable", strlen("stable")) == 0) {
        state = STATE_STABLE;
        /* degrad[ing] */
    } else if (argv[0] && strncmp(argv[0], "degrad", strlen("degrad")) == 0) {
        state = STATE_DEGRADING;
        /* unaccep[table] */
    } else if (argv[0] && strncmp(argv[0], "unaccep", strlen("unaccep")) == 0) {
        state = STATE_UNACCEPTABLE;
    } else {
        fprintf(stderr, "Unknown state.Use stable/degrading/unacceptable\n");
        return MLAN_STATUS_FAILURE;
    }
    if (argc != 3 && argc != 5) {
        fprintf(stderr, "Incorrect number of arguments.\n");
        return MLAN_STATUS_FAILURE;
    }
    if (argv[1] && strncmp(argv[1], "low", strlen("low")) == 0) {
        low = 1;
        lowval = atoi(argv[2]);
        /* TODO validation check on lowval */
    }
    if (argv[1 + (2 * low)] &&
        strncmp(argv[1 + (2 * low)], "high", strlen("high")) == 0) {
        high = 1;
        highval = atoi(argv[2 + (2 * low)]);
        /* TODO validation check on highval */
    }
    /* check if low is specified after high */
    if ((low == 0) && (argc == 5)) {
        if (argv[1 + (2 * high)] &&
            strncmp(argv[1 + (2 * high)], "low", strlen("low")) == 0) {
            low = 1;
            lowval = atoi(argv[2 + 2 * (high)]);
            /* TODO validation check on lowval */
        }
    }

    if (!low && !high) {
        fprintf(stderr,
                "Incorrect argument. Use low <lowval>/high <highval>\n");
        return MLAN_STATUS_FAILURE;
    }
    if (get_priv_ioctl("hostcmd",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    cmd_len = sizeof(HostCmd_DS_GEN) + sizeof(statsthreshold->action) +
        sizeof(statsthreshold->State) + sizeof(statsthreshold->Profile);
    if (type == RSSI_THRESHOLD) {
        if (high) {
            cmd_len += sizeof(MrvlIEtypes_BeaconHighRssiThreshold_t);
        }
        if (low) {
            cmd_len += sizeof(MrvlIEtypes_BeaconLowRssiThreshold_t);
        }
    } else {
        if (high) {
            cmd_len += sizeof(MrvlIEtypes_BeaconHighSnrThreshold_t);
        }
        if (low) {
            cmd_len += sizeof(MrvlIEtypes_BeaconLowSnrThreshold_t);
        }
    }

    buf = (t_u8 *) calloc(sizeof(t_u8), cmd_len);
    if (!buf) {
        printf("Memory allocation failed\n");
        return -ENOMEM;
    }

    hostcmd = (HostCmd_DS_GEN *) buf;
    hostcmd->command = HostCmd_CMD_ROAMAGENT_STATISTICS_THRESHOLD;
    hostcmd->size = 0;
    hostcmd->seq_num = 0;
    hostcmd->result = 0;

    pos = buf + sizeof(HostCmd_DS_GEN);

    statsthreshold = (HostCmd_DS_CMD_ROAMAGENT_STATISTICS_THRESHOLD *) pos;

    statsthreshold->action = HostCmd_ACT_GEN_SET;
    statsthreshold->State = state;

    /* 
     * TLV buffer start pointer initialization
     */
    i = 0;
    if (type == RSSI_THRESHOLD) {
        if (high) {
            bcnhighrssi = (MrvlIEtypes_BeaconHighRssiThreshold_t *)
                (((t_u8 *) & statsthreshold->TlvBuffer) + i);
            bcnhighrssi->Header.type = TLV_TYPE_RSSI_HIGH;
            bcnhighrssi->Header.len =
                sizeof(MrvlIEtypes_BeaconHighRssiThreshold_t) -
                sizeof(MrvlIEtypesHeader_t);
            bcnhighrssi->Value = highval;
            i += sizeof(MrvlIEtypes_BeaconHighRssiThreshold_t);
            /* 
             * ENDIANNESS 
             */
            bcnhighrssi->Header.type = cpu_to_le16(bcnhighrssi->Header.type);
            bcnhighrssi->Header.len = cpu_to_le16(bcnhighrssi->Header.len);
        }
        if (low) {
            bcnlowrssi = (MrvlIEtypes_BeaconLowRssiThreshold_t *)
                (((t_u8 *) & statsthreshold->TlvBuffer) + i);
            bcnlowrssi->Header.type = TLV_TYPE_RSSI_LOW;
            bcnlowrssi->Header.len =
                sizeof(MrvlIEtypes_BeaconLowRssiThreshold_t) -
                sizeof(MrvlIEtypesHeader_t);
            bcnlowrssi->Value = lowval;
            i += sizeof(MrvlIEtypes_BeaconLowRssiThreshold_t);
            /* 
             * ENDIANNESS 
             */
            bcnlowrssi->Header.type = cpu_to_le16(bcnlowrssi->Header.type);
            bcnlowrssi->Header.len = cpu_to_le16(bcnlowrssi->Header.len);
        }
    } else {
        if (high) {
            bcnhighsnr = (MrvlIEtypes_BeaconHighSnrThreshold_t *)
                (((t_u8 *) & statsthreshold->TlvBuffer) + i);
            bcnhighsnr->Header.type = TLV_TYPE_SNR_HIGH;
            bcnhighsnr->Header.len =
                sizeof(MrvlIEtypes_BeaconHighSnrThreshold_t) -
                sizeof(MrvlIEtypesHeader_t);
            bcnhighsnr->Value = highval;
            i += sizeof(MrvlIEtypes_BeaconHighSnrThreshold_t);
            /* 
             * ENDIANNESS 
             */
            bcnhighsnr->Header.type = cpu_to_le16(bcnhighsnr->Header.type);
            bcnhighsnr->Header.len = cpu_to_le16(bcnhighsnr->Header.len);
        }
        if (low) {
            bcnlowsnr = (MrvlIEtypes_BeaconLowSnrThreshold_t *)
                (((t_u8 *) & statsthreshold->TlvBuffer) + i);
            bcnlowsnr->Header.type = TLV_TYPE_SNR_LOW;
            bcnlowsnr->Header.len =
                sizeof(MrvlIEtypes_BeaconLowSnrThreshold_t) -
                sizeof(MrvlIEtypesHeader_t);
            bcnlowsnr->Value = lowval;
            i += sizeof(MrvlIEtypes_BeaconLowSnrThreshold_t);
            /* 
             * ENDIANNESS 
             */
            bcnlowsnr->Header.type = cpu_to_le16(bcnlowsnr->Header.type);
            bcnlowsnr->Header.len = cpu_to_le16(bcnlowsnr->Header.len);
        }
    }

    /* 
     * ENDIANNESS 
     */
    hostcmd->command = cpu_to_le16(hostcmd->command);
    hostcmd->size = cpu_to_le16(cmd_len);

    statsthreshold->action = cpu_to_le16(statsthreshold->action);

    /* 
     * Prepare wireless request struct and fire ioctl
     */
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.flags = 0;
    iwr.u.data.pointer = (caddr_t) buf;
    iwr.u.data.length = sizeof(HostCmd_DS_GEN) + sizeof(statsthreshold->action)
        + sizeof(statsthreshold->State) + sizeof(statsthreshold->Profile) + i;

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("mlanconfig: setra ioctl");
        free(buf);
        return -EFAULT;
    }

    if (!le16_to_cpu(hostcmd->result)) {
        printf("HOSTCMD_RESP: ReturnCode=%#04x, Result=%#04x\n",
               le16_to_cpu(hostcmd->command), le16_to_cpu(hostcmd->result));
    } else {
        printf("HOSTCMD failed: ReturnCode=%#04x, Result=%#04x\n",
               le16_to_cpu(hostcmd->command), le16_to_cpu(hostcmd->result));
    }

    free(buf);
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief          Issue setra rssi command 
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
roamSetRssiStatsThreshold(int argc, char *argv[])
{
    return roamSetStatsThreshold(argc, argv, RSSI_THRESHOLD);
}

/** 
 *  @brief          Issue setra scanperiod command 
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
roamSetScanPeriod(int argc, char *argv[])
{
    int ioctl_val, subioctl_val, cmd_len;
    int scanmode = 0, period = 0, state = 0;
    struct iwreq iwr;
    t_u8 *buf, *pos;
    HostCmd_DS_GEN *hostcmd;
    HostCmd_DS_CMD_ROAMAGENT_NEIGHBOR_SCANPERIOD *scanPeriodInfo;

    if (argv[0] == NULL) {
        printf("\nUnknown setra command... \n");
        return MLAN_STATUS_FAILURE;
    }
    /* scanperiod */
    if (argc != 3) {
        fprintf(stderr, "Incorrect number of arguments.\n");
        return MLAN_STATUS_FAILURE;
    }
    if (get_priv_ioctl("hostcmd",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    if (argv[0] && strncmp(argv[0], "stable", strlen("stable")) == 0) {
        state = STATE_STABLE;
        /* degrad[ing] */
    } else if (argv[0] && strncmp(argv[0], "degrad", strlen("degrad")) == 0) {
        state = STATE_DEGRADING;
        /* unaccep[table] */
    } else if (argv[0] && strncmp(argv[0], "unaccep", strlen("unaccep")) == 0) {
        state = STATE_UNACCEPTABLE;
    } else {
        fprintf(stderr, "Unknown state.Use stable/degrading/unacceptable\n");
        return MLAN_STATUS_FAILURE;
    }

    if (argv[1] && strncmp(argv[1], "disc", strlen("disc")) == 0) {
        scanmode = DISCOVERY_MODE;
        /* track[ing] */
    } else if (argv[1] && strncmp(argv[1], "track", strlen("track")) == 0) {
        scanmode = TRACKING_MODE;
    } else {
        fprintf(stderr, "Unknown scamode. Use discovery/ tracking\n");
        return MLAN_STATUS_FAILURE;
    }
    period = atoi(argv[2]);
    /* TODO validation check on period */

    cmd_len = sizeof(HostCmd_DS_GEN) +
        sizeof(HostCmd_DS_CMD_ROAMAGENT_NEIGHBOR_SCANPERIOD);

    buf = (t_u8 *) calloc(sizeof(t_u8), cmd_len);
    if (!buf) {
        printf("Memory allocation failed\n");
        return -ENOMEM;
    }

    hostcmd = (HostCmd_DS_GEN *) buf;
    hostcmd->command = HostCmd_CMD_ROAMAGENT_NEIGHBOR_SCAN_PERIOD;
    hostcmd->size = 0;
    hostcmd->seq_num = 0;
    hostcmd->result = 0;

    pos = buf + sizeof(HostCmd_DS_GEN);

    scanPeriodInfo = (HostCmd_DS_CMD_ROAMAGENT_NEIGHBOR_SCANPERIOD *) pos;

    scanPeriodInfo->action = HostCmd_ACT_GEN_SET;
    scanPeriodInfo->scanPeriod.Header.type = TLV_TYPE_NEIGHBOR_SCANPERIOD;
    scanPeriodInfo->scanPeriod.Header.len =
        sizeof(MrvlIEtypes_NeighborScanPeriod_t) - sizeof(MrvlIEtypesHeader_t);
    scanPeriodInfo->scanPeriod.SearchMode = scanmode;
    scanPeriodInfo->scanPeriod.State = state;
    scanPeriodInfo->scanPeriod.ScanPeriod = period;

    /* 
     * ENDIANNESS 
     */
    hostcmd->command = cpu_to_le16(hostcmd->command);
    hostcmd->size = cpu_to_le16(cmd_len);

    scanPeriodInfo->action = cpu_to_le16(scanPeriodInfo->action);
    scanPeriodInfo->scanPeriod.Header.type = cpu_to_le16
        (scanPeriodInfo->scanPeriod.Header.type);
    scanPeriodInfo->scanPeriod.Header.len = cpu_to_le16
        (scanPeriodInfo->scanPeriod.Header.len);
    scanPeriodInfo->scanPeriod.SearchMode = cpu_to_le16
        (scanPeriodInfo->scanPeriod.SearchMode);
    scanPeriodInfo->scanPeriod.State = cpu_to_le16
        (scanPeriodInfo->scanPeriod.State);
    scanPeriodInfo->scanPeriod.ScanPeriod = cpu_to_le32
        (scanPeriodInfo->scanPeriod.ScanPeriod);

    /* 
     * Prepare wireless request struct and fire ioctl
     */
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.flags = 0;
    iwr.u.data.pointer = (caddr_t) buf;
    iwr.u.data.length = cmd_len;

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("mlanconfig: roamscanperiod ioctl");
        free(buf);
        return -EFAULT;
    }

    if (!le16_to_cpu(hostcmd->result)) {
        printf("HOSTCMD_RESP: ReturnCode=%#04x, Result=%#04x\n",
               le16_to_cpu(hostcmd->command), le16_to_cpu(hostcmd->result));
    } else {
        printf("HOSTCMD failed: ReturnCode=%#04x, Result=%#04x\n",
               le16_to_cpu(hostcmd->command), le16_to_cpu(hostcmd->result));
    }

    free(buf);
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief          Check it string contains digits 
 *  @param str      string to check for digits
 *  @return         same as isdigit(char)
 */
static inline int
isdigitstr(char *str)
{
    int i = 0;
    for (i = 0; i < strlen(str); i++) {
        if (!isdigit((str)[i]))
            return 0;
    }
    return 1;
}

/**
 *  @brief          Process sub command 
 *
 *  @param valid_cmds   valid commands array
 *  @param count        number of  valid commands
 *  @param argc         number of arguments
 *  @param argv         A pointer to arguments array    
 *  @param param        argc parameter counter    
 *  @param value        values to update back    
 *
 *  @return             command index--success, otherwise--fail
 */
static int
process_subcmd(char *valid_cmds[], int count, int argc, char *argv[],
               int *param, int *value)
{
    int ret = 0;
    int j = *param;
    int k;
    while (1) {
        for (k = 0; k < count; k++)
            if (argv[j] &&
                !strncmp(valid_cmds[k], argv[j], strlen(valid_cmds[k])))
                break;
        if (k >= count)
            break;
        else {
            /* special case */
            if (!strncmp(valid_cmds[k], "perperiod", strlen("perperiod")) ||
                !strncmp(valid_cmds[k], "ferperiod", strlen("ferperiod"))) {
                *param = j;
                return (ret | 1 << k);
            }
            if (!argv[j + 1] || !isdigitstr(argv[j + 1]))
                return -1;
            value[k] = atoi(argv[j + 1]);
            j = j + 2;
            ret |= (1 << k);
        }
        if (j >= argc)
            break;
    }
    if (*param == j)
        return -1;
    *param = j;
    return ret;
}

/** 
 *  @brief          Issue setra neighbor command 
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
roamSetNborAssessment(int argc, char *argv[])
{
    int ioctl_val, subioctl_val, cmd_len, ret;
    int trackcount = 0;
    /** blacklist short and long */
    int blist[2] = { 0, 0 };    /* 0-short 1-long */
    int qualoffset[1] = { 0 };  /* rssi */
    int stalecount = ~0, staleperiod = ~0;
    struct iwreq iwr;
    t_u8 *buf, *pos;
    char *valid_cmds[] = { "trackcount", "qualoffset", "blacklistdur",
        "stalecount", "staleperiod"
    };

    /** blacklistdur sub commands */
    char *valid_blcmds[] = { "short", "long" };
    /** qualoffset sub commands */
    char *valid_bfcmds[] = { "rssi" };

    int i = 0, j = 0;

    HostCmd_DS_GEN *hostcmd;
    HostCmd_DS_CMD_ROAMAGENT_NEIGHBOR_ASSESSMENT *pNborAssessment;

    if (argv[0] == NULL) {
        printf("\nUnknown setra command... \n");
        return MLAN_STATUS_FAILURE;
    }

    if (argc < 2 || argc > 18) {
        fprintf(stderr, "Incorrect number of arguments.\n");
        return MLAN_STATUS_FAILURE;
    }
    if (get_priv_ioctl("hostcmd",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    cmd_len = sizeof(HostCmd_DS_GEN) +
        sizeof(HostCmd_DS_CMD_ROAMAGENT_NEIGHBOR_ASSESSMENT);

    buf = (t_u8 *) calloc(sizeof(t_u8), cmd_len);
    if (!buf) {
        printf("Memory allocation failed\n");
        return -ENOMEM;
    }

    hostcmd = (HostCmd_DS_GEN *) buf;
    hostcmd->command = HostCmd_CMD_ROAMAGENT_NEIGHBOR_ASSESSMENT;
    hostcmd->size = 0;
    hostcmd->seq_num = 0;
    hostcmd->result = 0;

    pos = buf + sizeof(HostCmd_DS_GEN);

    pNborAssessment = (HostCmd_DS_CMD_ROAMAGENT_NEIGHBOR_ASSESSMENT *) pos;

    /* Get parameters unspecified on command line */
    if (getNborAssessment(pNborAssessment) != MLAN_STATUS_SUCCESS) {
        free(buf);
        return MLAN_STATUS_FAILURE;
    }

    while (argc - j) {
        for (i = 0; i < NELEMENTS(valid_cmds); i++) {
            if (!strncmp(valid_cmds[i], argv[j], strlen(valid_cmds[i])))
                break;
        }
        if (i >= NELEMENTS(valid_cmds)) {
            fprintf(stderr, "Invalid argument (to) \"%s\"\n", argv[0]);
            return MLAN_STATUS_FAILURE;
        }
        switch (i) {

        case 0:/** trackcount */
            trackcount = atoi(argv[j + 1]);
            pNborAssessment->QualifyingNumNeighbor = trackcount;
            j = j + 2;
            break;

        case 3:/** stalecount */
            stalecount = atoi(argv[j + 1]);
            pNborAssessment->StaleCount = stalecount;
            j = j + 2;
            break;

        case 4:/** staleperiod */
            staleperiod = atoi(argv[j + 1]);
            pNborAssessment->StalePeriod = staleperiod;
            j = j + 2;
            break;

        case 1:/** qualoffset */
            j++;
            ret = process_subcmd(valid_bfcmds, NELEMENTS(valid_bfcmds),
                                 argc, argv, &j, qualoffset);
            if (ret == -1) {
                fprintf(stderr, "Invalid argument (to) \"%s\"\n", argv[j - 1]);
                free(buf);
                return ret;
            }
            pNborAssessment->ConfQualSignalStrength = qualoffset[0];
            break;

        case 2:/** blacklistdur */
            j++;
            ret = process_subcmd(valid_blcmds, NELEMENTS(valid_blcmds),
                                 argc, argv, &j, blist);
            if (ret == -1) {
                fprintf(stderr, "Invalid argument (to) \"%s\"\n", argv[j - 1]);
                free(buf);
                return ret;
            } else {
                if (ret & 0x01) {
                    pNborAssessment->ShortBlacklistPeriod = blist[0];
                }
                if (ret & 0x02) {
                    pNborAssessment->LongBlacklistPeriod = blist[1];
                }
            }
            break;
        }
    }

    pNborAssessment->action = HostCmd_ACT_GEN_SET;

    /* Both stalecount, staleperiod can not be zero */
    if ((pNborAssessment->StaleCount == 0) &&
        (pNborAssessment->StalePeriod == 0)) {
        fprintf(stderr, "stalecount and staleperiod both can not be zero\n");
        free(buf);
        return (MLAN_STATUS_FAILURE);
    }

    /* 
     * ENDIANNESS 
     */
    hostcmd->command = cpu_to_le16(hostcmd->command);
    hostcmd->size = cpu_to_le16(cmd_len);

    pNborAssessment->QualifyingNumNeighbor =
        cpu_to_le16(pNborAssessment->QualifyingNumNeighbor);
    pNborAssessment->ShortBlacklistPeriod =
        cpu_to_le32(pNborAssessment->ShortBlacklistPeriod);
    pNborAssessment->LongBlacklistPeriod =
        cpu_to_le32(pNborAssessment->LongBlacklistPeriod);
    pNborAssessment->StaleCount = cpu_to_le16(pNborAssessment->StaleCount);
    pNborAssessment->StalePeriod = cpu_to_le16(pNborAssessment->StalePeriod);

    /* 
     * Prepare wireless request struct and fire ioctl
     */
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.flags = 0;
    iwr.u.data.pointer = (caddr_t) buf;
    iwr.u.data.length = cmd_len;

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("mlanconfig: roam neighbor assessment ioctl");
        free(buf);
        return -EFAULT;
    }

    if (!le16_to_cpu(hostcmd->result)) {
        printf("HOSTCMD_RESP: ReturnCode=%#04x, Result=%#04x\n",
               le16_to_cpu(hostcmd->command), le16_to_cpu(hostcmd->result));
    } else {
        printf("HOSTCMD failed: ReturnCode=%#04x, Result=%#04x\n",
               le16_to_cpu(hostcmd->command), le16_to_cpu(hostcmd->result));
    }

    free(buf);
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief          Issue setra metrics command 
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 * 
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
roamSetMetrics(int argc, char *argv[])
{
    int ioctl_val, subioctl_val, cmd_len, ret;
    HostCmd_DS_GEN *hostcmd;
    HostCmd_DS_CMD_ROAMAGENT_ADV_METRIC_THRESHOLD *metricscmd;

    int perlist[3] = { 0, 0, 0 };       /* 0=inactivity, 1=perthreshold,
                                           2=packetthreshold */
    int ferlist[3] = { 0, 0, 0 };       /* 0=inactivity, 1=ferthreshold,
                                           2=framethreshold */
    int perstate[3] = { 0, 0, 0 };      /* 0=stable, 1=degrading,
                                           2=unacceptable */
    int ferstate[3] = { 0, 0, 0 };      /* 0=stable, 1=degrading,
                                           2=unacceptable */
    int datalist[2] = { 0, 0 }; /* 0=inactivity 1=datathreshold */
    struct iwreq iwr;
    t_u8 *buf, *pos;
    char *valid_cmds[] = { "Beacon", "Data", "PER", "FER" };
    /** PER sub commands */
    char *valid_percmds[] = { "inactivity", "perthreshold",
        "packetthreshold", "perperiod"
    };
    char *valid_fercmds[] = { "inactivity", "ferthreshold",
        "framethreshold", "ferperiod"
    };
    /** PER period states */
    char *per_states[] = { "stable", "degrading", "unacceptable" };
    /** Data sub commands */
    char *valid_datacmds[] = { "inactivity", "datathreshold" };

    int i = 0, j = 0;

    if (argv[0] == NULL) {
        printf("\nUnknown setra command... \n");
        return MLAN_STATUS_FAILURE;
    }

    if (argc < 1 || argc > 38) {
        fprintf(stderr, "Incorrect number of arguments.\n");
        return MLAN_STATUS_FAILURE;
    }
    if (get_priv_ioctl("hostcmd",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    cmd_len = (sizeof(HostCmd_DS_GEN) +
               sizeof(HostCmd_DS_CMD_ROAMAGENT_ADV_METRIC_THRESHOLD));

    buf = (t_u8 *) calloc(sizeof(t_u8), cmd_len);
    if (!buf) {
        printf("Memory allocation failed\n");
        return -ENOMEM;
    }

    hostcmd = (HostCmd_DS_GEN *) buf;
    hostcmd->command = HostCmd_CMD_ROAMAGENT_ADV_METRIC_THRESHOLD;
    hostcmd->size = 0;
    hostcmd->seq_num = 0;
    hostcmd->result = 0;

    pos = buf + sizeof(HostCmd_DS_GEN);

    metricscmd = (HostCmd_DS_CMD_ROAMAGENT_ADV_METRIC_THRESHOLD *) pos;
    metricscmd->action = HostCmd_ACT_GEN_SET;

    /* clear [ Beacon/Data/PER ] */
    if (!strncmp("clear", argv[j], strlen("clear"))) {
        if (argc == 1) {
            fprintf(stderr, "Invalid number of argument");
            free(buf);
            return MLAN_STATUS_FAILURE;
        }
        while (1) {
            j++;
            for (i = 0; i < NELEMENTS(valid_cmds); i++) {
                if (!strncmp(valid_cmds[i], argv[j], strlen(valid_cmds[i])))
                    break;
            }
            if (i >= NELEMENTS(valid_cmds)) {
                fprintf(stderr, "Invalid argument \"%s\"\n", argv[j]);
                free(buf);
                return MLAN_STATUS_FAILURE;
            } else {
                metricscmd->Metrics |= BIT(i);
            }
            if (j >= (argc - 1))
                break;
        }
        metricscmd->action = HostCmd_ACT_GEN_REMOVE;
    }

    /* set [ Beacon/Data/PER ] */
    else {

        while (argc - j) {
            for (i = 0; i < NELEMENTS(valid_cmds); i++) {
                if (!strncmp(valid_cmds[i], argv[j], strlen(valid_cmds[i])))
                    break;
            }
            if (i >= NELEMENTS(valid_cmds)) {
                fprintf(stderr, "Invalid argument (to) \"%s\"\n", argv[j]);
                free(buf);
                return MLAN_STATUS_FAILURE;
            }
            switch (i) {

            case 0:/** Beacon */
                j++;
                metricscmd->Metrics |= 0x01;
                break;

            case 1:/** Data*/
                j++;
                ret = process_subcmd(valid_datacmds, NELEMENTS(valid_datacmds),
                                     argc, argv, &j, datalist);
                if (ret > 0) {
                    if (ret & 0x01) {
                        metricscmd->InactivityPeriodThreshold_ms = datalist[0];
                    }
                    if (ret & 0x02) {
                        metricscmd->UiRxPktThreshold = datalist[1];
                    }
                }
                metricscmd->Metrics |= 0x02;
                break;

            case 2:/** PER */
                j++;
                ret = process_subcmd(valid_percmds, NELEMENTS(valid_percmds),
                                     argc, argv, &j, perlist);
                if (ret > 0) {
                    if (ret & 0x01) {
                        metricscmd->InactivityPeriodThreshold_ms = perlist[0];
                    }
                    if (ret & 0x02) {
                        metricscmd->UcPerThresholdValue = perlist[1];
                    }
                    if (ret & 0x04) {
                        metricscmd->UiPerPktThreshold = perlist[2];
                    }
                    if (ret & 0x08) {
                        j++;
                        ret = process_subcmd(per_states, NELEMENTS(per_states),
                                             argc, argv, &j, perstate);
                        if (ret & 0x01) {
                            metricscmd->StablePERPeriod_ms = perstate[0];
                        }
                        if (ret & 0x02) {
                            metricscmd->DegradingPERPeriod_ms = perstate[1];
                        }
                        if (ret & 0x04) {
                            metricscmd->UnacceptablePERPeriod_ms = perstate[2];
                        }
                        if (j < argc) {
                            ret =
                                process_subcmd(valid_percmds,
                                               NELEMENTS(valid_percmds), argc,
                                               argv, &j, perlist);
                            if (ret > 0) {
                                if (ret & 0x01) {
                                    metricscmd->InactivityPeriodThreshold_ms =
                                        perlist[0];
                                }
                                if (ret & 0x02) {
                                    metricscmd->UcPerThresholdValue =
                                        perlist[1];
                                }
                                if (ret & 0x04) {
                                    metricscmd->UiPerPktThreshold = perlist[2];
                                }
                            }
                        }
                    }
                }
                metricscmd->Metrics |= 0x04;
                break;

            case 3:/** FER */
                j++;
                ret = process_subcmd(valid_fercmds, NELEMENTS(valid_fercmds),
                                     argc, argv, &j, ferlist);
                if (ret > 0) {
                    if (ret & 0x01) {
                        metricscmd->InactivityPeriodThreshold_ms = ferlist[0];
                    }
                    if (ret & 0x02) {
                        metricscmd->UcFerThresholdValue = ferlist[1];
                    }
                    if (ret & 0x04) {
                        metricscmd->UiFerPktThreshold = ferlist[2];
                    }
                    if (ret & 0x08) {
                        j++;
                        ret = process_subcmd(per_states, NELEMENTS(per_states),
                                             argc, argv, &j, ferstate);
                        if (ret & 0x01) {
                            metricscmd->StableFERPeriod_ms = ferstate[0];
                        }
                        if (ret & 0x02) {
                            metricscmd->DegradingFERPeriod_ms = ferstate[1];
                        }
                        if (ret & 0x04) {
                            metricscmd->UnacceptableFERPeriod_ms = ferstate[2];
                        }
                        if (j < argc) {
                            ret =
                                process_subcmd(valid_fercmds,
                                               NELEMENTS(valid_fercmds), argc,
                                               argv, &j, ferlist);
                            if (ret > 0) {
                                if (ret & 0x01) {
                                    metricscmd->InactivityPeriodThreshold_ms =
                                        ferlist[0];
                                }
                                if (ret & 0x02) {
                                    metricscmd->UcFerThresholdValue =
                                        ferlist[1];
                                }
                                if (ret & 0x04) {
                                    metricscmd->UiFerPktThreshold = ferlist[2];
                                }
                            }
                        }
                    }
                }
                metricscmd->Metrics |= 0x08;
                break;
            }
            metricscmd->action = HostCmd_ACT_GEN_SET;
        }
    }

    /* 
     * ENDIANNESS 
     */
    hostcmd->command = cpu_to_le16(hostcmd->command);
    hostcmd->size = cpu_to_le16(cmd_len);

    metricscmd->action = cpu_to_le16(metricscmd->action);
    metricscmd->Metrics = cpu_to_le16(metricscmd->Metrics);
    metricscmd->UiPerPktThreshold = cpu_to_le32(metricscmd->UiPerPktThreshold);
    metricscmd->StablePERPeriod_ms =
        cpu_to_le32(metricscmd->StablePERPeriod_ms);
    metricscmd->DegradingPERPeriod_ms =
        cpu_to_le32(metricscmd->DegradingPERPeriod_ms);
    metricscmd->UnacceptablePERPeriod_ms =
        cpu_to_le32(metricscmd->UnacceptablePERPeriod_ms);
    metricscmd->InactivityPeriodThreshold_ms =
        cpu_to_le32(metricscmd->InactivityPeriodThreshold_ms);
    metricscmd->UiRxPktThreshold = cpu_to_le32(metricscmd->UiRxPktThreshold);

    /* 
     * Prepare wireless request struct and fire ioctl
     */
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.flags = 0;
    iwr.u.data.pointer = (caddr_t) buf;
    iwr.u.data.length = cmd_len;

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("mlanconfig: roam metrics ioctl");
        free(buf);
        return -EFAULT;
    }

    if (!le16_to_cpu(hostcmd->result)) {
        printf("HOSTCMD_RESP: ReturnCode=%#04x, Result=%#04x\n",
               le16_to_cpu(hostcmd->command), le16_to_cpu(hostcmd->result));
    } else {
        printf("HOSTCMD failed: ReturnCode=%#04x, Result=%#04x\n",
               le16_to_cpu(hostcmd->command), le16_to_cpu(hostcmd->result));
    }

    free(buf);
    return MLAN_STATUS_SUCCESS;
}

/**
 * @brief       Process maintenance of Neighbor list
 *
 * @param       argc    # arguments
 * @param       argv    A pointer to arguments array    
 *
 */
static int
roamSetNborMaintenance(int argc, char *argv[])
{
    int ioctl_val, subioctl_val, cmd_len;
    struct iwreq iwr;
    t_u8 *buf, *pos;
    char *valid_cmds[] = { "blacklist", "clear" };
    int i = 0, j = 0, k = 0;
    unsigned int mac[ETH_ALEN];

    HostCmd_DS_GEN *hostcmd;
    HostCmd_DS_CMD_ROAMAGENT_NEIGHBORLIST_MAINTENANCE *pNborMaintainance;

    if (argv[0] == NULL) {
        printf("\nUnknown setra command... \n");
        return MLAN_STATUS_FAILURE;
    }

    if (argc != 2) {
        fprintf(stderr, "Incorrect number of arguments.\n");
        return MLAN_STATUS_FAILURE;
    }
    for (i = 0; i < NELEMENTS(valid_cmds); i++)
        if (!strncmp(valid_cmds[i], argv[j], strlen(valid_cmds[i])))
            break;
    if (i >= NELEMENTS(valid_cmds)) {
        fprintf(stderr, "Invalid argument \"%s\"\n", argv[0]);
        return MLAN_STATUS_FAILURE;
    }
    switch (i) {
    case 0:/** blacklist */
        j++;
        sscanf(argv[j], "%2x:%2x:%2x:%2x:%2x:%2x",
               mac, mac + 1, mac + 2, mac + 3, mac + 4, mac + 5);
        for (k = 0; k < ETH_ALEN; k++) {
            if (*(mac + k) != 0xFF) {
                break;
            }
        }

        if (k == ETH_ALEN) {
            fprintf(stderr, "Blacklisting a Broadcast address is not allowed");
            return MLAN_STATUS_FAILURE;
        }
        break;

    case 1:/** clear */
        j++;
        if (!strncmp("all", argv[j], strlen("all"))) {
            for (k = 0; k < ETH_ALEN; k++) {
                *(mac + k) = 0xFF;
            }
        } else {
            sscanf(argv[j], "%2x:%2x:%2x:%2x:%2x:%2x",
                   mac, mac + 1, mac + 2, mac + 3, mac + 4, mac + 5);
        }
        break;
    }

    if (get_priv_ioctl("hostcmd",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    cmd_len = sizeof(HostCmd_DS_GEN) +
        sizeof(HostCmd_DS_CMD_ROAMAGENT_NEIGHBORLIST_MAINTENANCE);

    buf = (t_u8 *) calloc(sizeof(t_u8), cmd_len);
    if (!buf) {
        printf("Memory allocation failed\n");
        return -ENOMEM;
    }

    hostcmd = (HostCmd_DS_GEN *) buf;
    hostcmd->command = HostCmd_CMD_ROAMAGENT_NEIGHBORLIST_MAINTENANCE;
    hostcmd->size = 0;
    hostcmd->seq_num = 0;
    hostcmd->result = 0;

    pos = buf + sizeof(HostCmd_DS_GEN);

    pNborMaintainance
        = (HostCmd_DS_CMD_ROAMAGENT_NEIGHBORLIST_MAINTENANCE *) pos;

    pNborMaintainance->action = ((i == 0) ? HostCmd_ACT_ADD_TO_BLACKLIST :
                                 HostCmd_ACT_REMOVE_FROM_BLACKLIST);
    for (k = 0; k < ETH_ALEN; k++) {
        pNborMaintainance->BSSID[k] = *(mac + k);
    }

    /* 
     * ENDIANNESS 
     */
    hostcmd->command = cpu_to_le16(hostcmd->command);
    hostcmd->size = cpu_to_le16(cmd_len);
    pNborMaintainance->action = cpu_to_le16(pNborMaintainance->action);

    /* 
     * Prepare wireless request struct and fire ioctl
     */
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.flags = 0;
    iwr.u.data.pointer = (caddr_t) buf;
    iwr.u.data.length = cmd_len;

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("mlanconfig: neighborlist maintain");
        free(buf);
        return -EFAULT;
    }

    if (!le16_to_cpu(hostcmd->result)) {
        printf("HOSTCMD_RESP: ReturnCode=%#04x, Result=%#04x\n",
               le16_to_cpu(hostcmd->command), le16_to_cpu(hostcmd->result));
    } else {
        printf("HOSTCMD failed: ReturnCode=%#04x, Result=%#04x\n",
               le16_to_cpu(hostcmd->command), le16_to_cpu(hostcmd->result));
    }

    free(buf);
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief          Issue setra backoff command 
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
roamSetBackOff(int argc, char *argv[])
{
    int ioctl_val, subioctl_val, cmd_len;
    struct iwreq iwr;
    t_u8 *buf, *pos, i = 0;
    int spflag[2] = { 0, 0 };   /* scan,priod */
    t_u16 minscan;
    t_u32 bperiod;
    HostCmd_DS_GEN *hostcmd;
    HostCmd_DS_CMD_ROAMAGENT_BACKOFF *roambackoffcmd;

    if (argv[0] == NULL) {
        printf("\nUnknown setra command... \n");
        return MLAN_STATUS_FAILURE;
    }
    /* BackOff */
    if ((argc != 2) && (argc != 4)) {
        fprintf(stderr, "Incorrect number of arguments.\n");
        return MLAN_STATUS_FAILURE;
    }

    if (get_priv_ioctl("hostcmd",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    for (i = 0; i < 4; i++) {
        if (argv[i + 0] && strncmp(argv[i + 0], "scans", strlen("scans")) == 0) {
            minscan = atoi(argv[i + 1]);
            i++;
            spflag[0] = 1;
            /* TODO validation check on minscan */
        }
        if (argv[i + 0] &&
            strncmp(argv[i + 0], "period", strlen("period")) == 0) {
            bperiod = atoi(argv[i + 1]);
            i++;
            spflag[1] = 1;
            /* TODO validation check on bperiod */
        }
    }

    cmd_len = sizeof(HostCmd_DS_GEN) + sizeof(HostCmd_DS_CMD_ROAMAGENT_BACKOFF);

    buf = (t_u8 *) calloc(sizeof(t_u8), cmd_len);
    if (!buf) {
        printf("Memory allocation failed\n");
        return -ENOMEM;
    }

    hostcmd = (HostCmd_DS_GEN *) buf;
    hostcmd->command = HostCmd_CMD_ROAMAGENT_BACKOFF;
    hostcmd->size = 0;
    hostcmd->seq_num = 0;
    hostcmd->result = 0;

    pos = buf + sizeof(HostCmd_DS_GEN);

    roambackoffcmd = (HostCmd_DS_CMD_ROAMAGENT_BACKOFF *) pos;

    switch (spflag[0] + spflag[1]) {
    case 0:
        /* error */
        fprintf(stderr, "Incorrect arguments for setra backoff command.\n");
        free(buf);
        return MLAN_STATUS_FAILURE;
        break;

    case 1:
        /* get missing parameter */
        if (getBackOff(roambackoffcmd) != MLAN_STATUS_SUCCESS) {
            free(buf);
            return MLAN_STATUS_FAILURE;
        }
        if (spflag[0] == 0) {
            minscan = roambackoffcmd->Scans;
        } else {
            bperiod = roambackoffcmd->Period;
        }
        break;

    case 2:
    default:
        break;
    }
    roambackoffcmd->action = HostCmd_ACT_GEN_SET;
    roambackoffcmd->Scans = minscan;
    roambackoffcmd->Period = bperiod;

    /* 
     * ENDIANNESS 
     */
    hostcmd->command = cpu_to_le16(hostcmd->command);
    hostcmd->size = cpu_to_le16(cmd_len);

    roambackoffcmd->action = cpu_to_le16(roambackoffcmd->action);
    roambackoffcmd->Scans = cpu_to_le16(roambackoffcmd->Scans);
    roambackoffcmd->Period = cpu_to_le32(roambackoffcmd->Period);

    /* 
     * Prepare wireless request struct and fire ioctl
     */
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.flags = 0;
    iwr.u.data.pointer = (caddr_t) buf;
    iwr.u.data.length = cmd_len;

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("mlanconfig: roambackoff ioctl");
        free(buf);
        return -EFAULT;
    }

    if (!le16_to_cpu(hostcmd->result)) {
        printf("HOSTCMD_RESP: ReturnCode=%#04x, Result=%#04x\n",
               le16_to_cpu(hostcmd->command), le16_to_cpu(hostcmd->result));
    } else {
        printf("HOSTCMD failed: ReturnCode=%#04x, Result=%#04x\n",
               le16_to_cpu(hostcmd->command), le16_to_cpu(hostcmd->result));
    }

    free(buf);
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief          Issue setra enable/disable/resume/suspend command 
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *  @param set/reset set/reset flag    
 *  @param value    roam control/ host event bitmap value
 *
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
roamSetControl(int argc, char *argv[], int setreset, t_u8 value)
{
    int ioctl_val, subioctl_val, cmd_len;
    struct iwreq iwr;
    t_u8 *buf, *pos;
    HostCmd_DS_GEN *hostcmd;
    HostCmd_DS_CMD_ROAMAGENT_CONTROL *roamcontrolcmd;

    if (get_priv_ioctl("hostcmd",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    cmd_len = sizeof(HostCmd_DS_GEN) + sizeof(HostCmd_DS_CMD_ROAMAGENT_CONTROL);

    buf = (t_u8 *) calloc(sizeof(t_u8), cmd_len);
    if (!buf) {
        printf("Memory allocation failed\n");
        return -ENOMEM;
    }

    hostcmd = (HostCmd_DS_GEN *) buf;
    hostcmd->command = HostCmd_CMD_ROAMAGENT_CONTROL;
    hostcmd->size = 0;
    hostcmd->seq_num = 0;
    hostcmd->result = 0;

    pos = buf + sizeof(HostCmd_DS_GEN);

    roamcontrolcmd = (HostCmd_DS_CMD_ROAMAGENT_CONTROL *) pos;

    /* get current value */
    if (getControl(roamcontrolcmd) != MLAN_STATUS_SUCCESS) {
        free(buf);
        return MLAN_STATUS_FAILURE;
    }

    roamcontrolcmd->action = HostCmd_ACT_GEN_SET;
    switch (setreset) {
    case 0:
        roamcontrolcmd->Control &= value;
        break;
    case 1:
        roamcontrolcmd->Control |= value;
        break;
    case 2:
        roamcontrolcmd->HostEvent &= value;
        break;
    case 3:
        roamcontrolcmd->HostEvent |= value;
        break;
    }

    /* 
     * ENDIANNESS 
     */
    hostcmd->command = cpu_to_le16(hostcmd->command);
    hostcmd->size = cpu_to_le16(cmd_len);

    roamcontrolcmd->action = cpu_to_le16(roamcontrolcmd->action);

    /* 
     * Prepare wireless request struct and fire ioctl
     */
    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.flags = 0;
    iwr.u.data.pointer = (caddr_t) buf;
    iwr.u.data.length = cmd_len;

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("mlanconfig: roamcontrol ioctl");
        free(buf);
        return -EFAULT;
    }

    if (!le16_to_cpu(hostcmd->result)) {
        printf("HOSTCMD_RESP: ReturnCode=%#04x, Result=%#04x\n",
               le16_to_cpu(hostcmd->command), le16_to_cpu(hostcmd->result));
    } else {
        printf("HOSTCMD failed: ReturnCode=%#04x, Result=%#04x\n",
               le16_to_cpu(hostcmd->command), le16_to_cpu(hostcmd->result));
    }

    free(buf);
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief          Issue setra enable command 
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
roamSetEnableControl(int argc, char *argv[])
{
    return roamSetControl(argc, argv, 1, ROAM_CONTROL_ENABLE);
}

/** 
 *  @brief          Issue setra disable command 
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
roamSetDisableControl(int argc, char *argv[])
{
    return roamSetControl(argc, argv, 0, ROAM_CONTROL_DISABLE);
}

/** 
 *  @brief          Issue setra suspend command 
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
roamSetSuspendControl(int argc, char *argv[])
{
    return roamSetControl(argc, argv, 1, ROAM_CONTROL_SUSPEND);
}

/** 
 *  @brief          Issue setra resume command 
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
roamSetResumeControl(int argc, char *argv[])
{
    return roamSetControl(argc, argv, 0, ROAM_CONTROL_RESUME);
}

/**
 *  @brief          Issue setra crossband command
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to argument array
 *  
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
roamSetCrossband(int argc, char *argv[])
{
    if (argc != 1) {
        printf("Invalid Usage \n");
        return (MLAN_STATUS_FAILURE);
    }
    if (strncmp(argv[0], "enable", strlen("enable")) == 0) {
        return roamSetControl(argc, argv, 1, CROSSBAND_ENABLE);
    } else if (strncmp(argv[0], "disable", strlen("disable")) == 0) {
        return roamSetControl(argc, argv, 0, CROSSBAND_DISABLE);
    }

    printf("Invalid Usage \n");
    return (MLAN_STATUS_FAILURE);
}

/**
 *  @brief          Issue setra event command
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to argument array
 *  
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
static int
roamSetEventBitmap(int argc, char *argv[])
{
    int i, enableFlag, found = 0;
    t_u8 value;
    if (argc > 4) {
        printf("Invalid Usage \n");
        return (MLAN_STATUS_FAILURE);
    }
    if (strncmp(argv[0], "enable", strlen("enable")) == 0) {
        enableFlag = 3;
        value = HOST_EVENT_NBOR_DISABLE & HOST_EVENT_ROAM_DISABLE
            & HOST_EVENT_STATE_DISABLE;
    } else if (strncmp(argv[0], "disable", strlen("disable")) == 0) {
        enableFlag = 2;
        value = HOST_EVENT_NBOR_ENABLE | HOST_EVENT_ROAM_ENABLE
            | HOST_EVENT_STATE_ENABLE;
    } else {
        printf("Invalid parameter %s \n", argv[0]);
        return (MLAN_STATUS_FAILURE);
    }

    for (i = 1; i < argc; i++) {
        found = 0;
        if (strncmp(argv[i], "neighbor", strlen("neighbor")) == 0) {
            found = 1;
            if (enableFlag == 3)
                value |= HOST_EVENT_NBOR_ENABLE;
            else
                value &= HOST_EVENT_NBOR_DISABLE;
        }
        if (strncmp(argv[i], "roam", strlen("roam")) == 0) {
            found = 1;
            if (enableFlag == 3)
                value |= HOST_EVENT_ROAM_ENABLE;
            else
                value &= HOST_EVENT_ROAM_DISABLE;
        }
        if (strncmp(argv[i], "state", strlen("state")) == 0) {
            found = 1;
            if (enableFlag == 3)
                value |= HOST_EVENT_STATE_ENABLE;
            else
                value &= HOST_EVENT_STATE_DISABLE;
        }
        if (found == 0) {
            printf("Invalid parameter %s \n", argv[i]);
            return (MLAN_STATUS_FAILURE);
        }
    }

    return roamSetControl(argc, argv, enableFlag, value);
}

/** 
 *  @brief Set the Roaming agent configuration parameters to FW.
 *  
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array    
 *
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
int
process_set_ra_config(int argc, char *argv[])
{
    sub_cmd_exec_t subCmds[] = { {"rssi", 2, roamSetRssiStatsThreshold},
    {"prebcnmiss", 1, roamSetPreBeaconLoss},
    {"failcnt", 1, roamSetFailureCount},
    {"neighbor", 1, roamSetNborAssessment},
    {"maintain", 1, roamSetNborMaintenance},
    {"scanperiod", 2, roamSetScanPeriod},
    {"backoff", 1, roamSetBackOff},
    {"enable", 2, roamSetEnableControl},
    {"disable", 1, roamSetDisableControl},
    {"suspend", 2, roamSetSuspendControl},
    {"resume", 2, roamSetResumeControl},
    {"metrics", 2, roamSetMetrics},
    {"crossband", 1, roamSetCrossband},
    {"event", 2, roamSetEventBitmap}
    };

    return (process_sub_cmd(subCmds, NELEMENTS(subCmds), argc, argv));
}
