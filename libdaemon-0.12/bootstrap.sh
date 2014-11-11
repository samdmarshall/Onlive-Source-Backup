# $Id: bootstrap.sh 89 2005-08-24 22:57:12Z lennart $

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

FLAGS="--sysconfdir=/etc"

case `uname -s` in
    Darwin)
    export LIBTOOLIZE=/opt/local/bin/glibtoolize
    export PKG_CONFIG_PATH="/opt/local/lib/pkgconfig"
    FLAGS="$FLAGS --prefix=/opt/local"
    ;;
    FreeBSD)
    cp /usr/local/share/aclocal/libtool15.m4 common/
    ;;
esac

CFLAGS="$CFLAGS -g -O0" exec ./autogen.sh $FLAGS "$@"
