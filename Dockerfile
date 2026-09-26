FROM alpine:3.23

RUN apk update && apk add qemu-system-i386 grub grub-bios xorriso mtools xz novnc websockify

RUN mkdir -p /kfs/isodir/boot/grub

COPY srcs/kernel/build/kfs.elf /kfs/isodir/boot/grub/
COPY srcs/kernel/arch/i386/grub.cfg /kfs/isodir/boot/grub/

RUN grub-mkrescue -o /kfs/kfs.iso --compress=xz /kfs/isodir

CMD ["sh", "-c", "novnc_server --vnc 127.0.0.1:5900 --listen 5800 & exec qemu-system-i386 -drive file=/kfs/kfs.iso,format=raw -vnc 0.0.0.0:0 -s $QEMU_FLAGS"]