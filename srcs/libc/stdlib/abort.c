#include <stdlib.h>
#include <stdio.h>

#if defined(__is_libk)
#include <terminal.h>
#endif

__attribute__((__noreturn__))
void abort(void) {
#if defined(__is_libk)
	terminal_writestring("kernel: panic: abort()\n");
#else
	printf("abort()\n");
#endif
	while (1) { }
	__builtin_unreachable();
}