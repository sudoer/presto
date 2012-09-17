                              1 ;;;-----------------------------------------
                              2 ;;; Start MC6811 gcc assembly output
                              3 ;;; gcc compiler compiled on TBD
                              4 ;;; OPTIONS:	-mlong_branch optimize  
                              5 ;;; OPTIONS:	 peephole !omit_frame_pointer !signed-char
                              6 ;;; Source:		utils\string.c
                              7 ;;; Destination:	string.s
                              8 ;;; Compiled:	Tue Oct 24 21:55:31 2000
                              9 ;;; (META)compiled by GNU C version 2.8.1.
                             10 ;;;-----------------------------------------
                             11 	.module string.c
                             12 	.area	_CODE
                             13 	.globl	_string_IsSpace
   0000                      14 _string_IsSpace:
                             15 ;;;-----------------------------------------
                             16 ;;;  PROLOGUE for string_IsSpace
                             17 ;;;-----------------------------------------
   1EFB 18 3C                18 	pshy	; Save stack frame
   1EFD 18 30                19 	tsy	; Set current stack frame
   1EFF DE 00                20 	ldx	*ZD5
   1F01 3C                   21 	pshx	; pushed register *ZD5
   1F02 8F                   22 	xgdx	; saving D register
   1F03 D6 00                23 	ldab	*ZB1
   1F05 37                   24 	pshb	; pushed register *ZB1
   1F06 8F                   25 	xgdx	; restoring D register
                             26 ;;;END PROLOGUE
   1F07 D6 01                27 	ldab	*ZD0+1
   1F09 C8 20                28 	eorb	#32
   1F0B D7 01                29 	stab	*ZD5+1	; xorqi3: *ZD0 by #32 -> *ZD5
   1F0D D6 01                30 	ldab	*ZD5+1
   1F0F D7 00                31 	stab	*ZB1	; movqi: *ZD5 -> *ZB1
   1F11 D6 00                32 	ldab	*ZB1
   1F13 4F                   33 	clra
   1F14 DD 00                34 	std	*ZD5	; zero_extendqihi2: *ZB1 -> *ZD5
   1F16 DC 00                35 	ldd	*ZD5
   1F18 C3 FF FF             36 	addd	#-1
   1F1B DD 00                37 	std	*ZD5	; addhi3: *ZD5 by #-1 -> *ZD5
   1F1D DC 00                38 	ldd	*ZD5
   1F1F CE 00 0F             39 	ldx	#15
   1F22 2F 04                40 	ble	1$
   1F24 04                   41 	lsrd
   1F25 09                   42 	dex
   1F26 26 FC                43 	bne	.-2
                             44 1$:
   1F28 DD 00                45 	std	*ZD0	; lshrhi3: *ZD5 by #15 -> *ZD0
                             46 ;;;EPILOGUE
   1F2A 8F                   47 	xgdx	; saving D register
   1F2B 33                   48 	pulb	; Pulling register *ZB1
   1F2C D7 00                49 	stab	*ZB1
   1F2E 8F                   50 	xgdx	; restoring D register
   1F2F 38                   51 	pulx	; Pulling register *ZD5
   1F30 DF 00                52 	stx	*ZD5
   1F32 18 38                53 	puly	; Restore stack frame
   1F34 39                   54 	rts	; return from function
                             55 ;;;-----------------------------------------
                             56 ;;; END EPILOGUE for string_IsSpace
                             57 ;;;-----------------------------------------
                             58 	.globl	_string_IsNumber
   003A                      59 _string_IsNumber:
                             60 ;;;-----------------------------------------
                             61 ;;;  PROLOGUE for string_IsNumber
                             62 ;;;-----------------------------------------
   1F35 18 3C                63 	pshy	; Save stack frame
   1F37 18 30                64 	tsy	; Set current stack frame
   1F39 DE 00                65 	ldx	*ZD5
   1F3B 3C                   66 	pshx	; pushed register *ZD5
                             67 ;;;END PROLOGUE
   1F3C D6 01                68 	ldab	*ZD0+1
   1F3E CB D0                69 	addb	#-48
   1F40 D7 01                70 	stab	*ZD5+1	; addqi3: *ZD0 by #-48 -> *ZD5
   1F42 D6 01                71 	ldab	*ZD5+1
   1F44 C1 09                72 	cmpb	#9	; cmpqi *ZD5 with #9
   1F46 22 03                73 	bhi	.+5
   1F48 7E 1F 5D             74 	jmp	L7	; (bleu) long branch
   1F4B D6 01                75 	ldab	*ZD0+1
   1F4D C1 2D                76 	cmpb	#45	; cmpqi *ZD0 with #45
   1F4F 26 03                77 	bne	.+5
   1F51 7E 1F 5D             78 	jmp	L7	; (beq) long branch
   1F54 D6 01                79 	ldab	*ZD0+1
   1F56 C1 2B                80 	cmpb	#43	; cmpqi *ZD0 with #43
   1F58 27 03                81 	beq	.+5
   1F5A 7E 1F 65             82 	jmp	L6	; (bne) long branch
   0062                      83 L7:
   1F5D CC 00 01             84 	ldd	#1
   1F60 DD 00                85 	std	*ZD0	; movhi: #1 -> *ZD0
   1F62 7E 1F 6A             86 	jmp	L8
   006A                      87 L6:
   1F65 CC 00 00             88 	ldd	#0
   1F68 DD 00                89 	std	*ZD0	; movhi: #0 -> *ZD0
   006F                      90 L8:
                             91 ;;;EPILOGUE
   1F6A 38                   92 	pulx	; Pulling register *ZD5
   1F6B DF 00                93 	stx	*ZD5
   1F6D 18 38                94 	puly	; Restore stack frame
   1F6F 39                   95 	rts	; return from function
                             96 ;;;-----------------------------------------
                             97 ;;; END EPILOGUE for string_IsNumber
                             98 ;;;-----------------------------------------
                             99 	.globl	_string_IsDigit
   0075                     100 _string_IsDigit:
                            101 ;;;-----------------------------------------
                            102 ;;;  PROLOGUE for string_IsDigit
                            103 ;;;-----------------------------------------
   1F70 18 3C               104 	pshy	; Save stack frame
   1F72 18 30               105 	tsy	; Set current stack frame
   1F74 DE 00               106 	ldx	*ZD5
   1F76 3C                  107 	pshx	; pushed register *ZD5
                            108 ;;;END PROLOGUE
   1F77 D6 01               109 	ldab	*ZD0+1
   1F79 CB D0               110 	addb	#-48
   1F7B D7 01               111 	stab	*ZD5+1	; addqi3: *ZD0 by #-48 -> *ZD5
   1F7D CC 00 01            112 	ldd	#1
   1F80 DD 00               113 	std	*ZD0	; movhi: #1 -> *ZD0
   1F82 D6 01               114 	ldab	*ZD5+1
   1F84 C1 09               115 	cmpb	#9	; cmpqi *ZD5 with #9
   1F86 22 03               116 	bhi	.+5
   1F88 7E 1F 90            117 	jmp	L11	; (bleu) long branch
   1F8B CC 00 00            118 	ldd	#0
   1F8E DD 00               119 	std	*ZD0	; movhi: #0 -> *ZD0
   0095                     120 L11:
                            121 ;;;EPILOGUE
   1F90 38                  122 	pulx	; Pulling register *ZD5
   1F91 DF 00               123 	stx	*ZD5
   1F93 18 38               124 	puly	; Restore stack frame
   1F95 39                  125 	rts	; return from function
                            126 ;;;-----------------------------------------
                            127 ;;; END EPILOGUE for string_IsDigit
                            128 ;;;-----------------------------------------
                            129 	.globl	_string_IsHexDigit
   009B                     130 _string_IsHexDigit:
                            131 ;;;-----------------------------------------
                            132 ;;;  PROLOGUE for string_IsHexDigit
                            133 ;;;-----------------------------------------
   1F96 18 3C               134 	pshy	; Save stack frame
   1F98 18 30               135 	tsy	; Set current stack frame
   1F9A DE 00               136 	ldx	*ZD5
   1F9C 3C                  137 	pshx	; pushed register *ZD5
                            138 ;;;END PROLOGUE
   1F9D D6 01               139 	ldab	*ZD0+1
   1F9F CB D0               140 	addb	#-48
   1FA1 D7 01               141 	stab	*ZD5+1	; addqi3: *ZD0 by #-48 -> *ZD5
   1FA3 D6 01               142 	ldab	*ZD5+1
   1FA5 C1 09               143 	cmpb	#9	; cmpqi *ZD5 with #9
   1FA7 22 03               144 	bhi	.+5
   1FA9 7E 1F D7            145 	jmp	L14	; (bleu) long branch
   1FAC D6 01               146 	ldab	*ZD0+1
   1FAE CB BF               147 	addb	#-65
   1FB0 D7 01               148 	stab	*ZD5+1	; addqi3: *ZD0 by #-65 -> *ZD5
   1FB2 D6 01               149 	ldab	*ZD5+1
   1FB4 C1 05               150 	cmpb	#5	; cmpqi *ZD5 with #5
   1FB6 22 03               151 	bhi	.+5
   1FB8 7E 1F D7            152 	jmp	L14	; (bleu) long branch
   1FBB D6 01               153 	ldab	*ZD0+1
   1FBD CB 9F               154 	addb	#-97
   1FBF D7 01               155 	stab	*ZD5+1	; addqi3: *ZD0 by #-97 -> *ZD5
   1FC1 CC 00 01            156 	ldd	#1
   1FC4 DD 00               157 	std	*ZD0	; movhi: #1 -> *ZD0
   1FC6 D6 01               158 	ldab	*ZD5+1
   1FC8 C1 05               159 	cmpb	#5	; cmpqi *ZD5 with #5
   1FCA 22 03               160 	bhi	.+5
   1FCC 7E 1F DC            161 	jmp	L16	; (bleu) long branch
   1FCF CC 00 00            162 	ldd	#0
   1FD2 DD 00               163 	std	*ZD0	; movhi: #0 -> *ZD0
   1FD4 7E 1F DC            164 	jmp	L16
   00DC                     165 L14:
   1FD7 CC 00 01            166 	ldd	#1
   1FDA DD 00               167 	std	*ZD0	; movhi: #1 -> *ZD0
   00E1                     168 L16:
                            169 ;;;EPILOGUE
   1FDC 38                  170 	pulx	; Pulling register *ZD5
   1FDD DF 00               171 	stx	*ZD5
   1FDF 18 38               172 	puly	; Restore stack frame
   1FE1 39                  173 	rts	; return from function
                            174 ;;;-----------------------------------------
                            175 ;;; END EPILOGUE for string_IsHexDigit
                            176 ;;;-----------------------------------------
                            177 	.globl	_string_DigitToInteger
   00E7                     178 _string_DigitToInteger:
                            179 ;;;-----------------------------------------
                            180 ;;;  PROLOGUE for string_DigitToInteger
                            181 ;;;-----------------------------------------
   1FE2 18 3C               182 	pshy	; Save stack frame
   1FE4 18 30               183 	tsy	; Set current stack frame
   1FE6 DE 00               184 	ldx	*ZD5
   1FE8 3C                  185 	pshx	; pushed register *ZD5
   1FE9 8F                  186 	xgdx	; saving D register
   1FEA D6 00               187 	ldab	*ZB1
   1FEC 37                  188 	pshb	; pushed register *ZB1
   1FED 8F                  189 	xgdx	; restoring D register
                            190 ;;;END PROLOGUE
   1FEE D6 01               191 	ldab	*ZD0+1
   1FF0 CB D0               192 	addb	#-48
   1FF2 D7 01               193 	stab	*ZD5+1	; addqi3: *ZD0 by #-48 -> *ZD5
   1FF4 CC 00 00            194 	ldd	#0
   1FF7 DD 00               195 	std	*ZD0	; movhi: #0 -> *ZD0
   1FF9 D6 01               196 	ldab	*ZD5+1
   1FFB C1 09               197 	cmpb	#9	; cmpqi *ZD5 with #9
   1FFD 23 03               198 	bls	.+5
   1FFF 7E 20 0B            199 	jmp	L19	; (bgtu) long branch
   2002 D6 01               200 	ldab	*ZD5+1
   2004 D7 00               201 	stab	*ZB1	; movqi: *ZD5 -> *ZB1
   2006 D6 00               202 	ldab	*ZB1
   2008 4F                  203 	clra
   2009 DD 00               204 	std	*ZD0	; zero_extendqihi2: *ZB1 -> *ZD0
   0110                     205 L19:
                            206 ;;;EPILOGUE
   200B 8F                  207 	xgdx	; saving D register
   200C 33                  208 	pulb	; Pulling register *ZB1
   200D D7 00               209 	stab	*ZB1
   200F 8F                  210 	xgdx	; restoring D register
   2010 38                  211 	pulx	; Pulling register *ZD5
   2011 DF 00               212 	stx	*ZD5
   2013 18 38               213 	puly	; Restore stack frame
   2015 39                  214 	rts	; return from function
                            215 ;;;-----------------------------------------
                            216 ;;; END EPILOGUE for string_DigitToInteger
                            217 ;;;-----------------------------------------
                            218 	.globl	_string_StringToInteger
   011B                     219 _string_StringToInteger:
                            220 ;;;-----------------------------------------
                            221 ;;;  PROLOGUE for string_StringToInteger
                            222 ;;;-----------------------------------------
   2016 18 3C               223 	pshy	; Save stack frame
   2018 18 30               224 	tsy	; Set current stack frame
   201A DE 00               225 	ldx	*ZD5
   201C 3C                  226 	pshx	; pushed register *ZD5
   201D DE 00               227 	ldx	*ZD6
   201F 3C                  228 	pshx	; pushed register *ZD6
   2020 DE 00               229 	ldx	*ZD7
   2022 3C                  230 	pshx	; pushed register *ZD7
   2023 DE 00               231 	ldx	*ZD8
   2025 3C                  232 	pshx	; pushed register *ZD8
   2026 8F                  233 	xgdx	; saving D register
   2027 D6 00               234 	ldab	*ZB1
   2029 37                  235 	pshb	; pushed register *ZB1
   202A D6 00               236 	ldab	*ZB2
   202C 37                  237 	pshb	; pushed register *ZB2
   202D 8F                  238 	xgdx	; restoring D register
                            239 ;;;END PROLOGUE
   202E DC 00               240 	ldd	*ZD0
   2030 DD 00               241 	std	*ZD8	; movhi: *ZD0 -> *ZD8
   2032 C6 00               242 	ldab	#0
   2034 D7 00               243 	stab	*ZB1	; movqi: #0 -> *ZB1
   2036 DE 00               244 	ldx	*ZD8
   2038 E6 00               245 	ldab	0,x
   203A C1 20               246 	cmpb	#32	; cmpqi 0,x with #32
   203C 27 03               247 	beq	.+5
   203E 7E 20 5A            248 	jmp	L22	; (bne) long branch
   0146                     249 L23:
   2041 7C 00 00            250 	inc	*ZB1	; addqi3: *ZB1 by #1 -> *ZB1
   2044 D6 00               251 	ldab	*ZB1
   2046 4F                  252 	clra
   2047 DD 00               253 	std	*ZD5	; zero_extendqihi2: *ZB1 -> *ZD5
   2049 DC 00               254 	ldd	*ZD8
   204B D3 00               255 	addd	*ZD5
   204D DD 00               256 	std	*ZD5	; addhi3: *ZD8 by *ZD5 -> *ZD5
   204F DE 00               257 	ldx	*ZD5
   2051 E6 00               258 	ldab	0,x
   2053 C1 20               259 	cmpb	#32	; cmpqi 0,x with #32
   2055 26 03               260 	bne	.+5
   2057 7E 20 41            261 	jmp	L23	; (beq) long branch
   015F                     262 L22:
   205A D6 00               263 	ldab	*ZB1
   205C 4F                  264 	clra
   205D DD 00               265 	std	*ZD5	; zero_extendqihi2: *ZB1 -> *ZD5
   205F DC 00               266 	ldd	*ZD8
   2061 D3 00               267 	addd	*ZD5
   2063 DD 00               268 	std	*ZD5	; addhi3: *ZD8 by *ZD5 -> *ZD5
   2065 DE 00               269 	ldx	*ZD5
   2067 E6 00               270 	ldab	0,x
   2069 D7 01               271 	stab	*ZD5+1	; movqi: 0,x -> *ZD5
   206B C6 01               272 	ldab	#1
   206D D7 00               273 	stab	*ZB2	; movqi: #1 -> *ZB2
   206F D6 01               274 	ldab	*ZD5+1
   2071 C1 2D               275 	cmpb	#45	; cmpqi *ZD5 with #45
   2073 27 03               276 	beq	.+5
   2075 7E 20 85            277 	jmp	L35	; (bne) long branch
   2078 C6 FF               278 	ldab	#-1
   207A D7 00               279 	stab	*ZB2	; movqi: #-1 -> *ZB2
   207C D6 01               280 	ldab	*ZD5+1
   207E C1 2D               281 	cmpb	#45	; cmpqi *ZD5 with #45
   2080 26 03               282 	bne	.+5
   2082 7E 20 8E            283 	jmp	L29	; (beq) long branch
   018A                     284 L35:
   2085 D6 01               285 	ldab	*ZD5+1
   2087 C1 2B               286 	cmpb	#43	; cmpqi *ZD5 with #43
   2089 27 03               287 	beq	.+5
   208B 7E 20 91            288 	jmp	L28	; (bne) long branch
   0193                     289 L29:
   208E 7C 00 00            290 	inc	*ZB1	; addqi3: *ZB1 by #1 -> *ZB1
   0196                     291 L28:
   2091 CC 00 00            292 	ldd	#0
   2094 DD 00               293 	std	*ZD6	; movhi: #0 -> *ZD6
   2096 7E 20 C7            294 	jmp	L36
   019E                     295 L33:
   2099 DC 00               296 	ldd	*ZD6
   209B 05                  297 	asld
   209C 05                  298 	asld
   209D DD 00               299 	std	*ZD5	; ashlhi3: *ZD6 by #2 -> *ZD5
   209F DC 00               300 	ldd	*ZD5
   20A1 D3 00               301 	addd	*ZD6
   20A3 DD 00               302 	std	*ZD5	; addhi3: *ZD5 by *ZD6 -> *ZD5
   20A5 DC 00               303 	ldd	*ZD5
   20A7 05                  304 	asld
   20A8 DD 00               305 	std	*ZD5	; ashlhi3: *ZD5 by #1 -> *ZD5
   20AA DC 00               306 	ldd	*ZD5
   20AC C3 FF D0            307 	addd	#-48
   20AF DD 00               308 	std	*ZD6	; addhi3: *ZD5 by #-48 -> *ZD6
   20B1 DC 00               309 	ldd	*ZD8
   20B3 D3 00               310 	addd	*ZD7
   20B5 DD 00               311 	std	*ZD5	; addhi3: *ZD8 by *ZD7 -> *ZD5
   20B7 DE 00               312 	ldx	*ZD5
   20B9 E6 00               313 	ldab	0,x
   20BB 4F                  314 	clra
   20BC DD 00               315 	std	*ZD5	; zero_extendqihi2: 0,x -> *ZD5
   20BE DC 00               316 	ldd	*ZD6
   20C0 D3 00               317 	addd	*ZD5
   20C2 DD 00               318 	std	*ZD6	; addhi3: *ZD6 by *ZD5 -> *ZD6
   20C4 7C 00 00            319 	inc	*ZB1	; addqi3: *ZB1 by #1 -> *ZB1
   01CC                     320 L36:
   20C7 D6 00               321 	ldab	*ZB1
   20C9 4F                  322 	clra
   20CA DD 00               323 	std	*ZD7	; zero_extendqihi2: *ZB1 -> *ZD7
   20CC DC 00               324 	ldd	*ZD8
   20CE D3 00               325 	addd	*ZD7
   20D0 DD 00               326 	std	*ZD5	; addhi3: *ZD8 by *ZD7 -> *ZD5
   20D2 DE 00               327 	ldx	*ZD5
   20D4 E6 00               328 	ldab	0,x
   20D6 CB D0               329 	addb	#-48
   20D8 D7 01               330 	stab	*ZD5+1	; addqi3: 0,x by #-48 -> *ZD5
   20DA D6 01               331 	ldab	*ZD5+1
   20DC C1 09               332 	cmpb	#9	; cmpqi *ZD5 with #9
   20DE 22 03               333 	bhi	.+5
   20E0 7E 20 99            334 	jmp	L33	; (bleu) long branch
   20E3 D6 00               335 	ldab	*ZB2
   20E5 4F                  336 	clra
   20E6 5D                  337 	tstb
   20E7 2C 01               338 	bge	.+3
   20E9 4A                  339 	deca
   20EA DD 00               340 	std	*ZD5	; extendqihi2: *ZB2 -> *ZD5
   20EC DC 00               341 	ldd	*ZD5
   20EE DE 00               342 	ldx	*ZD6
   20F0 3C                  343 	pshx
   20F1 BD 00 00            344 	jsr	___mulhi3
   20F4 38                  345 	pulx
   20F5 DD 00               346 	std	*ZD0	; mulhi3: *ZD5 by *ZD6 -> *ZD0
                            347 ;;;EPILOGUE
   20F7 8F                  348 	xgdx	; saving D register
   20F8 33                  349 	pulb	; Pulling register *ZB2
   20F9 D7 00               350 	stab	*ZB2
   20FB 33                  351 	pulb	; Pulling register *ZB1
   20FC D7 00               352 	stab	*ZB1
   20FE 8F                  353 	xgdx	; restoring D register
   20FF 38                  354 	pulx	; Pulling register *ZD8
   2100 DF 00               355 	stx	*ZD8
   2102 38                  356 	pulx	; Pulling register *ZD7
   2103 DF 00               357 	stx	*ZD7
   2105 38                  358 	pulx	; Pulling register *ZD6
   2106 DF 00               359 	stx	*ZD6
   2108 38                  360 	pulx	; Pulling register *ZD5
   2109 DF 00               361 	stx	*ZD5
   210B 18 38               362 	puly	; Restore stack frame
   210D 39                  363 	rts	; return from function
                            364 ;;;-----------------------------------------
                            365 ;;; END EPILOGUE for string_StringToInteger
                            366 ;;;-----------------------------------------
                            367 	.globl	_string_IntegerToString
   0213                     368 _string_IntegerToString:
                            369 ;;;-----------------------------------------
                            370 ;;;  PROLOGUE for string_IntegerToString
                            371 ;;;-----------------------------------------
   210E 18 3C               372 	pshy	; Save stack frame
   2110 18 30               373 	tsy	; Set current stack frame
   2112 DE 00               374 	ldx	*ZD1
   2114 3C                  375 	pshx	; pushed register *ZD1
   2115 DE 00               376 	ldx	*ZD5
   2117 3C                  377 	pshx	; pushed register *ZD5
   2118 DE 00               378 	ldx	*ZD6
   211A 3C                  379 	pshx	; pushed register *ZD6
   211B DE 00               380 	ldx	*ZD7
   211D 3C                  381 	pshx	; pushed register *ZD7
   211E DE 00               382 	ldx	*ZD8
   2120 3C                  383 	pshx	; pushed register *ZD8
   2121 8F                  384 	xgdx	; saving D register
   2122 D6 00               385 	ldab	*ZB1
   2124 37                  386 	pshb	; pushed register *ZB1
   2125 D6 00               387 	ldab	*ZB2
   2127 37                  388 	pshb	; pushed register *ZB2
   2128 D6 00               389 	ldab	*ZB3
   212A 37                  390 	pshb	; pushed register *ZB3
   212B 8F                  391 	xgdx	; restoring D register
                            392 ;;;END PROLOGUE
   212C DC 00               393 	ldd	*ZD0
   212E DD 00               394 	std	*ZD7	; movhi: *ZD0 -> *ZD7
   2130 18 EC 04            395 	ldd	4,y
   2133 DD 00               396 	std	*ZD8	; movhi: 4,y -> *ZD8
   2135 18 E6 06            397 	ldab	6,y
   2138 D7 00               398 	stab	*ZB3	; movqi: 6,y -> *ZB3
   213A C6 00               399 	ldab	#0
   213C D7 00               400 	stab	*ZB2	; movqi: #0 -> *ZB2
   213E DC 00               401 	ldd	*ZD0	; tsthi: R:*ZD0
   2140 2D 03               402 	blt	.+5
   2142 7E 21 5E            403 	jmp	L38	; (bge) long branch
   2145 D6 00               404 	ldab	*ZB2
   2147 D1 00               405 	cmpb	*ZB3	; cmpqi *ZB2 with *ZB3
   2149 25 03               406 	blo	.+5
   214B 7E 21 58            407 	jmp	L39	; (bgeu) long branch
   214E C6 2D               408 	ldab	#45
   2150 DE 00               409 	ldx	*ZD8
   2152 E7 00               410 	stab	0,x	; movqi: #45 -> 0,x
   2154 C6 01               411 	ldab	#1
   2156 D7 00               412 	stab	*ZB2	; movqi: #1 -> *ZB2
   025D                     413 L39:
   2158 4F                  414 	clra
   2159 5F                  415 	clrb
   215A 93 00               416 	subd	*ZD0
   215C DD 00               417 	std	*ZD7	; neghi2: *ZD0 -> *ZD7
   0263                     418 L38:
   215E CC 00 01            419 	ldd	#1
   2161 DD 00               420 	std	*ZD6	; movhi: #1 -> *ZD6
   2163 DC 00               421 	ldd	*ZD7
   2165 1A 83 00 09         422 	cpd	#9	; cmphi *ZD7 with #9
   2169 22 03               423 	bhi	.+5
   216B 7E 21 E6            424 	jmp	L51	; (bleu) long branch
   0273                     425 L42:
   216E DC 00               426 	ldd	*ZD6
   2170 05                  427 	asld
   2171 05                  428 	asld
   2172 DD 00               429 	std	*ZD5	; ashlhi3: *ZD6 by #2 -> *ZD5
   2174 DC 00               430 	ldd	*ZD5
   2176 D3 00               431 	addd	*ZD6
   2178 DD 00               432 	std	*ZD5	; addhi3: *ZD5 by *ZD6 -> *ZD5
   217A DC 00               433 	ldd	*ZD5
   217C 05                  434 	asld
   217D DD 00               435 	std	*ZD6	; ashlhi3: *ZD5 by #1 -> *ZD6
   217F DC 00               436 	ldd	*ZD5
   2181 05                  437 	asld
   2182 05                  438 	asld
   2183 05                  439 	asld
   2184 DD 00               440 	std	*ZD5	; ashlhi3: *ZD5 by #3 -> *ZD5
   2186 DC 00               441 	ldd	*ZD5
   2188 D3 00               442 	addd	*ZD6
   218A DD 00               443 	std	*ZD5	; addhi3: *ZD5 by *ZD6 -> *ZD5
   218C DC 00               444 	ldd	*ZD5
   218E 05                  445 	asld
   218F DD 00               446 	std	*ZD5	; ashlhi3: *ZD5 by #1 -> *ZD5
   2191 DC 00               447 	ldd	*ZD5
   2193 1A 93 00            448 	cpd	*ZD7	; cmphi *ZD5 with *ZD7
   2196 22 03               449 	bhi	.+5
   2198 7E 21 6E            450 	jmp	L42	; (bleu) long branch
   219B 7E 21 E6            451 	jmp	L51
   02A3                     452 L46:
   219E DC 00               453 	ldd	*ZD7
   21A0 DE 00               454 	ldx	*ZD6
   21A2 02                  455 	idiv
   21A3 DF 00               456 	stx	*ZD1	; udivhi3: *ZD7 by *ZD6 -> *ZD1
   21A5 D6 01               457 	ldab	*ZD1+1
   21A7 D7 00               458 	stab	*ZB1	; movqi: *ZD1 -> *ZB1
   21A9 D6 00               459 	ldab	*ZB1
   21AB 4F                  460 	clra
   21AC DD 00               461 	std	*ZD5	; zero_extendqihi2: *ZB1 -> *ZD5
   21AE DC 00               462 	ldd	*ZD5
   21B0 DE 00               463 	ldx	*ZD6
   21B2 3C                  464 	pshx
   21B3 BD 00 00            465 	jsr	___mulhi3
   21B6 38                  466 	pulx
   21B7 DD 00               467 	std	*ZD5	; mulhi3: *ZD5 by *ZD6 -> *ZD5
   21B9 DC 00               468 	ldd	*ZD7
   21BB 93 00               469 	subd	*ZD5
   21BD DD 00               470 	std	*ZD7	; subhi3: *ZD7 by *ZD5 -> *ZD7
   21BF D6 00               471 	ldab	*ZB2
   21C1 D1 00               472 	cmpb	*ZB3	; cmpqi *ZB2 with *ZB3
   21C3 25 03               473 	blo	.+5
   21C5 7E 21 DE            474 	jmp	L47	; (bgeu) long branch
   21C8 D6 00               475 	ldab	*ZB2
   21CA 4F                  476 	clra
   21CB DD 00               477 	std	*ZD5	; zero_extendqihi2: *ZB2 -> *ZD5
   21CD DC 00               478 	ldd	*ZD8
   21CF D3 00               479 	addd	*ZD5
   21D1 DD 00               480 	std	*ZD5	; addhi3: *ZD8 by *ZD5 -> *ZD5
   21D3 D6 00               481 	ldab	*ZB1
   21D5 CB 30               482 	addb	#48
   21D7 DE 00               483 	ldx	*ZD5
   21D9 E7 00               484 	stab	0,x	; addqi3: *ZB1 by #48 -> 0,x
   21DB 7C 00 00            485 	inc	*ZB2	; addqi3: *ZB2 by #1 -> *ZB2
   02E3                     486 L47:
   21DE DC 00               487 	ldd	*ZD6
   21E0 CE 00 0A            488 	ldx	#10
   21E3 02                  489 	idiv
   21E4 DF 00               490 	stx	*ZD6	; udivhi3: *ZD6 by #10 -> *ZD6
   02EB                     491 L51:
   21E6 DC 00               492 	ldd	*ZD6	; tsthi: R:*ZD6
   21E8 27 03               493 	beq	.+5
   21EA 7E 21 9E            494 	jmp	L46	; (bne) long branch
   21ED D6 00               495 	ldab	*ZB2
   21EF D1 00               496 	cmpb	*ZB3	; cmpqi *ZB2 with *ZB3
   21F1 25 03               497 	blo	.+5
   21F3 7E 22 07            498 	jmp	L49	; (bgeu) long branch
   21F6 D6 00               499 	ldab	*ZB2
   21F8 4F                  500 	clra
   21F9 DD 00               501 	std	*ZD5	; zero_extendqihi2: *ZB2 -> *ZD5
   21FB DC 00               502 	ldd	*ZD8
   21FD D3 00               503 	addd	*ZD5
   21FF DD 00               504 	std	*ZD5	; addhi3: *ZD8 by *ZD5 -> *ZD5
   2201 C6 00               505 	ldab	#0
   2203 DE 00               506 	ldx	*ZD5
   2205 E7 00               507 	stab	0,x	; movqi: #0 -> 0,x
   030C                     508 L49:
   2207 D6 00               509 	ldab	*ZB3
   2209 4F                  510 	clra
   220A DD 00               511 	std	*ZD5	; zero_extendqihi2: *ZB3 -> *ZD5
   220C DC 00               512 	ldd	*ZD8
   220E D3 00               513 	addd	*ZD5
   2210 DD 00               514 	std	*ZD5	; addhi3: *ZD8 by *ZD5 -> *ZD5
   2212 C6 00               515 	ldab	#0
   2214 DE 00               516 	ldx	*ZD5
   2216 E7 00               517 	stab	0,x	; movqi: #0 -> 0,x
                            518 ;;;EPILOGUE
   2218 8F                  519 	xgdx	; saving D register
   2219 33                  520 	pulb	; Pulling register *ZB3
   221A D7 00               521 	stab	*ZB3
   221C 33                  522 	pulb	; Pulling register *ZB2
   221D D7 00               523 	stab	*ZB2
   221F 33                  524 	pulb	; Pulling register *ZB1
   2220 D7 00               525 	stab	*ZB1
   2222 8F                  526 	xgdx	; restoring D register
   2223 38                  527 	pulx	; Pulling register *ZD8
   2224 DF 00               528 	stx	*ZD8
   2226 38                  529 	pulx	; Pulling register *ZD7
   2227 DF 00               530 	stx	*ZD7
   2229 38                  531 	pulx	; Pulling register *ZD6
   222A DF 00               532 	stx	*ZD6
   222C 38                  533 	pulx	; Pulling register *ZD5
   222D DF 00               534 	stx	*ZD5
   222F 38                  535 	pulx	; Pulling register *ZD1
   2230 DF 00               536 	stx	*ZD1
   2232 18 38               537 	puly	; Restore stack frame
   2234 39                  538 	rts	; return from function
                            539 ;;;-----------------------------------------
                            540 ;;; END EPILOGUE for string_IntegerToString
                            541 ;;;-----------------------------------------
                            542 	.globl	_string_IntegerToHex
   033A                     543 _string_IntegerToHex:
                            544 ;;;-----------------------------------------
                            545 ;;;  PROLOGUE for string_IntegerToHex
                            546 ;;;-----------------------------------------
   2235 18 3C               547 	pshy	; Save stack frame
   2237 18 30               548 	tsy	; Set current stack frame
   2239 DE 00               549 	ldx	*ZD5
   223B 3C                  550 	pshx	; pushed register *ZD5
   223C DE 00               551 	ldx	*ZD6
   223E 3C                  552 	pshx	; pushed register *ZD6
   223F DE 00               553 	ldx	*ZD7
   2241 3C                  554 	pshx	; pushed register *ZD7
   2242 8F                  555 	xgdx	; saving D register
   2243 D6 00               556 	ldab	*ZB1
   2245 37                  557 	pshb	; pushed register *ZB1
   2246 D6 00               558 	ldab	*ZB2
   2248 37                  559 	pshb	; pushed register *ZB2
   2249 D6 00               560 	ldab	*ZB3
   224B 37                  561 	pshb	; pushed register *ZB3
   224C D6 00               562 	ldab	*ZB4
   224E 37                  563 	pshb	; pushed register *ZB4
   224F 8F                  564 	xgdx	; restoring D register
                            565 ;;;END PROLOGUE
   2250 DC 00               566 	ldd	*ZD0
   2252 DD 00               567 	std	*ZD6	; movhi: *ZD0 -> *ZD6
   2254 18 EC 04            568 	ldd	4,y
   2257 DD 00               569 	std	*ZD7	; movhi: 4,y -> *ZD7
   2259 18 E6 06            570 	ldab	6,y
   225C D7 00               571 	stab	*ZB3	; movqi: 6,y -> *ZB3
   225E D6 00               572 	ldab	*ZB3
   2260 D7 00               573 	stab	*ZB2	; movqi: *ZB3 -> *ZB2
   2262 26 03               574 	bne	.+5
   2264 7E 22 B2            575 	jmp	L54	; (beq) long branch
   036C                     576 L56:
   2267 D6 01               577 	ldab	*ZD6+1
   2269 D7 00               578 	stab	*ZB4	; movqi: *ZD6 -> *ZB4
   226B D6 00               579 	ldab	*ZB4
   226D C4 0F               580 	andb	#15
   226F D7 00               581 	stab	*ZB1	; andqi3: *ZB4 by #15 -> *ZB1
   2271 D6 00               582 	ldab	*ZB2
   2273 4F                  583 	clra
   2274 DD 00               584 	std	*ZD5	; zero_extendqihi2: *ZB2 -> *ZD5
   2276 DC 00               585 	ldd	*ZD5
   2278 D3 00               586 	addd	*ZD7
   227A DD 00               587 	std	*ZD5	; addhi3: *ZD5 by *ZD7 -> *ZD5
   227C DC 00               588 	ldd	*ZD5
   227E C3 FF FF            589 	addd	#-1
   2281 DD 00               590 	std	*ZD5	; addhi3: *ZD5 by #-1 -> *ZD5
   2283 D6 00               591 	ldab	*ZB1
   2285 C1 09               592 	cmpb	#9	; cmpqi *ZB1 with #9
   2287 23 03               593 	bls	.+5
   2289 7E 22 97            594 	jmp	L57	; (bgtu) long branch
   228C D6 00               595 	ldab	*ZB1
   228E CA 30               596 	orab	#48
   2290 DE 00               597 	ldx	*ZD5
   2292 E7 00               598 	stab	0,x	; iorqi3: *ZB1 by #48 -> 0,x
   2294 7E 22 9F            599 	jmp	L58
   039C                     600 L57:
   2297 D6 00               601 	ldab	*ZB1
   2299 CB 37               602 	addb	#55
   229B DE 00               603 	ldx	*ZD5
   229D E7 00               604 	stab	0,x	; addqi3: *ZB1 by #55 -> 0,x
   03A4                     605 L58:
   229F DC 00               606 	ldd	*ZD6
   22A1 04                  607 	lsrd
   22A2 04                  608 	lsrd
   22A3 04                  609 	lsrd
   22A4 04                  610 	lsrd
   22A5 DD 00               611 	std	*ZD6	; lshrhi3: *ZD6 by #4 -> *ZD6
   22A7 D6 00               612 	ldab	*ZB2
   22A9 CB FF               613 	addb	#-1
   22AB D7 00               614 	stab	*ZB2	; addqi3: *ZB2 by #-1 -> *ZB2
   22AD 27 03               615 	beq	.+5
   22AF 7E 22 67            616 	jmp	L56	; (bne) long branch
   03B7                     617 L54:
   22B2 D6 00               618 	ldab	*ZB3
   22B4 4F                  619 	clra
   22B5 DD 00               620 	std	*ZD5	; zero_extendqihi2: *ZB3 -> *ZD5
   22B7 DC 00               621 	ldd	*ZD7
   22B9 D3 00               622 	addd	*ZD5
   22BB DD 00               623 	std	*ZD5	; addhi3: *ZD7 by *ZD5 -> *ZD5
   22BD C6 00               624 	ldab	#0
   22BF DE 00               625 	ldx	*ZD5
   22C1 E7 00               626 	stab	0,x	; movqi: #0 -> 0,x
                            627 ;;;EPILOGUE
   22C3 8F                  628 	xgdx	; saving D register
   22C4 33                  629 	pulb	; Pulling register *ZB4
   22C5 D7 00               630 	stab	*ZB4
   22C7 33                  631 	pulb	; Pulling register *ZB3
   22C8 D7 00               632 	stab	*ZB3
   22CA 33                  633 	pulb	; Pulling register *ZB2
   22CB D7 00               634 	stab	*ZB2
   22CD 33                  635 	pulb	; Pulling register *ZB1
   22CE D7 00               636 	stab	*ZB1
   22D0 8F                  637 	xgdx	; restoring D register
   22D1 38                  638 	pulx	; Pulling register *ZD7
   22D2 DF 00               639 	stx	*ZD7
   22D4 38                  640 	pulx	; Pulling register *ZD6
   22D5 DF 00               641 	stx	*ZD6
   22D7 38                  642 	pulx	; Pulling register *ZD5
   22D8 DF 00               643 	stx	*ZD5
   22DA 18 38               644 	puly	; Restore stack frame
   22DC 39                  645 	rts	; return from function
                            646 ;;;-----------------------------------------
                            647 ;;; END EPILOGUE for string_IntegerToHex
                            648 ;;;-----------------------------------------
                            649 	.globl	_string_HexDigitToInteger
   03E2                     650 _string_HexDigitToInteger:
                            651 ;;;-----------------------------------------
                            652 ;;;  PROLOGUE for string_HexDigitToInteger
                            653 ;;;-----------------------------------------
   22DD 18 3C               654 	pshy	; Save stack frame
   22DF 18 30               655 	tsy	; Set current stack frame
   22E1 DE 00               656 	ldx	*ZD5
   22E3 3C                  657 	pshx	; pushed register *ZD5
   22E4 8F                  658 	xgdx	; saving D register
   22E5 D6 00               659 	ldab	*ZB1
   22E7 37                  660 	pshb	; pushed register *ZB1
   22E8 8F                  661 	xgdx	; restoring D register
                            662 ;;;END PROLOGUE
   22E9 D6 01               663 	ldab	*ZD0+1
   22EB CB D0               664 	addb	#-48
   22ED D7 01               665 	stab	*ZD5+1	; addqi3: *ZD0 by #-48 -> *ZD5
   22EF D6 01               666 	ldab	*ZD5+1
   22F1 C1 09               667 	cmpb	#9	; cmpqi *ZD5 with #9
   22F3 22 03               668 	bhi	.+5
   22F5 7E 23 2D            669 	jmp	L65	; (bleu) long branch
   22F8 D6 01               670 	ldab	*ZD0+1
   22FA CB BF               671 	addb	#-65
   22FC D7 01               672 	stab	*ZD5+1	; addqi3: *ZD0 by #-65 -> *ZD5
   22FE D6 01               673 	ldab	*ZD5+1
   2300 C1 05               674 	cmpb	#5	; cmpqi *ZD5 with #5
   2302 23 03               675 	bls	.+5
   2304 7E 23 10            676 	jmp	L62	; (bgtu) long branch
   2307 D6 01               677 	ldab	*ZD0+1
   2309 CB C9               678 	addb	#201
   230B D7 01               679 	stab	*ZD5+1	; addqi3: *ZD0 by #201 -> *ZD5
   230D 7E 23 2D            680 	jmp	L65
   0415                     681 L62:
   2310 D6 01               682 	ldab	*ZD0+1
   2312 CB 9F               683 	addb	#-97
   2314 D7 01               684 	stab	*ZD5+1	; addqi3: *ZD0 by #-97 -> *ZD5
   2316 D6 01               685 	ldab	*ZD5+1
   2318 C1 05               686 	cmpb	#5	; cmpqi *ZD5 with #5
   231A 22 03               687 	bhi	.+5
   231C 7E 23 27            688 	jmp	L63	; (bleu) long branch
   231F CC 00 00            689 	ldd	#0
   2322 DD 00               690 	std	*ZD0	; movhi: #0 -> *ZD0
   2324 7E 23 36            691 	jmp	L64
   042C                     692 L63:
   2327 D6 01               693 	ldab	*ZD0+1
   2329 CB A9               694 	addb	#169
   232B D7 01               695 	stab	*ZD5+1	; addqi3: *ZD0 by #169 -> *ZD5
   0432                     696 L65:
   232D D6 01               697 	ldab	*ZD5+1
   232F D7 00               698 	stab	*ZB1	; movqi: *ZD5 -> *ZB1
   2331 D6 00               699 	ldab	*ZB1
   2333 4F                  700 	clra
   2334 DD 00               701 	std	*ZD0	; zero_extendqihi2: *ZB1 -> *ZD0
   043B                     702 L64:
                            703 ;;;EPILOGUE
   2336 8F                  704 	xgdx	; saving D register
   2337 33                  705 	pulb	; Pulling register *ZB1
   2338 D7 00               706 	stab	*ZB1
   233A 8F                  707 	xgdx	; restoring D register
   233B 38                  708 	pulx	; Pulling register *ZD5
   233C DF 00               709 	stx	*ZD5
   233E 18 38               710 	puly	; Restore stack frame
   2340 39                  711 	rts	; return from function
                            712 ;;;-----------------------------------------
                            713 ;;; END EPILOGUE for string_HexDigitToInteger
                            714 ;;;-----------------------------------------
                            715 	.globl	_string_HexToInteger
   0446                     716 _string_HexToInteger:
                            717 ;;;-----------------------------------------
                            718 ;;;  PROLOGUE for string_HexToInteger
                            719 ;;;-----------------------------------------
   2341 18 3C               720 	pshy	; Save stack frame
   2343 18 30               721 	tsy	; Set current stack frame
   2345 DE 00               722 	ldx	*ZD5
   2347 3C                  723 	pshx	; pushed register *ZD5
   2348 DE 00               724 	ldx	*ZD6
   234A 3C                  725 	pshx	; pushed register *ZD6
   234B DE 00               726 	ldx	*ZD7
   234D 3C                  727 	pshx	; pushed register *ZD7
   234E 8F                  728 	xgdx	; saving D register
   234F D6 00               729 	ldab	*ZB1
   2351 37                  730 	pshb	; pushed register *ZB1
   2352 8F                  731 	xgdx	; restoring D register
                            732 ;;;END PROLOGUE
   2353 CC 00 00            733 	ldd	#0
   2356 DD 00               734 	std	*ZD6	; movhi: #0 -> *ZD6
   2358 DC 00               735 	ldd	*ZD0
   235A DD 00               736 	std	*ZD7	; movhi: *ZD0 -> *ZD7
   0461                     737 L67:
   235C DE 00               738 	ldx	*ZD7
   235E E6 00               739 	ldab	0,x
   2360 D7 01               740 	stab	*ZD0+1	; movqi: 0,x -> *ZD0
   2362 BD 1F 96            741 	jsr	_string_IsHexDigit	; CALL: R:*ZD0 = _string_IsHexDigit (0 bytes)
   2365 DC 00               742 	ldd	*ZD0	; tsthi: R:*ZD0
   2367 26 03               743 	bne	.+5
   2369 7E 23 98            744 	jmp	L68	; (beq) long branch
   236C DE 00               745 	ldx	*ZD7
   236E E6 00               746 	ldab	0,x
   2370 D7 01               747 	stab	*ZD0+1	; movqi: 0,x -> *ZD0
   2372 BD 22 DD            748 	jsr	_string_HexDigitToInteger	; CALL: R:*ZD0 = _string_HexDigitToInteger (0 bytes)
   2375 D6 01               749 	ldab	*ZD0+1
   2377 C1 10               750 	cmpb	#16	; cmpqi *ZD0 with #16
   2379 26 03               751 	bne	.+5
   237B 7E 23 98            752 	jmp	L68	; (beq) long branch
   237E DC 00               753 	ldd	*ZD6
   2380 05                  754 	asld
   2381 05                  755 	asld
   2382 05                  756 	asld
   2383 05                  757 	asld
   2384 DD 00               758 	std	*ZD6	; ashlhi3: *ZD6 by #4 -> *ZD6
   2386 D6 01               759 	ldab	*ZD0+1
   2388 D7 00               760 	stab	*ZB1	; movqi: *ZD0 -> *ZB1
   238A D6 00               761 	ldab	*ZB1
   238C 4F                  762 	clra
   238D DD 00               763 	std	*ZD5	; zero_extendqihi2: *ZB1 -> *ZD5
   238F DC 00               764 	ldd	*ZD6
   2391 D3 00               765 	addd	*ZD5
   2393 DD 00               766 	std	*ZD6	; addhi3: *ZD6 by *ZD5 -> *ZD6
   2395 7E 23 5C            767 	jmp	L67
   049D                     768 L68:
   2398 DC 00               769 	ldd	*ZD6
   239A DD 00               770 	std	*ZD0	; movhi: *ZD6 -> *ZD0
                            771 ;;;EPILOGUE
   239C 8F                  772 	xgdx	; saving D register
   239D 33                  773 	pulb	; Pulling register *ZB1
   239E D7 00               774 	stab	*ZB1
   23A0 8F                  775 	xgdx	; restoring D register
   23A1 38                  776 	pulx	; Pulling register *ZD7
   23A2 DF 00               777 	stx	*ZD7
   23A4 38                  778 	pulx	; Pulling register *ZD6
   23A5 DF 00               779 	stx	*ZD6
   23A7 38                  780 	pulx	; Pulling register *ZD5
   23A8 DF 00               781 	stx	*ZD5
   23AA 18 38               782 	puly	; Restore stack frame
   23AC 39                  783 	rts	; return from function
                            784 ;;;-----------------------------------------
                            785 ;;; END EPILOGUE for string_HexToInteger
                            786 ;;;-----------------------------------------
                            787 	.globl	_string_Copy
   04B2                     788 _string_Copy:
                            789 ;;;-----------------------------------------
                            790 ;;;  PROLOGUE for string_Copy
                            791 ;;;-----------------------------------------
   23AD 18 3C               792 	pshy	; Save stack frame
   23AF 18 30               793 	tsy	; Set current stack frame
   23B1 DE 00               794 	ldx	*ZD1
   23B3 3C                  795 	pshx	; pushed register *ZD1
   23B4 DE 00               796 	ldx	*ZD2
   23B6 3C                  797 	pshx	; pushed register *ZD2
   23B7 DE 00               798 	ldx	*ZD5
   23B9 3C                  799 	pshx	; pushed register *ZD5
   23BA DE 00               800 	ldx	*ZD6
   23BC 3C                  801 	pshx	; pushed register *ZD6
   23BD DE 00               802 	ldx	*ZD7
   23BF 3C                  803 	pshx	; pushed register *ZD7
   23C0 DE 00               804 	ldx	*ZD8
   23C2 3C                  805 	pshx	; pushed register *ZD8
   23C3 8F                  806 	xgdx	; saving D register
   23C4 D6 00               807 	ldab	*ZB1
   23C6 37                  808 	pshb	; pushed register *ZB1
   23C7 D6 00               809 	ldab	*ZB2
   23C9 37                  810 	pshb	; pushed register *ZB2
   23CA 8F                  811 	xgdx	; restoring D register
                            812 ;;;END PROLOGUE
   23CB DC 00               813 	ldd	*ZD0
   23CD DD 00               814 	std	*ZD8	; movhi: *ZD0 -> *ZD8
   23CF 18 EC 04            815 	ldd	4,y
   23D2 DD 00               816 	std	*ZD2	; movhi: 4,y -> *ZD2
   23D4 18 E6 06            817 	ldab	6,y
   23D7 D7 01               818 	stab	*ZD1+1	; movqi: 6,y -> *ZD1
   23D9 C6 00               819 	ldab	#0
   23DB D7 00               820 	stab	*ZB1	; movqi: #0 -> *ZB1
   23DD 7E 24 05            821 	jmp	L77
   04E5                     822 L76:
   23E0 D6 00               823 	ldab	*ZB1
   23E2 4F                  824 	clra
   23E3 DD 00               825 	std	*ZD7	; zero_extendqihi2: *ZB1 -> *ZD7
   23E5 DC 00               826 	ldd	*ZD2
   23E7 D3 00               827 	addd	*ZD7
   23E9 DD 00               828 	std	*ZD5	; addhi3: *ZD2 by *ZD7 -> *ZD5
   23EB DE 00               829 	ldx	*ZD5
   23ED E6 00               830 	ldab	0,x
   23EF D7 01               831 	stab	*ZD6+1	; movqi: 0,x -> *ZD6
   23F1 26 03               832 	bne	.+5
   23F3 7E 24 12            833 	jmp	L73	; (beq) long branch
   23F6 DC 00               834 	ldd	*ZD8
   23F8 D3 00               835 	addd	*ZD7
   23FA DD 00               836 	std	*ZD5	; addhi3: *ZD8 by *ZD7 -> *ZD5
   23FC D6 01               837 	ldab	*ZD6+1
   23FE DE 00               838 	ldx	*ZD5
   2400 E7 00               839 	stab	0,x	; movqi: *ZD6 -> 0,x
   2402 7C 00 00            840 	inc	*ZB1	; addqi3: *ZB1 by #1 -> *ZB1
   050A                     841 L77:
   2405 D6 01               842 	ldab	*ZD1+1
   2407 D7 00               843 	stab	*ZB2	; movqi: *ZD1 -> *ZB2
   2409 D6 00               844 	ldab	*ZB1
   240B D1 00               845 	cmpb	*ZB2	; cmpqi *ZB1 with *ZB2
   240D 24 03               846 	bhs	.+5
   240F 7E 23 E0            847 	jmp	L76	; (bltu) long branch
   0517                     848 L73:
   2412 D6 00               849 	ldab	*ZB1
   2414 4F                  850 	clra
   2415 DD 00               851 	std	*ZD5	; zero_extendqihi2: *ZB1 -> *ZD5
   2417 DC 00               852 	ldd	*ZD8
   2419 D3 00               853 	addd	*ZD5
   241B DD 00               854 	std	*ZD5	; addhi3: *ZD8 by *ZD5 -> *ZD5
   241D C6 00               855 	ldab	#0
   241F DE 00               856 	ldx	*ZD5
   2421 E7 00               857 	stab	0,x	; movqi: #0 -> 0,x
                            858 ;;;EPILOGUE
   2423 8F                  859 	xgdx	; saving D register
   2424 33                  860 	pulb	; Pulling register *ZB2
   2425 D7 00               861 	stab	*ZB2
   2427 33                  862 	pulb	; Pulling register *ZB1
   2428 D7 00               863 	stab	*ZB1
   242A 8F                  864 	xgdx	; restoring D register
   242B 38                  865 	pulx	; Pulling register *ZD8
   242C DF 00               866 	stx	*ZD8
   242E 38                  867 	pulx	; Pulling register *ZD7
   242F DF 00               868 	stx	*ZD7
   2431 38                  869 	pulx	; Pulling register *ZD6
   2432 DF 00               870 	stx	*ZD6
   2434 38                  871 	pulx	; Pulling register *ZD5
   2435 DF 00               872 	stx	*ZD5
   2437 38                  873 	pulx	; Pulling register *ZD2
   2438 DF 00               874 	stx	*ZD2
   243A 38                  875 	pulx	; Pulling register *ZD1
   243B DF 00               876 	stx	*ZD1
   243D 18 38               877 	puly	; Restore stack frame
   243F 39                  878 	rts	; return from function
                            879 ;;;-----------------------------------------
                            880 ;;; END EPILOGUE for string_Copy
                            881 ;;;-----------------------------------------
                            882 	.globl	_string_NextWord
   0545                     883 _string_NextWord:
                            884 ;;;-----------------------------------------
                            885 ;;;  PROLOGUE for string_NextWord
                            886 ;;;-----------------------------------------
   2440 18 3C               887 	pshy	; Save stack frame
   2442 18 30               888 	tsy	; Set current stack frame
   2444 DE 00               889 	ldx	*ZD5
   2446 3C                  890 	pshx	; pushed register *ZD5
   2447 DE 00               891 	ldx	*ZD6
   2449 3C                  892 	pshx	; pushed register *ZD6
   244A DE 00               893 	ldx	*ZD7
   244C 3C                  894 	pshx	; pushed register *ZD7
   244D DE 00               895 	ldx	*ZD8
   244F 3C                  896 	pshx	; pushed register *ZD8
                            897 ;;;END PROLOGUE
   2450 DC 00               898 	ldd	*ZD0
   2452 DD 00               899 	std	*ZD8	; movhi: *ZD0 -> *ZD8
   2454 CC 00 01            900 	ldd	#1
   2457 DD 00               901 	std	*ZD7	; movhi: #1 -> *ZD7
   2459 CC 00 00            902 	ldd	#0
   245C DD 00               903 	std	*ZD6	; movhi: #0 -> *ZD6
   0563                     904 L81:
   245E DC 00               905 	ldd	*ZD8
   2460 D3 00               906 	addd	*ZD6
   2462 DD 00               907 	std	*ZD5	; addhi3: *ZD8 by *ZD6 -> *ZD5
   2464 DE 00               908 	ldx	*ZD5
   2466 E6 00               909 	ldab	0,x
   2468 D7 01               910 	stab	*ZD5+1	; movqi: 0,x -> *ZD5
   246A 26 03               911 	bne	.+5
   246C 7E 24 A2            912 	jmp	L80	; (beq) long branch
   246F DC 00               913 	ldd	*ZD7	; tsthi: R:*ZD7
   2471 26 03               914 	bne	.+5
   2473 7E 24 8C            915 	jmp	L83	; (beq) long branch
   2476 D6 01               916 	ldab	*ZD5+1
   2478 D7 01               917 	stab	*ZD0+1	; movqi: *ZD5 -> *ZD0
   247A BD 1E FB            918 	jsr	_string_IsSpace	; CALL: R:*ZD0 = _string_IsSpace (0 bytes)
   247D DC 00               919 	ldd	*ZD0	; tsthi: R:*ZD0
   247F 26 03               920 	bne	.+5
   2481 7E 24 9A            921 	jmp	L85	; (beq) long branch
   2484 CC 00 00            922 	ldd	#0
   2487 DD 00               923 	std	*ZD7	; movhi: #0 -> *ZD7
   2489 7E 24 9A            924 	jmp	L85
   0591                     925 L83:
   248C D6 01               926 	ldab	*ZD5+1
   248E D7 01               927 	stab	*ZD0+1	; movqi: *ZD5 -> *ZD0
   2490 BD 1E FB            928 	jsr	_string_IsSpace	; CALL: R:*ZD0 = _string_IsSpace (0 bytes)
   2493 DC 00               929 	ldd	*ZD0	; tsthi: R:*ZD0
   2495 26 03               930 	bne	.+5
   2497 7E 24 A2            931 	jmp	L80	; (beq) long branch
   059F                     932 L85:
   249A DE 00               933 	ldx	*ZD6
   249C 08                  934 	inx
   249D DF 00               935 	stx	*ZD6	; addhi3: *ZD6 by #1 -> *ZD6
   249F 7E 24 5E            936 	jmp	L81
   05A7                     937 L80:
   24A2 DC 00               938 	ldd	*ZD8
   24A4 D3 00               939 	addd	*ZD6
   24A6 DD 00               940 	std	*ZD0	; addhi3: *ZD8 by *ZD6 -> *ZD0
                            941 ;;;EPILOGUE
   24A8 38                  942 	pulx	; Pulling register *ZD8
   24A9 DF 00               943 	stx	*ZD8
   24AB 38                  944 	pulx	; Pulling register *ZD7
   24AC DF 00               945 	stx	*ZD7
   24AE 38                  946 	pulx	; Pulling register *ZD6
   24AF DF 00               947 	stx	*ZD6
   24B1 38                  948 	pulx	; Pulling register *ZD5
   24B2 DF 00               949 	stx	*ZD5
   24B4 18 38               950 	puly	; Restore stack frame
   24B6 39                  951 	rts	; return from function
                            952 ;;;-----------------------------------------
                            953 ;;; END EPILOGUE for string_NextWord
                            954 ;;;-----------------------------------------
                            955 	.globl	_string_SkipSpaces
   05BC                     956 _string_SkipSpaces:
                            957 ;;;-----------------------------------------
                            958 ;;;  PROLOGUE for string_SkipSpaces
                            959 ;;;-----------------------------------------
   24B7 18 3C               960 	pshy	; Save stack frame
   24B9 18 30               961 	tsy	; Set current stack frame
   24BB DE 00               962 	ldx	*ZD5
   24BD 3C                  963 	pshx	; pushed register *ZD5
   24BE DE 00               964 	ldx	*ZD6
   24C0 3C                  965 	pshx	; pushed register *ZD6
   24C1 DE 00               966 	ldx	*ZD7
   24C3 3C                  967 	pshx	; pushed register *ZD7
                            968 ;;;END PROLOGUE
   24C4 DC 00               969 	ldd	*ZD0
   24C6 DD 00               970 	std	*ZD7	; movhi: *ZD0 -> *ZD7
   24C8 CC 00 00            971 	ldd	#0
   24CB DD 00               972 	std	*ZD6	; movhi: #0 -> *ZD6
   05D2                     973 L91:
   24CD DC 00               974 	ldd	*ZD7
   24CF D3 00               975 	addd	*ZD6
   24D1 DD 00               976 	std	*ZD5	; addhi3: *ZD7 by *ZD6 -> *ZD5
   24D3 DE 00               977 	ldx	*ZD5
   24D5 E6 00               978 	ldab	0,x
   24D7 D7 01               979 	stab	*ZD0+1	; movqi: 0,x -> *ZD0
   24D9 BD 1E FB            980 	jsr	_string_IsSpace	; CALL: R:*ZD0 = _string_IsSpace (0 bytes)
   24DC DC 00               981 	ldd	*ZD0	; tsthi: R:*ZD0
   24DE 26 03               982 	bne	.+5
   24E0 7E 24 EB            983 	jmp	L90	; (beq) long branch
   24E3 DE 00               984 	ldx	*ZD6
   24E5 08                  985 	inx
   24E6 DF 00               986 	stx	*ZD6	; addhi3: *ZD6 by #1 -> *ZD6
   24E8 7E 24 CD            987 	jmp	L91
   05F0                     988 L90:
   24EB DC 00               989 	ldd	*ZD7
   24ED D3 00               990 	addd	*ZD6
   24EF DD 00               991 	std	*ZD0	; addhi3: *ZD7 by *ZD6 -> *ZD0
                            992 ;;;EPILOGUE
   24F1 38                  993 	pulx	; Pulling register *ZD7
   24F2 DF 00               994 	stx	*ZD7
   24F4 38                  995 	pulx	; Pulling register *ZD6
   24F5 DF 00               996 	stx	*ZD6
   24F7 38                  997 	pulx	; Pulling register *ZD5
   24F8 DF 00               998 	stx	*ZD5
   24FA 18 38               999 	puly	; Restore stack frame
   24FC 39                 1000 	rts	; return from function
                           1001 ;;;-----------------------------------------
                           1002 ;;; END EPILOGUE for string_SkipSpaces
                           1003 ;;;-----------------------------------------
                           1004 	; END
