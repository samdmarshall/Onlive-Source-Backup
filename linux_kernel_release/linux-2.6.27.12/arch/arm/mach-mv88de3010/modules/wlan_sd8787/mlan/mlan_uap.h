/** @file mlan_uap.h
 *
 *  @brief This file contains related macros, enum, and struct
 *  of uap functionalities
 *
 *  Copyright (C) 2009-2010, Marvell International Ltd. 
 *  All Rights Reserved
 */

/********************************************************
Change log:
    02/05/2009: initial version
********************************************************/

#ifndef _MLAN_UAP_H_
#define _MLAN_UAP_H_

/** Convert TxPD to little endian format from CPU format */
#define uap_endian_convert_TxPD(x);                                         \
    {                                                                   \
        (x)->tx_pkt_length = wlan_cpu_to_le16((x)->tx_pkt_length);      \
        (x)->tx_pkt_offset = wlan_cpu_to_le16((x)->tx_pkt_offset);      \
        (x)->tx_pkt_type = wlan_cpu_to_le16((x)->tx_pkt_type);      \
        (x)->tx_control = wlan_cpu_to_le32((x)->tx_control);      \
    }

/** Convert RxPD from little endian format to CPU format */
#define uap_endian_convert_RxPD(x);                                         \
    {                                                                   \
        (x)->rx_pkt_length = wlan_le16_to_cpu((x)->rx_pkt_length);      \
        (x)->rx_pkt_offset = wlan_le16_to_cpu((x)->rx_pkt_offset);      \
        (x)->rx_pkt_type = wlan_le16_to_cpu((x)->rx_pkt_type);      \
        (x)->seq_num = wlan_le16_to_cpu((x)->seq_num);              \
    }

mlan_status mlan_uap_ioctl(t_void * adapter, pmlan_ioctl_req pioctl_req);

mlan_status mlan_uap_prepare_cmd(IN t_void * priv,
                                 IN t_u16 cmd_no,
                                 IN t_u16 cmd_action,
                                 IN t_u32 cmd_oid,
                                 IN t_void * pioctl_buf,
                                 IN t_void * pdata_buf, IN t_void * pcmd_buf);

mlan_status mlan_process_uap_cmdresp(IN t_void * priv,
                                     IN t_u16 cmdresp_no,
                                     IN t_void * pcmd_buf, IN t_void * pioctl);

mlan_status mlan_process_uap_rx_packet(IN t_void * adapter,
                                       IN pmlan_buffer pmbuf);

mlan_status mlan_process_uap_event(IN t_void * priv);

t_void *mlan_process_uap_txpd(IN t_void * priv, IN pmlan_buffer pmbuf);

mlan_status mlan_uap_init_cmd(IN t_void * priv, IN t_u8 first_sta);

#endif /* _MLAN_UAP_H_ */
