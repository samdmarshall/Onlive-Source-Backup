cat include/linux/autoconf.h | grep CONFIG_MV88DE3010_BERLIN_B0 | grep ^[#]
if [ $? = 0 ]; then
	echo "correct one."
else
	echo "error tools, exit."
	exit
fi

# can be executed from 88 or 151: update 88 and 151 only.
cp -v arch/arm/boot/uImage /tftpboot/uImage.asic.b0
cp -v arch/arm/boot/Image /tftpboot/uImage.asic.b0.linux.bin
cp -v arch/arm/boot/uImage /tftpboot/88_tftpboot/uImage.asic.b0
cp -v arch/arm/boot/Image /tftpboot/88_tftpboot/uImage.asic.b0.linux.bin
cp -v arch/arm/boot/uImage /tftpboot/151_tftpboot/uImage.asic.b0
cp -v arch/arm/boot/Image /tftpboot/151_tftpboot/uImage.asic.b0.linux.bin
#cp arch/arm/boot/uImage /tftpboot/142_tftpboot/uImage.asic.b0
#cp arch/arm/boot/Image /tftpboot/142_tftpboot/uImage.asic.b0.linux.bin

make INSTALL_MOD_PATH=BERLIN_TOOLS modules_install
cp -rf BERLIN_TOOLS/lib /tftpboot/151_rootfs/home/galois/linux_modules/
cp -rf BERLIN_TOOLS/lib /tftpboot/88_nfsroot/home/galois/linux_modules/
cp -rf BERLIN_TOOLS/lib /home/galois/galois-rootfs/home/galois/linux_modules/
rm -rf BERLIN_TOOLS/lib
