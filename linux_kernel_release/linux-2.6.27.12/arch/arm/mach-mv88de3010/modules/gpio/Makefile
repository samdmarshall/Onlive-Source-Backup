include $(srctree)/arch/arm/mach-mv88de3010/mv88de3010.mk
ccflags-y	+= $(MV_DEFINE) $(HEADER_PATH)

GALOIS_FIRMWARE_DIR = ../../GaloisSoftware

obj-$(CONFIG_MV88DE3010_GPIO) += kgpio.o
kgpio-objs := gpio_main.o
kgpio-objs += $(SOC_GPIO_DIR)/source/gpio.o

#ifeq ($(CONFIG_MV88DE3010_GPIO),m)
kgpio-objs += $(SOC_APB_DIR)/source/spi_master.o
#endif
