/*
 * Copyright 1999 Silicon Graphics, Inc.
 */
#include "arc.h"
#include "string.h"

#include "stdio.h"

#include <stdarg.h>

static FILE arc_stdin = ARC_STDIN;
FILE *stdin = &arc_stdin;

static FILE arc_stdout = ARC_STDOUT;
FILE *stdout = &arc_stdout;


int fputs(const char *s, FILE * stream)
{
	LONG status;
	ULONG count;

	if (strlen(s) > 0) {
		status = ArcWrite(*stream, (char *) s, strlen(s), &count);
		if ((status != ESUCCESS) || (count != strlen(s)))
			return EOF;
	}
	return 0;
}


int puts(const char *s)
{
	int status = fputs(s, stdout);

	if (status != EOF)
		status = fputs("\n\r", stdout);
	return status;
}


int fgetc(FILE * stream)
{
	LONG status;
	CHAR ch;
	ULONG count;

	status = ArcRead(*stream, &ch, sizeof(CHAR), &count);
	if ((status != ESUCCESS) || (count != sizeof(CHAR)))
		return EOF;
	return (int) ch;
}


static const char *numtostr(unsigned int num, unsigned int base)
{
	static char str[33];
	static char digits[] = {
		'0', '1', '2', '3', '4', '5', '6', '7',
		'8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
	};
	int pos = 32;

	str[pos] = '\0';
	do {
		str[--pos] = digits[num % base];
		num /= base;
	} while (num > 0);

	return &str[pos];
}


int vfprintf(FILE * stream, const char *format, va_list ap)
{
	int count = 0;
	const char *str;
	unsigned int uint;
	char tmp[2];

	if (format != NULL) {
		while (*format != '\0') {
			str = strchr(format, '%');
			if (str == NULL) {
				count += strlen(format);
				fputs(format, stream);
				break;
			} else {
				if (format < str) {
					LONG status;
					ULONG count;
					ULONG len = str - format;

					status =
					    ArcWrite(*stream,
						     (char *) format, len,
						     &count);
					if ((status != ESUCCESS)
					    || (count != len))
						return EOF;
					count += len;
				}

				format = str + 1;
				if(*format == 'l')
					format = str + 2;

				switch (*format) {
				case 'u':
					uint = va_arg(ap, unsigned int);
					str = numtostr(uint, 10);
					if (fputs(str, stream) == EOF)
						return EOF;
					count += strlen(str);
					break;

				case 'p':
				case 'x':
					uint = va_arg(ap, unsigned int);
					str = numtostr(uint, 16);
					if (fputs(str, stream) == EOF)
						return EOF;
					count += strlen(str);
					break;

				case 's':
					str = va_arg(ap, const char *);
					if( !str )
						str = "(NULL)";
					if (fputs(str, stream) == EOF)
						return EOF;
					count += strlen(str);
					break;

				case 'c':
					tmp[0] = (char)va_arg(ap, int);
					tmp[1] = '\0';
					if (fputs(tmp, stream) == EOF)
						return EOF;
					count++;
					break;

				case '%':
					if (fputs("%", stream) == EOF)
						return EOF;
					count += 1;
					break;

				case '\0': /* format error */
				default:
					return EOF;
				}

				format += 1;
			}
		}
	}

	return count;
}


int vprintf(const char *format, va_list ap)
{
	return vfprintf(stdout, format, ap);
}


int fprintf(FILE * stream, const char *format, ...)
{
	va_list ap;
	int result;

	va_start(ap, format);
	result = vfprintf(stream, format, ap);
	va_end(ap);

	return result;
}


int printf(const char *format, ...)
{
	va_list ap;
	int result;

	va_start(ap, format);
	result = vfprintf(stdout, format, ap);
	va_end(ap);

	return result;
}

int vsprintf(char* string, const char *format, va_list ap)
{
	int count = 0;
	const char *str;
	unsigned int uint;
	char tmp[2];

	if (format != NULL) {
		while (*format != '\0') {
			str = strchr(format, '%');
			if (str == NULL) {
				strcpy(&string[count], format);
				count += strlen(format);
				break;
			} else {
				if (format < str) {
					ULONG len = str - format;

					strncpy(&string[count], 
						(char *) format, len);
					count += len;
				}

				format = str + 1;
				if(*format == 'l')
					format = str + 2;

				switch (*format) {
				case 'u':
					uint = va_arg(ap, unsigned int);
					str = numtostr(uint, 10);
					strcpy(&string[count], str);
					count += strlen(str);
					break;

				case 'p':
				case 'x':
					uint = va_arg(ap, unsigned int);
					str = numtostr(uint, 16);
					strcpy(&string[count], str);
					count += strlen(str);
					break;

				case 's':
					str = va_arg(ap, const char *);
					if( !str )
						str = "(NULL)";
					strcpy(&string[count], str);
					count += strlen(str);
					break;

				case 'c':
					tmp[0] = (char)va_arg(ap, int);
					tmp[1] = '\0';
					strcpy(&string[count], tmp);
					count++;
					break;

				case '%':
					strcpy(&string[count], "%");
					count++;
					break;

				case '\0': /* format error */
				default:
					return EOF;
				}

				format += 1;
			}
		}
	}
	string[count]='\0';
	return count;
}


int sprintf(char* str, const char *format, ...)
{
	va_list ap;
	int result;

	va_start(ap, format);
	result = vsprintf(str, format, ap);
	va_end(ap);

	return result;
}
