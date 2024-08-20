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
    buf[len - 1] = '\0';
    
    for (int i = 0; i < len - 1; i++)
    {
        buf[i] = uart_getc();
        if (buf[i] == '\n' || buf[i] == '\r')
        {
            buf[i] = '\0';
            return;
        }
    }
}

void uint64_to_str(char *str, uint64_t val, uint8_t base)
{
    char* p = str;
    if (val == 0)
    {
        *p++ = '0';
        *p = '\0';
        return;
    }

    char* p1, *p2;
    p1 = p2 = p + 19;
    while (val != 0)
    {
        uint64_t remainder = val % base;
        *p1-- = (remainder < 10) ? (remainder + '0') : (remainder - 10 + 'A');
        val /= base;
    }

    p1++;
    while (p1 <= p2)
    {
        *p++ = *p1++;
    }
    *p = '\0';
}

void uart_put_uint64(uint64_t num, uint8_t base)
{
    char num_str[21];
    uint64_to_str(num_str, num, base);
    uart_puts(num_str);
}