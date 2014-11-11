
#!/bin/sh
module="MV_CC_Module"
device="MV_CC_SHM"
mode="664"

# Group: since distributions do it differently, look for wheel or use staff
if grep '^staff:' /etc/group > /dev/null; then
    group="staff"
else
    group="wheel"
fi

# invoke insmod with all arguments we got
# and use a pathname, as newer modutils don't look in . by default
/sbin/insmod ./$module.ko $* || exit 1

#major=`cat /proc/devices | awk "\\$2==\"$module\" {print \\$1}"`
#major=254

# Remove stale nodes and replace them, then give gid and perms
# Usually the script is shorter, it's simple that has several devices in it.

# Shared Memory for Non-Cache 
#rm -f /dev/${device}0 
#mknod /dev/${device}0 c $major 0
#chgrp $group /dev/${device}0
#chmod $mode  /dev/${device}0

# Shared Memory for Cache 
#rm -f /dev/${device}1 
#mknod /dev/${device}1 c $major 1
#chgrp $group /dev/${device}1
#chmod $mode  /dev/${device}1
