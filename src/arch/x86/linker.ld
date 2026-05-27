/*
 * Simple linker script for the x86 kernel.
 */

ENTRY(_start)

SECTIONS {
    . = 1M; /* Load the kernel at 1 MiB */

    .text : {
        *(.multiboot)
        *(.text)
    }

    .rodata : {
        *(.rodata)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss)
    }
}
