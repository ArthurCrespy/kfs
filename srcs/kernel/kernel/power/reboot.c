#include <reboot.h>

void reboot(void)
{
	__asm__ __volatile__("cli");
	keyboard_reset_system();
	while (1)
		__asm__ __volatile__("hlt");
}
