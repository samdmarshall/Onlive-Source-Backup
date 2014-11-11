include $(srctree)/arch/arm/mach-mv88de3010/mv88de3010.mk
ccflags-y	+= $(MV_DEFINE) $(HEADER_PATH)

ifeq ($(CONFIG_MV88DE3010_SHM_DEBUG),y)
  DEBFLAGS = -O -g -DENABLE_DEBUG -DENABLE_DEBUG_OSAL # "-O" is needed to expand inlines
else
  DEBFLAGS = -O2
endif

ccflags-y	+= $(DEBFLAGS)


GALOIS_FIRMWARE_DIR = ../../GaloisSoftware
SHM_SOURCE_DIR	    = $(SHM_DIR)/source/linux/kernel

obj-$(CONFIG_MV88DE3010_SHM) += galois_shm.o
galois_shm-objs := shm_driver.o $(SHM_SOURCE_DIR)/shm_device.o $(SHM_SOURCE_DIR)/memory_engine.o $(SHM_SOURCE_DIR)/shm_api_kernel.o

ifeq ($(CONFIG_MV88DE3010_SHM),m)
galois_shm-objs += $(SOC_COMMON_DIR)/source/galois_cache_flush.o
endif
