#include <kernel.h>

void kernel_main(void) 
{
	terminal_init();
	pic_init();
	gdt_init();
	idt_init();
	interrupts_init();
	keyboard_init();

	terminal_setcolor(VGA_COLOR_LIGHT_BLUE);
	print_42();

	terminal_setcolor(VGA_COLOR_GREEN);
	printf("\n\nHello world, 42 kernel!\n\n");

	terminal_setcolor(VGA_COLOR_WHITE);
	print_stack(128);

	for(;;)
		asm("hlt");
}
