#!/bin/sh

qemu-system-x86_64 \
 -s -S \
 -kernel ../arch/x86/boot/bzImage \
 -boot c \
 -M q35 \
 -nographic \
 -smp 2 \
 -hda debian10-base-x86.ext4 \
 -append "root=/dev/sda rw console=ttyS0,115200 acpi=off nokaslr" \
 -serial mon:stdio \
 -netdev user,id=eth0 \
 -device virtio-net-pci,netdev=eth0 \
 -nographic