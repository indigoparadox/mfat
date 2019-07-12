#!/bin/bash

IMAGE_NAME=testimg.fat.img
IMAGE_DIR=testimg

rm -rf "$IMAGE_DIR"
mkdir $IMAGE_DIR
rm "$IMAGE_NAME"
dd if=/dev/urandom of="$IMAGE_NAME" bs=1M count=25
/sbin/mkfs.vfat -F 16 "$IMAGE_NAME"

cp -v doxyfile $IMAGE_DIR
mcopy -i "$IMAGE_NAME" doxyfile ::
cp -Rv x86 $IMAGE_DIR
mcopy -i "$IMAGE_NAME" x86 ::

rm -f count.txt
for i in {1..10000}; do
   echo $i >> count.txt
done

cp -v count.txt $IMAGE_DIR
mcopy -i "$IMAGE_NAME" count.txt ::
rm count.txt

rm -f random.bmp
convert -size 32x32 -type truecolor xc:gray +noise random random.bmp
cp -v random.bmp $IMAGE_DIR
mcopy -i "$IMAGE_NAME" random.bmp ::
rm random.bmp

genext2fs -d $IMAGE_DIR -b 20480 ${IMAGE_DIR}.ext2.img

