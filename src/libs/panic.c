#include "panic.h"
#include "uart.h"
#include "riscv.h"
#include <stdint.h>

void panic_csr_dump(void)
{
    uint64_t scause = csr_read_scause();
    uint64_t sepc = csr_read_sepc();
    uint64_t sstatus = csr_read_sstatus();
    uint64_t stval = csr_read_stval();
    uint64_t sp = r_read_sp();

    uart_puts("-----START REGISTER DUMP-----\n");

    uart_puts("scause: 0x");
    uart_put_uint64(scause, 16);
    uart_puts("\n");

    uart_puts("sepc: 0x");
    uart_put_uint64(sepc, 16);
    uart_puts("\n");

    uart_puts("sstatus: 0x");
    uart_put_uint64(sstatus, 16);
    uart_puts("\n");

    uart_puts("stval: 0x");
    uart_put_uint64(stval, 16);
    uart_puts("\n");

    uart_puts("sp: 0x");
    uart_put_uint64(sp, 16);
    uart_puts("\n");

    uart_puts("-----END REGISTER DUMP-----\n");
}