#ifndef _IDT_H
#define _IDT_H

#include <stdint.h>

extern void idt_ptr();
extern void idt_load();

void idt_init();

void setvect(uint8_t vector, void (*handler)(void));

#endif
