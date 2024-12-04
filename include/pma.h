#ifndef PMA_H
#define PMA_H

#include "string.h"
#include <stdint.h>

typedef struct pageframe_t pageframe_t;
struct pageframe_t
{
    pageframe_t *next;
};

typedef struct freelist_t
{
    pageframe_t *free_list;
} freelist_t;

void kernel_phys_mem_init(void);
void *kernel_phys_alloc(void);
void kernel_phys_free(const void *ptr);

#endif
