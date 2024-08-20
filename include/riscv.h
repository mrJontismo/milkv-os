#ifndef RISCV_CSR_H
#define RISCV_CSR_H

#include <stdint.h>

static inline uint64_t csr_read_sstatus(void)
{
    uint64_t sstatus = 0;
    asm volatile("csrr %0, sstatus" : "=r" (sstatus));
    return sstatus;
}

static inline uint64_t csr_read_sepc(void)
{
    uint64_t sepc = 0;
    asm volatile("csrr %0, sepc" : "=r" (sepc));
    return sepc;
}

static inline void csr_write_sepc(uint64_t sepc)
{
    asm volatile("csrw sepc, %0" : : "r" (sepc));
}

static inline uint64_t csr_read_stvec(void)
{
    uint64_t stvec = 0;
    asm volatile("csrr %0, stvec" : "=r" (stvec));
    return stvec;
}

static inline uint64_t csr_read_scause(void)
{
    uint64_t scause = 0;
    asm volatile("csrr %0, scause" : "=r" (scause));
    return scause;
}

static inline uint64_t csr_read_stval(void)
{
    uint64_t stval = 0;
    asm volatile("csrr %0, stval" : "=r" (stval));
    return stval;
}

static inline uint64_t r_read_sp(void)
{
    uint64_t sp = 0;
    asm volatile("mv %0, sp" : "=r"(sp));
    return sp;
}

static inline void sfence_vma()
{
    asm volatile("sfence.vma zero, zero");
}

static inline void csr_write_satp(uint64_t pagetable)
{
    asm volatile("csrw satp, %0" : : "r" (pagetable));
}

#endif