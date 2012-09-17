                              1 ;;;-----------------------------------------
                              2 ;;; Start MC6811 gcc assembly output
                              3 ;;; gcc compiler compiled on TBD
                              4 ;;; OPTIONS:	-mlong_branch optimize  
                              5 ;;; OPTIONS:	 peephole !omit_frame_pointer !signed-char
                              6 ;;; Source:		kernel\kernel.c
                              7 ;;; Destination:	kernel.s
                              8 ;;; Compiled:	Tue Oct 24 21:55:00 2000
                              9 ;;; (META)compiled by GNU C version 2.8.1.
                             10 ;;;-----------------------------------------
                             11 	.module kernel.c
                             12 	.globl	_presto_asm_new_sp
                             13 	.area	_DATA
   0000                      14 _presto_asm_new_sp:
   272E 00 00                15 	.word	0
                             16 	.globl	_presto_asm_old_sp_p
   0002                      17 _presto_asm_old_sp_p:
   2730 00 00                18 	.word	0
                             19 	.globl	_presto_asm_new_fn
   0004                      20 _presto_asm_new_fn:
   2732 00 00                21 	.word	0
   0006                      22 _current_tcb_p:
   2734 00 00                23 	.word	0
   0008                      24 _tcb_head_p:
   2736 00 00                25 	.word	0
   000A                      26 _free_tcb_p:
   2738 00 00                27 	.word	0
   000C                      28 _presto_initialized:
   273A 00                   29 	.byte	0
   000D                      30 _free_mail_p:
   273B 00 00                31 	.word	0
   000F                      32 _po_mail_p:
   273D 00 00                33 	.word	0
                             34 	; extern	_clock_reset
                             35 	; extern	_presto_create_task
                             36 	.area	_CODE
                             37 	.globl	_presto_init
   0000                      38 _presto_init:
                             39 ;;;-----------------------------------------
                             40 ;;;  PROLOGUE for presto_init
                             41 ;;;-----------------------------------------
   0000 18 3C                42 	pshy	; Save stack frame
   0002 18 30                43 	tsy	; Set current stack frame
   0004 DE 00                44 	ldx	*ZD5
   0006 3C                   45 	pshx	; pushed register *ZD5
   0007 DE 00                46 	ldx	*ZD6
   0009 3C                   47 	pshx	; pushed register *ZD6
   000A DE 00                48 	ldx	*ZD7
   000C 3C                   49 	pshx	; pushed register *ZD7
   000D DE 00                50 	ldx	*ZD8
   000F 3C                   51 	pshx	; pushed register *ZD8
   0010 8F                   52 	xgdx	; saving D register
   0011 D6 00                53 	ldab	*ZB1
   0013 37                   54 	pshb	; pushed register *ZB1
   0014 8F                   55 	xgdx	; restoring D register
                             56 ;;;END PROLOGUE
   0015 7D 27 3A             57 	tst	_presto_initialized	; tstqi: MEM:_presto_initialized
   0018 27 03                58 	beq	.+5
   001A 7E 01 1E             59 	jmp	L1	; (bne) long branch
   001D C6 01                60 	ldab	#1
   001F F7 27 3A             61 	stab	_presto_initialized	; movqi: #1 -> _presto_initialized
   0022 F6 10 09             62 	ldab	4105
   0025 D7 01                63 	stab	*ZD5+1	; movqi: 4105 -> *ZD5
   0027 D6 01                64 	ldab	*ZD5+1
   0029 CA 10                65 	orab	#16
   002B D7 01                66 	stab	*ZD5+1	; iorqi3: *ZD5 by #16 -> *ZD5
   002D D6 01                67 	ldab	*ZD5+1
   002F F7 10 09             68 	stab	4105	; movqi: *ZD5 -> 4105
   0032 F6 10 09             69 	ldab	4105
   0035 D7 01                70 	stab	*ZD5+1	; movqi: 4105 -> *ZD5
   0037 D6 01                71 	ldab	*ZD5+1
   0039 CA 20                72 	orab	#32
   003B D7 01                73 	stab	*ZD5+1	; iorqi3: *ZD5 by #32 -> *ZD5
   003D D6 01                74 	ldab	*ZD5+1
   003F F7 10 09             75 	stab	4105	; movqi: *ZD5 -> 4105
   0042 F6 10 08             76 	ldab	4104
   0045 D7 01                77 	stab	*ZD5+1	; movqi: 4104 -> *ZD5
   0047 D6 01                78 	ldab	*ZD5+1
   0049 CA 20                79 	orab	#32
   004B D7 01                80 	stab	*ZD5+1	; iorqi3: *ZD5 by #32 -> *ZD5
   004D D6 01                81 	ldab	*ZD5+1
   004F F7 10 08             82 	stab	4104	; movqi: *ZD5 -> 4104
   0052 F6 10 08             83 	ldab	4104
   0055 D7 01                84 	stab	*ZD5+1	; movqi: 4104 -> *ZD5
   0057 D6 01                85 	ldab	*ZD5+1
   0059 CA 10                86 	orab	#16
   005B D7 01                87 	stab	*ZD5+1	; iorqi3: *ZD5 by #16 -> *ZD5
   005D D6 01                88 	ldab	*ZD5+1
   005F F7 10 08             89 	stab	4104	; movqi: *ZD5 -> 4104
   0062 CC 98 B1             90 	ldd	#_presto_master_clock
   0065 DD 00                91 	std	*ZD0	; movhi: #_presto_master_clock -> *ZD0
   0067 BD 07 52             92 	jsr	_clock_reset	; CALL: (VOIDmode) _clock_reset (0 bytes)
   006A C6 00                93 	ldab	#0
   006C D7 00                94 	stab	*ZB1	; movqi: #0 -> *ZB1
   006E                      95 L6:
   006E D6 00                96 	ldab	*ZB1
   0070 4F                   97 	clra
   0071 DD 00                98 	std	*ZD5	; zero_extendqihi2: *ZB1 -> *ZD5
   0073 DC 00                99 	ldd	*ZD5
   0075 05                  100 	asld
   0076 05                  101 	asld
   0077 05                  102 	asld
   0078 05                  103 	asld
   0079 DD 00               104 	std	*ZD5	; ashlhi3: *ZD5 by #4 -> *ZD5
   007B DC 00               105 	ldd	*ZD5
   007D C3 98 11            106 	addd	#_tcb_list+16
   0080 DD 00               107 	std	*ZD8	; addhi3: *ZD5 by #_tcb_list+16 -> *ZD8
   0082 DC 00               108 	ldd	*ZD8
   0084 DE 00               109 	ldx	*ZD5
   0086 8F                  110 	xgdx
   0087 C3 98 0B            111 	addd	#_tcb_list+10
   008A 8F                  112 	xgdx
                            113 ;;; (output_swap_regx: addr computed)
   008B ED 00               114 	std	0,x	; movhi: *ZD8 -> 0,x
   008D D6 00               115 	ldab	*ZB1
   008F E7 00               116 	stab	0,x	; movqi: *ZB1 -> 0,x
   0091 CC 00 02            117 	ldd	#2
   0094 ED 00               118 	std	0,x	; movhi: #2 -> 0,x
   0096 7C 00 00            119 	inc	*ZB1	; addqi3: *ZB1 by #1 -> *ZB1
   0099 D6 00               120 	ldab	*ZB1
   009B C1 0A               121 	cmpb	#10	; cmpqi *ZB1 with #10
   009D 22 03               122 	bhi	.+5
   009F 7E 00 6E            123 	jmp	L6	; (bleu) long branch
   00A2 CC 00 00            124 	ldd	#0
   00A5 FD 98 AB            125 	std	_tcb_list+170	; movhi: #0 -> _tcb_list+170
   00A8 CC 98 01            126 	ldd	#_tcb_list
   00AB FD 27 38            127 	std	_free_tcb_p	; movhi: #_tcb_list -> _free_tcb_p
   00AE C6 00               128 	ldab	#0
   00B0 D7 00               129 	stab	*ZB1	; movqi: #0 -> *ZB1
   00B2 CC 99 4E            130 	ldd	#_mail_list
   00B5 DD 00               131 	std	*ZD7	; movhi: #_mail_list -> *ZD7
   00B7 CC 99 5B            132 	ldd	#_mail_list+13
   00BA DD 00               133 	std	*ZD6	; movhi: #_mail_list+13 -> *ZD6
   00BC CC 99 5D            134 	ldd	#_mail_list+15
   00BF DD 00               135 	std	*ZD5	; movhi: #_mail_list+15 -> *ZD5
   00C1                     136 L11:
   00C1 DC 00               137 	ldd	*ZD5
   00C3 DE 00               138 	ldx	*ZD6
   00C5 ED 00               139 	std	0,x	; movhi: *ZD5 -> 0,x
   00C7 D6 00               140 	ldab	*ZB1
   00C9 4F                  141 	clra
   00CA DE 00               142 	ldx	*ZD7
   00CC ED 00               143 	std	0,x	; zero_extendqihi2: *ZB1 -> 0,x
   00CE DC 00               144 	ldd	*ZD7
   00D0 C3 00 0F            145 	addd	#15
   00D3 DD 00               146 	std	*ZD7	; addhi3: *ZD7 by #15 -> *ZD7
   00D5 DC 00               147 	ldd	*ZD6
   00D7 C3 00 0F            148 	addd	#15
   00DA DD 00               149 	std	*ZD6	; addhi3: *ZD6 by #15 -> *ZD6
   00DC DC 00               150 	ldd	*ZD5
   00DE C3 00 0F            151 	addd	#15
   00E1 DD 00               152 	std	*ZD5	; addhi3: *ZD5 by #15 -> *ZD5
   00E3 7C 00 00            153 	inc	*ZB1	; addqi3: *ZB1 by #1 -> *ZB1
   00E6 D6 00               154 	ldab	*ZB1
   00E8 C1 63               155 	cmpb	#99	; cmpqi *ZB1 with #99
   00EA 22 03               156 	bhi	.+5
   00EC 7E 00 C1            157 	jmp	L11	; (bleu) long branch
   00EF CC 00 00            158 	ldd	#0
   00F2 FD 9F 28            159 	std	_mail_list+1498	; movhi: #0 -> _mail_list+1498
   00F5 CC 99 4E            160 	ldd	#_mail_list
   00F8 FD 27 3B            161 	std	_free_mail_p	; movhi: #_mail_list -> _free_mail_p
   00FB C6 00               162 	ldab	#0
   00FD 37                  163 	pshb		; movqi: #0 -> ,-sp
   00FE CE 00 96            164 	ldx	#150
   0101 3C                  165 	pshx		; movhi: #150 -> ,--sp
   0102 CE 98 B5            166 	ldx	#_idle_stack
   0105 3C                  167 	pshx		; movhi: #_idle_stack -> ,--sp
   0106 CC 06 8C            168 	ldd	#_idle_task
   0109 DD 00               169 	std	*ZD0	; movhi: #_idle_task -> *ZD0
   010B BD 01 32            170 	jsr	_presto_create_task	; CALL: R:*ZD0 = _presto_create_task (5 bytes)
   010E D6 01               171 	ldab	*ZD0+1
   0110 F7 99 4D            172 	stab	_idle_tid	; movqi: *ZD0 -> _idle_tid
   0113 BD 06 96            173 	jsr	_tid_to_tcbptr	; CALL: R:*ZD0 = _tid_to_tcbptr (0 bytes)
   0116 DC 00               174 	ldd	*ZD0
   0118 FD 99 4B            175 	std	_idle_tcb_p	; movhi: *ZD0 -> _idle_tcb_p
   011B 38                  176 	pulx
   011C 38                  177 	pulx
   011D 33                  178 	pulb		; addhi3: sp by #5 -> sp
   011E                     179 L1:
                            180 ;;;EPILOGUE
   011E 8F                  181 	xgdx	; saving D register
   011F 33                  182 	pulb	; Pulling register *ZB1
   0120 D7 00               183 	stab	*ZB1
   0122 8F                  184 	xgdx	; restoring D register
   0123 38                  185 	pulx	; Pulling register *ZD8
   0124 DF 00               186 	stx	*ZD8
   0126 38                  187 	pulx	; Pulling register *ZD7
   0127 DF 00               188 	stx	*ZD7
   0129 38                  189 	pulx	; Pulling register *ZD6
   012A DF 00               190 	stx	*ZD6
   012C 38                  191 	pulx	; Pulling register *ZD5
   012D DF 00               192 	stx	*ZD5
   012F 18 38               193 	puly	; Restore stack frame
   0131 39                  194 	rts	; return from function
                            195 ;;;-----------------------------------------
                            196 ;;; END EPILOGUE for presto_init
                            197 ;;;-----------------------------------------
                            198 	; extern	_presto_setup_new_task
                            199 	.globl	_presto_create_task
   0132                     200 _presto_create_task:
                            201 ;;;-----------------------------------------
                            202 ;;;  PROLOGUE for presto_create_task
                            203 ;;;-----------------------------------------
   0132 18 3C               204 	pshy	; Save stack frame
   0134 18 30               205 	tsy	; Set current stack frame
   0136 DE 00               206 	ldx	*ZD5
   0138 3C                  207 	pshx	; pushed register *ZD5
   0139 DE 00               208 	ldx	*ZD6
   013B 3C                  209 	pshx	; pushed register *ZD6
   013C DE 00               210 	ldx	*ZD7
   013E 3C                  211 	pshx	; pushed register *ZD7
   013F DE 00               212 	ldx	*ZD8
   0141 3C                  213 	pshx	; pushed register *ZD8
                            214 ;;;END PROLOGUE
   0142 DC 00               215 	ldd	*ZD0
   0144 DD 00               216 	std	*ZD8	; movhi: *ZD0 -> *ZD8
   0146 18 EC 04            217 	ldd	4,y
   0149 DD 00               218 	std	*ZD6	; movhi: 4,y -> *ZD6
   014B 7D 27 3A            219 	tst	_presto_initialized	; tstqi: MEM:_presto_initialized
   014E 27 03               220 	beq	.+5
   0150 7E 01 56            221 	jmp	L14	; (bne) long branch
   0153 BD 00 00            222 	jsr	_presto_init	; CALL: (VOIDmode) _presto_init (0 bytes)
   0156                     223 L14:
   0156 FC 27 38            224 	ldd	_free_tcb_p
   0159 DD 00               225 	std	*ZD5	; movhi: _free_tcb_p -> *ZD5
   015B 27 03               226 	beq	.+5
   015D 7E 01 68            227 	jmp	L15	; (bne) long branch
   0160 CC FF FF            228 	ldd	#-1
   0163 DD 00               229 	std	*ZD0	; movhi: #-1 -> *ZD0
   0165 7E 02 34            230 	jmp	L25
   0168                     231 L15:
   0168 DC 00               232 	ldd	*ZD5
   016A DD 00               233 	std	*ZD7	; movhi: *ZD5 -> *ZD7
   016C DE 00               234 	ldx	*ZD7
   016E EC 0A               235 	ldd	10,x
   0170 FD 27 38            236 	std	_free_tcb_p	; movhi: 10,x -> _free_tcb_p
                            237 ; Begin inline assembler code
   0173 0F                  238 		sei
                            239 ; End of inline assembler code
   0174 DC 00               240 	ldd	*ZD6
   0176 18 E3 06            241 	addd	6,y
   0179 DD 00               242 	std	*ZD5	; addhi3: *ZD6 by 6,y -> *ZD5
   017B DC 00               243 	ldd	*ZD5
   017D C3 FF FF            244 	addd	#-1
   0180 DD 00               245 	std	*ZD5	; addhi3: *ZD5 by #-1 -> *ZD5
   0182 DC 00               246 	ldd	*ZD5
   0184 DE 00               247 	ldx	*ZD7
   0186 ED 03               248 	std	3,x	; movhi: *ZD5 -> 3,x
   0188 DC 00               249 	ldd	*ZD6
   018A DE 00               250 	ldx	*ZD7
   018C ED 05               251 	std	5,x	; movhi: *ZD6 -> 5,x
   018E DC 00               252 	ldd	*ZD5
   0190 DE 00               253 	ldx	*ZD7
   0192 ED 01               254 	std	1,x	; movhi: *ZD5 -> 1,x
   0194 18 E6 08            255 	ldab	8,y
   0197 DE 00               256 	ldx	*ZD7
   0199 E7 07               257 	stab	7,x	; movqi: 8,y -> 7,x
   019B CC 00 00            258 	ldd	#0
   019E DE 00               259 	ldx	*ZD7
   01A0 ED 08               260 	std	8,x	; movhi: #0 -> 8,x
   01A2 CC 00 00            261 	ldd	#0
   01A5 DE 00               262 	ldx	*ZD7
   01A7 ED 0C               263 	std	12,x	; movhi: #0 -> 12,x
   01A9 CC 00 00            264 	ldd	#0
   01AC DE 00               265 	ldx	*ZD7
   01AE ED 0E               266 	std	14,x	; movhi: #0 -> 14,x
   01B0 DC 00               267 	ldd	*ZD5
   01B2 FD 27 2E            268 	std	_presto_asm_new_sp	; movhi: *ZD5 -> _presto_asm_new_sp
   01B5 DC 00               269 	ldd	*ZD8
   01B7 FD 27 32            270 	std	_presto_asm_new_fn	; movhi: *ZD8 -> _presto_asm_new_fn
   01BA BD C0 34            271 	jsr	_presto_setup_new_task	; CALL: (VOIDmode) _presto_setup_new_task (0 bytes)
   01BD FC 27 2E            272 	ldd	_presto_asm_new_sp
   01C0 DE 00               273 	ldx	*ZD7
   01C2 ED 01               274 	std	1,x	; movhi: _presto_asm_new_sp -> 1,x
   01C4 FC 27 36            275 	ldd	_tcb_head_p
   01C7 DD 00               276 	std	*ZD5	; movhi: _tcb_head_p -> *ZD5
   01C9 27 03               277 	beq	.+5
   01CB 7E 01 DD            278 	jmp	L16	; (bne) long branch
   01CE DC 00               279 	ldd	*ZD7
   01D0 FD 27 36            280 	std	_tcb_head_p	; movhi: *ZD7 -> _tcb_head_p
   01D3 CC 00 00            281 	ldd	#0
   01D6 DE 00               282 	ldx	*ZD7
   01D8 ED 0A               283 	std	10,x	; movhi: #0 -> 10,x
   01DA 7E 02 28            284 	jmp	L17
   01DD                     285 L16:
   01DD DE 00               286 	ldx	*ZD7
   01DF E6 07               287 	ldab	7,x
   01E1 D7 01               288 	stab	*ZD8+1	; movqi: 7,x -> *ZD8
   01E3 D6 01               289 	ldab	*ZD8+1
   01E5 DE 00               290 	ldx	*ZD5
   01E7 E1 07               291 	cmpb	7,x	; cmpqi *ZD8 with 7,x
   01E9 22 03               292 	bhi	.+5
   01EB 7E 02 0D            293 	jmp	L26	; (bleu) long branch
   01EE DC 00               294 	ldd	*ZD5
   01F0 DE 00               295 	ldx	*ZD7
   01F2 ED 0A               296 	std	10,x	; movhi: *ZD5 -> 10,x
   01F4 DC 00               297 	ldd	*ZD7
   01F6 FD 27 36            298 	std	_tcb_head_p	; movhi: *ZD7 -> _tcb_head_p
   01F9 7E 02 28            299 	jmp	L17
   01FC                     300 L22:
   01FC DE 00               301 	ldx	*ZD6
   01FE EC 0A               302 	ldd	10,x
   0200 DD 00               303 	std	*ZD5	; movhi: 10,x -> *ZD5
   0202 D6 01               304 	ldab	*ZD8+1
   0204 DE 00               305 	ldx	*ZD5
   0206 E1 07               306 	cmpb	7,x	; cmpqi *ZD8 with 7,x
   0208 23 03               307 	bls	.+5
   020A 7E 02 1A            308 	jmp	L21	; (bgtu) long branch
   020D                     309 L26:
   020D DC 00               310 	ldd	*ZD5
   020F DD 00               311 	std	*ZD6	; movhi: *ZD5 -> *ZD6
   0211 DE 00               312 	ldx	*ZD6
   0213 EC 0A               313 	ldd	10,x	; tsthi: R:10,x
   0215 27 03               314 	beq	.+5
   0217 7E 01 FC            315 	jmp	L22	; (bne) long branch
   021A                     316 L21:
   021A DE 00               317 	ldx	*ZD6
   021C EC 0A               318 	ldd	10,x
   021E DE 00               319 	ldx	*ZD7
   0220 ED 0A               320 	std	10,x	; movhi: 10,x -> 10,x
   0222 DC 00               321 	ldd	*ZD7
   0224 DE 00               322 	ldx	*ZD6
   0226 ED 0A               323 	std	10,x	; movhi: *ZD7 -> 10,x
   0228                     324 L17:
                            325 ; Begin inline assembler code
   0228 0E                  326 		cli
                            327 ; End of inline assembler code
   0229 DE 00               328 	ldx	*ZD7
   022B E6 00               329 	ldab	0,x
   022D 4F                  330 	clra
   022E 5D                  331 	tstb
   022F 2C 01               332 	bge	.+3
   0231 4A                  333 	deca
   0232 DD 00               334 	std	*ZD0	; extendqihi2: 0,x -> *ZD0
   0234                     335 L25:
                            336 ;;;EPILOGUE
   0234 38                  337 	pulx	; Pulling register *ZD8
   0235 DF 00               338 	stx	*ZD8
   0237 38                  339 	pulx	; Pulling register *ZD7
   0238 DF 00               340 	stx	*ZD7
   023A 38                  341 	pulx	; Pulling register *ZD6
   023B DF 00               342 	stx	*ZD6
   023D 38                  343 	pulx	; Pulling register *ZD5
   023E DF 00               344 	stx	*ZD5
   0240 18 38               345 	puly	; Restore stack frame
   0242 39                  346 	rts	; return from function
                            347 ;;;-----------------------------------------
                            348 ;;; END EPILOGUE for presto_create_task
                            349 ;;;-----------------------------------------
                            350 	; extern	_set_interrupt
                            351 	; extern	_presto_system_isr
                            352 	; extern	_presto_start_task_switching
                            353 	; extern	_presto_fatal_error
                            354 	.globl	_presto_start_scheduler
   0243                     355 _presto_start_scheduler:
                            356 ;;;-----------------------------------------
                            357 ;;;  PROLOGUE for presto_start_scheduler
                            358 ;;;-----------------------------------------
   0243 18 3C               359 	pshy	; Save stack frame
   0245 18 30               360 	tsy	; Set current stack frame
   0247 DE 00               361 	ldx	*ZD5
   0249 3C                  362 	pshx	; pushed register *ZD5
                            363 ;;;END PROLOGUE
   024A 7D 27 3A            364 	tst	_presto_initialized	; tstqi: MEM:_presto_initialized
   024D 27 03               365 	beq	.+5
   024F 7E 02 55            366 	jmp	L28	; (bne) long branch
   0252 BD 00 00            367 	jsr	_presto_init	; CALL: (VOIDmode) _presto_init (0 bytes)
   0255                     368 L28:
                            369 ; Begin inline assembler code
   0255 0F                  370 		sei
                            371 ; End of inline assembler code
   0256 CE C0 00            372 	ldx	#_presto_system_isr
   0259 3C                  373 	pshx		; movhi: #_presto_system_isr -> ,--sp
   025A C6 08               374 	ldab	#8
   025C D7 01               375 	stab	*ZD0+1	; movqi: #8 -> *ZD0
   025E BD 08 51            376 	jsr	_set_interrupt	; CALL: (VOIDmode) _set_interrupt (2 bytes)
   0261 BD 06 EC            377 	jsr	_presto_start_master_timer	; CALL: (VOIDmode) _presto_start_master_timer (0 bytes)
   0264 FC 27 36            378 	ldd	_tcb_head_p
   0267 FD 27 34            379 	std	_current_tcb_p	; movhi: _tcb_head_p -> _current_tcb_p
   026A FC 27 36            380 	ldd	_tcb_head_p
   026D DD 00               381 	std	*ZD5	; movhi: _tcb_head_p -> *ZD5
   026F DE 00               382 	ldx	*ZD5
   0271 EC 01               383 	ldd	1,x
   0273 FD 27 2E            384 	std	_presto_asm_new_sp	; movhi: 1,x -> _presto_asm_new_sp
   0276 BD C0 26            385 	jsr	_presto_start_task_switching	; CALL: (VOIDmode) _presto_start_task_switching (0 bytes)
   0279 BD 08 58            386 	jsr	_presto_fatal_error	; CALL: (VOIDmode) _presto_fatal_error (0 bytes)
   027C 38                  387 	pulx		; addhi3: sp by #2 -> sp
                            388 ;;;EPILOGUE
   027D 38                  389 	pulx	; Pulling register *ZD5
   027E DF 00               390 	stx	*ZD5
   0280 18 38               391 	puly	; Restore stack frame
   0282 39                  392 	rts	; return from function
                            393 ;;;-----------------------------------------
                            394 ;;; END EPILOGUE for presto_start_scheduler
                            395 ;;;-----------------------------------------
                            396 	; extern	_presto_switch_tasks
                            397 	.globl	_presto_sleep
   0283                     398 _presto_sleep:
                            399 ;;;-----------------------------------------
                            400 ;;;  PROLOGUE for presto_sleep
                            401 ;;;-----------------------------------------
   0283 18 3C               402 	pshy	; Save stack frame
   0285 18 30               403 	tsy	; Set current stack frame
   0287 DE 00               404 	ldx	*ZD5
   0289 3C                  405 	pshx	; pushed register *ZD5
   028A DE 00               406 	ldx	*ZD6
   028C 3C                  407 	pshx	; pushed register *ZD6
                            408 ;;;END PROLOGUE
                            409 ; Begin inline assembler code
   028D 0F                  410 		sei
                            411 ; End of inline assembler code
   028E FC 27 34            412 	ldd	_current_tcb_p
   0291 DD 00               413 	std	*ZD5	; movhi: _current_tcb_p -> *ZD5
   0293 DE 00               414 	ldx	*ZD5
   0295 EC 01               415 	ldd	1,x
   0297 DD 00               416 	std	*ZD6	; movhi: 1,x -> *ZD6
   0299 DC 00               417 	ldd	*ZD6
   029B DE 00               418 	ldx	*ZD5
   029D 1A A3 03            419 	cpd	3,x	; cmphi *ZD6 with 3,x
   02A0 23 03               420 	bls	.+5
   02A2 7E 02 B1            421 	jmp	L31	; (bgtu) long branch
   02A5 DC 00               422 	ldd	*ZD6
   02A7 DE 00               423 	ldx	*ZD5
   02A9 1A A3 05            424 	cpd	5,x	; cmphi *ZD6 with 5,x
   02AC 25 03               425 	blo	.+5
   02AE 7E 02 B4            426 	jmp	L30	; (bgeu) long branch
   02B1                     427 L31:
   02B1 BD 08 58            428 	jsr	_presto_fatal_error	; CALL: (VOIDmode) _presto_fatal_error (0 bytes)
   02B4                     429 L30:
   02B4 FC 27 34            430 	ldd	_current_tcb_p
   02B7 DD 00               431 	std	*ZD5	; movhi: _current_tcb_p -> *ZD5
   02B9 DE 00               432 	ldx	*ZD5
   02BB EC 0C               433 	ldd	12,x	; tsthi: R:12,x
   02BD 27 03               434 	beq	.+5
   02BF 7E 02 E7            435 	jmp	L33	; (bne) long branch
   02C2 CC 00 01            436 	ldd	#1
   02C5 DE 00               437 	ldx	*ZD5
   02C7 ED 08               438 	std	8,x	; movhi: #1 -> 8,x
   02C9 DC 00               439 	ldd	*ZD5
   02CB C3 00 01            440 	addd	#1
   02CE DD 00               441 	std	*ZD5	; addhi3: *ZD5 by #1 -> *ZD5
   02D0 DC 00               442 	ldd	*ZD5
   02D2 FD 27 30            443 	std	_presto_asm_old_sp_p	; movhi: *ZD5 -> _presto_asm_old_sp_p
   02D5 BD 03 8D            444 	jsr	_presto_next_tcb_to_run	; CALL: R:*ZD0 = _presto_next_tcb_to_run (0 bytes)
   02D8 DC 00               445 	ldd	*ZD0
   02DA FD 27 34            446 	std	_current_tcb_p	; movhi: *ZD0 -> _current_tcb_p
   02DD DE 00               447 	ldx	*ZD0
   02DF EC 01               448 	ldd	1,x
   02E1 FD 27 2E            449 	std	_presto_asm_new_sp	; movhi: 1,x -> _presto_asm_new_sp
   02E4 BD C0 17            450 	jsr	_presto_switch_tasks	; CALL: (VOIDmode) _presto_switch_tasks (0 bytes)
   02E7                     451 L33:
                            452 ; Begin inline assembler code
   02E7 0E                  453 		cli
                            454 ; End of inline assembler code
                            455 ;;;EPILOGUE
   02E8 38                  456 	pulx	; Pulling register *ZD6
   02E9 DF 00               457 	stx	*ZD6
   02EB 38                  458 	pulx	; Pulling register *ZD5
   02EC DF 00               459 	stx	*ZD5
   02EE 18 38               460 	puly	; Restore stack frame
   02F0 39                  461 	rts	; return from function
                            462 ;;;-----------------------------------------
                            463 ;;; END EPILOGUE for presto_sleep
                            464 ;;;-----------------------------------------
                            465 	.globl	_presto_kill_self
   02F1                     466 _presto_kill_self:
                            467 ;;;-----------------------------------------
                            468 ;;;  PROLOGUE for presto_kill_self
                            469 ;;;-----------------------------------------
   02F1 18 3C               470 	pshy	; Save stack frame
   02F3 18 30               471 	tsy	; Set current stack frame
                            472 ;;;END PROLOGUE
   02F5 BD 08 58            473 	jsr	_presto_fatal_error	; CALL: (VOIDmode) _presto_fatal_error (0 bytes)
                            474 ;;;EPILOGUE
   02F8 18 38               475 	puly	; Restore stack frame
   02FA 39                  476 	rts	; return from function
                            477 ;;;-----------------------------------------
                            478 ;;; END EPILOGUE for presto_kill_self
                            479 ;;;-----------------------------------------
                            480 	; extern	_clock_add
                            481 	.globl	_presto_service_timer_interrupt
   02FB                     482 _presto_service_timer_interrupt:
                            483 ;;;-----------------------------------------
                            484 ;;;  PROLOGUE for presto_service_timer_interrupt
                            485 ;;;-----------------------------------------
   02FB 18 3C               486 	pshy	; Save stack frame
   02FD 18 30               487 	tsy	; Set current stack frame
   02FF DE 00               488 	ldx	*ZD5
   0301 3C                  489 	pshx	; pushed register *ZD5
   0302 DE 00               490 	ldx	*ZD6
   0304 3C                  491 	pshx	; pushed register *ZD6
   0305 DE 00               492 	ldx	*ZD7
   0307 3C                  493 	pshx	; pushed register *ZD7
                            494 ;;;END PROLOGUE
   0308 CE 00 05            495 	ldx	#5
   030B 3C                  496 	pshx		; movhi: #5 -> ,--sp
   030C FC 98 B1            497 	ldd	_presto_master_clock
   030F DD 00               498 	std	*ZD0	; movhi: _presto_master_clock -> *ZD0
   0311 FC 98 B3            499 	ldd	_presto_master_clock+2
   0314 ED 00               500 	std 0,x	; movhi: _presto_master_clock+2 -> x
   0316 BD 07 77            501 	jsr	_clock_add	; CALL: (VOIDmode) _clock_add (2 bytes)
   0319 BD 07 2F            502 	jsr	_presto_restart_master_timer	; CALL: (VOIDmode) _presto_restart_master_timer (0 bytes)
   031C BD 05 2C            503 	jsr	_deliver_mail	; CALL: R:*ZD0 = _deliver_mail (0 bytes)
   031F 38                  504 	pulx		; addhi3: sp by #2 -> sp
   0320 D6 01               505 	ldab	*ZD0+1	; tstqi: R:*ZD0
   0322 26 03               506 	bne	.+5
   0324 7E 03 7C            507 	jmp	L36	; (beq) long branch
   0327 FC 27 34            508 	ldd	_current_tcb_p
   032A DD 00               509 	std	*ZD5	; movhi: _current_tcb_p -> *ZD5
   032C DE 00               510 	ldx	*ZD5
   032E EC 01               511 	ldd	1,x
   0330 DD 00               512 	std	*ZD6	; movhi: 1,x -> *ZD6
   0332 DC 00               513 	ldd	*ZD6
   0334 DE 00               514 	ldx	*ZD5
   0336 1A A3 03            515 	cpd	3,x	; cmphi *ZD6 with 3,x
   0339 23 03               516 	bls	.+5
   033B 7E 03 4A            517 	jmp	L38	; (bgtu) long branch
   033E DC 00               518 	ldd	*ZD6
   0340 DE 00               519 	ldx	*ZD5
   0342 1A A3 05            520 	cpd	5,x	; cmphi *ZD6 with 5,x
   0345 25 03               521 	blo	.+5
   0347 7E 03 4D            522 	jmp	L37	; (bgeu) long branch
   034A                     523 L38:
   034A BD 08 58            524 	jsr	_presto_fatal_error	; CALL: (VOIDmode) _presto_fatal_error (0 bytes)
   034D                     525 L37:
   034D FC 27 34            526 	ldd	_current_tcb_p
   0350 DD 00               527 	std	*ZD5	; movhi: _current_tcb_p -> *ZD5
   0352 DC 00               528 	ldd	*ZD5
   0354 C3 00 01            529 	addd	#1
   0357 DD 00               530 	std	*ZD7	; addhi3: *ZD5 by #1 -> *ZD7
   0359 DC 00               531 	ldd	*ZD7
   035B FD 27 30            532 	std	_presto_asm_old_sp_p	; movhi: *ZD7 -> _presto_asm_old_sp_p
   035E CC 00 00            533 	ldd	#0
   0361 DE 00               534 	ldx	*ZD5
   0363 ED 08               535 	std	8,x	; movhi: #0 -> 8,x
   0365 BD 03 8D            536 	jsr	_presto_next_tcb_to_run	; CALL: R:*ZD0 = _presto_next_tcb_to_run (0 bytes)
   0368 DC 00               537 	ldd	*ZD0
   036A FD 27 34            538 	std	_current_tcb_p	; movhi: *ZD0 -> _current_tcb_p
   036D DE 00               539 	ldx	*ZD0
   036F EC 01               540 	ldd	1,x
   0371 FD 27 2E            541 	std	_presto_asm_new_sp	; movhi: 1,x -> _presto_asm_new_sp
   0374 C6 01               542 	ldab	#1
   0376 F7 98 00            543 	stab	_presto_asm_swap	; movqi: #1 -> _presto_asm_swap
   0379 7E 03 81            544 	jmp	L39
   037C                     545 L36:
   037C D6 01               546 	ldab	*ZD0+1
   037E F7 98 00            547 	stab	_presto_asm_swap	; movqi: *ZD0 -> _presto_asm_swap
   0381                     548 L39:
                            549 ;;;EPILOGUE
   0381 38                  550 	pulx	; Pulling register *ZD7
   0382 DF 00               551 	stx	*ZD7
   0384 38                  552 	pulx	; Pulling register *ZD6
   0385 DF 00               553 	stx	*ZD6
   0387 38                  554 	pulx	; Pulling register *ZD5
   0388 DF 00               555 	stx	*ZD5
   038A 18 38               556 	puly	; Restore stack frame
   038C 39                  557 	rts	; return from function
                            558 ;;;-----------------------------------------
                            559 ;;; END EPILOGUE for presto_service_timer_interrupt
                            560 ;;;-----------------------------------------
   038D                     561 _presto_next_tcb_to_run:
                            562 ;;;-----------------------------------------
                            563 ;;;  PROLOGUE for presto_next_tcb_to_run
                            564 ;;;-----------------------------------------
   038D 18 3C               565 	pshy	; Save stack frame
   038F 18 30               566 	tsy	; Set current stack frame
   0391 DE 00               567 	ldx	*ZD5
   0393 3C                  568 	pshx	; pushed register *ZD5
                            569 ;;;END PROLOGUE
   0394 FC 27 36            570 	ldd	_tcb_head_p
   0397 DD 00               571 	std	*ZD5	; movhi: _tcb_head_p -> *ZD5
   0399 26 03               572 	bne	.+5
   039B 7E 03 B9            573 	jmp	L42	; (beq) long branch
   039E                     574 L43:
   039E DE 00               575 	ldx	*ZD5
   03A0 EC 08               576 	ldd	8,x	; tsthi: R:8,x
   03A2 27 03               577 	beq	.+5
   03A4 7E 03 AE            578 	jmp	L44	; (bne) long branch
   03A7 DC 00               579 	ldd	*ZD5
   03A9 DD 00               580 	std	*ZD0	; movhi: *ZD5 -> *ZD0
   03AB 7E 03 C1            581 	jmp	L46
   03AE                     582 L44:
   03AE DE 00               583 	ldx	*ZD5
   03B0 EC 0A               584 	ldd	10,x
   03B2 DD 00               585 	std	*ZD5	; movhi: 10,x -> *ZD5
   03B4 27 03               586 	beq	.+5
   03B6 7E 03 9E            587 	jmp	L43	; (bne) long branch
   03B9                     588 L42:
   03B9 BD 08 58            589 	jsr	_presto_fatal_error	; CALL: (VOIDmode) _presto_fatal_error (0 bytes)
   03BC CC 00 00            590 	ldd	#0
   03BF DD 00               591 	std	*ZD0	; movhi: #0 -> *ZD0
   03C1                     592 L46:
                            593 ;;;EPILOGUE
   03C1 38                  594 	pulx	; Pulling register *ZD5
   03C2 DF 00               595 	stx	*ZD5
   03C4 18 38               596 	puly	; Restore stack frame
   03C6 39                  597 	rts	; return from function
                            598 ;;;-----------------------------------------
                            599 ;;; END EPILOGUE for presto_next_tcb_to_run
                            600 ;;;-----------------------------------------
                            601 	; extern	_presto_timer
                            602 	.globl	_presto_send_message
   03C7                     603 _presto_send_message:
                            604 ;;;-----------------------------------------
                            605 ;;;  PROLOGUE for presto_send_message
                            606 ;;;-----------------------------------------
   03C7 18 3C               607 	pshy	; Save stack frame
   03C9 18 30               608 	tsy	; Set current stack frame
   03CB 8F                  609 	xgdx	; saving D register
   03CC D6 00               610 	ldab	*ZB1
   03CE 37                  611 	pshb	; pushed register *ZB1
   03CF 8F                  612 	xgdx	; restoring D register
                            613 ;;;END PROLOGUE
   03D0 CD EE 06            614 	ldx	6,y
   03D3 3C                  615 	pshx		; movhi: 6,y -> ,--sp
   03D4 CD EE 04            616 	ldx	4,y
   03D7 3C                  617 	pshx		; movhi: 4,y -> ,--sp
   03D8 CE 00 00            618 	ldx	#0
   03DB 3C                  619 	pshx		; movhi: #0 -> ,--sp
   03DC BD 03 F3            620 	jsr	_presto_timer	; CALL: R:*ZD0 = _presto_timer (6 bytes)
   03DF D6 01               621 	ldab	*ZD0+1
   03E1 D7 00               622 	stab	*ZB1	; movqi: *ZD0 -> *ZB1
   03E3 D6 00               623 	ldab	*ZB1
   03E5 4F                  624 	clra
   03E6 DD 00               625 	std	*ZD0	; zero_extendqihi2: *ZB1 -> *ZD0
   03E8 38                  626 	pulx
   03E9 38                  627 	pulx
   03EA 38                  628 	pulx		; addhi3: sp by #6 -> sp
                            629 ;;;EPILOGUE
   03EB 8F                  630 	xgdx	; saving D register
   03EC 33                  631 	pulb	; Pulling register *ZB1
   03ED D7 00               632 	stab	*ZB1
   03EF 8F                  633 	xgdx	; restoring D register
   03F0 18 38               634 	puly	; Restore stack frame
   03F2 39                  635 	rts	; return from function
                            636 ;;;-----------------------------------------
                            637 ;;; END EPILOGUE for presto_send_message
                            638 ;;;-----------------------------------------
                            639 	; extern	_clock_compare
                            640 	.globl	_presto_timer
   03F3                     641 _presto_timer:
                            642 ;;;-----------------------------------------
                            643 ;;;  PROLOGUE for presto_timer
                            644 ;;;-----------------------------------------
   03F3 3C                  645 	pshx	; allocate 2 byte auto variable
   03F4 3C                  646 	pshx	; allocate 2 byte auto variable
   03F5 18 3C               647 	pshy	; Save stack frame
   03F7 18 30               648 	tsy	; Set current stack frame
   03F9 DE 00               649 	ldx	*ZD5
   03FB 3C                  650 	pshx	; pushed register *ZD5
   03FC DE 00               651 	ldx	*ZD6
   03FE 3C                  652 	pshx	; pushed register *ZD6
   03FF DE 00               653 	ldx	*ZD7
   0401 3C                  654 	pshx	; pushed register *ZD7
                            655 ;;;END PROLOGUE
   0402 D6 01               656 	ldab	*ZD0+1
   0404 D7 01               657 	stab	*ZD6+1	; movqi: *ZD0 -> *ZD6
                            658 ; Begin inline assembler code
   0406 0F                  659 		sei
                            660 ; End of inline assembler code
   0407 FC 27 3B            661 	ldd	_free_mail_p	; tsthi: R:_free_mail_p
   040A 27 03               662 	beq	.+5
   040C 7E 04 18            663 	jmp	L49	; (bne) long branch
                            664 ; Begin inline assembler code
   040F 0E                  665 		cli
                            666 ; End of inline assembler code
   0410 CC 00 01            667 	ldd	#1
   0413 DD 00               668 	std	*ZD0	; movhi: #1 -> *ZD0
   0415 7E 05 1E            669 	jmp	L60
   0418                     670 L49:
   0418 BD 06 96            671 	jsr	_tid_to_tcbptr	; CALL: R:*ZD0 = _tid_to_tcbptr (0 bytes)
   041B DC 00               672 	ldd	*ZD0	; tsthi: R:*ZD0
   041D 27 03               673 	beq	.+5
   041F 7E 04 2B            674 	jmp	L50	; (bne) long branch
                            675 ; Begin inline assembler code
   0422 0E                  676 		cli
                            677 ; End of inline assembler code
   0423 CC 00 02            678 	ldd	#2
   0426 DD 00               679 	std	*ZD0	; movhi: #2 -> *ZD0
   0428 7E 05 1E            680 	jmp	L60
   042B                     681 L50:
   042B FC 27 3B            682 	ldd	_free_mail_p
   042E DD 00               683 	std	*ZD7	; movhi: _free_mail_p -> *ZD7
   0430 DE 00               684 	ldx	*ZD7
   0432 EC 0D               685 	ldd	13,x
   0434 FD 27 3B            686 	std	_free_mail_p	; movhi: 13,x -> _free_mail_p
   0437 FC 27 34            687 	ldd	_current_tcb_p
   043A DD 00               688 	std	*ZD5	; movhi: _current_tcb_p -> *ZD5
   043C DE 00               689 	ldx	*ZD5
   043E E6 00               690 	ldab	0,x
   0440 DE 00               691 	ldx	*ZD7
   0442 E7 02               692 	stab	2,x	; movqi: 0,x -> 2,x
   0444 D6 01               693 	ldab	*ZD6+1
   0446 D7 01               694 	stab	*ZD0+1	; movqi: *ZD6 -> *ZD0
   0448 BD 06 96            695 	jsr	_tid_to_tcbptr	; CALL: R:*ZD0 = _tid_to_tcbptr (0 bytes)
   044B DC 00               696 	ldd	*ZD0
   044D DE 00               697 	ldx	*ZD7
   044F ED 03               698 	std	3,x	; movhi: *ZD0 -> 3,x
   0451 CD EE 08            699 	ldx	8,y
   0454 3C                  700 	pshx		; movhi: 8,y -> ,--sp
   0455 FC 98 B1            701 	ldd	_presto_master_clock
   0458 DD 00               702 	std	*ZD0	; movhi: _presto_master_clock -> *ZD0
   045A FC 98 B3            703 	ldd	_presto_master_clock+2
   045D ED 00               704 	std 0,x	; movhi: _presto_master_clock+2 -> x
   045F BD 07 77            705 	jsr	_clock_add	; CALL: (VOIDmode) _clock_add (2 bytes)
   0462 18 EC 02            706 	ldd	2,y
   0465 DE 00               707 	ldx	*ZD7
   0467 ED 05               708 	std	5,x	; movhi: 2,y -> 5,x
   0469 18 EC 04            709 	ldd	4,y
   046C DE 00               710 	ldx	*ZD7
   046E ED 07               711 	std	7,x	; movhi: 4,y -> 7,x
   0470 18 EC 0A            712 	ldd	10,y
   0473 DE 00               713 	ldx	*ZD7
   0475 ED 09               714 	std	9,x	; movhi: 10,y -> 9,x
   0477 18 EC 0C            715 	ldd	12,y
   047A DE 00               716 	ldx	*ZD7
   047C ED 0B               717 	std	11,x	; movhi: 12,y -> 11,x
   047E 38                  718 	pulx		; addhi3: sp by #2 -> sp
   047F FC 27 3D            719 	ldd	_po_mail_p
   0482 DD 00               720 	std	*ZD5	; movhi: _po_mail_p -> *ZD5
   0484 27 03               721 	beq	.+5
   0486 7E 04 98            722 	jmp	L51	; (bne) long branch
   0489 DC 00               723 	ldd	*ZD7
   048B FD 27 3D            724 	std	_po_mail_p	; movhi: *ZD7 -> _po_mail_p
   048E CC 00 00            725 	ldd	#0
   0491 DE 00               726 	ldx	*ZD7
   0493 ED 0D               727 	std	13,x	; movhi: #0 -> 13,x
   0495 7E 05 18            728 	jmp	L52
   0498                     729 L51:
   0498 DE 00               730 	ldx	*ZD7
   049A EE 07               731 	ldx	7,x
   049C 3C                  732 	pshx		; movhi: 7,x -> ,--sp
   049D DE 00               733 	ldx	*ZD7
   049F EE 05               734 	ldx	5,x
   04A1 3C                  735 	pshx		; movhi: 5,x -> ,--sp
   04A2 DE 00               736 	ldx	*ZD5
   04A4 EC 05               737 	ldd	5,x
   04A6 DD 00               738 	std	*ZD0	; movhi: 5,x -> *ZD0
   04A8 DE 00               739 	ldx	*ZD5
   04AA EC 07               740 	ldd	7,x
   04AC ED 00               741 	std 0,x	; movhi: 7,x -> x
   04AE BD 07 D2            742 	jsr	_clock_compare	; CALL: R:*ZD0 = _clock_compare (4 bytes)
   04B1 38                  743 	pulx
   04B2 38                  744 	pulx		; addhi3: sp by #4 -> sp
   04B3 D6 01               745 	ldab	*ZD0+1	; tstqi: R:*ZD0
   04B5 2E 03               746 	bgt	.+5
   04B7 7E 04 C9            747 	jmp	L53	; (ble) long branch
   04BA FC 27 3D            748 	ldd	_po_mail_p
   04BD DE 00               749 	ldx	*ZD7
   04BF ED 0D               750 	std	13,x	; movhi: _po_mail_p -> 13,x
   04C1 DC 00               751 	ldd	*ZD7
   04C3 FD 27 3D            752 	std	_po_mail_p	; movhi: *ZD7 -> _po_mail_p
   04C6 7E 05 18            753 	jmp	L52
   04C9                     754 L53:
   04C9 FC 27 3D            755 	ldd	_po_mail_p
   04CC DD 00               756 	std	*ZD6	; movhi: _po_mail_p -> *ZD6
   04CE DE 00               757 	ldx	*ZD6
   04D0 EC 0D               758 	ldd	13,x	; tsthi: R:13,x
   04D2 26 03               759 	bne	.+5
   04D4 7E 05 0A            760 	jmp	L56	; (beq) long branch
   04D7                     761 L57:
   04D7 DE 00               762 	ldx	*ZD6
   04D9 EC 0D               763 	ldd	13,x
   04DB DD 00               764 	std	*ZD5	; movhi: 13,x -> *ZD5
   04DD DE 00               765 	ldx	*ZD7
   04DF EE 07               766 	ldx	7,x
   04E1 3C                  767 	pshx		; movhi: 7,x -> ,--sp
   04E2 DE 00               768 	ldx	*ZD7
   04E4 EE 05               769 	ldx	5,x
   04E6 3C                  770 	pshx		; movhi: 5,x -> ,--sp
   04E7 DE 00               771 	ldx	*ZD5
   04E9 EC 05               772 	ldd	5,x
   04EB DD 00               773 	std	*ZD0	; movhi: 5,x -> *ZD0
   04ED DE 00               774 	ldx	*ZD5
   04EF EC 07               775 	ldd	7,x
   04F1 ED 00               776 	std 0,x	; movhi: 7,x -> x
   04F3 BD 07 D2            777 	jsr	_clock_compare	; CALL: R:*ZD0 = _clock_compare (4 bytes)
   04F6 38                  778 	pulx
   04F7 38                  779 	pulx		; addhi3: sp by #4 -> sp
   04F8 D6 01               780 	ldab	*ZD0+1	; tstqi: R:*ZD0
   04FA 2F 03               781 	ble	.+5
   04FC 7E 05 0A            782 	jmp	L56	; (bgt) long branch
   04FF DE 00               783 	ldx	*ZD6
   0501 EC 0D               784 	ldd	13,x
   0503 DD 00               785 	std	*ZD6	; movhi: 13,x -> *ZD6
   0505 27 03               786 	beq	.+5
   0507 7E 04 D7            787 	jmp	L57	; (bne) long branch
   050A                     788 L56:
   050A DE 00               789 	ldx	*ZD6
   050C EC 0D               790 	ldd	13,x
   050E DE 00               791 	ldx	*ZD7
   0510 ED 0D               792 	std	13,x	; movhi: 13,x -> 13,x
   0512 DC 00               793 	ldd	*ZD7
   0514 DE 00               794 	ldx	*ZD6
   0516 ED 0D               795 	std	13,x	; movhi: *ZD7 -> 13,x
   0518                     796 L52:
                            797 ; Begin inline assembler code
   0518 0E                  798 		cli
                            799 ; End of inline assembler code
   0519 CC 00 00            800 	ldd	#0
   051C DD 00               801 	std	*ZD0	; movhi: #0 -> *ZD0
   051E                     802 L60:
                            803 ;;;EPILOGUE
   051E 38                  804 	pulx	; Pulling register *ZD7
   051F DF 00               805 	stx	*ZD7
   0521 38                  806 	pulx	; Pulling register *ZD6
   0522 DF 00               807 	stx	*ZD6
   0524 38                  808 	pulx	; Pulling register *ZD5
   0525 DF 00               809 	stx	*ZD5
   0527 18 38               810 	puly	; Restore stack frame
   0529 38                  811 	pulx	; deallocate 2 byte auto variable
   052A 38                  812 	pulx	; deallocate 2 byte auto variable
   052B 39                  813 	rts	; return from function
                            814 ;;;-----------------------------------------
                            815 ;;; END EPILOGUE for presto_timer
                            816 ;;;-----------------------------------------
   052C                     817 _deliver_mail:
                            818 ;;;-----------------------------------------
                            819 ;;;  PROLOGUE for deliver_mail
                            820 ;;;-----------------------------------------
   052C 18 3C               821 	pshy	; Save stack frame
   052E 18 30               822 	tsy	; Set current stack frame
   0530 DE 00               823 	ldx	*ZD5
   0532 3C                  824 	pshx	; pushed register *ZD5
   0533 DE 00               825 	ldx	*ZD6
   0535 3C                  826 	pshx	; pushed register *ZD6
   0536 DE 00               827 	ldx	*ZD7
   0538 3C                  828 	pshx	; pushed register *ZD7
   0539 8F                  829 	xgdx	; saving D register
   053A D6 00               830 	ldab	*ZB1
   053C 37                  831 	pshb	; pushed register *ZB1
   053D 8F                  832 	xgdx	; restoring D register
                            833 ;;;END PROLOGUE
   053E C6 00               834 	ldab	#0
   0540 D7 00               835 	stab	*ZB1	; movqi: #0 -> *ZB1
   0542 FC 27 3D            836 	ldd	_po_mail_p	; tsthi: R:_po_mail_p
   0545 26 03               837 	bne	.+5
   0547 7E 05 CB            838 	jmp	L63	; (beq) long branch
   054A                     839 L69:
   054A FC 27 3D            840 	ldd	_po_mail_p
   054D DD 00               841 	std	*ZD5	; movhi: _po_mail_p -> *ZD5
   054F FE 98 B3            842 	ldx	_presto_master_clock+2
   0552 3C                  843 	pshx		; movhi: _presto_master_clock+2 -> ,--sp
   0553 FE 98 B1            844 	ldx	_presto_master_clock
   0556 3C                  845 	pshx		; movhi: _presto_master_clock -> ,--sp
   0557 DE 00               846 	ldx	*ZD5
   0559 EC 05               847 	ldd	5,x
   055B DD 00               848 	std	*ZD0	; movhi: 5,x -> *ZD0
   055D DE 00               849 	ldx	*ZD5
   055F EC 07               850 	ldd	7,x
   0561 ED 00               851 	std 0,x	; movhi: 7,x -> x
   0563 BD 07 D2            852 	jsr	_clock_compare	; CALL: R:*ZD0 = _clock_compare (4 bytes)
   0566 38                  853 	pulx
   0567 38                  854 	pulx		; addhi3: sp by #4 -> sp
   0568 D6 01               855 	ldab	*ZD0+1	; tstqi: R:*ZD0
   056A 2F 03               856 	ble	.+5
   056C 7E 05 CB            857 	jmp	L63	; (bgt) long branch
   056F FC 27 3D            858 	ldd	_po_mail_p
   0572 DD 00               859 	std	*ZD5	; movhi: _po_mail_p -> *ZD5
   0574 DE 00               860 	ldx	*ZD5
   0576 EC 03               861 	ldd	3,x
   0578 DD 00               862 	std	*ZD6	; movhi: 3,x -> *ZD6
   057A 27 03               863 	beq	.+5
   057C 7E 05 82            864 	jmp	L66	; (bne) long branch
   057F BD 08 58            865 	jsr	_presto_fatal_error	; CALL: (VOIDmode) _presto_fatal_error (0 bytes)
   0582                     866 L66:
   0582 CC 00 00            867 	ldd	#0
   0585 DE 00               868 	ldx	*ZD6
   0587 ED 08               869 	std	8,x	; movhi: #0 -> 8,x
   0589 FC 27 3D            870 	ldd	_po_mail_p
   058C DD 00               871 	std	*ZD7	; movhi: _po_mail_p -> *ZD7
   058E DE 00               872 	ldx	*ZD7
   0590 EC 0D               873 	ldd	13,x
   0592 FD 27 3D            874 	std	_po_mail_p	; movhi: 13,x -> _po_mail_p
   0595 DE 00               875 	ldx	*ZD6
   0597 EC 0C               876 	ldd	12,x	; tsthi: R:12,x
   0599 27 03               877 	beq	.+5
   059B 7E 05 A7            878 	jmp	L67	; (bne) long branch
   059E DC 00               879 	ldd	*ZD7
   05A0 DE 00               880 	ldx	*ZD6
   05A2 ED 0C               881 	std	12,x	; movhi: *ZD7 -> 12,x
   05A4 7E 05 B3            882 	jmp	L70
   05A7                     883 L67:
   05A7 DE 00               884 	ldx	*ZD6
   05A9 EC 0E               885 	ldd	14,x
   05AB DD 00               886 	std	*ZD5	; movhi: 14,x -> *ZD5
   05AD DC 00               887 	ldd	*ZD7
   05AF DE 00               888 	ldx	*ZD5
   05B1 ED 0D               889 	std	13,x	; movhi: *ZD7 -> 13,x
   05B3                     890 L70:
   05B3 DC 00               891 	ldd	*ZD7
   05B5 DE 00               892 	ldx	*ZD6
   05B7 ED 0E               893 	std	14,x	; movhi: *ZD7 -> 14,x
   05B9 CC 00 00            894 	ldd	#0
   05BC DE 00               895 	ldx	*ZD7
   05BE ED 0D               896 	std	13,x	; movhi: #0 -> 13,x
   05C0 7C 00 00            897 	inc	*ZB1	; addqi3: *ZB1 by #1 -> *ZB1
   05C3 FC 27 3D            898 	ldd	_po_mail_p	; tsthi: R:_po_mail_p
   05C6 27 03               899 	beq	.+5
   05C8 7E 05 4A            900 	jmp	L69	; (bne) long branch
   05CB                     901 L63:
   05CB D6 00               902 	ldab	*ZB1
   05CD 4F                  903 	clra
   05CE DD 00               904 	std	*ZD0	; zero_extendqihi2: *ZB1 -> *ZD0
                            905 ;;;EPILOGUE
   05D0 8F                  906 	xgdx	; saving D register
   05D1 33                  907 	pulb	; Pulling register *ZB1
   05D2 D7 00               908 	stab	*ZB1
   05D4 8F                  909 	xgdx	; restoring D register
   05D5 38                  910 	pulx	; Pulling register *ZD7
   05D6 DF 00               911 	stx	*ZD7
   05D8 38                  912 	pulx	; Pulling register *ZD6
   05D9 DF 00               913 	stx	*ZD6
   05DB 38                  914 	pulx	; Pulling register *ZD5
   05DC DF 00               915 	stx	*ZD5
   05DE 18 38               916 	puly	; Restore stack frame
   05E0 39                  917 	rts	; return from function
                            918 ;;;-----------------------------------------
                            919 ;;; END EPILOGUE for deliver_mail
                            920 ;;;-----------------------------------------
                            921 	.globl	_presto_mail_waiting
   05E1                     922 _presto_mail_waiting:
                            923 ;;;-----------------------------------------
                            924 ;;;  PROLOGUE for presto_mail_waiting
                            925 ;;;-----------------------------------------
   05E1 18 3C               926 	pshy	; Save stack frame
   05E3 18 30               927 	tsy	; Set current stack frame
   05E5 DE 00               928 	ldx	*ZD5
   05E7 3C                  929 	pshx	; pushed register *ZD5
                            930 ;;;END PROLOGUE
   05E8 FC 27 34            931 	ldd	_current_tcb_p
   05EB DD 00               932 	std	*ZD5	; movhi: _current_tcb_p -> *ZD5
   05ED DE 00               933 	ldx	*ZD5
   05EF EC 0C               934 	ldd	12,x
   05F1 DD 00               935 	std	*ZD5	; movhi: 12,x -> *ZD5
   05F3 26 03               936 	bne	.+5
   05F5 7E 05 FD            937 	jmp	L72	; (beq) long branch
   05F8 CC 00 01            938 	ldd	#1
   05FB DD 00               939 	std	*ZD5	; movhi: #1 -> *ZD5
   05FD                     940 L72:
   05FD DC 00               941 	ldd	*ZD5
   05FF DD 00               942 	std	*ZD0	; movhi: *ZD5 -> *ZD0
                            943 ;;;EPILOGUE
   0601 38                  944 	pulx	; Pulling register *ZD5
   0602 DF 00               945 	stx	*ZD5
   0604 18 38               946 	puly	; Restore stack frame
   0606 39                  947 	rts	; return from function
                            948 ;;;-----------------------------------------
                            949 ;;; END EPILOGUE for presto_mail_waiting
                            950 ;;;-----------------------------------------
                            951 	.globl	_presto_get_message
   0607                     952 _presto_get_message:
                            953 ;;;-----------------------------------------
                            954 ;;;  PROLOGUE for presto_get_message
                            955 ;;;-----------------------------------------
   0607 18 3C               956 	pshy	; Save stack frame
   0609 18 30               957 	tsy	; Set current stack frame
   060B DE 00               958 	ldx	*ZD5
   060D 3C                  959 	pshx	; pushed register *ZD5
   060E DE 00               960 	ldx	*ZD6
   0610 3C                  961 	pshx	; pushed register *ZD6
   0611 DE 00               962 	ldx	*ZD7
   0613 3C                  963 	pshx	; pushed register *ZD7
                            964 ;;;END PROLOGUE
   0614 DC 00               965 	ldd	*ZD0
   0616 DD 00               966 	std	*ZD7	; movhi: *ZD0 -> *ZD7
                            967 ; Begin inline assembler code
   0618 0F                  968 		sei
                            969 ; End of inline assembler code
   0619 FC 27 34            970 	ldd	_current_tcb_p
   061C DD 00               971 	std	*ZD6	; movhi: _current_tcb_p -> *ZD6
   061E DE 00               972 	ldx	*ZD6
   0620 EC 0C               973 	ldd	12,x
   0622 DD 00               974 	std	*ZD5	; movhi: 12,x -> *ZD5
   0624 27 03               975 	beq	.+5
   0626 7E 06 32            976 	jmp	L74	; (bne) long branch
                            977 ; Begin inline assembler code
   0629 0E                  978 		cli
                            979 ; End of inline assembler code
   062A CC 00 00            980 	ldd	#0
   062D DD 00               981 	std	*ZD0	; movhi: #0 -> *ZD0
   062F 7E 06 80            982 	jmp	L79
   0632                     983 L74:
   0632 DC 00               984 	ldd	*ZD5
   0634 DE 00               985 	ldx	*ZD6
   0636 1A A3 0E            986 	cpd	14,x	; cmphi *ZD5 with 14,x
   0639 27 03               987 	beq	.+5
   063B 7E 06 4F            988 	jmp	L76	; (bne) long branch
   063E CC 00 00            989 	ldd	#0
   0641 DE 00               990 	ldx	*ZD6
   0643 ED 0C               991 	std	12,x	; movhi: #0 -> 12,x
   0645 CC 00 00            992 	ldd	#0
   0648 DE 00               993 	ldx	*ZD6
   064A ED 0E               994 	std	14,x	; movhi: #0 -> 14,x
   064C 7E 06 57            995 	jmp	L75
   064F                     996 L76:
   064F DE 00               997 	ldx	*ZD5
   0651 EC 0D               998 	ldd	13,x
   0653 DE 00               999 	ldx	*ZD6
   0655 ED 0C              1000 	std	12,x	; movhi: 13,x -> 12,x
   0657                    1001 L75:
   0657 DC 00              1002 	ldd	*ZD7	; tsthi: R:*ZD7
   0659 26 03              1003 	bne	.+5
   065B 7E 06 6E           1004 	jmp	L78	; (beq) long branch
   065E DE 00              1005 	ldx	*ZD5
   0660 EC 09              1006 	ldd	9,x
   0662 DE 00              1007 	ldx	*ZD7
   0664 ED 00              1008 	std	0,x	; movhi: 9,x -> 0,x
   0666 DE 00              1009 	ldx	*ZD5
   0668 EC 0B              1010 	ldd	11,x
   066A DE 00              1011 	ldx	*ZD7
   066C ED 02              1012 	std	2,x	; movhi: 11,x -> 2,x
   066E                    1013 L78:
   066E FC 27 3B           1014 	ldd	_free_mail_p
   0671 DE 00              1015 	ldx	*ZD5
   0673 ED 0D              1016 	std	13,x	; movhi: _free_mail_p -> 13,x
   0675 DC 00              1017 	ldd	*ZD5
   0677 FD 27 3B           1018 	std	_free_mail_p	; movhi: *ZD5 -> _free_mail_p
                           1019 ; Begin inline assembler code
   067A 0E                 1020 		cli
                           1021 ; End of inline assembler code
   067B CC 00 01           1022 	ldd	#1
   067E DD 00              1023 	std	*ZD0	; movhi: #1 -> *ZD0
   0680                    1024 L79:
                           1025 ;;;EPILOGUE
   0680 38                 1026 	pulx	; Pulling register *ZD7
   0681 DF 00              1027 	stx	*ZD7
   0683 38                 1028 	pulx	; Pulling register *ZD6
   0684 DF 00              1029 	stx	*ZD6
   0686 38                 1030 	pulx	; Pulling register *ZD5
   0687 DF 00              1031 	stx	*ZD5
   0689 18 38              1032 	puly	; Restore stack frame
   068B 39                 1033 	rts	; return from function
                           1034 ;;;-----------------------------------------
                           1035 ;;; END EPILOGUE for presto_get_message
                           1036 ;;;-----------------------------------------
   068C                    1037 _idle_task:
                           1038 ;;;-----------------------------------------
                           1039 ;;;  PROLOGUE for idle_task
                           1040 ;;;-----------------------------------------
   068C 18 3C              1041 	pshy	; Save stack frame
   068E 18 30              1042 	tsy	; Set current stack frame
                           1043 ;;;END PROLOGUE
   0690                    1044 L81:
   0690 7E 06 90           1045 	jmp	L81
                           1046 ;;;EPILOGUE
   0693 18 38              1047 	puly	; Restore stack frame
   0695 39                 1048 	rts	; return from function
                           1049 ;;;-----------------------------------------
                           1050 ;;; END EPILOGUE for idle_task
                           1051 ;;;-----------------------------------------
   0696                    1052 _tid_to_tcbptr:
                           1053 ;;;-----------------------------------------
                           1054 ;;;  PROLOGUE for tid_to_tcbptr
                           1055 ;;;-----------------------------------------
   0696 18 3C              1056 	pshy	; Save stack frame
   0698 18 30              1057 	tsy	; Set current stack frame
   069A DE 00              1058 	ldx	*ZD5
   069C 3C                 1059 	pshx	; pushed register *ZD5
   069D 8F                 1060 	xgdx	; saving D register
   069E D6 00              1061 	ldab	*ZB1
   06A0 37                 1062 	pshb	; pushed register *ZB1
   06A1 8F                 1063 	xgdx	; restoring D register
                           1064 ;;;END PROLOGUE
   06A2 D6 01              1065 	ldab	*ZD0+1
   06A4 D7 01              1066 	stab	*ZD5+1	; movqi: *ZD0 -> *ZD5
   06A6 D6 01              1067 	ldab	*ZD5+1
   06A8 C1 0A              1068 	cmpb	#10	; cmpqi *ZD5 with #10
   06AA 22 03              1069 	bhi	.+5
   06AC 7E 06 B2           1070 	jmp	L86	; (bleu) long branch
   06AF BD 08 58           1071 	jsr	_presto_fatal_error	; CALL: (VOIDmode) _presto_fatal_error (0 bytes)
   06B2                    1072 L86:
   06B2 D6 01              1073 	ldab	*ZD5+1
   06B4 D7 00              1074 	stab	*ZB1	; movqi: *ZD5 -> *ZB1
   06B6 D6 00              1075 	ldab	*ZB1
   06B8 4F                 1076 	clra
   06B9 DD 00              1077 	std	*ZD5	; zero_extendqihi2: *ZB1 -> *ZD5
   06BB DC 00              1078 	ldd	*ZD5
   06BD 05                 1079 	asld
   06BE 05                 1080 	asld
   06BF 05                 1081 	asld
   06C0 05                 1082 	asld
   06C1 DD 00              1083 	std	*ZD5	; ashlhi3: *ZD5 by #4 -> *ZD5
   06C3 CC 00 00           1084 	ldd	#0
   06C6 DD 00              1085 	std	*ZD0	; movhi: #0 -> *ZD0
   06C8 DE 00              1086 	ldx	*ZD5
   06CA 8F                 1087 	xgdx
   06CB C3 98 09           1088 	addd	#_tcb_list+8
   06CE 8F                 1089 	xgdx
                           1090 ;;; (output_swap_regx: addr computed)
   06CF EC 00              1091 	ldd	0,x
   06D1 1A 83 00 02        1092 	cpd	#2	; cmphi 0,x with #2
   06D5 26 03              1093 	bne	.+5
   06D7 7E 06 E1           1094 	jmp	L87	; (beq) long branch
   06DA DC 00              1095 	ldd	*ZD5
   06DC C3 98 01           1096 	addd	#_tcb_list
   06DF DD 00              1097 	std	*ZD0	; addhi3: *ZD5 by #_tcb_list -> *ZD0
   06E1                    1098 L87:
                           1099 ;;;EPILOGUE
   06E1 8F                 1100 	xgdx	; saving D register
   06E2 33                 1101 	pulb	; Pulling register *ZB1
   06E3 D7 00              1102 	stab	*ZB1
   06E5 8F                 1103 	xgdx	; restoring D register
   06E6 38                 1104 	pulx	; Pulling register *ZD5
   06E7 DF 00              1105 	stx	*ZD5
   06E9 18 38              1106 	puly	; Restore stack frame
   06EB 39                 1107 	rts	; return from function
                           1108 ;;;-----------------------------------------
                           1109 ;;; END EPILOGUE for tid_to_tcbptr
                           1110 ;;;-----------------------------------------
   06EC                    1111 _presto_start_master_timer:
                           1112 ;;;-----------------------------------------
                           1113 ;;;  PROLOGUE for presto_start_master_timer
                           1114 ;;;-----------------------------------------
   06EC 18 3C              1115 	pshy	; Save stack frame
   06EE 18 30              1116 	tsy	; Set current stack frame
   06F0 DE 00              1117 	ldx	*ZD5
   06F2 3C                 1118 	pshx	; pushed register *ZD5
                           1119 ;;;END PROLOGUE
   06F3 FC 10 0E           1120 	ldd	4110
   06F6 DD 00              1121 	std	*ZD5	; movhi: 4110 -> *ZD5
   06F8 DC 00              1122 	ldd	*ZD5
   06FA C3 27 10           1123 	addd	#10000
   06FD DD 00              1124 	std	*ZD5	; addhi3: *ZD5 by #10000 -> *ZD5
   06FF DC 00              1125 	ldd	*ZD5
   0701 FD 10 18           1126 	std	4120	; movhi: *ZD5 -> 4120
   0704 F6 10 22           1127 	ldab	4130
   0707 D7 01              1128 	stab	*ZD5+1	; movqi: 4130 -> *ZD5
   0709 D6 01              1129 	ldab	*ZD5+1
   070B CA 40              1130 	orab	#64
   070D D7 01              1131 	stab	*ZD5+1	; iorqi3: *ZD5 by #64 -> *ZD5
   070F D6 01              1132 	ldab	*ZD5+1
   0711 F7 10 22           1133 	stab	4130	; movqi: *ZD5 -> 4130
   0714 C6 40              1134 	ldab	#64
   0716 F7 10 23           1135 	stab	4131	; movqi: #64 -> 4131
   0719 F6 10 20           1136 	ldab	4128
   071C D7 01              1137 	stab	*ZD5+1	; movqi: 4128 -> *ZD5
   071E D6 01              1138 	ldab	*ZD5+1
   0720 C4 3F              1139 	andb	#63
   0722 D7 01              1140 	stab	*ZD5+1	; andqi3: *ZD5 by #63 -> *ZD5
   0724 D6 01              1141 	ldab	*ZD5+1
   0726 F7 10 20           1142 	stab	4128	; movqi: *ZD5 -> 4128
                           1143 ;;;EPILOGUE
   0729 38                 1144 	pulx	; Pulling register *ZD5
   072A DF 00              1145 	stx	*ZD5
   072C 18 38              1146 	puly	; Restore stack frame
   072E 39                 1147 	rts	; return from function
                           1148 ;;;-----------------------------------------
                           1149 ;;; END EPILOGUE for presto_start_master_timer
                           1150 ;;;-----------------------------------------
   072F                    1151 _presto_restart_master_timer:
                           1152 ;;;-----------------------------------------
                           1153 ;;;  PROLOGUE for presto_restart_master_timer
                           1154 ;;;-----------------------------------------
   072F 18 3C              1155 	pshy	; Save stack frame
   0731 18 30              1156 	tsy	; Set current stack frame
   0733 DE 00              1157 	ldx	*ZD5
   0735 3C                 1158 	pshx	; pushed register *ZD5
                           1159 ;;;END PROLOGUE
   0736 FC 10 18           1160 	ldd	4120
   0739 DD 00              1161 	std	*ZD5	; movhi: 4120 -> *ZD5
   073B DC 00              1162 	ldd	*ZD5
   073D C3 27 10           1163 	addd	#10000
   0740 DD 00              1164 	std	*ZD5	; addhi3: *ZD5 by #10000 -> *ZD5
   0742 DC 00              1165 	ldd	*ZD5
   0744 FD 10 18           1166 	std	4120	; movhi: *ZD5 -> 4120
   0747 C6 40              1167 	ldab	#64
   0749 F7 10 23           1168 	stab	4131	; movqi: #64 -> 4131
                           1169 ;;;EPILOGUE
   074C 38                 1170 	pulx	; Pulling register *ZD5
   074D DF 00              1171 	stx	*ZD5
   074F 18 38              1172 	puly	; Restore stack frame
   0751 39                 1173 	rts	; return from function
                           1174 ;;;-----------------------------------------
                           1175 ;;; END EPILOGUE for presto_restart_master_timer
                           1176 ;;;-----------------------------------------
                           1177 	.area	_BSS
                           1178 	.globl	_presto_asm_swap
   0000                    1179 _presto_asm_swap:	.blkb	1
   0001                    1180 _tcb_list:	.blkb	176
   00B1                    1181 _presto_master_clock:	.blkb	4
   00B5                    1182 _idle_stack:	.blkb	150
   014B                    1183 _idle_tcb_p:	.blkb	2
   014D                    1184 _idle_tid:	.blkb	1
   014E                    1185 _mail_list:	.blkb	1500
                           1186 	; END
