#include "../../include/kernel/idt.h"

struct idt_entry {
	uint16_t offset_low;	// Lower 16 bits of the handler address.
	uint16_t selector;		// Kernel code segment selector.
	uint8_t  zero;			// Must be zero.
	uint8_t  type_attr;		// Type and attributes
	uint16_t offset_high;	// Upper 16 bits of the handler address.
};

/* Declare the IDT array defined in assembly */
extern struct idt_entry idt[];

void setvect(uint8_t vector, void (*handler)(void)) {
	uint32_t handler_addr = (uint32_t) handler;

	idt[vector].offset_low	= handler_addr & 0xFFFF;
	idt[vector].selector	= 0x08;
	idt[vector].zero		= 0;
	idt[vector].type_attr	= 0x8E;
	idt[vector].offset_high	= (handler_addr >> 16) & 0xFFFF;
}
