#include "uart.h"
#include "string.h"
#include "pma.h"
#include "memory.h"
#include <stdint.h>

extern uintptr_t __kernel_end;

freelist_t kernel_free_list = { NULL };

void kernel_phys_mem_init(void)
{
    size_t pages = 0;

    uintptr_t begin = PAGE_ROUND_UP(KERNEL_END);
    uintptr_t end = PAGE_ROUND_DOWN(ADDR_SPACE_END);

    for (; begin < end; begin += PAGE_SIZE)
    {
        if (begin % PAGE_SIZE != 0 || begin <= KERNEL_END || begin >= end)
        {
            uart_puts("Error! Invalid parameters for physical page during initialization.\n");
            return;
        }

        pageframe_t *page = (pageframe_t *)begin;
        page->next = kernel_free_list.free_list;
        kernel_free_list.free_list = page;
        ++pages;
    }

    uart_puts("[ OK ] PMA initialized with ");
    uart_put_uint64(pages, 10);
    uart_puts(" physical pages.\n");
}

void *kernel_phys_alloc(void)
{
    pageframe_t *page = kernel_free_list.free_list;

    if (page)
    {
        kernel_free_list.free_list = page->next;
        memset(page, 0, PAGE_SIZE);
    }
    else
    {
        return NULL;
    }

    return (void *)page;
}

void kernel_phys_free(const void *ptr)
{
    pageframe_t *page = (pageframe_t *) ptr;
    uintptr_t page_address = (uintptr_t) page;

    if (page_address % PAGE_SIZE != 0 || page_address < KERNEL_END || page_address >= ADDR_SPACE_END)
    {
        uart_puts("Error! Cannot free physical page due to invalid page properties.\n");
        return;
    }

    page->next = kernel_free_list.free_list;
    kernel_free_list.free_list = page;
}