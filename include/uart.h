#ifndef UART_H
#define UART_H

#include <stdint.h>

#define UART0_THR (*((volatile uint32_t*)0x04140000))
#define UART0_RBR (*((volatile uint32_t*)0x04140000))
#define UART0_LSR (*((volatile uint32_t*)0x04140014))

void uart_putc(char c);
void uart_puts(const char *str);
char uart_getc(void);
void uart_gets(char *buf, uint8_t len);
void uint64_to_str(char *str, uint64_t val, uint8_t base);
void uart_put_uint64(uint64_t num, uint8_t base);

#endif