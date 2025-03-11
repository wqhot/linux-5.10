#!/bin/bash

MNTDIR=/mnt/rootfs
mkdir -p rootfs 
rm ./debian10-base-x86.ext4
sudo debootstrap --arch amd64 --variant=minbase --include=whiptail,ca-certificates,tzdata,gcc,iproute2,openssh-server,gdb,nfs-common,iputils-ping,isc-dhcp-client,kmod,pciutils,ufw,vim \
    buster rootfs https://mirrors.ustc.edu.cn/debian/
dd if=/dev/zero of=debian10-base-x86.ext4 bs=1M count=2048
mkfs.ext4 debian10-base-x86.ext4
sudo mount -t ext4 debian10-base-x86.ext4 $MNTDIR/
sudo cp -raf rootfs/* $MNTDIR/
sudo cp /usr/bin/qemu-aarch64-static $MNTDIR/usr/bin/
sudo cp /etc/resolv.conf $MNTDIR/etc/
sudo mount -t proc /proc $MNTDIR/proc
sudo mount -t sysfs /sys $MNTDIR/sys
sudo mount -o bind /dev $MNTDIR/dev
sudo mount -o bind /dev/pts $MNTDIR/dev/pts

cd ..
sudo make modules_install INSTALL_MOD_PATH=$MNTDIR
sudo make headers_install INSTALL_HDR_PATH=$MNTDIR/usr
cd rootfs
sudo chroot $MNTDIR /bin/bash -c "apt update"
sudo chroot $MNTDIR /bin/bash -c "apt install -y systemd sudo rsyslog udev"
sudo chroot $MNTDIR /bin/bash -c "useradd -s '/bin/bash' -m -G adm,sudo wq"
sudo chroot $MNTDIR /bin/bash -c "echo 'wq:wq' | chpasswd"
sudo chroot $MNTDIR /bin/bash -c "echo "debian10" > /etc/hostname"
sudo chroot $MNTDIR /bin/bash -c "cp /lib/systemd/system/serial-getty@.service /lib/systemd/system/serial-getty@ttyAMA0.service"
sudo chroot $MNTDIR /bin/bash -c "systemctl enable serial-getty@ttyAMA0.service"
# 修改文件内容：
SERVICE_FILE="$MNTDIR/lib/systemd/system/serial-getty@ttyAMA0.service"

if [ -f "$SERVICE_FILE" ]; then
    # 将 BindsTo 行前面加上注释符 #
    sudo sed -i 's/^BindsTo=dev-%i.device$/#BindsTo=dev-%i.device/' "$SERVICE_FILE"
    # 将 After 行前面加上注释符 #
    sudo sed -i 's/^After=dev-%i.device systemd-user-sessions.service plymouth-quit-wait.service getty-pre.target$/#After=dev-%i.device systemd-user-sessions.service plymouth-quit-wait.service getty-pre.target/' "$SERVICE_FILE"
    echo "替换完成！"
else
    echo "文件 $SERVICE_FILE 不存在！"
fi

sudo umount $MNTDIR/proc 
sudo umount $MNTDIR/sys 
sudo umount $MNTDIR/dev/pts 
sudo umount $MNTDIR/dev 
sudo umount $MNTDIR

sudo rm -rf ./rootfs
