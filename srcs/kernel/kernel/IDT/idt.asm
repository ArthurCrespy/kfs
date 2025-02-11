section .data
global idt_ptr
; IDT pointer structure
idt_ptr:
	dw 256 * 8 - 1
	dd idt

section .bss
global idt
; Reserve space for the 256 IDT entries
idt:
	resb 256 * 8

section .text
global idt_load
; Load the IDT using the lidt instruction
idt_load:
	lidt [idt_ptr]
	ret
