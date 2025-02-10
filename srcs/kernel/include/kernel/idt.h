#ifndef _IDT_H
#define _IDT_H

#include <stdint.h>

void setvect(uint8_t vector, void (*handler)(void));

#endif
