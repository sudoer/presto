
\programs\embedded\avr\gnu\bin\uisp -dserial=com1 -dprog=stk500 -dpart=ATmega169 -v=3 --wr_osc=1240000
pause
\programs\embedded\avr\gnu\bin\uisp -dserial=com1 -dprog=stk500 -dpart=ATmega169 -v=3 --wr_fuse_e=0xFF
pause
\programs\embedded\avr\gnu\bin\uisp -dserial=com1 -dprog=stk500 -dpart=ATmega169 -v=3 --wr_fuse_h=0x99
pause
\programs\embedded\avr\gnu\bin\uisp -dserial=com1 -dprog=stk500 -dpart=ATmega169 -v=3 --wr_fuse_l=0x62
pause
\programs\embedded\avr\gnu\bin\uisp -dserial=com1 -dprog=stk500 -dpart=ATmega169 -v=3 --rd_fuses

