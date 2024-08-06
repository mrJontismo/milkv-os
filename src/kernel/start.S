.section .text
.global _start
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
    la sp, __stack_top
    jal ra, kernel_main

exit:
    j exit

.section .data
.section .bss
.section .rodata