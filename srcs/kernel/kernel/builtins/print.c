#include <builtins.h>

int print_stack(size_t size) {
	uint32_t esp;
	asm volatile("movl %%esp, %0" : "=r"(esp));
	printk("Kernel Stack Dump (ESP=%p)\n", (void *)esp);
	for (size_t offset = 0; offset < size; offset += 16) {
		uint32_t base = esp + offset;
		for (int j = 0; j < 4; ++j) {
			uint32_t *addr = (uint32_t *)(base + j * sizeof(uint32_t));
			if (j == 0)
				printk("\n%p: ", (void *)addr);
			printk("%p ", (void *)*addr);
		}
		for (int j = 0; j < 4; ++j) {
			uint8_t byte = *(uint8_t *)(base + j * sizeof(uint32_t));
			char c = (byte >= 0x20 && byte < 0x7F) ? byte : '.';
			printf("%c ", c);
			if (j == 3)
				printf("\n");
		}
	}
}

void print_42(void) {
	printf("                                 :::     :::::::: \n");
	printf("                               :+:     :+:    :+: \n");
	printf("                             +:+ +:+        +:+   \n");
	printf("                           +#+  +:+      +#+      \n");
	printf("                         +#+#+#+#+#+  +#+         \n");
	printf("                              #+#   #+#           \n");
	printf("                             ###  ##########      \n");
}
