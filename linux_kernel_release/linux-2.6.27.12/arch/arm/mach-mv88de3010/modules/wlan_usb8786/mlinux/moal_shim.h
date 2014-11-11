/** @file moal_shim.h
  *
  * @brief This file contains declaration referring to
  * functions defined in moal module
  *  
  * Copyright (C) 2008-2009, Marvell International Ltd.  
  * All Rights Reserved
  */
/*************************************************************
Change Log:
    10/21/2008: initial version
************************************************************/

#ifndef _MOAL_H
#define _MOAL_H
mlan_status moal_init_fw_complete(IN t_void * pmoal_handle,
                                  IN mlan_status status);
mlan_status moal_shutdown_fw_complete(IN t_void * pmoal_handle,
                                      IN mlan_status status);
mlan_status moal_ioctl_complete(IN t_void * pmoal_handle,
                                IN pmlan_ioctl_req pioctl_req,
                                IN mlan_status status);
mlan_status moal_send_packet_complete(IN t_void * pmoal_handle,
                                      IN pmlan_buffer pmbuf,
                                      IN mlan_status status);
mlan_status moal_recv_complete(IN t_void * pmoal_handle, IN pmlan_buffer pmbuf,
                               IN t_u32 port, IN mlan_status status);
mlan_status moal_write_data_async(IN t_void * pmoal_handle,
                                  IN pmlan_buffer pmbuf, IN t_u32 port);
mlan_status moal_write_data_sync(IN t_void * pmoal_handle,
                                 IN pmlan_buffer pmbuf, IN t_u32 port,
                                 IN t_u32 timeout);
mlan_status moal_read_data_sync(IN t_void * pmoal_handle,
                                IN OUT pmlan_buffer pmbuf, IN t_u32 port,
                                IN t_u32 timeout);
mlan_status moal_recv_packet(IN t_void * pmoal_handle, IN pmlan_buffer pmbuf);
mlan_status moal_recv_event(IN t_void * pmoal_handle, IN pmlan_event pmevent);
mlan_status moal_malloc(IN t_u32 size, OUT t_u8 ** ppbuf);
mlan_status moal_mfree(IN t_u8 * pbuf);
t_void *moal_memset(IN t_void * pmem, IN t_u8 byte, IN t_u32 num);
t_void *moal_memcpy(IN t_void * pdest, IN const t_void * psrc, IN t_u32 num);
t_void *moal_memmove(IN t_void * pdest, IN const t_void * psrc, IN t_u32 num);
t_s32 moal_memcmp(IN const t_void * pmem1,
                  IN const t_void * pmem2, IN t_u32 num);
mlan_status moal_get_system_time(OUT t_u32 * psec, OUT t_u32 * pusec);
mlan_status moal_init_lock(OUT t_void ** pplock);
mlan_status moal_free_lock(IN t_void * plock);
mlan_status moal_spin_lock(IN t_void * plock);
mlan_status moal_spin_unlock(IN t_void * plock);
t_void moal_print(IN t_u32 level, IN t_s8 * pformat, IN ...);
mlan_status moal_init_timer(OUT t_void ** pptimer,
                            IN t_void(*callback) (t_void * pcontext),
                            IN t_void * pcontext);
mlan_status moal_free_timer(IN t_void * ptimer);
mlan_status moal_start_timer(IN t_void * ptimer,
                             IN t_u8 periodic, IN t_u32 msec);
mlan_status moal_stop_timer(IN t_void * ptimer);
#endif /*_MOAL_H */
