#include "../drivers/uart.h"

void main(void)
{
    const char *str = "Hello from C!\n";
    uart_send(str);
}
