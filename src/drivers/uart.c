#include "uart.h"

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