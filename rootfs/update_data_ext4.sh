#!/bin/bash

mkdir -p data 
sudo mount -t ext4 debian10-data.ext4 data
cd ..
sudo make modules_install INSTALL_MOD_PATH=rootfs/data
cp -r ./kaoshi/app rootfs/data
cd rootfs
sudo umount data
sudo rm -rf ./data
