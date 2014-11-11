# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# $Id: configure.ac 128 2007-07-10 16:55:39Z lennart $

# This file is part of libdaemon.
#
# libdaemon is free software; you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation; either version 2.1 of the
# License, or (at your option) any later version.
#
# libdaemon is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with libdaemon; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
# USA

AC_PREREQ(2.59)
AC_INIT([libdaemon],[0.12],[mzqnrzba (at) 0pointer (dot) de])
AC_CONFIG_SRCDIR([libdaemon/dfork.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign 1.9 -Wall])

AC_SUBST(PACKAGE_URL, [http://0pointer.de/lennart/projects/libdaemon/])

if type -p stow > /dev/null && test -d /usr/local/stow ; then
   AC_MSG_NOTICE([*** Found /usr/local/stow: default install prefix set to /usr/local/stow/${PACKAGE_NAME}-${PACKAGE_VERSION} ***])
   ac_default_prefix="/usr/local/stow/${PACKAGE_NAME}-${PACKAGE_VERSION}"
fi

# Checks for programs.
AC_PROG_CC
AC_GNU_SOURCE
AC_PROG_CXX
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

# If using GCC specify some additional parameters
if test "x$GCC" = "xyes" ; then
   CFLAGS="$CFLAGS -pipe -W -Wall -pedantic"

   # Test whether sys/signal.h can be included when compiled with -std=c99
   AC_MSG_CHECKING([checking whether sys/signal.h can be included with -std=c99])
   OLDCFLAGS="$CFLAGS"
   CFLAGS="-std=c99"
   AC_TRY_COMPILE([#include <sys/signal.h>],
                   [int main(void) {
                   struct sigaction sa; 
                   sa.sa_handler = 0;
		   return 0;
		   };], use_stdc99=yes, use_stdc99=no)

   if test x"$use_stdc99" = xyes; then
      CFLAGS="-std=c99 $OLDCFLAGS"
      AC_MSG_RESULT([yes])
   else
      AC_MSG_RESULT([no])
      CFLAGS="$OLDCFLAGS"
   fi

   AC_LANG_CONFTEST([int main() {}])
   $CC -c conftest.c -Wno-unused-parameter $CFLAGS > /dev/null 2> /dev/null && CFLAGS="$CFLAGS -Wno-unused-parameter"
   rm -f conftest.o
fi

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h limits.h stdlib.h string.h syslog.h unistd.h sys/ioctl.h sys/time.h])
AC_HEADER_SYS_WAIT

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_PID_T
AC_HEADER_TIME
AC_C_VOLATILE

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_SELECT_ARGTYPES
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([select strerror dup2 memset strrchr])
AC_TYPE_MODE_T
AC_FUNC_SETPGRP
AC_TYPE_SIGNAL
AC_TYPE_SIZE_T

# LYNX documentation generation
AC_ARG_ENABLE(lynx,
        AS_HELP_STRING(--disable-lynx,Turn off lynx usage for documentation generation),
[case "${enableval}" in
  yes) lynx=yes ;;
  no)  lynx=no ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --disable-lynx) ;;
esac],[lynx=yes])

if test x$lynx != xno ; then
   AC_CHECK_PROG(have_lynx, lynx, yes, no)

   if test x$lynx = xyes && test x$have_lynx = xno ; then
     AC_MSG_ERROR([*** Lynx enabled but couldn't find lynx ***])
   fi
fi

AM_CONDITIONAL([USE_LYNX], [test "x$lynx" != xno])

AC_CONFIG_FILES([libdaemon/Makefile Makefile doc/Makefile doc/README.html doc/doxygen.conf examples/Makefile libdaemon.spec libdaemon.pc])
AC_OUTPUT
