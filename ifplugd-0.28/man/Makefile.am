# $Id: Makefile.am 96 2004-02-01 01:04:52Z lennart $

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

man_MANS = ifplugd.8 ifplugd.conf.5 ifplugstatus.8

noinst_DATA = ifplugd.8.xml ifplugd.conf.5.xml ifplugstatus.8.xml  

EXTRA_DIST = $(man_MANS) ifplugd.8.xml.in ifplugd.conf.5.xml.in ifplugstatus.8.xml.in xmltoman.css xmltoman.xsl xmltoman.dtd

CLEANFILES = ifplugd.8.xml ifplugd.conf.5.xml ifplugstatus.8.xml  

ifplugd.8.xml: ifplugd.8.xml.in Makefile
	sed -e 's,@sysconfdir\@,$(sysconfdir),g' -e 's,@sbindir\@,$(sbindir),g' -e 's,@PACKAGE_BUGREPORT\@,$(PACKAGE_BUGREPORT),g' -e 's,@PACKAGE_URL\@,$(PACKAGE_URL),g' $< > $@

ifplugd.conf.5.xml: ifplugd.conf.5.xml.in Makefile
	sed -e 's,@sysconfdir\@,$(sysconfdir),g' -e 's,@sbindir\@,$(sbindir),g' -e 's,@PACKAGE_BUGREPORT\@,$(PACKAGE_BUGREPORT),g'  -e 's,@PACKAGE_URL\@,$(PACKAGE_URL),g' $< > $@

ifplugstatus.8.xml: ifplugstatus.8.xml.in Makefile
	sed -e 's,@sysconfdir\@,$(sysconfdir),g' -e 's,@sbindir\@,$(sbindir),g' -e 's,@PACKAGE_BUGREPORT\@,$(PACKAGE_BUGREPORT),g' -e 's,@PACKAGE_URL\@,$(PACKAGE_URL),g' $< > $@

if USE_XMLTOMAN

CLEANFILES += $(man_MANS)

ifplugd.8: ifplugd.8.xml Makefile
	xmltoman $< > $@

ifplugd.conf.5: ifplugd.conf.5.xml Makefile
	xmltoman $< > $@

ifplugstatus.8: ifplugstatus.8.xml Makefile
	xmltoman $< > $@

endif
