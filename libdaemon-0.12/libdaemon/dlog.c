/* $Id: dlog.c 117 2007-06-11 15:34:47Z lennart $ */

/*
 * This file is part of libdaemon.
 *
 * libdaemon is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * libdaemon is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with libdaemon; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "dlog.h"

enum daemon_log_flags daemon_log_use = DAEMON_LOG_AUTO|DAEMON_LOG_STDERR;
const char* daemon_log_ident = NULL;

void daemon_logv(int prio, const char* template, va_list arglist) {
    int saved_errno;
    
    saved_errno = errno;
    
    if (daemon_log_use & DAEMON_LOG_SYSLOG) {
	openlog(daemon_log_ident ? daemon_log_ident : "UNKNOWN", LOG_PID, LOG_DAEMON);
        vsyslog(prio | LOG_DAEMON, template, arglist);
    }

    if (daemon_log_use & DAEMON_LOG_STDERR) {
        vfprintf(stderr, template, arglist);
        fprintf(stderr, "\n");
    }

    if (daemon_log_use & DAEMON_LOG_STDOUT) {
        vfprintf(stdout, template, arglist);
        fprintf(stdout, "\n");
    }

    errno = saved_errno;
}

void daemon_log(int prio, const char* template, ...) {
    va_list arglist;
    
    va_start(arglist, template);
    daemon_logv(prio, template, arglist);
    va_end(arglist);
}

char *daemon_ident_from_argv0(char *argv0) {
    char *p;

    if ((p = strrchr(argv0, '/')))
        return p+1;

    return argv0;
}
