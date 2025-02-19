#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include <idt.h>
#include <keymap.h>
#include <ports.h>
#include <terminal.h>

#include <stdio.h>

#include <stdbool.h>
#include <stdint.h>

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

bool keyboard_self_test(void);
void keyboard_disable(void);
void keyboard_enable(void);
void keyboard_reset_system(void);
void keyboard_init(void);

#endif
