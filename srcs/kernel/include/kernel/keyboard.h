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
	KB_ENC_INPUT_PORT	= 0x60,
	KB_ENC_CMD_PORT		= 0x60
};

enum KEYBOARD_CONTROLLER_IO {
	KB_CTRL_STATUS_PORT	= 0x64,
	KB_CTRL_CMD_PORT	= 0x64
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
	KB_CTRL_CMD_RESET			= 0xFE,
};

enum KEYBOARD_ERROR {
	KEYBOARD_ERR_BUF_OVERRUN	=	0,
	KEYBOARD_ERR_ID_RET			=	0x83AB,
	KEYBOARD_ERR_BAT			=	0xAA,
	KEYBOARD_ERR_ECHO_RET		=	0xEE,
	KEYBOARD_ERR_ACK			=	0xFA,
	KEYBOARD_ERR_BAT_FAILED		=	0xFC,
	KEYBOARD_ERR_DIAG_FAILED	=	0xFD,
	KEYBOARD_ERR_RESEND_CMD		=	0xFE,
	KEYBOARD_ERR_KEY			=	0xFF
};

static int _keyboard_scancode_std [] = {
	KEY_UNKNOWN,
	KEY_ESCAPE,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_0,
	KEY_MINUS,
	KEY_EQUAL,
	KEY_BACKSPACE,
	KEY_TAB,
	KEY_Q,
	KEY_W,
	KEY_E,
	KEY_R,
	KEY_T,
	KEY_Y,
	KEY_U,
	KEY_I,
	KEY_O,
	KEY_P,
	KEY_LEFTBRACKET,
	KEY_RIGHTBRACKET,
	KEY_RETURN,
	KEY_LCTRL,
	KEY_A,
	KEY_S,
	KEY_D,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_SEMICOLON,
	KEY_QUOTE,
	KEY_GRAVE,
	KEY_LSHIFT,
	KEY_BACKSLASH,
	KEY_Z,
	KEY_X,
	KEY_C,
	KEY_V,
	KEY_B,
	KEY_N,
	KEY_M,
	KEY_COMMA,
	KEY_DOT,
	KEY_SLASH,
	KEY_RSHIFT,
	KEY_KP_ASTERISK,
	KEY_RALT,
	KEY_SPACE,
	KEY_CAPSLOCK,
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_KP_NUMLOCK,
	KEY_SCROLLLOCK,
	KEY_HOME,
	KEY_KP_8,
	KEY_PAGEUP,
	KEY_KP_2,
	KEY_KP_3,
	KEY_KP_0,
	KEY_KP_DECIMAL,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_F11,
	KEY_F12
};

static char _scancode = INVALID_SCANCODE;

static bool _numlock	= false;
static bool _scrolllock	= false;
static bool _capslock	= false;
static bool _shift		= false;
static bool _alt		= false;
static bool _ctrl		= false;
static bool _win		= false;

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
void keyboard_read(void);
void keyboard_init(void);

#endif
