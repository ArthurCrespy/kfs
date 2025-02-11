#include <stdio.h>

#include "../../include/kernel/keyboard.h"
#include "../../include/kernel/ports.h"

static int keyboard_has_data() {
	return inb(PORT_STATUS) & 1;
}

uint8_t get_scancode() {
	while (!keyboard_has_data());
	return inb(PORT_DATA);
}

void keyboard_poll_loop() {
	while (1) {
		uint8_t scancode = get_scancode();
		printf("%c", scancode);
	}
}
