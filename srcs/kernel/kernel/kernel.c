#include "kernel/kernel.h"
#include "kernel/vga.h"
#include "kernel/ports.h"

#if !defined(__i386__)
#error "Not an i386-elf compiler"
#endif

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize(void) 
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = VGA_MEMORY;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color) 
{
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_delete_last_line() {
	for (size_t x = 0; x < VGA_WIDTH; x++) {
		const size_t index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
		terminal_buffer[index] = vga_entry(' ', terminal_color);
	}
}

void terminal_scroll() {
    uint16_t *vga = VGA_MEMORY;

    for (size_t y = 0; y < VGA_HEIGHT - 1; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            vga[y * VGA_WIDTH + x] = vga[(y + 1) * VGA_WIDTH + x];
        }
    }
	terminal_delete_last_line();
}

void terminal_update_cursor(void) {
	uint16_t pos = terminal_row * VGA_WIDTH + terminal_column;

	outb(0x3D4, 14);
	outb(0x3D5, pos >> 8);

	outb(0x3D4, 15);
	outb(0x3D5, pos & 0xFF);
}

void terminal_putchar(char c) 
{
	if (c == '\n') {
		terminal_column = 0;
		++terminal_row;
	}
	else {
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
		if (++terminal_column == VGA_WIDTH) {
			terminal_column = 0;
			++terminal_row;
		}
	}
	if (terminal_row == VGA_HEIGHT) {
		terminal_scroll();
		terminal_row = VGA_HEIGHT - 1;
	}
	terminal_update_cursor();
}

void terminal_write(const char* data, size_t size) 
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) 
{
	terminal_write(data, strlen(data));
}

void kernel_main(void) 
{
	terminal_initialize();

	terminal_setcolor(VGA_COLOR_LIGHT_BLUE);
	terminal_writestring("                                 :::     :::::::: \n");
	terminal_writestring("                               :+:     :+:    :+: \n");
	terminal_writestring("                             +:+ +:+        +:+   \n");
	terminal_writestring("                           +#+  +:+      +#+      \n");
	terminal_writestring("                         +#+#+#+#+#+  +#+         \n");
	terminal_writestring("                              #+#   #+#           \n");
	terminal_writestring("                             ###  ##########      \n");

    terminal_setcolor(VGA_COLOR_WHITE);
	terminal_writestring("\n\nHello, kernel World!\nThis is a newlineeeeeeeee\n");

	terminal_setcolor(VGA_COLOR_RED);
	terminal_writestring("1\n");
	terminal_writestring("2\n");
	terminal_writestring("3\n");
	terminal_writestring("4\n");
	terminal_writestring("5\n");
	terminal_writestring("6\n");
	terminal_writestring("7\n");
	terminal_writestring("8\n");
	terminal_writestring("9\n");
	terminal_writestring("10\n");
	terminal_writestring("11\n");
	terminal_writestring("12\n");
	terminal_writestring("13\n");
	terminal_writestring("14\n");

	terminal_setcolor(VGA_COLOR_GREEN);
    printf("Hello 42");
}
