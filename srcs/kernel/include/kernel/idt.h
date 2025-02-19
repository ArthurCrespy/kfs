#ifndef _IDT_H
#define _IDT_H

#include <isr.h>

#include <stdint.h>

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

extern void idt_ptr(void);
extern void idt_load(void);

void setvect(uint8_t vector, void (*handler)(void));
void idt_init(void);

#endif
