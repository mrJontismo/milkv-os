#include "pma.h"
#include "uart.h"
#include "memory.h"
#include "vmm.h"
#include "riscv.h"
#include <stdint.h>
#include <stdbool.h>

extern uintptr_t __text_end;

pte_t *kernel_vmm_walk_pagetable(pagetable_t pagetable, const uintptr_t virtual_addr, const bool alloc)
{
    if (virtual_addr >= MAX_VIRTUAL_ADDR)
    {
        uart_puts("Error! Virtual address is too large.\n");
        return NULL;
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

void kernel_vmm_map_pages(pagetable_t pagetable, const uintptr_t virtual_addr, const uintptr_t physical_addr, const size_t length, const uint64_t perm)
{
    if (length == 0 || (virtual_addr % PAGE_SIZE != 0) || (physical_addr % PAGE_SIZE != 0))
    {
        uart_puts("Error! Invalid page parameters.\n");
        return;
    }
  
    uintptr_t address = physical_addr;
    uintptr_t begin = PAGE_ROUND_DOWN(virtual_addr);
    uintptr_t end = PAGE_ROUND_DOWN(virtual_addr + length - 1);

    pte_t *pte;

    while (true)
    {
        pte = kernel_vmm_walk_pagetable(pagetable, begin, true);
        if (pte == NULL)
        {
            uart_puts("Error! Got NULL from kernel_vmm_walk_pagetable().\n");
            return;
        }

        if (*pte & PTE_V)
        {
            uart_puts("Error! Tried to remap a virtual address.\n");
            return;
        }

        kernel_vmm_pte_set_perms(pte, address, perm);

        if (begin == end)
        {
            break;
        }

        begin += PAGE_SIZE;
        address += PAGE_SIZE;
    }
}

void kernel_vmm_hw_paging_enable(const pagetable_t pagetable)
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
        return;
    }

    kernel_vmm_map_pages(kernel_pagetable, UART0, UART0, PAGE_SIZE * 16, PTE_R | PTE_W); // UART0
    kernel_vmm_map_pages(kernel_pagetable, KERNEL_BEGIN, KERNEL_BEGIN, TEXT_END - KERNEL_BEGIN, PTE_R | PTE_X); // kernel .text segment
    kernel_vmm_map_pages(kernel_pagetable, TEXT_END, TEXT_END, ADDR_SPACE_END - TEXT_END, PTE_R | PTE_W); // rest of memory until stack
    kernel_vmm_map_pages(kernel_pagetable, ADDR_SPACE_END, ADDR_SPACE_END, STACK_TOP - ADDR_SPACE_END, PTE_R | PTE_W | PTE_X); // stack

    kernel_vmm_hw_paging_enable(kernel_pagetable);

    uart_puts("[ OK ] Virtual memory and paging initialized.\n");
}


void kernel_vmm_pte_set_perms(pte_t *pte, const uintptr_t physical_addr, const uint64_t perm)
{
    /* SG2002 does not support hardware updating of PTE A/D bits, so we have have to manually set them. */
    *pte = PA2PTE(physical_addr) | perm | PTE_V | PTE_A | PTE_D;
}