######################################################
# Build configurations

include $(TOPDIR)/config

######################################################
# Module library

MODULE_LIBRARY = libCommon.a

ifeq ($(SoC), PCSim)
MODULE_SRCS = $(wildcard source/*.c)
else
MODULE_SRCS = $(wildcard source/*.c) $(wildcard source/*.s)
endif

MODULE_OBJS = $(patsubst %.c,%.o,$(patsubst %.s,%.o,$(MODULE_SRCS)))

######################################################
# Dynamic library

MODULE_DYN_LIB =

DYN_LIB_SRCS =

######################################################
# Header files

PUBLIC_HEADER_FILES = $(wildcard include/*.h)

######################################################
# Application

PROGRAM =

PROGRAM_SRCS =

PROGRAM_OBJS =

######################################################
# Module flags

MODULE_CFLAGS = -I$(TOPDIR)/Audio/AudioCommon/include

ifneq ($(dmx_figo), 0)
MODULE_CFLAGS += -DDMX_FIGO -I$(TOPDIR)/OSAL/include -I$(TOPDIR)/OSAL/include/CPU$(CPUINDEX) -I$(TOPDIR)/Drivers/Galois_SoC/Common/include 
endif

MODULE_LDFLAGS =

######################################################
# Unittest setting

UNITTEST_SRCS =

UNITTEST_OBJS =

######################################################
# Release setting

# released source files
RELEASE_SRCS =

# released header files
RELEASE_HEADER_FILES =

# released library
RELEASE_LIBRARY = MODULE_LIBRARY

# source files to build released library
RELEASE_LIB_SRCS = MODULE_SRCS

# object files to build released library
RELEASE_LIB_OBJS = MODULE_OBJS

######################################################
# Common dependency process

include $(TOPDIR)/Scripts/mk_default.in
