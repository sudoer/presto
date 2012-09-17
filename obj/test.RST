                              1 ;;;-----------------------------------------
                              2 ;;; Start MC6811 gcc assembly output
                              3 ;;; gcc compiler compiled on TBD
                              4 ;;; OPTIONS:	-mlong_branch optimize  
                              5 ;;; OPTIONS:	 peephole !omit_frame_pointer !signed-char
                              6 ;;; Source:		app\test.c
                              7 ;;; Destination:	test.s
                              8 ;;; Compiled:	Tue Oct 24 21:55:33 2000
                              9 ;;; (META)compiled by GNU C version 2.8.1.
                             10 ;;;-----------------------------------------
                             11 	.module test.c
                             12 	; extern	_presto_sleep
                             13 	; extern	_presto_get_message
                             14 	; extern	_lcd_raw_char
                             15 	; extern	_lcd_print
                             16 	.area	_CODE
                             17 	.globl	_LcdTask
   0000                      18 _LcdTask:
                             19 ;;;-----------------------------------------
                             20 ;;;  PROLOGUE for LcdTask
                             21 ;;;-----------------------------------------
   24FD 3C                   22 	pshx	; allocate 2 byte auto variable
   24FE 3C                   23 	pshx	; allocate 2 byte auto variable
   24FF 3C                   24 	pshx	; allocate 2 byte auto variable
   2500 18 3C                25 	pshy	; Save stack frame
   2502 18 30                26 	tsy	; Set current stack frame
   2504 DE 00                27 	ldx	*ZD5
   2506 3C                   28 	pshx	; pushed register *ZD5
   2507 DE 00                29 	ldx	*ZD6
   2509 3C                   30 	pshx	; pushed register *ZD6
                             31 ;;;END PROLOGUE
   250A C6 20                32 	ldab	#32
   250C D7 01                33 	stab	*ZD6+1	; movqi: #32 -> *ZD6
   250E CC FF 00             34 	ldd	#-256
   2511 DD 00                35 	std	*ZXT
   2513 18 EC 06             36 	ldd	6,y
   2516 94 00                37 	anda	*ZXT
   2518 D4 01                38 	andb	*ZXT+1
   251A 18 ED 06             39 	std	6,y	; andhi3: 6,y by #-256 -> 6,y
   0020                      40 L4:
   251D BD 02 83             41 	jsr	_presto_sleep	; CALL: (VOIDmode) _presto_sleep (0 bytes)
   2520 18 DF 00             42 	sty	*ZXT
   2523 DC 00                43 	ldd	*ZXT
   2525 C3 00 02             44 	addd	#2
   2528 DD 00                45 	std	*ZD0	; addhi3: y by #2 -> *ZD0
   252A BD 06 07             46 	jsr	_presto_get_message	; CALL: R:*ZD0 = _presto_get_message (0 bytes)
   252D D6 01                47 	ldab	*ZD0+1	; tstqi: R:*ZD0
   252F 26 03                48 	bne	.+5
   2531 7E 25 1D             49 	jmp	L4	; (beq) long branch
   2534 18 E6 02             50 	ldab	2,y
   2537 4F                   51 	clra
   2538 DD 00                52 	std	*ZD5	; zero_extendqihi2: 2,y -> *ZD5
   253A DC 00                53 	ldd	*ZD5
   253C 1A 83 00 01          54 	cpd	#1	; cmphi *ZD5 with #1
   2540 26 03                55 	bne	.+5
   2542 7E 25 53             56 	jmp	L7	; (beq) long branch
   2545 DC 00                57 	ldd	*ZD5
   2547 1A 83 00 02          58 	cpd	#2	; cmphi *ZD5 with #2
   254B 26 03                59 	bne	.+5
   254D 7E 25 74             60 	jmp	L9	; (beq) long branch
   2550 7E 25 1D             61 	jmp	L4
   0056                      62 L7:
   2553 7C 00 01             63 	inc	*ZD6+1	; addqi3: *ZD6 by #1 -> *ZD6
   2556 D6 01                64 	ldab	*ZD6+1
   2558 C1 26                65 	cmpb	#38	; cmpqi *ZD6 with #38
   255A 22 03                66 	bhi	.+5
   255C 7E 25 63             67 	jmp	L8	; (bleu) long branch
   255F C6 21                68 	ldab	#33
   2561 D7 01                69 	stab	*ZD6+1	; movqi: #33 -> *ZD6
   0066                      70 L8:
   2563 D6 01                71 	ldab	*ZD6+1
   2565 37                   72 	pshb		; movqi: *ZD6 -> ,-sp
   2566 C6 0B                73 	ldab	#11
   2568 37                   74 	pshb		; movqi: #11 -> ,-sp
   2569 C6 00                75 	ldab	#0
   256B D7 01                76 	stab	*ZD0+1	; movqi: #0 -> *ZD0
   256D BD 12 9F             77 	jsr	_lcd_raw_char	; CALL: (VOIDmode) _lcd_raw_char (2 bytes)
   2570 38                   78 	pulx		; addhi3: sp by #2 -> sp
   2571 7E 25 1D             79 	jmp	L4
   0077                      80 L9:
   2574 18 E6 03             81 	ldab	3,y
   2577 18 E7 06             82 	stab	6,y	; movqi: 3,y -> 6,y
   257A 18 DF 00             83 	sty	*ZXT
   257D DC 00                84 	ldd	*ZXT
   257F C3 00 06             85 	addd	#6
   2582 DD 00                86 	std	*ZD0	; addhi3: y by #6 -> *ZD0
   2584 BD 11 93             87 	jsr	_lcd_print	; CALL: (VOIDmode) _lcd_print (0 bytes)
   2587 7E 25 1D             88 	jmp	L4
                             89 ;;;EPILOGUE
   258A 38                   90 	pulx	; Pulling register *ZD6
   258B DF 00                91 	stx	*ZD6
   258D 38                   92 	pulx	; Pulling register *ZD5
   258E DF 00                93 	stx	*ZD5
   2590 18 38                94 	puly	; Restore stack frame
   2592 38                   95 	pulx	; deallocate 2 byte auto variable
   2593 38                   96 	pulx	; deallocate 2 byte auto variable
   2594 38                   97 	pulx	; deallocate 2 byte auto variable
   2595 39                   98 	rts	; return from function
                             99 ;;;-----------------------------------------
                            100 ;;; END EPILOGUE for LcdTask
                            101 ;;;-----------------------------------------
                            102 	; extern	_motor_set_speed
                            103 	; extern	_presto_timer
                            104 	; extern	_presto_send_message
                            105 	.globl	_Zero
   0099                     106 _Zero:
                            107 ;;;-----------------------------------------
                            108 ;;;  PROLOGUE for Zero
                            109 ;;;-----------------------------------------
   2596 3C                  110 	pshx	; allocate 2 byte auto variable
   2597 3C                  111 	pshx	; allocate 2 byte auto variable
   2598 18 3C               112 	pshy	; Save stack frame
   259A 18 30               113 	tsy	; Set current stack frame
   259C DE 00               114 	ldx	*ZD5
   259E 3C                  115 	pshx	; pushed register *ZD5
                            116 ;;;END PROLOGUE
   259F C6 06               117 	ldab	#6
   25A1 D7 01               118 	stab	*ZD5+1	; movqi: #6 -> *ZD5
   25A3 CC 00 00            119 	ldd	#0
   25A6 18 ED 02            120 	std	2,y	; movhi: #0 -> 2,y
   00AC                     121 L16:
   25A9 D6 01               122 	ldab	*ZD5+1
   25AB 37                  123 	pshb		; movqi: *ZD5 -> ,-sp
   25AC C6 00               124 	ldab	#0
   25AE D7 01               125 	stab	*ZD0+1	; movqi: #0 -> *ZD0
   25B0 BD 19 23            126 	jsr	_motor_set_speed	; CALL: (VOIDmode) _motor_set_speed (1 bytes)
   25B3 CD EE 04            127 	ldx	4,y
   25B6 3C                  128 	pshx		; movhi: 4,y -> ,--sp
   25B7 CD EE 02            129 	ldx	2,y
   25BA 3C                  130 	pshx		; movhi: 2,y -> ,--sp
   25BB CE 01 F4            131 	ldx	#500
   25BE 3C                  132 	pshx		; movhi: #500 -> ,--sp
   25BF F6 A6 A1            133 	ldab	_zero_tid
   25C2 D7 01               134 	stab	*ZD0+1	; movqi: _zero_tid -> *ZD0
   25C4 BD 03 F3            135 	jsr	_presto_timer	; CALL: R:*ZD0 = _presto_timer (6 bytes)
   25C7 BD 02 83            136 	jsr	_presto_sleep	; CALL: (VOIDmode) _presto_sleep (0 bytes)
   25CA 18 DF 00            137 	sty	*ZXT
   25CD DC 00               138 	ldd	*ZXT
   25CF C3 00 02            139 	addd	#2
   25D2 DD 00               140 	std	*ZD0	; addhi3: y by #2 -> *ZD0
   25D4 BD 06 07            141 	jsr	_presto_get_message	; CALL: R:*ZD0 = _presto_get_message (0 bytes)
   25D7 D6 01               142 	ldab	*ZD5+1
   25D9 50                  143 	negb
   25DA D7 01               144 	stab	*ZD5+1	; negqi2: *ZD5 -> *ZD5
   25DC C6 01               145 	ldab	#1
   25DE 18 E7 02            146 	stab	2,y	; movqi: #1 -> 2,y
   25E1 CD EE 04            147 	ldx	4,y
   25E4 3C                  148 	pshx		; movhi: 4,y -> ,--sp
   25E5 CD EE 02            149 	ldx	2,y
   25E8 3C                  150 	pshx		; movhi: 2,y -> ,--sp
   25E9 F6 A6 9D            151 	ldab	_lcd_task_tid
   25EC D7 01               152 	stab	*ZD0+1	; movqi: _lcd_task_tid -> *ZD0
   25EE BD 03 C7            153 	jsr	_presto_send_message	; CALL: R:*ZD0 = _presto_send_message (4 bytes)
   25F1 30                  154 	tsx
   25F2 8F                  155 	xgdx
   25F3 C3 00 0B            156 	addd	#11
   25F6 8F                  157 	xgdx
   25F7 35                  158 	txs		; addhi3: sp by #11 -> sp
   25F8 7E 25 A9            159 	jmp	L16
                            160 ;;;EPILOGUE
   25FB 38                  161 	pulx	; Pulling register *ZD5
   25FC DF 00               162 	stx	*ZD5
   25FE 18 38               163 	puly	; Restore stack frame
   2600 38                  164 	pulx	; deallocate 2 byte auto variable
   2601 38                  165 	pulx	; deallocate 2 byte auto variable
   2602 39                  166 	rts	; return from function
                            167 ;;;-----------------------------------------
                            168 ;;; END EPILOGUE for Zero
                            169 ;;;-----------------------------------------
                            170 	.globl	_One
   0106                     171 _One:
                            172 ;;;-----------------------------------------
                            173 ;;;  PROLOGUE for One
                            174 ;;;-----------------------------------------
   2603 3C                  175 	pshx	; allocate 2 byte auto variable
   2604 3C                  176 	pshx	; allocate 2 byte auto variable
   2605 18 3C               177 	pshy	; Save stack frame
   2607 18 30               178 	tsy	; Set current stack frame
   2609 DE 00               179 	ldx	*ZD5
   260B 3C                  180 	pshx	; pushed register *ZD5
                            181 ;;;END PROLOGUE
   260C C6 06               182 	ldab	#6
   260E D7 01               183 	stab	*ZD5+1	; movqi: #6 -> *ZD5
   2610 CC 00 00            184 	ldd	#0
   2613 18 ED 02            185 	std	2,y	; movhi: #0 -> 2,y
   0119                     186 L21:
   2616 D6 01               187 	ldab	*ZD5+1
   2618 37                  188 	pshb		; movqi: *ZD5 -> ,-sp
   2619 C6 01               189 	ldab	#1
   261B D7 01               190 	stab	*ZD0+1	; movqi: #1 -> *ZD0
   261D BD 19 23            191 	jsr	_motor_set_speed	; CALL: (VOIDmode) _motor_set_speed (1 bytes)
   2620 CD EE 04            192 	ldx	4,y
   2623 3C                  193 	pshx		; movhi: 4,y -> ,--sp
   2624 CD EE 02            194 	ldx	2,y
   2627 3C                  195 	pshx		; movhi: 2,y -> ,--sp
   2628 CE 02 58            196 	ldx	#600
   262B 3C                  197 	pshx		; movhi: #600 -> ,--sp
   262C F6 A6 9E            198 	ldab	_one_tid
   262F D7 01               199 	stab	*ZD0+1	; movqi: _one_tid -> *ZD0
   2631 BD 03 F3            200 	jsr	_presto_timer	; CALL: R:*ZD0 = _presto_timer (6 bytes)
   2634 BD 02 83            201 	jsr	_presto_sleep	; CALL: (VOIDmode) _presto_sleep (0 bytes)
   2637 18 DF 00            202 	sty	*ZXT
   263A DC 00               203 	ldd	*ZXT
   263C C3 00 02            204 	addd	#2
   263F DD 00               205 	std	*ZD0	; addhi3: y by #2 -> *ZD0
   2641 BD 06 07            206 	jsr	_presto_get_message	; CALL: R:*ZD0 = _presto_get_message (0 bytes)
   2644 D6 01               207 	ldab	*ZD5+1
   2646 50                  208 	negb
   2647 D7 01               209 	stab	*ZD5+1	; negqi2: *ZD5 -> *ZD5
   2649 38                  210 	pulx
   264A 38                  211 	pulx
   264B 38                  212 	pulx
   264C 33                  213 	pulb		; addhi3: sp by #7 -> sp
   264D 7E 26 16            214 	jmp	L21
                            215 ;;;EPILOGUE
   2650 38                  216 	pulx	; Pulling register *ZD5
   2651 DF 00               217 	stx	*ZD5
   2653 18 38               218 	puly	; Restore stack frame
   2655 38                  219 	pulx	; deallocate 2 byte auto variable
   2656 38                  220 	pulx	; deallocate 2 byte auto variable
   2657 39                  221 	rts	; return from function
                            222 ;;;-----------------------------------------
                            223 ;;; END EPILOGUE for One
                            224 ;;;-----------------------------------------
                            225 	.globl	_Two
   015B                     226 _Two:
                            227 ;;;-----------------------------------------
                            228 ;;;  PROLOGUE for Two
                            229 ;;;-----------------------------------------
   2658 3C                  230 	pshx	; allocate 2 byte auto variable
   2659 3C                  231 	pshx	; allocate 2 byte auto variable
   265A 18 3C               232 	pshy	; Save stack frame
   265C 18 30               233 	tsy	; Set current stack frame
   265E DE 00               234 	ldx	*ZD5
   2660 3C                  235 	pshx	; pushed register *ZD5
                            236 ;;;END PROLOGUE
   2661 C6 06               237 	ldab	#6
   2663 D7 01               238 	stab	*ZD5+1	; movqi: #6 -> *ZD5
   2665 CC 00 00            239 	ldd	#0
   2668 18 ED 02            240 	std	2,y	; movhi: #0 -> 2,y
   016E                     241 L26:
   266B D6 01               242 	ldab	*ZD5+1
   266D 37                  243 	pshb		; movqi: *ZD5 -> ,-sp
   266E C6 02               244 	ldab	#2
   2670 D7 01               245 	stab	*ZD0+1	; movqi: #2 -> *ZD0
   2672 BD 19 23            246 	jsr	_motor_set_speed	; CALL: (VOIDmode) _motor_set_speed (1 bytes)
   2675 CD EE 04            247 	ldx	4,y
   2678 3C                  248 	pshx		; movhi: 4,y -> ,--sp
   2679 CD EE 02            249 	ldx	2,y
   267C 3C                  250 	pshx		; movhi: 2,y -> ,--sp
   267D CE 02 BC            251 	ldx	#700
   2680 3C                  252 	pshx		; movhi: #700 -> ,--sp
   2681 F6 A6 9F            253 	ldab	_two_tid
   2684 D7 01               254 	stab	*ZD0+1	; movqi: _two_tid -> *ZD0
   2686 BD 03 F3            255 	jsr	_presto_timer	; CALL: R:*ZD0 = _presto_timer (6 bytes)
   2689 BD 02 83            256 	jsr	_presto_sleep	; CALL: (VOIDmode) _presto_sleep (0 bytes)
   268C 18 DF 00            257 	sty	*ZXT
   268F DC 00               258 	ldd	*ZXT
   2691 C3 00 02            259 	addd	#2
   2694 DD 00               260 	std	*ZD0	; addhi3: y by #2 -> *ZD0
   2696 BD 06 07            261 	jsr	_presto_get_message	; CALL: R:*ZD0 = _presto_get_message (0 bytes)
   2699 D6 01               262 	ldab	*ZD5+1
   269B 50                  263 	negb
   269C D7 01               264 	stab	*ZD5+1	; negqi2: *ZD5 -> *ZD5
   269E 38                  265 	pulx
   269F 38                  266 	pulx
   26A0 38                  267 	pulx
   26A1 33                  268 	pulb		; addhi3: sp by #7 -> sp
   26A2 7E 26 6B            269 	jmp	L26
                            270 ;;;EPILOGUE
   26A5 38                  271 	pulx	; Pulling register *ZD5
   26A6 DF 00               272 	stx	*ZD5
   26A8 18 38               273 	puly	; Restore stack frame
   26AA 38                  274 	pulx	; deallocate 2 byte auto variable
   26AB 38                  275 	pulx	; deallocate 2 byte auto variable
   26AC 39                  276 	rts	; return from function
                            277 ;;;-----------------------------------------
                            278 ;;; END EPILOGUE for Two
                            279 ;;;-----------------------------------------
                            280 	.globl	_Three
   01B0                     281 _Three:
                            282 ;;;-----------------------------------------
                            283 ;;;  PROLOGUE for Three
                            284 ;;;-----------------------------------------
   26AD 3C                  285 	pshx	; allocate 2 byte auto variable
   26AE 3C                  286 	pshx	; allocate 2 byte auto variable
   26AF 3C                  287 	pshx	; allocate 2 byte auto variable
   26B0 3C                  288 	pshx	; allocate 2 byte auto variable
   26B1 18 3C               289 	pshy	; Save stack frame
   26B3 18 30               290 	tsy	; Set current stack frame
   26B5 DE 00               291 	ldx	*ZD5
   26B7 3C                  292 	pshx	; pushed register *ZD5
                            293 ;;;END PROLOGUE
   26B8 C6 06               294 	ldab	#6
   26BA D7 01               295 	stab	*ZD5+1	; movqi: #6 -> *ZD5
   26BC CC 00 00            296 	ldd	#0
   26BF 18 ED 02            297 	std	2,y	; movhi: #0 -> 2,y
   01C5                     298 L31:
   26C2 D6 01               299 	ldab	*ZD5+1
   26C4 37                  300 	pshb		; movqi: *ZD5 -> ,-sp
   26C5 C6 03               301 	ldab	#3
   26C7 D7 01               302 	stab	*ZD0+1	; movqi: #3 -> *ZD0
   26C9 BD 19 23            303 	jsr	_motor_set_speed	; CALL: (VOIDmode) _motor_set_speed (1 bytes)
   26CC CD EE 04            304 	ldx	4,y
   26CF 3C                  305 	pshx		; movhi: 4,y -> ,--sp
   26D0 CD EE 02            306 	ldx	2,y
   26D3 3C                  307 	pshx		; movhi: 2,y -> ,--sp
   26D4 CE 00 D2            308 	ldx	#210
   26D7 3C                  309 	pshx		; movhi: #210 -> ,--sp
   26D8 F6 A6 A0            310 	ldab	_three_tid
   26DB D7 01               311 	stab	*ZD0+1	; movqi: _three_tid -> *ZD0
   26DD BD 03 F3            312 	jsr	_presto_timer	; CALL: R:*ZD0 = _presto_timer (6 bytes)
   26E0 C6 01               313 	ldab	#1
   26E2 18 E7 02            314 	stab	2,y	; movqi: #1 -> 2,y
   26E5 CD EE 04            315 	ldx	4,y
   26E8 3C                  316 	pshx		; movhi: 4,y -> ,--sp
   26E9 CD EE 02            317 	ldx	2,y
   26EC 3C                  318 	pshx		; movhi: 2,y -> ,--sp
   26ED F6 A6 9D            319 	ldab	_lcd_task_tid
   26F0 D7 01               320 	stab	*ZD0+1	; movqi: _lcd_task_tid -> *ZD0
   26F2 BD 03 C7            321 	jsr	_presto_send_message	; CALL: R:*ZD0 = _presto_send_message (4 bytes)
   26F5 BD 02 83            322 	jsr	_presto_sleep	; CALL: (VOIDmode) _presto_sleep (0 bytes)
   26F8 18 DF 00            323 	sty	*ZXT
   26FB DC 00               324 	ldd	*ZXT
   26FD C3 00 06            325 	addd	#6
   2700 DD 00               326 	std	*ZD0	; addhi3: y by #6 -> *ZD0
   2702 BD 06 07            327 	jsr	_presto_get_message	; CALL: R:*ZD0 = _presto_get_message (0 bytes)
   2705 D6 01               328 	ldab	*ZD5+1
   2707 50                  329 	negb
   2708 D7 01               330 	stab	*ZD5+1	; negqi2: *ZD5 -> *ZD5
   270A 30                  331 	tsx
   270B 8F                  332 	xgdx
   270C C3 00 0B            333 	addd	#11
   270F 8F                  334 	xgdx
   2710 35                  335 	txs		; addhi3: sp by #11 -> sp
   2711 7E 26 C2            336 	jmp	L31
                            337 ;;;EPILOGUE
   2714 38                  338 	pulx	; Pulling register *ZD5
   2715 DF 00               339 	stx	*ZD5
   2717 18 38               340 	puly	; Restore stack frame
   2719 38                  341 	pulx	; deallocate 2 byte auto variable
   271A 38                  342 	pulx	; deallocate 2 byte auto variable
   271B 38                  343 	pulx	; deallocate 2 byte auto variable
   271C 38                  344 	pulx	; deallocate 2 byte auto variable
   271D 39                  345 	rts	; return from function
                            346 ;;;-----------------------------------------
                            347 ;;; END EPILOGUE for Three
                            348 ;;;-----------------------------------------
                            349 	; extern	_lcd_init
                            350 	.globl	_main
   0221                     351 _main:
                            352 ;;;-----------------------------------------
                            353 ;;;  PROLOGUE for main
                            354 ;;;-----------------------------------------
   271E 18 3C               355 	pshy	; Save stack frame
   2720 18 30               356 	tsy	; Set current stack frame
                            357 ;;;END PROLOGUE
   2722 BD 00 00            358 	jsr	___main	; CALL: (VOIDmode) ___main (0 bytes)
   0228                     359 L36:
   2725 BD 10 7C            360 	jsr	_lcd_init	; CALL: (VOIDmode) _lcd_init (0 bytes)
   2728 7E 27 25            361 	jmp	L36
                            362 ;;;EPILOGUE
   272B 18 38               363 	puly	; Restore stack frame
   272D 39                  364 	rts	; return from function
                            365 ;;;-----------------------------------------
                            366 ;;; END EPILOGUE for main
                            367 ;;;-----------------------------------------
                            368 	.area	_BSS
   0000                     369 _lcd_task_stack:	.blkb	256
   0100                     370 _task_one_stack:	.blkb	256
   0200                     371 _task_two_stack:	.blkb	256
   0300                     372 _task_three_stack:	.blkb	256
   0400                     373 _task_zero_stack:	.blkb	256
   0500                     374 _lcd_task_tid:	.blkb	1
   0501                     375 _one_tid:	.blkb	1
   0502                     376 _two_tid:	.blkb	1
   0503                     377 _three_tid:	.blkb	1
   0504                     378 _zero_tid:	.blkb	1
                            379 	; END
