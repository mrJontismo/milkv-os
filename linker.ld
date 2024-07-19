MEMORY
{
    ROM (rx)  : ORIGIN = 0x04400000, LENGTH = 128K
    RAM (rwx) : ORIGIN = 0x30000000, LENGTH = 256M
}

ENTRY(_start)

SECTIONS
{
    .text : {
        *(.text)
        *(.rodata)
        . = ALIGN(4);
    } > ROM

    .data : {
        *(.data)
        . = ALIGN(4);
    } > RAM AT > ROM

    .bss : {
        __bss_start__ = .;
        *(.bss)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    } > RAM

    ram_end = ORIGIN(RAM) + LENGTH(RAM);
    _end = .;
}
