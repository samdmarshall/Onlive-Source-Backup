# $Id: Makefile.am 125 2005-06-04 19:44:24Z lennart $

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

EXTRA_DIST=bootstrap.sh README LICENSE ifplugd.spec ifplugd.spec.in
SUBDIRS=src man conf doc patches

MAINTAINERCLEANFILES = README ifplugd.spec
noinst_DATA = README ifplugd.spec

README:
	rm -f README
	$(MAKE) -C doc README
	cd $(srcdir) && ln -s doc/README README

homepage: all dist
	test -d $$HOME/homepage/private
	mkdir -p $$HOME/homepage/private/projects/ifplugd
	cp  ifplugd-@PACKAGE_VERSION@.tar.gz $$HOME/homepage/private/projects/ifplugd
	cp man/ifplugd.8.xml man/ifplugstatus.8.xml man/ifplugd.conf.5.xml man/xmltoman.css man/xmltoman.xsl man/xmltoman.dtd doc/README.html doc/style.css doc/NEWS $$HOME/homepage/private/projects/ifplugd
	ln -sf README.html $$HOME/homepage/private/projects/ifplugd/index.html

distcleancheck:
	@:

.PHONY: homepage distcleancheck
