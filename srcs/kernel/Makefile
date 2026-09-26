NAME            = kfs
.DEFAULT_GOAL   = all

CC              = i386-elf-gcc
CFLAGS          = -std=gnu11 -Wall -Wextra -Wconversion -fno-builtin -fno-stack-protector -ffreestanding -g
CPPFLAGS        = -Iinclude/kernel -I../libc/include -MMD -MP

ASM             = nasm
ASMFLAGS        = -w+all -f elf32 -F dwarf -MP -g

DISASM		  	= ndisasm
DISASMFLAGS	  	= -b 32

LD              = i386-elf-ld
LDFLAGS         = -T arch/i386/linker.ld

RM              = rm -rf

BUILD_DIR       = build
OBJS_DIR        := $(BUILD_DIR)/objs
LIBC_DIR        = ../libc

C_SRCS          := $(shell find kernel -type f -name '*.c')
ASM_SRCS        := $(shell find kernel arch -type f -name '*.asm')

C_OBJS          := $(C_SRCS:%.c=$(OBJS_DIR)/%.c.o)
ASM_OBJS        := $(ASM_SRCS:%.asm=$(OBJS_DIR)/%.asm.o)

OBJS            := $(C_OBJS) $(ASM_OBJS)
DEPS            := $(OBJS:.o=.d)
-include $(DEPS)

LIBK            := $(LIBC_DIR)/build/libk.a

ELF             := $(BUILD_DIR)/$(NAME).elf

all: $(ELF)

$(LIBK): FORCE
	$(MAKE) -C $(LIBC_DIR)

$(ELF): $(OBJS) $(LIBK)
	@mkdir -p $(dir $@)
	$(LD) $(LDFLAGS) -o $@ $^
	$(DISASM) $(DISASMFLAGS) $@ > $@.disasm

$(OBJS_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJS_DIR)/%.asm.o: %.asm
	@mkdir -p $(dir $@)
	$(ASM) $< -o $@ $(ASMFLAGS) -MD $(@:.o=.d)

clean:
	$(MAKE) -C $(LIBC_DIR) clean
	$(RM) $(OBJS_DIR)
	$(RM) $(ELF).disasm

fclean: clean
	$(MAKE) -C $(LIBC_DIR) fclean
	$(RM) $(BUILD_DIR)

re: fclean
	$(MAKE) all

FORCE:

.PHONY: all clean fclean re
