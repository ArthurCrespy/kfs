# GDT

The Global Descriptor Table (GDT) is a key data structure in protected mode (used in modern x86 systems). It defines how memory segmentation works.

## Segmentation

In real mode (the mode x86 CPUs start in), memory is accessed using segments, which are 64KB blocks referenced by segment registers (CS, DS, SS, etc.).

However, in protected mode, segmentation is more powerful:

- Segments can have any size
- Each segment has permissions (executable, readable, writeable, etc.)
- Each segment can be restricted to a privilege level

## Purpose of the GDT

The GDT defines segments. Each segment has:

- A 32-bit base address (where it starts in memory)
- A 20-bit segment limit (how large it is)
- An 8-bit access byte (describe the segment type and privilege level)
- 4 bits of flags (granularity, operant size, etc.)

Here's a typical segment layout: 

| 63-56 | 55-48 | 47-40 | 39-32 | 31-16 | 15-0 |
| ----- | ----- | ----- | ----- | ----- | ---- |
| Base High | Flags & Limit High | Access Byte | Base Middle | Base Low | Limit Low |

| Bits  | Field              | Size    | Description |
| ----- | ------------------ | --------| ----------- |
| 63-56 | Base High          | 8 bits  | Highest 8 bits of the base address |
| 55-48 | Flags & Limit High | 8 bits  | 4 bits for flags and high 4 bits of limit |
| 47-40 | Access Byte        | 8 bits  | Defines segment access properties |
| 39-32 | Base Middle        | 8 bits  | Next 8 bits of the base address |
| 31-16 | Base Low           | 16 bits | Lower 16 bits of the base address |
| 15-0  | Limit Low          | 16 bits | Lower 16 bits of the limit. Defines the segment size |

### Access Byte

The access byte layout is defined as follows:

|   7   | 6 5 |  4  |  3  |  2  |  1  |  0  |
| ----- | --- | --- | --- | --- | --- | --- |
|   P   | DPL |  S  |  E  |  DC |  RW |  A  |

| Bit | Name                             | Description |
| --- | ---------------------------------| ----------- |
|  7  | P (Present)                      | Must be 1 for valid segments. 0 means the segment is not present (causes a fault) |
| 6-5 | DPS (Descriptor Privilege Level) | 00 = Ring 0 (kernel), 11 = Ring 3 (user mode) |
|  4  | S (Descriptor type)              | 1 = Code/Data segment, 0 = System segment (like TSS, LDT) |
|  3  | E (Executable)                   | 1 = Code segment, 0 = Data segment |
|  2  | DC (Direction / Conforming)      | Data segments: 1 = grows down, 0 = grows up. Code segments: 1 = conforming, 0 = non-conforming |
|  1  | RW (Readable / Writable)         | Code segment: 1 = readable, 0 = execute-only. Data segments: 1 = writable, 0 = read-only |
|  0  | A (Accessed Bit)                 | CPU sets this to 1 when the segment is accessed |

### Flag Byte

The flag byte layout is defined as follows:

|  7  |  6  |  5  |  4  | (3-0: High 4 bits of segment limit) |
| --- | --- | --- | --- | --- |
|  G  |  D  |  L  | AVL | Limit (High 4 bits) |

| Bit | Name                           | Description |
| --- | ------------------------------ | ----------- |
|  7  | G (Granularity)                | 1 = 4KB blocks, 0 = 1 byte granularity |
|  6  | D (Default Operand Size)       | 1 = 32-bit, 0 = 16-bit |
|  5  | L (Long Mode)                  | 1 = 64-bit, 0 = 32-bit (always 0 in i386) |
|  4  | AVL (Available for system use) | Unused in most cases |

---

### GDT entry structure

A GDT descriptor (or GDT entry) is an 8-byte (64-bit) structure that describes a segment. Each entry in the GDT represents one segment. *Even though i386 is a 32-bit architecture, each GDT entry is 64 bits. This is possible thanks to special hidden registers inside the processor called Segment Descriptor Caches (also known as hidden descriptor registers). In our case, we'll be using the `lgdt` instruction used to load the GDT into a special CPU register called GDTR (GDT Register). This is a special register that stores the base address and size of the GDT.*

 
	struct gdt_entry {
		uint16_t limit_low;   // Lower 16 bits of the segment limit
		uint16_t base_low;    // Lower 16 bits of the base address
		uint8_t  base_middle; // Next 8 bits of the base address
		uint8_t  access;      // Access flags determine ring and segment type
		uint8_t  flags;       // Flags and high 4 bits of limit
		uint8_t  base_high;   // Highest 8 bits of the base address
	} __attribute__((packed));

Defines a GDT descriptor.

The base address (starting memory location) is split across:

- `base_low` (16 bits)
- `base_middle` (8 bits)
- `base_high` (8 bits)

The limit (size of the segment) is split into:

- `limit_low` (16 bits)
- `flags` (4 bits flags and high 4 bits of limit)

Access and flags:

- `access` defines privilege levels, present bit, executable bit, etc
- `flags` contains granularity and operand size (32-bit or 16-bit)

*`__attribute__((packed))` prevents compiler padding, ensuring that the structure is exactly 8 bytes. Otherwise the compiler adds padding to align data in memory for performance reasons.*

### GDT pointer structure

	struct gdt_ptr {
		uint16_t limit;    // Limit (size of GDT - 1)
		uint32_t base;     // Base address of the GDT
	} __attribute__((packed));

Holds the GDT's base address and size.

This structure is passed to the `lgdt` instruction.

### GDT pointer

	section .data
	global gdt_ptr
	gdt_ptr:
		dw 7 * 8 - 1   ; Size of GDT (7 entries, each 8 bytes) - 1
		dd gdt         ; Base address of the GDT

Defines gdt_ptr, which stores:

- Limit (`dw 7*8 -1`): Size of GDT in bytes (56 bytes - 1)
- Base address (`dd gdt`): Points to the gdt in memory

### GDT table (uninitialized)

	section .bss
	global gdt
	gdt:
		resb 7 * 8

Allocates 56 bytes (7 entries * 8 bytes) for the GDT

### GDT loading function

	section .text
	global gdt_load
	gdt_load:
		lgdt [gdt_ptr]      ; Load GDT descriptor (limit and base)
		jmp 0x08:.load      ; Far jump to refresh CS (Code Segment)
	.load:
		mov ax, 0x10        ; Load new Data Segment (0x10 = Kernel Data Segment)
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
		mov ax, 0x18        ; Load new Stack Segment (0x18 = Kernel Stack Segment)
		mov ss, ax
		ret

- `lgdt [gdt_ptr]` loads the GDT with the gdt_ptr structure into the CPU
- `jmp 0x08:.load`: far jump to load the new CS
- Sets segment registers (ds, es, fd, gs, ss)

### Setting a GDT entry

	void setentry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
		gdt[index].limit_low    = (uint16_t)(limit & 0xFFFF);
		gdt[index].base_low     = (uint16_t)(base & 0xFFFF);
		gdt[index].base_middle  = (uint8_t)((base >> 16) & 0xFF);
		gdt[index].access       = access;
		gdt[index].flags        = (uint8_t)(((limit >> 16) & 0x0F) | (flags & 0xF0));
		gdt[index].base_high    = (uint8_t)((base >> 24) & 0xFF);
	}

Splits the base and limit into different fields.

### Initializing the GDT

	void gdt_init(void) {
		setentry(0, 0, 0, 0, 0);    // Null segment
		setentry(1, 0, 0xFFFFF, 0x9A, 0xCF); // Kernel Code
		setentry(2, 0, 0xFFFFF, 0x92, 0xCF); // Kernel Data
		setentry(3, 0, 0xFFFFF, 0x96, 0xCF); // Kernel Stack
		setentry(4, 0, 0xFFFFF, 0xFA, 0xCF); // User Code
		setentry(5, 0, 0xFFFFF, 0xF2, 0xCF); // User Data
		setentry(6, 0, 0xFFFFF, 0xF6, 0xCF); // User Stack
		gdt_load();
	}

Initializes GDT with Kernel and User segments, and calls `gdt_load()` to activate the new GDT.