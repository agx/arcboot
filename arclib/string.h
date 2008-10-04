/*
 * Copyright 1999 Silicon Graphics, Inc.
 *           2001 Guido Guenther <agx@sigxcpu.org>
 *           2008 Florian Lohoff <flo@rfc822.org>
 */
#ifndef _STRING_H_
#define _STRING_H_

#include "stddef.h"

extern size_t strlen(const char *s);
extern int strcmp(const char *s1,const char *s2);
extern char *strcat(char *d, char *s);
extern int strncmp(const char *s1, const char *s2, size_t n);
extern char *strchr(const char *s, int c);
extern char *strcpy(char *s1, const char *s2);
extern char *strncpy(char *s1, const char *s2, size_t n);
extern char *strdup(const char *s1);

extern void *memcpy(void *s1, const void *s2, size_t n);
extern void *memmove(void *s1, const void *s2, size_t n);
extern void *memset(void *s, int c, size_t n);
int memcmp(const void * cs,const void * ct,size_t count);
extern void __bzero(char *p, int len);

#endif				/* _STRING_H_ */
