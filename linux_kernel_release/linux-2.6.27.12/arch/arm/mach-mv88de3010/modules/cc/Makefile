include $(srctree)/arch/arm/mach-mv88de3010/mv88de3010.mk
ccflags-y	+= $(MV_DEFINE) $(HEADER_PATH)

# Add your debugging flag (or not) to CFLAGS
ifeq ($(CONFIG_MV88DE3010_CC_DEBUG),y)
  DEBFLAGS = -O -g -DENABLE_DEBUG -DENABLE_DEBUG_OSAL # "-O" is needed to expand inlines
else
  DEBFLAGS = -O2
endif

ccflags-y += $(DEBFLAGS)

GALOIS_FIRMWARE_DIR = ../../GaloisSoftware
COMMON_SOURCE_DIR	= $(COMMON_DIR)/source
CC_SOURCE_DIR		= $(OSAL_DIR)/source_SingleCPU

obj-$(CONFIG_MV88DE3010_CC) += galois_cc.o

galois_cc-objs := cc_driver.o \
                    $(COMMON_SOURCE_DIR)/std_map.o \
                    $(CC_SOURCE_DIR)/Linux_Kernel/GSList.o \
                    $(CC_SOURCE_DIR)/Linux_Kernel/CC_CBuf_Body.o \
                    $(CC_SOURCE_DIR)/Linux_Kernel/CC_CBuf_Server.o \
                    $(CC_SOURCE_DIR)/Linux_Kernel/CCDSS.o \
                    $(CC_SOURCE_DIR)/Linux_Kernel/CCUDP.o \
                    $(CC_SOURCE_DIR)/Linux_Kernel/CC_MsgQ.o \
                    $(CC_SOURCE_DIR)/Linux_Kernel/CC_MsgQEx.o
                    
ifeq ($(CONFIG_MV88DE3010_CC),m)
galois_cc-objs +=
endif
