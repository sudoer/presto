                              1 ;-------------------------------------------------------------------------------
                              2 ; CRT0.ASM
                              3 ; Basic definitions for the compiled code.
                              4 ;-------------------------------------------------------------------------------
                              5 
                              6    .module crt0.s
                              7 
                              8 
                              9 ;-------------------------------------------------------------------------------
                             10 ; Null-Ptr Destination (hope, that nobody writes more than 2 bytes here)
                             11 ;-------------------------------------------------------------------------------
                             12    .area   DIRECT (ABS,PAG)
                             13    .org  0
   0000                      14 _os_null_ptr::
   0000                      15 	.blkb 2
                             16 
                             17 
                             18 ;-------------------------------------------------------------------------------
                             19 ; Registers area
                             20 ;   Steven L. Barnicki, 2/1/99
                             21 ;-------------------------------------------------------------------------------
                             22    .area   REGS (ABS)
                             23 	.org  0x1000
   1000                      24 _porta::.blkb 1
   1001                      25 	.blkb 1         ;reserved
   1002                      26 _pioc:: .blkb 1
   1003                      27 _portc::.blkb 1
   1004                      28 _portb::.blkb 1
   1005                      29 _portcl::.blkb 1
   1006                      30 	.blkb 1         ;reserved
   1007                      31 _ddrc:: .blkb 1
   1008                      32 _portd::.blkb 1
   1009                      33 _ddrd:: .blkb 1
   100A                      34 _porte::.blkb 1
   100B                      35 _cforc::.blkb 1
   100C                      36 _oc1m:: .blkb 1
   100D                      37 _oc1d:: .blkb 1
   100E                      38 _tcnt:: .blkw 1
   1010                      39 _tic1:: .blkw 1
   1012                      40 _tic2:: .blkw 1
   1014                      41 _tic3:: .blkw 1
   1016                      42 _toc1:: .blkw 1
   1018                      43 _toc2:: .blkw 1
   101A                      44 _toc3:: .blkw 1
   101C                      45 _toc4:: .blkw 1
   101E                      46 _ti4::  .blkw 1
   1020                      47 _tctl1::.blkb 1
   1021                      48 _tctl2::.blkb 1
   1022                      49 _tmsk1::.blkb 1
   1023                      50 _tflg1::.blkb 1
   1024                      51 _tmsk2::.blkb 1
   1025                      52 _tflg2::.blkb 1
   1026                      53 _pactl::.blkb 1
   1027                      54 _pacnt::.blkb 1
   1028                      55 _spcr:: .blkb 1
   1029                      56 _spsr:: .blkb 1
   102A                      57 _spdr:: .blkb 1
   102B                      58 _baud:: .blkb 1
   102C                      59 _sccr1::.blkb 1
   102D                      60 _sccr2::.blkb 1
   102E                      61 _scsr:: .blkb 1
   102F                      62 _scdr:: .blkb 1
   1030                      63 _adctl::.blkb 1
   1031                      64 _adr1:: .blkb 1
   1032                      65 _adr2:: .blkb 1
   1033                      66 _adr3:: .blkb 1
   1034                      67 _adr4:: .blkb 1
   1035                      68 _bprot::.blkb 1
   1036                      69 _eprog::.blkb 1
   1037                      70 	.blkb 2	;reserved
   1039                      71 _option::.blkb 1
   103A                      72 _coprst::.blkb 1
   103B                      73 _pprog::.blkb 1
   103C                      74 _hprio::.blkb 1
   103D                      75 _init:: .blkb 1
   103E                      76 _test1::.blkb 1
   103F                      77 _config::.blkb 1
                             78 
                             79 ;-------------------------------------------------------------------------------
                             80 
                             81 .area   STARTUP      (ABS)
   0000                      82 __start::
                             83 ;         lds  #_stack_begin-1   ; initialize stack pointer
                             84 ;         jsr  __HC11Setup
                             85 
                             86 
                             87 
   0000                      88 loop1:
   8000 86 FF                89          ldaa    #255
   8002 B7 10 09             90          staa    4105
                             91 
   8005 86 00                92          ldaa    #00
   8007 B7 10 08             93          staa    4104
                             94 
   800A 86 FF                95          ldaa    #255
   800C B7 10 08             96          staa    4104
                             97 
   800F 20 EF                98          bra     loop1
                             99 
                            100 
                            101 
   8011 CE 00 00            102          ldx     #0                 ; clear the NULL pointer
   8014 DF 00               103          stx     *_os_null_ptr
                            104 
   8016 0E                  105          cli
   8017 BD 27 1E            106 			jsr     _main              ; main()
   001A                     107 __exit::
   801A 20 FE               108          bra     __exit
                            109 
                            110 ;-------------------------------------------------------------------------------
                            111 
                            112       .area CODE
   0000                     113 _inert_isr::
   C055 3B                  114          rti
                            115 
                            116 ;-------------------------------------------------------------------------------
                            117 
                            118 .area   BOOTLIST     (ABS)
                            119 .org    0xbfd6
   7FAC C0 55               120 .word   _inert_isr           ; 0xffd6  SCI Serial System
   7FAE C0 55               121 .word   _inert_isr           ; 0xffd8  SPI Serial Transfer complete
   7FB0 C0 55               122 .word   _inert_isr           ; 0xffda  Pulse Akku Input Edge
   7FB2 C0 55               123 .word   _inert_isr           ; 0xffdc  Pulse Akku Overflow
   7FB4 C0 55               124 .word   _inert_isr           ; 0xffde  Timer Overflow
   7FB6 C0 55               125 .word   _inert_isr           ; 0xffe0
   7FB8 C0 55               126 .word   _inert_isr           ; 0xffe2
   7FBA C0 55               127 .word   _inert_isr           ; 0xffe4
   7FBC C0 55               128 .word   _inert_isr           ; 0xffe6
   7FBE C0 55               129 .word   _inert_isr           ; 0xffe8
   7FC0 C0 55               130 .word   _inert_isr           ; 0xffea
   7FC2 C0 55               131 .word   _inert_isr           ; 0xffec
   7FC4 C0 55               132 .word   _inert_isr           ; 0xffee
   7FC6 C0 55               133 .word   _inert_isr           ; 0xfff0  Real Time Interrupt
   7FC8 C0 55               134 .word   _inert_isr           ; 0xfff2  Maskable Interrupt, Interrupt Request (IRQ)
   7FCA C0 55               135 .word   _inert_isr           ; 0xfff4  Nonmaskable Interrupt Request (XIRQ)
   7FCC C0 55               136 .word   _inert_isr           ; 0xfff6  Software Interrupt (SWI)
   7FCE C0 55               137 .word   _inert_isr           ; 0xfff8  Illegal Opcode
   7FD0 C0 55               138 .word   _inert_isr           ; 0xfffa  COP Watchdog Time-Out
   7FD2 C0 55               139 .word   _inert_isr           ; 0xfffc  Clock Monitor Fail
   7FD4 00 00               140 .word   _start               ; 0xfffe  Power On Reset (POR) oder RESET Pin
                            141 
                            142 ;-------------------------------------------------------------------------------
                            143 ; _os_set_irq
                            144 ; C-Call: void os_set_irq(int number, void (*fn)() )
                            145 
                            146 .area  _CODE
   0000                     147 _ods_set_irq::
   08B5 18 3C               148 			pshy  ; Save stack frame
   08B7 18 30               149 			tsy   ; Set current stack frame
   08B9 3C                  150 			pshx
                            151 
   08BA 18 EC 02            152          ldd   2,y       ; load integer number from stack
   08BD 4F                  153          clra            ; truncate to 8-bit number (in 16-bit register)
   08BE 05                  154          lsld            ; multiply by two (each entry in table is 2 bytes)
   08BF C3 BF D0            155          addd  #0xbfd0   ; add base address of interrupt table
   08C2 8F                  156          xgdx            ; put it in X
   08C3 18 EC 04            157          ldd   4,y       ; load address of function from stack
   08C6 ED 00               158          std   0,x       ; store address in interrupt table
                            159 
   08C8 38                  160          pulx
   08C9 18 38               161 			puly  ; Restore stack frame
   08CB 39                  162 			rts   ; return from function
                            163 
                            164 ;-------------------------------------------------------------------------------
                            165 
