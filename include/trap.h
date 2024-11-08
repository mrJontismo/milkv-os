#ifndef TRAP_H
#define TRAP_H

#define SCAUSE_EXCEPTION_CODE(scause) ((scause) & 0x7FFFFFFFFFFFFFFF)

#define INSTRUCTION_ADDRESS_MISALIGNED 0x0
#define INSTRUCTION_ACCESS_FAULT 0x1
#define ILLEGAL_INSTRUCTION_EXCEPTION 0x2
#define BREAKPOINT 0x3
#define LOAD_ADDRESS_MISALIGNED 0x4
#define LOAD_ACCESS_FAULT 0x5
#define STORE_ADDRESS_MISALIGNED 0x6
#define STORE_ACCESS_FAULT 0x7
#define ECALL_U_MODE 0x8
#define ECALL_S_MODE 0x9
#define ECALL_M_MODE 0xB
#define INSTRUCTION_PAGE_FAULT 0xC
#define LOAD_PAGE_FAULT 0xD
#define STORE_PAGE_FAULT 0xF

void trap_handler(void);
void interrupt_handler(uint64_t scause);
void exception_handler(uint64_t scause, uint64_t sepc);

#endif