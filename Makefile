MAKE_DIR		= srcs

DOCKER_IMAGE    = kfs
DOCKER_PORT     = 8888
DOCKER_GDB      = 1234

all: kernel docker

all-gcc: kernel-gcc docker

docker:
	docker build -t $(DOCKER_IMAGE) .
	docker run --rm -d -p $(DOCKER_PORT):5900 -p $(DOCKER_GDB):1234 $(DOCKER_IMAGE)
	sleep 0.1
	vncviewer 0.0.0.0:$(DOCKER_PORT)
	docker ps -aq --filter "ancestor=$(DOCKER_IMAGE)" | xargs -r docker stop

libk:
	$(MAKE) -C $(MAKE_DIR)/libc

kernel: libk
	$(MAKE) -C $(MAKE_DIR)/kernel

kernel-gcc: libk
	$(MAKE) -C $(MAKE_DIR)/kernel all-gcc

clean:
	$(MAKE) -C $(MAKE_DIR)/libc clean
	$(MAKE) -C $(MAKE_DIR)/kernel clean

clean_container:
	docker ps -aq --filter "ancestor=$(DOCKER_IMAGE)" | xargs -r docker stop

fclean: clean_container
	$(MAKE) -C $(MAKE_DIR)/libc fclean
	$(MAKE) -C $(MAKE_DIR)/kernel fclean

re: fclean all

.PHONY: all libk kernel clean fclean re clean_container

# Test if multiboot is valid :
# if grub-file --is-x86-multiboot myos.bin; then
#	echo multiboot confirmed
# else
#	echo the file is not multiboot
# fi
