nasm -felf32 boot.asm -o boot.o

i386-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
# -fno-builtin -fno-exception -fno-stack-protector -fno-rtti -nostdlib -nodefaultlibs ? C++ but maybe equivalents in C

i386-elf-gcc -T srcs/linker.ld -o KFS.bin -ffreestanding -O2 -nostdlib objs/boot.o objs/kernel.o -lgcc
# gcc instead of ld -> ld prevents the compiler from performing various tasks during linking.

### Test if multiboot is valid :
#if grub-file --is-x86-multiboot myos.bin; then
#  echo multiboot confirmed
#else
#  echo the file is not multiboot
#fi

# Building a bootable CD-ROM image
# cp kfs.bin isodir/boot/grub
# cp grub/grub.cfg isodir/boot/grub
# grub-mkrescue -o kfs.iso isodir