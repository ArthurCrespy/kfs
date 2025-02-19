extern isr_handler
extern isr_handler_keyboard

section .text
global isr_wrapper
isr_wrapper:
    pushad
    cld							; Clear the direction flag
    call isr_handler
    popad
    iret						; Return from interrupt

global isr_wrapper_keyboard
isr_wrapper_keyboard:
    pushad
    cld							; Clear the direction flag
    call isr_handler_keyboard
    popad
    iret						; Return from interrupt