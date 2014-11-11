/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*- */
/* dir-watch-kqueue.c  OS specific directory change notification for message bus
 *
 * Copyright (C) 2003 Red Hat, Inc.
 *
 * Licensed under the Academic Free License version 2.1
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <config.h>

#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif

#include "bus.h"
#include <dbus/dbus-watch.h>

#include <dbus/dbus-internals.h>
#include "dir-watch.h"

#define MAX_DIRS_TO_WATCH 128

static int kq = -1;
static int fds[MAX_DIRS_TO_WATCH];
static int num_fds = 0;
static DBusWatch *watch = NULL;
static DBusLoop *loop = NULL;

static dbus_bool_t
_kqueue_watch_callback (DBusWatch *watch, unsigned int condition, void *data)
{
  return dbus_watch_handle (watch, condition);
}

static dbus_bool_t
_handle_kqueue_watch (DBusWatch *watch, unsigned int flags, void *data)
{
  struct kevent ev;
  struct timespec nullts = { 0, 0 };
  int res;
  pid_t pid;

  res = kevent (kq, NULL, 0, &ev, 1, &nullts);

  /* Sleep for half a second to avoid a race when files are install(1)'d
   * to system.d. */
  usleep(500000);

  if (res > 0)
    {
      pid = getpid ();
      _dbus_verbose ("Sending SIGHUP signal on reception of a kevent\n");
      (void) kill (pid, SIGHUP);
    }
  else if (res < 0 && errno == EBADF)
    {
      kq = -1;
      if (watch != NULL)
	{
	  _dbus_loop_remove_watch (loop, watch, _kqueue_watch_callback, NULL);
          _dbus_watch_unref (watch);
	  watch = NULL;
	}
      pid = getpid ();
      _dbus_verbose ("Sending SIGHUP signal since kqueue has been closed\n");
      (void) kill (pid, SIGHUP);
    }

  return TRUE;
}

void
bus_watch_directory (const char *dir, BusContext *context)
{
  int fd;
  struct kevent ev;

  _dbus_assert (dir != NULL);

  if (kq < 0)
    {

      kq = kqueue ();
      if (kq < 0)
        {
          _dbus_warn ("Cannot create kqueue; error '%s'\n", _dbus_strerror (errno));
	  goto out;
	}

        loop = bus_context_get_loop (context);

        watch = _dbus_watch_new (kq, DBUS_WATCH_READABLE, TRUE,
                                 _handle_kqueue_watch, NULL, NULL);

	if (watch == NULL)
          {
            _dbus_warn ("Unable to create kqueue watch\n");
	    close (kq);
	    kq = -1;
	    goto out;
	  }

	if (!_dbus_loop_add_watch (loop, watch, _kqueue_watch_callback,
                                   NULL, NULL))
          {
            _dbus_warn ("Unable to add reload watch to main loop");
	    close (kq);
	    kq = -1;
	    _dbus_watch_unref (watch);
	    watch = NULL;
            goto out;
	  }
    }

  if (num_fds >= MAX_DIRS_TO_WATCH )
    {
      _dbus_warn ("Cannot watch config directory '%s'. Already watching %d directories\n", dir, MAX_DIRS_TO_WATCH);
      goto out;
    }

  fd = open (dir, O_RDONLY);
  if (fd < 0)
    {
      _dbus_warn ("Cannot open directory '%s'; error '%s'\n", dir, _dbus_strerror (errno));
      goto out;
    }

  EV_SET (&ev, fd, EVFILT_VNODE, EV_ADD | EV_ENABLE | EV_CLEAR,
          NOTE_DELETE | NOTE_EXTEND | NOTE_WRITE | NOTE_RENAME, 0, 0);
  if (kevent (kq, &ev, 1, NULL, 0, NULL) == -1)
    {
      _dbus_warn ("Cannot setup a kevent for '%s'; error '%s'\n", dir, _dbus_strerror (errno));
      close (fd);
      goto out;
    }

  fds[num_fds++] = fd;
  _dbus_verbose ("Added kqueue watch on config directory '%s'\n", dir);

 out:
  ;
}

void
bus_drop_all_directory_watches (void)
{
  int i;

  _dbus_verbose ("Dropping all watches on config directories\n");

  for (i = 0; i < num_fds; i++)
    {
      if (close (fds[i]) != 0)
        {
          _dbus_verbose ("Error closing fd %d for config directory watch\n", fds[i]);
	}
    }

  num_fds = 0;
}
