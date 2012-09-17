
MEMORY
{
   /* 32k of "nothing" */
   page0     (rw) : ORIGIN =  0x0040, LENGTH =  0x0100 - 0x0040
   nothing1       : ORIGIN =  0x0100, LENGTH =  0x1000 - 0x0100
   registers (rw) : ORIGIN =  0x1000, LENGTH =  0x1040 - 0x1000
   nothing2       : ORIGIN =  0x1040, LENGTH =  0x8000 - 0x1040

   /* 16k of "ROM" */
   rom      (rwx) : ORIGIN =  0x8000, LENGTH =  0xBFD6 - 0x8000
   specvect   (r) : ORIGIN =  0xBFD6, LENGTH =  0xC000 - 0xBFD6

   /* 16k of "RAM" */
   ram       (rw) : ORIGIN =  0xC000, LENGTH =  0xFFD6 - 0xC000
   normvect  (rw) : ORIGIN =  0xFFD6, LENGTH = 0x10000 - 0xFFD6

   /* 64k of "nothing" */
   trash          : ORIGIN = 0x10000, LENGTH = 0x10000
}


SECTIONS
{

   /*---------------------------------------*/
   /*   I N T E R R U P T   V E C T O R S   */
   /*---------------------------------------*/

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

   /*-----------*/
   /*   R O M   */
   /*-----------*/

   /* code */
   .text : {
     __text_start = . ;
     *(.text)
     __text_end = . ;
   } > rom


   /* strings */
   .strings : {
     __strings_start = . ;
     *(.strings)
     __strings_end = . ;
   } > rom


   /* RO data */
   .rodata : {
     __rodata_start = . ;
     *(.rodata)
     __rodata_end = . ;
   } > rom


   .end_of_rom : {
     __idata_start = . ;
     /* 'idata' must be the last section in ROM, because we don't know how   */
     /* big it will be (yet).  When we create the 'data' section in RAM      */
     /* (below), we will specify that the initial values should be loaded    */
     /* here (using the AT directive).  The size of the 'idata' section      */
     /* will be the same as the size of the 'data' section.  After we are    */
     /* finished, we will calculate the value for __idata_end.  - Alan       */
   } > rom


   /*-----------*/
   /*   R A M   */
   /*-----------*/

   /* initialized data storage */
   /* stored in RAM, but initial values after RO data */
   .data : AT (__idata_start) {
     __data_start = . ;
      *(.data)
     __data_end = . ;
   } > ram


   /* (uninitialized) static global data */
   .bss : {
     __bss_start = . ;
      *(.bss)
     __bss_end = . ;
   } > ram


   /* uninitialized global data */
   .common : {
     __common_start = . ;
      *(COMMON)
     __common_end = . ;
   } > ram


   /* stack */
   .stack : {
     __stack_start = . ;
      *(.stack)
     __stack_end = . ;
   } > ram


   /* heap */
   .heap : {
     __heap_start = . ;
      *(.heap)
     __heap_end = . ;
   } > ram


   /*---------------*/
   /*   T R A S H   */
   /*---------------*/

   /* symbol information */
   .comment : {
     __comment_start = . ;
     *(.comment)
     __comment_end = . ;
   } > trash


   /* debug information */
   .debug : {
     __debug_start = . ;
     *(.debug_abbrev)
     *(.debug_aranges)
     *(.debug_info)
     *(.debug_line)
     *(.debug_pubnames)
     __debug_end = . ;
   } > trash


}


/* initialized data values */
__idata_end = __data_end - __data_start + __idata_start ;


