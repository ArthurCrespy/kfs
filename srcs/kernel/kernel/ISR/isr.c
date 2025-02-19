#include <isr.h>

void isr_handler(void) {
	outb(PIC1_COMMAND, PIC_EOI);
	printf("OK: ISR Routine called (isr_handler)\n");
	printf("    IRQ 0x20 (32) OR 0x31 (49)\n");
}

void isr_handler_keyboard(void) {
	outb(PIC1_COMMAND, PIC_EOI);
	uint8_t scancode = keyboard_encoder_read_buffer();
	printf("OK: ISR Routine called (isr_handler_keyboard)\n");
	printf("    IRQ 0x20 (32)\n");
	printf("    INP: %c (%d)\n", scancode, scancode);

	bool key_released = (scancode & 0x80) != 0;
	uint8_t key_index = scancode & 0x7F;

	enum KEYCODE key = _keyboard_scancode_std[key_index];

	switch (key) {
		case KEY_LSHIFT:
		case KEY_RSHIFT:
			_shift = !key_released;
		break;

		case KEY_LCTRL:
			_ctrl = !key_released;
		break;

		case KEY_RALT:
			_alt = !key_released;
		break;

		case KEY_CAPSLOCK:
			if (!key_released) {
				_capslock = !_capslock;
				// LED logic
			}
		break;

		default:
			if (!key_released) {
				char ascii = keyboard_key_to_ascii(key);
				if (ascii) {
					terminal_putchar(ascii);
					printf("\n");
				}
			}
		break;
	}
	printf("\n");
}