NAME            = libk.a
.DEFAULT_GOAL   = all

CC              = i386-elf-gcc
CFLAGS          = -std=gnu11 -Wall -Wextra -Wconversion -fno-builtin -fno-stack-protector -nostdlib -nodefaultlibs -ffreestanding -g
CPPFLAGS        = -D__is_libc -Iinclude -I../kernel/include/kernel -MMD -MP

LIBK_CFLAGS     := $(CFLAGS)
LIBK_CPPFLAGS   := $(CPPFLAGS) -D__is_libk

AR              = i386-elf-ar
ARFLAGS         = rcs

RM              = rm -rf

BUILD_DIR       = build
SRC_DIRS        = ctype stdio stdlib string
OBJS_DIR        = $(BUILD_DIR)/objs

C_SRCS          := $(shell find $(SRC_DIRS) -type f -name '*.c')

C_OBJS          := $(C_SRCS:%.c=$(OBJS_DIR)/%.o)
LIBK_OBJS       := $(C_OBJS:.o=.libk.o)
DEPS            := $(LIBK_OBJS:.o=.d)
-include $(DEPS)

LIBK            := $(BUILD_DIR)/$(NAME)

all: $(LIBK)

$(LIBK): $(LIBK_OBJS)
	@mkdir -p $(dir $@)
	$(RM) $@
	$(AR) $(ARFLAGS) $@ $^

$(OBJS_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJS_DIR)/%.libk.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

clean:
	$(RM) $(OBJS_DIR)

fclean: clean
	$(RM) $(BUILD_DIR)

re: fclean
	$(MAKE) all

.PHONY: all clean fclean re
