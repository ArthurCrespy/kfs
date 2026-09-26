#include <stdio.h>
#include <builtins.h>

void print_stack(void) {
	uintptr_t esp;
	extern char stack_top[];
	asm volatile("movl %%esp, %0" : "=r"(esp));

	printk("Kernel Stack Dump (%p to %p):\n", (void *)esp, (void *)stack_top);

	size_t size = (uintptr_t)stack_top - esp;
	for (size_t offset = 0; offset < size; offset += 16) {
		uintptr_t base = esp + offset;
		size_t line = size - offset < 16 ? size - offset : 16;
		printk("%p: ", (void *)base);
		for (size_t j = 0; j + 4 <= line; j += 4) {
			printk("%p ", *(const uint32_t *)(base + j));
		}
		for (size_t j = 0; j < line; ++j) {
			uint8_t byte = *(uint8_t *)(base + j);
			printf("%c", byte >= 0x20 && byte < 0x7F ? (char) byte : '.'); // TODO: change to printk()
		}
		printk("\n");
	}
}


void print_42(void) {
	printk("\n");

	printk("                                 :::     :::::::: \n");
	printk("                               :+:     :+:    :+: \n");
	printk("                             +:+ +:+        +:+   \n");
	printk("                           +#+  +:+      +#+      \n");
	printk("                         +#+#+#+#+#+  +#+         \n");
	printk("                              #+#   #+#           \n");
	printk("                             ###  ##########      \n");
}
