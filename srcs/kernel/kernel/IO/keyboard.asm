global keyboard_i86_irq_asm
extern keyboard_i86_irq

section .text
keyboard_i86_irq_asm:
	pusha						; Save all general purpose registers
	call keyboard_i86_irq		; Call the C handler
	popa						; Restore registers

								; Send EOI to the PIC
	mov dx, 0x20				; PIC command port
	mov al, 0x20				; EOI command
	out dx, al

	iretd						; Return from interrupt (32-bit iret)
