section .data
global gdt_ptr
; GDT pointer structure
gdt_ptr:
    dw 7 * 8 - 1
    dd gdt

section .bss
global gdt
; Reserve space for GDT entries
gdt:
    resb 7 * 8

section .text
global gdt_load
; Load the GDT using lgdt
gdt_load:
    lgdt [gdt_ptr]
    jmp 0x08:.load  ; 1: Code segment
.load:
    mov ax, 0x10
    mov ds, ax      ; 2: Data segment
    mov es, ax      ; 2: Data destination segment
    mov fs, ax      ; 2: Data TLS segment
    mov gs, ax      ; 2: Data extra segment
    mov ax, 0x18
    mov ss, ax      ; 3: Stack segment
    ret
