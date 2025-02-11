#ifndef _GDT_H
#define _GDT_H

#include <stdint.h>

extern void gdt_ptr(void);
extern void gdt_load(void);

void gdt_init(void);

void setentry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity);

#endif
