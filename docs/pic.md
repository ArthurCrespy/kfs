# PIC

The Programmable Interrupt Controller (PIC) is a hardware component that manages interrupts and ensures that the CPU handles them in an organized manner. The primary purpose of the PIC is to prioritize and control [interrupt requests (IRQs)](irq.md) from various hardware devices, such as keyboards, mice, timers and other peripherals.

In x86 systems, there are typically 2 PICs: PIC1 (primary) and PIC2 (secondary).

There two controllers work together to manage up to 15 IRQs. PIC1 handles IRQs 0-7 while PIC2 handles IRQs 8-15. The two PICS are cascaded, meaning that PIC2 is connected to PIC1, allowing them to extend the interrupt range.

## How the PIC works

When an interrupt occurs, the PIC signals the CPU, causing an interrupt request. The PICs prioritize these interrupts based on their IRQ number. If an interrupt occurs on PIC2, PIC1 will also be notified to that the CPU knows to process the interrupt from PIC2. The PIC will also handle End of Interrupt (EOI) signals.

The remapping process of the PIC allows the redefinition of the interrupt vector numbers in order to customize interrupt handling for different hardware events.

---

	#define PIC_READ_IRR	0x0a
	#define PIC_READ_ISR	0x0b
	#define PIC_EOI			0x20

These are constants that represent specific commands for interacting with the PIC:

- `PIC_READ_IRR`: Command to read the Interrupt Request Register (IRR)
- `PIC_READ_ISR`: Command to read the In-Service Register (ISR)
- `PIC_EOI`: Command for sending the End of Interrupt (EOI) signal to the PIC.

```
#define PIC1			0x20
#define PIC2			0xA0
#define PIC1_COMMAND	PIC1
#define PIC1_DATA		(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA		(PIC2+1)
```

These define the I/O ports for PIC1 and PIC2.

	#define ICW1_ICW4		0x01
	#define ICW1_SINGLE		0x02
	#define ICW1_INTERVAL4	0x04
	#define ICW1_LEVEL		0x08
	#define ICW1_INIT		0x10

	#define ICW4_8086		0x01
	#define ICW4_AUTO		0x02
	#define ICW4_BUF_SLAVE	0x08
	#define ICW4_BUF_MASTER	0x0C
	#define ICW4_SFNM		0x10

These are Initialization Command Words (ICW) values that are used to configure the PIC. They define different settings for initialization:

- ICW1 and ICW4 are used during the remapping process to initialize the PIC with the correct settings
- ICW1_INIT initiates the initialization process
- ICW4_8086 configures the PIC for compatibility with the x86 architecture

```
void pic_send_eoi(uint8_t irq) {
	if (irq >= 8)
		outb(PIC2_COMMAND, PIC_EOI);
	outb(PIC1_COMMAND, PIC_EOI);
}
```

This functions sends an EOI signal to the PIC, which tells it that the CPU has finished processing the interrupt and is ready for the next interrupt.

If the interrupt number is >= 8, it sends the EOI signal to PIC2 as IRQs 8-15 are managed by it.

	uint16_t pic_get_irq_reg(int ocw3) {
		outb(PIC1_COMMAND, ocw3);
		outb(PIC2_COMMAND, ocw3);
		return inb(PIC2_COMMAND) << 8 | inb(PIC1_COMMAND);
	}

*TODO*

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

This function remaps the PICs to a new interrupt vector range, which is necessary because the default mapping interferes with system interrupt vectors. The command sequence ensures that the PICs are set to work in 8086 mod and are configured for correct operation with cascading.