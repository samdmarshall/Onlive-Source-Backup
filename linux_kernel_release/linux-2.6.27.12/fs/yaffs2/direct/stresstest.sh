#!/bin/sh

ITERATIONS=

DTEST_PATH=

test_simple(){
	cp 8M.img em2k.img
#	./directtest2k simple ./em2k.img 8 100
#	./directtest2k acp ./em2k.img 8 100
	./directtest2k rand ./em2k.img 8 100
}

test2(){
	cp 8M.img em2k.img
	./directtest2k cp ./em2k.img 8 mtd-src /
	./directtest2k diff ./em2k.img 8 mtd-src /
	./directtest2k verify ./em2k.img 8 mtd-src/ 50
}

test_small(){
#	../utils/mkyaffs2image  -r mtd-src/ -p 8 yaffs2.mtd-src.img
	cp yaffs2.mtd-src.img em2k.img
#	./directtest2k ls ./em2k.img 8
#	./directtest2k cp ./em2k.img 8 mtd-src /
	./directtest2k diff ./em2k.img 8 mtd-src /
	./directtest2k verify ./em2k.img 8 mtd-src 200
}

test_large(){
#	../utils/mkyaffs2image  -r mtd-utils-1.2.0/ -p 8 yaffs2.mtd-utils.img
	cp yaffs2.mtd-utils.img em2k.img
#	./directtest2k ls ./em2k.img 8
#	./directtest2k cp ./em2k.img 8 mtd-utils-1.2.0 /
#	./directtest2k diff em2k.img 8 mtd-utils-1.2.0 /
	./directtest2k verify ./em2k.img 8 mtd-utils-1.2.0/ 200
}

ITERATIONS=500
i=0

cp yaffs2.mtd-utils.img em2k.img

while [ $i -lt $ITERATIONS ]
do
	./directtest2k verify ./em2k.img 8 mtd-utils-1.2.0/ 1
	echo "verify iteration $i done"
	ITERATION_TEMP=$?
	if [ $ITERATION_TEMP -ne 0 ];then
		let FAIL=$FAIL+1
		echo "$FAIL failures by now"
	fi
	let i=$i+1
done


#test_simple
#test2
#test_small
#test_large
