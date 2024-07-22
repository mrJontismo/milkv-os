#ifndef STRING_H
#define STRING_H

#include <stdint.h>
#include <stddef.h>

void *memset(void *ptr, uint8_t value, size_t size);
size_t strlen(const char *str);

#endif