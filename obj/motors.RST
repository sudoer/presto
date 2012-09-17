                              1 ;;;-----------------------------------------
                              2 ;;; Start MC6811 gcc assembly output
                              3 ;;; gcc compiler compiled on TBD
                              4 ;;; OPTIONS:	-mlong_branch optimize  
                              5 ;;; OPTIONS:	 peephole !omit_frame_pointer !signed-char
                              6 ;;; Source:		services\motors.c
                              7 ;;; Destination:	motors.s
                              8 ;;; Compiled:	Tue Oct 24 21:55:22 2000
                              9 ;;; (META)compiled by GNU C version 2.8.1.
                             10 ;;;-----------------------------------------
                             11 	.module motors.c
                             12 	.area	_DATA
   0000                      13 _speed_2_pwm:
   2741 00 00                14 	.word	0
   2743 80 80                15 	.word	-32640
   2745 88 88                16 	.word	-30584
   2747 AA AA                17 	.word	-21846
   2749 EE EE                18 	.word	-4370
   274B FE FE                19 	.word	-258
   274D FF FF                20 	.word	-1
   000E                      21 _current_motor_speed:
   274F 00                   22 	.byte	0
   2750 00                   23 	.byte	0
   2751 00                   24 	.byte	0
   2752 00                   25 	.byte	0
   0012                      26 _current_motor_dir:
   2753 00                   27 	.byte	0
   2754 00                   28 	.byte	0
   2755 00                   29 	.byte	0
   2756 00                   30 	.byte	0
                             31 	; extern	_motor_set_speed
                             32 	; extern	_set_interrupt
                             33 	.area	_CODE
                             34 	.globl	_motor_init
   0000                      35 _motor_init:
                             36 ;;;-----------------------------------------
                             37 ;;;  PROLOGUE for motor_init
                             38 ;;;-----------------------------------------
   183B 18 3C                39 	pshy	; Save stack frame
   183D 18 30                40 	tsy	; Set current stack frame
   183F DE 00                41 	ldx	*ZD5
   1841 3C                   42 	pshx	; pushed register *ZD5
                             43 ;;;END PROLOGUE
                             44 ; Begin inline assembler code
   1842 0F                   45 		sei
                             46 ; End of inline assembler code
   1843 C6 00                47 	ldab	#0
   1845 F7 7F FF             48 	stab	32767	; movqi: #0 -> 32767
   1848 C6 00                49 	ldab	#0
   184A 37                   50 	pshb		; movqi: #0 -> ,-sp
   184B C6 00                51 	ldab	#0
   184D D7 01                52 	stab	*ZD0+1	; movqi: #0 -> *ZD0
   184F BD 19 23             53 	jsr	_motor_set_speed	; CALL: (VOIDmode) _motor_set_speed (1 bytes)
   1852 C6 00                54 	ldab	#0
   1854 37                   55 	pshb		; movqi: #0 -> ,-sp
   1855 C6 01                56 	ldab	#1
   1857 D7 01                57 	stab	*ZD0+1	; movqi: #1 -> *ZD0
   1859 BD 19 23             58 	jsr	_motor_set_speed	; CALL: (VOIDmode) _motor_set_speed (1 bytes)
   185C C6 00                59 	ldab	#0
   185E 37                   60 	pshb		; movqi: #0 -> ,-sp
   185F C6 02                61 	ldab	#2
   1861 D7 01                62 	stab	*ZD0+1	; movqi: #2 -> *ZD0
   1863 BD 19 23             63 	jsr	_motor_set_speed	; CALL: (VOIDmode) _motor_set_speed (1 bytes)
   1866 C6 00                64 	ldab	#0
   1868 37                   65 	pshb		; movqi: #0 -> ,-sp
   1869 C6 03                66 	ldab	#3
   186B D7 01                67 	stab	*ZD0+1	; movqi: #3 -> *ZD0
   186D BD 19 23             68 	jsr	_motor_set_speed	; CALL: (VOIDmode) _motor_set_speed (1 bytes)
   1870 BD 19 C9             69 	jsr	_apply_motor_pwm	; CALL: (VOIDmode) _apply_motor_pwm (0 bytes)
   1873 CE 19 99             70 	ldx	#_motor_isr
   1876 3C                   71 	pshx		; movhi: #_motor_isr -> ,--sp
   1877 CC 00 07             72 	ldd	#7
   187A DD 00                73 	std	*ZD0	; movhi: #7 -> *ZD0
   187C BD 08 51             74 	jsr	_set_interrupt	; CALL: R:*ZD0 = _set_interrupt (2 bytes)
   187F FC 10 0E             75 	ldd	4110
   1882 DD 00                76 	std	*ZD5	; movhi: 4110 -> *ZD5
   1884 DC 00                77 	ldd	*ZD5
   1886 C3 4E 21             78 	addd	#20001
   1889 DD 00                79 	std	*ZD5	; addhi3: *ZD5 by #20001 -> *ZD5
   188B DC 00                80 	ldd	*ZD5
   188D FD 10 1A             81 	std	4122	; movhi: *ZD5 -> 4122
   1890 F6 10 22             82 	ldab	4130
   1893 D7 01                83 	stab	*ZD5+1	; movqi: 4130 -> *ZD5
   1895 D6 01                84 	ldab	*ZD5+1
   1897 CA 20                85 	orab	#32
   1899 D7 01                86 	stab	*ZD5+1	; iorqi3: *ZD5 by #32 -> *ZD5
   189B D6 01                87 	ldab	*ZD5+1
   189D F7 10 22             88 	stab	4130	; movqi: *ZD5 -> 4130
   18A0 C6 20                89 	ldab	#32
   18A2 F7 10 23             90 	stab	4131	; movqi: #32 -> 4131
   18A5 F6 10 20             91 	ldab	4128
   18A8 D7 01                92 	stab	*ZD5+1	; movqi: 4128 -> *ZD5
   18AA D6 01                93 	ldab	*ZD5+1
   18AC C4 CF                94 	andb	#207
   18AE D7 01                95 	stab	*ZD5+1	; andqi3: *ZD5 by #207 -> *ZD5
   18B0 D6 01                96 	ldab	*ZD5+1
   18B2 F7 10 20             97 	stab	4128	; movqi: *ZD5 -> 4128
                             98 ; Begin inline assembler code
   18B5 0E                   99 		cli
                            100 ; End of inline assembler code
   18B6 38                  101 	pulx
   18B7 38                  102 	pulx
   18B8 38                  103 	pulx		; addhi3: sp by #6 -> sp
                            104 ;;;EPILOGUE
   18B9 38                  105 	pulx	; Pulling register *ZD5
   18BA DF 00               106 	stx	*ZD5
   18BC 18 38               107 	puly	; Restore stack frame
   18BE 39                  108 	rts	; return from function
                            109 ;;;-----------------------------------------
                            110 ;;; END EPILOGUE for motor_init
                            111 ;;;-----------------------------------------
                            112 	.globl	_motor_get_speed
   0084                     113 _motor_get_speed:
                            114 ;;;-----------------------------------------
                            115 ;;;  PROLOGUE for motor_get_speed
                            116 ;;;-----------------------------------------
   18BF 18 3C               117 	pshy	; Save stack frame
   18C1 18 30               118 	tsy	; Set current stack frame
   18C3 DE 00               119 	ldx	*ZD5
   18C5 3C                  120 	pshx	; pushed register *ZD5
   18C6 DE 00               121 	ldx	*ZD6
   18C8 3C                  122 	pshx	; pushed register *ZD6
   18C9 8F                  123 	xgdx	; saving D register
   18CA D6 00               124 	ldab	*ZB1
   18CC 37                  125 	pshb	; pushed register *ZB1
   18CD 8F                  126 	xgdx	; restoring D register
                            127 ;;;END PROLOGUE
   18CE D6 01               128 	ldab	*ZD0+1
   18D0 C1 03               129 	cmpb	#3	; cmpqi *ZD0 with #3
   18D2 22 03               130 	bhi	.+5
   18D4 7E 18 DF            131 	jmp	L3	; (bleu) long branch
   18D7 CC 00 00            132 	ldd	#0
   18DA DD 00               133 	std	*ZD0	; movhi: #0 -> *ZD0
   18DC 7E 19 15            134 	jmp	L5
   00A4                     135 L3:
   18DF D6 01               136 	ldab	*ZD0+1
   18E1 D7 00               137 	stab	*ZB1	; movqi: *ZD0 -> *ZB1
   18E3 D6 00               138 	ldab	*ZB1
   18E5 4F                  139 	clra
   18E6 DD 00               140 	std	*ZD5	; zero_extendqihi2: *ZB1 -> *ZD5
   18E8 E6 00               141 	ldab	0,x
   18EA D7 01               142 	stab	*ZD6+1	; movqi: 0,x -> *ZD6
   18EC DE 00               143 	ldx	*ZD5
   18EE 8F                  144 	xgdx
   18EF C3 27 53            145 	addd	#_current_motor_dir
   18F2 8F                  146 	xgdx
                            147 ;;; (output_swap_regx: addr computed)
   18F3 E6 00               148 	ldab	0,x
   18F5 C1 0F               149 	cmpb	#15	; cmpqi 0,x with #15
   18F7 26 03               150 	bne	.+5
   18F9 7E 19 03            151 	jmp	L4	; (beq) long branch
   18FC D6 01               152 	ldab	*ZD6+1
   18FE D7 00               153 	stab	*ZB1	; movqi: *ZD6 -> *ZB1
   1900 7E 19 0C            154 	jmp	L6
   00C8                     155 L4:
   1903 D6 01               156 	ldab	*ZD6+1
   1905 50                  157 	negb
   1906 D7 01               158 	stab	*ZD5+1	; negqi2: *ZD6 -> *ZD5
   1908 D6 01               159 	ldab	*ZD5+1
   190A D7 00               160 	stab	*ZB1	; movqi: *ZD5 -> *ZB1
   00D1                     161 L6:
   190C D6 00               162 	ldab	*ZB1
   190E 4F                  163 	clra
   190F 5D                  164 	tstb
   1910 2C 01               165 	bge	.+3
   1912 4A                  166 	deca
   1913 DD 00               167 	std	*ZD0	; extendqihi2: *ZB1 -> *ZD0
   00DA                     168 L5:
                            169 ;;;EPILOGUE
   1915 8F                  170 	xgdx	; saving D register
   1916 33                  171 	pulb	; Pulling register *ZB1
   1917 D7 00               172 	stab	*ZB1
   1919 8F                  173 	xgdx	; restoring D register
   191A 38                  174 	pulx	; Pulling register *ZD6
   191B DF 00               175 	stx	*ZD6
   191D 38                  176 	pulx	; Pulling register *ZD5
   191E DF 00               177 	stx	*ZD5
   1920 18 38               178 	puly	; Restore stack frame
   1922 39                  179 	rts	; return from function
                            180 ;;;-----------------------------------------
                            181 ;;; END EPILOGUE for motor_get_speed
                            182 ;;;-----------------------------------------
                            183 	.globl	_motor_set_speed
   00E8                     184 _motor_set_speed:
                            185 ;;;-----------------------------------------
                            186 ;;;  PROLOGUE for motor_set_speed
                            187 ;;;-----------------------------------------
   1923 18 3C               188 	pshy	; Save stack frame
   1925 18 30               189 	tsy	; Set current stack frame
   1927 DE 00               190 	ldx	*ZD5
   1929 3C                  191 	pshx	; pushed register *ZD5
   192A DE 00               192 	ldx	*ZD6
   192C 3C                  193 	pshx	; pushed register *ZD6
   192D DE 00               194 	ldx	*ZD7
   192F 3C                  195 	pshx	; pushed register *ZD7
   1930 8F                  196 	xgdx	; saving D register
   1931 D6 00               197 	ldab	*ZB1
   1933 37                  198 	pshb	; pushed register *ZB1
   1934 8F                  199 	xgdx	; restoring D register
                            200 ;;;END PROLOGUE
   1935 D6 01               201 	ldab	*ZD0+1
   1937 D7 01               202 	stab	*ZD7+1	; movqi: *ZD0 -> *ZD7
   1939 18 E6 04            203 	ldab	4,y
   193C D7 01               204 	stab	*ZD6+1	; movqi: 4,y -> *ZD6
   193E D6 01               205 	ldab	*ZD0+1
   1940 C1 03               206 	cmpb	#3	; cmpqi *ZD0 with #3
   1942 23 03               207 	bls	.+5
   1944 7E 19 85            208 	jmp	L8	; (bgtu) long branch
   1947 D6 01               209 	ldab	*ZD0+1
   1949 D7 00               210 	stab	*ZB1	; movqi: *ZD0 -> *ZB1
   194B D6 00               211 	ldab	*ZB1
   194D 4F                  212 	clra
   194E DD 00               213 	std	*ZD5	; zero_extendqihi2: *ZB1 -> *ZD5
   1950 C6 00               214 	ldab	#0
   1952 E7 00               215 	stab	0,x	; movqi: #0 -> 0,x
   1954 D6 01               216 	ldab	*ZD6+1	; tstqi: R:*ZD6
   1956 2C 03               217 	bge	.+5
   1958 7E 19 62            218 	jmp	L9	; (blt) long branch
   195B C6 00               219 	ldab	#0
   195D E7 00               220 	stab	0,x	; movqi: #0 -> 0,x
   195F 7E 19 6B            221 	jmp	L10
   0127                     222 L9:
   1962 D6 01               223 	ldab	*ZD6+1
   1964 50                  224 	negb
   1965 D7 01               225 	stab	*ZD6+1	; negqi2: *ZD6 -> *ZD6
   1967 C6 0F               226 	ldab	#15
   1969 E7 00               227 	stab	0,x	; movqi: #15 -> 0,x
   0130                     228 L10:
   196B D6 01               229 	ldab	*ZD6+1
   196D C1 06               230 	cmpb	#6	; cmpqi *ZD6 with #6
   196F 2E 03               231 	bgt	.+5
   1971 7E 19 78            232 	jmp	L11	; (ble) long branch
   1974 C6 06               233 	ldab	#6
   1976 D7 01               234 	stab	*ZD6+1	; movqi: #6 -> *ZD6
   013D                     235 L11:
   1978 D6 01               236 	ldab	*ZD7+1
   197A D7 00               237 	stab	*ZB1	; movqi: *ZD7 -> *ZB1
   197C D6 00               238 	ldab	*ZB1
   197E 4F                  239 	clra
   197F DD 00               240 	std	*ZD5	; zero_extendqihi2: *ZB1 -> *ZD5
   1981 D6 01               241 	ldab	*ZD6+1
   1983 E7 00               242 	stab	0,x	; movqi: *ZD6 -> 0,x
   014A                     243 L8:
   1985 BD 19 C9            244 	jsr	_apply_motor_pwm	; CALL: (VOIDmode) _apply_motor_pwm (0 bytes)
                            245 ;;;EPILOGUE
   1988 8F                  246 	xgdx	; saving D register
   1989 33                  247 	pulb	; Pulling register *ZB1
   198A D7 00               248 	stab	*ZB1
   198C 8F                  249 	xgdx	; restoring D register
   198D 38                  250 	pulx	; Pulling register *ZD7
   198E DF 00               251 	stx	*ZD7
   1990 38                  252 	pulx	; Pulling register *ZD6
   1991 DF 00               253 	stx	*ZD6
   1993 38                  254 	pulx	; Pulling register *ZD5
   1994 DF 00               255 	stx	*ZD5
   1996 18 38               256 	puly	; Restore stack frame
   1998 39                  257 	rts	; return from function
                            258 ;;;-----------------------------------------
                            259 ;;; END EPILOGUE for motor_set_speed
                            260 ;;;-----------------------------------------
   015E                     261 _motor_isr:
                            262 ;;;-----------------------------------------
                            263 ;;;  PROLOGUE for motor_isr
                            264 ;;;-----------------------------------------
   1999 18 3C               265 	pshy	; Save stack frame
   199B 18 30               266 	tsy	; Set current stack frame
                            267 ;;;END PROLOGUE
   199D BD 19 C9            268 	jsr	_apply_motor_pwm	; CALL: (VOIDmode) _apply_motor_pwm (0 bytes)
   19A0 BD 19 A6            269 	jsr	_restart_motor_timer	; CALL: (VOIDmode) _restart_motor_timer (0 bytes)
                            270 ;;;EPILOGUE
   19A3 18 38               271 	puly	; Restore stack frame
   19A5 39                  272 	rts	; return from function
                            273 ;;;-----------------------------------------
                            274 ;;; END EPILOGUE for motor_isr
                            275 ;;;-----------------------------------------
   016B                     276 _restart_motor_timer:
                            277 ;;;-----------------------------------------
                            278 ;;;  PROLOGUE for restart_motor_timer
                            279 ;;;-----------------------------------------
   19A6 18 3C               280 	pshy	; Save stack frame
   19A8 18 30               281 	tsy	; Set current stack frame
   19AA DE 00               282 	ldx	*ZD5
   19AC 3C                  283 	pshx	; pushed register *ZD5
                            284 ;;;END PROLOGUE
   19AD FC 10 1A            285 	ldd	4122
   19B0 DD 00               286 	std	*ZD5	; movhi: 4122 -> *ZD5
   19B2 DC 00               287 	ldd	*ZD5
   19B4 C3 4E 21            288 	addd	#20001
   19B7 DD 00               289 	std	*ZD5	; addhi3: *ZD5 by #20001 -> *ZD5
   19B9 DC 00               290 	ldd	*ZD5
   19BB FD 10 1A            291 	std	4122	; movhi: *ZD5 -> 4122
   19BE C6 20               292 	ldab	#32
   19C0 F7 10 23            293 	stab	4131	; movqi: #32 -> 4131
                            294 ;;;EPILOGUE
   19C3 38                  295 	pulx	; Pulling register *ZD5
   19C4 DF 00               296 	stx	*ZD5
   19C6 18 38               297 	puly	; Restore stack frame
   19C8 39                  298 	rts	; return from function
                            299 ;;;-----------------------------------------
                            300 ;;; END EPILOGUE for restart_motor_timer
                            301 ;;;-----------------------------------------
                            302 	.area	_DATA
   0016                     303 _speed_mask.12:
   2757 00 01               304 	.word	1
                            305 	.area	_CODE
   018E                     306 _apply_motor_pwm:
                            307 ;;;-----------------------------------------
                            308 ;;;  PROLOGUE for apply_motor_pwm
                            309 ;;;-----------------------------------------
   19C9 18 3C               310 	pshy	; Save stack frame
   19CB 18 30               311 	tsy	; Set current stack frame
   19CD DE 00               312 	ldx	*ZD1
   19CF 3C                  313 	pshx	; pushed register *ZD1
   19D0 DE 00               314 	ldx	*ZD5
   19D2 3C                  315 	pshx	; pushed register *ZD5
   19D3 DE 00               316 	ldx	*ZD6
   19D5 3C                  317 	pshx	; pushed register *ZD6
   19D6 DE 00               318 	ldx	*ZD7
   19D8 3C                  319 	pshx	; pushed register *ZD7
   19D9 DE 00               320 	ldx	*ZD8
   19DB 3C                  321 	pshx	; pushed register *ZD8
   19DC 8F                  322 	xgdx	; saving D register
   19DD D6 00               323 	ldab	*ZB1
   19DF 37                  324 	pshb	; pushed register *ZB1
   19E0 8F                  325 	xgdx	; restoring D register
                            326 ;;;END PROLOGUE
   19E1 C6 00               327 	ldab	#0
   19E3 D7 01               328 	stab	*ZD8+1	; movqi: #0 -> *ZD8
   19E5 C6 11               329 	ldab	#17
   19E7 D7 01               330 	stab	*ZD7+1	; movqi: #17 -> *ZD7
   19E9 D6 01               331 	ldab	*ZD8+1
   19EB D7 00               332 	stab	*ZB1	; movqi: *ZD8 -> *ZB1
   19ED FC 27 57            333 	ldd	_speed_mask.12
   19F0 DD 00               334 	std	*ZD1	; movhi: _speed_mask.12 -> *ZD1
   01B7                     335 L18:
   19F2 D6 00               336 	ldab	*ZB1
   19F4 4F                  337 	clra
   19F5 DD 00               338 	std	*ZD6	; zero_extendqihi2: *ZB1 -> *ZD6
   19F7 E6 00               339 	ldab	0,x
   19F9 4F                  340 	clra
   19FA DD 00               341 	std	*ZD5	; zero_extendqihi2: 0,x -> *ZD5
   19FC DC 00               342 	ldd	*ZD5
   19FE 05                  343 	asld
   19FF DD 00               344 	std	*ZD5	; ashlhi3: *ZD5 by #1 -> *ZD5
   1A01 DE 00               345 	ldx	*ZD5
   1A03 8F                  346 	xgdx
   1A04 C3 27 41            347 	addd	#_speed_2_pwm
   1A07 8F                  348 	xgdx
                            349 ;;; (output_swap_regx: addr computed)
   1A08 EC 00               350 	ldd	0,x
   1A0A 94 00               351 	anda	*ZD1
   1A0C D4 01               352 	andb	*ZD1+1
   1A0E DD 00               353 	std	*ZD5	; andhi3: 0,x by *ZD1 -> *ZD5
   1A10 26 03               354 	bne	.+5
   1A12 7E 1A 2E            355 	jmp	L19	; (beq) long branch
   1A15 DE 00               356 	ldx	*ZD6
   1A17 8F                  357 	xgdx
   1A18 C3 27 53            358 	addd	#_current_motor_dir
   1A1B 8F                  359 	xgdx
                            360 ;;; (output_swap_regx: addr computed)
   1A1C E6 00               361 	ldab	0,x
   1A1E CA F0               362 	orab	#240
   1A20 D7 01               363 	stab	*ZD5+1	; iorqi3: 0,x by #240 -> *ZD5
   1A22 D6 01               364 	ldab	*ZD5+1
   1A24 D4 01               365 	andb	*ZD7+1
   1A26 D7 01               366 	stab	*ZD5+1	; andqi3: *ZD5 by *ZD7 -> *ZD5
   1A28 D6 01               367 	ldab	*ZD8+1
   1A2A DA 01               368 	orab	*ZD5+1
   1A2C D7 01               369 	stab	*ZD8+1	; iorqi3: *ZD8 by *ZD5 -> *ZD8
   01F3                     370 L19:
   1A2E D6 01               371 	ldab	*ZD7+1
   1A30 58                  372 	aslb
   1A31 D7 01               373 	stab	*ZD7+1	; ashlqi3: *ZD7 by #1 -> *ZD7
   1A33 7C 00 00            374 	inc	*ZB1	; addqi3: *ZB1 by #1 -> *ZB1
   1A36 D6 00               375 	ldab	*ZB1
   1A38 C1 03               376 	cmpb	#3	; cmpqi *ZB1 with #3
   1A3A 22 03               377 	bhi	.+5
   1A3C 7E 19 F2            378 	jmp	L18	; (bleu) long branch
   1A3F FC 27 57            379 	ldd	_speed_mask.12
   1A42 05                  380 	asld
   1A43 DD 00               381 	std	*ZD5	; ashlhi3: _speed_mask.12 by #1 -> *ZD5
   1A45 DC 00               382 	ldd	*ZD5
   1A47 FD 27 57            383 	std	_speed_mask.12	; movhi: *ZD5 -> _speed_mask.12
   1A4A 27 03               384 	beq	.+5
   1A4C 7E 1A 55            385 	jmp	L21	; (bne) long branch
   1A4F CC 00 01            386 	ldd	#1
   1A52 FD 27 57            387 	std	_speed_mask.12	; movhi: #1 -> _speed_mask.12
   021A                     388 L21:
   1A55 D6 01               389 	ldab	*ZD8+1
   1A57 F7 7F FF            390 	stab	32767	; movqi: *ZD8 -> 32767
                            391 ;;;EPILOGUE
   1A5A 8F                  392 	xgdx	; saving D register
   1A5B 33                  393 	pulb	; Pulling register *ZB1
   1A5C D7 00               394 	stab	*ZB1
   1A5E 8F                  395 	xgdx	; restoring D register
   1A5F 38                  396 	pulx	; Pulling register *ZD8
   1A60 DF 00               397 	stx	*ZD8
   1A62 38                  398 	pulx	; Pulling register *ZD7
   1A63 DF 00               399 	stx	*ZD7
   1A65 38                  400 	pulx	; Pulling register *ZD6
   1A66 DF 00               401 	stx	*ZD6
   1A68 38                  402 	pulx	; Pulling register *ZD5
   1A69 DF 00               403 	stx	*ZD5
   1A6B 38                  404 	pulx	; Pulling register *ZD1
   1A6C DF 00               405 	stx	*ZD1
   1A6E 18 38               406 	puly	; Restore stack frame
   1A70 39                  407 	rts	; return from function
                            408 ;;;-----------------------------------------
                            409 ;;; END EPILOGUE for apply_motor_pwm
                            410 ;;;-----------------------------------------
                            411 	; END
