NAME            = libk.a

CC              = cc
CFLAGS          = -Wall -Wextra -fno-builtin -fno-stack-protector -nostdlib -nodefaultlibs -ffreestanding -g
CPPFLAGS        = -D__is_libc -Iinclude -I../kernel/include
LIBK_CFLAGS     = ${CFLAGS}
LIBK_CPPFLAGS   = ${CPPFLAGS} -D__is_libk

AR              = ar
ARFLAGS         = rcs

RM              = rm -rf

BUILD_DIR       = build

SRCS            = stdio/printf.c \
                  stdio/putchar.c \
                  stdio/puts.c \
                  stdlib/abort.c \
                  string/memcmp.c \
                  string/memcpy.c \
                  string/memmove.c \
                  string/memset.c \
                  string/strlen.c

OBJS_DIR         = build/objs
OBJS            = ${SRCS:%.c=${OBJS_DIR}/%.o}
LIBK_OBJS       = ${OBJS:%.o=%.libk.o}

all: ${OBJS_DIR} ${NAME}

${NAME}: ${LIBK_OBJS}
	${AR} ${ARFLAGS} ${BUILD_DIR}/${NAME} ${LIBK_OBJS}

${OBJS_DIR}/%.o: %.c | ${OBJS_DIR}
	${CC} -MD -c $< -o $@ -std=gnu11 ${CFLAGS} ${CPPFLAGS}

${OBJS_DIR}/%.libk.o: %.c | ${OBJS_DIR}
	${CC} -MD -c $< -o $@ -std=gnu11 ${LIBK_CFLAGS} ${LIBK_CPPFLAGS}

${OBJS_DIR}:
	mkdir -p ${OBJS_DIR}/stdio ${OBJS_DIR}/stdlib ${OBJS_DIR}/string

clean:
	${RM} ${OBJS_DIR}

fclean: clean
	${RM} ${BUILD_DIR}

re: fclean all

.PHONY: all clean fclean re