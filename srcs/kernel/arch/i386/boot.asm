extern kernel_main

; Declare constants for the multiboot header
MBALIGN  equ  1 << 0                ; modules aligned on page boundaries (4096 bytes)   ; 1 << 0 = 00000001 << 0 = 00000001
MEMINFO  equ  1 << 1                ; memory map provided with ‘mem_*’ fields           ; 1 << 1 = 00000001 << 1 = 00000010
MBFLAGS  equ  MBALIGN | MEMINFO     ; multiboot flag field                              ; MBALIGN OR MBALIGN     = 00000011
MAGIC    equ  0x1BADB002            ; magic number lets bootloader find the header
CHECKSUM equ -(MAGIC + MBFLAGS)     ; checksum of above values to tell this is mb       ; inverse of the sum of MAGIC and MBFLAGS

; Declare a multiboot header that marks the program as a kernel
section .multiboot
align 4
	dd MAGIC
	dd MBFLAGS
	dd CHECKSUM

; This allocates room for a small stack
section .bss
align 16
stack_bottom:
resb 16384
stack_top:

; Declare _start as a function symbol with the given symbol size
section .text
global _start
_start:
	mov esp, stack_top              ; Set the esp register to point to the top of our stack
    call kernel_main                ; Enter the high-level kernel
    cli                             ; Disable interrupts
    hlt                             ; Halt the CPU