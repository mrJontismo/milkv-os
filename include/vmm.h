#include "memory.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef uint64_t *pagetable_t;
typedef uint64_t pte_t;

void kernel_vmm_init(void);
pagetable_t kernel_pagetable_init(void);
void kernel_vm_map(pagetable_t kpgtbl, uintptr_t va, uintptr_t pa, size_t sz, uint32_t perm);
int map_pages(pagetable_t pagetable, uintptr_t va, size_t size, uintptr_t pa, uint32_t perm);
pte_t *get_pagetable_entry(pagetable_t pagetable, uintptr_t va, bool alloc);