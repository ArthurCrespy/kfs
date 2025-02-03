nasm -felf32 boot.asm -o boot.o

i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
# -fno-builtin -fno-exception -fno-stack-protector -fno-rtti -nostdlib -nodefaultlibs ? C++ but maybe equivalents in C
