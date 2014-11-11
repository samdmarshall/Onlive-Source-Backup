GALOIS_FW_DIR	= /home/galois/video-system/vidcode/GaloisSoftware
SOC_DIR			= $(GALOIS_FW_DIR)/Drivers/Galois_SoC
COMMON_DIR		= $(GALOIS_FW_DIR)/Common

# Low-level driver codes under SOC_DIR
SOC_APB_DIR		= $(SOC_DIR)/APB
SOC_COMMON_DIR	= $(SOC_DIR)/Common
SOC_DMA_DIR		= $(SOC_DIR)/DMA
SOC_MISC_DIR	= $(SOC_DIR)/MISC
SOC_PINMUX_DIR	= $(SOC_DIR)/PINMUX
SOC_DEVINIT_DIR = $(SOC_DIR)/DEV_INIT
SOC_VOP_DIR		= $(SOC_DIR)/VOP

# Include headers
HEADER_PATH		= -I$(SOC_APB_DIR)/include \
				  -I$(SOC_COMMON_DIR)/include -I$(SOC_COMMON_DIR)/include/Firmware \
				  -I$(SOC_DMA_DIR)/include \
				  -I$(SOC_MISC_DIR)/include \
				  -I$(SOC_PINMUX_DIR)/include \
				  -I$(SOC_DEVINIT_DIR)/include \
				  -I$(SOC_VOP_DIR)/include
HEADER_PATH		+= -I$(COMMON_DIR)/include
HEADER_PATH		+= -I$(CURRENT_DIR)/include

# Internal definitions
# __UBOOT__ is used in OS-independent header files 
MV_DEFINE = -D__UBOOT__ -D__CODE_LINK__=0 -mmrvl-use-iwmmxt -mcpu=iwmmxt

# Global flags passed to gcc
CFLAGS	+= $(MV_DEFINE) $(HEADER_PATH)

