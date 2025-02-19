#ifndef _TERMINAL_H
#define _TERMINAL_H

#include <ports.h>
#include <vga.h>

#include <string.h>

#include <stdint.h>
#include <stddef.h>

void terminal_setcolor(uint8_t color);
void terminal_putchar(char c);
void terminal_writestring(const char* data);
void terminal_init(void);

#endif
