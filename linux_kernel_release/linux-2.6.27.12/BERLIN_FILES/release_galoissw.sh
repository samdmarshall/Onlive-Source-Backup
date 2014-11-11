#!/bin/bash

if [ $# != 1 -a $# != 2 ]; then
	echo "Usage: $0 <GALOISSW_DIR> <VERSION>"
	exit 1
fi

galoissw_dir=$1
sys_memmap_file=$galoissw_dir/Common/include/sys_memmap.h
memmap_lines=`cat $sys_memmap_file | grep '#define\ SYSMEM_SETTING' | sed s/\ // | sed s/SYSMEM_SETTING_/@/` 

MY_PATH=`dirname $0`

if [ $2 -eq 1 ]; then
	RELEASE_SCRIPT_PATH=$MY_PATH/../release.v1
	RELEASE_TARGET_DIR=$galoissw_dir/OS/Linux/ARM/linux_kernel/v1
	mkdir -p $RELEASE_TARGET_DIR
else
	RELEASE_SCRIPT_PATH=$MY_PATH/../release
	RELEASE_TARGET_DIR=$galoissw_dir/OS/Linux/ARM/linux_kernel
fi

for i in $memmap_lines; do
	memmap=`echo $i | cut -d @ -f 2`

	echo "Building linux image for $memmap"
	rm -rf $MY_PATH/uImage $MY_PATH/linux.bin
	$RELEASE_SCRIPT_PATH C $memmap . && \
	cp $MY_PATH/uImage \
		$RELEASE_TARGET_DIR/uImage.asic.c0.$memmap && \
	cp $MY_PATH/linux.bin \
		$RELEASE_TARGET_DIR/uImage.asic.c0.$memmap.linux.bin \
		|| exit 1

	echo "Building linux image for $memmap for Android"
	rm -rf $MY_PATH/uImage $MY_PATH/linux.bin
	$RELEASE_SCRIPT_PATH C $memmap . android && \
	cp $MY_PATH/uImage \
		$RELEASE_TARGET_DIR/uImage.asic.c0.android.$memmap && \
	cp $MY_PATH/linux.bin \
		$RELEASE_TARGET_DIR/uImage.asic.c0.android.$memmap.linux.bin \
		|| exit 1
done

