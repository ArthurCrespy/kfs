section .data
global gdt_ptr
; GDT pointer structure
gdt_ptr:
    dw 3 * 8 - 1
    dd gdt

section .bss
global gdt
; Reserve space for the 3 GDT entries
gdt:
    resb 3 * 8

section .text
global gdt_load
; Load the GDT using the lgdt instruction
gdt_load:
    lgdt [gdt_ptr]
    ret
