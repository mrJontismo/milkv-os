#include "pma.h"
#include "uart.h"
#include "memory.h"
#include "vmm.h"
#include <stdint.h>
#include <stdbool.h>

extern uintptr_t __kernel_begin;
extern uintptr_t __text_end;

pagetable_t kernel_pagetable;

void kernel_vmm_init(void)
{
    kernel_pagetable = kernel_pagetable_init();
}

pagetable_t kernel_pagetable_init(void)
{
    pagetable_t kpgtbl = (pagetable_t) kernel_phys_alloc();

    kernel_vm_map(kpgtbl, UART0_THR, UART0_THR, PAGE_SIZE * 16, PTE_R | PTE_W);
    kernel_vm_map(kpgtbl, KERNEL_BEGIN, KERNEL_BEGIN, TEXT_END - KERNEL_BEGIN, PTE_R | PTE_X);
    kernel_vm_map(kpgtbl, TEXT_END, TEXT_END, SG2002_DDR_END - TEXT_END, PTE_R | PTE_W); // Problem child

    return kpgtbl;
}

void kernel_vm_map(pagetable_t kpgtbl, uintptr_t va, uintptr_t pa, size_t sz, uint32_t perm)
{
    if (map_pages(kpgtbl, va, sz, pa, perm) != 0)
    {
        uart_puts("Error! Failed to map vm pages.\n");
    }
}

int map_pages(pagetable_t pagetable, uintptr_t va, size_t size, uintptr_t pa, uint32_t perm)
{
    uintptr_t a, last;
    pte_t *pte;

    if (size == 0)
    {
        uart_puts("Error! Invalid size in map_pages()\n");
    }
  
    a = PAGE_ROUND_DOWN(va);
    last = PAGE_ROUND_DOWN(va + size - 1);

    while (1)
    {
        if ((pte = get_pagetable_entry(pagetable, a, true)) == 0)
        {
            return -1;
        }

        if (*pte & PTE_V)
        {
            uart_puts("mappages: remap\n");
        }

        *pte = PA_TO_PTE(pa) | perm | PTE_V;
        
        if (a == last)
        {
            break;
        }
        
        a += PAGE_SIZE;
        pa += PAGE_SIZE;
    }

    return 0;
}

pte_t *get_pagetable_entry(pagetable_t pagetable, uintptr_t va, bool alloc)
{
    if (va >= MAX_VIRTUAL_ADDR)
    {
        uart_puts("get_pagetable_entry\n");
    }
    
    for (int level = 2; level > 0; level--)
    {
        pte_t *pte = &pagetable[PAGETABLE_INDEX(level, va)];

        if (*pte & PTE_V)
        {
            pagetable = (pagetable_t) PTE_TO_PA(*pte);
        } else
        {
            if (!alloc || (pagetable = (pte_t*)kernel_phys_alloc()) == 0)
            {
                return 0;
            }

            memset(pagetable, 0, PAGE_SIZE);
            *pte = PA_TO_PTE(pagetable) | PTE_V;
        }
    }

  return &pagetable[PAGETABLE_INDEX(0, va)];
}