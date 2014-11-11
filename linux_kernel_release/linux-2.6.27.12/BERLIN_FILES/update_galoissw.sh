#!/bin/sh

GALOISSW=$1
if [ -z $GALOISSW ];  then
	echo "no galoissw"
	exit
fi

cp arch/arm/boot/uImage $GALOISSW/OS/Linux/ARM/linux_kernel/uImage.asic.b0
cp arch/arm/boot/Image $GALOISSW/OS/Linux/ARM/linux_kernel/uImage.asic.b0.linux.bin
make INSTALL_MOD_PATH=BERLIN_TOOLS modules_install
cp -rf BERLIN_TOOLS/lib $GALOISSW/OS/Linux/ARM/linux_kernel/linux_modules.b0/
rm -rf BERLIN_TOOLS/lib

echo ""
echo "===================================================================================="
echo "  remember to update linux SVN Revision in $GALOISSW/OS/Linux/ARM/linux_version.h"
echo "===================================================================================="
echo ""

