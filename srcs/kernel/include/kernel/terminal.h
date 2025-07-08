#ifndef _TERMINAL_H
#define _TERMINAL_H

#include <ports.h>
#include <vga.h>

#include <stdio.h>
#include <string.h>

#include <stdint.h>
#include <stddef.h>

#define NUM_SCREEN 10
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);
void terminal_delete_last_char(void);
void terminal_load_screen(int new_screen);
void terminal_setcolor(uint8_t color);
void terminal_putchar(char c);
void terminal_writestring(const char* data);
void terminal_init(void);
void terminal_42(void);

#endif
