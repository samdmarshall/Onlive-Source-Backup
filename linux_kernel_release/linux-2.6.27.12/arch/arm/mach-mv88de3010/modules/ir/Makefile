include $(srctree)/arch/arm/mach-mv88de3010/mv88de3010.mk
ccflags-y	+= $(MV_DEFINE) $(HEADER_PATH)

GALOIS_FIRMWARE_DIR	= ../../GaloisSoftware

obj-$(CONFIG_MV88DE3010_IR) += ir.o
ir-objs := ir_main.o 

ifneq ($(CONFIG_MV88DE3010_IR_SM),y)
ir-objs += $(SOC_APB_DIR)/source/apb_gpio_ir.o 
endif

ifeq ($(CONFIG_MV88DE3010_IR),m)
ir-objs += $(SOC_APB_DIR)/source/apb_timer.o 
endif

