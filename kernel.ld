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
    __text_end = .;

    .rodata : {
        *(.rodata)
    } > DDR

    .data : {
        *(.data)
    } > DDR

    .bss : {
        *(.bss)
        *(COMMON)
    } > DDR

    __kernel_end = .;

    /DISCARD/ : {
        *(.note.GNU-stack)
    }
}