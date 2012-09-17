
OUTPUT_FORMAT("srec")

MEMORY
{
   page0    (rwx) : ORIGIN = 0x0040, LENGTH = 0x0100 - 0x0040
   nothing        : ORIGIN = 0x0100, LENGTH = 0x8000 - 0x0100
   ram1     (rwx) : ORIGIN = 0x8000, LENGTH = 0xBFD6 - 0x8000
   specvect  (r)  : ORIGIN = 0xBFD6, LENGTH = 0xC000 - 0xBFD6
   ram2     (rwx) : ORIGIN = 0xC000, LENGTH = 0xFFD6 - 0xC000
   normvect  (r)  : ORIGIN = 0xFFD6, LENGTH = 0x0000 - 0xFFD6
}

/* ram1     (rwx) : ORIGIN = 0x8000, LENGTH = 0xB600 - 0x8000 */
/* eeprom         : ORIGIN = 0xB600, LENGTH = 0xB800 - 0xB600 */
/* ram2     (rwx) : ORIGIN = 0xB800, LENGTH = 0xBFD6 - 0xB800 */


SECTIONS
{

   /* interrupt vectors */
   .specvect : {
     __specvect_start = . ;
      *(.specvect)
     __specvect_end = . ;
   } > specvect


   /* interrupt vectors */
   .normvect : {
     __normvect_start = . ;
      *(.normvect)
     __normvect_end = . ;
   } > normvect


   /* code */
   .text : {
     __text_start = . ;
     *(.install0)
     *(.install2)
     *(.install4)
     *(.text)
     *(.strings)
     __text_end = . ;
   } > ram1


   /* uninitialized data (variables) */
   .bss : {
     __bss_start = . ;
      *(.bss)
      *(COMMON)
     __bss_end = . ;
   } > ram2


   /* initialized data (values) */
   .idata : {
     __idata_start = . ;
      *(.data)
     __idata_end = . ;
   } > ram1


   /* initialized data (variables) */
   .data : {
     __data_start = . ;
      *(.data)
     __data_end = . ;
   } > ram2


   /* stack */
   .stack : {
     __stack_start = . ;
      *(.stack)
     __stack_end = . ;
   } > ram2


   /* symbol information */
   .comment : {
     __comment_start = . ;
     *(.comment)
     __comment_end = . ;
   } > ram2


}
