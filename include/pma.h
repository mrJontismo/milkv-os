#ifndef PMA_H
#define PMA_H

#include "string.h"
#include <stdint.h>

#define PAGE_SIZE 4096
#define PAGE_ROUND_UP(sz) (((sz)+PAGE_SIZE-1) & ~(PAGE_SIZE-1))
#define PAGE_ROUND_DOWN(a) (((a)) & ~(PAGE_SIZE-1))

typedef struct PageFrame PageFrame;
struct PageFrame
{
    PageFrame *next;
};

typedef struct KernelFreeList
{
    PageFrame *free_list;
} KernelFreeList;

void kernel_phys_mem_init(void);
void *kernel_phys_alloc(void);
void kernel_phys_free(void *ptr);

#endif