.section .text
.global _start
.global trap_entry
.global kernel_main

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
    la t0, trap_entry
    csrw stvec, t0
    
    li sp, 0xFFFFF000
    jal ra, kernel_main

exit:
    j exit