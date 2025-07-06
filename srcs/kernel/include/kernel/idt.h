#ifndef _IDT_H
#define _IDT_H

#include <isr.h>

#include <stdint.h>

struct gate_descriptor {
	uint16_t offset_low;	// Lower 16 bits of the handler address
	uint16_t selector;		// Kernel code segment selector
	uint8_t  zero;			// Must be zero
	uint8_t  type_attr;		// Type and attributes
	uint16_t offset_high;	// Upper 16 bits of the handler address
} __attribute__((packed));

struct idtr {
	uint16_t limit;			// Limit (size of IDT - 1)
	uint32_t base;			// Base address of the IDT
} __attribute__((packed));

extern void idtr(void);
extern void idt_load(void);

void setvect(uint8_t vector, uint16_t selector, uint8_t type_attr, void (*handler)(void));
void idt_init(void);

#endif
