/* The bootloader will look at this image and start execution at the symbol designated */
ENTRY(_start)

SECTIONS
{
    /* Usually, between 0x0000 and 0x1000: */
    /* 0x0000–0x03FF : Interrupt Vector Table (IVT) */
    /* 0x0400–0x07FF : BIOS Data Area (BDA) */
    /* 0x0800–0x0FFF : GDT, emergency IDT, bootstrap allocator, etc.. */

    . = 0x0800;

    /* Global Descriptor Table (GDT) */
    .gdt (NOLOAD) : ALIGN(8)
    {
        *(.gdt)
    }

	. = 0x200000;

	/* Put the multiboot header first then the kernel code */
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
		*(.text)
	}

	/* Read-only data */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}

	/* Read-write data (initialized) */
	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}

	/* Read-write data (uninitialized) */
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}
}
