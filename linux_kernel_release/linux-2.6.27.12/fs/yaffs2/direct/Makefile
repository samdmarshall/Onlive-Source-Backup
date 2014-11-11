# Makefile for YAFFS direct test
#
#
# YAFFS: Yet another Flash File System. A NAND-flash specific file system.
#
# Copyright (C) 2003 Aleph One Ltd.
#
#
# Created by Charles Manning <charles@aleph1.co.uk>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#
# NB Warning this Makefile does not include header dependencies.
#
# $Id: Makefile,v 1.18 2008/07/21 01:03:19 charles Exp $

#EXTRA_COMPILE_FLAGS = -DYAFFS_IGNORE_TAGS_ECC

CFLAGS =      -DCONFIG_YAFFS_DIRECT -DCONFIG_YAFFS_SHORT_NAMES_IN_RAM -DCONFIG_YAFFS_YAFFS2  
CFLAGS +=     -DCONFIG_YAFFSFS_PROVIDE_VALUES -DNO_Y_INLINE
CFLAGS +=     -DCONFIG_YAFFS_PROVIDE_DEFS
CFLAGS +=    -Wall -g $(EXTRA_COMPILE_FLAGS)
#CFLAGS+=    -fstack-check -O0

#CFLAGS+=   -Wshadow -Wpointer-arith -Wwrite-strings -Wstrict-prototypes -Wmissing-declarations
#CFLAGS+=   -Wmissing-prototypes -Wredundant-decls -Wnested-externs -Winline

YAFFS_SVN_VERSION = $(shell svn info .. 2> /dev/null | \
						  grep Revision 2> /dev/null | \
						  awk '{print $$2}' 2> /dev/null)
CFLAGS += -DYAFFS_SVN_VERSION=$(YAFFS_SVN_VERSION)

YAFFS_ACP = 1

ifeq ($(YAFFS_ACP),1)
  CFLAGS += -DYAFFS_ACP
endif

YAFFSLIBOBJS = yaffs_ecc.o yaffsfs.o yaffs_guts.o \
		 yaffs_packedtags1.o \
		 yaffs_tagscompat.o yaffs_packedtags2.o yaffs_tagsvalidity.o yaffs_nand.o \
		 yaffs_checkptrw.o  yaffs_qsort.o

ifeq ($(YAFFS_ACP),1)
  YAFFSLIBOBJS += rlist.o yaffs_ACP.o yaffscrc.o
endif

DIRECTTESTOBJS = dtest.o yaffscfg2k.o yaffs_fileem.o yaffs_fileem2k.o yaffs_ramdisk.o yaffs_ramem2k.o

#DIRECTTESTOBJS = dtest.o yaffscfg2k.o yaffs_ecc.o yaffs_fileem.o yaffs_fileem2k.o yaffsfs.o yaffs_guts.o \
#		 yaffs_packedtags1.o yaffs_ramdisk.o yaffs_ramem2k.o \
#		 yaffs_tagscompat.o yaffs_packedtags2.o yaffs_tagsvalidity.o yaffs_nand.o \
#		 yaffs_checkptrw.o  yaffs_qsort.o
#		 yaffs_checkptrwtest.o\

#ifeq ($(YAFFS_ACP),1)
#  DIRECTTESTOBJS += rlist.o yaffs_ACP.o
#endif

FSTESTOBJS = fstest.o fstestcfg.o fstest_fileem.o

BOOTTESTOBJS = bootldtst.o yboot.o nand_ecc.o

#ALLOBJS =  dtest.o nand_ecc.o yaffscfg.o yaffs_fileem.o yaffsfs.o yaffs_ramdisk.o bootldtst.o yboot.o yaffs_ramem2k.o

ALLOBJS = $(DIRECTTESTOBJS) $(BOOTTESTOBJS) $(FSTESTOBJS) $(YAFFSLIBOBJS)

SYMLINKS = devextras.h yaffs_ecc.c yaffs_ecc.h yaffs_guts.c yaffs_guts.h yaffsinterface.h yportenv.h yaffs_tagscompat.c yaffs_tagscompat.h \
          yaffs_packedtags1.c yaffs_packedtags1.h yaffs_packedtags2.c yaffs_packedtags2.h  yaffs_nandemul2k.h \
          yaffs_nand.c yaffs_nand.h yaffs_getblockinfo.h \
          yaffs_tagsvalidity.c yaffs_tagsvalidity.h yaffs_checkptrw.h yaffs_checkptrw.c \
          yaffs_qsort.c yaffs_qsort.h

ifeq ($(YAFFS_ACP),1)
  SYMLINKS += rlist.c yaffs_ACP.c
endif

#all: directtest2k boottest

all: directtest2k fstest

$(ALLOBJS): %.o: %.c
	gcc -c $(CFLAGS) -o $@ $<

$(SYMLINKS):
	ln -s ../$@ $@

directtest2k: $(SYMLINKS) $(DIRECTTESTOBJS) $(YAFFSLIBOBJS)
	gcc -o $@ $(DIRECTTESTOBJS)  $(YAFFSLIBOBJS)

fstest: $(SYMLINKS) $(FSTESTOBJS) $(YAFFSLIBOBJS)
	gcc -o $@ $(FSTESTOBJS)  $(YAFFSLIBOBJS)

boottest: $(SYMLINKS) $(BOOTTESTOBJS)
	gcc -o $@ $(BOOTTESTOBJS)


clean:
	rm -f $(ALLOBJS) core $(SYMLINKS)
	rm -f *.o
