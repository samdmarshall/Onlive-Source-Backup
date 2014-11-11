#ifndef foonlapihfoo
#define foonlapihfoo

/* $Id: nlapi.h 31 2003-08-11 16:19:52Z lennart $ */

/*
 * This file is part of ifplugd.
 *
 * ifplugd is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * ifplugd is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ifplugd; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 */

#include <stdint.h>
#include <sys/socket.h>
#include <linux/types.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>

typedef int (*nlapi_callback_t) (struct nlmsghdr *n, void *u);

extern int nlapi_fd;

int nlapi_open(uint32_t groups);
void nlapi_close(void);
int nlapi_work(int block);
int nlapi_register(nlapi_callback_t cb, void *u);

#endif
