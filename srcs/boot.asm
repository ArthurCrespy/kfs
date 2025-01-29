; ------------------------------------------------------------------------------------- ;
; Multiboot-compliant OS images always contain a magic Multiboot header, which allows the boot loader
; to load the image without having to understand numerous a.out variants or other executable formats

; Declare constants for the multiboot header
MBALIGN  equ  1 << 0                ; modules aligned on page boundaries (4096 bytes)   ; 1 << 0 = 00000001 << 0 = 00000001
MEMINFO  equ  1 << 1                ; memory map provided with ‘mem_*’ fields           ; 1 << 1 = 00000001 << 1 = 00000010
MBFLAGS  equ  MBALIGN | MEMINFO     ; multiboot flag field                              ; MBALIGN OR MBALIGN     = 00000011
MAGIC    equ  0x1BADB002            ; magic number lets bootloader find the header
CHECKSUM equ -(MAGIC + MBFLAGS)     ; checksum of above values to tell this is mb       ; inverse of the sum of MAGIC and MBFLAGS


; ------------------------------------------------------------------------------------- ;
; The bootloader will search for this signature in the first 8 KiB of the kernel file
; Declare a multiboot header that marks the program as a kernel
section .multiboot
align 4
	dd MAGIC
	dd MBFLAGS
	dd CHECKSUM


; ------------------------------------------------------------------------------------- ;
; The multiboot standard does not define the value of the stack pointer register (esp)
; The stack grows downwards on x86. Thus, in its own section it can be marked nobits,
; which means the kernel file is smaller because it does not contain an uninitialized stack
; The stack on x86 must be 16-byte aligned according to the System V ABI standard and de-facto extensions

; This allocates room for a small stack by creating a symbol at the bottom of it,
; then allocating 16384 bytes for it, and finally creating a symbol at the top
section .bss
align 16
stack_bottom:
resb 16384
stack_top:


; ------------------------------------------------------------------------------------- ;
; The linker script specifies _start as the entry point to the kernel and the
; bootloader will jump to this position once the kernel has been loaded
; It doesn't make sense to return from this function as the bootloader is gone

; Declare _start as a function symbol with the given symbol size
section .text
global _start:function (_start.end - _start)
_start:

    ; 32-bit protected mode on x86 machine. Interrupts are disabled. Paging is disabled.
    ; The processor state is as defined in the multiboot standard

	mov esp, stack_top              ; Set the esp register to point to the top of our stack

	; The ABI requires the stack is 16-byte aligned at the time of the call instruction
	; (which afterwards pushes the return pointer of size 4 bytes)
	; The stack was 16-byte aligned and since we've only pushed a multiple of 16 bytes
	; the alignment is preserved and the call is well defined

    extern kernel_main
    call kernel_main                ; Enter the high-level kernel

    ; When the system has nothing more to do, loop until infinite

    cli                             ; Disable interrupts

.hang:	hlt                         ; Wait for the next interrupt who will block the computer
    jmp .hang                       ; Jump to hlt if non-maskable interrupt occurs

.end:
