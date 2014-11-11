# File: Makefile
#
# Copyright (C) 2003-2008, Marvell International Ltd.
# All Rights Reserved

#############################################################################
# Configuration Options
#############################################################################

# Debug Option
# DEBUG LEVEL n/1/2:
# n: NO DEBUG
# 1: Only PRINTM(MSG,...), PRINTM(FATAL,...), ...
# 2: All PRINTM()
CONFIG_DEBUG=1

# Proc in /proc/net/wlan
CONFIG_PROC_DEBUG=y

# Re-association in driver
# CONFIG_REASSOCIATION=n

# Manufacturing firmware support
CONFIG_MFG_CMD_SUPPORT=n

# Big-endian platform
CONFIG_BIG_ENDIAN=n

# Milli seconds byfore operations on hardware
CONFIG_USB_DELAY=y

#############################################################################
# Compiler Flags
#############################################################################

EXTRA_CFLAGS += -I$(KERNELDIR)/include

EXTRA_CFLAGS += -DFPNUM='"41"'

ifeq ($(CONFIG_MV88DE3010_USB_WLAN_DEBUG),y)
	EXTRA_CFLAGS += -DDEBUG_LEVEL1
	EXTRA_CFLAGS += -DDEBUG_LEVEL2
	DBG=	-dbg
endif

ifeq ($(CONFIG_PROC_DEBUG),y)
	EXTRA_CFLAGS += -DPROC_DEBUG
	export CONFIG_PROC_DEBUG
endif

ifeq ($(CONFIG_MV88DE3010_USB_WLAN_REASSOCIATION),y)
	EXTRA_CFLAGS += -DREASSOCIATION
endif

ifeq ($(CONFIG_MFG_CMD_SUPPORT),y)
	EXTRA_CFLAGS += -DMFG_CMD_SUPPORT
endif

ifeq ($(CONFIG_BIG_ENDIAN),y)
	EXTRA_CFLAGS += -DBIG_ENDIAN
endif

ifeq ($(CONFIG_USB_DELAY),y)
	EXTRA_CFLAGS += -DUSB_DELAY
endif

#############################################################################
# Make Targets
#############################################################################

WLANOBJS = wlan/wlan_main.o wlan/wlan_init.o \
		wlan/wlan_wext.o \
		wlan/wlan_rx.o wlan/wlan_tx.o \
		wlan/wlan_cmd.o wlan/wlan_cmdresp.o \
		wlan/wlan_scan.o wlan/wlan_join.o

WLANOBJS += wlan/wlan_priv.o
WLANOBJS += wlan/wlan_wmm.o
WLANOBJS += wlan/wlan_11d.o
WLANOBJS += wlan/wlan_esupplicant.o

ifdef CONFIG_PROC_FS
WLANOBJS += wlan/wlan_proc.o
ifeq ($(CONFIG_PROC_DEBUG), y)
WLANOBJS += wlan/wlan_debug.o
endif
endif

WLANOBJS += wlan/wlan_usb.o
WLANOBJS += wlan/wlan_usb_bt.o
obj-m := usb8xxx.o
usb8xxx-objs := $(WLANOBJS)

# End of file
