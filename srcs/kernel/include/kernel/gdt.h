#ifndef _GDT_H
#define _GDT_H

#include <stdint.h>

struct gdt_entry {
	uint16_t limit_low;		// Lower 16 bits of the segment limit
	uint16_t base_low;		// Lower 16 bits of the base address
	uint8_t  base_middle;	// Next 8 bits of the base address
	uint8_t  access;		// Access flags determine ring and segment type
	uint8_t  flags;			// Flags and high 4 bits of limit
	uint8_t  base_high;		// Highest 8 bits of the base address
} __attribute__((packed));

struct gdt_ptr {
	uint16_t limit;			// Limit (size of GDT - 1)
	uint32_t base;			// Base address of the GDT
} __attribute__((packed));

extern void gdt_ptr(void);
extern void gdt_load(void);

void setentry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags);
void gdt_init(void);

#endif
