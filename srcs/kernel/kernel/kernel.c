#include "../include/kernel/kernel.h"
#include "../include/kernel/vga.h"
#include "../include/kernel/ports.h"
#include "../include/kernel/keyboard.h"
#include "../include/kernel/idt.h"
#include "../include/kernel/gdt.h"
#include "../include/kernel/tests.h"

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
	gdt_init();
	idt_init();

	terminal_initialize();

	printf("KO <- ASM IRQ 49\n");

	keyboard_init();

	terminal_setcolor(VGA_COLOR_LIGHT_BLUE);
	terminal_writestring("                                 :::     :::::::: \n");
	terminal_writestring("                               :+:     :+:    :+: \n");
	terminal_writestring("                             +:+ +:+        +:+   \n");
	terminal_writestring("                           +#+  +:+      +#+      \n");
	terminal_writestring("                         +#+#+#+#+#+  +#+         \n");
	terminal_writestring("                              #+#   #+#           \n");
	terminal_writestring("                             ###  ##########      \n");

	terminal_setcolor(VGA_COLOR_WHITE);
	terminal_writestring("\n\nHello ");
	printf("world 42 kernel!\n\n");

	terminal_setcolor(VGA_COLOR_LIGHT_GREY);

	test_irq_49();
	// test_irq_49_asm();

	printf("OK <- CPU still running\n");

	test_irq_32();

	printf("OK <- CPU still running\n");
}
