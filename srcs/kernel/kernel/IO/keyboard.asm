global keyboard_i86_irq_asm
extern keyboard_i86_irq

section .text
keyboard_i86_irq_asm:
	pusha						; Save registers
	call keyboard_i86_irq		; Call C handler
	popa						; Restore registers
	mov al, 0x20				; Store EOI code
	out 0x20, al				; Send EOI to PIC
	iretd						; Return from interrupt