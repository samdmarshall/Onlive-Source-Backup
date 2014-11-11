include $(srctree)/arch/arm/mach-mv88de3010/mv88de3010.mk
ccflags-y	+= $(MV_DEFINE) $(HEADER_PATH)
# Compilation options
ccflags-y  += -I$(src)/include
ccflags-y  += -D__MV_DEBUG__
ccflags-y  += -D__AC_DBG__

$(obj)/spruce_microcode.o: $(src)/spruce_microcode.S $(obj)/spruce_microcode.bin

# Common Objects
COMM_OBJS       := com_util.o com_u64.o com_scsi.o com_tag.o
CORE_OBJS       := spruce_microcode.o core_exp.o core_init.o scsi2sata.o
LINUX_OBJS      := hba_mod.o hba_exp.o hba_timer.o linux_main.o   

GALOIS_SATA_OBJS    := $(COMM_OBJS) $(CORE_OBJS) $(LINUX_OBJS)

# Kernal module: mv61xx
obj-$(CONFIG_MV_SATA_THOR) := mv61xx.o
mv61xx-objs := $(GALOIS_SATA_OBJS)

