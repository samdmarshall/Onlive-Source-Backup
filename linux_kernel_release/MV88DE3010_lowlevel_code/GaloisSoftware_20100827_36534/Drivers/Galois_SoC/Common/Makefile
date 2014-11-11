######################################################
# Build configurations

include $(TOPDIR)/config

######################################################
# Module library
MODULE_LIBRARY = libGSocCom.a

MODULE_SRCS = $(wildcard source/*.c)

MODULE_OBJS = $(patsubst %.c,%.o,$(patsubst %.s,%.o,$(MODULE_SRCS)))

######################################################
# Header files
ifeq ($(SoC), Berlin)
ifeq ($(SoC_Ver), BG2)
PUBLIC_HEADER_FILES = $(wildcard include/*.h) $(wildcard include/Firmware_Berlin_BG2/*.h)
else
ifeq ($(SoC_Ver), C0)
#ifeq ($(SoC_Subversion), 2)
ifeq ($(SoC_Subversion), $(filter 2 3, $(SoC_Subversion)))
PUBLIC_HEADER_FILES = $(wildcard include/*.h) $(wildcard include/Firmware_Berlin_C2/*.h)
else
PUBLIC_HEADER_FILES = $(wildcard include/*.h) $(wildcard include/Firmware_Berlin_C0/*.h)
endif
else
ifeq ($(SoC_Ver), B0)
PUBLIC_HEADER_FILES = $(wildcard include/*.h) $(wildcard include/Firmware_Berlin_B0/*.h)
else
PUBLIC_HEADER_FILES = $(wildcard include/*.h) $(wildcard include/Firmware_Berlin/*.h)
endif
endif
endif
else	#($(SoC),Galois)
PUBLIC_HEADER_FILES = $(wildcard include/*.h) $(wildcard include/Firmware/*.h)
endif

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

######################################################
# Special process for header file only module

all:

clean:

#install:
#	mkdir -p $(SYS_BUILD_DIR)/include/SoC $(SYS_BUILD_DIR)/include/SoC/Firmware
#	-cp -f --preserve=timestamp include/*.h $(SYS_BUILD_DIR)/include/SoC
#	-cp -f --preserve=timestamp include/Firmware/*.h $(SYS_BUILD_DIR)/include/SoC/Firmware
