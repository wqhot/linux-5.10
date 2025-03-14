#!/bin/bash

mkdir -p data 
rm ./debian10-data.ext4
dd if=/dev/zero of=debian10-data.ext4 bs=1M count=256
mkfs.ext4 debian10-data.ext4
sudo mount -t ext4 debian10-data.ext4 data
cd ..
sudo make modules_install INSTALL_MOD_PATH=rootfs/data
cd rootfs
sudo umount data
sudo rm -rf ./data
