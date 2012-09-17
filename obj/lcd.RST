                              1 ;;;-----------------------------------------
                              2 ;;; Start MC6811 gcc assembly output
                              3 ;;; gcc compiler compiled on TBD
                              4 ;;; OPTIONS:	-mlong_branch optimize  
                              5 ;;; OPTIONS:	 peephole !omit_frame_pointer !signed-char
                              6 ;;; Source:		services\lcd.c
                              7 ;;; Destination:	lcd.s
                              8 ;;; Compiled:	Tue Oct 24 21:55:14 2000
                              9 ;;; (META)compiled by GNU C version 2.8.1.
                             10 ;;;-----------------------------------------
                             11 	.module lcd.c
                             12 	.area	_DATA
   0000                      13 _current_row:
   273F 00                   14 	.byte	0
   0001                      15 _current_col:
   2740 00                   16 	.byte	0
                             17 	; extern	_presto_i2c_init
                             18 	; extern	_presto_i2c_send
                             19 	; extern	_lcd_clear
                             20 	.area	_CODE
                             21 	.globl	_lcd_init
   0000                      22 _lcd_init:
                             23 ;;;-----------------------------------------
                             24 ;;;  PROLOGUE for lcd_init
                             25 ;;;-----------------------------------------
   107C 3C                   26 	pshx	; allocate 2 byte auto variable
   107D 3C                   27 	pshx	; allocate 2 byte auto variable
   107E 3C                   28 	pshx	; allocate 2 byte auto variable
   107F 18 3C                29 	pshy	; Save stack frame
   1081 18 30                30 	tsy	; Set current stack frame
                             31 ;;;END PROLOGUE
   1083 C6 74                32 	ldab	#116
   1085 18 E7 02             33 	stab	2,y	; movqi: #116 -> 2,y
   1088 C6 00                34 	ldab	#0
   108A 18 E7 03             35 	stab	3,y	; movqi: #0 -> 3,y
   108D C6 3E                36 	ldab	#62
   108F 18 E7 04             37 	stab	4,y	; movqi: #62 -> 4,y
   1092 C6 0C                38 	ldab	#12
   1094 18 E7 05             39 	stab	5,y	; movqi: #12 -> 5,y
   1097 C6 06                40 	ldab	#6
   1099 18 E7 06             41 	stab	6,y	; movqi: #6 -> 6,y
   109C C6 A0                42 	ldab	#160
   109E 18 E7 07             43 	stab	7,y	; movqi: #160 -> 7,y
   10A1 BD 14 33             44 	jsr	_presto_i2c_init	; CALL: (VOIDmode) _presto_i2c_init (0 bytes)
   10A4 C6 06                45 	ldab	#6
   10A6 37                   46 	pshb		; movqi: #6 -> ,-sp
   10A7 18 DF 00             47 	sty	*ZXT
   10AA DC 00                48 	ldd	*ZXT
   10AC C3 00 02             49 	addd	#2
   10AF DD 00                50 	std	*ZD0	; addhi3: y by #2 -> *ZD0
   10B1 BD 14 90             51 	jsr	_presto_i2c_send	; CALL: (VOIDmode) _presto_i2c_send (1 bytes)
   10B4 BD 10 C1             52 	jsr	_lcd_clear	; CALL: (VOIDmode) _lcd_clear (0 bytes)
   10B7 BD 13 02             53 	jsr	_send_entire_display	; CALL: (VOIDmode) _send_entire_display (0 bytes)
   10BA 33                   54 	pulb		; addhi3: sp by #1 -> sp
                             55 ;;;EPILOGUE
   10BB 18 38                56 	puly	; Restore stack frame
   10BD 38                   57 	pulx	; deallocate 2 byte auto variable
   10BE 38                   58 	pulx	; deallocate 2 byte auto variable
   10BF 38                   59 	pulx	; deallocate 2 byte auto variable
   10C0 39                   60 	rts	; return from function
                             61 ;;;-----------------------------------------
                             62 ;;; END EPILOGUE for lcd_init
                             63 ;;;-----------------------------------------
                             64 	.globl	_lcd_clear
   0045                      65 _lcd_clear:
                             66 ;;;-----------------------------------------
                             67 ;;;  PROLOGUE for lcd_clear
                             68 ;;;-----------------------------------------
   10C1 18 3C                69 	pshy	; Save stack frame
   10C3 18 30                70 	tsy	; Set current stack frame
   10C5 DE 00                71 	ldx	*ZD5
   10C7 3C                   72 	pshx	; pushed register *ZD5
   10C8 DE 00                73 	ldx	*ZD6
   10CA 3C                   74 	pshx	; pushed register *ZD6
   10CB DE 00                75 	ldx	*ZD7
   10CD 3C                   76 	pshx	; pushed register *ZD7
   10CE 8F                   77 	xgdx	; saving D register
   10CF D6 00                78 	ldab	*ZB1
   10D1 37                   79 	pshb	; pushed register *ZB1
   10D2 8F                   80 	xgdx	; restoring D register
                             81 ;;;END PROLOGUE
   10D3 C6 00                82 	ldab	#0
   10D5 D7 01                83 	stab	*ZD7+1	; movqi: #0 -> *ZD7
   10D7 CC 00 00             84 	ldd	#0
   10DA DD 00                85 	std	*ZD6	; movhi: #0 -> *ZD6
   0060                      86 L6:
   10DC C6 00                87 	ldab	#0
   10DE D7 00                88 	stab	*ZB1	; movqi: #0 -> *ZB1
   0064                      89 L10:
   10E0 D6 00                90 	ldab	*ZB1
   10E2 4F                   91 	clra
   10E3 DD 00                92 	std	*ZD5	; zero_extendqihi2: *ZB1 -> *ZD5
   10E5 DC 00                93 	ldd	*ZD5
   10E7 D3 00                94 	addd	*ZD6
   10E9 DD 00                95 	std	*ZD5	; addhi3: *ZD5 by *ZD6 -> *ZD5
   10EB C6 A0                96 	ldab	#160
   10ED E7 00                97 	stab	0,x	; movqi: #160 -> 0,x
   10EF 7C 00 00             98 	inc	*ZB1	; addqi3: *ZB1 by #1 -> *ZB1
   10F2 D6 00                99 	ldab	*ZB1
   10F4 C1 0B               100 	cmpb	#11	; cmpqi *ZB1 with #11
   10F6 22 03               101 	bhi	.+5
   10F8 7E 10 E0            102 	jmp	L10	; (bleu) long branch
   10FB DC 00               103 	ldd	*ZD6
   10FD C3 00 0C            104 	addd	#12
   1100 DD 00               105 	std	*ZD6	; addhi3: *ZD6 by #12 -> *ZD6
   1102 7C 00 01            106 	inc	*ZD7+1	; addqi3: *ZD7 by #1 -> *ZD7
   1105 D6 01               107 	ldab	*ZD7+1
   1107 C1 02               108 	cmpb	#2	; cmpqi *ZD7 with #2
   1109 22 03               109 	bhi	.+5
   110B 7E 10 DC            110 	jmp	L6	; (bleu) long branch
   110E C6 00               111 	ldab	#0
   1110 F7 27 3F            112 	stab	_current_row	; movqi: #0 -> _current_row
   1113 C6 00               113 	ldab	#0
   1115 F7 27 40            114 	stab	_current_col	; movqi: #0 -> _current_col
                            115 ;;;EPILOGUE
   1118 8F                  116 	xgdx	; saving D register
   1119 33                  117 	pulb	; Pulling register *ZB1
   111A D7 00               118 	stab	*ZB1
   111C 8F                  119 	xgdx	; restoring D register
   111D 38                  120 	pulx	; Pulling register *ZD7
   111E DF 00               121 	stx	*ZD7
   1120 38                  122 	pulx	; Pulling register *ZD6
   1121 DF 00               123 	stx	*ZD6
   1123 38                  124 	pulx	; Pulling register *ZD5
   1124 DF 00               125 	stx	*ZD5
   1126 18 38               126 	puly	; Restore stack frame
   1128 39                  127 	rts	; return from function
                            128 ;;;-----------------------------------------
                            129 ;;; END EPILOGUE for lcd_clear
                            130 ;;;-----------------------------------------
                            131 	; extern	_lcd_goto_xy
                            132 	.globl	_lcd_print_at
   00AD                     133 _lcd_print_at:
                            134 ;;;-----------------------------------------
                            135 ;;;  PROLOGUE for lcd_print_at
                            136 ;;;-----------------------------------------
   1129 18 3C               137 	pshy	; Save stack frame
   112B 18 30               138 	tsy	; Set current stack frame
   112D DE 00               139 	ldx	*ZD5
   112F 3C                  140 	pshx	; pushed register *ZD5
   1130 DE 00               141 	ldx	*ZD6
   1132 3C                  142 	pshx	; pushed register *ZD6
   1133 DE 00               143 	ldx	*ZD7
   1135 3C                  144 	pshx	; pushed register *ZD7
                            145 ;;;END PROLOGUE
   1136 D6 01               146 	ldab	*ZD0+1
   1138 D7 01               147 	stab	*ZD7+1	; movqi: *ZD0 -> *ZD7
   113A 18 E6 04            148 	ldab	4,y
   113D D7 01               149 	stab	*ZD6+1	; movqi: 4,y -> *ZD6
   113F 18 EC 05            150 	ldd	5,y
   1142 DD 00               151 	std	*ZD5	; movhi: 5,y -> *ZD5
   1144 D6 01               152 	ldab	*ZD6+1
   1146 37                  153 	pshb		; movqi: *ZD6 -> ,-sp
   1147 BD 11 65            154 	jsr	_lcd_goto_xy	; CALL: (VOIDmode) _lcd_goto_xy (1 bytes)
   114A DE 00               155 	ldx	*ZD5
   114C 3C                  156 	pshx		; movhi: *ZD5 -> ,--sp
   114D D6 01               157 	ldab	*ZD6+1
   114F 37                  158 	pshb		; movqi: *ZD6 -> ,-sp
   1150 D6 01               159 	ldab	*ZD7+1
   1152 D7 01               160 	stab	*ZD0+1	; movqi: *ZD7 -> *ZD0
   1154 BD 11 29            161 	jsr	_lcd_print_at	; CALL: (VOIDmode) _lcd_print_at (3 bytes)
   1157 38                  162 	pulx
   1158 38                  163 	pulx		; addhi3: sp by #4 -> sp
                            164 ;;;EPILOGUE
   1159 38                  165 	pulx	; Pulling register *ZD7
   115A DF 00               166 	stx	*ZD7
   115C 38                  167 	pulx	; Pulling register *ZD6
   115D DF 00               168 	stx	*ZD6
   115F 38                  169 	pulx	; Pulling register *ZD5
   1160 DF 00               170 	stx	*ZD5
   1162 18 38               171 	puly	; Restore stack frame
   1164 39                  172 	rts	; return from function
                            173 ;;;-----------------------------------------
                            174 ;;; END EPILOGUE for lcd_print_at
                            175 ;;;-----------------------------------------
                            176 	.globl	_lcd_goto_xy
   00E9                     177 _lcd_goto_xy:
                            178 ;;;-----------------------------------------
                            179 ;;;  PROLOGUE for lcd_goto_xy
                            180 ;;;-----------------------------------------
   1165 18 3C               181 	pshy	; Save stack frame
   1167 18 30               182 	tsy	; Set current stack frame
   1169 DE 00               183 	ldx	*ZD5
   116B 3C                  184 	pshx	; pushed register *ZD5
                            185 ;;;END PROLOGUE
   116C 18 E6 04            186 	ldab	4,y
   116F D7 01               187 	stab	*ZD5+1	; movqi: 4,y -> *ZD5
   1171 D6 01               188 	ldab	*ZD5+1
   1173 C1 0B               189 	cmpb	#11	; cmpqi *ZD5 with #11
   1175 23 03               190 	bls	.+5
   1177 7E 11 8D            191 	jmp	L14	; (bgtu) long branch
   117A D6 01               192 	ldab	*ZD0+1
   117C C1 02               193 	cmpb	#2	; cmpqi *ZD0 with #2
   117E 23 03               194 	bls	.+5
   1180 7E 11 8D            195 	jmp	L14	; (bgtu) long branch
   1183 D6 01               196 	ldab	*ZD0+1
   1185 F7 27 3F            197 	stab	_current_row	; movqi: *ZD0 -> _current_row
   1188 D6 01               198 	ldab	*ZD5+1
   118A F7 27 40            199 	stab	_current_col	; movqi: *ZD5 -> _current_col
   0111                     200 L14:
                            201 ;;;EPILOGUE
   118D 38                  202 	pulx	; Pulling register *ZD5
   118E DF 00               203 	stx	*ZD5
   1190 18 38               204 	puly	; Restore stack frame
   1192 39                  205 	rts	; return from function
                            206 ;;;-----------------------------------------
                            207 ;;; END EPILOGUE for lcd_goto_xy
                            208 ;;;-----------------------------------------
                            209 	; extern	_ascii_to_lcd
                            210 	.globl	_lcd_print
   0117                     211 _lcd_print:
                            212 ;;;-----------------------------------------
                            213 ;;;  PROLOGUE for lcd_print
                            214 ;;;-----------------------------------------
   1193 18 3C               215 	pshy	; Save stack frame
   1195 18 30               216 	tsy	; Set current stack frame
   1197 DE 00               217 	ldx	*ZD5
   1199 3C                  218 	pshx	; pushed register *ZD5
   119A DE 00               219 	ldx	*ZD6
   119C 3C                  220 	pshx	; pushed register *ZD6
   119D DE 00               221 	ldx	*ZD7
   119F 3C                  222 	pshx	; pushed register *ZD7
   11A0 8F                  223 	xgdx	; saving D register
   11A1 D6 00               224 	ldab	*ZB1
   11A3 37                  225 	pshb	; pushed register *ZB1
   11A4 8F                  226 	xgdx	; restoring D register
                            227 ;;;END PROLOGUE
   11A5 DC 00               228 	ldd	*ZD0
   11A7 DD 00               229 	std	*ZD7	; movhi: *ZD0 -> *ZD7
   11A9 C6 00               230 	ldab	#0
   11AB D7 00               231 	stab	*ZB1	; movqi: #0 -> *ZB1
   11AD DE 00               232 	ldx	*ZD7
   11AF 6D 00               233 	tst	0,x	; tstqi: MEM:0,x
   11B1 26 03               234 	bne	.+5
   11B3 7E 12 1D            235 	jmp	L19	; (beq) long branch
   013A                     236 L20:
   11B6 D6 00               237 	ldab	*ZB1
   11B8 4F                  238 	clra
   11B9 DD 00               239 	std	*ZD5	; zero_extendqihi2: *ZB1 -> *ZD5
   11BB DC 00               240 	ldd	*ZD7
   11BD D3 00               241 	addd	*ZD5
   11BF DD 00               242 	std	*ZD5	; addhi3: *ZD7 by *ZD5 -> *ZD5
   11C1 DE 00               243 	ldx	*ZD5
   11C3 E6 00               244 	ldab	0,x
   11C5 D7 01               245 	stab	*ZD0+1	; movqi: 0,x -> *ZD0
   11C7 BD 12 31            246 	jsr	_ascii_to_lcd	; CALL: R:*ZD0 = _ascii_to_lcd (0 bytes)
   11CA F6 27 40            247 	ldab	_current_col
   11CD 4F                  248 	clra
   11CE DD 00               249 	std	*ZD6	; zero_extendqihi2: _current_col -> *ZD6
   11D0 F6 27 3F            250 	ldab	_current_row
   11D3 86 0C               251 	ldaa	#12
   11D5 3D                  252 	mul
   11D6 DD 00               253 	std	*ZD5	; umulqihi3: _current_row by #12 -> *ZD5
   11D8 DC 00               254 	ldd	*ZD6
   11DA D3 00               255 	addd	*ZD5
   11DC DD 00               256 	std	*ZD5	; addhi3: *ZD6 by *ZD5 -> *ZD5
   11DE D6 01               257 	ldab	*ZD0+1
   11E0 E7 00               258 	stab	0,x	; movqi: *ZD0 -> 0,x
   11E2 7C 27 40            259 	inc	_current_col	; addqi3: _current_col by #1 -> _current_col
   11E5 F6 27 40            260 	ldab	_current_col
   11E8 C1 0B               261 	cmpb	#11	; cmpqi _current_col with #11
   11EA 22 03               262 	bhi	.+5
   11EC 7E 12 06            263 	jmp	L21	; (bleu) long branch
   11EF C6 00               264 	ldab	#0
   11F1 F7 27 40            265 	stab	_current_col	; movqi: #0 -> _current_col
   11F4 7C 27 3F            266 	inc	_current_row	; addqi3: _current_row by #1 -> _current_row
   11F7 F6 27 3F            267 	ldab	_current_row
   11FA C1 02               268 	cmpb	#2	; cmpqi _current_row with #2
   11FC 22 03               269 	bhi	.+5
   11FE 7E 12 06            270 	jmp	L21	; (bleu) long branch
   1201 C6 00               271 	ldab	#0
   1203 F7 27 3F            272 	stab	_current_row	; movqi: #0 -> _current_row
   018A                     273 L21:
   1206 7C 00 00            274 	inc	*ZB1	; addqi3: *ZB1 by #1 -> *ZB1
   1209 D6 00               275 	ldab	*ZB1
   120B 4F                  276 	clra
   120C DD 00               277 	std	*ZD5	; zero_extendqihi2: *ZB1 -> *ZD5
   120E DC 00               278 	ldd	*ZD7
   1210 D3 00               279 	addd	*ZD5
   1212 DD 00               280 	std	*ZD5	; addhi3: *ZD7 by *ZD5 -> *ZD5
   1214 DE 00               281 	ldx	*ZD5
   1216 6D 00               282 	tst	0,x	; tstqi: MEM:0,x
   1218 27 03               283 	beq	.+5
   121A 7E 11 B6            284 	jmp	L20	; (bne) long branch
   01A1                     285 L19:
   121D BD 13 02            286 	jsr	_send_entire_display	; CALL: (VOIDmode) _send_entire_display (0 bytes)
                            287 ;;;EPILOGUE
   1220 8F                  288 	xgdx	; saving D register
   1221 33                  289 	pulb	; Pulling register *ZB1
   1222 D7 00               290 	stab	*ZB1
   1224 8F                  291 	xgdx	; restoring D register
   1225 38                  292 	pulx	; Pulling register *ZD7
   1226 DF 00               293 	stx	*ZD7
   1228 38                  294 	pulx	; Pulling register *ZD6
   1229 DF 00               295 	stx	*ZD6
   122B 38                  296 	pulx	; Pulling register *ZD5
   122C DF 00               297 	stx	*ZD5
   122E 18 38               298 	puly	; Restore stack frame
   1230 39                  299 	rts	; return from function
                            300 ;;;-----------------------------------------
                            301 ;;; END EPILOGUE for lcd_print
                            302 ;;;-----------------------------------------
                            303 	.globl	_ascii_to_lcd
   01B5                     304 _ascii_to_lcd:
                            305 ;;;-----------------------------------------
                            306 ;;;  PROLOGUE for ascii_to_lcd
                            307 ;;;-----------------------------------------
   1231 18 3C               308 	pshy	; Save stack frame
   1233 18 30               309 	tsy	; Set current stack frame
   1235 DE 00               310 	ldx	*ZD5
   1237 3C                  311 	pshx	; pushed register *ZD5
   1238 8F                  312 	xgdx	; saving D register
   1239 D6 00               313 	ldab	*ZB1
   123B 37                  314 	pshb	; pushed register *ZB1
   123C 8F                  315 	xgdx	; restoring D register
                            316 ;;;END PROLOGUE
   123D D6 01               317 	ldab	*ZD0+1
   123F C1 24               318 	cmpb	#36	; cmpqi *ZD0 with #36
   1241 27 03               319 	beq	.+5
   1243 7E 12 4E            320 	jmp	L25	; (bne) long branch
   1246 CC 00 82            321 	ldd	#130
   1249 DD 00               322 	std	*ZD0	; movhi: #130 -> *ZD0
   124B 7E 12 94            323 	jmp	L33
   01D2                     324 L25:
   124E D6 01               325 	ldab	*ZD0+1
   1250 C1 40               326 	cmpb	#64	; cmpqi *ZD0 with #64
   1252 27 03               327 	beq	.+5
   1254 7E 12 5F            328 	jmp	L27	; (bne) long branch
   1257 CC 00 C0            329 	ldd	#192
   125A DD 00               330 	std	*ZD0	; movhi: #192 -> *ZD0
   125C 7E 12 94            331 	jmp	L33
   01E3                     332 L27:
   125F D6 01               333 	ldab	*ZD0+1
   1261 CB DF               334 	addb	#-33
   1263 D7 01               335 	stab	*ZD5+1	; addqi3: *ZD0 by #-33 -> *ZD5
   1265 D6 01               336 	ldab	*ZD5+1
   1267 C1 39               337 	cmpb	#57	; cmpqi *ZD5 with #57
   1269 22 03               338 	bhi	.+5
   126B 7E 12 85            339 	jmp	L31	; (bleu) long branch
   126E D6 01               340 	ldab	*ZD0+1
   1270 CB 9F               341 	addb	#-97
   1272 D7 01               342 	stab	*ZD5+1	; addqi3: *ZD0 by #-97 -> *ZD5
   1274 D6 01               343 	ldab	*ZD5+1
   1276 C1 19               344 	cmpb	#25	; cmpqi *ZD5 with #25
   1278 22 03               345 	bhi	.+5
   127A 7E 12 85            346 	jmp	L31	; (bleu) long branch
   127D CC 00 A0            347 	ldd	#160
   1280 DD 00               348 	std	*ZD0	; movhi: #160 -> *ZD0
   1282 7E 12 94            349 	jmp	L33
   0209                     350 L31:
   1285 D6 01               351 	ldab	*ZD0+1
   1287 CB 80               352 	addb	#128
   1289 D7 01               353 	stab	*ZD5+1	; addqi3: *ZD0 by #128 -> *ZD5
   128B D6 01               354 	ldab	*ZD5+1
   128D D7 00               355 	stab	*ZB1	; movqi: *ZD5 -> *ZB1
   128F D6 00               356 	ldab	*ZB1
   1291 4F                  357 	clra
   1292 DD 00               358 	std	*ZD0	; zero_extendqihi2: *ZB1 -> *ZD0
   0218                     359 L33:
                            360 ;;;EPILOGUE
   1294 8F                  361 	xgdx	; saving D register
   1295 33                  362 	pulb	; Pulling register *ZB1
   1296 D7 00               363 	stab	*ZB1
   1298 8F                  364 	xgdx	; restoring D register
   1299 38                  365 	pulx	; Pulling register *ZD5
   129A DF 00               366 	stx	*ZD5
   129C 18 38               367 	puly	; Restore stack frame
   129E 39                  368 	rts	; return from function
                            369 ;;;-----------------------------------------
                            370 ;;; END EPILOGUE for ascii_to_lcd
                            371 ;;;-----------------------------------------
                            372 	.globl	_lcd_raw_char
   0223                     373 _lcd_raw_char:
                            374 ;;;-----------------------------------------
                            375 ;;;  PROLOGUE for lcd_raw_char
                            376 ;;;-----------------------------------------
   129F 18 3C               377 	pshy	; Save stack frame
   12A1 18 30               378 	tsy	; Set current stack frame
   12A3 DE 00               379 	ldx	*ZD5
   12A5 3C                  380 	pshx	; pushed register *ZD5
   12A6 DE 00               381 	ldx	*ZD6
   12A8 3C                  382 	pshx	; pushed register *ZD6
   12A9 DE 00               383 	ldx	*ZD7
   12AB 3C                  384 	pshx	; pushed register *ZD7
   12AC 8F                  385 	xgdx	; saving D register
   12AD D6 00               386 	ldab	*ZB1
   12AF 37                  387 	pshb	; pushed register *ZB1
   12B0 8F                  388 	xgdx	; restoring D register
                            389 ;;;END PROLOGUE
   12B1 D6 01               390 	ldab	*ZD0+1
   12B3 D7 01               391 	stab	*ZD7+1	; movqi: *ZD0 -> *ZD7
   12B5 18 E6 04            392 	ldab	4,y
   12B8 D7 00               393 	stab	*ZB1	; movqi: 4,y -> *ZB1
   12BA D6 00               394 	ldab	*ZB1
   12BC C1 0B               395 	cmpb	#11	; cmpqi *ZB1 with #11
   12BE 23 03               396 	bls	.+5
   12C0 7E 12 F1            397 	jmp	L34	; (bgtu) long branch
   12C3 D6 01               398 	ldab	*ZD0+1
   12C5 C1 02               399 	cmpb	#2	; cmpqi *ZD0 with #2
   12C7 23 03               400 	bls	.+5
   12C9 7E 12 F1            401 	jmp	L34	; (bgtu) long branch
   12CC D6 00               402 	ldab	*ZB1
   12CE 4F                  403 	clra
   12CF DD 00               404 	std	*ZD5	; zero_extendqihi2: *ZB1 -> *ZD5
   12D1 D6 01               405 	ldab	*ZD0+1
   12D3 86 0C               406 	ldaa	#12
   12D5 3D                  407 	mul
   12D6 DD 00               408 	std	*ZD6	; umulqihi3: *ZD0 by #12 -> *ZD6
   12D8 DC 00               409 	ldd	*ZD5
   12DA D3 00               410 	addd	*ZD6
   12DC DD 00               411 	std	*ZD5	; addhi3: *ZD5 by *ZD6 -> *ZD5
   12DE 18 E6 05            412 	ldab	5,y
   12E1 E7 00               413 	stab	0,x	; movqi: 5,y -> 0,x
   12E3 BD 13 02            414 	jsr	_send_entire_display	; CALL: (VOIDmode) _send_entire_display (0 bytes)
   12E6 D6 00               415 	ldab	*ZB1
   12E8 37                  416 	pshb		; movqi: *ZB1 -> ,-sp
   12E9 D6 01               417 	ldab	*ZD7+1
   12EB D7 01               418 	stab	*ZD0+1	; movqi: *ZD7 -> *ZD0
   12ED BD 13 A4            419 	jsr	_send_one_char	; CALL: (VOIDmode) _send_one_char (1 bytes)
   12F0 33                  420 	pulb		; addhi3: sp by #1 -> sp
   0275                     421 L34:
                            422 ;;;EPILOGUE
   12F1 8F                  423 	xgdx	; saving D register
   12F2 33                  424 	pulb	; Pulling register *ZB1
   12F3 D7 00               425 	stab	*ZB1
   12F5 8F                  426 	xgdx	; restoring D register
   12F6 38                  427 	pulx	; Pulling register *ZD7
   12F7 DF 00               428 	stx	*ZD7
   12F9 38                  429 	pulx	; Pulling register *ZD6
   12FA DF 00               430 	stx	*ZD6
   12FC 38                  431 	pulx	; Pulling register *ZD5
   12FD DF 00               432 	stx	*ZD5
   12FF 18 38               433 	puly	; Restore stack frame
   1301 39                  434 	rts	; return from function
                            435 ;;;-----------------------------------------
                            436 ;;; END EPILOGUE for lcd_raw_char
                            437 ;;;-----------------------------------------
                            438 	; extern	_presto_i2c_start
                            439 	; extern	_presto_i2c_send_byte
                            440 	; extern	_presto_i2c_stop
   0286                     441 _send_entire_display:
                            442 ;;;-----------------------------------------
                            443 ;;;  PROLOGUE for send_entire_display
                            444 ;;;-----------------------------------------
   1302 18 3C               445 	pshy	; Save stack frame
   1304 18 30               446 	tsy	; Set current stack frame
   1306 DE 00               447 	ldx	*ZD5
   1308 3C                  448 	pshx	; pushed register *ZD5
   1309 DE 00               449 	ldx	*ZD6
   130B 3C                  450 	pshx	; pushed register *ZD6
   130C DE 00               451 	ldx	*ZD7
   130E 3C                  452 	pshx	; pushed register *ZD7
   130F DE 00               453 	ldx	*ZD8
   1311 3C                  454 	pshx	; pushed register *ZD8
   1312 8F                  455 	xgdx	; saving D register
   1313 D6 00               456 	ldab	*ZB1
   1315 37                  457 	pshb	; pushed register *ZB1
   1316 8F                  458 	xgdx	; restoring D register
                            459 ;;;END PROLOGUE
   1317 C6 00               460 	ldab	#0
   1319 D7 01               461 	stab	*ZD8+1	; movqi: #0 -> *ZD8
   131B CC 00 00            462 	ldd	#0
   131E DD 00               463 	std	*ZD6	; movhi: #0 -> *ZD6
   1320 C6 20               464 	ldab	#32
   1322 D7 01               465 	stab	*ZD7+1	; movqi: #32 -> *ZD7
   02A8                     466 L41:
   1324 BD 14 F3            467 	jsr	_presto_i2c_start	; CALL: (VOIDmode) _presto_i2c_start (0 bytes)
   1327 C6 74               468 	ldab	#116
   1329 D7 01               469 	stab	*ZD0+1	; movqi: #116 -> *ZD0
   132B BD 15 20            470 	jsr	_presto_i2c_send_byte	; CALL: (VOIDmode) _presto_i2c_send_byte (0 bytes)
   132E C6 00               471 	ldab	#0
   1330 D7 01               472 	stab	*ZD0+1	; movqi: #0 -> *ZD0
   1332 BD 15 20            473 	jsr	_presto_i2c_send_byte	; CALL: (VOIDmode) _presto_i2c_send_byte (0 bytes)
   1335 D6 01               474 	ldab	*ZD7+1
   1337 CA 80               475 	orab	#-128
   1339 D7 01               476 	stab	*ZD0+1	; iorqi3: *ZD7 by #-128 -> *ZD0
   133B BD 15 20            477 	jsr	_presto_i2c_send_byte	; CALL: (VOIDmode) _presto_i2c_send_byte (0 bytes)
   133E BD 15 D4            478 	jsr	_presto_i2c_stop	; CALL: (VOIDmode) _presto_i2c_stop (0 bytes)
   1341 BD 14 F3            479 	jsr	_presto_i2c_start	; CALL: (VOIDmode) _presto_i2c_start (0 bytes)
   1344 C6 74               480 	ldab	#116
   1346 D7 01               481 	stab	*ZD0+1	; movqi: #116 -> *ZD0
   1348 BD 15 20            482 	jsr	_presto_i2c_send_byte	; CALL: (VOIDmode) _presto_i2c_send_byte (0 bytes)
   134B C6 40               483 	ldab	#64
   134D D7 01               484 	stab	*ZD0+1	; movqi: #64 -> *ZD0
   134F BD 15 20            485 	jsr	_presto_i2c_send_byte	; CALL: (VOIDmode) _presto_i2c_send_byte (0 bytes)
   1352 C6 00               486 	ldab	#0
   1354 D7 00               487 	stab	*ZB1	; movqi: #0 -> *ZB1
   02DA                     488 L45:
   1356 D6 00               489 	ldab	*ZB1
   1358 4F                  490 	clra
   1359 DD 00               491 	std	*ZD5	; zero_extendqihi2: *ZB1 -> *ZD5
   135B DC 00               492 	ldd	*ZD5
   135D D3 00               493 	addd	*ZD6
   135F DD 00               494 	std	*ZD5	; addhi3: *ZD5 by *ZD6 -> *ZD5
   1361 E6 00               495 	ldab	0,x
   1363 D7 01               496 	stab	*ZD0+1	; movqi: 0,x -> *ZD0
   1365 BD 15 20            497 	jsr	_presto_i2c_send_byte	; CALL: (VOIDmode) _presto_i2c_send_byte (0 bytes)
   1368 7C 00 00            498 	inc	*ZB1	; addqi3: *ZB1 by #1 -> *ZB1
   136B D6 00               499 	ldab	*ZB1
   136D C1 0B               500 	cmpb	#11	; cmpqi *ZB1 with #11
   136F 22 03               501 	bhi	.+5
   1371 7E 13 56            502 	jmp	L45	; (bleu) long branch
   1374 BD 15 D4            503 	jsr	_presto_i2c_stop	; CALL: (VOIDmode) _presto_i2c_stop (0 bytes)
   1377 DC 00               504 	ldd	*ZD6
   1379 C3 00 0C            505 	addd	#12
   137C DD 00               506 	std	*ZD6	; addhi3: *ZD6 by #12 -> *ZD6
   137E D6 01               507 	ldab	*ZD7+1
   1380 CB 20               508 	addb	#32
   1382 D7 01               509 	stab	*ZD7+1	; addqi3: *ZD7 by #32 -> *ZD7
   1384 7C 00 01            510 	inc	*ZD8+1	; addqi3: *ZD8 by #1 -> *ZD8
   1387 D6 01               511 	ldab	*ZD8+1
   1389 C1 02               512 	cmpb	#2	; cmpqi *ZD8 with #2
   138B 22 03               513 	bhi	.+5
   138D 7E 13 24            514 	jmp	L41	; (bleu) long branch
                            515 ;;;EPILOGUE
   1390 8F                  516 	xgdx	; saving D register
   1391 33                  517 	pulb	; Pulling register *ZB1
   1392 D7 00               518 	stab	*ZB1
   1394 8F                  519 	xgdx	; restoring D register
   1395 38                  520 	pulx	; Pulling register *ZD8
   1396 DF 00               521 	stx	*ZD8
   1398 38                  522 	pulx	; Pulling register *ZD7
   1399 DF 00               523 	stx	*ZD7
   139B 38                  524 	pulx	; Pulling register *ZD6
   139C DF 00               525 	stx	*ZD6
   139E 38                  526 	pulx	; Pulling register *ZD5
   139F DF 00               527 	stx	*ZD5
   13A1 18 38               528 	puly	; Restore stack frame
   13A3 39                  529 	rts	; return from function
                            530 ;;;-----------------------------------------
                            531 ;;; END EPILOGUE for send_entire_display
                            532 ;;;-----------------------------------------
   0328                     533 _send_one_char:
                            534 ;;;-----------------------------------------
                            535 ;;;  PROLOGUE for send_one_char
                            536 ;;;-----------------------------------------
   13A4 18 3C               537 	pshy	; Save stack frame
   13A6 18 30               538 	tsy	; Set current stack frame
   13A8 DE 00               539 	ldx	*ZD5
   13AA 3C                  540 	pshx	; pushed register *ZD5
   13AB DE 00               541 	ldx	*ZD6
   13AD 3C                  542 	pshx	; pushed register *ZD6
   13AE DE 00               543 	ldx	*ZD7
   13B0 3C                  544 	pshx	; pushed register *ZD7
   13B1 8F                  545 	xgdx	; saving D register
   13B2 D6 00               546 	ldab	*ZB1
   13B4 37                  547 	pshb	; pushed register *ZB1
   13B5 8F                  548 	xgdx	; restoring D register
                            549 ;;;END PROLOGUE
   13B6 D6 01               550 	ldab	*ZD0+1
   13B8 D7 01               551 	stab	*ZD7+1	; movqi: *ZD0 -> *ZD7
   13BA 18 E6 04            552 	ldab	4,y
   13BD D7 01               553 	stab	*ZD6+1	; movqi: 4,y -> *ZD6
   13BF BD 14 F3            554 	jsr	_presto_i2c_start	; CALL: (VOIDmode) _presto_i2c_start (0 bytes)
   13C2 C6 74               555 	ldab	#116
   13C4 D7 01               556 	stab	*ZD0+1	; movqi: #116 -> *ZD0
   13C6 BD 15 20            557 	jsr	_presto_i2c_send_byte	; CALL: (VOIDmode) _presto_i2c_send_byte (0 bytes)
   13C9 C6 00               558 	ldab	#0
   13CB D7 01               559 	stab	*ZD0+1	; movqi: #0 -> *ZD0
   13CD BD 15 20            560 	jsr	_presto_i2c_send_byte	; CALL: (VOIDmode) _presto_i2c_send_byte (0 bytes)
   13D0 D6 01               561 	ldab	*ZD7+1
   13D2 CB 01               562 	addb	#1
   13D4 D7 01               563 	stab	*ZD5+1	; addqi3: *ZD7 by #1 -> *ZD5
   13D6 D6 01               564 	ldab	*ZD5+1
   13D8 58                  565 	aslb
   13D9 58                  566 	aslb
   13DA 58                  567 	aslb
   13DB 58                  568 	aslb
   13DC 58                  569 	aslb
   13DD D7 01               570 	stab	*ZD5+1	; ashlqi3: *ZD5 by #5 -> *ZD5
   13DF D6 01               571 	ldab	*ZD5+1
   13E1 DB 01               572 	addb	*ZD6+1
   13E3 D7 01               573 	stab	*ZD5+1	; addqi3: *ZD5 by *ZD6 -> *ZD5
   13E5 D6 01               574 	ldab	*ZD5+1
   13E7 CA 80               575 	orab	#-128
   13E9 D7 01               576 	stab	*ZD0+1	; iorqi3: *ZD5 by #-128 -> *ZD0
   13EB BD 15 20            577 	jsr	_presto_i2c_send_byte	; CALL: (VOIDmode) _presto_i2c_send_byte (0 bytes)
   13EE BD 15 D4            578 	jsr	_presto_i2c_stop	; CALL: (VOIDmode) _presto_i2c_stop (0 bytes)
   13F1 BD 14 F3            579 	jsr	_presto_i2c_start	; CALL: (VOIDmode) _presto_i2c_start (0 bytes)
   13F4 C6 74               580 	ldab	#116
   13F6 D7 01               581 	stab	*ZD0+1	; movqi: #116 -> *ZD0
   13F8 BD 15 20            582 	jsr	_presto_i2c_send_byte	; CALL: (VOIDmode) _presto_i2c_send_byte (0 bytes)
   13FB C6 40               583 	ldab	#64
   13FD D7 01               584 	stab	*ZD0+1	; movqi: #64 -> *ZD0
   13FF BD 15 20            585 	jsr	_presto_i2c_send_byte	; CALL: (VOIDmode) _presto_i2c_send_byte (0 bytes)
   1402 D6 01               586 	ldab	*ZD6+1
   1404 D7 00               587 	stab	*ZB1	; movqi: *ZD6 -> *ZB1
   1406 D6 00               588 	ldab	*ZB1
   1408 4F                  589 	clra
   1409 DD 00               590 	std	*ZD6	; zero_extendqihi2: *ZB1 -> *ZD6
   140B D6 01               591 	ldab	*ZD7+1
   140D 86 0C               592 	ldaa	#12
   140F 3D                  593 	mul
   1410 DD 00               594 	std	*ZD5	; umulqihi3: *ZD7 by #12 -> *ZD5
   1412 DC 00               595 	ldd	*ZD6
   1414 D3 00               596 	addd	*ZD5
   1416 DD 00               597 	std	*ZD5	; addhi3: *ZD6 by *ZD5 -> *ZD5
   1418 E6 00               598 	ldab	0,x
   141A D7 01               599 	stab	*ZD0+1	; movqi: 0,x -> *ZD0
   141C BD 15 20            600 	jsr	_presto_i2c_send_byte	; CALL: (VOIDmode) _presto_i2c_send_byte (0 bytes)
   141F BD 15 D4            601 	jsr	_presto_i2c_stop	; CALL: (VOIDmode) _presto_i2c_stop (0 bytes)
                            602 ;;;EPILOGUE
   1422 8F                  603 	xgdx	; saving D register
   1423 33                  604 	pulb	; Pulling register *ZB1
   1424 D7 00               605 	stab	*ZB1
   1426 8F                  606 	xgdx	; restoring D register
   1427 38                  607 	pulx	; Pulling register *ZD7
   1428 DF 00               608 	stx	*ZD7
   142A 38                  609 	pulx	; Pulling register *ZD6
   142B DF 00               610 	stx	*ZD6
   142D 38                  611 	pulx	; Pulling register *ZD5
   142E DF 00               612 	stx	*ZD5
   1430 18 38               613 	puly	; Restore stack frame
   1432 39                  614 	rts	; return from function
                            615 ;;;-----------------------------------------
                            616 ;;; END EPILOGUE for send_one_char
                            617 ;;;-----------------------------------------
                            618 	.area	_BSS
   0000                     619 _display:	.blkb	36
                            620 	; END
