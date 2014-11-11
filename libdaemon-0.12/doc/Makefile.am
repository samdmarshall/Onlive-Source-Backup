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

noinst_DATA = README.html
EXTRA_DIST = $(noinst_DATA) style.css README.html.in

MAINTAINERCLEANFILES = README.html
CLEANFILES =

doxygen: doxygen.conf
	doxygen doxygen.conf

maintainer-clean-local:
	rm -rf reference

if USE_LYNX
noinst_DATA += README
MAINTAINERCLEANFILES += README
README: README.html
	lynx --dump $^ | sed 's,file://localhost/.*/doc/README.html,README,' | sed 's,file://localhost/.*/doc/reference/,reference/,' > $@

CLEANFILES += README
endif

tidy: README.html
	tidy -e < README.html

.PHONY: doxygen tidy
