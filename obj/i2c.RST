                              1 ;;;-----------------------------------------
                              2 ;;; Start MC6811 gcc assembly output
                              3 ;;; gcc compiler compiled on TBD
                              4 ;;; OPTIONS:	-mlong_branch optimize  
                              5 ;;; OPTIONS:	 peephole !omit_frame_pointer !signed-char
                              6 ;;; Source:		services\i2c.c
                              7 ;;; Destination:	i2c.s
                              8 ;;; Compiled:	Tue Oct 24 21:55:17 2000
                              9 ;;; (META)compiled by GNU C version 2.8.1.
                             10 ;;;-----------------------------------------
                             11 	.module i2c.c
                             12 	.area	_CODE
                             13 	.globl	_presto_i2c_init
   0000                      14 _presto_i2c_init:
                             15 ;;;-----------------------------------------
                             16 ;;;  PROLOGUE for presto_i2c_init
                             17 ;;;-----------------------------------------
   1433 18 3C                18 	pshy	; Save stack frame
   1435 18 30                19 	tsy	; Set current stack frame
   1437 DE 00                20 	ldx	*ZD5
   1439 3C                   21 	pshx	; pushed register *ZD5
                             22 ;;;END PROLOGUE
   143A F6 10 09             23 	ldab	4105
   143D D7 01                24 	stab	*ZD5+1	; movqi: 4105 -> *ZD5
   143F D6 01                25 	ldab	*ZD5+1
   1441 CA 04                26 	orab	#4
   1443 D7 01                27 	stab	*ZD5+1	; iorqi3: *ZD5 by #4 -> *ZD5
   1445 D6 01                28 	ldab	*ZD5+1
   1447 F7 10 09             29 	stab	4105	; movqi: *ZD5 -> 4105
   144A F6 10 09             30 	ldab	4105
   144D D7 01                31 	stab	*ZD5+1	; movqi: 4105 -> *ZD5
   144F D6 01                32 	ldab	*ZD5+1
   1451 CA 20                33 	orab	#32
   1453 D7 01                34 	stab	*ZD5+1	; iorqi3: *ZD5 by #32 -> *ZD5
   1455 D6 01                35 	ldab	*ZD5+1
   1457 F7 10 09             36 	stab	4105	; movqi: *ZD5 -> 4105
   145A F6 10 08             37 	ldab	4104
   145D D7 01                38 	stab	*ZD5+1	; movqi: 4104 -> *ZD5
   145F D6 01                39 	ldab	*ZD5+1
   1461 C4 DF                40 	andb	#223
   1463 D7 01                41 	stab	*ZD5+1	; andqi3: *ZD5 by #223 -> *ZD5
   1465 D6 01                42 	ldab	*ZD5+1
   1467 F7 10 08             43 	stab	4104	; movqi: *ZD5 -> 4104
   146A F6 10 08             44 	ldab	4104
   146D D7 01                45 	stab	*ZD5+1	; movqi: 4104 -> *ZD5
   146F D6 01                46 	ldab	*ZD5+1
   1471 CA 04                47 	orab	#4
   1473 D7 01                48 	stab	*ZD5+1	; iorqi3: *ZD5 by #4 -> *ZD5
   1475 D6 01                49 	ldab	*ZD5+1
   1477 F7 10 08             50 	stab	4104	; movqi: *ZD5 -> 4104
   147A F6 10 08             51 	ldab	4104
   147D D7 01                52 	stab	*ZD5+1	; movqi: 4104 -> *ZD5
   147F D6 01                53 	ldab	*ZD5+1
   1481 CA 20                54 	orab	#32
   1483 D7 01                55 	stab	*ZD5+1	; iorqi3: *ZD5 by #32 -> *ZD5
   1485 D6 01                56 	ldab	*ZD5+1
   1487 F7 10 08             57 	stab	4104	; movqi: *ZD5 -> 4104
                             58 ;;;EPILOGUE
   148A 38                   59 	pulx	; Pulling register *ZD5
   148B DF 00                60 	stx	*ZD5
   148D 18 38                61 	puly	; Restore stack frame
   148F 39                   62 	rts	; return from function
                             63 ;;;-----------------------------------------
                             64 ;;; END EPILOGUE for presto_i2c_init
                             65 ;;;-----------------------------------------
                             66 	; extern	_presto_i2c_start
                             67 	; extern	_presto_i2c_send_byte
                             68 	; extern	_presto_i2c_stop
                             69 	.globl	_presto_i2c_send
   005D                      70 _presto_i2c_send:
                             71 ;;;-----------------------------------------
                             72 ;;;  PROLOGUE for presto_i2c_send
                             73 ;;;-----------------------------------------
   1490 18 3C                74 	pshy	; Save stack frame
   1492 18 30                75 	tsy	; Set current stack frame
   1494 DE 00                76 	ldx	*ZD5
   1496 3C                   77 	pshx	; pushed register *ZD5
   1497 DE 00                78 	ldx	*ZD6
   1499 3C                   79 	pshx	; pushed register *ZD6
   149A DE 00                80 	ldx	*ZD7
   149C 3C                   81 	pshx	; pushed register *ZD7
   149D 8F                   82 	xgdx	; saving D register
   149E D6 00                83 	ldab	*ZB1
   14A0 37                   84 	pshb	; pushed register *ZB1
   14A1 D6 00                85 	ldab	*ZB2
   14A3 37                   86 	pshb	; pushed register *ZB2
   14A4 8F                   87 	xgdx	; restoring D register
                             88 ;;;END PROLOGUE
   14A5 DC 00                89 	ldd	*ZD0
   14A7 DD 00                90 	std	*ZD7	; movhi: *ZD0 -> *ZD7
   14A9 18 E6 04             91 	ldab	4,y
   14AC D7 01                92 	stab	*ZD6+1	; movqi: 4,y -> *ZD6
   14AE C6 00                93 	ldab	#0
   14B0 D7 00                94 	stab	*ZB1	; movqi: #0 -> *ZB1
   14B2 BD 14 F3             95 	jsr	_presto_i2c_start	; CALL: (VOIDmode) _presto_i2c_start (0 bytes)
   14B5 7E 14 CF             96 	jmp	L7
   0085                      97 L5:
   14B8 D6 00                98 	ldab	*ZB1
   14BA 4F                   99 	clra
   14BB DD 00               100 	std	*ZD5	; zero_extendqihi2: *ZB1 -> *ZD5
   14BD DC 00               101 	ldd	*ZD7
   14BF D3 00               102 	addd	*ZD5
   14C1 DD 00               103 	std	*ZD5	; addhi3: *ZD7 by *ZD5 -> *ZD5
   14C3 7C 00 00            104 	inc	*ZB1	; addqi3: *ZB1 by #1 -> *ZB1
   14C6 DE 00               105 	ldx	*ZD5
   14C8 E6 00               106 	ldab	0,x
   14CA D7 01               107 	stab	*ZD0+1	; movqi: 0,x -> *ZD0
   14CC BD 15 20            108 	jsr	_presto_i2c_send_byte	; CALL: (VOIDmode) _presto_i2c_send_byte (0 bytes)
   009C                     109 L7:
   14CF D6 01               110 	ldab	*ZD6+1
   14D1 D7 00               111 	stab	*ZB2	; movqi: *ZD6 -> *ZB2
   14D3 D6 00               112 	ldab	*ZB1
   14D5 D1 00               113 	cmpb	*ZB2	; cmpqi *ZB1 with *ZB2
   14D7 24 03               114 	bhs	.+5
   14D9 7E 14 B8            115 	jmp	L5	; (bltu) long branch
   14DC BD 15 D4            116 	jsr	_presto_i2c_stop	; CALL: (VOIDmode) _presto_i2c_stop (0 bytes)
                            117 ;;;EPILOGUE
   14DF 8F                  118 	xgdx	; saving D register
   14E0 33                  119 	pulb	; Pulling register *ZB2
   14E1 D7 00               120 	stab	*ZB2
   14E3 33                  121 	pulb	; Pulling register *ZB1
   14E4 D7 00               122 	stab	*ZB1
   14E6 8F                  123 	xgdx	; restoring D register
   14E7 38                  124 	pulx	; Pulling register *ZD7
   14E8 DF 00               125 	stx	*ZD7
   14EA 38                  126 	pulx	; Pulling register *ZD6
   14EB DF 00               127 	stx	*ZD6
   14ED 38                  128 	pulx	; Pulling register *ZD5
   14EE DF 00               129 	stx	*ZD5
   14F0 18 38               130 	puly	; Restore stack frame
   14F2 39                  131 	rts	; return from function
                            132 ;;;-----------------------------------------
                            133 ;;; END EPILOGUE for presto_i2c_send
                            134 ;;;-----------------------------------------
                            135 	.globl	_presto_i2c_start
   00C0                     136 _presto_i2c_start:
                            137 ;;;-----------------------------------------
                            138 ;;;  PROLOGUE for presto_i2c_start
                            139 ;;;-----------------------------------------
   14F3 18 3C               140 	pshy	; Save stack frame
   14F5 18 30               141 	tsy	; Set current stack frame
   14F7 DE 00               142 	ldx	*ZD5
   14F9 3C                  143 	pshx	; pushed register *ZD5
                            144 ;;;END PROLOGUE
   14FA F6 10 08            145 	ldab	4104
   14FD D7 01               146 	stab	*ZD5+1	; movqi: 4104 -> *ZD5
   14FF D6 01               147 	ldab	*ZD5+1
   1501 C4 DF               148 	andb	#223
   1503 D7 01               149 	stab	*ZD5+1	; andqi3: *ZD5 by #223 -> *ZD5
   1505 D6 01               150 	ldab	*ZD5+1
   1507 F7 10 08            151 	stab	4104	; movqi: *ZD5 -> 4104
   150A F6 10 08            152 	ldab	4104
   150D D7 01               153 	stab	*ZD5+1	; movqi: 4104 -> *ZD5
   150F D6 01               154 	ldab	*ZD5+1
   1511 C4 FB               155 	andb	#251
   1513 D7 01               156 	stab	*ZD5+1	; andqi3: *ZD5 by #251 -> *ZD5
   1515 D6 01               157 	ldab	*ZD5+1
   1517 F7 10 08            158 	stab	4104	; movqi: *ZD5 -> 4104
                            159 ;;;EPILOGUE
   151A 38                  160 	pulx	; Pulling register *ZD5
   151B DF 00               161 	stx	*ZD5
   151D 18 38               162 	puly	; Restore stack frame
   151F 39                  163 	rts	; return from function
                            164 ;;;-----------------------------------------
                            165 ;;; END EPILOGUE for presto_i2c_start
                            166 ;;;-----------------------------------------
                            167 	.globl	_presto_i2c_send_byte
   00ED                     168 _presto_i2c_send_byte:
                            169 ;;;-----------------------------------------
                            170 ;;;  PROLOGUE for presto_i2c_send_byte
                            171 ;;;-----------------------------------------
   1520 18 3C               172 	pshy	; Save stack frame
   1522 18 30               173 	tsy	; Set current stack frame
   1524 DE 00               174 	ldx	*ZD5
   1526 3C                  175 	pshx	; pushed register *ZD5
   1527 DE 00               176 	ldx	*ZD6
   1529 3C                  177 	pshx	; pushed register *ZD6
   152A DE 00               178 	ldx	*ZD7
   152C 3C                  179 	pshx	; pushed register *ZD7
                            180 ;;;END PROLOGUE
   152D D6 01               181 	ldab	*ZD0+1
   152F D7 01               182 	stab	*ZD7+1	; movqi: *ZD0 -> *ZD7
   1531 C6 80               183 	ldab	#128
   1533 D7 01               184 	stab	*ZD6+1	; movqi: #128 -> *ZD6
   0102                     185 L13:
   1535 D6 01               186 	ldab	*ZD7+1
   1537 D4 01               187 	andb	*ZD6+1
   1539 D7 01               188 	stab	*ZD5+1	; andqi3: *ZD7 by *ZD6 -> *ZD5
   153B 26 03               189 	bne	.+5
   153D 7E 15 4E            190 	jmp	L14	; (beq) long branch
   1540 F6 10 08            191 	ldab	4104
   1543 D7 01               192 	stab	*ZD5+1	; movqi: 4104 -> *ZD5
   1545 D6 01               193 	ldab	*ZD5+1
   1547 CA 20               194 	orab	#32
   1549 D7 01               195 	stab	*ZD5+1	; iorqi3: *ZD5 by #32 -> *ZD5
   154B 7E 15 59            196 	jmp	L17
   011B                     197 L14:
   154E F6 10 08            198 	ldab	4104
   1551 D7 01               199 	stab	*ZD5+1	; movqi: 4104 -> *ZD5
   1553 D6 01               200 	ldab	*ZD5+1
   1555 C4 DF               201 	andb	#223
   1557 D7 01               202 	stab	*ZD5+1	; andqi3: *ZD5 by #223 -> *ZD5
   0126                     203 L17:
   1559 D6 01               204 	ldab	*ZD5+1
   155B F7 10 08            205 	stab	4104	; movqi: *ZD5 -> 4104
   155E F6 10 08            206 	ldab	4104
   1561 D7 01               207 	stab	*ZD5+1	; movqi: 4104 -> *ZD5
   1563 D6 01               208 	ldab	*ZD5+1
   1565 CA 04               209 	orab	#4
   1567 D7 01               210 	stab	*ZD5+1	; iorqi3: *ZD5 by #4 -> *ZD5
   1569 D6 01               211 	ldab	*ZD5+1
   156B F7 10 08            212 	stab	4104	; movqi: *ZD5 -> 4104
   156E F6 10 08            213 	ldab	4104
   1571 D7 01               214 	stab	*ZD5+1	; movqi: 4104 -> *ZD5
   1573 D6 01               215 	ldab	*ZD5+1
   1575 C4 FB               216 	andb	#251
   1577 D7 01               217 	stab	*ZD5+1	; andqi3: *ZD5 by #251 -> *ZD5
   1579 D6 01               218 	ldab	*ZD5+1
   157B F7 10 08            219 	stab	4104	; movqi: *ZD5 -> 4104
   157E D6 01               220 	ldab	*ZD6+1
   1580 54                  221 	lsrb
   1581 D7 01               222 	stab	*ZD6+1	; lshrqi3: *ZD6 by #1 -> *ZD6
   1583 27 03               223 	beq	.+5
   1585 7E 15 35            224 	jmp	L13	; (bne) long branch
   1588 F6 10 09            225 	ldab	4105
   158B D7 01               226 	stab	*ZD5+1	; movqi: 4105 -> *ZD5
   158D D6 01               227 	ldab	*ZD5+1
   158F C4 DF               228 	andb	#223
   1591 D7 01               229 	stab	*ZD5+1	; andqi3: *ZD5 by #223 -> *ZD5
   1593 D6 01               230 	ldab	*ZD5+1
   1595 F7 10 09            231 	stab	4105	; movqi: *ZD5 -> 4105
   1598 F6 10 08            232 	ldab	4104
   159B D7 01               233 	stab	*ZD5+1	; movqi: 4104 -> *ZD5
   159D D6 01               234 	ldab	*ZD5+1
   159F CA 04               235 	orab	#4
   15A1 D7 01               236 	stab	*ZD5+1	; iorqi3: *ZD5 by #4 -> *ZD5
   15A3 D6 01               237 	ldab	*ZD5+1
   15A5 F7 10 08            238 	stab	4104	; movqi: *ZD5 -> 4104
   15A8 F6 10 08            239 	ldab	4104
   15AB D7 01               240 	stab	*ZD5+1	; movqi: 4104 -> *ZD5
   15AD D6 01               241 	ldab	*ZD5+1
   15AF C4 FB               242 	andb	#251
   15B1 D7 01               243 	stab	*ZD5+1	; andqi3: *ZD5 by #251 -> *ZD5
   15B3 D6 01               244 	ldab	*ZD5+1
   15B5 F7 10 08            245 	stab	4104	; movqi: *ZD5 -> 4104
   15B8 F6 10 09            246 	ldab	4105
   15BB D7 01               247 	stab	*ZD5+1	; movqi: 4105 -> *ZD5
   15BD D6 01               248 	ldab	*ZD5+1
   15BF CA 20               249 	orab	#32
   15C1 D7 01               250 	stab	*ZD5+1	; iorqi3: *ZD5 by #32 -> *ZD5
   15C3 D6 01               251 	ldab	*ZD5+1
   15C5 F7 10 09            252 	stab	4105	; movqi: *ZD5 -> 4105
                            253 ;;;EPILOGUE
   15C8 38                  254 	pulx	; Pulling register *ZD7
   15C9 DF 00               255 	stx	*ZD7
   15CB 38                  256 	pulx	; Pulling register *ZD6
   15CC DF 00               257 	stx	*ZD6
   15CE 38                  258 	pulx	; Pulling register *ZD5
   15CF DF 00               259 	stx	*ZD5
   15D1 18 38               260 	puly	; Restore stack frame
   15D3 39                  261 	rts	; return from function
                            262 ;;;-----------------------------------------
                            263 ;;; END EPILOGUE for presto_i2c_send_byte
                            264 ;;;-----------------------------------------
                            265 	.globl	_presto_i2c_stop
   01A1                     266 _presto_i2c_stop:
                            267 ;;;-----------------------------------------
                            268 ;;;  PROLOGUE for presto_i2c_stop
                            269 ;;;-----------------------------------------
   15D4 18 3C               270 	pshy	; Save stack frame
   15D6 18 30               271 	tsy	; Set current stack frame
   15D8 DE 00               272 	ldx	*ZD5
   15DA 3C                  273 	pshx	; pushed register *ZD5
                            274 ;;;END PROLOGUE
   15DB F6 10 08            275 	ldab	4104
   15DE D7 01               276 	stab	*ZD5+1	; movqi: 4104 -> *ZD5
   15E0 D6 01               277 	ldab	*ZD5+1
   15E2 C4 DF               278 	andb	#223
   15E4 D7 01               279 	stab	*ZD5+1	; andqi3: *ZD5 by #223 -> *ZD5
   15E6 D6 01               280 	ldab	*ZD5+1
   15E8 F7 10 08            281 	stab	4104	; movqi: *ZD5 -> 4104
   15EB F6 10 08            282 	ldab	4104
   15EE D7 01               283 	stab	*ZD5+1	; movqi: 4104 -> *ZD5
   15F0 D6 01               284 	ldab	*ZD5+1
   15F2 CA 04               285 	orab	#4
   15F4 D7 01               286 	stab	*ZD5+1	; iorqi3: *ZD5 by #4 -> *ZD5
   15F6 D6 01               287 	ldab	*ZD5+1
   15F8 F7 10 08            288 	stab	4104	; movqi: *ZD5 -> 4104
   15FB F6 10 08            289 	ldab	4104
   15FE D7 01               290 	stab	*ZD5+1	; movqi: 4104 -> *ZD5
   1600 D6 01               291 	ldab	*ZD5+1
   1602 CA 20               292 	orab	#32
   1604 D7 01               293 	stab	*ZD5+1	; iorqi3: *ZD5 by #32 -> *ZD5
   1606 D6 01               294 	ldab	*ZD5+1
   1608 F7 10 08            295 	stab	4104	; movqi: *ZD5 -> 4104
                            296 ;;;EPILOGUE
   160B 38                  297 	pulx	; Pulling register *ZD5
   160C DF 00               298 	stx	*ZD5
   160E 18 38               299 	puly	; Restore stack frame
   1610 39                  300 	rts	; return from function
                            301 ;;;-----------------------------------------
                            302 ;;; END EPILOGUE for presto_i2c_stop
                            303 ;;;-----------------------------------------
                            304 	; END
