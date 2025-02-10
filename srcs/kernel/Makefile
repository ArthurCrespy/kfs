NAME            = kfs.bin

CC              = i386-elf-gcc
CFLAGS          = -Wall -Wextra -fno-builtin -fno-stack-protector -nostdlib -nodefaultlibs -ffreestanding -g
CPPFLAGS        = -Iinclude -I../libc/include

ASM             = nasm
ASMFLAGS        = -felf32
LIBK            = ../libc/build/libk.a

LD              = i386-elf-gcc
LDFLAGS         = -T arch/i386/linker.ld

RM              = rm -rf

C_SRCS          := $(shell find kernel -type f -name '*.c')
KERNEL_SRCS     := $(shell find kernel -type f -name '*.asm')
BOOT_SRCS		:= $(shell find arch -type f -name '*.asm')
ASM_SRCS        := $(KERNEL_SRCS) $(BOOT_SRCS)

BUILD_DIR       = build
OBJS_DIR        = $(BUILD_DIR)/objs
C_OBJS          := $(patsubst %.c, $(OBJS_DIR)/%.o, $(C_SRCS))
KERNEL_OBJS     := $(patsubst %.asm, $(OBJS_DIR)/%.o, $(KERNEL_SRCS))
BOOT_OBJS       := $(patsubst arch/%.asm, $(OBJS_DIR)/arch/%.o, $(BOOT_SRCS))
OBJS            = $(C_OBJS) $(KERNEL_OBJS) $(BOOT_OBJS)

all: $(BUILD_DIR) $(OBJS_DIR) $(NAME)

$(NAME): $(OBJS) $(LIBK)
	$(LD) $(LDFLAGS) -o $(BUILD_DIR)/$(NAME) $(OBJS) $(LIBK) $(CFLAGS)

$(OBJS_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJS_DIR)/%.o: %.asm
	@mkdir -p $(dir $@)
	$(ASM) $(ASMFLAGS) $< -o $@

$(BUILD_DIR) $(OBJS_DIR):
	mkdir -p $@

clean:
	$(RM) $(OBJS_DIR)

fclean: clean
	$(RM) $(BUILD_DIR)

re: fclean all

.PHONY: all clean fclean re
