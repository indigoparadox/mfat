#!/bin/bash

IMAGE_NAME=testimg.fat.img
IMAGE_DIR=testimg

TEST_DIR_NAME=testdir
COUNT_NAME=count.txt
COUNT_SUB_NAME=countsub.txt
BMP_NAME=random.bmp
RAND_FILE_NAME=randfile.bin
RAND_FILE_BYES=512

echo "Creating image directory and FAT image..."
rm -rf "$IMAGE_DIR"
mkdir "$IMAGE_DIR"
rm "$IMAGE_NAME"
dd if=/dev/urandom of="$IMAGE_NAME" bs=1M count=25
/sbin/mkfs.vfat -F 16 "$IMAGE_NAME"

# Use meaningful test file.
cp -v doxyfile "$IMAGE_DIR"
mcopy -i "$IMAGE_NAME" doxyfile ::

# Create linear count for testing contiguity.
echo "Creating count file..."
rm -f $COUNT_NAME
for i in {1..10000}; do
   echo $i >> $COUNT_NAME
done

cp -v "$COUNT_NAME" "$IMAGE_DIR"
mcopy -i "$IMAGE_NAME" $COUNT_NAME ::
rm "$COUNT_NAME"

# Create small text file.
echo "Creating fox file..."
rm -f fox.txt
echo "The quick brown fox." > fox.txt
mv fox.txt "$TEST_DIR_NAME"
rm -f fox.txt

# Make test directory.
rm -rf "$TEST_DIR_NAME"
mkdir "$TEST_DIR_NAME"

# Some subdirectory test files.
echo "Filling test directory..."
echo "Creating random binary file..."
dd if=/dev/urandom \
   of="$TEST_DIR_NAME/$RAND_FILE_NAME" bs=$RAND_FILE_BYES count=1
rm -f $COUNT_SUB_NAME

echo "Creating dog file..."
rm -f dog.txt
echo "Ran over the lazy dog." > dog.txt
mv dog.txt "$TEST_DIR_NAME"

echo "Creating count file..."
for i in {20001..30000}; do
   echo "$i" >> "$COUNT_SUB_NAME"
done
mv "$COUNT_SUB_NAME" "$TEST_DIR_NAME"

cp -Rv "$TEST_DIR_NAME" "$IMAGE_DIR"
mcopy -i "$IMAGE_NAME" "$TEST_DIR_NAME" ::
rm -rf "$TEST_DIR_NAME"

# Create random noise bitmap for binary reading.
echo "Creating random bitmap..."
rm -f "$BMP_NAME"
convert -size 32x32 -type truecolor xc:gray +noise random "$BMP_NAME"
cp -v "$BMP_NAME" "$IMAGE_DIR"
mcopy -i "$IMAGE_NAME" "$BMP_NAME" ::
rm "$BMP_NAME"

echo "Creating ext2 image..."
genext2fs -d "$IMAGE_DIR" -b 20480 "${IMAGE_DIR}.ext2.img"

echo "Cleaning up..."
rm -rf "$IMAGE_DIR"

