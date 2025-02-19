#ifndef _PORTS_H
#define _PORTS_H

#include <stdint.h>

extern uint8_t inb(uint16_t port);
extern uint16_t inw(uint16_t port);
extern void outb(uint16_t port, uint8_t data);
extern void outw(uint16_t port, uint16_t data);

#endif
