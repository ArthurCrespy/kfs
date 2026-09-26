export PATH     := $(CURDIR)/cross-compiler/kfs/bin:$(PATH)
OPEN            := $(shell command -v xdg-open || echo open)

KERNEL_DIR      = srcs/kernel

DOCKER_IMAGE    = kfs
DOCKER_PLATFORM = linux/amd64
DOCKER_NOVNC	= 5800
DOCKER_VNC		= 5900
DOCKER_GDB      = 1234

EMU             ?= bochs

QEMU_FLAGS		?=
BOCHS_FLAGS		?=

all: vnc

debug: QEMU_FLAGS += -S
debug: all

kernel:
	$(MAKE) -C $(KERNEL_DIR)

image: kernel stop
	docker build --platform $(DOCKER_PLATFORM) -t $(DOCKER_IMAGE) .

run: image
	@docker run -d --rm --name $(DOCKER_IMAGE) --platform $(DOCKER_PLATFORM) \
		-p $(DOCKER_NOVNC):5800 -p $(DOCKER_VNC):5900 -p $(DOCKER_GDB):1234 \
		-e QEMU_FLAGS="$(QEMU_FLAGS)" -e EMU="$(EMU)" $(DOCKER_IMAGE)
	@for i in $$(seq 1 20); do curl -fs -o /dev/null http://localhost:$(DOCKER_NOVNC) && exit 0; sleep 0.5; done; \
	echo "noVNC unreachable on port $(DOCKER_NOVNC)"; exit 1

vnc: run
	$(OPEN) "http://localhost:$(DOCKER_NOVNC)/vnc.html?autoconnect=true&resize=scale"

stop:
	@docker rm -f $(DOCKER_IMAGE) > /dev/null 2>&1 || true

clean:
	$(MAKE) -C $(KERNEL_DIR) clean

fclean: stop
	$(MAKE) -C $(KERNEL_DIR) fclean

re: fclean
	$(MAKE) all

.PHONY: all debug kernel image run vnc stop clean fclean re

# How to test if multiboot is valid (requires grub-file) :
# if grub-file --is-x86-multiboot kfs.bin; then
#	echo "multiboot confirmed"
# else
#	echo "multiboot unconfirmed"; exit 1
# fi
