#include <stdio.h>

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
		size_t maxrem = (size_t) (INT_MAX - written);

		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t pos = 1;
			while (format[pos] && format[pos] != '%')
				pos++;
			if (maxrem < pos) {
				// TODO: Set errno to EOVERFLOW.
				written = -1;
				break;
			}
			if (!print(format, pos)) {
				written = -1;
				break;
			}
			format += pos;
			written += (int)pos;
			continue;
		}

		const char* format_begun_at = format++;

		if (*format == 'c') {
			char c = (char) va_arg(parameters, int);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				written = -1;
				break;
			}
			if (!print(&c, sizeof(c))) {
				written = -1;
				break;
			}
			written++;
			format++;
		} else if (*format == 'd' || *format == 'i' || *format == 'x' || *format == 'X') {
			int num = va_arg(parameters, int);
			char num_buffer[32];
			size_t pos = 0;
			unsigned int unum;

			if (num == 0) {
				if (!maxrem) {
					// TODO: Set errno to EOVERFLOW.
					written = -1;
					break;
				}
				if (!print("0", 1)) {
					written = -1;
					break;
				}
				written++;
			}

			if (num < 0 && (*format == 'd' || *format == 'i')) {
				if (!maxrem) {
					// TODO: Set errno to EOVERFLOW.
					written = -1;
					break;
				}
				if (!print("-", 1)) {
					written = -1;
					break;
				}
				written++;
				maxrem--;
				unum = (unsigned int)-(long long)num;
			} else {
				unum = (unsigned int)num;
			}

			while (unum != 0 && (*format == 'd' || *format == 'i')) {
				num_buffer[pos++] = (char) ('0' + unum % 10);
				unum /= 10;
			}

			while (unum != 0 && (*format == 'x' || *format == 'X')) {
				size_t i = unum % 16;
				if (i < 10)
					num_buffer[pos++] = (char) ('0' + i);
				else if (*format == 'x')
					num_buffer[pos++] = (char) ('a' + i - 10);
				else if (*format == 'X')
					num_buffer[pos++] = (char) ('A' + i - 10);
				unum /= 16;
			}

			for (size_t i = 0; i < pos / 2; i++) {
				char tmp = num_buffer[i];
				num_buffer[i] = num_buffer[pos - i - 1];
				num_buffer[pos - i - 1] = tmp;
			}

			if (maxrem < pos) {
				// TODO: Set errno to EOVERFLOW.
				written = -1;
				break;
			}
			if (!print(num_buffer, pos)) {
				written = -1;
				break;
			}
			written += (int)pos;
			format++;
		} else if (*format == 's') {
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);

			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				written = -1;
				break;
			}
			if (!print(str, len)) {
				written = -1;
				break;
			}
			written += (int)len;
			format++;
		} else {
			format = format_begun_at;
			size_t len = strlen(format);

			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				written = -1;
				break;
			}
			if (!print(format, len)) {
				written = -1;
				break;
			}
			written += (int)len;
			format += len;
		}
	}

	va_end(parameters);
	return written;
}
