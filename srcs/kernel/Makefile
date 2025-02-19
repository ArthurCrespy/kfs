NAME            = kfs.bin

CC              = i386-elf-gcc
CFLAGS          = -Wall -Wextra -fno-builtin -fno-stack-protector -nostdlib -nodefaultlibs -ffreestanding -g
CPPFLAGS        = -Iinclude/kernel -I../libc/include

ASM             = nasm
ASMFLAGS        = -f elf32 -F dwarf -g
LIBK            = ../libc/build/libk.a

DISASM		  	= ndisasm
DISASMFLAGS	  	= -b 32

LD              = i386-elf-gcc
LDFLAGS         = -T arch/i386/linker.ld

RM              = rm -rf

C_SRCS          := $(shell find kernel -type f -name '*.c')
KERNEL_SRCS     := $(shell find kernel -type f -name '*.asm')
BOOT_SRCS		:= $(shell find arch -type f -name '*.asm')
ASM_SRCS        := $(KERNEL_SRCS) $(BOOT_SRCS)

BUILD_DIR       = build
OBJS_DIR        = $(BUILD_DIR)/objs
C_OBJS          := $(patsubst %.c, $(OBJS_DIR)/%.c.o, $(C_SRCS))
KERNEL_OBJS     := $(patsubst %.asm, $(OBJS_DIR)/%.asm.o, $(KERNEL_SRCS))
BOOT_OBJS       := $(patsubst arch/%.asm, $(OBJS_DIR)/arch/%.asm.o, $(BOOT_SRCS))
OBJS            = $(C_OBJS) $(KERNEL_OBJS) $(BOOT_OBJS)

all: $(BUILD_DIR) $(OBJS_DIR) $(NAME)

$(NAME): $(OBJS) $(LIBK)
	$(LD) $(LDFLAGS) -o $(BUILD_DIR)/$(NAME) $(OBJS) $(LIBK) $(CFLAGS)
	$(DISASM) $(DISASMFLAGS) $(BUILD_DIR)/$(NAME) > $(BUILD_DIR)/$(NAME).disasm

$(OBJS_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJS_DIR)/%.asm.o: %.asm
	@mkdir -p $(dir $@)
	$(ASM) $(ASMFLAGS) $< -o $@

$(BUILD_DIR) $(OBJS_DIR):
	mkdir -p $@

clean:
	$(RM) $(OBJS_DIR)
	$(RM) $(BUILD_DIR)/$(NAME).disasm

fclean: clean
	$(RM) $(BUILD_DIR)

re: fclean all

.PHONY: all clean fclean re
