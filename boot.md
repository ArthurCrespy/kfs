### Constant and Macros

    MBALIGN  equ  1 << 0
Ensures that any module loaded by the bootloader (e.g. the kernel or additional files) are aligned on page boundaries. *This simplifies paging and memory management, also more efficiency on certain CPUs and MMUs.*

*In x86 architecture, memory is typically managed in pages, which are blocks of memory (commonly 4KB / 4096 bytes in size). A page boundary is a memory address that is divisible by the page size. For a 4KB page, this means addresses like 0x0000, 0x1000 (4096), 0x2000 (8192) etc.*

    MEMINFO  equ  1 << 1
This constant enable the memory map (provides info about the memory layout of the system to the kernel).

    MBFLAGS  equ  MBALIGN | MEMINFO
Combines the 2 flags above. Used in the multiboot header to tell the bootloader which features the kernel supports.

    MAGIC    equ  0x1BADB002
Magic number used by the bootloader to recognize the kernel as multiboot-compatible.

    CHECKSUM equ -(MAGIC + MBFLAGS)
Field used for error-checking.

### Multiboot Header Section

    section .multiboot
This section is where the multiboot header is defined. *The header is a special data structure that tells the bootloader info about the kernel.*

    align 4
Ensures that the header is 4-bytes aligned, which is required by the multiboot specification. *In x86, most data types are 4 bytes in size, and the CPU can read and write memory faster when data is aligned due to its natural size.*

	    dd MAGIC
	    dd MBFLAGS
	    dd CHECKSUM
Defines doubleword (4 bytes). Used to declare the multiboot header's fields. These values are placed in the object file to be recognized by the bootloader

*This header is placed at the beginning of the kernel image (aligned to a 32-bit boundary) and allows the bootloader to identify and pass relevant information about the kernel to the kernel itself.*

### Stack setup

*The multiboot standard doesn't define the value of ESP, it's up to the kernel to provide a stack.*

    section .bss
Defines uninitialized data. The stack for the kernel will be placed here, as it doesn't need any initial data, just space.

    align 16
Ensures that the stack is 16-byte aligned, as required by the System V ABI for x86 systems.

*Many modern CPUs fetch 16bytes at time from memory. If the stack is misaligned, function calls can be slower. Some calling conventions also require stack alignment to ensure that local variables and function arguments are correctly aligned.*

    stack_bottom:
Label that marks the bottom of the stack.

    resb 16384
Reserves 16KiB of space for the stack. *Aligns well with memory pages (4 * 4096)*

    stack_top;
Label that marks the top of the stack, which will be used to initialize the ESP register.

*The stack is set to grow downwards in x86 architecture so stack_top is at the high address and stack_bottom at the low address.*

### Kernel Entry Point

    section .text
Contains the actual code that the kernel will run.

    global _start:function (_start.end - _start)
Declares `_start` as a global symbol, making it the entry point for the kernel. *`(_start.end - start)` calculates the size of the function. This helps debugging tools recognize `_start` as a function and improves readability.*

*The linker script specifies `_start` as the entry point of the kernel, and the bootloader will jump to this position once the kernel has been loaded. There is no return in the function because the bootloader will be gone at this point.*

    _start:
*32-bit protected mode: Interrupts are disabled, paging is disabled. The kernel has full control over the CPU. It can only make use of hardware features and any code it provides as part of itself. There are no security restrictions, no safeguards and no debugging mechanisms, only what the kernel provides itself.*

    mov esp, stack_top
Sets ESP to the top of the stack. Since the stack grows downwards, the top of the stack is the highest memory address, and the stack will grow downward into the reserved 16KiB space.

*At this point the kernel has a functional stack and can safely call functions, including C functions if they exist.*

*Later on, things like floating point instructions, instruction set extensions,GDT, paging and more should be initialized and loaded here.*

### Call to `kernel_main`

*Entering the high-level kernel. The ABI requires the stack to be 16-byte aligned at the time of the call instruction. The stack has been 16-byte aligned during its initialization, and every push to the stack must be a multiple of 16 afterwards to preserve this alignment (for now nothing has been pushed)*

    extern kernel_main
Declares the `kernel_main` function as an external symbol, it's implementation is defined in `kernel.asm`.

    call kernel_main
Transfers control to the `kernel_main` function, which will be the primary entry point for the higher-level kernel code.

*At this point, the kernel starts its high-level operations, such as initializing hardware drivers, setting up memory management and so on.*

### System halt

*If the system has nothing more to do, the computer will be put into an infinite loop.*

    cli
Clears the interrupt flag (disables interrupts). 
*While interrupts are already disabled by the bootloader, it's a good practice to ensure they remain disabled after the transition to the kernel.*

    .hang:  hlt
        jmp .hang
Infinite loop.

    .end:
This label marks the end of the assembly code for the kernel, but isn't used in any active instructions.