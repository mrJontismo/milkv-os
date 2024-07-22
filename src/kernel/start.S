.section .text
.global _start
.global main

_start:
    j program_start
    .balign 4
    .word 0x33334c42  /* b'BL33' */
    .word 0xdeadbeea  /* CKSUM */
    .word 0xdeadbeeb  /* SIZE */
    .quad 0x80200000  /* RUNADDR */
    .word 0xdeadbeec
    .balign 4
    j program_start
    .balign 4

program_start:
    la sp, __stack_top

    la t0, __bss_start
    la t1, __bss_end
    li t2, 0

zero_bss:
    bge t0, t1, bss_done
    sw t2, 0(t0)
    addi t0, t0, 4
    j zero_bss

bss_done:
    jal ra, main

exit:
    j exit

.section .data
.section .bss
.section .rodata