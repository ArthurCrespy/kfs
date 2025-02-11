#include "../../include/kernel/idt.h"

struct idt_entry {
	uint16_t offset_low;	// Lower 16 bits of the handler address
	uint16_t selector;		// Kernel code segment selector
	uint8_t  zero;			// Must be zero
	uint8_t  type_attr;		// Type and attributes
	uint16_t offset_high;	// Upper 16 bits of the handler address
} __attribute__((packed));

struct idt_ptr {
	uint16_t limit;			// Limit (size of IDT - 1)
	uint32_t base;			// Base address of the IDT
} __attribute__((packed));

// IDT array declared in assembly
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
	idt_load();
}


// ----- IDT STACK ----- //

// IDT section:
// 0x203018:       0x00000000      0x00000000      0x00000000      0x00000000
// 0x203028:       0x00000000      0x00000000      0x00000000      0x00000000
// [...]
// 0x203118: {offset_low = 2160, selector = 8, zero = 0 '\000', type_attr = 142 '\216', offset_high = 32}
// 0x203118: 0x70 0x08 0x08 0x00 0x00 0x8e 0x20 0x00
//          |   0x0870| select. | 0 | typ |   0x0020|

// symbols table:
// 0x200870 <keyboard_irq_wrapper>

// summary:
// IDT is allocated correctly by idt_load()
// Interruption handler is set correctly by setvect()
// I think the issue is from the GDT because the kernel is not able to load the selector 0x08 from it