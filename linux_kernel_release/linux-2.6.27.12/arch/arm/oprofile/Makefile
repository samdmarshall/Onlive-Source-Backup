include $(TOPDIR)/.config
include $(TOPDIR)/arch/arm/mach-mv88de3010/mv88de3010.mk

# Global flags passed to gcc
EXTRA_CFLAGS    += $(MV_DEFINE) $(HEADER_PATH)
EXTRA_AFLAGS    += $(MV_DEFINE) $(HEADER_PATH)


obj-$(CONFIG_OPROFILE) += oprofile.o

DRIVER_OBJS = $(addprefix ../../../drivers/oprofile/, \
		oprof.o cpu_buffer.o buffer_sync.o \
		event_buffer.o oprofile_files.o \
		oprofilefs.o oprofile_stats.o \
		timer_int.o )

oprofile-y				:= $(DRIVER_OBJS) common.o backtrace.o
oprofile-$(CONFIG_CPU_XSCALE)		+= op_model_xscale.o
oprofile-$(CONFIG_OPROFILE_ARM11_CORE)	+= op_model_arm11_core.o
oprofile-$(CONFIG_OPROFILE_ARMV6)	+= op_model_v6.o
oprofile-$(CONFIG_OPROFILE_MPCORE)	+= op_model_mpcore.o
oprofile-$(CONFIG_ARCH_MV88DE3010)	+= op_model_88de3010.o
