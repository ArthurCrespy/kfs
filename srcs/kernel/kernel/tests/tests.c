#include "../../include/kernel/tests.h"

void test_irq_handler(void) {
	printf("OK <- C IRQ 49\n");
	outb(0x20, 0x20);
}

void test_irq_49(void) {
	setvect(49, test_irq_handler);
    __asm__ __volatile__("int $49");
}

void test_irq_32(void) {
	__asm__ __volatile__("int $32");
}