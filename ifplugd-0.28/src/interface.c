/* $Id: interface.c 114 2004-12-19 00:08:01Z lennart $ */

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

#include <linux/sockios.h>
#include <linux/if_ether.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>

#include "ethtool-local.h"
#include "interface.h"
#include "wireless.h"

#include <libdaemon/dlog.h>

void interface_up(int fd, char *iface) {
    struct ifreq ifr;

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, iface, sizeof(ifr.ifr_name)-1);
    
    if (ioctl(fd, SIOCGIFFLAGS, &ifr) < 0) {
        if (interface_do_message)
            daemon_log(LOG_WARNING, "Warning: Could not get interface flags.");
        
        return;
    }

    if ((ifr.ifr_flags & IFF_UP) == IFF_UP)
        return;
    
    if (ioctl(fd, SIOCGIFADDR, &ifr) < 0) {
        if (interface_do_message)
            daemon_log(LOG_WARNING, "Warning: Could not get interface address.");
    } else if (ifr.ifr_addr.sa_family != AF_INET) {
        if (interface_do_message)
            daemon_log(LOG_WARNING, "Warning: The interface is not IP-based.");
    } else {
        ((struct sockaddr_in *)(&ifr.ifr_addr))->sin_addr.s_addr = INADDR_ANY;
        if (ioctl(fd, SIOCSIFADDR, &ifr) < 0) {
            if (interface_do_message)
                daemon_log(LOG_WARNING, "Warning: Could not set interface address.");
        }
    }

    if (ioctl(fd, SIOCGIFFLAGS, &ifr) < 0) {
        if (interface_do_message)
            daemon_log(LOG_WARNING, "Warning: Could not get interface flags.");
        
        return;
    }
    
    ifr.ifr_flags |= IFF_UP;
    
    if (ioctl(fd, SIOCSIFFLAGS, &ifr) < 0)
        if (interface_do_message)
            daemon_log(LOG_WARNING, "Warning: Could not set interface flags.");
}

interface_status_t interface_detect_beat_mii(int fd, char *iface) {
    struct ifreq ifr;
    
    if (interface_auto_up)
        interface_up(fd, iface);
    
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, iface, sizeof(ifr.ifr_name)-1);

    if (ioctl(fd, SIOCGMIIPHY, &ifr) == -1) {
        if (interface_do_message)
            daemon_log(LOG_ERR, "SIOCGMIIPHY failed: %s", strerror(errno));
        
        return IFSTATUS_ERR;
    }

    ((unsigned short*) &ifr.ifr_data)[1] = 1;

    if (ioctl(fd, SIOCGMIIREG, &ifr) == -1) {
        if (interface_do_message)
            daemon_log(LOG_ERR, "SIOCGMIIREG failed: %s", strerror(errno));
        
        return IFSTATUS_ERR;
    }

    return (((unsigned short*) &ifr.ifr_data)[3] & 0x0004) ? IFSTATUS_UP : IFSTATUS_DOWN;
}

interface_status_t interface_detect_beat_priv(int fd, char *iface) {
    struct ifreq ifr;
    
    if (interface_auto_up)
        interface_up(fd, iface);
    
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, iface, sizeof(ifr.ifr_name)-1);

    if (ioctl(fd, SIOCDEVPRIVATE, &ifr) == -1) {
        if (interface_do_message)
            daemon_log(LOG_ERR, "SIOCDEVPRIVATE failed: %s", strerror(errno));
        
        return IFSTATUS_ERR;
    }

    ((unsigned short*) &ifr.ifr_data)[1] = 1;

    if (ioctl(fd, SIOCDEVPRIVATE+1, &ifr) == -1) {
        if (interface_do_message)
            daemon_log(LOG_ERR, "SIOCDEVPRIVATE+1 failed: %s", strerror(errno));
        
        return IFSTATUS_ERR;
    }

    return (((unsigned short*) &ifr.ifr_data)[3] & 0x0004) ? IFSTATUS_UP : IFSTATUS_DOWN;
}

interface_status_t interface_detect_beat_ethtool(int fd, char *iface) {

    struct ifreq ifr;
    struct ethtool_value edata;

    if (interface_auto_up)
        interface_up(fd, iface);
    
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, iface, sizeof(ifr.ifr_name)-1);

    edata.cmd = ETHTOOL_GLINK;
    ifr.ifr_data = (caddr_t) &edata;

    if (ioctl(fd, SIOCETHTOOL, &ifr) == -1) {
        if (interface_do_message)
            daemon_log(LOG_ERR, "ETHTOOL_GLINK failed: %s", strerror(errno));
        
        return IFSTATUS_ERR;
    }

    return edata.data ? IFSTATUS_UP : IFSTATUS_DOWN;
}

interface_status_t interface_detect_beat_iff(int fd, char *iface) {

    struct ifreq ifr;

    if (interface_auto_up)
        interface_up(fd, iface);

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, iface, sizeof(ifr.ifr_name)-1);

    if (ioctl(fd, SIOCGIFFLAGS, &ifr) == -1) {
        if (interface_do_message)
            daemon_log(LOG_ERR, "SIOCGIFFLAGS failed: %s", strerror(errno));

        return IFSTATUS_ERR;
    }

    return ifr.ifr_flags & IFF_RUNNING ? IFSTATUS_UP : IFSTATUS_DOWN;
}

static int get_wlan_qual_old(char *iface) {
    FILE *f;
    char buf[256];
    char *bp;
    int l, q = -1;
    
    l = strlen(iface);
    
    if (!(f = fopen("/proc/net/wireless", "r"))) {
        if (interface_do_message)
            daemon_log(LOG_WARNING, "Failed to open /proc/net/wireless: %s",strerror(errno));
        
        return -1;
    }
    
    while (fgets(buf, sizeof(buf)-1, f)) {
        bp = buf;

        while (*bp && isspace(*bp))
            bp++;
        
        if(!strncmp(bp, iface, l) && bp[l]==':') {

            /* skip device name */
            if (!(bp = strchr(bp,' ')))
                break;

            bp++;
            
            /* skip status */
            if (!(bp = strchr(bp,' ')))
                break;

            q = atoi(bp);
            break;
        };
    }

    fclose(f);

    if (q < 0) {
        if (interface_do_message)
            daemon_log(LOG_ERR, "Failed to find interface in /proc/net/wireless");
    }
        
    return q;
}

static int get_wlan_qual_new(int fd, char *iface) {
    struct iwreq req;
    struct iw_statistics q;
    static struct iw_range range;

    memset(&req, 0, sizeof(req));
    strncpy(req.ifr_ifrn.ifrn_name, iface, IFNAMSIZ);

    req.u.data.pointer = (caddr_t) &q;
    req.u.data.length = sizeof(q);
    req.u.data.flags = 1;

    if (ioctl(fd, SIOCGIWSTATS, &req) < 0) {
        if (interface_do_message)
            daemon_log(LOG_ERR, "Failed to get interface quality: %s\n", strerror(errno));
        return -1;
    }

    memset(&req, 0, sizeof(req));
    strncpy(req.ifr_ifrn.ifrn_name, iface, IFNAMSIZ);

    memset(&range, 0, sizeof(struct iw_range));
    req.u.data.pointer = (caddr_t) &range;
    req.u.data.length = sizeof(struct iw_range);
    req.u.data.flags = 0;
     
    if (ioctl(fd, SIOCGIWRANGE, &req) < 0) {
        if (interface_do_message)
            daemon_log(LOG_ERR, "SIOCGIWRANGE failed: %s\n", strerror(errno));
        return -1;
    }
    
    /* Test if both qual and level are on their lowest level */
    if (q.qual.qual <= 0 &&
        (q.qual.level > range.max_qual.level ? q.qual.level <= 156 : q.qual.level <= 0))
        return 0;

    return 1;
}


static int is_assoc_ap(uint8_t mac[ETH_ALEN]) {
    int b, j;
    b = 1;
    
    for (j = 1; j < ETH_ALEN; j++)
        if (mac[j] != mac[0]) {
            b = 0;
            break;
        }

    return !b || (mac[0] != 0xFF && mac[0] != 0x44 && mac[0] != 0x00);
}

interface_status_t interface_detect_beat_wlan(int fd, char *iface) {
    uint8_t mac[6];
    int q;
    struct iwreq req;

    if (interface_auto_up)
        interface_up(fd, iface);
    
    memset(&req, 0, sizeof(req));
    strncpy(req.ifr_ifrn.ifrn_name, iface, IFNAMSIZ);
     
    if (ioctl(fd, SIOCGIWAP, &req) < 0) {
        if (interface_do_message)
            daemon_log(LOG_WARNING, "Failed to get AP address: %s",strerror(errno));
        return IFSTATUS_ERR;
    }

    memcpy(mac, &(req.u.ap_addr.sa_data), ETH_ALEN);
           
    if (!is_assoc_ap(mac))
        return IFSTATUS_DOWN;

    if ((q = get_wlan_qual_new(fd, iface)) < 0)
        if ((q = get_wlan_qual_old(iface)) < 0) {
            if (interface_do_message)
                daemon_log(LOG_WARNING, "Failed to get wireless link quality.");
            
            return IFSTATUS_ERR;
        }
    
    return q > 0 ? IFSTATUS_UP : IFSTATUS_DOWN;
}

