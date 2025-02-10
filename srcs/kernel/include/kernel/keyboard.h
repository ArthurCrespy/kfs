/*

Ports used for communication in ix86 architecture

				  | PORT  |  READ/WRITE | DESCRIPTION        |
				  | ######################################## |
Keyboard Encoder  | 0x60  |  Read       | Read input buffer  |
				  | 0x60  |  Write      | Send command       |
				  |-------|-------------|--------------------|
Onboard keyboard  | 0x64  |  Read       | Status register    |
controller        | 0x64  |  Write      | Send command       |


Status Register:
	Bit 0: Output buffer status
		0: Empty (don't read yet)
		1: Full (can be read)

	Bit 1: Input buffer status
		0: Empty (can be written)
		1: Full (don't write)

	Bit 2: System flag
		0: set after power on reset
		1: Set after successful completion of kb controller's self test (Basic Assurance Test / BAT)

	Bit 3: Command data
		0: Last write to input buffer was data (via 0x60)
		1: Last write to input buffer was a command (via 0x64)

	Bit 4: Keyboard locked
		0: Locked
		1: Not locked

	Bit 5: Auxiliary output buffer full
		PS/2 Systems :
			0: Determines if read from port 0x60 is valid. If valid, 0=Keyboard data
			1: Mouse data, only if port 0x60 is readable
		AT Systems:
			0: OK flag
			1: Timeout on transmission from controller to keyboard (possibly no keyboard present)
	
	Bit 6: Timeout
		0: OK flag
		1: Timeout
		PS/2: General timeout
		AT: Timeout on transmission from keyboard to controller (possibly parity error -> bits 6 and 7 are set)

	Bit 7: Parity error
		0: OK flag (no error)
		1: Parity error with last byte
*/

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

enum KEYBOARD_ENCODER_IO {
	KB_ENC_INPUT_BUFFER = 0x60,
	KB_ENC_CMD_REGISTER = 0x60
};

enum KEYBOARD_CONTROLLER_IO {
	KB_CTRL_STATUS_REGISTER = 0x64,
	KB_CTRL_CMD_REGISTER = 0x64
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

// Add more according to needs
enum KEYBOARD_CONTROLLER_COMMANDS {
	KB_CTRL_CMD_SELF_TEST = 0xAA,
	KB_CTRL_CMD_DISABLE = 0xAD,
	KB_CTRL_CMD_ENABLE = 0xAE,
	KB_CTRL_CMD_WRITE_OUT_PORT = 0xD1,
	// KB_CTRL_CMD_
};

// enum KEYBOARD_ERROR {

// 	KEYBOARD_ERR_BUF_OVERRUN			=	0,
// 	KEYBOARD_ERR_ID_RET				=	0x83AB,
// 	KEYBOARD_ERR_BAT					=	0xAA,	//note: can also be L. shift key make code
// 	KEYBOARD_ERR_ECHO_RET				=	0xEE,
// 	KEYBOARD_ERR_ACK					=	0xFA,
// 	KEYBOARD_ERR_BAT_FAILED			=	0xFC,
// 	KEYBOARD_ERR_DIAG_FAILED			=	0xFD,
// 	KEYBOARD_ERR_RESEND_CMD			=	0xFE,
// 	KEYBOARD_ERR_KEY					=	0xFF
// };

// return status of tests
bool keyboard_self_test();

// keyboard enable/disable
void	keyboard_disable();
void	keyboard_enable();

// resset system
void	keyboard_reset_system();

void    keyboard_install();

#endif