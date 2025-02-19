#ifndef _TESTS_H
#define _TESTS_H

#include <idt.h>
#include <ports.h>

#include <stdio.h>

extern void test_irq_49_asm();

void test_irq_49(void);
void test_irq_32(void);

#endif
