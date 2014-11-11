/* $Id: nlapi.c 116 2004-12-19 00:24:29Z lennart $ */

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
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <stdlib.h>

#include <libdaemon/dlog.h>

#include "nlapi.h"

int nlapi_fd = -1;

struct callback_info {
    nlapi_callback_t callback;
    void *userdata;
    struct callback_info * next;
};

struct callback_info *callbacks = NULL;

int nlapi_open(uint32_t groups) {
    struct sockaddr_nl addr;

    if ((nlapi_fd = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE)) < 0) {
        daemon_log(LOG_ERR, "socket(PF_NETLINK): %s\n", strerror(errno));
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.nl_family = AF_NETLINK;
    addr.nl_groups = groups;
    addr.nl_pid = getpid();

    if (bind(nlapi_fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        close(nlapi_fd);
        daemon_log(LOG_ERR, "bind(): %s\n", strerror(errno));
        return -1;
    }

    return 0;
}

int nlapi_work(int block) {

    assert(nlapi_fd >= 0);
    
    for (;;) {
        int bytes;
        char replybuf[1024];
        struct nlmsghdr *p = (struct nlmsghdr *) replybuf;
        
        if ((bytes = recv(nlapi_fd, &replybuf, sizeof(replybuf), block ? 0 : MSG_DONTWAIT)) < 0) {

            if (errno == EAGAIN || errno == EINTR)
                return 0;
            
            daemon_log(LOG_ERR, "NLAPI: recv(): %s\n", strerror(errno));
            return -1;
        }

        for (; bytes > 0; p = NLMSG_NEXT(p, bytes)) {
            struct callback_info *c;
            
            if (!NLMSG_OK(p, bytes) || (size_t) bytes < sizeof(struct nlmsghdr) || (size_t) bytes < p->nlmsg_len) {
                daemon_log(LOG_ERR, "NLAPI: Packet too small or truncated!\n");
                return -1;
            }
            
            for (c = callbacks; c; c = c->next)
                if (c->callback(p, c->userdata) < 0)
                    return -1;
        }

        if (block)
            break;
    }

    return 0;
}

void nlapi_close(void) {
    if (nlapi_fd >= 0)
        close(nlapi_fd);
    nlapi_fd = -1;

    while (callbacks) {
        struct callback_info *c = callbacks;
        callbacks = callbacks->next;
        free(c);
    }
}

int nlapi_register(int (*callback) (struct nlmsghdr *n, void *u), void *u) {
    struct callback_info *c;

    assert(callback);
    
    if (!(c = malloc(sizeof(struct callback_info)))) {
        daemon_log(LOG_ERR, "NLAPI: Not enough memory.\n");
        return -1;
    }

    c->callback = callback;
    c->userdata = u;

    c->next = callbacks;
    callbacks = c;
    return 0;
}
