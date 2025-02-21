# ISR

The Interrupt Service Routines (ISRs) are critical for handling interrupts once they are triggered. They are responsible for managing different interrupt sources, like [hardware IRQs](irq.md) and possibly exceptions.

## Interrupt handling process

When an interrupt is triggered, the CPU will:

- Save its state (registers) to preserve the current execution context

- Transfer control to the appropriate ISR (via the IDT)

- The ISR processes the interrupt, which can involve interacting with hardware (reading from the keyboard, acknowledging the interrupt etc.)

- The ISR ends by restoring the CPU's state and resuming normal program execution

---

	extern isr_handler
	extern isr_handler_keyboard

	section .text
	global isr_wrapper

	isr_wrapper:
		pushad
		cld                    ; Clear the direction flag
		call isr_handler        ; Call the interrupt handler for this interrupt
		popad
		iret                    ; Return from interrupt (restore registers)

`isr_wrapper` is a generic interrupt wrapper that saves registers, calls the C handler (`isr_handler`), and then uses the `iret` instruction to return control back to the interrupted program.

	global isr_wrapper_keyboard

	isr_wrapper_keyboard:
		pushad
		cld                    ; Clear the direction flag
		call isr_handler_keyboard  ; Call the keyboard interrupt handler
		popad
		iret                    ; Return from interrupt (restore registers)

`isr_wrapper_keyboard` is a specific ISR wrapper for handling keyboard interrupts, invoking the `isr_handler_keyboard` function and then returning with `iret`.

- `pushad` pushes all the general purposes registers (eax, ebx, etc.) onto the stack. This saves the state of the registers so that they can be restored later

- `cld` clears the direction flag (DF) in the EFLAGS register. This ensures operations works in the forward direction (from lower to higher memory addresses)

- `popad` pops the saved registers from the stack, restoring the register state to what it was before the interrupt was triggered

- `iret` is used to return from an interrupt. It restores the processor's state and continues execution from where the interrupt occurred, like a normal function return btu with interrupt-specific behavior (restores EFLAGS and CS registers)

```
void isr_handler(void) {
	outb(PIC1_COMMAND, PIC_EOI);       // Send End-of-Interrupt signal to PIC
	printf("OK: ISR Routine called (isr_handler)\n");
	printf("    IRQ 0x20 (32) OR 0x31 (49)\n");
}
```

`isr_handler` is a generic ISR handler called by the wrapper function. It acknowledges the interrupt by sending an End-of-Interrupt (EOI) signal to the [PIC](pic.md) and prints some debug information.

	void isr_handler_keyboard(void) {
		outb(PIC1_COMMAND, PIC_EOI);       // Send End-of-Interrupt signal to PIC
		// printf("OK: ISR Routine called (isr_handler_keyboard)\n");
		// printf("    IRQ 0x21 (33)\n");
		keyboard_read();  // Read keyboard input from the device
	}

`isr_handler_keyboard` handles keyboard interrupts. Like the `isr_handler`, it acknowledges the interrupt byb sending an EOI signal to the PIC and then reads input from the keyboard using the `keyboard_read` function.