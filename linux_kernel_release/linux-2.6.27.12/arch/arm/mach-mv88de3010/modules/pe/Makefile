include $(srctree)/arch/arm/mach-mv88de3010/mv88de3010.mk
ccflags-y	+= $(MV_DEFINE) $(HEADER_PATH)

ccflags-y	+= -DBERLIN_SINGLE_CPU

GALOIS_FIRMWARE_DIR = ../../GaloisSoftware
SHM_SOURCE_DIR	    = $(SHM_DIR)/source/linux/kernel


obj-$(CONFIG_MV88DE3010_PE_MODULE) += galois_pe.o
galois_pe-objs := pe_driver.o

ifeq ($(CONFIG_MV88DE3010_PE_MODULE),m)
galois_pe-objs += $(SOC_COMMON_DIR)/source/galois_cache_flush.o $(SOC_APB_DIR)/source/pic_driver.o $(SOC_APB_DIR)/source/apb_timer.o $(SOC_DHUB_DIR)/source/hal_dhub.o   $(SOC_DHUB_DIR)/source/avio_dhub_drv.o
endif

