/*
 * Copyright 1999 Silicon Graphics, Inc.
 */
#include "stdlib.h"
#include "string.h"
#include "arc.h"


typedef struct _Node {
	size_t size;
	struct _Node *next;
} Node;

static Node *freeList = NULL;


void *malloc(size_t size)
{
	Node **free, *mem;

	size +=
	    /* header */ sizeof(Node) + /* round up */ (sizeof(Node) - 1);
	size -= size % sizeof(Node);

	free = &freeList;
	while ((*free != NULL) && ((*free)->size < size))
		free = &((*free)->next);

	mem = *free;
	if (mem != NULL) {
		if (mem->size > size) {
			Node *split = mem + (size / sizeof(Node));

			split->size = mem->size - size;
			split->next = mem->next;
			mem->size = size;
			mem->next = split;
		}
		*free = mem->next;
		mem += 1;
	}

	return ((void *) mem);
}


void free(void *ptr)
{
	if (ptr != NULL) {
		Node *mem = ((Node *) ptr) - 1;
		Node **free = &freeList;

		while ((*free != NULL) && (*free < mem)) {
			if (mem ==
			    (*free + ((*free)->size / sizeof(Node)))) {
				(*free)->size += mem->size;
				mem = *free;
				break;
			}
			free = &((*free)->next);
		}

		if (mem != *free) {
			mem->next = *free;
			*free = mem;
		}

		if (mem->next == (mem + (mem->size / sizeof(Node)))) {
			mem->size += mem->next->size;
			mem->next = mem->next->next;
		}
	}
}


void *realloc(void *ptr, size_t size)
{
	if (ptr == NULL) {
		ptr = malloc(size);
	} else {
		Node *mem = ((Node *) ptr) - 1;

		size +=
		    /* header */ sizeof(Node) +
		/* round up */ (sizeof(Node) - 1);
		size -= size % sizeof(Node);

		if (size > mem->size) {
			/* Should try to grow */
			void *optr = ptr;

			ptr = malloc(size);
			if (ptr != NULL) {
				memcpy(ptr, optr,
				       mem->size - sizeof(Node));
				free(optr);
			}
		} else if (size < mem->size) {
			Node *split = mem + (size / sizeof(Node));

			split->size = mem->size - size;
			split->next = mem->next;
			mem->size = size;
			free((void *) (split + 1));
		}
	}

	return ptr;
}


void arclib_malloc_add(ULONG start, ULONG size)
{
	Node *node = (Node *) start;

	node->size = size - (size % sizeof(Node));
	free((void *) (node + 1));
}
