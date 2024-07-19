OUTPUT_ARCH(riscv)
ENTRY(_start)

MEMORY
{
    ROM (rx)  : ORIGIN = 0x04400000, LENGTH = 128K
    DDR (rwx) : ORIGIN = 0x80000000, LENGTH = 2048M
}

SECTIONS
{
    . = 0x80200000;

    .text : {
        *(.text)
    } > DDR

    .rodata : {
        *(.rodata)
    } > DDR

    __data_start = .;
    .data : {
        *(.data)
    } > DDR
    __data_end = .;

    __bss_start = .;
    .bss : {
        *(.bss)
        *(COMMON)
    } > DDR
    __bss_end = .;

    /DISCARD/ : {
        *(.note.GNU-stack)
    }

    /* Offset because of the starting address */
    __stack_top = ORIGIN(DDR) + LENGTH(DDR) - 0x00200000;
}