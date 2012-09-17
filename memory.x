
MEMORY
{
   page0    (rwx) : ORIGIN = 0x0040, LENGTH =  0x0100 - 0x0040
   nothing        : ORIGIN = 0x0100, LENGTH =  0x8000 - 0x0100
   rom      (rwx) : ORIGIN = 0x8000, LENGTH =  0xBFD6 - 0x8000
   specvect  (r)  : ORIGIN = 0xBFD6, LENGTH =  0xC000 - 0xBFD6
   ram      (rwx) : ORIGIN = 0xC000, LENGTH =  0xFFD6 - 0xC000
   normvect  (r)  : ORIGIN = 0xFFD6, LENGTH = 0x10000 - 0xFFD6
}


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
     *(.text)
     *(.strings)
     __text_end = . ;
   } > rom


   /* uninitialized data */
   .bss : {
     __bss_start = . ;
      *(COMMON)
      *(.bss)
     __bss_end = . ;
   } > ram


   /* initialized data */
   .data : AT (__text_end) {
     __data_start = . ;
      *(.data)
     __data_end = . ;
   } > ram


   /* stack */
   .stack : {
     __stack_start = . ;
      *(.stack)
     __stack_end = . ;
   } > ram


   /* symbol information */
   .comment : {
     __comment_start = . ;
     *(.comment)
     __comment_end = . ;
   } > ram


   /* debug information */
   .debug : {
     __debug_start = . ;
     *(.debug_abbrev)
     *(.debug_info)
     *(.debug_line)
     *(.debug_pubnames)
     __debug_end = . ;
   } > nothing


}


__idata_start = __text_end ;
__idata_end = __data_end - __data_start + __idata_start ;


