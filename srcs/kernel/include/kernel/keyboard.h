#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include <idt.h>
#include <keymap.h>
#include <pic.h>
#include <ports.h>
#include <terminal.h>

#include <ctype.h>
#include <stdio.h>

#include <stdbool.h>
#include <stdint.h>

#define INVALID_SCANCODE 0
#define PORT_DATA 0x60
#define PORT_STATUS 0x64

enum KEYBOARD_ENCODER_IO {
	KB_ENC_INPUT_BUFFER	= 0x60,
	KB_ENC_CMD_REGISTER	= 0x60
};

enum KEYBOARD_CONTROLLER_IO {
	KB_CTRL_STATUS_REGISTER	= 0x64,
	KB_CTRL_CMD_REGISTER	= 0x64
};

enum KEYBOARD_CONTROLLER_STATUS_MASK {
	KB_CTRL_STATUS_MASK_OUT_BUFFER	= 1,
	KB_CTRL_STATUS_MASK_IN_BUFFER	= 2,
	KB_CTRL_STATUS_MASK_SYSTEM		= 4,
	KB_CTRL_STATUS_MASK_CMD_DATA	= 8,
	KB_CTRL_STATUS_MASK_LOCKED		= 0x10,
	KB_CTRL_STATUS_MASK_AUX_BUFFER	= 0x20,
	KB_CTRL_STATUS_MASK_TIMEOUT		= 0x40,
	KB_CTRL_STATUS_MASK_PARITY		= 0x80
};

enum KEYBOARD_CONTROLLER_COMMANDS {
	KB_CTRL_CMD_SELF_TEST		= 0xAA,
	KB_CTRL_CMD_DISABLE			= 0xAD,
	KB_CTRL_CMD_ENABLE			= 0xAE,
	KB_CTRL_CMD_WRITE_OUT_PORT	= 0xD1,
};

enum KEYBOARD_ERROR {
	KEYBOARD_ERR_BUF_OVERRUN		=	0,
	KEYBOARD_ERR_ID_RET				=	0x83AB,
	KEYBOARD_ERR_BAT				=	0xAA,
	KEYBOARD_ERR_ECHO_RET			=	0xEE,
	KEYBOARD_ERR_ACK				=	0xFA,
	KEYBOARD_ERR_BAT_FAILED			=	0xFC,
	KEYBOARD_ERR_DIAG_FAILED		=	0xFD,
	KEYBOARD_ERR_RESEND_CMD			=	0xFE,
	KEYBOARD_ERR_KEY				=	0xFF
};

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
static bool _keyboard_disable = true;

extern void isr_handler_keyboard_asm(void);

char keyboard_key_to_ascii(enum KEYCODE code);
uint8_t keyboard_encoder_read_buffer(void);
bool keyboard_self_test(void);
void keyboard_disable(void);
void keyboard_enable(void);
void keyboard_reset_system(void);
void keyboard_init(void);

#endif
