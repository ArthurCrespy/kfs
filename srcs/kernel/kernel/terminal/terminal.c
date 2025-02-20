#include <terminal.h>

static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

int current_screen = 0;
size_t terminal_row[NUM_SCREEN];
size_t terminal_column[NUM_SCREEN];
uint8_t terminal_color[NUM_SCREEN];
static uint16_t terminal_buffer[NUM_SCREEN][VGA_WIDTH * VGA_HEIGHT];

void terminal_update_cursor(void) {
	uint16_t pos = terminal_row[current_screen] * VGA_WIDTH + terminal_column[current_screen];

	outb(0x3D4, 14);
	outb(0x3D5, pos >> 8);

	outb(0x3D4, 15);
	outb(0x3D5, pos & 0xFF);
}

void terminal_setcolor(uint8_t color) {
	terminal_color[current_screen] = color;
}

uint8_t terminal_getcolor(void) {
	return terminal_color[current_screen];
}

void terminal_sync_with_vga(void) {
	for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
		VGA_MEMORY[i] = terminal_buffer[current_screen][i];
	VGA_MEMORY[VGA_WIDTH - 1] = vga_entry('0' + current_screen, vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK));
}

void terminal_load_screen(int new_screen) {
	if (new_screen < 0 || new_screen >= NUM_SCREEN)
          return ;

	current_screen = new_screen;

	terminal_update_cursor();
	terminal_sync_with_vga();
}

void terminal_delete_last_line(void) {
	for (size_t x = 0; x < VGA_WIDTH; x++) {
		const size_t index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
		terminal_buffer[current_screen][index] = vga_entry(' ', terminal_color[current_screen]);
	}
}

void terminal_scroll(void) {
	uint16_t *vga = terminal_buffer[current_screen];

	for (size_t y = 0; y < VGA_HEIGHT - 1; y++)
		for (size_t x = 0; x < VGA_WIDTH; x++)
			vga[y * VGA_WIDTH + x] = vga[(y + 1) * VGA_WIDTH + x];
	terminal_delete_last_line();
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[current_screen][index] = vga_entry(c, color);
}

void terminal_putchar(char c) {
	if (c == '\n') {
		terminal_column[current_screen] = 0;
		++terminal_row[current_screen];
        uint8_t color = terminal_getcolor();
        terminal_setcolor(VGA_COLOR_LIGHT_GREEN);
        terminal_writestring("kfs> ");
        terminal_setcolor(color);
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
	terminal_sync_with_vga();
}

void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}

void terminal_init(void) {
	for (int screen = 0; screen < NUM_SCREEN; screen++) {
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
    terminal_setcolor(VGA_COLOR_LIGHT_GREEN);
    terminal_writestring("kfs> ");
    terminal_setcolor(VGA_COLOR_WHITE);
}
