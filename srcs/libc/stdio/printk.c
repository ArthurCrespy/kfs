#include <stdio.h>

static bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == EOF)
			return false;
	return true;
}

int printk(const char* restrict format, ...) {
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

		if (*format == 'p') {
			unsigned long addr = va_arg(parameters, unsigned long); // TODO: change to uniptr_t when stdint.h
			format++;
			if (*format == 's') {
				format++;
				// TODO: Implement kallsyms_lookup logic
			} else {
				if (!maxrem) {
					// TODO: Set errno to EOVERFLOW.
					written = -1;
					break;
				}
				if (addr == 0) {
					if (maxrem < 10) {
						// TODO: Set errno to EOVERFLOW.
						written = -1;
						break;
					}
					if (!print("0x00000000", 10)) {
						written = -1;
						break;
					}
					written += 10;
					continue;
				}
				char buf[11];
				size_t len = sizeof(buf);
				buf[--len] = '\0';
				while (addr != 0 && len > 2) { // TODO: make this x86-64 proof
					int i = addr & 0xF;
					if (i < 10)
						buf[--len] = (char)('0' + i);
					else
						buf[--len] = (char)('a' + i - 10);
					addr >>= 4;
				}
				while (len > 2)
					buf[--len] = '0';
				buf[--len] = 'x';
				buf[--len] = '0';
				if (maxrem < strlen(buf)) {
					// TODO: Set errno to EOVERFLOW.
					written = -1;
					break;
				}
				if (!print(buf, strlen(buf))) {
					written = -1;
					break;
				}
				written += (int)strlen(buf);
			}
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
