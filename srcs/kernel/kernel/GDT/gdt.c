#include <gdt.h>

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

// GDT array declared in assembly
extern struct gdt_entry gdt[];

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


// ----- GDT STACK ----- //

// GDT section:
// 0x203818:       0x00000000      0x00000000      0x0000ffff      0x00cf9a00
// 0x203828:       0x0000ffff      0x00cf9200      0x00000000      0x00000000
// [---]
// 0x203818: {limit_low = 0, base_low = 0, base_middle = 0 '\000', access = 0 '\000', granularity = 0 '\000', base_high = 0 '\000'}
// 0x203818: 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
//          |   0x0000|   low   |mid.|acc.|gra.|hig.|
//                                        |granularity: 0x00 = 0000 0000 ; N/A
// 0x203820: {limit_low = 65535, base_low = 0, base_middle = 0 '\000', access = 154 '\232', granularity = 207 '\317', base_high = 0 '\000'}
// 0x203820: 0xff 0xff 0x00 0x00 0x00 0x9a 0xcf 0x00
//          |   0xffff|   low   |mid.|acc.|gra.|hig.|
//                                        |granularity: 0xCF = 1100 1111 ; 4KB blocks, 32-bit mode, 4GB limit
// 0x203828: {limit_low = 65535, base_low = 0, base_middle = 0 '\000', access = 146 '\222', granularity = 207 '\317', base_high = 0 '\000'}
// 0x203828: 0xff 0xff 0x00 0x00 0x00 0x92 0xcf 0x00
//          |   0xffff|   low   |mid.|acc.|gra.|hig.|
//                                        |granularity: 0xCF = 1100 1111 ; 4KB blocks, 32-bit mode, 4GB limit

// summary:
// GDT is allocated correctly by gdt_load()
// GDT entries are set correctly by setentry()