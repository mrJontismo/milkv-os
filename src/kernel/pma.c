#include "uart.h"
#include "string.h"
#include "pma.h"
#include <stdint.h>

KernelFreeList kernel_free_list = { NULL };

void kernel_phys_mem_init(void)
{

    uintptr_t begin = PAGE_ROUND_UP((uintptr_t)__kernel_end);
    uintptr_t end = SG2002_DDR_END;

    size_t pages = 0;

    for (; begin + PAGE_SIZE < end; begin += PAGE_SIZE)
    {
        if (begin % PAGE_SIZE != 0 || (char *)begin <= __kernel_end || begin >= SG2002_DDR_END)
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
        memset((uint8_t *)page, 1, PAGE_SIZE);
    }

    return (void *)page;
}

void kernel_phys_free(PageFrame *page)
{
    uintptr_t page_address = (uintptr_t)page;
    if (page_address % PAGE_SIZE != 0 || (char *)page_address < __kernel_end || page_address >= SG2002_DDR_END)
    {
        uart_puts("Fatal error! Failed to add physical page to free list.\n");
        return;
    }

    page->next = kernel_free_list.free_list;
    kernel_free_list.free_list = page;
}