include $(srctree)/arch/arm/mach-mv88de3010/mv88de3010.mk
ccflags-y	+= $(MV_DEFINE) $(HEADER_PATH)

GALOIS_FIRMWARE_DIR	= ../../GaloisSoftware

obj-$(CONFIG_MV88DE3010_I2C) += i2c.o
i2c-objs := i2c_bus.o sw_i2c.o
i2c-objs += $(SOC_APB_DIR)/source/i2c_master.o 
