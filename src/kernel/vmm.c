#include "pma.h"
#include "uart.h"
#include "memory.h"
#include "vmm.h"
#include "riscv.h"
#include <stdint.h>
#include <stdbool.h>

extern uintptr_t __text_end;

pte_t *kernel_vmm_walk_pagetable(pagetable_t pagetable, uintptr_t virtual_addr, bool alloc)
{
    if (virtual_addr >= MAX_VIRTUAL_ADDR)
    {
        uart_puts("Error! Virtual address is too large.\n");
    }

    for (uint8_t level = 2; level > 0; level--)
    {
        pte_t *pte = &pagetable[PAGE_INDEX(level, virtual_addr)];
        
        if (*pte & PTE_V)
        {
            pagetable = (pagetable_t) PTE2PA(*pte);
        }
        else
        {
            pagetable = (pagetable_t) kernel_phys_alloc();
            if (!alloc || pagetable == NULL)
            {
                uart_puts("Error! Failed to allocate memory for page table.\n");
                return NULL;
            }

            memset(pagetable, 0, PAGE_SIZE);
            *pte = PA2PTE(pagetable) | PTE_V;
        }
    }
    return &pagetable[PAGE_INDEX(0, virtual_addr)];
}

void kernel_vmm_map_pages(pagetable_t pagetable, uintptr_t virtual_addr, uintptr_t physical_addr, size_t length, uint64_t perm)
{
    if (length == 0)
    {
        uart_puts("Error! Cannot map page of size 0.\n");
    }
  
    uintptr_t begin = PAGE_ROUND_DOWN(virtual_addr);
    uintptr_t end = PAGE_ROUND_DOWN(virtual_addr + length - 1);

    pte_t *pte;

    while (true)
    {
        pte = kernel_vmm_walk_pagetable(pagetable, begin, true);
        if (pte == NULL)
        {
            uart_puts("Error! kernel_vmm_walk_pagetable() returned NULL in kernel_vmm_map_pages().\n");
            return;
        }

        if (*pte & PTE_V)
        {
            uart_puts("Error! Tried to remap a virtual address.\n");
        }

        /* SG2002 does not support hardware updating of PTE A/D bits, so we have have to manually set them. */
        *pte = PA2PTE(physical_addr) | perm | PTE_V | PTE_A | PTE_D;

        if (begin == end)
        {
            break;
        }

        begin += PAGE_SIZE;
        physical_addr += PAGE_SIZE;
    }
}

void kernel_hw_paging_enable(pagetable_t pagetable)
{
    sfence_vma();
    csr_write_satp(MAKE_SATP(pagetable));
    sfence_vma();
}

void kernel_vmm_pagetable_init(void)
{
    pagetable_t kernel_pagetable = (pagetable_t) kernel_phys_alloc();

    if (kernel_pagetable == NULL)
    {
        uart_puts("Error! Unable to allocate kernel pagetable.\n");
    }

    kernel_vmm_map_pages(kernel_pagetable, UART0, UART0, PAGE_SIZE * 16, PTE_R | PTE_W);
    kernel_vmm_map_pages(kernel_pagetable, KERNEL_BEGIN, KERNEL_BEGIN, TEXT_END - KERNEL_BEGIN, PTE_R | PTE_X);
    kernel_vmm_map_pages(kernel_pagetable, TEXT_END, TEXT_END, STACK_TOP - TEXT_END, PTE_R | PTE_W | PTE_X);

    kernel_hw_paging_enable(kernel_pagetable);

    uart_puts("[ OK ] Virtual memory and paging initialized.\n");
}