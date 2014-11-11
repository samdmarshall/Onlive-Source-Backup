######################################################
# Build configurations

include $(TOPDIR)/config

######################################################
# Module library

MODULE_LIBRARY = libGSocMISC.a

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

MODULE_CFLAGS = -I$(TOPDIR)/sys_monitor/source/

MODULE_LDFLAGS =

######################################################
# Common dependency process

include $(TOPDIR)/Scripts/mk_default.in

######################################################
# Special case for hal_diag.o

ifeq ("$(wildcard lib/CPU$(CPUINDEX)/hal_diag.o)","lib/CPU$(CPUINDEX)/hal_diag.o")
module_command:
	cp -f --preserve=timestamp lib/CPU$(CPUINDEX)/hal_diag.o $(SYS_BUILD_DIR)/lib/hal_diag.o
else
$(OBJDIR)source/hal_diag.o: source/hal_diag.c
	$(CC) $(Galois_CFLAGS) $(MODULE_CFLAGS) -c -o $@ $<
	cp -f --preserve=timestamp $@ $(SYS_BUILD_DIR)/lib
endif
ifeq ("$(audio_hw_app)", "enable")
MODULE_CFLAGS += -DAUDIO_HW_APP_INTR
endif
