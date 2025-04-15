ENTRY(_reset);

MEMORY {
  flash(rx)  : ORIGIN = 0x08000000, LENGTH = 1M 
  sram(rwx) : ORIGIN = 0x20000000, LENGTH = 256
}

_estack     = ORIGIN(sram) + LENGTH(sram);

SECTIONS {
  .vectors  : { KEEP(*(.vectors)) }   > flash
  .text     : { *(.text*) }           > flash
  .rodata   : { *(.rodata*) }         > flash

  .data : {
    _sdata = .;   /* .data section start */
    *(.first_data)
    *(.data SORT(.data.*))
    _edata = .;  /* .data section end */
  } > sram AT > flash
  _sidata = LOADADDR(.data);

  .bss : {
    _sbss = .;             
    *(.bss SORT(.bss.*) COMMON)
    _ebss = .;              
  } > sram

  . = ALIGN(8);
  _end = .;     
}
