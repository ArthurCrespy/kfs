# Linker script

---

Defines how the kernel binary is structured in memory.

- Defines kernel memory layout
- Align sections for proper execution
- Ensure multiboot compatibility
- Combine C and ASM into a single executable
- Separate data and code for protection
 
 ### Entry Point
 
    ENTRY(_start)

Tells the linker that the kernel should start execution at the symbol `_start`. *Defined in `boot.asm`.* The bootloader jumps to this function once it loads the kernel into memory.

### Memory Layout

*The SECTIONS block defines where different parts of the kernel are placed in memory.*

    SECTIONS
    {
	    . = 2M;

The kernel is loaded at 2MB (0x2000000) instead of the traditional 1MB because UEFI systems have made memory layout less predictable. 2MB is a safer choice because it's less likely to be occupied by firmware or reserved memory.

	    .text BLOCK(4K) : ALIGN(4K)

- `.text` contains the kernel's executable code (machine instructions). 
- `BLOCK(4K) : ALIGN(4K)` aligns the section on a 4KB boundary.
- `BLOCK(4K)` ensures that each section starts at a 4KB-aligned address.


	    {
	    	*(.multiboot)

Includes the multiboot header, which the bootloader needs to recognize the kernel.

	    	*(.text)
	    }

Includes all executable code.

### Read-Only Data

	    .rodata BLOCK(4K) : ALIGN(4K)
	    {
	    	*(.rodata)
	    }

Stores constant data (e.g. string literals, lookup tables). *Marked read-only,meaning it cannot be modified at runtime.*

### Initialized Global Variables
	    .data BLOCK(4K) : ALIGN(4K)
	    {
	    	*(.data)
	    }

Contains global and static variables that are initialized before execution. *This section remains writeable because variables can change.*

### Uninitialized Global Variables and Stack
	    .bss BLOCK(4K) : ALIGN(4K)
	    {
	    	*(COMMON)

`COMMON` handles global variables that are declared but not explicitly initialized.

	    	*(.bss)
	    }

Stores uninitialized global/static variables. *These variables are zero-initialized at runtime.*

The stack is also included here, as it does not need to be stored in the kernel binary.