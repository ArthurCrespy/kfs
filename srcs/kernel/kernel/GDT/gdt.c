#include <gdt.h>

extern struct segment_descriptor gdt[];

void setentry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
	gdt[index].limit_low	= (uint16_t)(limit & 0xFFFF);
	gdt[index].base_low		= (uint16_t)(base & 0xFFFF);
	gdt[index].base_middle	= (uint8_t)((base >> 16) & 0xFF);
	gdt[index].access		= access;
	gdt[index].flags		= (uint8_t)(((limit >> 16) & 0x0F) | (flags & 0xF0));
	gdt[index].base_high	= (uint8_t)((base >> 24) & 0xFF);
}

void gdt_init(void) {
	setentry(0, 0, 0, 0, 0);				// 0: Null segment
	setentry(1, 0, 0xFFFFF, 0x9A, 0xCF);	// 1: Kernel code segment
	setentry(2, 0, 0xFFFFF, 0x92, 0xCF);	// 2: Kernel data segment
	setentry(3, 0, 0xFFFFF, 0x96, 0xCF);	// 3: Kernel stack segment
	setentry(4, 0, 0xFFFFF, 0xFA, 0xCF);	// 4: User code segment
	setentry(5, 0, 0xFFFFF, 0xF2, 0xCF);	// 5: User data segment
	setentry(6, 0, 0xFFFFF, 0xF6, 0xCF);	// 6: User stack segment
	gdt_load();
}
