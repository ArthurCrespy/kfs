global keyboard_i86_irq_asm
extern keyboard_i86_irq

section .text
keyboard_i86_irq_asm:
	pusha						; Save all general purpose registers
	call keyboard_i86_irq		; Call the C handler
	popa						; Restore registers

	mov al, 0x20				; EOI command
	out 0x20, al				; Send to PIC command port

	iretd						; Return from interrupt