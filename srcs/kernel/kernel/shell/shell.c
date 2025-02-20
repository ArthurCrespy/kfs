#include <shell.h>

char _shell_buffer[4096];
int _shell_buffer_index = 0;

void shell_buffer_add(char c) {
	_shell_buffer[_shell_buffer_index++] = c;
}

void shell_buffer_remove(int i) {
	while (i-- > 0 && _shell_buffer_index > 0)
		_shell_buffer[--_shell_buffer_index] = '\0';
}

void shell_buffer_reset(void) {
	while (_shell_buffer_index > 0)
		_shell_buffer[_shell_buffer_index--] = '\0';
}

void shell_buffer_init(void) {
	for (int i = 0; i < 256; i++)
		_shell_buffer[i] = '\0';
}

void shell_exec(void) {
	printf("\nexec: %s\n", _shell_buffer);
	shell_buffer_reset();
}

void shell(void) {
	printf("kfs> ");
}