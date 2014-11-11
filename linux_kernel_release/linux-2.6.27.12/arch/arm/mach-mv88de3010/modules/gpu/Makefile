include $(srctree)/arch/arm/mach-mv88de3010/mv88de3010.mk
ccflags-y	+= $(MV_DEFINE) $(HEADER_PATH)
ccflags-y	+= -DBERLIN_SINGLE_CPU
#ccflags-y	+= -I../GaloisSoftware/GFXDS_gc100/GfxLinux/inc/


GALOIS_FIRMWARE_DIR	= ../../GaloisSoftware

#obj-y += gfx.o
obj-$(CONFIG_MV88DE3010_GPU) += gfx.o
gfx-objs := gfx_linux_device.o  gfx_linux_dispatch.o  gfx_linux_kernel.o $(SOC_APB_DIR)/source/pic_driver.o

