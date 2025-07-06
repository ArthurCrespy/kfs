/* The bootloader will look at this image and start execution at the symbol designated as the entry point. */
ENTRY(_start)

/* Tell where the various sections of the object files will be put in the final kernel image. */
SECTIONS
{
    /* Usually, between 0x0000 and 0x1000, the sapce is reserved for: */
    /* 0x0000–0x03FF : IVT (Interrupt Vector Table) */
    /* 0x0400–0x07FF : BIOS Data Area (BDA) */
    /* 0x0800–0x0FFF : Other low level stuff (GDT, emergency IDT, bootstrap allocator, etc..) */
    . = 0x0800;
    .gdt_ptr ALIGN(8) :
    {
        *(.gdt_ptr)
    }

    .gdt ALIGN(8) :
    {
        *(.gdt)
    }

    /* Go to 2MiB offset for the kernel image */
	. = 2M;

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
	    /* Symbols COMM */
		*(COMMON)
		/* Uninitialized data */
		*(.bss)
	}
}