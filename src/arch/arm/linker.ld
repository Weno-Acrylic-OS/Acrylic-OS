/* src/arch/arm/linker.ld */
ENTRY(Reset_Handler)

MEMORY
{
FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 256K
  RAM (rwx)  : ORIGIN = 0x20000000, LENGTH = 64K}

_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector))
  } >FLASH

 .text :
  {
    . = ALIGN(4);
    *(.text*)
    *(.rodata*)
    *(.init)    /* Explicitly include .init here so it doesn't float */
    *(.fini)
    . = ALIGN(4);
  } >FLASH

  /* Use the keyword LOADADDR or a dedicated variable to ensure no overlap */
  _etext_flash_end = .; 

  .data : AT ( _etext_flash_end )
  {
    . = ALIGN(4);
    _sdata = .;
    *(.data*)
    . = ALIGN(4);
    _edata = .;
  } >RAM

  .bss :
  {
    . = ALIGN(4);
    _sbss = .;
    __bss_start__ = _sbss; /* Add this */
    *(.bss*)
    *(COMMON)              /* Add this to catch uninitialized globals */
    . = ALIGN(4);
    _ebss = .;
    __bss_end__ = _ebss;   /* Add this */
  } >RAM

  .stack :
  {
    . = ALIGN(8);
    . = . + 0x400; /* 1KB stack */
    _estack = .;
  } >RAM

  /DISCARD/ :
  {
    *(.comment)
    *(.ARM.attributes)
  }
}
