#ifndef _SHELL_H
#define _SHELL_H

#include <stdio.h>

void shell_buffer_add(char c);
void shell_buffer_remove(int i);
void shell_buffer_reset(void);
void shell_buffer_init(void);
void shell_exec(void);
void shell(void);

#endif