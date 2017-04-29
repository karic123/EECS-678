#ifndef BUDDY_H
#define BUDDY_H

#include "list.h"

typedef struct {
	struct list_head list; // contains pointers to next and prev
	char * ptr_memory;
	int order;
	int buddy_index;
	/* TODO: DECLARE NECESSARY MEMBER VARIABLES */
} page_t;

void buddy_init();
void *buddy_alloc(int size);
void buddy_free(void *addr);
void buddy_dump();
void breakdown( page_t* page, int order, int requiredOrder );

#endif // BUDDY_H
