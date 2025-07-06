section .data
global idt_ptr
; IDT pointer structure
idtr:
	dw 256 * 8 - 1
	dd idt

section .bss
global idt
; Reserve space for 256 IDT entries
idt:
	resb 256 * 8

section .text
global idt_load
; Load the IDT using lidt
idt_load:
	lidt [idtr]
	ret
