#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == EOF)
			return false;
	return true;
}

int printf(const char* restrict format, ...) {
	va_list parameters;
	va_start(parameters, format);

	int written = 0;

	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;

		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format++;

		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(&c, sizeof(c)))
				return -1;
			written++;
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
		} else if (*format == 'd' || *format == 'i') {
			format++;
			int num = va_arg(parameters, int);
			char num_buffer[32];
			int pos = 0;
			unsigned int unum;

			if (num < 0) {
				if (!maxrem) {
					// TODO: Set errno to EOVERFLOW.
					return -1;
				}
				if (!print("-", 1))
					return -1;
				written++;
				maxrem--;
				unum = (unsigned int)(-(long long)num);
			} else {
				unum = (unsigned int)num;
			}

			do {
				num_buffer[pos++] = '0' + (unum % 10);
				unum /= 10;
			} while (unum != 0);

			for (int i = 0; i < pos / 2; i++) {
				char tmp = num_buffer[i];
				num_buffer[i] = num_buffer[pos - i - 1];
				num_buffer[pos - i - 1] = tmp;
			}

			if (maxrem < (size_t)pos) {
				// TODO: Set errno to EOVERFLOW.
				va_end(parameters);
				return -1;
			}
			if (!print(num_buffer, pos))
				return -1;
			written += pos;
		} else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, len))
				return -1;
			written += len;
			format += len;
		}
	}

	va_end(parameters);
	return written;
}
