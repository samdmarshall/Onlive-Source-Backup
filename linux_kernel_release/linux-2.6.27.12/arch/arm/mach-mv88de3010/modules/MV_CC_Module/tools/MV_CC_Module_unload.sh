#!/bin/sh
module="MV_CC_Module"
device="MV_CC_SHM"

# invoke rmmod with all arguments we got
/sbin/rmmod $module $* || exit 1

# Remove stale nodes
#rm -f /dev/${device}[rn] 
#rm -f /dev/${device}0
#rm -f /dev/${device}1




