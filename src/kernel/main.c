#include "uart.h"
#include "pma.h"

void kernel_main(void)
{
    uart_puts("MilkV-OS booting...\n");
    kernel_phys_mem_init();
    uart_puts("MilkV-OS shutting down.\n");
}