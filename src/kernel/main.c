#include "uart.h"
#include "pma.h"
#include "vmm.h"

void kernel_main(void)
{
    uart_puts("MilkV-OS booting...\n");
    kernel_phys_mem_init();
    kernel_vmm_init();
    uart_puts("MilkV-OS shutting down.\n");
}