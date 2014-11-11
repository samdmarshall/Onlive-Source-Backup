# $Id: Makefile.am 101 2005-11-18 20:39:12Z lennart $

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

EXTRA_DIST=bootstrap.sh LICENSE libdaemon.spec
SUBDIRS=libdaemon doc examples

MAINTAINERCLEANFILES = libdaemon.spec
noinst_DATA = libdaemon.spec

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libdaemon.pc

if USE_LYNX
noinst_DATA += README
EXTRA_DIST += README
MAINTAINERCLEANFILES += README

README:
	rm -f README
	$(MAKE) -C doc README
	ln -s doc/README README
endif

doxygen:
	$(MAKE) -C doc doxygen

homepage: dist doxygen
	test -d $$HOME/homepage/private
	mkdir -p $$HOME/homepage/private/projects/libdaemon
	cp libdaemon-@PACKAGE_VERSION@.tar.gz $$HOME/homepage/private/projects/libdaemon
	cp doc/README.html doc/style.css $$HOME/homepage/private/projects/libdaemon
	ln -sf README.html $$HOME/homepage/private/projects/libdaemon/index.html
	cp -av doc/reference/ $$HOME/homepage/private/projects/libdaemon/

distcleancheck:
	@:

.PHONY: homepage distcleancheck
