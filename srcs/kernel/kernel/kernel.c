#include <kernel.h>

void kernel_main(void) 
{
	pic_init();
	gdt_init();
	idt_init();
	interrupts_init();
	keyboard_init();
    terminal_init();

	terminal_setcolor(VGA_COLOR_LIGHT_BLUE);
	printf("                                 :::     :::::::: \n");
	printf("                               :+:     :+:    :+: \n");
	printf("                             +:+ +:+        +:+   \n");
	printf("                           +#+  +:+      +#+      \n");
	printf("                         +#+#+#+#+#+  +#+         \n");
	printf("                              #+#   #+#           \n");
	printf("                             ###  ##########      \n");

	terminal_setcolor(VGA_COLOR_LIGHT_RED);
	printf("\n\nHello world, 42 kernel!\n\n");

	terminal_setcolor(VGA_COLOR_WHITE);


	for(;;)
		asm("hlt");
}
