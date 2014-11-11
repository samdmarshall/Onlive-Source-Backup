/**
 * @file mlan_cfp.c
 *
 *  @brief This file contains WLAN client mode channel, frequency and power
 *  related code
 *
 *  Copyright (C) 2009-2010, Marvell International Ltd. 
 *  All Rights Reserved
 *   
 */

/*************************************************************
Change Log:
    04/16/2009: initial version
************************************************************/

#include "mlan.h"
#include "mlan_util.h"
#include "mlan_fw.h"
#include "mlan_join.h"
#include "mlan_main.h"

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

/** Band: 'A', Region: USA FCC, Canada IC, Spain, France */
static chan_freq_power_t channel_freq_power_A[] = {
    {36, 5180, WLAN_TX_PWR_US_DEFAULT},
    {40, 5200, WLAN_TX_PWR_US_DEFAULT},
    {44, 5220, WLAN_TX_PWR_US_DEFAULT},
    {48, 5240, WLAN_TX_PWR_US_DEFAULT},
    {52, 5260, WLAN_TX_PWR_US_DEFAULT},
    {56, 5280, WLAN_TX_PWR_US_DEFAULT},
    {60, 5300, WLAN_TX_PWR_US_DEFAULT},
    {64, 5320, WLAN_TX_PWR_US_DEFAULT},
    {100, 5500, WLAN_TX_PWR_US_DEFAULT},
    {104, 5520, WLAN_TX_PWR_US_DEFAULT},
    {108, 5540, WLAN_TX_PWR_US_DEFAULT},
    {112, 5560, WLAN_TX_PWR_US_DEFAULT},
    {116, 5580, WLAN_TX_PWR_US_DEFAULT},
    {120, 5600, WLAN_TX_PWR_US_DEFAULT},
    {124, 5620, WLAN_TX_PWR_US_DEFAULT},
    {128, 5640, WLAN_TX_PWR_US_DEFAULT},
    {132, 5660, WLAN_TX_PWR_US_DEFAULT},
    {136, 5680, WLAN_TX_PWR_US_DEFAULT},
    {140, 5700, WLAN_TX_PWR_US_DEFAULT},
    {149, 5745, WLAN_TX_PWR_US_DEFAULT},
    {153, 5765, WLAN_TX_PWR_US_DEFAULT},
    {157, 5785, WLAN_TX_PWR_US_DEFAULT},
    {161, 5805, WLAN_TX_PWR_US_DEFAULT},
    {165, 5825, WLAN_TX_PWR_US_DEFAULT}
};

/** Band: 'A', Region: Europe ETSI */
static chan_freq_power_t channel_freq_power_EU_A[] = {
    {36, 5180, WLAN_TX_PWR_EMEA_DEFAULT},
    {40, 5200, WLAN_TX_PWR_EMEA_DEFAULT},
    {44, 5220, WLAN_TX_PWR_EMEA_DEFAULT},
    {48, 5240, WLAN_TX_PWR_EMEA_DEFAULT},
    {52, 5260, WLAN_TX_PWR_EMEA_DEFAULT},
    {56, 5280, WLAN_TX_PWR_EMEA_DEFAULT},
    {60, 5300, WLAN_TX_PWR_EMEA_DEFAULT},
    {64, 5320, WLAN_TX_PWR_EMEA_DEFAULT},
    {100, 5500, WLAN_TX_PWR_EMEA_DEFAULT},
    {104, 5520, WLAN_TX_PWR_EMEA_DEFAULT},
    {108, 5540, WLAN_TX_PWR_EMEA_DEFAULT},
    {112, 5560, WLAN_TX_PWR_EMEA_DEFAULT},
    {116, 5580, WLAN_TX_PWR_EMEA_DEFAULT},
    {120, 5600, WLAN_TX_PWR_EMEA_DEFAULT},
    {124, 5620, WLAN_TX_PWR_EMEA_DEFAULT},
    {128, 5640, WLAN_TX_PWR_EMEA_DEFAULT},
    {132, 5660, WLAN_TX_PWR_EMEA_DEFAULT},
    {136, 5680, WLAN_TX_PWR_EMEA_DEFAULT},
    {140, 5700, WLAN_TX_PWR_EMEA_DEFAULT}
};

/** Band: 'A', Region: Japan */
static chan_freq_power_t channel_freq_power_JPN_A[] = {
    {8, 5040, WLAN_TX_PWR_JP_DEFAULT},
    {12, 5060, WLAN_TX_PWR_JP_DEFAULT},
    {16, 5080, WLAN_TX_PWR_JP_DEFAULT},
    {34, 5170, WLAN_TX_PWR_JP_DEFAULT},
    {38, 5190, WLAN_TX_PWR_JP_DEFAULT},
    {42, 5210, WLAN_TX_PWR_JP_DEFAULT},
    {46, 5230, WLAN_TX_PWR_JP_DEFAULT},
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
    /** Frequency/Power for band A */
    chan_freq_power_t *cfp_A;
    /** No A flag */
    int cfp_no_A;
} region_cfp_table_t;

/**
 * The structure for the mapping between region and CFP
 */
static region_cfp_table_t region_cfp_table[] = {
    {0x10,                      /* US FCC */
     channel_freq_power_US_BG,
     sizeof(channel_freq_power_US_BG) / sizeof(chan_freq_power_t),
     channel_freq_power_A,
     sizeof(channel_freq_power_A) / sizeof(chan_freq_power_t),
     }
    ,
    {0x20,                      /* CANADA IC */
     channel_freq_power_US_BG,
     sizeof(channel_freq_power_US_BG) / sizeof(chan_freq_power_t),
     channel_freq_power_A,
     sizeof(channel_freq_power_A) / sizeof(chan_freq_power_t),
     }
    ,
    {0x30,                      /* EU */
     channel_freq_power_EU_BG,
     sizeof(channel_freq_power_EU_BG) / sizeof(chan_freq_power_t),
     channel_freq_power_EU_A,
     sizeof(channel_freq_power_EU_A) / sizeof(chan_freq_power_t),
     }
    ,
    {0x32,                      /* FRANCE */
     channel_freq_power_FR_BG,
     sizeof(channel_freq_power_FR_BG) / sizeof(chan_freq_power_t),
     channel_freq_power_A,
     sizeof(channel_freq_power_A) / sizeof(chan_freq_power_t),
     }
    ,
    {0x40,                      /* JAPAN */
     channel_freq_power_JPN40_BG,
     sizeof(channel_freq_power_JPN40_BG) / sizeof(chan_freq_power_t),
     channel_freq_power_JPN_A,
     sizeof(channel_freq_power_JPN_A) / sizeof(chan_freq_power_t),
     }
    ,
    {0x41,                      /* JAPAN */
     channel_freq_power_JPN41_BG,
     sizeof(channel_freq_power_JPN41_BG) / sizeof(chan_freq_power_t),
     channel_freq_power_JPN_A,
     sizeof(channel_freq_power_JPN_A) / sizeof(chan_freq_power_t),
     }
    ,
    {0xff,                      /* Special */
     channel_freq_power_SPECIAL_BG,
     sizeof(channel_freq_power_SPECIAL_BG) / sizeof(chan_freq_power_t),
     channel_freq_power_JPN_A,
     sizeof(channel_freq_power_JPN_A) / sizeof(chan_freq_power_t),
     }
    ,
/* Add new region here */
};

/********************************************************
    Global Variables
********************************************************/
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

/**
 * The rates supported in A mode for ad-hoc
 */
t_u8 AdhocRates_A[A_SUPPORTED_RATES] =
    { 0x8c, 0x12, 0x98, 0x24, 0xb0, 0x48, 0x60, 0x6c, 0 };
/**
 * The rates supported in A mode (used for BAND_A)
 */
t_u8 SupportedRates_A[A_SUPPORTED_RATES] =
    { 0x0c, 0x12, 0x18, 0x24, 0xb0, 0x48, 0x60, 0x6c, 0 };
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
 * The table to keep region code
 */
t_u16 region_code_index[MRVDRV_MAX_REGION_CODE] =
    { 0x10, 0x20, 0x30, 0x32, 0x40, 0x41, 0xff };

/**
 * The rates supported in N mode
 */
t_u8 SupportedRates_N[N_SUPPORTED_RATES] = { 0x02, 0x04, 0 };

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
    const t_u8 *p = (t_u8 *) s;

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
        if (index == MLAN_RATE_BITMAP_MCS0) {
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
            return (t_u8) (ptr - WlanDataRates);
        }
    LEAVE();
    return 0;
}

/** 
 *  @brief Get active data rates
 *   
 *  @param pmpriv           A pointer to mlan_private structure
 *  @param rates            The buf to return the active rates
 *
 *  @return                 The number of Rates
 */
t_u32
wlan_get_active_data_rates(mlan_private * pmpriv, WLAN_802_11_RATES rates)
{
    t_u32 k;

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
    t_u32 i;

    ENTER();

    for (i = 0; i < sizeof(region_cfp_table) / sizeof(region_cfp_table_t); i++) {
        PRINTM(MINFO, "region_cfp_table[i].region=%d\n",
               region_cfp_table[i].region);
        /* Check if region matches */
        if (region_cfp_table[i].region == region) {
            /* Select by band */
            if (band & (BAND_B | BAND_G | BAND_GN)) {
                *cfp_no = region_cfp_table[i].cfp_no_BG;
                LEAVE();
                return region_cfp_table[i].cfp_BG;
            } else if (band & (BAND_A | BAND_AN)) {
                *cfp_no = region_cfp_table[i].cfp_no_A;
                LEAVE();
                return region_cfp_table[i].cfp_A;
            } else {
                PRINTM(MERROR, "Error Band[0x%x]\n", band);
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
    chan_freq_power_t *cfp_a = MNULL;
    t_u32 cfp_no_a;

    ENTER();

    for (i = 0; i < NELEMENTS(region_cfp_table); i++) {
        /* Get CFP */
        cfp = region_cfp_table[i].cfp_BG;
        cfp_no = region_cfp_table[i].cfp_no_BG;

        cfp_a = region_cfp_table[i].cfp_A;
        cfp_no_a = region_cfp_table[i].cfp_no_A;
        /* Find matching channel and get Tx power */
        for (j = 0; j < cfp_no; j++) {
            if ((cfp + j)->channel == channel) {
                if (tx_power != 0)
                    tx_power = MIN(tx_power, (cfp + j)->max_tx_power);
                else
                    tx_power = (t_u8) (cfp + j)->max_tx_power;
                break;
            }
        }

        for (j = 0; j < cfp_no_a; j++) {
            if ((cfp_a + j)->channel == channel) {
                if (tx_power != 0)
                    tx_power = MIN(tx_power, (cfp_a + j)->max_tx_power);
                else
                    tx_power = (cfp_a + j)->max_tx_power;
                break;
            }
        }
    }

    LEAVE();
    return tx_power;
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
        case BAND_A:
            switch (band) {
            case BAND_AN:
            case BAND_A | BAND_AN:
            case BAND_A:       /* Matching BAND_A */
                break;

            default:
                continue;
            }
            break;
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
        PRINTM(MERROR, "wlan_get_cfp_by_band_and_channel(): cannot find "
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
        case BAND_A:
            switch (band) {
            case BAND_AN:
            case BAND_A | BAND_AN:
            case BAND_A:       /* Matching BAND_A */
                break;
            default:
                continue;
            }
            break;
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
        PRINTM(MERROR, "find_cfp_by_band_and_freql(): cannot find cfp by "
               "band %d & freq %d\n", band, freq);

    LEAVE();
    return cfp;
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
    t_u32 i;
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
 *  @brief Get supported data rates
 *   
 *  @param pmpriv           A pointer to mlan_private structure
 *  @param rates            The buf to return the supported rates
 *
 *  @return                 The number of Rates
 */
t_u32
wlan_get_supported_rates(mlan_private * pmpriv, WLAN_802_11_RATES rates)
{
    t_u32 k = 0;
    mlan_adapter *pmadapter = pmpriv->adapter;

    ENTER();

    if (pmpriv->bss_mode == MLAN_BSS_MODE_INFRA) {
        /* Infra. mode */
        switch (pmadapter->config_bands) {
        case BAND_B:
            PRINTM(MINFO, "Infra Band=%d SupportedRates_B\n",
                   pmadapter->config_bands);
            k = wlan_copy_rates(rates, k, SupportedRates_B,
                                sizeof(SupportedRates_B));
            break;
        case BAND_G:
        case BAND_G | BAND_GN:
            PRINTM(MINFO, "Infra band=%d SupportedRates_G\n",
                   pmadapter->config_bands);
            k = wlan_copy_rates(rates, k, SupportedRates_G,
                                sizeof(SupportedRates_G));
            break;
        case BAND_B | BAND_G:
        case BAND_A | BAND_B | BAND_G:
        case BAND_A | BAND_B:
        case BAND_A | BAND_B | BAND_G | BAND_GN | BAND_AN:
        case BAND_B | BAND_G | BAND_GN:
            PRINTM(MINFO, "Infra band=%d SupportedRates_BG\n",
                   pmadapter->config_bands);
            k = wlan_copy_rates(rates, k, SupportedRates_BG,
                                sizeof(SupportedRates_BG));
            break;
        case BAND_A:
        case BAND_A | BAND_G:
            PRINTM(MINFO, "Infra band=%d SupportedRates_A\n",
                   pmadapter->config_bands);
            k = wlan_copy_rates(rates, k, SupportedRates_A,
                                sizeof(SupportedRates_A));
            break;
        case BAND_A | BAND_AN:
        case BAND_A | BAND_G | BAND_AN | BAND_GN:
            PRINTM(MINFO, "Infra band=%d SupportedRates_A\n",
                   pmadapter->config_bands);
            k = wlan_copy_rates(rates, k, SupportedRates_A,
                                sizeof(SupportedRates_A));
            break;
        case BAND_GN:
            PRINTM(MINFO, "Infra band=%d SupportedRates_N\n",
                   pmadapter->config_bands);
            k = wlan_copy_rates(rates, k, SupportedRates_N,
                                sizeof(SupportedRates_N));
            break;
        }
    } else {
        /* Ad-hoc mode */
        switch (pmadapter->adhoc_start_band) {
        case BAND_B:
            PRINTM(MINFO, "Band: Adhoc B\n");
            k = wlan_copy_rates(rates, k, AdhocRates_B, sizeof(AdhocRates_B));
            break;
        case BAND_G:
        case BAND_G | BAND_GN:
            PRINTM(MINFO, "Band: Adhoc G only\n");
            k = wlan_copy_rates(rates, k, AdhocRates_G, sizeof(AdhocRates_G));
            break;
        case BAND_B | BAND_G:
        case BAND_B | BAND_G | BAND_GN:
            PRINTM(MINFO, "Band: Adhoc BG\n");
            k = wlan_copy_rates(rates, k, AdhocRates_BG, sizeof(AdhocRates_BG));
            break;
        case BAND_A:
        case BAND_A | BAND_AN:
            PRINTM(MINFO, "Band: Adhoc A\n");
            k = wlan_copy_rates(rates, k, AdhocRates_A, sizeof(AdhocRates_A));
            break;
        }
    }

    LEAVE();
    return k;
}

/** 
 *  @brief This function sets region table. 
 *  
 *  @param pmpriv  A pointer to mlan_private structure
 *  @param region  The region code
 *  @param band    The band
 *
 *  @return        MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
wlan_set_regiontable(mlan_private * pmpriv, t_u8 region, t_u8 band)
{
    mlan_adapter *pmadapter = pmpriv->adapter;
    int i = 0;
    chan_freq_power_t *cfp;
    int cfp_no;

    ENTER();

    memset(pmadapter, pmadapter->region_channel, 0,
           sizeof(pmadapter->region_channel));

    if (band & (BAND_B | BAND_G | BAND_GN)) {
        cfp =
            wlan_get_region_cfp_table(pmadapter, region,
                                      BAND_G | BAND_B | BAND_GN, &cfp_no);
        if (cfp) {
            pmadapter->region_channel[i].num_cfp = (t_u8) cfp_no;
            pmadapter->region_channel[i].pcfp = cfp;
        } else {
            PRINTM(MERROR, "wrong region code %#x in Band B-G\n", region);
            LEAVE();
            return MLAN_STATUS_FAILURE;
        }
        pmadapter->region_channel[i].valid = MTRUE;
        pmadapter->region_channel[i].region = region;
        if (band & BAND_GN)
            pmadapter->region_channel[i].band = BAND_G;
        else
            pmadapter->region_channel[i].band =
                (band & BAND_G) ? BAND_G : BAND_B;
        i++;
    }
    if (band & (BAND_A | BAND_AN)) {
        cfp = wlan_get_region_cfp_table(pmadapter, region, BAND_A, &cfp_no);
        if (cfp) {
            pmadapter->region_channel[i].num_cfp = cfp_no;
            pmadapter->region_channel[i].pcfp = cfp;
        } else {
            PRINTM(MERROR, "wrong region code %#x in Band A\n", region);
            LEAVE();
            return MLAN_STATUS_FAILURE;
        }
        pmadapter->region_channel[i].valid = MTRUE;
        pmadapter->region_channel[i].region = region;
        pmadapter->region_channel[i].band = BAND_A;
    }
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}
