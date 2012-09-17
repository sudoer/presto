
OUTPUT_FORMAT("srec")

MEMORY
{
  page0    (rwx) : ORIGIN = 0x0040, LENGTH = 0x0100 - 0x0040
  nothing        : ORIGIN = 0x0100, LENGTH = 0x8000 - 0x0100
  ram1     (rwx) : ORIGIN = 0x8000, LENGTH = 0xB600 - 0x8000
  eeprom         : ORIGIN = 0xB600, LENGTH = 0xB800 - 0xB600
  ram2     (rwx) : ORIGIN = 0xB800, LENGTH = 0xBFD6 - 0xB800
  specvect  (r)  : ORIGIN = 0xBFD6, LENGTH = 0xC000 - 0xBFD6
  ram3     (rwx) : ORIGIN = 0xC000, LENGTH = 0xFFD6 - 0xC000
  normvect  (r)  : ORIGIN = 0xFFD6, LENGTH = 0x0000 - 0xFFD6
}


SECTIONS
{

   /* interrupt vectors */
   .specvect : {
     ___specvect_start = . ;
      *(.specvect)
     ___specvect_end = . ;
   } > specvect


   /* interrupt vectors */
   .normvect : {
     ___normvect_start = . ;
      *(.normvect)
     ___normvect_end = . ;
   } > normvect


   /* code */
   .text : {
     ___text_start = . ;
     *(.install0)
     *(.install2)
     *(.install4)
     *(.text)
     *(.strings)
     ___text_end = . ;
   } > ram3


   /* uninitialized data */
   .bss : {
     ___bss_start = . ;
      *(.bss)
      *(COMMON)
     ___bss_end = . ;
   } > ram1


   /* initialized data */
   .init : {
     ___data_start = . ;
      *(.data)
     ___data_end = . ;
   } > ram2


   /* stack */
   .stack : {
     ___stack_start = . ;
      *(.stack)
     ___stack_end = . ;
   } > ram1


   /* symbol information */
   .comment : {
     ___comment_start = . ;
     *(.comment)
     ___comment_end = . ;
   } > ram1


}
