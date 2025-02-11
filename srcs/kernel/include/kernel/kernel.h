#ifndef _KERNEL_H
#define _KERNEL_H

#include <stddef.h>
#include <stdint.h>

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

void terminal_load_screen(int new_screen);
void terminal_initialize(void);
void terminal_setcolor(uint8_t color);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);

#endif
