#include <idt.h>

extern struct gate_descriptor idt[];

void setvect(uint8_t vector, uint16_t selector, uint8_t type_attr, void (*handler)(void)) {
	idt[vector].offset_low	= (uint16_t)((uint32_t)handler & 0xFFFF);
	idt[vector].selector	= selector;
	idt[vector].zero		= 0x00;
	idt[vector].type_attr	= type_attr;
	idt[vector].offset_high	= (uint16_t)(((uint32_t)handler >> 16) & 0xFFFF);
}

void idt_init(void) {
	setvect(0x20, 0x08, 0x8E, isr_wrapper);
	setvect(0x21, 0x08, 0x8E, isr_wrapper_keyboard);
	setvect(0x31, 0x08, 0x8E, isr_wrapper);
	idt_load();
}
