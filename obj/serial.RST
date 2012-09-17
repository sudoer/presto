                              1 ;;;-----------------------------------------
                              2 ;;; Start MC6811 gcc assembly output
                              3 ;;; gcc compiler compiled on TBD
                              4 ;;; OPTIONS:	-mlong_branch optimize  
                              5 ;;; OPTIONS:	 peephole !omit_frame_pointer !signed-char
                              6 ;;; Source:		services\serial.c
                              7 ;;; Destination:	serial.s
                              8 ;;; Compiled:	Tue Oct 24 21:55:25 2000
                              9 ;;; (META)compiled by GNU C version 2.8.1.
                             10 ;;;-----------------------------------------
                             11 	.module serial.c
                             12 	; extern	_set_interrupt
                             13 	.area	_CODE
                             14 	.globl	_serial_init
   0000                      15 _serial_init:
                             16 ;;;-----------------------------------------
                             17 ;;;  PROLOGUE for serial_init
                             18 ;;;-----------------------------------------
   1A71 18 3C                19 	pshy	; Save stack frame
   1A73 18 30                20 	tsy	; Set current stack frame
                             21 ;;;END PROLOGUE
                             22 ; Begin inline assembler code
   1A75 0F                   23 		sei
                             24 ; End of inline assembler code
   1A76 C6 10                25 	ldab	#16
   1A78 F7 10 2C             26 	stab	4140	; movqi: #16 -> 4140
   1A7B C6 AC                27 	ldab	#172
   1A7D F7 10 2D             28 	stab	4141	; movqi: #172 -> 4141
   1A80 C6 30                29 	ldab	#48
   1A82 F7 10 2B             30 	stab	4139	; movqi: #48 -> 4139
   1A85 BD 1A 99             31 	jsr	_clear_buffers	; CALL: (VOIDmode) _clear_buffers (0 bytes)
   1A88 CE 1C 18             32 	ldx	#_presto_serial_isr
   1A8B 3C                   33 	pshx		; movhi: #_presto_serial_isr -> ,--sp
   1A8C CC 00 00             34 	ldd	#0
   1A8F DD 00                35 	std	*ZD0	; movhi: #0 -> *ZD0
   1A91 BD 08 51             36 	jsr	_set_interrupt	; CALL: R:*ZD0 = _set_interrupt (2 bytes)
                             37 ; Begin inline assembler code
   1A94 0E                   38 		cli
                             39 ; End of inline assembler code
   1A95 38                   40 	pulx		; addhi3: sp by #2 -> sp
                             41 ;;;EPILOGUE
   1A96 18 38                42 	puly	; Restore stack frame
   1A98 39                   43 	rts	; return from function
                             44 ;;;-----------------------------------------
                             45 ;;; END EPILOGUE for serial_init
                             46 ;;;-----------------------------------------
   0028                      47 _clear_buffers:
                             48 ;;;-----------------------------------------
                             49 ;;;  PROLOGUE for clear_buffers
                             50 ;;;-----------------------------------------
   1A99 18 3C                51 	pshy	; Save stack frame
   1A9B 18 30                52 	tsy	; Set current stack frame
                             53 ;;;END PROLOGUE
   1A9D C6 14                54 	ldab	#20
   1A9F 37                   55 	pshb		; movqi: #20 -> ,-sp
   1AA0 CE A0 88             56 	ldx	#_com1_rx_buffer
   1AA3 3C                   57 	pshx		; movhi: #_com1_rx_buffer -> ,--sp
   1AA4 CC A0 78             58 	ldd	#_com1_rx_queue
   1AA7 DD 00                59 	std	*ZD0	; movhi: #_com1_rx_queue -> *ZD0
   1AA9 BD 1C 8C             60 	jsr	_cq_init	; CALL: (VOIDmode) _cq_init (3 bytes)
   1AAC C6 FF                61 	ldab	#255
   1AAE 37                   62 	pshb		; movqi: #255 -> ,-sp
   1AAF CE A0 9C             63 	ldx	#_com1_tx_buffer
   1AB2 3C                   64 	pshx		; movhi: #_com1_tx_buffer -> ,--sp
   1AB3 CC A0 80             65 	ldd	#_com1_tx_queue
   1AB6 DD 00                66 	std	*ZD0	; movhi: #_com1_tx_queue -> *ZD0
   1AB8 BD 1C 8C             67 	jsr	_cq_init	; CALL: (VOIDmode) _cq_init (3 bytes)
   1ABB 38                   68 	pulx
   1ABC 38                   69 	pulx
   1ABD 38                   70 	pulx		; addhi3: sp by #6 -> sp
                             71 ;;;EPILOGUE
   1ABE 18 38                72 	puly	; Restore stack frame
   1AC0 39                   73 	rts	; return from function
                             74 ;;;-----------------------------------------
                             75 ;;; END EPILOGUE for clear_buffers
                             76 ;;;-----------------------------------------
                             77 	.globl	_serial_send_byte
   0050                      78 _serial_send_byte:
                             79 ;;;-----------------------------------------
                             80 ;;;  PROLOGUE for serial_send_byte
                             81 ;;;-----------------------------------------
   1AC1 18 3C                82 	pshy	; Save stack frame
   1AC3 18 30                83 	tsy	; Set current stack frame
   1AC5 DE 00                84 	ldx	*ZD5
   1AC7 3C                   85 	pshx	; pushed register *ZD5
                             86 ;;;END PROLOGUE
   1AC8 F6 10 2D             87 	ldab	4141
   1ACB D7 01                88 	stab	*ZD5+1	; movqi: 4141 -> *ZD5
   1ACD D6 01                89 	ldab	*ZD5+1
   1ACF C4 7F                90 	andb	#127
   1AD1 D7 01                91 	stab	*ZD5+1	; andqi3: *ZD5 by #127 -> *ZD5
   1AD3 D6 01                92 	ldab	*ZD5+1
   1AD5 F7 10 2D             93 	stab	4141	; movqi: *ZD5 -> 4141
   1AD8 D6 01                94 	ldab	*ZD0+1
   1ADA 37                   95 	pshb		; movqi: *ZD0 -> ,-sp
   1ADB CC A0 80             96 	ldd	#_com1_tx_queue
   1ADE DD 00                97 	std	*ZD0	; movhi: #_com1_tx_queue -> *ZD0
   1AE0 BD 1D 4F             98 	jsr	_cq_put_byte	; CALL: R:*ZD0 = _cq_put_byte (1 bytes)
   1AE3 F6 10 2D             99 	ldab	4141
   1AE6 D7 01               100 	stab	*ZD5+1	; movqi: 4141 -> *ZD5
   1AE8 D6 01               101 	ldab	*ZD5+1
   1AEA CA 80               102 	orab	#128
   1AEC D7 01               103 	stab	*ZD5+1	; iorqi3: *ZD5 by #128 -> *ZD5
   1AEE D6 01               104 	ldab	*ZD5+1
   1AF0 F7 10 2D            105 	stab	4141	; movqi: *ZD5 -> 4141
   1AF3 33                  106 	pulb		; addhi3: sp by #1 -> sp
                            107 ;;;EPILOGUE
   1AF4 38                  108 	pulx	; Pulling register *ZD5
   1AF5 DF 00               109 	stx	*ZD5
   1AF7 18 38               110 	puly	; Restore stack frame
   1AF9 39                  111 	rts	; return from function
                            112 ;;;-----------------------------------------
                            113 ;;; END EPILOGUE for serial_send_byte
                            114 ;;;-----------------------------------------
                            115 	.globl	_serial_send_string
   0089                     116 _serial_send_string:
                            117 ;;;-----------------------------------------
                            118 ;;;  PROLOGUE for serial_send_string
                            119 ;;;-----------------------------------------
   1AFA 18 3C               120 	pshy	; Save stack frame
   1AFC 18 30               121 	tsy	; Set current stack frame
   1AFE DE 00               122 	ldx	*ZD5
   1B00 3C                  123 	pshx	; pushed register *ZD5
   1B01 DE 00               124 	ldx	*ZD6
   1B03 3C                  125 	pshx	; pushed register *ZD6
                            126 ;;;END PROLOGUE
   1B04 DC 00               127 	ldd	*ZD0
   1B06 DD 00               128 	std	*ZD6	; movhi: *ZD0 -> *ZD6
   1B08 F6 10 2D            129 	ldab	4141
   1B0B D7 01               130 	stab	*ZD5+1	; movqi: 4141 -> *ZD5
   1B0D D6 01               131 	ldab	*ZD5+1
   1B0F C4 7F               132 	andb	#127
   1B11 D7 01               133 	stab	*ZD5+1	; andqi3: *ZD5 by #127 -> *ZD5
   1B13 D6 01               134 	ldab	*ZD5+1
   1B15 F7 10 2D            135 	stab	4141	; movqi: *ZD5 -> 4141
   1B18 DE 00               136 	ldx	*ZD6
   1B1A 6D 00               137 	tst	0,x	; tstqi: MEM:0,x
   1B1C 26 03               138 	bne	.+5
   1B1E 7E 1B 3D            139 	jmp	L6	; (beq) long branch
   00B0                     140 L7:
   1B21 DE 00               141 	ldx	*ZD6
   1B23 E6 00               142 	ldab	0,x
   1B25 37                  143 	pshb		; movqi: 0,x -> ,-sp
   1B26 CC A0 80            144 	ldd	#_com1_tx_queue
   1B29 DD 00               145 	std	*ZD0	; movhi: #_com1_tx_queue -> *ZD0
   1B2B BD 1D 4F            146 	jsr	_cq_put_byte	; CALL: R:*ZD0 = _cq_put_byte (1 bytes)
   1B2E DE 00               147 	ldx	*ZD6
   1B30 08                  148 	inx
   1B31 DF 00               149 	stx	*ZD6	; addhi3: *ZD6 by #1 -> *ZD6
   1B33 33                  150 	pulb		; addhi3: sp by #1 -> sp
   1B34 DE 00               151 	ldx	*ZD6
   1B36 6D 00               152 	tst	0,x	; tstqi: MEM:0,x
   1B38 27 03               153 	beq	.+5
   1B3A 7E 1B 21            154 	jmp	L7	; (bne) long branch
   00CC                     155 L6:
   1B3D F6 10 2D            156 	ldab	4141
   1B40 D7 01               157 	stab	*ZD5+1	; movqi: 4141 -> *ZD5
   1B42 D6 01               158 	ldab	*ZD5+1
   1B44 CA 80               159 	orab	#128
   1B46 D7 01               160 	stab	*ZD5+1	; iorqi3: *ZD5 by #128 -> *ZD5
   1B48 D6 01               161 	ldab	*ZD5+1
   1B4A F7 10 2D            162 	stab	4141	; movqi: *ZD5 -> 4141
                            163 ;;;EPILOGUE
   1B4D 38                  164 	pulx	; Pulling register *ZD6
   1B4E DF 00               165 	stx	*ZD6
   1B50 38                  166 	pulx	; Pulling register *ZD5
   1B51 DF 00               167 	stx	*ZD5
   1B53 18 38               168 	puly	; Restore stack frame
   1B55 39                  169 	rts	; return from function
                            170 ;;;-----------------------------------------
                            171 ;;; END EPILOGUE for serial_send_string
                            172 ;;;-----------------------------------------
                            173 	.globl	_serial_recv
   00E5                     174 _serial_recv:
                            175 ;;;-----------------------------------------
                            176 ;;;  PROLOGUE for serial_recv
                            177 ;;;-----------------------------------------
   1B56 18 3C               178 	pshy	; Save stack frame
   1B58 18 30               179 	tsy	; Set current stack frame
   1B5A DE 00               180 	ldx	*ZD5
   1B5C 3C                  181 	pshx	; pushed register *ZD5
   1B5D 8F                  182 	xgdx	; saving D register
   1B5E D6 00               183 	ldab	*ZB1
   1B60 37                  184 	pshb	; pushed register *ZB1
   1B61 8F                  185 	xgdx	; restoring D register
                            186 ;;;END PROLOGUE
   1B62 F6 10 2D            187 	ldab	4141
   1B65 D7 01               188 	stab	*ZD5+1	; movqi: 4141 -> *ZD5
   1B67 D6 01               189 	ldab	*ZD5+1
   1B69 C4 DF               190 	andb	#223
   1B6B D7 01               191 	stab	*ZD5+1	; andqi3: *ZD5 by #223 -> *ZD5
   1B6D D6 01               192 	ldab	*ZD5+1
   1B6F F7 10 2D            193 	stab	4141	; movqi: *ZD5 -> 4141
   1B72 DE 00               194 	ldx	*ZD0
   1B74 3C                  195 	pshx		; movhi: *ZD0 -> ,--sp
   1B75 CC A0 78            196 	ldd	#_com1_rx_queue
   1B78 DD 00               197 	std	*ZD0	; movhi: #_com1_rx_queue -> *ZD0
   1B7A BD 1C BD            198 	jsr	_cq_get_byte	; CALL: R:*ZD0 = _cq_get_byte (2 bytes)
   1B7D F6 10 2D            199 	ldab	4141
   1B80 D7 01               200 	stab	*ZD5+1	; movqi: 4141 -> *ZD5
   1B82 D6 01               201 	ldab	*ZD5+1
   1B84 CA 20               202 	orab	#32
   1B86 D7 01               203 	stab	*ZD5+1	; iorqi3: *ZD5 by #32 -> *ZD5
   1B88 D6 01               204 	ldab	*ZD5+1
   1B8A F7 10 2D            205 	stab	4141	; movqi: *ZD5 -> 4141
   1B8D D6 01               206 	ldab	*ZD0+1
   1B8F D7 00               207 	stab	*ZB1	; movqi: *ZD0 -> *ZB1
   1B91 D6 00               208 	ldab	*ZB1
   1B93 4F                  209 	clra
   1B94 DD 00               210 	std	*ZD0	; zero_extendqihi2: *ZB1 -> *ZD0
   1B96 38                  211 	pulx		; addhi3: sp by #2 -> sp
                            212 ;;;EPILOGUE
   1B97 8F                  213 	xgdx	; saving D register
   1B98 33                  214 	pulb	; Pulling register *ZB1
   1B99 D7 00               215 	stab	*ZB1
   1B9B 8F                  216 	xgdx	; restoring D register
   1B9C 38                  217 	pulx	; Pulling register *ZD5
   1B9D DF 00               218 	stx	*ZD5
   1B9F 18 38               219 	puly	; Restore stack frame
   1BA1 39                  220 	rts	; return from function
                            221 ;;;-----------------------------------------
                            222 ;;; END EPILOGUE for serial_recv
                            223 ;;;-----------------------------------------
                            224 	.globl	_serial_recv_string
   0131                     225 _serial_recv_string:
                            226 ;;;-----------------------------------------
                            227 ;;;  PROLOGUE for serial_recv_string
                            228 ;;;-----------------------------------------
   1BA2 18 3C               229 	pshy	; Save stack frame
   1BA4 18 30               230 	tsy	; Set current stack frame
   1BA6 DE 00               231 	ldx	*ZD5
   1BA8 3C                  232 	pshx	; pushed register *ZD5
   1BA9 DE 00               233 	ldx	*ZD6
   1BAB 3C                  234 	pshx	; pushed register *ZD6
   1BAC DE 00               235 	ldx	*ZD7
   1BAE 3C                  236 	pshx	; pushed register *ZD7
                            237 ;;;END PROLOGUE
   1BAF CC 00 00            238 	ldd	#0
   1BB2 DD 00               239 	std	*ZD6	; movhi: #0 -> *ZD6
   1BB4 DC 00               240 	ldd	*ZD0
   1BB6 DD 00               241 	std	*ZD7	; movhi: *ZD0 -> *ZD7
   1BB8 F6 10 2D            242 	ldab	4141
   1BBB D7 01               243 	stab	*ZD5+1	; movqi: 4141 -> *ZD5
   1BBD D6 01               244 	ldab	*ZD5+1
   1BBF C4 DF               245 	andb	#223
   1BC1 D7 01               246 	stab	*ZD5+1	; andqi3: *ZD5 by #223 -> *ZD5
   1BC3 D6 01               247 	ldab	*ZD5+1
   1BC5 F7 10 2D            248 	stab	4141	; movqi: *ZD5 -> 4141
   1BC8 18 E6 04            249 	ldab	4,y
   1BCB 4F                  250 	clra
   1BCC DD 00               251 	std	*ZD5	; zero_extendqihi2: 4,y -> *ZD5
   015D                     252 L11:
   1BCE DE 00               253 	ldx	*ZD7
   1BD0 3C                  254 	pshx		; movhi: *ZD7 -> ,--sp
   1BD1 DE 00               255 	ldx	*ZD7
   1BD3 08                  256 	inx
   1BD4 DF 00               257 	stx	*ZD7	; addhi3: *ZD7 by #1 -> *ZD7
   1BD6 CC A0 78            258 	ldd	#_com1_rx_queue
   1BD9 DD 00               259 	std	*ZD0	; movhi: #_com1_rx_queue -> *ZD0
   1BDB BD 1C BD            260 	jsr	_cq_get_byte	; CALL: R:*ZD0 = _cq_get_byte (2 bytes)
   1BDE 38                  261 	pulx		; addhi3: sp by #2 -> sp
   1BDF D6 01               262 	ldab	*ZD0+1	; tstqi: R:*ZD0
   1BE1 26 03               263 	bne	.+5
   1BE3 7E 1B F8            264 	jmp	L12	; (beq) long branch
   1BE6 DC 00               265 	ldd	*ZD6
   1BE8 1A 93 00            266 	cpd	*ZD5	; cmphi *ZD6 with *ZD5
   1BEB 2D 03               267 	blt	.+5
   1BED 7E 1B F8            268 	jmp	L12	; (bge) long branch
   1BF0 DE 00               269 	ldx	*ZD6
   1BF2 08                  270 	inx
   1BF3 DF 00               271 	stx	*ZD6	; addhi3: *ZD6 by #1 -> *ZD6
   1BF5 7E 1B CE            272 	jmp	L11
   0187                     273 L12:
   1BF8 F6 10 2D            274 	ldab	4141
   1BFB D7 01               275 	stab	*ZD5+1	; movqi: 4141 -> *ZD5
   1BFD D6 01               276 	ldab	*ZD5+1
   1BFF CA 20               277 	orab	#32
   1C01 D7 01               278 	stab	*ZD5+1	; iorqi3: *ZD5 by #32 -> *ZD5
   1C03 D6 01               279 	ldab	*ZD5+1
   1C05 F7 10 2D            280 	stab	4141	; movqi: *ZD5 -> 4141
   1C08 DC 00               281 	ldd	*ZD6
   1C0A DD 00               282 	std	*ZD0	; movhi: *ZD6 -> *ZD0
                            283 ;;;EPILOGUE
   1C0C 38                  284 	pulx	; Pulling register *ZD7
   1C0D DF 00               285 	stx	*ZD7
   1C0F 38                  286 	pulx	; Pulling register *ZD6
   1C10 DF 00               287 	stx	*ZD6
   1C12 38                  288 	pulx	; Pulling register *ZD5
   1C13 DF 00               289 	stx	*ZD5
   1C15 18 38               290 	puly	; Restore stack frame
   1C17 39                  291 	rts	; return from function
                            292 ;;;-----------------------------------------
                            293 ;;; END EPILOGUE for serial_recv_string
                            294 ;;;-----------------------------------------
   01A7                     295 _presto_serial_isr:
                            296 ;;;-----------------------------------------
                            297 ;;;  PROLOGUE for presto_serial_isr
                            298 ;;;-----------------------------------------
   1C18 34                  299 	des	; allocate 1 byte auto variable
   1C19 18 3C               300 	pshy	; Save stack frame
   1C1B 18 30               301 	tsy	; Set current stack frame
   1C1D DE 00               302 	ldx	*ZD5
   1C1F 3C                  303 	pshx	; pushed register *ZD5
   1C20 DE 00               304 	ldx	*ZD6
   1C22 3C                  305 	pshx	; pushed register *ZD6
                            306 ;;;END PROLOGUE
                            307 ; Begin inline assembler code
   1C23 0F                  308 		sei
                            309 ; End of inline assembler code
   1C24 F6 10 2E            310 	ldab	4142
   1C27 D7 01               311 	stab	*ZD5+1	; movqi: 4142 -> *ZD5
   1C29 2D 03               312 	blt	.+5
   1C2B 7E 1C 64            313 	jmp	L16	; (bge) long branch
   1C2E 18 DF 00            314 	sty	*ZXT
   1C31 DC 00               315 	ldd	*ZXT
   1C33 C3 00 02            316 	addd	#2
   1C36 DD 00               317 	std	*ZD6	; addhi3: y by #2 -> *ZD6
   1C38 DE 00               318 	ldx	*ZD6
   1C3A 3C                  319 	pshx		; movhi: *ZD6 -> ,--sp
   1C3B CC A0 80            320 	ldd	#_com1_tx_queue
   1C3E DD 00               321 	std	*ZD0	; movhi: #_com1_tx_queue -> *ZD0
   1C40 BD 1C BD            322 	jsr	_cq_get_byte	; CALL: R:*ZD0 = _cq_get_byte (2 bytes)
   1C43 38                  323 	pulx		; addhi3: sp by #2 -> sp
   1C44 D6 01               324 	ldab	*ZD0+1	; tstqi: R:*ZD0
   1C46 26 03               325 	bne	.+5
   1C48 7E 1C 54            326 	jmp	L17	; (beq) long branch
   1C4B 18 E6 02            327 	ldab	2,y
   1C4E F7 10 2F            328 	stab	4143	; movqi: 2,y -> 4143
   1C51 7E 1C 64            329 	jmp	L16
   01E3                     330 L17:
   1C54 F6 10 2D            331 	ldab	4141
   1C57 D7 01               332 	stab	*ZD5+1	; movqi: 4141 -> *ZD5
   1C59 D6 01               333 	ldab	*ZD5+1
   1C5B C4 7F               334 	andb	#127
   1C5D D7 01               335 	stab	*ZD5+1	; andqi3: *ZD5 by #127 -> *ZD5
   1C5F D6 01               336 	ldab	*ZD5+1
   1C61 F7 10 2D            337 	stab	4141	; movqi: *ZD5 -> 4141
   01F3                     338 L16:
   1C64 F6 10 2E            339 	ldab	4142
   1C67 D7 01               340 	stab	*ZD5+1	; movqi: 4142 -> *ZD5
   1C69 D6 01               341 	ldab	*ZD5+1
   1C6B C4 20               342 	andb	#32
   1C6D D7 01               343 	stab	*ZD5+1	; andqi3: *ZD5 by #32 -> *ZD5
   1C6F 26 03               344 	bne	.+5
   1C71 7E 1C 81            345 	jmp	L19	; (beq) long branch
   1C74 F6 10 2F            346 	ldab	4143
   1C77 37                  347 	pshb		; movqi: 4143 -> ,-sp
   1C78 CC A0 78            348 	ldd	#_com1_rx_queue
   1C7B DD 00               349 	std	*ZD0	; movhi: #_com1_rx_queue -> *ZD0
   1C7D BD 1D 4F            350 	jsr	_cq_put_byte	; CALL: R:*ZD0 = _cq_put_byte (1 bytes)
   1C80 33                  351 	pulb		; addhi3: sp by #1 -> sp
   0210                     352 L19:
                            353 ; Begin inline assembler code
   1C81 0E                  354 		cli
                            355 ; End of inline assembler code
                            356 ;;;EPILOGUE
   1C82 38                  357 	pulx	; Pulling register *ZD6
   1C83 DF 00               358 	stx	*ZD6
   1C85 38                  359 	pulx	; Pulling register *ZD5
   1C86 DF 00               360 	stx	*ZD5
   1C88 18 38               361 	puly	; Restore stack frame
   1C8A 31                  362 	ins	; deallocate 1 byte auto variable
   1C8B 39                  363 	rts	; return from function
                            364 ;;;-----------------------------------------
                            365 ;;; END EPILOGUE for presto_serial_isr
                            366 ;;;-----------------------------------------
   021B                     367 _cq_init:
                            368 ;;;-----------------------------------------
                            369 ;;;  PROLOGUE for cq_init
                            370 ;;;-----------------------------------------
   1C8C 18 3C               371 	pshy	; Save stack frame
   1C8E 18 30               372 	tsy	; Set current stack frame
   1C90 DE 00               373 	ldx	*ZD5
   1C92 3C                  374 	pshx	; pushed register *ZD5
                            375 ;;;END PROLOGUE
   1C93 18 EC 04            376 	ldd	4,y
   1C96 DD 00               377 	std	*ZD5	; movhi: 4,y -> *ZD5
   1C98 DC 00               378 	ldd	*ZD5
   1C9A DE 00               379 	ldx	*ZD0
   1C9C ED 00               380 	std	0,x	; movhi: *ZD5 -> 0,x
   1C9E DC 00               381 	ldd	*ZD5
   1CA0 DE 00               382 	ldx	*ZD0
   1CA2 ED 02               383 	std	2,x	; movhi: *ZD5 -> 2,x
   1CA4 DC 00               384 	ldd	*ZD5
   1CA6 DE 00               385 	ldx	*ZD0
   1CA8 ED 04               386 	std	4,x	; movhi: *ZD5 -> 4,x
   1CAA 18 E6 06            387 	ldab	6,y
   1CAD DE 00               388 	ldx	*ZD0
   1CAF E7 06               389 	stab	6,x	; movqi: 6,y -> 6,x
   1CB1 C6 00               390 	ldab	#0
   1CB3 DE 00               391 	ldx	*ZD0
   1CB5 E7 07               392 	stab	7,x	; movqi: #0 -> 7,x
                            393 ;;;EPILOGUE
   1CB7 38                  394 	pulx	; Pulling register *ZD5
   1CB8 DF 00               395 	stx	*ZD5
   1CBA 18 38               396 	puly	; Restore stack frame
   1CBC 39                  397 	rts	; return from function
                            398 ;;;-----------------------------------------
                            399 ;;; END EPILOGUE for cq_init
                            400 ;;;-----------------------------------------
   024C                     401 _cq_get_byte:
                            402 ;;;-----------------------------------------
                            403 ;;;  PROLOGUE for cq_get_byte
                            404 ;;;-----------------------------------------
   1CBD 18 3C               405 	pshy	; Save stack frame
   1CBF 18 30               406 	tsy	; Set current stack frame
   1CC1 DE 00               407 	ldx	*ZD1
   1CC3 3C                  408 	pshx	; pushed register *ZD1
   1CC4 DE 00               409 	ldx	*ZD5
   1CC6 3C                  410 	pshx	; pushed register *ZD5
   1CC7 DE 00               411 	ldx	*ZD6
   1CC9 3C                  412 	pshx	; pushed register *ZD6
   1CCA DE 00               413 	ldx	*ZD7
   1CCC 3C                  414 	pshx	; pushed register *ZD7
   1CCD DE 00               415 	ldx	*ZD8
   1CCF 3C                  416 	pshx	; pushed register *ZD8
                            417 ;;;END PROLOGUE
   1CD0 DE 00               418 	ldx	*ZD0
   1CD2 EC 04               419 	ldd	4,x
   1CD4 DD 00               420 	std	*ZD6	; movhi: 4,x -> *ZD6
   1CD6 DC 00               421 	ldd	*ZD6
   1CD8 DE 00               422 	ldx	*ZD0
   1CDA 1A A3 02            423 	cpd	2,x	; cmphi *ZD6 with 2,x
   1CDD 26 03               424 	bne	.+5
   1CDF 7E 1D 38            425 	jmp	L22	; (beq) long branch
   1CE2 18 EC 04            426 	ldd	4,y
   1CE5 DD 00               427 	std	*ZD5	; movhi: 4,y -> *ZD5
   1CE7 DE 00               428 	ldx	*ZD6
   1CE9 E6 00               429 	ldab	0,x
   1CEB DE 00               430 	ldx	*ZD5
   1CED E7 00               431 	stab	0,x	; movqi: 0,x -> 0,x
   1CEF DE 00               432 	ldx	*ZD0
   1CF1 EC 04               433 	ldd	4,x
   1CF3 DD 00               434 	std	*ZD7	; movhi: 4,x -> *ZD7
   1CF5 DC 00               435 	ldd	*ZD7
   1CF7 C3 00 01            436 	addd	#1
   1CFA DD 00               437 	std	*ZD1	; addhi3: *ZD7 by #1 -> *ZD1
   1CFC DC 00               438 	ldd	*ZD1
   1CFE DE 00               439 	ldx	*ZD0
   1D00 ED 04               440 	std	4,x	; movhi: *ZD1 -> 4,x
   1D02 DE 00               441 	ldx	*ZD0
   1D04 E6 06               442 	ldab	6,x
   1D06 4F                  443 	clra
   1D07 DD 00               444 	std	*ZD5	; zero_extendqihi2: 6,x -> *ZD5
   1D09 DE 00               445 	ldx	*ZD0
   1D0B EC 00               446 	ldd	0,x
   1D0D DD 00               447 	std	*ZD8	; movhi: 0,x -> *ZD8
   1D0F DC 00               448 	ldd	*ZD8
   1D11 D3 00               449 	addd	*ZD5
   1D13 DD 00               450 	std	*ZD5	; addhi3: *ZD8 by *ZD5 -> *ZD5
   1D15 DC 00               451 	ldd	*ZD5
   1D17 C3 FF FF            452 	addd	#-1
   1D1A DD 00               453 	std	*ZD6	; addhi3: *ZD5 by #-1 -> *ZD6
   1D1C DC 00               454 	ldd	*ZD1
   1D1E DD 00               455 	std	*ZD5	; movhi: *ZD1 -> *ZD5
   1D20 DC 00               456 	ldd	*ZD5
   1D22 1A 93 00            457 	cpd	*ZD6	; cmphi *ZD5 with *ZD6
   1D25 22 03               458 	bhi	.+5
   1D27 7E 1D 30            459 	jmp	L23	; (bleu) long branch
   1D2A DC 00               460 	ldd	*ZD8
   1D2C DE 00               461 	ldx	*ZD0
   1D2E ED 04               462 	std	4,x	; movhi: *ZD8 -> 4,x
   02BF                     463 L23:
   1D30 CC 00 01            464 	ldd	#1
   1D33 DD 00               465 	std	*ZD0	; movhi: #1 -> *ZD0
   1D35 7E 1D 3D            466 	jmp	L25
   02C7                     467 L22:
   1D38 CC 00 00            468 	ldd	#0
   1D3B DD 00               469 	std	*ZD0	; movhi: #0 -> *ZD0
   02CC                     470 L25:
                            471 ;;;EPILOGUE
   1D3D 38                  472 	pulx	; Pulling register *ZD8
   1D3E DF 00               473 	stx	*ZD8
   1D40 38                  474 	pulx	; Pulling register *ZD7
   1D41 DF 00               475 	stx	*ZD7
   1D43 38                  476 	pulx	; Pulling register *ZD6
   1D44 DF 00               477 	stx	*ZD6
   1D46 38                  478 	pulx	; Pulling register *ZD5
   1D47 DF 00               479 	stx	*ZD5
   1D49 38                  480 	pulx	; Pulling register *ZD1
   1D4A DF 00               481 	stx	*ZD1
   1D4C 18 38               482 	puly	; Restore stack frame
   1D4E 39                  483 	rts	; return from function
                            484 ;;;-----------------------------------------
                            485 ;;; END EPILOGUE for cq_get_byte
                            486 ;;;-----------------------------------------
   02DE                     487 _cq_put_byte:
                            488 ;;;-----------------------------------------
                            489 ;;;  PROLOGUE for cq_put_byte
                            490 ;;;-----------------------------------------
   1D4F 18 3C               491 	pshy	; Save stack frame
   1D51 18 30               492 	tsy	; Set current stack frame
   1D53 DE 00               493 	ldx	*ZD1
   1D55 3C                  494 	pshx	; pushed register *ZD1
   1D56 DE 00               495 	ldx	*ZD5
   1D58 3C                  496 	pshx	; pushed register *ZD5
   1D59 DE 00               497 	ldx	*ZD6
   1D5B 3C                  498 	pshx	; pushed register *ZD6
   1D5C DE 00               499 	ldx	*ZD7
   1D5E 3C                  500 	pshx	; pushed register *ZD7
   1D5F DE 00               501 	ldx	*ZD8
   1D61 3C                  502 	pshx	; pushed register *ZD8
   1D62 8F                  503 	xgdx	; saving D register
   1D63 D6 00               504 	ldab	*ZB1
   1D65 37                  505 	pshb	; pushed register *ZB1
   1D66 8F                  506 	xgdx	; restoring D register
                            507 ;;;END PROLOGUE
   1D67 C6 00               508 	ldab	#0
   1D69 D7 00               509 	stab	*ZB1	; movqi: #0 -> *ZB1
   1D6B DE 00               510 	ldx	*ZD0
   1D6D EC 02               511 	ldd	2,x
   1D6F DD 00               512 	std	*ZD5	; movhi: 2,x -> *ZD5
   1D71 18 E6 04            513 	ldab	4,y
   1D74 DE 00               514 	ldx	*ZD5
   1D76 E7 00               515 	stab	0,x	; movqi: 4,y -> 0,x
   1D78 DE 00               516 	ldx	*ZD0
   1D7A EC 02               517 	ldd	2,x
   1D7C DD 00               518 	std	*ZD7	; movhi: 2,x -> *ZD7
   1D7E DC 00               519 	ldd	*ZD7
   1D80 C3 00 01            520 	addd	#1
   1D83 DD 00               521 	std	*ZD1	; addhi3: *ZD7 by #1 -> *ZD1
   1D85 DC 00               522 	ldd	*ZD1
   1D87 DE 00               523 	ldx	*ZD0
   1D89 ED 02               524 	std	2,x	; movhi: *ZD1 -> 2,x
   1D8B DE 00               525 	ldx	*ZD0
   1D8D E6 06               526 	ldab	6,x
   1D8F 4F                  527 	clra
   1D90 DD 00               528 	std	*ZD5	; zero_extendqihi2: 6,x -> *ZD5
   1D92 DE 00               529 	ldx	*ZD0
   1D94 EC 00               530 	ldd	0,x
   1D96 DD 00               531 	std	*ZD8	; movhi: 0,x -> *ZD8
   1D98 DC 00               532 	ldd	*ZD8
   1D9A D3 00               533 	addd	*ZD5
   1D9C DD 00               534 	std	*ZD5	; addhi3: *ZD8 by *ZD5 -> *ZD5
   1D9E DC 00               535 	ldd	*ZD5
   1DA0 C3 FF FF            536 	addd	#-1
   1DA3 DD 00               537 	std	*ZD6	; addhi3: *ZD5 by #-1 -> *ZD6
   1DA5 DC 00               538 	ldd	*ZD1
   1DA7 DD 00               539 	std	*ZD5	; movhi: *ZD1 -> *ZD5
   1DA9 DC 00               540 	ldd	*ZD5
   1DAB 1A 93 00            541 	cpd	*ZD6	; cmphi *ZD5 with *ZD6
   1DAE 22 03               542 	bhi	.+5
   1DB0 7E 1D B9            543 	jmp	L27	; (bleu) long branch
   1DB3 DC 00               544 	ldd	*ZD8
   1DB5 DE 00               545 	ldx	*ZD0
   1DB7 ED 02               546 	std	2,x	; movhi: *ZD8 -> 2,x
   0348                     547 L27:
   1DB9 DE 00               548 	ldx	*ZD0
   1DBB EC 02               549 	ldd	2,x
   1DBD DD 00               550 	std	*ZD7	; movhi: 2,x -> *ZD7
   1DBF DC 00               551 	ldd	*ZD7
   1DC1 DE 00               552 	ldx	*ZD0
   1DC3 1A A3 04            553 	cpd	4,x	; cmphi *ZD7 with 4,x
   1DC6 27 03               554 	beq	.+5
   1DC8 7E 1E 0A            555 	jmp	L28	; (bne) long branch
   1DCB DC 00               556 	ldd	*ZD7
   1DCD C3 00 01            557 	addd	#1
   1DD0 DD 00               558 	std	*ZD1	; addhi3: *ZD7 by #1 -> *ZD1
   1DD2 DC 00               559 	ldd	*ZD1
   1DD4 DE 00               560 	ldx	*ZD0
   1DD6 ED 04               561 	std	4,x	; movhi: *ZD1 -> 4,x
   1DD8 C6 01               562 	ldab	#1
   1DDA D7 00               563 	stab	*ZB1	; movqi: #1 -> *ZB1
   1DDC DE 00               564 	ldx	*ZD0
   1DDE E6 06               565 	ldab	6,x
   1DE0 4F                  566 	clra
   1DE1 DD 00               567 	std	*ZD5	; zero_extendqihi2: 6,x -> *ZD5
   1DE3 DE 00               568 	ldx	*ZD0
   1DE5 EC 00               569 	ldd	0,x
   1DE7 DD 00               570 	std	*ZD8	; movhi: 0,x -> *ZD8
   1DE9 DC 00               571 	ldd	*ZD8
   1DEB D3 00               572 	addd	*ZD5
   1DED DD 00               573 	std	*ZD5	; addhi3: *ZD8 by *ZD5 -> *ZD5
   1DEF DC 00               574 	ldd	*ZD5
   1DF1 C3 FF FF            575 	addd	#-1
   1DF4 DD 00               576 	std	*ZD6	; addhi3: *ZD5 by #-1 -> *ZD6
   1DF6 DC 00               577 	ldd	*ZD1
   1DF8 DD 00               578 	std	*ZD5	; movhi: *ZD1 -> *ZD5
   1DFA DC 00               579 	ldd	*ZD5
   1DFC 1A 93 00            580 	cpd	*ZD6	; cmphi *ZD5 with *ZD6
   1DFF 22 03               581 	bhi	.+5
   1E01 7E 1E 0A            582 	jmp	L28	; (bleu) long branch
   1E04 DC 00               583 	ldd	*ZD8
   1E06 DE 00               584 	ldx	*ZD0
   1E08 ED 04               585 	std	4,x	; movhi: *ZD8 -> 4,x
   0399                     586 L28:
   1E0A D6 00               587 	ldab	*ZB1
   1E0C 4F                  588 	clra
   1E0D DD 00               589 	std	*ZD0	; zero_extendqihi2: *ZB1 -> *ZD0
                            590 ;;;EPILOGUE
   1E0F 8F                  591 	xgdx	; saving D register
   1E10 33                  592 	pulb	; Pulling register *ZB1
   1E11 D7 00               593 	stab	*ZB1
   1E13 8F                  594 	xgdx	; restoring D register
   1E14 38                  595 	pulx	; Pulling register *ZD8
   1E15 DF 00               596 	stx	*ZD8
   1E17 38                  597 	pulx	; Pulling register *ZD7
   1E18 DF 00               598 	stx	*ZD7
   1E1A 38                  599 	pulx	; Pulling register *ZD6
   1E1B DF 00               600 	stx	*ZD6
   1E1D 38                  601 	pulx	; Pulling register *ZD5
   1E1E DF 00               602 	stx	*ZD5
   1E20 38                  603 	pulx	; Pulling register *ZD1
   1E21 DF 00               604 	stx	*ZD1
   1E23 18 38               605 	puly	; Restore stack frame
   1E25 39                  606 	rts	; return from function
                            607 ;;;-----------------------------------------
                            608 ;;; END EPILOGUE for cq_put_byte
                            609 ;;;-----------------------------------------
                            610 	.area	_BSS
   0000                     611 _com1_rx_queue:	.blkb	8
   0008                     612 _com1_tx_queue:	.blkb	8
   0010                     613 _com1_rx_buffer:	.blkb	20
   0024                     614 _com1_tx_buffer:	.blkb	255
                            615 	; END
