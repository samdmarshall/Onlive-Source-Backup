######################################################
# Build configurations

include $(TOPDIR)/config

######################################################
# Module library

MODULE_LIBRARY = libGSocDEV_INIT.a

MODULE_SRCS = $(wildcard source/*.c)

MODULE_OBJS = $(patsubst %.c,%.o,$(patsubst %.s,%.o,$(MODULE_SRCS)))

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

MODULE_CFLAGS =

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
