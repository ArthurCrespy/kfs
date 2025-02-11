#include "../../include/kernel/tests.h"

void int_handler(void) {
	printf("OK <- C IRQ 49 handled\n");
	outb(0x20, 0x20);
}

void osdev_test1(void) {
	setvect(49, int_handler);
    __asm__ __volatile__("int $49");
}