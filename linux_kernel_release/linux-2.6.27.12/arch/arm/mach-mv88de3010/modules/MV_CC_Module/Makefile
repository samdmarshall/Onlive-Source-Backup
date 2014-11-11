include $(srctree)/arch/arm/mach-mv88de3010/mv88de3010.mk
ccflags-y	+= $(MV_DEFINE) $(HEADER_PATH)
# Comment/uncomment the following line to disable/enable debugging
# DEBUG = y

# Add your debugging flag (or not) to CFLAGS
ifeq ($(DEBUG),y)
  DEBFLAGS = -O -g -DENABLE_DEBUG -DENABLE_DEBUG_OSAL # "-O" is needed to expand inlines
else
  DEBFLAGS = -O2
endif

ccflags-y += $(DEBFLAGS)

GALOIS_FIRMWARE_DIR	= ../../GaloisSoftware

obj-$(CONFIG_MV88DE3010_CC_MODULE) += MV_CC_Module.o
MV_CC_Module-objs := $(OSAL_DIR)/source_MultiCPU/Galois_Linux_Kernel/ICCHAL.o $(OSAL_DIR)/source_MultiCPU/ICCFIFO.o $(OSAL_DIR)/source_MultiCPU/ICCFIFOArray.o $(OSAL_DIR)/source_MultiCPU/Galois_Linux_Kernel/ICCNet.o $(COMMON_DIR)/source/std_map.o $(OSAL_DIR)/source_MultiCPU/GSList.o $(OSAL_DIR)/source_MultiCPU/Galois_Linux_Kernel/CCDSS.o main.o 
ifeq ($(CONFIG_MV88DE3010_CC_MODULE),m)
MV_CC_Module-objs += $(SOC_COMMON_DIR)/source/galois_cache_flush.o $(SOC_APB_DIR)/source/pic_driver.o $(SOC_APB_DIR)/source/apb_timer.o
endif
