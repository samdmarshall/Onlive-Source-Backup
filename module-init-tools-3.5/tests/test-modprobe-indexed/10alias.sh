#! /bin/sh
# Test for modules.alias usage.

for BITNESS in 32 64; do

MODTEST_OVERRIDE1=/lib/modules/$MODTEST_UNAME/modules.dep
MODTEST_OVERRIDE_WITH1=tests/tmp/modules.dep
export MODTEST_OVERRIDE1 MODTEST_OVERRIDE_WITH1

MODTEST_OVERRIDE2=/lib/modules/$MODTEST_UNAME/modules.dep.bin
MODTEST_OVERRIDE_WITH2=tests/tmp/modules.dep.bin
export MODTEST_OVERRIDE2 MODTEST_OVERRIDE_WITH2

MODTEST_OVERRIDE3=/etc/modprobe.conf
MODTEST_OVERRIDE_WITH3=tests/tmp/modprobe.conf
export MODTEST_OVERRIDE3 MODTEST_OVERRIDE_WITH3

MODTEST_OVERRIDE4=/lib/modules/$MODTEST_UNAME/modules.alias
MODTEST_OVERRIDE_WITH4=tests/tmp/modules.alias
export MODTEST_OVERRIDE4 MODTEST_OVERRIDE_WITH4

MODTEST_OVERRIDE5=/lib/modules/$MODTEST_UNAME/modules.alias.bin
MODTEST_OVERRIDE_WITH5=tests/tmp/modules.alias.bin
export MODTEST_OVERRIDE5 MODTEST_OVERRIDE_WITH5

MODTEST_OVERRIDE6=/lib/modules/$MODTEST_UNAME/modules.symbols
MODTEST_OVERRIDE_WITH6=/dev/null
export MODTEST_OVERRIDE6 MODTEST_OVERRIDE_WITH6

MODTEST_OVERRIDE7=/lib/modules/$MODTEST_UNAME/kernel/alias-$BITNESS.ko
MODTEST_OVERRIDE_WITH7=tests/data/$BITNESS/alias/alias-$BITNESS.ko
export MODTEST_OVERRIDE7 MODTEST_OVERRIDE_WITH7

MODTEST_OVERRIDE8=/lib/modules/$MODTEST_UNAME/kernel/foo.ko
MODTEST_OVERRIDE_WITH8=tests/tmp/foo.ko
export MODTEST_OVERRIDE8 MODTEST_OVERRIDE_WITH8

MODTEST_OVERRIDE9=/etc/modprobe.d
MODTEST_OVERRIDE_WITH9=NOSUCHFILENAME
export MODTEST_OVERRIDE9 MODTEST_OVERRIDE_WITH9

SIZE=$(echo `wc -c < tests/data/$BITNESS/alias/alias-$BITNESS.ko`)

echo "alias_$BITNESS /lib/modules/$MODTEST_UNAME/kernel/alias-$BITNESS.ko:" > tests/tmp/modules.dep.bin.temp
echo "foo /lib/modules/$MODTEST_UNAME/kernel/foo.ko:" >> tests/tmp/modules.dep.bin.temp
modindex -o tests/tmp/modules.dep.bin < tests/tmp/modules.dep.bin.temp

rm -f tests/tmp/modules.alias
rm -f tests/tmp/modules.alias.bin
rm -f tests/tmp/modprobe.conf

echo Test > tests/tmp/foo.ko

# Shouldn't complain if can't open modules.alias
[ "`modprobe bar 2>&1`" = "FATAL: Module bar not found." ]

# Now, alias found in modules.alias works.
echo "bar alias_$BITNESS" > tests/tmp/modules.alias.bin.temp
modindex -o tests/tmp/modules.alias.bin < tests/tmp/modules.alias.bin.temp
[ "`modprobe bar 2>&1`" = "INIT_MODULE: $SIZE " ]

# Normal alias should override it.
echo 'alias bar foo' > tests/tmp/modprobe.conf
[ "`modprobe foo 2>&1`" = "INIT_MODULE: 5 " ]

# If there's a real module, alias from modules.alias must NOT override.
echo "foo alias_$BITNESS" > tests/tmp/modules.alias.bin.temp
modindex -o tests/tmp/modules.alias.bin < tests/tmp/modules.alias.bin.temp
[ "`modprobe foo 2>&1`" = "INIT_MODULE: 5 " ]

# If there's an install command, modules.alias must not override.
echo 'install bar echo foo' > tests/tmp/modprobe.conf
[ "`modprobe bar 2>&1`" = "SYSTEM: echo foo" ]
echo 'remove bar echo foo remove' > tests/tmp/modprobe.conf
[ "`modprobe -r bar 2>&1`" = "SYSTEM: echo foo remove" ]

# Should gather up options from other alias name as well.
echo "bar alias_$BITNESS" > tests/tmp/modules.alias.bin.temp
modindex -o tests/tmp/modules.alias.bin < tests/tmp/modules.alias.bin.temp
echo "options bar option1" > tests/tmp/modprobe.conf
echo "options alias_$BITNESS option2" >> tests/tmp/modprobe.conf
[ "`modprobe bar 2>&1`" = "INIT_MODULE: $SIZE option1 option2" ]

# Duplicated alias: both get probed (either order)
echo "bar foo" >> tests/tmp/modules.alias.bin.temp
modindex -o tests/tmp/modules.alias.bin < tests/tmp/modules.alias.bin.temp
OUT="`modprobe bar 2>&1`"

[ "$OUT" = "INIT_MODULE: $SIZE option1 option2
INIT_MODULE: 5 option1" ] || [ "$OUT" = "INIT_MODULE: 5 option1
INIT_MODULE: $SIZE option1 option2" ]
done
