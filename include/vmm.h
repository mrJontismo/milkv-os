#ifndef VMM_H
#define VMM_H

#include "memory.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef uint64_t pte_t;
typedef uint64_t *pagetable_t;

#define MAX_VIRTUAL_ADDR (1L << (9 + 9 + 9 + 12 - 1))
#define PAGE_OFFSET 12
#define PTE_V (1L << 0)
#define PTE_R (1L << 1)
#define PTE_W (1L << 2)
#define PTE_X (1L << 3)
#define PTE_U (1L << 4)
#define PTE_A (1L << 6)
#define PTE_D (1L << 7)
#define PA2PTE(pa) ((((uintptr_t)pa) >> 12) << 10)
#define PTE2PA(pte) (((pte) >> 10) << 12)
#define PAGE_INDEX_MASK 0x1FF
#define PAGE_INDEX_SHIFT(level) (PAGE_OFFSET + (9 * (level)))
#define PAGE_INDEX(level, virtual_addr) ((((pte_t) (virtual_addr)) >> PAGE_INDEX_SHIFT(level)) & PAGE_INDEX_MASK)
#define SATP_SV39 (8L << 60)
#define MAKE_SATP(pagetable) (uintptr_t)(SATP_SV39 | (((uintptr_t)pagetable) >> 12))

pte_t *kernel_vmm_walk_pagetable(pagetable_t pagetable, uintptr_t virtual_addr, bool alloc);
bool kernel_vmm_map_pages(pagetable_t pagetable, uintptr_t virtual_addr, uintptr_t physical_addr, size_t length, uint64_t perm);
void kernel_vmm_pagetable_init(void);

#endif