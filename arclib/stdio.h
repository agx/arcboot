/*
 * Copyright 1999 Silicon Graphics, Inc.
 */
#ifndef _STDIO_H_
#define _STDIO_H_

#include "types.h"
#include <stdarg.h>

typedef ULONG FILE;

#define EOF	(-1)

extern FILE *stdin;
extern FILE *stdout;

extern int fputs(const char *s, FILE * stream);
extern int puts(const char *s);

extern int fgetc(FILE * stream);
#define getc(stream)	fgetc(stream)
#define getchar()	getc(stdin)

extern int printf(const char *format, ...);
extern int fprintf(FILE * stream, const char *format, ...);
extern int sprintf(char* string, const char* format, ...);
extern int vprintf(const char *format, va_list ap);
extern int vfprintf(FILE * stream, const char *format, va_list ap);
extern int vsprtinf(char* string, const char* format, va_list ap);

#endif				/* _STDIO_H_ */
