                              1 ;;;-----------------------------------------
                              2 ;;; Start MC6811 gcc assembly output
                              3 ;;; gcc compiler compiled on TBD
                              4 ;;; OPTIONS:	-mlong_branch optimize  
                              5 ;;; OPTIONS:	 peephole !omit_frame_pointer !signed-char
                              6 ;;; Source:		kernel\system.c
                              7 ;;; Destination:	system.s
                              8 ;;; Compiled:	Tue Oct 24 21:55:07 2000
                              9 ;;; (META)compiled by GNU C version 2.8.1.
                             10 ;;;-----------------------------------------
                             11 	.module system.c
                             12 	.area	_CODE
                             13 	.globl	__HC11Setup
   0000                      14 __HC11Setup:
                             15 ;;;-----------------------------------------
                             16 ;;;  PROLOGUE for _HC11Setup
                             17 ;;;-----------------------------------------
   0834 18 3C                18 	pshy	; Save stack frame
   0836 18 30                19 	tsy	; Set current stack frame
                             20 ;;;END PROLOGUE
                             21 ; Begin inline assembler code
   0838 0F                   22 		sei
                             23 ; End of inline assembler code
   0839 C6 00                24 	ldab	#0
   083B F7 10 22             25 	stab	4130	; movqi: #0 -> 4130
   083E C6 00                26 	ldab	#0
   0840 F7 10 24             27 	stab	4132	; movqi: #0 -> 4132
   0843 C6 0C                28 	ldab	#12
   0845 F7 10 3F             29 	stab	4159	; movqi: #12 -> 4159
   0848 C6 A0                30 	ldab	#160
   084A F7 10 39             31 	stab	4153	; movqi: #160 -> 4153
                             32 ; Begin inline assembler code
   084D 0E                   33 		cli
                             34 ; End of inline assembler code
                             35 ;;;EPILOGUE
   084E 18 38                36 	puly	; Restore stack frame
   0850 39                   37 	rts	; return from function
                             38 ;;;-----------------------------------------
                             39 ;;; END EPILOGUE for _HC11Setup
                             40 ;;;-----------------------------------------
                             41 	.globl	_set_interrupt
   001D                      42 _set_interrupt:
                             43 ;;;-----------------------------------------
                             44 ;;;  PROLOGUE for set_interrupt
                             45 ;;;-----------------------------------------
   0851 18 3C                46 	pshy	; Save stack frame
   0853 18 30                47 	tsy	; Set current stack frame
                             48 ;;;END PROLOGUE
                             49 ;;;EPILOGUE
   0855 18 38                50 	puly	; Restore stack frame
   0857 39                   51 	rts	; return from function
                             52 ;;;-----------------------------------------
                             53 ;;; END EPILOGUE for set_interrupt
                             54 ;;;-----------------------------------------
                             55 	.globl	_presto_fatal_error
   0024                      56 _presto_fatal_error:
                             57 ;;;-----------------------------------------
                             58 ;;;  PROLOGUE for presto_fatal_error
                             59 ;;;-----------------------------------------
   0858 18 3C                60 	pshy	; Save stack frame
   085A 18 30                61 	tsy	; Set current stack frame
   085C DE 00                62 	ldx	*ZD5
   085E 3C                   63 	pshx	; pushed register *ZD5
   085F DE 00                64 	ldx	*ZD6
   0861 3C                   65 	pshx	; pushed register *ZD6
                             66 ;;;END PROLOGUE
   0862 CC 00 00             67 	ldd	#0
   0865 DD 00                68 	std	*ZD6	; movhi: #0 -> *ZD6
                             69 ; Begin inline assembler code
   0867 0F                   70 		sei
                             71 ; End of inline assembler code
   0868 F6 10 09             72 	ldab	4105
   086B D7 01                73 	stab	*ZD5+1	; movqi: 4105 -> *ZD5
   086D D6 01                74 	ldab	*ZD5+1
   086F CA 10                75 	orab	#16
   0871 D7 01                76 	stab	*ZD5+1	; iorqi3: *ZD5 by #16 -> *ZD5
   0873 D6 01                77 	ldab	*ZD5+1
   0875 F7 10 09             78 	stab	4105	; movqi: *ZD5 -> 4105
   0044                      79 L7:
   0878 F6 10 00             80 	ldab	4096
   087B D7 01                81 	stab	*ZD5+1	; movqi: 4096 -> *ZD5
   087D D6 01                82 	ldab	*ZD5+1
   087F C8 08                83 	eorb	#8
   0881 D7 01                84 	stab	*ZD5+1	; xorqi3: *ZD5 by #8 -> *ZD5
   0883 D6 01                85 	ldab	*ZD5+1
   0885 F7 10 00             86 	stab	4096	; movqi: *ZD5 -> 4096
   0888 F6 10 08             87 	ldab	4104
   088B D7 01                88 	stab	*ZD5+1	; movqi: 4104 -> *ZD5
   088D D6 01                89 	ldab	*ZD5+1
   088F C4 EF                90 	andb	#239
   0891 D7 01                91 	stab	*ZD5+1	; andqi3: *ZD5 by #239 -> *ZD5
   0893 D6 01                92 	ldab	*ZD5+1
   0895 F7 10 08             93 	stab	4104	; movqi: *ZD5 -> 4104
   0898 DC 00                94 	ldd	*ZD6	; tsthi: R:*ZD6
   089A 26 03                95 	bne	.+5
   089C 7E 08 78             96 	jmp	L7	; (beq) long branch
   006B                      97 L10:
   089F DE 00                98 	ldx	*ZD6
   08A1 08                   99 	inx
   08A2 DF 00               100 	stx	*ZD6	; addhi3: *ZD6 by #1 -> *ZD6
   08A4 27 03               101 	beq	.+5
   08A6 7E 08 9F            102 	jmp	L10	; (bne) long branch
   08A9 7E 08 78            103 	jmp	L7
                            104 ;;;EPILOGUE
   08AC 38                  105 	pulx	; Pulling register *ZD6
   08AD DF 00               106 	stx	*ZD6
   08AF 38                  107 	pulx	; Pulling register *ZD5
   08B0 DF 00               108 	stx	*ZD5
   08B2 18 38               109 	puly	; Restore stack frame
   08B4 39                  110 	rts	; return from function
                            111 ;;;-----------------------------------------
                            112 ;;; END EPILOGUE for presto_fatal_error
                            113 ;;;-----------------------------------------
                            114 	; END
