/* Minimal linker script for STM32F103C8T6 (64KB flash, 20KB RAM) */

/* Memory spaces */
MEMORY
{
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 64K
  RAM (xrw)  : ORIGIN = 0x20000000, LENGTH = 20K
}

_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
  .isr_vector :
  {
    KEEP(*(.isr_vector .isr_vector.*))
  } >FLASH

  .text :
  {
    _stext = .;
    *(.text*)
    *(.rodata*)
    *(.glue_7)
    *(.glue_7t)
    _etext = .;
  } >FLASH

  .data : AT(_etext)
  {
    _sdata = .;
    *(.data*)
    _edata = .;
  } >RAM

  .bss (NOLOAD) :
  {
    _sbss = .;
    *(.bss*)
    *(COMMON)
    _ebss = .;
  } >RAM

  /* Stack */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE(__HeapBase = .);
    . = . + 0x1000; /* 4KB heap default */
    PROVIDE(__HeapLimit = .);
    . = ALIGN(8);
    PROVIDE(__StackTop = _estack);
  } >RAM

  /DISCARD/ : { *(.note.gnu.*) }
}

/* Provide symbols for startup */
PROVIDE(_sidata = _etext);
