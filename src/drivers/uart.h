#ifndef UART_H
#define UART_H

#include "../include/stdint.h"
#include "../include/stddef.h"

#define UART0_THR (*((volatile uint32_t*)0x04140000))
#define UART0_LSR (*((volatile uint32_t*)0x04140014))

void uart_sendc(char c);
void uart_send(const char *str);

#endif