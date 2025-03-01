; inb(uint16_t port) -> uint8_t
; [ebp+8] = port
; Return value in EAX.
section .text
global inb
inb:
	push	ebp
	mov		ebp, esp
	mov		dx, [ebp+8]		; load port number from the stack into DX (temporary 16 bit register)
	in		al, dx			; read a byte from the port specified in DX and store it to AL (AL is the lower 8 bits of AX who is the lower 16 bits of EAX)
	movzx	eax, al			; 8 to 32 bit zero extension into EAX which is the return register
	pop		ebp
	ret

; inw(uint16_t port) -> uint16_t
; [ebp+8] = port
; Return value in EAX.
global inw
inw:
	push	ebp
	mov		ebp, esp
	mov		dx, [ebp+8]		; load port number from the stack into DX (temporary 16 bit register)
	in		ax, dx			; read a word from the port specified in DX and store it to AX (AX is the lower 16 bits of EAX)
	movzx	eax, ax			; 16 to 32 bit zero extension into EAX which is the return register
	pop		ebp
	ret

; outb(uint16_t port, uint8_t data)
; [ebp+8] = port, [ebp+12] = data
; Returns nothing
global outb
outb:
	push	ebp
	mov		ebp, esp
	mov		dx, [ebp+8]		; load the port number from the stack into DX (temporary 16-bit register)
	mov		al, [ebp+12]	; load the byte of data from the stack into AL (AL is the 8 bit pseudo return value register)
	out		dx, al			; write the byte in AL to the I/O port specified by DX
	pop		ebp
	ret

; outw(uint16_t port, uint16_t data)
; [ebp+8] = port, [ebp+12] = data
; Returns nothing
global outw
outw:
	push	ebp
	mov		ebp, esp
	mov		dx, [ebp+8]		; load the port number from the stack into DX (temporary 16-bit register)
	mov		ax, [ebp+12]	; load the word of data from the stack into AX (AX is the 16 bit pseudo return value register)
	out		dx, ax			; write the word in AX to the I/O port specified by DX
	pop		ebp
	ret

global io_wait
io_wait:
	mov	dx, 0x80	; load the port number into DX (0x80 does usually nothing)
	out	dx, al		; write the byte in AL to the I/O port to pass clock cycle
	ret