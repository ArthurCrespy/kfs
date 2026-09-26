FROM alpine:3.23

RUN echo "@testing https://dl-cdn.alpinelinux.org/alpine/edge/testing" >> /etc/apk/repositories \
 && apk add --no-cache qemu-system-i386 grub grub-bios xorriso mtools xz novnc websockify bochs@testing xvfb x11vnc

RUN mkdir -p /kfs/isodir/boot/grub

COPY srcs/kernel/build/kfs.elf /kfs/isodir/boot/grub/
COPY srcs/kernel/arch/i386/grub.cfg /kfs/isodir/boot/grub/

COPY bochsrc /kfs/bochsrc
COPY run.sh /kfs/run.sh

RUN chmod +x /kfs/run.sh

RUN grub-mkrescue -o /kfs/kfs.iso --compress=xz /kfs/isodir

CMD ["/kfs/run.sh"]
