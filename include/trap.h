#ifndef TRAP_H
#define TRAP_H

#define SCAUSE_EXCEPTION_CODE(scause) ((scause) & 0x7FFFFFFFFFFFFFFF)

void trap_handler(void);
void interrupt_handler(uint64_t scause);
void exception_handler(uint64_t scause, uint64_t sepc);

#endif