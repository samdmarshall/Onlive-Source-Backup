/* -*- Mode: C; tab-width: 4; indent-tabs-mode: f; c-basic-offset: 4 -*- */
/* $Id: ifplugd.c 123 2005-06-04 16:07:11Z lennart $ */

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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>
#include <getopt.h>
#include <stdarg.h>
#include <syslog.h>
#include <fcntl.h>
#include <linux/kd.h>
#include <sys/ioctl.h>
#include <limits.h>
#include <net/if.h>
#include <linux/sockios.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/time.h>
#include <time.h>

#include <libdaemon/dlog.h>
#include <libdaemon/dpid.h>
#include <libdaemon/dsignal.h>
#include <libdaemon/dfork.h>

#include "ethtool-local.h"
#include "interface.h"
#include "nlapi.h"
#include "ifmonitor.h"
#include "svn-revision.h"

#define VARRUN "/var/run"
#define IFPLUGD_ENV_PREVIOUS "IFPLUGD_PREVIOUS"
#define IFPLUGD_ENV_CURRENT "IFPLUGD_CURRENT"

int interface_auto_up = 1,
    interface_do_message = 1;

char *interface = NULL;
char *run = SYSCONFDIR"/ifplugd/ifplugd.action";
char *extra_arg = NULL;

int polltime = 1,
    delay_up = 0,
    delay_down = 5;

int daemonize = 1,
    use_beep = 1,
    no_startup_script = 0,
    no_shutdown_script = 0,
    wait_on_fork = 0,
    wait_on_kill = 0,
    use_syslog = 1,
    ignore_retval = 0,
    initial_down = 0,
    use_ifmonitor = 0;

int disabled = 0;

interface_status_t failure_status = IFSTATUS_ERR;

enum { API_AUTO, API_ETHTOOL, API_MII, API_PRIVATE, API_WLAN, API_IFF } api_mode = API_AUTO;

interface_status_t (*detect_beat_func)(int, char*);
interface_status_t (*cached_detect_beat_func)(int, char*) = NULL;

// 0: high, 1: low, 2: very low
void beep(int b) {
    int fd = -1, argp;

    if (!use_beep)
        return;
    
    if ((fd = open("/dev/tty1", O_WRONLY|O_NOCTTY)) < 0) {
        use_beep = 0;
        daemon_log(LOG_WARNING, "Could not open /dev/tty, cannot beep.");
        goto finish;
    }

    switch (b) {
        case 0: argp = (100<<16) + 0x637; break;
        case 1: argp = (100<<16) + 0x937; break;
        default: argp = (100<<16) + 0x1237; break;
    }

    if (ioctl(fd, KDMKTONE, argp) != 0) {
        use_beep = 0;
        daemon_log(LOG_WARNING, "Beep failure, disabled.");
        goto finish;
    }

    usleep((argp >> 16)*1000);
    
finish:

    if (fd >= 0)
        close(fd);

    return;
}

const char *pid_file_proc() {
    static char fn[PATH_MAX];
    snprintf(fn, sizeof(fn), "%s/ifplugd.%s.pid", VARRUN, interface);
    return fn;
}

int action(interface_status_t status) {
    pid_t pid;
    int _pipe[2];
    unsigned n = 0;
    static char buf[256];
    char *arg = (status == IFSTATUS_UP ? "up" : "down");
    int sigfd, r;
    fd_set rfds;

    daemon_log(LOG_INFO, "Executing '%s %s %s'.", run, interface, arg);
    
    if (pipe(_pipe) < 0) {
        daemon_log(LOG_ERR, "pipe() failed: %s", strerror(errno));
        return -1;
    }

    if ((pid = fork()) < 0) {
        daemon_log(LOG_ERR, "fork() failed: %s", strerror(errno));
        return -1;
            
    } else if (pid == 0) {
        dup2(_pipe[1], 1);
        dup2(_pipe[1], 2);
        
        if (_pipe[0] > 2)
            close(_pipe[0]);

        if (_pipe[1] > 2)
            close(_pipe[1]);

        umask(0022); /* Set up a sane umask */
        
        execl(run, run, interface, arg, extra_arg, (char *) NULL);

        _exit(EXIT_FAILURE);
    }

    close(_pipe[1]);

    FD_ZERO(&rfds);
    FD_SET(_pipe[0], &rfds);
    sigfd = daemon_signal_fd();
    FD_SET(sigfd, &rfds);    

    n = 0;

    for (;;) {
        fd_set wrfds = rfds;

        if (select(FD_SETSIZE, &wrfds, NULL, NULL, NULL) < 0) {

            if (errno == EINTR)
                continue;

            break;
        }


        if (FD_ISSET(_pipe[0], &wrfds)) {
            char c;

            if (read(_pipe[0], &c, 1) != 1)
                break;

            buf[n] = c;
            
            if (c == '\n' || n >= sizeof(buf) - 2) {
                if (c != '\n') n++;
                buf[n] = 0;

                if (buf[0])
                    daemon_log(LOG_WARNING, "client: %s", buf);
            
                n = 0;
            } else
                n++;
        }

        if (FD_ISSET(sigfd, &wrfds)) {
            int sig;
            
            if ((sig = daemon_signal_next()) < 0) {
                daemon_log(LOG_ERR, "daemon_signal_next(): %s", strerror(errno));
                break;
            }

            if (sig != SIGCHLD) {
                daemon_log(LOG_WARNING, "Killing child.");
                kill(pid, SIGTERM);
            }
        }
        
    }

    if (n > 0) {
        buf[n] = 0;
        daemon_log(LOG_WARNING, "client: %s", buf);
    }

    waitpid(pid, &r, 0);

    close(_pipe[0]);

    if (!WIFEXITED(r) || WEXITSTATUS(r) != 0) {
        if (status == IFSTATUS_UP)
            beep(2);
        daemon_log(LOG_ERR, "Program execution failed, return value is %i.", WEXITSTATUS(r));

        return ignore_retval ? 0 : -1;
    } else {
        if (status == IFSTATUS_UP)
            beep(0);

        daemon_log(LOG_INFO, "Program executed successfully.");
        return 0;
    }
}

interface_status_t detect_beat_auto(int fd, char *iface) {
    interface_status_t status;

    if (cached_detect_beat_func && (status = cached_detect_beat_func(fd, iface)) != IFSTATUS_ERR)
        return status;
    
    if ((status = interface_detect_beat_ethtool(fd, iface)) != IFSTATUS_ERR) {
        cached_detect_beat_func = interface_detect_beat_ethtool;
        daemon_log(LOG_INFO, "Using detection mode: SIOCETHTOOL");
        return status;
    }
    
    if ((status = interface_detect_beat_mii(fd, iface)) != IFSTATUS_ERR) {
        cached_detect_beat_func = interface_detect_beat_mii;
        daemon_log(LOG_INFO, "Using detection mode: SIOCGMIIPHY");
        return status;
    }

    if ((status = interface_detect_beat_wlan(fd, iface)) != IFSTATUS_ERR) {
        cached_detect_beat_func = interface_detect_beat_wlan;
        daemon_log(LOG_INFO, "Using detection mode: wireless extension");
        return status;
    }

    if ((status = interface_detect_beat_iff(fd, iface)) != IFSTATUS_ERR) {
        cached_detect_beat_func = interface_detect_beat_iff;
        daemon_log(LOG_INFO, "Using detection mode: IFF_RUNNING");
        return status;
    }

    return IFSTATUS_ERR;
}

char *strstatus(interface_status_t s) {
    switch(s) {
        case IFSTATUS_UP: return "up";
        case IFSTATUS_DOWN: return "down";
        case IFSTATUS_ERR: return "error";
        default: return "disabled";
    }
}

interface_status_t detect_beat(int fd, char*iface) {
    interface_status_t status;
    static interface_status_t last_status = (interface_status_t) -1;

    if (disabled)
        return IFSTATUS_DOWN;
    
    if ((status = detect_beat_func(fd, iface)) == IFSTATUS_ERR)
        status = failure_status;

    if (status == IFSTATUS_ERR && detect_beat_func == detect_beat_auto)
        daemon_log(LOG_INFO, "Failed to detect plug status of %s", interface);
    
    if (status != last_status) {
        setenv(IFPLUGD_ENV_PREVIOUS, strstatus(last_status), 1);
        setenv(IFPLUGD_ENV_CURRENT, strstatus(status), 1);
        last_status = status;
    }
    
    return status;
}

int welcome_iface(int fd, char *iface) {
    struct ifreq ifr;
    struct ethtool_drvinfo drvinfo;
    char txt[256];

    if (interface_auto_up)
        interface_up(fd, iface);
    
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, iface, sizeof(ifr.ifr_name)-1);
    
    if (ioctl(fd, SIOCGIFHWADDR, &ifr) == -1)
        snprintf(txt, sizeof(txt)-1, "Using interface %s", iface);
    else
        snprintf(txt, sizeof(txt)-1, "Using interface %s/%02X:%02X:%02X:%02X:%02X:%02X", iface, ifr.ifr_hwaddr.sa_data[0] & 0xFF, ifr.ifr_hwaddr.sa_data[1] & 0xFF, ifr.ifr_hwaddr.sa_data[2] & 0xFF, ifr.ifr_hwaddr.sa_data[3] & 0xFF, ifr.ifr_hwaddr.sa_data[4] & 0xFF, ifr.ifr_hwaddr.sa_data[5] & 0xFF);

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, iface, sizeof(ifr.ifr_name)-1);

    drvinfo.cmd = ETHTOOL_GDRVINFO;
    ifr.ifr_data = (caddr_t) &drvinfo;
    
    if (ioctl(fd, SIOCETHTOOL, &ifr) != -1)
        daemon_log(LOG_INFO, "%s with driver <%s> (version: %s)", txt, drvinfo.driver, drvinfo.version);
    else
        daemon_log(LOG_INFO, "%s", txt);

    cached_detect_beat_func = NULL;
    
    return 0;
}

int ifmonitor_cb(int b, int index, unsigned short type, const char *name) {
    if (!name)
        return 0;

    if (!strcmp(name, interface))
        disabled = !b;

    return 0;
}

int is_iface_available(int s, char *p) {
    struct ifreq req;
    int r;

    memset(&req, 0, sizeof(req));
    strncpy(req.ifr_name, p, IFNAMSIZ);
    
    if ((r = ioctl(s, SIOCGIFINDEX, &req)) < 0 && errno != ENODEV) {
        daemon_log(LOG_ERR, "SIOCGIFINDEX failed: %s\n", strerror(errno));
        return -1;
    }
    return r >= 0 && req.ifr_ifindex >= 0;
}

void work(void) {
    interface_status_t status;
    int fd = -1;
    fd_set fds;
    int sigfd;
    time_t t = 0;
    int send_retval = 1;
    int paused = 0;
    static char log_ident[256];

    snprintf(log_ident, sizeof(log_ident), "ifplugd(%s)", interface);

    daemon_log_ident = log_ident;

    daemon_log(LOG_INFO, "ifplugd "VERSION" initializing%s.", use_ifmonitor ? ", using NETLINK device monitoring" : "");

    if (daemon_pid_file_create() < 0) {
        daemon_log(LOG_ERR, "Could not create PID file %s.", daemon_pid_file_proc());
        goto finish;
    }

    if (daemon_signal_init(SIGINT, SIGTERM, SIGQUIT, SIGHUP, SIGCHLD, SIGUSR1, SIGUSR2, -1) < 0) {
        daemon_log(LOG_ERR, "Could not register signal handler: %s", strerror(errno));
        goto finish;
    }

    switch (api_mode) {
        case API_ETHTOOL: detect_beat_func = interface_detect_beat_ethtool; break;
        case API_MII: detect_beat_func = interface_detect_beat_mii; break;
        case API_PRIVATE: detect_beat_func = interface_detect_beat_priv; break;
        case API_WLAN: detect_beat_func = interface_detect_beat_wlan; break;
        case API_IFF: detect_beat_func = interface_detect_beat_iff; break;
            
        default:
            detect_beat_func = detect_beat_auto; 
            interface_do_message = 0;
            break;
    }

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        daemon_log(LOG_ERR, "socket(): %s", strerror(errno));
        goto finish;
    }

    if (use_ifmonitor) {
        int b;
        if ((b = is_iface_available(fd, interface)) < 0) {
            daemon_log(LOG_ERR, "Failed to check interface availabilty!");
            goto finish;
        }
        
        disabled = !b;

        if (nlapi_open(RTMGRP_LINK) < 0)
            goto finish;

        if (ifmonitor_init(ifmonitor_cb) < 0)
            goto finish;
    } else
        disabled = 0;

    if (!disabled) {
        if (welcome_iface(fd, interface) < 0)
            goto finish;
    }

    if ((status = detect_beat(fd, interface)) == IFSTATUS_ERR)
        goto finish;
    
    daemon_log(LOG_INFO, "Initialization complete, link beat %sdetected%s.", status == IFSTATUS_UP ? "" : "not ", use_ifmonitor ? (disabled ? ", interface disabled" : ", interface enabled") : "");
    beep(status == IFSTATUS_UP ? 0 : 1);

    if ((!no_startup_script && status == IFSTATUS_UP) || initial_down)
        if (action(status) < 0)
            goto finish;

    if (send_retval && daemonize && wait_on_fork) {
        char c = status == IFSTATUS_UP ? 2 : (status == IFSTATUS_DOWN ? 3 : 1);
        daemon_retval_send(c);
        send_retval = 0;
    }

    FD_ZERO(&fds);
    sigfd = daemon_signal_fd();
    FD_SET(sigfd, &fds);

    if (use_ifmonitor)
        FD_SET(nlapi_fd, &fds);

    for (;;) {
        interface_status_t s;
        fd_set qfds = fds;
        int d;
        struct timeval tv;

        tv.tv_sec = polltime;
        tv.tv_usec = 0;
        
        if (select(FD_SETSIZE, &qfds, NULL, NULL, &tv) < 0) {
            if (errno == EINTR)
                continue;

            daemon_log(LOG_ERR, "select(): %s", strerror(errno));
            goto finish;
        }

        //daemon_log(LOG_INFO, "select()");
        
        d = disabled;
        s = status;
        
        if (use_ifmonitor) {

            if (FD_ISSET(nlapi_fd, &qfds)) {
                if (nlapi_work(0) < 0)
                    goto finish;
            }

            if (d && !disabled) {
                daemon_log(LOG_INFO, "Interface enabled");
                welcome_iface(fd, interface);
                status = IFSTATUS_DOWN;
            }
            
            if (!d && disabled) {
                daemon_log(LOG_INFO, "Interface disabled");
                status = IFSTATUS_DOWN;
            }
        }
            

        if (!paused && !disabled) {
            //daemon_log(LOG_INFO, "detect");
            if ((status = detect_beat(fd, interface)) == IFSTATUS_ERR) {
                if (!use_ifmonitor)
                    goto finish;

                status = IFSTATUS_DOWN;
            }
        }

        if (status != s) {
            daemon_log(LOG_INFO, "Link beat %s.", status == IFSTATUS_DOWN ? "lost" : "detected");
            beep(status == IFSTATUS_UP ? 0 : 1);
            
            if (t)
                t = 0;
            else {
                t = time(NULL);
                
                if (status == IFSTATUS_UP)
                    t += delay_up;
                
                if (status == IFSTATUS_DOWN)
                    t += delay_down;
            }
        }

        if (FD_ISSET(sigfd, &qfds)) {
            int sig;

            if ((sig = daemon_signal_next()) < 0) {
                daemon_log(LOG_ERR, "daemon_signal_next(): %s", strerror(errno));
                goto finish;
            }

            switch (sig) {

                case SIGINT:
                case SIGTERM:
                    goto cleanup;
                    
                case SIGQUIT:
                    goto finish;
                    
                case SIGCHLD:
                    break;

                case SIGHUP:
                    daemon_log(LOG_INFO, "SIGHUP: %s, link detected on %s: %s", paused ? "Suspended" : "Running", interface, status == IFSTATUS_DOWN ? "no" : "yes");

                    if (use_ifmonitor)
                        daemon_log(LOG_INFO, "SIGHUP: Interface %s", disabled ? "disabled" : "enabled");
                    break;
                    
                case SIGUSR1:
                    daemon_log(LOG_INFO, "SIGUSR1: Daemon suspended (#%i)", ++paused);
                    break;

                case SIGUSR2:
                    if (paused > 0) {
                        daemon_log(LOG_INFO, "SIGUSR2: Daemon resumed (#%i)", paused);
                        paused --;
                    }
                    
                    break;
                    
                default:
                    daemon_log(LOG_INFO, "Ignoring unknown signal %s", strsignal(sig));
                    break;
            }
        }

        if (t && t < time(NULL)) {
            t = 0;

            if (action(status) < 0)
                goto finish;
        }
    }

cleanup:
    if (!no_shutdown_script && (status == IFSTATUS_UP || (status == IFSTATUS_DOWN && t))) {
        setenv(IFPLUGD_ENV_PREVIOUS, strstatus(status), 1);
        setenv(IFPLUGD_ENV_CURRENT, strstatus(-1), 1);
        action(IFSTATUS_DOWN);
        beep(1);
    }
    
finish:

    if (fd >= 0)
        close(fd);

    if (use_ifmonitor)
        nlapi_close();
    
    if (send_retval && daemonize && wait_on_fork)
        daemon_retval_send(255);

    daemon_pid_file_remove();
    daemon_signal_done();
    
    daemon_log(LOG_INFO, "Exiting.");
}

void usage(char *p) {
    char *m;

    switch (api_mode) {
        case API_ETHTOOL: m = "ethtool"; break;
        case API_MII: m = "mii"; break;
        case API_PRIVATE: m = "priv"; break;
        case API_WLAN: m = "wlan"; break;
        case API_IFF: m = "iff"; break;
        default: m = "auto";
    }
    
    if (strrchr(p, '/'))
        p = strchr(p, '/')+1;

    printf("%s -- Network Interface Plug Detection Daemon\n\n"
           "Usage: %s [options]\n\n"
           "Options:\n"
           "   -a --no-auto              Do not enable interface automatically (%s)\n"
           "   -n --no-daemon            Do not daemonize (for debugging) (%s)\n"
           "   -s --no-syslog            Do not use syslog, use stderr instead (for debugging) (%s)\n"
           "   -b --no-beep              Do not beep (%s)\n"
           "   -f --ignore-fail          Ignore detection failure, retry instead (failure is treated as DOWN) (%s)\n"
           "   -F --ignore-fail-positive Ignore detection failure, retry instead (failure is treated as UP) (%s)\n"
           "   -i --iface=IFACE          Specify ethernet interface (%s)\n"
           "   -r --run=EXEC             Specify program to execute (%s)\n"
           "   -I --ignore-retval        Don't exit on nonzero return value of program executed (%s)\n"
           "   -t --poll-time=SECS       Specify poll time in seconds (%i)\n"
           "   -u --delay-up=SECS        Specify delay for configuring interface (%i)\n"
           "   -d --delay-down=SECS      Specify delay for deconfiguring interface (%i)\n"
           "   -m --api-mode=MODE        Force API mode (mii, priv, ethtool, wlan, auto) (%s)\n"
           "   -p --no-startup           Don't run script on daemon startup (%s)\n"
           "   -q --no-shutdown          Don't run script on daemon quit (%s)\n"
           "   -l --initial-down         Run \"down\" script on startup if no cable is detected (%s)\n"
           "   -w --wait-on-fork         Wait until daemon fork finished (%s)\n"
           "   -W --wait-on-kill         When run with -k, wait until the daemon died (%s)\n"
           "   -x --extra-arg            Specify an extra argument for action script\n"
           "   -M --monitor              Use interface monitoring (%s)\n"
           "   -h --help                 Show this help\n"
           "   -k --kill                 Kill a running daemon\n"
           "   -c --check-running        Check if a daemon is currently running\n"
           "   -v --version              Show version\n"
           "   -S --suspend              Suspend running daemon\n"
           "   -R --resume               Resume running daemon\n"
           "   -z --info                 Write status of running daemon to syslog\n",
           p, p,
           !interface_auto_up ? "on" : "off",
           !daemonize ? "on" : "off",
           !use_syslog ? "on" : "off",
           !use_beep ? "on" : "off",
           failure_status == IFSTATUS_DOWN ? "on" : "off",
           failure_status == IFSTATUS_UP ? "on" : "off",
           interface,
           run,
           ignore_retval ? "on" : "off",
           polltime,
           delay_up,
           delay_down,
           m,
           no_startup_script ? "on" : "off",
           no_shutdown_script ? "on" : "off",
           initial_down ? "on" : "off",
           wait_on_fork ? "on" : "off",
           wait_on_kill ? "on" : "off",
           use_ifmonitor ? "on" : "off");
}

void parse_args(int argc, char *argv[]) {
    static struct option long_options[] = {
        {"no-auto",              no_argument, 0, 'a'},
        {"no-daemon",            no_argument, 0, 'n'},
        {"no-syslog",            no_argument, 0, 's'},
        {"no-beep",              no_argument, 0, 'b'},
        {"ignore-fail",          no_argument, 0, 'f'},
        {"ignore-fail-positive", no_argument, 0, 'F'},
        {"ignore-retval",        no_argument, 0, 'I'},
        {"iface",                required_argument, 0, 'i'},
        {"run",                  required_argument, 0, 'r'},
        {"poll-time",            required_argument, 0, 't'},
        {"delay-up",             required_argument, 0, 'u'},
        {"delay-down",           required_argument, 0, 'd'},
        {"api-mode",             required_argument, 0, 'm'},
        {"wait-on-fork",         no_argument, 0, 'w'},
        {"wait-on-kill",         no_argument, 0, 'W'},
        {"no-startup",           no_argument, 0, 'p'},
        {"no-shutdown",          no_argument, 0, 'q'},
        {"help",                 no_argument, 0, 'h'},
        {"kill",                 no_argument, 0, 'k'},
        {"check-running",        no_argument, 0, 'c'},
        {"version",              no_argument, 0, 'v'},
        {"extra-arg",            required_argument, 0, 'x'},
        {"suspend",              no_argument, 0, 'S'},
        {"resume",               no_argument, 0, 'R'},
        {"info",                 no_argument, 0, 'z'},
        {"initial-down",         no_argument, 0, 'l'},
        {"monitor",              no_argument, 0, 'M'},
        {0, 0, 0, 0}
    };
    int option_index = 0;
    int help = 0, _kill = 0, _check = 0, _version = 0, _suspend = 0, _resume = 0, _info = 0;
    
    for (;;) {
        int c;
        
        if ((c = getopt_long(argc, argv, "asni:r:t:u:d:hkbfFvm:pqwx:cISRzlMW", long_options, &option_index)) < 0)
            break;

        switch (c) {
            case 'a' :
                interface_auto_up = !interface_auto_up;
                break;
            case 's' :
                use_syslog = !use_syslog;
                break;
            case 'n' :
                daemonize = !daemonize;
                break;
            case 'i' :
                if (interface)
                    free(interface);
                interface = strdup(optarg);
                break;
            case 'r':
                run = strdup(optarg);
                break;
            case 'I':
                ignore_retval = !ignore_retval;
                break;
            case 't':
                polltime = atoi(optarg);
                if (polltime < 0) polltime = 0;
                break;
            case 'u':
                delay_up = atoi(optarg);
                break;
            case 'd':
                delay_down = atoi(optarg);
                break;
            case 'h':
                help = 1;
                break;
            case 'k':
                _kill = 1;
                break;
            case 'c':
                _check = 1;
                break;
            case 'v':
                _version = 1;
                break;
            case 'b':
                use_beep = !use_beep;
                break;
            case 'f':
                failure_status = IFSTATUS_DOWN;
                break;
            case 'F':
                failure_status = IFSTATUS_UP;
                break;
            case 'm':
                switch (tolower(optarg[0])) {
                    case 'e': api_mode = API_ETHTOOL; break;
                    case 'm': api_mode = API_MII; break;
                    case 'p': api_mode = API_PRIVATE; break;
                    case 'w': api_mode = API_WLAN; break;
                    case 'a': api_mode = API_AUTO; break;
                    case 'i': api_mode = API_IFF; break;
                    default:
                        daemon_log(LOG_ERR, "Unknown API mode: %s", optarg);
                        exit(2);
                }
                break;
            case 'p':
                no_startup_script = !no_startup_script;
                break;
            case 'q':
                no_shutdown_script = !no_shutdown_script;
                break;
            case 'l':
                initial_down = !initial_down;
                break;
            case 'w':
                wait_on_fork = !wait_on_fork;
                break;
            case 'W':
                wait_on_kill = !wait_on_kill;
                break;
            case 'x':
                extra_arg = strdup(optarg);
                break;
            case 'S':
                _suspend = 1;
                break;
            case 'R':
                _resume = 1;
                break;
            case 'z':
                _info = 1;
                break;
            case 'M':
                use_ifmonitor = !use_ifmonitor;
                break;
            default:
                daemon_log(LOG_ERR, "Unknown parameter.");
                exit(1);
        }
    }


    if (!interface)
        interface = strdup("eth0");
    if (!use_syslog)
        daemon_log_use = DAEMON_LOG_STDERR;
    

    if (help) {
        usage(argv[0]);
        exit(0);
    }

    if (_kill || _resume || _suspend || _info) {
        int rv;
        
        if (_kill && wait_on_kill)
            rv = daemon_pid_file_kill_wait(SIGINT, 5);
        else
            rv = daemon_pid_file_kill(_kill ? SIGINT : (_resume ? SIGUSR2 : (_info ? SIGHUP : SIGUSR1)));

        if (rv < 0) {
            daemon_log(LOG_ERR, "Failed to kill daemon. (%s)", strerror(errno));
            exit(6);
        }

        exit(0);
    }

    if (_version) {

#ifdef SVN_REVISION
        printf("ifplugd "VERSION" (SVN: "SVN_REVISION")\n");
#else
        printf("ifplugd "VERSION"\n");
#endif

        exit(0);
    }

    if (_check) {
        pid_t pid = daemon_pid_file_is_running();

        if (pid == (pid_t) -1 || pid == 0) {
            printf("ifplugd not running.\n");
            exit(255);
        } else {
            printf("ifplugd process for device %s running as pid %u.\n", interface, pid);
            exit(0);
        }
    }
    
}

static volatile int alarmed = 0;

void sigalrm() {
    alarmed = 1;
}

int main(int argc, char* argv[]) {

    daemon_pid_file_proc = pid_file_proc;

    if ((daemon_log_ident = strrchr(argv[0], '/')))
        daemon_log_ident++;
    else
        daemon_log_ident = argv[0];

    parse_args(argc, argv);

    if (geteuid() != 0) {
        daemon_log(LOG_ERR, "Sorry, you need to be root to run this binary.");
        return 2;
    }

    if (daemon_pid_file_is_running() >= 0) {
        daemon_log(LOG_ERR, "Sorry, there is already an instance of ifplugd for %s running.", interface);
        return 4;
    }
    
    if (daemonize) {
        pid_t pid;

        if (wait_on_fork)
            if (daemon_retval_init() < 0) {
                daemon_log(LOG_ERR, "Sorry, could not create pipe: %s", strerror(errno));
                return 4;
            }
        
        if ((pid = daemon_fork()) < 0)
            return 3;

        if (pid) {
            int c = 0;
            
            // Parent process

            if (wait_on_fork)
                if ((c = daemon_retval_wait(60)) < 0) {
                    daemon_log(LOG_WARNING, "Killing background process.");
                    kill(pid, SIGTERM);
                }
            
            if (c > 3)
                daemon_log(LOG_ERR, "Daemon failed with error condition #%i. See syslog for details", c);
            
            return c;
        }
    }

    // Let's go
    work();
    return 0;
}
