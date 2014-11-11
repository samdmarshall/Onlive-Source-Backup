include $(src)/mv88de3010.mk

# Global flags passed to gcc
ccflags-y	+= $(MV_DEFINE) $(HEADER_PATH)
asflags-y	+= $(MV_DEFINE) $(HEADER_PATH)

# SOC_OBJS defines low-level driver objects that should be
# compiled and linked into U-Boot.
GALOIS_FIRMWARE_DIR     = GaloisSoftware
SOC_OBJS	:=
SOC_OBJS	+= $(SOC_PINMUX_DIR)/source/pinmux.o
SOC_OBJS	+= $(SOC_APB_DIR)/source/pic_driver.o 
SOC_OBJS	+= $(SOC_APB_DIR)/source/apb_ictl.o 
SOC_OBJS	+= $(SOC_APB_DIR)/source/apb_timer.o 
SOC_OBJS	+= $(SOC_DHUB_DIR)/source/hal_dhub.o
SOC_OBJS	+= $(SOC_APB_DIR)/source/apb_uart_driver.o
SOC_OBJS	+= $(SOC_APB_DIR)/source/eth.o
SOC_OBJS	+= $(SOC_MISC_DIR)/source/galois_speed.o

# COMMON_OBJS contains OS-specific API used by low-level drivers.
COMMON_OBJS	:=
COMMON_OBJS	+= $(SOC_COMMON_DIR)/source/galois_cache_flush.o

obj-y 	:= core.o time.o platform_devs.o sm_ictl.o pinmux.o

ifeq ($(CONFIG_MV88DE3010_DYNAMIC_FREQ),y)
  obj-y	+= freq.o
endif

obj-m 	:=
obj-n	:=
obj-	:=

#ifeq ($(CONFIG_MV88DE3010_BERLIN_UARTBUG),y)
obj-y	+= serial.o
#endif
obj-$(CONFIG_MV88DE3010_ETHERNET)	+= ethernet.o
obj-$(CONFIG_MV88DE3010_MTD_NAND)	+= mv_nand.o

obj-y	+= mv88de3010.o
mv88de3010-objs	:= $(COMMON_OBJS) $(SOC_OBJS)

obj-$(CONFIG_MV88DE3010_PCIE) += pcie.o
obj-$(CONFIG_MV88DE3010_CC_MODULE) += modules/MV_CC_Module/
obj-$(CONFIG_MV88DE3010_SHM) += modules/shm/
obj-$(CONFIG_MV88DE3010_CC) += modules/cc/
obj-$(CONFIG_MV88DE3010_PFTIMER_MODULE) += modules/perf_timer/
obj-$(CONFIG_MV88DE3010_REGMAP) += modules/regmap/
obj-$(CONFIG_MV_SATA_THOR) += sata/
obj-$(CONFIG_MV88DE3010_PWM) += modules/pwm/
obj-$(CONFIG_MV88DE3010_I2C) += modules/i2c/
obj-$(CONFIG_MV88DE3010_SPI) += modules/spi/
obj-$(CONFIG_MV88DE3010_IOMAPPER) += modules/iomapper/
obj-$(CONFIG_MV88DE3010_IR) += modules/ir/
obj-$(CONFIG_MV88DE3010_SM) += modules/sm/
obj-$(CONFIG_MV88DE3010_SOFTKEYBOARD) += modules/soft_keyboard/
obj-$(CONFIG_MV88DE3010_GPIO) += modules/gpio/
obj-$(CONFIG_MV88DE3010_USB_WLAN_8682) += modules/wlan_usb8682/
obj-$(CONFIG_MV88DE3010_USB_WLAN_8786) += modules/wlan_usb8786/
obj-$(CONFIG_MV88DE3010_SDIO_WLAN_8786) += modules/wlan_sd8786/
obj-$(CONFIG_MV88DE3010_SDIO_WLAN_8787) += modules/wlan_sd8787/
obj-$(CONFIG_MV88DE3010_DIRECTFB_FUSION) += modules/linux-fusion-8.1.1/
obj-$(CONFIG_MV88DE3010_MVPM) += modules/mvpm/
obj-$(CONFIG_MV88DE3010_PE_MODULE) += modules/pe/
obj-$(CONFIG_MV88DE3010_GPU) += modules/gpu/
obj-$(CONFIG_PM) += pm.o pm_ll.o
obj-y	+= reset.o

clean:
	rm -f $(SOBJS) $(OBJS)

