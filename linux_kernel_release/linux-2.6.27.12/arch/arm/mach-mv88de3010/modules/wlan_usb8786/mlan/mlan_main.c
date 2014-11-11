/** @file mlan_main.c
 *  
 *  @brief This file contains MLAN main process and APIs to MOAL module.
 * 
 *  Copyright (C) 2008-2009, Marvell International Ltd. 
 *  All Rights Reserved
 */

/**
 *  @mainpage MLAN Driver
 *
 *  @section overview_sec Overview
 *
 *  The MLAN is an OS independent WLAN driver for Marvell 802.11
 *  embedded chipset.
 * 
 *  @section copyright_sec Copyright
 *
 *  Copyright (C) 2008-2009, Marvell International Ltd. 
 */

/********************************************************
Change log:
    10/13/2008: initial version
********************************************************/

#include "mlan.h"
#include "mlan_11d.h"
#include "mlan_join.h"
#include "mlan_scan.h"
#include "mlan_util.h"
#include "mlan_fw.h"
#include "mlan_main.h"
#include "mlan_tx.h"
#include "mlan_wmm.h"

/********************************************************
                Local Variables
********************************************************/

/** 100mW */
#define WLAN_TX_PWR_DEFAULT     20
/** 100mW */
#define WLAN_TX_PWR_US_DEFAULT      20
/** 50mW */
#define WLAN_TX_PWR_JP_DEFAULT      16
/** 100mW */
#define WLAN_TX_PWR_FR_100MW        20
/** 10mW */
#define WLAN_TX_PWR_FR_10MW         10
/** 100mW */
#define WLAN_TX_PWR_EMEA_DEFAULT    20

/* Format { Channel, Frequency (MHz), MaxTxPower } */
/** Band: 'B/G', Region: USA FCC/Canada IC */
static chan_freq_power_t channel_freq_power_US_BG[] = {
    {1, 2412, WLAN_TX_PWR_US_DEFAULT},
    {2, 2417, WLAN_TX_PWR_US_DEFAULT},
    {3, 2422, WLAN_TX_PWR_US_DEFAULT},
    {4, 2427, WLAN_TX_PWR_US_DEFAULT},
    {5, 2432, WLAN_TX_PWR_US_DEFAULT},
    {6, 2437, WLAN_TX_PWR_US_DEFAULT},
    {7, 2442, WLAN_TX_PWR_US_DEFAULT},
    {8, 2447, WLAN_TX_PWR_US_DEFAULT},
    {9, 2452, WLAN_TX_PWR_US_DEFAULT},
    {10, 2457, WLAN_TX_PWR_US_DEFAULT},
    {11, 2462, WLAN_TX_PWR_US_DEFAULT}
};

/** Band: 'B/G', Region: Europe ETSI */
static chan_freq_power_t channel_freq_power_EU_BG[] = {
    {1, 2412, WLAN_TX_PWR_EMEA_DEFAULT},
    {2, 2417, WLAN_TX_PWR_EMEA_DEFAULT},
    {3, 2422, WLAN_TX_PWR_EMEA_DEFAULT},
    {4, 2427, WLAN_TX_PWR_EMEA_DEFAULT},
    {5, 2432, WLAN_TX_PWR_EMEA_DEFAULT},
    {6, 2437, WLAN_TX_PWR_EMEA_DEFAULT},
    {7, 2442, WLAN_TX_PWR_EMEA_DEFAULT},
    {8, 2447, WLAN_TX_PWR_EMEA_DEFAULT},
    {9, 2452, WLAN_TX_PWR_EMEA_DEFAULT},
    {10, 2457, WLAN_TX_PWR_EMEA_DEFAULT},
    {11, 2462, WLAN_TX_PWR_EMEA_DEFAULT},
    {12, 2467, WLAN_TX_PWR_EMEA_DEFAULT},
    {13, 2472, WLAN_TX_PWR_EMEA_DEFAULT}
};

/** Band: 'B/G', Region: France */
static chan_freq_power_t channel_freq_power_FR_BG[] = {
    {1, 2412, WLAN_TX_PWR_FR_100MW},
    {2, 2417, WLAN_TX_PWR_FR_100MW},
    {3, 2422, WLAN_TX_PWR_FR_100MW},
    {4, 2427, WLAN_TX_PWR_FR_100MW},
    {5, 2432, WLAN_TX_PWR_FR_100MW},
    {6, 2437, WLAN_TX_PWR_FR_100MW},
    {7, 2442, WLAN_TX_PWR_FR_100MW},
    {8, 2447, WLAN_TX_PWR_FR_100MW},
    {9, 2452, WLAN_TX_PWR_FR_100MW},
    {10, 2457, WLAN_TX_PWR_FR_10MW},
    {11, 2462, WLAN_TX_PWR_FR_10MW},
    {12, 2467, WLAN_TX_PWR_FR_10MW},
    {13, 2472, WLAN_TX_PWR_FR_10MW}
};

/** Band: 'B/G', Region: Japan */
static chan_freq_power_t channel_freq_power_JPN41_BG[] = {
    {1, 2412, WLAN_TX_PWR_JP_DEFAULT},
    {2, 2417, WLAN_TX_PWR_JP_DEFAULT},
    {3, 2422, WLAN_TX_PWR_JP_DEFAULT},
    {4, 2427, WLAN_TX_PWR_JP_DEFAULT},
    {5, 2432, WLAN_TX_PWR_JP_DEFAULT},
    {6, 2437, WLAN_TX_PWR_JP_DEFAULT},
    {7, 2442, WLAN_TX_PWR_JP_DEFAULT},
    {8, 2447, WLAN_TX_PWR_JP_DEFAULT},
    {9, 2452, WLAN_TX_PWR_JP_DEFAULT},
    {10, 2457, WLAN_TX_PWR_JP_DEFAULT},
    {11, 2462, WLAN_TX_PWR_JP_DEFAULT},
    {12, 2467, WLAN_TX_PWR_JP_DEFAULT},
    {13, 2472, WLAN_TX_PWR_JP_DEFAULT}
};

/** Band: 'B/G', Region: Japan */
static chan_freq_power_t channel_freq_power_JPN40_BG[] = {
    {14, 2484, WLAN_TX_PWR_JP_DEFAULT}
};

/** Band : 'B/G', Region: Special */
static chan_freq_power_t channel_freq_power_SPECIAL_BG[] = {
    {1, 2412, WLAN_TX_PWR_JP_DEFAULT},
    {2, 2417, WLAN_TX_PWR_JP_DEFAULT},
    {3, 2422, WLAN_TX_PWR_JP_DEFAULT},
    {4, 2427, WLAN_TX_PWR_JP_DEFAULT},
    {5, 2432, WLAN_TX_PWR_JP_DEFAULT},
    {6, 2437, WLAN_TX_PWR_JP_DEFAULT},
    {7, 2442, WLAN_TX_PWR_JP_DEFAULT},
    {8, 2447, WLAN_TX_PWR_JP_DEFAULT},
    {9, 2452, WLAN_TX_PWR_JP_DEFAULT},
    {10, 2457, WLAN_TX_PWR_JP_DEFAULT},
    {11, 2462, WLAN_TX_PWR_JP_DEFAULT},
    {12, 2467, WLAN_TX_PWR_JP_DEFAULT},
    {13, 2472, WLAN_TX_PWR_JP_DEFAULT},
    {14, 2484, WLAN_TX_PWR_JP_DEFAULT}
};

/**
 * The structure for channel, frequency and power
 */
typedef struct _region_cfp_table
{
    /** Region */
    t_u8 region;
    /** Frequency/Power */
    chan_freq_power_t *cfp_BG;
    /** No BG flag */
    int cfp_no_BG;
} region_cfp_table_t;

/**
 * The structure for the mapping between region and CFP
 */
static region_cfp_table_t region_cfp_table[] = {
    {0x10,                      /* US FCC */
     channel_freq_power_US_BG,
     sizeof(channel_freq_power_US_BG) / sizeof(chan_freq_power_t),
     }
    ,
    {0x20,                      /* CANADA IC */
     channel_freq_power_US_BG,
     sizeof(channel_freq_power_US_BG) / sizeof(chan_freq_power_t),
     }
    ,
    {0x30,                      /* EU */
     channel_freq_power_EU_BG,
     sizeof(channel_freq_power_EU_BG) / sizeof(chan_freq_power_t),
     }
    ,
    {0x32,                      /* FRANCE */
     channel_freq_power_FR_BG,
     sizeof(channel_freq_power_FR_BG) / sizeof(chan_freq_power_t),
     }
    ,
    {0x40,                      /* JAPAN */
     channel_freq_power_JPN40_BG,
     sizeof(channel_freq_power_JPN40_BG) / sizeof(chan_freq_power_t),
     }
    ,
    {0x41,                      /* JAPAN */
     channel_freq_power_JPN41_BG,
     sizeof(channel_freq_power_JPN41_BG) / sizeof(chan_freq_power_t),
     }
    ,
    {0xff,                      /* Special */
     channel_freq_power_SPECIAL_BG,
     sizeof(channel_freq_power_SPECIAL_BG) / sizeof(chan_freq_power_t),
     }
    ,
/* Add new region here */
};

/********************************************************
        Global Variables
********************************************************/

/**
 *  MLAN Adapter pointer
 */
mlan_adapter *g_pmadapter = MNULL;

/**
 * The table to keep region code
 */
t_u16 region_code_index[MRVDRV_MAX_REGION_CODE] =
    { 0x10, 0x20, 0x30, 0x32, 0x40, 0x41, 0xff };

/**
 * The rates supported by the card
 */
t_u16 WlanDataRates[WLAN_SUPPORTED_RATES_EXT] =
    { 0x02, 0x04, 0x0B, 0x16, 0x00, 0x0C, 0x12,
    0x18, 0x24, 0x30, 0x48, 0x60, 0x6C, 0x90,
    0x0D, 0x1A, 0x27, 0x34, 0x4E, 0x68, 0x75,
    0x82, 0x0C, 0x1B, 0x36, 0x51, 0x6C, 0xA2,
    0xD8, 0xF3, 0x10E, 0x00
};

/**
 * The rates supported in B mode
 */
t_u8 SupportedRates_B[B_SUPPORTED_RATES] = { 0x02, 0x04, 0x0b, 0x16, 0 };

/**
 * The rates supported in G mode (BAND_G, BAND_G|BAND_GN)
 */
t_u8 SupportedRates_G[G_SUPPORTED_RATES] =
    { 0x0c, 0x12, 0x18, 0x24, 0x30, 0x48, 0x60, 0x6c, 0 };

/**
 * The rates supported in BG mode (BAND_B|BAND_G, BAND_B|BAND_G|BAND_GN)
 */
t_u8 SupportedRates_BG[BG_SUPPORTED_RATES] =
    { 0x02, 0x04, 0x0b, 0x0c, 0x12, 0x16, 0x18, 0x24, 0x30, 0x48,
    0x60, 0x6c, 0
};

/**
 * The rates supported in N mode
 */
t_u8 SupportedRates_N[N_SUPPORTED_RATES] = { 0x02, 0x04, 0 };

/**
 * The rates supported for ad-hoc B mode
 */
t_u8 AdhocRates_B[B_SUPPORTED_RATES] = { 0x82, 0x84, 0x8b, 0x96, 0 };

/**
 * The rates supported for ad-hoc G mode
 */
t_u8 AdhocRates_G[G_SUPPORTED_RATES] =
    { 0x8c, 0x12, 0x98, 0x24, 0xb0, 0x48, 0x60, 0x6c, 0 };

/**
 * The rates supported for ad-hoc BG mode
 */
t_u8 AdhocRates_BG[BG_SUPPORTED_RATES] =
    { 0x82, 0x84, 0x8b, 0x96, 0x0c, 0x12, 0x18, 0x24, 0x30, 0x48,
    0x60, 0x6c, 0
};

void wlan_delete_bsspriotbl(pmlan_private priv);

/********************************************************
        Local Functions
********************************************************/
/** 
 *  @brief Find a character in a string.
 *   
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param s            A pointer to string
 *  @param c            Character to be located 
 *  @param n            The length of string
 *
 *  @return        A pointer to the first occurrence of c in string, or MNULL if c is not found.
 */
static void *
wlan_memchr(pmlan_adapter pmadapter, void *s, int c, int n)
{
    const t_u8 *p = s;

    ENTER();

    while (n--) {
        if ((t_u8) c == *p++) {
            LEAVE();
            return (void *) (p - 1);
        }
    }

    LEAVE();
    return MNULL;
}

/** 
 *  @brief This timer function handles host to device heart beat.
 *  It sends a HeartBeat command if there is not any traffic from
 *  host to device during last heart beat time period.
 *  
 *  @param function_context   A pointer to function_context
 *  @return 	   n/a
 */
static t_void
wlan_host_heart_beat(void *function_context)
{
    pmlan_adapter pmadapter = (pmlan_adapter) function_context;
    pmlan_callbacks pcb = (pmlan_callbacks) & pmadapter->callbacks;

    ENTER();

    if ((pmadapter->is_hb_ignored == MFALSE)
        && (pmadapter->is_hs_configured == MFALSE)
        ) {
        if ((pmadapter->host_to_dev_traffic <= 1)
            && (!pmadapter->curr_cmd)) {
            pmadapter->host_to_dev_traffic = 0;
            wlan_prepare_cmd(pmadapter->priv[0],
                             HostCmd_CMD_HEART_BEAT,
                             HostCmd_ACT_GEN_GET, 0, 0, MNULL);
            /* Signal MOAL to trigger mlan_main_process */
            wlan_recv_event(pmadapter->priv[0],
                            MLAN_EVENT_ID_DRV_DEFER_HANDLING, MNULL);
        } else {
            pmadapter->host_to_dev_traffic = 1;
        }
    }
    pcb->moal_start_timer(pmadapter->hb_host_timer, MFALSE,
                          pmadapter->hb_host_time);

    LEAVE();
}

/** 
 *  @brief This timer function handles device to host heart beat.
 *  It's a fatal error if there is not any traffic from device
 *  to host during last two heart beat time periods.
 *  
 *  @param function_context    A pointer to function_context
 *  @return 	   n/a
 */
static t_void
wlan_device_heart_beat(void *function_context)
{
    pmlan_adapter pmadapter = (pmlan_adapter) function_context;
    pmlan_callbacks pcb = (pmlan_callbacks) & pmadapter->callbacks;

    ENTER();

    if ((pmadapter->is_hb_ignored == MFALSE)
        && (pmadapter->is_hs_configured == MFALSE)
        ) {
        pmadapter->hb_dev_counter++;
        if (pmadapter->hb_dev_counter > 2) {
            pmadapter->hb_dev_counter = 0;
            PRINTM(FATAL, "ERROR: No heart beat from device!\n");
        }
    }
    pcb->moal_start_timer(pmadapter->hb_device_timer, MFALSE,
                          pmadapter->hb_dev_time);

    LEAVE();
}

/**
 *  @brief This function append the vendor specific IE TLV
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param vsie_mask    VSIE bit mask
 *  @param ppbuffer     A Pointer to command buffer pointer
 *
 *  @return bytes added to the buffer       
 */
int
wlan_cmd_append_vsie_tlv(IN mlan_private * pmpriv,
                         IN t_u16 vsie_mask, OUT t_u8 ** ppbuffer)
{
    int id, ret_len = 0;
    MrvlIETypes_VendorParamSet_t *pvs_param_set;

    ENTER();

    /* Null Checks */
    if (ppbuffer == 0) {
        LEAVE();
        return 0;
    }
    if (*ppbuffer == 0) {
        LEAVE();
        return 0;
    }

    /**
     * Traverse through the saved vendor specific IE array and append
     * the selected(scan/assoc/adhoc) IE as TLV to the command
     */
    for (id = 0; id < MLAN_MAX_VSIE_NUM; id++) {
        if (pmpriv->vs_ie[id].mask & vsie_mask) {
            pvs_param_set = (MrvlIETypes_VendorParamSet_t *) * ppbuffer;
            pvs_param_set->header.type = wlan_cpu_to_le16(TLV_TYPE_PASSTHROUGH);
            pvs_param_set->header.len =
                (((t_u16) pmpriv->vs_ie[id].ie[1]) & 0x00FF) + 2;
            memcpy(pvs_param_set->ie, pmpriv->vs_ie[id].ie,
                   pvs_param_set->header.len);
            HEXDUMP("VENDOR_SPEC_IE", (t_u8 *) pvs_param_set,
                    sizeof(MrvlIEtypesHeader_t) + pvs_param_set->header.len);
            *ppbuffer +=
                pvs_param_set->header.len + sizeof(MrvlIEtypesHeader_t);
            ret_len += pvs_param_set->header.len + sizeof(MrvlIEtypesHeader_t);
            pvs_param_set->header.len =
                wlan_cpu_to_le16(pvs_param_set->header.len);
        }
    }

    LEAVE();
    return ret_len;
}

/********************************************************
        Global Functions
********************************************************/
/** 
 *  @brief This function finds the CFP in 
 *  		region_cfp_table based on region and band parameter.
 *  
 *  @param pmadapter  A pointer to mlan_adapter structure
 *  @param region     The region code
 *  @param band       The band
 *  @param cfp_no     A pointer to CFP number
 *
 *  @return           A pointer to CFP
 */
chan_freq_power_t *
wlan_get_region_cfp_table(pmlan_adapter pmadapter, t_u8 region, t_u8 band,
                          int *cfp_no)
{
    int i;

    ENTER();

    for (i = 0; i < sizeof(region_cfp_table) / sizeof(region_cfp_table_t); i++) {
        PRINTM(INFO, "region_cfp_table[i].region=%d\n",
               region_cfp_table[i].region);
        /* Check if region matches */
        if (region_cfp_table[i].region == region) {
            /* Select by band */
            if (band & (BAND_B | BAND_G | BAND_GN)) {
                *cfp_no = region_cfp_table[i].cfp_no_BG;
                LEAVE();
                return region_cfp_table[i].cfp_BG;
            } else {
                PRINTM(ERROR, "Error Band[0x%x]\n", band);
                LEAVE();
                return MNULL;

            }
        }
    }

    LEAVE();
    return MNULL;
}

/** 
 *  @brief This function search through all the regions cfp table to find the channel, 
 *            if the channel is found then gets the MIN txpower of the channel 
 *            present in all the regions.
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param channel      Channel number.
 *
 *  @return             The Tx power
 */
t_u8
wlan_get_txpwr_of_chan_from_cfp(mlan_private * pmpriv, t_u8 channel)
{
    t_u8 i = 0;
    t_u8 j = 0;
    t_u8 tx_power = 0;
    t_u32 cfp_no;
    chan_freq_power_t *cfp = MNULL;

    ENTER();

    for (i = 0; i < NELEMENTS(region_cfp_table); i++) {
        /* Get CFP */
        cfp = region_cfp_table[i].cfp_BG;
        cfp_no = region_cfp_table[i].cfp_no_BG;

        /* Find matching channel and get Tx power */
        for (j = 0; j < cfp_no; j++) {
            if ((cfp + j)->channel == channel) {
                if (tx_power != 0)
                    tx_power = MIN(tx_power, (cfp + j)->max_tx_power);
                else
                    tx_power = (cfp + j)->max_tx_power;
                break;
            }
        }

    }

    LEAVE();
    return tx_power;
}

/**
 *  @brief This function registers MOAL to MLAN module.
 *  
 *  @param pmdevice        A pointer to a mlan_device structure
 *                         allocated in MOAL
 *  @param ppmlan_adapter  A pointer to a t_void pointer to store
 *                         mlan_adapter structure pointer as the context
 *
 *  @return                MLAN_STATUS_SUCCESS
 *                             The registration succeeded.
 *                         MLAN_STATUS_FAILURE
 *                             The registration failed.
 *
 * mlan_status mlan_register (
 *   IN pmlan_device     pmdevice,
 *   OUT t_void          **ppmlan_adapter
 * );
 *
 * Comments
 *   MOAL constructs mlan_device data structure to pass moal_handle and
 *   mlan_callback table to MLAN. MLAN returns mlan_adapter pointer to
 *   the ppmlan_adapter buffer provided by MOAL.
 * Headers:
 *   declared in mlan_decl.h
 * See Also
 *   mlan_unregister
 */
mlan_status
mlan_register(IN pmlan_device pmdevice, OUT t_void ** ppmlan_adapter)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    pmlan_adapter pmadapter = MNULL;
    pmlan_callbacks pcb = MNULL;
    t_u32 i = 0;

    ASSERT(pmdevice);
    ASSERT(ppmlan_adapter);
    ASSERT(pmdevice->callbacks.moal_malloc);
    ASSERT(pmdevice->callbacks.moal_memset);
    ASSERT(pmdevice->callbacks.moal_memmove);

    /* Allocate memory for adapter structure */
    if ((pmdevice->callbacks.moal_malloc(sizeof(mlan_adapter),
                                         (t_u8 **) & pmadapter) !=
         MLAN_STATUS_SUCCESS)
        || !pmadapter) {
        ret = MLAN_STATUS_FAILURE;
        goto exit_register;
    }
    g_pmadapter = pmadapter;

    pmdevice->callbacks.moal_memset(pmadapter, 0, sizeof(mlan_adapter));

    pcb = &pmadapter->callbacks;

    /* Save callback functions */
    pmdevice->callbacks.moal_memmove(pcb,
                                     &pmdevice->callbacks,
                                     sizeof(mlan_callbacks));

    /* Assertion for all callback functions */
    ASSERT(pcb->moal_init_fw_complete);
    ASSERT(pcb->moal_shutdown_fw_complete);
    ASSERT(pcb->moal_send_packet_complete);
    ASSERT(pcb->moal_recv_complete);
    ASSERT(pcb->moal_recv_packet);
    ASSERT(pcb->moal_recv_event);
    ASSERT(pcb->moal_ioctl_complete);
    ASSERT(pcb->moal_write_data_async);
    ASSERT(pcb->moal_write_data_sync);
    ASSERT(pcb->moal_read_data_sync);
    ASSERT(pcb->moal_mfree);
    ASSERT(pcb->moal_memcpy);
    ASSERT(pcb->moal_memcmp);
    ASSERT(pcb->moal_get_system_time);
    ASSERT(pcb->moal_init_timer);
    ASSERT(pcb->moal_free_timer);
    ASSERT(pcb->moal_start_timer);
    ASSERT(pcb->moal_stop_timer);
    ASSERT(pcb->moal_init_lock);
    ASSERT(pcb->moal_free_lock);
    ASSERT(pcb->moal_spin_lock);
    ASSERT(pcb->moal_spin_unlock);
    ASSERT(pcb->moal_print);

    ENTER();

    /* Save pmoal_handle */
    pmadapter->pmoal_handle = pmdevice->pmoal_handle;

#ifdef MFG_CMD_SUPPORT
    pmadapter->mfgmode = pmdevice->mfgmode;
#endif

    for (i = 0; i < MLAN_MAX_BSS_NUM; i++) {
        pmadapter->priv[i] = MNULL;
        if (pmdevice->bss_attr[i].active == MTRUE) {
            /* For valid bss_attr, allocate memory for private structure */
            if ((pcb->moal_malloc(sizeof(mlan_private),
                                  (t_u8 **) & pmadapter->priv[i]) !=
                 MLAN_STATUS_SUCCESS)
                || !pmadapter->priv[i]) {
                ret = MLAN_STATUS_FAILURE;
                goto error;
            }

            memset(pmadapter->priv[i], 0, sizeof(mlan_private));

            pmadapter->priv[i]->adapter = pmadapter;

            /* Save bss_type, frame_type & bss_priority */
            pmadapter->priv[i]->bss_type = pmdevice->bss_attr[i].bss_type;
            pmadapter->priv[i]->frame_type = pmdevice->bss_attr[i].frame_type;
            pmadapter->priv[i]->bss_priority =
                pmdevice->bss_attr[i].bss_priority;

            /* Save bss_num */
            pmadapter->priv[i]->bss_num = i;
        }
    }

    /* Initialize locks */
    if (pcb->moal_init_lock(&pmadapter->pmlan_lock)
        != MLAN_STATUS_SUCCESS) {
        ret = MLAN_STATUS_FAILURE;
        goto error;
    }

    if (pcb->moal_init_lock(&pmadapter->pmain_proc_lock)
        != MLAN_STATUS_SUCCESS) {
        ret = MLAN_STATUS_FAILURE;
        goto error;
    }

    if (pcb->moal_init_lock(&pmadapter->pmlan_cmd_lock)
        != MLAN_STATUS_SUCCESS) {
        ret = MLAN_STATUS_FAILURE;
        goto error;
    }
    if (pcb->
        moal_init_timer(&pmadapter->pmlan_cmd_timer, wlan_cmd_timeout_func,
                        pmadapter)
        != MLAN_STATUS_SUCCESS) {
        ret = MLAN_STATUS_FAILURE;
        goto error;
    }
    /* Initializing host to device Heart Beat timer */
    if (pcb->
        moal_init_timer(&pmadapter->hb_host_timer, wlan_host_heart_beat,
                        pmadapter)
        != MLAN_STATUS_SUCCESS) {
        ret = MLAN_STATUS_FAILURE;
        goto error;
    }
    pmadapter->hb_host_timer_is_set = MFALSE;
    /* Initializing device to host Heart Beat timer */
    if (pcb->
        moal_init_timer(&pmadapter->hb_device_timer, wlan_device_heart_beat,
                        pmadapter)
        != MLAN_STATUS_SUCCESS) {
        ret = MLAN_STATUS_FAILURE;
        goto error;
    }
    pmadapter->hb_dev_timer_is_set = MFALSE;
    /* Return pointer of mlan_adapter to MOAL */
    *ppmlan_adapter = pmadapter;

    LEAVE();
    goto exit_register;

  error:
    PRINTM(INFO, "Leave mlan_register with error\n");
    LEAVE();
    /* Free resources */
    if (pmadapter->hb_device_timer)
        pcb->moal_free_timer(pmadapter->hb_device_timer);
    if (pmadapter->hb_host_timer)
        pcb->moal_free_timer(pmadapter->hb_host_timer);
    if (pmadapter->pmlan_cmd_timer)
        pcb->moal_free_timer(pmadapter->pmlan_cmd_timer);
    if (pmadapter->pmlan_cmd_lock)
        pcb->moal_free_lock(pmadapter->pmlan_cmd_lock);
    if (pmadapter->pmain_proc_lock)
        pcb->moal_free_lock(pmadapter->pmain_proc_lock);
    if (pmadapter->pmlan_lock)
        pcb->moal_free_lock(pmadapter->pmlan_lock);
    for (i = 0; i < MLAN_MAX_BSS_NUM; i++) {
        if (pmadapter->priv[i])
            pcb->moal_mfree((t_u8 *) pmadapter->priv[i]);
    }
    pcb->moal_mfree((t_u8 *) pmadapter);

  exit_register:
    return ret;
}

/**
 *  @brief This function unregisters MOAL from MLAN module.
 *  
 *  @param pmlan_adapter   A pointer to a mlan_device structure
 *                         allocated in MOAL
 *
 *  @return                MLAN_STATUS_SUCCESS
 *                             The deregistration succeeded.
 *                         MLAN_STATUS_FAILURE
 *                             The deregistration failed.
 */
mlan_status
mlan_unregister(IN t_void * pmlan_adapter)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_adapter *pmadapter = pmlan_adapter;
    pmlan_callbacks pcb;
    t_s32 i = 0;

    ASSERT(pmlan_adapter);

    ENTER();

    pcb = &pmadapter->callbacks;

    /* Free timers */
    if (pmadapter->hb_device_timer)
        pcb->moal_free_timer(pmadapter->hb_device_timer);
    if (pmadapter->hb_host_timer)
        pcb->moal_free_timer(pmadapter->hb_host_timer);
    if (pmadapter->pmlan_cmd_timer)
        pcb->moal_free_timer(pmadapter->pmlan_cmd_timer);

    /* Free locks */
    pcb->moal_free_lock(pmadapter->pmlan_cmd_lock);
    pcb->moal_free_lock(pmadapter->pmain_proc_lock);
    pcb->moal_free_lock(pmadapter->pmlan_lock);

    /* Free private structures */
    for (i = 0; i < MLAN_MAX_BSS_NUM; i++) {
        if (pmadapter->priv[i]) {
            wlan_free_curr_bcn(pmadapter->priv[i]);
            pcb->moal_free_lock(pmadapter->priv[i]->curr_bcn_buf_lock);
            pcb->moal_mfree((t_u8 *) pmadapter->priv[i]);
        }
    }

    LEAVE();
    /* Free mlan_adapter */
    pcb->moal_mfree((t_u8 *) pmadapter);

    return ret;
}

/**
 *  @brief This function downloads the firmware
 *  
 *  @param pmlan_adapter   A pointer to a t_void pointer to store
 *                         mlan_adapter structure pointer
 *  @param pmfw            A pointer to firmware image
 *
 *  @return                MLAN_STATUS_SUCCESS
 *                             The firmware download succeeded.
 *                         MLAN_STATUS_FAILURE
 *                             The firmware download failed.
 */
mlan_status
mlan_dnld_fw(IN t_void * pmlan_adapter, IN pmlan_fw_image pmfw)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_adapter *pmadapter = pmlan_adapter;

    ENTER();
    ASSERT(pmlan_adapter);

    if (pmfw) {
        /* Download helper/firmware */
        ret = wlan_dnld_fw(pmadapter, pmfw);
        if (ret != MLAN_STATUS_SUCCESS) {
            PRINTM(ERROR, "wlan_dnld_fw fail ret=0x%x\n", ret);
            LEAVE();
            return ret;
        }
    }

    LEAVE();
    return ret;
}

/**
 *  @brief This function initializes the firmware
 *  
 *  @param pmlan_adapter   A pointer to a t_void pointer to store
 *                         mlan_adapter structure pointer
 *
 *  @return                MLAN_STATUS_SUCCESS
 *                             The firmware initialization succeeded.
 *                         MLAN_STATUS_PENDING
 *                             The firmware initialization is pending.
 *                         MLAN_STATUS_FAILURE
 *                             The firmware initialization failed.
 */
mlan_status
mlan_init_fw(IN t_void * pmlan_adapter)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_adapter *pmadapter = pmlan_adapter;

    ENTER();
    ASSERT(pmlan_adapter);

    pmadapter->hw_status = WlanHardwareStatusInitializing;

    /* Initialize firmware, may return PENDING */
    ret = wlan_init_fw(pmadapter);
    PRINTM(INFO, "wlan_init_fw returned ret=0x%x\n", ret);

    LEAVE();
    return ret;
}

/**
 *  @brief The cmdresp handler calls this function for init_fw_complete callback
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *
 *  @return		MLAN_STATUS_SUCCESS
 *                      	The firmware initialization callback succeeded.
 * 			MLAN_STATUS_FAILURE
 *                      	The firmware initialization callback failed.
 */
mlan_status
wlan_init_fw_complete(IN pmlan_adapter pmadapter)
{
    mlan_status status = MLAN_STATUS_SUCCESS;
    pmlan_callbacks pcb = &pmadapter->callbacks;

    ENTER();

    /* Check if hardware is ready */
    if (pmadapter->hw_status != WlanHardwareStatusReady)
        status = MLAN_STATUS_FAILURE;

    LEAVE();
    /* Invoke callback */
    return pcb->moal_init_fw_complete(pmadapter->pmoal_handle, status);
}

/**
 *  @brief The cmdresp handler calls this function for shutdown_fw_complete callback
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *
 *  @return		MLAN_STATUS_SUCCESS
 *                      	The firmware shutdown callback succeeded.
 *			MLAN_STATUS_FAILURE
 *                      	The firmware shutdown callback failed.
 */
mlan_status
wlan_shutdown_fw_complete(IN pmlan_adapter pmadapter)
{
    pmlan_callbacks pcb = &pmadapter->callbacks;
    mlan_status status = MLAN_STATUS_SUCCESS;

    pmadapter->hw_status = WlanHardwareStatusNotReady;
    /* Invoke callback */
    return pcb->moal_shutdown_fw_complete(pmadapter->pmoal_handle, status);
}

/**
 *  @brief Shutdown firmware
 *
 *  @param pmlan_adapter	A pointer to mlan_adapter structure
 *
 *  @return			MLAN_STATUS_SUCCESS
 *                      		The firmware shutdown call succeeded.
 *				MLAN_STATUS_PENDING
 *      	                	The firmware shutdown call is pending.
 *				MLAN_STATUS_FAILURE
 *      	                	The firmware shutdown call failed.
 */
mlan_status
mlan_shutdown_fw(IN t_void * pmlan_adapter)
{
    mlan_status ret = MLAN_STATUS_PENDING;
    mlan_adapter *pmadapter = pmlan_adapter;
    pmlan_callbacks pcb;
    t_s32 i = 0;
    pmlan_buffer pmbuf;

    ENTER();
    ASSERT(pmlan_adapter);
    /* mlan already shutdown */
    if (pmadapter->hw_status == WlanHardwareStatusNotReady)
        return MLAN_STATUS_SUCCESS;

    pmadapter->hw_status = WlanHardwareStatusClosing;
    /* wait for mlan_process to complete */
    if (pmadapter->mlan_processing) {
        PRINTM(WARN, "mlan main processing is still running\n");
        return ret;
    }

    /* shut down mlan */
    PRINTM(INFO, "Shutdown mlan...\n");
    pcb = &pmadapter->callbacks;
    /* Clean up Tx/Rx queues and delete BSS priority table */
    for (i = 0; i < MLAN_MAX_BSS_NUM; i++) {
        if (pmadapter->priv[i]) {
            wlan_clean_txrx(pmadapter->priv[i]);
            wlan_wmm_cleanup_node(pmadapter->priv[i]);
            pcb->moal_free_lock(pmadapter->priv[i]->rx_pkt_lock);
            wlan_delete_bsspriotbl(pmadapter->priv[i]);
            pcb->moal_free_lock(pmadapter->priv[i]->wmm.ra_list_spinlock);
        }
    }
    while ((pmbuf =
            (pmlan_buffer) util_dequeue_list(&pmadapter->rx_data_queue,
                                             pcb->moal_spin_lock,
                                             pcb->moal_spin_unlock))) {
        pcb->moal_recv_complete(pmadapter->pmoal_handle, pmbuf,
                                MLAN_USB_EP_DATA, MLAN_STATUS_FAILURE);
    }

    util_free_list_head(&pmadapter->rx_data_queue, pcb->moal_free_lock);

    for (i = 0; i < MLAN_MAX_BSS_NUM; i++)
        util_free_list_head(&pmadapter->bssprio_tbl[i].bssprio_head,
                            pcb->moal_free_lock);

    if (pcb->moal_spin_lock(pmadapter->pmlan_lock)
        != MLAN_STATUS_SUCCESS) {
        ret = MLAN_STATUS_FAILURE;
        goto exit_shutdown_fw;
    }

    /* Free adapter structure */
    wlan_free_adapter(pmadapter);

    if (pcb->moal_spin_unlock(pmadapter->pmlan_lock)
        != MLAN_STATUS_SUCCESS) {
        ret = MLAN_STATUS_FAILURE;
        goto exit_shutdown_fw;
    }

    /* Notify completion */
    ret = wlan_shutdown_fw_complete(pmadapter);

  exit_shutdown_fw:
    LEAVE();
    return ret;
}

/**
 *  @brief This function wakes up the card.
 *
 *  @param pmadapter		A pointer to mlan_adapter structure
 *
 *  @return			MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_pm_wakeup_card(IN pmlan_adapter pmadapter)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;

    ENTER();

    /* Simulation of HS_AWAKE event */
    pmadapter->pm_wakeup_fw_try = MFALSE;
    pmadapter->pm_wakeup_card_req = MFALSE;

    LEAVE();
    return ret;
}

/**
 *  @brief This function resets the PM setting of the card.
 *
 *  @param pmadapter		A pointer to mlan_adapter structure
 *
 *  @return			MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_pm_reset_card(IN pmlan_adapter pmadapter)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;

    ENTER();
    LEAVE();
    return ret;
}

/**
 *  @brief The main process
 *
 *  @param pmlan_adapter	A pointer to mlan_adapter structure
 *
 *  @return			MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
mlan_main_process(IN t_void * pmlan_adapter)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_adapter *pmadapter = pmlan_adapter;
    pmlan_callbacks pcb;
    pmlan_buffer pmbuf;

    ENTER();

    ASSERT(pmlan_adapter);

    pcb = &pmadapter->callbacks;

    pcb->moal_spin_lock(pmadapter->pmain_proc_lock);

    /* Check if already processing */
    if (pmadapter->mlan_processing) {
        pcb->moal_spin_unlock(pmadapter->pmain_proc_lock);
        goto exit_main_proc;
    } else {
        pmadapter->mlan_processing = MTRUE;
        pcb->moal_spin_unlock(pmadapter->pmain_proc_lock);
    }
  process_start:
    do {
        /* Is MLAN shutting down or not ready? */
        if ((pmadapter->hw_status == WlanHardwareStatusClosing) ||
            (pmadapter->hw_status == WlanHardwareStatusNotReady))
            break;

        if (IS_CARD_RX_RCVD(pmadapter)) {
            pmadapter->pm_wakeup_fw_try = MFALSE;

            if (pmadapter->ps_state == PS_STATE_SLEEP)
                pmadapter->ps_state = PS_STATE_AWAKE;
        } else {
            /* We have tried to wakeup the card already */
            if (pmadapter->pm_wakeup_fw_try)
                break;

            /* In PRE_SLEEP state we cannot transmit any data hence we just
               break from here */
            if (pmadapter->ps_state == PS_STATE_PRE_SLEEP)
                break;

            /* Sleep state, no need to wake up the card */
            if ((pmadapter->ps_state == PS_STATE_SLEEP)
                && (!pmadapter->pm_wakeup_card_req))
                break;

            if (pmadapter->scan_processing || pmadapter->data_sent
                || wlan_wmm_lists_empty(pmadapter)
                ) {
                if (pmadapter->cmd_sent || pmadapter->curr_cmd ||
                    (!util_peek_list(&pmadapter->cmd_pending_q,
                                     pcb->moal_spin_lock,
                                     pcb->moal_spin_unlock)))
                    break;
            }

            /* Need to wake up the card ? */
            if (pmadapter->pm_wakeup_card_req) {
                pmadapter->pm_wakeup_fw_try = MTRUE;
                wlan_pm_wakeup_card(pmadapter);
                continue;
            }
        }

        /* Check for Rx data */
        while ((pmbuf =
                (pmlan_buffer) util_dequeue_list(&pmadapter->rx_data_queue,
                                                 pmadapter->callbacks.
                                                 moal_spin_lock,
                                                 pmadapter->callbacks.
                                                 moal_spin_unlock))) {
            wlan_handle_rx_packet(pmadapter, pmbuf);
        }

        /* Check for Cmd Resp */
        if (pmadapter->cmd_resp_received) {
            pmadapter->cmd_resp_received = MFALSE;
            wlan_process_cmdresp(pmadapter);

            /* call moal back when init_fw is done */
            if (pmadapter->hw_status == WlanHardwareStatusInitdone) {
                pmadapter->hw_status = WlanHardwareStatusReady;
                wlan_init_fw_complete(pmadapter);
            }
        }

        /* Check for event */
        if (pmadapter->event_received && pmadapter->pmlan_buffer_event) {
            pmadapter->event_received = MFALSE;
            wlan_process_event(pmadapter);
        }

        /* Check if we need to confirm Sleep Request received previously */
        if (pmadapter->ps_state == PS_STATE_PRE_SLEEP) {
            if (!pmadapter->cmd_sent && !pmadapter->curr_cmd) {
                ASSERT(pmadapter->priv[0]->media_connected == MTRUE);
                wlan_check_ps_cond(pmadapter->priv[0]);
            }
        }

        /* 
         * The ps_state may have been changed during processing of
         * Sleep Request event.
         */
        if ((pmadapter->ps_state == PS_STATE_SLEEP)
            || (pmadapter->ps_state == PS_STATE_PRE_SLEEP))
            continue;

        /* The hs_activated flag may be changed during processing of
           HOST_SLEEP_ACTIVATE command response */
        /* We cannot send command or data if both hs_activated and
           pm_wakeup_card_req flags are set */
        if (pmadapter->hs_activated && pmadapter->pm_wakeup_card_req) {
            PRINTM(WARN, "main_process: cannot send command or data, "
                   "hs_activated=%d\n", pmadapter->hs_activated);
            continue;
        }

        if (!pmadapter->cmd_sent && !pmadapter->curr_cmd) {
            if (wlan_exec_next_cmd(pmadapter) == MLAN_STATUS_FAILURE) {
                ret = MLAN_STATUS_FAILURE;
                break;
            }
        }

        if (!pmadapter->scan_processing && !pmadapter->data_sent &&
            !wlan_wmm_lists_empty(pmadapter))
            wlan_wmm_process_tx(pmadapter);
    } while (MTRUE);

    pcb->moal_spin_lock(pmadapter->pmain_proc_lock);
    if (IS_CARD_RX_RCVD(pmadapter)) {
        pcb->moal_spin_unlock(pmadapter->pmain_proc_lock);
        goto process_start;
    }
    pmadapter->mlan_processing = MFALSE;
    pcb->moal_spin_unlock(pmadapter->pmain_proc_lock);

  exit_main_proc:
    if (pmadapter->hw_status == WlanHardwareStatusClosing)
        mlan_shutdown_fw(pmadapter);
    LEAVE();
    return ret;
}

/**
 *  @brief Function to send packet
 *
 *  @param pmlan_adapter	A pointer to mlan_adapter structure
 *  @param pmbuf		A pointer to mlan_buffer structure
 *
 *  @return			MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
mlan_send_packet(IN t_void * pmlan_adapter, IN pmlan_buffer pmbuf)
{
    mlan_status ret = MLAN_STATUS_PENDING;
    mlan_adapter *pmadapter = pmlan_adapter;

    ENTER();
    ASSERT(pmlan_adapter && pmbuf);

    ASSERT(pmbuf->bss_num < MLAN_MAX_BSS_NUM);

    /* Transmit the packet */
    wlan_wmm_add_buf_txqueue(pmadapter, pmbuf);

    LEAVE();
    return ret;
}

/**
 *  @brief Packet send completion callback
 *
 *  @param pmlan_adapter	A pointer to mlan_adapter structure
 *  @param pmbuf		A pointer to mlan_buffer structure
 *  @param port			Data port
 *  @param status		Callback status
 *
 *  @return			MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
mlan_write_data_async_complete(IN t_void * pmlan_adapter,
                               IN pmlan_buffer pmbuf,
                               IN t_u32 port, IN mlan_status status)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_adapter *pmadapter = pmlan_adapter;
    pmlan_callbacks pcb;

    ENTER();

    ASSERT(pmlan_adapter && pmbuf);

    pcb = &pmadapter->callbacks;
    if (pmadapter->hb_host_timer_is_set && pmadapter->host_to_dev_traffic <= 1)
        pmadapter->host_to_dev_traffic++;
    if (port == MLAN_USB_EP_CMD_EVENT) {
        pmadapter->cmd_sent = MFALSE;
        PRINTM(CMND, "mlan_write_data_async_complete: CMD\n");
    } else if (port == MLAN_USB_EP_DATA) {
        pmadapter->data_sent = MFALSE;
        PRINTM(DATA, "mlan_write_data_async_complete: DATA\n");

        if (!pmbuf->pdesc) {
            /* pmbuf was allocated by MLAN */
            wlan_free_mlan_buffer(&pmadapter->callbacks, pmbuf);
        } else {
            /* pmbuf was allocated by MOAL */
            pcb->moal_send_packet_complete(pmadapter->pmoal_handle,
                                           pmbuf, status);
        }
    }

    LEAVE();
    return ret;
}

/**
 *  @brief Packet receive handler
 *
 *  @param pmlan_adapter	A pointer to mlan_adapter structure
 *  @param pmbuf		A pointer to mlan_buffer structure
 *  @param port			Data port
 *
 *  @return			MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE or MLAN_STATUS_PENDING
 */
mlan_status
mlan_recv(IN t_void * pmlan_adapter, IN pmlan_buffer pmbuf, IN t_u32 port)
{
    mlan_status ret = MLAN_STATUS_PENDING;
    mlan_adapter *pmadapter = pmlan_adapter;
    t_u8 *pbuf;
    t_u32 len, recv_type;
    t_u32 event_cause;
    t_u32 sec, usec;

    ENTER();

    ASSERT(pmlan_adapter && pmbuf);

    if (pmadapter->hb_dev_timer_is_set) {
        pmadapter->hb_dev_counter = 0;
    }

    pbuf = pmbuf->pbuf + pmbuf->data_offset;
    len = pmbuf->data_len;

    ASSERT(len >= sizeof(t_u32));
    recv_type = *(t_u32 *) pbuf;
    recv_type = wlan_le32_to_cpu(recv_type);
    pbuf += sizeof(t_u32);
    len -= sizeof(t_u32);

    pmadapter->callbacks.moal_get_system_time(&sec, &usec);

    switch (port) {
    case MLAN_USB_EP_CTRL:
        PRINTM(INFO, "mlan_recv: CTRL\n");
        ret = MLAN_STATUS_SUCCESS;
        break;
    case MLAN_USB_EP_CMD_EVENT:
        switch (recv_type) {
        case MLAN_USB_TYPE_CMD:
            PRINTM(CMND, "mlan_recv: CMD (%lu.%lu)\n", sec, usec);
            if (len > MRVDRV_SIZE_OF_CMD_BUFFER) {
                ret = MLAN_STATUS_FAILURE;
                PRINTM(ERROR, "mlan_recv: CMD too large\n");
            } else if (!pmadapter->curr_cmd) {
                ret = MLAN_STATUS_FAILURE;
                PRINTM(ERROR, "mlan_recv: no curr_cmd\n");
            } else {
                pmadapter->upld_len = len;
                memmove(pmadapter->curr_cmd->pcmd_buf, pbuf, len);
                pmadapter->cmd_resp_received = MTRUE;
                pmadapter->callbacks.moal_recv_complete(pmadapter->pmoal_handle,
                                                        pmbuf, port,
                                                        MLAN_STATUS_SUCCESS);
                /* MOAL to call mlan_main_process for processing */
            }
            break;
        case MLAN_USB_TYPE_EVENT:
            ASSERT(len >= sizeof(t_u32));
            memmove(&event_cause, pbuf, sizeof(t_u32));
            pmadapter->event_cause = wlan_le32_to_cpu(event_cause);
            PRINTM(EVENT, "mlan_recv: EVENT 0x%x (%lu.%lu)\n",
                   pmadapter->event_cause, sec, usec);
            pbuf += sizeof(t_u32);
            len -= sizeof(t_u32);
            if ((len > 0) && (len < MAX_EVENT_SIZE))
                memmove(pmadapter->event_body, pbuf, len);
            pmadapter->event_received = MTRUE;
            pmadapter->pmlan_buffer_event = pmbuf;
            /* MOAL to call mlan_main_process for processing */
            break;
        default:
            ret = MLAN_STATUS_FAILURE;
            PRINTM(ERROR, "mlan_recv: unknown recv_type 0x%x\n", recv_type);
            break;
        }
        break;
    case MLAN_USB_EP_DATA:
        PRINTM(DATA, "mlan_recv: DATA (%lu.%lu)\n", sec, usec);
        if (len > MLAN_RX_DATA_BUF_SIZE) {
            ret = MLAN_STATUS_FAILURE;
            PRINTM(ERROR, "mlan_recv: DATA too large\n");
        } else {
            pmadapter->upld_len = len;
            pmadapter->callbacks.moal_get_system_time(&pmbuf->in_ts_sec,
                                                      &pmbuf->in_ts_usec);
            util_enqueue_list_tail(&pmadapter->rx_data_queue,
                                   (pmlan_linked_list) pmbuf,
                                   pmadapter->callbacks.moal_spin_lock,
                                   pmadapter->callbacks.moal_spin_unlock);
            pmadapter->data_received = MTRUE;
        }
        break;
    default:
        ret = MLAN_STATUS_FAILURE;
        PRINTM(ERROR, "mlan_recv: unknown port number 0x%x\n", port);
        break;
    }

    LEAVE();
    return ret;
}

/**
 *  @brief Packet receive completion callback handler
 *
 *  @param pmlan_adapter	A pointer to mlan_adapter structure
 *  @param pmbuf		A pointer to mlan_buffer structure
 *  @param status		Callback status
 *
 *  @return			MLAN_STATUS_SUCCESS
 */
mlan_status
mlan_recv_packet_complete(IN t_void * pmlan_adapter,
                          IN pmlan_buffer pmbuf, IN mlan_status status)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_adapter *pmadapter = pmlan_adapter;
    pmlan_private pmp;
    pmlan_callbacks pcb;
    pmlan_buffer pmbuf_parent;

    ENTER();
    ASSERT(pmlan_adapter && pmbuf);

    pcb = &pmadapter->callbacks;

    ASSERT(pmbuf->bss_num < MLAN_MAX_BSS_NUM);

    pmp = pmadapter->priv[pmbuf->bss_num];

    if (pmbuf->pparent) {
        pmbuf_parent = pmbuf->pparent;

        pmadapter->callbacks.moal_spin_lock(pmp->rx_pkt_lock);
        --pmbuf_parent->use_count;
        if (!pmbuf_parent->use_count) {
            pmadapter->callbacks.moal_spin_unlock(pmp->rx_pkt_lock);
            pcb->moal_recv_complete(pmadapter->pmoal_handle,
                                    pmbuf_parent, MLAN_USB_EP_DATA, status);
        } else {
            pmadapter->callbacks.moal_spin_unlock(pmp->rx_pkt_lock);
        }

        pcb->moal_mfree((t_u8 *) pmbuf);
    } else {
        pcb->moal_recv_complete(pmadapter->pmoal_handle, pmbuf,
                                MLAN_USB_EP_DATA, status);
    }

    LEAVE();
    return ret;
}

/** 
 *  @brief This function allocates a mlan_buffer.
 *
 *  @param pcb        Pointer to mlan_callbacks
 *  @param data_len   Data length
 *
 *  @return           mlan_buffer pointer or MNULL
 */
pmlan_buffer
wlan_alloc_mlan_buffer(pmlan_callbacks pcb, t_u32 data_len)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    pmlan_buffer pmbuf = MNULL;
    t_u32 buf_size = sizeof(mlan_buffer) + data_len;

    ENTER();

    ret = pcb->moal_malloc(buf_size, (t_u8 **) & pmbuf);
    if ((ret != MLAN_STATUS_SUCCESS) || !pmbuf) {
        pmbuf = MNULL;
        goto exit;
    }

    memset(pmbuf, 0, sizeof(mlan_buffer));

    pmbuf->pdesc = MNULL;
    pmbuf->pbuf = (t_u8 *) pmbuf + sizeof(mlan_buffer);
    pmbuf->data_offset = 0;
    pmbuf->data_len = data_len;

  exit:
    LEAVE();
    return pmbuf;
}

/** 
 *  @brief This function frees a mlan_buffer.
 *
 *  @param pcb        Pointer to mlan_callbacks
 *  @param pmbuf      Pointer to mlan_buffer
 *
 *  @return           N/A
 */
t_void
wlan_free_mlan_buffer(pmlan_callbacks pcb, pmlan_buffer pmbuf)
{
    ENTER();

    if (pcb && pmbuf)
        pcb->moal_mfree((t_u8 *) pmbuf);

    LEAVE();
    return;
}

/** 
 *  @brief Get the channel frequency power info for a specific channel
 *   
 *  @param pmadapter            A pointer to mlan_adapter structure
 *  @param band                 It can be BAND_A, BAND_G or BAND_B
 *  @param channel              The channel to search for
 *  @param region_channel       A pointer to region_chan_t structure
 *
 *  @return                     A pointer to chan_freq_power_t structure or MNULL if not found.
 */

chan_freq_power_t *
wlan_get_cfp_by_band_and_channel(pmlan_adapter pmadapter,
                                 t_u8 band,
                                 t_u16 channel, region_chan_t * region_channel)
{
    region_chan_t *rc;
    chan_freq_power_t *cfp = MNULL;
    int i, j;

    ENTER();

    for (j = 0; !cfp && (j < MAX_REGION_CHANNEL_NUM); j++) {
        rc = &region_channel[j];

        if (!rc->valid || !rc->pcfp)
            continue;
        switch (rc->band) {
        case BAND_B:
        case BAND_G:
            switch (band) {
            case BAND_GN:
            case BAND_B | BAND_G | BAND_GN:
            case BAND_G | BAND_GN:
            case BAND_B | BAND_G:
            case BAND_B:       /* Matching BAND_B/G */
            case BAND_G:
            case 0:
                break;
            default:
                continue;
            }
            break;
        default:
            continue;
        }
        if (channel == FIRST_VALID_CHANNEL)
            cfp = &rc->pcfp[0];
        else {
            for (i = 0; i < rc->num_cfp; i++) {
                if (rc->pcfp[i].channel == channel) {
                    cfp = &rc->pcfp[i];
                    break;
                }
            }
        }
    }

    if (!cfp && channel)
        PRINTM(ERROR, "wlan_get_cfp_by_band_and_channel(): cannot find "
               "cfp by band %d & channel %d\n", band, channel);

    LEAVE();
    return cfp;
}

/** 
 *  @brief Find the channel frequency power info for a specific channel
 *   
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param band         It can be BAND_A, BAND_G or BAND_B
 *  @param channel      The channel to search for
 *
 *  @return             A pointer to chan_freq_power_t structure or MNULL if not found.
 */
chan_freq_power_t *
wlan_find_cfp_by_band_and_channel(mlan_adapter * pmadapter,
                                  t_u8 band, t_u16 channel)
{
    chan_freq_power_t *cfp = MNULL;

    ENTER();

    if (pmadapter->state_11d.enable_11d == ENABLE_11D)
        cfp =
            wlan_get_cfp_by_band_and_channel(pmadapter, band, channel,
                                             pmadapter->universal_channel);
    else
        cfp =
            wlan_get_cfp_by_band_and_channel(pmadapter, band, channel,
                                             pmadapter->region_channel);

    LEAVE();
    return cfp;
}

/** 
 *  @brief Find the channel frequency power info for a specific frequency
 *   
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param band         It can be BAND_A, BAND_G or BAND_B
 *  @param freq         The frequency to search for
 *
 *  @return         Pointer to chan_freq_power_t structure; MNULL if not found
 */
chan_freq_power_t *
wlan_find_cfp_by_band_and_freq(mlan_adapter * pmadapter, t_u8 band, t_u32 freq)
{
    chan_freq_power_t *cfp = MNULL;
    region_chan_t *rc;
    int count = sizeof(pmadapter->region_channel) /
        sizeof(pmadapter->region_channel[0]);
    int i, j;

    ENTER();

    for (j = 0; !cfp && (j < count); j++) {
        rc = &pmadapter->region_channel[j];

        if (pmadapter->state_11d.enable_11d == ENABLE_11D)
            rc = &pmadapter->universal_channel[j];

        if (!rc->valid || !rc->pcfp)
            continue;
        switch (rc->band) {
        case BAND_B:
        case BAND_G:
            switch (band) {
            case BAND_GN:
            case BAND_B | BAND_G | BAND_GN:
            case BAND_G | BAND_GN:
            case BAND_B | BAND_G:
            case BAND_B:
            case BAND_G:
            case 0:
                break;
            default:
                continue;
            }
            break;
        default:
            continue;
        }
        for (i = 0; i < rc->num_cfp; i++) {
            if (rc->pcfp[i].freq == freq) {
                cfp = &rc->pcfp[i];
                break;
            }
        }
    }

    if (!cfp && freq)
        PRINTM(ERROR, "find_cfp_by_band_and_freql(): cannot find cfp by "
               "band %d & freq %d\n", band, freq);

    LEAVE();
    return cfp;
}

/** 
 *  @brief Use index to get the data rate
 *   
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param index        The index of data rate
 *  @param ht_info      ht info
 *
 *  @return                     Data rate or 0 
 */
t_u32
wlan_index_to_data_rate(pmlan_adapter pmadapter, t_u8 index, t_u8 ht_info)
{
    t_u16 mcs_rate[4][8] = { {0x1b, 0x36, 0x51, 0x6c, 0xa2, 0xd8, 0xf3, 0x10e}
    ,                           /* LG 40M */
    {0x1e, 0x3c, 0x5a, 0x78, 0xb4, 0xf0, 0x10e, 0x12c}
    ,                           /* SG 40M */
    {0x0d, 0x1a, 0x27, 0x34, 0x4e, 0x68, 0x75, 0x82}
    ,                           /* LG 20M */
    {0x0e, 0x1c, 0x2b, 0x39, 0x56, 0x73, 0x82, 0x90}
    };                          /* SG 20M */

    t_u32 rate;
    ENTER();

    if (ht_info & MBIT(0)) {
        if (index == MCS_INDEX_32) {
            if (ht_info & MBIT(2))
                rate = 0x0D;    /* MCS 32 SGI rate */
            else
                rate = 0x0C;    /* MCS 32 LGI rate */
        } else if (index < 8) {
            if (ht_info & MBIT(1)) {
                if (ht_info & MBIT(2))
                    rate = mcs_rate[1][index];  /* SGI, 40M */
                else
                    rate = mcs_rate[0][index];  /* LGI, 40M */
            } else {
                if (ht_info & MBIT(2))
                    rate = mcs_rate[3][index];  /* SGI, 20M */
                else
                    rate = mcs_rate[2][index];  /* LGI, 20M */
            }
        } else
            rate = WlanDataRates[0];
    } else {
        if (index >= WLAN_SUPPORTED_RATES_EXT)
            index = 0;
        rate = WlanDataRates[index];
    }
    LEAVE();
    return rate;
}

/** 
 *  @brief Check if Rate Auto
 *   
 *  @param pmpriv               A pointer to mlan_private structure
 *
 *  @return                     MTRUE or MFALSE
 */
t_u8
wlan_is_rate_auto(mlan_private * pmpriv)
{
    int i;
    int rate_num = 0;

    ENTER();

    for (i = 0; i < NELEMENTS(pmpriv->bitmap_rates); i++)
        if (pmpriv->bitmap_rates[i])
            rate_num++;

    LEAVE();
    if (rate_num > 1)
        return MTRUE;
    else
        return MFALSE;
}

/** 
 *  @brief Covert Rate Bitmap to Rate index
 *   
 *  @param pmadapter    Pointer to mlan_adapter structure
 *  @param rate_bitmap  Pointer to rate bitmap
 *  @param size         Size of the bitmap array
 *
 *  @return             Rate index
 */
int
wlan_get_rate_index(pmlan_adapter pmadapter, t_u16 * rate_bitmap, int size)
{
    int i;

    ENTER();

    for (i = 0; i < size * 8; i++) {
        if (rate_bitmap[i / 16] & (1 << (i % 16))) {
            LEAVE();
            return i;
        }
    }

    LEAVE();
    return 0;
}

/** 
 *  @brief Use rate to get the index
 *   
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param rate         Data rate
 *
 *  @return                     Index or 0 
 */
t_u8
wlan_data_rate_to_index(pmlan_adapter pmadapter, t_u32 rate)
{
    t_u16 *ptr;

    ENTER();
    if (rate)
        if ((ptr = wlan_memchr(pmadapter, WlanDataRates, (t_u8) rate,
                               sizeof(WlanDataRates)))) {
            LEAVE();
            return (ptr - WlanDataRates);
        }
    LEAVE();
    return 0;
}

/** 
 *  @brief Get supported data rates
 *   
 *  @param pmpriv           A pointer to mlan_private structure
 *  @param rates            The buf to return the supported rates
 *
 *  @return                 The number of Rates
 */
int
wlan_get_supported_rates(mlan_private * pmpriv, WLAN_802_11_RATES rates)
{
    int k = 0;
    mlan_adapter *pmadapter = pmpriv->adapter;

    ENTER();

    if (pmpriv->bss_mode == MLAN_BSS_MODE_INFRA) {
        /* Infra. mode */
        switch (pmadapter->config_bands) {
        case BAND_B:
            PRINTM(INFO, "Infra Band=%d SupportedRates_B\n",
                   pmadapter->config_bands);
            k = wlan_copy_rates(rates, k, SupportedRates_B,
                                sizeof(SupportedRates_B));
            break;
        case BAND_G:
        case BAND_G | BAND_GN:
            PRINTM(INFO, "Infra band=%d SupportedRates_G\n",
                   pmadapter->config_bands);
            k = wlan_copy_rates(rates, k, SupportedRates_G,
                                sizeof(SupportedRates_G));
            break;
        case BAND_B | BAND_G:
        case BAND_B | BAND_G | BAND_GN:
            PRINTM(INFO, "Infra band=%d SupportedRates_BG\n",
                   pmadapter->config_bands);
            k = wlan_copy_rates(rates, k, SupportedRates_BG,
                                sizeof(SupportedRates_BG));
            break;
        case BAND_GN:
            PRINTM(INFO, "Infra band=%d SupportedRates_N\n",
                   pmadapter->config_bands);
            k = wlan_copy_rates(rates, k, SupportedRates_N,
                                sizeof(SupportedRates_N));
            break;
        }
    } else {
        /* Ad-hoc mode */
        switch (pmadapter->adhoc_start_band) {
        case BAND_B:
            PRINTM(INFO, "Adhoc B\n");
            k = wlan_copy_rates(rates, k, AdhocRates_B, sizeof(AdhocRates_B));
            break;
        case BAND_G:
            PRINTM(INFO, "Adhoc G only\n");
            k = wlan_copy_rates(rates, k, AdhocRates_G, sizeof(AdhocRates_G));
            break;
        case BAND_B | BAND_G:
            PRINTM(INFO, "Adhoc BG\n");
            k = wlan_copy_rates(rates, k, AdhocRates_BG, sizeof(AdhocRates_BG));
            break;
        }
    }

    LEAVE();
    return k;
}

/** 
 *  @brief Get active data rates
 *   
 *  @param pmpriv           A pointer to mlan_private structure
 *  @param rates            The buf to return the active rates
 *
 *  @return                 The number of Rates
 */
int
wlan_get_active_data_rates(mlan_private * pmpriv, WLAN_802_11_RATES rates)
{
    int k;

    ENTER();
    if (pmpriv->media_connected != MTRUE) {
        k = wlan_get_supported_rates(pmpriv, rates);
    } else {
        k = wlan_copy_rates(rates, 0, pmpriv->curr_bss_params.data_rates,
                            pmpriv->curr_bss_params.num_of_rates);
    }

    LEAVE();
    return k;
}
