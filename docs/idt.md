# IDT

The IDT is a table used by the processor to handle interrupts and exceptions. Each entry in the IDT corresponds to an interrupt vector, and each entry contains the address of the handler function ([ISR - Interrupt Service Routine](isr.md)) to be called when that interrupt occurs. This handler is a function that will execute when a specific interrupt or exception is raised.

There are generally 3 classes of interrupts on most platforms :

- Exceptions are generated internally by the CPU and used to alert the running kernel of an event or situation which requires its attention. On x86 CPUsm these include exception conditions such as Double Fault, Page Fault, etc.

- [Interrupts Requests (IRQs)](irq.md) are generated externally by the chipset to notify the CPU about events that require its attention.

- Software Interrupts are signaled by software running on a CPU to indicate that it needs the kernel's attention. These types of interrupts are generally used for System Calls.

In x86 architecture, the IDT can handle up to 256 interrupts (from 0x00 to 0xFF), and each interrupt has its own entry. The IDT is similar to the [GDT](gdt.md), but the entries are for interrupt handlers rather than segments.

Each entry in the IDT corresponds to an interrupt vector, which is identified by an [IRQ number](irq.md). When the CPU receives an interrupt (through an IRQ signal), it uses the vector number to find the corresponding ISR.

*Exceptions are mapped to specific entries in the IDT, just like IRQs, but they represent exceptional conditions, not routine hardware events. Software interrupts are also handled in the IDT using ISR.*

## Purpose of the IDT

An IDT entry is an 8-byte (64-bit) structure that stores information about the interrupt handler function, such as its address and various attributes.

| 63-48       | 47-40     | 39-32 | 31-16    | 15-0       |
| ----------- | --------- | ----- | -------- | ---------- |
| offset_high | type_attr | zero  | selector | offset_low |

| Bits  | Field       | Size    | Description |
| ----- | ----------- | ------- | ----------- |
| 63-48 | offset_high | 16 bits | Remaining upper 16 bits of the handler's 32-bit address |
| 47-40 | type_attr   | 8 bits  | Interrupt gate type and attributes |
| 39-32 | zero        | 8 bits  | Reserved, must be zero |
| 31-16 | selector    | 16 bits | Points to the kernel code segment in the GDT (typically 0x08). This is where the interrupt handler code is located |
| 15-0  | offset_low  | 16 bits | Lower 16 bits of the handler's 32-bit address |

`type_attr` (bits 47-40) :

| 47  | 46-45 | 44  |  43  | 42  | 41  | 40  |
| --- | ----- | --- | ---- | --- | --- | --- |
|  P  |  DPL  |  0  | Type |  0  | DPL |  0  |

| Bit   | Flag                             | Description |
| ----- | ---------------------------------| ----------- |
| 47    | P (Present)                      | 1 = Present (interrupt is active and enabled), 0 = Not Present (interrupt is disabled) |
| 46-45 | DPL (Descriptor Privilege Level) | Defines the privilege level (ring 0 to ring 3) |
| 44    | 0 (Always 0)                     | - |
| 43    | Type                             | Defines the interrupt gate type: 1 = Task Gate (rarely used),   0 = Interrupt Gate (interrupt handler) |
| 42    | 0 (Always 0)                     | - |
| 41    | DPL                              | This bit is typically combined with Type: 0 = interrupt is allowed only in ring 0, 1 = interrupt is allowed from ring 3 (ring 0 can also trigger it) |
| 40    | 0 (Always 0)                     | - |

### IDT entry structure

	struct idt_entry {
		uint16_t offset_low;      // Lower 16 bits of the handler address
		uint16_t selector;        // Kernel code segment selector
		uint8_t  zero;            // Must be zero
		uint8_t  type_attr;       // Type and attributes
		uint16_t offset_high;     // Upper 16 bits of the handler address
	} __attribute__((packed));

Defines an IDT entry.

### IDT pointer structure

	struct idt_ptr {
		uint16_t limit;  // Limit (size of IDT - 1)
		uint32_t base;   // Base address of the IDT
	} __attribute__((packed));

The IDT pointer structure manages the base address of the IDT and its size.

- `limit`: The size of the IDT minus 1. This value is typically 256 * 8 - 1 for a 256-entry IDT. This represents the total size of the IDT in bytes.
- `base`: The base address of the IDT in memory. This is where the IDT starts.

### IDT loading

	section .data
	global idt_ptr
	; IDT pointer structure
	idt_ptr:
		dw 256 * 8 - 1      ; Size of IDT (256 entries * 8 bytes) - 1
		dd idt              ; Base address of the IDT

`idt_ptr` stores the base address and size of the IDT.

	section .bss
	global idt
	; Reserve space for 256 IDT entries
	idt:
		resb 256 * 8        ; Reserve space for 256 entries (8 bytes per entry)

This is where the space for the 256 entries of the IDT is reserved, each entry taking 8 bytes.

	section .text
	global idt_load
	; Load the IDT using lidt
	idt_load:
		lidt [idt_ptr]      ; Load the IDT using the 'lidt' instruction
		ret

`idt_load` loads the IDT with `lidt` using the information in the `idt_ptr` structure. This allows the CPU to start using the IDT for interrupt handling. `lidt` is similar to [`lgdt`](gdt.md#gdt-entry-structure), and will load the IDT into the IDTR (IDT Register).

### Setting interrupt vector

	void setvect(uint8_t vector, void (*handler)(void)) {
		uint32_t handler_addr = (uint32_t) handler;

		idt[vector].offset_low    = handler_addr & 0xFFFF;
		idt[vector].selector      = 0x08;
		idt[vector].zero          = 0;
		idt[vector].type_attr     = 0x8E;
		idt[vector].offset_high   = (handler_addr >> 16) & 0xFFFF;
	}

`setvect` is used to set an interrupt vector in the IDT.

- `vector`: interrupt vector (0-255) to be set in the IDT
- `handler`: pointer to the interrupt handler function
- `handler_addr`: Address of the handler function, which is split into the low and high 16 bits
- `type_attr`: set type and attributes. *Here `0x8E` means the interrupt is present, ring 0 and it's an interrupt gate*

### IDT initialization

	void idt_init(void) {
		setvect(0x20, isr_wrapper);                // Set handler for IRQ 0 (Timer)
		setvect(0x21, isr_wrapper_keyboard);       // Set handler for IRQ 1 (Keyboard)
		setvect(0x31, isr_wrapper);                // Set handler for IRQ 49 (example)
		idt_load();                                // Load the IDT into the CPU
	}

Sets up interrupt handlers for different interrupt vectors and loads the IDT into the CPU using `idt_load`
