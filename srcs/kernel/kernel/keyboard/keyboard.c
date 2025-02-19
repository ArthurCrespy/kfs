#include <keyboard.h>
#include <pic.h>

char keyboard_key_to_ascii(enum KEYCODE code) {
	char key = (char) code;

	if (isascii(key) && isprint(key)) {
		if ((_shift && !_capslock) || (!_shift && _capslock)) {
			if (key >= 97 && key <= 122)
				key -= 32;
		}
		return key;
	}
	return 0;
}

uint8_t keyboard_controller_read_status(void) {
	return inb(KB_CTRL_STATUS_REGISTER);
}

void keyboard_controller_send_command(uint8_t cmd) {
	while (1)
		if ((keyboard_controller_read_status() & KB_CTRL_STATUS_MASK_IN_BUFFER) == 0)
			break;
	outb(KB_CTRL_CMD_REGISTER, cmd);
}

uint8_t keyboard_encoder_read_buffer(void) {
	return inb(KB_ENC_INPUT_BUFFER);
}

void keyboard_encoder_send_command(uint8_t cmd) {
	while (1)
		if ((keyboard_controller_read_status() & KB_CTRL_STATUS_MASK_IN_BUFFER) == 0)
			break;
	outb(KB_ENC_CMD_REGISTER, cmd);
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
	keyboard_encoder_send_command(0xFE);
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

	outb(PIC1_DATA, 0xFD);
}
