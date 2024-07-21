#include "uart.h"
#include <stddef.h>

void uart_putc(char c)
{
    while (!(UART0_LSR & 0x20));
    UART0_THR = c;
}

void uart_puts(const char *str)
{
    if (str != NULL)
    {
        while (*str)
        {
            uart_putc(*str++);
        }
    }
}

char uart_getc(void)
{
    while (!(UART0_LSR & 0x01));
    return UART0_RBR;
}

void uart_gets(char *buf, uint8_t len)
{
    for (int i = 0; i < len - 1; i++)
    {
        buf[i] = uart_getc();
        if (buf[i] == '\n' || buf[i] == '\r')
        {
            buf[i] = '\0';
            return;
        }
    }
    buf[len - 1] = '\0';
}
