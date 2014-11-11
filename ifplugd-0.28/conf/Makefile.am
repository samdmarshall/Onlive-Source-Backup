# $Id: Makefile.am 76 2003-12-02 20:03:03Z lennart $

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

sysinitdir = @SYSINITDIR@

noinst_DATA = ifplugd.init ifplugd.action ifplugd.conf

EXTRA_DIST = ifplugd.init.in ifplugd.action ifplugd.conf

CLEANFILES = ifplugd.init

ifplugd.init: ifplugd.init.in Makefile
	sed -e 's,@sysconfdir\@,$(sysconfdir),g' -e 's,@sbindir\@,$(sbindir),g' $< > $@
	chmod 755 $@

install-data-local: ifplugd.init
	$(INSTALL) -b -D -m755 ifplugd.init ${DESTDIR}${sysinitdir}/ifplugd
	$(INSTALL) -b -D -m755 $(srcdir)/ifplugd.action ${DESTDIR}${sysconfdir}/ifplugd/ifplugd.action
	$(INSTALL) -b -D -m644 $(srcdir)/ifplugd.conf ${DESTDIR}${sysconfdir}/ifplugd/ifplugd.conf

uninstall-local:
	rm -f ${DESTDIR}${sysconfdir}/ifplugd/ifplugd.action ${DESTDIR}${sysconfdir}/ifplugd/ifplugd.conf ${DESTDIR}${sysinitdir}/ifplugd
