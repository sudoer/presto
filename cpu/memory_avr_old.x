
MEMORY
{
   /* 8k of flash ROM */
   vectors    (r) : ORIGIN = 0x000000, LENGTH =  0x001a - 0x0000
   rom       (rx) : ORIGIN = 0x00001a, LENGTH =  0x1000 - 0x001a

   /* not much RAM */

   registers (rw) : ORIGIN = 0x800000, LENGTH =  0x0020 - 0x0000
   ioregs    (rw) : ORIGIN = 0x800020, LENGTH =  0x0060 - 0x0020
   sram      (rw) : ORIGIN = 0x800060, LENGTH =  0x0260 - 0x0060

   /* 64k of "nothing" */
   trash          : ORIGIN = 0x810000, LENGTH = 0x10000
}


SECTIONS
{

   /*---------------------------------------*/
   /*   I N T E R R U P T   V E C T O R S   */
   /*---------------------------------------*/

   /* interrupt vectors */
   .vectors : {
     __vectors_start = . ;
     *(.vectors)
     __vectors_end = . ;
   } > vectors

   /*-----------*/
   /*   R O M   */
   /*-----------*/

   .other : {
     *(.stab)
     *(.stabstr)
   } > rom


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
   } > sram


   /* (uninitialized) static global data */
   .bss : {
     __bss_start = . ;
      *(.bss)
     __bss_end = . ;
   } > sram


   /* uninitialized global data */
   .common : {
     __common_start = . ;
      *(COMMON)
     __common_end = . ;
   } > sram


   /* stack */
   .stack : {
     __stack_start = . ;
      *(.stack)
     __stack_end = . ;
   } > sram


   /* heap */
   .heap : {
     __heap_start = . ;
      *(.heap)
     __heap_end = . ;
   } > sram


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


