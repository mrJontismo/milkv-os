#ifndef PMA_H
#define PMA_H

#include "string.h"
#include <stdint.h>

#define SG2002_DDR_END 0xFFFFFFFF
#define PAGE_SIZE 4096
#define PAGE_ROUND_UP(sz)  (((sz)+PAGE_SIZE-1) & ~(PAGE_SIZE-1))

extern char __kernel_end[];

typedef struct PageFrame PageFrame;

struct PageFrame
{
    PageFrame *next;
};

typedef struct KernelFreeList
{
    PageFrame *free_list;
} KernelFreeList;

void physical_memory_init(void);
void *physical_alloc(void);
void physical_free(PageFrame *page);

#endif