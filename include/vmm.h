#include "memory.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define SATP_SV39 (8L << 60)
#define MAKE_SATP(pagetable) (SATP_SV39 | (((uint64_t)pagetable) >> PAGE_OFFSET))

typedef uint64_t *pagetable_t;
typedef uint64_t pte_t;

void kernel_vmm_init(void);
pagetable_t kernel_pagetable_init(void);
void kernel_vm_map(pagetable_t kernel_pagetable_internal, uintptr_t virtual_addr, uintptr_t physical_addr, size_t size, uint32_t perm);
bool map_pages(pagetable_t pagetable, uintptr_t va, size_t size, uintptr_t pa, uint32_t perm);
pte_t *get_pagetable_entry(pagetable_t pagetable, uintptr_t va, bool alloc);

static inline void tlb_flush(void)
{
    asm volatile("sfence.vma zero, zero");
}

static inline void satp_write(uint64_t pagetable)
{
    asm volatile("csrw satp, %0" : : "r" (pagetable));
}

void paging_enable(void);