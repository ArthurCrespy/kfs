#include <keyboard.h>

uint8_t keyboard_controller_read_status(void) {
	return inb(KB_CTRL_STATUS_PORT);
}

void keyboard_controller_send_command(uint8_t cmd) {
	while (1)
		if ((keyboard_controller_read_status() & KB_CTRL_STATUS_MASK_IN_BUFFER) == 0)
			break;
	outb(KB_CTRL_CMD_PORT, cmd);
}

uint8_t keyboard_encoder_read_buffer(void) {
	return inb(KB_ENC_INPUT_PORT);
}

void keyboard_encoder_send_command(uint8_t cmd) {
	while (1)
		if ((keyboard_controller_read_status() & KB_CTRL_STATUS_MASK_IN_BUFFER) == 0)
			break;
	outb(KB_ENC_CMD_PORT, cmd);
}

bool keyboard_self_test(void) {
	keyboard_controller_send_command(KB_CTRL_CMD_SELF_TEST);
	while (1)
		if (keyboard_controller_read_status() & KB_CTRL_STATUS_MASK_OUT_BUFFER)
			break;
	return (keyboard_encoder_read_buffer() == 0x55) ? true : false;
}

void keyboard_disable(void) {
	keyboard_controller_send_command(KB_CTRL_CMD_DISABLE);
	_keyboard_disable = true;
}

void keyboard_enable(void) {
	keyboard_controller_send_command(KB_CTRL_CMD_ENABLE);
	_keyboard_disable = false;
}

void keyboard_reset_system(void) {
	keyboard_controller_send_command(KB_CTRL_CMD_WRITE_OUT_PORT);
	keyboard_encoder_send_command(KB_CTRL_CMD_RESET);
}

char keyboard_key_to_ascii(enum KEYCODE code) {
	char key = (char) code;
	if (key == KEY_BACKSPACE)
		terminal_delete_last_char();
	if (key == KEY_RETURN)
		terminal_putchar('\n');
	if (isascii(key) && isprint(key)) {
		if ((_shift && !_capslock) || (!_shift && _capslock)) {
			if (key >= 97 && key <= 122)
				key -= 32;
		}
		return key;
	}
	return 0;
}

void keyboard_read(void) {
	uint8_t scancode = keyboard_encoder_read_buffer();
	bool key_released = (scancode & 0x80) != 0;
	uint8_t key_index = scancode & 0x7F;
	enum KEYCODE key = _keyboard_scancode_std[key_index];

	char modifier = 0;

	if (_ctrl && !key_released)
		modifier = '^';
	else if (_alt && !key_released)
		modifier = '@';
	else if (_win && !key_released)
		modifier = '#';

	if (!key_released && (key >= 0x1201 && key <= 0x120e)) {
		terminal_load_screen(key - 0x1201);
		return ;
    }

	switch (key) {
		case KEY_LSHIFT:
		case KEY_RSHIFT:
			_shift = !key_released;
		break;

		case KEY_LCTRL:
		case KEY_RCTRL:
			_ctrl = !key_released;
		break;

		case KEY_LALT:
		case KEY_RALT:
			_alt = !key_released;
		break;

		case KEY_LWIN:
		case KEY_RWIN:
			_win = !key_released;
		break;

		case KEY_CAPSLOCK:
			if (!key_released)
				_capslock = !_capslock;
		break;

		default:
			if (!key_released) {
				char ascii = keyboard_key_to_ascii(key);
				if (ascii) {
					if (modifier)
						terminal_putchar(modifier);
					terminal_putchar(ascii);
				}
			}
		break;
	}
}

void keyboard_init(void) {
	_scancode	= INVALID_SCANCODE;
	_numlock	= false;
	_scrolllock	= false;
	_capslock	= false;
	_shift		= false;
	_alt		= false;
	_ctrl		= false;

//	if (!keyboard_self_test())
//		printf("KO: KB Test\n");
//	else
//		printf("OK: KB Test\n");

	outb(PIC1_DATA, 0xF9);
}
