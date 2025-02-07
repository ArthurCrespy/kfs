NAME            = libk.a

CC              = i386-elf-gcc
CFLAGS          = -Wall -Wextra -fno-builtin -fno-stack-protector -nostdlib -nodefaultlibs -ffreestanding -g
CPPFLAGS        = -D__is_libc -Iinclude -I../kernel/include

LIBK_CFLAGS     = $(CFLAGS)
LIBK_CPPFLAGS   = $(CPPFLAGS) -D__is_libk

AR              = i386-elf-ar
ARFLAGS         = rcs

RM              = rm -rf

C_SRCS          := $(shell find . -type f -name '*.c')

BUILD_DIR       = build
OBJS_DIR        = $(BUILD_DIR)/objs
C_OBJS          := $(patsubst ./%, $(OBJS_DIR)/%, $(C_SRCS:.c=.o))
LIBK_OBJS       := $(patsubst %.o,%.libk.o,$(C_OBJS))

all: $(OBJS_DIR) $(NAME)

$(NAME): $(LIBK_OBJS)
	$(AR) $(ARFLAGS) $(BUILD_DIR)/$(NAME) $(LIBK_OBJS)

$(OBJS_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

$(OBJS_DIR)/%.libk.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -MD -c $< -o $@ -std=gnu11 $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

$(BUILD_DIR) $(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

clean:
	$(RM) $(OBJS_DIR)

fclean: clean
	$(RM) $(BUILD_DIR)

re: fclean all

.PHONY: all clean fclean re
