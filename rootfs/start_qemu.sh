#!/bin/sh

qemu-system-x86_64 \
 -s \
 -kernel ../arch/x86/boot/bzImage \
 -boot c \
 -M q35 \
 -nographic \
 -m 1024 \
 -smp 1 \
 -hda debian10-base-x86.ext4 \
 -drive file=debian10-data.ext4,if=virtio,format=raw \
 -append "root=/dev/sda rw console=ttyS0,115200 acpi=off nokaslr" \
 -serial mon:stdio \
 -net user,hostfwd=tcp::2222-:22 -net nic \
 -nographic