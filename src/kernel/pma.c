#include "uart.h"
#include "string.h"
#include "pma.h"
#include "memory.h"
#include <stdint.h>

extern uintptr_t __kernel_end;
KernelFreeList kernel_free_list = { NULL };

void kernel_phys_mem_init(void)
{
    uintptr_t begin = PAGE_ROUND_UP(KERNEL_END);
    uintptr_t end = PAGE_ROUND_DOWN(SG2002_DDR_END);
    size_t pages = 0;

    for (; begin < end; begin += PAGE_SIZE)
    {
        if (begin % PAGE_SIZE != 0 || begin <= KERNEL_END || begin >= end)
        {
            uart_puts("Fatal error! Failed to add physical page to free list.\n");
            return;
        }

        PageFrame *page;
        page = (PageFrame *)begin;
        page->next = kernel_free_list.free_list;
        kernel_free_list.free_list = page;
        ++pages;
    }

    uart_puts("[OK] PMA initialized with ");
    uart_put_uint64(pages, 10);
    uart_puts(" physical pages.\n");
}

void *kernel_phys_alloc(void)
{
    PageFrame *page;
    page = kernel_free_list.free_list;

    if (page)
    {
        kernel_free_list.free_list = page->next;
        memset((void *)page, 0, PAGE_SIZE);
    }

    return (void *)page;
}

void kernel_phys_free(void *ptr)
{
    PageFrame *page = (PageFrame *)ptr;
    uintptr_t page_address = (uintptr_t)page;

    if (page_address % PAGE_SIZE != 0 || page_address < KERNEL_END || page_address >= SG2002_DDR_END)
    {
        uart_puts("Fatal error! Failed to add physical page to free list.\n");
        return;
    }

    page->next = kernel_free_list.free_list;
    kernel_free_list.free_list = page;
}