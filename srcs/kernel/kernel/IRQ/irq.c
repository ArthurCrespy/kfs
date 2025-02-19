#include <irq.h>

void irq_test_49(void) {
	__asm__ __volatile__("int $49"); // 0x31
}

void irq_test_33(void) {
	__asm__ __volatile__("int $33"); // 0x21
}

void irq_test_32(void) {
	__asm__ __volatile__("int $32"); // 0x20
}

bool irq_test_enabled(void) {
	unsigned long flags;
	__asm__ __volatile__("pushf\n\t" "pop %0" : "=g"(flags));
	return flags & (1 << 9);
}