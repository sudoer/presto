                              1 ;;;-----------------------------------------
                              2 ;;; Start MC6811 gcc assembly output
                              3 ;;; gcc compiler compiled on TBD
                              4 ;;; OPTIONS:	-mlong_branch optimize  
                              5 ;;; OPTIONS:	 peephole !omit_frame_pointer !signed-char
                              6 ;;; Source:		kernel\clock.c
                              7 ;;; Destination:	clock.s
                              8 ;;; Compiled:	Tue Oct 24 21:55:04 2000
                              9 ;;; (META)compiled by GNU C version 2.8.1.
                             10 ;;;-----------------------------------------
                             11 	.module clock.c
                             12 	.area	_CODE
                             13 	.globl	_clock_reset
   0000                      14 _clock_reset:
                             15 ;;;-----------------------------------------
                             16 ;;;  PROLOGUE for clock_reset
                             17 ;;;-----------------------------------------
   0752 3C                   18 	pshx	; allocate 2 byte auto variable
   0753 3C                   19 	pshx	; allocate 2 byte auto variable
   0754 18 3C                20 	pshy	; Save stack frame
   0756 18 30                21 	tsy	; Set current stack frame
                             22 ;;;END PROLOGUE
   0758 CC 00 00             23 	ldd	#0
   075B 18 ED 04             24 	std	4,y	; movhi: #0 -> 4,y
   075E CC 00 00             25 	ldd	#0
   0761 18 ED 02             26 	std	2,y	; movhi: #0 -> 2,y
   0764 CC 00 00             27 	ldd	#0
   0767 DE 00                28 	ldx	*ZD0
   0769 ED 00                29 	std	0,x	; movhi: #0 -> 0,x
   076B 18 EC 04             30 	ldd	4,y
   076E DE 00                31 	ldx	*ZD0
   0770 ED 02                32 	std	2,x	; movhi: 4,y -> 2,x
                             33 ;;;EPILOGUE
   0772 18 38                34 	puly	; Restore stack frame
   0774 38                   35 	pulx	; deallocate 2 byte auto variable
   0775 38                   36 	pulx	; deallocate 2 byte auto variable
   0776 39                   37 	rts	; return from function
                             38 ;;;-----------------------------------------
                             39 ;;; END EPILOGUE for clock_reset
                             40 ;;;-----------------------------------------
                             41 	.globl	_clock_add
   0025                      42 _clock_add:
                             43 ;;;-----------------------------------------
                             44 ;;;  PROLOGUE for clock_add
                             45 ;;;-----------------------------------------
   0777 3C                   46 	pshx	; allocate 2 byte auto variable
   0778 3C                   47 	pshx	; allocate 2 byte auto variable
   0779 18 3C                48 	pshy	; Save stack frame
   077B 18 30                49 	tsy	; Set current stack frame
   077D DE 00                50 	ldx	*ZD5
   077F 3C                   51 	pshx	; pushed register *ZD5
   0780 DE 00                52 	ldx	*ZD6
   0782 3C                   53 	pshx	; pushed register *ZD6
   0783 DE 00                54 	ldx	*ZD7
   0785 3C                   55 	pshx	; pushed register *ZD7
                             56 ;;;END PROLOGUE
   0786 DC 00                57 	ldd	*ZD0
   0788 18 ED 02             58 	std	2,y	; movhi: *ZD0 -> 2,y
   078B EC 00                59 	ldd 0,x
   078D 18 ED 04             60 	std	4,y	; movhi: x -> 4,y
   0790 18 EC 08             61 	ldd	8,y
   0793 DD 00                62 	std	*ZD6	; movhi: 8,y -> *ZD6
   0795 EC 00                63 	ldd 0,x
   0797 D3 00                64 	addd	*ZD6
   0799 DD 00                65 	std	*ZD5	; addhi3: x by *ZD6 -> *ZD5
   079B DC 00                66 	ldd	*ZD5
   079D 18 ED 04             67 	std	4,y	; movhi: *ZD5 -> 4,y
   07A0 DC 00                68 	ldd	*ZD5
   07A2 1A 93 00             69 	cpd	*ZD6	; cmphi *ZD5 with *ZD6
   07A5 25 03                70 	blo	.+5
   07A7 7E 07 B6             71 	jmp	L3	; (bgeu) long branch
   07AA DC 00                72 	ldd	*ZD0
   07AC C3 00 01             73 	addd	#1
   07AF DD 00                74 	std	*ZD7	; addhi3: *ZD0 by #1 -> *ZD7
   07B1 DC 00                75 	ldd	*ZD7
   07B3 18 ED 02             76 	std	2,y	; movhi: *ZD7 -> 2,y
   0064                      77 L3:
   07B6 18 EC 02             78 	ldd	2,y
   07B9 DE 00                79 	ldx	*ZD0
   07BB ED 00                80 	std	0,x	; movhi: 2,y -> 0,x
   07BD 18 EC 04             81 	ldd	4,y
   07C0 DE 00                82 	ldx	*ZD0
   07C2 ED 02                83 	std	2,x	; movhi: 4,y -> 2,x
                             84 ;;;EPILOGUE
   07C4 38                   85 	pulx	; Pulling register *ZD7
   07C5 DF 00                86 	stx	*ZD7
   07C7 38                   87 	pulx	; Pulling register *ZD6
   07C8 DF 00                88 	stx	*ZD6
   07CA 38                   89 	pulx	; Pulling register *ZD5
   07CB DF 00                90 	stx	*ZD5
   07CD 18 38                91 	puly	; Restore stack frame
   07CF 38                   92 	pulx	; deallocate 2 byte auto variable
   07D0 38                   93 	pulx	; deallocate 2 byte auto variable
   07D1 39                   94 	rts	; return from function
                             95 ;;;-----------------------------------------
                             96 ;;; END EPILOGUE for clock_add
                             97 ;;;-----------------------------------------
                             98 	.globl	_clock_compare
   0080                      99 _clock_compare:
                            100 ;;;-----------------------------------------
                            101 ;;;  PROLOGUE for clock_compare
                            102 ;;;-----------------------------------------
   07D2 3C                  103 	pshx	; allocate 2 byte auto variable
   07D3 3C                  104 	pshx	; allocate 2 byte auto variable
   07D4 18 3C               105 	pshy	; Save stack frame
   07D6 18 30               106 	tsy	; Set current stack frame
   07D8 DE 00               107 	ldx	*ZD5
   07DA 3C                  108 	pshx	; pushed register *ZD5
                            109 ;;;END PROLOGUE
   07DB DC 00               110 	ldd	*ZD0
   07DD 18 ED 02            111 	std	2,y	; movhi: *ZD0 -> 2,y
   07E0 EC 00               112 	ldd 0,x
   07E2 18 ED 04            113 	std	4,y	; movhi: x -> 4,y
   07E5 18 EC 08            114 	ldd	8,y
   07E8 DD 00               115 	std	*ZD5	; movhi: 8,y -> *ZD5
   07EA DC 00               116 	ldd	*ZD0
   07EC 1A 93 00            117 	cpd	*ZD5	; cmphi *ZD0 with *ZD5
   07EF 24 03               118 	bhs	.+5
   07F1 7E 08 27            119 	jmp	L7	; (bltu) long branch
   07F4 22 03               120 	bhi	.+5
   07F6 7E 08 01            121 	jmp	L6	; (bleu) long branch
   07F9 CC 00 01            122 	ldd	#1
   07FC DD 00               123 	std	*ZD0	; movhi: #1 -> *ZD0
   07FE 7E 08 2C            124 	jmp	L9
   00AF                     125 L6:
   0801 18 EC 0A            126 	ldd	10,y
   0804 DD 00               127 	std	*ZD5	; movhi: 10,y -> *ZD5
   0806 EC 00               128 	ldd 0,x
   0808 1A 93 00            129 	cpd	*ZD5	; cmphi x with *ZD5
   080B 24 03               130 	bhs	.+5
   080D 7E 08 27            131 	jmp	L7	; (bltu) long branch
   0810 CC 00 01            132 	ldd	#1
   0813 DD 00               133 	std	*ZD0	; movhi: #1 -> *ZD0
   0815 EC 00               134 	ldd 0,x
   0817 1A 93 00            135 	cpd	*ZD5	; cmphi x with *ZD5
   081A 23 03               136 	bls	.+5
   081C 7E 08 2C            137 	jmp	L9	; (bgtu) long branch
   081F CC 00 00            138 	ldd	#0
   0822 DD 00               139 	std	*ZD0	; movhi: #0 -> *ZD0
   0824 7E 08 2C            140 	jmp	L9
   00D5                     141 L7:
   0827 CC FF FF            142 	ldd	#-1
   082A DD 00               143 	std	*ZD0	; movhi: #-1 -> *ZD0
   00DA                     144 L9:
                            145 ;;;EPILOGUE
   082C 38                  146 	pulx	; Pulling register *ZD5
   082D DF 00               147 	stx	*ZD5
   082F 18 38               148 	puly	; Restore stack frame
   0831 38                  149 	pulx	; deallocate 2 byte auto variable
   0832 38                  150 	pulx	; deallocate 2 byte auto variable
   0833 39                  151 	rts	; return from function
                            152 ;;;-----------------------------------------
                            153 ;;; END EPILOGUE for clock_compare
                            154 ;;;-----------------------------------------
                            155 	; END
