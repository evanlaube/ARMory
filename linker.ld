ENTRY(_reset);

MEMORY {
  flash(rx)  : ORIGIN = 0x08000000, LENGTH = 512K
  sram(rwx)  : ORIGIN = 0x20000000, LENGTH = 128K
}

_estack = ORIGIN(sram) + LENGTH(sram);  /* stack top: grows down from top of RAM */

SECTIONS {
  .vectors  : { KEEP(*(.vectors)) }   > flash
  .text     : { *(.text*) }           > flash
  .rodata   : { *(.rodata*) }         > flash

  .data : {
    . = ALIGN(4);
    _sdata = .;   /* .data section start */
    *(.first_data)
    *(.data SORT(.data.*))
    . = ALIGN(4);
    _edata = .;  /* .data section end */
  } > sram AT > flash
  _sidata = LOADADDR(.data);

  .bss : {
    . = ALIGN(4);
    _sbss = .;
    *(.bss SORT(.bss.*) COMMON)
    . = ALIGN(4);
    _ebss = .;
  } > sram

  . = ALIGN(8);
  _end = .;     /* heap start: grows up toward _estack */
}
