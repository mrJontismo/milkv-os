#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

#define PAGE_ROUND_UP(addr)    (((addr)+PAGE_SIZE-1) & ~(PAGE_SIZE-1))
#define PAGE_ROUND_DOWN(addr)  (((addr)) & ~(PAGE_SIZE-1))

#define PAGE_SIZE 4096

#define SG2002_DDR_END  (uintptr_t)0xFFFFD000
#define STACK_TOP       (uintptr_t)0xFFFFF000
#define KERNEL_BEGIN    (uintptr_t)0x80200000
#define KERNEL_END      (uintptr_t)&__kernel_end
#define TEXT_END        (uintptr_t)&__text_end

#define UART0            0x04140000 // size: 64K

#endif