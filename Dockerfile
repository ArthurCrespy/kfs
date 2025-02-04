FROM alpine:3.21.2

RUN apk update && apk add qemu-system-i386 grub xorriso grub-bios mtools build-base xz

RUN mkdir -p /kfs/isodir/boot/grub

COPY srcs/kfs.bin /kfs/isodir/boot/grub
COPY srcs/grub.cfg /kfs/isodir/boot/grub

RUN grub-mkrescue -o /kfs/kfs.iso --compress=xz /kfs/isodir

CMD ["qemu-system-i386", "-vnc", "0.0.0.0:0", "-cdrom", "/kfs/kfs.iso"]
