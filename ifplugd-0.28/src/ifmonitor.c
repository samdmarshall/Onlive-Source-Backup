/* $Id: ifmonitor.c 116 2004-12-19 00:24:29Z lennart $ */

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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <sys/socket.h>
#include <linux/types.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <linux/if.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <libdaemon/dlog.h>

#include "nlapi.h"

static int callback(struct nlmsghdr *n, void *u) {
    int (*cb)(int b, int index, unsigned short type, const char *name) = (int (*)(int b, int index, unsigned short type, const char *name)) u;
    
    if (n->nlmsg_type == RTM_NEWLINK || n->nlmsg_type == RTM_DELLINK) {
        struct rtattr *a;
        struct ifinfomsg *i;
        char ifname[IFNAMSIZ+1];
        int la;
        
        i = NLMSG_DATA(n);
        
        if (n->nlmsg_len < NLMSG_LENGTH(sizeof(struct ifinfomsg))) {
            daemon_log(LOG_ERR, "NETLINK: Packet too small or truncated! (2)\n");
            return -1;
        }

        memset(&ifname, 0, sizeof(ifname));
        
        a = (struct rtattr*) ((char*) i + NLMSG_ALIGN(sizeof(struct ifinfomsg)));
        la = NLMSG_PAYLOAD(n, sizeof(struct ifinfomsg));
                
        while (RTA_OK(a, la)) {
            
            if(a->rta_type == IFLA_IFNAME) {
                int l = RTA_PAYLOAD(a);
                if (l > IFNAMSIZ)
                    l = IFNAMSIZ;
                strncpy(ifname, RTA_DATA(a), l);
            }
                        
            a = RTA_NEXT(a, la);
        }

        if (cb(n->nlmsg_type == RTM_NEWLINK, i->ifi_index, i->ifi_type, ifname[0] ? ifname : NULL) < 0)
            return -1;
    }

    
    return 0;
}

int ifmonitor_init(int (*cb) (int b, int index, unsigned short type, const char *name)) {
    return nlapi_register(callback, (void*) cb);
}
