                              1 ;;;-----------------------------------------
                              2 ;;; Start MC6811 gcc assembly output
                              3 ;;; gcc compiler compiled on TBD
                              4 ;;; OPTIONS:	-mlong_branch optimize  
                              5 ;;; OPTIONS:	 peephole !omit_frame_pointer !signed-char
                              6 ;;; Source:		services\inputs.c
                              7 ;;; Destination:	inputs.s
                              8 ;;; Compiled:	Tue Oct 24 21:55:20 2000
                              9 ;;; (META)compiled by GNU C version 2.8.1.
                             10 ;;;-----------------------------------------
                             11 	.module inputs.c
                             12 	.area	_CODE
                             13 	.globl	_input_sample_analog
   0000                      14 _input_sample_analog:
                             15 ;;;-----------------------------------------
                             16 ;;;  PROLOGUE for input_sample_analog
                             17 ;;;-----------------------------------------
   1611 18 3C                18 	pshy	; Save stack frame
   1613 18 30                19 	tsy	; Set current stack frame
   1615 DE 00                20 	ldx	*ZD5
   1617 3C                   21 	pshx	; pushed register *ZD5
   1618 8F                   22 	xgdx	; saving D register
   1619 D6 00                23 	ldab	*ZB1
   161B 37                   24 	pshb	; pushed register *ZB1
   161C 8F                   25 	xgdx	; restoring D register
                             26 ;;;END PROLOGUE
                             27 ; Begin inline assembler code
   161D 0F                   28 		sei
                             29 ; End of inline assembler code
   161E D6 01                30 	ldab	*ZD0+1
   1620 C4 07                31 	andb	#7
   1622 D7 01                32 	stab	*ZD5+1	; andqi3: *ZD0 by #7 -> *ZD5
   1624 D6 01                33 	ldab	*ZD5+1
   1626 F7 10 30             34 	stab	4144	; movqi: *ZD5 -> 4144
   0018                      35 L2:
   1629 F6 10 30             36 	ldab	4144
   162C D7 01                37 	stab	*ZD5+1	; movqi: 4144 -> *ZD5
   162E 2D 03                38 	blt	.+5
   1630 7E 16 29             39 	jmp	L2	; (bge) long branch
                             40 ; Begin inline assembler code
   1633 0E                   41 		cli
                             42 ; End of inline assembler code
   1634 F6 10 31             43 	ldab	4145
   1637 D7 00                44 	stab	*ZB1	; movqi: 4145 -> *ZB1
   1639 D6 00                45 	ldab	*ZB1
   163B 4F                   46 	clra
   163C DD 00                47 	std	*ZD0	; zero_extendqihi2: *ZB1 -> *ZD0
                             48 ;;;EPILOGUE
   163E 8F                   49 	xgdx	; saving D register
   163F 33                   50 	pulb	; Pulling register *ZB1
   1640 D7 00                51 	stab	*ZB1
   1642 8F                   52 	xgdx	; restoring D register
   1643 38                   53 	pulx	; Pulling register *ZD5
   1644 DF 00                54 	stx	*ZD5
   1646 18 38                55 	puly	; Restore stack frame
   1648 39                   56 	rts	; return from function
                             57 ;;;-----------------------------------------
                             58 ;;; END EPILOGUE for input_sample_analog
                             59 ;;;-----------------------------------------
                             60 	; extern	_serial_send_string
   0038                      61 LC0:
   1649 5B                   62 	.ascii	"["
   164A 00                   63 	.byte	0x0	
                             64 	; extern	_string_IntegerToString
   003A                      65 LC1:
   164B 2C                   66 	.ascii	","
   164C 00                   67 	.byte	0x0	
   003C                      68 LC2:
   164D 41                   69 	.ascii	"A"
   164E 00                   70 	.byte	0x0	
   003E                      71 LC3:
   164F 42                   72 	.ascii	"B"
   1650 00                   73 	.byte	0x0	
   0040                      74 LC4:
   1651 43                   75 	.ascii	"C"
   1652 00                   76 	.byte	0x0	
   0042                      77 LC5:
   1653 44                   78 	.ascii	"D"
   1654 00                   79 	.byte	0x0	
   0044                      80 LC6:
   1655 5D                   81 	.ascii	"]"
   1656 0D                   82 	.byte	0xD	
   1657 0A                   83 	.byte	0xA	
   1658 00                   84 	.byte	0x0	
                             85 	.globl	_input_sample_digital
   0048                      86 _input_sample_digital:
                             87 ;;;-----------------------------------------
                             88 ;;;  PROLOGUE for input_sample_digital
                             89 ;;;-----------------------------------------
   1659 3C                   90 	pshx	; allocate 2 byte auto variable
   165A 34                   91 	des	; allocate 1 byte auto variable
   165B 18 3C                92 	pshy	; Save stack frame
   165D 18 30                93 	tsy	; Set current stack frame
   165F DE 00                94 	ldx	*ZD5
   1661 3C                   95 	pshx	; pushed register *ZD5
   1662 DE 00                96 	ldx	*ZD6
   1664 3C                   97 	pshx	; pushed register *ZD6
   1665 DE 00                98 	ldx	*ZD7
   1667 3C                   99 	pshx	; pushed register *ZD7
   1668 8F                  100 	xgdx	; saving D register
   1669 D6 00               101 	ldab	*ZB1
   166B 37                  102 	pshb	; pushed register *ZB1
   166C D6 00               103 	ldab	*ZB2
   166E 37                  104 	pshb	; pushed register *ZB2
   166F 8F                  105 	xgdx	; restoring D register
                            106 ;;;END PROLOGUE
   1670 D6 01               107 	ldab	*ZD0+1
   1672 D7 01               108 	stab	*ZD6+1	; movqi: *ZD0 -> *ZD6
   1674 CC 16 49            109 	ldd	#LC0
   1677 DD 00               110 	std	*ZD0	; movhi: #LC0 -> *ZD0
   1679 BD 1A FA            111 	jsr	_serial_send_string	; CALL: (VOIDmode) _serial_send_string (0 bytes)
   167C D6 01               112 	ldab	*ZD6+1
   167E D7 00               113 	stab	*ZB2	; movqi: *ZD6 -> *ZB2
   1680 D6 00               114 	ldab	*ZB2
   1682 4F                  115 	clra
   1683 DD 00               116 	std	*ZD5	; zero_extendqihi2: *ZB2 -> *ZD5
   1685 CE 00 02            117 	ldx	#2
   1688 3C                  118 	pshx		; movhi: #2 -> ,--sp
   1689 18 DF 00            119 	sty	*ZXT
   168C DC 00               120 	ldd	*ZXT
   168E C3 00 02            121 	addd	#2
   1691 DD 00               122 	std	*ZD7	; addhi3: y by #2 -> *ZD7
   1693 DE 00               123 	ldx	*ZD7
   1695 3C                  124 	pshx		; movhi: *ZD7 -> ,--sp
   1696 DC 00               125 	ldd	*ZD5
   1698 DD 00               126 	std	*ZD0	; movhi: *ZD5 -> *ZD0
   169A BD 21 0E            127 	jsr	_string_IntegerToString	; CALL: R:*ZD0 = _string_IntegerToString (4 bytes)
   169D 18 DF 00            128 	sty	*ZXT
   16A0 DC 00               129 	ldd	*ZXT
   16A2 C3 00 02            130 	addd	#2
   16A5 DD 00               131 	std	*ZD0	; addhi3: y by #2 -> *ZD0
   16A7 BD 1A FA            132 	jsr	_serial_send_string	; CALL: (VOIDmode) _serial_send_string (0 bytes)
   16AA CC 16 4B            133 	ldd	#LC1
   16AD DD 00               134 	std	*ZD0	; movhi: #LC1 -> *ZD0
   16AF BD 1A FA            135 	jsr	_serial_send_string	; CALL: (VOIDmode) _serial_send_string (0 bytes)
   16B2 38                  136 	pulx
   16B3 38                  137 	pulx		; addhi3: sp by #4 -> sp
   16B4 D6 01               138 	ldab	*ZD6+1
   16B6 C1 09               139 	cmpb	#9	; cmpqi *ZD6 with #9
   16B8 22 03               140 	bhi	.+5
   16BA 7E 16 C5            141 	jmp	L7	; (bleu) long branch
   16BD CC 00 00            142 	ldd	#0
   16C0 DD 00               143 	std	*ZD0	; movhi: #0 -> *ZD0
   16C2 7E 17 C8            144 	jmp	L6
   00B4                     145 L7:
   16C5 DC 00               146 	ldd	*ZD5
   16C7 1A 83 00 08         147 	cpd	#8	; cmphi *ZD5 with #8
   16CB 26 03               148 	bne	.+5
   16CD 7E 17 0E            149 	jmp	L10	; (beq) long branch
   16D0 2F 03               150 	ble	.+5
   16D2 7E 16 E3            151 	jmp	L14	; (bgt) long branch
   16D5 DC 00               152 	ldd	*ZD5
   16D7 1A 83 00 07         153 	cpd	#7	; cmphi *ZD5 with #7
   16DB 26 03               154 	bne	.+5
   16DD 7E 16 F1            155 	jmp	L9	; (beq) long branch
   16E0 7E 17 5E            156 	jmp	L12
   00D2                     157 L14:
   16E3 DC 00               158 	ldd	*ZD5
   16E5 1A 83 00 09         159 	cpd	#9	; cmphi *ZD5 with #9
   16E9 26 03               160 	bne	.+5
   16EB 7E 17 30            161 	jmp	L11	; (beq) long branch
   16EE 7E 17 5E            162 	jmp	L12
   00E0                     163 L9:
   16F1 F6 10 00            164 	ldab	4096
   16F4 D7 01               165 	stab	*ZD5+1	; movqi: 4096 -> *ZD5
   16F6 DC 00               166 	ldd	*ZD5
   16F8 43                  167 	coma
   16F9 53                  168 	comb
   16FA C3 00 00            169 	addd	#0
   16FD DD 00               170 	std	*ZD5	; one_cmplhi2: *ZD5 -> *ZD5
   16FF DC 00               171 	ldd	*ZD5
   1701 4F                  172 	clra
   1702 C4 01               173 	andb	#1
   1704 DD 00               174 	std	*ZD5	; andhi3: *ZD5 by #1 -> *ZD5
   1706 CC 16 4D            175 	ldd	#LC2
   1709 DD 00               176 	std	*ZD0	; movhi: #LC2 -> *ZD0
   170B 7E 17 8A            177 	jmp	L16
   00FD                     178 L10:
   170E F6 10 00            179 	ldab	4096
   1711 D7 01               180 	stab	*ZD5+1	; movqi: 4096 -> *ZD5
   1713 D6 01               181 	ldab	*ZD5+1
   1715 54                  182 	lsrb
   1716 D7 01               183 	stab	*ZD5+1	; lshrqi3: *ZD5 by #1 -> *ZD5
   1718 DC 00               184 	ldd	*ZD5
   171A 43                  185 	coma
   171B 53                  186 	comb
   171C C3 00 00            187 	addd	#0
   171F DD 00               188 	std	*ZD5	; one_cmplhi2: *ZD5 -> *ZD5
   1721 DC 00               189 	ldd	*ZD5
   1723 4F                  190 	clra
   1724 C4 01               191 	andb	#1
   1726 DD 00               192 	std	*ZD5	; andhi3: *ZD5 by #1 -> *ZD5
   1728 CC 16 4F            193 	ldd	#LC3
   172B DD 00               194 	std	*ZD0	; movhi: #LC3 -> *ZD0
   172D 7E 17 8A            195 	jmp	L16
   011F                     196 L11:
   1730 F6 10 00            197 	ldab	4096
   1733 D7 01               198 	stab	*ZD5+1	; movqi: 4096 -> *ZD5
   1735 D6 01               199 	ldab	*ZD5+1
   1737 D7 00               200 	stab	*ZB2	; movqi: *ZD5 -> *ZB2
   1739 D6 00               201 	ldab	*ZB2
   173B 54                  202 	lsrb
   173C 54                  203 	lsrb
   173D 54                  204 	lsrb
   173E 54                  205 	lsrb
   173F 54                  206 	lsrb
   1740 54                  207 	lsrb
   1741 54                  208 	lsrb
   1742 D7 00               209 	stab	*ZB1	; lshrqi3: *ZB2 by #7 -> *ZB1
   1744 D6 00               210 	ldab	*ZB1
   1746 4F                  211 	clra
   1747 DD 00               212 	std	*ZD5	; zero_extendqihi2: *ZB1 -> *ZD5
   1749 CC 00 01            213 	ldd	#1
   174C DD 00               214 	std	*ZXT
   174E DC 00               215 	ldd	*ZD5
   1750 98 00               216 	eora	*ZXT
   1752 D8 01               217 	eorb	*ZXT+1
   1754 DD 00               218 	std	*ZD5	; eorhi3: *ZD5 by #1 -> *ZD5
   1756 CC 16 51            219 	ldd	#LC4
   1759 DD 00               220 	std	*ZD0	; movhi: #LC4 -> *ZD0
   175B 7E 17 8A            221 	jmp	L16
   014D                     222 L12:
   175E F6 7F FF            223 	ldab	32767
   1761 4F                  224 	clra
   1762 DD 00               225 	std	*ZD5	; zero_extendqihi2: 32767 -> *ZD5
   1764 DC 00               226 	ldd	*ZD5
   1766 DE 00               227 	ldx	*ZD6
   1768 2F 05               228 	ble	1$
   176A 47                  229 	asra
   176B 56                  230 	rorb
   176C 09                  231 	dex
   176D 26 FB               232 	bne	.-3
                            233 1$:
   176F DD 00               234 	std	*ZD5	; ashrhi3: *ZD5 by *ZD6 -> *ZD5
   1771 CC 00 01            235 	ldd	#1
   1774 DD 00               236 	std	*ZXT
   1776 DC 00               237 	ldd	*ZD5
   1778 98 00               238 	eora	*ZXT
   177A D8 01               239 	eorb	*ZXT+1
   177C DD 00               240 	std	*ZD5	; eorhi3: *ZD5 by #1 -> *ZD5
   177E DC 00               241 	ldd	*ZD5
   1780 4F                  242 	clra
   1781 C4 01               243 	andb	#1
   1783 DD 00               244 	std	*ZD5	; andhi3: *ZD5 by #1 -> *ZD5
   1785 CC 16 53            245 	ldd	#LC5
   1788 DD 00               246 	std	*ZD0	; movhi: #LC5 -> *ZD0
   0179                     247 L16:
   178A BD 1A FA            248 	jsr	_serial_send_string	; CALL: (VOIDmode) _serial_send_string (0 bytes)
   178D CC 16 4B            249 	ldd	#LC1
   1790 DD 00               250 	std	*ZD0	; movhi: #LC1 -> *ZD0
   1792 BD 1A FA            251 	jsr	_serial_send_string	; CALL: (VOIDmode) _serial_send_string (0 bytes)
   1795 CE 00 01            252 	ldx	#1
   1798 3C                  253 	pshx		; movhi: #1 -> ,--sp
   1799 18 DF 00            254 	sty	*ZXT
   179C DC 00               255 	ldd	*ZXT
   179E C3 00 02            256 	addd	#2
   17A1 DD 00               257 	std	*ZD7	; addhi3: y by #2 -> *ZD7
   17A3 DE 00               258 	ldx	*ZD7
   17A5 3C                  259 	pshx		; movhi: *ZD7 -> ,--sp
   17A6 DC 00               260 	ldd	*ZD5
   17A8 DD 00               261 	std	*ZD0	; movhi: *ZD5 -> *ZD0
   17AA BD 21 0E            262 	jsr	_string_IntegerToString	; CALL: R:*ZD0 = _string_IntegerToString (4 bytes)
   17AD 18 DF 00            263 	sty	*ZXT
   17B0 DC 00               264 	ldd	*ZXT
   17B2 C3 00 02            265 	addd	#2
   17B5 DD 00               266 	std	*ZD0	; addhi3: y by #2 -> *ZD0
   17B7 BD 1A FA            267 	jsr	_serial_send_string	; CALL: (VOIDmode) _serial_send_string (0 bytes)
   17BA CC 16 55            268 	ldd	#LC6
   17BD DD 00               269 	std	*ZD0	; movhi: #LC6 -> *ZD0
   17BF BD 1A FA            270 	jsr	_serial_send_string	; CALL: (VOIDmode) _serial_send_string (0 bytes)
   17C2 DC 00               271 	ldd	*ZD5
   17C4 DD 00               272 	std	*ZD0	; movhi: *ZD5 -> *ZD0
   17C6 38                  273 	pulx
   17C7 38                  274 	pulx		; addhi3: sp by #4 -> sp
   01B7                     275 L6:
                            276 ;;;EPILOGUE
   17C8 8F                  277 	xgdx	; saving D register
   17C9 33                  278 	pulb	; Pulling register *ZB2
   17CA D7 00               279 	stab	*ZB2
   17CC 33                  280 	pulb	; Pulling register *ZB1
   17CD D7 00               281 	stab	*ZB1
   17CF 8F                  282 	xgdx	; restoring D register
   17D0 38                  283 	pulx	; Pulling register *ZD7
   17D1 DF 00               284 	stx	*ZD7
   17D3 38                  285 	pulx	; Pulling register *ZD6
   17D4 DF 00               286 	stx	*ZD6
   17D6 38                  287 	pulx	; Pulling register *ZD5
   17D7 DF 00               288 	stx	*ZD5
   17D9 18 38               289 	puly	; Restore stack frame
   17DB 38                  290 	pulx	; deallocate 2 byte auto variable
   17DC 31                  291 	ins	; deallocate 1 byte auto variable
   17DD 39                  292 	rts	; return from function
                            293 ;;;-----------------------------------------
                            294 ;;; END EPILOGUE for input_sample_digital
                            295 ;;;-----------------------------------------
                            296 	.globl	_input_stop_button
   01CD                     297 _input_stop_button:
                            298 ;;;-----------------------------------------
                            299 ;;;  PROLOGUE for input_stop_button
                            300 ;;;-----------------------------------------
   17DE 18 3C               301 	pshy	; Save stack frame
   17E0 18 30               302 	tsy	; Set current stack frame
   17E2 DE 00               303 	ldx	*ZD5
   17E4 3C                  304 	pshx	; pushed register *ZD5
                            305 ;;;END PROLOGUE
   17E5 F6 7F FF            306 	ldab	32767
   17E8 54                  307 	lsrb
   17E9 54                  308 	lsrb
   17EA 54                  309 	lsrb
   17EB 54                  310 	lsrb
   17EC 54                  311 	lsrb
   17ED 54                  312 	lsrb
   17EE D7 01               313 	stab	*ZD5+1	; lshrqi3: 32767 by #6 -> *ZD5
   17F0 DC 00               314 	ldd	*ZD5
   17F2 43                  315 	coma
   17F3 53                  316 	comb
   17F4 C3 00 00            317 	addd	#0
   17F7 DD 00               318 	std	*ZD5	; one_cmplhi2: *ZD5 -> *ZD5
   17F9 DC 00               319 	ldd	*ZD5
   17FB 4F                  320 	clra
   17FC C4 01               321 	andb	#1
   17FE DD 00               322 	std	*ZD0	; andhi3: *ZD5 by #1 -> *ZD0
                            323 ;;;EPILOGUE
   1800 38                  324 	pulx	; Pulling register *ZD5
   1801 DF 00               325 	stx	*ZD5
   1803 18 38               326 	puly	; Restore stack frame
   1805 39                  327 	rts	; return from function
                            328 ;;;-----------------------------------------
                            329 ;;; END EPILOGUE for input_stop_button
                            330 ;;;-----------------------------------------
                            331 	.globl	_input_start_button
   01F5                     332 _input_start_button:
                            333 ;;;-----------------------------------------
                            334 ;;;  PROLOGUE for input_start_button
                            335 ;;;-----------------------------------------
   1806 18 3C               336 	pshy	; Save stack frame
   1808 18 30               337 	tsy	; Set current stack frame
   180A DE 00               338 	ldx	*ZD5
   180C 3C                  339 	pshx	; pushed register *ZD5
   180D 8F                  340 	xgdx	; saving D register
   180E D6 00               341 	ldab	*ZB1
   1810 37                  342 	pshb	; pushed register *ZB1
   1811 8F                  343 	xgdx	; restoring D register
                            344 ;;;END PROLOGUE
   1812 F6 7F FF            345 	ldab	32767
   1815 54                  346 	lsrb
   1816 54                  347 	lsrb
   1817 54                  348 	lsrb
   1818 54                  349 	lsrb
   1819 54                  350 	lsrb
   181A 54                  351 	lsrb
   181B 54                  352 	lsrb
   181C D7 00               353 	stab	*ZB1	; lshrqi3: 32767 by #7 -> *ZB1
   181E D6 00               354 	ldab	*ZB1
   1820 4F                  355 	clra
   1821 DD 00               356 	std	*ZD5	; zero_extendqihi2: *ZB1 -> *ZD5
   1823 CC 00 01            357 	ldd	#1
   1826 DD 00               358 	std	*ZXT
   1828 DC 00               359 	ldd	*ZD5
   182A 98 00               360 	eora	*ZXT
   182C D8 01               361 	eorb	*ZXT+1
   182E DD 00               362 	std	*ZD0	; eorhi3: *ZD5 by #1 -> *ZD0
                            363 ;;;EPILOGUE
   1830 8F                  364 	xgdx	; saving D register
   1831 33                  365 	pulb	; Pulling register *ZB1
   1832 D7 00               366 	stab	*ZB1
   1834 8F                  367 	xgdx	; restoring D register
   1835 38                  368 	pulx	; Pulling register *ZD5
   1836 DF 00               369 	stx	*ZD5
   1838 18 38               370 	puly	; Restore stack frame
   183A 39                  371 	rts	; return from function
                            372 ;;;-----------------------------------------
                            373 ;;; END EPILOGUE for input_start_button
                            374 ;;;-----------------------------------------
                            375 	; END
