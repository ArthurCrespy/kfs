#include <tests.h>

void test_irq_handler(void) {
	printf("OK: IRQ Routine called (test_irq_handler)\n");
	printf("    Scancode: N/A\n");
	outb(0x20, 0x20);
}

void test_irq_49(void) {
	setvect(49, test_irq_handler);
    __asm__ __volatile__("int $49");
}

void test_irq_32(void) {
    setvect(32, test_irq_handler);
	__asm__ __volatile__("int $32");
}
