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
    KEEP(*(.isr_vector))  /* Keep vector table at the very start of Flash [cite: 3] */
    *(.text*)             /* All code [cite: 4] */
    *(.rodata*)           /* All constants/read-only data [cite: 4] */
    
    /* Optional but recommended: include these to prevent floating overlaps */
    KEEP(*(.init))
    KEEP(*(.fini))
    
    . = ALIGN(4);
  } >FLASH

  /* Exception index table required for ARM standard libraries [cite: 7] */
  .ARM.exidx : 
  {
    __exidx_start = .;
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    __exidx_end = .;
  } >FLASH

  /* _etext marks the absolute end of stuff in FLASH. 
     The .data section will be copied FROM here into RAM. [cite: 8] */
  _etext = .; 
  __etext = .; 
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
    _sbss = .;
    __bss_start__ = .;
    *(.bss*)
    *(COMMON)
    _ebss = .;
    __bss_end__ = .;
    end = .;           /* Add this for _sbrk */
    _end = .;          /* Add this for _sbrk */
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
