include $(srctree)/arch/arm/mach-mv88de3010/mv88de3010.mk
ccflags-y	+= $(MV_DEFINE) $(HEADER_PATH)

GALOIS_FIRMWARE_DIR	= ../../GaloisSoftware

obj-$(CONFIG_MV88DE3010_SPI) += spi.o
spi-objs := spi_bus.o
