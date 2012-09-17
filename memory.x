
MEMORY
{
  page0    (rwx) : ORIGIN = 0x0040, LENGTH = 0x00100 - 0x0040
  nothing        : ORIGIN = 0x0100, LENGTH = 0x08000 - 0x0100
  ram1     (rwx) : ORIGIN = 0x8000, LENGTH = 0x0B600 - 0x8000
  eeprom         : ORIGIN = 0xB600, LENGTH = 0x0B800 - 0xB600
  ram2     (rwx) : ORIGIN = 0xB800, LENGTH = 0x0BFC0 - 0xB800
  specvect  (r)  : ORIGIN = 0xBFC0, LENGTH = 0x0C000 - 0xBFC0
  ram3     (rwx) : ORIGIN = 0xC000, LENGTH = 0x0FFC0 - 0xC000
  normvect  (r)  : ORIGIN = 0xFFC0, LENGTH = 0x10000 - 0xFFC0
}


SECTIONS
{
   .text     : { *(.text) } > ram3              /* code */
   .bss      : { *(.bss) } > ram1               /* uninitialized data */
   .data     : { *(.data) } > ram2              /* initialized data */
   .specvect : { *(.specvect) } > specvect      /* interrupt vectors */
   .normvect : { *(.normvect) } > normvect      /* interrupt vectors */
}

