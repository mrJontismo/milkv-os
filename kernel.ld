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
        *(.text .text.*)
        . = ALIGN(0x1000);
        PROVIDE(__text_end = .);
    } > DDR

    .rodata : {
        . = ALIGN(16);
        *(.srodata .srodata.*)
        . = ALIGN(16);
        *(.rodata .rodata.*)
    } > DDR

    .data : {
        . = ALIGN(16);
        *(.sdata .sdata.*)
        . = ALIGN(16);
        *(.data .data.*)
    } > DDR

    .bss : {
        . = ALIGN(16);
        *(.sbss .sbss.*)
        . = ALIGN(16);
        *(.bss .bss.*)
    } > DDR

    __kernel_end = .;

    /DISCARD/ : {
        *(.note.GNU-stack)
    }
}