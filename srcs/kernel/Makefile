NAME            = kfs.bin

CC              = i386-elf-gcc
CFLAGS          = -Wall -Wextra -fno-builtin -fno-stack-protector -nostdlib -nodefaultlibs -ffreestanding -g
CPPFLAGS        = -Iinclude -I../libc/include

ASM             = nasm
ASMFLAGS        = -felf32

LD              = i386-elf-gcc
LDFLAGS         = -T arch/i386/linker.ld

RM              = rm -rf

BUILD_DIR       = build

C_SRCS          = kernel/kernel.c
ASM_SRCS        = arch/i386/boot.asm \
				  kernel/ports.asm

OBJS_DIR        = build/objs
C_OBJS          = ${OBJS_DIR}/kernel.o
ASM_OBJS        = ${OBJS_DIR}/boot.o

LIBK            = ../libc/build/libk.a

all: ${OBJS_DIR} ${NAME}

${NAME}: ${ASM_OBJS} ${C_OBJS} ${LIBK}
	${LD} ${LDFLAGS} -o ${BUILD_DIR}/${NAME} ${ASM_OBJS} ${C_OBJS} ${LIBK} ${CFLAGS}

${OBJS_DIR}/%.o: arch/i386/%.asm | ${OBJS_DIR}
	${ASM} ${ASMFLAGS} $< -o $@

${OBJS_DIR}/%.o: kernel/%.c | ${OBJS_DIR}
	${CC} -c $< -o $@ ${CFLAGS} ${CPPFLAGS}

${BUILD_DIR} ${OBJS_DIR}:
	mkdir -p $@

clean:
	${RM} ${OBJS_DIR}

fclean: clean
	${RM} ${BUILD_DIR}

re: fclean all

.PHONY: all clean fclean re
