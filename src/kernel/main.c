#include "../drivers/uart.h"

void main(void)
{
    uart_puts("Booting...\n");
    uart_puts("Enter your name: ");
    
    char buf[64];
    uart_gets(buf, 64);

    uart_puts("Welcome to Milkv-OS, ");
    uart_puts(buf);
    uart_puts("!\n");
}
