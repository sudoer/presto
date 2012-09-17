                              1 ;;;-----------------------------------------
                              2 ;;; Start MC6811 gcc assembly output
                              3 ;;; gcc compiler compiled on TBD
                              4 ;;; OPTIONS:	-mlong_branch optimize  
                              5 ;;; OPTIONS:	 peephole !omit_frame_pointer !signed-char
                              6 ;;; Source:		services\sound.c
                              7 ;;; Destination:	sound.s
                              8 ;;; Compiled:	Tue Oct 24 21:55:28 2000
                              9 ;;; (META)compiled by GNU C version 2.8.1.
                             10 ;;;-----------------------------------------
                             11 	.module sound.c
                             12 	.area	_CODE
                             13 	.globl	_sound_tick
   0000                      14 _sound_tick:
                             15 ;;;-----------------------------------------
                             16 ;;;  PROLOGUE for sound_tick
                             17 ;;;-----------------------------------------
   1E26 18 3C                18 	pshy	; Save stack frame
   1E28 18 30                19 	tsy	; Set current stack frame
   1E2A DE 00                20 	ldx	*ZD5
   1E2C 3C                   21 	pshx	; pushed register *ZD5
                             22 ;;;END PROLOGUE
   1E2D F6 10 00             23 	ldab	4096
   1E30 D7 01                24 	stab	*ZD5+1	; movqi: 4096 -> *ZD5
   1E32 D6 01                25 	ldab	*ZD5+1
   1E34 C8 08                26 	eorb	#8
   1E36 D7 01                27 	stab	*ZD5+1	; xorqi3: *ZD5 by #8 -> *ZD5
   1E38 D6 01                28 	ldab	*ZD5+1
   1E3A F7 10 00             29 	stab	4096	; movqi: *ZD5 -> 4096
                             30 ;;;EPILOGUE
   1E3D 38                   31 	pulx	; Pulling register *ZD5
   1E3E DF 00                32 	stx	*ZD5
   1E40 18 38                33 	puly	; Restore stack frame
   1E42 39                   34 	rts	; return from function
                             35 ;;;-----------------------------------------
                             36 ;;; END EPILOGUE for sound_tick
                             37 ;;;-----------------------------------------
                             38 	.globl	_sound_on
   001D                      39 _sound_on:
                             40 ;;;-----------------------------------------
                             41 ;;;  PROLOGUE for sound_on
                             42 ;;;-----------------------------------------
   1E43 18 3C                43 	pshy	; Save stack frame
   1E45 18 30                44 	tsy	; Set current stack frame
   1E47 DE 00                45 	ldx	*ZD5
   1E49 3C                   46 	pshx	; pushed register *ZD5
                             47 ;;;END PROLOGUE
   1E4A DC 00                48 	ldd	*ZD0
   1E4C 1A B3 A1 9B          49 	cpd	_beep_tone	; cmphi *ZD0 with _beep_tone
   1E50 26 03                50 	bne	.+5
   1E52 7E 1E 9F             51 	jmp	L3	; (beq) long branch
   1E55 DC 00                52 	ldd	*ZD0
   1E57 FD A1 9B             53 	std	_beep_tone	; movhi: *ZD0 -> _beep_tone
   1E5A FC 10 0E             54 	ldd	4110
   1E5D DD 00                55 	std	*ZD5	; movhi: 4110 -> *ZD5
   1E5F DC 00                56 	ldd	*ZD5
   1E61 D3 00                57 	addd	*ZD0
   1E63 DD 00                58 	std	*ZD5	; addhi3: *ZD5 by *ZD0 -> *ZD5
   1E65 DC 00                59 	ldd	*ZD5
   1E67 FD 10 1E             60 	std	4126	; movhi: *ZD5 -> 4126
   1E6A C6 08                61 	ldab	#8
   1E6C F7 10 23             62 	stab	4131	; movqi: #8 -> 4131
   1E6F F6 10 20             63 	ldab	4128
   1E72 D7 01                64 	stab	*ZD5+1	; movqi: 4128 -> *ZD5
   1E74 D6 01                65 	ldab	*ZD5+1
   1E76 CA 01                66 	orab	#1
   1E78 D7 01                67 	stab	*ZD5+1	; iorqi3: *ZD5 by #1 -> *ZD5
   1E7A D6 01                68 	ldab	*ZD5+1
   1E7C F7 10 20             69 	stab	4128	; movqi: *ZD5 -> 4128
   1E7F F6 10 20             70 	ldab	4128
   1E82 D7 01                71 	stab	*ZD5+1	; movqi: 4128 -> *ZD5
   1E84 D6 01                72 	ldab	*ZD5+1
   1E86 C4 FD                73 	andb	#253
   1E88 D7 01                74 	stab	*ZD5+1	; andqi3: *ZD5 by #253 -> *ZD5
   1E8A D6 01                75 	ldab	*ZD5+1
   1E8C F7 10 20             76 	stab	4128	; movqi: *ZD5 -> 4128
   1E8F F6 10 22             77 	ldab	4130
   1E92 D7 01                78 	stab	*ZD5+1	; movqi: 4130 -> *ZD5
   1E94 D6 01                79 	ldab	*ZD5+1
   1E96 CA 08                80 	orab	#8
   1E98 D7 01                81 	stab	*ZD5+1	; iorqi3: *ZD5 by #8 -> *ZD5
   1E9A D6 01                82 	ldab	*ZD5+1
   1E9C F7 10 22             83 	stab	4130	; movqi: *ZD5 -> 4130
   0079                      84 L3:
                             85 ;;;EPILOGUE
   1E9F 38                   86 	pulx	; Pulling register *ZD5
   1EA0 DF 00                87 	stx	*ZD5
   1EA2 18 38                88 	puly	; Restore stack frame
   1EA4 39                   89 	rts	; return from function
                             90 ;;;-----------------------------------------
                             91 ;;; END EPILOGUE for sound_on
                             92 ;;;-----------------------------------------
                             93 	.globl	_sound_off
   007F                      94 _sound_off:
                             95 ;;;-----------------------------------------
                             96 ;;;  PROLOGUE for sound_off
                             97 ;;;-----------------------------------------
   1EA5 18 3C                98 	pshy	; Save stack frame
   1EA7 18 30                99 	tsy	; Set current stack frame
   1EA9 DE 00               100 	ldx	*ZD5
   1EAB 3C                  101 	pshx	; pushed register *ZD5
                            102 ;;;END PROLOGUE
   1EAC F6 10 22            103 	ldab	4130
   1EAF D7 01               104 	stab	*ZD5+1	; movqi: 4130 -> *ZD5
   1EB1 D6 01               105 	ldab	*ZD5+1
   1EB3 C4 7F               106 	andb	#127
   1EB5 D7 01               107 	stab	*ZD5+1	; andqi3: *ZD5 by #127 -> *ZD5
   1EB7 D6 01               108 	ldab	*ZD5+1
   1EB9 F7 10 22            109 	stab	4130	; movqi: *ZD5 -> 4130
   1EBC F6 10 20            110 	ldab	4128
   1EBF D7 01               111 	stab	*ZD5+1	; movqi: 4128 -> *ZD5
   1EC1 D6 01               112 	ldab	*ZD5+1
   1EC3 C4 FC               113 	andb	#252
   1EC5 D7 01               114 	stab	*ZD5+1	; andqi3: *ZD5 by #252 -> *ZD5
   1EC7 D6 01               115 	ldab	*ZD5+1
   1EC9 F7 10 20            116 	stab	4128	; movqi: *ZD5 -> 4128
   1ECC CC 00 00            117 	ldd	#0
   1ECF FD A1 9B            118 	std	_beep_tone	; movhi: #0 -> _beep_tone
                            119 ;;;EPILOGUE
   1ED2 38                  120 	pulx	; Pulling register *ZD5
   1ED3 DF 00               121 	stx	*ZD5
   1ED5 18 38               122 	puly	; Restore stack frame
   1ED7 39                  123 	rts	; return from function
                            124 ;;;-----------------------------------------
                            125 ;;; END EPILOGUE for sound_off
                            126 ;;;-----------------------------------------
                            127 	.globl	_sound_isr
   00B2                     128 _sound_isr:
                            129 ;;;-----------------------------------------
                            130 ;;;  PROLOGUE for sound_isr
                            131 ;;;-----------------------------------------
   1ED8 18 3C               132 	pshy	; Save stack frame
   1EDA 18 30               133 	tsy	; Set current stack frame
   1EDC DE 00               134 	ldx	*ZD5
   1EDE 3C                  135 	pshx	; pushed register *ZD5
                            136 ;;;END PROLOGUE
   1EDF FC 10 0E            137 	ldd	4110
   1EE2 DD 00               138 	std	*ZD5	; movhi: 4110 -> *ZD5
   1EE4 DC 00               139 	ldd	*ZD5
   1EE6 F3 A1 9B            140 	addd	_beep_tone
   1EE9 DD 00               141 	std	*ZD5	; addhi3: *ZD5 by _beep_tone -> *ZD5
   1EEB DC 00               142 	ldd	*ZD5
   1EED FD 10 1E            143 	std	4126	; movhi: *ZD5 -> 4126
   1EF0 C6 08               144 	ldab	#8
   1EF2 F7 10 23            145 	stab	4131	; movqi: #8 -> 4131
                            146 ;;;EPILOGUE
   1EF5 38                  147 	pulx	; Pulling register *ZD5
   1EF6 DF 00               148 	stx	*ZD5
   1EF8 18 38               149 	puly	; Restore stack frame
   1EFA 39                  150 	rts	; return from function
                            151 ;;;-----------------------------------------
                            152 ;;; END EPILOGUE for sound_isr
                            153 ;;;-----------------------------------------
                            154 	.area	_BSS
                            155 	.globl	_beep_tone
   0000                     156 _beep_tone:	.blkb	2
                            157 	; END
