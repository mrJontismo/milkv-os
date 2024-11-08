#include "uart.h"
#include "trap.h"
#include "riscv.h"
#include "panic.h"
#include <stdint.h>

void trap_handler(void)
{
    uint64_t scause = csr_read_scause();
    uint64_t sepc = csr_read_sepc();
    uint64_t sstatus = csr_read_sstatus();

    if (scause >> 63)
    {
        interrupt_handler(scause);
    }
    else
    {
        exception_handler(scause, sepc);
    }
}

void interrupt_handler(uint64_t scause)
{
    uart_puts("Interrupt triggered!\n");
}

void exception_handler(uint64_t scause, uint64_t sepc)
{
    switch (SCAUSE_EXCEPTION_CODE(scause))
    {
        case INSTRUCTION_ADDRESS_MISALIGNED:
            uart_puts("Instruction address misaligned.\n");
            sepc += 4;
            break;
        case INSTRUCTION_ACCESS_FAULT:
            uart_puts("Instruction access fault.\n");
            sepc += 4;
            break;
        case ILLEGAL_INSTRUCTION_EXCEPTION:
            uart_puts("Illegal instruction exception.\n");
            sepc += 4;
            break;
        case BREAKPOINT:
            uart_puts("Breakpoint.\n");
            sepc += 4;
            break;
        case LOAD_ADDRESS_MISALIGNED:
            uart_puts("Load address misaligned.\n");
            sepc += 4;
            break;
        case LOAD_ACCESS_FAULT:
            uart_puts("Load access fault.\n");
            sepc += 4;
            break;
        case STORE_ADDRESS_MISALIGNED:
            uart_puts("Store/AMO address misaligned.\n");
            sepc += 4;
            break;
        case STORE_ACCESS_FAULT:
            uart_puts("Store/AMO access fault.\n");
            sepc += 4;
            break;
        case ECALL_U_MODE:
            uart_puts("Environment call (ecall) from U-mode.\n");
            sepc += 4;
            break;
        case ECALL_S_MODE:
            uart_puts("Environment call (ecall) from S-mode.\n");
            sepc += 4;
            break;
        case ECALL_M_MODE:
            uart_puts("Environment call (ecall) from M-mode.\n");
            sepc += 4;
            break;
        case INSTRUCTION_PAGE_FAULT:
            uart_puts("Instruction page fault.\n");
            sepc += 4;
            break;
        case LOAD_PAGE_FAULT:
            uart_puts("Load page fault.\n");
            sepc += 4;
            break;
        case STORE_PAGE_FAULT:
            uart_puts("Store/AMO page fault.\n");
            sepc += 4;
            break;
        default:
            uart_puts("Unhandled exception.\n");
            sepc += 4;
            break;
    }

    csr_write_sepc(sepc);
}