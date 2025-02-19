#include <pic.h>

void pic_send_eoi(uint8_t irq) {
	if (irq >= 8)
		outb(PIC2_COMMAND, PIC_EOI);
	outb(PIC1_COMMAND, PIC_EOI);
}

uint16_t pic_get_irq_reg(int ocw3) {
	outb(PIC1_COMMAND, ocw3);
	outb(PIC2_COMMAND, ocw3);
	return inb(PIC2_COMMAND) << 8 | inb(PIC1_COMMAND);
}

uint16_t pic_get_irr(void) {
	return pic_get_irq_reg(PIC_READ_IRR);
}

uint16_t pic_get_isr(void) {
	return pic_get_irq_reg(PIC_READ_ISR);
}

void pic_remap(int pic1_offset, int pic2_offset) {
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

	outb(PIC1_DATA, pic1_offset);
	outb(PIC2_DATA, pic2_offset);

	outb(PIC1_DATA, 0x04);
	outb(PIC2_DATA, 0x02);

	outb(PIC1_DATA, ICW4_8086);
	outb(PIC2_DATA, ICW4_8086);

	outb(PIC1_DATA, 0x00);
	outb(PIC2_DATA, 0x00);
}

void pic_disable(void) {
	outb(PIC1_DATA, 0xFF);
	outb(PIC2_DATA, 0xFF);
}

void pic_init(void) {
	pic_remap(0x20, 0x28);
	pic_disable();
}
