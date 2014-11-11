/** @file  mlanhostcmd.h
  *
  * @brief This file contains command structures for mlanconfig application
  * 
  *  Copyright (C) 2008-2009, Marvell International Ltd.
  *  All Rights Reserved
  */
/************************************************************************
Change log:
     11/26/2008: initial version
************************************************************************/
#ifndef _MLANHOSTCMD_H_
#define _MLANHOSTCMD_H_

/** Find number of elements */
#define NELEMENTS(x) (sizeof(x)/sizeof(x[0]))

/** MLAN MAC Address Length */
#define MLAN_MAC_ADDR_LENGTH     (6)
/** Size of command buffer */
#define MRVDRV_SIZE_OF_CMD_BUFFER		(2 * 1024)

/** Command RET code, MSB is set to 1 */
#define HostCmd_RET_BIT					0x8000
/** General purpose action : Get */
#define HostCmd_ACT_GEN_GET				0x0000
/** General purpose action : Set */
#define HostCmd_ACT_GEN_SET				0x0001
/** General purpose action : Remove */
#define HostCmd_ACT_GEN_REMOVE          0x0004

/** Host Command ID : 802.11 BG scan configuration */
#define HostCmd_CMD_802_11_BG_SCAN_CONFIG     0x006b
/** Host Command ID : Configuration data */
#define HostCmd_CMD_CFG_DATA                  0x008f
/** Host Command ID : 802.11 crypto */
#define HostCmd_CMD_802_11_CRYPTO             0x0078
/** Host Command ID : 802.11 auto Tx */
#define HostCmd_CMD_802_11_AUTO_TX      	0x0082

/** Host Command ID : 802.11 subscribe event */
#define HostCmd_CMD_802_11_SUBSCRIBE_EVENT       0x0075

/** TLV  type ID definition */
#define PROPRIETARY_TLV_BASE_ID     0x0100
/** TLV type : Beacon RSSI low */
#define TLV_TYPE_RSSI_LOW           (PROPRIETARY_TLV_BASE_ID + 4)
/** TLV type : Beacon SNR low */
#define TLV_TYPE_SNR_LOW            (PROPRIETARY_TLV_BASE_ID + 5)
/** TLV type : Fail count */
#define TLV_TYPE_FAILCOUNT          (PROPRIETARY_TLV_BASE_ID + 6)
/** TLV type : BCN miss */
#define TLV_TYPE_BCNMISS            (PROPRIETARY_TLV_BASE_ID + 7)
/** TLV type : Beacon RSSI high */
#define TLV_TYPE_RSSI_HIGH          (PROPRIETARY_TLV_BASE_ID + 22)
/** TLV type : Beacon SNR high */
#define TLV_TYPE_SNR_HIGH           (PROPRIETARY_TLV_BASE_ID + 23)
/** TLV type : Auto Tx */
#define TLV_TYPE_AUTO_TX            (PROPRIETARY_TLV_BASE_ID + 24)
/** TLV type :Link Quality */
#define TLV_TYPE_LINK_QUALITY       (PROPRIETARY_TLV_BASE_ID + 36)
/** TLV type : Data RSSI low */
#define TLV_TYPE_RSSI_LOW_DATA      (PROPRIETARY_TLV_BASE_ID + 38)
/** TLV type : Data SNR low */
#define TLV_TYPE_SNR_LOW_DATA       (PROPRIETARY_TLV_BASE_ID + 39)
/** TLV type : Data RSSI high */
#define TLV_TYPE_RSSI_HIGH_DATA     (PROPRIETARY_TLV_BASE_ID + 40)
/** TLV type : Data SNR high */
#define TLV_TYPE_SNR_HIGH_DATA      (PROPRIETARY_TLV_BASE_ID + 41)
/** TLV type: Pre-Beacon Lost */
#define TLV_TYPE_PRE_BEACON_LOST    (PROPRIETARY_TLV_BASE_ID + 73)

/* Define general hostcmd data structure */
/** HostCmd_DS_GEN */
typedef struct MAPP_HostCmd_DS_GEN
{
    /** Command */
    t_u16 command;
    /** Size */
    t_u16 size;
    /** Sequence number */
    t_u16 seq_num;
    /** Result */
    t_u16 result;
} __attribute__ ((packed)) HostCmd_DS_GEN;

/** Size of HostCmd_DS_GEN */
#define S_DS_GEN    sizeof(HostCmd_DS_GEN)

/** HostCmd_DS_802_11_CFG_DATA */
typedef struct MAPP_HostCmd_DS_802_11_CFG_DATA
{
    /** Action */
    t_u16 action;
    /** Type */
    t_u16 type;
    /** Data length */
    t_u16 data_len;
    /** Data */
    t_u8 data[1];
} __attribute__ ((packed)) HostCmd_DS_802_11_CFG_DATA;

/** MrvlIEtypesHeader_t */
typedef struct MrvlIEtypesHeader
{
    /** Header type */
    t_u16 type;
    /** Header length */
    t_u16 len;
} __attribute__ ((packed)) MrvlIEtypesHeader_t;

/** MrvlIEtypes_Data_t */
typedef struct MrvlIEtypes_Data_t
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** Data */
    t_u8 data[1];
} __attribute__ ((packed)) MrvlIEtypes_Data_t;

/** HostCmd_DS_802_11_CRYPTO */
typedef struct MAPP_HostCmd_DS_802_11_CRYPTO
{
    t_u16 encdec;         /**< Decrypt=0, Encrypt=1 */
    t_u16 algorithm;      /**< RC4=1 AES=2 , AES_KEY_WRAP=3 */
    t_u16 key_IV_length;    /**< Length of Key IV (bytes)   */
    t_u8 keyIV[32];       /**< Key IV */
    t_u16 key_length;      /**< Length of Key (bytes) */
    t_u8 key[32];         /**< Key */
    MrvlIEtypes_Data_t data;       /**< Plain text if encdec=Encrypt, Ciphertext data if encdec=Decrypt*/
} __attribute__ ((packed)) HostCmd_DS_802_11_CRYPTO;

/** HostCmd_DS_802_11_CRYPTO_AES_CCM */
typedef struct MAPP_HostCmd_DS_802_11_CRYPTO_AES_CCM
{
    t_u16 encdec;         /**< Decrypt=0, Encrypt=1 */
    t_u16 algorithm;      /**< AES_CCM=4 */
    t_u16 key_length;      /**< Length of Key (bytes)  */
    t_u8 key[32];         /**< Key  */
    t_u16 nonce_length;    /**< Length of Nonce (bytes) */
    t_u8 nonce[14];       /**< Nonce */
    t_u16 AAD_length;      /**< Length of AAD (bytes) */
    t_u8 AAD[32];         /**< AAD */
    MrvlIEtypes_Data_t data;       /**< Plain text if encdec=Encrypt, Ciphertext data if encdec=Decrypt*/
} __attribute__ ((packed)) HostCmd_DS_802_11_CRYPTO_AES_CCM;

/** AES CCM cipher test */
#define CIPHER_TEST_AES_CCM (4)
/** AutoTx_MacFrame_t */
typedef struct AutoTx_MacFrame
{
    t_u16 interval;           /**< in seconds */
    t_u8 priority;            /**< User Priority: 0~7, ignored if non-WMM */
    t_u8 reserved;            /**< set to 0 */
    t_u16 frame_len;           /**< Length of MAC frame payload */
    t_u8 dest_mac_addr[MLAN_MAC_ADDR_LENGTH];           /**< Destination MAC address */
    t_u8 src_mac_addr[MLAN_MAC_ADDR_LENGTH];            /**< Source MAC address */
    t_u8 payload[];                       /**< Payload */
} __attribute__ ((packed)) AutoTx_MacFrame_t;

/** MrvlIEtypes_AutoTx_t */
typedef struct MrvlIEtypes_AutoTx
{
    MrvlIEtypesHeader_t header;             /**< Header */
    AutoTx_MacFrame_t auto_tx_mac_frame;      /**< Auto Tx MAC frame */
} __attribute__ ((packed)) MrvlIEtypes_AutoTx_t;

/** HostCmd_DS_802_11_AUTO_TX */
typedef struct MAPP_HostCmd_DS_802_11_AUTO_TX
{
    /** Action */
    t_u16 action;               /* 0 = ACT_GET; 1 = ACT_SET; */
    MrvlIEtypes_AutoTx_t auto_tx;        /**< Auto Tx */
} __attribute__ ((packed)) HostCmd_DS_802_11_AUTO_TX;

/** HostCmd_DS_802_11_SUBSCRIBE_EVENT */
typedef struct MAPP_HostCmd_DS_802_11_SUBSCRIBE_EVENT
{
    /** Action */
    t_u16 action;
    /** Events */
    t_u16 events;
} __attribute__ ((packed)) HostCmd_DS_802_11_SUBSCRIBE_EVENT;

/** MrvlIEtypes_RssiParamSet_t */
typedef struct MrvlIEtypes_RssiThreshold
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** RSSI value */
    t_u8 RSSI_value;
    /** RSSI frequency */
    t_u8 RSSI_freq;
} __attribute__ ((packed)) MrvlIEtypes_RssiThreshold_t;

/** MrvlIEtypes_SnrThreshold_t */
typedef struct MrvlIEtypes_SnrThreshold
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** SNR value */
    t_u8 SNR_value;
    /** SNR frequency */
    t_u8 SNR_freq;
} __attribute__ ((packed)) MrvlIEtypes_SnrThreshold_t;

/** MrvlIEtypes_FailureCount_t */
typedef struct MrvlIEtypes_FailureCount
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** Failure value */
    t_u8 fail_value;
    /** Failure frequency */
    t_u8 fail_freq;
} __attribute__ ((packed)) MrvlIEtypes_FailureCount_t;

/** MrvlIEtypes_BeaconsMissed_t */
typedef struct MrvlIEtypes_BeaconsMissed
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** Number of beacons missed */
    t_u8 beacon_missed;
    /** Reserved */
    t_u8 reserved;
} __attribute__ ((packed)) MrvlIEtypes_BeaconsMissed_t;

/** MrvlIEtypes_LinkQuality_t */
typedef struct MrvlIEtypes_LinkQuality
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** Link SNR threshold */
    t_u16 link_SNR_thrs;
    /** Link SNR frequency */
    t_u16 link_SNR_freq;
    /** Minimum rate value */
    t_u16 min_rate_val;
    /** Minimum rate frequency */
    t_u16 min_rate_freq;
    /** Tx latency value */
    t_u32 tx_latency_val;
    /** Tx latency threshold */
    t_u32 tx_latency_thrs;
} __attribute__ ((packed)) MrvlIEtypes_LinkQuality_t;

/** MrvlIEtypes_PreBeaconLost_t */
typedef struct MrvlIEtypes_PreBeaconLost
{
    /** Header */
    MrvlIEtypesHeader_t header;
    /** Pre-Beacon Lost */
    t_u8 pre_beacon_lost;
    /** Reserved */
    t_u8 reserved;
} __attribute__ ((packed)) MrvlIEtypes_PreBeaconLost_t;

/* String helper functions */
/** Convert char to hex integer */
int hexval(t_s32 chr);
/** Convert char to hex integer */
t_u8 hexc2bin(t_s8 chr);
/** Convert string to hex integer */
t_u32 a2hex(t_s8 * s);
/** Check the Hex String */
int ishexstring(t_s8 * s);
/** Convert String to integer */
t_u32 a2hex_or_atoi(t_s8 * value);
/** Convert String to Integer */
int atoval(t_s8 * buf);
/** Hump hex data */
void hexdump(t_s8 * prompt, void *p, t_s32 len, t_s8 delim);
/** Convert String to Hex */
t_s8 *convert2hex(t_s8 * ptr, t_u8 * chr);

#endif /* _MLANHOSTCMD_H_ */
