#include "../include/stdint.h"
#include "../include/stddef.h"

#define UART0_THR (*((volatile uint32_t*)0x04140000))
#define UART0_LSR (*((volatile uint32_t*)0x04140014))

void uart_sendc(char c)
{
    while (!(UART0_LSR & 0x20));
    UART0_THR = c;
}

void uart_send(const char *str)
{
    while (*str)
    {
        uart_sendc(*str++);
    }
}

int main(void)
{
    const char *str = "Hello from C!\n";
    uart_send(str);
    return 0;
}
