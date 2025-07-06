section .gdtr
global gdtr
; GDT pointer structure
gdtr:
    dw 7 * 8 - 1
    dd gdt

section .gdt
global gdt
; Reserve space for GDT entries
gdt:
    resb 7 * 8

section .text
global gdt_load
; Load the GDT using lgdt
gdt_load:
    lgdt [gdtr]
    jmp 0x08:.load  ; Ring 0: (1 << 3) | 0
.load:
    mov ax, 0x10    ; Ring 0: (2 << 3) | 0
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ax, 0x18    ; Ring 0: (3 << 3) | 0
    mov ss, ax
    ret
