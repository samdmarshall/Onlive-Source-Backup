#The script has to be run at MV88DE3010_lowlevel_code direcotry.

GALOISSW_REV=$1

error_exit() {
	exit 1
}

checkout_code() {
	svn export $CO_PARAM $1
}

if [ "$GALOISSW_REV" = "" ]; then
	GALOISSW_REV=`svn info http://svn-02.marvell.com/repos/vidcode/GaloisSoftware/ | grep Revision | awk '{print $2}'`
	echo "Checking out latest GaloisSoftware Trunk(rev.$GALOISSW_REV) Header files"
else
	echo "Checking out GaloisSoftware Trunk rev.$GALOISSW_REV Header files"
fi
CO_PARAM="-r$GALOISSW_REV"

# create a new GaloisSoftware Directory
CURRENT_DATE=`date +%Y%m%d`
CURRENT_DIR=`pwd`
GALOISSW_DIR=$CURRENT_DIR/GaloisSoftware_"$CURRENT_DATE"_$GALOISSW_REV
echo "Creating directory $GALOISSW_DIR"
mkdir -p $GALOISSW_DIR

# check if low level code dir exist.
ls $GALOISSW_DIR
if [ $? != '0' ]; then
	echo "No $GALOISSW_DIR, exit!"
	error_exit
fi

# code checkout needs some sleep,
# otherwise unde slow network environment, it will fail
# create Common and OSAL
cd $GALOISSW_DIR
checkout_code http://svn-02.marvell.com/repos/vidcode/GaloisSoftware/Common || error_exit
sleep 2
checkout_code http://svn-02.marvell.com/repos/vidcode/GaloisSoftware/OSAL || error_exit
sleep 2
checkout_code http://svn-02.marvell.com/repos/vidcode/GaloisSoftware/SHM || error_exit
sleep 2
mkdir -p $GALOISSW_DIR/Drivers/Galois_SoC

# check if low level code dir exist.
ls $GALOISSW_DIR/Drivers/Galois_SoC
if [ $? != '0' ]; then
	echo "No $GALOISSW_DIR/Drivers/Galois_SoC, exit!"
	exit
fi

# create Drivers/Galois_SoC/*
cd $GALOISSW_DIR/Drivers/Galois_SoC
checkout_code http://svn-02.marvell.com/repos/vidcode/GaloisSoftware/Drivers/Galois_SoC/APB || error_exit
sleep 2
checkout_code http://svn-02.marvell.com/repos/vidcode/GaloisSoftware/Drivers/Galois_SoC/Common || error_exit
sleep 2
checkout_code http://svn-02.marvell.com/repos/vidcode/GaloisSoftware/Drivers/Galois_SoC/DEV_INIT || error_exit
sleep 2
checkout_code http://svn-02.marvell.com/repos/vidcode/GaloisSoftware/Drivers/Galois_SoC/DHUB || error_exit
sleep 2
checkout_code http://svn-02.marvell.com/repos/vidcode/GaloisSoftware/Drivers/Galois_SoC/GPIO || error_exit
sleep 2
checkout_code http://svn-02.marvell.com/repos/vidcode/GaloisSoftware/Drivers/Galois_SoC/MISC || error_exit
sleep 2
checkout_code http://svn-02.marvell.com/repos/vidcode/GaloisSoftware/Drivers/Galois_SoC/PINMUX || error_exit
sleep 2

# use svn co, so no need to delete .svn
#delete .svn temp files
#find $GALOISSW_DIR/ | grep .svn | xargs rm -rf
