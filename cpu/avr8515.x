
OUTPUT_FORMAT("elf32-avr","elf32-avr","elf32-avr")
OUTPUT_ARCH(avr:2)

MEMORY
{
/*
   rom     (rx)  : ORIGIN = 0x000000, LENGTH = 8K
*/

/* REGION  RWX     START              LENGTH       */
/* ------  ----    -----------------  -------------*/
   vectors (rx)  : ORIGIN = 0x000000, LENGTH = 0x1c
   rom     (rx)  : ORIGIN = 0x00001c, LENGTH = 8K-0x1c

   gpregs  (rw)  : ORIGIN = 0x800000, LENGTH = 0x20
   ioregs  (rw)  : ORIGIN = 0x800020, LENGTH = 0x40
   ram     (rw)  : ORIGIN = 0x800060, LENGTH = 512
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
/*    __DTOR_LIST__ = .; */
     *(.dtors)
/*    __DTOR_END__ = .; */
  }


  /***************************************************************************/


  /* Read-only sections, merged into text segment: */
  .hash          : { *(.hash)		}
  .dynsym        : { *(.dynsym)		}
  .dynstr        : { *(.dynstr)		}
  .gnu.version   : { *(.gnu.version)	}
  .gnu.version_d   : { *(.gnu.version_d)	}
  .gnu.version_r   : { *(.gnu.version_r)	}
  .rel.init      : { *(.rel.init)	}
  .rela.init     : { *(.rela.init) }


  /***************************************************************************/


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
  .rel.fini      : { *(.rel.fini)	}
  .rela.fini     : { *(.rela.fini) }
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
  .rel.ctors     : { *(.rel.ctors)	}
  .rela.ctors    : { *(.rela.ctors)	}
  .rel.dtors     : { *(.rel.dtors)	}
  .rela.dtors    : { *(.rela.dtors)	}
  .rel.got       : { *(.rel.got)		}
  .rela.got      : { *(.rela.got)		}
  .rel.bss       : { *(.rel.bss)		}
  .rela.bss      : { *(.rela.bss)		}
  .rel.plt       : { *(.rel.plt)		}
  .rela.plt      : { *(.rela.plt)		}

  /* Internal text space or external memory */
  .text :
  {
    *(.init0)  /* Start here after reset.  */
    *(.init1)
    *(.init2)  /* Clear __zero_reg__, set up stack pointer.  */
    *(.init3)
    *(.init4)  /* Initialize data and BSS.  */
    *(.init5)
    *(.init6)  /* C++ constructors.  */
    *(.init7)
    *(.init8)
    *(.init9)  /* Call main().  */
    *(.init)
    *(.progmem.gcc*)
    *(.progmem*)
    . = ALIGN(2);
    *(.text)
    . = ALIGN(2);
    *(.text.*)
    . = ALIGN(2);
    __CTOR_LIST__ = .;
    *(.ctors)
    __CTOR_END__ = .;
     _etext = . ; 
  }  > rom


  /***************************************************************************/


  .data	  : AT (ADDR (.text) + SIZEOF (.text))
  {
    *(.data)
    *(.gnu.linkonce.d*)
    . = ALIGN(2);
     _edata = . ; 
  }  > ram


  __data_length = SIZEOF(.data);
  __data_start = ADDR(.data);
  __data_end = __data_start + SIZEOF(.data);
  __data_load_start = LOADADDR(.data);
  __data_load_end = __data_load_start + SIZEOF(.data);


  /***************************************************************************/


  .bss  SIZEOF(.data) + ADDR(.data) :
  {
    *(.bss)
    *(COMMON)
     _end = . ;  
  }  > ram


  __bss_length = SIZEOF(.bss);
  __bss_start = LOADADDR(.bss);
  __bss_end = __bss_start + SIZEOF(.bss);


  /***************************************************************************/


  .eeprom  :
	AT (ADDR (.text) + SIZEOF (.text) + SIZEOF (.data))
  {
    *(.eeprom*)
     __eeprom_end = . ; 
  }  > eeprom


  /***************************************************************************/


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


  /***************************************************************************/


  /* PROVIDE (__stack = 0x25F) ;  */

}




