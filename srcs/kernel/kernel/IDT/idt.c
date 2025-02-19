#include <idt.h>

extern struct idt_entry idt[];

void setvect(uint8_t vector, void (*handler)(void)) {
	uint32_t handler_addr = (uint32_t) handler;

	idt[vector].offset_low	= handler_addr & 0xFFFF;
	idt[vector].selector	= 0x08;
	idt[vector].zero		= 0;
	idt[vector].type_attr	= 0x8E;
	idt[vector].offset_high	= (handler_addr >> 16) & 0xFFFF;
}

void idt_init(void) {
	setvect(0x20, isr_wrapper);
	setvect(0x21, isr_wrapper_keyboard);
	setvect(0x31, isr_wrapper);
	idt_load();
}
