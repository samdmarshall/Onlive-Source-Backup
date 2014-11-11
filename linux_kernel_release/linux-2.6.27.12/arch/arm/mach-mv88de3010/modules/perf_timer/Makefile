include $(srctree)/arch/arm/mach-mv88de3010/mv88de3010.mk
ccflags-y	+= $(MV_DEFINE) $(HEADER_PATH)

GALOIS_FIRMWARE_DIR	= ../../GaloisSoftware

obj-$(CONFIG_MV88DE3010_PFTIMER_MODULE) += pftimer.o
pftimer-objs := perf_timer.o 
ifeq ($(CONFIG_MV88DE3010_PFTIMER_MODULE),m)
pftimer-objs += $(SOC_APB_DIR)/source/apb_timer.o 
endif

