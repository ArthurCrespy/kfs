all: build

build:
	mkdir -p objs
	nasm -felf32 srcs/boot.asm -o objs/boot.o
	i386-elf-gcc -c srcs/kernel.c -o objs/kernel.o -fno-builtin -fno-stack-protector -nostdlib -nodefaultlibs -ffreestanding
	i386-elf-gcc -T srcs/linker.ld -o srcs/kfs.bin objs/boot.o objs/kernel.o -fno-builtin -fno-stack-protector -nostdlib -nodefaultlibs -ffreestanding
	docker build -t kfs .
	docker run --rm -d -p 8888:5900 kfs
	sleep 0.1
	vncviewer 0.0.0.0:8888

build-ld:
	mkdir -p objs
	nasm -felf32 srcs/boot.asm -o objs/boot.o
	i386-elf-gcc -c srcs/kernel.c -o objs/kernel.o -fno-builtin -fno-stack-protector -nostdlib -nodefaultlibs -ffreestanding
	i386-elf-ld -m elf_i386 -T srcs/linker.ld -o srcs/kfs.bin objs/boot.o objs/kernel.o -fno-builtin -fno-stack-protector -nostdlib -nodefaultlibs -ffreestanding
	docker build -t kfs .
	docker run --rm -d -p 8888:5900 kfs
	vncviewer 0.0.0.0:8888

clean:
	rm -rf objs
	rm -rf srcs/kfs.bin

fclean: clean
	docker ps -aq --filter "ancestor=kfs" | xargs -r docker stop
	docker ps -aq --filter "ancestor=kfs" | xargs -r docker rm

re: fclean all

.PHONY: build buill-ld clean fclean re



#nasm -felf32 boot.asm -o boot.o

#i386-elf-gcc -c srcs/kernel.c -o objs/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
# Flags: -fno-builtin -fno-stack-protector -nostdlib -nodefaultlibs ? C++ but maybe there are equivalents in C

#i386-elf-gcc -T srcs/linker.ld -o srcs/kfs.bin -ffreestanding -O2 -nostdlib objs/boot.o objs/kernel.o -lgcc
# gcc instead of ld -> ld prevents the compiler from performing various tasks during linking.

#i386-elf-ld -m elf_i386 -T srcs/linker.ld -o srcs/kfs.bin objs/boot.o objs/kernel.o
# LD instead of GCC (make a separate rule if correcteur is un peu tatillon)

#docker build -t kfs .
#docker run --rm -d -p 8888:5900 kfs

#vncviewer localhost:8888 # Works in the terminal

# Test if multiboot is valid :
#if grub-file --is-x86-multiboot myos.bin; then
#  echo multiboot confirmed
#else
#  echo the file is not multiboot
#fi