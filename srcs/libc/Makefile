NAME            = libk.a

CC              = cc
CFLAGS          = -Wall -Wextra -Werror -ffreestanding -O2 -g
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
                  string/strlen.c \

OBJS            = ${SRCS:%.c=${BUILD_DIR}/%.o}
LIBK_OBJS       = ${OBJS:%.o=%.libk.o}

all: ${BUILD_DIR} ${NAME}

${NAME}: ${LIBK_OBJS}
	${AR} ${ARFLAGS} ${NAME} ${LIBK_OBJS}

${BUILD_DIR}:
	mkdir -p ${BUILD_DIR}/stdio ${BUILD_DIR}/stdlib ${BUILD_DIR}/string

${BUILD_DIR}/%.o: %.c | ${BUILD_DIR}
	${CC} -MD -c $< -o $@ -std=gnu11 ${CFLAGS} ${CPPFLAGS}

${BUILD_DIR}/%.libk.o: %.c | ${BUILD_DIR}
	${CC} -MD -c $< -o $@ -std=gnu11 ${LIBK_CFLAGS} ${LIBK_CPPFLAGS}

install: install-headers install-libs

install-headers:
	mkdir -p ${DESTDIR}${INCLUDEDIR}
	cp -R --preserve=timestamps include/. ${DESTDIR}${INCLUDEDIR}/.

install-libs: ${NAME}
	mkdir -p ${DESTDIR}${LIBDIR}
	cp ${NAME} ${DESTDIR}${LIBDIR}

clean:
	${RM} ${NAME} ${BUILD_DIR}

.PHONY: all clean install install-headers install-libs