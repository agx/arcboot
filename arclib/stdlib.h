/*
 * Copyright 1999 Silicon Graphics, Inc.
 */
#ifndef _STDLIB_H_
#define _STDLIB_H_

#include "stddef.h"
#include "types.h"

extern void *malloc(size_t size);
extern void free(void *ptr);
extern void *realloc(void *ptr, size_t size);

extern void arclib_malloc_add(ULONG start, ULONG size);

#endif				/* _STDLIB_H_ */
