#ifndef _KERNEL_H
#define _KERNEL_H

#include <builtins.h>
#include <gdt.h>
#include <idt.h>
#include <irq.h>
#include <keyboard.h>
#include <pic.h>
#include <terminal.h>
#include <vga.h>

#include <stdio.h>

extern void interrupts_init(void);

void kernel_main(void);

#endif
