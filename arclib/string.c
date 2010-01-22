/*
 * Copyright 1999, 2001 Silicon Graphics, Inc.
 * Copyright 2001 Ralf Baechle
 *           2001 Guido Guenther <agx@sgixcpu.org>
 *           2008 Florian Lohoff <flo@rfc822.org>
 */
#include "string.h"
#include "stdlib.h"

char *strcat(char *d, char *s) {
	char	*a;

	for(a=d;*a;a++);
	for(;*s;*a++=*s++);
	*a=0x0;

	return d;
}

size_t strlen(const char *s)
{
	size_t len = 0;

	while (*(s++) != '\0')
		len += 1;

	return len;
}


int strncmp(const char *s1, const char *s2, size_t n)
{
	while (n-- > 0) {
		if (*s1 < *s2)
			return -1;
		else if (*s1 > *s2)
			return 1;
		else if (*s1 == '\0')
			return 0;
		s1 += 1;
		s2 += 1;
	}

	return 0;
}

/* from lib/string.c in the kernel sources */
int strcmp(const char * cs,const char * ct)
{
	register signed char __res;

	while (1) {
		if ((__res = *cs - *ct++) != 0 || !*cs++)
			break;
	}

	return __res;
}

char *strchr(const char *s, int c)
{
	while ((*s != (char) c) && (*s != '\0'))
		s += 1;
	return (char *) ((*s == '\0') ? NULL : s);
}


char *strcpy(char *s1, const char *s2)
{
	char *s = s1;

	do {
		*(s1++) = *(s2);
	} while(*s2++ != 0x0);

	return s;
}


char *strncpy(char *s1, const char *s2, size_t n)
{
	char *s = s1;

	while (n-- > 0) {
		if ((*(s1++) = *(s2++)) == '\0')
			break;
	}

	return s;
}


char *strdup(const char *s1)
{
	char *str = malloc(strlen(s1) + 1);

	if (str != NULL)
		strcpy(str, s1);
	return str;
}

int memcmp(const void * cs,const void * ct,size_t count)
{
	const unsigned char *su1, *su2;
	signed char res = 0;

	for (su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
		if ((res = *su1 - *su2) != 0)
			break;
	return res;
}

void *memcpy(void *s1, const void *s2, size_t n)
{
	char *c1 = (char *) s1;
	const char *c2 = (const char *) s2;

	while (n-- > 0)
		*(c1++) = *(c2++);
	return s1;
}

void *memmove(void *s1, const void *s2, size_t n)
{
	char *c1 = (char *) s1;
	const char *c2 = (const char *) s2;

	if (s1 < s2)
		return memcpy(s1, s2, n);

	c1+=n;
	c2+=n;

	while (n-- > 0)
		*(--c1) = *(--c2);
	return s1;
}

void *memset(void *s, int c, size_t n)
{
	char *mem = (char *) s;

	while (n-- > 0)
		*(mem++) = (char) c;
	return s;
}

void __bzero(char *p, int len)
{
	memset(p, 0, len);
}

char *strcat(char *dest, const char *src)
{
	char *tmp = dest;

	while (*dest)
		dest++;
	while ((*dest++ = *src++) != '\0')
		;
	return tmp;
}

void *memmove(void *dest, const void *src, size_t count)
{
	char *tmp;
	const char *s;

	if (dest <= src) {
		tmp = dest;
		s = src;
		while (count--)
			*tmp++ = *s++;
	} else {
		tmp = dest;
		tmp += count;
		s = src;
		s += count;
		while (count--)
			*--tmp = *--s;
	}
	return dest;
}

