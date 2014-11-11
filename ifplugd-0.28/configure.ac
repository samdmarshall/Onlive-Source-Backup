# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# $Id: configure.ac 123 2005-06-04 16:07:11Z lennart $

# This file is part of ifplugd.
#
# ifplugd is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# ifplugd is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with ifplugd; if not, write to the Free Software Foundation,
# Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.

AC_PREREQ(2.59)
AC_INIT([ifplugd],[0.28],[mzvscyhtq (at) 0pointer (dot) de])
AC_CONFIG_SRCDIR([src/ifplugd.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign -Wall])

AC_SUBST(PACKAGE_URL, [http://0pointer.de/lennart/projects/ifplugd/])

if type -p stow > /dev/null && test -d /usr/local/stow ; then
   AC_MSG_NOTICE([*** Found /usr/local/stow: default install prefix set to /usr/local/stow/${PACKAGE_NAME}-${PACKAGE_VERSION} ***])
   ac_default_prefix="/usr/local/stow/${PACKAGE_NAME}-${PACKAGE_VERSION}"
fi

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

# If using GCC specify some additional parameters
if test "x$GCC" = "xyes" ; then
   CFLAGS="$CFLAGS -pipe -W -Wall"

   AC_LANG_CONFTEST([int main() {}])
   $CC -c conftest.c -Wno-unused-parameter $CFLAGS > /dev/null 2> /dev/null && CFLAGS="$CFLAGS -Wno-unused-parameter"
   rm -f conftest.o
fi

# Checks for header files.
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([fcntl.h limits.h netinet/in.h stdlib.h string.h sys/ioctl.h sys/time.h syslog.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_PID_T
AC_HEADER_TIME
AC_C_VOLATILE

# Checks for library functions.
AC_FUNC_FORK
AC_PROG_GCC_TRADITIONAL
AC_FUNC_SELECT_ARGTYPES
AC_FUNC_MALLOC
AC_TYPE_SIGNAL
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([alarm dup2 memset select setenv socket strchr strdup strerror strrchr strspn])

PKG_CHECK_MODULES(LIBDAEMON, [ libdaemon >= 0.5 ])
AC_SUBST(LIBDAEMON_CFLAGS)
AC_SUBST(LIBDAEMON_LIBS)

# Init script location
AC_ARG_WITH(initdir, AS_HELP_STRING(--with-initdir=DIR,Install init script in DIR (system dependent default)))

test "x$with_initdir" = xyes -o "x$with_initdir" = xno && with_initdir=

if test "x$with_initdir" = x ; then
    if test -d /etc/init.d ; then
        SYSINITDIR=/etc/init.d
    else
    	if test -d /etc/rc.d/init.d ; then
        	SYSINITDIR=/etc/rc.d/init.d
    	else
        	AC_MSG_ERROR([missing --with-initdir=DIR])
	    fi
    fi
else
   SYSINITDIR="$with_initdir"
fi

AC_MSG_NOTICE([*** Init sript will be installed in $SYSINITDIR ***])
AC_SUBST(SYSINITDIR)

# LYNX documentation generation
AC_ARG_ENABLE(lynx,
        AS_HELP_STRING(--disable-lynx,Turn off lynx usage for documentation generation),
[case "${enableval}" in
  yes) lynx=yes ;;
  no)  lynx=no ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --disable-lynx) ;;
esac],[lynx=yes])

if test x$lynx = xyes ; then
   AC_CHECK_PROG(have_lynx, lynx, yes, no)

   if test x$have_lynx = xno ; then
     AC_MSG_ERROR([*** Sorry, you have to install lynx or use --disable-lynx ***])
   fi
fi

AM_CONDITIONAL([USE_LYNX], [test "x$lynx" = xyes])

# XMLTOMAN manpage generation
AC_ARG_ENABLE(xmltoman,
        AS_HELP_STRING(--disable-xmltoman,Disable rebuilding of man pages with xmltoman),
[case "${enableval}" in
  yes) xmltoman=yes ;;
  no)  xmltoman=no ;;
  *) AC_MSG_ERROR([bad value ${enableval} for --disable-xmltoman]) ;;
esac],[xmltoman=yes])

if test x$xmltoman = xyes ; then
   AC_CHECK_PROG(have_xmltoman, xmltoman, yes, no)

   if test x$have_xmltoman = xno ; then
     AC_MSG_WARN([*** Not rebuilding man pages as xmltoman is not found ***])
     xmltoman=no
   fi
fi

AM_CONDITIONAL([USE_XMLTOMAN], [test "x$xmltoman" = xyes])

# subversion release number generation
AC_ARG_ENABLE(subversion,
        AS_HELP_STRING(--disable-subversion,Disable rebuilding of svn-release.h),
[case "${enableval}" in
  yes) subversion=yes ;;
  no)  subversion=no ;;
  *) AC_MSG_ERROR([bad value ${enableval} for --disable-subversion]) ;;
esac],[subversion=yes])
                                                                                                                                                                                      
if test x$subversion = xyes ; then
   AC_CHECK_PROG(have_subversion, svn, yes, no)
                                                                                                                                                                                      
   if test x$have_subversion = xno ; then
     AC_MSG_WARN([*** Not rebuilding svn-release.h as subversion is not found ***])
     subversion=no
   fi
fi

AM_CONDITIONAL([USE_SUBVERSION], [test "x$subversion" = xyes])

AC_CONFIG_FILES([src/Makefile Makefile man/Makefile conf/Makefile doc/Makefile doc/README.html patches/Makefile ifplugd.spec])
AC_OUTPUT
