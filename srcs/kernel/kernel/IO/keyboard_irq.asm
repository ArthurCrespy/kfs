global keyboard_irq_wrapper
extern i86_keyboard_irq

section .text
keyboard_irq_wrapper:
	pusha						; Save all general purpose registers
	call i86_keyboard_irq		; Call the C handler
	popa						; Restore registers

								; Send EOI to the PIC
	mov dx, 0x20				; PIC command port
	mov al, 0x20				; EOI command
	out dx, al

	iretd						; Return from interrupt (32-bit iret)
