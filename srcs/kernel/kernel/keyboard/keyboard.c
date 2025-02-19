#include <keyboard.h>

static char _scancode = INVALID_SCANCODE;

static bool _numlock	= false;
static bool _scrolllock	= false;
static bool _capslock	= false;
static bool _shift		= false;
static bool _alt		= false;
static bool _ctrl		= false;

// static int _keyboard_error = 0;
// static bool _keyboard_bat_res = false;
// static bool _keyboard_diag_res = false;
// static bool _keyboard_resend_res = false;
bool _keyboard_disable = true;

static int _keyboard_scancode_std [] = {
	//! key			scancode
	KEY_UNKNOWN,	//0
	KEY_ESCAPE,		//1
	KEY_1,			//2
	KEY_2,			//3
	KEY_3,			//4
	KEY_4,			//5
	KEY_5,			//6
	KEY_6,			//7
	KEY_7,			//8
	KEY_8,			//9
	KEY_9,			//0xa
	KEY_0,			//0xb
	KEY_MINUS,		//0xc
	KEY_EQUAL,		//0xd
	KEY_BACKSPACE,	//0xe
	KEY_TAB,		//0xf
	KEY_Q,			//0x10
	KEY_W,			//0x11
	KEY_E,			//0x12
	KEY_R,			//0x13
	KEY_T,			//0x14
	KEY_Y,			//0x15
	KEY_U,			//0x16
	KEY_I,			//0x17
	KEY_O,			//0x18
	KEY_P,			//0x19
	KEY_LEFTBRACKET,//0x1a
	KEY_RIGHTBRACKET,//0x1b
	KEY_RETURN,		//0x1c
	KEY_LCTRL,		//0x1d
	KEY_A,			//0x1e
	KEY_S,			//0x1f
	KEY_D,			//0x20
	KEY_F,			//0x21
	KEY_G,			//0x22
	KEY_H,			//0x23
	KEY_J,			//0x24
	KEY_K,			//0x25
	KEY_L,			//0x26
	KEY_SEMICOLON,	//0x27
	KEY_QUOTE,		//0x28
	KEY_GRAVE,		//0x29
	KEY_LSHIFT,		//0x2a
	KEY_BACKSLASH,	//0x2b
	KEY_Z,			//0x2c
	KEY_X,			//0x2d
	KEY_C,			//0x2e
	KEY_V,			//0x2f
	KEY_B,			//0x30
	KEY_N,			//0x31
	KEY_M,			//0x32
	KEY_COMMA,		//0x33
	KEY_DOT,		//0x34
	KEY_SLASH,		//0x35
	KEY_RSHIFT,		//0x36
	KEY_KP_ASTERISK,//0x37
	KEY_RALT,		//0x38
	KEY_SPACE,		//0x39
	KEY_CAPSLOCK,	//0x3a
	KEY_F1,			//0x3b
	KEY_F2,			//0x3c
	KEY_F3,			//0x3d
	KEY_F4,			//0x3e
	KEY_F5,			//0x3f
	KEY_F6,			//0x40
	KEY_F7,			//0x41
	KEY_F8,			//0x42
	KEY_F9,			//0x43
	KEY_F10,		//0x44
	KEY_KP_NUMLOCK,	//0x45
	KEY_SCROLLLOCK,	//0x46
	KEY_HOME,		//0x47
	KEY_KP_8,		//0x48	//keypad up arrow
	KEY_PAGEUP,		//0x49
	KEY_KP_2,		//0x50	//keypad down arrow
	KEY_KP_3,		//0x51	//keypad page down
	KEY_KP_0,		//0x52	//keypad insert key
	KEY_KP_DECIMAL,	//0x53	//keypad delete key
	KEY_UNKNOWN,	//0x54
	KEY_UNKNOWN,	//0x55
	KEY_UNKNOWN,	//0x56
	KEY_F11,		//0x57
	KEY_F12			//0x58
};

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
	keyboard_encoder_send_command(0xfe);
}

void keyboard_i86_irq(void) {
	uint8_t scancode = keyboard_encoder_read_buffer();

	bool key_released = (scancode & 0x80) != 0;
	uint8_t key_index = scancode & 0x7F;

	enum KEYCODE key = _keyboard_scancode_std[key_index];

	printf("OK: IRQ Routine called (kb_i86_irq)\n");
	printf("    Scancode: %d\n", scancode);

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
				if (ascii)
					terminal_putchar(ascii);
			}
		break;
	}
	outb(0x20, 0x20);
}

extern void keyboard_i86_irq_asm(void);

void keyboard_init(void) {
	setvect(0x20, keyboard_i86_irq);
//	setvect(0x20, keyboard_i86_irq_asm);

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

	keyboard_enable();
}
