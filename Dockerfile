FROM alpine:3.21.2

RUN apk update && apk add qemu-system-i386 grub xorriso grub-bios mtools build-base xz

RUN mkdir -p /kfs/isodir/boot/grub

COPY srcs/kernel/build/kfs /kfs/isodir/boot/grub
COPY srcs/kernel/arch/i386/grub.cfg /kfs/isodir/boot/grub

RUN echo "GRUB_TIMEOUT=3" > /etc/default/grub

RUN update-grub ; grub-mkrescue -o /kfs/kfs.iso --compress=xz /kfs/isodir

#CMD ["qemu-system-i386", "-boot", "order=c", "-drive", "file=/kfs/kfs.iso,if=ide,media=disk", "-vnc", "0.0.0.0:0"]
CMD ["qemu-system-i386", "-boot", "order=c", "-drive", "file=/kfs/kfs.iso,if=ide,media=disk", "-vnc", "0.0.0.0:0", "-s", "-S"]