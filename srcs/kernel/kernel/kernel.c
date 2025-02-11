#include "../include/kernel/kernel.h"
#include "../include/kernel/vga.h"
#include "../include/kernel/ports.h"
#include "../include/kernel/keyboard.h"
// #include "../include/kernel/idt.h"


size_t terminal_row[NUM_SCREEN];
size_t terminal_column[NUM_SCREEN];
uint8_t terminal_color[NUM_SCREEN];
uint16_t* terminal_buffer[NUM_SCREEN];
// uint16_t *screens_buffer[NUM_SCREEN];

uint16_t *vga = VGA_MEMORY;


int current_screen = 0;

void terminal_load_screen(int new_screen) {
	if (new_screen < 0 || new_screen >= NUM_SCREEN) return;

	vga = terminal_buffer[new_screen];
	current_screen = new_screen;
}

void terminal_initialize(void) 
{
	terminal_buffer[0] = VGA_MEMORY; // load screen 0
	for (int screen = 0; screen < NUM_SCREEN; screen++){
		terminal_row[screen] = 0;
		terminal_column[screen] = 0;
		terminal_color[screen] = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
		for (size_t y = 0; y < VGA_HEIGHT; y++) {
			for (size_t x = 0; x < VGA_WIDTH; x++) {
				const size_t index = y * VGA_WIDTH + x;
				terminal_buffer[screen][index] = vga_entry(' ', terminal_color[screen]);
			}
		}
	}
}

void terminal_setcolor(uint8_t color) 
{
	terminal_color[current_screen] = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[current_screen][index] = vga_entry(c, color);
}

void terminal_delete_last_line() {
	for (size_t x = 0; x < VGA_WIDTH; x++) {
		const size_t index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
		terminal_buffer[current_screen][index] = vga_entry(' ', terminal_color[current_screen]);
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
	uint16_t pos = terminal_row[current_screen] * VGA_WIDTH + terminal_column[current_screen];

	outb(0x3D4, 14);
	outb(0x3D5, pos >> 8);

	outb(0x3D4, 15);
	outb(0x3D5, pos & 0xFF);
}

void terminal_putchar(char c) 
{
	if (c == '\n') {
		terminal_column[current_screen] = 0;
		++terminal_row[current_screen];
	}
	else {
		terminal_putentryat(c, terminal_color[current_screen], terminal_column[current_screen], terminal_row[current_screen]);
		if (++terminal_column[current_screen] == VGA_WIDTH) {
			terminal_column[current_screen] = 0;
			++terminal_row[current_screen];
		}
	}
	if (terminal_row[current_screen] == VGA_HEIGHT) {
		terminal_scroll();
		terminal_row[current_screen] = VGA_HEIGHT - 1;
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
	printf("                                 :::     :::::::: \n");
	printf("                               :+:     :+:    :+: \n");
	printf("                             +:+ +:+        +:+   \n");
	printf("                           +#+  +:+      +#+      \n");
	printf("                         +#+#+#+#+#+  +#+         \n");
	printf("                              #+#   #+#           \n");
	printf("                             ###  ##########      \n");

	terminal_setcolor(VGA_COLOR_GREEN);
	printf("Hello 42\n");
	keyboard_poll_loop();
}
