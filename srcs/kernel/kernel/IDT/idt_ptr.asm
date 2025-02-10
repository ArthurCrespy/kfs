section .data
global idt_ptr
; IDT pointer structure 16-bit limit followed by 32-bit base address
idt_ptr:
	dw 256 * 8 - 1		; 256 entries * 8 bytes each - 1
	dd idt				; Base address of the IDT

section .bss
global idt
; Reserve space for the 256 IDT entries
idt:
	resb 256 * 8

section .text
global load_idt
; A simple function that loads the IDT using the lidt instruction.
load_idt:
	lidt [idt_ptr]
	ret
