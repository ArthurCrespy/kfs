#include <isr.h>

void isr_handler(void) {
	outb(PIC1_COMMAND, PIC_EOI);
	printf("OK: ISR Routine called (isr_handler)\n");
	printf("    IRQ 0x20 (32) OR 0x31 (49)\n");
}

void isr_handler_keyboard(void) {
	outb(PIC1_COMMAND, PIC_EOI);
	// printf("OK: ISR Routine called (isr_handler_keyboard)\n");
	// printf("    IRQ 0x21 (33)\n");
	keyboard_read();
}