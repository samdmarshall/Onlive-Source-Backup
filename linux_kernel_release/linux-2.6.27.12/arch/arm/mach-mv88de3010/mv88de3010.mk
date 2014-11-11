# GALOIS_FIRMWARE_DIR contains all firmware codes.
# SOC_DIR and COMMON_DIR may be changed according to GALOIS_FIRMWARE_DIR
# SOC_DIR and COMMON_DIR contain codes shared by Linux/eCOS/U-Boot
# GALOIS_FIRMWARE_DIR	= $(TOPDIR)/../../../../../vidcode/GaloisSoftware
GALOIS_FIRMWARE_DIR     = $(srctree)/arch/arm/mach-mv88de3010/GaloisSoftware
SOC_DIR			= $(GALOIS_FIRMWARE_DIR)/Drivers/Galois_SoC
COMMON_DIR		= $(GALOIS_FIRMWARE_DIR)/Common
OSAL_DIR		= $(GALOIS_FIRMWARE_DIR)/OSAL
SHM_DIR			= $(GALOIS_FIRMWARE_DIR)/SHM

# Low-level driver codes under SOC_DIR
SOC_APB_DIR	= $(SOC_DIR)/APB
SOC_COMMON_DIR	= $(SOC_DIR)/Common
SOC_DEVINIT_DIR	= $(SOC_DIR)/DEV_INIT
SOC_DHUB_DIR	= $(SOC_DIR)/DHUB
SOC_GPIO_DIR	= $(SOC_DIR)/GPIO
SOC_MISC_DIR	= $(SOC_DIR)/MISC
SOC_PINMUX_DIR	= $(SOC_DIR)/PINMUX
SOC_AVPLL_DIR 	= $(SOC_DIR)/AVPLL
SOC_MISC_DIR 	= $(SOC_DIR)/MISC

# Include headers
# BERLIN_A0 is asic, BERLIN_B0 is fpga.
ifeq ($(CONFIG_MV88DE3010_BERLIN_C0),y)
FIRMWARE = Firmware_Berlin_C0
else
ifeq ($(CONFIG_MV88DE3010_BERLIN_B0),y)
FIRMWARE = Firmware_Berlin_B0
else
ifeq ($(CONFIG_MV88DE3010_BERLIN),y)
FIRMWARE = Firmware_Berlin
else
FIRMWARE = Firmware
endif
endif
endif

HEADER_PATH :=	-I$(SOC_APB_DIR)/include \
		-I$(SOC_COMMON_DIR)/include \
		-I$(SOC_COMMON_DIR)/include/$(FIRMWARE) \
		-I$(SOC_DHUB_DIR)/include \
		-I$(SOC_MISC_DIR)/include \
		-I$(SOC_PINMUX_DIR)/include \
		-I$(SOC_DEVINIT_DIR)/include \
		-I$(SOC_GPIO_DIR)/include	\
		-I$(SOC_MISC_DIR)/include 
HEADER_PATH +=	-I$(COMMON_DIR)/include
HEADER_PATH +=	-I$(OSAL_DIR)/include \
		-I$(OSAL_DIR)/include/CPU0
HEADER_PATH +=	-I$(SHM_DIR)/include \
		-I$(SHM_DIR)/source/linux/include \
		-I$(SHM_DIR)/source/linux/kernel

ifeq ($(CONFIG_MV88DE3010_CC),y)
HEADER_PATH +=	-I$(OSAL_DIR)/source_SingleCPU/ \
		-I$(OSAL_DIR)/source_SingleCPU/Linux_Kernel
endif
ifeq ($(CONFIG_MV88DE3010_CC),m)
HEADER_PATH +=	-I$(OSAL_DIR)/source_SingleCPU/ \
		-I$(OSAL_DIR)/source_SingleCPU/Linux_Kernel
endif
ifeq ($(CONFIG_MV88DE3010_CC_MODULE),y)
HEADER_PATH +=	-I$(OSAL_DIR)/source_MultiCPU \
		-I$(OSAL_DIR)/source_MultiCPU/Galois_Linux_Kernel
endif

# Include pseudo headers
#HEADER_PATH +=	-I$(MV88DE3010_DIR)/include

# Internal definitions
# __LINUX_KERNEL__ is used in OS-independent header files 
MV_DEFINE := -DSoC_Galois -D__LINUX_KERNEL__ -D__CODE_LINK__=0
MV_DEFINE += -DBERLIN_B_0=0x1000 -DBERLIN_C_0=0x2000 -DBERLIN_A_0=0x0000 -DBERLIN_A_1=0x0010 -DBERLIN_A_2=0x0020 -DBERLIN_BG2=0x3000

ifeq ($(CONFIG_MV88DE3010_BERLIN_CPU1),y)
MV_DEFINE += -DCPUINDEX=1
else
MV_DEFINE += -DCPUINDEX=0
endif

ifeq ($(CONFIG_MV88DE3010_BERLIN),y)
MV_DEFINE += -DBERLIN
endif

# define BERLIN_CHIP_VERSION
ifeq ($(CONFIG_MV88DE3010_BERLIN_C0),y)
MV_DEFINE += -DBERLIN_CHIP_VERSION=BERLIN_C_0
else
ifeq ($(CONFIG_MV88DE3010_BERLIN_B0),y)
MV_DEFINE += -DBERLIN_CHIP_VERSION=BERLIN_B_0
endif
endif

# BERLIN_A0 is asic, BERLIN_B0 is fpga.
ifeq ($(CONFIG_MV88DE3010_ASIC),y)
ifeq ($(CONFIG_MV88DE3010_BERLIN_A0),y)
MV_DEFINE += -DBERLIN_PEK -DBERLIN_CHIP_VERSION=BERLIN_A_0
else
MV_DEFINE += -DSOC_ES1
endif
endif

# SVN version info
LINUXKERNEL_SVN_VERSION = $(shell svn info $(srctree) 2> /dev/null | \
						  grep Revision 2> /dev/null | \
						  awk '{print $$2}' 2> /dev/null)
GALOISSOFTWARE_SVN_VERSION = $(shell svn info `readlink $(srctree)/arch/arm/mach-mv88de3010/GaloisSoftware 2> /dev/null` 2> /dev/null | \
							 grep Revision 2> /dev/null | \
							 awk '{print $$2}' 2> /dev/null)
MV_DEFINE += -DLINUXKERNEL_SVN_VERSION=$(LINUXKERNEL_SVN_VERSION)
MV_DEFINE += -DGALOISSOFTWARE_SVN_VERSION=$(GALOISSOFTWARE_SVN_VERSION)

