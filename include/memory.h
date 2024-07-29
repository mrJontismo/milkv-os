#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

#define SG2002_DDR_END 0xFFFFFFFF
#define KERNEL_BEGIN 0x80200000
#define KERNEL_END (uintptr_t)&__kernel_end
#define TEXT_END (uintptr_t)&__text_end

#define PAGE_SIZE 4096
#define PAGE_ROUND_UP(sz) (((sz)+PAGE_SIZE-1) & ~(PAGE_SIZE-1))
#define PAGE_ROUND_DOWN(a) (((a)) & ~(PAGE_SIZE-1))

#define PTE_V (1L << 0)
#define PTE_R (1L << 1)
#define PTE_W (1L << 2)
#define PTE_X (1L << 3)
#define PTE_U (1L << 4)
#define MAX_VIRTUAL_ADDR (1L << (9 + 9 + 9 + 12 - 1))
#define PAGE_OFFSET 12
#define PAGE_INDEX_MASK 0x1FF

#define PA_TO_PTE(pa) ((((uintptr_t)pa) >> 12) << 10)
#define PTE_TO_PA(pte) (((pte) >> 10) << 12)
#define PAGETABLE_INDEX_SHIFT(level) (PAGE_OFFSET + (9 * (level)))
#define PAGETABLE_INDEX(level, va) ((((uintptr_t) (va)) >> PAGETABLE_INDEX_SHIFT(level)) & PAGE_INDEX_MASK)

#endif