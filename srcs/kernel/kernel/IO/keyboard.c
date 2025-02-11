#include <stdio.h>

#include "../../include/kernel/keyboard.h"
#include "../../include/kernel/ports.h"

char scancode_to_ascii[] = {
	0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
	'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
	0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
	0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
	'*', 0, ' ', 0
};

char scancode_to_ascii_shifted[] = {
	0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
	'\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
	0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
	0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
	'*', 0, ' ', 0
};

int shift_pressed = 0;

static int keyboard_has_data() {
	return inb(PORT_STATUS) & 1;
}

uint8_t get_scancode() {
	while (!keyboard_has_data());
	return inb(PORT_DATA);
}

char get_ascii_from_scancode(uint8_t scancode) {
	if (scancode == 0x2A || scancode == 0x36) { // Left or Right Shift pressed
		shift_pressed = 1;
		return 0;
	}
	if (scancode == 0xAA || scancode == 0xB6) { // Left or Right Shift released
		shift_pressed = 0;
		return 0;
	}

	if (scancode > 0x39) return 0; // Ignore unknown keys

	return shift_pressed ? scancode_to_ascii_shifted[scancode] : scancode_to_ascii[scancode];
}

// Highly inefficient, switch to interrupt when IDT and GDT are set up
void keyboard_poll_loop() {
	while (1) {
		uint8_t scancode = get_scancode();
		char c = get_ascii_from_scancode(scancode);
		if (c) printf("%c", c);
	}
}
