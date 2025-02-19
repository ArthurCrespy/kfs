extern isr_handler
extern isr_handler_keyboard

section .text
global isr_wrapper
isr_wrapper:
    pushad
    cld
    call isr_handler
    popad
    iret

global isr_wrapper_keyboard
isr_wrapper_keyboard:
    pushad
    cld
    call isr_handler_keyboard
    popad
    iret