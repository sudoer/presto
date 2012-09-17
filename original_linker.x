/* Default linker script, for normal executables */
/* Linker script for 68HC11 executable (PROM).  */
OUTPUT_FORMAT("elf32-m68hc11", "elf32-m68hc11", "elf32-m68hc11")
OUTPUT_ARCH(m68hc11)
ENTRY(_start)
 SEARCH_DIR(/usr/local/m6811-elf/lib);
/* Fixed definition of the available memory banks.
   See generic emulation script for a user defined configuration.  */
MEMORY
{
  page0 (rwx)    : ORIGIN = 0x0, LENGTH = 256
  text_low (rx)  : ORIGIN = 0x08000, LENGTH = 0x0B600 - 0x08000 + 128
  text  (rx)     : ORIGIN = 0x0C000, LENGTH = 0x10000 - 0x0C000 + 128
  data           : ORIGIN = 0x01040, LENGTH = 0x08000 - 0x01040
}
/* Setup the stack on the top of the data memory bank.  */
PROVIDE (_stack = 0x08000 - 1);
SECTIONS
{
  .hash          : { *(.hash)		}
  .dynsym        : { *(.dynsym)		}
  .dynstr        : { *(.dynstr)		}
  .gnu.version		  : { *(.gnu.version) }
  .gnu.version_d	  : { *(.gnu.version_d) }
  .gnu.version_r	  : { *(.gnu.version_r) }
  .rel.text      :
    {
      *(.rel.text)
      *(.rel.text.*)
      *(.rel.gnu.linkonce.t.*)
    }
  .rela.text     :
    {
      *(.rela.text)
      *(.rela.text.*)
      *(.rela.gnu.linkonce.t.*)
    }
  .rel.data      :
    {
      *(.rel.data)
      *(.rel.data.*)
      *(.rel.gnu.linkonce.d.*)
    }
  .rela.data     :
    {
      *(.rela.data)
      *(.rela.data.*)
      *(.rela.gnu.linkonce.d.*)
    }
  .rel.rodata    :
    {
      *(.rel.rodata)
      *(.rel.rodata.*)
      *(.rel.gnu.linkonce.r.*)
    }
  .rela.rodata   :
    {
      *(.rela.rodata)
      *(.rela.rodata.*)
      *(.rela.gnu.linkonce.r.*)
    }
  .rel.sdata     :
    {
      *(.rel.sdata)
      *(.rel.sdata.*)
      *(.rel.gnu.linkonce.s.*)
    }
  .rela.sdata     :
    {
      *(.rela.sdata)
      *(.rela.sdata.*)
      *(.rela.gnu.linkonce.s.*)
    }
  .rel.sbss      :
    {
      *(.rel.sbss)
      *(.rel.sbss.*)
      *(.rel.gnu.linkonce.sb.*)
    }
  .rela.sbss     :
    {
      *(.rela.sbss)
      *(.rela.sbss.*)
      *(.rel.gnu.linkonce.sb.*)
    }
  .rel.bss       :
    {
      *(.rel.bss)
      *(.rel.bss.*)
      *(.rel.gnu.linkonce.b.*)
    }
  .rela.bss      :
    {
      *(.rela.bss)
      *(.rela.bss.*)
      *(.rela.gnu.linkonce.b.*)
    }
  .rela.stext		  : { *(.rela.stest) }
  .rela.etext		  : { *(.rela.etest) }
  .rela.sdata		  : { *(.rela.sdata) }
  .rela.edata		  : { *(.rela.edata) }
  .rela.eit_v		  : { *(.rela.eit_v) }
  .rela.ebss		  : { *(.rela.ebss) }
  .rela.srodata		  : { *(.rela.srodata) }
  .rela.erodata		  : { *(.rela.erodata) }
  .rela.got		  : { *(.rela.got) }
  .rela.ctors		  : { *(.rela.ctors) }
  .rela.dtors		  : { *(.rela.dtors) }
  .rela.init		  : { *(.rela.init) }
  .rela.fini		  : { *(.rela.fini) }
  .rela.plt		  : { *(.rela.plt) }
  .rel.stext		  : { *(.rel.stest) }
  .rel.etext		  : { *(.rel.etest) }
  .rel.sdata		  : { *(.rel.sdata) }
  .rel.edata		  : { *(.rel.edata) }
  .rel.ebss		  : { *(.rel.ebss) }
  .rel.eit_v		  : { *(.rel.eit_v) }
  .rel.srodata		  : { *(.rel.srodata) }
  .rel.erodata		  : { *(.rel.erodata) }
  .rel.got		  : { *(.rel.got) }
  .rel.ctors		  : { *(.rel.ctors) }
  .rel.dtors		  : { *(.rel.dtors) }
  .rel.init		  : { *(.rel.init) }
  .rel.fini		  : { *(.rel.fini) }
  .rel.plt		  : { *(.rel.plt) }
  /* Concatenate .page0 sections.  Put them in the page0 memory bank
     unless we are creating a relocatable file.  */
  .page0 :
  {
    *(.page0)
    *(.softregs)
  }  > page0
  /* Start of text section.  */
  .stext   :
  {
    *(.stext)
  }  > text
  .init	  :
  {
    *(.init)
  } =0
  .text_low  :
  {
    /* Put startup code at beginning so that _start keeps same address.  */
    /* Startup code.  */
    *(.install0)	/* Section should setup the stack pointer.  */
    *(.install1)	/* Place holder for applications.  */
    *(.install2)	/* Optional installation of data sections in RAM.  */
    *(.install3)	/* Place holder for applications.  */
    *(.install4)	/* Section that calls the main.  */
    *(.init)
    ebcs.o(.text)
    date-panel.o(.text)
    main-panel.o(.text)
    info-panel.o(.text)
    debug.o(.text)
  }  > text_low
  .text  :
  {
    *(.text)
    *(.text.*)
    /* .gnu.warning sections are handled specially by elf32.em.  */
    *(.gnu.warning)
    *(.gnu.linkonce.t.*)
    /* Finish code.  */
    *(.fini0)		/* Beginning of finish code (_exit symbol).  */
    *(.fini1)		/* Place holder for applications.  */
    *(.fini2)		/* C++ destructors.  */
    *(.fini3)		/* Place holder for applications.  */
    *(.fini4)		/* Runtime exit.  */
    _etext = .;
    PROVIDE (etext = .);
  }  > text
  .eh_frame   :
  {
    KEEP (*(.eh_frame))
  }  > text
  .rodata    :
  {
    *(.rodata)
    *(.rodata.*)
    *(.gnu.linkonce.r*)
  }  > text_low
  .rodata1   :
  {
    *(.rodata1)
  }  > text_low
  /* Constructor and destructor tables are in ROM.  */
  .ctors   :
  {
     PROVIDE (__CTOR_LIST__ = .);
    *(.ctors)
    /* We don't want to include the .ctor section from
       from the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last
    KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors)) */
     PROVIDE(__CTOR_END__ = .);
  }  > text
    .dtors	  :
  {
     PROVIDE(__DTOR_LIST__ = .);
    *(.dtors)
    /*
    KEEP (*crtbegin.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors)) */
     PROVIDE(__DTOR_END__ = .);
  }  > text
  .jcr   :
  {
    KEEP (*(.jcr))
  }  > text
  /* Start of the data section image in ROM.  */
  __data_image = .;
  PROVIDE (__data_image = .);
  /* All read-only sections that normally go in PROM must be above.
     We construct the DATA image section in PROM at end of all these
     read-only sections.  The data image must be copied at init time.
     Refer to GNU ld, Section 3.6.8.2 Output Section LMA.  */
  .data    : AT (__data_image)
  {
    __data_section_start = .;
    PROVIDE (__data_section_start = .);
    *(.sdata)
    *(.data)
    *(.data.*)
    *(.data1)
    *(.gnu.linkonce.d.*)
    CONSTRUCTORS
    _edata  =  .;
    PROVIDE (edata = .);
  }  > data
  __data_section_size = SIZEOF(.data);
  PROVIDE (__data_section_size = SIZEOF(.data));
  __data_image_end = __data_image + __data_section_size;
/* SCz: this does not work yet... This is supposed to force the loading
   of _map_data.o (from libgcc.a) when the .data section is not empty.
   By doing so, this should bring the code that copies the .data section
   from ROM to RAM at init time.
  ___pre_comp_data_size = SIZEOF(.data);
  __install_data_sections = ___pre_comp_data_size > 0 ?
		__map_data_sections : 0;
*/
  /* .install  :
  {
    . = _data_image_end;
  }  > text */
  /* Relocation for some bss and data sections.  */
  .bss   :
  {
    __bss_start = .;
    *(.sbss)
    *(.scommon)
    *(.dynbss)
    *(.bss)
    *(.bss.*)
    *(.gnu.linkonce.b.*)
    *(COMMON)
    PROVIDE (_end = .);
  }  > data
  __bss_size = SIZEOF(.bss);
  PROVIDE (__bss_size = SIZEOF(.bss));
  /* If the 'vectors_addr' symbol is defined, it indicates the start address
     of interrupt vectors.  This depends on the 68HC11 operating mode:
			Addr
     Single chip	0xffc0
     Extended mode	0xffc0
     Bootstrap		0x00c0
     Test		0xbfc0
     In general, the vectors address is 0xffc0.  This can be overriden
     with the '-defsym vectors_addr=0xbfc0' ld option.
     Note: for the bootstrap mode, the interrupt vectors are at 0xbfc0 but
     they are redirected to 0x00c0 by the internal PROM.  Application's vectors
     must also consist of jump instructions (see Motorola's manual).  */
  PROVIDE (_vectors_addr = DEFINED (vectors_addr) ? vectors_addr : 0xffc0);
  .vectors DEFINED (vectors_addr) ? vectors_addr : 0xffc0 :
  {
    *(.vectors)
  }
  /* Stabs debugging sections.  */
  .stab		 0 : { *(.stab) }
  .stabstr	 0 : { *(.stabstr) }
  .stab.excl	 0 : { *(.stab.excl) }
  .stab.exclstr	 0 : { *(.stab.exclstr) }
  .stab.index	 0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment	 0 : { *(.comment) }
  /* DWARF debug sections.
     Symbols in the DWARF debugging sections are relative to the beginning
     of the section so we begin them at 0.
     Treatment of DWARF debug section must be at end of the linker
     script to avoid problems when there are undefined symbols. It's necessary
     to avoid that the DWARF section is relocated before such undefined
     symbols are found.  */
  /* DWARF 1 */
  .debug	 0 : { *(.debug) }
  .line		 0 : { *(.line) }
  /* GNU DWARF 1 extensions */
  .debug_srcinfo 0 : { *(.debug_srcinfo) }
  .debug_sfnames 0 : { *(.debug_sfnames) }
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
