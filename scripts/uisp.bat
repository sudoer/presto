
rem was --wr_osc=3686400
\programs\embedded\avr\gnu\bin\uisp -dserial=com1 -dprog=stk500 -dpart=ATmega169 -v=3 --wr_osc=1228800 --erase --upload if=obj\butterfly\presto.hex --verify

