#include "uart.h"
#include "pma.h"

void main(void)
{
    uart_puts("MilkV-OS booting...\n");
    physical_memory_init();
    uart_puts("MilkV-OS shutting down.\n");
}