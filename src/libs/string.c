#include <stdint.h>
#include <stddef.h>
#include "string.h"

void *memset(void *ptr, uint8_t value, size_t size)
{
    uint8_t *destination = (uint8_t *)ptr;
    for (size_t i = 0; i < size; i++)
    {
        destination[i] = value;
    }
    return ptr;
}

size_t strlen(const char *str)
{
    size_t i;
    for(i = 0; str[i]; i++);
    return i;
}