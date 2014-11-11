#! /bin/sh
# Test --show-depends.

for BITNESS in 32 64; do

# Inputs
MODTEST_OVERRIDE1=/lib/modules/$MODTEST_UNAME
MODTEST_OVERRIDE_WITH1=tests/data/$BITNESS/normal
export MODTEST_OVERRIDE1 MODTEST_OVERRIDE_WITH1

MODTEST_OVERRIDE2=/lib/modules/$MODTEST_UNAME/export_dep-$BITNESS.ko
MODTEST_OVERRIDE_WITH2=tests/data/$BITNESS/normal/export_dep-$BITNESS.ko
export MODTEST_OVERRIDE2 MODTEST_OVERRIDE_WITH2

MODTEST_OVERRIDE3=/lib/modules/$MODTEST_UNAME/noexport_dep-$BITNESS.ko
MODTEST_OVERRIDE_WITH3=tests/data/$BITNESS/normal/noexport_dep-$BITNESS.ko
export MODTEST_OVERRIDE3 MODTEST_OVERRIDE_WITH3

MODTEST_OVERRIDE4=/lib/modules/$MODTEST_UNAME/noexport_nodep-$BITNESS.ko
MODTEST_OVERRIDE_WITH4=tests/data/$BITNESS/normal/noexport_nodep-$BITNESS.ko
export MODTEST_OVERRIDE4 MODTEST_OVERRIDE_WITH4

MODTEST_OVERRIDE5=/lib/modules/$MODTEST_UNAME/export_nodep-$BITNESS.ko
MODTEST_OVERRIDE_WITH5=tests/data/$BITNESS/normal/export_nodep-$BITNESS.ko
export MODTEST_OVERRIDE5 MODTEST_OVERRIDE_WITH5

MODTEST_OVERRIDE6=/lib/modules/$MODTEST_UNAME/noexport_doubledep-$BITNESS.ko
MODTEST_OVERRIDE_WITH6=tests/data/$BITNESS/normal/noexport_doubledep-$BITNESS.ko
export MODTEST_OVERRIDE6 MODTEST_OVERRIDE_WITH6

MODTEST_OVERRIDE7=/lib/modules/$MODTEST_UNAME/modules.dep
MODTEST_OVERRIDE_WITH7=tests/tmp/modules.dep
export MODTEST_OVERRIDE7 MODTEST_OVERRIDE_WITH7

MODTEST_OVERRIDE8=/etc/modprobe.conf
MODTEST_OVERRIDE_WITH8=tests/tmp/modprobe.conf
export MODTEST_OVERRIDE8 MODTEST_OVERRIDE_WITH8

MODTEST_OVERRIDE9=/lib/modules/$MODTEST_UNAME/modules.dep.bin
MODTEST_OVERRIDE_WITH9=tests/tmp/modules.dep.bin
export MODTEST_OVERRIDE9 MODTEST_OVERRIDE_WITH9

MODTEST_OVERRIDE10=/proc/modules
MODTEST_OVERRIDE_WITH10=tests/tmp/proc
export MODTEST_OVERRIDE10 MODTEST_OVERRIDE_WITH10

# Now create modules.dep.bin
cat > tests/tmp/modules.dep.temp <<EOF
noexport_nodep_$BITNESS /lib/modules/2.5.52/noexport_nodep-$BITNESS.ko:
noexport_doubledep_$BITNESS /lib/modules/2.5.52/noexport_doubledep-$BITNESS.ko: /lib/modules/2.5.52/export_dep-$BITNESS.ko /lib/modules/2.5.52/export_nodep-$BITNESS.ko
noexport_dep_$BITNESS /lib/modules/2.5.52/noexport_dep-$BITNESS.ko: /lib/modules/2.5.52/export_nodep-$BITNESS.ko
export_nodep_$BITNESS /lib/modules/2.5.52/export_nodep-$BITNESS.ko:
export_dep_$BITNESS /lib/modules/2.5.52/export_dep-$BITNESS.ko: /lib/modules/2.5.52/export_nodep-$BITNESS.ko
EOF
# Build modules.dep.bin, which modprobe will prefer
modindex -o tests/tmp/modules.dep.bin < tests/tmp/modules.dep.temp

# Empty proc
cp /dev/null tests/tmp/proc

[ "`modprobe --show-depends noexport_nodep-$BITNESS 2>>tests/tmp/stderr`" = "insmod /lib/modules/2.5.52/noexport_nodep-$BITNESS.ko " ]
[ "`modprobe --show-depends export_nodep-$BITNESS 2>>tests/tmp/stderr`" = "insmod /lib/modules/2.5.52/export_nodep-$BITNESS.ko " ]
[ "`modprobe --show-depends noexport_dep-$BITNESS 2>>tests/tmp/stderr`" = "insmod /lib/modules/2.5.52/export_nodep-$BITNESS.ko 
insmod /lib/modules/2.5.52/noexport_dep-$BITNESS.ko " ]
[ "`modprobe --show-depends export_dep-$BITNESS 2>>tests/tmp/stderr`" = "insmod /lib/modules/2.5.52/export_nodep-$BITNESS.ko 
insmod /lib/modules/2.5.52/export_dep-$BITNESS.ko " ]
[ "`modprobe --show-depends noexport_doubledep-$BITNESS 2>>tests/tmp/stderr`" = "insmod /lib/modules/2.5.52/export_nodep-$BITNESS.ko 
insmod /lib/modules/2.5.52/export_dep-$BITNESS.ko 
insmod /lib/modules/2.5.52/noexport_doubledep-$BITNESS.ko " ]

# Nothing in stderr...
[ `wc -c < tests/tmp/stderr` = 0 ]
done
