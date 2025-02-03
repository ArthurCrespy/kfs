    enum vga_color {
    	VGA_COLOR_BLACK = 0,
    	VGA_COLOR_BLUE = 1,
    	VGA_COLOR_GREEN = 2,
    	VGA_COLOR_CYAN = 3,
    	VGA_COLOR_RED = 4,
    	VGA_COLOR_MAGENTA = 5,
    	VGA_COLOR_BROWN = 6,
    	VGA_COLOR_LIGHT_GREY = 7,
    	VGA_COLOR_DARK_GREY = 8,
    	VGA_COLOR_LIGHT_BLUE = 9,
    	VGA_COLOR_LIGHT_GREEN = 10,
    	VGA_COLOR_LIGHT_CYAN = 11,
    	VGA_COLOR_LIGHT_RED = 12,
    	VGA_COLOR_LIGHT_MAGENTA = 13,
    	VGA_COLOR_LIGHT_BROWN = 14,
    	VGA_COLOR_WHITE = 15,
    };
Color constants for the VGA text mode. VGA stores foreground (text) and background (screen) colors as 4-bits values (0-15).

    static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
    {
    	return fg | bg << 4;
    }
This function combines foreground and background colors into a single byte.

    static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
    {
    	return (uint16_t) uc | (uint16_t) color << 8;
    }

VGA memory stores characters and colors in a 16-bit value:
Lower 8 bits (uc) store the ASCII character,
upper 8 bits (color) store the color.

    size_t strlen(const char* str) 
    {
    	size_t len = 0;
    	while (str[len])
    		len++;
    	return len;
    }
Basic implementation of strlen(). *No standard library available.*

    static const size_t VGA_WIDTH = 80;
    static const size_t VGA_HEIGHT = 25;

    size_t terminal_row;
    size_t terminal_column;
    uint8_t terminal_color;
    uint16_t* terminal_buffer;
Text resolution, storing cursor position, and current text color. `terminal_buffer` points to the VGA memory where the text is stored.

    void terminal_initialize(void) 
    {
    	terminal_row = 0;
    	terminal_column = 0;
    	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    	terminal_buffer = (uint16_t*) 0xB8000;
    	for (size_t y = 0; y < VGA_HEIGHT; y++) {
    		for (size_t x = 0; x < VGA_WIDTH; x++) {
    			const size_t index = y * VGA_WIDTH + x;
    			terminal_buffer[index] = vga_entry(' ', terminal_color);
    		}
    	}
    }
Resets the screen by setting every character to `' '` (space) with a default color. The screen is cleared by looping over all cells in the VGA buffer.
`terminal_buffer` is assigned to 0xB8000, which is the start of VGA text memory.

*0xB8000 is fixed by IBM's VGA standard for text mode video memory. It's a memory-mapped I/O region for direct screen manipulation. Writing here immediately updates the screen without needing BIOS or OS call.*

    void terminal_setcolor(uint8_t color) 
    {
    	terminal_color = color;
    }
Pretty straightforward

    void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
    {
    	const size_t index = y * VGA_WIDTH + x;
    	terminal_buffer[index] = vga_entry(c, color);
    }
Computes the index in the VGA buffer *`(y * VGA_WIDTH + x)`* and writes the character `c` of color `color` to VGA memory at this location.

    void terminal_putchar(char c) 
    {
    	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    	if (++terminal_column == VGA_WIDTH) {
    		terminal_column = 0;
    		if (++terminal_row == VGA_HEIGHT)
    			terminal_row = 0;
    	}
    }
Prints a character at the current cursor position. If the column reaches the screen width, it moves to the next line. If the row reaches the screen height, it loops back to the top. *No scrolling support for now.*

    void terminal_write(const char* data, size_t size) 
    {
    	for (size_t i = 0; i < size; i++)
    		terminal_putchar(data[i]);
    }
Prints a raw array of characters.

    void terminal_writestring(const char* data) 
    {
    	terminal_write(data, strlen(data));
    }
Prints the string `data`.

    void kernel_main(void) 
    {
    	terminal_initialize();

    	terminal_writestring("Hello, kernel World!\n");
    }

*The VGA text mode is deprecated on newer machines, and UEFI only supports pixel buffers. This will have to be changed later on for forward-compatibility.*

- *Add scroll and cursor support*
- *Add newline support*