# $Id: Makefile.am 109 2004-04-12 13:58:11Z lennart $

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

AM_CFLAGS = $(LIBDAEMON_CFLAGS) -DSYSCONFDIR="\"$(sysconfdir)\"" -D_GNU_SOURCE
LDADD = $(LIBDAEMON_LIBS)

sbin_PROGRAMS = ifplugd ifplugstatus

ifplugd_SOURCES = ifplugd.c \
			interface.c interface.h \
			ethtool-kernel.h ethtool-local.h \
			wireless.h wireless.15.h wireless.16.h \
			ifmonitor.h ifmonitor.c \
			nlapi.h nlapi.c \
			svn-revision.h

ifplugstatus_SOURCES = ifplugstatus.c \
			interface.c interface.h \
			ethtool-kernel.h ethtool-local.h \
			wireless.h wireless.15.h wireless.16.h \
			svn-revision.h

MAINTAINERCLEANFILES =
BUILT_SOURCES =

if USE_SUBVERSION
MAINTAINERCLEANFILES += svn-revision.h
BUILT_SOURCES += svn-revision.h
endif

ifstatus.$(OBJEXT): svn-revision.h
ifplugd.$(OBJEXT): svn-revision.h

if USE_SUBVERSION
svn-revision.h: Makefile
	if test -d "$(top_srcdir)/.svn" ; then \
		if REV=`svn info "$(top_srcdir)" | grep ^Revision | cut -f2 -d" "` 2> /dev/null ; then \
			echo -e "#ifndef foosvnrevisionhfoo\n#define foosvnrevisionhfoo\n#define SVN_REVISION \"$$REV\"\n#endif" > $@ ; \
	    fi \
	fi

svnkeywords:
	rm -f svn-revision.h
	svn propset svn:keywords Id *.c *.h Makefile.am

.PHONY: svnkeywords
endif

