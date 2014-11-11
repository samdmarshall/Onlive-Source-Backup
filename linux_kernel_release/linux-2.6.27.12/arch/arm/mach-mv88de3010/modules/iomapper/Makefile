include $(srctree)/arch/arm/mach-mv88de3010/mv88de3010.mk
ccflags-y	+= $(MV_DEFINE) $(HEADER_PATH)

GALOIS_FIRMWARE_DIR	= ../../GaloisSoftware

obj-$(CONFIG_MV88DE3010_IOMAPPER) += iomapper.o
iomapper-objs := main.o 
ifeq ($(CONFIG_MV88DE3010_IOMAPPER),m)
iomapper-objs += $(SOC_APB_DIR)/source/spi_master.o 
endif

