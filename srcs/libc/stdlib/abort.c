#include <stdio.h>
#include <stdlib.h>

__attribute__((__noreturn__))
void abort(void) {
#if defined(__is_libk)
  	#include <kernel/kernel.h>
	terminal_writestring("kernel: panic: abort()\n");
#else
	printf("abort()\n");
#endif
	while (1) { }
	__builtin_unreachable();
}