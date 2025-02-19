#include <pic.h>

void pic_remap(void) {
	outb(PIC1_COMMAND, 0x11);
	outb(PIC2_COMMAND, 0x11);

	outb(PIC1_DATA, 0x20);
	outb(PIC2_DATA, 0x28);

	outb(PIC1_DATA, 0x00);
	outb(PIC2_DATA, 0x00);

	outb(PIC1_DATA, 0xff);
	outb(PIC2_DATA, 0xff);
}

void pic_init(void) {
	pic_remap();
}
