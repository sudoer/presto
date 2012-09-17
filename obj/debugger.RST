                              1 ;;;-----------------------------------------
                              2 ;;; Start MC6811 gcc assembly output
                              3 ;;; gcc compiler compiled on TBD
                              4 ;;; OPTIONS:	-mlong_branch optimize  
                              5 ;;; OPTIONS:	 peephole !omit_frame_pointer !signed-char
                              6 ;;; Source:		services\debugger.c
                              7 ;;; Destination:	debugger.s
                              8 ;;; Compiled:	Tue Oct 24 21:55:11 2000
                              9 ;;; (META)compiled by GNU C version 2.8.1.
                             10 ;;;-----------------------------------------
                             11 	.module debugger.c
                             12 	; extern	_presto_create_task
                             13 	.area	_CODE
                             14 	.globl	_debugger_init
   0000                      15 _debugger_init:
                             16 ;;;-----------------------------------------
                             17 ;;;  PROLOGUE for debugger_init
                             18 ;;;-----------------------------------------
   08CC 18 3C                19 	pshy	; Save stack frame
   08CE 18 30                20 	tsy	; Set current stack frame
                             21 ;;;END PROLOGUE
   08D0 C6 01                22 	ldab	#1
   08D2 37                   23 	pshb		; movqi: #1 -> ,-sp
   08D3 CE 01 00             24 	ldx	#256
   08D6 3C                   25 	pshx		; movhi: #256 -> ,--sp
   08D7 CE 9F 53             26 	ldx	#_debugger_stack
   08DA 3C                   27 	pshx		; movhi: #_debugger_stack -> ,--sp
   08DB CC 0F 47             28 	ldd	#_debugger
   08DE DD 00                29 	std	*ZD0	; movhi: #_debugger -> *ZD0
   08E0 BD 01 32             30 	jsr	_presto_create_task	; CALL: R:*ZD0 = _presto_create_task (5 bytes)
   08E3 D6 01                31 	ldab	*ZD0+1
   08E5 F7 A0 53             32 	stab	_debugger_tid	; movqi: *ZD0 -> _debugger_tid
   08E8 38                   33 	pulx
   08E9 38                   34 	pulx
   08EA 33                   35 	pulb		; addhi3: sp by #5 -> sp
                             36 ;;;EPILOGUE
   08EB 18 38                37 	puly	; Restore stack frame
   08ED 39                   38 	rts	; return from function
                             39 ;;;-----------------------------------------
                             40 ;;; END EPILOGUE for debugger_init
                             41 ;;;-----------------------------------------
                             42 	; extern	_input_sample_analog
                             43 	; extern	_serial_send_string
   0022                      44 LC0:
   08EE 61 6E 61 6C 6F 67    45 	.ascii	"analog "
        20
   08F5 00                   46 	.byte	0x0	
                             47 	; extern	_string_IntegerToString
   002A                      48 LC1:
   08F6 20 76 61 6C 75 65    49 	.ascii	" value "
        20
   08FD 00                   50 	.byte	0x0	
   0032                      51 LC2:
   08FE 0D                   52 	.byte	0xD	
   08FF 0A                   53 	.byte	0xA	
   0900 00                   54 	.byte	0x0	
   0035                      55 _feedback_analog:
                             56 ;;;-----------------------------------------
                             57 ;;;  PROLOGUE for feedback_analog
                             58 ;;;-----------------------------------------
   0901 3C                   59 	pshx	; allocate 2 byte auto variable
   0902 34                   60 	des	; allocate 1 byte auto variable
   0903 18 3C                61 	pshy	; Save stack frame
   0905 18 30                62 	tsy	; Set current stack frame
   0907 DE 00                63 	ldx	*ZD5
   0909 3C                   64 	pshx	; pushed register *ZD5
   090A DE 00                65 	ldx	*ZD6
   090C 3C                   66 	pshx	; pushed register *ZD6
   090D DE 00                67 	ldx	*ZD7
   090F 3C                   68 	pshx	; pushed register *ZD7
   0910 8F                   69 	xgdx	; saving D register
   0911 D6 00                70 	ldab	*ZB1
   0913 37                   71 	pshb	; pushed register *ZB1
   0914 8F                   72 	xgdx	; restoring D register
                             73 ;;;END PROLOGUE
   0915 D6 01                74 	ldab	*ZD0+1
   0917 D7 01                75 	stab	*ZD5+1	; movqi: *ZD0 -> *ZD5
   0919 BD 16 11             76 	jsr	_input_sample_analog	; CALL: R:*ZD0 = _input_sample_analog (0 bytes)
   091C D6 01                77 	ldab	*ZD0+1
   091E D7 01                78 	stab	*ZD6+1	; movqi: *ZD0 -> *ZD6
   0920 CC 08 EE             79 	ldd	#LC0
   0923 DD 00                80 	std	*ZD0	; movhi: #LC0 -> *ZD0
   0925 BD 1A FA             81 	jsr	_serial_send_string	; CALL: R:*ZD0 = _serial_send_string (0 bytes)
   0928 C6 02                82 	ldab	#2
   092A 37                   83 	pshb		; movqi: #2 -> ,-sp
   092B 18 DF 00             84 	sty	*ZXT
   092E DC 00                85 	ldd	*ZXT
   0930 C3 00 02             86 	addd	#2
   0933 DD 00                87 	std	*ZD7	; addhi3: y by #2 -> *ZD7
   0935 DE 00                88 	ldx	*ZD7
   0937 3C                   89 	pshx		; movhi: *ZD7 -> ,--sp
   0938 D6 01                90 	ldab	*ZD5+1
   093A D7 00                91 	stab	*ZB1	; movqi: *ZD5 -> *ZB1
   093C D6 00                92 	ldab	*ZB1
   093E 4F                   93 	clra
   093F DD 00                94 	std	*ZD0	; zero_extendqihi2: *ZB1 -> *ZD0
   0941 BD 21 0E             95 	jsr	_string_IntegerToString	; CALL: (VOIDmode) _string_IntegerToString (3 bytes)
   0944 18 DF 00             96 	sty	*ZXT
   0947 DC 00                97 	ldd	*ZXT
   0949 C3 00 02             98 	addd	#2
   094C DD 00                99 	std	*ZD0	; addhi3: y by #2 -> *ZD0
   094E BD 1A FA            100 	jsr	_serial_send_string	; CALL: R:*ZD0 = _serial_send_string (0 bytes)
   0951 CC 08 F6            101 	ldd	#LC1
   0954 DD 00               102 	std	*ZD0	; movhi: #LC1 -> *ZD0
   0956 BD 1A FA            103 	jsr	_serial_send_string	; CALL: R:*ZD0 = _serial_send_string (0 bytes)
   0959 C6 02               104 	ldab	#2
   095B 37                  105 	pshb		; movqi: #2 -> ,-sp
   095C 18 DF 00            106 	sty	*ZXT
   095F DC 00               107 	ldd	*ZXT
   0961 C3 00 02            108 	addd	#2
   0964 DD 00               109 	std	*ZD7	; addhi3: y by #2 -> *ZD7
   0966 DE 00               110 	ldx	*ZD7
   0968 3C                  111 	pshx		; movhi: *ZD7 -> ,--sp
   0969 D6 01               112 	ldab	*ZD6+1
   096B D7 00               113 	stab	*ZB1	; movqi: *ZD6 -> *ZB1
   096D D6 00               114 	ldab	*ZB1
   096F 4F                  115 	clra
   0970 DD 00               116 	std	*ZD0	; zero_extendqihi2: *ZB1 -> *ZD0
   0972 BD 21 0E            117 	jsr	_string_IntegerToString	; CALL: (VOIDmode) _string_IntegerToString (3 bytes)
   0975 18 DF 00            118 	sty	*ZXT
   0978 DC 00               119 	ldd	*ZXT
   097A C3 00 02            120 	addd	#2
   097D DD 00               121 	std	*ZD0	; addhi3: y by #2 -> *ZD0
   097F BD 1A FA            122 	jsr	_serial_send_string	; CALL: R:*ZD0 = _serial_send_string (0 bytes)
   0982 CC 08 FE            123 	ldd	#LC2
   0985 DD 00               124 	std	*ZD0	; movhi: #LC2 -> *ZD0
   0987 BD 1A FA            125 	jsr	_serial_send_string	; CALL: R:*ZD0 = _serial_send_string (0 bytes)
   098A 38                  126 	pulx
   098B 38                  127 	pulx
   098C 38                  128 	pulx		; addhi3: sp by #6 -> sp
                            129 ;;;EPILOGUE
   098D 8F                  130 	xgdx	; saving D register
   098E 33                  131 	pulb	; Pulling register *ZB1
   098F D7 00               132 	stab	*ZB1
   0991 8F                  133 	xgdx	; restoring D register
   0992 38                  134 	pulx	; Pulling register *ZD7
   0993 DF 00               135 	stx	*ZD7
   0995 38                  136 	pulx	; Pulling register *ZD6
   0996 DF 00               137 	stx	*ZD6
   0998 38                  138 	pulx	; Pulling register *ZD5
   0999 DF 00               139 	stx	*ZD5
   099B 18 38               140 	puly	; Restore stack frame
   099D 38                  141 	pulx	; deallocate 2 byte auto variable
   099E 31                  142 	ins	; deallocate 1 byte auto variable
   099F 39                  143 	rts	; return from function
                            144 ;;;-----------------------------------------
                            145 ;;; END EPILOGUE for feedback_analog
                            146 ;;;-----------------------------------------
                            147 	; extern	_input_sample_digital
                            148 	; extern	_serial_send_string
   00D4                     149 LC3:
   09A0 64 69 67 69 74 61   150 	.ascii	"digital "
        6C 20
   09A8 00                  151 	.byte	0x0	
   00DD                     152 _feedback_digital:
                            153 ;;;-----------------------------------------
                            154 ;;;  PROLOGUE for feedback_digital
                            155 ;;;-----------------------------------------
   09A9 3C                  156 	pshx	; allocate 2 byte auto variable
   09AA 34                  157 	des	; allocate 1 byte auto variable
   09AB 18 3C               158 	pshy	; Save stack frame
   09AD 18 30               159 	tsy	; Set current stack frame
   09AF DE 00               160 	ldx	*ZD5
   09B1 3C                  161 	pshx	; pushed register *ZD5
   09B2 DE 00               162 	ldx	*ZD6
   09B4 3C                  163 	pshx	; pushed register *ZD6
   09B5 DE 00               164 	ldx	*ZD7
   09B7 3C                  165 	pshx	; pushed register *ZD7
   09B8 8F                  166 	xgdx	; saving D register
   09B9 D6 00               167 	ldab	*ZB1
   09BB 37                  168 	pshb	; pushed register *ZB1
   09BC 8F                  169 	xgdx	; restoring D register
                            170 ;;;END PROLOGUE
   09BD D6 01               171 	ldab	*ZD0+1
   09BF D7 01               172 	stab	*ZD5+1	; movqi: *ZD0 -> *ZD5
   09C1 BD 16 59            173 	jsr	_input_sample_digital	; CALL: R:*ZD0 = _input_sample_digital (0 bytes)
   09C4 D6 01               174 	ldab	*ZD0+1
   09C6 D7 01               175 	stab	*ZD6+1	; movqi: *ZD0 -> *ZD6
   09C8 CC 09 A0            176 	ldd	#LC3
   09CB DD 00               177 	std	*ZD0	; movhi: #LC3 -> *ZD0
   09CD BD 1A FA            178 	jsr	_serial_send_string	; CALL: R:*ZD0 = _serial_send_string (0 bytes)
   09D0 C6 02               179 	ldab	#2
   09D2 37                  180 	pshb		; movqi: #2 -> ,-sp
   09D3 18 DF 00            181 	sty	*ZXT
   09D6 DC 00               182 	ldd	*ZXT
   09D8 C3 00 02            183 	addd	#2
   09DB DD 00               184 	std	*ZD7	; addhi3: y by #2 -> *ZD7
   09DD DE 00               185 	ldx	*ZD7
   09DF 3C                  186 	pshx		; movhi: *ZD7 -> ,--sp
   09E0 D6 01               187 	ldab	*ZD5+1
   09E2 D7 00               188 	stab	*ZB1	; movqi: *ZD5 -> *ZB1
   09E4 D6 00               189 	ldab	*ZB1
   09E6 4F                  190 	clra
   09E7 DD 00               191 	std	*ZD0	; zero_extendqihi2: *ZB1 -> *ZD0
   09E9 BD 21 0E            192 	jsr	_string_IntegerToString	; CALL: (VOIDmode) _string_IntegerToString (3 bytes)
   09EC 18 DF 00            193 	sty	*ZXT
   09EF DC 00               194 	ldd	*ZXT
   09F1 C3 00 02            195 	addd	#2
   09F4 DD 00               196 	std	*ZD0	; addhi3: y by #2 -> *ZD0
   09F6 BD 1A FA            197 	jsr	_serial_send_string	; CALL: R:*ZD0 = _serial_send_string (0 bytes)
   09F9 CC 08 F6            198 	ldd	#LC1
   09FC DD 00               199 	std	*ZD0	; movhi: #LC1 -> *ZD0
   09FE BD 1A FA            200 	jsr	_serial_send_string	; CALL: R:*ZD0 = _serial_send_string (0 bytes)
   0A01 C6 02               201 	ldab	#2
   0A03 37                  202 	pshb		; movqi: #2 -> ,-sp
   0A04 18 DF 00            203 	sty	*ZXT
   0A07 DC 00               204 	ldd	*ZXT
   0A09 C3 00 02            205 	addd	#2
   0A0C DD 00               206 	std	*ZD7	; addhi3: y by #2 -> *ZD7
   0A0E DE 00               207 	ldx	*ZD7
   0A10 3C                  208 	pshx		; movhi: *ZD7 -> ,--sp
   0A11 D6 01               209 	ldab	*ZD6+1
   0A13 D7 00               210 	stab	*ZB1	; movqi: *ZD6 -> *ZB1
   0A15 D6 00               211 	ldab	*ZB1
   0A17 4F                  212 	clra
   0A18 DD 00               213 	std	*ZD0	; zero_extendqihi2: *ZB1 -> *ZD0
   0A1A BD 21 0E            214 	jsr	_string_IntegerToString	; CALL: (VOIDmode) _string_IntegerToString (3 bytes)
   0A1D 18 DF 00            215 	sty	*ZXT
   0A20 DC 00               216 	ldd	*ZXT
   0A22 C3 00 02            217 	addd	#2
   0A25 DD 00               218 	std	*ZD0	; addhi3: y by #2 -> *ZD0
   0A27 BD 1A FA            219 	jsr	_serial_send_string	; CALL: R:*ZD0 = _serial_send_string (0 bytes)
   0A2A CC 08 FE            220 	ldd	#LC2
   0A2D DD 00               221 	std	*ZD0	; movhi: #LC2 -> *ZD0
   0A2F BD 1A FA            222 	jsr	_serial_send_string	; CALL: R:*ZD0 = _serial_send_string (0 bytes)
   0A32 38                  223 	pulx
   0A33 38                  224 	pulx
   0A34 38                  225 	pulx		; addhi3: sp by #6 -> sp
                            226 ;;;EPILOGUE
   0A35 8F                  227 	xgdx	; saving D register
   0A36 33                  228 	pulb	; Pulling register *ZB1
   0A37 D7 00               229 	stab	*ZB1
   0A39 8F                  230 	xgdx	; restoring D register
   0A3A 38                  231 	pulx	; Pulling register *ZD7
   0A3B DF 00               232 	stx	*ZD7
   0A3D 38                  233 	pulx	; Pulling register *ZD6
   0A3E DF 00               234 	stx	*ZD6
   0A40 38                  235 	pulx	; Pulling register *ZD5
   0A41 DF 00               236 	stx	*ZD5
   0A43 18 38               237 	puly	; Restore stack frame
   0A45 38                  238 	pulx	; deallocate 2 byte auto variable
   0A46 31                  239 	ins	; deallocate 1 byte auto variable
   0A47 39                  240 	rts	; return from function
                            241 ;;;-----------------------------------------
                            242 ;;; END EPILOGUE for feedback_digital
                            243 ;;;-----------------------------------------
                            244 	; extern	_motor_get_speed
                            245 	; extern	_serial_send_string
   017C                     246 LC4:
   0A48 6D 6F 74 6F 72 20   247 	.ascii	"motor "
   0A4E 00                  248 	.byte	0x0	
   0183                     249 LC5:
   0A4F 20 73 70 65 65 64   250 	.ascii	" speed "
        20
   0A56 00                  251 	.byte	0x0	
   018B                     252 _feedback_motor:
                            253 ;;;-----------------------------------------
                            254 ;;;  PROLOGUE for feedback_motor
                            255 ;;;-----------------------------------------
   0A57 3C                  256 	pshx	; allocate 2 byte auto variable
   0A58 34                  257 	des	; allocate 1 byte auto variable
   0A59 18 3C               258 	pshy	; Save stack frame
   0A5B 18 30               259 	tsy	; Set current stack frame
   0A5D DE 00               260 	ldx	*ZD5
   0A5F 3C                  261 	pshx	; pushed register *ZD5
   0A60 DE 00               262 	ldx	*ZD6
   0A62 3C                  263 	pshx	; pushed register *ZD6
   0A63 DE 00               264 	ldx	*ZD7
   0A65 3C                  265 	pshx	; pushed register *ZD7
   0A66 8F                  266 	xgdx	; saving D register
   0A67 D6 00               267 	ldab	*ZB1
   0A69 37                  268 	pshb	; pushed register *ZB1
   0A6A 8F                  269 	xgdx	; restoring D register
                            270 ;;;END PROLOGUE
   0A6B D6 01               271 	ldab	*ZD0+1
   0A6D D7 01               272 	stab	*ZD5+1	; movqi: *ZD0 -> *ZD5
   0A6F BD 18 BF            273 	jsr	_motor_get_speed	; CALL: R:*ZD0 = _motor_get_speed (0 bytes)
   0A72 D6 01               274 	ldab	*ZD0+1
   0A74 D7 01               275 	stab	*ZD6+1	; movqi: *ZD0 -> *ZD6
   0A76 CC 0A 48            276 	ldd	#LC4
   0A79 DD 00               277 	std	*ZD0	; movhi: #LC4 -> *ZD0
   0A7B BD 1A FA            278 	jsr	_serial_send_string	; CALL: R:*ZD0 = _serial_send_string (0 bytes)
   0A7E C6 02               279 	ldab	#2
   0A80 37                  280 	pshb		; movqi: #2 -> ,-sp
   0A81 18 DF 00            281 	sty	*ZXT
   0A84 DC 00               282 	ldd	*ZXT
   0A86 C3 00 02            283 	addd	#2
   0A89 DD 00               284 	std	*ZD7	; addhi3: y by #2 -> *ZD7
   0A8B DE 00               285 	ldx	*ZD7
   0A8D 3C                  286 	pshx		; movhi: *ZD7 -> ,--sp
   0A8E D6 01               287 	ldab	*ZD5+1
   0A90 D7 00               288 	stab	*ZB1	; movqi: *ZD5 -> *ZB1
   0A92 D6 00               289 	ldab	*ZB1
   0A94 4F                  290 	clra
   0A95 DD 00               291 	std	*ZD0	; zero_extendqihi2: *ZB1 -> *ZD0
   0A97 BD 21 0E            292 	jsr	_string_IntegerToString	; CALL: (VOIDmode) _string_IntegerToString (3 bytes)
   0A9A 18 DF 00            293 	sty	*ZXT
   0A9D DC 00               294 	ldd	*ZXT
   0A9F C3 00 02            295 	addd	#2
   0AA2 DD 00               296 	std	*ZD0	; addhi3: y by #2 -> *ZD0
   0AA4 BD 1A FA            297 	jsr	_serial_send_string	; CALL: R:*ZD0 = _serial_send_string (0 bytes)
   0AA7 CC 0A 4F            298 	ldd	#LC5
   0AAA DD 00               299 	std	*ZD0	; movhi: #LC5 -> *ZD0
   0AAC BD 1A FA            300 	jsr	_serial_send_string	; CALL: R:*ZD0 = _serial_send_string (0 bytes)
   0AAF C6 02               301 	ldab	#2
   0AB1 37                  302 	pshb		; movqi: #2 -> ,-sp
   0AB2 18 DF 00            303 	sty	*ZXT
   0AB5 DC 00               304 	ldd	*ZXT
   0AB7 C3 00 02            305 	addd	#2
   0ABA DD 00               306 	std	*ZD7	; addhi3: y by #2 -> *ZD7
   0ABC DE 00               307 	ldx	*ZD7
   0ABE 3C                  308 	pshx		; movhi: *ZD7 -> ,--sp
   0ABF D6 01               309 	ldab	*ZD6+1
   0AC1 D7 00               310 	stab	*ZB1	; movqi: *ZD6 -> *ZB1
   0AC3 D6 00               311 	ldab	*ZB1
   0AC5 4F                  312 	clra
   0AC6 5D                  313 	tstb
   0AC7 2C 01               314 	bge	.+3
   0AC9 4A                  315 	deca
   0ACA DD 00               316 	std	*ZD0	; extendqihi2: *ZB1 -> *ZD0
   0ACC BD 21 0E            317 	jsr	_string_IntegerToString	; CALL: (VOIDmode) _string_IntegerToString (3 bytes)
   0ACF 18 DF 00            318 	sty	*ZXT
   0AD2 DC 00               319 	ldd	*ZXT
   0AD4 C3 00 02            320 	addd	#2
   0AD7 DD 00               321 	std	*ZD0	; addhi3: y by #2 -> *ZD0
   0AD9 BD 1A FA            322 	jsr	_serial_send_string	; CALL: R:*ZD0 = _serial_send_string (0 bytes)
   0ADC CC 08 FE            323 	ldd	#LC2
   0ADF DD 00               324 	std	*ZD0	; movhi: #LC2 -> *ZD0
   0AE1 BD 1A FA            325 	jsr	_serial_send_string	; CALL: R:*ZD0 = _serial_send_string (0 bytes)
   0AE4 38                  326 	pulx
   0AE5 38                  327 	pulx
   0AE6 38                  328 	pulx		; addhi3: sp by #6 -> sp
                            329 ;;;EPILOGUE
   0AE7 8F                  330 	xgdx	; saving D register
   0AE8 33                  331 	pulb	; Pulling register *ZB1
   0AE9 D7 00               332 	stab	*ZB1
   0AEB 8F                  333 	xgdx	; restoring D register
   0AEC 38                  334 	pulx	; Pulling register *ZD7
   0AED DF 00               335 	stx	*ZD7
   0AEF 38                  336 	pulx	; Pulling register *ZD6
   0AF0 DF 00               337 	stx	*ZD6
   0AF2 38                  338 	pulx	; Pulling register *ZD5
   0AF3 DF 00               339 	stx	*ZD5
   0AF5 18 38               340 	puly	; Restore stack frame
   0AF7 38                  341 	pulx	; deallocate 2 byte auto variable
   0AF8 31                  342 	ins	; deallocate 1 byte auto variable
   0AF9 39                  343 	rts	; return from function
                            344 ;;;-----------------------------------------
                            345 ;;; END EPILOGUE for feedback_motor
                            346 ;;;-----------------------------------------
                            347 	; extern	_string_SkipSpaces
                            348 	; extern	_string_IntegerToHex
                            349 	; extern	_serial_send_string
                            350 	; extern	_serial_send_byte
                            351 	; extern	_string_NextWord
                            352 	; extern	_string_IsDigit
                            353 	; extern	_string_StringToInteger
                            354 	; extern	_serial_send_string
   022E                     355 LC6:
   0AFA 61 20 3F 20 3D 20   356 	.ascii	"a ? = analog input help"
        61 6E 61 6C 6F 67
        20 69 6E 70 75 74
        20 68 65 6C 70
   0B11 0D                  357 	.byte	0xD	
   0B12 0A                  358 	.byte	0xA	
   0B13 00                  359 	.byte	0x0	
   0248                     360 LC7:
   0B14 61 20 31 20 3D 20   361 	.ascii	"a 1 = read analog input 1"
        72 65 61 64 20 61
        6E 61 6C 6F 67 20
        69 6E 70 75 74 20
        31
   0B2D 0D                  362 	.byte	0xD	
   0B2E 0A                  363 	.byte	0xA	
   0B2F 00                  364 	.byte	0x0	
                            365 	; extern	_serial_send_string
   0264                     366 LC8:
   0B30 62 20 3F 20 3D 20   367 	.ascii	"b ? = binary input help"
        62 69 6E 61 72 79
        20 69 6E 70 75 74
        20 68 65 6C 70
   0B47 0D                  368 	.byte	0xD	
   0B48 0A                  369 	.byte	0xA	
   0B49 00                  370 	.byte	0x0	
   027E                     371 LC9:
   0B4A 62 20 38 20 3D 20   372 	.ascii	"b 8 = read binary input 8"
        72 65 61 64 20 62
        69 6E 61 72 79 20
        69 6E 70 75 74 20
        38
   0B63 0D                  373 	.byte	0xD	
   0B64 0A                  374 	.byte	0xA	
   0B65 00                  375 	.byte	0x0	
                            376 	; extern	_motor_set_speed
                            377 	; extern	_string_IsNumber
                            378 	; extern	_serial_send_string
   029A                     379 LC10:
   0B66 6D 20 3F 20 20 20   380 	.ascii	"m ?    = motor help"
        20 3D 20 6D 6F 74
        6F 72 20 68 65 6C
        70
   0B79 0D                  381 	.byte	0xD	
   0B7A 0A                  382 	.byte	0xA	
   0B7B 00                  383 	.byte	0x0	
   02B0                     384 LC11:
   0B7C 6D 20 31 20 20 20   385 	.ascii	"m 1    = shows speed of motor 1"
        20 3D 20 73 68 6F
        77 73 20 73 70 65
        65 64 20 6F 66 20
        6D 6F 74 6F 72 20
        31
   0B9B 0D                  386 	.byte	0xD	
   0B9C 0A                  387 	.byte	0xA	
   0B9D 00                  388 	.byte	0x0	
   02D2                     389 LC12:
   0B9E 6D 20 32 20 35 20   390 	.ascii	"m 2 5  = sets motor 2 to forward speed 5"
        20 3D 20 73 65 74
        73 20 6D 6F 74 6F
        72 20 32 20 74 6F
        20 66 6F 72 77 61
        72 64 20 73 70 65
        65 64 20 35
   0BC6 0D                  391 	.byte	0xD	
   0BC7 0A                  392 	.byte	0xA	
   0BC8 00                  393 	.byte	0x0	
   02FD                     394 LC13:
   0BC9 6D 20 33 20 2D 32   395 	.ascii	"m 3 -2 = sets motor 3 to backward speed 2"
        20 3D 20 73 65 74
        73 20 6D 6F 74 6F
        72 20 33 20 74 6F
        20 62 61 63 6B 77
        61 72 64 20 73 70
        65 65 64 20 32
   0BF2 0D                  396 	.byte	0xD	
   0BF3 0A                  397 	.byte	0xA	
   0BF4 00                  398 	.byte	0x0	
   0329                     399 LC14:
   0BF5 6D 20 78 20 20 20   400 	.ascii	"m x    = stops all motors"
        20 3D 20 73 74 6F
        70 73 20 61 6C 6C
        20 6D 6F 74 6F 72
        73
   0C0E 0D                  401 	.byte	0xD	
   0C0F 0A                  402 	.byte	0xA	
   0C10 00                  403 	.byte	0x0	
                            404 	; extern	_serial_send_string
   0345                     405 LC15:
   0C11 61 20 3D 20 61 6E   406 	.ascii	"a = analog input"
        61 6C 6F 67 20 69
        6E 70 75 74
   0C21 0D                  407 	.byte	0xD	
   0C22 0A                  408 	.byte	0xA	
   0C23 00                  409 	.byte	0x0	
   0358                     410 LC16:
   0C24 64 20 3D 20 64 75   411 	.ascii	"d = dump (memory)"
        6D 70 20 28 6D 65
        6D 6F 72 79 29
   0C35 0D                  412 	.byte	0xD	
   0C36 0A                  413 	.byte	0xA	
   0C37 00                  414 	.byte	0x0	
   036C                     415 LC17:
   0C38 6D 20 3D 20 6D 6F   416 	.ascii	"m = motor"
        74 6F 72
   0C41 0D                  417 	.byte	0xD	
   0C42 0A                  418 	.byte	0xA	
   0C43 00                  419 	.byte	0x0	
   0378                     420 LC18:
   0C44 3F 20 3D 20 68 65   421 	.ascii	"? = help"
        6C 70
   0C4C 0D                  422 	.byte	0xD	
   0C4D 0A                  423 	.byte	0xA	
   0C4E 00                  424 	.byte	0x0	
                            425 	; extern	_serial_send_string
   0383                     426 LC19:
   0C4F 75 6E 6B 6E 6F 77   427 	.ascii	"unknown command, ? for help"
        6E 20 63 6F 6D 6D
        61 6E 64 2C 20 3F
        20 66 6F 72 20 68
        65 6C 70
   0C6A 0D                  428 	.byte	0xD	
   0C6B 0A                  429 	.byte	0xA	
   0C6C 00                  430 	.byte	0x0	
   03A1                     431 _interpret_command:
                            432 ;;;-----------------------------------------
                            433 ;;;  PROLOGUE for interpret_command
                            434 ;;;-----------------------------------------
   0C6D 3C                  435 	pshx	; allocate 2 byte auto variable
   0C6E 3C                  436 	pshx	; allocate 2 byte auto variable
   0C6F 18 3C               437 	pshy	; Save stack frame
   0C71 18 30               438 	tsy	; Set current stack frame
   0C73 DE 00               439 	ldx	*ZD5
   0C75 3C                  440 	pshx	; pushed register *ZD5
   0C76 DE 00               441 	ldx	*ZD6
   0C78 3C                  442 	pshx	; pushed register *ZD6
   0C79 DE 00               443 	ldx	*ZD7
   0C7B 3C                  444 	pshx	; pushed register *ZD7
   0C7C DE 00               445 	ldx	*ZD8
   0C7E 3C                  446 	pshx	; pushed register *ZD8
                            447 ;;;END PROLOGUE
   0C7F BD 24 B7            448 	jsr	_string_SkipSpaces	; CALL: R:*ZD0 = _string_SkipSpaces (0 bytes)
   0C82 DE 00               449 	ldx	*ZD0
   0C84 E6 00               450 	ldab	0,x
   0C86 4F                  451 	clra
   0C87 DD 00               452 	std	*ZD5	; zero_extendqihi2: 0,x -> *ZD5
   0C89 DC 00               453 	ldd	*ZD5
   0C8B 1A 83 00 62         454 	cpd	#98	; cmphi *ZD5 with #98
   0C8F 26 03               455 	bne	.+5
   0C91 7E 0D AA            456 	jmp	L24	; (beq) long branch
   0C94 2F 03               457 	ble	.+5
   0C96 7E 0C B9            458 	jmp	L56	; (bgt) long branch
   0C99 DC 00               459 	ldd	*ZD5
   0C9B 1A 83 00 61         460 	cpd	#97	; cmphi *ZD5 with #97
   0C9F 26 03               461 	bne	.+5
   0CA1 7E 0D 4E            462 	jmp	L15	; (beq) long branch
   0CA4 DC 00               463 	ldd	*ZD5	; tsthi: R:*ZD5
   0CA6 26 03               464 	bne	.+5
   0CA8 7E 0F 24            465 	jmp	L6	; (beq) long branch
   0CAB DC 00               466 	ldd	*ZD5
   0CAD 1A 83 00 3F         467 	cpd	#63	; cmphi *ZD5 with #63
   0CB1 26 03               468 	bne	.+5
   0CB3 7E 0E FC            469 	jmp	L52	; (beq) long branch
   0CB6 7E 0F 1C            470 	jmp	L54
   03ED                     471 L56:
   0CB9 DC 00               472 	ldd	*ZD5
   0CBB 1A 83 00 64         473 	cpd	#100	; cmphi *ZD5 with #100
   0CBF 26 03               474 	bne	.+5
   0CC1 7E 0C D2            475 	jmp	L7	; (beq) long branch
   0CC4 DC 00               476 	ldd	*ZD5
   0CC6 1A 83 00 6D         477 	cpd	#109	; cmphi *ZD5 with #109
   0CCA 26 03               478 	bne	.+5
   0CCC 7E 0E 06            479 	jmp	L33	; (beq) long branch
   0CCF 7E 0F 1C            480 	jmp	L54
   0406                     481 L7:
   0CD2 CC 00 00            482 	ldd	#0
   0CD5 DD 00               483 	std	*ZD6	; movhi: #0 -> *ZD6
   040B                     484 L11:
   0CD7 C6 02               485 	ldab	#2
   0CD9 37                  486 	pshb		; movqi: #2 -> ,-sp
   0CDA 18 DF 00            487 	sty	*ZXT
   0CDD DC 00               488 	ldd	*ZXT
   0CDF C3 00 02            489 	addd	#2
   0CE2 DD 00               490 	std	*ZD8	; addhi3: y by #2 -> *ZD8
   0CE4 DE 00               491 	ldx	*ZD8
   0CE6 3C                  492 	pshx		; movhi: *ZD8 -> ,--sp
   0CE7 DC 00               493 	ldd	*ZD6
   0CE9 DD 00               494 	std	*ZD0	; movhi: *ZD6 -> *ZD0
   0CEB BD 22 35            495 	jsr	_string_IntegerToHex	; CALL: (VOIDmode) _string_IntegerToHex (3 bytes)
   0CEE 18 DF 00            496 	sty	*ZXT
   0CF1 DC 00               497 	ldd	*ZXT
   0CF3 C3 00 02            498 	addd	#2
   0CF6 DD 00               499 	std	*ZD0	; addhi3: y by #2 -> *ZD0
   0CF8 BD 1A FA            500 	jsr	_serial_send_string	; CALL: R:*ZD0 = _serial_send_string (0 bytes)
   0CFB CC 00 20            501 	ldd	#32
   0CFE DD 00               502 	std	*ZD0	; movhi: #32 -> *ZD0
   0D00 BD 1A C1            503 	jsr	_serial_send_byte	; CALL: R:*ZD0 = _serial_send_byte (0 bytes)
   0D03 DC 00               504 	ldd	*ZD6
   0D05 DD 00               505 	std	*ZD5	; movhi: *ZD6 -> *ZD5
   0D07 2D 03               506 	blt	.+5
   0D09 7E 0D 13            507 	jmp	L13	; (bge) long branch
   0D0C DC 00               508 	ldd	*ZD6
   0D0E C3 00 0F            509 	addd	#15
   0D11 DD 00               510 	std	*ZD5	; addhi3: *ZD6 by #15 -> *ZD5
   0447                     511 L13:
   0D13 CC FF F0            512 	ldd	#-16
   0D16 DD 00               513 	std	*ZXT
   0D18 DC 00               514 	ldd	*ZD5
   0D1A 94 00               515 	anda	*ZXT
   0D1C D4 01               516 	andb	*ZXT+1
   0D1E DD 00               517 	std	*ZD5	; andhi3: *ZD5 by #-16 -> *ZD5
   0D20 DC 00               518 	ldd	*ZD6
   0D22 93 00               519 	subd	*ZD5
   0D24 DD 00               520 	std	*ZD5	; subhi3: *ZD6 by *ZD5 -> *ZD5
   0D26 38                  521 	pulx
   0D27 33                  522 	pulb		; addhi3: sp by #3 -> sp
   0D28 DC 00               523 	ldd	*ZD5
   0D2A 1A 83 00 0F         524 	cpd	#15	; cmphi *ZD5 with #15
   0D2E 27 03               525 	beq	.+5
   0D30 7E 0D 3B            526 	jmp	L10	; (bne) long branch
   0D33 CC 08 FE            527 	ldd	#LC2
   0D36 DD 00               528 	std	*ZD0	; movhi: #LC2 -> *ZD0
   0D38 BD 1A FA            529 	jsr	_serial_send_string	; CALL: R:*ZD0 = _serial_send_string (0 bytes)
   046F                     530 L10:
   0D3B DE 00               531 	ldx	*ZD6
   0D3D 08                  532 	inx
   0D3E DF 00               533 	stx	*ZD6	; addhi3: *ZD6 by #1 -> *ZD6
   0D40 DC 00               534 	ldd	*ZD6
   0D42 1A 83 00 7F         535 	cpd	#127	; cmphi *ZD6 with #127
   0D46 2E 03               536 	bgt	.+5
   0D48 7E 0C D7            537 	jmp	L11	; (ble) long branch
   0D4B 7E 0F 24            538 	jmp	L6
   0482                     539 L15:
   0D4E CC 00 00            540 	ldd	#0
   0D51 DD 00               541 	std	*ZD7	; movhi: #0 -> *ZD7
   0D53 BD 24 40            542 	jsr	_string_NextWord	; CALL: R:*ZD0 = _string_NextWord (0 bytes)
   0D56 DC 00               543 	ldd	*ZD0
   0D58 DD 00               544 	std	*ZD6	; movhi: *ZD0 -> *ZD6
   0D5A DE 00               545 	ldx	*ZD0
   0D5C E6 00               546 	ldab	0,x
   0D5E 4F                  547 	clra
   0D5F DD 00               548 	std	*ZD5	; zero_extendqihi2: 0,x -> *ZD5
   0D61 26 03               549 	bne	.+5
   0D63 7E 0D 9A            550 	jmp	L57	; (beq) long branch
   0D66 DC 00               551 	ldd	*ZD5
   0D68 1A 83 00 3F         552 	cpd	#63	; cmphi *ZD5 with #63
   0D6C 26 03               553 	bne	.+5
   0D6E 7E 0D 9A            554 	jmp	L57	; (beq) long branch
   0D71 DE 00               555 	ldx	*ZD0
   0D73 E6 00               556 	ldab	0,x
   0D75 D7 01               557 	stab	*ZD0+1	; movqi: 0,x -> *ZD0
   0D77 BD 1F 70            558 	jsr	_string_IsDigit	; CALL: R:*ZD0 = _string_IsDigit (0 bytes)
   0D7A DC 00               559 	ldd	*ZD0	; tsthi: R:*ZD0
   0D7C 26 03               560 	bne	.+5
   0D7E 7E 0D 8E            561 	jmp	L20	; (beq) long branch
   0D81 DC 00               562 	ldd	*ZD6
   0D83 DD 00               563 	std	*ZD0	; movhi: *ZD6 -> *ZD0
   0D85 BD 20 16            564 	jsr	_string_StringToInteger	; CALL: R:*ZD0 = _string_StringToInteger (0 bytes)
   0D88 BD 09 01            565 	jsr	_feedback_analog	; CALL: (VOIDmode) _feedback_analog (0 bytes)
   0D8B 7E 0D 93            566 	jmp	L16
   04C2                     567 L20:
   0D8E CC 00 01            568 	ldd	#1
   0D91 DD 00               569 	std	*ZD7	; movhi: #1 -> *ZD7
   04C7                     570 L16:
   0D93 DC 00               571 	ldd	*ZD7	; tsthi: R:*ZD7
   0D95 26 03               572 	bne	.+5
   0D97 7E 0F 24            573 	jmp	L6	; (beq) long branch
   04CE                     574 L57:
   0D9A CC 0A FA            575 	ldd	#LC6
   0D9D DD 00               576 	std	*ZD0	; movhi: #LC6 -> *ZD0
   0D9F BD 1A FA            577 	jsr	_serial_send_string	; CALL: R:*ZD0 = _serial_send_string (0 bytes)
   0DA2 CC 0B 14            578 	ldd	#LC7
   0DA5 DD 00               579 	std	*ZD0	; movhi: #LC7 -> *ZD0
   0DA7 7E 0F 21            580 	jmp	L60
   04DE                     581 L24:
   0DAA CC 00 00            582 	ldd	#0
   0DAD DD 00               583 	std	*ZD7	; movhi: #0 -> *ZD7
   0DAF BD 24 40            584 	jsr	_string_NextWord	; CALL: R:*ZD0 = _string_NextWord (0 bytes)
   0DB2 DC 00               585 	ldd	*ZD0
   0DB4 DD 00               586 	std	*ZD6	; movhi: *ZD0 -> *ZD6
   0DB6 DE 00               587 	ldx	*ZD0
   0DB8 E6 00               588 	ldab	0,x
   0DBA 4F                  589 	clra
   0DBB DD 00               590 	std	*ZD5	; zero_extendqihi2: 0,x -> *ZD5
   0DBD 26 03               591 	bne	.+5
   0DBF 7E 0D F6            592 	jmp	L58	; (beq) long branch
   0DC2 DC 00               593 	ldd	*ZD5
   0DC4 1A 83 00 3F         594 	cpd	#63	; cmphi *ZD5 with #63
   0DC8 26 03               595 	bne	.+5
   0DCA 7E 0D F6            596 	jmp	L58	; (beq) long branch
   0DCD DE 00               597 	ldx	*ZD0
   0DCF E6 00               598 	ldab	0,x
   0DD1 D7 01               599 	stab	*ZD0+1	; movqi: 0,x -> *ZD0
   0DD3 BD 1F 70            600 	jsr	_string_IsDigit	; CALL: R:*ZD0 = _string_IsDigit (0 bytes)
   0DD6 DC 00               601 	ldd	*ZD0	; tsthi: R:*ZD0
   0DD8 26 03               602 	bne	.+5
   0DDA 7E 0D EA            603 	jmp	L29	; (beq) long branch
   0DDD DC 00               604 	ldd	*ZD6
   0DDF DD 00               605 	std	*ZD0	; movhi: *ZD6 -> *ZD0
   0DE1 BD 20 16            606 	jsr	_string_StringToInteger	; CALL: R:*ZD0 = _string_StringToInteger (0 bytes)
   0DE4 BD 09 A9            607 	jsr	_feedback_digital	; CALL: (VOIDmode) _feedback_digital (0 bytes)
   0DE7 7E 0D EF            608 	jmp	L25
   051E                     609 L29:
   0DEA CC 00 01            610 	ldd	#1
   0DED DD 00               611 	std	*ZD7	; movhi: #1 -> *ZD7
   0523                     612 L25:
   0DEF DC 00               613 	ldd	*ZD7	; tsthi: R:*ZD7
   0DF1 26 03               614 	bne	.+5
   0DF3 7E 0F 24            615 	jmp	L6	; (beq) long branch
   052A                     616 L58:
   0DF6 CC 0B 30            617 	ldd	#LC8
   0DF9 DD 00               618 	std	*ZD0	; movhi: #LC8 -> *ZD0
   0DFB BD 1A FA            619 	jsr	_serial_send_string	; CALL: R:*ZD0 = _serial_send_string (0 bytes)
   0DFE CC 0B 4A            620 	ldd	#LC9
   0E01 DD 00               621 	std	*ZD0	; movhi: #LC9 -> *ZD0
   0E03 7E 0F 21            622 	jmp	L60
   053A                     623 L33:
   0E06 CC 00 00            624 	ldd	#0
   0E09 DD 00               625 	std	*ZD7	; movhi: #0 -> *ZD7
   0E0B BD 24 40            626 	jsr	_string_NextWord	; CALL: R:*ZD0 = _string_NextWord (0 bytes)
   0E0E DC 00               627 	ldd	*ZD0
   0E10 DD 00               628 	std	*ZD6	; movhi: *ZD0 -> *ZD6
   0E12 DE 00               629 	ldx	*ZD6
   0E14 E6 00               630 	ldab	0,x
   0E16 4F                  631 	clra
   0E17 DD 00               632 	std	*ZD5	; zero_extendqihi2: 0,x -> *ZD5
   0E19 DC 00               633 	ldd	*ZD5
   0E1B 1A 83 00 78         634 	cpd	#120	; cmphi *ZD5 with #120
   0E1F 26 03               635 	bne	.+5
   0E21 7E 0E 3E            636 	jmp	L35	; (beq) long branch
   0E24 2F 03               637 	ble	.+5
   0E26 7E 0E 63            638 	jmp	L43	; (bgt) long branch
   0E29 DC 00               639 	ldd	*ZD5	; tsthi: R:*ZD5
   0E2B 26 03               640 	bne	.+5
   0E2D 7E 0E D4            641 	jmp	L59	; (beq) long branch
   0E30 DC 00               642 	ldd	*ZD5
   0E32 1A 83 00 3F         643 	cpd	#63	; cmphi *ZD5 with #63
   0E36 26 03               644 	bne	.+5
   0E38 7E 0E D4            645 	jmp	L59	; (beq) long branch
   0E3B 7E 0E 63            646 	jmp	L43
   0572                     647 L35:
   0E3E C6 00               648 	ldab	#0
   0E40 D7 01               649 	stab	*ZD5+1	; movqi: #0 -> *ZD5
   0576                     650 L39:
   0E42 C6 00               651 	ldab	#0
   0E44 37                  652 	pshb		; movqi: #0 -> ,-sp
   0E45 D6 01               653 	ldab	*ZD5+1
   0E47 D7 01               654 	stab	*ZD0+1	; movqi: *ZD5 -> *ZD0
   0E49 BD 19 23            655 	jsr	_motor_set_speed	; CALL: (VOIDmode) _motor_set_speed (1 bytes)
   0E4C D6 01               656 	ldab	*ZD5+1
   0E4E D7 01               657 	stab	*ZD0+1	; movqi: *ZD5 -> *ZD0
   0E50 BD 0A 57            658 	jsr	_feedback_motor	; CALL: (VOIDmode) _feedback_motor (0 bytes)
   0E53 33                  659 	pulb		; addhi3: sp by #1 -> sp
   0E54 7C 00 01            660 	inc	*ZD5+1	; addqi3: *ZD5 by #1 -> *ZD5
   0E57 D6 01               661 	ldab	*ZD5+1
   0E59 C1 03               662 	cmpb	#3	; cmpqi *ZD5 with #3
   0E5B 22 03               663 	bhi	.+5
   0E5D 7E 0E 42            664 	jmp	L39	; (bleu) long branch
   0E60 7E 0E CD            665 	jmp	L34
   0597                     666 L43:
   0E63 DE 00               667 	ldx	*ZD6
   0E65 E6 00               668 	ldab	0,x
   0E67 D7 01               669 	stab	*ZD0+1	; movqi: 0,x -> *ZD0
   0E69 BD 1F 70            670 	jsr	_string_IsDigit	; CALL: R:*ZD0 = _string_IsDigit (0 bytes)
   0E6C DC 00               671 	ldd	*ZD0	; tsthi: R:*ZD0
   0E6E 26 03               672 	bne	.+5
   0E70 7E 0E C8            673 	jmp	L44	; (beq) long branch
   0E73 DC 00               674 	ldd	*ZD6
   0E75 DD 00               675 	std	*ZD0	; movhi: *ZD6 -> *ZD0
   0E77 BD 20 16            676 	jsr	_string_StringToInteger	; CALL: R:*ZD0 = _string_StringToInteger (0 bytes)
   0E7A D6 01               677 	ldab	*ZD0+1
   0E7C D7 01               678 	stab	*ZD5+1	; movqi: *ZD0 -> *ZD5
   0E7E DC 00               679 	ldd	*ZD6
   0E80 DD 00               680 	std	*ZD0	; movhi: *ZD6 -> *ZD0
   0E82 BD 24 40            681 	jsr	_string_NextWord	; CALL: R:*ZD0 = _string_NextWord (0 bytes)
   0E85 DC 00               682 	ldd	*ZD0
   0E87 DD 00               683 	std	*ZD6	; movhi: *ZD0 -> *ZD6
   0E89 DE 00               684 	ldx	*ZD0
   0E8B E6 00               685 	ldab	0,x
   0E8D D7 01               686 	stab	*ZD0+1	; movqi: 0,x -> *ZD0
   0E8F BD 1F 35            687 	jsr	_string_IsNumber	; CALL: R:*ZD0 = _string_IsNumber (0 bytes)
   0E92 DC 00               688 	ldd	*ZD0	; tsthi: R:*ZD0
   0E94 26 03               689 	bne	.+5
   0E96 7E 0E B5            690 	jmp	L45	; (beq) long branch
   0E99 DC 00               691 	ldd	*ZD6
   0E9B DD 00               692 	std	*ZD0	; movhi: *ZD6 -> *ZD0
   0E9D BD 20 16            693 	jsr	_string_StringToInteger	; CALL: R:*ZD0 = _string_StringToInteger (0 bytes)
   0EA0 D6 01               694 	ldab	*ZD0+1
   0EA2 37                  695 	pshb		; movqi: *ZD0 -> ,-sp
   0EA3 D6 01               696 	ldab	*ZD5+1
   0EA5 D7 01               697 	stab	*ZD0+1	; movqi: *ZD5 -> *ZD0
   0EA7 BD 19 23            698 	jsr	_motor_set_speed	; CALL: (VOIDmode) _motor_set_speed (1 bytes)
   0EAA D6 01               699 	ldab	*ZD5+1
   0EAC D7 01               700 	stab	*ZD0+1	; movqi: *ZD5 -> *ZD0
   0EAE BD 0A 57            701 	jsr	_feedback_motor	; CALL: (VOIDmode) _feedback_motor (0 bytes)
   0EB1 33                  702 	pulb		; addhi3: sp by #1 -> sp
   0EB2 7E 0E CD            703 	jmp	L34
   05E9                     704 L45:
   0EB5 DE 00               705 	ldx	*ZD6
   0EB7 6D 00               706 	tst	0,x	; tstqi: MEM:0,x
   0EB9 27 03               707 	beq	.+5
   0EBB 7E 0E D4            708 	jmp	L59	; (bne) long branch
   0EBE D6 01               709 	ldab	*ZD5+1
   0EC0 D7 01               710 	stab	*ZD0+1	; movqi: *ZD5 -> *ZD0
   0EC2 BD 0A 57            711 	jsr	_feedback_motor	; CALL: (VOIDmode) _feedback_motor (0 bytes)
   0EC5 7E 0E CD            712 	jmp	L34
   05FC                     713 L44:
   0EC8 CC 00 01            714 	ldd	#1
   0ECB DD 00               715 	std	*ZD7	; movhi: #1 -> *ZD7
   0601                     716 L34:
   0ECD DC 00               717 	ldd	*ZD7	; tsthi: R:*ZD7
   0ECF 26 03               718 	bne	.+5
   0ED1 7E 0F 24            719 	jmp	L6	; (beq) long branch
   0608                     720 L59:
   0ED4 CC 0B 66            721 	ldd	#LC10
   0ED7 DD 00               722 	std	*ZD0	; movhi: #LC10 -> *ZD0
   0ED9 BD 1A FA            723 	jsr	_serial_send_string	; CALL: R:*ZD0 = _serial_send_string (0 bytes)
   0EDC CC 0B 7C            724 	ldd	#LC11
   0EDF DD 00               725 	std	*ZD0	; movhi: #LC11 -> *ZD0
   0EE1 BD 1A FA            726 	jsr	_serial_send_string	; CALL: R:*ZD0 = _serial_send_string (0 bytes)
   0EE4 CC 0B 9E            727 	ldd	#LC12
   0EE7 DD 00               728 	std	*ZD0	; movhi: #LC12 -> *ZD0
   0EE9 BD 1A FA            729 	jsr	_serial_send_string	; CALL: R:*ZD0 = _serial_send_string (0 bytes)
   0EEC CC 0B C9            730 	ldd	#LC13
   0EEF DD 00               731 	std	*ZD0	; movhi: #LC13 -> *ZD0
   0EF1 BD 1A FA            732 	jsr	_serial_send_string	; CALL: R:*ZD0 = _serial_send_string (0 bytes)
   0EF4 CC 0B F5            733 	ldd	#LC14
   0EF7 DD 00               734 	std	*ZD0	; movhi: #LC14 -> *ZD0
   0EF9 7E 0F 21            735 	jmp	L60
   0630                     736 L52:
   0EFC CC 0C 11            737 	ldd	#LC15
   0EFF DD 00               738 	std	*ZD0	; movhi: #LC15 -> *ZD0
   0F01 BD 1A FA            739 	jsr	_serial_send_string	; CALL: R:*ZD0 = _serial_send_string (0 bytes)
   0F04 CC 0C 24            740 	ldd	#LC16
   0F07 DD 00               741 	std	*ZD0	; movhi: #LC16 -> *ZD0
   0F09 BD 1A FA            742 	jsr	_serial_send_string	; CALL: R:*ZD0 = _serial_send_string (0 bytes)
   0F0C CC 0C 38            743 	ldd	#LC17
   0F0F DD 00               744 	std	*ZD0	; movhi: #LC17 -> *ZD0
   0F11 BD 1A FA            745 	jsr	_serial_send_string	; CALL: R:*ZD0 = _serial_send_string (0 bytes)
   0F14 CC 0C 44            746 	ldd	#LC18
   0F17 DD 00               747 	std	*ZD0	; movhi: #LC18 -> *ZD0
   0F19 7E 0F 21            748 	jmp	L60
   0650                     749 L54:
   0F1C CC 0C 4F            750 	ldd	#LC19
   0F1F DD 00               751 	std	*ZD0	; movhi: #LC19 -> *ZD0
   0655                     752 L60:
   0F21 BD 1A FA            753 	jsr	_serial_send_string	; CALL: R:*ZD0 = _serial_send_string (0 bytes)
   0658                     754 L6:
                            755 ;;;EPILOGUE
   0F24 38                  756 	pulx	; Pulling register *ZD8
   0F25 DF 00               757 	stx	*ZD8
   0F27 38                  758 	pulx	; Pulling register *ZD7
   0F28 DF 00               759 	stx	*ZD7
   0F2A 38                  760 	pulx	; Pulling register *ZD6
   0F2B DF 00               761 	stx	*ZD6
   0F2D 38                  762 	pulx	; Pulling register *ZD5
   0F2E DF 00               763 	stx	*ZD5
   0F30 18 38               764 	puly	; Restore stack frame
   0F32 38                  765 	pulx	; deallocate 2 byte auto variable
   0F33 38                  766 	pulx	; deallocate 2 byte auto variable
   0F34 39                  767 	rts	; return from function
                            768 ;;;-----------------------------------------
                            769 ;;; END EPILOGUE for interpret_command
                            770 ;;;-----------------------------------------
                            771 	; extern	_serial_send_string
   0669                     772 LC20:
   0F35 3E 20               773 	.ascii	"> "
   0F37 00                  774 	.byte	0x0	
   066C                     775 _prompt:
                            776 ;;;-----------------------------------------
                            777 ;;;  PROLOGUE for prompt
                            778 ;;;-----------------------------------------
   0F38 18 3C               779 	pshy	; Save stack frame
   0F3A 18 30               780 	tsy	; Set current stack frame
                            781 ;;;END PROLOGUE
   0F3C CC 0F 35            782 	ldd	#LC20
   0F3F DD 00               783 	std	*ZD0	; movhi: #LC20 -> *ZD0
   0F41 BD 1A FA            784 	jsr	_serial_send_string	; CALL: R:*ZD0 = _serial_send_string (0 bytes)
                            785 ;;;EPILOGUE
   0F44 18 38               786 	puly	; Restore stack frame
   0F46 39                  787 	rts	; return from function
                            788 ;;;-----------------------------------------
                            789 ;;; END EPILOGUE for prompt
                            790 ;;;-----------------------------------------
                            791 	.area	_BSS
   0000                     792 _cmdline.14:	.blkb	41
                            793 	; extern	_input_stop_button
                            794 	; extern	_input_start_button
                            795 	; extern	_presto_timer
                            796 	; extern	_presto_sleep
                            797 	; extern	_presto_get_message
                            798 	; extern	_serial_recv
                            799 	; extern	_serial_send_string
                            800 	; extern	_serial_send_byte
                            801 	; extern	_serial_send_byte
                            802 	; extern	_serial_send_byte
                            803 	.area	_CODE
   067B                     804 _debugger:
                            805 ;;;-----------------------------------------
                            806 ;;;  PROLOGUE for debugger
                            807 ;;;-----------------------------------------
   0F47 3C                  808 	pshx	; allocate 2 byte auto variable
   0F48 3C                  809 	pshx	; allocate 2 byte auto variable
   0F49 34                  810 	des	; allocate 1 byte auto variable
   0F4A 18 3C               811 	pshy	; Save stack frame
   0F4C 18 30               812 	tsy	; Set current stack frame
   0F4E DE 00               813 	ldx	*ZD5
   0F50 3C                  814 	pshx	; pushed register *ZD5
   0F51 DE 00               815 	ldx	*ZD6
   0F53 3C                  816 	pshx	; pushed register *ZD6
   0F54 8F                  817 	xgdx	; saving D register
   0F55 D6 00               818 	ldab	*ZB1
   0F57 37                  819 	pshb	; pushed register *ZB1
   0F58 8F                  820 	xgdx	; restoring D register
                            821 ;;;END PROLOGUE
   0F59 C6 00               822 	ldab	#0
   0F5B D7 00               823 	stab	*ZB1	; movqi: #0 -> *ZB1
   0F5D BD 0F 38            824 	jsr	_prompt	; CALL: (VOIDmode) _prompt (0 bytes)
   0694                     825 L65:
   0F60 BD 17 DE            826 	jsr	_input_stop_button	; CALL: R:*ZD0 = _input_stop_button (0 bytes)
   0F63 DC 00               827 	ldd	*ZD0	; tsthi: R:*ZD0
   0F65 26 03               828 	bne	.+5
   0F67 7E 0F 75            829 	jmp	L66	; (beq) long branch
   0F6A C6 FA               830 	ldab	#-6
   0F6C 37                  831 	pshb		; movqi: #-6 -> ,-sp
   0F6D C6 00               832 	ldab	#0
   0F6F D7 01               833 	stab	*ZD0+1	; movqi: #0 -> *ZD0
   0F71 BD 19 23            834 	jsr	_motor_set_speed	; CALL: (VOIDmode) _motor_set_speed (1 bytes)
   0F74 33                  835 	pulb		; addhi3: sp by #1 -> sp
   06A9                     836 L66:
   0F75 BD 18 06            837 	jsr	_input_start_button	; CALL: R:*ZD0 = _input_start_button (0 bytes)
   0F78 DC 00               838 	ldd	*ZD0	; tsthi: R:*ZD0
   0F7A 26 03               839 	bne	.+5
   0F7C 7E 0F 8A            840 	jmp	L67	; (beq) long branch
   0F7F C6 06               841 	ldab	#6
   0F81 37                  842 	pshb		; movqi: #6 -> ,-sp
   0F82 C6 00               843 	ldab	#0
   0F84 D7 01               844 	stab	*ZD0+1	; movqi: #0 -> *ZD0
   0F86 BD 19 23            845 	jsr	_motor_set_speed	; CALL: (VOIDmode) _motor_set_speed (1 bytes)
   0F89 33                  846 	pulb		; addhi3: sp by #1 -> sp
   06BE                     847 L67:
   0F8A CD EE 04            848 	ldx	4,y
   0F8D 3C                  849 	pshx		; movhi: 4,y -> ,--sp
   0F8E CD EE 02            850 	ldx	2,y
   0F91 3C                  851 	pshx		; movhi: 2,y -> ,--sp
   0F92 CE 00 64            852 	ldx	#100
   0F95 3C                  853 	pshx		; movhi: #100 -> ,--sp
   0F96 F6 A0 53            854 	ldab	_debugger_tid
   0F99 D7 01               855 	stab	*ZD0+1	; movqi: _debugger_tid -> *ZD0
   0F9B BD 03 F3            856 	jsr	_presto_timer	; CALL: R:*ZD0 = _presto_timer (6 bytes)
   0F9E BD 02 83            857 	jsr	_presto_sleep	; CALL: (VOIDmode) _presto_sleep (0 bytes)
   0FA1 18 DF 00            858 	sty	*ZXT
   0FA4 DC 00               859 	ldd	*ZXT
   0FA6 C3 00 02            860 	addd	#2
   0FA9 DD 00               861 	std	*ZD0	; addhi3: y by #2 -> *ZD0
   0FAB BD 06 07            862 	jsr	_presto_get_message	; CALL: R:*ZD0 = _presto_get_message (0 bytes)
   0FAE 38                  863 	pulx
   0FAF 38                  864 	pulx
   0FB0 38                  865 	pulx		; addhi3: sp by #6 -> sp
   06E5                     866 L68:
   0FB1 18 DF 00            867 	sty	*ZXT
   0FB4 DC 00               868 	ldd	*ZXT
   0FB6 C3 00 06            869 	addd	#6
   0FB9 DD 00               870 	std	*ZD0	; addhi3: y by #6 -> *ZD0
   0FBB BD 1B 56            871 	jsr	_serial_recv	; CALL: R:*ZD0 = _serial_recv (0 bytes)
   0FBE DC 00               872 	ldd	*ZD0	; tsthi: R:*ZD0
   0FC0 26 03               873 	bne	.+5
   0FC2 7E 0F 60            874 	jmp	L65	; (beq) long branch
   0FC5 18 E6 06            875 	ldab	6,y
   0FC8 D7 01               876 	stab	*ZD6+1	; movqi: 6,y -> *ZD6
   0FCA D6 01               877 	ldab	*ZD6+1
   0FCC C1 0D               878 	cmpb	#13	; cmpqi *ZD6 with #13
   0FCE 26 03               879 	bne	.+5
   0FD0 7E 0F DC            880 	jmp	L72	; (beq) long branch
   0FD3 D6 01               881 	ldab	*ZD6+1
   0FD5 C1 0A               882 	cmpb	#10	; cmpqi *ZD6 with #10
   0FD7 27 03               883 	beq	.+5
   0FD9 7E 0F FF            884 	jmp	L71	; (bne) long branch
   0710                     885 L72:
   0FDC D6 00               886 	ldab	*ZB1
   0FDE 4F                  887 	clra
   0FDF DD 00               888 	std	*ZD5	; zero_extendqihi2: *ZB1 -> *ZD5
   0FE1 C6 00               889 	ldab	#0
   0FE3 E7 00               890 	stab	0,x	; movqi: #0 -> 0,x
   0FE5 C6 00               891 	ldab	#0
   0FE7 D7 00               892 	stab	*ZB1	; movqi: #0 -> *ZB1
   0FE9 CC 08 FE            893 	ldd	#LC2
   0FEC DD 00               894 	std	*ZD0	; movhi: #LC2 -> *ZD0
   0FEE BD 1A FA            895 	jsr	_serial_send_string	; CALL: R:*ZD0 = _serial_send_string (0 bytes)
   0FF1 CC 9F 2A            896 	ldd	#_cmdline.14
   0FF4 DD 00               897 	std	*ZD0	; movhi: #_cmdline.14 -> *ZD0
   0FF6 BD 0C 6D            898 	jsr	_interpret_command	; CALL: (VOIDmode) _interpret_command (0 bytes)
   0FF9 BD 0F 38            899 	jsr	_prompt	; CALL: (VOIDmode) _prompt (0 bytes)
   0FFC 7E 0F B1            900 	jmp	L68
   0733                     901 L71:
   0FFF D6 01               902 	ldab	*ZD6+1
   1001 C1 08               903 	cmpb	#8	; cmpqi *ZD6 with #8
   1003 27 03               904 	beq	.+5
   1005 7E 10 30            905 	jmp	L74	; (bne) long branch
   1008 D6 00               906 	ldab	*ZB1	; tstqi: R:*ZB1
   100A 26 03               907 	bne	.+5
   100C 7E 0F B1            908 	jmp	L68	; (beq) long branch
   100F D6 00               909 	ldab	*ZB1
   1011 CB FF               910 	addb	#-1
   1013 D7 00               911 	stab	*ZB1	; addqi3: *ZB1 by #-1 -> *ZB1
   1015 CC 00 08            912 	ldd	#8
   1018 DD 00               913 	std	*ZD0	; movhi: #8 -> *ZD0
   101A BD 1A C1            914 	jsr	_serial_send_byte	; CALL: R:*ZD0 = _serial_send_byte (0 bytes)
   101D CC 00 20            915 	ldd	#32
   1020 DD 00               916 	std	*ZD0	; movhi: #32 -> *ZD0
   1022 BD 1A C1            917 	jsr	_serial_send_byte	; CALL: R:*ZD0 = _serial_send_byte (0 bytes)
   1025 CC 00 08            918 	ldd	#8
   1028 DD 00               919 	std	*ZD0	; movhi: #8 -> *ZD0
   102A BD 1A C1            920 	jsr	_serial_send_byte	; CALL: R:*ZD0 = _serial_send_byte (0 bytes)
   102D 7E 0F B1            921 	jmp	L68
   0764                     922 L74:
   1030 D6 01               923 	ldab	*ZD6+1
   1032 CB E0               924 	addb	#-32
   1034 D7 01               925 	stab	*ZD5+1	; addqi3: *ZD6 by #-32 -> *ZD5
   1036 D6 01               926 	ldab	*ZD5+1
   1038 C1 5F               927 	cmpb	#95	; cmpqi *ZD5 with #95
   103A 23 03               928 	bls	.+5
   103C 7E 0F B1            929 	jmp	L68	; (bgtu) long branch
   103F D6 00               930 	ldab	*ZB1
   1041 C1 27               931 	cmpb	#39	; cmpqi *ZB1 with #39
   1043 23 03               932 	bls	.+5
   1045 7E 10 60            933 	jmp	L78	; (bgtu) long branch
   1048 D6 00               934 	ldab	*ZB1
   104A 4F                  935 	clra
   104B DD 00               936 	std	*ZD5	; zero_extendqihi2: *ZB1 -> *ZD5
   104D D6 01               937 	ldab	*ZD6+1
   104F E7 00               938 	stab	0,x	; movqi: *ZD6 -> 0,x
   1051 7C 00 00            939 	inc	*ZB1	; addqi3: *ZB1 by #1 -> *ZB1
   1054 18 E6 06            940 	ldab	6,y
   1057 4F                  941 	clra
   1058 DD 00               942 	std	*ZD0	; zero_extendqihi2: 6,y -> *ZD0
   105A BD 1A C1            943 	jsr	_serial_send_byte	; CALL: R:*ZD0 = _serial_send_byte (0 bytes)
   105D 7E 0F B1            944 	jmp	L68
   0794                     945 L78:
   1060 CC 00 21            946 	ldd	#33
   1063 DD 00               947 	std	*ZD0	; movhi: #33 -> *ZD0
   1065 BD 1A C1            948 	jsr	_serial_send_byte	; CALL: R:*ZD0 = _serial_send_byte (0 bytes)
   1068 7E 0F B1            949 	jmp	L68
                            950 ;;;EPILOGUE
   106B 8F                  951 	xgdx	; saving D register
   106C 33                  952 	pulb	; Pulling register *ZB1
   106D D7 00               953 	stab	*ZB1
   106F 8F                  954 	xgdx	; restoring D register
   1070 38                  955 	pulx	; Pulling register *ZD6
   1071 DF 00               956 	stx	*ZD6
   1073 38                  957 	pulx	; Pulling register *ZD5
   1074 DF 00               958 	stx	*ZD5
   1076 18 38               959 	puly	; Restore stack frame
   1078 38                  960 	pulx	; deallocate 2 byte auto variable
   1079 38                  961 	pulx	; deallocate 2 byte auto variable
   107A 31                  962 	ins	; deallocate 1 byte auto variable
   107B 39                  963 	rts	; return from function
                            964 ;;;-----------------------------------------
                            965 ;;; END EPILOGUE for debugger
                            966 ;;;-----------------------------------------
                            967 	.area	_BSS
   0029                     968 _debugger_stack:	.blkb	256
   0129                     969 _debugger_tid:	.blkb	1
                            970 	; END
