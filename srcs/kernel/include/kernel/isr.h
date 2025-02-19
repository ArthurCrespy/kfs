#ifndef _ISR_H
#define _ISR_H

#include <keyboard.h>
#include <pic.h>
#include <ports.h>
#include <tests.h>

#include <stdio.h>

extern void isr_wrapper(void);
extern void isr_wrapper_keyboard(void);

void isr_handler(void);
void isr_handler_keyboard(void);

#endif
