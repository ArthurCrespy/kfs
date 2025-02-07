#include "../../include/kernel/keyboard.h"
#include "../../include/kernel/ports.h"
// #include "../../include/kernel/kernel.h"

// // current scan code
// static char _scancode;

// // lock keys
// static bool _numlock, _scrolllock, _capslock;

// // shift, alt, and ctrl keys current state
// static bool _shift, _alt, _ctrl;

// // set if keyboard error
// static int _keyboard_error = 0;

// // set if the Basic Assurance Test (BAT) failed
// static bool _keyboard_bat_res = false;

// // set if diagnostics failed
// static bool _keyboard_diag_res = false;

// // set if system should resend last command
// static bool _keyboard_resend_res = false;

// set if keyboard is disabled
bool _keyboard_disable = true;

// static int _keyboard_scancode_std [] = {

// 	//! key			scancode
// 	KEY_UNKNOWN,	//0
// 	KEY_ESCAPE,		//1
// 	KEY_1,			//2
// 	KEY_2,			//3
// 	KEY_3,			//4
// 	KEY_4,			//5
// 	KEY_5,			//6
// 	KEY_6,			//7
// 	KEY_7,			//8
// 	KEY_8,			//9
// 	KEY_9,			//0xa
// 	KEY_0,			//0xb
// 	KEY_MINUS,		//0xc
// 	KEY_EQUAL,		//0xd
// 	KEY_BACKSPACE,	//0xe
// 	KEY_TAB,		//0xf
// 	KEY_Q,			//0x10
// 	KEY_W,			//0x11
// 	KEY_E,			//0x12
// 	KEY_R,			//0x13
// 	KEY_T,			//0x14
// 	KEY_Y,			//0x15
// 	KEY_U,			//0x16
// 	KEY_I,			//0x17
// 	KEY_O,			//0x18
// 	KEY_P,			//0x19
// 	KEY_LEFTBRACKET,//0x1a
// 	KEY_RIGHTBRACKET,//0x1b
// 	KEY_RETURN,		//0x1c
// 	KEY_LCTRL,		//0x1d
// 	KEY_A,			//0x1e
// 	KEY_S,			//0x1f
// 	KEY_D,			//0x20
// 	KEY_F,			//0x21
// 	KEY_G,			//0x22
// 	KEY_H,			//0x23
// 	KEY_J,			//0x24
// 	KEY_K,			//0x25
// 	KEY_L,			//0x26
// 	KEY_SEMICOLON,	//0x27
// 	KEY_QUOTE,		//0x28
// 	KEY_GRAVE,		//0x29
// 	KEY_LSHIFT,		//0x2a
// 	KEY_BACKSLASH,	//0x2b
// 	KEY_Z,			//0x2c
// 	KEY_X,			//0x2d
// 	KEY_C,			//0x2e
// 	KEY_V,			//0x2f
// 	KEY_B,			//0x30
// 	KEY_N,			//0x31
// 	KEY_M,			//0x32
// 	KEY_COMMA,		//0x33
// 	KEY_DOT,		//0x34
// 	KEY_SLASH,		//0x35
// 	KEY_RSHIFT,		//0x36
// 	KEY_KP_ASTERISK,//0x37
// 	KEY_RALT,		//0x38
// 	KEY_SPACE,		//0x39
// 	KEY_CAPSLOCK,	//0x3a
// 	KEY_F1,			//0x3b
// 	KEY_F2,			//0x3c
// 	KEY_F3,			//0x3d
// 	KEY_F4,			//0x3e
// 	KEY_F5,			//0x3f
// 	KEY_F6,			//0x40
// 	KEY_F7,			//0x41
// 	KEY_F8,			//0x42
// 	KEY_F9,			//0x43
// 	KEY_F10,		//0x44
// 	KEY_KP_NUMLOCK,	//0x45
// 	KEY_SCROLLLOCK,	//0x46
// 	KEY_HOME,		//0x47
// 	KEY_KP_8,		//0x48	//keypad up arrow
// 	KEY_PAGEUP,		//0x49
// 	KEY_KP_2,		//0x50	//keypad down arrow
// 	KEY_KP_3,		//0x51	//keypad page down
// 	KEY_KP_0,		//0x52	//keypad insert key
// 	KEY_KP_DECIMAL,	//0x53	//keypad delete key
// 	KEY_UNKNOWN,	//0x54
// 	KEY_UNKNOWN,	//0x55
// 	KEY_UNKNOWN,	//0x56
// 	KEY_F11,		//0x57
// 	KEY_F12			//0x58
// };

// invalid scan code. Used to indicate the last scan code is not to be reused
// const int INVALID_SCANCODE = 0;

char keyboard_key_to_ascii (enum KEYCODE code);

uint8_t	keyboard_controller_read_status() {
	return inb (KB_CTRL_STATUS_REGISTER);
}

void	keyboard_controller_send_command(uint8_t cmd) {
	// Wait for keyboard controller's input buffer to be clear
	while (1)
		if ((keyboard_controller_read_status() & KB_CTRL_STATUS_MASK_IN_BUFFER) == 0)
			break;
	outb(KB_CTRL_CMD_REGISTER, cmd);
}

uint8_t	keyboard_encoder_read_buffer() {
	return inb(KB_ENC_INPUT_BUFFER);
}

void	keyboard_encoder_send_command(uint8_t cmd) {
	// Wait for keyboard controller's input buffer to be clear
	while (1)
		if ((keyboard_controller_read_status() & KB_CTRL_STATUS_MASK_IN_BUFFER) == 0)
			break;
	outb(KB_ENC_CMD_REGISTER, cmd);
}

// interrupt handler
// void __attribute__((cdecl)) i86_keyboard_irq () {

// 	asm volatile ("add esp, 12");
// 	asm volatile ("pushad");
// 	asm volatile ("cli");

// 	static bool _extended = false;

// 	int code = 0;

// 	//! read scan code only if the kkybrd controller output buffer is full (scan code is in it)
// 	if (keyboard_controller_read_status () & KB_CTRL_STATUS_MASK_OUT_BUFFER) {

// 		//! read the scan code
// 		code = keyboard_encoder_read_buffer ();

// 		if (code < 0x80) {
// 			char ascii = keyboard_key_to_ascii((enum KEYCODE)_keyboard_scancode_std[code]);
// 			if (ascii)
// 				terminal_putchar(ascii);
// 		}
// 	}
// 	// send EOI signal to the PIC at port 0x20
// 	outb(0x20, 0x20);

// 	// return from interrupt handler
// 	asm volatile ("sti");
// 	asm volatile ("popad");
// 	asm volatile ("iretd");
// }

// enum KEYCODE keyboard_get_last_key () {

// 	return (_scancode!=INVALID_SCANCODE) ? ((enum KEYCODE)_keyboard_scancode_std [_scancode]) : (KEY_UNKNOWN);
// }

// // discards last scan
// void	keyboard_discard_last_key() {
// 	_scancode = INVALID_SCANCODE;
// }

// char keyboard_key_to_ascii (enum KEYCODE code) {

// 	uint8_t key = code;

// 	//! insure key is an ascii character
// 	if (isascii (key)) {

// 		//! if shift key is down or caps lock is on, make the key uppercase
// 		if (_shift || _capslock)
// 			if (key >= 'a' && key <= 'z')
// 				key -= 32;

// 		if (_shift && !_capslock)
// 			if (key >= '0' && key <= '9')
// 				switch (key) {

// 					case '0':
// 						key = KEY_RIGHTPARENTHESIS;
// 						break;
// 					case '1':
// 						key = KEY_EXCLAMATION;
// 						break;
// 					case '2':
// 						key = KEY_AT;
// 						break;
// 					case '3':
// 						key = KEY_EXCLAMATION;
// 						break;
// 					case '4':
// 						key = KEY_HASH;
// 						break;
// 					case '5':
// 						key = KEY_PERCENT;
// 						break;
// 					case '6':
// 						key = KEY_CARRET;
// 						break;
// 					case '7':
// 						key = KEY_AMPERSAND;
// 						break;
// 					case '8':
// 						key = KEY_ASTERISK;
// 						break;
// 					case '9':
// 						key = KEY_LEFTPARENTHESIS;
// 						break;
// 				}
// 			else {

// 				switch (key) {
// 					case KEY_COMMA:
// 						key = KEY_LESS;
// 						break;

// 					case KEY_DOT:
// 						key = KEY_GREATER;
// 						break;

// 					case KEY_SLASH:
// 						key = KEY_QUESTION;
// 						break;

// 					case KEY_SEMICOLON:
// 						key = KEY_COLON;
// 						break;

// 					case KEY_QUOTE:
// 						key = KEY_QUOTEDOUBLE;
// 						break;

// 					case KEY_LEFTBRACKET :
// 						key = KEY_LEFTCURL;
// 						break;

// 					case KEY_RIGHTBRACKET :
// 						key = KEY_RIGHTCURL;
// 						break;

// 					case KEY_GRAVE:
// 						key = KEY_TILDE;
// 						break;

// 					case KEY_MINUS:
// 						key = KEY_UNDERSCORE;
// 						break;

// 					case KEY_PLUS:
// 						key = KEY_EQUAL;
// 						break;

// 					case KEY_BACKSLASH:
// 						key = KEY_BAR;
// 						break;
// 				}
// 			}

// 		//! return the key
// 		return key;
// 	}

// 	//! scan code != a valid ascii char so no conversion is possible
// 	return 0;
// }

bool	keyboard_self_test() {
	keyboard_controller_send_command(KB_CTRL_CMD_SELF_TEST);
	while (1)
		if (keyboard_controller_read_status() & KB_CTRL_STATUS_MASK_OUT_BUFFER)
			break;
	return (keyboard_encoder_read_buffer() == 0x55) ? true : false;
}

void	keyboard_disable() {
	keyboard_controller_send_command(KB_CTRL_CMD_DISABLE);
	_keyboard_disable = true;
}

void	keyboard_enable() {
	keyboard_controller_send_command(KB_CTRL_CMD_ENABLE);
	_keyboard_disable = false;
}

void	keyboard_reset_system() {
	// writes 11111110 to the output port
	keyboard_controller_send_command(KB_CTRL_CMD_WRITE_OUT_PORT);
	keyboard_encoder_send_command(0xfe);
}

// // prepares driver for use
// void keyboard_install (int irq) {

// 	//! Install our interrupt handler (irq 1 uses interrupt 33)
// 	setvect (irq, i86_keyboard_irq);

// 	//! assume BAT test is good. If there is a problem, the IRQ handler where catch the error
// 	_keyboard_bat_res = true;
// 	_scancode = 0;

// 	//! set lock keys and led lights
// 	_numlock = _scrolllock = _capslock = false;
// 	// kkybrd_set_leds (false, false, false);

// 	//! shift, ctrl, and alt keys
// 	_shift = _alt = _ctrl = false;
// }
