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

		if (*format == 'p') {
			format++;
			if (*format == 's') {
				format++;
				// TODO: Implement kallsyms_lookup logic
			} else {
				unsigned long addr = va_arg(parameters, unsigned long);
				if (!maxrem) {
					// TODO: Set errno to EOVERFLOW.
					return -1;
				}
				if (addr == 0) {
					if (maxrem < 5) {
						// TODO: Set errno to EOVERFLOW.
						return -1;
					}
					if (!print("(nil)", 5))
						return -1;
					written += 5;
					continue;
				}
				char buf[64];
				size_t len = sizeof(buf);
				buf[--len] = '\0';
				while (addr != 0 && len > 0) {
					int i = addr & 0xF;
					if (i < 10)
						buf[--len] = '0' + i;
					else
						buf[--len] = 'a' + i - 10;
					addr >>= 4;
				}
				char *hexstr = &buf[len];
				len = strlen(hexstr) + strlen("0x");
				if (maxrem < len)
					return -1;
				if (!print("0x", strlen("0x")) || !print(hexstr, strlen(hexstr)))
					return -1;
				written += len;
			}
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
