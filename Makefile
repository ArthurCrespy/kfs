MAKE_DIR		= srcs

DOCKER_IMAGE    = kfs
DOCKER_PORT     = 8888

all: kernel
	docker build -t $(DOCKER_IMAGE) .
	docker run --rm -d -p $(DOCKER_PORT):5900 $(DOCKER_IMAGE)
	sleep 0.1
	vncviewer 0.0.0.0:$(DOCKER_PORT)
	docker ps -aq --filter "ancestor=$(DOCKER_IMAGE)" | xargs -r docker stop
	docker ps -aq --filter "ancestor=$(DOCKER_IMAGE)" | xargs -r docker rm

custom: kernel_custom
	docker build -t $(DOCKER_IMAGE) .
	docker run --rm -d -p $(DOCKER_PORT):5900 $(DOCKER_IMAGE)
	sleep 0.1
	vncviewer 0.0.0.0:$(DOCKER_PORT)
	docker ps -aq --filter "ancestor=$(DOCKER_IMAGE)" | xargs -r docker stop
	docker ps -aq --filter "ancestor=$(DOCKER_IMAGE)" | xargs -r docker rm

libk:
	$(MAKE) -C $(MAKE_DIR)/libc

kernel: libk
	$(MAKE) -C $(MAKE_DIR)/kernel

kernel_custom: libk
	$(MAKE) -C $(MAKE_DIR)/kernel

clean:
	$(MAKE) -C $(MAKE_DIR)/libc clean
	$(MAKE) -C $(MAKE_DIR)/kernel clean

clean_container:
	docker ps -aq --filter "ancestor=$(DOCKER_IMAGE)" | xargs -r docker stop
	docker ps -aq --filter "ancestor=$(DOCKER_IMAGE)" | xargs -r docker rm

fclean: clean_container
	$(MAKE) -C $(MAKE_DIR)/libc fclean
	$(MAKE) -C $(MAKE_DIR)/kernel fclean

re: fclean all

.PHONY: all libk kernel clean fclean re clean_container

#i386-elf-ld -m elf_i386 -T srcs/linker.ld -o srcs/kfs.bin objs/boot.o objs/kernel.o

# Test if multiboot is valid :
#if grub-file --is-x86-multiboot myos.bin; then
#  echo multiboot confirmed
#else
#  echo the file is not multiboot
#fi
