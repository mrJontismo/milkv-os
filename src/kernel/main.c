#include "uart.h"
#include "pma.h"
#include "vmm.h"

void kernel_main(void)
{
    uart_puts("Booting...\n");
    kernel_phys_mem_init();
    kernel_vmm_pagetable_init();
    uart_puts("Done.\n");
}