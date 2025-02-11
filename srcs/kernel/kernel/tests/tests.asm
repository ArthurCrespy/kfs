extern idt
global osdev_test1_asm

%define CODE_SELECTOR 0x08
%define LINEAR_DATA_SELECTOR 0x10

int_handler:
    mov ax, LINEAR_DATA_SELECTOR
    mov gs, ax
    mov dword [gs:0xB8000],'O K '
    hlt

; https://wiki.osdev.org/I_Can%27t_Get_Interrupts_Working#Assembly_Examples
osdev_test1_asm:
    mov eax,int_handler
    mov [idt+49*8],ax
    mov word [idt+49*8+2],CODE_SELECTOR
    mov word [idt+49*8+4],0x8E00
    shr eax,16
    mov [idt+49*8+6],ax
    int 49