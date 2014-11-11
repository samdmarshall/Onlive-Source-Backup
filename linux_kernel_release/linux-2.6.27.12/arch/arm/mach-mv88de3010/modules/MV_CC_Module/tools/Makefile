CC = arm-marvell-linux-gnueabi-gcc
LD = arm-marvell-linux-gnueabi-ld

# You must have such a dir tree: xxx_dir => {vidcode, videolinux}
# GALOIS_TOPDIR is like: /home/yfzheng/video_rd.
GALOIS_TOPDIR = $(PWD)/../../../../../../../..
SYSROOT_DIR = $(GALOIS_TOPDIR)/vidcode/GaloisSoftware/OS/Linux/ARM/rootfs

GALOIS_FIRMWARE_DIR		= $(GALOIS_TOPDIR)/vidcode/GaloisSoftware
GALOIS_HEADER_PATH		= -I$(GALOIS_FIRMWARE_DIR)/Drivers/Galois_SoC/APB/include \
						  -I$(GALOIS_FIRMWARE_DIR)/Drivers/Galois_SoC/Common/include \
						  -I$(GALOIS_FIRMWARE_DIR)/Drivers/Galois_SoC/Common/include/Firmware
GALOIS_HEADER_PATH		+= -I$(GALOIS_FIRMWARE_DIR)/Common/include

#__LINUX__ is for GALOIS_FIRMWARE_DIR headers
CFLAGS = --sysroot=$(SYSROOT_DIR) -I$(GALOIS_HEADER_PATH) -I.. -D__LINUX__ 

all:
	$(CC) $(CFLAGS) -c -o test_netlink-recv.o test_netlink-recv.c
	$(CC) $(CFLAGS) -c -o test_netlink-send.o test_netlink-send.c
	$(CC) $(CFLAGS) -c -o testtool.o testtool.c

	$(CC) $(CFLAGS) -o test_netlink-recv test_netlink-recv.o testtool.o
	$(CC) $(CFLAGS) -o test_netlink-send test_netlink-send.o testtool.o

	$(CC) $(CFLAGS) -c -o test_MV_CC_SHM.o test_MV_CC_SHM.c
	$(CC) $(CFLAGS) -o test_MV_CC_SHM test_MV_CC_SHM.o
	
	$(CC) $(CFLAGS) -c -o test_MV_CC_SHM_Write.o test_MV_CC_SHM_Write.c
	$(CC) $(CFLAGS) -o test_MV_CC_SHM_Write test_MV_CC_SHM_Write.o

	$(CC) $(CFLAGS) -c -o test_MV_CC_SHM_Read.o test_MV_CC_SHM_Read.c
	$(CC) $(CFLAGS) -o test_MV_CC_SHM_Read test_MV_CC_SHM_Read.o

clean:
	rm -rf *.o test_netlink-recv test_netlink-send test_MV_CC_SHM test_MV_CC_SHM_Write test_MV_CC_SHM_Read
