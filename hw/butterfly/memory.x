/* Default linker script, for normal executables */
OUTPUT_FORMAT("elf32-avr","elf32-avr","elf32-avr")
OUTPUT_ARCH(avr:5)



MEMORY
{
/* REGION  RWX     START              LENGTH       */
/* ------  ----    -----------------  -------------*/
   vectors (rx)  : ORIGIN = 0x000000, LENGTH = 0x5C
   rom     (rx)  : ORIGIN = 0x00005C, LENGTH = 16K-0x5C
   gpregs  (rw)  : ORIGIN = 0x800000, LENGTH = 0x20
   ioregs  (rw)  : ORIGIN = 0x800020, LENGTH = 0x40
   exregs  (rw)  : ORIGIN = 0x800060, LENGTH = 0xA0
   ram    (rw!x) : ORIGIN = 0x800100, LENGTH = 1K
   eeprom (rw!x) : ORIGIN = 0x810000, LENGTH = 512
}



SECTIONS
{


   /* interrupt vectors */
   .vectors : {
     __vectors_start = . ;
     vectors.o(.vectors)
     __vectors_end = . ;
   } > vectors

   /*
   not_used : {
     *(.vectors)
   } > rom
   */

  /***************************************************************************/


  /* We don't want these in an embedded system, if you want
   * them put them in the .text section above */
  /DISCARD/ :
  {
     *(.vectors)
     *(.fini)
     *(.fini7)
     *(.fini6)
     *(.fini5)
     *(.fini4)
     *(.fini3)
     *(.fini2)
     *(.fini1)
     *(.fini0)
     *(.dtors)
  }


  /***************************************************************************/


  /* Internal text space or external memory */
  .text :
  {
     __ctors_start = . ;
     *(.ctors)
     __ctors_end = . ;
     __dtors_start = . ;
     *(.dtors)
     __dtors_end = . ;
    *(.progmem.gcc*)
    *(.progmem*)
    . = ALIGN(2);
    *(.init0)  /* Start here after reset.  */
    *(.init1)
    *(.init3)
    *(.init4)  /* Initialize data and BSS.  */
    *(.init5)
    *(.init6)  /* C++ constructors.  */
    *(.init7)
    *(.init8)
    *(.init9)  /* Call main().  */
    *(.text)
    . = ALIGN(2);
    *(.text.*)
    . = ALIGN(2);
    *(.fini9)  /* _exit() starts here.  */
    *(.fini8)
    *(.fini7)
    *(.fini6)  /* C++ destructors.  */
    *(.fini5)
    *(.fini4)
    *(.fini3)
    *(.fini2)
    *(.fini1)
    *(.fini0)  /* Infinite loop after program termination.  */
     _etext = . ;
  }  > rom


  /DISCARD/ :
  {
    *(.init2)  /* Clear __zero_reg__, set up stack pointer.  */
  }



  .strings : {
    *(.string)
  }  > ram


  .heap : {
    *(.heap)
  }  > ram


  .stack : {
    *(.stack)
     /* PROVIDE (__stack = . - 1) ;  */
  }  > ram

  .data    : AT (ADDR (.text) + SIZEOF (.text))
  {
     PROVIDE (__data_start = .) ;
    *(.data)
    *(.gnu.linkonce.d*)
    . = ALIGN(2);
     _edata = . ;
     PROVIDE (__data_end = .) ;
  }  > ram


  .bss  SIZEOF(.data) + ADDR(.data) :
  {
     PROVIDE (__bss_start = .) ;
    *(.bss)
    *(COMMON)
     PROVIDE (__bss_end = .) ;
  }  > ram
   __data_load_start = LOADADDR(.data);
   __data_load_end = __data_load_start + SIZEOF(.data);


  /* Global data not cleared after reset.  */
  .noinit  SIZEOF(.bss) + ADDR(.bss) :
  {
     PROVIDE (__noinit_start = .) ;
    *(.noinit*)
     PROVIDE (__noinit_end = .) ;
     _end = . ;
     PROVIDE (__heap_start = .) ;
  }  > ram


  .eeprom  :
   AT (ADDR (.text) + SIZEOF (.text) + SIZEOF (.data))
  {
    *(.eeprom*)
     __eeprom_end = . ;
  }  > eeprom


  /* Read-only sections, merged into text segment: */
  .hash          : { *(.hash)    }
  .dynsym        : { *(.dynsym)     }
  .dynstr        : { *(.dynstr)     }
  .gnu.version   : { *(.gnu.version)   }
  .gnu.version_d   : { *(.gnu.version_d)  }
  .gnu.version_r   : { *(.gnu.version_r)  }
  .rel.init      : { *(.rel.init)      }
  .rela.init     : { *(.rela.init)  }
  .rel.text      :
    {
      *(.rel.text)
      *(.rel.text.*)
      *(.rel.gnu.linkonce.t*)
    }
  .rela.text     :
    {
      *(.rela.text)
      *(.rela.text.*)
      *(.rela.gnu.linkonce.t*)
    }
  .rel.fini      : { *(.rel.fini)      }
  .rela.fini     : { *(.rela.fini)  }
  .rel.rodata    :
    {
      *(.rel.rodata)
      *(.rel.rodata.*)
      *(.rel.gnu.linkonce.r*)
    }
  .rela.rodata   :
    {
      *(.rela.rodata)
      *(.rela.rodata.*)
      *(.rela.gnu.linkonce.r*)
    }
  .rel.data      :
    {
      *(.rel.data)
      *(.rel.data.*)
      *(.rel.gnu.linkonce.d*)
    }
  .rela.data     :
    {
      *(.rela.data)
      *(.rela.data.*)
      *(.rela.gnu.linkonce.d*)
    }
  .rel.ctors     : { *(.rel.ctors)  }
  .rela.ctors    : { *(.rela.ctors) }
  .rel.dtors     : { *(.rel.dtors)  }
  .rela.dtors    : { *(.rela.dtors) }
  .rel.got       : { *(.rel.got)    }
  .rela.got      : { *(.rela.got)      }
  .rel.bss       : { *(.rel.bss)    }
  .rela.bss      : { *(.rela.bss)      }
  .rel.plt       : { *(.rel.plt)    }
  .rela.plt      : { *(.rela.plt)      }


  /* Stabs debugging sections.  */
  .stab 0 : { *(.stab) }
  .stabstr 0 : { *(.stabstr) }
  .stab.excl 0 : { *(.stab.excl) }
  .stab.exclstr 0 : { *(.stab.exclstr) }
  .stab.index 0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment 0 : { *(.comment) }


  /* DWARF debug sections.
     Symbols in the DWARF debugging sections are relative to the beginning
     of the section so we begin them at 0.  */
  /* DWARF 1 */
  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }
  /* GNU DWARF 1 extensions */
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }
  /* DWARF 1.1 and DWARF 2 */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  /* DWARF 2 */
  .debug_info     0 : { *(.debug_info) *(.gnu.linkonce.wi.*) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }

}



