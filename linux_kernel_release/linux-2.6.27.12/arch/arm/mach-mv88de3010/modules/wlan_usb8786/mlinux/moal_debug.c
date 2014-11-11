/** @file moal_debug.c
  *
  * @brief This file contains functions for debug proc file.
  *
  * Copyright (C) 2008-2009, Marvell International Ltd.
  * All Rights Reserved
  */

/********************************************************
Change log:
    11/03/2008: initial version
********************************************************/

#include	"moal_main.h"

/********************************************************
		Local Variables
********************************************************/
#ifdef CONFIG_PROC_FS
/** MLAN debug info */
static mlan_debug_info info;

/** Get info item size */
#define item_size(n) (sizeof(info.n))
/** Get info item address */
#define item_addr(n) ((t_u32) &(info.n))

/** Get moal_private member size */
#define item_priv_size(n) (sizeof ((moal_private *)0)->n)
/** Get moal_private member address */
#define item_priv_addr(n) ((t_u32) &((moal_private *)0)->n)

/** Get moal_handle member size */
#define item_handle_size(n) (sizeof ((moal_handle *)0)->n)
/** Get moal_handle member address */
#define item_handle_addr(n) ((t_u32) &((moal_handle *)0)->n)

/** Debug data */
struct debug_data
{
    /** Name */
    char name[32];
    /** Size */
    t_u32 size;
    /** Address */
    t_u32 addr;
};

/** Private debug data */
struct debug_data_priv
{
    /** moal_private handle */
    moal_private *priv;
    /** Debug items */
    struct debug_data *items;
};

static struct debug_data items[] = {
    {"int_counter", item_size(int_counter), item_addr(int_counter)},
    {"wmm_ac_vo", item_size(packets_out[WMM_AC_VO]),
     item_addr(packets_out[WMM_AC_VO])},
    {"wmm_ac_vi", item_size(packets_out[WMM_AC_VI]),
     item_addr(packets_out[WMM_AC_VI])},
    {"wmm_ac_be", item_size(packets_out[WMM_AC_BE]),
     item_addr(packets_out[WMM_AC_BE])},
    {"wmm_ac_bk", item_size(packets_out[WMM_AC_BK]),
     item_addr(packets_out[WMM_AC_BK])},
    {"max_tx_buf_size", item_size(max_tx_buf_size), item_addr(max_tx_buf_size)},
    {"tx_buf_size", item_size(tx_buf_size), item_addr(tx_buf_size)},
    {"ps_mode", item_size(ps_mode), item_addr(ps_mode)},
    {"ps_state", item_size(ps_state), item_addr(ps_state)},
    {"wakeup_dev_req", item_size(pm_wakeup_card_req),
     item_addr(pm_wakeup_card_req)},
    {"wakeup_tries", item_size(pm_wakeup_fw_try), item_addr(pm_wakeup_fw_try)},
    {"hs_configured", item_size(is_hs_configured), item_addr(is_hs_configured)},
    {"hs_activated", item_size(hs_activated), item_addr(hs_activated)},
    {"num_tx_timeout", item_size(num_tx_timeout), item_addr(num_tx_timeout)},
    {"num_cmd_timeout", item_size(num_cmd_timeout), item_addr(num_cmd_timeout)},
    {"timeout_cmd_id", item_size(timeout_cmd_id), item_addr(timeout_cmd_id)},
    {"timeout_cmd_act", item_size(timeout_cmd_act), item_addr(timeout_cmd_act)},
    {"last_cmd_id", item_size(last_cmd_id), item_addr(last_cmd_id)},
    {"last_cmd_act", item_size(last_cmd_act), item_addr(last_cmd_act)},
    {"last_cmd_index", item_size(last_cmd_index), item_addr(last_cmd_index)},
    {"last_cmd_resp_id", item_size(last_cmd_resp_id),
     item_addr(last_cmd_resp_id)},
    {"last_cmd_resp_index", item_size(last_cmd_resp_index),
     item_addr(last_cmd_resp_index)},
    {"last_event", item_size(last_event), item_addr(last_event)},
    {"last_event_index", item_size(last_event_index),
     item_addr(last_event_index)},
    {"num_cmd_h2c_fail", item_size(num_cmd_host_to_card_failure),
     item_addr(num_cmd_host_to_card_failure)},
    {"num_cmd_sleep_cfm_fail",
     item_size(num_cmd_sleep_cfm_host_to_card_failure),
     item_addr(num_cmd_sleep_cfm_host_to_card_failure)},
    {"num_tx_h2c_fail", item_size(num_tx_host_to_card_failure),
     item_addr(num_tx_host_to_card_failure)},
    {"num_evt_deauth", item_size(num_event_deauth),
     item_addr(num_event_deauth)},
    {"num_evt_disassoc", item_size(num_event_disassoc),
     item_addr(num_event_disassoc)},
    {"num_evt_link_lost", item_size(num_event_link_lost),
     item_addr(num_event_link_lost)},
    {"num_cmd_deauth", item_size(num_cmd_deauth), item_addr(num_cmd_deauth)},
    {"num_cmd_assoc_ok", item_size(num_cmd_assoc_success),
     item_addr(num_cmd_assoc_success)},
    {"num_cmd_assoc_fail", item_size(num_cmd_assoc_failure),
     item_addr(num_cmd_assoc_failure)},

    {"cmd_sent", item_size(cmd_sent), item_addr(cmd_sent)},
    {"data_sent", item_size(data_sent), item_addr(data_sent)},
    {"cmd_resp_received", item_size(cmd_resp_received),
     item_addr(cmd_resp_received)},
    {"event_received", item_size(event_received), item_addr(event_received)},

    {"ioctl_pending", item_priv_size(ioctl_pending),
     item_priv_addr(ioctl_pending)},
    {"tx_pending", item_priv_size(tx_pending), item_priv_addr(tx_pending)},
    {"rx_pending", item_handle_size(rx_pending), item_handle_addr(rx_pending)},
    {"malloc_count", item_handle_size(malloc_count),
     item_handle_addr(malloc_count)},
    {"lock_count", item_handle_size(lock_count), item_handle_addr(lock_count)},
};

/** Number of items in debug data table */
static int num_of_items = sizeof(items) / sizeof(items[0]);

/** Private debug data */
static struct debug_data_priv items_priv;

/********************************************************
		Global Variables
********************************************************/

/********************************************************
		Local Functions
********************************************************/
/** 
 *  @brief Convert string to number
 *
 *  @param s   	   Pointer to numbered string
 *
 *  @return 	   Converted number from string s
 */
int
woal_string_to_number(char *s)
{
    int r = 0;
    int base = 0;
    int pn = 1;

    if (!strncmp(s, "-", 1)) {
        pn = -1;
        s++;
    }
    if (!strncmp(s, "0x", 2) || !strncmp(s, "0X", 2)) {
        base = 16;
        s += 2;
    } else
        base = 10;

    for (s = s; *s; s++) {
        if ((*s >= '0') && (*s <= '9'))
            r = (r * base) + (*s - '0');
        else if ((*s >= 'A') && (*s <= 'F'))
            r = (r * base) + (*s - 'A' + 10);
        else if ((*s >= 'a') && (*s <= 'f'))
            r = (r * base) + (*s - 'a' + 10);
        else
            break;
    }

    return (r * pn);
}

/**
 *  @brief Proc read function
 *
 *  @param page	   Pointer to buffer
 *  @param s       Read data starting position
 *  @param off     Offset
 *  @param cnt     Counter
 *  @param eof     End of file flag
 *  @param data    Data to output
 *
 *  @return 	   Number of output data
 */
static int
woal_debug_read(char *page, char **s, off_t off, int cnt, int *eof, void *data)
{
    int val = 0;
    int i;
    char *p = page;
    struct debug_data *d = ((struct debug_data_priv *) data)->items;
    moal_private *priv = ((struct debug_data_priv *) data)->priv;

    if (MODULE_GET == 0)
        return MLAN_STATUS_FAILURE;

    /* Get debug information */
    if (woal_get_debug_info(priv, MOAL_PROC_WAIT, &info)) {
        *eof = 1;
        goto exit;
    }
    for (i = 0; i < num_of_items; i++) {
        if (d[i].size == 1)
            val = *((t_u8 *) d[i].addr);
        else if (d[i].size == 2)
            val = *((t_u16 *) d[i].addr);
        else if (d[i].size == 4)
            val = *((t_u32 *) d[i].addr);
        else {
            int j;
            p += sprintf(p, "%s=", d[i].name);
            for (j = 0; j < d[i].size; j += 2) {
                val = *(t_u16 *) (d[i].addr + j);
                p += sprintf(p, "0x%x ", val);
            }
            p += sprintf(p, "\n");
            continue;
        }
        if (strstr(d[i].name, "id"))
            p += sprintf(p, "%s=0x%x\n", d[i].name, val);
        else
            p += sprintf(p, "%s=%d\n", d[i].name, val);
    }
    if (info.tx_tbl_num)
        p += sprintf(p, "Tx BA stream table:\n");
    for (i = 0; i < info.tx_tbl_num; i++) {
        p += sprintf(p, "Tid = %d, RA = %02x:%02x:%02x:%02x:%02x:%02x\n",
                     (int) info.tx_tbl[i].tid, info.tx_tbl[i].ra[0],
                     info.tx_tbl[i].ra[1], info.tx_tbl[i].ra[2],
                     info.tx_tbl[i].ra[3], info.tx_tbl[i].ra[4],
                     info.tx_tbl[i].ra[5]);
    }
    if (info.rx_tbl_num)
        p += sprintf(p, "Rx reorder table:\n");
    for (i = 0; i < info.rx_tbl_num; i++) {
        int j;

        p += sprintf(p,
                     "Tid = %d, TA =  %02x:%02x:%02x:%02x:%02x:%02x, StartWin = %d, "
                     "WinSize = %d, Buffer: ", (int) info.rx_tbl[i].tid,
                     info.rx_tbl[i].ta[0], info.rx_tbl[i].ta[1],
                     info.rx_tbl[i].ta[2], info.rx_tbl[i].ta[3],
                     info.rx_tbl[i].ta[4], info.rx_tbl[i].ta[5],
                     (int) info.rx_tbl[i].start_win,
                     (int) info.rx_tbl[i].win_size);
        for (j = 0; j < info.rx_tbl[i].win_size; j++) {
            if (info.rx_tbl[i].buffer[j] == MTRUE)
                p += sprintf(p, "1 ");
            else
                p += sprintf(p, "0 ");
        }
        p += sprintf(p, "\n");
    }
  exit:
    MODULE_PUT;
    return p - page;
}

/**
 *  @brief Proc write function
 *
 *  @param f	   File pointer
 *  @param buf     Pointer to data buffer
 *  @param cnt     Data number to write
 *  @param data    Data to write
 *
 *  @return 	   Number of data
 */
static int
woal_debug_write(struct file *f, const char *buf, unsigned long cnt, void *data)
{
    int r, i;
    char *pdata;
    char *p;
    char *p0;
    char *p1;
    char *p2;
    struct debug_data *d = ((struct debug_data_priv *) data)->items;
    moal_private *priv = ((struct debug_data_priv *) data)->priv;

    if (MODULE_GET == 0)
        return MLAN_STATUS_FAILURE;

    pdata = (char *) kmalloc(cnt, GFP_ATOMIC);
    if (pdata == NULL) {
        MODULE_PUT;
        return 0;
    }

    if (copy_from_user(pdata, buf, cnt)) {
        PRINTM(ERROR, "Copy from user failed\n");
        kfree(pdata);
        MODULE_PUT;
        return 0;
    }

    p0 = pdata;
    for (i = 0; i < num_of_items; i++) {
        do {
            p = strstr(p0, d[i].name);
            if (p == NULL)
                break;
            p1 = strchr(p, '\n');
            if (p1 == NULL)
                break;
            p0 = p1++;
            p2 = strchr(p, '=');
            if (!p2)
                break;
            p2++;
            r = woal_string_to_number(p2);
            if (d[i].size == 1)
                *((t_u8 *) d[i].addr) = (t_u8) r;
            else if (d[i].size == 2)
                *((t_u16 *) d[i].addr) = (t_u16) r;
            else if (d[i].size == 4)
                *((t_u32 *) d[i].addr) = (t_u32) r;
            break;
        } while (MTRUE);
    }
    kfree(pdata);

    /* Set debug information */
    if (woal_set_debug_info(priv, MOAL_IOCTL_WAIT, &info)) {
        MODULE_PUT;
        return 0;
    }

    MODULE_PUT;
    return cnt;
}

/********************************************************
		Global Functions
********************************************************/
/**
 *  @brief Create debug proc file
 *
 *  @param priv	   Pointer moal_private
 *
 *  @return 	   N/A
 */
void
woal_debug_entry(moal_private * priv)
{
    struct proc_dir_entry *r;

    ENTER();

    if (priv->proc_entry == NULL)
        return;

    items_priv.priv = priv;
    items_priv.items = items;

    if (woal_get_debug_info(priv, MOAL_PROC_WAIT, &info)) {
        LEAVE();
        return;
    }

    /* Setup MOAL debug info */
    items[num_of_items - 1].addr += (t_u32) (priv->phandle);
    items[num_of_items - 2].addr += (t_u32) (priv->phandle);
    items[num_of_items - 3].addr += (t_u32) (priv->phandle);
    items[num_of_items - 4].addr += (t_u32) priv;
    items[num_of_items - 5].addr += (t_u32) priv;

    /* Create proc entry */
    r = create_proc_entry("debug", 0644, priv->proc_entry);
    if (r == NULL) {
        LEAVE();
        return;
    }

    /* Populate proc entry handlers */
    r->data = &items_priv;
    r->read_proc = woal_debug_read;
    r->write_proc = woal_debug_write;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,30)
    r->owner = THIS_MODULE;
#endif

    LEAVE();
}

/**
 *  @brief Remove proc file
 *
 *  @param priv	   Pointer moal_private
 *
 *  @return 	   N/A
 */
void
woal_debug_remove(moal_private * priv)
{
    ENTER();

    /* Reset MOAL debug info */
    items[num_of_items - 1].addr -= (t_u32) (priv->phandle);
    items[num_of_items - 2].addr -= (t_u32) (priv->phandle);
    items[num_of_items - 3].addr -= (t_u32) (priv->phandle);
    items[num_of_items - 4].addr -= (t_u32) priv;
    items[num_of_items - 5].addr -= (t_u32) priv;

    /* Remove proc entry */
    remove_proc_entry("debug", priv->proc_entry);

    LEAVE();
}
#endif
