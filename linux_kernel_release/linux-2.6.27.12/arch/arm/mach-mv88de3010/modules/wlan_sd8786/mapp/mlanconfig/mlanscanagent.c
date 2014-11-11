/** @file  mlanscanagent.c
  *
  * @brief This files contains mlanconfig scanagent command handling.
  *
  *  Usage: mlanconfig mlanX scanagent [...] 
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
#include    "mlanscanagent.h"

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
 *  @brief scanagent configure scan table
 *
 *  @param age_limit    age limit
 *  @param hold_limit   hold limit
 *
 *  @return             MLAN_STATUS_SUCCESS on success, otherwise error code
 */
static int
scanagent_cfg_scan_table_limits(t_u32 age_limit, t_u32 hold_limit)
{
    int ioctl_val, subioctl_val, cmd_len;
    struct iwreq iwr;
    t_u8 *buf, *pos;
    HostCmd_DS_GEN *hostcmd;
    HostCmd_DS_SCANAGENT_SCAN_TABLE_LIMITS *scan_table_limits;

    if (get_priv_ioctl("hostcmd",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    cmd_len = sizeof(HostCmd_DS_GEN) +
        sizeof(HostCmd_DS_SCANAGENT_SCAN_TABLE_LIMITS);
    buf = (t_u8 *) calloc(sizeof(t_u8), cmd_len);
    if (!buf) {
        printf("Memory allocation failed\n");
        return -ENOMEM;
    }

    hostcmd = (HostCmd_DS_GEN *) buf;
    hostcmd->command = cpu_to_le16(HostCmd_CMD_SCANAGENT_SCAN_TABLE_LIMITS);
    hostcmd->size = cpu_to_le16(cmd_len);
    hostcmd->seq_num = 0;
    hostcmd->result = 0;
    pos = buf + sizeof(HostCmd_DS_GEN);

    scan_table_limits = (HostCmd_DS_SCANAGENT_SCAN_TABLE_LIMITS *) pos;
    scan_table_limits->table_age_limit = cpu_to_le16(age_limit);
    scan_table_limits->table_hold_limit = cpu_to_le16(hold_limit);

    /* 0 set values are ignored by firmware */
    scan_table_limits->action = cpu_to_le16(HostCmd_ACT_GEN_SET);

    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.flags = 0;
    iwr.u.data.pointer = (caddr_t) buf;
    iwr.u.data.length = cmd_len;

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("mlanconfig: scanAgentIoctl ioctl");
        free(buf);
        return -EFAULT;
    }

    printf("\nAge limit  = %7d seconds\n",
           le16_to_cpu(scan_table_limits->table_age_limit));
    printf("Hold limit = %7d seconds\n\n",
           le16_to_cpu(scan_table_limits->table_hold_limit));
    free(buf);
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief Set scanagent age limit
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array
 *
 *  @return         MLAN_STATUS_SUCCESS on success, otherwise error code
 */
static int
scanagent_age_limit(int argc, char *argv[])
{
    t_u32 age_limit = 0;

    if (argc) {
        age_limit = atoi(argv[0]);
    }

    return (scanagent_cfg_scan_table_limits(age_limit, 0));
}

/**
 *  @brief Set scanagent hold limit
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array
 *
 *  @return         MLAN_STATUS_SUCCESS on success, otherwise error code
 */
static int
scanagent_hold_limit(int argc, char *argv[])
{
    t_u32 hold_limit = 0;

    if (argc) {
        hold_limit = atoi(argv[0]);
    }

    return (scanagent_cfg_scan_table_limits(0, hold_limit));
}

/**
 *  @brief Set scanagent scan timing
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array
 *
 *  @return         MLAN_STATUS_SUCCESS on success, otherwise error code
 */
static int
scanagent_timing(int argc, char *argv[])
{
    int ioctl_val, subioctl_val;
    struct iwreq iwr;
    HostCmd_DS_GEN *hostcmd;
    t_u8 *buf, *pos;
    HostCmd_DS_SCANAGENT_CONFIG_TIMING *cfg_timing_cmd;
    MrvlIEtypes_ConfigScanTiming_t *cfg_timing_tlv;
    int idx, cmd_len, rsp_len;
    t_u8 sel;
    t_u16 tlv_len;
    timing_sel_t sel_str[] = { {"disconnected", 1},
    {"adhoc", 1},
    {"fullpower", 1},
    {"ieeeps", 1},
    {"periodic", 1}
    };

    if (get_priv_ioctl("hostcmd",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    cmd_len = (sizeof(HostCmd_DS_GEN) + sizeof(t_u16)
               + sizeof(MrvlIEtypes_ConfigScanTiming_t) * 5);

    buf = (t_u8 *) calloc(sizeof(t_u8), cmd_len);

    if (!buf) {
        printf("Memory allocation failed\n");
        return -ENOMEM;
    }

    hostcmd = (HostCmd_DS_GEN *) buf;
    hostcmd->command = cpu_to_le16(HostCmd_CMD_SCANAGENT_SCAN_TIMING);
    hostcmd->size = (sizeof(HostCmd_DS_GEN) + sizeof(t_u16));
    hostcmd->seq_num = 0;
    hostcmd->result = 0;
    pos = buf + sizeof(HostCmd_DS_GEN);

    cfg_timing_cmd = (HostCmd_DS_SCANAGENT_CONFIG_TIMING *) pos;
    if (argc == 4) {
        cfg_timing_cmd->action = cpu_to_le16(HostCmd_ACT_GEN_SET);
    } else {
        cfg_timing_cmd->action = cpu_to_le16(HostCmd_ACT_GEN_GET);
    }

    cfg_timing_tlv
        = (MrvlIEtypes_ConfigScanTiming_t *) cfg_timing_cmd->tlv_buffer;

    if (argc == 4) {
        cfg_timing_tlv->header.type = cpu_to_le16(TLV_TYPE_SCAN_TIMING);
        cfg_timing_tlv->header.len = (sizeof(MrvlIEtypes_ConfigScanTiming_t)
                                      - sizeof(cfg_timing_tlv->header));

        for (idx = 0; idx < NELEMENTS(sel_str); idx++) {
            if (strncmp(argv[0], sel_str[idx].str, sel_str[idx].match_len) == 0) {
                sel = idx + 1;
                break;
            }
        }

        if (idx == NELEMENTS(sel_str)) {
            printf("Wrong argument for mode selected \"%s\"\n", argv[0]);
            free(buf);
            return -EINVAL;
        }

        /* 
         *  HostCmd_DS_ScanagentTimingMode_e;
         *     TIMING_MODE_INVALID        = 0,
         *     TIMING_MODE_DISCONNECTED   = 1,
         *     TIMING_MODE_ADHOC          = 2,
         *     TIMING_MODE_FULL_POWER     = 3,
         *     TIMING_MODE_IEEE_PS        = 4,
         *     TIMING_MODE_PERIODIC_PS    = 5,
         */
        cfg_timing_tlv->mode = cpu_to_le32(sel);
        cfg_timing_tlv->dwell = cpu_to_le32(atoi(argv[1]));
        cfg_timing_tlv->max_off = cpu_to_le32(atoi(argv[2]));
        cfg_timing_tlv->min_link = cpu_to_le32(atoi(argv[3]));

        hostcmd->size += sizeof(MrvlIEtypes_ConfigScanTiming_t);
    }

    hostcmd->size = cpu_to_le16(hostcmd->size);

    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.flags = 0;
    iwr.u.data.pointer = (caddr_t) buf;
    iwr.u.data.length = cmd_len;

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("mlanconfig: scanAgentIoctl ioctl");
        free(buf);
        return -EFAULT;
    }

    printf("---------------------------------------");
    printf("------------------------\n");
    printf("%48s\n", "ScanAgent Timing Profiles (in ms)");
    printf("---------------------------------------");
    printf("------------------------\n");
    printf("     Profile    |     Dwell    |  MaxOffChan  |"
           "  MinLinkReturn\n");
    printf("---------------------------------------");
    printf("------------------------\n");

    /* Changed to TLV parsing */
    rsp_len = le16_to_cpu(hostcmd->size);
    rsp_len -= (sizeof(HostCmd_DS_GEN) + sizeof(t_u16));
    pos = buf + sizeof(HostCmd_DS_GEN) + sizeof(t_u16);

    while (rsp_len > sizeof(MrvlIEtypesHeader_t)) {
        switch (le16_to_cpu(*(t_u16 *) (pos))) {
        case TLV_TYPE_SCAN_TIMING:
            cfg_timing_tlv = (MrvlIEtypes_ConfigScanTiming_t *) pos;
            tlv_len = le16_to_cpu(cfg_timing_tlv->header.len);
            printf("%15s | %12d | %12d | %12d\n",
                   sel_str[le32_to_cpu(cfg_timing_tlv->mode) - 1].str,
                   (int) le32_to_cpu(cfg_timing_tlv->dwell),
                   (int) le32_to_cpu(cfg_timing_tlv->max_off),
                   (int) le32_to_cpu(cfg_timing_tlv->min_link));
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
 *  @brief Set scanagent profile scan period
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array
 *
 *  @return         MLAN_STATUS_SUCCESS on success, otherwise error code
 */
static int
scanagent_profile_period(int argc, char *argv[])
{
    int ioctl_val, subioctl_val, cmd_len;
    struct iwreq iwr;
    t_u8 *buf, *pos;
    HostCmd_DS_GEN *hostcmd;
    HostCmd_DS_SCANAGENT_CONFIG_PROFILE_SCAN *cfg_profile_scan;

    if (get_priv_ioctl("hostcmd",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    cmd_len = sizeof(HostCmd_DS_GEN) +
        sizeof(HostCmd_DS_SCANAGENT_CONFIG_PROFILE_SCAN);

    buf = (t_u8 *) calloc(sizeof(t_u8), cmd_len);
    if (!buf) {
        printf("Memory allocation failed\n");
        return -ENOMEM;
    }

    hostcmd = (HostCmd_DS_GEN *) buf;
    hostcmd->command = cpu_to_le16(HostCmd_CMD_SCANAGENT_CONFIG_PROFILE_SCAN);
    hostcmd->size = cpu_to_le16(cmd_len);
    hostcmd->seq_num = 0;
    hostcmd->result = 0;
    pos = buf + sizeof(HostCmd_DS_GEN);

    cfg_profile_scan = (HostCmd_DS_SCANAGENT_CONFIG_PROFILE_SCAN *) pos;
    if (argc == 1) {
        cfg_profile_scan->action = cpu_to_le16(HostCmd_ACT_GEN_SET);
        cfg_profile_scan->scan_interval = cpu_to_le16(atoi(argv[0]));
    } else {
        cfg_profile_scan->action = cpu_to_le16(HostCmd_ACT_GEN_GET);
    }

    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.flags = 0;
    iwr.u.data.pointer = (caddr_t) buf;
    iwr.u.data.length = cmd_len;

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("mlanconfig: scanAgentIoctl ioctl");
        free(buf);
        return -EFAULT;
    }

    cfg_profile_scan->scan_interval =
        le16_to_cpu(cfg_profile_scan->scan_interval);
    if ((int) cfg_profile_scan->scan_interval == 0) {
        printf("\nProfile Scan interval: <disabled>\n\n");
    } else {
        printf("\nProfile Scan interval: %d seconds\n\n",
               (int) cfg_profile_scan->scan_interval);
    }

    free(buf);
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief scanagent parse entry selection
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array
 *  @param psel     A pointer to scanagent entry selection
 *
 *  @return         None
 */
static void
scanAgentParseEntrySel(int argc, char *argv[],
                       HostCmd_DS_SCANAGENT_TABLE_MAINTENANCE * psel,
                       int *cmd_len)
{
    int arg_idx, tmp_idx;
    t_u8 *tlv_pos;
    MrvlIEtypes_SsIdParamSet_t *ssid;
    MrvlIEtypes_Bssid_List_t *bssid;
    unsigned int mac[ETH_ALEN];

    tlv_pos = (t_u8 *) psel->tlv_buffer;

    for (arg_idx = 0; arg_idx < argc; arg_idx++) {
        if (strncmp(argv[arg_idx], "ssid=", strlen("ssid=")) == 0) {
            ssid = (MrvlIEtypes_SsIdParamSet_t *) tlv_pos;
            ssid->header.type = cpu_to_le16(TLV_TYPE_SSID);
            ssid->header.len = strlen(argv[arg_idx]) - strlen("ssid=");
            strncpy((char *) ssid->ssid, (argv[arg_idx] + strlen("ssid=")),
                    ssid->header.len);
            tlv_pos += ssid->header.len + sizeof(MrvlIEtypesHeader_t);
            ssid->header.len = cpu_to_le16(ssid->header.len);

        } else if (strncmp(argv[arg_idx], "bssid=", strlen("bssid=")) == 0) {
            bssid = (MrvlIEtypes_Bssid_List_t *) tlv_pos;
            bssid->header.type = cpu_to_le16(TLV_TYPE_BSSID);
            bssid->header.len = ETH_ALEN;
            /* 
             *  "bssid" token string handler
             */
            sscanf(argv[arg_idx] + strlen("bssid="), "%2x:%2x:%2x:%2x:%2x:%2x",
                   mac + 0, mac + 1, mac + 2, mac + 3, mac + 4, mac + 5);
            for (tmp_idx = 0; tmp_idx < NELEMENTS(mac); tmp_idx++) {
                bssid->bssid[tmp_idx] = (t_u8) mac[tmp_idx];
            }
            tlv_pos += bssid->header.len + sizeof(MrvlIEtypesHeader_t);
            bssid->header.len = cpu_to_le16(bssid->header.len);

        } else if (strncmp(argv[arg_idx], "age=", strlen("age=")) == 0) {
            psel->age = cpu_to_le32(atoi(argv[arg_idx] + strlen("age=")));

        } else if (strncmp(argv[arg_idx], "id=", strlen("id=")) == 0) {
            psel->scan_request_id =
                cpu_to_le32(atoi(argv[arg_idx] + strlen("id=")));
        }
    }

    *cmd_len += (tlv_pos - psel->tlv_buffer);
}

/**
 *  @brief scanagent execute scan
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array
 *
 *  @return         MLAN_STATUS_SUCCESS on success, otherwise error code
 */
static int
scanagent_exec_scan(int argc, char *argv[])
{
    int ioctl_val, subioctl_val;
    struct iwreq iwr;
    int arg_idx, cmd_len, tmp_idx;
    t_u8 *buf, *pos, *tlv_pos;
    HostCmd_DS_GEN *hostcmd;
    HostCmd_DS_SCANAGENT_SCAN_EXEC *scan_exec;
    MrvlIEtypes_SsIdParamSet_t *ssid;
    MrvlIEtypes_Bssid_List_t *bssid;
    MrvlIEtypes_ConfigScanTiming_t *cfg_timing_tlv;
    unsigned int mac[ETH_ALEN];

    if (get_priv_ioctl("hostcmd",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    /* -1 is for not to include t_u8 tlvBuffer[1] in cmd_len */
    cmd_len =
        sizeof(HostCmd_DS_GEN) + sizeof(HostCmd_DS_SCANAGENT_SCAN_EXEC) - 1;

    /* Not sure of the length so, allocate Max command size */
    buf = (t_u8 *) calloc(sizeof(t_u8), MRVDRV_SIZE_OF_CMD_BUFFER);
    if (!buf) {
        printf("Memory allocation failed\n");
        return -ENOMEM;
    }

    hostcmd = (HostCmd_DS_GEN *) buf;
    hostcmd->command = cpu_to_le16(HostCmd_CMD_SCANAGENT_SCAN_EXEC);
    hostcmd->size = 0;
    hostcmd->seq_num = 0;
    hostcmd->result = 0;

    pos = buf + sizeof(HostCmd_DS_GEN);

    scan_exec = (HostCmd_DS_SCANAGENT_SCAN_EXEC *) pos;
    tlv_pos = scan_exec->tlv_buffer;

    for (arg_idx = 0; arg_idx < argc; arg_idx++) {
        if (strncmp(argv[arg_idx], "ssid=", strlen("ssid=")) == 0) {
            /* 
             *  "ssid" token string handler
             */
            ssid = (MrvlIEtypes_SsIdParamSet_t *) tlv_pos;
            ssid->header.type = cpu_to_le16(TLV_TYPE_SSID);
            ssid->header.len = strlen(argv[arg_idx]) - strlen("ssid=");
            strncpy((char *) ssid->ssid, (argv[arg_idx] + strlen("ssid=")),
                    ssid->header.len);
            tlv_pos += ssid->header.len + sizeof(MrvlIEtypesHeader_t);
            ssid->header.len = cpu_to_le16(ssid->header.len);

        } else if (strncmp(argv[arg_idx], "bssid=", strlen("bssid=")) == 0) {
            bssid = (MrvlIEtypes_Bssid_List_t *) tlv_pos;
            bssid->header.type = cpu_to_le16(TLV_TYPE_BSSID);
            bssid->header.len = ETH_ALEN;
            /* 
             *  "bssid" token string handler
             */
            sscanf(argv[arg_idx] + strlen("bssid="), "%2x:%2x:%2x:%2x:%2x:%2x",
                   mac + 0, mac + 1, mac + 2, mac + 3, mac + 4, mac + 5);
            for (tmp_idx = 0; tmp_idx < NELEMENTS(mac); tmp_idx++) {
                bssid->bssid[tmp_idx] = (t_u8) mac[tmp_idx];
            }
            tlv_pos += bssid->header.len + sizeof(MrvlIEtypesHeader_t);
            bssid->header.len = cpu_to_le16(bssid->header.len);

        } else if (strncmp(argv[arg_idx], "table=", strlen("table=")) == 0) {
            reg_class_table_select(argv[arg_idx] + strlen("table="),
                                   (reg_chan_table_e *) & scan_exec->table);
            scan_exec->table = cpu_to_le16(scan_exec->table);

        } else if (strncmp(argv[arg_idx], "type=", strlen("type=")) == 0) {
            if (strcmp(argv[arg_idx] + strlen("type="), "prof") == 0) {
                scan_exec->scan_type = CONFIG_PROFILE;
            } else if (strcmp(argv[arg_idx] + strlen("type="), "neigh") == 0) {
                scan_exec->scan_type = CONFIG_NEIGHBOR;
            } else if (strcmp(argv[arg_idx] + strlen("type="), "arb") == 0) {
                scan_exec->scan_type = CONFIG_ARBITRARY_CHANNEL;
            } else {
                scan_exec->scan_type = CONFIG_SITE_SURVEY;
            }
            scan_exec->scan_type = cpu_to_le16(scan_exec->scan_type);

        } else if (strncmp(argv[arg_idx], "band=", strlen("band=")) == 0) {
            scan_exec->filter.band_2_4_GHZ_enabled = 1;
            scan_exec->filter.band_5_GHZ_enabled = 1;
            scan_exec->filter.band_4_GHZ_enabled = 1;
            scan_exec->filter.chanwidth_20_MHZ_enabled = 1;
            scan_exec->filter.chanwidth_10_MHZ_enabled = 1;
            /** No endian conversion for this as the filter structure
                is defined  according to endian */

        } else if (strncmp(argv[arg_idx], "delay=", strlen("delay=")) == 0) {
            /* 
             *  "delay" token string handler
             */
            sscanf(argv[arg_idx] + strlen("delay="),
                   "%d", (int *) &scan_exec->delay);
            scan_exec->delay = cpu_to_le32(scan_exec->delay);

        } else if (strncmp(argv[arg_idx], "timing=", strlen("timing=")) == 0) {
            cfg_timing_tlv = (MrvlIEtypes_ConfigScanTiming_t *) tlv_pos;
            cfg_timing_tlv->header.type = cpu_to_le16(TLV_TYPE_SCAN_TIMING);
            cfg_timing_tlv->header.len
                = (sizeof(cfg_timing_tlv) - sizeof(cfg_timing_tlv->header));
            /* 
             *  "timing" token string handler
             */
            sscanf(argv[arg_idx] + strlen("timing="), "%d,%d,%d",
                   (int *) &cfg_timing_tlv->dwell,
                   (int *) &cfg_timing_tlv->max_off,
                   (int *) &cfg_timing_tlv->min_link);

            cfg_timing_tlv->mode = 0;
            tlv_pos += (cfg_timing_tlv->header.len
                        + sizeof(MrvlIEtypesHeader_t));
            cfg_timing_tlv->header.len =
                cpu_to_le16(cfg_timing_tlv->header.len);
        }
    }

    cmd_len += (tlv_pos - scan_exec->tlv_buffer);
    hostcmd->size = cpu_to_le16(cmd_len);

    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.flags = 0;
    iwr.u.data.pointer = (caddr_t) buf;
    iwr.u.data.length = cmd_len;

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("mlanconfig: scanAgentIoctl ioctl");
        free(buf);
        return -EFAULT;
    } else {
        printf("\nScan Scheduled, ID = %d\n\n",
               (int) le32_to_cpu(scan_exec->scan_req_id_out));
    }

    free(buf);
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief Issue a scanagent cmd_type subcommand
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array
 *  @param cmd_type command type
 *
 *  @return         MLAN_STATUS_SUCCESS on success, otherwise error code
 */
static int
scanagent_table_entry_sub_cmd(int argc, char *argv[],
                              HostCmd_DS_ScanagentTableMaintenance_e cmd_type)
{
    int ioctl_val, subioctl_val, cmd_len;
    struct iwreq iwr;
    t_u8 *buf, *pos;
    HostCmd_DS_GEN *hostcmd;
    HostCmd_DS_SCANAGENT_TABLE_MAINTENANCE *table_maintenance;

    if (get_priv_ioctl("hostcmd",
                       &ioctl_val, &subioctl_val) == MLAN_STATUS_FAILURE) {
        return -EOPNOTSUPP;
    }

    /* -1 is for not to include t_u8 tlvBuffer[1] in cmd_len */
    cmd_len = sizeof(HostCmd_DS_GEN) +
        sizeof(HostCmd_DS_SCANAGENT_TABLE_MAINTENANCE) - 1;

    /* Not sure of the length so, allocate Max command size */
    buf = (t_u8 *) calloc(sizeof(t_u8), MRVDRV_SIZE_OF_CMD_BUFFER);
    if (!buf) {
        printf("Memory allocation failed\n");
        return -ENOMEM;
    }

    hostcmd = (HostCmd_DS_GEN *) buf;
    hostcmd->command = cpu_to_le16(HostCmd_CMD_SCANAGENT_TABLE_MAINTENANCE);
    hostcmd->size = 0;
    hostcmd->seq_num = 0;
    hostcmd->result = 0;
    pos = buf + sizeof(HostCmd_DS_GEN);
    table_maintenance = (HostCmd_DS_SCANAGENT_TABLE_MAINTENANCE *) pos;
    table_maintenance->action = cpu_to_le16((t_u16) cmd_type);

    scanAgentParseEntrySel(argc, argv, table_maintenance, &cmd_len);

    hostcmd->size = cpu_to_le16(cmd_len);
    strncpy(iwr.ifr_name, dev_name, IFNAMSIZ);
    iwr.u.data.flags = 0;
    iwr.u.data.pointer = (caddr_t) buf;
    iwr.u.data.length = cmd_len;

    if (ioctl(sockfd, ioctl_val, &iwr) < 0) {
        perror("mlanconfig: scanAgentIoctl ioctl");
        free(buf);
        return -EFAULT;
    }

    free(buf);
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief Issue a scanagent table lock subcommand
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array
 *
 *  @return         MLAN_STATUS_SUCCESS on success, otherwise error code
 */
static int
scanagent_table_lock(int argc, char *argv[])
{
    return (scanagent_table_entry_sub_cmd(argc, argv, SCAN_TABLE_OP_LOCK));
}

/**
 *  @brief Issue a scanagent table unlock subcommand
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array
 *
 *  @return         MLAN_STATUS_SUCCESS on success, otherwise error code
 */
static int
scanagent_table_unlock(int argc, char *argv[])
{
    return (scanagent_table_entry_sub_cmd(argc, argv, SCAN_TABLE_OP_UNLOCK));
}

/**
 *  @brief Issue a scanagent table purge subcommand
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array
 *
 *  @return         MLAN_STATUS_SUCCESS on success, otherwise error code
 */
static int
scanagent_table_purge(int argc, char *argv[])
{
    return (scanagent_table_entry_sub_cmd(argc, argv, SCAN_TABLE_OP_PURGE));
}

/**
 *  @brief Issue a scanagent command
 *
 *  @param argc     number of arguments
 *  @param argv     A pointer to arguments array
 *
 *  @return         MLAN_STATUS_SUCCESS--success, otherwise--fail
 */
int
process_scanagent(int argc, char *argv[])
{
    sub_cmd_exec_t sub_cmd[] = { {"timing", 1, scanagent_timing},
    {"scan", 1, scanagent_exec_scan},
    {"lock", 1, scanagent_table_lock},
    {"unlock", 1, scanagent_table_unlock},
    {"purge", 2, scanagent_table_purge},
    {"profile", 2, scanagent_profile_period},
    {"holdlimit", 1, scanagent_hold_limit},
    {"agelimit", 1, scanagent_age_limit}
    };

    return (process_sub_cmd(sub_cmd, NELEMENTS(sub_cmd), argc, argv));
}
